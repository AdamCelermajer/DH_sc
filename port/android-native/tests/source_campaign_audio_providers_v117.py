"""Audit exact Act 1 audio names and their selected source-table identities.

This is provenance/composition coverage only. It does not claim that the
Android device backend is running or that queued samples were audible.
"""
from pathlib import Path
import json
import re
import xml.etree.ElementTree as ET


ROOT = Path(__file__).resolve().parents[3]
LEDGER = ROOT / "port/engine-audio/reference/source-bindings-v38/ledger.json"
SOUNDS = ROOT / "port/level-world/reference/audio-source-v34/sounds.xml"
RULE = ROOT / "port/android-native/app/src/main/assets/worlds/007_crypt_01.rule.xml"
STAGE17 = ROOT / ".local-inputs/character-script-assets-v1/bootstrap-cache/data/scripts/ai/swampking_core.luac"
SCRIPT_AUDIO = ROOT / "port/android-native/app/src/main/cpp/source_campaign_script_audio_v117.cpp"
AUDIO_SERVICES = ROOT / "port/android-native/app/src/main/cpp/renderer_campaign_audio_services_v68.inc"
AUDIO_MUSIC = ROOT / "port/android-native/app/src/main/cpp/renderer_campaign_music_v101.inc"
AUDIO_COMPOSE = ROOT / "port/android-native/app/src/main/cpp/renderer_campaign_audio_v46.inc"
RUNTIME = ROOT / "port/android-native/app/src/main/cpp/source_campaign_runtime_v61.cpp"


def need(condition, message):
    if not condition:
        raise AssertionError(message)


def main():
    ledger = json.loads(LEDGER.read_text(encoding="utf-8"))
    by_name = {row["source_name"]: row for row in ledger}
    catalog = ET.parse(SOUNDS).getroot()
    xml_by_label = {row.attrib["label"]: row.attrib for row in catalog.findall("./sounds/sound")}

    # Generated source ordinals are what the production resolver returns;
    # each row then maps to an XML UID. These values are intentionally distinct.
    expected = {
        "CryptOneAmbientMusic": (38, 486),
        "SwampHubAmbientMusic": (211, 467),
        "SwampKingMusic": (213, 511),
        "sfx_serpent_submerge": (525, 321),
        "ChestOpen": (33, 162),
        "DropGold": (62, 151),
    }
    for name, (source_id, uid) in expected.items():
        row = by_name.get(name)
        need(row is not None, f"missing original generated sound name {name}")
        need((row["source_id"], row["uid"], row["event"]) == (source_id, uid, 0),
             f"wrong source ordinal/UID/event for {name}: {row}")
        xml = xml_by_label.get(name)
        need(xml is not None and int(xml["uid"]) == uid,
             f"generated {name} UID disagrees with selected source XML")

    rule = RULE.read_text(encoding="utf-8")
    music_name = re.search(r'\bmusic="([^"]*)"', rule)
    ambience_name = re.search(r'\bambiant_music="([^"]*)"', rule)
    need(music_name and music_name.group(1) == "CryptOneAmbientMusic",
         "first-flow authored music name changed")
    need(ambience_name and ambience_name.group(1) == "sfx_catacomb_ambiance",
         "first-flow authored ambience name changed")
    need(ambience_name.group(1) not in by_name,
         "first-flow ambience unexpectedly resolved in current generated Sounds table")

    swamp = STAGE17.read_text(encoding="utf-8")
    for call in (
        'StopSound("SwampKingMusic", 2000)',
        'PlayMusic("SwampKingMusic", 500)',
        'PlaySound("sfx_serpent_submerge", false, 0, false)',
        'PlaySound("SwampHubAmbientMusic", true, 1000, true)',
    ):
        need(call in swamp, f"Stage17 authored audio case missing: {call}")

    script_audio = SCRIPT_AUDIO.read_text(encoding="utf-8")
    services = AUDIO_SERVICES.read_text(encoding="utf-8")
    music = AUDIO_MUSIC.read_text(encoding="utf-8")
    compose = AUDIO_COMPOSE.read_text(encoding="utf-8")
    runtime = RUNTIME.read_text(encoding="utf-8")
    need("runtime->bindings().source_id(name)" in music,
         "script names must resolve through the selected generated source table")
    need("borrow_campaign_music_owner_v101(world,owner,e)&&owner->play_music" in music,
         "script music must use the retained Level gameplay audio owner")
    need("borrow_campaign_music_owner_v101(world,owner,e)&&owner->play_plain_v115" in music,
         "script plain sounds must use the retained Level gameplay audio owner")
    need("actual.actual_world!=world" in music and "captured.identity()!=out->bridge()->manager().identity()" in music,
         "Level audio owner must stay on the same World and Application SoundManager")
    need("candidate.application!=actual.application" in services and "candidate.level!=actual.level" in services,
         "provider lease must reject a foreign selected campaign candidate")
    need("source_initialize_complete_v100()" in services,
         "provider lease must require completed process Vox initialization")
    need("campaign_audio_v46=owner->bridge();campaign_level_audio_v101=owner" in compose,
         "music and script calls must share the composed campaign audio provider")
    need("borrow_campaign_audio_services_v68(actual,campaign_audio,audio_bindings,error)" in runtime,
         "canonical Level source activation must construct the concrete campaign audio lease")

    print("PASS: Act 1 and Stage17 source names, ordinals, XML UIDs, and same-provider composition are consistent")
    print("NOTE: first-flow ambiant_music=sfx_catacomb_ambiance is absent from generated Sounds; original name lookup yields source no-op")


if __name__ == "__main__":
    main()
