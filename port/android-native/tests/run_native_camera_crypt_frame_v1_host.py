from __future__ import annotations

import shutil
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
COMPILER = shutil.which("g++") or shutil.which("clang++")
if not COMPILER:
    raise SystemExit("A host C++17 compiler (g++ or clang++) is required")

with tempfile.TemporaryDirectory(prefix="dh2-camera-crypt-frame-") as temp:
    exe = Path(temp) / "native_camera_crypt_frame_v1_host.exe"
    source = ROOT / "port/android-native/tests/native_camera_crypt_frame_v1_host.cpp"
    projection = ROOT / "port/engine-camera/projection_matrix.cpp"
    subprocess.run([COMPILER, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
                    "-I", str(ROOT / "port/engine-camera"), str(source), str(projection),
                    "-o", str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
