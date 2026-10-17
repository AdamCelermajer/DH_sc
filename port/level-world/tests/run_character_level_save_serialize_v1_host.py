#!/usr/bin/env python3
"""Compile and run the Character::Serialize byte-order regression."""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
test = root / "tests" / "character_level_save_serialize_v1.cpp"
cmake = shutil.which("cmake")
ninja = shutil.which("ninja")
cxx = shutil.which(os.environ.get("CXX", "g++"))
cc = shutil.which(os.environ.get("CC", "gcc"))
if not all((cmake, ninja, cxx, cc)):
    raise RuntimeError("focused serializer host test requires CMake, Ninja, g++, and gcc")
with tempfile.TemporaryDirectory(prefix="dh2-character-level-save-") as tmp:
    tmp_path = Path(tmp)
    wrapper, build = tmp_path / "wrapper", tmp_path / "selected"
    wrapper.mkdir()
    (wrapper / "CMakeLists.txt").write_text(
        "\n".join((
            "cmake_minimum_required(VERSION 3.22)",
            "project(character_level_save_serialize_host LANGUAGES C CXX)",
            "set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)",
            f'add_subdirectory("{root.as_posix()}" selected-world)',
            f'add_executable(character_level_save_serialize_v1_host "{test.as_posix()}")',
            "target_link_libraries(character_level_save_serialize_v1_host PRIVATE dh2_level_world dh2_script_runtime)",
            "target_compile_features(character_level_save_serialize_v1_host PRIVATE cxx_std_17)",
            "target_compile_options(character_level_save_serialize_v1_host PRIVATE -Wall -Wextra -Werror)",
            "",
        )), encoding="utf-8")
    subprocess.run([cmake, "-S", str(wrapper), "-B", str(build), "-G", "Ninja",
                    f"-DCMAKE_MAKE_PROGRAM={ninja}", f"-DCMAKE_C_COMPILER={cc}",
                    f"-DCMAKE_CXX_COMPILER={cxx}", "-DCMAKE_BUILD_TYPE=Release"], check=True)
    subprocess.run([cmake, "--build", str(build), "--target",
                    "character_level_save_serialize_v1_host"], check=True)
    exe = build / ("character_level_save_serialize_v1_host.exe" if os.name == "nt"
                   else "character_level_save_serialize_v1_host")
    env = os.environ.copy()
    dll_dirs = [str(path.parent) for path in build.rglob("*.dll")]
    env["PATH"] = os.pathsep.join(dll_dirs + [env.get("PATH", "")])
    subprocess.run([str(exe)], check=True, env=env)
