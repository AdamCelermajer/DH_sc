"""Run the actual Stage8/9 source regressions on Linux or Windows with WSL Ubuntu.

The executable is built in a temporary host directory. No APK/emulator is used.
"""
from pathlib import Path
import os
import subprocess
import tempfile


PORT = Path(__file__).resolve().parents[2]
LOADER = PORT / "level-loader"
ROOT = PORT.parent


def source_wiring():
    wrapper = (PORT / "android-native/app/src/main/cpp/renderer_source_stage9_v94.inc").read_text()
    runtime = (PORT / "android-native/app/src/main/cpp/source_campaign_runtime_v61.cpp").read_text()
    relay = (LOADER / "canonical_module_files_v1.cpp").read_text()
    native_root = (LOADER / "native_root_source_binding_v65.hpp").read_text()
    assert wrapper.index("if(!trace.step(e)||failed)") < wrapper.index("if(!current(actual,producer,context,e))")
    assert "source->trace=dh2::loader::Stage9TracePrefixV53(std::move(debug));" in wrapper
    assert "bind_source_stage9_v94(state.world,state.candidate,actual,out.early_debug,out.source,error)" in runtime
    assert "files->begin_level_file(e,true)" in wrapper
    assert "complete_absent_level_file_=false;" in relay
    assert "std::make_unique<FilenameRootRouteV65>(*native_filename_,complete_absent_level_file_)" in relay
    assert "std::make_shared<FilenameRootRouteV65>(std::move(leaves.filename))" in native_root
    assert "if(result==LevelFileWalkStepV1::failed&&e.empty())e=files_.back()->error();" in relay
    assert "if(!*relay_failure_logged)" in wrapper and "nested=%s" in wrapper
    print("PASS Stage9 production wiring, legacy root policy and nested diagnostic preservation")


def main():
    source_wiring()
    windows = os.name == "nt"
    command_prefix = ["wsl.exe", "-d", "Ubuntu", "--exec"] if windows else []

    def native_path(path):
        path = path.resolve()
        if not windows:
            return str(path)
        drive = path.drive.rstrip(":").lower()
        if len(drive) != 1:
            raise RuntimeError("WSL regression runner requires a local drive checkout")
        return "/mnt/" + drive + "/" + "/".join(path.parts[1:])

    if windows:
        build = subprocess.check_output(command_prefix + ["/usr/bin/mktemp", "-d", "-t", "dh2-stage8-stage9-XXXXXX"], text=True).strip()
    else:
        build = tempfile.mkdtemp(prefix="dh2-stage8-stage9-")
    binary = build + "/source-regression"
    sources = [
        "tests/stage8_stage9_source_regression_v1.cpp", "level_lightset_stage9_v53.cpp",
        "level_root_filename_route_v52.cpp", "resource_paths_v1.cpp", "canonical_level_context_v1.cpp",
        "assigned_loadfile_kernel_v65.cpp", "xml_document_v1.cpp", "../level-world/event_manager_owner_v12.cpp", "vendor/tinyxml/tinyxml.cpp",
        "vendor/tinyxml/tinyxmlparser.cpp", "vendor/tinyxml/tinyxmlerror.cpp", "vendor/tinyxml/tinystr.cpp",
    ]
    compiler = "/usr/bin/g++" if windows else os.environ.get("CXX", "g++")
    command = command_prefix + [compiler, "-std=c++17", "-O0", "-g", "-Wall", "-Wextra", "-Werror",
                                "-Wno-misleading-indentation", "-Wno-implicit-fallthrough",
                                "-ffunction-sections", "-fdata-sections", "-DTIXML_USE_STL"]
    for directory in (LOADER, PORT / "level-world", PORT / "game-data", PORT / "engine-audio",
                      PORT / "asset-payloads", LOADER / "vendor/tinyxml"):
        command += ["-I", native_path(directory)]
    command += [native_path(LOADER / source) for source in sources]
    command += ["-Wl,--gc-sections", "-o", binary]
    subprocess.run(command, check=True)
    subprocess.run(command_prefix + [binary], check=True)
    print("Host regression executable:", binary)


if __name__ == "__main__":
    main()
