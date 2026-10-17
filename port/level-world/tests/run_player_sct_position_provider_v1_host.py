#!/usr/bin/env python3
"""Compile and run the isolated canonical Player SCT projection check."""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
sources = [root / "tests" / "player_sct_position_provider_v1.cpp",
           root / "player_sct_position_provider_v1.cpp"]
with tempfile.TemporaryDirectory(prefix="dh2-player-sct-provider-") as tmp:
    exe = Path(tmp) / "player_sct_position_provider_v1.exe"
    subprocess.run(["g++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
                    *map(str, sources), "-o", str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
