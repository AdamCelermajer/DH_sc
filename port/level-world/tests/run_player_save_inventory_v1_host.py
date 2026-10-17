"""Build the GEAR writer test against the selected level-world library."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess

TESTS = Path(__file__).resolve().parent
MODULE = TESTS.parent
ROOT = MODULE.parents[1]
LEVEL_WORLD = ROOT / "port/level-world"


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(args, *, cwd: Path = ROOT, env=None) -> str:
    result = subprocess.run(list(map(str, args)), cwd=cwd, env=env, text=True,
                            capture_output=True)
    if result.returncode:
        raise RuntimeError(f"{args}\n{result.stdout}\n{result.stderr}")
    return result.stdout


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", type=Path,
                        default=Path(shutil.which("g++") or "g++"))
    parser.add_argument("--output-dir", type=Path,
                        default=ROOT / "tmp/player-save-inventory-v1-host")
    args = parser.parse_args()

    compiler = Path(shutil.which(str(args.compiler)) or args.compiler).resolve()
    cc = compiler.with_name(compiler.name.replace("g++", "gcc"))
    objdump = compiler.with_name(compiler.name.replace("g++", "objdump"))
    cmake = shutil.which("cmake")
    ninja = shutil.which("ninja")
    if not cmake or not ninja:
        raise RuntimeError("CMake and Ninja are required for the selected target gate")

    output = args.output_dir.resolve()
    wrapper = output / "wrapper"
    build = output / "selected"
    wrapper.mkdir(parents=True, exist_ok=True)
    cmake_text = f'''cmake_minimum_required(VERSION 3.22)
project(player_save_inventory_v1_host LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_subdirectory("{LEVEL_WORLD.as_posix()}" selected-level-world)
add_executable(player_save_inventory_v1_host
  "{(TESTS / 'player_save_inventory_v1.cpp').as_posix()}")
target_compile_features(player_save_inventory_v1_host PRIVATE cxx_std_17)
target_compile_options(player_save_inventory_v1_host PRIVATE
  -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
target_link_libraries(player_save_inventory_v1_host PRIVATE dh2_level_world)
'''
    (wrapper / "CMakeLists.txt").write_text(cmake_text, encoding="utf-8")

    cmake_args = [cmake, "-S", wrapper, "-B", build, "-G", "Ninja",
                  f"-DCMAKE_MAKE_PROGRAM={Path(ninja).as_posix()}",
                  f"-DCMAKE_CXX_COMPILER={compiler.as_posix()}",
                  f"-DCMAKE_C_COMPILER={cc.as_posix()}",
                  "-DCMAKE_BUILD_TYPE=Release",
                  "-DCMAKE_CXX_FLAGS_RELEASE=-O1",
                  "-DCMAKE_C_FLAGS_RELEASE=-O1"]
    run(cmake_args)
    commands = run([ninja, "-C", build, "-t", "commands",
                    "player_save_inventory_v1_host"])
    compile_db = json.loads((build / "compile_commands.json").read_text(
        encoding="utf-8"))
    compiled = {str((Path(row["directory"]) / row["file"]).resolve())
                for row in compile_db}
    expected_sources = {
        str((TESTS / "player_save_inventory_v1.cpp").resolve()),
    }
    selected_writer = str((MODULE / "player_save_inventory_v1.cpp").resolve())
    selected_game = str((MODULE / "world.cpp").resolve())
    if (not expected_sources.issubset(compiled) or
            selected_writer not in compiled or selected_game not in compiled):
        raise RuntimeError("writer/test or production sources are absent from selected compile database")

    evidence = [MODULE / "player_save_inventory_v1.hpp",
                MODULE / "player_save_inventory_v1.cpp",
                TESTS / "player_save_inventory_v1.cpp",
                Path(__file__).resolve(), LEVEL_WORLD / "CMakeLists.txt"]
    before = {str(path.relative_to(ROOT).as_posix()): sha(path)
              for path in evidence}
    run([cmake, "--build", build, "--target",
         "player_save_inventory_v1_host", "--parallel", "2"])

    exe = build / "player_save_inventory_v1_host.exe"
    libraries = list(build.rglob("libdh2_level_world.dll"))
    runtime_libraries = list(build.rglob("*.dll"))
    if not exe.is_file() or len(libraries) != 1:
        raise RuntimeError("selected host executable or single level-world DLL is missing")
    imports = re.findall(r"DLL Name: (\S+)", run([objdump, "-p", exe]))
    if "libdh2_level_world.dll" not in imports:
        raise RuntimeError(f"host test did not import selected level-world DLL: {imports}")

    env = os.environ.copy()
    env["PATH"] = os.pathsep.join(
        [str(compiler.parent),
         *(str(path.parent) for path in runtime_libraries),
         env.get("PATH", "")])
    host = json.loads(run([exe], env=env))
    if host.get("validation") != "PASS":
        raise RuntimeError(f"focused writer test failed: {host}")
    after = {str(path.relative_to(ROOT).as_posix()): sha(path)
             for path in evidence}
    if before != after:
        raise RuntimeError("writer/test/selected CMake input changed during the host gate")

    print(json.dumps({
        "validation": "PASS",
        "selected_target": "dh2_level_world",
        "selected_dll_import": "libdh2_level_world.dll",
        "host": host,
        "source_sha256": before,
        "source_hashes_stable": True,
        "compile_sources": sorted(
            Path(path).relative_to(ROOT).as_posix()
            for path in expected_sources | {selected_writer, selected_game}),
        "selected_command_count": len(commands.splitlines()),
    }))


if __name__ == "__main__":
    main()
