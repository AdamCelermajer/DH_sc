"""Verify the created-VM callback ownership bridge with real source-built Lua.

This is a port ownership contract, not a reconstructed original ARM body.
The unchanged original commons/monster scripts execute on one actual VM.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
RUNTIME = ROOT / "port/adam-script-runtime"
CORE = "lapi lcode ldebug ldo ldump lfunc lgc llex lmem lobject lopcodes lparser lstate lstring ltable ltm lundump lvm lzio lauxlib lbaselib ltablib lstrlib lmathlib".split()


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(command: list[str]) -> subprocess.CompletedProcess:
    result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(result.stdout + result.stderr)
    return result


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler")
    parser.add_argument("--c-compiler")
    parser.add_argument("--output", type=Path, default=MODULE / "build/monster-created-service-install/host.exe")
    parser.add_argument("--report", type=Path, default=MODULE / "build/monster-created-service-install/validation.json")
    args = parser.parse_args()
    cxx = args.compiler or os.environ.get("CXX") or shutil.which("g++")
    if not cxx:
        parser.error("pass --compiler or set CXX")
    cc = args.c_compiler or str(Path(cxx).with_name("gcc.exe" if Path(cxx).suffix == ".exe" else "gcc"))
    exe = args.output.resolve()
    exe.parent.mkdir(parents=True, exist_ok=True)
    objects_dir = exe.parent / "objects"
    objects_dir.mkdir(exist_ok=True)
    c_sources = [RUNTIME / "lua" / (name + ".c") for name in CORE] + [
        RUNTIME / "script_runtime.c", ROOT / "port/lua-numeric/numeric.c"]
    cpp_sources = [RUNTIME / "script_function_alias.cpp"] + [
        MODULE / (name + ".cpp") for name in (
            "ais_native_bindings", "ais_external_init_callbacks", "ais_state_callbacks",
            "lua_script_load_once", "character_oid_cache_v1", "monster_external_script_session")
    ] + [MODULE / "tests/monster_created_service_install.cpp"]
    commands, objects, warnings = [], [], []
    for path in c_sources + cpp_sources:
        obj = objects_dir / (path.parent.name + "-" + path.stem + ".o")
        strict = path in cpp_sources[1:]
        command = [cxx if path.suffix == ".cpp" else cc,
                   "-std=c++17" if path.suffix == ".cpp" else "-std=c99",
                   "-O1", "-fno-fast-math", "-ffp-contract=off", "-Wall",
                   "-I", str(RUNTIME / "lua")]
        if strict:
            command += ["-Wextra", "-Werror", "-pedantic"]
        command += ["-c", str(path), "-o", str(obj)]
        result = run(command)
        if result.stderr:
            warnings.append({"path": path.relative_to(ROOT).as_posix(), "diagnostics": result.stderr})
        commands.append(command)
        objects.append(obj)
    command = [cxx, *map(str, objects), "-lm", "-o", str(exe)]
    run(command)
    commands.append(command)
    scripts = ROOT / "recovered/scripts/original/data/scripts/ai"
    common, monster = scripts / "_commons.luac", scripts / "monster.luac"
    assert digest(common) == "20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c"
    assert digest(monster) == "84f07caaeb2c04f2024cc3e27d41f33806b2c53d6e8861bb3d8b371118fd0e1d"
    host = json.loads(run([str(exe), str(common), str(monster)]).stdout)
    assert host == {"validation": "PASS", "bridge_cases": 7, "guard_cases": 11,
                    "retirement_cases": 3, "same_actual_vm": True,
                    "original_scripts_used": True, "native_wired": False,
                    "new_original_bodies": 0, "mismatches": 0}, host
    inputs = c_sources + cpp_sources + [
        MODULE / "monster_external_script_session.hpp", MODULE / "tests/monster_external_script_session.cpp",
        MODULE / "ais_native_bindings.hpp", MODULE / "ais_external_init_callbacks.hpp",
        MODULE / "ais_state_callbacks.hpp", MODULE / "lua_script_load_once.hpp",
        MODULE / "character_oid_cache_v1.hpp",
        RUNTIME / "script_runtime.h", RUNTIME / "script_function_alias.h",
        ROOT / "port/lua-numeric/numeric.h", Path(__file__).resolve(),
        *sorted((RUNTIME / "lua").glob("*.h")), common, monster]
    report = {"validation": "PASS", "host_report": host, "compiler_commands": commands,
              "source_sha256": {path.relative_to(ROOT).as_posix(): digest(path) for path in inputs},
              "original_script_sha256": {path.relative_to(ROOT).as_posix(): digest(path) for path in (common, monster)},
              "executable_sha256": digest(exe), "dependency_warnings": warnings,
              "scope": "Port ownership bridge only. Actual VM identity, original Lua dispatch, stage/owner/lifetime guards, preserved aliases/cache and install/reset/destructor reentry. Native frame activation and new original ARM body credit are not claimed."}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf8")
    print(json.dumps({"validation": "PASS", "host_report": host, "report": str(args.report)}, indent=2))


if __name__ == "__main__":
    main()
