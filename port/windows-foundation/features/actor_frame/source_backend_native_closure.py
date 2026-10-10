import json
import pathlib
import subprocess
import sys


ROOT = pathlib.Path.cwd()
OWN = ROOT / "port/windows-foundation/features/actor_frame"
OUT = OWN / "source_backend_native_closure_build"
OUT.mkdir(exist_ok=True)
COMPILER = ROOT / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe"

FLAGS = [
    "-std=c++20", "-Dfinite=_finite", "-include", "exception",
    "-Wall", "-Wextra", "-Werror", "-Wno-unused-value",
    "-ffunction-sections", "-fdata-sections", "-flto=thin",
]
for path in (ROOT / "port").iterdir():
    if path.is_dir():
        FLAGS.append("-I" + str(path))
FLAGS += [
    "-I" + str(ROOT / "port/android-native/app/src/main/cpp"),
    "-I" + str(ROOT / "port/engine-audio/integration-v42"),
    "-I" + str(ROOT / ".local-inputs/runtime-test-20261009/windows-include"),
    "-isystem", str(ROOT / "port/physics-backend/box2d-2.0.1/Include"),
]

# Actual source definitions for the provider TU's existing undefined symbols.
# These are ordinary source owners, not substitutes or behavior stubs.
UNITS = [
    "port/level-world/character_world_target_frame_v2.cpp",
    "port/level-world/character_target_update.cpp",
    "port/level-world/character_ai_frame.cpp",
    "port/level-world/source_character_aggro_v84.cpp",
    "port/level-world/character_skill_aggro_v6.cpp",
    "port/level-world/character_stance.cpp",
    "port/level-world/character_native_effects.cpp",
    "port/level-world/character_knockback_reaction_v1.cpp",
    "port/level-world/character_slow_reaction_v1.cpp",
    "port/level-world/player_injure_state_v7.cpp",
    "port/level-world/player_injury_runtime_v7.cpp",
    "port/level-world/character_defensive_state_v1.cpp",
    "port/level-world/character_cancel_sneaking.cpp",
    "port/level-world/character_idle_update.cpp",
    "port/level-world/character_skill_target_binding_v46.cpp",
    "port/level-world/character_skill_state_v4.cpp",
    "port/level-world/character_skill_application_v6.cpp",
    "port/android-native/app/src/main/cpp/faery_cast_state_v2.cpp",
    "port/level-world/character_animation_event_owner_v1.cpp",
    "port/level-world/character_mesh_fx_owner_v1.cpp",
    "port/level-world/character_melee_animation_event_v1.cpp",
    "port/level-world/npc_animation_event_owner_v1.cpp",
    "port/level-world/character_world_attack_geometry_v1.cpp",
    "port/level-world/character_world_ai_can_attack_v1.cpp",
    "port/level-world/character_world_skill_combat_v6.cpp",
    "port/level-world/character_close_range_v38.cpp",
    "port/level-world/character_world_player_attack_owner_v1.cpp",
    "port/level-world/target_frontal_sort_v41.cpp",
    "port/level-world/npc_attack_command_owner_v1.cpp",
    "port/level-world/character_target_search.cpp",
    "port/level-world/character_target_search_v5.cpp",
    "port/level-world/world_click_target_v1.cpp",
    "port/level-world/character_update_aggro_v108.cpp",
    "port/game-data/aggro.cpp",
    "port/level-world/character_heading_owner_v1.cpp",
    "port/level-world/character_head_object_v2.cpp",
    "port/level-world/canonical_gameobject_graph_v68.cpp",
    "port/level-world/retained_scene_visual_connection_v3.cpp",
    "port/level-world/character_combat_result_v6.cpp",
    "port/level-world/character_skill_combat_v6.cpp",
    "port/level-world/character_hit_v6.cpp",
    "port/level-world/character_skill_attack_v6.cpp",
    "port/level-world/character_collision_lifecycle_v1.cpp",
    "port/engine-audio/audio_source_bindings_v38.cpp",
    "port/engine-audio/audio_campaign_bridge_v46.cpp",
    "port/engine-audio/audio_listener_rows_v38.cpp",
    "port/engine-audio/audio_clock_v40.cpp",
    "port/engine-audio/audio_source_command_v40.cpp",
    "port/engine-audio/audio_gameplay_runtime_v42.cpp",
    "port/engine-audio/audio_gameplay_runtime_v40.cpp",
    "port/engine-audio/audio_catalog_v34.cpp",
    "port/engine-audio/audio_bank_v34.cpp",
    "port/engine-audio/audio_mixer_v34.cpp",
    "port/engine-audio/audio_native_envelope_v34.cpp",
    "port/engine-audio/audio_sample_v34.cpp",
    "port/engine-audio/audio_spatial_v34.cpp",
    "port/engine-audio/vox_source_fields_v38.cpp",
    "port/level-world/vox_music_state_owner_v1.cpp",
    "port/level-world/generic_lua_script_owner_v13.cpp",
    "port/level-world/level_savegame_owner_v1.cpp",
    "port/level-world/level_savegame_runtime_v1.cpp",
    "port/engine-audio/integration-v40/focus/audio_lifecycle_gate_v40.cpp",
    "port/engine-audio/integration-v42/audio_application_manager_v42.cpp",
    "port/engine-audio/integration-v42/audio_native_session_v42.cpp",
    "port/windows-foundation/features/audio/windows_source_session_control_v1.cpp",
    "port/windows-foundation/features/audio/winmm_output.cpp",
    "port/windows-foundation/features/audio/feature_audio.cpp",
    "port/level-loader/native_level_application_v25.cpp",
    "port/level-loader/level_constructor_bindings_v4.cpp",
    "port/level-loader/native_gslevel_runtime_v27.cpp",
]


