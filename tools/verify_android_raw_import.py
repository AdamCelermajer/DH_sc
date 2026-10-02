#!/usr/bin/env python3
"""Verify the exact historical raw Android code import and optional source ZIP."""

import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import zipfile


ARCHIVE_SHA256 = "b3ff974e2b74f50387465d5665f60d56ac79c29a449c6299745461998045c4d8"
PREFIX = PurePosixPath("recovered/android")
EXPECTED_COUNTS = {"java": 364, "smali": 357}


def digest(data):
    return hashlib.sha256(data).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo-root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--archive", type=Path,
                        help="Optional verified Dungeon-Hunter-2-Source-Recovery.zip")
    args = parser.parse_args()
    root = args.repo_root.resolve()
    folder = root.joinpath(*PREFIX.parts)
    manifest = json.loads((folder / "code-import-manifest.json").read_text(encoding="utf-8"))
    if manifest.get("schema_version") != 1 or manifest.get("source_archive_sha256") != ARCHIVE_SHA256:
        raise ValueError("Manifest schema or source archive hash differs")

    rows = manifest.get("files")
    if not isinstance(rows, list) or len(rows) != sum(EXPECTED_COUNTS.values()):
        raise ValueError("Manifest must enumerate all 721 raw code files")
    seen = set()
    counts = {name: 0 for name in EXPECTED_COUNTS}
    for row in rows:
        rel = PurePosixPath(row["path"])
        parts = rel.parts
        if (len(parts) < len(PREFIX.parts) + 2 or parts[:len(PREFIX.parts)] != PREFIX.parts
                or parts[len(PREFIX.parts)] not in EXPECTED_COUNTS
                or ".." in parts or any(not part or ":" in part for part in parts)):
            raise ValueError(f"Unexpected raw code path: {row['path']}")
        kind = parts[len(PREFIX.parts)]
        if not parts[-1].endswith("." + kind):
            raise ValueError(f"Unexpected raw code extension: {row['path']}")
        if rel in seen:
            raise ValueError(f"Duplicate raw code path: {rel}")
        seen.add(rel)
        counts[kind] += 1
        target = root.joinpath(*parts)
        if target.is_symlink() or not target.is_file() or not target.resolve().is_relative_to(folder):
            raise ValueError(f"Missing or unsafe raw code file: {rel}")
        data = target.read_bytes()
        if len(data) != row["bytes"] or digest(data) != row["sha256"]:
            raise ValueError(f"Raw code differs from recorded source: {rel}")
    if counts != EXPECTED_COUNTS:
        raise ValueError(f"Unexpected raw code counts: {counts}")
    actual = {path.relative_to(root).as_posix() for path in folder.rglob("*") if path.is_file()}
    expected = {str(rel) for rel in seen} | {str(PREFIX / "README.md"),
                                              str(PREFIX / "code-import-manifest.json")}
    if actual != expected:
        raise ValueError(f"Unrecorded or missing file(s): {sorted(actual ^ expected)}")

    if args.archive:
        archive = args.archive.resolve()
        if digest(archive.read_bytes()) != ARCHIVE_SHA256:
            raise ValueError("Source recovery ZIP SHA-256 differs from the verified handoff")
        with zipfile.ZipFile(archive) as source:
            bad = source.testzip()
            if bad is not None:
                raise ValueError(f"Source recovery ZIP member failed CRC: {bad}")
            for row in rows:
                original = source.read("dh2-reconstruction/" + row["path"])
                if len(original) != row["bytes"] or digest(original) != row["sha256"]:
                    raise ValueError(f"Archive member differs from checked-out raw code: {row['path']}")
    print(json.dumps({"files_verified": len(rows),
                      "bytes_verified": sum(row["bytes"] for row in rows),
                      "archive_verified": bool(args.archive),
                      "source_archive_sha256": ARCHIVE_SHA256}, indent=2))


if __name__ == "__main__":
    main()
