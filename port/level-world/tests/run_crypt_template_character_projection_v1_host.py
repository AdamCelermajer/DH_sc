#!/usr/bin/env python3
"""Compile the Crypt template projection against the packaged source MGPs."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile


HERE = Path(__file__).resolve().parent
MODULE = HERE.parent
PORT = MODULE.parent
REPO = PORT.parent
DEFAULT_ASSETS = REPO / "port/android-native/app/src/main/assets"


def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def digest(data):
    return hashlib.sha256(data).hexdigest()


def verify_source_mgp(assets):
    provenance = json.loads(
        (assets / "worlds/crypt01-provenance.json").read_text(encoding="utf-8"))
    source = next((row for row in provenance["inputs"]
                   if row["name"] == "crypt_straight_ns_01.mgp"), None)
    require(source is not None, "Crypt provenance has no straight-north MGP entry")
    raw = (assets / "worlds/crypt_straight_ns_01.mgp").read_bytes()
    require(hashlib.sha256(raw).hexdigest() == source["sha256"],
            "Packaged source MGP hash differs from Crypt provenance")
    return source["sha256"]


def verify_catalog_assets(assets):
    provenance = json.loads((MODULE / "reference/character-template-catalog-v1.json")
                            .read_text(encoding="utf-8"))
    verified = {}
    for name, expected in provenance["inputs"].items():
        path = assets / "data" / name
        raw = path.read_bytes()
        require(len(raw) == expected["bytes"] and digest(raw) == expected["sha256"],
                f"cache-derived template catalog input differs from provenance: {name}")
        verified[name] = digest(raw)
    return provenance, verified


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--assets-root", type=Path, default=DEFAULT_ASSETS)
    parser.add_argument("--cxx", default=os.environ.get("CXX", "g++"))
    args = parser.parse_args()
    assets = args.assets_root.resolve()
    source_hash = verify_source_mgp(assets)
    provenance, catalog_hashes = verify_catalog_assets(assets)
    compiler = shutil.which(args.cxx) or (args.cxx if Path(args.cxx).is_file() else None)
    require(compiler is not None, f"C++ compiler not found: {args.cxx}")

    with tempfile.TemporaryDirectory(prefix="dh2-crypt-template-projection-") as temporary:
        executable = Path(temporary) / (
            "crypt_template_projection.exe" if os.name == "nt"
            else "crypt_template_projection")
        command = [
            compiler, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
            "-pedantic",
            str(PORT / "world-data/world.cpp"),
            str(PORT / "game-data/data.cpp"),
            str(MODULE / "character_template_factory.cpp"),
            str(MODULE / "character_template_catalog_v1.cpp"),
            str(MODULE / "crypt_template_character_projection_v1.cpp"),
            str(HERE / "crypt_template_character_projection_v1.cpp"),
            "-o", str(executable),
        ]
        built = subprocess.run(command, cwd=REPO, text=True, capture_output=True)
        require(built.returncode == 0,
                f"host compile failed:\n{built.stdout}{built.stderr}")
        tested = subprocess.run([str(executable), str(assets)],
                                cwd=REPO, text=True, capture_output=True)
        require(tested.returncode == 0,
                f"host regression failed:\n{tested.stdout}{tested.stderr}")
        print(tested.stdout.strip())
        print(f"source MGP SHA-256 verified: {source_hash}")
        print("template catalog hashes verified: " + ", ".join(
            f"{name}={catalog_hashes[name]}" for name in provenance["inputs"]
            if name.startswith("character_templates_")))


if __name__ == "__main__":
    try:
        main()
    except (OSError, RuntimeError, subprocess.CalledProcessError, ValueError) as exc:
        raise SystemExit(f"ERROR: {exc}") from exc
