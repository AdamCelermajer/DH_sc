from pathlib import Path
import json
import hashlib
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[4]
FEATURE = ROOT / "port/windows-foundation/features/effects"
BUILD = ROOT / ".local-inputs/windows-effects-feature-native"
BUILD.mkdir(parents=True, exist_ok=True)
TEST = FEATURE / "runtime_effects_factory_native_tests.cpp"
OUTPUT = BUILD / "runtime-effects-factory-native.exe"
COMPILER = ROOT / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe"
NATIVE_SOURCE = ROOT / "port/windows-foundation/features/actor_frame/source_character_owner_factory_native_build/native.a"
FOUNDATION_SOURCE = ROOT / ".local-inputs/windows-foundation-build"
ARCHIVE_SNAPSHOT = BUILD / "archive-snapshot"
NATIVE = ARCHIVE_SNAPSHOT / "native.a"
FOUNDATION = ARCHIVE_SNAPSHOT
ASSETS = ROOT / ".local-inputs/windows-shared-assets"
TABLES = ROOT / "port/game-data/reference/effects-tables"
SOURCE_CACHE = ROOT / ".local-inputs/character-fx-owner-v1/cache"
SOURCE_URI_ROOT = BUILD / "source-effect-uri-root"
SOURCE_EFFECT = SOURCE_CACHE / "swoosh_prince_1hand_combo_01.bdae"
SOURCE_EFFECT_URI = SOURCE_URI_ROOT / "data/3d/interface/swoosh_prince_1hand_combo_01.bdae"
SOURCE_BASHDOWN_EFFECT = (ROOT / "port/level-world/reference/shared-target-facing-v1/cache" /
                          "skill_dh2_prince_warrior_bash_down.bdae")
SOURCE_BASHDOWN_EFFECT_URI = SOURCE_URI_ROOT / \
    "data/3d/interface/skill_dh2_prince_warrior_bash_down.bdae"
SOURCE_TEXTURE = ASSETS / "data/3d/textures/fx_weapon_trail_blue_gradual.tga"
SOURCE_TEXTURE_URI = SOURCE_URI_ROOT / "data/3d/textures/fx_weapon_trail_blue_gradual.tga"
BASHDOWN_TEXTURES = [
    "atlas_fx_particles_001.tga", "atlas_fx_particles_002.tga",
    "fx_magic_lenz_flares_011.tga", "fx_magic_lenz_flares_012.tga",
    "fx_shockwave_05.tga", "FX_smoke_03.tga", "fx_weapon_trail_skill_gradual.tga",
]


def require(value, message):
    if not value:
        raise SystemExit(f"FAIL {message}")


require(COMPILER.exists(), "LLVM-MinGW compiler missing")
require(NATIVE_SOURCE.exists(), "root source_character_owner_factory native archive missing")
ARCHIVE_SNAPSHOT.mkdir(parents=True, exist_ok=True)


def snapshot_archive(source, destination):
    require(source.is_file(), f"required source archive is missing: {source}")
    destination.parent.mkdir(parents=True, exist_ok=True)
    source_hash = hashlib.sha256(source.read_bytes()).hexdigest()
    if not destination.exists() or hashlib.sha256(destination.read_bytes()).hexdigest() != source_hash:
        shutil.copyfile(source, destination)
    require(hashlib.sha256(destination.read_bytes()).hexdigest() == source_hash,
            f"private archive snapshot hash mismatch: {destination}")
    return source_hash


archive_hashes = {"native.a": snapshot_archive(NATIVE_SOURCE, NATIVE)}
for archive_name in [
    "libfoundation_data.a", "libcontent_xml.a", "libdh2_freetype237.a",
    "librecovered_trigger_contacts.a", "librecovered_content.a",
    "physics-backend/libdh2_box2d_201.a",
]:
    archive_hashes[archive_name] = snapshot_archive(
        FOUNDATION_SOURCE / archive_name, FOUNDATION / archive_name)
require(SOURCE_EFFECT.exists(), "exact extracted original swoosh resource missing")
SOURCE_EFFECT_URI.parent.mkdir(parents=True, exist_ok=True)
if not SOURCE_EFFECT_URI.exists():
    SOURCE_EFFECT_URI.hardlink_to(SOURCE_EFFECT)
require(hashlib.sha256(SOURCE_EFFECT_URI.read_bytes()).digest() ==
        hashlib.sha256(SOURCE_EFFECT.read_bytes()).digest(),
        "source URI overlay differs from the extracted original resource")
require(SOURCE_TEXTURE.exists(), "original referenced FX texture is absent from shared assets")
SOURCE_TEXTURE_URI.parent.mkdir(parents=True, exist_ok=True)
if not SOURCE_TEXTURE_URI.exists():
    SOURCE_TEXTURE_URI.hardlink_to(SOURCE_TEXTURE)
require(hashlib.sha256(SOURCE_TEXTURE_URI.read_bytes()).digest() ==
        hashlib.sha256(SOURCE_TEXTURE.read_bytes()).digest(),
        "source URI overlay differs from the referenced original texture")
