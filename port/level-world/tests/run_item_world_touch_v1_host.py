#!/usr/bin/env python3
import os
import pathlib
import shutil
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[3]
TEST = pathlib.Path(__file__).with_name("item_world_touch_v1.cpp")
compiler = os.environ.get("CXX") or shutil.which("g++")
if not compiler:
    raise SystemExit("C++17 compiler not found; set CXX")

with tempfile.TemporaryDirectory(prefix="dh2-item-world-touch-") as temp:
    output = pathlib.Path(temp) / ("touch.exe" if os.name == "nt" else "touch")
    subprocess.run([
        compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic",
        "-O2", str(TEST), "-o", str(output),
    ], cwd=ROOT, check=True)
    subprocess.run([str(output)], cwd=ROOT, check=True)
