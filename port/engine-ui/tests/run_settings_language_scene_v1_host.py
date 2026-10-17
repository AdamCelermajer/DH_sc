import os
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[3]
COMPILER = Path(os.environ.get("CXX", "g++"))
with tempfile.TemporaryDirectory(prefix="dh2-settings-language-") as temporary:
    executable = Path(temporary) / "settings_language_scene_v1.exe"
    subprocess.run([str(COMPILER), "-std=c++17", "-Wall", "-Wextra", "-Werror",
                    "-Wno-error=misleading-indentation",
                    str(ROOT / "port/engine-ui/settings_language_scene_v1.cpp"),
                    str(ROOT / "port/engine-ui/tests/settings_language_scene_v1.cpp"),
                    "-o", str(executable)], check=True)
    subprocess.run([str(executable)], check=True)
