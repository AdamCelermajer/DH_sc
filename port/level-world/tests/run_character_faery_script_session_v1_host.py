"""Build and run the fail-closed AISFaery script-session regression."""
from __future__ import annotations

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

LEVEL_WORLD = Path(__file__).resolve().parents[1]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++17 host compiler")
    args = parser.parse_args()
    compiler = (args.compiler or os.environ.get("CXX") or
                shutil.which("g++") or shutil.which("clang++"))
    if not compiler:
        parser.error("no C++17 host compiler found; pass --compiler or set CXX")
    with tempfile.TemporaryDirectory(prefix="dh2-faery-session-") as temporary:
        output = Path(temporary) / "character_faery_script_session_v1.exe"
        command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
            str(LEVEL_WORLD / "tests" / "character_faery_script_session_v1.cpp"),
            str(LEVEL_WORLD / "character_faery_script_session_v1.cpp"),
            "-o", str(output)]
        built = subprocess.run(command, text=True, capture_output=True)
        if built.stdout:
            sys.stdout.write(built.stdout)
        if built.stderr:
            sys.stderr.write(built.stderr)
        if built.returncode:
            return built.returncode
        run = subprocess.run([str(output)], text=True, capture_output=True)
        if run.stdout:
            sys.stdout.write(run.stdout)
        if run.stderr:
            sys.stderr.write(run.stderr)
        if run.returncode:
            return run.returncode
    print("PASS AISFaery single-session lifecycle, identity, order, and failure gates")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
