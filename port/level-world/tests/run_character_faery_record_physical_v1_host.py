"""Build and run the Record-owned POFaerie identity regression."""
from __future__ import annotations

import os
from pathlib import Path
import subprocess
import sys

LEVEL_WORLD = Path(__file__).resolve().parents[1]
BUILD = LEVEL_WORLD.parents[1] / ".checks" / "level-world-host"


def main() -> int:
    built = subprocess.run(["cmake", "--build", str(BUILD), "--target",
        "character_faery_record_physical_v1_audit", "--parallel", "2"],
        text=True, capture_output=True)
    if built.stdout:
        sys.stdout.write(built.stdout)
    if built.stderr:
        sys.stderr.write(built.stderr)
    if built.returncode:
        return built.returncode
    env = os.environ.copy()
    env["PATH"] = str(BUILD) + os.pathsep + env.get("PATH", "")
    executable = BUILD / "character_faery_record_physical_v1_audit.exe"
    run = subprocess.run([str(executable)], text=True, capture_output=True, env=env)
    if run.stdout:
        sys.stdout.write(run.stdout)
    if run.stderr:
        sys.stderr.write(run.stderr)
    if run.returncode:
        return run.returncode
    print("PASS Record-owned typed POFaerie identity and body lifecycle")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
