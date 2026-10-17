"""Stage the source-selected DefaultFairy model and animation asset closure.

This bundles content only. It does not create a Crypt Character or DACT actor.
The supplied cache ZIP is read-only and must match the pinned archive digest.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import struct
import zipfile

from inventory import EXPECTED
from inspect_animation_data import parse as parse_animation_data
from inspect_animation_tables import sections as animation_sections
from prepare_actors import images


MODEL_PINS = {
    "faeries_00_hotty.bdae": (57856, "d06162de1931f805559c49629e3556b5a83080773312b5a94c5f8dcd4d0047c7"),
    "faeries_01_wetty.bdae": (59152, "55d1229457653e1d0017d38ac8e23b55a81d129323cbff41f066a58fee20d433"),
    "faeries_02_celeste.bdae": (59684, "841531e0cba192d00898309e7dc53b0b56445c9e72e71d22163178ae3f6ebe22"),
    "faeries_03_rocky.bdae": (63000, "686891182eb2161d1c0282d189f13ea243b9734254488de052fc9a8df3d7e308"),
    "faeries_04_windy.bdae": (63492, "4e905bcbf61f6cd2aab558cbb1c35456ca513d4a716e180c64c44a69b2a8cd74"),
}
EXPECTED_DEFAULT = (
    ("Fake_Celest", "Celest", 0, 34, "faeries_02_celeste.bdae"),
    ("Fake_Rocky", "Rocky", 1, 35, "faeries_03_rocky.bdae"),
    ("Fake_Wetty", "Wetty", 2, 33, "faeries_01_wetty.bdae"),
    ("Fake_Windy", "Windy", 3, 36, "faeries_04_windy.bdae"),
    ("Fake_Hotty", "Hotty", 4, 32, "faeries_00_hotty.bdae"),
)
EXPECTED_DEFAULT_ROWS = (2, 4, 5, 6, 3)
CHARACTER_INPUTS = (
    "character_properties_pyarray.bin",
    "character_properties_pyarraynames.bin",
    "character_properties_pystructnames.bin",
    "character_models_dictionary_pyarray.bin",
)
FAERY_INPUTS = (
    "faeries_pyarray.bin", "faeries_pyarraynames.bin", "faeries_pystructnames.bin",
)
ANIMATION_INPUTS = (
    "animations_pyarray.bin", "animations_pyarraynames.bin",
    "animations_pystructnames.bin", "animations_dictionary_pyarray.bin",
)


class Reader:
    def __init__(self, raw: bytes):
        self.raw, self.offset = raw, 0

    def word(self) -> int:
        if self.offset + 4 > len(self.raw):
            raise ValueError("truncated source table")
        value, = struct.unpack_from("<I", self.raw, self.offset)
        self.offset += 4
        return value

    def integer(self) -> int:
        if self.offset + 4 > len(self.raw):
            raise ValueError("truncated source integer")
        value, = struct.unpack_from("<i", self.raw, self.offset)
        self.offset += 4
        return value

    def string(self) -> str:
        size = self.word()
        if not size or size > 1_000_000 or self.offset + size > len(self.raw):
            raise ValueError("invalid source string size")
        value = self.raw[self.offset:self.offset + size].decode("ascii")
        self.offset += size
        return value

    def names(self) -> list[str]:
        count = self.word()
        if not count or count > 10_000:
            raise ValueError("source name count rejected")
        return [self.string() for _ in range(count)]

    def strings(self) -> list[str]:
        return self.names()


def name_sections(raw: bytes) -> list[list[str]]:
    reader = Reader(raw)
    sections = []
    while reader.offset < len(raw):
        sections.append(reader.names())
    return sections


def dictionary_strings(raw: bytes) -> list[str]:
    reader = Reader(raw)
    result = reader.strings()
    if reader.offset != len(raw):
        raise ValueError("model/animation dictionary has trailing bytes")
    return result


def source_path(value: str) -> str:
    path = value.replace("\\", "/")
    pure = PurePosixPath(path)
    if (not path.casefold().startswith("data/3d/") or pure.is_absolute()
            or any(part in ("", ".", "..") for part in path.split("/"))):
        raise ValueError(f"unsafe source BDAE path: {value!r}")
    return path.lower()


def character_table(raw: bytes, names_raw: bytes, fields_raw: bytes) -> tuple[list[str], list[str], list[list[int]]]:
    names = Reader(names_raw).strings()
    fields = Reader(fields_raw).strings()
    reader = Reader(raw)
    count = reader.word()
    if count != len(names) or len(fields) != 224:
        raise ValueError("CharacterTable names/count/schema mismatch")
    rows = [[reader.integer() for _ in fields] for _ in range(count)]
    if reader.offset > len(raw):
        raise ValueError("CharacterTable is truncated")
    return names, fields, rows


def faery_tables(records: bytes, names_raw: bytes, schema_raw: bytes):
    names = name_sections(names_raw)
    schema = name_sections(schema_raw)
    expected_schema = [
        ["Description", "Elemental", "ModelFile", "Name", "SpellScript", "SpellType", "Type"],
        ["List"], ["List"],
    ]
    if len(names) != 2 or schema != expected_schema:
        raise ValueError("Faery table name/schema sections changed")
    reader = Reader(records)
    list_count = reader.word()
    if list_count != len(names[0]):
        raise ValueError("FaeryList count differs from names")
    lists = []
    for _ in range(list_count):
        count = reader.word()
        if count > 4096:
            raise ValueError("FaeryList member count rejected")
        lists.append([reader.integer() for _ in range(count)])
    row_count = reader.word()
    if row_count != len(names[1]):
        raise ValueError("Faery count differs from names")
    rows = []
    for _ in range(row_count):
        description, elemental, model, name_id = (reader.integer() for _ in range(4))
        script_size = reader.integer()
        if script_size < 0 or script_size > 1_000_000 or reader.offset + script_size > len(records):
            raise ValueError("Faery SpellScript length rejected")
        reader.offset += script_size
        spell_type, kind = reader.integer(), reader.integer()
        rows.append({"description": description, "elemental": elemental,
                     "model": model, "name_id": name_id,
                     "spell_type": spell_type, "type": kind})
    if reader.offset != len(records):
        raise ValueError("Faery tables have trailing bytes")
    return names, lists, rows


def reachable_animation_clips(table: dict, paths: list[str], table_id: int) -> tuple[list[dict], set[int]]:
    if not 0 <= table_id < len(table["characters"]):
        raise ValueError("DefaultFairy AnimTable is out of range")
    character = table["characters"][table_id]
    sequences: set[int] = set()
    clips: set[int] = set()

    def visit(sequence_id: int, stack: tuple[int, ...]) -> None:
        if (sequence_id < 0 or sequence_id >= len(table["animations"])
                or sequence_id in stack or len(stack) >= 8):
            raise ValueError("Faery animation sequence is invalid/cyclic")
        if sequence_id in sequences:
            return
        sequences.add(sequence_id)
        for step in table["animations"][sequence_id]["Steps"]:
            if step["Redir"] == 1:
                visit(step["Anim"], stack + (sequence_id,))
            elif step["Redir"] == 0 and 0 <= step["Anim"] < len(paths):
                clips.add(step["Anim"])
            else:
                raise ValueError("Faery animation step has invalid redirect/clip")

    for field, value in character.items():
        if field == "name":
            continue
        values = value if isinstance(value, list) else [value]
        for sequence_id in values:
            if isinstance(sequence_id, int) and sequence_id >= 0:
                visit(sequence_id, ())
    details = [{"sequence_id": sid, "sequence_name": table["animations"][sid]["name"]}
               for sid in sorted(sequences)]
    return details, clips


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("cache", type=Path, help="original cache ZIP; read only")
    parser.add_argument("--project", type=Path,
                        default=Path(__file__).resolve().parents[2] / "android-native")
    args = parser.parse_args()
    with args.cache.open("rb") as source:
        if hashlib.file_digest(source, "sha256").hexdigest() != EXPECTED:
            parser.error("cache ZIP hash differs from the pinned original")

    assets = args.project / "app/src/main/assets"
    payloads: dict[str, tuple[bytes, str, str]] = {}
    with zipfile.ZipFile(args.cache) as archive:
        by_name: dict[str, list[zipfile.ZipInfo]] = {}
        for entry in archive.infolist():
            if not entry.is_dir():
                by_name.setdefault(PurePosixPath(entry.filename).name.casefold(), []).append(entry)

        def read_table(name: str) -> bytes:
            matches = by_name.get(name.casefold(), [])
            if len(matches) != 1:
                raise ValueError(f"cache table is missing or ambiguous: {name}")
            return archive.read(matches[0])

        char_blobs = {name: read_table(name) for name in CHARACTER_INPUTS}
        names, fields, char_rows = character_table(
            char_blobs[CHARACTER_INPUTS[0]], char_blobs[CHARACTER_INPUTS[1]],
            char_blobs[CHARACTER_INPUTS[2]])
        field_ids = {name: index for index, name in enumerate(fields)}
        if "DefaultFairy" not in names:
            raise ValueError("CharacterTable no longer contains DefaultFairy")
        character_id = names.index("DefaultFairy")
        char = char_rows[character_id]
        if character_id != 97 or any(field not in field_ids for field in ("AI", "AnimTable", "ModelFile", "FaeryList")):
            raise ValueError("DefaultFairy identity/required CharacterTable fields changed")
        if tuple(char[field_ids[key]] for key in ("AI", "AnimTable", "ModelFile", "FaeryList")) != (20, 23, 34, -1):
            raise ValueError("DefaultFairy source table selectors changed")

        model_paths = dictionary_strings(char_blobs[CHARACTER_INPUTS[3]])
        faery_blobs = {name: read_table(name) for name in FAERY_INPUTS}
        faery_names, faery_lists, faeries = faery_tables(
            faery_blobs[FAERY_INPUTS[0]], faery_blobs[FAERY_INPUTS[1]], faery_blobs[FAERY_INPUTS[2]])
        list_names, row_names = faery_names
        if "DEFAULT" not in list_names:
            raise ValueError("FaeryList.DEFAULT is missing")
        default_list_id = list_names.index("DEFAULT")
        if default_list_id != 0 or tuple(faery_lists[default_list_id]) != EXPECTED_DEFAULT_ROWS:
            raise ValueError("DefaultFairy fallback list contents changed")

        selected = []
        for row_id, (source_name, display_name, slot, model_id, basename) in zip(
                faery_lists[default_list_id], EXPECTED_DEFAULT):
            if row_id < 0 or row_id >= len(faeries) or row_names[row_id] != source_name:
                raise ValueError("DefaultFairy selected FaeryTable row/name changed")
            row = faeries[row_id]
            if row["type"] != slot or row["model"] != model_id:
                raise ValueError(f"DefaultFairy {source_name} type/model selector changed")
            model_path = source_path(model_paths[model_id])
            if PurePosixPath(model_path).name != basename:
                raise ValueError(f"DefaultFairy {source_name} ModelDict path changed")
            selected.append({"save_slot_type": slot, "faery_table_row": row_id,
                             "faery_table_name": source_name, "display_name": display_name,
                             "model_dictionary_id": model_id,
                             "source_path": model_path})

        animation_blobs = {name: read_table(name) for name in ANIMATION_INPUTS}
        schema = {name: animation_sections(animation_blobs[name])
                  for name in ANIMATION_INPUTS[1:3]}
        animations = parse_animation_data(animation_blobs[ANIMATION_INPUTS[0]], schema)
        clip_paths = dictionary_strings(animation_blobs[ANIMATION_INPUTS[3]])
        reachable_sequences, clip_ids = reachable_animation_clips(animations, clip_paths, 23)
        clip_sources = {source_path(clip_paths[index]) for index in clip_ids}

        def read_resource(path: str) -> tuple[bytes, str]:
            wanted = path.casefold()
            matches = [entry for entry in archive.infolist()
                       if not entry.is_dir() and entry.filename.casefold().endswith("/" + wanted)]
            if len(matches) != 1:
                raise ValueError(f"source BDAE is missing or ambiguous: {path}")
            return archive.read(matches[0]), matches[0].filename

        model_paths_selected = [row["source_path"] for row in selected]
        model_texture_names: set[str] = set()
        for path in sorted(set(model_paths_selected) | clip_sources):
            raw, entry = read_resource(path)
            basename = PurePosixPath(path).name
            if basename in MODEL_PINS:
                expected_size, expected_hash = MODEL_PINS[basename]
                if len(raw) != expected_size or hashlib.sha256(raw).hexdigest() != expected_hash:
                    raise ValueError(f"byte pin failed for {basename}")
                model_texture_names.update(images(raw))
            payloads[path] = (raw, entry, "faery_model" if basename in MODEL_PINS else "animation_clip")

        for texture in sorted(model_texture_names):
            matches = by_name.get(texture.casefold(), [])
            if not matches:
                matches = by_name.get(("pvr2_" + texture).casefold(), [])
            if len(matches) != 1:
                raise ValueError(f"Faery model texture is missing or ambiguous: {texture}")
            entry = matches[0]
            payloads["texture:" + texture.lower()] = (archive.read(entry), entry.filename, "model_texture")

    staged = []
    for key, (raw, entry, role) in payloads.items():
        if key.startswith("texture:"):
            destinations = ("textures/" + key.split(":", 1)[1],)
            source = PurePosixPath(entry).as_posix()
        else:
            source = key
            basename = PurePosixPath(source).name
            destinations = ("original-cache/" + source, "actors/" + basename)
        digest = hashlib.sha256(raw).hexdigest()
        if role == "faery_model":
            expected_size, expected_hash = MODEL_PINS[PurePosixPath(source).name]
            if len(raw) != expected_size or digest != expected_hash:
                raise ValueError("Faery model changed after input validation")
        for relative in destinations:
            target = assets / PurePosixPath(relative)
            if target.exists() and hashlib.sha256(target.read_bytes()).hexdigest() != digest:
                raise ValueError(f"staged asset conflicts with existing bytes: {relative}")
            staged.append((target, raw, {"asset": relative, "role": role,
                                         "source_path": source, "archive_entry": entry,
                                         "bytes": len(raw), "sha256": digest}))

    # All source/archive/table/hash checks finish before any output is written.
    for target, raw, _ in staged:
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(raw)
    report = {
        "cache_sha256": EXPECTED,
        "character": {"name": "DefaultFairy", "character_table_id": character_id,
                      "ai_id": 20, "animation_table_id": 23,
                      "model_dictionary_id": 34, "authored_faery_list_id": -1,
                      "fallback_faery_list": "DEFAULT"},
        "selected_faeries": selected,
        "reachable_animation_sequences": reachable_sequences,
        "reachable_animation_clips": sorted(clip_sources),
        "inputs": [entry for _, _, entry in staged],
        "runtime_binding": "OPEN: assets are staged only; no live Faery Character/ObjectManager factory or autonomous Character lifecycle is installed.",
        "loader_note": "actors/<basename> aliases use the existing BDAE resource reader. original-cache paths preserve source directories in lowercase; runtime fallback call-site/case normalization remains a separate integration task.",
    }
    report_path = assets / "faery-asset-provenance.json"
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"models": len(model_paths_selected), "clips": len(clip_sources),
                      "textures": len(model_texture_names), "staged_files": len(staged),
                      "actor_runtime": "not implemented"}))


if __name__ == "__main__":
    main()
