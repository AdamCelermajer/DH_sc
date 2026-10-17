#!/usr/bin/env python3
"""Small host compile/run check for source-ordered Faery placement."""
from pathlib import Path
import os
import subprocess
import tempfile

TESTS = Path(__file__).resolve().parent
PORT = TESTS.parents[1]

with tempfile.TemporaryDirectory(prefix="dh2-faery-placement-") as temporary:
    executable = Path(temporary) / ("faery-placement.exe" if os.name == "nt" else "faery-placement")
    command = [os.environ.get("CXX", "g++"), "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
               str(PORT / "level-world/character_faery_placement_v1.cpp"),
               str(TESTS / "character_faery_placement_v1.cpp"), "-o", str(executable)]
    subprocess.run(command, check=True)
    subprocess.run([str(executable)], check=True)
