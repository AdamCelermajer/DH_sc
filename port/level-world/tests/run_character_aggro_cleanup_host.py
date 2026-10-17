"""Compile and run the source-ordered CharAI all-aggro cleanup kernel."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys


LEVEL_WORLD = Path(__file__).resolve().parents[1]
REPOSITORY = LEVEL_WORLD.parents[1]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++ compiler executable (defaults to CXX, g++, or clang++)")
    parser.add_argument("--report", type=Path, help="optional saved source-bound evidence JSON")
    parser.add_argument(
        "--output", type=Path,
        default=LEVEL_WORLD / "build" / ("character_aggro_cleanup_host.exe" if os.name == "nt" else "character_aggro_cleanup_host"),
        help="host test executable path (default: ignored level-world/build/)",
    )
    args = parser.parse_args()

    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("no C++ compiler found; pass --compiler or set CXX")

    output = args.output.resolve()
    if os.name == "nt" and not output.suffix:
        output = output.with_suffix(".exe")
    output.parent.mkdir(parents=True, exist_ok=True)

    sources = [
        LEVEL_WORLD / "tests" / "character_aggro_cleanup.cpp",
        LEVEL_WORLD / "character_aggro_cleanup.cpp",
    ]
    command = [
        compiler,
        "-std=c++17",
        "-O1",
        "-Wall",
        "-Wextra",
        "-Werror",
        *(str(source) for source in sources),
        "-o",
        str(output),
    ]
    built = subprocess.run(command, cwd=REPOSITORY, text=True, capture_output=True)
    if built.stdout:
        sys.stdout.write(built.stdout)
    if built.stderr:
        sys.stderr.write(built.stderr)
    if built.returncode:
        return built.returncode

    result = subprocess.run([str(output)], cwd=REPOSITORY, text=True, capture_output=True)
    if result.stdout:
        sys.stdout.write(result.stdout)
    if result.stderr:
        sys.stderr.write(result.stderr)
    if result.returncode:
        return result.returncode

    report = json.loads(result.stdout)
    if (report.get("mismatches") != 0 or
            report.get("clear_all_aggro_cases") != 7 or
            report.get("on_deaggro_virtual_notifications") is not True or
            report.get("self_peer_supported") is not True or
            report.get("self_peer_callbacks") != 1 or
            report.get("incoming_erase_precedes_owner_clear") is not True or
            report.get("source_reentry_snapshot") is not True or
            report.get("retained_peers_not_freed") is not True or
            report.get("callback_failure_rollback_claimed") is not False):
        raise SystemExit(f"host report did not satisfy expectations: {report}")
    evidence = {
        "validation": "PASS",
        "executable": str(output),
        "host_report": report,
        "compiler_command": command,
        "source_sha256": {
            str(path.relative_to(REPOSITORY)).replace("\\", "/"):
            hashlib.sha256(path.read_bytes()).hexdigest() for path in sources
        },
        "live_aggro_tables_wired": False,
    }
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(evidence, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(evidence))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
