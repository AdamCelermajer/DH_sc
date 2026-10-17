#!/usr/bin/env python3
"""Verify the imported Adam evidence and combined original-function ledger.

Run from any directory with ``python tools/verify_combined_function_audit.py``.
This is a read-only integrity check; it does not regenerate the audit or run a
build/emulator.
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import json
import sys
import subprocess
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--root",
        type=Path,
        default=Path(__file__).resolve().parents[1],
        help="DH2R checkout root (defaults to the parent of tools/)",
    )
    parser.add_argument('--git-index', action='store_true',
                        help='Also check the staged Git blobs preserve imported bytes')
    args = parser.parse_args()
    root = args.root.resolve()
    generated = root / "docs" / "generated"
    import_path = generated / "adam-evidence-import.json"
    audit_path = generated / "combined-function-audit.json"
    index_path = (
        root
        / "recovered"
        / "native"
        / "symbols"
        / "libDungeonHunter2.so"
        / "function-index.csv"
    )

    for path in (import_path, audit_path, index_path):
        if not path.is_file():
            print(f"missing required input: {path}", file=sys.stderr)
            return 2

    imports = json.loads(import_path.read_text(encoding="utf-8-sig"))
    audit = json.loads(audit_path.read_text(encoding="utf-8-sig"))

    errors: list[str] = []
    imported_files = imports.get("imported_files", [])
    if len(imported_files) != imports.get("summary", {}).get("files_copied"):
        errors.append("import manifest file count differs from summary")
    for entry in imported_files:
        rel = Path(entry["path"])
        candidate = (root / rel).resolve()
        if not candidate.is_relative_to(root):
            errors.append(f"import path escapes repository: {entry['path']}")
            continue
        if not candidate.is_file():
            errors.append(f"imported file is missing: {entry['path']}")
            continue
        data = candidate.read_bytes()
        if len(data) != entry["size_bytes"]:
            errors.append(
                f"size mismatch: {entry['path']} expected {entry['size_bytes']} got {len(data)}"
            )
        digest = hashlib.sha256(data).hexdigest()
        if digest != entry["sha256"]:
            errors.append(f"SHA-256 mismatch: {entry['path']}")
        if args.git_index:
            staged = subprocess.run(['git', 'show', ':' + entry['path']],
                                    cwd=root, capture_output=True)
            if staged.returncode or hashlib.sha256(staged.stdout).hexdigest() != entry['sha256']:
                errors.append(f"Git index differs from pinned bytes: {entry['path']}")

    with index_path.open(encoding="utf-8-sig", newline="") as stream:
        function_index = {
            int(row["address"], 0): row for row in csv.DictReader(stream)
        }

    mapped_rows = audit.get("mapped_function_starts", [])
    mapped_by_address: dict[int, dict] = {}
    for row in mapped_rows:
        address_text = row.get("address")
        try:
            address = int(address_text, 0)
        except (TypeError, ValueError):
            errors.append(f"invalid mapped address: {address_text!r}")
            continue
        if address in mapped_by_address:
            errors.append(f"duplicate ledger row for ELF start 0x{address:08x}")
        mapped_by_address[address] = row
        if address not in function_index:
            errors.append(f"mapped address absent from function-index.csv: 0x{address:08x}")

    expected_count = audit.get("manifest_counts", {}).get("adam_pinned", {}).get(
        "unique_addresses"
    )
    if expected_count != 1455:
        errors.append(f"pinned mapping denominator expected 1455, found {expected_count!r}")
    if len(mapped_by_address) != expected_count:
        errors.append(
            f"ledger has {len(mapped_by_address)} unique starts; expected {expected_count}"
        )

    if errors:
        for error in errors:
            print(f"FAIL: {error}", file=sys.stderr)
        print(
            f"FAILED: {len(errors)} issue(s); checked {len(imported_files)} imported files "
            f"and {len(mapped_by_address)} mapped addresses.",
            file=sys.stderr,
        )
        return 1

    print(
        f"OK: SHA-256 and sizes match for {len(imported_files)} imported files; "
        f"all {len(mapped_by_address)} ledger addresses occur in function-index.csv."
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
