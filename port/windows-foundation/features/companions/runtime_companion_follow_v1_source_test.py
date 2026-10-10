"""Pin the runtime companion plan to original script/table/placement evidence."""
from __future__ import annotations

import hashlib
import json
import struct
import xml.etree.ElementTree as ET
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
FEATURE = ROOT / "port/windows-foundation/features/companions"
MGP = ROOT / ".local-inputs/windows-source-clock-v19-preview-2/assets/original-cache/data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp"
CHAR_DATA = ROOT / ".local-inputs/actors/character_properties_pyarray.bin"
CHAR_NAMES = ROOT / ".local-inputs/actors/character_properties_pyarraynames.bin"
CHAR_FIELDS = ROOT / ".local-inputs/actors/character_properties_pystructnames.bin"
AI_DATA = ROOT / ".local-inputs/ai-data/ai_pyarray.bin"
AI_NAMES = ROOT / ".local-inputs/ai-data/ai_pyarraynames.bin"
FOLLOWER = FEATURE / "reference/follower.luac"
RENE = FEATURE / "reference/rene.luac"
VIDEO_MANIFEST = ROOT / ".local-inputs/referenceframes/dh2-act1/first-spawn/manifest.json"
REPORT = FEATURE / "runtime-companion-follow-v1.json"
CPP = FEATURE / "runtime_companion_follow_v1.cpp"
HEADER = FEATURE / "runtime_companion_follow_v1.hpp"


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def string_arrays(path: Path) -> list[list[str]]:
    raw = path.read_bytes()
    offset = 0
    groups = []
    while offset < len(raw):
        count, = struct.unpack_from("<I", raw, offset)
        offset += 4
        values = []
        for _ in range(count):
            size, = struct.unpack_from("<I", raw, offset)
            offset += 4
            values.append(raw[offset:offset + size].decode("ascii"))
            offset += size
        groups.append(values)
    assert offset == len(raw)
    return groups


def character_record(name: str) -> dict[str, int]:
    names = string_arrays(CHAR_NAMES)[0]
    fields = string_arrays(CHAR_FIELDS)[0]
    raw = CHAR_DATA.read_bytes()
    count, = struct.unpack_from("<I", raw)
    assert count == len(names) == 448 and len(fields) == 224
    index = names.index(name)
    values = struct.unpack_from("<224i", raw, 4 + index * 224 * 4)
    return {field: values[i] for i, field in enumerate(fields)}


def ai_rows() -> tuple[list[str], list[dict[str, object]]]:
    names = string_arrays(AI_NAMES)[0]
    raw = AI_DATA.read_bytes()
    offset = 0

    def word() -> int:
        nonlocal offset
        value, = struct.unpack_from("<I", raw, offset)
        offset += 4
        return value

    def integer() -> int:
        return struct.unpack("<i", struct.pack("<I", word()))[0]

    def real() -> float:
        nonlocal offset
        value, = struct.unpack_from("<f", raw, offset)
        offset += 4
        return value

    def text() -> str:
        nonlocal offset
        size = word()
        value = raw[offset:offset + size].decode("ascii")
        offset += size
        return value

    count, = struct.unpack_from("<I", raw)
    offset = 4
    assert count == len(names)
    rows = []
    for _ in range(count):
        attack_delay, combat_beat, combat_music = integer(), integer(), integer()
        delayed_load = raw[offset]
        offset += 1
        flags = word()
        interact_radius, leash_distance, melee_radius = real(), real(), real()
        on_aggro_sfx = integer()
        script = text()
        self_fx, trophy, type_id = integer(), integer(), integer()
        view_radius, view_radius_no_aggro = real(), real()
        rows.append({"attack_delay": attack_delay, "combat_beat": combat_beat,
                     "combat_music": combat_music, "delayed_load": delayed_load,
                     "flags": flags, "interact_radius": interact_radius,
                     "leash_distance": leash_distance, "melee_radius": melee_radius,
                     "on_aggro_sfx": on_aggro_sfx, "script": script,
                     "self_fx": self_fx, "trophy": trophy, "type": type_id,
                     "view_radius": view_radius, "view_radius_no_aggro": view_radius_no_aggro})
    assert offset == len(raw)
    return names, rows


