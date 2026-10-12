#!/usr/bin/env python3
"""Print Ghidra pseudocode for queued decompiled functions.

  python3 tools/decomp_extract.py --category core-platform --next 20
  python3 tools/decomp_extract.py --category core-platform --owner Console
  python3 tools/decomp_extract.py --address 0x3cbb08
  python3 tools/decomp_extract.py --category vox-audio --remaining

--next/--owner skip addresses already present in the category's ledger.csv.
"""
from __future__ import annotations

import argparse
import csv
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DECOMP = ROOT / "recovered" / "native" / "decompiled" / "libDungeonHunter2.so"
PORT = ROOT / "port" / "decomp-reconstruction"


def index() -> dict[int, dict]:
    out = {}
    for line in (DECOMP / "function-index.jsonl").open(encoding="utf-8"):
        r = json.loads(line)
        out[int(r["elf_address"], 16)] = r
    return out


def pseudo(r: dict) -> str:
    with (DECOMP / r["file"]).open("rb") as fh:
        fh.seek(r["byte_offset"])
        return fh.read(r["byte_length"]).decode("utf-8", errors="replace")


def done(category: str) -> set[int]:
    led = PORT / category / "ledger.csv"
    if not led.exists():
        return set()
    return {int(row["elf_address"], 16) for row in csv.DictReader(led.open(encoding="utf-8")) if row.get("elf_address")}


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--category")
    ap.add_argument("--next", type=int, default=0)
    ap.add_argument("--owner")
    ap.add_argument("--address", action="append", default=[])
    ap.add_argument("--remaining", action="store_true", help="print counts only")
    ap.add_argument("--max-bytes", type=int, default=0, help="skip functions whose body is larger")
    a = ap.parse_args()
    idx = index()
    targets = [int(x, 16) for x in a.address]
    if a.category:
        q = list(csv.DictReader((PORT / a.category / "queue.csv").open(encoding="utf-8")))
        finished = done(a.category)
        todo = [r for r in q if int(r["elf_address"], 16) not in finished]
        if a.owner:
            todo = [r for r in todo if r["owner"] == a.owner]
        if a.max_bytes:
            todo = [r for r in todo if int(r["body_bytes"]) <= a.max_bytes]
        if a.remaining:
            print(f"{a.category}: {len(todo)} remaining of {len(q)} queued ({len(finished)} in ledger)")
            return 0
        targets += [int(r["elf_address"], 16) for r in todo[: a.next or len(todo)]]
    for t in targets:
        r = idx.get(t)
        if r is None:
            print(f"// no pseudocode for 0x{t:x}", file=sys.stderr)
            continue
        print(f"// ===== elf_address=0x{t:x} size={r['body_bytes']} symbol={r['name']}")
        print(pseudo(r).rstrip())
        print()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