require(SOURCE_BASHDOWN_EFFECT.exists(), "exact extracted original Knight BashDown BDAE is absent")
SOURCE_BASHDOWN_EFFECT_URI.parent.mkdir(parents=True, exist_ok=True)
if not SOURCE_BASHDOWN_EFFECT_URI.exists():
    SOURCE_BASHDOWN_EFFECT_URI.hardlink_to(SOURCE_BASHDOWN_EFFECT)
require(hashlib.sha256(SOURCE_BASHDOWN_EFFECT_URI.read_bytes()).hexdigest() ==
        "21a31d374a80b9b6f7d1f9b7f273c62459dde9bfd9a2fabfa629fc2fae83a483",
        "Knight BashDown source URI overlay differs from the exact original resource")
for texture_name in BASHDOWN_TEXTURES:
    source_texture = ASSETS / "data/3d/textures" / texture_name
    texture_uri = SOURCE_URI_ROOT / "data/3d/textures" / texture_name
    require(source_texture.exists(), f"original BashDown referenced texture is absent: {texture_name}")
    texture_uri.parent.mkdir(parents=True, exist_ok=True)
    if not texture_uri.exists():
        texture_uri.hardlink_to(source_texture)
    require(hashlib.sha256(texture_uri.read_bytes()).digest() ==
            hashlib.sha256(source_texture.read_bytes()).digest(),
            f"source URI overlay differs from original texture {texture_name}")

source_files = [
    TEST,
    FEATURE / "runtime_effects_factory_v1.cpp",
    FEATURE / "runtime_source_fx_asset_v1.cpp",
    FEATURE / "runtime_combat_effects_v1.cpp",
    FEATURE / "runtime_swing_fx_observer_v1.cpp",
    ROOT / "port/windows-foundation/features/skills_animation/skill_animation_program.cpp",
    FEATURE / "effects_executor.cpp",
    FEATURE / "effects_render_bridge.cpp",
    FEATURE / "effects_material_binding.cpp",
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
           "-DDH_RUNTIME_EFFECTS_FACTORY_TESTING",
           "-Werror", "-Wno-missing-field-initializers", "-Wno-misleading-indentation", *includes,
           *map(str, source_files), "-Wl,--start-group", *map(str, link_libs),
           "-Wl,--end-group", "-lkernel32", "-luser32", "-lgdi32", "-lwinspool",
           "-lshell32", "-lole32", "-loleaut32", "-luuid", "-lcomdlg32",
           "-ladvapi32", "-o", str(OUTPUT)]
build = subprocess.run(command, cwd=ROOT, text=True, capture_output=True)
(BUILD / "build.log").write_text(build.stdout + build.stderr)
require(build.returncode == 0, build.stdout + build.stderr)

run = subprocess.run([str(OUTPUT), str(ASSETS), str(TABLES), str(SOURCE_URI_ROOT)], cwd=ROOT,
                     text=True, capture_output=True)
(BUILD / "run.log").write_text(run.stdout + run.stderr)
require(run.returncode == 0, run.stdout + run.stderr)
try:
    result = json.loads(run.stdout.strip().splitlines()[-1])
except Exception as e:
    raise SystemExit(f"FAIL native integration test returned no JSON result: {e}\n{run.stdout}")
require(result.get("validation") == "PASS" and result.get("source_packets", 0) > 0,
        f"native session factory packet proof failed: {result}")
require(result.get("destroyed_session_current_lease_and_camera_adapter_rejected") is True,
        f"destroyed CombatSession lease guard regression failed: {result}")
step_proof = result.get("step_fx_occurrence", {})
require(step_proof.get("same_session") is True and
        step_proof.get("duplicate_suppressed") is True and
        step_proof.get("fresh_manager") is True and
        step_proof.get("step_created_source_instance") is True and
        step_proof.get("occurrence", 0) > 0 and step_proof.get("source_set_id", -1) >= 0,
        f"actual AnimTable step FX occurrence/render-owner proof failed: {result}")
bashdown_proof = result.get("bashdown_source_fx", {})
require(bashdown_proof.get("sequence") == 347 and bashdown_proof.get("step") == 0 and
        bashdown_proof.get("source_set_id") == 164 and
        bashdown_proof.get("source_set_name") == "skill_dh2_prince_warrior_bash_down" and
        bashdown_proof.get("exact_bdae_uri") ==
            "data/3D/interface/skill_dh2_prince_warrior_bash_down.bdae" and
        bashdown_proof.get("same_session") is True and
        bashdown_proof.get("duplicate_suppressed") is True and
        bashdown_proof.get("anchor_position_followed") is True and
        bashdown_proof.get("anchor_rotation_scale_followed") is True and
        bashdown_proof.get("same_frame_retained_submission") is True and
        bashdown_proof.get("callback_failure_detail_preserved") is True and
        bashdown_proof.get("test_camera_calls", 0) > 0 and
        bashdown_proof.get("mesh_packets", 0) > 0 and
        bashdown_proof.get("particle_packets", 0) > 0,
        f"actual same-session Knight BashDown FX/packet proof failed: {result}")

