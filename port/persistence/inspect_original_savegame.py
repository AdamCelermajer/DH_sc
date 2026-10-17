#!/usr/bin/env python3
"""Read-only, fixture-authenticated inspector for recovered DH2 save files.

Only the outer section stream demonstrated by the recovered level-checkpoint
fixtures is decoded. Player-profile, settings, and debug save payloads remain
opaque until their layouts are independently established.
"""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import struct
import sys
from typing import Any, Iterable


MAX_FILE_BYTES = 16 * 1024 * 1024
MAX_SECTION_COUNT = 128
MAX_SECTION_NAME_BYTES = 256
LEVEL_SECTION_NAMES = {"INFO", "OBJS"}


class InspectionError(ValueError):
    """Input is missing, unauthenticated, or outside the evidenced envelope."""


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def load_manifest(manifest_path: Path) -> dict[str, dict[str, Any]]:
    try:
        manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as exc:
        raise InspectionError(f"cannot read cache manifest: {exc}") from exc
    entries = manifest.get("entries") if isinstance(manifest, dict) else None
    if not isinstance(entries, list):
        raise InspectionError("cache manifest has no entries array")
    result: dict[str, dict[str, Any]] = {}
    for entry in entries:
        if not isinstance(entry, dict):
            continue
        logical_path = entry.get("path")
        if isinstance(logical_path, str) and logical_path.lower().endswith(
            (".savegame", ".savegame.bak")
        ):
            result[logical_path] = entry
    return result


def _relative_cache_path(logical_path: str) -> Path:
    """Map manifest package paths to the extracted `.../files` directory."""
    marker = "/files/"
    if marker not in logical_path:
        raise InspectionError(f"manifest save path is not under package files/: {logical_path}")
    suffix = logical_path.split(marker, 1)[1]
    posix = PurePosixPath(suffix)
    if posix.is_absolute() or any(part in ("", ".", "..") for part in posix.parts):
        raise InspectionError(f"unsafe manifest-relative save path: {logical_path}")
    return Path(*posix.parts)


def _manifest_digest(entry: dict[str, Any], logical_path: str) -> str:
    if entry.get("verified") is not True:
        raise InspectionError(f"manifest does not mark save as verified: {logical_path}")
    digest = entry.get("sha256")
    if not isinstance(digest, str) or len(digest) != 64:
        raise InspectionError(f"manifest SHA-256 is missing or malformed: {logical_path}")
    try:
        int(digest, 16)
    except ValueError as exc:
        raise InspectionError(f"manifest SHA-256 is not hexadecimal: {logical_path}") from exc
    return digest.lower()


def verify_file(path: Path, entry: dict[str, Any], logical_path: str) -> tuple[bytes, str]:
    expected = _manifest_digest(entry, logical_path)
    try:
        size = path.stat().st_size
    except OSError as exc:
        raise InspectionError(f"cannot stat {logical_path}: {exc}") from exc
    if size > MAX_FILE_BYTES:
        raise InspectionError(f"save exceeds {MAX_FILE_BYTES} byte inspection limit: {logical_path}")
    try:
        data = path.read_bytes()
    except OSError as exc:
        raise InspectionError(f"cannot read {logical_path}: {exc}") from exc
    actual = sha256_bytes(data)
    if actual != expected:
        raise InspectionError(
            f"SHA-256 mismatch for {logical_path}: expected {expected}, got {actual}"
        )
    manifest_size = entry.get("size")
    if isinstance(manifest_size, int) and manifest_size != len(data):
        raise InspectionError(
            f"size mismatch for {logical_path}: manifest {manifest_size}, actual {len(data)}"
        )
    return data, actual


def _read_u32(data: bytes, offset: int, limit: int) -> tuple[int, int]:
    end = offset + 4
    if offset < 0 or end > limit or end > len(data):
        raise InspectionError(f"truncated uint32 at offset {offset}")
    return struct.unpack_from("<I", data, offset)[0], end


