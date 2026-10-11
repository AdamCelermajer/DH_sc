#!/usr/bin/env python3
import os
import pathlib
import shutil
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[3]
TEST = pathlib.Path(__file__).with_name("item_collision_action_v1.cpp")
LEVEL = pathlib.Path(__file__).parents[1]
compiler = os.environ.get("CXX") or shutil.which("g++")
if not compiler:
    raise SystemExit("C++17 compiler not found; set CXX")

with tempfile.TemporaryDirectory(prefix="dh2-item-collision-action-") as temp:
    output = pathlib.Path(temp) / ("collision.exe" if os.name == "nt" else "collision")
    subprocess.run([
        compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic",
        "-O2", str(TEST), str(LEVEL / "item_collision_action_v1.cpp"),
        str(LEVEL / "item_manager_pool_v1.cpp"), "-o", str(output),
    ], cwd=ROOT, check=True)
    subprocess.run([str(output)], cwd=ROOT, check=True)
