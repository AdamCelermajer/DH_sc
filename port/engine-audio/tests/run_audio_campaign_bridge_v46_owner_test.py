"""Strict Windows native test for typed V42 audio publication ownership."""
import json
import os
import pathlib
import subprocess
import sys

ROOT = pathlib.Path.cwd()
OUT = ROOT / "port/engine-audio/tests/audio_campaign_bridge_v46_owner_build"
OUT.mkdir(exist_ok=True)
BIN = ROOT / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin"
CXX = BIN / "clang++.exe"
FLAGS = [
    "-std=c++20", "-Dfinite=_finite", "-include", "exception",
    "-Wall", "-Wextra", "-Werror", "-Wno-unused-value",
    "-ffunction-sections", "-fdata-sections", "-fno-fast-math", "-ffp-contract=off",
    "-I" + str(ROOT / "port/engine-audio/integration-v42"),
    "-I" + str(ROOT / "port/android-native/app/src/main/cpp"),
    "-I" + str(ROOT / ".local-inputs/runtime-test-20261009/windows-include"),
    "-isystem", str(ROOT / "port/physics-backend/box2d-2.0.1/Include"),
]
for directory in (
    "level-world", "game-data", "engine-textures", "engine-ui", "engine-animation",
    "engine-skinning", "level-loader", "level-loader/vendor/tinyxml", "scene-materials",
    "script-runtime", "script-runtime/lua", "engine-resources", "engine-math",
    "engine-ui/vendor/gameswf1714", "windows-foundation/features/actor_frame",
):
    FLAGS.append("-I" + str(ROOT / "port" / directory))

UNITS = [
    "port/engine-audio/audio_campaign_bridge_v46.cpp",
    "port/engine-audio/integration-v42/audio_application_manager_v42.cpp",
    "port/engine-audio/integration-v42/audio_native_session_v42.cpp",
    "port/engine-audio/audio_gameplay_runtime_v42.cpp",
    "port/engine-audio/audio_gameplay_runtime_v40.cpp",
    "port/engine-audio/audio_clock_v40.cpp",
    "port/engine-audio/audio_source_bindings_v38.cpp",
    "port/engine-audio/audio_listener_rows_v38.cpp",
    "port/engine-audio/audio_source_command_v40.cpp",
    "port/engine-audio/audio_spatial_v34.cpp",
    "port/engine-audio/vox_source_fields_v38.cpp",
    "port/engine-audio/audio_catalog_v34.cpp",
    "port/engine-audio/audio_bank_v34.cpp",
    "port/engine-audio/audio_sample_v34.cpp",
    "port/engine-audio/audio_mixer_v34.cpp",
    "port/engine-audio/audio_native_envelope_v34.cpp",
    "port/engine-audio/integration-v40/focus/audio_lifecycle_gate_v40.cpp",
    "port/windows-foundation/features/audio/windows_source_session_control_v1.cpp",
    "port/windows-foundation/features/audio/winmm_output.cpp",
    "port/level-world/vox_play3d_owner_v2.cpp",
    "port/level-world/vox_music_state_owner_v1.cpp",
    "port/level-loader/native_level_application_v25.cpp",
    "port/level-loader/level_constructor_bindings_v4.cpp",
    "port/level-loader/native_gslevel_runtime_v27.cpp",
    "port/level-loader/canonical_level_context_v1.cpp",
    "port/level-world/generic_lua_script_owner_v13.cpp",
    "port/level-world/level_savegame_owner_v1.cpp",
    "port/level-world/level_savegame_runtime_v1.cpp",
    "port/windows-foundation/features/actor_frame/source_current_level_backend_v1.cpp",
]

def run(args):
    return subprocess.run([str(x) for x in args], capture_output=True, text=True)

def main():
    records = {}
    objects = []
    sources = [ROOT / "port/engine-audio/tests/audio_campaign_bridge_v46_owner_test.cpp"]
    sources += [ROOT / unit for unit in UNITS]
    for source in sources:
        obj = OUT / (source.relative_to(ROOT).as_posix().replace("/", "_").replace(".", "_") + ".o")
        result = run([CXX, *FLAGS, "-c", source, "-o", obj])
        records[source.relative_to(ROOT).as_posix()] = {"exit": result.returncode, "stderr": result.stderr}
        if result.returncode:
            print("COMPILE FAIL", source, result.stderr, file=sys.stderr)
            (OUT / "build.json").write_text(json.dumps(records, indent=2))
            return result.returncode
        objects.append(obj)

    archive = ROOT / "port/windows-foundation/features/actor_frame/source_character_owner_factory_native_build/native.a"
    foundation = ROOT / ".local-inputs/windows-foundation-build"
    libs = [archive, foundation / "libfoundation_data.a", foundation / "librecovered_content.a",
            foundation / "libcontent_xml.a", archive]
    exe = OUT / "audio_campaign_bridge_v46_owner_test.exe"
    link = run([CXX, *objects, *libs, "-Wl,--gc-sections", "-Wl,--error-limit=0",
                "-Wl,-Map," + str(OUT / "native.map"), "-lopengl32", "-luser32", "-lgdi32",
                "-lwinmm", "-o", exe])
    records["native_link"] = {"exit": link.returncode, "stderr": link.stderr}
    (OUT / "link-errors.txt").write_text(link.stderr)
    if link.returncode:
        print("LINK FAIL", link.stderr[:12000], file=sys.stderr)
        (OUT / "build.json").write_text(json.dumps(records, indent=2))
        return link.returncode

    source_data = ROOT / ".local-inputs/publication/checkpoint/session-contributions/level-loader/source/port/level-loader/tests/native-ctor-v4/design.bin"
    scripts = ROOT / ".local-inputs/publication/checkpoint/session-contributions/level-loader/source/port/level-loader/tests/native-ctor-v4/scripts"
    saves = ROOT / ".local-inputs/publication/checkpoint/session-contributions/level-loader/source/port/level-loader/tests/native-ctor-v4"
    env = dict(os.environ)
    env["PATH"] = str(BIN) + os.pathsep + env.get("PATH", "")
    runtime = subprocess.run([str(exe), str(source_data), str(scripts), str(saves)],
                             cwd=ROOT, env=env, capture_output=True, text=True)
    (OUT / "runtime.txt").write_text(runtime.stdout + runtime.stderr +
                                         "\nnative_exit=" + str(runtime.returncode) + "\n")
    records["native_runtime"] = {"exit": runtime.returncode, "stdout": runtime.stdout,
                                 "stderr": runtime.stderr}
    (OUT / "build.json").write_text(json.dumps(records, indent=2))
    print(runtime.stdout + runtime.stderr, end="")
    return runtime.returncode

if __name__ == "__main__":
    raise SystemExit(main())
