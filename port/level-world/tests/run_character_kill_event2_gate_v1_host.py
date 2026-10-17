#!/usr/bin/env python3
import argparse
import os
import pathlib
import shutil
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[3]
TEST = pathlib.Path(__file__).with_name("character_kill_event2_gate_v1.cpp")

parser = argparse.ArgumentParser()
parser.add_argument("--compiler", default=os.environ.get("CXX") or shutil.which("g++"))
args = parser.parse_args()
if not args.compiler:
    raise SystemExit("C++17 compiler not found; pass --compiler")

with tempfile.TemporaryDirectory(prefix="dh2-kill-event2-gate-") as temp:
    output = pathlib.Path(temp) / ("gate.exe" if os.name == "nt" else "gate")
    subprocess.run([
        args.compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror",
        "-pedantic", "-O2", str(TEST), "-o", str(output),
    ], cwd=ROOT, check=True)
    subprocess.run([str(output)], cwd=ROOT, check=True)
