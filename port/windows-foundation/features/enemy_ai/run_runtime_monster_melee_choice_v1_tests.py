"""Execute the actual monster.luac melee branch against the source policy kernel."""
from __future__ import annotations

import os
import pathlib
import subprocess

root = pathlib.Path.cwd()
feature = root / "port/windows-foundation/features/enemy_ai"
tool = root / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin"
build = root / ".local-inputs/windows-foundation-build"
compiler = tool / "clang++.exe"
c_compiler = tool / "clang.exe"
output = feature / "runtime_monster_melee_choice_v1_tests.exe"
decisions_output = feature / "runtime_monster_decisions_source_tests.exe"
assets = root / ".local-inputs/windows-shared-assets"
script = root / ".local-inputs/character-script-owner-extension/monster.luac"

if not compiler.is_file() or not (build / "libfoundation_data.a").is_file():
    raise SystemExit("Build the Windows foundation libraries before running this test.")

runtime = root / "port/script-runtime"
runtime_build = root / ".local-inputs/runtime-monster-melee-choice-v1"
runtime_build.mkdir(parents=True, exist_ok=True)
lua_units = "lapi lcode ldebug ldo ldump lfunc lgc llex lmem lobject lopcodes lparser lstate lstring ltable ltm lundump lvm lzio lauxlib lbaselib ltablib lstrlib lmathlib".split()
c_units = ["script_runtime_return_v3.c", "script_game_bindings.c", "script_scalar_bindings.c",
           "script_design_bindings.c", "script_object_bridge.c"] + ["lua/" + unit + ".c" for unit in lua_units]
runtime_objects = []
for index, unit in enumerate(c_units):
    obj = runtime_build / f"{index}.o"
    subprocess.run([str(c_compiler), "-std=c99", "-O0", "-I" + str(runtime / "lua"),
                    "-I" + str(runtime), "-c", str(runtime / unit), "-o", str(obj)], check=True)
    runtime_objects.append(str(obj))

includes = (
    "level-world", "game-data", "engine-textures", "engine-ui",
    "engine-animation", "engine-skinning", "level-loader",
    "level-loader/vendor/tinyxml", "scene-materials", "script-runtime",
    "script-runtime/lua", "engine-resources", "engine-math",
)
command = [str(compiler), "-std=c++17", "-Dfinite=_finite",
           "-DDH2_NATIVE_PROFILE_TRANSPORT_EMBED", "-O0", "-ffunction-sections",
           "-fdata-sections", "-fno-fast-math", "-ffp-contract=off", "-w",
           "-include", "exception"]
command.extend("-I" + str(root / "port" / directory) for directory in includes)
command.extend(("-isystem", str(root / "port/physics-backend/box2d-2.0.1/Include"),
                "-I" + str(root / "port"),
                str(feature / "monster_lua_differential_tests.cpp"),
                str(feature / "monster_decisions.cpp"),
                str(root / "port/windows-foundation/original_actor_properties.cpp"),
                str(root / "port/windows-foundation/original_combat_properties.cpp"),
                str(root / "port/windows-foundation/playable_actor_world.cpp"),
                str(build / "libfoundation_data.a"), str(build / "libcontent_xml.a"),
                str(build / "libdh2_freetype237.a"),
                str(build / "librecovered_trigger_contacts.a"),
                str(build / "librecovered_content.a"),
                str(build / "physics-backend/libdh2_box2d_201.a"),
                *runtime_objects,
                "-Wl,--gc-sections", "-lkernel32", "-luser32", "-lgdi32",
                "-lwinspool", "-lshell32", "-lole32", "-loleaut32", "-luuid",
                "-lcomdlg32", "-ladvapi32", "-o", str(output)))
subprocess.run(command, check=True)
environment = dict(os.environ)
environment["PATH"] = str(tool) + os.pathsep + environment.get("PATH", "")
subprocess.run((str(output), str(assets), str(script)), check=True, env=environment)

# Exercise the same exact FromFixed result against every original generic mob
# row, rather than reusing a raw/256 approximation in the native decision tests.
decisions_command = list(command)
source_test = str(feature / "monster_lua_differential_tests.cpp")
decisions_command[decisions_command.index(source_test)] = str(feature / "monster_decisions_tests.cpp")
decisions_command[-1] = str(decisions_output)
subprocess.run(decisions_command, check=True)
subprocess.run((str(decisions_output), str(assets)), check=True, env=environment)
