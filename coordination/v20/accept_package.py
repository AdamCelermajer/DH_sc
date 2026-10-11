"""Record an independent verdict for a frozen candidate without changing its EXE."""
import argparse
import hashlib
import json
from pathlib import Path


def sha(path):
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest().upper()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("package", type=Path)
    parser.add_argument("--sha256", required=True)
    parser.add_argument("--verdict", choices=("APPROVE", "APPROVE WITH CAVEATS"), required=True)
    parser.add_argument("--report", required=True)
    args = parser.parse_args()
    receipt_path = args.package / "package-receipt.json"
    notes_path = args.package / "release-notes.json"
    receipt = json.loads(receipt_path.read_text(encoding="utf-8"))
    notes = json.loads(notes_path.read_text(encoding="utf-8"))
    if receipt.get("version") != "v20" or receipt.get("status") != "Frozen candidate; independent verification pending":
        raise ValueError("Only a pending v20 candidate may be accepted")
    actual = sha(args.package / "dh-foundation.exe")
    if actual != args.sha256.upper() or actual != receipt["exe_sha256"]:
        raise ValueError("The independently verified EXE differs from the candidate")
    # Check the entire frozen content before changing metadata.
    manifest = json.loads((args.package / "manifest.json").read_text(encoding="utf-8"))
    expected = {entry["path"] for entry in manifest["files"]}
    present = {p.relative_to(args.package).as_posix() for p in args.package.rglob("*")
               if p.is_file() and p.name != "manifest.json"}
    if expected != present:
        raise ValueError("Frozen package file roster changed")
    for entry in manifest["files"]:
        path = args.package / entry["path"]
        if path.stat().st_size != entry["bytes"] or sha(path) != entry["sha256"]:
            raise ValueError("Frozen package content changed: " + entry["path"])
    for value in (receipt, notes):
        value["status"] = "Accepted v20 for hand testing: " + args.verdict
        value["independent_verdict"] = args.verdict
        value["independent_report"] = args.report
    receipt_path.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    notes_path.write_text(json.dumps(notes, indent=2) + "\n", encoding="utf-8")
    for entry in manifest["files"]:
        path = args.package / entry["path"]
        entry["bytes"] = path.stat().st_size
        entry["sha256"] = sha(path)
    (args.package / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"exe_sha256": actual, "verdict": args.verdict}))


if __name__ == "__main__":
    main()
