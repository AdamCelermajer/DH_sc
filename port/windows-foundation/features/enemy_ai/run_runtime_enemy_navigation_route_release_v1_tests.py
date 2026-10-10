"""Build the route-release Session regression against private copies of the terminal archives."""
from __future__ import annotations

import os
import pathlib
import shutil
import subprocess

root = pathlib.Path.cwd()
feature = root / "port/windows-foundation/features/enemy_ai"
tool = root / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin"
source_build = root / ".local-inputs/windows-foundation-build"
private_build = root / ".local-inputs/runtime-enemy-navigation-route-release/private-libs"
compiler = tool / "clang++.exe"
output = root / ".local-inputs/runtime-enemy-navigation-route-release/runtime_enemy_navigation_route_release_v1_tests.exe"
assets = root / ".local-inputs/windows-shared-assets"

libraries = (
    "libfoundation_data.a",
    "libcontent_xml.a",
    "libdh2_freetype237.a",
    "librecovered_trigger_contacts.a",
    "librecovered_content.a",
    "physics-backend/libdh2_box2d_201.a",
)
if not compiler.is_file():
    raise SystemExit("Windows LLVM toolchain is unavailable.")
for relative in libraries:
    source = source_build / relative
    if not source.is_file():
        raise SystemExit(f"Terminal archive is missing: {source}")
    destination = private_build / relative
    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, destination)

output.parent.mkdir(parents=True, exist_ok=True)
includes = (
    "level-world", "game-data", "engine-textures", "engine-ui",
    "engine-animation", "engine-skinning", "level-loader",
    "level-loader/vendor/tinyxml", "scene-materials", "script-runtime",
    "script-runtime/lua", "engine-resources", "engine-math",
)
command = [str(compiler), "-std=c++20", "-Dfinite=_finite",
           "-DDH2_NATIVE_PROFILE_TRANSPORT_EMBED", "-O0", "-ffunction-sections",
           "-fdata-sections", "-fno-fast-math", "-ffp-contract=off", "-w",
           "-include", "exception"]
command.extend("-I" + str(root / "port" / directory) for directory in includes)
command.extend(("-isystem", str(root / "port/physics-backend/box2d-2.0.1/Include"),
                "-I" + str(root / "port"),
                str(feature / "runtime_enemy_controller_v1_tests.cpp"),
                str(feature / "runtime_enemy_controller_v1.cpp"),
                str(feature / "runtime_enemy_navigation_v1.cpp"),
                str(feature / "runtime_monster_level_policy_v1.cpp"),
                str(feature / "monster_decisions.cpp"),
                str(root / "port/game-data/level_tables.cpp"),
                str(root / "port/windows-foundation/playable_actor_bodies.cpp"),
                str(root / "port/windows-foundation/original_actor_navigation.cpp"),
                str(root / "port/windows-foundation/features/physics/runtime_session_contact_v1.cpp"),
                str(root / "port/level-world/character_script_collision.cpp"),
                str(root / "port/level-world/character_cancel_sneaking.cpp"),
                str(root / "port/level-world/character_sneaking_tables.cpp"),
                str(private_build / "libfoundation_data.a"),
                str(private_build / "libcontent_xml.a"),
                str(private_build / "libdh2_freetype237.a"),
                str(private_build / "librecovered_trigger_contacts.a"),
                str(private_build / "librecovered_content.a"),
                str(private_build / "physics-backend/libdh2_box2d_201.a"),
                "-Wl,--gc-sections", "-lkernel32", "-luser32", "-lgdi32",
                "-lwinspool", "-lshell32", "-lole32", "-loleaut32", "-luuid",
                "-lcomdlg32", "-ladvapi32", "-o", str(output)))
subprocess.run(command, check=True)
environment = dict(os.environ)
environment["PATH"] = str(tool) + os.pathsep + environment.get("PATH", "")
subprocess.run((str(output), str(assets), str(root)), check=True, env=environment)
