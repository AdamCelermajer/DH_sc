#!/usr/bin/env python3
"""Build host readers used by the bounded static-level runtime coordinator."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parent
REPO = ROOT.parents[1]


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=ROOT / "build")
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    sources = [
        REPO / "port/world-data/world.cpp",
        REPO / "port/world-data/world_scene.cpp",
        REPO / "port/scene-payloads/scene.cpp",
        REPO / "port/engine-resources/resources.cpp",
        REPO / "port/engine-math/math.cpp",
    ]
    flags = ["-std=c++17", "-O2", "-fPIC", "-fno-exceptions", "-fno-rtti",
             "-fno-fast-math", "-ffp-contract=off", "-fno-builtin",
             "-Wall", "-Wextra", "-Werror"]
    output = args.output / ("libdh2_level_runtime_host.dll" if os.name == "nt"
                            else "libdh2_level_runtime_host.so")
    link_flags = ["-Wl,--no-insert-timestamp"] if os.name == "nt" else []
    subprocess.run([os.environ.get("CXX", "c++"), *flags, "-shared", *link_flags,
                    *(str(source) for source in sources), "-lm", "-o", str(output)],
                   check=True)
    inputs = sources + [
        REPO / "port/world-data/world.hpp",
        REPO / "port/world-data/world_scene.hpp",
        REPO / "port/scene-payloads/scene.hpp",
        REPO / "port/engine-resources/resources.hpp",
        REPO / "port/engine-math/math.hpp",
        REPO / "port/level-catalogue/catalogue.py",
        ROOT / "runtime.py",
    ]
    report = {
        "host_build": True,
        "android_build": False,
        "complete_engine": False,
        "objects_activated": False,
        "source_sha256": {
            str(path.relative_to(REPO)).replace("\\", "/"): digest(path)
            for path in inputs
        },
        "artifact_sha256": {output.name: digest(output)},
    }
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report))


if __name__ == "__main__":
    main()
