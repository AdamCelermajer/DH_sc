#!/usr/bin/env python3
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[3]
LEVEL = ROOT / "port/level-world"
compiler = os.environ.get("CXX") or shutil.which("g++")
if not compiler:
    raise SystemExit("C++17 compiler not found; set CXX")

with tempfile.TemporaryDirectory(prefix="dh2-kill-field-14e4-") as temp:
    output = Path(temp) / ("kill-field.exe" if os.name == "nt" else "kill-field")
    subprocess.run([
        compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic", "-O2",
        str(LEVEL / "character_kill_field_14e4_v1.cpp"),
        str(LEVEL / "tests/character_kill_field_14e4_v1.cpp"),
        "-o", str(output),
    ], cwd=ROOT, check=True)
    subprocess.run([str(output)], cwd=ROOT, check=True)
