"""Verify the companion cinematic clip bank against the exact local source tables."""
from pathlib import Path
import hashlib
import json
import struct
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[4]
ASSETS = ROOT / ".local-inputs/windows-source-clock-v19-preview-3/assets"
PYDATA = ASSETS / "original-cache/data/pydata"
DICT_NAMES = ASSETS / "data/animations_dictionary_pyarraynames.bin"
DICT_VALUES = PYDATA / "animations_dictionary_pyarray.bin"
ANIM_RECORDS = PYDATA / "animations_pyarray.bin"
ANIM_NAMES = PYDATA / "animations_pyarraynames.bin"
ANIM_SCHEMA = PYDATA / "animations_pystructnames.bin"
CAMPAIGN = ASSETS / "original-campaign.xml"
REFERENCE_CACHE = ROOT / ".local-inputs/publication/checkpoint/port/level-world/reference/character-visual-v6/cache"
PRIEST_BDAE = REFERENCE_CACHE / "data/3d/characters/npcs/animations/cs_swamp_intro_priest_scene05.bdae"
FAERY_BDAE = REFERENCE_CACHE / "data/3d/characters/faeries/animations/cs_swamp_intro_faery_scene05.bdae"


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def string_arrays(path: Path) -> list[list[str]]:
    raw = path.read_bytes()
    offset = 0
    arrays = []
    while offset < len(raw):
        count, = struct.unpack_from("<I", raw, offset)
        offset += 4
        values = []
        for _ in range(count):
            size, = struct.unpack_from("<I", raw, offset)
            offset += 4
            values.append(raw[offset:offset + size].decode("ascii"))
            offset += size
        arrays.append(values)
    assert offset == len(raw)
    return arrays


def source_sequence_animation_references() -> tuple[int, dict[int, list[dict[str, object]]]]:
    raw = ANIM_RECORDS.read_bytes()
    names = string_arrays(ANIM_NAMES)[0]
    offset = 0

    def word() -> int:
        nonlocal offset
        value, = struct.unpack_from("<I", raw, offset)
        offset += 4
        return value

    def signed() -> int:
        return struct.unpack("<i", struct.pack("<I", word()))[0]

    def boolean() -> int:
        nonlocal offset
        value = raw[offset]
        offset += 1
        assert value in (0, 1)
        return value

    def integers() -> list[int]:
        return [signed() for _ in range(word())]

    count = word()
    assert count == len(names)
    found: dict[int, list[dict[str, object]]] = {396: [], 377: [], 686: []}
    for sequence in range(count):
        loop = signed()
        for step in range(word()):
            boolean()  # AnchorFX
            anim = signed()
            signed()  # BlendOut
            signed()  # Cam
            boolean()  # CamDir
            signed()  # FX
            boolean()  # MoveGO
            integers()  # RandomCam
            redirect = signed()
            signed()  # Sound
            speed, = struct.unpack_from("<f", raw, offset)
            offset += 4
            boolean()  # Swoosh
            if anim in found and redirect == 0:
                found[anim].append({"sequence_index": sequence, "sequence_name": names[sequence],
                                   "step_index": step, "loop": loop, "speed": speed})
        signed()  # sequence Type
    return offset, found


assert sha256(CAMPAIGN) == "3ab2055920a8b648b16d77aa07b0435cbd899c624f5be3bcc145e2133c5cdd14"
dictionary_names = string_arrays(DICT_NAMES)[0]
dictionary_paths = string_arrays(DICT_VALUES)[0]
assert len(dictionary_names) == len(dictionary_paths) == 1447
expected_rows = {
    396: ("cs_swamp_intro_priest_scene05", "data/3D/characters/npcs/animations/cs_swamp_intro_priest_scene05.bdae"),
    377: ("cs_swamp_intro_faery_scene05", "data/3D/characters/faeries/animations/cs_swamp_intro_faery_scene05.bdae"),
    686: ("faeries_celeste_idle", "data/3D/characters/faeries/animations/faeries_celeste_idle.bdae"),
}
for animation_id, row in expected_rows.items():
    assert (dictionary_names[animation_id], dictionary_paths[animation_id]) == row

root = ET.parse(CAMPAIGN).getroot()
swamp = next(script for script in root.iter("script") if script.attrib.get("name") == "Swamp_Intro")
commands = {int(command.attrib["index"]): command for command in swamp.findall("command")}
expected_commands = {
    45: ("_prim_NPC_PriestGood", 396, 0xFFFFFFFF, 2, 20, 0),
    46: ("_prim_Faery", 377, 0xFFFFFFFF, 3, 11, 0),
}
for index, (receiver, anim, next_anim, slot, offset20, wait) in expected_commands.items():
    command = commands[index]
    scalars = {int(item.attrib["offset"]): int(item.attrib["bits"]) for item in command.findall("scalar")}
    strings = {int(item.attrib["offset"]): item.attrib["value"] for item in command.findall("string")}
    assert command.attrib["className"] == "Script_PlayActorAnim"
    assert (strings[24], scalars[8], scalars[12], scalars[16], scalars[20], scalars[28]) == \
           (receiver, anim, next_anim, slot, offset20, wait)

sequence_bytes, table_refs = source_sequence_animation_references()
assert 0 < sequence_bytes < len(ANIM_RECORDS.read_bytes())
assert table_refs[396] == [] and table_refs[377] == []
assert table_refs[686] == [{"sequence_index": 196, "sequence_name": "Faeries_Idle",
                            "step_index": 0, "loop": -1, "speed": 1.0}]
assert PRIEST_BDAE.is_file() and sha256(PRIEST_BDAE) == \
       "912986b33e45f324a6a3949ade5c479dd6a3ee3b4023f78949847cbb75386315"
assert not FAERY_BDAE.exists()
assert not list(REFERENCE_CACHE.rglob("cs_swamp_intro_faery_scene05.bdae"))

print("source_companion_animation_bank_v1 source evidence PASS: exact AnimDict rows 396/377; 396/377 have no AnimationTables sequence loop/rate rows; only Priest BDAE is locally present; existing FaeryIdle 686 remains loop=-1/rate=1")
