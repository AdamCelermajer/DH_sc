#!/usr/bin/env python3
"""Verify the selected source import and, optionally, its original recovery ZIP."""

import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import zipfile


def digest(data):
    return hashlib.sha256(data).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo-root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--archive", type=Path,
                        help="Optional Dungeon-Hunter-2-Source-Recovery.zip to verify provenance")
    args = parser.parse_args()
    root = args.repo_root.resolve()
    manifest = json.loads((root / "reports/recovery-source-import.json").read_text(encoding="utf-8"))
    archive = args.archive.resolve() if args.archive else None
    if archive and digest(archive.read_bytes()) != manifest["source_archive_sha256"]:
        raise ValueError("Recovery source ZIP SHA-256 differs from the published handoff")

    with zipfile.ZipFile(archive) if archive else _NoArchive() as source:
        if archive and source.testzip() is not None:
            raise ValueError("Recovery source ZIP has a failed member CRC")
        for row in manifest["files"]:
            rel = PurePosixPath(row["path"])
            if rel.is_absolute() or ".." in rel.parts or any(":" in part for part in rel.parts):
                raise ValueError(f"Unsafe imported path: {row['path']}")
            target = root.joinpath(*rel.parts)
            if not target.resolve().is_relative_to(root):
                raise ValueError(f"Imported path leaves repository: {row['path']}")
            data = target.read_bytes()
            if len(data) != row["target_bytes"] or digest(data) != row["target_sha256"]:
                raise ValueError(f"Imported file differs from recorded Git source: {row['path']}")
            if archive:
                original = source.read("dh2-reconstruction/" + row["path"])
                if len(original) != row["source_bytes"] or digest(original) != row["source_sha256"]:
                    raise ValueError(f"Recovery archive member differs: {row['path']}")
    print(json.dumps({"files_verified": len(manifest["files"]),
                      "archive_verified": bool(archive),
                      "source_archive_sha256": manifest["source_archive_sha256"]}, indent=2))


class _NoArchive:
    def __enter__(self):
        return None

    def __exit__(self, *_):
        return False


if __name__ == "__main__":
    main()
