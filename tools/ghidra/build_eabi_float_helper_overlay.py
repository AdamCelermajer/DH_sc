#!/usr/bin/env python3
"""Build a supplemental Ghidra export for callers fixed by ARM EABI float prototypes."""
import argparse
import hashlib
import json
import os
from pathlib import Path

NO_RETURN = "WARNING: Subroutine does not return"


def load_index(root):
    rows = [json.loads(line) for line in (root / "function-index.jsonl").read_text(
        encoding="utf-8").splitlines() if line.strip()]
    indexed = {}
    blobs = {}
    for row in rows:
        address = row["elf_address"].lower().removeprefix("0x")
        if address in indexed:
            raise ValueError(f"duplicate function index address: {address}")
        indexed[address] = row

    def body(row):
        shard = row["file"]
        if shard not in blobs:
            blobs[shard] = (root / shard).read_bytes()
        start = row["byte_offset"]
        raw = blobs[shard][start:start + row["byte_length"]]
        if len(raw) != row["byte_length"]:
            raise ValueError(f"truncated body at {row['elf_address']}")
        text = raw.decode("utf-8")
        if not text.startswith("/* address=") or "\n" not in text:
            raise ValueError(f"invalid pseudocode record at {row['elf_address']}")
        return raw, text.split("\n", 1)[1]

    return indexed, body


def sha256(path):
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--baseline", type=Path, required=True,
                        help="checked-in archival Ghidra export directory")
    parser.add_argument("--refreshed", type=Path, required=True,
                        help="corrected Ghidra export directory")
    parser.add_argument("--callers", type=Path, required=True,
                        help="ELF-address TSV for the affected callers")
    parser.add_argument("--original-elf", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True,
                        help="new, empty overlay output directory")
    parser.add_argument("--ghidra-version", default="11.0.3")
    parser.add_argument("--expected-cleared", type=int, default=1424)
    args = parser.parse_args()

    baseline_root = args.baseline.resolve()
    refreshed_root = args.refreshed.resolve()
    output = args.output.resolve()
    baseline, baseline_body = load_index(baseline_root)
    refreshed, refreshed_body = load_index(refreshed_root)
    targets = [line.split("\t", 1)[0].lower().removeprefix("0x")
               for line in args.callers.read_text(encoding="utf-8").splitlines()
               if line.strip() and not line.startswith("#")]
    if len(targets) != len(set(targets)):
        raise ValueError("caller list contains duplicate addresses")
    existing = list(output.iterdir()) if output.exists() else []
    generated_name = lambda path: (path.name in {
        "function-index.jsonl", "summary.json", "overlay-manifest.json"
    } or (path.name.startswith("functions-") and path.name.endswith(".pseudo.c")))
    if existing and ((output / "overlay-manifest.json").exists() or
                     any(not generated_name(path) for path in existing)):
        raise ValueError(f"refusing to overwrite a completed or non-generated output: {output}")

    include = []
    residual = 0
    overlay_any_warning = 0
    for address in targets:
        old = baseline.get(address)
        new = refreshed.get(address)
        if old is None or new is None:
            raise ValueError(f"caller absent from one export: {address}")
        if old.get("address") != new.get("address"):
            raise ValueError(f"program address changed at ELF offset {address}")
        _, old_code = baseline_body(old)
        new_raw, new_code = refreshed_body(new)
        if not old.get("success") or not old_code.strip():
            raise ValueError(f"baseline body unavailable at {address}")
        if not new.get("success") or not new_code.strip():
            raise ValueError(f"refreshed body unavailable at {address}")
        if NO_RETURN not in old_code:
            raise ValueError(f"baseline caller lacks expected cutoff at {address}")
        if NO_RETURN in new_code:
            residual += 1
            continue
        entry = dict(new)
        entry["baseline_name"] = old.get("name")
        entry["baseline_signature"] = old.get("signature")
        overlay_any_warning += "WARNING:" in new_code
        include.append((address, entry, new_raw))

    if len(include) != args.expected_cleared:
        raise ValueError(f"expected {args.expected_cleared} improved callers; got {len(include)}")
    output.mkdir(parents=True, exist_ok=True)
    shards = 0
    shard_bytes = 0
    shard = None
    shard_name = ""
    index_rows = []
    try:
        with (output / "function-index.jsonl").open("w", encoding="utf-8", newline="\n") as index:
            for _, row, raw in include:
                if shard is None or shard_bytes > 1024 * 1024:
                    if shard is not None:
                        shard.close()
                    shard_name = f"functions-{shards:03d}.pseudo.c"
                    shards += 1
                    shard = (output / shard_name).open("wb")
                    header = (f"/* AUTOMATIC RECOVERY: Ghidra {args.ghidra_version}; "
                              "libDungeonHunter2.so.\n"
                              " * Supplemental corrected-helper caller overlay.\n"
                              " * Not buildable original C/C++.\n */\n").encode("utf-8")
                    shard.write(header)
                    shard_bytes = len(header)
                offset = shard_bytes
                shard.write(raw)
                shard_bytes += len(raw)
                row.update(file=shard_name, byte_offset=offset, byte_length=len(raw))
                row["has_warning"] = "WARNING:" in raw.decode("utf-8")
                index.write(json.dumps(row, ensure_ascii=False, separators=(",", ":")) + "\n")
                index_rows.append(row)
    finally:
        if shard is not None:
            shard.close()

    summary = {"library": "libDungeonHunter2.so", "ghidra_version": args.ghidra_version,
               "attempted": len(index_rows), "success": len(index_rows), "failed": 0,
               "shards": shards, "compilable": False, "gameplay_validated": False}
    (output / "summary.json").write_text(json.dumps(summary, indent=2) + "\n",
                                         encoding="utf-8")
    baseline_ref = os.path.relpath(baseline_root, output).replace("\\", "/")
    manifest = {
        "type": "supplemental_pseudocode_overlay",
        "original_elf_sha256": sha256(args.original_elf.resolve()),
        "ghidra_version": args.ghidra_version,
        "baseline_export": baseline_ref,
        "targets_reviewed": len(targets),
        "functions_included": len(index_rows),
        "residual_cutoffs_excluded": residual,
        "selection": "baseline had the exact non-return warning; refreshed decompilation succeeded and that warning cleared",
        "metadata": "index uses refreshed Ghidra name/signature and records baseline_name/baseline_signature aliases",
        "warning_counts": {
            "overlay_any_warning": overlay_any_warning,
            "overlay_non_return_warning": 0,
        },
        "archive_overwritten": False,
        "compilable": False,
        "gameplay_validated": False,
    }
    (output / "overlay-manifest.json").write_text(json.dumps(manifest, indent=2) + "\n",
                                                  encoding="utf-8")
    print(json.dumps({"output": str(output), "functions": len(index_rows),
                      "residual_excluded": residual, "shards": shards,
                      "bytes": sum(path.stat().st_size for path in output.iterdir()
                                   if path.is_file())}, indent=2))


if __name__ == "__main__":
    main()
