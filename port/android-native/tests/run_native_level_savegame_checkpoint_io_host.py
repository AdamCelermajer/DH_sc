#!/usr/bin/env python3
"""Compile and run bounded Level/Player checkpoint filename readers."""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
test = root / "tests" / "native_level_savegame_checkpoint_io.cpp"
publisher = root / "app" / "src" / "main" / "cpp" / "native_level_savegame_publisher_v1.cpp"
with tempfile.TemporaryDirectory(prefix="dh2-checkpoint-io-host-") as temp:
    exe = Path(temp) / "native_level_savegame_checkpoint_io.exe"
    subprocess.run(["g++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
                    str(test), str(publisher), "-o", str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
