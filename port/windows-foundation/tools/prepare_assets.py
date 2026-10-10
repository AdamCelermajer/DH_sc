#!/usr/bin/env python3
"""Copy explicitly selected staged/cache content; never delete or copy a default map.

Manifest format: {"staged": ["relative/file", "relative/directory"],
                  "cache": ["data/3d/textures"]}
Cache may be a directory or ZIP. ZIP paths containing /files/ are exposed relative
to that prefix. CRC validation is provided by ZipFile while reading selected files.
The emitted preparation-manifest.json can also be used as package.ps1 AssetList.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import stat
import sys
import zipfile
from pathlib import Path, PurePosixPath


REPORT_NAME = "preparation-manifest.json"


def relative_name(value: object) -> str:
    if not isinstance(value, str) or not value.strip():
        raise ValueError("Selections must be nonempty relative path strings")
    text = value.replace("\\", "/").rstrip("/")
    path = PurePosixPath(text)
    if (path.is_absolute() or not path.parts or text != path.as_posix() or
            any(part in ("..", ".") for part in text.split("/")) or
            any(character in text for character in ':*?<>|\"\x00') or
            any(part.endswith((".", " ")) for part in path.parts)):
        raise ValueError(f"Unsafe relative path: {value!r}")
    reserved = {"CON", "PRN", "AUX", "NUL"} | {
        f"{prefix}{number}" for prefix in ("COM", "LPT") for number in range(1, 10)
    }
    if any(part.split(".")[0].upper() in reserved for part in path.parts):
        raise ValueError(f"Reserved Windows path: {value!r}")
    return path.as_posix()


def assert_regular(path: Path, root: Path) -> None:
    cursor = path
    while True:
        if cursor.is_symlink() or (hasattr(cursor, "is_junction") and cursor.is_junction()):
            raise ValueError(f"Links are unsupported in selected content: {cursor}")
        if cursor == root:
            break
        cursor = cursor.parent
    if not path.is_file():
        raise ValueError(f"Selected content is not a regular file: {path}")


def directory_selection(root: Path, selections: list[str]) -> list[tuple[str, Path]]:
    result = []
    for selection in selections:
        selected = root / selection
        resolved = selected.resolve()
        if not resolved.is_relative_to(root):
            raise ValueError(f"Selected content escapes its root: {selection}")
        if not selected.exists():
            raise ValueError(f"Selected content is missing: {selected}")
        candidates = sorted(selected.rglob("*")) if selected.is_dir() else [selected]
        for candidate in candidates:
            if candidate.is_symlink() or (hasattr(candidate, "is_junction") and candidate.is_junction()):
                raise ValueError(f"Links are unsupported in selected content: {candidate}")
            if candidate.is_file():
                assert_regular(candidate, root)
                result.append((relative_name(candidate.relative_to(root).as_posix()), candidate))
    return result


def archive_selection(archive: zipfile.ZipFile, selections: list[str]):
    available = {}
    for info in archive.infolist():
        if info.is_dir():
            continue
        raw = info.filename.replace("\\", "/")
        if "/files/" in raw:
            raw = raw.split("/files/", 1)[1]
        elif raw.startswith("files/"):
            raw = raw[len("files/"):]
        # Unselected wrapper entries need not be extracted or interpreted.
        available.setdefault(raw.casefold(), []).append((raw, info))
    result = []
    for selection in selections:
        key = selection.casefold()
        matches = [(name, info) for logical, entries in available.items()
                   if logical == key or logical.startswith(key + "/")
                   for name, info in entries]
        if not matches:
            raise ValueError(f"Selected content is missing in cache archive: {selection}")
        for name, info in matches:
            name = relative_name(name)
            if len(available[name.casefold()]) > 1:
                raise ValueError(f"Ambiguous cache archive path: {name}")
            if stat.S_ISLNK(info.external_attr >> 16):
                raise ValueError(f"Archive symlink is unsupported: {info.filename}")
            result.append((name, info))
    return result


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, required=True)
    parser.add_argument("--staged", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--manifest", type=Path,
                        help="Explicit selection JSON; required to select any content")
    args = parser.parse_args()
    if args.manifest is None:
        parser.error("--manifest is required; there are no implicit asset selections")
    selection = json.loads(args.manifest.read_text(encoding="utf-8-sig"))
    if not isinstance(selection, dict) or set(selection) - {"staged", "cache"}:
        raise ValueError("Manifest must contain only staged and/or cache arrays")
    normalized = {}
    for kind in ("staged", "cache"):
        values = selection.get(kind, [])
        if not isinstance(values, list):
            raise ValueError(f"Manifest {kind} must be an array")
        normalized[kind] = [relative_name(value) for value in values]
    if not any(normalized.values()):
        raise ValueError("Manifest selects no content")
    output = args.output.resolve()
    staged = args.staged.resolve()
    cache = args.cache.resolve()
    for root in (staged, cache):
        if root.is_dir() and (output == root or output.is_relative_to(root)):
            raise ValueError("Output must be outside input directories")
    if output.exists() and not output.is_dir():
        raise ValueError(f"Output is not a directory: {output}")
    archive = None
    try:
        selected = []
        if normalized["staged"]:
            if not staged.is_dir():
                raise ValueError(f"Staged root is missing: {staged}")
            selected.extend((name, source, "staged") for name, source in
                            directory_selection(staged, normalized["staged"]))
        if normalized["cache"]:
            if cache.is_dir():
                entries = directory_selection(cache, normalized["cache"])
            else:
                archive = zipfile.ZipFile(cache)
                entries = archive_selection(archive, normalized["cache"])
            selected.extend((name, source, "cache") for name, source in entries)
        unique = {}
        for name, source, kind in selected:
            key = name.casefold()
            if key == REPORT_NAME.casefold():
                raise ValueError(f"Selected asset conflicts with report name: {name}")
            if key in unique:
                if unique[key] == (name, source, kind):
                    continue
                raise ValueError(f"Selections collide at destination: {name}")
            destination = output / name
            if not destination.resolve().is_relative_to(output):
                raise ValueError(f"Destination escapes output: {name}")
            if destination.exists():
                raise ValueError(f"Refusing to overwrite existing content: {destination}")
            unique[key] = (name, source, kind)
        report = output / REPORT_NAME
        if report.exists():
            raise ValueError(f"Refusing to overwrite existing report: {report}")
        if not unique:
            raise ValueError("Selection contains no files")
        records = []
        for name, source, kind in unique.values():
            destination = output / name
            destination.parent.mkdir(parents=True, exist_ok=True)
            digest = hashlib.sha256()
            if isinstance(source, zipfile.ZipInfo):
                stream = archive.open(source)
            else:
                stream = source.open("rb")
            with stream, destination.open("xb") as target:
                while chunk := stream.read(1024 * 1024):
                    target.write(chunk)
                    digest.update(chunk)
            records.append({"path": name, "source": kind, "bytes": destination.stat().st_size,
                            "sha256": digest.hexdigest()})
        payload = {"schemaVersion": 1, "files": [record["path"] for record in records],
                   "sources": records}
        with report.open("x", encoding="utf-8") as target:
            json.dump(payload, target, indent=2)
            target.write("\n")
        print(json.dumps({"output": str(output), "files": len(records),
                          "manifest": str(report)}))
    finally:
        if archive is not None:
            archive.close()


if __name__ == "__main__":
    try:
        main()
    except (OSError, ValueError, zipfile.BadZipFile, RuntimeError) as error:
        print(f"Asset preparation failed: {error}", file=sys.stderr)
        sys.exit(1)
