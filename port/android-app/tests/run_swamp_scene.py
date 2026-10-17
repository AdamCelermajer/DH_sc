#!/usr/bin/env python3
"""Build and run the original SWAMP module-zero geometry regression."""
from __future__ import annotations

import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile


TESTS = Path(__file__).resolve().parent
REPO = TESTS.parents[2]

SOURCES = [
    TESTS / 'swamp_scene.cpp',
    REPO / 'port/world-data/world.cpp',
    REPO / 'port/world-data/world_scene.cpp',
    REPO / 'port/android-app/scene_buffers.cpp',
    REPO / 'port/skin-payloads/skin.cpp',
    REPO / 'port/animation-pose/pose.cpp',
    REPO / 'port/animation-values/values.cpp',
    REPO / 'port/animation-timeline/timeline.cpp',
    REPO / 'port/animation-mixing/mixing.cpp',
    REPO / 'port/animation-layers/layers.cpp',
    REPO / 'port/scene-draw/draw.cpp',
    REPO / 'port/scene-payloads/scene.cpp',
    REPO / 'port/asset-payloads/payloads.cpp',
    REPO / 'port/engine-resources/resources.cpp',
    REPO / 'port/engine-math/math.cpp',
    REPO / 'port/material-bindings/bindings.cpp',
    REPO / 'port/scene-materials/technique_selector.cpp',
]


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', required=True, type=Path,
                        help='Extracted original cache root containing data/scene/001_swamp.mlx')
    parser.add_argument('--cxx', default='g++', help='Host C++ compiler (default: g++)')
    parser.add_argument('--dump-draws', action='store_true',
                        help='Print every source draw identity, local effect reference and bounds')
    args = parser.parse_args()
    cache = args.cache.resolve()
    if not (cache / 'data/scene/001_swamp.mlx').is_file():
        parser.error(f'cache root does not contain data/scene/001_swamp.mlx: {cache}')
    compiler = shutil.which(args.cxx)
    if not compiler:
        parser.error(f'host C++ compiler not found: {args.cxx}')
    with tempfile.TemporaryDirectory(prefix='dh2-swamp-scene-') as directory:
        executable = Path(directory) / ('swamp_scene.exe' if __import__('os').name == 'nt'
                                        else 'swamp_scene')
        command = [compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
                   '-fno-exceptions', '-fno-rtti', *map(str, SOURCES), '-o', str(executable)]
        # The fixture itself reports failures without throwing across C ABI boundaries;
        # production translation units follow the same no-exception/no-RTTI build policy.
        subprocess.run(command, check=True)
        command = [str(executable), str(cache)]
        if args.dump_draws:
            command.append('--dump-draws')
        subprocess.run(command, check=True)


if __name__ == '__main__':
    main()
