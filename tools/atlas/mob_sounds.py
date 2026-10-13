"""Decode the legacy CharSounds rows used by character and mob properties.

This is deliberately separate from the generated 638-row SoundAutoGen table:
CharSounds stores four vectors of SoundAutoGen ordinals plus two material flags.
"""
from __future__ import annotations

import struct
from pathlib import Path

from common import read_json, write_json, stable_id


class _Reader:
    def __init__(self, path: Path):
        self.path = path
        self.data = path.read_bytes()
        self.at = 0

    def take(self, size: int) -> bytes:
        if size < 0 or self.at + size > len(self.data):
            raise ValueError(f"truncated {self.path.name} at {self.at} (need {size})")
        value = self.data[self.at:self.at + size]
        self.at += size
        return value

    def u32(self) -> int:
        return struct.unpack("<I", self.take(4))[0]

    def i32(self) -> int:
        return struct.unpack("<i", self.take(4))[0]

    def count(self, limit: int = 65536) -> int:
        value = self.u32()
        if value > limit:
            raise ValueError(f"count {value} exceeds {limit} in {self.path.name}")
        return value

    def names(self) -> list[str]:
        count = self.count()
        result = []
        for _ in range(count):
            size = self.count(1_000_000)
            result.append(self.take(size).decode("utf-8", "replace"))
        return result

    def end(self) -> None:
        if self.at != len(self.data):
            raise ValueError(f"unconsumed {len(self.data) - self.at} bytes in {self.path.name}")


_FIELDS = ("Die", "Hurt", "ImpactVsFlesh", "ImpactVsMetal")
_SOURCE = "Structs::CharSounds::read 0x4eade4; Arrays::CharSoundsTable::read 0x4b96bc"


def _read_name_groups(path: Path) -> list[list[str]]:
    reader = _Reader(path)
    groups = []
    while reader.at < len(reader.data):
        groups.append(reader.names())
    reader.end()
    return groups


def build(ctx, tables, references):
    """Return decoded CharSounds rows and sound-ordinal reference edges.

    The JSON-friendly result has ``rows`` and ``references`` arrays. Each row
    records fields exactly as serialized; each nonnegative list item points to
    the same ordinal in ``sdd_dungeon_hunter_2_iphone`` when in range. Invalid
    ordinals remain UNRESOLVED with an explicit reason and are never clamped.
    ``tables`` and ``references`` are accepted for a uniform atlas builder API;
    source row names and the target namespace are validated from original data.
    """
    del references  # Edges are produced from the original four CharSounds vectors.
    py = ctx.inputs / "assets-extra/android/data/pydata"
    data_path = py / "sounds_pyarray.bin"
    names_path = py / "sounds_pyarraynames.bin"
    name_groups = _read_name_groups(names_path)
    if len(name_groups) < 2:
        raise ValueError("sounds_pyarraynames.bin lacks the next registered table names")
    names = name_groups[0]
    schemas = _read_name_groups(py / "sounds_pystructnames.bin")
    if len(schemas) <= 5 or schemas[5] != [*_FIELDS, "IsFlesh", "IsMetallic"]:
        raise ValueError("CharSounds schema names differ from the recovered six-field layout")

    known_rows = {(r.get("table"), r.get("index")): r for r in tables
                  if isinstance(r, dict) and r.get("index") is not None}
    if len(names) != len(set(names)):
        raise ValueError("duplicate names in sounds#group0")
    for index, name in enumerate(names):
        record = known_rows.get(("sounds#group0", index))
        if record is not None and record.get("name") != name:
            raise ValueError(f"atlas row mismatch at sounds#group0[{index}]")

    sound_rows = [r for r in tables if isinstance(r, dict)
                  and r.get("table") == "sdd_dungeon_hunter_2_iphone"
                  and isinstance(r.get("index"), int)]
    target_names = {r["index"]: r.get("name") for r in sound_rows}
    if sound_rows and sorted(target_names) != list(range(len(target_names))):
        raise ValueError("sdd_dungeon_hunter_2_iphone rows are not a contiguous ordinal table")

    reader = _Reader(data_path)
    row_count = reader.count()
    if row_count != len(names):
        raise ValueError(f"CharSounds row count {row_count} != names count {len(names)}")

    rows = []
    edges = []
    for row_index, name in enumerate(names):
        fields = {}
        for field in _FIELDS:
            count = reader.count()
            if count > (len(reader.data) - reader.at) // 4:
                raise ValueError(f"{name}.{field} vector exceeds remaining source bytes")
            fields[field] = [reader.i32() for _ in range(count)]
        flags = reader.take(2)
        fields["IsFlesh"] = bool(flags[0])
        fields["IsMetallic"] = bool(flags[1])
        rows.append({"table": "sounds#group0", "index": row_index, "name": name,
                     "fields": fields, "status": "RESOLVED", "confidence": "PROVEN",
                     "schema_evidence": _SOURCE})
        for field in _FIELDS:
            for member_index, ordinal in enumerate(fields[field]):
                edge = {"site": f"sounds#group0[{row_index}].{field}[{member_index}]",
                        "source_table": "sounds#group0", "source_index": row_index,
                        "source_name": name, "field": field, "member_index": member_index,
                        "sound_id": ordinal, "kind": "sound",
                        "rule": "CharSounds vector member is a SoundAutoGen ordinal; resolve by the same index in the registered 638-row Sounds table.",
                        "schema_evidence": _SOURCE}
                if ordinal < 0:
                    edge.update(status="UNRESOLVED", reason="NEGATIVE_SOUND_ORDINAL",
                                candidates=[], resolves_to=[])
                elif target_names and ordinal in target_names:
                    edge.update(status="RESOLVED", target_table="sdd_dungeon_hunter_2_iphone",
                                target_index=ordinal, target_name=target_names[ordinal],
                                resolves_to=[{"type": "table-row", "table": "sdd_dungeon_hunter_2_iphone",
                                              "index": ordinal}])
                elif target_names:
                    edge.update(status="UNRESOLVED", reason="ID_OUT_OF_RANGE", candidates=[], resolves_to=[])
                else:
                    edge.update(status="UNRESOLVED", reason="TARGET_TABLE_NOT_SUPPLIED", candidates=[], resolves_to=[])
                edges.append(edge)
    section_end = reader.at
    # The next registration in this multiplexed legacy stream is Listeners;
    # its count is the first word after CharSounds and must match names group 1.
    if len(reader.data) - section_end < 4:
        raise ValueError("legacy sounds stream ends before the following Listeners section")
    next_count = struct.unpack_from("<I", reader.data, section_end)[0]
    if next_count != len(name_groups[1]):
        raise ValueError("CharSounds boundary does not lead to the registered Listeners section")

    # Source-grounded known answers: the named humanoid row demonstrates a
    # non-default configuration; Default is the native fallback index.
    by_name = {r["name"]: r for r in rows}
    if "Default" not in by_name or "ArmoredHuman" not in by_name:
        raise ValueError("expected Default and ArmoredHuman in sounds#group0 names")
    if by_name["Default"]["index"] != 2:
        raise ValueError("CharSounds Default source fallback moved from ordinal 2")
    return {"rows": rows, "references": edges,
            "consumption": {"source": ".local-inputs/assets-extra/android/data/pydata/sounds_pyarray.bin",
                            "bytes_consumed": section_end, "bytes_total": len(reader.data),
                            "following_registered_tables_bytes": len(reader.data) - section_end,
                            "next_registered_table": "Listeners",
                            "next_registered_table_rows": next_count,
                            "rows": row_count, "field_schema": "[i][i][i][i]bb"}}


