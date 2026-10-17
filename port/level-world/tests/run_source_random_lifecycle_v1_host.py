#!/usr/bin/env python3
"""Verify original RNG lifecycle pins and run the selected-library host audit."""
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
MANIFEST = MODULE / "reference/source-random-lifecycle-v1/original-functions.json"


def require(condition: bool, message: str) -> None:
    if not condition:
        raise RuntimeError(message)


def run(command: list[str], *, cwd: Path = ROOT, env=None) -> str:
    result = subprocess.run(command, cwd=cwd, env=env, capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(
            f"command failed ({result.returncode}): {command}\n{result.stdout}\n{result.stderr}"
        )
    return result.stdout


def verify_original(path: Path, document: dict) -> list[dict]:
    from elftools.elf.elffile import ELFFile

    raw = path.read_bytes()
    require(hashlib.sha256(raw).hexdigest() == document["original_sha256"],
            "original library SHA256 does not match the checked-in pin")
    verified = []
    with path.open("rb") as stream:
        elf = ELFFile(stream)
        require(elf["e_machine"] == "EM_ARM" and elf.elfclass == 32,
                "original ELF must be ARM32")
        symtab = elf.get_section_by_name(".symtab")
        require(symtab is not None, "original ELF has no full symbol table")
        symbols = {symbol.name: symbol for symbol in symtab.iter_symbols()}
        loads = [segment for segment in elf.iter_segments() if segment["p_type"] == "PT_LOAD"]
        for row in document["functions"]:
            symbol = symbols.get(row["original_symbol"])
            require(symbol is not None, f"missing original symbol: {row['original_symbol']}")
            address, size = int(row["elf_address"], 0), row["size"]
            require((symbol["st_value"], symbol["st_size"]) == (address, size),
                    f"symbol range changed: {row['original_symbol']}")
            segment = next((item for item in loads
                            if item["p_vaddr"] <= address and
                            address + size <= item["p_vaddr"] + item["p_filesz"]), None)
            require(segment is not None, f"function is not file-backed: {row['original_symbol']}")
            offset = int(segment["p_offset"]) + address - int(segment["p_vaddr"])
            actual = hashlib.sha256(raw[offset:offset + size]).hexdigest()
            require(actual == row["sha256"], f"function bytes changed: {row['original_symbol']}")
            verified.append({"symbol": row["original_symbol"], "address": row["elf_address"],
                             "size": size, "sha256": actual})
    return verified


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--original-elf", type=Path,
                        default=Path(r"C:\tmp\dh2-original-libDungeonHunter2.so"))
    parser.add_argument("--compiler", default="g++")
    parser.add_argument("--c-compiler", default="gcc")
    parser.add_argument("--cmake", default="cmake")
    parser.add_argument("--output-dir", type=Path,
                        default=MODULE / "build/source-random-lifecycle-v1-selected-host")
    parser.add_argument("--jobs", type=int, default=8)
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()

    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    original = args.original_elf.resolve()
    require(original.is_file(), f"original ELF not found: {original}")
    pinned = verify_original(original, manifest)

    cmake = shutil.which(args.cmake)
    cxx = shutil.which(args.compiler)
    cc = shutil.which(args.c_compiler)
    require(cmake is not None, f"CMake not found: {args.cmake}")
    require(cxx is not None, f"C++ compiler not found: {args.compiler}")
    require(cc is not None, f"C compiler not found: {args.c_compiler}")

    output = args.output_dir.resolve()
    wrapper = output / "wrapper"
    build = output / "selected"
    wrapper.mkdir(parents=True, exist_ok=True)
    module_cmake_path = MODULE.as_posix()
    wrapper_cmake = f"""cmake_minimum_required(VERSION 3.22)
project(source_random_lifecycle_v1_selected_host LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory(\"{module_cmake_path}\" selected-world)
"""
    (wrapper / "CMakeLists.txt").write_text(wrapper_cmake, encoding="utf-8")

    run([cmake, "-S", str(wrapper), "-B", str(build), "-G", "Ninja",
         f"-DCMAKE_C_COMPILER={cc}", f"-DCMAKE_CXX_COMPILER={cxx}",
         "-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"], cwd=ROOT)
    build_output = run([cmake, "--build", str(build), "--target",
                        "source_random_lifecycle_v1_audit", "--parallel", str(args.jobs)], cwd=ROOT)

    compile_commands_path = build / "compile_commands.json"
    compile_commands = json.loads(compile_commands_path.read_text(encoding="utf-8"))
    source_path = (MODULE / "source_random_lifecycle_v1.cpp").resolve()
    selected_entries = [entry for entry in compile_commands
                        if Path(entry["file"]).resolve() == source_path]
    require(len(selected_entries) == 1,
            f"lifecycle source must compile exactly once in the selected library; saw {len(selected_entries)}")

    executable = build / "selected-world/source_random_lifecycle_v1_audit.exe"
    require(executable.is_file(), f"host audit executable not produced: {executable}")
    selected_library_dir = build / "selected-world"
    env = os.environ.copy()
    runtime_dirs = {selected_library_dir, Path(cxx).resolve().parent}
    runtime_dirs.update(path.parent for path in build.rglob("*.dll"))
    env["PATH"] = os.pathsep.join(
        [*(str(path) for path in runtime_dirs), env.get("PATH", "")]
    )
    test_output = run([str(executable)], env=env)

    report = {
        "status": "passed",
        "original_elf": str(original),
        "original_elf_sha256": manifest["original_sha256"],
        "verified_original_functions": pinned,
        "selected_target": "dh2_level_world",
        "selected_module_compilation_count": len(selected_entries),
        "selected_test_executable": str(executable),
        "test_output": test_output.strip(),
        "scope": "selected host-library lifecycle owner only; not Android runtime or live gameplay lifecycle wiring",
    }
    report_path = args.report.resolve() if args.report else output / "validation.json"
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(test_output.strip())
    print(f"original-function pins verified: {len(pinned)}")
    print(f"selected dh2_level_world compile count: {len(selected_entries)}")
    print(f"validation report: {report_path}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, RuntimeError) as error:
        raise SystemExit(f"ERROR: {error}") from error