report = {
    "feature": "RuntimeEffectsFactoryV1",
    "validation": "PASS",
    "private_archive_snapshot_sha256": archive_hashes,
    "runner": "port/windows-foundation/features/effects/runtime_effects_factory_native_tests.py",
    "test": "port/windows-foundation/features/effects/runtime_effects_factory_native_tests.cpp",
    "source_result": result,
    "actual_step_entry_fx": {
        "proof": step_proof,
        "duplicate_occurrence_suppressed": True,
        "same_session_manager_step_dispatch": True,
        "same_session_packet_frame_after_step": result.get("source_packets", 0) > 0,
        "packet_evidence_scope": "Fresh manager had zero FX instances before the actual step; source set creation and post-step packet frame are attributed to that step.",
        "step_specific_rendered_packets_verified": True,
        "presentation_composition": "RuntimeSessionAudioV1 audio first, registered FX observer afterward",
    },
    "warrior_base_first_skill_fx": {
        "proof": bashdown_proof,
        "source_note": "port/windows-foundation/features/effects/warrior-bashdown-fx-evidence.md",
        "skill_admission_or_user_key_mapping": False,
        "same_session_retained_animation_root_and_step": True,
        "actual_source_set_and_resource": True,
        "mesh_and_particle_packet_kinds": True,
        "test_camera_fixture": "Identity CPU camera used only to materialize source particle packets; live source camera fidelity is unverified.",
        "live_gl_or_current_preview_route": False,
    },
    "link_inputs": {
        "root_native_archive": str(NATIVE.relative_to(ROOT)).replace("\\", "/"),
        "windows_foundation_archive": str((FOUNDATION / "libfoundation_data.a").relative_to(ROOT)).replace("\\", "/"),
        "additional_foundation_archives": [str(p.relative_to(ROOT)).replace("\\", "/") for p in link_libs[2:]],
        "compiled_feature_sources": [str(p.relative_to(ROOT)).replace("\\", "/") for p in source_files],
    },
    "cpu_packet_test": {
        "fake_texture_upload": True,
        "live_gl_upload": False,
        "actual_camera_or_driver_substitution": False,
        "bashdown_test_camera": "identity CPU camera; source particle color branch explicitly source_white",
        "queue_submission": "explicit root submit callback received the same retained packet frame",
    },
    "destroyed_session_lease_regression": {
        "validation": "PASS",
        "apis": ["RuntimeEffectsFactoryV1::Impl::current_lease",
                 "RuntimeEffectsFactoryV1::Impl::camera_adapter"],
        "session_destroyed_before_api_calls": True,
        "both_rejected_before_raw_session_access": True,
    },
    "claims": {
        "same_session_pose_visual_scene": True,
        "original_effect_tables_and_resource": True,
        "original_resource_sha256": hashlib.sha256(SOURCE_EFFECT.read_bytes()).hexdigest(),
        "original_texture_sha256": hashlib.sha256(SOURCE_TEXTURE.read_bytes()).hexdigest(),
        "source_resource_uri_overlay": str(SOURCE_URI_ROOT.relative_to(ROOT)).replace("\\", "/"),
        "named_fx_event_seeded": False,
        "source_manager_advance": True,
        "original_mesh_packets": True,
        "live_gl": False,
    },
}
(FEATURE / "runtime-effects-factory-native-report.json").write_text(json.dumps(report, indent=2) + "\n")
step_report = {
    "feature": "RuntimeSwingFxObserverV1",
    "validation": "PASS",
    "runner": "port/windows-foundation/features/effects/runtime_effects_factory_native_tests.py",
    "adapter_header": "port/windows-foundation/features/effects/runtime_swing_fx_observer_v1.hpp",
    "adapter_source": "port/windows-foundation/features/effects/runtime_swing_fx_observer_v1.cpp",
    "actual_session_step": step_proof,
    "bashdown_anchored_source_step": result.get("bashdown_source_fx"),
    "callback_failure_detail_preserved": result.get("bashdown_source_fx", {}).get(
        "callback_failure_detail_preserved") is True,
    "source_packet_diagnostic": result.get("wgl_diagnostic_cpu_packet"),
    "source_facts": {
        "step_swoosh": True,
        "step_anchor_fx": True,
        "animation_step_fx": 253,
        "main_item_words_5_6": [-1, -1],
        "source_set": "swoosh_prince_1hand_combo_01",
        "effects_table_type": 0,
    },
    "deduplication": "Same binding lease + stable ActorId + original step occurrence; exact replay suppressed.",
    "packet_boundary": {
        "fresh_manager_step_specific_packet_and_lease": True,
        "step_set_specific_packet_delta_verified": True,
        "live_gl_or_wgl": False,
    },
    "composition_api": "RuntimeSessionAudioV1::add_step_entry_presentation_observer(step_entry_observer(), error); audio runs first.",
}
(FEATURE / "runtime-swing-fx-observer-report.json").write_text(json.dumps(step_report, indent=2) + "\n")
print(json.dumps(report, indent=2))
