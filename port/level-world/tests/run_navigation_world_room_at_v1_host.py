#!/usr/bin/env python3
"""Build the bounded PFWorld::GetRoomAt projection fixture."""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
test = root / "tests" / "navigation_world_room_at_v1.cpp"
source = root / "navigation_room_query.cpp"
with tempfile.TemporaryDirectory(prefix="dh2-navigation-room-at-") as temp:
    exe = Path(temp) / "navigation_world_room_at_v1.exe"
    subprocess.run([
        "g++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
        "-ffunction-sections", "-fdata-sections", "-Wl,--gc-sections",
        str(test), str(source), "-o", str(exe)
    ], check=True)
    subprocess.run([str(exe)], check=True)
