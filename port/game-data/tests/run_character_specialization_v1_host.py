from __future__ import annotations

import shutil
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
COMPILER = shutil.which("g++") or shutil.which("clang++")
if not COMPILER:
    raise SystemExit("A host C++17 compiler (g++ or clang++) is required")

cache = ROOT / ".local-inputs" / "ui-inventory-cache"
if not (cache / "character_properties_pyarray.bin").is_file():
    raise SystemExit(f"Original Character table cache is missing: {cache}")

with tempfile.TemporaryDirectory(prefix="dh2-character-specialization-") as temp:
    executable = Path(temp) / "character_specialization_v1_host.exe"
    subprocess.run(
        [COMPILER, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
         str(ROOT / "port" / "game-data" / "tests" / "character_specialization_v1.cpp"),
         str(ROOT / "port" / "game-data" / "data.cpp"), "-o", str(executable)],
        check=True,
    )
    subprocess.run([str(executable), str(cache)], check=True)