def parse_level_checkpoint_directory(data: bytes) -> dict[str, Any]:
    """Parse only the descriptor prefix evidenced by the verified checkpoint.

    Observed prefix: LE u32 count, then repeated LE u32 name length, ASCII name,
    and an opaque four-byte value. Native code calls each section's callback to
    interpret subsequent bytes; the four-byte values are deliberately not
    called lengths. The remainder is kept as one opaque body because section
    payload boundaries are not established here.
    """
    if len(data) > MAX_FILE_BYTES:
        raise InspectionError(f"file exceeds {MAX_FILE_BYTES} byte inspection limit")
    if len(data) < 4:
        raise InspectionError("truncated level section stream header")
    section_count, cursor = _read_u32(data, 0, len(data))
    if not 1 <= section_count <= MAX_SECTION_COUNT:
        raise InspectionError(f"section count {section_count} is outside 1..{MAX_SECTION_COUNT}")

    sections: list[dict[str, Any]] = []
    for index in range(section_count):
        name_size, cursor = _read_u32(data, cursor, len(data))
        if not 1 <= name_size <= MAX_SECTION_NAME_BYTES:
            raise InspectionError(
                f"section {index} name length {name_size} is outside 1..{MAX_SECTION_NAME_BYTES}"
            )
        name_end = cursor + name_size
        if name_end > len(data):
            raise InspectionError(f"truncated section {index} name at offset {cursor}")
        name_bytes = data[cursor:name_end]
        try:
            name = name_bytes.decode("ascii")
        except UnicodeDecodeError as exc:
            raise InspectionError(f"section {index} name is not ASCII") from exc
        if any(ord(char) < 0x20 or ord(char) > 0x7E for char in name):
            raise InspectionError(f"section {index} name contains non-printable bytes")
        cursor = name_end
        value_offset = cursor
        value, cursor = _read_u32(data, cursor, len(data))
        value_bytes = data[value_offset:cursor]
        sections.append(
            {
                "index": index,
                "name": name,
                "name_offset": name_end - name_size,
                "opaque_descriptor_offset": value_offset,
                "opaque_descriptor_u32_le": value,
                "opaque_descriptor_byte_length": len(value_bytes),
                "opaque_descriptor_sha256": sha256_bytes(value_bytes),
                "descriptor_interpretation": "unclassified; not established as a byte length",
                "classification": "recognized-section-label-body-opaque"
                if name in LEVEL_SECTION_NAMES
                else "unknown-section-label-body-opaque",
            }
        )

    body = data[cursor:]
    return {
        "format": "observed-level-checkpoint-directory-prefix-v1",
        "byte_order": "little-endian",
        "section_count": section_count,
        "sections": sections,
        "opaque_body": {
            "offset": cursor,
            "byte_length": len(body),
            "sha256": sha256_bytes(body),
            "payload_included": False,
            "reason": "section payload boundaries and callback schemas are not decoded",
        },
    }


def _is_level_checkpoint(filename: str) -> bool:
    no_backup = filename[:-4] if filename.lower().endswith(".bak") else filename
    return no_backup.lower().endswith("_level.savegame")


def inspect_entry(
    logical_path: str,
    local_path: Path,
    entry: dict[str, Any],
) -> dict[str, Any]:
    data, digest = verify_file(local_path, entry, logical_path)
    file_record: dict[str, Any] = {
        "manifest_path": logical_path,
        "local_name": local_path.name,
        "local_relative_path": local_path.name,
        "manifest_verified": True,
        "byte_length": len(data),
        "sha256": digest,
        "classification": "opaque-original-save-file",
        "payload_included": False,
    }
    if _is_level_checkpoint(local_path.name):
        file_record["classification"] = "level-checkpoint-section-stream"
        file_record["envelope"] = parse_level_checkpoint_directory(data)
    else:
        file_record["opaque_payload"] = {
            "offset": 0,
            "byte_length": len(data),
            "sha256": digest,
            "reason": "file-specific profile/settings/debug framing is not decoded",
        }
    return file_record


