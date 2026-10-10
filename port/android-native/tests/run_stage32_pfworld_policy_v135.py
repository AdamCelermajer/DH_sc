"""Build/run the actual Stage32 adapter + current SpotTarget with native floors.

No Android runtime or emulator. Fixture callbacks model service boundaries;
the shipping bodies, PFWorld policy storage and navigation kernels execute.
"""
from pathlib import Path
import argparse
import hashlib
import json
import subprocess

ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"


def linux_path(path):
    path = path.resolve()
    return "/mnt/" + path.drive[0].lower() + path.as_posix()[2:]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, default=ROOT / ".local-inputs/stage32-pfworld-policy-v135")
    args = parser.parse_args()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    source = (CPP / "source_campaign_character_frame_v111.cpp").read_text(encoding="utf-8")
    spot = source[source.index("bool spot(Record&"):source.index("bool suffix(Record&")]
    (output / "stage32-production-spot.inc").write_text(spot, encoding="utf-8")

    # Verify current consumers lend the same receiver cell instead of scratch
    # or copied fields. Dynamic tests above read/mutate that cell in kernels.
    environment = (CPP / "source_campaign_frame_environment_v76.cpp").read_text(encoding="utf-8")
    items = (CPP / "source_campaign_items_v88.cpp").read_text(encoding="utf-8")
    assert "prepared.motion_policy=&lease->floors->source_motion_policy_v95" in environment
    assert "out.motion=&c.floors->source_motion_policy_v95" in items
    assert "&scope.floors->source_motion_policy_v95" in spot
    assert "byte94_v95" not in (ROOT / "port/level-world/source_loading_application_fields_v55.hpp").read_text()

    world = ROOT / "port/level-world"
    command = ["wsl.exe", "-d", "Ubuntu", "--", "g++", "-std=c++17", "-O0", "-g",
               "-fsanitize=address,undefined", "-fno-omit-frame-pointer", "-fno-fast-math",
               "-ffp-contract=off", "-ffunction-sections", "-fdata-sections", "-Wl,--gc-sections",
               "-I" + linux_path(world), "-I" + linux_path(ROOT / "port/level-loader"),
               "-I" + linux_path(output),
               linux_path(ROOT / "port/android-native/tests/stage32_pfworld_policy_v135.cpp")]
    command += [linux_path(world / name) for name in ("navigation_motion.cpp", "selector.cpp", "octree.cpp", "collision.cpp")]
    command += [linux_path(ROOT / "port/level-loader/lifecycle_v36.cpp")]
    binary = output / "stage32-pfworld-policy-v135"
    command += ["-o", linux_path(binary)]
    compile_result = subprocess.run(command, capture_output=True, text=True)
    (output / "compile.log").write_text(compile_result.stdout + compile_result.stderr, encoding="utf-8")
    if compile_result.returncode:
        raise RuntimeError(compile_result.stderr)
    result = subprocess.run(["wsl.exe", "-d", "Ubuntu", "--", "env", "ASAN_OPTIONS=detect_leaks=0", linux_path(binary)], capture_output=True, text=True)
    print(result.stdout, end="")
    if result.returncode:
        raise RuntimeError(result.stderr)
    receipt = json.loads(result.stdout)
    receipt["source_sha256"] = {
        str(path.relative_to(ROOT)).replace("\\", "/"): hashlib.sha256(path.read_bytes()).hexdigest()
        for path in (CPP / "renderer_source_stage18_stage32_v95.inc",
                     CPP / "source_campaign_character_frame_v111.cpp",
                     CPP / "source_campaign_frame_environment_v76.cpp",
                     CPP / "source_campaign_items_v88.cpp",
                     world / "floors.hpp", world / "navigation_motion.cpp",
                     ROOT / "port/level-loader/lifecycle_v36.cpp")
    }
    receipt["compile_command"] = command
    (output / "receipt.json").write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
