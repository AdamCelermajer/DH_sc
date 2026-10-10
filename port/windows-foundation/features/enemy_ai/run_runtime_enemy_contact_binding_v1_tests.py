"""Compile and run the same-CombatSession enemy/contact composition regression."""
from __future__ import annotations

import os
import pathlib
import subprocess

root = pathlib.Path.cwd()
feature = root / "port/windows-foundation/features/enemy_ai"
tool = root / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin"
compiler = tool / "clang++.exe"
shared = root / ".local-inputs/windows-foundation-build"
build = root / ".local-inputs/runtime-enemy-contact-binding-v1-build"
build.mkdir(parents=True, exist_ok=True)
output = build / "runtime_enemy_contact_binding_v1_tests.exe"
assets = root / ".local-inputs/windows-shared-assets"

if not compiler.is_file() or not (shared / "libfoundation_data.a").is_file():
    raise SystemExit("Build the coherent Windows foundation libraries before running this test.")

includes = (
    "windows-foundation", "game-data", "level-world", "level-loader",
    "scene-materials", "engine-textures", "engine-ui", "engine-animation",
    "engine-skinning", "engine-resources", "engine-math", "script-runtime",
    "script-runtime/lua",
)
command = [str(compiler), "-std=c++17", "-O2", "-DNDEBUG", "-Wall", "-Wextra",
           "-Werror", "-Wno-missing-field-initializers", "-Dfinite=_finite", "-static"]
command.extend("-I" + str(root / "port" / directory) for directory in includes)
command.extend(("-isystem", str(root / "port/physics-backend/box2d-2.0.1/Include")))
command.extend((
    str(feature / "runtime_enemy_contact_binding_v1_tests.cpp"),
    str(feature / "runtime_enemy_contact_binding_v1.cpp"),
    str(feature / "runtime_enemy_controller_v1.cpp"),
    str(feature / "monster_decisions.cpp"),
    str(root / "port/level-world/character_script_collision.cpp"),
    str(root / "port/level-world/character_cancel_sneaking.cpp"),
    str(root / "port/level-world/character_sneaking_tables.cpp"),
    str(root / "port/game-data/skill_tables.cpp"),
    str(root / "port/windows-foundation/features/physics/runtime_session_contact_v1.cpp"),
    # Compile the current Session implementation into this isolated executable
    # so lifetime_lease is tested without modifying/rebuilding shared archives.
    str(root / "port/windows-foundation/combat_session.cpp"),
    str(shared / "libfoundation_data.a"),
    str(shared / "libcontent_xml.a"),
    str(shared / "libdh2_freetype237.a"),
    str(shared / "librecovered_trigger_contacts.a"),
    str(shared / "librecovered_content.a"),
    str(shared / "physics-backend/libdh2_box2d_201.a"),
    "-Wl,--gc-sections", "-lkernel32", "-luser32", "-lgdi32", "-lwinspool",
    "-lshell32", "-lole32", "-loleaut32", "-luuid", "-lcomdlg32",
    "-ladvapi32", "-o", str(output),
))
subprocess.run(command, check=True)
environment = dict(os.environ)
environment["PATH"] = str(tool) + os.pathsep + environment.get("PATH", "")
subprocess.run((str(output), str(assets)), check=True, env=environment)
