#!/usr/bin/env python3
import argparse
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[3]
LEVEL = ROOT / "port/level-world"
GAME_DATA = ROOT / "port/game-data"

parser = argparse.ArgumentParser()
parser.add_argument("--compiler", default=os.environ.get("CXX") or shutil.which("g++"))
args = parser.parse_args()
if not args.compiler:
    raise SystemExit("C++17 compiler not found; pass --compiler")

with tempfile.TemporaryDirectory(prefix="dh2-ctrl-kill-compose-") as temp:
    output = Path(temp) / ("composition.exe" if os.name == "nt" else "composition")
    subprocess.run([
        args.compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror",
        "-pedantic", "-O2", "-I", str(ROOT), "-I", str(GAME_DATA),
        str(LEVEL / "character_kill_death_tail_v1.cpp"),
        str(LEVEL / "player_kill_continuation_v1.cpp"),
        str(LEVEL / "character_kill_source_bridge_v1.cpp"),
        str(GAME_DATA / "properties.cpp"),
        str(GAME_DATA / "class_tables.cpp"),
        str(LEVEL / "tests/character_kill_ctrl_composition_v1.cpp"),
        "-o", str(output),
    ], cwd=ROOT, check=True)
    subprocess.run([str(output)], cwd=ROOT, check=True)
