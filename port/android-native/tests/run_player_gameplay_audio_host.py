#!/usr/bin/env python3
"""Verify a source-authored combat sound reaches the native Android audio queue."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import tempfile
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT / "port/android-native/app/src/main"
TEST = Path(__file__).with_name("player_gameplay_audio.cpp")
LABEL = "sfx_skill_mage_thunder_braid_add_2"
FILE = "sfx_skill_mage_thunder_braid_add_2.wav"

parser = argparse.ArgumentParser()
parser.add_argument("--compiler", default=os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++"))
args = parser.parse_args()
if not args.compiler:
    raise SystemExit("C++17 compiler not found; pass --compiler")

# Source evidence: chain lightning selects this non-looping cue when a second
# victim is found. The source sound catalog contains a comma typo in filename;
# the cache's extracted PCM asset uses the intended .wav filename.
skill = (APP / "assets/scripts/skills/prince_mage_chain_lightning.luac").read_text(encoding="utf-8")
assert f'PlaySound("{LABEL}",false,0,false)' in skill
catalog = ROOT / "recovered/assets/source-data/com.gameloft.android.GAND.GloftD2SS/files/data/sounds/sounds_he.xml"
rows = [row for row in ET.parse(catalog).getroot().iter("sound") if row.get("label") == LABEL]
assert len(rows) == 1 and rows[0].get("uid") == "118" and rows[0].get("format") == "pcm"
assert rows[0].get("loop") == "no" and rows[0].get("filename") == "sfx_skill_mage_thunder_braid_add_2,wav"
assert (APP / f"assets/original-media/gameplay/{FILE}").is_file()

# CharSoundsTable is read from sounds_pyarray.bin, while its three row names
# and the SDD Sounds labels/indices are preserved in the source inventory.
inventory = json.loads((ROOT / "reports/pydata-array-name-trace.json").read_text(encoding="utf-8"))
sound_names = {}
character_names = None
listener_names = None
for source_file in inventory["files"]:
    if source_file["path"] == "data/pydata/sounds_pyarraynames.bin":
        character_table = next(t for t in source_file["tables"] if t["class"] == "CharSoundsTable")
        character_names = character_table["names"]
        listeners = next(t for t in source_file["tables"] if t["class"] == "Listeners")
        listener_names = listeners["names"]
    elif source_file["path"] == "data/pydata/sdd_dungeon_hunter_2_iphone_pyarraynames.bin":
        sound_table = next(t for t in source_file["tables"] if t["class"] == "Sounds")
        sound_names = sound_table["names"]
assert character_names == ["AAA_DONT_DELETE", "ArmoredHuman", "Default"]
assert listener_names == ["AAA_DONT_DELETE_Listener", "curListener", "PlayerListener", "BAPListener", "CameraListener"]
struct_inventory = json.loads((ROOT / "reports/pydata-struct-name-trace.json").read_text(encoding="utf-8"))
listener_struct = next(x for x in struct_inventory["tables"] if x["class"] == "Listener")
assert listener_struct["names"] == ["Anchor", "MaxDistance", "Orientation", "RefDistance", "RolloffFactor", "UpVector"]
source_rows = {
    28: ("CharacterDie", "sfx_character_death.wav", 50),
    29: ("CharacterHurt1", "sfx_character_hurt_1.wav", 38),
    30: ("CharacterHurt2", "sfx_character_hurt_2.wav", 39),
    31: ("CharacterHurt3", "sfx_character_hurt_3.wav", 40),
    117: ("ImpactMetal1", "sfx_impact_metal_1.wav", 41),
    118: ("ImpactMetal2", "sfx_impact_metal_2.wav", 42),
    119: ("ImpactMetal3", "sfx_impact_metal_3.wav", 43),
    120: ("ImpactMetalFlesh1", "sfx_impact_metal_flesh_1.wav", 44),
    121: ("ImpactMetalFlesh2", "sfx_impact_metal_flesh_2.wav", 45),
    122: ("ImpactMetalFlesh3", "sfx_impact_metal_flesh_3.wav", 46),
}
xml_sounds = {row.get("label"): row for row in ET.parse(catalog).getroot().iter("sound")}
for source_id, (label, filename, uid) in source_rows.items():
    assert sound_names[source_id] == label
    assert xml_sounds[label].get("uid") == str(uid)
    # ImpactMetal XML has a known duplicated "metal" token; the retained
    # cache files and original_menu_sound_data map to this corrected asset name.
    asset = APP / f"assets/original-media/gameplay/{filename}"
    assert asset.is_file()
    with asset.open("rb") as pcm:
        header = pcm.read(12)
    assert header[:4] == b"RIFF" and header[8:12] == b"WAVE"

# The production lookup is the complete ordered SDD Sounds name table, not
# the smaller retained playback catalog. Keep every source row ID exact.
cpp = APP / "cpp"
sound_name_header = cpp / "source_sound_names_v1.hpp"
generated_names = [
    (int(row_id), json.loads('"' + name + '"'))
    for row_id, name in re.findall(
        r'^\s*\{(\d+), "((?:[^"\\]|\\.)*)"\},$',
        sound_name_header.read_text(encoding="utf-8"), re.M)
]
assert generated_names == list(enumerate(sound_names)) and len(generated_names) == 638
audio_header = (cpp / "player_gameplay_audio.hpp").read_text(encoding="utf-8")
catalog_block = audio_header.split("inline constexpr std::array<CatalogEntry,51> catalog{{", 1)[1].split("}};", 1)[0]
catalog_rows = re.findall(
    r'^\s*\{"([^"]+)","([^"]+)","([^"]+)",(\d+),(\d+)\},$',
    catalog_block, re.M)
assert len(catalog_rows) == 51
for label, source_filename, playback_filename, uid, source_id in catalog_rows:
    assert int(source_id) < len(sound_names) and sound_names[int(source_id)] == label
    source_row = xml_sounds[label]
    assert source_row.get("filename") == source_filename
    assert source_row.get("uid") == uid
    assert (APP / f"assets/original-media/gameplay/{playback_filename}").is_file()

animation_manifest_path = APP / "assets/original-media/gameplay/animation-sfx-provenance.json"
animation_manifest = json.loads(animation_manifest_path.read_text(encoding="utf-8"))
assert animation_manifest["source_archive"] == "Dungeon-Hunter-2-HD-v1-0-2-cache.zip"
assert len(animation_manifest["staged"]) == 14 and len(animation_manifest["unavailable"]) == 5
for row in animation_manifest["staged"]:
    assert sound_names[row["source_sound_id"]] == row["source_sound_name"]
    source = xml_sounds[row["source_sound_name"]]
    assert source.get("uid") == str(row["uid"])
    assert source.get("filename") == row["filename"]
    asset = APP / f"assets/original-media/gameplay/{row['filename']}"
    data = asset.read_bytes()
    assert len(data) == row["bytes"] and hashlib.sha256(data).hexdigest() == row["sha256"]
    assert data[:4] == b"RIFF" and data[8:12] == b"WAVE"
for row in animation_manifest["unavailable"]:
    assert sound_names[row["source_sound_id"]] == row["source_sound_name"]
    assert xml_sounds[row["source_sound_name"]].get("uid") == str(row["uid"])
    assert not (APP / f"assets/original-media/gameplay/{row['filename']}").exists()

script_manifest = json.loads(
    (APP / "assets/original-media/gameplay/script-sfx-provenance.json").read_text(encoding="utf-8"))
assert len(script_manifest["script_sound_assets"]) == 10
for row in script_manifest["script_sound_assets"]:
    assert sound_names[row["source_sound_id"]] == row["source_sound_name"]
    source = xml_sounds[row["source_sound_name"]]
    assert source.get("uid") == str(row["uid"])
    assert source.get("filename") == row["filename"]
    asset = APP / f"assets/original-media/gameplay/{row['filename']}"
    data = asset.read_bytes()
    assert len(data) == row["bytes"] and hashlib.sha256(data).hexdigest() == row["sha256"]
    assert data[:4] == b"RIFF" and data[8:12] == b"WAVE"

# Verify the existing JNI/Android path remains connected end-to-end.
native_skills = (cpp / "native_player_skills.cpp").read_text(encoding="utf-8")
native_bridge = (cpp / "native_app.cpp").read_text(encoding="utf-8")
front_audio = (APP / "java/com/example/dh2/FrontAudio.java").read_text(encoding="utf-8")
activity = (APP / "java/com/example/dh2/MainActivity.java").read_text(encoding="utf-8")
gradle = (ROOT / "port/android-native/app/build.gradle.kts").read_text(encoding="utf-8")
assert "player_gameplay_audio::enqueue" in native_skills
assert "player_gameplay_audio::consume" in native_bridge
assert 'file.startsWith("gameplay:")' in front_audio and '"original-media/gameplay/"+gameplayFile' in front_audio
assert 'file.startsWith("dh2fx,")' in front_audio
assert 'file.startsWith("dh2music,")' in front_audio
assert '"CryptOneAmbientMusic"' not in front_audio
for state in ("ambient", "combat"):
    track = APP / f"assets/original-media/gameplay/music/crypt_{state}_music.wav"
    assert track.is_file() and track.stat().st_size == 5_140_540
    raw = track.read_bytes()[:60]
    assert raw[:4] == b"RIFF" and raw[8:12] == b"WAVE"
    assert struct.unpack_from("<HHI", raw, 20) == (17, 2, 32_000)
    assert struct.unpack_from("<HHHH", raw, 32) == (1024, 4, 2, 1017)
    assert struct.unpack_from("<I", raw, 48)[0] == 5_105_117
for state in ("ambient", "combat"):
    track = APP / f"assets/original-media/gameplay/music/swamp_{state}_music.wav"
    assert track.is_file() and track.stat().st_size == 6_138_940
    raw = track.read_bytes()[:60]
    assert raw[:4] == b"RIFF" and raw[8:12] == b"WAVE"
    assert struct.unpack_from("<HHI", raw, 20) == (17, 2, 32_000)
    assert struct.unpack_from("<HHHH", raw, 32) == (1024, 4, 2, 1017)
    assert struct.unpack_from("<I", raw, 48)[0] == 6_096_650
safezone = APP / "assets/original-media/gameplay/music/swamp_merchant_camp_music.wav"
assert safezone.is_file() and safezone.stat().st_size == 3_591_228
assert (APP / "assets/original-media/gameplay/music/swamp-music-provenance.json").is_file()
assert 'effect.owner!=owner||effect.soundId!=soundId' in front_audio
assert "player_gameplay_audio::stop_source_sound" in (cpp / "model_renderer.cpp").read_text(encoding="utf-8")
audio_owner = (cpp / "player_gameplay_audio.hpp").read_text(encoding="utf-8")
assert "enqueue_spatial_play" in audio_owner and "evaluate_stereo_pan(request)" in audio_owner
assert 'fields.length==10' in front_audio and "leftMix=leftQ14/16384f" in front_audio
assert "consumeMenuSound()" in activity and 'noCompress += listOf("wav", "mp4")' in gradle
start_game = (cpp / "model_renderer.cpp").read_text(encoding="utf-8").split(
    "std::string start_menu_game", 1)[1].split("std::vector<std::uint8_t> read_asset", 1)[0]
assert start_game.index("load_world(") < start_game.index("World load failed:")
assert start_game.index("World load failed:") < start_game.index(
    'enqueue_source_vxn_music_state(source_music_uid,"ambient")')
renderer = (cpp / "model_renderer.cpp").read_text(encoding="utf-8")
assert 'active_level_assets.level_file=="001_swamp.mlx")return 467' in renderer
assert 'active_level_assets.level_file=="007_crypt_01.rule.xml")return 486' in renderer
assert 'enqueue_source_vxn_music_state(music_uid,state)' in renderer
assert "SourceMusicResources.resolve(soundUid,fields[2])" in front_audio
assert "gameplayMusic.selectState(soundUid,state,resource)" in front_audio
decoder = (APP / "java/com/example/dh2/GameplayVxnMusic.java").read_text(encoding="utf-8")
block_decoder = (APP / "java/com/example/dh2/VxnImaAdpcmDecoder.java").read_text(encoding="utf-8")
resolver = (APP / "java/com/example/dh2/SourceMusicResources.java").read_text(encoding="utf-8")
assert '"original-media/gameplay/music/swamp_"+state+"_music.wav"' in resolver
assert '"original-media/gameplay/music/crypt_"+state+"_music.wav"' in resolver
assert "SourceMusicResources.resolve(soundUid,next)" in decoder
assert "AudioTrack.MODE_STREAM" in decoder and "AudioTrack.WRITE_BLOCKING" in decoder
assert "Source music mixer active" in decoder and "active_slots" in decoder
assert "FADE_FRAMES=RATE*2" in decoder and "VxnSegmentMixMath.SLOT_COUNT" in decoder
assert "VxnSegmentMixMath.mix(source,activeFrames" in decoder
assert "cueQueue.addLast(resource)" in decoder and "cueQueue.pollFirst()" in decoder
assert "cue!=null&&!cue.sameSegment(selected)" in decoder
music_entry=front_audio.split("private void playGameplayMusic(int soundUid",1)[1].split("private void updateGameplayMusicPlayback()",1)[0]
assert "gameplayMusic.selectState(soundUid,state,resource)" in music_entry
assert "if(gameplayMusic==null)gameplayMusic=new GameplayVxnMusic(context)" in music_entry
music_manifest=json.loads((ROOT/"port/engine-audio/native-format/reference/crypt-music-loader-v1/manifest.json").read_text(encoding="utf-8"))
shared_uids={row["uid"] for row in music_manifest["music"]["shared_file_tracks"]}
assert shared_uids=={484,485,486,487}
native_step = (ROOT / "port/engine-audio/native-format/ima_adpcm_step_v1.hpp").read_text(encoding="utf-8")
def table_values(source: str, name: str) -> tuple[int, ...]:
    values = (re.search(rf"{name}[^{{]*\{{\{{([\s\S]*?)\}}\}}", source)
              or re.search(rf"{name}\s*=\s*\{{([\s\S]*?)\}}", source))
    assert values, name
    return tuple(int(value) for value in re.findall(r"-?\d+", values.group(1)))
assert table_values(block_decoder, "STEP") == table_values(native_step, "kImaAdpcmStepTableV1")
assert table_values(block_decoder, "INDEX") == table_values(native_step, "kImaAdpcmIndexTableV1")

with tempfile.TemporaryDirectory(prefix="dh2-gameplay-audio-") as temp:
    exe = Path(temp) / ("audio-test.exe" if os.name == "nt" else "audio-test")
    subprocess.run([
        args.compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic", "-O2",
        str(TEST), "-o", str(exe),
    ], cwd=ROOT, check=True)
    subprocess.run([str(exe)], cwd=ROOT, check=True)
    javac=shutil.which("javac")
    java=shutil.which("java")
    if not javac or not java:
        raise SystemExit("JDK javac/java required to exercise the exact Android IMA block decoder")
    java_test=ROOT/"port/android-native/tests/VxnImaAdpcmDecoderHostTest.java"
    java_mix_test=ROOT/"port/android-native/tests/VxnSegmentMixMathHostTest.java"
    java_resource_test=ROOT/"port/android-native/tests/SourceMusicResourcesHostTest.java"
    java_decoder=APP/"java/com/example/dh2/VxnImaAdpcmDecoder.java"
    java_mix=APP/"java/com/example/dh2/VxnSegmentMixMath.java"
    java_resource=APP/"java/com/example/dh2/SourceMusicResources.java"
    subprocess.run([javac,"-d",temp,str(java_decoder),str(java_mix),str(java_resource),
                    str(java_test),str(java_mix_test),str(java_resource_test)],cwd=ROOT,check=True)
    subprocess.run([java,"-cp",temp,"com.example.dh2.VxnImaAdpcmDecoderHostTest"],cwd=ROOT,check=True)
    subprocess.run([java,"-cp",temp,"com.example.dh2.VxnSegmentMixMathHostTest"],cwd=ROOT,check=True)
    subprocess.run([java,"-cp",temp,"com.example.dh2.SourceMusicResourcesHostTest"],cwd=ROOT,check=True)
print("PASS: original skill script -> sound catalog/PCM asset -> Android audio owner")
