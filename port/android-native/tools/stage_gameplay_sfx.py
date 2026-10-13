#!/usr/bin/env python3
"""Stage the original gameplay SFX referenced by Sounds.xml and generate their catalog.

Inputs (owner supplied, not committed): the complete cache ZIP checked by
``port/level-world/tools/inventory.py``. The Sounds.xml pack catalog is the
committed ``recovered/.../sounds_he.xml``; the SDD ``Sounds`` row table
(Arrays::Sounds::read 0x4b5690 -> Structs::SoundAutoGen::read 0x502af4:
{int sound-pack UID or event UID, int is_event}) is read from
``data/pydata/sdd_dungeon_hunter_2_iphone_pyarray.bin``.

Selection rule (never edits game data, never invents files):
  * format ``pcm`` Sounds.xml rows outside the three MUSIC groups (1..3),
  * whose exact ``filename`` is a file of the cache ``data/sounds`` folder,
  * minus the five menu effects staged at ``original-media/`` (UIDs 0..4),
  * minus UIDs already carried by the hand-audited ``catalog`` in
    ``player_gameplay_audio.hpp`` (their files are already staged).
Selected files are copied byte-exact into
``assets/original-media/gameplay`` and recorded, with the complete SDD row
table, in ``sfx-catalog-provenance.json``. The header
``gameplay_sfx_catalog_v1.hpp`` is generated from the same data.

  python tools/stage_gameplay_sfx.py --cache <cache.zip>      # stage + generate
  python tools/stage_gameplay_sfx.py --check                  # verify, no cache
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
import sys
import xml.etree.ElementTree as ET
import zipfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
APP = REPO / "port/android-native/app/src/main"
CPP = APP / "cpp"
GAMEPLAY = APP / "assets/original-media/gameplay"
PROVENANCE = GAMEPLAY / "sfx-catalog-provenance.json"
HEADER = CPP / "gameplay_sfx_catalog_v1.hpp"
XML_PATH = REPO / ("recovered/assets/source-data/com.gameloft.android.GAND.GloftD2SS/"
                   "files/data/sounds/sounds_he.xml")
NAMES_PATH = REPO / "reports/pydata-array-name-trace.json"
CACHE_PREFIX = "com.gameloft.android.GAND.GloftD2SS/files/data/"
EXPECTED_CACHE_SHA256 = "3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679"
MUSIC_GROUPS = {1, 2, 3}
MENU_UIDS = {0, 1, 2, 3, 4}  # MenuSpending/Back/Confirm/Select/Tab: original-media/ root
BANK_BEHAVIOUR = {  # vox::VoxSoundPackXML::LoadXML 0x88d344 (+ BankXMLDef default 3)
    "steal oldest": 0,
    "steal lowest priority": 1,
    "steal low. prio. or old. same prio": 2,
}
POS3D = {"no": 0, "yes": 1, "relative": 2}  # LoadXML: 'y' -> 1, 'r' -> 2, else 0
SLIDER = {"MUSIC": 0, "SFX": 1, "VFX": 2}
MAX_EVENT_MEMBERS = 4
SELECTION_RULE = [
    "format pcm Sounds.xml rows outside the three MUSIC groups (1..3)",
    "whose exact filename is a file of the cache data/sounds folder",
    "minus the five menu effects staged at original-media/ (UIDs 0..4)",
    "plus the 51 hand-audited player_gameplay_audio.hpp catalog UIDs, whose files were staged earlier",
]


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def read_old_catalog() -> dict[int, tuple[str, str]]:
    text = (CPP / "player_gameplay_audio.hpp").read_text(encoding="utf-8")
    block = text.split("inline constexpr std::array<CatalogEntry,51> catalog{{", 1)[1].split("}};", 1)[0]
    rows = re.findall(r'^\s*\{"([^"]+)","([^"]+)","([^"]+)",(\d+),(\d+)\},$', block, re.M)
    assert len(rows) == 51
    return {int(uid): (label, filename) for label, _src, filename, uid, _row in rows}


def load_catalog_xml():
    root = ET.parse(XML_PATH).getroot()
    sounds = {int(s.get("uid")): s.attrib for s in root.iter("sound")}
    events = {int(e.get("uid")): e.attrib for e in root.iter("event")}
    banks = {int(b.get("uid")): b.attrib for b in root.iter("bank")}
    groups = {int(g.get("uid")): g.attrib for g in root.iter("group")}
    masks = {m.get("label"): int(m.get("mask")) for m in root.iter("groupmask")}
    return sounds, events, banks, groups, masks


def sdd_names() -> list[str]:
    inventory = json.loads(NAMES_PATH.read_text(encoding="utf-8"))
    for source in inventory["files"]:
        for table in source["tables"]:
            if table["class"] == "Sounds":
                return table["names"]
    raise SystemExit("SDD Sounds names missing from reports/pydata-array-name-trace.json")


def parse_event(attrib: dict[str, str]):
    """EventXMLDef: params = '<history>[;<probability>]', default history 0, probability 100."""
    members = [int(v) for v in re.split(r"[ ,]+", attrib["value"].strip()) if v]
    params = [p for p in re.split(r"[ ;]+", attrib.get("params", "").strip()) if p]
    history = min(int(params[0]), len(members)) if params else 0
    probability = min(int(params[1]), 100) if len(params) > 1 else 100
    return {"playlist": attrib["type"] == "playlist", "history": history,
            "probability": probability, "members": members}


def build_model(archive: zipfile.ZipFile | None, provenance: dict | None):
    sounds, events, banks, groups, masks = load_catalog_xml()
    names = sdd_names()
    old = read_old_catalog()
    if archive is not None:
        raw = archive.read(CACHE_PREFIX + "pydata/sdd_dungeon_hunter_2_iphone_pyarray.bin")
        count = struct.unpack_from("<I", raw, 0)[0]
        assert count == len(names) == 638 and len(raw) == 4 + 8 * count
        rows = [struct.unpack_from("<ii", raw, 4 + 8 * i) for i in range(count)]
        sound_dir = CACHE_PREFIX + "sounds/"
        cache_files = {i.filename[len(sound_dir):]: i for i in archive.infolist()
                       if i.filename.startswith(sound_dir) and not i.is_dir()}
    else:
        rows = [(r["target"], 1 if r["kind"] == "event" else 0) for r in provenance["rows"]]
        cache_files = {}
    row_of_uid: dict[int, list[int]] = {}
    for row, (target, is_event) in enumerate(rows):
        if not is_event:
            row_of_uid.setdefault(target, []).append(row)
            assert sounds[target]["label"] == names[row], (row, target)
        else:
            assert target in events

    staged_by_file: dict[str, dict] = {}
    selected: list[dict] = []
    unavailable: list[dict] = []
    for uid in sorted(sounds):
        s = sounds[uid]
        music = int(s["group"]) in MUSIC_GROUPS or s["format"] != "pcm"
        if music or uid in MENU_UIDS:
            continue
        if uid in old:
            selected.append({"uid": uid, "label": s["label"], "filename": old[uid][1],
                             "xml_filename": s["filename"], "old": True})
            continue
        if archive is not None:
            available = s["filename"] in cache_files
        else:
            available = any(r["uid"] == uid for r in provenance["sounds"])
        if available:
            selected.append({"uid": uid, "label": s["label"], "filename": s["filename"],
                             "xml_filename": s["filename"], "old": False})
        else:
            unavailable.append({"uid": uid, "label": s["label"], "filename": s["filename"],
                                "source_sound_ids": row_of_uid.get(uid, [])})
    return dict(sounds=sounds, events=events, banks=banks, groups=groups, masks=masks,
                names=names, rows=rows, row_of_uid=row_of_uid, selected=selected,
                unavailable=unavailable, cache_files=cache_files, old=old)


def cpp_string(value: str) -> str:
    assert re.fullmatch(r"[A-Za-z0-9_ .\-]+", value), value
    return value


def generate_header(model: dict) -> str:
    sounds, events, banks, groups = (model[k] for k in ("sounds", "events", "banks", "groups"))
    selected, old = model["selected"], model["old"]
    available_uids = {s["uid"] for s in selected}
    out: list[str] = []
    w = out.append
    w("// Generated by port/android-native/tools/stage_gameplay_sfx.py from the committed")
    w("// recovered Sounds.xml (sounds_he.xml) and the SDD Sounds row table; do not edit.")
    w("// Evidence: vox::VoxSoundPackXML::LoadXML 0x88d344, GetBankInfo 0x889498,")
    w("// PriorityBankManager::_CanAddEmitter 0x863578 / AddEmitter 0x86ea54,")
    w("// VoxSoundManager::Play 0x36b80c / Play3D 0x36b5d8 / PlaySoundPackSound 0x36a7c0,")
    w("// VoxSoundPackXML::GetEventSoundUid 0x88bb6c.")
    w("#pragma once")
    w('#include "gameplay_sfx_types_v1.hpp"')
    w("#include <array>")
    w("#include <cstdint>")
    w("namespace dh2::player_gameplay_audio {")
    # Extended catalog entries: files first staged by this generator.
    new_entries = [s for s in selected if not s["old"]]
    w(f"inline constexpr std::array<CatalogEntry,{len(new_entries)}> extended_catalog{{{{")
    for s in new_entries:
        rows = model["row_of_uid"].get(s["uid"], [])
        row = rows[0] if rows else 0xffff
        w(f'    {{"{cpp_string(s["label"])}","{cpp_string(s["xml_filename"])}",'
          f'"{cpp_string(s["filename"])}",{s["uid"]},{row}}},')
    w("}};")
    # Per-UID Sounds.xml attributes for every playable (old + extended) SFX.
    w(f"inline constexpr std::array<SfxDefV1,{len(selected)}> sfx_defs{{{{")
    for s in selected:
        a = sounds[s["uid"]]
        loop = "true" if a.get("loop") == "yes" else "false"
        w(f'    {{{s["uid"]},{int(a["bank"])},{int(a["group"])},{int(a["priority"])},{loop}}},')
    w("}};")
    bank_count = max(banks) + 1
    w(f"inline constexpr std::array<SfxBankV1,{bank_count}> sfx_banks{{{{")
    for b in range(bank_count):
        if b in banks:
            a = banks[b]
            w(f'    {{{b},{int(a["maxplaybacks"])},{BANK_BEHAVIOUR.get(a["behaviour"], 3)}}},'
              f'  // {a["name"]}: {a["behaviour"]}')
        else:  # BankXMLDef default: behaviour 3, maxplaybacks 0x7fffffff
            w(f"    {{{b},2147483647,3}},  // unset bank")
    w("}};")
    group_count = max(groups) + 1
    w(f"inline constexpr std::array<SfxGroupV1,{group_count}> sfx_groups{{{{")
    for g in range(group_count):
        if g in groups:
            a = groups[g]
            w(f'    {{{g},{POS3D[a["pos3d"]]},{SLIDER[a["volumeslider"]]}}},  // {a["name"]}')
        else:
            w(f"    {{{g},0,0}},  // unset group")
    w("}};")
    # Events: any event with at least one staged member.
    usable = {}
    for e, attrib in sorted(events.items()):
        parsed = parse_event(attrib)
        assert len(parsed["members"]) <= MAX_EVENT_MEMBERS
        if any(m in available_uids for m in parsed["members"]):
            usable[e] = (attrib["label"], parsed)
    w(f"inline constexpr std::array<SfxEventV1,{len(usable)}> sfx_events{{{{")
    for e, (label, p) in usable.items():
        members = p["members"] + [0] * (MAX_EVENT_MEMBERS - len(p["members"]))
        play = "true" if p["playlist"] else "false"
        w(f'    {{{e},{play},{p["history"]},{p["probability"]},{len(p["members"])},'
          f'{{{{{",".join(str(m) for m in members)}}}}}}},  // {label}')
    w("}};")
    event_rows = [(row, target) for row, (target, is_event) in enumerate(model["rows"])
                  if is_event and target in usable]
    w(f"inline constexpr std::array<SfxEventRowV1,{len(event_rows)}> sfx_event_rows{{{{")
    for row, target in event_rows:
        w(f"    {{{row},{target}}},  // {model['names'][row]}")
    w("}};")
    w("} // namespace dh2::player_gameplay_audio")
    return "\n".join(out) + "\n"


def provenance_document(model: dict, archive_sha: str) -> dict:
    by_file: dict[str, dict] = {}
    for s in model["selected"]:
        by_file.setdefault(s["filename"], {"filename": s["filename"], "uids": []})["uids"].append(s["uid"])
    staged = []
    for filename, record in sorted(by_file.items()):
        data = (GAMEPLAY / filename).read_bytes()
        rec = {"filename": filename, "bytes": len(data), "sha256": sha256(data),
               "source_entry": CACHE_PREFIX + "sounds/" + filename,
               "uids": record["uids"]}
        staged.append(rec)
    return {
        "source_archive": "Dungeon-Hunter-2-HD-v1-0-2-cache.zip",
        "source_archive_sha256": archive_sha,
        "source_audio_catalog": "recovered/assets/source-data/com.gameloft.android.GAND.GloftD2SS/files/data/sounds/sounds_he.xml",
        "source_row_table": CACHE_PREFIX + "pydata/sdd_dungeon_hunter_2_iphone_pyarray.bin",
        "selection_rule": SELECTION_RULE,
        "sounds": [{"uid": s["uid"], "label": s["label"], "filename": s["filename"],
                    "xml_filename": s["xml_filename"], "hand_audited_catalog": s["old"]}
                   for s in model["selected"]],
        "staged": staged,
        "unavailable": model["unavailable"],
        "rows": [{"id": i, "name": model["names"][i],
                  "kind": "event" if is_event else "sound", "target": target}
                 for i, (target, is_event) in enumerate(model["rows"])],
    }


def stage(args) -> None:
    with args.cache.open("rb") as stream:
        archive_sha = hashlib.file_digest(stream, "sha256").hexdigest()
    assert archive_sha == EXPECTED_CACHE_SHA256, "unexpected cache archive"
    with zipfile.ZipFile(args.cache) as archive:
        model = build_model(archive, None)
        copied = skipped = 0
        for s in model["selected"]:
            if s["old"]:
                continue
            data = archive.read(model["cache_files"][s["xml_filename"]])
            target = GAMEPLAY / s["filename"]
            if target.exists():
                assert target.read_bytes() == data, f"staged file differs from the cache: {target}"
                skipped += 1
            else:
                target.write_bytes(data)
                copied += 1
        for s in model["selected"]:  # every selected file must now exist and be RIFF/WAVE
            data = (GAMEPLAY / s["filename"]).read_bytes()
            assert data[:4] == b"RIFF" and data[8:12] == b"WAVE", s
        HEADER.write_text(generate_header(model), encoding="utf-8", newline="\n")
        document = provenance_document(model, archive_sha)
        PROVENANCE.write_text(json.dumps(document, indent=1) + "\n", encoding="utf-8", newline="\n")
    print(f"staged {copied} new files ({skipped} already present); "
          f"{len(model['selected'])} playable sounds, {len(model['unavailable'])} sounds without cache files")


def check(_args) -> None:
    document = json.loads(PROVENANCE.read_text(encoding="utf-8"))
    model = build_model(None, document)
    expected = generate_header(model)
    assert HEADER.read_text(encoding="utf-8") == expected, "gameplay_sfx_catalog_v1.hpp is stale"
    for record in document["staged"]:
        data = (GAMEPLAY / record["filename"]).read_bytes()
        assert len(data) == record["bytes"] and sha256(data) == record["sha256"], record["filename"]
    for record in document["unavailable"]:
        assert not (GAMEPLAY / record["filename"]).exists(), record["filename"]
    total = sum(r["bytes"] for r in document["staged"])
    print(f"OK: {len(document['staged'])} staged SFX files ({total} bytes), header current")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--cache", type=Path)
    parser.add_argument("--check", action="store_true")
    options = parser.parse_args()
    if options.check:
        check(options)
    elif options.cache:
        stage(options)
    else:
        sys.exit("pass --cache <zip> to stage or --check to verify")