def integrate(ctx):
    """Merge proven sound payloads into the common ledger before view generation."""
    tables = read_json(ctx.out / 'data/tables.json', [])
    refs = read_json(ctx.out / 'data/references.json', [])
    decoded = build(ctx, tables, refs)
    updates = {(r['table'], r['index']): r for r in decoded['rows']}
    for filename in ('tables.json', 'tables-extra.json'):
        rows = tables if filename == 'tables.json' else read_json(ctx.out / 'data' / filename, [])
        for row in rows:
            update = updates.get((row['table'], row['index']))
            if update:
                row.update(update)
                row.update(work_status='DONE', reason=None, candidate_note=None, candidates=[])
        write_json(ctx.out / 'data' / filename, rows)
    table_index = {(r['table'], r['index']): r for r in tables}
    edges = []
    for edge in decoded['references']:
        field = f"{edge['field']}[{edge['member_index']}]"
        site = decoded['consumption']['source'] + f"[section=0,row={edge['source_index']}].{field}"
        row = dict(edge)
        row.update(id=stable_id('charsounds-ref', site), site=site,
                   name=str(edge['sound_id']), context={'file': decoded['consumption']['source'],
                   'table': edge['source_table'], 'index': edge['source_index'], 'field': field},
                   work_status='DONE', confidence='PROVEN', port_status='PARTIAL',
                   implementation='port/level-world/character_combat_sound_v1.cpp', candidates=[])
        for target in row.get('resolves_to') or []:
            source = table_index.get((target['table'], target['index']))
            if source:
                target['source_row'] = {k: source[k] for k in ('table', 'index', 'name', 'fields')}
        if row['status'] == 'UNRESOLVED':
            row['candidate_note'] = row.get('reason')
            row['reason'] = 'ID_OUT_OF_RANGE' if row.get('reason') == 'ID_OUT_OF_RANGE' else 'OTHER'
        edges.append(row)
    by_id = {r['id']: r for r in refs}
    by_id.update({r['id']: r for r in edges})
    refs = list(by_id.values())
    for ref in refs:
        for target in ref.get('resolves_to') or []:
            key = (target.get('table'), target.get('index'))
            if key in updates:
                target['source_row'] = {k: table_index[key][k] for k in ('table', 'index', 'name', 'fields')}
    write_json(ctx.out / 'data/references.json', refs)
    write_json(ctx.out / 'data/character-sound-references.json', edges)
    write_json(ctx.out / 'counts/character-sounds.json', decoded['consumption'])
    counts = read_json(ctx.out / 'counts/references.json', {})
    counts.update(references=len(refs), table_rows_unresolved=sum(r['status'] == 'UNRESOLVED' for r in tables),
                  references_by_kind={k: sum(r.get('kind') == k for r in refs)
                    for k in sorted({r.get('kind', '') for r in refs})})
    write_json(ctx.out / 'counts/references.json', counts)
    import references as resolver
    resolver._write_report(ctx, counts, refs, tables)


def coverage_check(ctx):
    tables = read_json(ctx.out / 'data/tables.json', [])
    refs = read_json(ctx.out / 'data/references.json', [])
    decoded = build(ctx, tables, refs)
    expected = {stable_id('charsounds-ref', decoded['consumption']['source'] +
                f"[section=0,row={e['source_index']}].{e['field']}[{e['member_index']}]") for e in decoded['references']}
    present = {r['id'] for r in refs}
    errors = ['CharSounds source reference sites missing from ledger'] if expected - present else []
    saved = {(r['table'], r['index']): r for r in tables}
    for row in decoded['rows']:
        if saved[row['table'], row['index']]['fields'] != row['fields']:
            errors.append('CharSounds decoded field drift: ' + row['name'])
    return errors
