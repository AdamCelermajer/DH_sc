"""Build/run the Stage5 adapter against actual NativeWorld and Box2D on Linux/WSL.

The Level scalars and Debug file-open service are explicit host fixtures. The
production Stage5 body, Debug map, dispatcher, and physical backend execute.
"""
import argparse
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--build-dir", type=Path, required=True)
    args = parser.parse_args()
    args.build_dir.mkdir(parents=True, exist_ok=True)
    output = args.build_dir / "stage5_physical_v135"
    box = ROOT / "port/physics-backend/box2d-2.0.1"
    command = ["g++", "-std=c++17", "-O0", "-Wall", "-Wextra", "-Werror",
               "-Wno-misleading-indentation", "-fno-fast-math", "-ffp-contract=off",
               "-include", "cstring", "-I" + str(ROOT / "port/level-world"),
               "-isystem", str(box / "Include")]
    command += [str(ROOT / path) for path in (
        "port/level-loader/tests/stage5_physical_v135.cpp",
        "port/level-loader/lifecycle_v36.cpp",
        "port/level-world/character_design_services.cpp",
        "port/level-world/physical_world.cpp")]
    # Box2D 2.0.1 carries old warning patterns; keep strict warnings on the
    # reconstructed production/test units and compile the vendor separately.
    vendor_objects = []
    for index, path in enumerate(sorted((box / "Source").rglob("*.cpp"))):
        obj = args.build_dir / ("box2d_" + str(index) + ".o")
        subprocess.run(["g++", "-std=c++17", "-O0", "-include", "cstring",
                        "-I" + str(box / "Include"), "-c", str(path), "-o", str(obj)], check=True)
        vendor_objects.append(str(obj))
    command += vendor_objects + ["-o", str(output)]
    subprocess.run(command, check=True)
    # Verify the actual composer supplies both leaves from its same-World
    # prefix transport; a disconnected passing fixture cannot replace wiring.
    runtime = (ROOT / "port/android-native/app/src/main/cpp/source_campaign_runtime_v61.cpp").read_text(encoding="utf-8")
    assert "out.early_debug={current_world->debug,out.prefix.debug_instance_get_switch,out.prefix.debug_load};" in runtime
    subprocess.run([str(output)], check=True)


if __name__ == "__main__":
    if sys.platform == "win32":
        raise SystemExit("Run this bounded host regression under Linux/WSL.")
    main()
