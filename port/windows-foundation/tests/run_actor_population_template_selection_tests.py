"""Build and run the asset-backed ActorPopulation template selector test."""
from __future__ import annotations

import os
import pathlib
import subprocess

root = pathlib.Path.cwd()
tests = root / "port/windows-foundation/tests"
tool = root / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin"
build = root / ".local-inputs/windows-foundation-build"
compiler = tool / "clang++.exe"
output = tests / "actor_population_template_selection_tests.exe"

if not compiler.is_file() or not (build / "libfoundation_data.a").is_file():
    raise SystemExit("Build the Windows foundation libraries before running this test.")

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
                str(tests / "actor_population_template_selection_tests.cpp"),
                str(root / "port/windows-foundation/actor_population.cpp"),
                str(root / "port/game-data/character_templates_v78.cpp"),
                str(build / "libfoundation_data.a"), str(build / "libcontent_xml.a"),
                str(build / "libdh2_freetype237.a"),
                str(build / "librecovered_trigger_contacts.a"),
                str(build / "librecovered_content.a"),
                str(build / "physics-backend/libdh2_box2d_201.a"),
                "-Wl,--gc-sections", "-lkernel32", "-luser32", "-lgdi32",
                "-lwinspool", "-lshell32", "-lole32", "-loleaut32", "-luuid",
                "-lcomdlg32", "-ladvapi32", "-o", str(output)))
subprocess.run(command, check=True)
environment = dict(os.environ)
environment["PATH"] = str(tool) + os.pathsep + environment.get("PATH", "")
subprocess.run((str(output), str(root)), check=True, env=environment)
