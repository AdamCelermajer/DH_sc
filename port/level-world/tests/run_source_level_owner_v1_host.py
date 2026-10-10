#!/usr/bin/env python3
"""Compile and run the isolated source-level lifecycle/save-owner regression."""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
test = root / "tests" / "source_level_owner_v1.cpp"
sources = [test, root / "source_level_owner_v1.cpp",
           root / "object_manager_runtime_owner_v1.cpp",
           root / "level_quick_save_v1.cpp", root / "level_savegame_save_v1.cpp",
           root / "level_savegame_owner_v1.cpp",
           root.parent / "game-data" / "player_save_level_states_v1.cpp"]
with tempfile.TemporaryDirectory(prefix="dh2-source-level-owner-") as tmp:
    exe = Path(tmp) / "source_level_owner_v1.exe"
    subprocess.run(["g++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
                    *map(str, sources), "-o", str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