def _compare_backup_pairs(files: list[dict[str, Any]], cache_root: Path) -> list[dict[str, Any]]:
    by_name = {record["local_name"]: record for record in files}
    comparisons: list[dict[str, Any]] = []
    for name in sorted(by_name):
        if not name.lower().endswith(".savegame.bak"):
            continue
        primary_name = name[:-4]
        backup = by_name[name]
        primary = by_name.get(primary_name)
        if primary is None:
            comparisons.append(
                {
                    "primary": primary_name,
                    "backup": name,
                    "status": "primary-not-in-verified-manifest",
                    "backup_sha256": backup["sha256"],
                    "primary_read": False,
                }
            )
            continue
        primary_bytes = (cache_root / primary["local_relative_path"]).read_bytes()
        backup_bytes = (cache_root / backup["local_relative_path"]).read_bytes()
        differing = [i for i, (a, b) in enumerate(zip(primary_bytes, backup_bytes)) if a != b]
        comparisons.append(
            {
                "primary": primary_name,
                "backup": name,
                "status": "compared",
                "same_length": len(primary_bytes) == len(backup_bytes),
                "same_sha256": primary["sha256"] == backup["sha256"],
                "differing_byte_count": len(differing) + abs(len(primary_bytes) - len(backup_bytes)),
                "first_differing_offsets": differing[:32],
            }
        )
    return comparisons


def inspect_cache(cache_files: Path, manifest_path: Path) -> dict[str, Any]:
    manifest_entries = load_manifest(manifest_path)
    cache_root = cache_files.resolve()
    if not cache_root.is_dir():
        raise InspectionError(f"cache files directory does not exist: {cache_files}")
    records: list[dict[str, Any]] = []
    errors: list[dict[str, str]] = []
    for logical_path, entry in sorted(manifest_entries.items()):
        try:
            local_relative = _relative_cache_path(logical_path)
            local_path = (cache_root / local_relative).resolve()
            if not local_path.is_relative_to(cache_root):
                raise InspectionError(f"resolved path escapes cache files directory: {logical_path}")
            record = inspect_entry(logical_path, local_path, entry)
            record["local_relative_path"] = local_relative.as_posix()
            records.append(record)
        except InspectionError as exc:
            errors.append({"manifest_path": logical_path, "error": str(exc)})
    return {
        "schema": "dh2-original-save-inspection/v1",
        "read_only": True,
        "manifest": str(manifest_path),
        "cache_files": str(cache_root),
        "files": records,
        "backup_comparisons": _compare_backup_pairs(records, cache_root),
        "errors": errors,
        "success": not errors,
        "limits": [
            "Only manifest entries marked verified with matching SHA-256 and size are inspected.",
            "Only the outer framing of level checkpoint files is decoded.",
            "INFO and OBJS payloads, plus player/settings/debug save files, remain opaque bytes.",
            "No save file is written, normalized, migrated, or passed to original game callbacks.",
        ],
    }


def copy_verified_fixtures(
    cache_files: Path,
    manifest_path: Path,
    destination: Path,
) -> list[Path]:
    """Copy manifest-verified save fixtures to a caller-owned temporary dir."""
    entries = load_manifest(manifest_path)
    cache_root = cache_files.resolve()
    destination.mkdir(parents=True, exist_ok=True)
    copied: list[Path] = []
    for logical_path, entry in sorted(entries.items()):
        rel = _relative_cache_path(logical_path)
        source = (cache_root / rel).resolve()
        if not source.is_relative_to(cache_root):
            raise InspectionError(f"resolved source escapes cache directory: {logical_path}")
        data, _ = verify_file(source, entry, logical_path)
        target = destination / rel
        target.parent.mkdir(parents=True, exist_ok=True)
        if target.exists():
            raise InspectionError(f"refusing to replace existing fixture copy: {target}")
        target.write_bytes(data)
        verify_file(target, entry, logical_path)
        copied.append(target)
    return copied


def main(argv: Iterable[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache-files", type=Path, required=True, help="extracted cache files/ directory")
    parser.add_argument("--manifest", type=Path, required=True, help="recovered cache-manifest.json")
    args = parser.parse_args(argv)
    try:
        report = inspect_cache(args.cache_files, args.manifest)
    except InspectionError as exc:
        print(json.dumps({"success": False, "error": str(exc)}, indent=2), file=sys.stderr)
        return 1
    print(json.dumps(report, indent=2, sort_keys=True))
    return 0 if report["success"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
