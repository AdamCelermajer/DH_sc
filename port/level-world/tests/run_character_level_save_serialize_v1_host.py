#!/usr/bin/env python3
"""Compile and run the Character::Serialize byte-order regression."""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
test = root / "tests" / "character_level_save_serialize_v1.cpp"
source = root / "character_level_save_serialize_v1.cpp"
level_source = root / "level_savegame_owner_v1.cpp"
with tempfile.TemporaryDirectory(prefix="dh2-character-level-save-") as tmp:
    exe = Path(tmp) / "character_level_save_serialize_v1.exe"
    subprocess.run(["g++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
                    "-I" + str(root.parent / "game-data"),
                    "-I" + str(root.parent / "engine-animation"),
                    "-I" + str(root.parent / "engine-skinning"),
                    str(test), str(source), str(level_source), "-o", str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
