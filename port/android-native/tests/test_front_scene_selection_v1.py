"""Compile the source-owned front scene routing policy against its edge cases."""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"


class FrontSceneSelection(unittest.TestCase):
    def test_name_entry_uses_authored_swf_scene(self):
        if shutil.which("g++"):
            prefix, convert = [], str
        elif os.name == "nt" and shutil.which("wsl"):
            prefix = ["wsl", "--exec"]

            def convert(path):
                return subprocess.check_output(prefix + ["wslpath", "-a", path.as_posix()], text=True).strip()
        else:
            self.fail("A C++17 host compiler is required")

        with tempfile.TemporaryDirectory(prefix="dh2-front-scene-") as temporary:
            executable = Path(temporary) / "front-scene-selection"
            command = prefix + ["g++", "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror"]
            command += ["-I" + convert(CPP), convert(ROOT / "port/android-native/tests/front_scene_selection_v1.cpp")]
            command += ["-o", convert(executable)]
            build = subprocess.run(command, text=True, capture_output=True, timeout=90)
            self.assertEqual(build.returncode, 0, build.stdout + build.stderr)
            result = subprocess.run(prefix + [convert(executable)], text=True, capture_output=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)


if __name__ == "__main__":
    unittest.main()
