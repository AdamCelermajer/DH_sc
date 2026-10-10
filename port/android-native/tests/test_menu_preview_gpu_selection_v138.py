"""Shipping menu GPU draw/cache regression, with authored assets and GL recorder.

This extends the actual creation draw/PCLS/module regression by executing the
production final menu GPU admission and upload functions verbatim. Character
allocation and GL remain host endpoints; APK visual confirmation is separate.
"""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
HOST_BUILD = Path(os.environ.get("DH2_CLASS_PREVIEW_HOST_BUILD", str(ROOT / ".local-inputs/native-host-v128/build/native")))

class MenuPreviewGpuSelectionTest(unittest.TestCase):
    def test_production_gpu_selection_and_empty_retirement(self):
        libraries = [HOST_BUILD / p for p in ("level-world/game-data/libdh2_game_data.so", "scene-materials/libdh2_scene_materials.so", "engine-animation/libdh2_engine_animation.so", "engine-skinning/libdh2_engine_skinning.so")]
        for library in libraries:
            self.assertTrue(library.is_file(), str(library))
        prefix = [] if shutil.which("g++") else ["wsl", "--exec"]
        def convert(path):
            if not prefix:
                return str(path)
            return subprocess.check_output(prefix + ["wslpath", "-a", path.as_posix()], text=True).strip()
        sources = [ROOT / "port" / p for p in ("android-native/tests/menu_preview_gpu_selection_v138.cpp", "level-world/objects.cpp", "level-world/character_props_id_owner_v1.cpp", "engine-ui/menu_avatar_preview_v1.cpp", "game-data/class_preview_setup.cpp", "game-data/menu_profile_metadata_v1.cpp", "game-data/fresh_player_profile_v1.cpp", "engine-resources/resource_budget_v37.cpp")]
        includes = [ROOT / "port" / p for p in ("engine-ui", "game-data", "level-world", "engine-animation", "engine-resources", "engine-math", "engine-skinning", "scene-materials")]
        cpp = ROOT / "port/android-native/app/src/main/cpp"
        with tempfile.TemporaryDirectory(prefix="dh2-preview-gpu-") as temporary:
            temp = Path(temporary)
            exports = (cpp / "renderer_front_exports_v87.inc").read_text()
            (temp / "menu_preview_class_transition_under_test.inc").write_text(exports[exports.index("bool select_class_scene("):exports.index("void draw_class_scene(")])
            renderer = (cpp / "model_renderer.cpp").read_text()
            (temp / "menu_preview_gpu_sync_under_test.inc").write_text(renderer[renderer.index("void sync_visual_draws_v64("):renderer.index("void sync_equipment_draws(")])
            gpu = (cpp / "renderer_menu_geometry_v123.inc").read_text()
            (temp / "menu_preview_gpu_draw_under_test.inc").write_text(gpu[gpu.index("void draw_native_menu_character_v123("):])
            executable = temp / "preview-gpu"
            command = prefix + ["g++", "-std=c++17", "-O1", "-ffunction-sections", "-fdata-sections", "-Wl,--gc-sections", "-fno-fast-math", "-ffp-contract=off"]
            command += ["-I" + convert(p) for p in includes + [temp]]
            command += [convert(p) for p in sources + libraries]
            command += ["-Wl,-rpath," + convert(p.parent) for p in libraries] + ["-o", convert(executable)]
            result = subprocess.run(command, text=True, capture_output=True, timeout=120)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            result = subprocess.run(prefix + [convert(executable), convert(ROOT / "port/android-native/app/src/main/assets")], text=True, capture_output=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn("PASS production final GPU selection", result.stdout)
            print(result.stdout, end="")

if __name__ == "__main__":
    unittest.main()
