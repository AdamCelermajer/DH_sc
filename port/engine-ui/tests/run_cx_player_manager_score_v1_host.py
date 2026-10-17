#!/usr/bin/env python3
"""Compile and run the selected source CXPlayerManager score-branch audit."""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
test = root / "tests" / "cx_player_manager_score_v1.cpp"
source = root / "cx_player_manager_score_v1.cpp"
with tempfile.TemporaryDirectory(prefix="dh2-cx-score-") as tmp:
    exe = Path(tmp) / "cx_player_manager_score_v1.exe"
    subprocess.run(["g++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
                    str(test), str(source), "-o", str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
