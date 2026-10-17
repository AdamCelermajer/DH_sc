"""Compile and run the Character factory audit against the real CMake targets."""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

LEVEL_WORLD = Path(__file__).resolve().parents[1]
REPOSITORY = LEVEL_WORLD.parents[1]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cmake", default=shutil.which("cmake"))
    parser.add_argument("--ninja", default=shutil.which("ninja"))
    parser.add_argument("--compiler", default=os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++"))
    parser.add_argument("--c-compiler", default=os.environ.get("CC") or shutil.which("gcc") or shutil.which("clang"))
    parser.add_argument("--build-dir", type=Path,
        default=LEVEL_WORLD / ".checks" / "character-runtime-factory-v1-host")
    args = parser.parse_args()
    if not all((args.cmake, args.ninja, args.compiler, args.c_compiler)):
        parser.error("focused factory audit requires CMake, Ninja, a C++ compiler, and a C compiler")

    test = LEVEL_WORLD / "tests" / "character_runtime_factory_v1.cpp"
    player_test = LEVEL_WORLD / "tests" / "native_player_character_owner_v1.cpp"
    cache = REPOSITORY / "port" / "android-native" / "app" / "src" / "main" / "assets" / "data"
    native_character_list = REPOSITORY / "port" / "android-native" / "app" / "src" / "main" / "cpp" / "native_character_list.cpp"
    wrapper = args.build_dir.resolve() / "wrapper"
    build = args.build_dir.resolve() / "selected"
    wrapper.mkdir(parents=True, exist_ok=True)
    (wrapper / "CMakeLists.txt").write_text("\n".join((
            "cmake_minimum_required(VERSION 3.22)",
            "project(character_runtime_factory_v1_host LANGUAGES C CXX)",
            "set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)",
            f'add_subdirectory("{LEVEL_WORLD.as_posix()}" selected-world)',
            f'add_executable(character_runtime_factory_v1_host "{test.as_posix()}" "{native_character_list.as_posix()}")',
            "target_link_libraries(character_runtime_factory_v1_host PRIVATE dh2_level_world dh2_script_runtime)",
            "target_compile_features(character_runtime_factory_v1_host PRIVATE cxx_std_17)",
            "target_compile_options(character_runtime_factory_v1_host PRIVATE -UNDEBUG -Wall -Wextra -Werror)",
            f'add_executable(native_player_character_owner_v1_host "{player_test.as_posix()}")',
            "target_link_libraries(native_player_character_owner_v1_host PRIVATE dh2_level_world dh2_script_runtime)",
            "target_compile_features(native_player_character_owner_v1_host PRIVATE cxx_std_17)",
            "target_compile_options(native_player_character_owner_v1_host PRIVATE -UNDEBUG -Wall -Wextra -Werror)",
            "",
    )), encoding="utf-8")
    subprocess.run([args.cmake, "-S", str(wrapper), "-B", str(build), "-G", "Ninja",
                        f"-DCMAKE_MAKE_PROGRAM={args.ninja}",
                        f"-DCMAKE_C_COMPILER={args.c_compiler}",
                        f"-DCMAKE_CXX_COMPILER={args.compiler}",
                    "-DCMAKE_BUILD_TYPE=Release"], check=True)
    subprocess.run([args.cmake, "--build", str(build), "--target",
                    "character_runtime_factory_v1_host",
                    "native_player_character_owner_v1_host"], check=True)
    executable_names = ["character_runtime_factory_v1_host",
                        "native_player_character_owner_v1_host"]
    runtime_dirs = [build / "selected-world",
        build / "selected-world" / "adam-script-runtime",
        build / "selected-world" / "game-data",
        build / "selected-world" / "engine-skinning",
        build / "selected-world" / "engine-skinning" / "engine-animation",
        build / "selected-world" / "engine-skinning" / "engine-animation" / "scene-materials",
        Path(args.compiler).resolve().parent]
    env = os.environ.copy()
    env["PATH"] = os.pathsep.join(str(path) for path in runtime_dirs) + os.pathsep + env.get("PATH", "")
    for executable_name in executable_names:
        executable = build / (executable_name + ".exe" if os.name == "nt"
                              else executable_name)
        command = [str(executable)]
        if executable_name == "native_player_character_owner_v1_host":
            command.append(str(cache))
        run = subprocess.run(command, text=True, capture_output=True, env=env)
        if run.stdout:
            sys.stdout.write(run.stdout)
        if run.stderr:
            sys.stderr.write(run.stderr)
        if run.returncode:
            return run.returncode
        if executable_name == "character_runtime_factory_v1_host":
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
                "native_roster_adapter": True,
                "dynamic_script_npc_same_owner": True,
                "script_summon_order": True}
            if report != expected:
                raise SystemExit(f"unexpected factory regression result: {report}")
        elif not json.loads(run.stdout).get("validation") == "PASS":
            raise SystemExit(f"Player Character owner audit did not report PASS: {run.stdout}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
