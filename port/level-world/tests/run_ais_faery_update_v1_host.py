"""Build the AISFaery update regression in an isolated temporary directory."""
from __future__ import annotations

import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]


def main() -> int:
    compiler = shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        raise SystemExit("No host C++ compiler found")
    with tempfile.TemporaryDirectory(prefix="dh2-ais-faery-") as temporary:
        output = Path(temporary) / "ais_faery_update_v1.exe"
        command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
            str(ROOT / "tests" / "ais_faery_update_v1.cpp"),
            str(ROOT / "ais_faery_update_v1.cpp"), "-o", str(output)]
        build = subprocess.run(command, text=True, capture_output=True)
        if build.stdout:
            sys.stdout.write(build.stdout)
        if build.stderr:
            sys.stderr.write(build.stderr)
        if build.returncode:
            return build.returncode
        run = subprocess.run([str(output)], text=True, capture_output=True)
        if run.stdout:
            sys.stdout.write(run.stdout)
        if run.stderr:
            sys.stderr.write(run.stderr)
        if run.returncode:
            return run.returncode
        expected = {"ais_faery_source_order": True,
            "changed_skin_applied_once": True, "changed_value_reread": True,
            "source_store_retained_on_visual_failure": True}
        if json.loads(run.stdout) != expected:
            raise SystemExit("Unexpected AISFaery host report")
        print(json.dumps({"validation": "PASS", "source_level": "host-only"}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
