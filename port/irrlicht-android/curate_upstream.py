#!/usr/bin/env python3
"""Create a source-only Irrlicht OGL-ES r6038 snapshot from a full export.

The checked-in vendor input keeps the complete Irrlicht engine source/header
set, its Android build sample, Android.mk-selected third-party translation
units and headers/notices, all upstream shaders, and the six smoke assets.
Unrelated demos, platform tools, tests, docs, media, and prebuilt outputs stay
out of the build input.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil


SOURCE_SUFFIXES = {".c", ".cc", ".cpp", ".cxx", ".h", ".hpp", ".inl", ".mm", ".m"}
HEADER_SUFFIXES = {".h", ".hpp", ".inl"}
VENDOR_DIRS = {"aesGladman", "bzip2", "jpeglib", "libpng", "lzma", "zlib"}
SMOKE_MEDIA = {
    "media/irrlichtlogo3.png",
    "media/dwarf.x",
    "media/dwarf.jpg",
    "media/axe.jpg",
    "media/fonthaettenschweiler.bmp",
    "media/bigfont.png",
}
ROOT_DOCS = {"changes.txt", "ogles-readme.txt", "readme.txt"}
LICENSES = {
    "doc/aesGladman.txt",
    "doc/bzip2-license.txt",
    "doc/irrlicht-license.txt",
    "doc/jpglib-license.txt",
    "doc/libpng-license.txt",
    "source/Irrlicht/aesGladman/Readme.txt",
    "source/Irrlicht/bzip2/LICENSE",
    "source/Irrlicht/bzip2/README",
    "source/Irrlicht/jpeglib/README",
    "source/Irrlicht/libpng/LICENSE",
    "source/Irrlicht/libpng/README",
    "source/Irrlicht/zlib/README",
}


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def canonical_hash(records: list[dict]) -> str:
    content = json.dumps(records, sort_keys=True, separators=(",", ":")).encode()
    return hashlib.sha256(content).hexdigest()


def android_make_sources(source: Path) -> set[str]:
    makefile = source / "source/Irrlicht/Android/jni/Android.mk"
    text = makefile.read_text(encoding="utf-8")
    match = re.search(r"(?ms)^LOCAL_SRC_FILES\s*:=\s*\\?\s*\n(.*?)(?=\n\s*\n|\Z)", text)
    if not match:
        raise RuntimeError("could not locate LOCAL_SRC_FILES in Irrlicht Android.mk")
    names = set(re.findall(r"\b[\w./-]+\.(?:c|cc|cpp|cxx)\b", match.group(1)))
    if not names:
        raise RuntimeError("Irrlicht Android.mk contains no source entries")
    return {f"source/Irrlicht/{name}" for name in names}


def selected_paths(source: Path, original_paths: set[str]) -> tuple[set[str], set[str]]:
    compiled = android_make_sources(source)
    missing = compiled - original_paths
    if missing:
        raise RuntimeError(f"Android.mk sources absent from full manifest: {sorted(missing)}")

    keep = {path for path in original_paths if path in ROOT_DOCS or
            path.startswith("include/") or
            path.startswith("examples/01.HelloWorld_Android/") or
            path.startswith("media/Shaders/") or
            path in SMOKE_MEDIA or path in LICENSES}
    for path in original_paths:
        if not path.startswith("source/Irrlicht/"):
            continue
        relative = path.removeprefix("source/Irrlicht/")
        parts = relative.split("/")
        suffix = Path(path).suffix.lower()
        if len(parts) == 1 and suffix in SOURCE_SUFFIXES:
            keep.add(path)
        elif parts[0] == "Android" and suffix in SOURCE_SUFFIXES | {".mk"}:
            keep.add(path)
        elif parts[0] == "KHR" and suffix in SOURCE_SUFFIXES:
            keep.add(path)
        elif parts[0] in VENDOR_DIRS and (path in compiled or suffix in HEADER_SUFFIXES):
            keep.add(path)
    return keep, compiled


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--source", type=Path, required=True,
                        help="unmodified full SVN export")
    parser.add_argument("--source-manifest", type=Path, required=True,
                        help="full export's file/hash manifest")
    parser.add_argument("--destination", type=Path, required=True,
                        help="new curated directory; it must not already exist")
    parser.add_argument("--manifest-out", type=Path, required=True,
                        help="output curated manifest JSON")
    args = parser.parse_args()

    source = args.source.resolve()
    manifest_path = args.source_manifest.resolve()
    destination = args.destination.resolve()
    manifest_out = args.manifest_out.resolve()
    if destination.exists() or manifest_out.exists():
        raise RuntimeError("destination and manifest output must not already exist")
    if destination == source or source in destination.parents:
        raise RuntimeError("curated destination must be outside the full source tree")

    original = json.loads(manifest_path.read_text(encoding="utf-8"))
    original_records = original["files"]
    if canonical_hash(original_records) != original["tree_manifest_sha256"]:
        raise RuntimeError("full upstream manifest canonical hash mismatch")
    original_by_path = {record["path"]: record for record in original_records}
    actual = {path.relative_to(source).as_posix()
              for path in source.rglob("*") if path.is_file()}
    if actual != set(original_by_path):
        raise RuntimeError("full upstream tree differs from its original manifest")
    for relative, record in original_by_path.items():
        path = source.joinpath(*Path(relative).parts)
        if path.stat().st_size != record["bytes"] or sha256(path) != record["sha256"]:
            raise RuntimeError(f"full upstream file failed verification: {relative}")

    keep, compiled = selected_paths(source, set(original_by_path))
    missing_required = (ROOT_DOCS | SMOKE_MEDIA | LICENSES) - keep
    if missing_required:
        raise RuntimeError(f"required source/license/assets missing: {sorted(missing_required)}")
    shader_paths = {p for p in original_by_path if p.startswith("media/Shaders/")}
    if not shader_paths or not shader_paths <= keep:
        raise RuntimeError("the complete upstream shader directory must be retained")

    destination.mkdir(parents=True)
    records = []
    try:
        for relative in sorted(keep):
            src = source.joinpath(*Path(relative).parts)
            dst = destination.joinpath(*Path(relative).parts)
            dst.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(src, dst)
            records.append(original_by_path[relative])
        total_bytes = sum(record["bytes"] for record in records)
        curated = {key: value for key, value in original.items()
                   if key not in {"schema", "file_count", "tree_manifest_sha256", "files"}}
        curated.update({
            "schema": "dh2.irrlicht-svn-curated-snapshot.v1",
            "file_count": len(records),
            "bytes_total": total_bytes,
            "tree_manifest_sha256": canonical_hash(records),
            "original_snapshot": {
                "file_count": original["file_count"],
                "bytes_total": sum(row["bytes"] for row in original_records),
                "tree_manifest_sha256": original["tree_manifest_sha256"],
            },
            "curation": {
                "policy_version": "android-engine-source-only-v1",
                "compiled_android_mk_source_count": len(compiled),
                "retained_scope": [
                    "all public include/ headers",
                    "all direct Irrlicht engine source and headers, including platform and GLES source",
                    "Android platform source/headers/build makefiles",
                    "all translation units named by source/Irrlicht/Android/jni/Android.mk",
                    "all headers and notices for the bundled Android dependencies",
                    "complete media/Shaders tree and six named standalone smoke assets",
                    "the 01.HelloWorld_Android source/build sample",
                    "upstream provenance documents and applicable license notices",
                ],
                "excluded_scope": [
                    "unrelated examples and demo media",
                    "upstream tests, tools, scripts, packaged installers, and prebuilt binaries",
                    "desktop IDE project files and unrelated platform-only build outputs",
                ],
            },
            "files": records,
        })
        manifest_out.parent.mkdir(parents=True, exist_ok=True)
        manifest_out.write_text(json.dumps(curated, indent=2) + "\n", encoding="utf-8")
    except BaseException:
        shutil.rmtree(destination, ignore_errors=True)
        raise

    print(json.dumps({
        "retained_files": len(records),
        "retained_bytes": total_bytes,
        "tree_manifest_sha256": curated["tree_manifest_sha256"],
        "original_files": original["file_count"],
        "original_bytes": curated["original_snapshot"]["bytes_total"],
        "android_mk_sources": len(compiled),
        "shaders": len(shader_paths),
    }, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
