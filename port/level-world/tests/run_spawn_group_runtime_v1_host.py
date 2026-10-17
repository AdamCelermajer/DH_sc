"""Build and run the isolated SpawnGroupManager policy test on the host."""
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
    parser.add_argument("--compiler", help="C++ compiler executable")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        raise SystemExit("no host C++ compiler found; pass --compiler")
    output = LEVEL_WORLD / "build" / "spawn_group_runtime_v1_host"
    if os.name == "nt":
        output = output.with_suffix(".exe")
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [
        compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
        str(LEVEL_WORLD / "tests" / "spawn_group_runtime_v1.cpp"),
        str(LEVEL_WORLD / "spawn_group_runtime_v1.cpp"), "-o", str(output),
    ]
    child_env = os.environ.copy()
    compiler_path = shutil.which(compiler) or compiler
    compiler_dir = str(Path(compiler_path).resolve().parent)
    child_env["PATH"] = compiler_dir + os.pathsep + child_env.get("PATH", "")
    built = subprocess.run(command, cwd=REPOSITORY, text=True, capture_output=True,
                           env=child_env)
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
        "weighted_spawn_handoff": True,
        "active_spot_filter_and_removal": True,
        "empty_spot_retry_timer": True,
        "mismatches": 0,
    }
    if any(report.get(key) != value for key, value in expected.items()):
        raise SystemExit(f"host report did not satisfy expectations: {report}")
    print(json.dumps({"validation": "PASS", "host_report": report}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
