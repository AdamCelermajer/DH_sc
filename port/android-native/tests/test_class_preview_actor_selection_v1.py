"""Execute the real preview draw include with recording providers.

This covers actor and weapon submission through selection changes, without
claiming authored-asset or emulator visual validation. Requires C++17 g++
(native, or WSL g++ on Windows).
"""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[3]


class ClassPreviewActorSelectionTest(unittest.TestCase):
    def test_authored_actor_and_weapon_owners_survive_class_transitions(self):
        source = ROOT / "port/android-native/tests/class_preview_actor_submission_v1.cpp"
        with tempfile.TemporaryDirectory(prefix="dh2-class-preview-") as directory:
            executable = Path(directory) / "class-preview"
            if shutil.which("g++"):
                prefix = []
                source_arg, output_arg = str(source), str(executable)
            elif os.name == "nt" and shutil.which("wsl"):
                prefix = ["wsl", "--exec"]

                def linux_path(path):
                    return subprocess.check_output(
                        prefix + ["wslpath", "-a", path.as_posix()], text=True,
                    ).strip()

                source_arg, output_arg = linux_path(source), linux_path(executable)
            else:
                self.fail("A C++17 host compiler is required")
            compiled = subprocess.run(
                prefix + ["g++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
                          "-fsanitize=address,undefined", "-fno-omit-frame-pointer",
                          source_arg, "-o", output_arg],
                text=True, capture_output=True, timeout=60,
            )
            self.assertEqual(compiled.returncode, 0,
                             compiled.stdout + compiled.stderr)
            result = subprocess.run(prefix + [output_arg], text=True,
                                    capture_output=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn("PASS", result.stdout)
            print(result.stdout, end="")


if __name__ == "__main__":
    unittest.main()
