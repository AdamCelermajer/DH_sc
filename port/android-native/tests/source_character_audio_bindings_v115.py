"""Regression guard for the authored character-session audio callback route.

The native owner is Android-only, so this checks its production composition
with the exact packaged SwampKing script and source callback/helper contracts.
It intentionally does not claim to execute the SoundManager on a host.
"""
from pathlib import Path
import re


ROOT = Path(__file__).resolve().parents[3]
OWNER = (ROOT / "port/android-native/app/src/main/cpp/source_campaign_combat_v115.cpp").read_text(encoding="utf-8")
SCRIPT = (ROOT / ".local-inputs/character-script-assets-v1/bootstrap-cache/data/scripts/ai/swampking_core.luac").read_text(encoding="utf-8")


def require(condition, message):
    if not condition:
        raise AssertionError(message)


def main():
    binding = OWNER.split("struct ScriptBindingV115 {", 1)[1].split("template<class Input>bool bind_input", 1)[0]
    for address in ("0x37e420", "0x37e578", "0x37e730"):
        require(address in binding, f"missing actual source callback {address}")
    require("input.gameplay_binding=ScriptBindingV115::select" in OWNER,
            "CharacterScriptSessionV3 production input does not install selector")
    require("bind_source_character_combat_natives_v115" in OWNER,
            "character session binding entry point disappeared")

    # IDA order and the game's argument/result behavior: PlaySound can stop
    # music before an unknown name returns; an unknown sound otherwise returns
    # before consuming the remaining arguments or calling Vox.
    require(binding.index("stop_campaign_music_v117(world,0") < binding.index("campaign_sound_index_v115(world,name"),
            "PlaySound stop-music flag must run before sound-name lookup")
    require("if(id<0){if(error&&size)error[0]=0;return 0;}" in binding,
            "unknown source sound names must preserve native early no-op")
    require("stop_campaign_sound_v106(world,id,fade" in binding,
            "StopSound must reach same-campaign SoundManager stop helper")
    require("play_campaign_music_v101(world,id,true,false,fade" in binding,
            "PlayMusic must preserve native loop/stop/fade arguments")
    require("play_campaign_plain_sound_v115(world,id,enabled,fade,0,false" in binding,
            "PlaySound must preserve native plain-sound arguments")
    require("set_campaign_music_state_v101" in binding,
            "PlayMusic must retain native current-level music-state update")

    # This actual authored AI initializes during session loading and later
    # reaches every adjacent audio global on its normal callbacks.
    require(re.search(r'StopSound\("SwampKingMusic",\s*2000\)', SCRIPT),
            "SwampKing InitVars no longer exercises the reported StopSound path")
    require(re.search(r'PlayMusic\("SwampKingMusic",\s*500\)', SCRIPT),
            "SwampKing music callback route is no longer covered")
    require(re.search(r'PlaySound\("sfx_serpent_submerge",\s*false,\s*0,\s*false\)', SCRIPT),
            "SwampKing sound callback route is no longer covered")
    print("PASS: Stage17 SwampKing session selects source-faithful StopSound/PlayMusic/PlaySound providers")


if __name__ == "__main__":
    main()
