"""Run the production Front render body with declared movie/GPU transports.

This covers the draw-after-Show boundary absent from the older creation delivery
regression. Runtime APK verification is separate from this host fixture.
"""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"


class ClassCreationRenderTransition(unittest.TestCase):
    def test_current_front_render_after_creation_show(self):
        if shutil.which("g++"):
            prefix, convert = [], str
        elif os.name == "nt" and shutil.which("wsl"):
            prefix = ["wsl", "--exec"]

            def convert(path):
                return subprocess.check_output(prefix + ["wslpath", "-a", path.as_posix()], text=True).strip()
        else:
            self.fail("A C++17 host compiler is required")
        text = (CPP / "front_ui_session_v87.cpp").read_text()
        start = text.index("bool FrontUiSessionV87::render_source_movies_v93(")
        end = text.index("bool FrontUiSessionV87::character_menu_sound_v4(", start)
        with tempfile.TemporaryDirectory(prefix="dh2-class-render-") as temporary:
            directory = Path(temporary)
            (directory / "class_creation_render_under_test.inc").write_text(text[start:end])
            executable = directory / "class-render"
            command = prefix + ["g++", "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror"]
            command += ["-I" + convert(path) for path in [CPP, directory]]
            command += [convert(ROOT / "port/android-native/tests/class_creation_render_transition_v137.cpp"), "-o", convert(executable)]
            build = subprocess.run(command, text=True, capture_output=True, timeout=90)
            self.assertEqual(build.returncode, 0, build.stdout + build.stderr)
            result = subprocess.run(prefix + [convert(executable)], text=True, capture_output=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn("PASS production Front Confirm render", result.stdout)
            print(result.stdout, end="")


if __name__ == "__main__":
    unittest.main()
