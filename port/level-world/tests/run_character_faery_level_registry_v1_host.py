"""Build and run the source Character/Level Faery binding regression."""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("no C++17 host compiler found")
    repo = ROOT.parents[1]
    build_dir = repo / ".checks" / "level-world-host"
    if not (build_dir / "CMakeCache.txt").exists():
        configure = subprocess.run(["cmake", "-S", str(ROOT), "-B", str(build_dir),
            "-G", "Ninja", f"-DCMAKE_CXX_COMPILER={compiler}"],
            text=True, capture_output=True)
        if configure.stdout:
            sys.stdout.write(configure.stdout)
        if configure.stderr:
            sys.stderr.write(configure.stderr)
        if configure.returncode:
            return configure.returncode
    build = subprocess.run(["cmake", "--build", str(build_dir),
        "--target", "character_faery_level_registry_v1_audit", "-j", "4"],
        text=True, capture_output=True)
    if build.stdout:
        sys.stdout.write(build.stdout)
    if build.stderr:
        sys.stderr.write(build.stderr)
    if build.returncode:
        return build.returncode
    exe = build_dir / "character_faery_level_registry_v1_audit.exe"
    env = os.environ.copy()
    runtime_dirs = [Path(compiler).resolve().parent, build_dir,
        build_dir / "game-data", build_dir / "engine-skinning",
        build_dir / "engine-skinning" / "engine-animation",
        build_dir / "engine-skinning" / "engine-animation" / "scene-materials",
        build_dir / "adam-script-runtime"]
    env["PATH"] = os.pathsep.join(str(path) for path in runtime_dirs) + os.pathsep + env.get("PATH", "")
    run = subprocess.run([str(exe)], text=True, capture_output=True, env=env)
    if run.stdout:
        sys.stdout.write(run.stdout)
    if run.stderr:
        sys.stderr.write(run.stderr)
    if run.returncode:
        return run.returncode
    report = json.loads(run.stdout)
    expected = {"missing_member_rejected_before_mutation": True,
                "complete_roster_preflight": True,
                "position_routed_by_identity": True,
                "faery_links_and_master_restore": True,
                "follower_common_graph": True}
    if report != expected:
        raise SystemExit(f"unexpected host report: {report}")
    print(json.dumps({"validation": "PASS", "host_report": report}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
