"""Host GLES comparison of actual menu material data and original GLSL.

Synthetic clip positions, UVs and one-pixel texture samples isolate material
transport. This is not an integrated Android/reference screenshot acceptance.
"""
from pathlib import Path
import hashlib
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"
ASSETS = ROOT / "port/android-native/app/src/main/assets"


class MenuBackdropMaterialTest(unittest.TestCase):
    def test_original_menu_material_pixels(self):
        source = (CPP / "renderer_menu_geometry_v123.inc").read_text()
        block = source.split("// BEGIN menu-background-material-v124 host extraction\n", 1)[1].split("// END menu-background-material-v124 host extraction", 1)[0]
        self.assertIn("prepare_menu_background_materials_v124(app);", source)
        self.assertNotIn(".584", block)  # Value is decoded from the asset.
        generic = (CPP / "model_renderer.cpp").read_text()
        shader = generic[generic.index("void create_program("):]
        vs = shader.split('const char* vs=R"(', 1)[1].split(')";', 1)[0]
        fs = shader.split('const char* fs=R"(', 1)[1].split(')";', 1)[0]
        with tempfile.TemporaryDirectory(prefix="dh2-menu-material-") as directory:
            temp = Path(directory)
            (temp / "menu_material_under_test.inc").write_text(block)
            (temp / "generic-vs.glsl").write_text(vs)
            (temp / "generic-fs.glsl").write_text(fs)
            prefix = [] if shutil.which("g++") else ["wsl", "--exec"]

            def path(value):
                return str(value) if not prefix else subprocess.check_output(prefix + ["wslpath", "-a", value.as_posix()], text=True).strip()

            executable = temp / "menu-material"
            sources = [ROOT / "port/android-native/tests/menu_background_material_v124.cpp"] + [ROOT / "port" / file for file in (
                "scene-materials/scene.cpp", "engine-resources/resources.cpp", "asset-payloads/payloads.cpp", "engine-math/math.cpp", "level-world/native_batch_material_values_v113.cpp")]
            command = prefix + ["g++", "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-Wno-misleading-indentation", "-fno-fast-math", "-ffp-contract=off", "-I" + path(temp), "-I" + path(ROOT / "port/scene-materials"), "-I" + path(ROOT / "port/level-world")]
            result = subprocess.run(command + [path(p) for p in sources] + ["-lEGL", "-lGLESv2", "-o", path(executable)], capture_output=True, text=True, timeout=60)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            result = subprocess.run(prefix + ["env", "EGL_PLATFORM=surfaceless", "LIBGL_ALWAYS_SOFTWARE=1", path(executable), path(ASSETS), path(temp)], capture_output=True, text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            print(result.stdout, end="")
            for file in (ASSETS / "models/main_menu_charactere_swamp.bdae", CPP / "renderer_menu_geometry_v123.inc"):
                print("source", file.relative_to(ROOT), hashlib.sha256(file.read_bytes()).hexdigest())


if __name__ == "__main__":
    unittest.main()
