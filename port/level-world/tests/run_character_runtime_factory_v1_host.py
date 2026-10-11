"""Compile and run the bounded native Character factory regression."""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

LEVEL_WORLD = Path(__file__).resolve().parents[1]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++17 host compiler")
    parser.add_argument("--output", type=Path,
        default=LEVEL_WORLD / "build" / "character_runtime_factory_v1_host.exe")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("no C++ host compiler found; pass --compiler or set CXX")
    output = args.output.resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    include_dirs = [LEVEL_WORLD, LEVEL_WORLD.parent / "game-data",
        LEVEL_WORLD.parent / "engine-skinning",
        LEVEL_WORLD.parent / "engine-animation",
        LEVEL_WORLD.parent / "scene-materials",
        LEVEL_WORLD.parent / "adam-script-runtime",
        LEVEL_WORLD.parent / "player-info-level",
        LEVEL_WORLD.parent / "physics-backend" / "box2d-2.0.1" / "Include"]
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
        *[arg for path in include_dirs for arg in ("-I", str(path))],
        str(LEVEL_WORLD / "tests" / "character_runtime_factory_v1.cpp"),
        str(LEVEL_WORLD / "character_runtime_factory_v1.cpp"),
        str(LEVEL_WORLD / "character_faery_controller_v1.cpp"),
        str(LEVEL_WORLD / "character_controller_commands.cpp"),
        str(LEVEL_WORLD / "character_faery_script_session_v1.cpp"),
        str(LEVEL_WORLD / "character_gameplay_save_v1.cpp"),
        str(LEVEL_WORLD / "character_constructor_owner_v1.cpp"),
        str(LEVEL_WORLD / "character_kill_field_14e4_v1.cpp"),
        str(LEVEL_WORLD / "character_kill_death_tail_v1.cpp"),
        str(LEVEL_WORLD / "object_update_culling.cpp"),
        str(LEVEL_WORLD / "character_net_state_owner_v1.cpp"),
        str(LEVEL_WORLD / "character_coordinator.cpp"),
        str(LEVEL_WORLD / "character_state.cpp"),
        str(LEVEL_WORLD / "character_timers.cpp"),
        str(LEVEL_WORLD / "character_skill_state_dispatch_v1.cpp"),
        str(LEVEL_WORLD / "character_skill_fsm_callbacks_v1.cpp"),
        str(LEVEL_WORLD / "character_cast_state_dispatch_v1.cpp"),
        str(LEVEL_WORLD / "character_cast_lifecycle_v1.cpp"),
        str(LEVEL_WORLD / "character_ai_frame.cpp"),
        str(LEVEL_WORLD / "character_init_post_nonplayer_v1.cpp"),
        str(LEVEL_WORLD / "character_script_lifecycle.cpp"),
        str(LEVEL_WORLD / ".." / "game-data" / "ai.cpp"),
        str(LEVEL_WORLD / "object_manager_runtime_owner_v1.cpp"),
        str(LEVEL_WORLD / "character_aggro_object_manager_list.cpp"),
        str(LEVEL_WORLD / ".." / "game-data" / "player_save_load_owner_v1.cpp"),
        str(LEVEL_WORLD / ".." / "game-data" / "player_profile_index_v1.cpp"),
        str(LEVEL_WORLD / ".." / "game-data" / "player_save_level_states_v1.cpp"),
        str(LEVEL_WORLD / ".." / "game-data" / "player_savegame_v1.cpp"),
        str(LEVEL_WORLD / ".." / "android-native" / "app" / "src" / "main" / "cpp" / "native_character_list.cpp"),
        "-o", str(output)]
    built = subprocess.run(command, text=True, capture_output=True)
    if built.stdout:
        sys.stdout.write(built.stdout)
    if built.stderr:
        sys.stderr.write(built.stderr)
    if built.returncode:
        return built.returncode
    run = subprocess.run([str(output)], text=True, capture_output=True)
    if run.stdout:
        sys.stdout.write(run.stdout)
    if run.stderr:
        sys.stderr.write(run.stderr)
    if run.returncode:
        return run.returncode
    report = json.loads(run.stdout)
    expected = {"stable_owner": True, "constructor_order": True,
        "kill_field_14e4_constructor_projection": True,
        "source_item_inventory_constructor": True,
        "net_state_pair": True, "canonical_timer_fsm_owner": True,
        "nonplayer_init_post_prefix": True,
        "nonplayer_ai_postload_branch": True,
        "nonplayer_init_post_fx_animation_sound_chain": True,
        "script_created_character_flag": True,
        "script_created_kill_objective_gate": True,
        "init_final_gated": True,
        "source_save_slot_null_and_player_mask2": True,
        "registered_states": 20,
        "manager_roster_publication": True,
        "constructor_and_roster_rollback": True, "retirement": True,
        "staged_lifecycle_order": True,
        "premature_stage_rejection": True,
        "lifecycle_failure_retry": True,
        "lifecycle_idempotence": True, "faery_session_retirement_gate": True,
        "native_roster_adapter": True}
    if report != expected:
        raise SystemExit(f"unexpected regression result: {report}")
    print(json.dumps({"validation": "PASS", "host_report": report}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
