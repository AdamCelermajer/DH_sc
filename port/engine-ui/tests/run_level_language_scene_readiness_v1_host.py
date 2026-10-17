#!/usr/bin/env python3
"""Compile and run the isolated Level-language readiness contract audit."""
from pathlib import Path
import os
import subprocess
import tempfile

root = Path(__file__).resolve().parents[3]
compiler = os.environ.get("CXX", "g++")
with tempfile.TemporaryDirectory(prefix="dh2-level-language-readiness-") as temporary:
    executable = Path(temporary) / "readiness.exe"
    subprocess.run([
        compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror",
        str(root / "port/engine-ui/tests/level_language_scene_readiness_v1.cpp"),
        "-o", str(executable),
    ], check=True)
    subprocess.run([str(executable)], check=True)
