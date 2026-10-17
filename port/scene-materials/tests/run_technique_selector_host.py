#!/usr/bin/env python3
"""Compile and run the bounded SWAMP CurrentTechnique/effect-list host gate."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys


TESTS = Path(__file__).resolve().parent
SCENE_MATERIALS = TESTS.parent
REPO = TESTS.parents[2]
GAME = REPO / 'port/irrlicht-android/game'
WORLD_SOURCES = [
    TESTS / 'technique_selector_host.cpp',
    SCENE_MATERIALS / 'technique_selector.cpp',
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


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(command: list[str | Path]) -> str:
    result = subprocess.run([str(value) for value in command], cwd=REPO,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            text=True, check=False)
    if result.returncode:
        raise RuntimeError('command failed: ' + ' '.join(map(str, command)) +
                           '\n' + result.stdout[-20000:])
    return result.stdout


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', required=True, type=Path,
                        help='private extracted cache containing SWAMP/effect BRES and MLX')
    parser.add_argument('--cxx', default='g++', help='host C++17 compiler')
    parser.add_argument('--output', type=Path,
                        default=REPO / 'port/irrlicht-android/build/swamp-technique-selector-host')
    args = parser.parse_args()
    cache = args.cache.resolve(strict=True)
    inputs = [
        'data/scene/001_swamp.mlx',
        'data/3d/modules/swamp/swamp.bdae',
        'data/gfx/effects/gl_diffuse_l1_vc_iphone.bdae',
    ]
    for relative in inputs:
        if not (cache / relative).is_file():
            parser.error('cache is missing ' + relative)
    compiler = shutil.which(args.cxx)
    if not compiler:
        parser.error('host C++ compiler not found: ' + args.cxx)

    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    executable = output / ('technique-selector-host.exe' if sys.platform == 'win32'
                           else 'technique-selector-host')
    compile_command = [compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
        '-Werror', '-fno-exceptions', '-fno-rtti', '-fno-fast-math',
        '-ffp-contract=off', *WORLD_SOURCES, '-o', executable]
    run(compile_command)
    output_text = run([executable, cache])
    line = next((row for row in output_text.splitlines()
                 if row.startswith('{') and row.endswith('}')), None)
    if not line:
        raise RuntimeError('selector host did not return structured output:\n' + output_text)
    evidence = json.loads(line)
    if evidence.get('validation') != 'PASS' or len(evidence.get('modules', [])) != 9:
        raise RuntimeError('selector host did not validate the nine SWAMP modules')
    alpha = evidence.get('alpha_selector_mappings', [])
    opaque = evidence.get('opaque_selector_mappings', [])
    if len(alpha) != 2 or len(opaque) != 2:
        raise RuntimeError('expected both GLES and GLES2 selector mappings')
    if any('_Al_' not in item['selector'] or '_At_' in item['selector']
           for item in alpha):
        raise RuntimeError('Material__11611 no longer resolves AL-only selectors')
    if any('_Al_' in item['selector'] or '_At_' in item['selector']
           for item in opaque):
        raise RuntimeError('Material__11610 unexpectedly resolves an alpha variant')
    if evidence.get('draw_totals', {}).get('Material__11610') < 25 or \
            evidence.get('draw_totals', {}).get('Material__11611') < 22:
        raise RuntimeError('all-nine source-module walk missed module-zero material draws')

    report = {
        'scope': ('bounded BRES CurrentTechnique selector decoding and exact external '
                  'effect-name resolution across all nine selected SWAMP module roots; '
                  'no shader execution, blend/depth-state recovery, APK, or device run'),
        'validation': 'PASS',
        'evidence': evidence,
        'cache_inputs_sha256': {relative: sha256(cache / relative) for relative in inputs},
        'source_sha256': {path.relative_to(REPO).as_posix(): sha256(path)
                          for path in WORLD_SOURCES + [SCENE_MATERIALS / 'technique_selector.hpp',
                              SCENE_MATERIALS / 'technique_selector.cpp',
                              TESTS / 'run_technique_selector_host.py']},
        'compiler': compiler,
        'compile_command': [str(value) for value in compile_command],
        'host_executable': str(executable),
        'host_executable_sha256': sha256(executable),
    }
    report_path = output / 'validation.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print('PASS: CurrentTechnique selectors resolve exactly against both Multilight effect groups across all nine SWAMP module roots')
    print('report=' + str(report_path))
    print(json.dumps(report))
    return 0


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except (OSError, RuntimeError, ValueError) as error:
        print(str(error), file=sys.stderr)
        raise SystemExit(1)
