#!/usr/bin/env python3
"""Verify the owner's complete cache ZIP without extracting or publishing assets."""

from __future__ import annotations

import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path, PurePosixPath
import zipfile


ROOT = PurePosixPath("com.gameloft.android.GAND.GloftD2SS/files")
INTERESTING = {
    "data/3d/characters/prince/prince_modular.bdae",
    "data/sounds/m_world_map.wav",
}


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        while block := source.read(1024 * 1024):
            digest.update(block)
    return digest.hexdigest()


def audit(path: Path) -> dict:
    extensions: Counter[str] = Counter()
    formats: Counter[str] = Counter()
    seen: set[str] = set()
    interesting: dict[str, dict] = {}
    files = directories = total_bytes = 0
    with zipfile.ZipFile(path) as archive:
        for entry in archive.infolist():
            name = entry.filename
            member = PurePosixPath(name)
            if member.is_absolute() or ".." in member.parts or "\\" in name:
                raise ValueError(f"unsafe path: {name!r}")
            if member in (ROOT.parent, ROOT):
                continue
            if member.parts[: len(ROOT.parts)] != ROOT.parts:
                raise ValueError(f"unexpected root: {name!r}")
            relative = PurePosixPath(*member.parts[len(ROOT.parts) :])
            key = str(relative).casefold()
            if key in seen:
                raise ValueError(f"duplicate path: {name!r}")
            seen.add(key)
            if (entry.external_attr >> 16) & 0o170000 == 0o120000:
                raise ValueError(f"symlink entry: {name!r}")
            if entry.is_dir():
                directories += 1
                continue
            files += 1
            total_bytes += entry.file_size
            extensions[relative.suffix.lower()] += 1
            count = 0
            first = b""
            with archive.open(entry) as stream:
                while block := stream.read(1024 * 1024):
                    if not first:
                        first = block[:16]
                    count += len(block)
            # ZipExtFile checks CRC when fully consumed above.
            if count != entry.file_size:
                raise ValueError(f"length mismatch: {name!r}")
            if relative.suffix.lower() == ".bdae":
                formats["BRES"] += first[:4] == b"BRES"
            if relative.suffix.lower() == ".tga":
                formats["BTEXpvr"] += first[:8] == b"BTEXpvr\0"
                formats["other_tga"] += first[:8] != b"BTEXpvr\0"
            if str(relative).casefold() in INTERESTING:
                interesting[str(relative)] = {"bytes": count, "crc32": f"{entry.CRC:08x}"}
    return {
        "archive_sha256": sha256(path),
        "archive_bytes": path.stat().st_size,
        "zip_integrity": "all entries read with valid CRC and length",
        "files": files,
        "directories_below_files_root": directories,
        "uncompressed_file_bytes": total_bytes,
        "extensions": dict(sorted(extensions.items())),
        "format_signatures": dict(sorted(formats.items())),
        "key_entries": interesting,
        "scope": "archive integrity and inventory; game loading and asset-reference completeness untested",
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("archive", type=Path)
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()
    report = audit(args.archive)
    content = json.dumps(report, indent=2) + "\n"
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(content, encoding="utf-8")
    print(content, end="")


if __name__ == "__main__":
    main()
