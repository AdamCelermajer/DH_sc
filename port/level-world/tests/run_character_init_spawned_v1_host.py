"""Compile and run the narrow Character::InitSpawned host check."""
from __future__ import annotations

import json
import os
import argparse
from pathlib import Path
import shutil
import subprocess
import sys

LEVEL_WORLD = Path(__file__).resolve().parents[1]
REPOSITORY = LEVEL_WORLD.parents[1]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++ compiler executable")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        raise SystemExit("no host C++ compiler found; pass --compiler or use the Android NDK test build")
    output = LEVEL_WORLD / "build" / "character_init_spawned_v1_host"
    if os.name == "nt":
        output = output.with_suffix(".exe")
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [
        compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
        str(LEVEL_WORLD / "tests" / "character_init_spawned_v1.cpp"),
        str(LEVEL_WORLD / "character_init_spawned_v1.cpp"), "-o", str(output),
    ]
    compiler_path = shutil.which(compiler) or compiler
    child_env = os.environ.copy()
    child_env["PATH"] = str(Path(compiler_path).resolve().parent) + os.pathsep + child_env.get("PATH", "")
    built = subprocess.run(command, cwd=REPOSITORY, text=True,
                           capture_output=True, env=child_env)
    if built.stdout:
        sys.stdout.write(built.stdout)
    if built.stderr:
        sys.stderr.write(built.stderr)
    if built.returncode:
        return built.returncode
    result = subprocess.run([str(output)], cwd=REPOSITORY, text=True,
                            capture_output=True, env=child_env)
    if result.stdout:
        sys.stdout.write(result.stdout)
    if result.stderr:
        sys.stderr.write(result.stderr)
    if result.returncode:
        return result.returncode
    report = json.loads(result.stdout)
    expected = {
        "character_init_spawned_order": True,
        "partial_failure_short_circuits": True,
        "mismatches": 0,
    }
    if any(report.get(key) != value for key, value in expected.items()):
        raise SystemExit(f"host report did not satisfy expectations: {report}")
    print(json.dumps({"validation": "PASS", "host_report": report}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