mgp_root = ET.parse(MGP).getroot()
objects = {row.attrib.get("name"): row.attrib for row in mgp_root.iter("GameObject")}
priest = objects["_prim_NPC_PriestGood"]
faery = objects["_prim_Faery"]
assert priest["charpropsname"] == "WanderingPriest" and priest["activate_cond"] == "RENE_FOLLOW"
assert priest["position"] == "793.9,-617.658,272.064"
assert faery["charpropsname"] == "DefaultFairy" and faery["position"] == "544.191,414.689,-108.175"

priest_props = character_record("WanderingPriest")
faery_props = character_record("DefaultFairy")
assert (priest_props["AI"], priest_props["AnimTable"], priest_props["HP"],
        priest_props["Max_HP"], priest_props["MP"], priest_props["Max_MP"]) == (50, 47, -1, -1, -1, -1)
assert (faery_props["AI"], faery_props["AnimTable"], faery_props["HP"],
        faery_props["Max_HP"], faery_props["MP"], faery_props["Max_MP"]) == (20, 23, 153600, 153600, -1, -1)

ai_names, ais = ai_rows()
assert ai_names[20] == "Faery" and ais[20]["script"] == "follower" and ais[20]["type"] == 3
assert ai_names[50] == "Rene" and ais[50]["script"] == "rene" and ais[50]["type"] == 2

follower = FOLLOWER.read_text(encoding="utf-8")
rene = RENE.read_text(encoding="utf-8")
assert "if(not HasMaster())" in follower and "SetMaster(friend);" in follower and "g_master = friend;" in follower
out_of_range = follower.split("function follower_OnMasterOutOfRange()", 1)[1].split("end\nAddToVFTable", 1)[0]
assert 'GetState() ~= GetPyCst("AIStates", "Move")' in out_of_range
assert "MoveTo(g_master);" in out_of_range and out_of_range.index("MoveTo(g_master);") < out_of_range.index("ClearTarget();")
out_of_sight = follower.split("function follower_OnMasterOutOfSight()", 1)[1].split("end\nAddToVFTable", 1)[0]
assert "WarpBehind(g_master);" in out_of_sight and "ClearTarget" not in out_of_sight
for callback in ("OnMasterInRangedRange", "OnMasterInCloseRange", "OnMasterInMeleeRange"):
    block = follower.split(f"function follower_{callback}()", 1)[1].split("end\nAddToVFTable", 1)[0]
    assert "Stop();" in block
assert "MoveTo(g_master);" in rene and "HasPath()" in rene and "WarpTo(g_master);" in rene
assert "StartTimerCB(200, false, CaughtUp)" in rene and "ApplyBuff(OID_HASTE, buff)" in rene
cpp_source = CPP.read_text(encoding="utf-8")
header_source = HEADER.read_text(encoding="utf-8")
assert "SourceFollowerEventV1::master_out_of_range" in cpp_source
assert 'member->source->ai_script != "follower"' in cpp_source
assert "never writes" in header_source.lower()

video = json.loads(VIDEO_MANIFEST.read_text(encoding="utf-8"))
assert video["version"] == "1.0.3"
for timestamp, sha in ((210, "b6c22b5f9cb7eb7282d9cc15092c14cb2a87abd8ca0362d955a76fd4614144f6"),
                       (212, "2a37ee5e1577919d96413b200812af3b666c73c859de401968f67100d246e163"),
                       (230, "26e644b55f8f3a7e7f96503a2bc8f70d9bf22fa3ed2bc2af43ee36faf7b5de02")):
    frame = next(row for row in video["frames"] if row["timestamp_seconds"] == timestamp)
    assert digest(ROOT / Path(frame["path"]).relative_to(ROOT)) == sha

report = json.loads(REPORT.read_text(encoding="utf-8"))
assert report["sources"]["mgp"]["sha256"] == digest(MGP)
assert report["sources"]["scripts"]["follower_script"]["sha256"] == digest(FOLLOWER)
assert report["sources"]["scripts"]["rene_script"]["sha256"] == digest(RENE)
assert report["verification"]["isolated_cpp17_test_result"] == "PASS"
assert report["verification"]["source_evidence_test_result"] == "PASS"
print("runtime_companion_follow_v1 source evidence PASS: actual Swamp character/AI/animation rows, source callbacks, placements, footage frame hashes, and explicit formation unknowns")
