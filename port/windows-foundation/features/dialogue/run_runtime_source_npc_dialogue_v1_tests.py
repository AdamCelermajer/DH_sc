import json
import hashlib
import os
import pathlib
import subprocess
import sys

root = pathlib.Path.cwd()
feature = root / "port/windows-foundation/features/dialogue"
build = feature / "runtime_source_npc_dialogue_v1_build"
build.mkdir(exist_ok=True)
toolchain = root / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin"
compiler = toolchain / "clang++.exe"
sources = [
    feature / "runtime_source_npc_dialogue_v1.cpp",
    feature / "runtime_source_npc_dialogue_v1_tests.cpp",
    root / "port/windows-foundation/features/quests/character_quest_progress_v1.cpp",
    root / "port/windows-foundation/features/quests/quest_text_resolver_v1.cpp",
    root / "port/game-data/quest_persistence_v51.cpp",
    root / "port/game-data/quest_savegame_v1.cpp",
    root / "port/game-data/data.cpp",
    root / "port/script-runtime/script_constants.cpp",
    root / "port/engine-ui/hud_text_v1.cpp",
    root / "port/engine-ui/hud_text_format_v1.cpp",
    root / "port/engine-ui/localization_parse_ex_v1.cpp",
    root / "port/engine-ui/localization.cpp",
]
includes = [
    root / "port/game-data", root / "port/level-world", root / "port/engine-ui",
    root / "port/script-runtime", root / "port/windows-foundation",
]
exe = build / "runtime_source_npc_dialogue_v1_tests.exe"
command = [str(compiler), "-std=c++17", "-O1", "-static", "-ffunction-sections",
           "-fdata-sections", *("-I" + str(path) for path in includes),
           *(str(path) for path in sources), "-Wl,--gc-sections", "-o", str(exe)]
result = subprocess.run(command, cwd=root, text=True, capture_output=True)
report = {
    "purpose": "Verify the Act 1 Crypt TalkToNPC target against the actual CharacterTable and separately verify the localized StringID. The test must not join a target to a speaker without an authored route.",
    "implementation_claim": "source table mapping gate plus isolated caller-supplied text projection; no production caller or in-game dialogue admission is claimed",
    "source_route": {
        "quest": "Crypt02_explore (decoded source Quest act=1)",
        "talk_objective_type": "v2QuestObjectiveType.TalkToNPC (actual constant 5)",
        "npc_identity": 348,
        "dialogue_symbol": "GRAVEYARD_CRYPT_INTRO_CELESTE_1",
        "dialogue_string_id": 393217,
        "route_join_note": "The Quest target is a CharacterProperties/CharacterTable key, not a scene actor instance ID: the original Character.Interact payload copies CharProps13c8 into QE_TalkToNPC.id18, and the source CharacterTable key 348 is SisterEllen. GRAVEYARD_CRYPT_INTRO_CELESTE_1 resolves to a localized StringID only. No script command joins that key to this line; the fixture rejects a Celeste attribution.",
        "target_property_row": "CharacterTable row 348 = SisterEllen; no Celeste row is present",
        "identity_join_accepted": False,
        "gate": "An actual Crypt02 scene actor declaration with its CharacterProperties key plus its source script/dialogue command selecting StringID 393217 is required. The available Crypt01 MGP provenance contains no Celeste actor and is not a complete Crypt02 instance mapping.",
    },
    "visual_evidence": {
        "reference": ".local-inputs/referenceframes/dh2-act1/at-0120s.png and at-0960s.png from the existing v1.0.3 reference-video receipt",
        "observation": "At 120 s, caged priest dialogue shows nameplate RENE and line text; at 960 s, close-up dialogue again labels RENE. These frames establish the user-visible dialogue layout (speaker name and localized line together), but do not depict the tested Crypt/Celeste route.",
        "inference_limit": "The inspected footage is v1.0.3; decoded source/cache is v1.0.2. No Crypt-specific gameplay frame was inspected in this bounded task.",
    },
    "logic_evidence": [
        "port/android-native/app/src/main/cpp/source_campaign_character_interaction_v114.cpp:52-60 resolves the actual TalkToNPC constant and reads the same Character TalkToNPC2fa gate; the full Character.Interact caller preserves state!=13 and current-Level / quest-event ordering.",
        "source_campaign_character_interaction_v114.cpp:47-50 copies the retained Character CharProps13c8 into the QE_TalkToNPC id18 field; game_event_runtime_v75.cpp:147-152 uses the authored TalkToNPC target for the Character2fa marker scan. This establishes the target namespace as the CharacterProperties key, not an actor-instance ID or a dialogue selector.",
        "port/level-loader/game_event_runtime_v75.cpp:147-152 preserves the source TalkToNPC compiled-object and first-matching Character talk-flag gates; it does not select dialogue text.",
        "port/windows-foundation/reports/encounter-source-scripts.json: Camp_Intro command indices 12-15 are StartDialog/WaitDialog in source order; common_text_pycst maps string IDs 2031649-2031653 to the SWAMP camp-intro speaker rows.",
        "port/windows-foundation/features/quests/character_quest_progress_v1.cpp query validates the same CharacterState owner and source row; source_quest_log_services_v1 uses the same active state range 6..12 and HudText integer StringID provider.",
    ],
    "expected_behavior": "For a reached TalkToNPC response route, keep the accepted NPC identity, exact same Character quest owner/state, and source-localized dialogue string together; do not mutate quest state or invent fallback dialogue.",
    "uncertainties": [
        "The decoded main Quest table has no Dialog-type objective rows and only four TalkToNPC objective rows; the TalkToNPC event/objective path carries the authored CharacterTable key but does not select a particular dialogue row.",
        "No integrated production caller or live initialized source Character/Session was exercised.",
        "The available Crypt01 MGP/object provenance includes a WanderingPriest Character and monster rows, not a Celeste actor; it is not the complete Crypt02 actor table. The required Crypt02 MGP actor record and script StartDialog/StringID route remain missing.",
    ],
    "verification": {"compile_exit": result.returncode, "compile_stderr": result.stderr},
}
provenance_path = root / "port/android-native/app/src/main/assets/worlds/crypt01-provenance.json"
provenance_bytes = provenance_path.read_bytes()
provenance = json.loads(provenance_bytes)
character_objects = [obj for obj in provenance["objects"]
                     if obj.get("gametype") == "Character"]
