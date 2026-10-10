"""Build and run the factory-backed Faery Character link regression."""
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
    parser.add_argument("--compiler", help="C++17 host compiler used by CMake")
    parser.add_argument("--cmake", default=shutil.which("cmake"),
                        help="CMake executable (default: discovered on PATH)")
    parser.add_argument("--build-dir", type=Path,
                        help="reuse a host build directory instead of a temporary one")
    args = parser.parse_args()
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(errors="replace")
        sys.stderr.reconfigure(errors="replace")
    if not args.cmake:
        parser.error("CMake is required to link the selected dh2_level_world library")
    def run_build(build: Path) -> int:
        configure = [args.cmake, "-S", str(LEVEL_WORLD), "-B", str(build)]
        compiler = args.compiler or os.environ.get("CXX")
        if compiler:
            configure.append(f"-DCMAKE_CXX_COMPILER={compiler}")
        configured = subprocess.run(configure, text=True, capture_output=True,
                                    encoding="utf-8", errors="replace")
        if configured.stdout:
            sys.stdout.write(configured.stdout)
        if configured.stderr:
            sys.stderr.write(configured.stderr)
        if configured.returncode:
            return configured.returncode
        built = subprocess.run([args.cmake, "--build", str(build),
                                "--target", "character_faery_links_v1_audit"],
                               text=True, capture_output=True,
                               encoding="utf-8", errors="replace")
        if built.returncode:
            if built.stdout:
                sys.stdout.write("\n".join(built.stdout.splitlines()[-80:]) + "\n")
            if built.stderr:
                sys.stderr.write("\n".join(built.stderr.splitlines()[-80:]) + "\n")
            return built.returncode
        if built.stdout:
            sys.stdout.write("\n".join(built.stdout.splitlines()[-4:]) + "\n")
        output = build / "character_faery_links_v1_audit.exe"
        if not output.exists():
            output = build / "character_faery_links_v1_audit"
        run_env = os.environ.copy()
        runtime_dirs = [build, build / "game-data", build / "adam-script-runtime",
                        build / "engine-skinning", build / "engine-skinning" / "engine-animation",
                        build / "engine-skinning" / "engine-animation" / "scene-materials"]
        run_env["PATH"] = os.pathsep.join([str(path) for path in runtime_dirs] +
                                          [run_env.get("PATH", "")])
        run = subprocess.run([str(output)], text=True, capture_output=True,
                             env=run_env, encoding="utf-8", errors="replace")
        if run.stdout:
            sys.stdout.write(run.stdout)
        if run.stderr:
            sys.stderr.write(run.stderr)
        if run.returncode:
            return run.returncode
        expected = {"factory_identity_links": True, "player_field_420": True,
                    "ai_master_unwired_fail_closed": True,
                    "ai_master_50_restored": True, "ai_master_bytes_54_55": True,
                    "placement_completed": True}
        report = json.loads(run.stdout)
        if report != expected:
            raise SystemExit(f"unexpected regression result: {report}")
        print(json.dumps({"validation": "PASS", "host_report": report}))
        return 0

    if args.build_dir:
        return run_build(args.build_dir.resolve())
    with tempfile.TemporaryDirectory(prefix="dh2-faery-links-") as temporary:
        return run_build(Path(temporary) / "build")


if __name__ == "__main__":
    raise SystemExit(main())
