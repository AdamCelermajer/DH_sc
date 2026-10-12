#!/usr/bin/env python3
"""Plan Track B worker batches: unledgered (or partial) queue rows grouped by owner class.

  py -3 tools/trackb/plan_batches.py --categories game-world game-ui --max-funcs 20 --max-bytes 24000 > batches.json
  py -3 tools/trackb/plan_batches.py --categories glitch-scene-collada-gui --redo-partial

Each batch holds functions of one owner only, so one agent owns that class's files while it
runs. Batches of the same owner are numbered in order (part 0, 1, ...) and must run one
after another. Rows already in ledger.csv or in a ledger.d/*.csv part are skipped, unless
--redo-partial selects rows whose current status is partial.
"""
import argparse
import csv
import json
import re
from collections import OrderedDict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
PORT = ROOT / "port" / "decomp-reconstruction"


def statuses(cat: Path) -> dict:
    out = {}
    files = [cat / "ledger.csv"] + sorted((cat / "ledger.d").glob("*.csv"))
    for f in files:
        if f.exists():
            for r in csv.DictReader(f.open(encoding="utf-8")):
                if r.get("elf_address"):
                    out[r["elf_address"].lower().removeprefix("0x")] = r["status"]
    return out


def slug(s: str) -> str:
    return re.sub(r"[^A-Za-z0-9]+", "_", s).strip("_")[:48] or "global"


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--categories", nargs="+", required=True)
    ap.add_argument("--max-funcs", type=int, default=20)
    ap.add_argument("--max-bytes", type=int, default=24000)
    ap.add_argument("--redo-partial", action="store_true")
    ap.add_argument("--prefix", default="b")
    a = ap.parse_args()
    batches = []
    for c in a.categories:
        cat = PORT / c
        st = statuses(cat)
        by_owner = OrderedDict()
        for r in csv.DictReader((cat / "queue.csv").open(encoding="utf-8")):
            addr = r["elf_address"].lower().removeprefix("0x")
            s = st.get(addr)
            if (a.redo_partial and s != "partial") or (not a.redo_partial and s is not None):
                continue
            by_owner.setdefault(r["owner"], []).append(r)
        for owner, rows in by_owner.items():
            rows.sort(key=lambda r: int(r["elf_address"], 16))
            parts, cur, size = [], [], 0
            for r in rows:
                b = int(r["body_bytes"] or 0)
                if cur and (len(cur) >= a.max_funcs or size + b > a.max_bytes):
                    parts.append(cur)
                    cur, size = [], 0
                cur.append(r)
                size += b
            if cur:
                parts.append(cur)
            for i, p in enumerate(parts):
                batches.append({
                    "id": f"{a.prefix}-{c}-{slug(owner)}-{i}",
                    "category": c, "owner": owner, "part": i, "parts": len(parts),
                    "functions": [{"address": "0x" + r["elf_address"].lower().removeprefix("0x"), "bytes": int(r["body_bytes"] or 0),
                                   "demangled": r["demangled"]} for r in p],
                    "bytes": sum(int(r["body_bytes"] or 0) for r in p),
                })
    print(json.dumps(batches, indent=0))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
