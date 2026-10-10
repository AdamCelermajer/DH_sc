"""Build and run the native source deferred-Spawn enrollment prefix.

The test links the existing canonical Character factory/data closure. It does
not open a graphics device or claim a completed InitPost/current Level.
Run from the repository root, optionally passing the original Android data
directory as the sole argument.
"""
import os
import pathlib
import subprocess
import sys

root = pathlib.Path.cwd()
feature = root / "port/windows-foundation/features/enemy_ai"
tool = root / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin"
compiler = tool / "clang++.exe"
archive = root / "port/windows-foundation/features/actor_frame/source_character_owner_factory_native_build/native.a"
foundation = root / ".local-inputs/windows-foundation-build"
output = feature / "source_summon_backend_native_test.exe"
cache = pathlib.Path(sys.argv[1]) if len(sys.argv) > 1 else root / ".local-inputs/publication/checkpoint/port/android-native/app/src/main/assets/data"

if not compiler.is_file() or not archive.is_file():
    raise SystemExit("Build the existing Windows native Character factory closure first.")
if not all((foundation / name).is_file() for name in
           ("libfoundation_data.a", "librecovered_content.a", "libcontent_xml.a")):
    raise SystemExit("Build the current Windows foundation libraries first.")

include_dirs = (
    "level-world", "game-data", "engine-textures", "engine-ui",
    "engine-animation", "engine-skinning", "level-loader",
    "level-loader/vendor/tinyxml", "scene-materials", "script-runtime",
    "script-runtime/lua", "engine-resources", "engine-math",
)
command = [str(compiler), "-std=c++20", "-Dfinite=_finite",
           "-DDH2_NATIVE_PROFILE_TRANSPORT_EMBED", "-O0", "-ffunction-sections",
           "-fdata-sections", "-fno-fast-math", "-ffp-contract=off", "-w",
           "-include", "exception"]
command.extend("-I" + str(root / "port" / directory) for directory in include_dirs)
command.extend(("-isystem", str(root / "port/physics-backend/box2d-2.0.1/Include"),
                "-I" + str(root / "port"),
                str(feature / "source_summon_backend_native_test.cpp"),
                str(feature / "source_canonical_spawn_adapter.cpp"), str(archive),
                str(foundation / "libfoundation_data.a"),
                str(foundation / "librecovered_content.a"),
                str(foundation / "libcontent_xml.a"), str(archive),
                "-Wl,--gc-sections", "-lopengl32", "-luser32", "-lgdi32",
                "-o", str(output)))
subprocess.run(command, check=True)
environment = dict(os.environ)
environment["PATH"] = str(tool) + os.pathsep + environment.get("PATH", "")
subprocess.run((str(output), str(cache)), check=True, env=environment)
