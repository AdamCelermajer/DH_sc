#!/usr/bin/env python3
"""Classify serialized BRES fixup target offsets against block-bearing files.

The script reads only the extracted cache and the checked-in block-table census.
It does not extract, alter, or rewrite source assets.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import struct
from bisect import bisect_right
from collections import Counter
from pathlib import Path


HEADER = struct.Struct("<IHH13I")
U32 = struct.Struct("<I")
CATEGORIES = (
    "header",
    "pre_fixup_gap",
    "fixup_table",
    "main_image",
    "block_table",
    "block_payload",
    "trailer",
    "eof_endpoint",
    "out_of_file",
)


def classify(target: int, *, header_size: int, fixup_offset: int,
             after_fixups: int, bulk_offset: int, table_end: int,
             bulk_end: int, file_size: int, trailer_size: int,
             row_starts: list[int], row_sizes: list[int]) -> tuple[str, int | None]:
    if target < header_size:
        return "header", None
    if header_size <= target < fixup_offset:
        return "pre_fixup_gap", None
    if fixup_offset <= target < after_fixups:
        return "fixup_table", None
    if after_fixups <= target < bulk_offset:
        return "main_image", None
    if bulk_offset <= target < table_end:
        return "block_table", None
    if table_end <= target < bulk_end:
        row = bisect_right(row_starts, target) - 1
        if row >= 0 and row_starts[row] <= target < row_starts[row] + row_sizes[row]:
            return "block_payload", row
    if bulk_end <= target < file_size:
        # A nonempty trailer begins exactly at bulk_end. With no trailer,
        # bulk_end equals the file size and is handled below as an endpoint.
        if trailer_size and target >= bulk_end:
            return "trailer", None
    if target == file_size:
        return "eof_endpoint", None
    if target > file_size:
        return "out_of_file", None
    # Any remaining in-range byte would indicate an unmodeled gap.
    return "out_of_file", None


def scan(asset_root: Path, census_path: Path) -> dict[str, object]:
    census = json.loads(census_path.read_text(encoding="utf-8"))
    if census.get("file_count") != 2901 or census.get("files_with_blocks") != 1505:
        raise ValueError("unexpected block-table census scope")
    block_entries = [item for item in census["files"] if item["block_count"]]
    entries = census["files"]
    totals_all: Counter[str] = Counter()
    totals_nonfirst: Counter[str] = Counter()
    source_field_totals: Counter[str] = Counter()
    source_to_target: Counter[tuple[str, str]] = Counter()
    payload_target_sources: Counter[str] = Counter()
    payload_rows_with_table_reference = 0
    payload_rows_with_main_reference = 0
    payload_rows_with_exactly_one_table_reference = 0
    payload_rows_with_exactly_one_main_reference = 0
    payload_internal_same_row = 0
    payload_internal_cross_row = 0
    cross_row_examples: list[dict[str, object]] = []
    files_by_category: Counter[str] = Counter()
    nonfirst_files_by_category: Counter[str] = Counter()
    payload_row_count = 0
    payload_rows_referenced = 0
    records: list[dict[str, object]] = []
    total_fixups = 0
    total_nonfirst = 0
    first_entry_field_values: Counter[tuple[int, int]] = Counter()

    for entry in entries:
        rel = Path(*entry["path"].split("/"))
        path = asset_root / rel
        data = path.read_bytes()
        digest = hashlib.sha256(data).hexdigest()
        if len(data) != entry["size"] or digest != entry["sha256"]:
            raise ValueError(f"census identity mismatch: {entry['path']}")

        fields = HEADER.unpack_from(data)
        (magic, byte_order, flags, header_size, file_size, fixup_count,
         reference_base, fixup_offset, after_fixups, root_offset,
         auxiliary_offset, bulk_offset, bulk_size, block_count,
         allocation_selector, trailer_size) = fields
        if magic != 0x53455242 or byte_order != 0xFFFE:
            raise ValueError(f"unexpected BRES header: {entry['path']}")
        if file_size != len(data) or block_count != entry["block_count"]:
            raise ValueError(f"census/header mismatch: {entry['path']}")
        if fixup_offset + fixup_count * 4 != after_fixups:
            raise ValueError(f"fixup table extent mismatch: {entry['path']}")
        if fixup_offset != 60 or bulk_offset != entry["bulk_offset"]:
            raise ValueError(f"unexpected corpus layout: {entry['path']}")

        rows: list[tuple[int, int]] = []
        for row_index in range(block_count):
            row_size, row_start = struct.unpack_from("<II", data, bulk_offset + row_index * 8)
            expected = entry["rows"][row_index]
            if [row_size, row_start] != expected:
                raise ValueError(f"census/row mismatch: {entry['path']} row {row_index}")
            rows.append((row_size, row_start))
        table_end = bulk_offset + block_count * 8
        bulk_end = bulk_offset + bulk_size
        row_sizes = [size for size, _ in rows]
        row_starts = [start for _, start in rows]
        if rows and (row_starts[0] != table_end
                     or any(row_starts[i] + row_sizes[i] != row_starts[i + 1]
                            for i in range(len(rows) - 1))
                     or row_starts[-1] + row_sizes[-1] != bulk_end):
            raise ValueError(f"payload tiling mismatch: {entry['path']}")

        per_all: Counter[str] = Counter()
        per_nonfirst: Counter[str] = Counter()
        per_source_to_target: Counter[tuple[str, str]] = Counter()
        per_row: Counter[int] = Counter()
        per_row_target_sources: dict[int, Counter[str]] = {}
        file_same_row_targets = 0
        file_cross_row_targets = 0
        target_examples: dict[str, list[int]] = {name: [] for name in CATEGORIES}
        field_examples: dict[str, list[int]] = {name: [] for name in CATEGORIES}
        first_pair: tuple[int, int] | None = None
        for fixup_index in range(fixup_count):
            field = U32.unpack_from(data, fixup_offset + fixup_index * 4)[0]
            if field % 4 or field + 4 > len(data):
                raise ValueError(f"invalid fixup field: {entry['path']} #{fixup_index}")
            target = U32.unpack_from(data, field)[0]
            category, row_index = classify(
                target,
                header_size=header_size,
                fixup_offset=fixup_offset,
                after_fixups=after_fixups,
                bulk_offset=bulk_offset,
                table_end=table_end,
                bulk_end=bulk_end,
                file_size=file_size,
                trailer_size=trailer_size,
                row_starts=row_starts,
                row_sizes=row_sizes,
            )
            source_category, source_row = classify(
                field,
                header_size=header_size,
                fixup_offset=fixup_offset,
                after_fixups=after_fixups,
                bulk_offset=bulk_offset,
                table_end=table_end,
                bulk_end=bulk_end,
                file_size=file_size,
                trailer_size=trailer_size,
                row_starts=row_starts,
                row_sizes=row_sizes,
            )
            source_field_totals[source_category] += 1
            source_to_target[(source_category, category)] += 1
            per_source_to_target[(source_category, category)] += 1
            per_all[category] += 1
            totals_all[category] += 1
            if len(target_examples[category]) < 4:
                target_examples[category].append(target)
            if len(field_examples[category]) < 4:
                field_examples[category].append(field)
            if row_index is not None:
                per_row[row_index] += 1
                payload_target_sources[source_category] += 1
                per_row_target_sources.setdefault(row_index, Counter())[source_category] += 1
                if source_category == "block_payload":
                    if source_row == row_index:
                        payload_internal_same_row += 1
                        file_same_row_targets += 1
                    else:
                        payload_internal_cross_row += 1
                        file_cross_row_targets += 1
                        if len(cross_row_examples) < 20:
                            cross_row_examples.append({
                                "path": entry["path"],
                                "source_row": source_row,
                                "target_row": row_index,
                                "field_offset": field,
                                "target_offset": target,
                                "file_sha256": digest,
                            })
            if fixup_index == 0:
                first_pair = (field, target)
                first_entry_field_values[first_pair] += 1
            else:
                per_nonfirst[category] += 1
                totals_nonfirst[category] += 1

        total_fixups += fixup_count
        total_nonfirst += max(0, fixup_count - 1)
        payload_row_count += block_count
        payload_rows_referenced += len(per_row)
        for counts in per_row_target_sources.values():
            table_count = counts["block_table"]
            main_count = counts["main_image"]
            payload_rows_with_table_reference += table_count > 0
            payload_rows_with_main_reference += main_count > 0
            payload_rows_with_exactly_one_table_reference += table_count == 1
            payload_rows_with_exactly_one_main_reference += main_count == 1
        file_payload_row_source_coverage = {
            source: sum(counts[source] > 0 for counts in per_row_target_sources.values())
            for source in ("block_table", "main_image")
        }
        for category, count in per_all.items():
            if count:
                files_by_category[category] += 1
        for category, count in per_nonfirst.items():
            if count:
                nonfirst_files_by_category[category] += 1

        records.append({
            "path": entry["path"],
            "size": len(data),
            "sha256": digest,
            "fixup_count": fixup_count,
            "block_count": block_count,
            "bulk_offset": bulk_offset,
            "block_table_end": table_end,
            "bulk_end": bulk_end,
            "trailer_size": trailer_size,
            "first_fixup_field_and_target": list(first_pair) if first_pair else None,
            "all_target_counts": {name: per_all[name] for name in CATEGORIES if per_all[name]},
            "nonfirst_target_counts": {name: per_nonfirst[name] for name in CATEGORIES if per_nonfirst[name]},
            "payload_rows_with_target": len(per_row),
            "payload_rows_with_target_by_source": file_payload_row_source_coverage,
            "payload_to_payload_same_row_targets": file_same_row_targets,
            "payload_to_payload_cross_row_targets": file_cross_row_targets,
            "target_examples_by_category": {name: values for name, values in target_examples.items() if values},
            "target_field_examples_by_category": {name: values for name, values in field_examples.items() if values},
            "source_field_to_target_counts": {
                source: {
                    target: per_source_to_target[(source, target)]
                    for target in CATEGORIES
                    if per_source_to_target[(source, target)]
                }
                for source in CATEGORIES
                if any(per_source_to_target[(source, target)] for target in CATEGORIES)
            },
        })

    def distribution(counter: Counter[str], denominator: int) -> dict[str, object]:
        return {
            name: {
                "fixup_entries": counter[name],
                "entry_percent": round(100 * counter[name] / denominator, 6) if denominator else 0.0,
            }
            for name in CATEGORIES
        }

    return {
        "schema": "dh2-bres-fixup-target-audit/v1",
        "scope": {
            "block_table_census_path": str(census_path),
            "asset_root": str(asset_root),
            "block_table_census_source_file_index_sha256": census["source_file_index_sha256"],
            "apk_sha256": "32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200",
            "arm_elf_sha256": "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80",
            "files_in_full_corpus": len(entries),
            "block_bearing_files": len(block_entries),
            "files_hash_and_row_matched_to_census": len(records),
            "total_fixup_entries": total_fixups,
            "nonfirst_fixup_entries_excluding_special_first_entry": total_nonfirst,
            "first_fixup_field_target_pairs": [
                {"field": field, "target": target, "files": count}
                for (field, target), count in sorted(first_entry_field_values.items())
            ],
            "offset_region_definitions": {
                "header": "[0, header_size)",
                "pre_fixup_gap": "[header_size, fixup_offset)",
                "fixup_table": "[fixup_offset, after_fixups)",
                "main_image": "[after_fixups, bulk_offset)",
                "block_table": "[bulk_offset, bulk_offset + 8 * block_count)",
                "block_payload": "the row payload intervals [row_file_offset, row_file_offset + row_size)",
                "trailer": "[bulk_offset + bulk_size, file_size) when trailer_size is nonzero",
                "eof_endpoint": "target exactly equals file_size",
                "out_of_file": "target exceeds file_size or falls in an unmodeled in-range gap",
            },
            "semantics_limit": "These are serialized file-offset classifications, not proof that the original reader accepted or dereferenced each target. The original fixup code has separate context remapping behavior.",
        },
        "all_fixup_entries": distribution(totals_all, total_fixups),
        "nonfirst_entries_only": distribution(totals_nonfirst, total_nonfirst),
        "files_with_target_by_category": {name: files_by_category[name] for name in CATEGORIES},
        "nonfirst_files_with_target_by_category": {name: nonfirst_files_by_category[name] for name in CATEGORIES},
        "fixup_field_source_regions": {name: source_field_totals[name] for name in CATEGORIES},
        "fixup_field_source_to_target_regions": {
            source: {
                target: source_to_target[(source, target)]
                for target in CATEGORIES
                if source_to_target[(source, target)]
            }
            for source in CATEGORIES
            if any(source_to_target[(source, target)] for target in CATEGORIES)
        },
        "payload_target_sources": {name: payload_target_sources[name] for name in CATEGORIES},
        "payload_row_reference_coverage_by_source": {
            "rows_with_block_table_row_pointer": payload_rows_with_table_reference,
            "rows_with_main_image_pointer": payload_rows_with_main_reference,
            "rows_with_exactly_one_block_table_row_pointer": payload_rows_with_exactly_one_table_reference,
            "rows_with_exactly_one_main_image_pointer": payload_rows_with_exactly_one_main_reference,
            "total_rows": payload_row_count,
        },
        "payload_to_payload_fixups": {
            "same_row_targets": payload_internal_same_row,
            "cross_row_targets": payload_internal_cross_row,
            "cross_row_examples": cross_row_examples,
        },
        "payload_row_coverage": {
            "rows_with_at_least_one_fixup_target": payload_rows_referenced,
            "rows_in_block_bearing_files": payload_row_count,
            "percent_rows_referenced": round(100 * payload_rows_referenced / payload_row_count, 6) if payload_row_count else 0.0,
        },
        "files": records,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("asset_root", type=Path, help="root containing the extracted files/ tree")
    parser.add_argument("census", type=Path, help="block-table-audit/census.json")
    parser.add_argument("--output", type=Path, help="write full per-file JSON result")
    args = parser.parse_args()
    result = scan(args.asset_root.resolve(), args.census.resolve())
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(result, indent=2, sort_keys=True) + "\n", encoding="utf-8", newline="\n")
    summary = {
        key: result[key]
        for key in (
            "schema", "scope", "all_fixup_entries", "nonfirst_entries_only",
            "files_with_target_by_category", "nonfirst_files_with_target_by_category",
            "fixup_field_source_regions", "fixup_field_source_to_target_regions",
            "payload_target_sources", "payload_row_reference_coverage_by_source",
            "payload_to_payload_fixups", "payload_row_coverage",
        )
    }
    print(json.dumps(summary, indent=2, sort_keys=True))


if __name__ == "__main__":
    main()
