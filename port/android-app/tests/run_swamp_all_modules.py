#!/usr/bin/env python3
"""Compile and run source assembly against every original SWAMP module."""
from __future__ import annotations

import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile
import os


TESTS = Path(__file__).resolve().parent
REPO = TESTS.parents[2]
SOURCES = [
    TESTS / 'swamp_all_modules.cpp',
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
]


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', required=True, type=Path,
                        help='Extracted cache root containing data/scene/001_swamp.mlx')
    parser.add_argument('--cxx', default='g++', help='Host C++ compiler')
    args = parser.parse_args()
    cache = args.cache.resolve()
    if not (cache / 'data/scene/001_swamp.mlx').is_file():
        parser.error(f'cache root lacks data/scene/001_swamp.mlx: {cache}')
    compiler = shutil.which(args.cxx)
    if not compiler:
        parser.error(f'host C++ compiler not found: {args.cxx}')
    with tempfile.TemporaryDirectory(prefix='dh2-swamp-all-modules-') as directory:
        executable = Path(directory) / ('swamp_all_modules.exe' if os.name == 'nt'
                                        else 'swamp_all_modules')
        subprocess.run([compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
                        '-fno-exceptions', '-fno-rtti', *map(str, SOURCES),
                        '-o', str(executable)], check=True)
        subprocess.run([str(executable), str(cache)], check=True)


if __name__ == '__main__':
    main()
