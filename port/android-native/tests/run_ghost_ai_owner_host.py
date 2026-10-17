"""Build and run the bounded native Ghost owner composition on the host."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]
LEVEL = ROOT / "port/level-world"
RUNTIME = ROOT / "port/adam-script-runtime"
LUA = RUNTIME / "lua"
CORE = "lapi lcode ldebug ldo ldump lfunc lgc llex lmem lobject lopcodes lparser lstate lstring ltable ltm lundump lvm lzio lauxlib lbaselib ltablib lstrlib lmathlib".split()
CPP = [
    RUNTIME / "script_function_alias.cpp",
    ROOT / "port/game-data/ai.cpp",
    LEVEL / "character_aggro_delay.cpp", LEVEL / "character_ai_turn.cpp",
    LEVEL / "character_aggro_target_search.cpp",
    LEVEL / "character_aggro_character_list.cpp",
    LEVEL / "character_aggro_object_manager_list.cpp",
    LEVEL / "character_aggro_candidate_events.cpp",
    LEVEL / "character_ai_relations.cpp", LEVEL / "character_enemy_spotted.cpp",
    LEVEL / "character_oid_cache_v1.cpp",
    LEVEL / "character_ai_events.cpp", LEVEL / "character_ai_set_target.cpp",
    LEVEL / "character_controller_commands.cpp", LEVEL / "character_path_commands.cpp",
    LEVEL / "navigation_heading.cpp", LEVEL / "character_ai_sight.cpp",
    LEVEL / "character_ai_in_combat.cpp", LEVEL / "character_aggro_acquisition_prefix.cpp",
    LEVEL / "monster_external_script_session.cpp", LEVEL / "ghost_ai_session.cpp",
    LEVEL / "lua_script_load_once.cpp",
    LEVEL / "ais_native_bindings.cpp",
    LEVEL / "character_ai_update_target.cpp", LEVEL / "character_ai_master_update.cpp",
    LEVEL / "character_ai_frame.cpp", LEVEL / "ais_external_update.cpp",
    LEVEL / "character_ai_update.cpp",
    LEVEL / "character_script_lifecycle.cpp",
    LEVEL / "ais_state_callbacks.cpp",
    LEVEL / "character_monster_retarget.cpp", LEVEL / "character_enemy_retention.cpp",
    LEVEL / "character_target_search.cpp",
    ROOT / "port/android-native/app/src/main/cpp/ghost_ai_owner.cpp",
    ROOT / "port/android-native/app/src/main/cpp/native_character_list.cpp",
    ROOT / "port/android-native/tests/ghost_ai_owner_host.cpp",
]


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++ compiler")
    parser.add_argument("--c-compiler", help="matching C compiler")
    parser.add_argument("--output", type=Path, default=ROOT / "port/android-native/build/ghost-ai-owner/host")
    parser.add_argument("--report", type=Path, default=ROOT / "port/android-native/build/ghost-ai-owner/validation.json")
    args = parser.parse_args()
    cxx = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not cxx:
        parser.error("pass --compiler or set CXX")
    cc = args.c_compiler or str(Path(cxx).with_name(Path(cxx).name.replace("g++", "gcc").replace("clang++", "clang")))
    out = args.output.resolve()
    if os.name == "nt" and not out.suffix:
        out = out.with_suffix(".exe")
    out.parent.mkdir(parents=True, exist_ok=True)
    objdir = out.parent / "objects"
    objdir.mkdir(exist_ok=True)
    c_sources = [LUA / f"{name}.c" for name in CORE] + [
        RUNTIME / "script_runtime.c", ROOT / "port/lua-numeric/numeric.c",
        ROOT / "port/random/random.c",
    ]
    objects: list[Path] = []
    commands: list[list[str]] = []
    warnings: list[dict[str, str]] = []
    for source in c_sources + CPP:
        obj = objdir / f"{source.parent.name}-{source.stem}.o"
        compiler = cxx if source.suffix == ".cpp" else cc
        cmd = [compiler, "-std=c++17" if source.suffix == ".cpp" else "-std=c99",
               "-O1", "-fno-fast-math", "-ffp-contract=off", "-Wall",
               "-I", str(LUA), "-I", str(LEVEL)]
        if source.suffix == ".cpp" and source != RUNTIME / "script_function_alias.cpp":
            cmd += ["-Wextra", "-Werror", "-pedantic"]
        cmd += ["-c", str(source), "-o", str(obj)]
        result = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
        if result.returncode:
            sys.stderr.write(result.stderr)
            return result.returncode
        if result.stderr:
            warnings.append({"source": source.relative_to(ROOT).as_posix(), "diagnostics": result.stderr})
        commands.append(cmd)
        objects.append(obj)
    link = [cxx, *(str(obj) for obj in objects), "-lm", "-o", str(out)]
    result = subprocess.run(link, cwd=ROOT, capture_output=True, text=True)
    if result.returncode:
        sys.stderr.write(result.stderr)
        return result.returncode
    if result.stderr:
        warnings.append({"source": "link", "diagnostics": result.stderr})
    scripts = ROOT / "recovered/scripts/original/data/scripts/ai"
    common, monster = scripts / "_commons.luac", scripts / "monster.luac"
    expected = {
        "_commons.luac": "20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c",
        "monster.luac": "84f07caaeb2c04f2024cc3e27d41f33806b2c53d6e8861bb3d8b371118fd0e1d",
    }
    for source in (common, monster):
        if sha(source) != expected[source.name]:
            raise RuntimeError(f"original script changed: {source}")
    run = subprocess.run([str(out), str(common), str(monster)], cwd=ROOT, capture_output=True, text=True)
    sys.stdout.write(run.stdout); sys.stderr.write(run.stderr)
    if run.returncode:
        return run.returncode
    host = json.loads(run.stdout)
    assert host["ghost_ai_owner_host_cases"] == 9 and host["existing_target_cases"] == 6 and host["mismatches"] == 0, host
    assert host["combat_vm_dispatch"] is True, host
    assert host["pending_vm_shared"] is True, host
    assert host["constructor_before_pending_cases"] == 3, host
    assert host["manager_cursor_owner_cases"] == 2 and host["manager_cursor_live_links"] is True, host
    assert host["flat_character_owner_cases"] == 4 and host["flat_published_vm_shared"] is True, host
    assert host["status"] == "PASS" and host["candidates"] == 1 and host["events"] == 1, host
    assert host["script_callbacks"] == host["set_target_calls"] == host["head_to_calls"] == host["path_count"] == 1, host
    assert host["ais_update_calls"] == 2, host
    inputs = c_sources + CPP + [common, monster, Path(__file__).resolve(),
        LEVEL / "tests/ghost_ai_session.cpp", LEVEL / "ghost_ai_session.hpp",
        LEVEL / "character_script_lifecycle.hpp", LEVEL / "monster_external_script_session.hpp",
        LEVEL / "lua_script_load_once.hpp",
        LEVEL / "ais_state_callbacks.hpp",
        LEVEL / "character_aggro_character_list.hpp",
        LEVEL / "character_aggro_object_manager_list.hpp",
        ROOT / "port/android-native/app/src/main/cpp/ghost_ai_owner.hpp",
        ROOT / "port/android-native/app/src/main/cpp/native_character_list.hpp"]
    report = {
        "validation": "PASS", "host": host, "commands": commands + [link],
        "source_sha256": {p.relative_to(ROOT).as_posix(): sha(p) for p in inputs},
        "original_script_sha256": {p.relative_to(ROOT).as_posix(): sha(p) for p in (common, monster)},
        "executable_sha256": sha(out), "compiler_warnings": warnings,
        "scope": "Host composition executes CharAI frame dispatch, normal acquisition, event/Lua/SetTarget/HeadTo/PathTo, complete CharAI OnUpdate/AISExternal with null-state wrappers, and existing-target retarget/retention including captured-owner mutation and missing-provider refusal. Three staged cases create the actual VM before pending publication, install prepared callbacks before binding/loading and adopt that same VM for source acquisition/path dispatch. Retarget/retention component bodies retain their independent original ARM proofs; this host adapter is new composition. Renderer bindings, Android body motion, collision-persist production and full gameplay are not claimed.",
    }
    report_path = args.report.resolve()
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf8")
    print("report:", report_path)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
