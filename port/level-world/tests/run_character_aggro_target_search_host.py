#!/usr/bin/env python3
"""Pin the original aggro TargetList ranges, then build and run the host adapter checks."""
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
MANIFEST = REPO / "port" / "level-world" / "reference" / "character-aggro-target-search" / "original-functions.json"
EXPECTED_ORIGINAL_SHA256 = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"


def require(condition: bool, message: str) -> None:
    if not condition:
        raise RuntimeError(message)


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def verify_original(path: Path) -> dict:
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    image = path.read_bytes()
    image_hash = sha256(image)
    require(image_hash == EXPECTED_ORIGINAL_SHA256,
            f"wrong original library: SHA-256 {image_hash}")
    require(manifest.get("original_sha256") == image_hash,
            "original-functions manifest is not pinned to this library")
    records = []
    for function in manifest["functions"]:
        address = int(function["elf_address"], 0)
        size = int(function["size"])
        body = image[address:address + size]
        actual = sha256(body)
        require(len(body) == size and actual == function["sha256"],
                f"original range changed: {function['original_symbol']} at {function['elf_address']}")
        records.append({
            "kind": "function",
            "symbol": function["original_symbol"],
            "elf_address": function["elf_address"],
            "size": size,
            "sha256": actual,
        })
    for table in manifest.get("vtable_ranges", []):
        address = int(table["elf_address"], 0)
        size = int(table["size"])
        body = image[address:address + size]
        actual = sha256(body)
        require(len(body) == size and actual == table["sha256"],
                f"original vtable changed: {table['symbol']} at {table['elf_address']}")
        records.append({
            "kind": "vtable",
            "symbol": table["symbol"],
            "elf_address": table["elf_address"],
            "size": size,
            "sha256": actual,
        })
    return {"path": str(path), "sha256": image_hash, "verified_ranges": records}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--original", type=Path,
                        default=REPO.parent / "test_strategy" / "libDungeonHunter2.so")
    parser.add_argument("--compiler", default="g++")
    parser.add_argument("--output", type=Path,
                        default=MODULE / "build" / "character-aggro-target-search" /
                        "character-aggro-target-search-host.exe")
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build" / "character-aggro-target-search" /
                        "validation.json")
    args = parser.parse_args()

    original = verify_original(args.original.resolve())
    compiler = shutil.which(args.compiler)
    require(compiler is not None, f"C++ compiler not found: {args.compiler}")
    output = args.output.resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [
        compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic",
        str(MODULE / "character_aggro_target_search.cpp"),
        str(HERE / "character_aggro_target_search.cpp"),
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
            "Host test of the `_UpdateAggro` TargetList(owner,0x7fffffff,2,1) "
            "candidate path over a borrowed intrusive room registry; friend/neutral/enemy "
            "dispatch is recorded in source pop order but its classifier and event routing "
            "are not implemented here. No Android or live actor integration is claimed."
        ),
    }
    report_path = args.report.resolve()
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(run.stdout.strip())
    function_count = sum(row["kind"] == "function" for row in original["verified_ranges"])
    vtable_count = sum(row["kind"] == "vtable" for row in original["verified_ranges"])
    print(f"pinned {function_count} function and {vtable_count} vtable ranges")
    print(f"report: {report_path}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError, RuntimeError) as exc:
        raise SystemExit(f"ERROR: {exc}") from exc
