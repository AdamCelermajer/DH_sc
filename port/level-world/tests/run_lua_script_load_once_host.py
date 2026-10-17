#!/usr/bin/env python3
"""Build and run the focused per-VM LuaScript path-cache host test."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]
RUNTIME = REPO / "port" / "adam-script-runtime"
LUA = RUNTIME / "lua"
COMMONS = REPO / "recovered" / "scripts" / "original" / "data" / "scripts" / "skills" / "_commons.luac"
COMMONS_SHA256 = "f3abf69124728bab3d344b56e8f3cb67127fbea4a8d1a7ab1d7401f2a2132e9d"
COMMONS_SIZE = 11291
ORIGINAL_SHA256 = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
ORIGINAL_FUNCTIONS = ROOT / "reference" / "lua-script-load-once" / "original-functions.json"
LUA_SOURCES = [
    "lapi", "lcode", "ldebug", "ldo", "ldump", "lfunc", "lgc", "llex",
    "lmem", "lobject", "lopcodes", "lparser", "lstate", "lstring",
    "ltable", "ltm", "lundump", "lvm", "lzio", "lauxlib", "lbaselib",
    "ltablib", "lstrlib", "lmathlib",
]


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(command: list[str], *, cwd: Path, env: dict[str, str]) -> str:
    completed = subprocess.run(command, cwd=cwd, env=env, text=True,
                               stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if completed.returncode:
        raise RuntimeError(
            f"command failed ({completed.returncode}): {' '.join(command)}\n{completed.stdout}")
    return completed.stdout


def verify_original_elf(path: Path) -> tuple[str, list[dict[str, object]]]:
    from elftools.elf.elffile import ELFFile

    if digest(path) != ORIGINAL_SHA256:
        raise ValueError("pinned original ELF SHA does not match the manifest")
    manifest = json.loads(ORIGINAL_FUNCTIONS.read_text(encoding="utf-8"))
    if manifest.get("original_sha256") != ORIGINAL_SHA256:
        raise ValueError("original-function manifest ELF identity changed")
    verified = []
    with path.open("rb") as stream:
        elf = ELFFile(stream)
        if elf.elfclass != 32 or not elf.little_endian or elf["e_machine"] != "EM_ARM":
            raise ValueError("expected little-endian ELF32 ARM original library")
        symbols = {}
        symtab = elf.get_section_by_name(".symtab")
        if symtab is None:
            raise ValueError("pinned ELF lacks .symtab")
        expected_names = {entry["original_symbol"] for entry in manifest["functions"]}
        for symbol in symtab.iter_symbols():
            if symbol.name in expected_names:
                if symbol.name in symbols:
                    raise ValueError(f"duplicate source symbol: {symbol.name}")
                symbols[symbol.name] = symbol
        loads = [segment for segment in elf.iter_segments()
                 if segment["p_type"] == "PT_LOAD"]
        for expected in manifest["functions"]:
            name = expected["original_symbol"]
            symbol = symbols.get(name)
            address = int(expected["elf_address"], 16)
            size = int(expected["size"])
            if symbol is None or (int(symbol["st_value"]), int(symbol["st_size"])) != \
                    (address, size):
                raise ValueError(f"original symbol identity mismatch: {name}")
            segment = next((candidate for candidate in loads
                            if int(candidate["p_vaddr"]) <= address and
                            address + size <= int(candidate["p_vaddr"]) +
                            int(candidate["p_filesz"])), None)
            if segment is None:
                raise ValueError(f"original function body is not file-backed: {name}")
            offset = int(segment["p_offset"]) + address - int(segment["p_vaddr"])
            stream.seek(offset)
            body = stream.read(size)
            body_hash = hashlib.sha256(body).hexdigest()
            if len(body) != size or body_hash != expected["sha256"]:
                raise ValueError(f"original function body hash mismatch: {name}")
            verified.append({"original_symbol": name, "address": expected["elf_address"],
                             "size": size, "sha256": body_hash})
    return ORIGINAL_SHA256, verified


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--compiler-dir", type=Path,
                        default=Path("C:/Users/noamc/.local/mingw/mingw64/bin"))
    parser.add_argument("--output", type=Path,
                        default=ROOT / "build" / "lua-script-load-once-host")
    parser.add_argument("--original-elf", type=Path,
                        default=REPO.parent / "test_strategy" / "libDungeonHunter2.so")
    args = parser.parse_args()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    gcc = args.compiler_dir / "gcc.exe"
    gxx = args.compiler_dir / "g++.exe"
    if not gcc.is_file() or not gxx.is_file():
        raise FileNotFoundError(f"expected gcc.exe and g++.exe under {args.compiler_dir}")
    if not COMMONS.is_file():
        raise FileNotFoundError(f"recovered skills/_commons is missing: {COMMONS}")
    commons_hash = digest(COMMONS)
    if (COMMONS.stat().st_size, commons_hash) != (COMMONS_SIZE, COMMONS_SHA256):
        raise ValueError("skills/_commons bytes do not match the pinned recovered input")
    original_hash, original_functions = verify_original_elf(args.original_elf)

    cflags = ["-std=c99", "-O1", "-fno-fast-math", "-ffp-contract=off",
              "-I", str(RUNTIME), "-I", str(LUA)]
    c_sources = [RUNTIME / "script_runtime.c"] + [LUA / f"{unit}.c" for unit in LUA_SOURCES]
    c_objects: list[Path] = []
    compile_commands: list[list[str]] = []
    env = os.environ.copy()
    env["PATH"] = str(args.compiler_dir.resolve()) + os.pathsep + env.get("PATH", "")
    for source in c_sources:
        obj = output / (source.stem + "-" + hashlib.sha256(str(source).encode()).hexdigest()[:8] + ".o")
        command = [str(gcc), *cflags, "-c", str(source), "-o", str(obj)]
        run(command, cwd=REPO, env=env)
        compile_commands.append(command)
        c_objects.append(obj)

    executable = output / "lua_script_load_once_host.exe"
    command = [
        str(gxx), "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic",
        "-fno-fast-math", "-ffp-contract=off", "-I", str(RUNTIME),
        str(ROOT / "lua_script_load_once.cpp"),
        str(ROOT / "tests" / "lua_script_load_once.cpp"),
        *(str(path) for path in c_objects), "-lm", "-o", str(executable),
    ]
    run(command, cwd=REPO, env=env)
    test_output = run([str(executable), str(COMMONS)], cwd=REPO, env=env)
    result_line = next((line for line in reversed(test_output.splitlines())
                        if line.startswith("{")), None)
    if result_line is None:
        raise RuntimeError(f"host test did not emit structured result:\n{test_output}")
    result = json.loads(result_line)
    if result.get("validation") != "PASS" or result.get("mismatches") != 0:
        raise RuntimeError(f"host test did not pass: {result}")

    source_paths = [ROOT / "lua_script_load_once.hpp",
                    ROOT / "lua_script_load_once.cpp",
                    ROOT / "tests" / "lua_script_load_once.cpp",
                    Path(__file__).resolve(), ORIGINAL_FUNCTIONS,
                    ROOT / "reference" / "lua-script-load-once" / "NOTES.md"]
    runtime_paths = c_sources + [RUNTIME / "script_runtime.h"] + \
        sorted(LUA.glob("*.h"))
    toolchain = {
        "gcc": run([str(gcc), "--version"], cwd=REPO, env=env).splitlines()[0],
        "g++": run([str(gxx), "--version"], cwd=REPO, env=env).splitlines()[0],
    }
    report = {
        "validation": "PASS",
        "scope": "per-LuaScript exact resolved-path execution set only; shared LuaManager byte cache and full AddFile body are outside this helper",
        "original_skills_commons": {
            "path": str(COMMONS), "bytes": COMMONS_SIZE, "sha256": commons_hash,
        },
        "original_elf": {"sha256": original_hash, "functions_verified": original_functions},
        "result": result,
        "source_sha256": {str(path.relative_to(REPO)): digest(path) for path in source_paths},
        "lua_runtime_sources": [str(path.relative_to(REPO)) for path in c_sources],
        "lua_runtime_inputs_sha256": {
            str(path.relative_to(REPO)): digest(path) for path in runtime_paths
        },
        "toolchain": toolchain,
        "compile_command_count": len(compile_commands) + 1,
        "executable": str(executable),
        "output": test_output,
    }
    report_path = output / "validation.json"
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "checks": result["checks"],
                      "report": str(report_path), "executable": str(executable)}, indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as error:
        print(f"ERROR: {error}", file=sys.stderr)
        raise SystemExit(1)
