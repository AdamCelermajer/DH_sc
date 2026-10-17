import argparse
from pathlib import Path
import shutil
import subprocess

TESTS = Path(__file__).resolve().parent
MODULE = TESTS.parent
ROOT = MODULE.parents[1]

parser = argparse.ArgumentParser()
parser.add_argument("--compiler", default=shutil.which("g++") or "g++")
parser.add_argument("--output-dir", type=Path, default=MODULE / "build/player-faery-change-v1")
args = parser.parse_args()
compiler = shutil.which(args.compiler) or args.compiler
args.output_dir.mkdir(parents=True, exist_ok=True)
binary = args.output_dir.resolve() / "player_faery_change_v1_host.exe"
sources = [TESTS / "player_faery_change_v1.cpp",
           MODULE / "player_faery_change_v1.cpp",
           MODULE / "player_savegame_v1.cpp",
           MODULE / "player_save_level_states_v1.cpp",
           MODULE / "skill_tables.cpp"]
subprocess.run([compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
                "-fno-fast-math", f"-I{MODULE}", *(str(p) for p in sources),
                "-o", str(binary)], cwd=ROOT, check=True)
completed = subprocess.run([str(binary)], cwd=ROOT, check=True,
                           capture_output=True, text=True)
print(completed.stdout, end="")
