from pathlib import Path
import hashlib
import json
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[4]
FEATURE = ROOT / "port/windows-foundation/features/effects"
BUILD = ROOT / ".local-inputs/windows-effects-feature-wgl"
BUILD.mkdir(parents=True, exist_ok=True)
TEST = FEATURE / "runtime_effects_factory_wgl_smoke.cpp"
OUTPUT = BUILD / "runtime-effects-factory-wgl-smoke.exe"
COMPILER = ROOT / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe"
NATIVE = ROOT / "port/windows-foundation/features/actor_frame/source_character_owner_factory_native_build/native.a"
FOUNDATION = ROOT / ".local-inputs/windows-foundation-build"
ASSETS = ROOT / ".local-inputs/windows-shared-assets"
TABLES = ROOT / "port/game-data/reference/effects-tables"
SOURCE_EFFECT = ROOT / ".local-inputs/character-fx-owner-v1/cache/swoosh_prince_1hand_combo_01.bdae"
SOURCE_TEXTURE = ASSETS / "data/3d/textures/fx_weapon_trail_blue_gradual.tga"
SOURCE_URI_ROOT = BUILD / "source-effect-uri-root"
VIRTUAL_EFFECT = SOURCE_URI_ROOT / "data/3d/interface/swoosh_prince_1hand_combo_01.bdae"
VIRTUAL_TEXTURE = SOURCE_URI_ROOT / "data/3d/textures/fx_weapon_trail_blue_gradual.tga"
FRAMEBUFFER = BUILD / "runtime-effects-factory-wgl-smoke.ppm"


def require(value, message):
    if not value:
        raise SystemExit(f"FAIL {message}")


def link_exact(source: Path, destination: Path, label: str):
    require(source.is_file(), f"missing actual source input: {source}")
    destination.parent.mkdir(parents=True, exist_ok=True)
    if not destination.exists():
        destination.hardlink_to(source)
    require(hashlib.sha256(source.read_bytes()).digest() ==
            hashlib.sha256(destination.read_bytes()).digest(),
            f"{label} virtual URI changed original asset bytes")


require(COMPILER.exists(), "LLVM-MinGW compiler missing")
require(NATIVE.exists(), "root source_character_owner_factory_native archive missing")
require((FOUNDATION / "libfoundation_data.a").exists(),
        "Windows Foundation COFF archive missing")
link_exact(SOURCE_EFFECT, VIRTUAL_EFFECT, "original effect resource")
link_exact(SOURCE_TEXTURE, VIRTUAL_TEXTURE, "original referenced texture")

source_files = [
    TEST,
    FEATURE / "runtime_effects_factory_v1.cpp",
    FEATURE / "runtime_source_fx_asset_v1.cpp",
    FEATURE / "runtime_combat_effects_v1.cpp",
    FEATURE / "effects_executor.cpp",
    FEATURE / "effects_render_bridge.cpp",
    FEATURE / "effects_material_binding.cpp",
    ROOT / "port/windows-foundation/platform_win32.cpp",
    ROOT / "port/windows-foundation/renderer.cpp",
    ROOT / "port/level-world/character_authored_resource_v32.cpp",
    ROOT / "port/level-world/character_authored_fx_forces_v4.cpp",
    ROOT / "port/level-world/character_fx_floor_query_v3.cpp",
    ROOT / "port/level-world/character_animation_step_fx_v2.cpp",
    ROOT / "port/level-world/authored_fx_mesh_graph_v32.cpp",
    ROOT / "port/level-world/authored_fx_nonrender_geometry_v32.cpp",
    ROOT / "port/level-world/authored_fx_transform_v5.cpp",
    ROOT / "port/scene-materials/particle_scene_v1.cpp",
    ROOT / "port/engine-skinning/skinning.cpp",
    ROOT / "port/engine-animation/animation.cpp",
    ROOT / "port/engine-animation/component_applicator.cpp",
]
source_files += [ROOT / "port/engine-animation" / (name + ".cpp") for name in [
    "particle_scalar_animation_v6", "particle_cloud_runtime_v1",
    "particle_cloud_runtime_v3", "particle_deflector_v1",
    "particle_force_scene_v2", "particle_bound_forces_v4",
    "particle_resource_init_v2", "particle_cloud_models_v1",
    "particle_billboard_v1", "particle_force_scene_v1",
    "particle_scene_color_v1", "particle_factory", "particle_random_v1",
    "particle_emission", "particle_box_v2", "material_color",
    "material_color_v3", "particle_parameter", "particle_resource_init_v32",
    "particle_billboard_v32",
]]
source_files += [ROOT / "port/windows-foundation" / (name + ".cpp") for name in [
    "asset_catalog", "content_paths", "texture_loader", "source_material_pass",
    "actor_lighting",
]]
source_files += [ROOT / "port/engine-textures" / (name + ".cpp") for name in [
    "textures", "pvrtc",
]]
source_files += [ROOT / "port/scene-materials" / (name + ".cpp") for name in [
    "scene", "effect_render_pass_v4",
]]
source_files += [ROOT / "port/level-loader/vendor/tinyxml" / (name + ".cpp") for name in [
    "tinyxml", "tinyxmlerror", "tinyxmlparser", "tinystr",
]]
for path in source_files:
    require(path.exists(), f"source file missing: {path}")

