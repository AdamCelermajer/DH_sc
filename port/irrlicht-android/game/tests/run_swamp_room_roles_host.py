#!/usr/bin/env python3
"""Host-test source room scenery roles across all nine SWAMP modules."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys


TESTS = Path(__file__).resolve().parent
GAME = TESTS.parent
REPO = TESTS.parents[3]
WORLD_SOURCES = [
    TESTS / 'swamp_room_roles_host.cpp',
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


def run(command: list[str | Path], *, cwd: Path = REPO) -> str:
    result = subprocess.run([str(item) for item in command], cwd=cwd,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            text=True, check=False)
    if result.returncode:
        raise RuntimeError('command failed: ' + ' '.join(map(str, command)) +
                           '\n' + result.stdout[-18000:])
    return result.stdout


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', required=True, type=Path,
                        help='Extracted owner-supplied cache with SWAMP MLX/BRES')
    parser.add_argument('--cxx', default='g++')
    parser.add_argument('--output', type=Path,
                        default=REPO / 'port/irrlicht-android/build/swamp-room-roles-host')
    args = parser.parse_args()
    cache = args.cache.resolve(strict=True)
    if not (cache / 'data/scene/001_swamp.mlx').is_file():
        parser.error('cache is missing data/scene/001_swamp.mlx')
    compiler = shutil.which(args.cxx)
    if not compiler:
        parser.error('host C++ compiler not found: ' + args.cxx)
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    executable = output / ('swamp-room-roles.exe' if sys.platform == 'win32'
                           else 'swamp-room-roles')
    compile_command = [compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
        '-Werror', '-fno-exceptions', '-fno-rtti',
        '-I', REPO / 'port/irrlicht-android/upstream/include',
        *WORLD_SOURCES, '-o', executable]
    run(compile_command)
    output_text = run([executable, cache])
    line = next((row for row in output_text.splitlines()
                 if row.startswith('{') and row.endswith('}')), None)
    if not line:
        raise RuntimeError('room-role host did not return its structured result:\n' +
                           output_text)
    roles = json.loads(line)
    modules = roles.get('modules', [])
    if roles.get('validation') != 'PASS' or len(modules) != 9:
        raise RuntimeError('room-role host did not cover all nine modules: ' + line)
    if not roles.get('source_draws') or not roles.get('scenery_draws') \
            or not roles.get('root_bounds') or not roles.get('floor_draws') \
            or not roles.get('exit_markers'):
        raise RuntimeError('source room roles were not observed across SWAMP modules')

    bridge_output = output / 'floor-bridge'
    bridge_text = run([sys.executable,
        GAME / 'tests/run_swamp_actor_floor_bridge_host.py',
        '--cache', cache, '--output', bridge_output])
    bridge_report_path = bridge_output / 'validation.json'
    bridge = json.loads(bridge_report_path.read_text(encoding='utf-8'))
    if bridge.get('validation') != 'PASS' or bridge.get('module') != 0 \
            or bridge.get('path_mask') != 2:
        raise RuntimeError('source floor/nav bridge regression did not pass:\n' +
                           bridge_text[-12000:])
    if roles.get('module_zero_floor_draws') != bridge.get('source_module_zero_surfaces') \
            or roles.get('module_zero_floor_triangles') != bridge.get('mapped_triangles'):
        raise RuntimeError('room render filtering disagrees with retained module-zero nav geometry: '
            f"draws={roles.get('module_zero_floor_draws')} vs {bridge.get('source_module_zero_surfaces')}; "
            f"triangles={roles.get('module_zero_floor_triangles')} vs {bridge.get('mapped_triangles')}")
    if bridge.get('mapped_triangles') != 99 or bridge.get('start', {}).get('node_id') \
            != '_floor_obj_4of4_brdwalk_sw_-node' \
            or bridge.get('water', {}).get('node_id') \
            != '_floor_water_obj_4of4_brdwalk_sw_-node' \
            or bridge.get('water', {}).get('flags') != 2 \
            or bridge.get('source_queries', {}).get('water_mask0', {}).get('found') is not False \
            or bridge.get('outside', {}).get('found') is not False:
        raise RuntimeError('room role gate lost source boardwalk/water/path-mask/outside evidence')

    mlx = (cache / 'data/scene/001_swamp.mlx').read_bytes()
    bres = (cache / 'data/3d/modules/swamp/swamp.bdae').read_bytes()
    input_sha = {
        'data/scene/001_swamp.mlx': hashlib.sha256(mlx).hexdigest(),
        'data/3d/modules/swamp/swamp.bdae': hashlib.sha256(bres).hexdigest(),
    }
    report = {
        **roles,
        'host_build': True,
        'scope': 'host SceneMesh role-classification over all nine supplied SWAMP module roots; no APK or device run',
        'source_cache_sha256': input_sha,
        'module_zero_navigation_check': {
            'passed': True,
            'source_floor_draws': roles['module_zero_floor_draws'],
            'navigation_surfaces': bridge['source_module_zero_surfaces'],
            'scene_mesh_floor_triangles': roles['module_zero_floor_triangles'],
            'native_floor_bridge_triangles': bridge['mapped_triangles'],
            'boardwalk_query': bridge['start'],
            'water_query': bridge['water'],
            'water_mask_zero_rejected': bridge['source_queries']['water_mask0'],
            'outside_rejected': bridge['outside'],
            'path_mask': bridge['path_mask'],
            'source_navigation_snapshot_sha256': bridge['source_snapshot_sha256'],
            'comparison': 'filtered scenery is a render-only view; the independent source floor bridge still imports both module-zero floor surfaces and all 99 triangles, and source-ID/flags/height/path-mask queries pass',
        },
        'compiler': compiler,
        'compile_command': [str(item) for item in compile_command],
        'host_executable': str(executable),
        'host_executable_sha256': sha256(executable),
        'bridge_report': str(bridge_report_path),
        'bridge_report_sha256': sha256(bridge_report_path),
        'source_sha256': {path.relative_to(REPO).as_posix(): sha256(path)
            for path in WORLD_SOURCES + [GAME / 'scene_mesh_adapter.hpp',
                GAME / 'scene_mesh_adapter.cpp',
                GAME.parent / 'swamp-smoke/main.cpp',
                GAME / 'tests/run_swamp_room_roles_host.py']},
    }
    report_path = output / 'validation.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print('PASS: SWAMP source room render roles on all nine modules; module-zero navigation remains 2 surfaces / 99 triangles; report=' + str(report_path))
    print(json.dumps(report))


if __name__ == '__main__':
    main()
