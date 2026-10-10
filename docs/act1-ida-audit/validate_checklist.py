from __future__ import annotations
import argparse
import csv
import json
import re
import sys
from collections import Counter
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[1]
IDA_LIBRARIES = REPO / ".local-inputs" / "ida-apk-export-2026-10-07" / "libraries"
MASTER = HERE / "checklist.csv"
HEADER = [
    "ID", "System", "Subsystem", "IDA library", "IDA address", "IDA function",
    "Source path(s)", "Check / expected behavior", "IDA evidence", "Implementation status",
    "Implementation evidence", "Runtime status", "Acceptance evidence", "Priority", "Owner/session",
]
ADDR_RE = re.compile(r"0x[0-9a-fA-F]+")
PSEUDO_RE = re.compile(r"(?:pseudocode(?:-all)?\.c|pseudocode/[A-Za-z0-9_./+-]+\.c)(?::L(\d+)(?:-L(\d+))?)?")


def load_index(lib: str) -> dict[int, dict]:
    path = IDA_LIBRARIES / lib / "functions.jsonl"
    result: dict[int, dict] = {}
    with path.open("r", encoding="utf-8") as stream:
        for line in stream:
            record = json.loads(line)
            result[int(record["address"], 16)] = record
    return result


def part_files() -> list[Path]:
    return sorted(HERE.glob("part-*.csv"))


def merge_parts() -> None:
    files = part_files()
    if not files:
        raise RuntimeError("No part-*.csv files found")
    rows = []
    indexes: dict[str, dict[int, dict]] = {}
    for path in files:
        with path.open("r", encoding="utf-8-sig", newline="") as stream:
            reader = csv.DictReader(stream)
            if reader.fieldnames != HEADER:
                raise RuntimeError(f"Header mismatch in {path.name}: {reader.fieldnames}")
            for row in reader:
                row = dict(row)
                lib = row.get("IDA library", "")
                if lib not in indexes:
                    indexes[lib] = load_index(lib)
                addresses = [int(value, 16) for value in ADDR_RE.findall(row.get("IDA address", ""))]
                canonical_names = []
                for address in addresses:
                    record = indexes[lib].get(address)
                    if record is None:
                        raise RuntimeError(f"{path.name}: {row.get('ID')} references unknown {lib} address {hex(address)}")
                    canonical_names.append(record.get("demangled") or record.get("name") or "")
                if not canonical_names:
                    raise RuntimeError(f"{path.name}: {row.get('ID')} has no IDA address")
                # Store the exact exported function names for unambiguous future lookup.
                row["IDA function"] = "; ".join(canonical_names)
                rows.append(row)
    rows.sort(key=lambda row: row["ID"])
    with MASTER.open("w", encoding="utf-8-sig", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=HEADER, extrasaction="raise")
        writer.writeheader()
        writer.writerows(rows)
    print(f"Merged {len(rows)} rows from {len(files)} fragments into {MASTER.name}")