includes = []
for relative in [
    "port/windows-foundation", "port/level-world", "port/scene-materials",
    "port/engine-resources", "port/game-data", "port/engine-animation",
    "port/engine-skinning", "port/engine-math", "port/engine-textures",
    "port/asset-payloads", "port/level-loader/vendor/tinyxml", "port/physics-backend",
]:
    includes += ["-I", str(ROOT / relative)]

link_libs = [
    NATIVE,
    FOUNDATION / "libfoundation_data.a",
    FOUNDATION / "libcontent_xml.a",
    FOUNDATION / "libdh2_freetype237.a",
    FOUNDATION / "librecovered_trigger_contacts.a",
    FOUNDATION / "librecovered_content.a",
    FOUNDATION / "physics-backend/libdh2_box2d_201.a",
]
command = [str(COMPILER), "-std=c++17", "-O1", "-g", "-static", "-Wall", "-Wextra",
           "-Werror", "-Wno-missing-field-initializers", "-Wno-misleading-indentation", *includes,
           *map(str, source_files), "-Wl,--start-group", *map(str, link_libs),
           "-Wl,--end-group", "-lopengl32", "-lkernel32", "-luser32", "-lgdi32",
           "-lwinspool", "-lshell32", "-lole32", "-loleaut32", "-luuid",
           "-lcomdlg32", "-ladvapi32", "-o", str(OUTPUT)]
build = subprocess.run(command, cwd=ROOT, text=True, capture_output=True)
(BUILD / "build.log").write_text(build.stdout + build.stderr)
require(build.returncode == 0, build.stdout + build.stderr)
if "--compile-only" in sys.argv:
    print(json.dumps({"validation": "COMPILED", "exe": str(OUTPUT.relative_to(ROOT))}))
    raise SystemExit(0)

run = subprocess.run([str(OUTPUT), str(ROOT), str(TABLES), str(SOURCE_URI_ROOT), str(FRAMEBUFFER)],
                     cwd=ROOT, text=True, capture_output=True)
(BUILD / "run.log").write_text(run.stdout + run.stderr)
require(run.returncode == 0, run.stdout + run.stderr)
try:
    result = json.loads(run.stdout.strip().splitlines()[-1])
except Exception as error:
    raise SystemExit(f"FAIL WGL smoke emitted no JSON result: {error}\n{run.stdout}")
require(result.get("validation") == "PASS" and result.get("changed_pixels", 0) > 10,
        f"WGL source FX render/readback failed: {result}")
require(FRAMEBUFFER.is_file() and FRAMEBUFFER.stat().st_size > 32,
        "WGL framebuffer capture output missing")

archive_hashes = {str(path.relative_to(ROOT)).replace("\\", "/"):
                  hashlib.sha256(path.read_bytes()).hexdigest() for path in link_libs}
report = {
    "feature": "RuntimeEffectsFactoryV1 WGL smoke",
    "validation": "PASS",
    "runner": "port/windows-foundation/features/effects/runtime_effects_factory_wgl_smoke.py",
    "test": "port/windows-foundation/features/effects/runtime_effects_factory_wgl_smoke.cpp",
    "source_result": result,
    "archives_sha256": archive_hashes,
    "original_assets_sha256": {
        "resource": hashlib.sha256(SOURCE_EFFECT.read_bytes()).hexdigest(),
        "texture": hashlib.sha256(SOURCE_TEXTURE.read_bytes()).hexdigest(),
    },
    "framebuffer": str(FRAMEBUFFER.relative_to(ROOT)).replace("\\", "/"),
    "limits": {
        "native_wgl_and_root_renderer": True,
        "real_original_texture_upload": True,
        "gl_finish_and_readback": True,
        "changed_pixels": result["changed_pixels"],
        "production_host_scene_manager_queue": False,
        "same_frame_test_queue_retention_and_drain": True,
        "mesh_only_source_set_camera_or_particle_driver_invocation": False,
    },
}
(FEATURE / "runtime-effects-factory-wgl-report.json").write_text(json.dumps(report, indent=2) + "\n")
print(json.dumps(report, indent=2))
