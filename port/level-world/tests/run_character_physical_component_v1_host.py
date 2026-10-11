"""Build and run the source-configured Faery physical component regression."""
from __future__ import annotations

import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[3]
BUILD = ROOT / "outputs" / "level-world-host"
EXPECTED = {
    "faery_source_config": True,
    "typed_component_lifecycle": True,
    "box2d_body_shapes": True,
    "transform": True,
    "source_stop_body_state": True,
    "pin_unpin": True,
    "teardown": True,
    "mismatches": 0,
}


def run(command: list[str]) -> subprocess.CompletedProcess[str]:
    result = subprocess.run(command, text=True, capture_output=True)
    if result.stdout:
        sys.stdout.write(result.stdout)
    if result.stderr:
        sys.stderr.write(result.stderr)
    return result


def main() -> int:
    compiler = os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        raise SystemExit("No C++ host compiler found")
    built = run(["cmake", "--build", str(BUILD), "--target", "dh2_box2d_201", "--parallel", "2"])
    if built.returncode:
        return built.returncode
    box2d = BUILD / "physics-backend" / "libdh2_box2d_201.a"
    if not box2d.exists():
        raise SystemExit(f"Missing host Box2D archive: {box2d}")

    level = ROOT / "port" / "level-world"
    sources = [
        level / "tests" / "character_physical_component_v1.cpp",
        level / "character_physical_component_v1.cpp",
        level / "character_body_config.cpp",
        level / "physical_world.cpp",
        level / "physical_controls.cpp",
        level / "native_body.cpp",
        level / "navigation_avoidance.cpp",
    ]
    with tempfile.TemporaryDirectory(prefix="dh2-faery-physical-") as temporary:
        output = Path(temporary) / "character_physical_component_v1_host.exe"
        command = [compiler, "-std=c++17", "-O1", "-fno-fast-math", "-ffp-contract=off",
                   "-I" + str(level), "-I" + str(ROOT / "port" / "physics-backend" /
                   "box2d-2.0.1" / "Include"), "-include", "cstring"]
        if os.name == "nt":
            command.append("-Dfinite=_finite")
        command.extend([*(str(source) for source in sources), str(box2d), "-o", str(output)])
        compiled = run(command)
        if compiled.returncode:
            return compiled.returncode
        executed = run([str(output)])
        if executed.returncode:
            return executed.returncode
        report = json.loads(executed.stdout)
        if report != EXPECTED:
            raise SystemExit(f"Unexpected host result: {report}")
        print(json.dumps({"validation": "PASS", "host_report": report}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
