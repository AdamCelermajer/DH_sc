from __future__ import annotations
import shutil
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]
ASSETS = REPO / "port" / "android-native" / "app" / "src" / "main" / "assets"
CAMERA = ASSETS / "original-cache" / "data" / "3d" / "camera"
COMPILER = shutil.which("g++") or shutil.which("clang++")
if not COMPILER:
    raise SystemExit("A host C++17 compiler (g++ or clang++) is required")

sources = [
    ROOT / "tests" / "map_camera_rig_v1.cpp",
    ROOT / "player_camera_rig_v1.cpp",
    ROOT / "visual_timeline.cpp",
    REPO / "port" / "engine-animation" / "animation.cpp",
    REPO / "port" / "engine-animation" / "events.cpp",
    REPO / "port" / "engine-animation" / "event_track.cpp",
    REPO / "port" / "engine-animation" / "angle_interpreter.cpp",
    REPO / "port" / "scene-materials" / "scene.cpp",
    REPO / "port" / "scene-payloads" / "scene.cpp",
    REPO / "port" / "engine-resources" / "resources.cpp",
    REPO / "port" / "asset-payloads" / "payloads.cpp",
    REPO / "port" / "engine-math" / "math.cpp",
]
includes = [
    ROOT, REPO / "port" / "engine-animation", REPO / "port" / "scene-materials",
    REPO / "port" / "scene-payloads", REPO / "port" / "engine-resources",
    REPO / "port" / "asset-payloads", REPO / "port" / "engine-math",
]
with tempfile.TemporaryDirectory(prefix="dh2-map-camera-rig-") as temp:
    exe = Path(temp) / "map_camera_rig_v1.exe"
    subprocess.run([
        COMPILER, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
        "-fno-fast-math", "-ffp-contract=off",
        *[arg for include in includes for arg in ("-I", str(include))],
        *map(str, sources), "-o", str(exe),
    ], check=True)
    subprocess.run([
        str(exe), str(CAMERA / "minimapcameras.bdae"),
        str(CAMERA / "animations" / "common" / "camera_minimap_idle.bdae"),
    ], check=True)
print("PASS: source minimap camera scene and idle animation bind through selected Rig")
