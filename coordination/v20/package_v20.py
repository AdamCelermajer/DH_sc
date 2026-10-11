"""Package a frozen v20 candidate; run only with py -3 -I.

Metadata is supplied from actual integration results, never inferred by this tool.
Existing packages and saves are preserved. The manifest covers all other files.
"""
import argparse
import hashlib
import json
import shutil
from pathlib import Path


def sha(path):
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest().upper()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--inputs", type=Path, required=True)
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--metadata", type=Path, required=True)
    parser.add_argument("--folder", default="windows-source-clock-v20")
    args = parser.parse_args()
    metadata = json.loads(args.metadata.read_text(encoding="utf-8-sig"))
    for key in ("source_commit", "ctest", "fixes", "open", "verification", "readme"):
        if key not in metadata:
            raise ValueError("Missing evidence field: " + key)
    if Path(args.folder).name != args.folder:
        raise ValueError("Folder must be a single directory name")
    source = args.inputs / "windows-source-clock-v19-preview-16"
    target = args.inputs / args.folder
    if target.exists():
        raise FileExistsError("Preserving existing package: " + str(target))
    # Prepare inputs before copying: a failed preflight must not leave a candidate.
    added = []
    for rel in ("animateddecors/swamp_tikitorches.bdae",
                "animateddecors/swamp_watercaustics.bdae",
                "animateddecors/swamp_caveentrance_effect.bdae",
                "animateddecors/swamp_cave_entrance_troll.bdae",
                "animateddecors/swamp_rayolight_dust.bdae",
                "props/swamp/prop_swamp_corpses.bdae"):
        added.append((args.inputs / "assets-extra/android/data/3d" / rel,
                      "assets/data/3d/" + rel))
    for item in metadata.get("additional_files", []):
        relative = Path(item["destination"])
        if relative.is_absolute() or ".." in relative.parts:
            raise ValueError("Unsafe asset destination")
        added.append((Path(item["source"]), relative.as_posix()))
    for path in [args.exe, source / "README.txt"] + [p for p, _ in added]:
        if not path.is_file():
            raise FileNotFoundError(path)
    unique = {}
    for path, relative in added:
        key = relative.lower()
        if key in unique and sha(path) != sha(unique[key][0]):
            raise ValueError("Conflicting asset sources: " + relative)
        unique[key] = (path, relative)
    added = list(unique.values())
    if any(p.name.lower() in {"gameplay.save", "character.save", "dh2_settings.tutorials"}
           or p.suffix.lower() == ".save" for p in source.rglob("*") if p.is_file()):
        raise ValueError("Base package contains user save data; do not copy")
    shutil.copytree(source, target)
    shutil.copy2(args.exe, target / "dh-foundation.exe")
    for path, relative in added:
        destination = target / relative
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(path, destination)
    for filename, flags in metadata.get("remove_arg_pairs", {}).items():
        if filename not in ("startup.args", "swamp.args"):
            raise ValueError("Only the shipped startup argument files may be edited")
        path = target / filename
        lines = path.read_text(encoding="utf-8-sig").splitlines()
        for flag in flags:
            while flag in lines:
                index = lines.index(flag)
                if index + 1 == len(lines) or lines[index + 1].startswith("--"):
                    raise ValueError("Missing argument value for " + flag)
                del lines[index:index + 2]
        path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    executable_sha = sha(target / "dh-foundation.exe")
    receipt = {"version": "v20", "source_commit": metadata["source_commit"],
               "exe_sha256": executable_sha,
               "exe_bytes": (target / "dh-foundation.exe").stat().st_size,
               "base_package": source.name, "ctest": metadata["ctest"],
               "verification": metadata["verification"],
               "status": "Frozen candidate; independent verification pending"}
    notes = {"version": "v20", "platform": "Windows x64",
             "exe_sha256": executable_sha, "source_commit": metadata["source_commit"],
             "fixes": metadata["fixes"], "open": metadata["open"],
             "added_files": [rel for _, rel in added],
             "status": receipt["status"]}
    (target / "README.txt").write_text(metadata["readme"], encoding="utf-8")
    for name, value in (("release-notes.json", notes), ("package-receipt.json", receipt)):
        (target / name).write_text(json.dumps(value, indent=2) + "\n", encoding="utf-8")
    files = [{"path": p.relative_to(target).as_posix(), "bytes": p.stat().st_size,
              "sha256": sha(p)} for p in sorted(target.rglob("*"))
             if p.is_file() and p.name != "manifest.json"]
    (target / "manifest.json").write_text(
        json.dumps({"version": "v20", "files": files}, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"folder": str(target), "exe_sha256": executable_sha,
                      "files": len(files)}))


if __name__ == "__main__":
    main()
