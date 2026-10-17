#!/usr/bin/env python3
"""Census raw BRES block-table records without interpreting their fields."""

from __future__ import annotations

import argparse
import hashlib
import json
import struct
import zlib
from collections import Counter
from pathlib import Path


HEADER = struct.Struct("<IHH13I")


def scan(root: Path) -> dict[str, object]:
    files = sorted(root.rglob("*.bdae"), key=lambda p: p.as_posix().casefold())
    summaries: list[dict[str, object]] = []
    errors: list[dict[str, str]] = []
    source_index = hashlib.sha256()

    for path in files:
        try:
            data = path.read_bytes()
            if len(data) < HEADER.size:
                raise ValueError("short header")
            fields = HEADER.unpack_from(data)
            (magic, byte_order, flags, header_size, file_size, fixup_count,
             reference_base, fixup_offset, after_fixups, root_offset,
             auxiliary_offset, bulk_offset, bulk_size, block_count,
             allocation_selector, trailer_size) = fields
            if magic != 0x53455242:
                raise ValueError("wrong magic")
            if byte_order != 0xfffe:
                raise ValueError("unexpected byte-order marker")
            if header_size != HEADER.size or file_size != len(data):
                raise ValueError("header size or file length mismatch")
            if reference_base != 0:
                raise ValueError("nonzero reference/base selector")
            if fixup_offset + fixup_count * 4 != after_fixups:
                raise ValueError("fixup table end mismatch")

            table_start = bulk_offset
            table_end = table_start + block_count * 8
            bulk_end = bulk_offset + bulk_size
            reader_image_size = len(data) - fixup_count * 4 - bulk_size - trailer_size
            reader_stream_bulk_start = reader_image_size + fixup_count * 4
            reader_row_bytes = block_count * 8
            reader_payload_bytes = bulk_size - reader_row_bytes
            block_rows: list[tuple[int, int]] = []
            if bulk_offset < after_fixups or bulk_offset > len(data):
                raise ValueError("bulk table starts outside post-fixup image")
            if table_end > len(data) or bulk_end > len(data):
                raise ValueError("block table outside file")
            if table_end > bulk_end:
                raise ValueError("block table exceeds declared bulk span")
            if reader_image_size < HEADER.size:
                raise ValueError("reader image would be shorter than the copied header")
            for i in range(block_count):
                block_rows.append(struct.unpack_from("<II", data, table_start + 8 * i))

            row_length_sum = sum(a for a, _ in block_rows)
            row_second_sum = sum(b for _, b in block_rows)
            expected_payload_offset = table_end
            sequential_offsets = True
            rows_in_bulk = True
            for row_length, row_offset in block_rows:
                sequential_offsets &= row_offset == expected_payload_offset
                rows_in_bulk &= (row_offset >= table_end
                                 and row_offset + row_length <= bulk_end)
                expected_payload_offset = row_offset + row_length
            modeled_bulk_size = block_count * 8 + row_length_sum
            summaries.append({
                "path": path.relative_to(root).as_posix(),
                "size": len(data),
                "sha256": hashlib.sha256(data).hexdigest(),
                "crc32": f"{zlib.crc32(data) & 0xffffffff:08x}",
                "fixup_count": fixup_count,
                "after_fixups": after_fixups,
                "reader_image_size": reader_image_size,
                "reader_source_tail_start": after_fixups,
                "reader_stream_bulk_start": reader_stream_bulk_start,
                "reader_stream_matches_bulk_offset": reader_stream_bulk_start == bulk_offset,
                "block_count": block_count,
                "block_table_start": table_start,
                "block_table_end": table_end,
                "reader_row_bytes": reader_row_bytes,
                "reader_payload_bytes": reader_payload_bytes,
                "bulk_offset": bulk_offset,
                "bulk_size": bulk_size,
                "bulk_end": bulk_end,
                "allocation_selector": allocation_selector,
                "trailer_size": trailer_size,
                "first_word_sum": row_length_sum,
                "second_word_sum": row_second_sum,
                "modeled_bulk_size": modeled_bulk_size,
                "modeled_bulk_size_matches_header": modeled_bulk_size == bulk_size,
                "row_offsets_are_sequential": sequential_offsets,
                "rows_fit_bulk_span": rows_in_bulk,
                "bulk_ends_at_file_end": bulk_end == len(data),
                "bulk_plus_trailer_ends_at_file": bulk_end + trailer_size == len(data),
                "reader_stream_segments_end_at_trailer": (
                    reader_stream_bulk_start + bulk_size + trailer_size == len(data)
                ),
                "rows": [[a, b] for a, b in block_rows],
            })
            source_index.update(path.relative_to(root).as_posix().encode("utf-8"))
            source_index.update(b"\0")
            source_index.update(len(data).to_bytes(8, "little"))
            source_index.update(hashlib.sha256(data).digest())
        except Exception as exc:
            errors.append({"path": path.relative_to(root).as_posix(), "error": str(exc)})

    block_files = [r for r in summaries if r["block_count"]]
    size_match_count = sum(bool(r["modeled_bulk_size_matches_header"]) for r in block_files)
    sequential_count = sum(bool(r["row_offsets_are_sequential"]) for r in block_files)
    rows_fit_count = sum(bool(r["rows_fit_bulk_span"]) for r in block_files)
    bulk_plus_trailer_count = sum(bool(r["bulk_plus_trailer_ends_at_file"]) for r in summaries)
    reader_bulk_start_match_count = sum(
        bool(r["reader_stream_matches_bulk_offset"]) for r in summaries
    )
    reader_stream_end_match_count = sum(
        bool(r["reader_stream_segments_end_at_trailer"]) for r in summaries
    )
    reader_main_header_valid_count = sum(
        int(r["reader_image_size"]) >= HEADER.size for r in summaries
    )
    selector_counts = Counter(int(r["allocation_selector"]) for r in summaries)
    return {
        "root": str(root),
        "file_count": len(files),
        "valid_file_count": len(summaries),
        "error_count": len(errors),
        "source_file_index_sha256": source_index.hexdigest(),
        "total_bytes": sum(int(r["size"]) for r in summaries),
        "total_block_records": sum(int(r["block_count"]) for r in summaries),
        "files_with_blocks": len(block_files),
        "files_without_blocks": len(summaries) - len(block_files),
        "allocation_selector_counts": {str(k): selector_counts[k] for k in sorted(selector_counts)},
        "modeled_bulk_size_matches_header": size_match_count,
        "modeled_bulk_size_mismatches_header": len(block_files) - size_match_count,
        "block_row_offsets_sequential": sequential_count,
        "block_rows_within_bulk_span": rows_fit_count,
        "bulk_plus_trailer_ends_at_file": bulk_plus_trailer_count,
        "reader_bulk_start_matches_declared_offset": reader_bulk_start_match_count,
        "reader_stream_segments_end_at_trailer": reader_stream_end_match_count,
        "reader_image_at_least_header_size": reader_main_header_valid_count,
        "errors": errors,
        "files": summaries,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("root", type=Path, help="root of the recovered BDAE cache")
    parser.add_argument("--output", type=Path, help="write JSON to this path")
    args = parser.parse_args()
    result = scan(args.root.resolve())
    serialized = json.dumps(result, indent=2, sort_keys=True) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(serialized, encoding="utf-8", newline="\n")
    print(json.dumps({k: v for k, v in result.items() if k != "files"}, indent=2))


if __name__ == "__main__":
    main()
