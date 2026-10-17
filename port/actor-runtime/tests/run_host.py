#!/usr/bin/env python3
"""Build and exercise the owned actor registry against the supplied cache."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile


HERE = Path(__file__).resolve().parent
PORT = HERE.parents[1]
REPO = HERE.parents[2]


def compiler() -> str:
    requested = os.environ.get("CXX", "c++")
    found = shutil.which(requested)
    if found:
        return found
    if Path(requested).is_file():
        return str(Path(requested).resolve())
    raise SystemExit(f"C++ compiler not found: {requested}; set CXX to override")


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--cache", type=Path,
        default=REPO.parent / "cache" / "files",
        help="supplied cache/files directory; cache material is never copied",
    )
    parser.add_argument(
        "--report", type=Path, default=PORT / "actor-runtime" / "validation.json",
        help="write evidence JSON (default: port/actor-runtime/validation.json)",
    )
    args = parser.parse_args()
    cache = args.cache.resolve()
    source_paths = [
        cache / "data/scene/005_infectedvillage.mlx",
        cache / "data/3d/modules/infectedvillage/mgp/infected01.mgp",
        cache / "data/3d/modules/infectedvillage/mgp/infected02.mgp",
    ]
    for path in source_paths:
        if not path.is_file():
            raise SystemExit(f"missing source cache file: {path}")

    sources = [
        HERE / "actor_registry_test.cpp",
        PORT / "actor-runtime" / "actor_registry.cpp",
        PORT / "world-data" / "world.cpp",
        PORT / "world-data" / "world_scene.cpp",
        PORT / "scene-payloads" / "scene.cpp",
        PORT / "engine-resources" / "resources.cpp",
        PORT / "engine-math" / "math.cpp",
    ]
    flags = [
        "-std=c++17", "-O2", "-fno-exceptions", "-fno-rtti",
        "-fno-fast-math", "-ffp-contract=off", "-fno-builtin",
        "-Wall", "-Wextra", "-Werror",
    ]
    with tempfile.TemporaryDirectory(prefix="dh2-actor-runtime-") as temp:
        executable = Path(temp) / ("actor_registry_test.exe" if os.name == "nt"
                                   else "actor_registry_test")
        command = [compiler(), *flags, *(str(path) for path in sources),
                   "-lm", "-o", str(executable)]
        print("+", subprocess.list2cmdline(command), flush=True)
        subprocess.run(command, check=True)
        print("+", subprocess.list2cmdline([str(executable), str(cache)]), flush=True)
        completed = subprocess.run([str(executable), str(cache)], check=True,
                                   capture_output=True, text=True)

    try:
        evidence = json.loads(completed.stdout)
    except json.JSONDecodeError as exc:
        raise SystemExit(f"test executable returned invalid evidence JSON: {exc}") from exc
    if evidence.get("actor_count") != 26:
        raise SystemExit("expected 26 imported Infected Village Characters")
    if evidence.get("translation_checks") != evidence.get("actor_count"):
        raise SystemExit("not every imported actor passed world translation checks")
    if evidence.get("missing_actor", {}).get("result") != "lookup_miss":
        raise SystemExit("unresolved infected17 lookup was not reported as a miss")
    if evidence.get("missing_actor", {}).get("lookup_miss_count") != 1:
        raise SystemExit("expected exactly one reported Ambush actor miss")
    checks = evidence.get("checks", {})
    if not checks or not all(checks.values()):
        raise SystemExit("one or more actor registry checks did not pass")

    evidence["input_files"] = {
        path.relative_to(cache).as_posix(): {
            "bytes": path.stat().st_size,
            "sha256": sha256(path),
        }
        for path in source_paths
    }
    evidence["scope"] = (
        "Port-owned record registry and idempotent spawn-request projection; "
        "not the native object ABI, renderer, AI, or a complete spawn state machine."
    )
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(evidence, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(evidence, indent=2))
    print(f"wrote {args.report}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