def validate(path: Path) -> int:
    indexes: dict[str, dict[int, dict]] = {}
    pseudo_line_counts: dict[Path, int] = {}
    errors: list[str] = []
    systems = Counter()
    ids = set()
    exact_checks = Counter()
    rows = []
    with path.open("r", encoding="utf-8-sig", newline="") as stream:
        reader = csv.DictReader(stream)
        if reader.fieldnames != HEADER:
            raise RuntimeError(f"Header mismatch in {path.name}: {reader.fieldnames}")
        for row in reader:
            rows.append(row)
    for rownum, row in enumerate(rows, start=2):
        row_id = row.get("ID", "")
        if not row_id or row_id in ids:
            errors.append(f"{path.name}:{rownum}: blank or duplicate ID {row_id!r}")
        ids.add(row_id)
        if not row.get("Check / expected behavior", "").strip():
            errors.append(f"{path.name}:{rownum}: empty check")
        systems[row.get("System", "")] += 1
        pair_key = (row.get("System", ""), row.get("Subsystem", ""), row.get("IDA function", ""), row.get("Check / expected behavior", ""))
        exact_checks[pair_key] += 1

        lib = row.get("IDA library", "")
        if lib not in indexes:
            try:
                indexes[lib] = load_index(lib)
            except Exception as exc:
                errors.append(f"{path.name}:{rownum}: cannot load IDA index {lib!r}: {exc}")
                indexes[lib] = {}
        addresses = [int(value, 16) for value in ADDR_RE.findall(row.get("IDA address", ""))]
        if not addresses:
            errors.append(f"{path.name}:{rownum}: no IDA address")
        name_cell = row.get("IDA function", "")
        if len(addresses) > 1:
            names = [value.strip() for value in name_cell.split("; ")]
        else:
            names = [name_cell.strip()]
        for index, address in enumerate(addresses):
            record = indexes.get(lib, {}).get(address)
            if record is None:
                errors.append(f"{path.name}:{rownum}: {lib} missing IDA address {hex(address)}")
                continue
            candidates = [record.get("name") or "", record.get("demangled") or ""]
            presented = names[index] if index < len(names) else name_cell
            normalize = lambda value: re.sub(r"\s+", "", value or "").casefold()
            if not any(candidate and normalize(presented) == normalize(candidate) for candidate in candidates):
                errors.append(f"{path.name}:{rownum}: address/function mismatch {hex(address)} vs {presented!r}; IDA={candidates!r}")

        for src in row.get("Source path(s)", "").split("; "):
            src = src.strip()
            if not src or src.lower() in {"not located", "none", "n/a"}:
                continue
            candidate = REPO / Path(src.replace("/", "\\"))
            if not candidate.exists():
                errors.append(f"{path.name}:{rownum}: source path does not exist: {src}")

        lib_root = IDA_LIBRARIES / lib
        for match in PSEUDO_RE.finditer(row.get("IDA evidence", "")):
            pseudo = match.group(0).split(":L", 1)[0]
            pseudo_path = lib_root / Path(pseudo.replace("/", "\\"))
            if not pseudo_path.exists():
                errors.append(f"{path.name}:{rownum}: pseudocode evidence path does not exist: {pseudo}")
            elif match.group(1):
                if pseudo_path not in pseudo_line_counts:
                    with pseudo_path.open("r", encoding="utf-8", errors="replace") as pseudo_stream:
                        pseudo_line_counts[pseudo_path] = sum(1 for _ in pseudo_stream)
                line_count = pseudo_line_counts[pseudo_path]
                start = int(match.group(1))
                end = int(match.group(2) or match.group(1))
                if start < 1 or end < start or end > line_count:
                    errors.append(f"{path.name}:{rownum}: pseudocode line range {start}-{end} invalid (lines={line_count})")

    dupes = sum(1 for count in exact_checks.values() if count > 1)
    print(f"Validated {len(rows)} rows in {path.name}")
    print(f"Systems: {dict(sorted(systems.items()))}")
    print(f"Duplicate exact-check groups: {dupes}")
    if len(rows) < 1000:
        errors.append(f"Only {len(rows)} rows; the requested backlog threshold is 1000")
    if errors:
        print(f"Issues: {len(errors)}")
        for issue in errors[:100]:
            print("- " + issue)
        if len(errors) > 100:
            print(f"- ... {len(errors) - 100} more")
        return 1
    print("IDA address/function pairs, source paths, and cited pseudocode paths/line ranges are valid.")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--merge", action="store_true", help="merge all part-*.csv files into checklist.csv")
    args = parser.parse_args()
    if args.merge:
        merge_parts()
    target = MASTER if MASTER.exists() else None
    if target is None:
        raise RuntimeError("No checklist.csv yet; pass --merge")
    return validate(target)

if __name__ == "__main__":
    sys.exit(main())
