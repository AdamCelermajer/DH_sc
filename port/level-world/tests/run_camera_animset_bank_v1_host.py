from __future__ import annotations
import shutil
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]
ASSETS = REPO / "port" / "android-native" / "app" / "src" / "main" / "assets"
COMPILER = shutil.which("g++") or shutil.which("clang++")
if not COMPILER:
    raise SystemExit("A host C++17 compiler (g++ or clang++) is required")

sources = [
    ROOT / "camera_animset_v1.cpp",
    ROOT / "camera_animset_bank_v1.cpp",
    ROOT.parent / "engine-resources" / "resources.cpp",
    REPO / "port" / "game-data" / "data.cpp",
    REPO / "port" / "game-data" / "animation_tables.cpp",
    REPO / "port" / "game-data" / "animation_selection.cpp",
    ROOT / "tests" / "camera_animset_bank_v1.cpp",
]
with tempfile.TemporaryDirectory(prefix="dh2-camera-animset-bank-") as temp:
    exe = Path(temp) / "camera_animset_bank_v1.exe"
    subprocess.run([
        COMPILER, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
        "-I", str(ROOT), "-I", str(REPO / "port" / "game-data"),
        *map(str, sources), "-o", str(exe),
    ], check=True)
    subprocess.run([str(exe), str(ASSETS)], check=True)
print("PASS: Default and SwampCam selected BDAE clip banks")
