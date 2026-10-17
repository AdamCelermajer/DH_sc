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
    ROOT / "character_critical_camera_bridge_v1.cpp",
    ROOT / "camera_animset_bank_v1.cpp",
    ROOT / "camera_animset_playback_v1.cpp",
    ROOT / "camera_animset_bank_playback_v1.cpp",
    ROOT / "camera_level_runtime_v1.cpp",
    ROOT / "camera_design_zoom_v1.cpp",
    ROOT / "player_camera_rig_v1.cpp",
    ROOT / "visual_timeline.cpp",
    REPO / "port" / "engine-animation" / "animation.cpp",
    REPO / "port" / "engine-animation" / "events.cpp",
    REPO / "port" / "engine-animation" / "event_track.cpp",
    REPO / "port" / "engine-animation" / "angle_interpreter.cpp",
    REPO / "port" / "scene-materials" / "scene.cpp",
    REPO / "port" / "scene-payloads" / "scene.cpp",
    ROOT.parent / "engine-resources" / "resources.cpp",
    REPO / "port" / "asset-payloads" / "payloads.cpp",
    REPO / "port" / "engine-math" / "math.cpp",
    REPO / "port" / "game-data" / "data.cpp",
    REPO / "port" / "game-data" / "animation_tables.cpp",
    REPO / "port" / "game-data" / "animation_selection.cpp",
    ROOT / "tests" / "camera_animset_bank_playback_v1.cpp",
]
with tempfile.TemporaryDirectory(prefix="dh2-camera-bank-playback-") as temp:
    exe = Path(temp) / "camera_animset_bank_playback_v1.exe"
    subprocess.run([
        COMPILER, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
        "-I", str(ROOT), "-I", str(REPO / "port" / "game-data"),
        "-I", str(REPO / "port" / "engine-animation"),
        "-I", str(REPO / "port" / "scene-materials"),
        "-I", str(REPO / "port" / "scene-payloads"),
        "-I", str(REPO / "port" / "engine-resources"),
        "-I", str(REPO / "port" / "asset-payloads"),
        "-I", str(REPO / "port" / "engine-math"),
        *map(str, sources), "-o", str(exe),
    ], check=True)
    subprocess.run([str(exe), str(ASSETS)], check=True)
print("PASS: selected BDAE bank dispatch and shared CameraLevel playback state")