def run(args):
    return subprocess.run(args, capture_output=True, text=True)


def main():
    results = {}
    objects = []
    for relative in UNITS:
        source = ROOT / relative
        obj = OUT / (relative.replace("/", "_").replace(".", "_") + ".o")
        proc = run([str(COMPILER), *FLAGS, "-c", str(source), "-o", str(obj)])
        results[relative] = {"exit": proc.returncode, "stderr": proc.stderr}
        if proc.returncode:
            print("COMPILE FAIL", relative, proc.stderr, file=sys.stderr)
            continue
        objects.append(obj)

    required = [
        OWN / "source_backend_native_closure_test.cpp",
        ROOT / "port/android-native/app/src/main/cpp/source_campaign_character_fsm_v101.cpp",
        OWN / "source_campaign_backend_v1.cpp",
        OWN / "source_current_level_backend_v1.cpp",
    ]
    for source in required:
        obj = OUT / (source.stem + ".o")
        proc = run([str(COMPILER), *FLAGS, "-c", str(source), "-o", str(obj)])
        results[str(source.relative_to(ROOT))] = {"exit": proc.returncode, "stderr": proc.stderr}
        if proc.returncode:
            print("COMPILE FAIL", source, proc.stderr, file=sys.stderr)
        else:
            objects.append(obj)

    archives = [
        OWN / "source_character_owner_factory_native_build/native.a",
        ROOT / ".local-inputs/windows-foundation-build/libfoundation_data.a",
        ROOT / ".local-inputs/windows-foundation-build/librecovered_content.a",
        ROOT / ".local-inputs/windows-foundation-build/libcontent_xml.a",
    ]
    link = run([
        str(COMPILER), *map(str, objects), *map(str, archives),
        str(archives[0]), "-Wl,--gc-sections", "-Wl,--error-limit=0",
        "-lopengl32", "-luser32", "-lgdi32", "-lwinmm", "-o",
        str(OUT / "source_backend_native_closure_test.exe"),
    ])
    results["native_link"] = {"exit": link.returncode, "stderr": link.stderr}
    (OUT / "link-errors.txt").write_text(link.stderr)
    (OUT / "build.json").write_text(json.dumps(results, indent=2))
    if link.returncode:
        print("LINK FAIL", link.stderr[:8000], file=sys.stderr)
    else:
        print("LINK PASS", OUT / "source_backend_native_closure_test.exe")
    return 1 if any(value["exit"] for value in results.values()) else 0


if __name__ == "__main__":
    raise SystemExit(main())
