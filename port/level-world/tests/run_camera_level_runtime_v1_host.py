from __future__ import annotations
import shutil
import subprocess
import tempfile
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
COMPILER=shutil.which("g++") or shutil.which("clang++")
if not COMPILER:
    raise SystemExit("A host C++17 compiler (g++ or clang++) is required")

with tempfile.TemporaryDirectory(prefix="dh2-camera-level-") as temp:
    exe=Path(temp)/"camera_level_runtime_v1_host.exe"
    subprocess.run([COMPILER,"-std=c++17","-O2","-Wall","-Wextra","-Werror",
                    str(ROOT/"camera_level_runtime_v1.cpp"),
                    str(ROOT/"camera_design_zoom_v1.cpp"),
                    str(ROOT/"tests/camera_level_runtime_v1_host.cpp"),"-o",str(exe)],check=True)
    subprocess.run([str(exe)],check=True)
