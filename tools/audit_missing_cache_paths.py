#!/usr/bin/env python3
"""Classify Android ``Could not open file`` messages against a local game cache.

This is a read-only diagnostic. It never copies or renames copyrighted assets.
"""

from __future__ import annotations

import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path, PurePosixPath
import re


MISSING = re.compile(r"Could not open file\s*:\s*(\S+)")
FILES_MARKER = "/files/"


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        while block := source.read(1024 * 1024):
            digest.update(block)
    return digest.hexdigest()


def cache_relative(request: str) -> str | None:
    normalized = request.replace("\\", "/")
    if FILES_MARKER not in normalized:
        return None
    relative = normalized.split(FILES_MARKER, 1)[1]
    path = PurePosixPath(relative)
    if not relative or path.is_absolute() or ".." in path.parts:
        return None
    return path.as_posix()


def audit(cache: Path, log: Path) -> dict:
    if not cache.is_dir():
        raise ValueError(f"cache files root is not a directory: {cache}")
    files = {
        path.relative_to(cache).as_posix().casefold(): path
        for path in cache.rglob("*") if path.is_file()
    }
    requests: Counter[str] = Counter()
    for line in log.read_text(encoding="utf-8", errors="replace").splitlines():
        match = MISSING.search(line)
        if match:
            requests[match.group(1)] += 1

    findings = []
    classes: Counter[str] = Counter()
    for requested, count in sorted(requests.items()):
        relative = cache_relative(requested)
        candidates = []
        if relative is None:
            category = "outside_cache_or_unsafe_path"
        elif relative.casefold() in files:
            category = "exact_cache_file_exists"
            candidates = [files[relative.casefold()]]
        elif relative.casefold().startswith("qata/"):
            corrected = "data/" + relative[5:]
            match = files.get(corrected.casefold())
            if match:
                category = "qata_to_data_exact_suffix"
                candidates = [match]
            else:
                category = "unresolved"
        else:
            category = "unresolved"
        classes[category] += count
        findings.append({
            "requested": requested,
            "occurrences": count,
            "cache_relative": relative,
            "classification": category,
            "candidates": [
                {
                    "cache_relative": path.relative_to(cache).as_posix(),
                    "bytes": path.stat().st_size,
                    "sha256": sha256(path),
                }
                for path in candidates
            ],
        })
    return {
        "scope": "read-only classification of logged opens; candidate existence does not prove that loading succeeds",
        "cache_file_count": len(files),
        "unique_logged_paths": len(requests),
        "logged_occurrences": sum(requests.values()),
        "classification_occurrences": dict(sorted(classes.items())),
        "findings": findings,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", required=True, type=Path, help="extracted files/ directory")
    parser.add_argument("--log", required=True, type=Path, help="Android logcat text")
    parser.add_argument("--report", type=Path, help="optional JSON output")
    args = parser.parse_args()
    content = json.dumps(audit(args.cache, args.log), indent=2) + "\n"
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(content, encoding="utf-8")
    print(content, end="")


if __name__ == "__main__":
    main()
