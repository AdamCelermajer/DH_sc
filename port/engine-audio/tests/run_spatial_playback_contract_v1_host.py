#!/usr/bin/env python3
"""Build the source-input-only spatial playback contract regression."""
from __future__ import annotations

import os
from pathlib import Path
import shutil
import subprocess
import tempfile

TEST = Path(__file__).with_name("spatial_playback_contract_v1.cpp")
COMPILER = os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
if not COMPILER:
    raise SystemExit("C++17 compiler not found; pass CXX")

with tempfile.TemporaryDirectory(prefix="dh2-spatial-contract-") as temp:
    exe = Path(temp) / ("spatial-contract.exe" if os.name == "nt" else "spatial-contract")
    subprocess.run([COMPILER, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
                    "-pedantic", str(TEST), "-o", str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
print("PASS: source ID/driver offsets, stereo pan branches, degenerates, Q14 truncation, and unclamped gain math")
