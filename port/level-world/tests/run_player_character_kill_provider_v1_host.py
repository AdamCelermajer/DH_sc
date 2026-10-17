#!/usr/bin/env python3
import argparse
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[3]
LEVEL = ROOT / "port/level-world"
DATA = ROOT / "port/game-data"

parser = argparse.ArgumentParser()
parser.add_argument("--compiler", default=os.environ.get("CXX") or shutil.which("g++"))
args = parser.parse_args()
if not args.compiler:
    raise SystemExit("C++17 compiler not found; pass --compiler")

with tempfile.TemporaryDirectory(prefix="dh2-player-kill-provider-") as temp:
    output = Path(temp) / ("kill-provider.exe" if os.name == "nt" else "kill-provider")
    subprocess.run([
        args.compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic",
        "-O2", "-I", str(ROOT), "-I", str(DATA),
        str(LEVEL / "player_character_kill_provider_v1.cpp"),
        str(LEVEL / "player_kill_continuation_v1.cpp"),
        str(ROOT / "port/player-info-level/player_locality_v1.cpp"),
        str(DATA / "properties.cpp"), str(DATA / "class_tables.cpp"),
        str(LEVEL / "tests/player_character_kill_provider_v1.cpp"),
        "-o", str(output),
    ], cwd=ROOT, check=True)
    subprocess.run([str(output)], cwd=ROOT, check=True)
