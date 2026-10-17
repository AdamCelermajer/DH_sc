"""Build and run the isolated input-to-CharacterController projection audit."""
from __future__ import annotations

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[3]
LEVEL = ROOT / "port/level-world"
SOURCES = [
    LEVEL / "navigation_heading.cpp",
    ROOT / "port/android-native/tests/native_player_input_controller_v1_host.cpp",
]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++ compiler (defaults to CXX, g++, or clang++)")
    parser.add_argument("--output", type=Path, help="Keep the host executable at this path")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("pass --compiler or set CXX")
    if args.output:
        output = args.output.resolve()
        output.parent.mkdir(parents=True, exist_ok=True)
        temporary = None
    else:
        temporary = tempfile.TemporaryDirectory(prefix="dh2-player-input-controller-")
        output = Path(temporary.name) / ("audit.exe" if os.name == "nt" else "audit")
    command = [
        compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic",
        "-fno-fast-math", "-ffp-contract=off", "-I", str(LEVEL),
        *(str(source) for source in SOURCES), "-o", str(output),
    ]
    built = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if built.returncode:
        raise RuntimeError(built.stdout + built.stderr)
    ran = subprocess.run([str(output)], cwd=ROOT, capture_output=True, text=True)
    if ran.returncode:
        raise RuntimeError(ran.stdout + ran.stderr)
    print(ran.stdout.strip())
    if temporary:
        temporary.cleanup()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
