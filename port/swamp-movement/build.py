#!/usr/bin/env python3
"""Build the bounded SWAMP movement slice and its source navigation inputs."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parent


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--output', type=Path, default=ROOT/'build')
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    sources = [
        ROOT/'movement.cpp',
        ROOT/'../navigation/navigation.cpp',
        ROOT/'../floor-types/floor_types.cpp',
        ROOT/'../world-data/world.cpp',
        ROOT/'../world-data/world_scene.cpp',
        ROOT/'../scene-payloads/scene.cpp',
        ROOT/'../asset-payloads/payloads.cpp',
        ROOT/'../engine-resources/resources.cpp',
        ROOT/'../engine-math/math.cpp',
    ]
    headers = [
        ROOT/'movement.hpp',
        ROOT/'../navigation/navigation.hpp',
        ROOT/'../floor-types/floor_types.hpp',
        ROOT/'../world-data/world.hpp',
        ROOT/'../world-data/world_scene.hpp',
        ROOT/'../scene-payloads/scene.hpp',
        ROOT/'../asset-payloads/payloads.hpp',
        ROOT/'../engine-resources/resources.hpp',
        ROOT/'../engine-math/math.hpp',
    ]
    flags = ['-std=c++17', '-O2', '-fPIC', '-fno-exceptions', '-fno-rtti',
             '-fno-fast-math', '-ffp-contract=off', '-fno-builtin',
             '-Wall', '-Wextra', '-Werror']
    library = args.output/('libdh2_swamp_movement_host.dll' if os.name == 'nt'
                           else 'libdh2_swamp_movement_host.so')
    link = ['-Wl,--no-insert-timestamp'] if os.name == 'nt' else []
    subprocess.run([os.environ.get('CXX', 'c++'), *flags, '-shared', *link,
                    *(str(source) for source in sources), '-lm', '-o', str(library)],
                   check=True)
    report = {
        'host_build': True,
        'complete_movement': False,
        'scope': 'bounded module-zero endpoint stepping applies constructor baseline path mask 2 and strict native floor-height tolerance; no sweep or full controller parity',
        'navigation_implementation_included': True,
        'source_sha256': {
            str(path.relative_to(ROOT)).replace('\\', '/'): digest(path)
            for path in sources + headers
        },
        'artifact_sha256': {library.name: digest(library)},
    }
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
