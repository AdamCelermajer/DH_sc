#!/usr/bin/env python3
"""Build the checked source-derived SWAMP floor-mesh host library."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parent


def sha256(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--output', type=Path, default=ROOT/'build')
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    sources = [ROOT/'navigation.cpp', ROOT/'../world-data/world.cpp',
               ROOT/'../world-data/world_scene.cpp', ROOT/'../scene-payloads/scene.cpp',
               ROOT/'../asset-payloads/payloads.cpp',
               ROOT/'../floor-types/floor_types.cpp',
               ROOT/'../engine-resources/resources.cpp', ROOT/'../engine-math/math.cpp']
    headers = [ROOT/'navigation.hpp', ROOT/'../world-data/world.hpp',
               ROOT/'../world-data/world_scene.hpp', ROOT/'../scene-payloads/scene.hpp',
               ROOT/'../floor-types/floor_types.hpp',
               ROOT/'../asset-payloads/payloads.hpp',
               ROOT/'../engine-resources/resources.hpp', ROOT/'../engine-math/math.hpp']
    flags = ['-std=c++17', '-O2', '-fPIC', '-fno-exceptions', '-fno-rtti',
             '-fno-fast-math', '-ffp-contract=off', '-fno-builtin',
             '-Wall', '-Wextra', '-Werror']
    library = args.output/('libdh2_navigation_host.dll' if os.name == 'nt'
                           else 'libdh2_navigation_host.so')
    host_link = ['-Wl,--no-insert-timestamp'] if os.name == 'nt' else []
    subprocess.run([os.environ.get('CXX', 'c++'), *flags, '-shared', *host_link,
                    *(str(source) for source in sources), '-lm', '-o', str(library)],
                   check=True)
    report = {
        'host_build': True,
        'complete_navigation': False,
        'scope': 'source-selected SWAMP floor meshes; geometric height, source-backed floor eligibility, and bounded zero-radius segment-vs-triangle queries; no actor movement integration',
        'source_sha256': {str(path.relative_to(ROOT)).replace('\\', '/'): sha256(path)
                          for path in sources + headers},
        'artifact_sha256': {library.name: sha256(library)},
    }
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
