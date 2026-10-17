"""Build and run the source GameObject visual attachment regression."""
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
EXPECTED = {
    "failed_scene_preserves_previous": True,
    "replace_destroys_old_first": True,
    "owner_backlink": True,
    "same_pointer_no_replace": True,
    "detach_destroys": True,
    "foreign_owner_rejected": True,
    "missing_destructor_fails_closed": True,
    "failed_destructor_preserves_attachment": True,
}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++17 host compiler")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("no C++ host compiler found; pass --compiler or set CXX")
    with tempfile.TemporaryDirectory(prefix="dh2-visual-attachment-") as temporary:
        output = Path(temporary) / "game_object_visual_attachment_v1.exe"
        command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
                   str(LEVEL_WORLD / "tests" / "game_object_visual_attachment_v1.cpp"),
                   str(LEVEL_WORLD / "game_object_visual_attachment_v1.cpp"),
                   "-o", str(output)]
        built = subprocess.run(command, text=True, capture_output=True)
        if built.stdout:
            sys.stdout.write(built.stdout)
        if built.stderr:
            sys.stderr.write(built.stderr)
        if built.returncode:
            return built.returncode
        run = subprocess.run([str(output)], text=True, capture_output=True)
        if run.stdout:
            sys.stdout.write(run.stdout)
        if run.stderr:
            sys.stderr.write(run.stderr)
        if run.returncode:
            return run.returncode
        report = json.loads(run.stdout)
        if report != EXPECTED:
            raise SystemExit(f"unexpected regression result: {report}")
        print(json.dumps({"validation": "PASS", "host_report": report}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
