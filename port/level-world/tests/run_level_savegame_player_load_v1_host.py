#!/usr/bin/env python3
"""Build and run the focused LevelSavegame Player-load regression."""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
test = root / "tests" / "level_savegame_player_load_v1.cpp"
cmake = shutil.which("cmake")
ninja = shutil.which("ninja")
cxx = shutil.which(os.environ.get("CXX", "g++"))
cc = shutil.which(os.environ.get("CC", "gcc"))
if not all((cmake, ninja, cxx, cc)):
    raise RuntimeError("focused Player-load host test requires CMake, Ninja, g++, and gcc")
with tempfile.TemporaryDirectory(prefix="dh2-level-save-player-load-") as tmp:
    tmp_path = Path(tmp)
    wrapper, build = tmp_path / "wrapper", tmp_path / "selected"
    wrapper.mkdir()
    (wrapper / "CMakeLists.txt").write_text(
        "\n".join((
            "cmake_minimum_required(VERSION 3.22)",
            "project(level_savegame_player_load_host LANGUAGES C CXX)",
            "set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)",
            f'add_subdirectory("{root.as_posix()}" selected-world)',
            f'add_executable(level_savegame_player_load_v1_host "{test.as_posix()}")',
            "target_link_libraries(level_savegame_player_load_v1_host PRIVATE dh2_level_world dh2_script_runtime)",
            "target_compile_features(level_savegame_player_load_v1_host PRIVATE cxx_std_17)",
            "target_compile_options(level_savegame_player_load_v1_host PRIVATE -Wall -Wextra -Werror -UNDEBUG)",
            "",
        )), encoding="utf-8")
    subprocess.run([cmake, "-S", str(wrapper), "-B", str(build), "-G", "Ninja",
                    f"-DCMAKE_MAKE_PROGRAM={ninja}", f"-DCMAKE_C_COMPILER={cc}",
                    f"-DCMAKE_CXX_COMPILER={cxx}", "-DCMAKE_BUILD_TYPE=Release"], check=True)
    subprocess.run([cmake, "--build", str(build), "--target",
                    "level_savegame_player_load_v1_host"], check=True)
    exe = build / ("level_savegame_player_load_v1_host.exe" if os.name == "nt"
                   else "level_savegame_player_load_v1_host")
    env = os.environ.copy()
    dll_dirs = [str(path.parent) for path in build.rglob("*.dll")]
    env["PATH"] = os.pathsep.join(dll_dirs + [env.get("PATH", "")])
    subprocess.run([str(exe)], check=True, env=env)
