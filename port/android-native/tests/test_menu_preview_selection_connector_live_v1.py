"""Run current production slot-to-preview connection with declared native leaves.

This is a host-only compiler fixture. It builds no application target and uses
no APK, ADB or emulator. Canonical CreatePlayer/PCLS/model resource behavior is
an explicit boundary, covered separately by the existing authored GPU checks.
"""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"


def section(text, start, end):
    first = text.index(start)
    return text[first:text.index(end, first + len(start))]


class MenuSelectionConnector(unittest.TestCase):
    def test_current_saved_empty_and_creation_slot_routing(self):
        if shutil.which("g++"):
            prefix, convert = [], str
        elif os.name == "nt" and shutil.which("wsl"):
            prefix = ["wsl", "--exec"]

            def convert(path):
                return subprocess.check_output(prefix + ["wslpath", "-a", path.as_posix()], text=True).strip()
        else:
            self.fail("A C++17 host compiler is required")
        preview = (CPP / "native_menu_preview_v121.cpp").read_text()
        methods = {
            "destroy": (" bool destroy_character(", " bool destroy_scene("),
            "setup": (" bool setup_character(", " template<class F> bool run("),
            "run": (" template<class F> bool run(", "public:"),
            "avatar_destroy": (" bool avatar_destroy(", " bool scene_destroy("),
            "avatar_setup": (" bool avatar_setup(", " bool avatar_camera("),
            "avatar_camera": (" bool avatar_camera(", " bool camera("),
        }
        with tempfile.TemporaryDirectory(prefix="dh2-menu-selection-connector-") as temporary:
            directory = Path(temporary)
            for name, (start, end) in methods.items():
                (directory / ("menu_selection_" + name + "_live_v1.inc")).write_text(section(preview, start, end))
            executable = directory / "menu-selection-connector"
            command = prefix + ["g++", "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror"]
            command += ["-I" + convert(path) for path in [CPP, ROOT / "port/engine-ui", ROOT / "port/engine-math", directory]]
            sources = [ROOT / "port/android-native/tests/menu_preview_selection_connector_live_v1.cpp",
                ROOT / "port/engine-ui/menu_avatar_preview_v1.cpp", ROOT / "port/engine-math/math.cpp"]
            command += [convert(path) for path in sources] + ["-o", convert(executable)]
            result = subprocess.run(command, text=True, capture_output=True, timeout=90)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            result = subprocess.run(prefix + [convert(executable)], text=True, capture_output=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn("PASS current NativeSetSlot underlying ChangeCharacter", result.stdout)
            print(result.stdout, end="")


if __name__ == "__main__":
    unittest.main()
