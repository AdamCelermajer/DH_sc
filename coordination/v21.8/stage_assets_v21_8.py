"""Stage original Android files into a new v21.8 package-input folder; run with py -3 -I.

Method (same shape as coordination/v20.1/package_v201.py):
1. Preflight the accepted base package: every manifest entry must exist with the
   recorded size and SHA256. Files not in the base manifest (user save state,
   tutorial progress) are not carried forward.
2. Preflight every selected original file: regular file, no reparse point, not
   already in the base roster, no duplicate selections.
3. Copy the base roster into a NEW folder (an existing folder is never touched),
   copy the selected originals to assets/data/<same relative path>.
4. Rewrite manifest.json over every other file (path, bytes, SHA256 upper-case).
5. Re-read the written folder and check that manifest and disk agree exactly.

Nothing is built, committed or deleted. The base package and its executable are
not modified.
"""
import argparse
import hashlib
import json
import os
import shutil
import stat
from pathlib import Path

FILE_ATTRIBUTE_REPARSE_POINT = 0x400


def sha(path):
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest().upper()


def is_reparse(path):
    attributes = getattr(os.lstat(path), "st_file_attributes", 0)
    return bool(attributes & FILE_ATTRIBUTE_REPARSE_POINT) or stat.S_ISLNK(os.lstat(path).st_mode)


def walk_files(root):
    found = {}
    for dirpath, dirnames, filenames in os.walk(root):
        for name in filenames:
            full = Path(dirpath) / name
            rel = full.relative_to(root).as_posix()
            found[rel.lower()] = (rel, full)
    return found


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--inputs", type=Path, required=True, help="the .local-inputs directory")
    parser.add_argument("--base", default="windows-source-clock-v21.7")
    parser.add_argument("--selection", type=Path, required=True)
    parser.add_argument("--folder", default="windows-source-clock-v21.8-staged")
    args = parser.parse_args()

    selection = json.loads(args.selection.read_text(encoding="utf-8-sig"))
    source_root = args.inputs / selection["source_root"]
    destination_root = selection["destination_root"].strip("/")
    # Optional per-group destination roots (v21.8 mage: the complete PyData set belongs under assets/original-cache/data).
    group_destinations = {name: value.strip("/") for name, value in selection.get("group_destinations", {}).items()}
    selected = []
    seen = set()
    for group, paths in selection["groups"].items():
        for rel in paths:
            key = rel.lower()
            if key in seen:
                raise ValueError("Duplicate selection: " + rel)
            seen.add(key)
            selected.append((group, rel))

    base = args.inputs / args.base
    target = args.inputs / args.folder
    if target.exists():
        raise FileExistsError("Preserving existing folder: " + str(target))
    if not (base / "manifest.json").is_file():
        raise FileNotFoundError("Base manifest missing")

    # 1. Base roster must match disk exactly (case-insensitive path match).
    manifest = json.loads((base / "manifest.json").read_text(encoding="utf-8-sig"))
    base_files = walk_files(base)
    roster = []
    for entry in manifest["files"]:
        key = entry["path"].lower()
        if key not in base_files:
            raise ValueError("Base manifest entry missing on disk: " + entry["path"])
        rel, full = base_files[key]
        if is_reparse(full):
            raise ValueError("Reparse point in base: " + rel)
        if full.stat().st_size != entry["bytes"] or sha(full) != entry["sha256"].upper():
            raise ValueError("Base content changed: " + entry["path"])
        roster.append((rel, full))
    roster_keys = {rel.lower() for rel, _ in roster}

    # 2. Selected originals must be regular, new, and absent from the base roster.
    additions = []
    for group, rel in selected:
        src = source_root / rel
        if not src.is_file():
            raise FileNotFoundError("Original file missing: " + str(src))
        if is_reparse(src):
            raise ValueError("Reparse point in original data: " + str(src))
        dest_rel = group_destinations.get(group, destination_root) + "/" + rel
        if dest_rel.lower() in roster_keys:
            raise ValueError("Already in base package: " + dest_rel)
        additions.append((group, src, dest_rel))

    # 3. Copy into a new folder.
    target.mkdir(parents=True)
    for rel, full in roster:
        out = target / rel
        out.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(full, out)
    for group, src, dest_rel in additions:
        out = target / dest_rel
        out.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(src, out)

    # 4. Rewrite the manifest over every other file.
    files = []
    for path in sorted(target.rglob("*"), key=lambda p: p.as_posix().lower()):
        if path.is_file() and path.name != "manifest.json":
            files.append({"path": path.relative_to(target).as_posix(),
                          "bytes": path.stat().st_size, "sha256": sha(path)})
    version = "v21.8-staged-assets"
    (target / "manifest.json").write_text(
        json.dumps({"version": version, "files": files}, indent=2) + "\n", encoding="utf-8")

    # 5. Verify the written folder against its own manifest.
    written = json.loads((target / "manifest.json").read_text(encoding="utf-8"))
    on_disk = {p.relative_to(target).as_posix() for p in target.rglob("*")
               if p.is_file() and p.name != "manifest.json"}
    listed = {entry["path"] for entry in written["files"]}
    if on_disk != listed:
        raise ValueError("Manifest roster differs from folder contents")
    for entry in written["files"]:
        path = target / entry["path"]
        if path.stat().st_size != entry["bytes"] or sha(path) != entry["sha256"]:
            raise ValueError("Hash mismatch after write: " + entry["path"])

    added_paths = [dest_rel for _, _, dest_rel in additions]
    counts = {}
    for group, _, _ in additions:
        counts[group] = counts.get(group, 0) + 1
    print(json.dumps({"folder": str(target), "manifest_files": len(files),
                      "base_files": len(roster), "added_files": len(added_paths),
                      "added_by_group": counts}, indent=2))


if __name__ == "__main__":
    main()
