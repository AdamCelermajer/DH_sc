#!/usr/bin/env python3
"""Compile and check source-skinned actor meshes and animation samples."""
from __future__ import annotations

import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile


REPO = Path(__file__).resolve().parents[3]
SOURCES = [
    'port/android-app/tests/infected_actor_skin_mesh_host.cpp',
    'port/android-app/scene_buffers.cpp',
    'port/scene-payloads/scene.cpp',
    'port/scene-draw/draw.cpp',
    'port/asset-payloads/payloads.cpp',
    'port/material-bindings/bindings.cpp',
    'port/engine-resources/resources.cpp',
    'port/engine-math/math.cpp',
    'port/animation-pose/pose.cpp',
    'port/animation-values/values.cpp',
    'port/animation-timeline/timeline.cpp',
    'port/animation-mixing/mixing.cpp',
    'port/animation-layers/layers.cpp',
    'port/skin-payloads/skin.cpp',
]


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', type=Path, default=REPO.parent / 'cache' / 'files')
    parser.add_argument('--compiler', default=shutil.which('g++') or 'g++')
    args = parser.parse_args()
    cache = args.cache.resolve()
    if not cache.is_dir():
        raise FileNotFoundError(f'Unpacked source cache not found: {cache}')
    with tempfile.TemporaryDirectory(prefix='dh2-actor-skin-host-') as temporary:
        executable = Path(temporary) / 'infected_actor_skin_mesh_host.exe'
        command = [args.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
                   '-ffunction-sections', '-fdata-sections',
                   *(str(REPO / source) for source in SOURCES),
                   '-Wl,--gc-sections', '-o', str(executable)]
        subprocess.run(command, check=True, cwd=REPO)
        subprocess.run([str(executable), str(cache)], check=True, cwd=REPO)


if __name__ == '__main__':
    main()
