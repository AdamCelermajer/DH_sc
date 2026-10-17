"""Build a host-only source search-to-Ghost script/controller composition test."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
RUNTIME = ROOT / "port/adam-script-runtime"
CORE = "lapi lcode ldebug ldo ldump lfunc lgc llex lmem lobject lopcodes lparser lstate lstring ltable ltm lundump lvm lzio lauxlib lbaselib ltablib lstrlib lmathlib".split()


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++ compiler")
    parser.add_argument("--c-compiler", help="matching C compiler")
    parser.add_argument("--output", type=Path,
                        default=MODULE / "build/ghost-ai-session/host")
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build/ghost-ai-session/validation.json")
    args = parser.parse_args()
    cxx = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not cxx:
        parser.error("pass --compiler or set CXX")
    cc = args.c_compiler or str(Path(cxx).with_name(
        Path(cxx).name.replace("g++", "gcc").replace("clang++", "clang")))
    out = args.output.resolve()
    if os.name == "nt" and not out.suffix:
        out = out.with_suffix(".exe")
    out.parent.mkdir(parents=True, exist_ok=True)
    objects_dir = out.parent / "objects"
    objects_dir.mkdir(exist_ok=True)

    c_sources = [RUNTIME / "lua" / f"{name}.c" for name in CORE] + [
        RUNTIME / "script_runtime.c", ROOT / "port/lua-numeric/numeric.c"]
    cpp_sources = [
        RUNTIME / "script_function_alias.cpp",
        ROOT / "port/game-data/ai.cpp",
        MODULE / "ais_native_bindings.cpp",
        MODULE / "character_aggro_target_search.cpp",
        MODULE / "character_aggro_character_list.cpp",
        MODULE / "character_aggro_candidate_events.cpp",
        MODULE / "character_ai_relations.cpp",
        MODULE / "character_enemy_spotted.cpp",
        MODULE / "character_ai_events.cpp",
        MODULE / "character_ai_set_target.cpp",
        MODULE / "character_controller_commands.cpp",
        MODULE / "character_path_commands.cpp",
        MODULE / "character_script_lifecycle.cpp",
        MODULE / "navigation_heading.cpp",
        MODULE / "character_oid_cache_v1.cpp",
        MODULE / "monster_external_script_session.cpp",
        MODULE / "lua_script_load_once.cpp",
        MODULE / "ais_state_callbacks.cpp",
        MODULE / "ghost_ai_session.cpp",
        MODULE / "tests/ghost_ai_session.cpp",
    ]
    objects: list[Path] = []
    commands: list[list[str]] = []
    warnings: list[dict[str, str]] = []
    for source in c_sources + cpp_sources:
        obj = objects_dir / f"{source.parent.name}-{source.stem}.o"
        compiler = cxx if source.suffix == ".cpp" else cc
        command = [compiler,
                   "-std=c++17" if source.suffix == ".cpp" else "-std=c99",
                   "-O1", "-fno-fast-math", "-ffp-contract=off",
                   "-I", str(RUNTIME / "lua"), "-Wall"]
        if source in cpp_sources[1:]:
            command += ["-Wextra", "-Werror", "-pedantic"]
        command += ["-c", str(source), "-o", str(obj)]
        result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
        if result.returncode:
            sys.stderr.write(result.stderr)
            return result.returncode
        if result.stderr:
            warnings.append({"source": source.relative_to(ROOT).as_posix(),
                             "diagnostics": result.stderr})
        commands.append(command)
        objects.append(obj)

    link = [cxx, *(str(obj) for obj in objects), "-lm", "-o", str(out)]
    linked = subprocess.run(link, cwd=ROOT, capture_output=True, text=True)
    commands.append(link)
    if linked.returncode:
        sys.stderr.write(linked.stderr)
        return linked.returncode
    if linked.stderr:
        warnings.append({"source": "link", "diagnostics": linked.stderr})

    scripts = ROOT / "recovered/scripts/original/data/scripts/ai"
    common, monster = scripts / "_commons.luac", scripts / "monster.luac"
    expected = {
        "_commons.luac": "20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c",
        "monster.luac": "84f07caaeb2c04f2024cc3e27d41f33806b2c53d6e8861bb3d8b371118fd0e1d",
    }
    for path in (common, monster):
        if digest(path) != expected[path.name]:
            raise RuntimeError(f"source Lua changed: {path}")
    tested = subprocess.run([str(out), str(common), str(monster)], cwd=ROOT,
                            capture_output=True, text=True)
    sys.stdout.write(tested.stdout)
    sys.stderr.write(tested.stderr)
    if tested.returncode:
        return tested.returncode
    host = json.loads(tested.stdout)
    assert host["ghost_ai_session_cases"] == 19 and host["mismatches"] == 0, host
    for key in ("source_search_to_path", "per_actor_vm_and_target_identity",
                "stale_owner_rebind", "reentrant_rebind_guard", "partial_failure_effects",
                "fresh_empty_search_event_12", "fresh_all_false_relation_event_12",
                "output_alias_guard",
                "staged_vm_adopted_without_duplicate",
                "source_pending_publication", "pending_replacement_guard", "pending_owner_guard",
                "lifecycle_output_alias_guard",
                "unbuilt_updateaggro_prefix"):
        assert host[key] is True, (key, host)
    assert host["flat_character_route_cases"] == 6 and host["source_flat_character_to_path"] is True
    assert host["native_wired"] is False

    dependencies = cpp_sources + c_sources + [
        MODULE / "ghost_ai_session.hpp",
        MODULE / "character_aggro_character_list.hpp",
        MODULE / "character_script_lifecycle.hpp",
        MODULE / "character_ai_set_target.hpp",
        MODULE / "character_ai_relations.hpp",
        MODULE / "character_enemy_spotted.hpp",
        MODULE / "character_aggro_candidate_events.hpp",
        MODULE / "character_controller_commands.hpp",
        MODULE / "character_path_commands.hpp",
        MODULE / "monster_external_script_session.hpp",
        MODULE / "lua_script_load_once.hpp",
        MODULE / "ais_state_callbacks.hpp",
        MODULE / "ais_native_bindings.hpp",
        Path(__file__).resolve(), common, monster,
    ] + list((RUNTIME / "lua").glob("*.h"))
    report = {
        "validation": "PASS",
        "host_report": host,
        "compiler_commands": commands,
        "compiled_source_sha256": {
            p.relative_to(ROOT).as_posix(): digest(p) for p in dependencies
        },
        "unchanged_source_lua_sha256": {
            p.relative_to(ROOT).as_posix(): digest(p) for p in (common, monster)
        },
        "executable_sha256": digest(out),
        "compiler_warnings": warnings,
        "native_wired": False,
        "scope": "Actor-owned host composition of filter2 aggro search, source relation query, candidate event consumption, AI event/OnEnemySpotted gates, unchanged external monster Lua, AI_SetTarget, and controller Character PathTo. Caller supplies the selected inner-branch radius and source-owned providers; full _UpdateAggro prefix/native owner integration is not claimed.",
    }
    report_path = args.report.resolve()
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf8")
    print("report:", report_path)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
