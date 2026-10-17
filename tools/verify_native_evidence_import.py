#!/usr/bin/env python3
"""Verify the exact native assembly/symbol text import and optional bundles."""

import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import tarfile


EXPECTED = {
    "assembly": {
        "bundle_sha256": "e7bdc73d7db5c0b8320815f92c7d27c9c86c17710649645c027a4df4081e5601",
        "files": 3625,
        "bytes": 134762768,
    },
    "symbols": {
        "bundle_sha256": "f2dd9d64a9f19a66cfd05e194d30c7482c399ef07d7b488d737ac929652481ca",
        "files": 71,
        "bytes": 166746022,
    },
}
LIBRARIES = {"libDungeonHunter2.so", "libStormGLOFT.so", "libnativeinterface.so"}


def digest_stream(stream):
    result = hashlib.sha256()
    size = 0
    while chunk := stream.read(1024 * 1024):
        result.update(chunk)
        size += len(chunk)
    return size, result.hexdigest()


def check_path(kind, name):
    rel = PurePosixPath(name)
    parts = rel.parts
    if (rel.is_absolute() or ".." in parts or "\\" in name
            or any(not part or ":" in part for part in parts)
            or parts[:3] != ("recovered", "native", kind)):
        raise ValueError(f"Unsafe or unexpected {kind} path: {name}")
    if kind == "assembly":
        valid = len(parts) == 5 and parts[3] in LIBRARIES and parts[4].endswith(".asm")
    else:
        valid = (len(parts) == 4 and parts[3] == "README.md"
                 or len(parts) == 5 and parts[3] in LIBRARIES
                 and parts[4].endswith((".json", ".csv")))
    if not valid:
        raise ValueError(f"Unexpected {kind} member type: {name}")
    return rel


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo-root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--bundle-dir", type=Path,
                        help="Directory containing the exact assembly.tar.gz and symbols.tar.gz")
    args = parser.parse_args()
    root = args.repo_root.resolve()
    manifest = json.loads((root / "recovered/native/evidence-manifest.json").read_text(encoding="utf-8"))
    if manifest.get("schema_version") != 1 or len(manifest.get("bundles", [])) != 2:
        raise ValueError("Unexpected native evidence manifest schema or bundle count")

    results = {}
    for bundle in manifest["bundles"]:
        name = bundle["source_bundle"]
        kind = name.removesuffix(".tar.gz")
        if kind not in EXPECTED or name != kind + ".tar.gz" or kind in results:
            raise ValueError(f"Unexpected or repeated source bundle: {name}")
        spec = EXPECTED[kind]
        if bundle["source_bundle_sha256"] != spec["bundle_sha256"]:
            raise ValueError(f"Pinned {kind} bundle hash differs")
        rows = bundle["files"]
        if len(rows) != spec["files"] or sum(row["bytes"] for row in rows) != spec["bytes"]:
            raise ValueError(f"Unexpected {kind} file count or byte total")
        seen = set()
        for row in rows:
            rel = check_path(kind, row["path"])
            if rel in seen:
                raise ValueError(f"Repeated import path: {rel}")
            seen.add(rel)
            target = root.joinpath(*rel.parts)
            if (target.is_symlink() or not target.is_file()
                    or not target.resolve().is_relative_to(root / "recovered/native" / kind)):
                raise ValueError(f"Missing or unsafe imported file: {rel}")
            with target.open("rb") as stream:
                size, sha256 = digest_stream(stream)
            if size != row["bytes"] or sha256 != row["sha256"]:
                raise ValueError(f"Imported file differs: {rel}")
        actual = {p.relative_to(root).as_posix()
                  for p in (root / "recovered/native" / kind).rglob("*") if p.is_file()}
        if actual != {str(rel) for rel in seen}:
            raise ValueError(f"Unrecorded or missing {kind} files: {sorted(actual ^ {str(rel) for rel in seen})}")

        if args.bundle_dir:
            archive = args.bundle_dir.resolve() / name
            with archive.open("rb") as stream:
                _, sha256 = digest_stream(stream)
            if sha256 != spec["bundle_sha256"]:
                raise ValueError(f"Source bundle SHA-256 differs: {name}")
            expected_rows = {row["path"]: row for row in rows}
            with tarfile.open(archive, "r:gz") as source:
                members = source.getmembers()
                if len(members) != spec["files"] or any(not m.isfile() for m in members):
                    raise ValueError(f"Unexpected {kind} tar member count or type")
                archive_seen = set()
                for member in members:
                    check_path(kind, member.name)
                    if member.name in archive_seen or member.name not in expected_rows:
                        raise ValueError(f"Unrecorded or repeated tar member: {member.name}")
                    archive_seen.add(member.name)
                    stream = source.extractfile(member)
                    if stream is None:
                        raise ValueError(f"Unreadable tar member: {member.name}")
                    with stream:
                        size, sha256 = digest_stream(stream)
                    row = expected_rows[member.name]
                    if size != member.size or size != row["bytes"] or sha256 != row["sha256"]:
                        raise ValueError(f"Tar member differs from checked-out evidence: {member.name}")
                if archive_seen != set(expected_rows):
                    raise ValueError(f"Missing {kind} tar members")
        results[kind] = {"files": len(rows), "bytes": spec["bytes"]}
    if set(results) != set(EXPECTED):
        raise ValueError("Both evidence bundles are required")
    print(json.dumps({"bundles": results, "archives_verified": bool(args.bundle_dir)}, indent=2))


if __name__ == "__main__":
    main()
