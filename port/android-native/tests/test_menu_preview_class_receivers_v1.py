"""Verify saved-slot and creation previews against the packaged authored assets.

Builds the current receiver routing, PCLS/property selection, modular-resource
loader and production creation draw include. The existing desktop data/scene/
animation/skinning DSOs supply their other dependencies; this is a host
regression, not a packaged APK or GL/device render check.
"""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[3]
HOST_BUILD = Path(os.environ.get(
    "DH2_CLASS_PREVIEW_HOST_BUILD",
    str(ROOT / ".local-inputs/native-host-v128/build/native"),
))


class MenuPreviewClassReceiversTest(unittest.TestCase):
    def test_mage_and_warrior_keep_their_authored_receivers_in_both_flows(self):
        libraries = [
            HOST_BUILD / "level-world/game-data/libdh2_game_data.so",
            HOST_BUILD / "scene-materials/libdh2_scene_materials.so",
            HOST_BUILD / "engine-animation/libdh2_engine_animation.so",
            HOST_BUILD / "engine-skinning/libdh2_engine_skinning.so",
        ]
        for library in libraries:
            self.assertTrue(library.is_file(), f"Missing desktop dependency: {library}")
        if shutil.which("g++"):
            prefix = []
            convert = str
        elif os.name == "nt" and shutil.which("wsl"):
            prefix = ["wsl", "--exec"]

            def convert(path):
                return subprocess.check_output(
                    prefix + ["wslpath", "-a", path.as_posix()], text=True,
                ).strip()
        else:
            self.fail("A C++17 host compiler is required")
        sources = [
            ROOT / "port/android-native/tests/menu_preview_class_receivers_v1.cpp",
            ROOT / "port/level-world/objects.cpp",
            ROOT / "port/level-world/character_props_id_owner_v1.cpp",
            ROOT / "port/engine-ui/menu_avatar_preview_v1.cpp",
            ROOT / "port/game-data/class_preview_setup.cpp",
            ROOT / "port/game-data/menu_profile_metadata_v1.cpp",
            ROOT / "port/game-data/fresh_player_profile_v1.cpp",
        ]
        includes = [ROOT / "port" / name for name in (
            "engine-ui", "game-data", "level-world", "engine-animation",
            "engine-resources", "engine-math", "engine-skinning", "scene-materials",
        )]
        with tempfile.TemporaryDirectory(prefix="dh2-preview-class-") as temporary:
            executable = Path(temporary) / "preview-class"
            exports = (ROOT / "port/android-native/app/src/main/cpp/renderer_front_exports_v87.inc").read_text()
            start = exports.index("bool select_class_scene(")
            end = exports.index("void draw_class_scene(", start)
            transition = Path(temporary) / "menu_preview_class_transition_under_test.inc"
            transition.write_text(exports[start:end])
            command = prefix + [
                "g++", "-std=c++17", "-O1", "-ffunction-sections",
                "-fdata-sections", "-Wl,--gc-sections", "-fno-fast-math",
                "-ffp-contract=off",
            ]
            command += ["-I" + convert(path) for path in includes]
            command += ["-I" + convert(Path(temporary))]
            command += [convert(path) for path in sources + libraries]
            command += ["-Wl,-rpath," + convert(path.parent) for path in libraries]
            command += ["-o", convert(executable)]
            compiled = subprocess.run(command, text=True, capture_output=True, timeout=120)
            self.assertEqual(compiled.returncode, 0, compiled.stdout + compiled.stderr)
            result = subprocess.run(
                prefix + [convert(executable), convert(ROOT / "port/android-native/app/src/main/assets")],
                text=True, capture_output=True, timeout=30,
            )
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn("PASS saved chooser slot0->GALCHOU/Mage slot2->slot0", result.stdout)
            print(result.stdout, end="")


if __name__ == "__main__":
    unittest.main()
