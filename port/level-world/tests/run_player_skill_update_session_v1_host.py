#!/usr/bin/env python3
"""Build the focused Player skill-update test against the selected CMake library."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(command: list[str], cwd: Path, env=None) -> str:
    result = subprocess.run(command, cwd=cwd, env=env, capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(f"command failed ({result.returncode}): {command}\n{result.stdout}\n{result.stderr}")
    return result.stdout


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", required=True, type=Path)
    parser.add_argument("--cache", required=True, type=Path)
    parser.add_argument("--output", type=Path,
                        default=Path(tempfile.gettempdir()) / "dh2-player-skill-update-session-v1-selected")
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build/player-skill-update-session-v1-selected/validation.json")
    args = parser.parse_args()
    out = args.output.resolve()
    cache = args.cache.resolve()
    wrapper = out / "wrapper"
    build = out / "selected"
    wrapper.mkdir(parents=True, exist_ok=True)
    build.mkdir(parents=True, exist_ok=True)
    compiler = args.compiler.resolve()
    c_compiler = compiler.with_name(compiler.name.replace("g++", "gcc"))
    cmake = shutil.which("cmake")
    ninja = shutil.which("ninja")
    if not cmake or not ninja or not cache.is_dir():
        raise SystemExit("CMake, Ninja and an existing cache root are required")

    test = MODULE / "tests/player_skill_update_session_v1.cpp"
    backend = ROOT / "port/android-native/app/src/main/cpp/native_debug_files.cpp"
    names = ROOT / "port/pydata-names/names.c"
    wrapper_text = f'''cmake_minimum_required(VERSION 3.22)
project(player_skill_update_selected_audit LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(player_skill_update_session_audit
  "{test.as_posix()}" "{backend.as_posix()}" "{names.as_posix()}")
target_compile_features(player_skill_update_session_audit PRIVATE cxx_std_17)
target_compile_options(player_skill_update_session_audit PRIVATE
  -Wall -Wextra -Werror -pedantic -fno-fast-math -ffp-contract=off)
target_link_libraries(player_skill_update_session_audit PRIVATE
  dh2_level_world dh2_script_runtime)
'''
    (wrapper / "CMakeLists.txt").write_text(wrapper_text, encoding="utf-8")
    commands = []
    configure = [cmake, "-S", str(wrapper), "-B", str(build), "-G", "Ninja",
                 f"-DCMAKE_MAKE_PROGRAM={ninja}", f"-DCMAKE_CXX_COMPILER={compiler}",
                 f"-DCMAKE_C_COMPILER={c_compiler}", "-DCMAKE_BUILD_TYPE=Release",
                 "-DCMAKE_CXX_FLAGS_RELEASE=-O1", "-DCMAKE_C_FLAGS_RELEASE=-O1"]
    commands.append(configure)
    configure_out = run(configure, ROOT)
    selected_commands = run([ninja, "-C", str(build), "-t", "commands",
                             "player_skill_update_session_audit", "player_savegame_v1_audit",
                             "player_skill_property_services_v1_audit"], ROOT)
    compile_db_path = build / "compile_commands.json"
    compile_db = json.loads(compile_db_path.read_text(encoding="utf-8"))
    selected_sources = {
        Path(entry["file"]).resolve() for entry in compile_db
        if str(entry.get("file", "")).replace("\\", "/") in selected_commands.replace("\\", "/")
    }
    required = {
        MODULE / "player_skill_session_v1.cpp",
        MODULE / "player_skill_update_session_v1.cpp",
        MODULE / "character_ai_update_all_skills.cpp",
        MODULE / "character_ai_update_skills.cpp",
        MODULE / "character_ai_skill_script_update.cpp",
        MODULE / "player_skill_property_services_v1.cpp",
        MODULE / "tests/player_skill_property_services_v1.cpp",
        ROOT / "port/game-data/player_savegame_v1.cpp",
        ROOT / "port/game-data/tests/player_savegame_v1.cpp",
        test,
    }
    missing = [path for path in required if path.resolve() not in selected_sources]
    if missing:
        raise RuntimeError(f"selected CMake gate omitted production source(s): {missing}")

    selected_inputs = sorted(selected_sources | {test.resolve(), backend.resolve(), names.resolve()})
    for folder in {path.parent for path in selected_inputs}:
        selected_inputs.extend(path.resolve() for pattern in ("*.h", "*.hpp") for path in folder.glob(pattern))
        cmake_list = folder / "CMakeLists.txt"
        if cmake_list.is_file():
            selected_inputs.append(cmake_list.resolve())
    selected_inputs = sorted(set(selected_inputs))
    before = {path: digest(path) for path in selected_inputs if path.is_file()}
    build_command = [cmake, "--build", str(build), "--target",
                     "player_skill_update_session_audit", "player_savegame_v1_audit",
                     "player_skill_property_services_v1_audit", "--parallel", "2"]
    commands.append(build_command)
    build_out = run(build_command, ROOT)
    executable = build / "player_skill_update_session_audit.exe"
    saved_executable = build / "selected-world/game-data/player_savegame_v1_audit.exe"
    property_executable = build / "selected-world/player_skill_property_services_v1_audit.exe"
    libraries = sorted(build.rglob("*.dll"))
    world = next((p for p in libraries if p.name == "libdh2_level_world.dll"), None)
    script_runtime = next((p for p in libraries if p.name == "libdh2_script_runtime.dll"), None)
    if not executable.is_file() or world is None or script_runtime is None:
        raise RuntimeError("selected target did not produce the test executable and both production DSOs")
    if sum(p.name == script_runtime.name for p in libraries) != 1:
        raise RuntimeError("selected build contains more than one Lua core DSO")
    objdump = compiler.with_name("objdump.exe")
    imports = {}
    for binary in [executable, world, saved_executable, property_executable]:
        output = run([str(objdump), "-p", str(binary)], ROOT)
        imports[binary.name] = [line.split(":", 1)[1].strip()
                               for line in output.splitlines() if "DLL Name:" in line]
    if script_runtime.name not in imports[executable.name] or script_runtime.name not in imports[world.name]:
        raise RuntimeError("selected test/world do not share the same Lua runtime DSO")
    if "libdh2_game_data.dll" not in imports[saved_executable.name] or world.name not in imports[property_executable.name]:
        raise RuntimeError("saved-skill/property tests do not import selected production DSOs")

    env = os.environ.copy()
    env["PATH"] = os.pathsep.join([str(compiler.parent), *(str(p.parent) for p in libraries),
                                   env.get("PATH", "")])
    temp = Path(tempfile.mkdtemp(prefix="dh2-player-skill-update-"))
    execution = run([str(executable), str(cache), str(temp)], ROOT, env=env)
    host = json.loads(execution)
    expected = {
        "validation": "PASS", "source_slots": 21, "fsm_skips": 2,
        "cross_character_guard": True,
        "zero_return_updates": 13, "one_return_updates": 13,
        "multi_return_updates": 13, "multi_return_set_skill_updates": 13,
        "one_return_set_skill_updates": 13, "ordinary_set_skill_errors": 13,
        "ordinary_update_errors": 13, "required_failures": 1,
        "selected_faery_updates": 1, "selected_required_failures": 1,
        "retained_failed_resources": 2, "real_cache_preparation": True,
        "native_player_wiring": False,
    }
    if any(host.get(key) != value for key, value in expected.items()):
        raise RuntimeError(f"unexpected selected-library host result: {execution}")
    gold = ROOT / "port/game-data/reference/player-savegame-v1/fixtures.bin"
    saved = json.loads(run([str(saved_executable), str(gold), str(cache / "data/pydata")], ROOT, env=env))
    if saved.get("validation") != "PASS" or saved.get("source_gold_cases") != 64 or saved.get("mismatches") != 0:
        raise RuntimeError(f"selected saved-skill replay failed: {saved}")
    property_output = run([str(property_executable), str(cache / "data/pydata")], ROOT, env=env)
    if "PASS checks=29" not in property_output or "actual_bashdown_data=PASS" not in property_output:
        raise RuntimeError(f"selected property regression failed: {property_output}")
    changed = [str(path) for path, value in before.items() if digest(path) != value]
    if changed:
        raise RuntimeError(f"selected production/test source changed during build or replay: {changed}")

    pydata = cache / "data/pydata"
    cache_inputs = [
        pydata / f"{name}.bin" for name in (
            "skills_pyarray", "skills_pyarraynames", "skills_pystructnames",
            "faeries_pyarray", "faeries_pyarraynames", "faeries_pystructnames",
            "character_properties_pyarray", "character_properties_pyarraynames",
            "character_properties_pystructnames", "character_classes_pyarray",
            "character_classes_pyarraynames", "character_classes_pystructnames",
            "effects_pyarraynames", "projectiles_pyarraynames", "faeries_pycst")
    ] + [cache / "DebugSwitches.savegame",
         cache / "data/scripts/ai/_commons.luac",
         cache / "data/scripts/skills/_commons.luac"]
    if any(not path.is_file() for path in cache_inputs):
        raise RuntimeError("required selected cache fixture is missing")
    selected_inputs = sorted(set(selected_inputs) | set(before))
    report_path = args.report.resolve()
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report = {
        **host,
        "selected_saved_skills": saved,
        "selected_property_output": property_output,
        "saved_gold_sha256": digest(gold),
        "cache_root": str(cache),
        "cache_inputs_sha256": {path.relative_to(cache).as_posix(): digest(path) for path in cache_inputs},
        "selected_source_sha256": {path.relative_to(ROOT).as_posix(): digest(path)
                                   for path in selected_inputs if path.is_relative_to(ROOT)},
        "compile_commands_sha256": digest(compile_db_path),
        "wrapper_sha256": digest(wrapper / "CMakeLists.txt"),
        "selected_source_count": len(selected_sources),
        "production_linkage": {
            "world_library": str(world.relative_to(out).as_posix()),
            "script_runtime_library": str(script_runtime.relative_to(out).as_posix()),
            "executable_imports": imports[executable.name],
            "world_imports": imports[world.name],
            "saved_test_imports": imports[saved_executable.name],
            "property_test_imports": imports[property_executable.name],
        },
        "compiler_commands": commands,
        "configure_output": configure_out,
        "build_output": build_out,
        "test_executable_sha256": digest(executable),
        "saved_test_executable_sha256": digest(saved_executable),
        "property_test_executable_sha256": digest(property_executable),
        "scope": "Actual selected dh2_game_data/dh2_level_world and single dh2_script_runtime CMake libraries; original saved-skill gold, actual Bashdown property data, actual Knight source tables/Arguments and AI common, test skill Lua overlays. No Android Player skill wiring claim.",
    }
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(host, separators=(",", ":")))


if __name__ == "__main__":
    main()
