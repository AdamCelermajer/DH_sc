"""Build the selected UI library and exercise the real authored Android HUD SWF."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[3]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build", type=Path, required=True)
    parser.add_argument("--compiler", type=Path, required=True)
    parser.add_argument("--report", type=Path, required=True)
    args = parser.parse_args()
    args.build.mkdir(parents=True, exist_ok=True)
    args.report.parent.mkdir(parents=True, exist_ok=True)
    env = os.environ.copy()
    gcc = args.compiler.with_name("gcc.exe")
    env["PATH"] = os.pathsep.join((str(args.compiler.parent), env.get("PATH", "")))
    wrapper = ROOT / "port/engine-ui/tests/menu-frontend-v69"
    native_cmake = ROOT / "port/android-native/app/src/main/cpp/CMakeLists.txt"
    production_cmake = ROOT / "port/engine-ui/menu_frontend_v69.cmake"
    assert 'menu_frontend_v69.cmake' in native_cmake.read_text()
    assert "authored_gameplay_hud_v1.cpp" in production_cmake.read_text()
    assert "authored_hud_edge_layout_v6.cpp" in production_cmake.read_text()
    assert "authored_joystick_v1.cpp" in production_cmake.read_text()

    inputs = [
        ROOT / "port/engine-ui/authored_gameplay_hud_v1.cpp",
        ROOT / "port/engine-ui/authored_gameplay_hud_v1.hpp",
        ROOT / "port/engine-ui/gameplay_hud_usable_refresh_v1.hpp",
        ROOT / "port/engine-ui/authored_hud_edge_layout_v6.cpp",
        ROOT / "port/engine-ui/authored_hud_edge_layout_v6.hpp",
        ROOT / "port/engine-ui/authored_joystick_v1.cpp",
        ROOT / "port/engine-ui/authored_joystick_v1.hpp",
        ROOT / "port/engine-ui/swf_movie.cpp",
        ROOT / "port/engine-ui/swf_movie.hpp",
        ROOT / "port/engine-ui/menu_frontend_v69.cmake",
        ROOT / "port/engine-ui/tests/authored_gameplay_hud_v1.cpp",
        ROOT / "port/android-native/app/src/main/cpp/gameplay_main_menu_request_v1.hpp",
        ROOT / "port/engine-ui/tests/authored_joystick_v1.cpp",
        ROOT / "port/engine-ui/tests/swf_movie_test_fixture.hpp",
        ROOT / "port/engine-ui/reference/authored-joystick-v1/fixtures.bin",
        wrapper / "CMakeLists.txt",
        native_cmake,
        ROOT / "port/android-native/app/src/main/assets/original-cache/data/menus/dqshared_droid.swf",
        ROOT / "port/android-native/app/src/main/assets/original-cache/data/menus/dqhud_droid.swf",
    ]
    before = {p.relative_to(ROOT).as_posix(): sha(p) for p in inputs}

    commands = []

    def run(command):
        cmd = [str(x) for x in command]
        result = subprocess.run(cmd, cwd=ROOT, env=env, capture_output=True, text=True)
        commands.append({"command": cmd, "returncode": result.returncode,
                         "stdout": result.stdout, "stderr": result.stderr})
        (args.build / "commands.json").write_text(json.dumps(commands, indent=2) + "\n")
        if result.returncode:
            raise RuntimeError((result.stdout + result.stderr)[-8000:])
        return result.stdout

    # CMake 3.22 writes compiler paths into generated CMake scripts. Native
    # Windows backslashes become escapes there (for example, ``\m`` in
    # ``C:\msys64``), so pass canonical forward-slash paths for -D values.
    compiler_path = args.compiler.resolve().as_posix()
    gcc_path = gcc.resolve().as_posix()
    run(["cmake", "-S", wrapper, "-B", args.build, "-G", "Ninja",
         "-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
         "-DCMAKE_CXX_COMPILER=" + compiler_path,
         "-DCMAKE_C_COMPILER=" + gcc_path])
    compilation = json.loads((args.build / "compile_commands.json").read_text())
    selected = [entry for entry in compilation
                if "CMakeFiles/dh2_engine_ui.dir" in entry["command"].replace("\\", "/")]
    selected_sources = {Path(entry["file"]).resolve() for entry in selected}
    for source in ("authored_gameplay_hud_v1.cpp", "authored_hud_edge_layout_v6.cpp",
                   "authored_joystick_v1.cpp", "swf_movie.cpp"):
        assert any(path.name == source for path in selected_sources), "selected UI library omits " + source
    assert any(Path(entry["file"]).name == "authored_gameplay_hud_v1.cpp" and
               "CMakeFiles/authored_gameplay_hud_v1_host.dir" in entry["command"].replace("\\", "/")
               for entry in compilation)
    run(["cmake", "--build", args.build, "--parallel", "3",
         "--target", "authored_gameplay_hud_v1_host", "authored_joystick_v1_host"])

    ui = args.build / "libdh2_engine_ui.dll"
    game_data = args.build / "game-data"
    executable = args.build / "authored_gameplay_hud_v1_host.exe"
    joystick_executable = args.build / "authored_joystick_v1_host.exe"
    assert ui.is_file() and executable.is_file() and joystick_executable.is_file()
    env["PATH"] = os.pathsep.join((str(args.build), str(game_data), env["PATH"]))
    menu_assets = ROOT / "port/android-native/app/src/main/assets/original-cache/data/menus"
    result = json.loads(run([executable, menu_assets]))
    assert result["validation"] == "PASS" and result["checks"] >= 400 and result["vertices"] > 0
    joystick_result = json.loads(run([joystick_executable]))
    assert joystick_result["validation"] == "PASS" and joystick_result["original_cases"] > 0
    after = {p.relative_to(ROOT).as_posix(): sha(p) for p in inputs}
    assert before == after, "source or packaged SWF changed during host build"
    report = {
        "validation": "PASS",
        "scope": "Selected dh2_engine_ui plus real packaged droid HUD SWF; host texture/localization/native providers are declared fixtures; no Android/live-gameplay claim.",
        "adam_source_commit": "11fa5242de525e0fd132d8019920baa862ef70d7",
        "tests": {"authored_gameplay_hud": result, "authored_joystick": joystick_result},
        "selected_ui_translation_units": len(selected),
        "source_and_asset_sha256": before,
        "commands": commands,
        "executable_sha256": sha(executable),
        "ui_library_sha256": sha(ui),
    }
    args.report.write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({"validation": "PASS", "tests": {"authored_gameplay_hud": result,
                      "authored_joystick": joystick_result},
                      "selected_ui_translation_units": len(selected),
                      "report": str(args.report)}))


if __name__ == "__main__":
    main()
