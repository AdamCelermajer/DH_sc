#!/usr/bin/env python3
import argparse
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[3]
CPP = Path(__file__).resolve().parents[1]

parser = argparse.ArgumentParser()
parser.add_argument("--compiler", default=os.environ.get("CXX") or shutil.which("g++"))
args = parser.parse_args()
if not args.compiler:
    raise SystemExit("C++17 compiler not found; pass --compiler")

with tempfile.TemporaryDirectory(prefix="dh2-kill-death-tail-") as temp:
    output = Path(temp) / ("tail.exe" if os.name == "nt" else "tail")
    subprocess.run([
        args.compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror",
        "-pedantic", "-O2", "-I", str(CPP),
        str(CPP / "object_update_culling.cpp"),
        str(CPP / "character_kill_death_tail_v1.cpp"),
        str(CPP / "tests/character_kill_death_tail_v1.cpp"),
        "-o", str(output),
    ], cwd=ROOT, check=True)
    subprocess.run([str(output)], cwd=ROOT, check=True)
