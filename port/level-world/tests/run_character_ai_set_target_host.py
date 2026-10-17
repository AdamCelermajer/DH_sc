#!/usr/bin/env python3
"""Verify the pinned ARM source ranges, then build/run the host target setter."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

HERE = Path(__file__).resolve().parent
MODULE = HERE.parent
REPO = HERE.parents[2]
MANIFEST = (REPO / "port" / "level-world" / "reference" /
            "character-ai-set-target" / "original-functions.json")
ORIGINAL_SHA256 = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"


def require(condition: bool, message: str) -> None:
    if not condition:
        raise RuntimeError(message)


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def verify_original(path: Path) -> dict:
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    image = path.read_bytes()
    actual_library_hash = sha256(image)
    require(actual_library_hash == ORIGINAL_SHA256,
            f"wrong original library: SHA-256 {actual_library_hash}")
    require(manifest["original_sha256"] == actual_library_hash,
            "source manifest is not pinned to this library")
    ranges = []
    for kind, items, symbol_key in (
        ("function", manifest["functions"], "original_symbol"),
        ("vtable", manifest.get("vtable_ranges", []), "symbol"),
    ):
        for item in items:
            address = int(item["elf_address"], 0)
            size = int(item["size"])
            body = image[address:address + size]
            actual = sha256(body)
            require(len(body) == size and actual == item["sha256"],
                    f"original {kind} range changed: {item[symbol_key]} at {item['elf_address']}")
            ranges.append({
                "kind": kind,
                "symbol": item[symbol_key],
                "elf_address": item["elf_address"],
                "size": size,
                "sha256": actual,
            })
    return {"path": str(path), "sha256": actual_library_hash,
            "verified_ranges": ranges}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--original", type=Path,
                        default=REPO.parent / "test_strategy" / "libDungeonHunter2.so")
    parser.add_argument("--compiler", default="g++")
    parser.add_argument("--output", type=Path,
                        default=MODULE / "build" / "character-ai-set-target" /
                        "character-ai-set-target-host.exe")
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build" / "character-ai-set-target" /
                        "validation.json")
    args = parser.parse_args()

    original = verify_original(args.original.resolve())
    compiler = shutil.which(args.compiler)
    require(compiler is not None, f"C++ compiler not found: {args.compiler}")
    output = args.output.resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [
        compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic",
        str(MODULE / "character_ai_set_target.cpp"),
        str(HERE / "character_ai_set_target.cpp"),
        "-o", str(output),
    ]
    build = subprocess.run(command, cwd=REPO, text=True, capture_output=True)
    if build.returncode:
        raise RuntimeError(f"host compile failed ({build.returncode}):\n{build.stdout}{build.stderr}")
    run = subprocess.run([str(output)], cwd=REPO, text=True, capture_output=True)
    if run.returncode:
        raise RuntimeError(f"host checks failed ({run.returncode}):\n{run.stdout}{run.stderr}")

    report = {
        "status": "passed",
        "original": original,
        "compiler_command": command,
        "test_output": run.stdout.strip(),
        "scope": (
            "Host checks for the source AI_SetTarget body: force/nonforce, null, duplicate, "
            "trace branches, source-time target rereads, virtual boundary and partial adapter "
            "failure. Debug manager, IsDead dynamic dispatch and AI_IsInSight are synchronous "
            "fixtures. No ARM differential or Android wiring is claimed."
        ),
    }
    report_path = args.report.resolve()
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    counts = {kind: sum(row["kind"] == kind for row in original["verified_ranges"])
              for kind in ("function", "vtable")}
    print(run.stdout.strip())
    print(f"pinned {counts['function']} functions and {counts['vtable']} vtable ranges")
    print(f"report: {report_path}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError, RuntimeError) as exc:
        raise SystemExit(f"ERROR: {exc}") from exc
