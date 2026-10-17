"""Build and run the per-Character Faery visual/Save/placement transaction."""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

LEVEL_WORLD = Path(__file__).resolve().parents[1]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++17 compiler used by CMake")
    parser.add_argument("--cmake", default=shutil.which("cmake"))
    parser.add_argument("--build-dir", type=Path)
    args = parser.parse_args()
    if not args.cmake:
        parser.error("CMake is required to link selected game-data and level-world libraries")
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(errors="replace")
        sys.stderr.reconfigure(errors="replace")

    def run_build(build: Path) -> int:
        configure = [args.cmake, "-S", str(LEVEL_WORLD), "-B", str(build)]
        compiler = args.compiler or os.environ.get("CXX")
        if compiler:
            configure.append(f"-DCMAKE_CXX_COMPILER={compiler}")
        configured = subprocess.run(configure, text=True, capture_output=True,
                                    encoding="utf-8", errors="replace")
        if configured.returncode:
            sys.stdout.write(configured.stdout)
            sys.stderr.write(configured.stderr)
            return configured.returncode
        built = subprocess.run([args.cmake, "--build", str(build), "--target",
                                "character_faery_selection_owner_v1_audit"],
                               text=True, capture_output=True,
                               encoding="utf-8", errors="replace")
        if built.returncode:
            sys.stdout.write("\n".join(built.stdout.splitlines()[-80:]) + "\n")
            sys.stderr.write("\n".join(built.stderr.splitlines()[-80:]) + "\n")
            return built.returncode
        if built.stdout:
            sys.stdout.write("\n".join(built.stdout.splitlines()[-5:]) + "\n")
        executable = build / "character_faery_selection_owner_v1_audit.exe"
        if not executable.exists():
            executable = build / "character_faery_selection_owner_v1_audit"
        env = os.environ.copy()
        runtime_dirs = [build, build / "game-data", build / "adam-script-runtime",
                        build / "engine-skinning",
                        build / "engine-skinning" / "engine-animation",
                        build / "engine-skinning" / "engine-animation" / "scene-materials"]
        env["PATH"] = os.pathsep.join([str(path) for path in runtime_dirs] +
                                      [env.get("PATH", "")])
        ran = subprocess.run([str(executable)], text=True, capture_output=True,
                             env=env, encoding="utf-8", errors="replace")
        sys.stdout.write(ran.stdout)
        sys.stderr.write(ran.stderr)
        if ran.returncode:
            return ran.returncode
        report = json.loads(ran.stdout)
        expected = {"validation": "PASS", "player_faery_420_guard": True,
                    "save_visual_placement": True,
                    "placement_rollback": True, "skill_failure_restore": True,
                    "shared_asset_lifetime": True}
        if report != expected:
            raise SystemExit(f"unexpected regression result: {report}")
        print(json.dumps({"validation": "PASS", "host_report": report}))
        return 0

    if args.build_dir:
        return run_build(args.build_dir.resolve())
    with tempfile.TemporaryDirectory(prefix="dh2-faery-selection-") as temporary:
        return run_build(Path(temporary) / "build")


if __name__ == "__main__":
    raise SystemExit(main())
