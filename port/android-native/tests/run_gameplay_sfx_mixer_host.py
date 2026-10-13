#!/usr/bin/env python3
"""Host test for the Android gameplay SFX backend core.

Builds and runs gameplay_sfx_mixer_host.cpp (WAV decode, mixing, fades, Vox
priority banks, shared-queue consumer, every staged asset decodes) and checks
that the generated catalog/provenance still match the committed Sounds.xml data.
"""
from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
TEST = ROOT / "port/android-native/tests/gameplay_sfx_mixer_host.cpp"
STAGER = ROOT / "port/android-native/tools/stage_gameplay_sfx.py"

parser = argparse.ArgumentParser()
parser.add_argument("--compiler", default=os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++"))
args = parser.parse_args()
if not args.compiler:
    raise SystemExit("C++17 compiler not found; pass --compiler")

subprocess.run([sys.executable, str(STAGER), "--check"], cwd=ROOT, check=True)
with tempfile.TemporaryDirectory(prefix="dh2-gameplay-sfx-") as temp:
    exe = Path(temp) / ("sfx-test.exe" if os.name == "nt" else "sfx-test")
    subprocess.run([
        args.compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic", "-O2",
        "-static-libstdc++", "-static-libgcc", str(TEST), "-o", str(exe),
    ], cwd=ROOT, check=True)
    subprocess.run([str(exe), str(ROOT / "port/android-native/app/src/main/assets")],
                   cwd=ROOT, check=True)
