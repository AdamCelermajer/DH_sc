"""Build and run the canonical GEAR writer against selected level-world."""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess

TESTS = Path(__file__).resolve().parent
MODULE = TESTS.parent
ROOT = MODULE.parents[1]


def run(args, cwd=ROOT, env=None):
    result = subprocess.run(list(map(str, args)), cwd=cwd, env=env,
                            text=True, capture_output=True)
    if result.returncode:
        raise RuntimeError(f"{args}\n{result.stdout}\n{result.stderr}")
    return result.stdout


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path,
                        default=ROOT / ".local-inputs/ui-inventory-cache")
    parser.add_argument("--output-dir", type=Path,
                        default=ROOT / "tmp/player-gear-save-writer-v1-host")
    args = parser.parse_args()
    compiler = Path(shutil.which("g++") or "g++").resolve()
    cc = compiler.with_name(compiler.name.replace("g++", "gcc"))
    cmake, ninja = shutil.which("cmake"), shutil.which("ninja")
    if not cmake or not ninja:
        raise RuntimeError("CMake and Ninja are required")

    output = args.output_dir.resolve()
    wrapper, build = output / "wrapper", output / "selected"
    wrapper.mkdir(parents=True, exist_ok=True)
    (wrapper / "CMakeLists.txt").write_text(f'''cmake_minimum_required(VERSION 3.22)
project(player_gear_save_writer_v1_host LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_OBJECT_PATH_MAX 128)
add_subdirectory("{MODULE.as_posix()}" selected-level-world)
''', encoding="utf-8")
    run([cmake, "-S", wrapper, "-B", build, "-G", "Ninja",
         f"-DCMAKE_MAKE_PROGRAM={Path(ninja).as_posix()}",
         f"-DCMAKE_CXX_COMPILER={compiler.as_posix()}",
         f"-DCMAKE_C_COMPILER={cc.as_posix()}",
         "-DCMAKE_BUILD_TYPE=Release",
         "-DCMAKE_CXX_FLAGS_RELEASE=-O1", "-DCMAKE_C_FLAGS_RELEASE=-O1"])
    run([cmake, "--build", build, "--target",
         "player_gear_save_writer_v1_audit", "--parallel", "2"])
    exe = build / "selected-level-world/player_gear_save_writer_v1_audit.exe"
    if not exe.is_file():
        candidates = list(build.rglob("player_gear_save_writer_v1_audit.exe"))
        if len(candidates) != 1:
            raise RuntimeError("selected GEAR writer audit executable missing")
        exe = candidates[0]
    env = os.environ.copy()
    runtime_dlls = [p.parent for p in build.rglob("*.dll")]
    env["PATH"] = os.pathsep.join([str(compiler.parent),
                                   *(str(p) for p in runtime_dlls),
                                   env.get("PATH", "")])
    report = json.loads(run([exe, args.cache.resolve()], env=env))
    if report.get("validation") != "PASS":
        raise RuntimeError(f"focused GEAR writer validation failed: {report}")
    print(json.dumps({"validation": "PASS", "selected_target": "dh2_level_world",
                      "host": report, "test": str(TESTS / "player_gear_save_writer_v1.cpp"),
                      "writer": str(MODULE / "player_gear_save_writer_v1.cpp")}))


if __name__ == "__main__":
    main()