celeste_declarations = [obj for obj in character_objects
                        if "celeste" in str(obj.get("name", "")).lower() or
                        "celeste" in str(obj.get("charpropsname", "")).lower()]
exact_actor_key_links = [obj for obj in character_objects
                         if obj.get("actor_id", obj.get("source_id", obj.get("id"))) == 348 and
                         "celeste" in str(obj.get("charpropsname", "")).lower()]
if (provenance.get("layout") != "x07_crypt_backup.mlx" or
        len(provenance.get("rooms", [])) != 8 or len(provenance.get("objects", [])) != 166 or
        provenance.get("procedural_rules_executed") is not False):
    raise SystemExit("Crypt01 MGP provenance inventory changed; re-audit its source scope")
if celeste_declarations or exact_actor_key_links:
    raise SystemExit("Crypt01 MGP inventory acquired a candidate Celeste link; require explicit script route before accepting")
report["source_scene_inventory"] = {
    "path": str(provenance_path.relative_to(root)).replace("\\", "/"),
    "sha256": hashlib.sha256(provenance_bytes).hexdigest(),
    "layout": provenance["layout"],
    "rooms": len(provenance["rooms"]),
    "objects": len(provenance["objects"]),
    "character_objects": len(character_objects),
    "celeste_character_declarations": len(celeste_declarations),
    "actor_id_348_to_celeste_declarations": len(exact_actor_key_links),
    "procedural_rules_executed": provenance["procedural_rules_executed"],
    "scope_note": "This authentic Crypt01 MGP-derived inventory is incomplete for Crypt02 and provides no actor/script route linking target 348 to StringID 393217.",
}
report["source_route"]["identity_join_accepted"] = False
report["source_route"]["crypt01_scene_actor_mapping_present"] = bool(exact_actor_key_links)
if result.returncode == 0:
    env = dict(os.environ)
    env["PATH"] = str(toolchain) + os.pathsep + env.get("PATH", "")
    run = subprocess.run([
        str(exe),
        str(root / "port/level-world/reference/character-menu-profile-v51/cache"),
        str(root / "port/android-native/app/src/main/assets/original-cache/data"),
        str(root / "port/android-native/app/src/main/assets"),
    ], cwd=root, text=True, capture_output=True, env=env)
    report["verification"].update({
        "runtime_exit": run.returncode,
        "runtime_stdout": run.stdout,
        "runtime_stderr": run.stderr,
    })
else:
    report["verification"]["runtime_exit"] = None
(feature / "runtime_source_npc_dialogue_v1_report.json").write_text(json.dumps(report, indent=2) + "\n")
print(json.dumps(report["verification"], indent=2))
raise SystemExit(result.returncode if result.returncode else report["verification"]["runtime_exit"])
