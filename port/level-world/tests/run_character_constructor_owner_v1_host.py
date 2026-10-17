"""Compile and run the semantic Character constructor owner regression."""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

LEVEL_WORLD = Path(__file__).resolve().parents[1]

def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++17 host compiler")
    parser.add_argument("--output", type=Path,
        default=LEVEL_WORLD / "build" / "character_constructor_owner_v1_host.exe")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("no C++ host compiler found; pass --compiler or set CXX")
    output = args.output.resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
        str(LEVEL_WORLD / "tests" / "character_constructor_owner_v1.cpp"),
        str(LEVEL_WORLD / "character_constructor_owner_v1.cpp"), "-o", str(output)]
    built = subprocess.run(command, text=True, capture_output=True)
    if built.stdout:
        sys.stdout.write(built.stdout)
    if built.stderr:
        sys.stderr.write(built.stderr)
    if built.returncode:
        return built.returncode
    result = subprocess.run([str(output)], text=True, capture_output=True)
    if result.stdout:
        sys.stdout.write(result.stdout)
    if result.stderr:
        sys.stderr.write(result.stderr)
    if result.returncode:
        return result.returncode
    report = json.loads(result.stdout)
    if report != {"constructor_order": True, "registered_states": 20,
                  "failure_rollback": True}:
        raise SystemExit(f"unexpected regression result: {report}")
    print(json.dumps({"validation": "PASS", "host_report": report}))
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
