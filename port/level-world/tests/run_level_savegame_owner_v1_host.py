#!/usr/bin/env python3
"""Compile and run the per-level INFO/OBJS save owner regression."""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
test = root / "tests" / "level_savegame_owner_v1.cpp"
source = root / "level_savegame_owner_v1.cpp"
with tempfile.TemporaryDirectory(prefix="dh2-level-savegame-owner-") as tmp:
    exe = Path(tmp) / "level_savegame_owner_v1.exe"
    subprocess.run(["g++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
                    str(test), str(source), "-o", str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
