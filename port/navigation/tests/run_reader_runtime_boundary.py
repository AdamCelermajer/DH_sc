#!/usr/bin/env python3
"""Link imported records and live scene/world/PF navigation in one host image."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess


ROOT = Path(__file__).resolve().parents[3]
SOURCES = [
    'port/navigation/tests/reader_runtime_boundary.cpp',
    'port/navigation/navigation.cpp',
    'port/world-data/world.cpp',
    'port/world-data/world_scene.cpp',
    'port/scene-payloads/scene.cpp',
    'port/scene-materials/scene.cpp',
    'port/level-world/world.cpp',
    'port/level-world/floors.cpp',
    'port/level-world/floor_source.cpp',
    'port/level-world/octree.cpp',
    'port/level-world/selector.cpp',
    'port/level-world/collision.cpp',
    'port/level-world/navigation.cpp',
    'port/level-world/navigation_search.cpp',
    'port/level-world/navigation_world.cpp',
    'port/level-world/navigation_motion.cpp',
    'port/level-world/navigation_path.cpp',
    'port/asset-payloads/payloads.cpp',
    'port/floor-types/floor_types.cpp',
    'port/engine-resources/resources.cpp',
    'port/engine-math/math.cpp',
]
HEADERS = [
    'port/navigation/navigation.hpp',
    'port/world-data/world.hpp',
    'port/world-data/world_scene.hpp',
    'port/scene-payloads/scene.hpp',
    'port/scene-materials/scene.hpp',
    'port/level-world/world.hpp',
    'port/level-world/navigation.hpp',
]


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', type=Path, required=True)
    parser.add_argument('--cxx', default=os.environ.get('CXX', 'c++'))
    parser.add_argument('--output', type=Path,
                        default=ROOT / 'port/navigation/build/reader-runtime-boundary')
    args = parser.parse_args()
    cache = args.cache.resolve()
    args.output.mkdir(parents=True, exist_ok=True)
    executable = args.output / ('reader-runtime-boundary.exe' if os.name == 'nt'
                                else 'reader-runtime-boundary')
    flags = ['-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
             '-fno-fast-math', '-ffp-contract=off']
    if os.name == 'nt':
        flags += ['-Wl,--no-insert-timestamp']
    subprocess.run([args.cxx, *flags, *(str(ROOT / name) for name in SOURCES),
                    '-lm', '-o', str(executable)], check=True)
    result = subprocess.run([str(executable), str(cache)], check=True,
                            capture_output=True, text=True)
    report = json.loads(result.stdout)
    report['scope'] = ('Importer/live type and C symbol coexistence; actual SWAMP '
                       'MLX/catalogue readers and one live PF graph triangle; '
                       'no Android execution or full gameplay validation')
    report['source_sha256'] = {name: digest(ROOT / name)
                               for name in SOURCES + HEADERS}
    report['cache_sha256'] = {name: digest(cache / name) for name in (
        'data/scene/001_swamp.mlx', 'data/3d/modules/swamp/swamp.bdae')}
    report['executable_sha256'] = digest(executable)
    path = args.output / 'validation.json'
    path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'PASS: reader/live runtime single-image link boundary; report={path}')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
