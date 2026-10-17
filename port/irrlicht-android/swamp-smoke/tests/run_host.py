#!/usr/bin/env python3
"""Run the cache-scene, source-movement, and Irrlicht joystick host assertions."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys


TESTS = Path(__file__).resolve().parent
SMOKE = TESTS.parent
REPO = TESTS.parents[3]


def run(command: list[str | Path], *, cwd: Path = REPO) -> str:
    result = subprocess.run([str(item) for item in command], cwd=cwd,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            text=True, check=False)
    if result.returncode:
        raise RuntimeError("command failed: " + " ".join(map(str, command)) +
                           "\n" + result.stdout[-12000:])
    return result.stdout


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', required=True, type=Path,
                        help='Extracted supplied cache containing SWAMP BRES/MLX files')
    parser.add_argument('--cxx', default='g++',
                        help='Host C++ compiler used for the touch/fixed-step policy test')
    parser.add_argument('--output', type=Path,
                        default=REPO / 'port/irrlicht-android/build/swamp-host-checks')
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    cache = args.cache.resolve(strict=True)
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    compiler = shutil.which(args.cxx)
    if not compiler:
        parser.error(f'host C++ compiler not found: {args.cxx}')

    control_exe = output / ('control-policy.exe' if sys.platform == 'win32'
                            else 'control-policy')
    run([compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
         '-fno-exceptions', '-fno-rtti', SMOKE / 'tests/control_policy.cpp',
         '-o', control_exe])
    control_output = run([control_exe])
    if 'touch axes, diagonal clamp, Idle release, blocked Idle, fixed-step, frame cap and pause reset pass' not in control_output:
        raise RuntimeError('Irrlicht touch/fixed-step policy assertions did not report pass')

    alpha_exe = output / ('alpha-map-policy.exe' if sys.platform == 'win32'
                          else 'alpha-map-policy')
    run([compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
         '-fno-exceptions', '-fno-rtti', SMOKE / 'tests/alpha_map_policy.cpp',
         '-o', alpha_exe])
    alpha_output = run([alpha_exe])
    if 'source CurrentTechnique AL/AT profile resolution, fail-closed selector handling, and blue-to-alpha composition pass' not in alpha_output:
        raise RuntimeError('Irrlicht source AlphaMap policy assertions did not report pass')

    scene_output = run([sys.executable,
                        REPO / 'port/android-app/tests/run_swamp_scene.py',
                        '--cache', cache])
    expected_scene = (
        '103 subtree records, 54 source draw commands (54 source-visible; '
        '53 drawn diagnostics; 1 unresolved diagnostic draw omitted)')
    if expected_scene not in scene_output:
        raise RuntimeError('SWAMP source module scene assertions changed:\n' + scene_output)
    expected_alpha = '22 resolved AlphaMap refs/22 source-selector-derived AL draws (AT=0 unresolved=0'
    if expected_alpha not in scene_output or '10816 vertices, 13284 indices' not in scene_output:
        raise RuntimeError('SWAMP AlphaMap/cache-source assertions changed:\n' + scene_output)
    if 'correction=(-52000,-3000,0); BRES unchanged' not in scene_output:
        raise RuntimeError('SWAMP module-zero source placement/immutability assertion missing')

    # Reuse the dedicated checked complete-bank runner rather than restaging a
    # partial idle/walk-only input set here. It records every asset and source
    # hash and keeps the umbrella gate aligned with the current Character path.
    prince_host_dir = output / 'prince-character-host'
    prince_output = run([
        sys.executable,
        REPO / 'port/irrlicht-android/game/tests/run_prince_host.py',
        '--assets', REPO / 'port/android-native/app/src/main/assets',
        '--compiler', args.cxx,
        '--output', prince_host_dir,
    ])
    prince_report_path = prince_host_dir / 'validation.json'
    prince_report = json.loads(prince_report_path.read_text(encoding='utf-8'))
    prince_test_output = prince_report.get('test_output', '')
    required_prince = (
        'controllers=4', 'bank_resources=116', 'registration_occurrences=158',
        'idle_release=pass', 'source_fsm=pass',
        'full_game_ai_physics_combat=not_implemented',
    )
    if prince_report.get('validation') != 'PASS' or not all(
            token in prince_test_output for token in required_prince):
        raise RuntimeError('Prince source Character host assertions changed:\n' +
                           prince_test_output)
    prince_exe = Path(prince_report['executable']['path'])

    movement_build = output / 'movement'
    movement_report = movement_build / 'build-validation.json'
    run([sys.executable, REPO / 'port/swamp-movement/build.py',
         '--output', movement_build, '--report', movement_report])
    movement_library = movement_build / (
        'libdh2_swamp_movement_host.dll' if sys.platform == 'win32'
        else 'libdh2_swamp_movement_host.so')
    movement_result = output / 'movement-validation.json'
    run([sys.executable, REPO / 'port/swamp-movement/tests/check_movement.py',
         '--library', movement_library, '--cache', cache, '--report', movement_result])
    movement_data = json.loads(movement_result.read_text(encoding='utf-8'))
    if movement_data.get('pass') is not True or movement_data.get('complete_movement') is not False:
        raise RuntimeError('Existing bounded source SWAMP movement assertions failed or changed scope')

    source_files = [SMOKE / 'main.cpp', SMOKE / 'README.md', SMOKE / 'alpha_map_policy.hpp',
                    REPO / 'port/scene-materials/technique_selector.hpp',
                    REPO / 'port/scene-materials/technique_selector.cpp',
                    SMOKE / 'tests/run_host.py',
                    SMOKE / 'tests/alpha_map_policy.cpp', SMOKE / 'control_policy.hpp',
                    SMOKE / 'tests/control_policy.cpp',
                    REPO / 'port/android-app/tests/swamp_scene.cpp',
                    REPO / 'port/android-app/tests/run_swamp_scene.py',
                    REPO / 'port/swamp-movement/movement.cpp',
                    REPO / 'port/irrlicht-android/game/tests/run_prince_host.py']
    source_files = list(dict.fromkeys(source_files))
    source_hashes = {
        path.relative_to(REPO).as_posix(): sha256(path) for path in source_files
    }
    source_hashes.update(prince_report.get('source_sha256', {}))
    report = {
        'pass': True,
        'scope': 'Host assertions validate SWAMP module-zero import/placement, exact source CurrentTechnique AL/AT selection by GLES/GLES2 profile with unsupported/conflicting selectors fail-closed, all 22 AlphaMap-to-AL mappings and blue-channel composition, path-mask movement and touch/fixed-step policy, plus four source Prince warrior skins, idle/walk deformation, idle release, and one-time owner translation; Android render/install remains separate.',
        'source_scene': expected_scene,
        'source_alpha_map': expected_alpha,
        'control_policy': control_output.strip(),
        'alpha_map_policy': alpha_output.strip(),
        'prince_character': prince_test_output,
        'prince_input_sha256': {
            relative: asset['sha256'] for relative, asset in
            prince_report.get('asset_inputs', {}).items()},
        'movement_checks': movement_data.get('checks', {}),
        'source_sha256': source_hashes,
        'host_artifacts': {
            'control_test_sha256': sha256(control_exe),
            'prince_character_test_sha256': sha256(prince_exe),
            'movement_library_sha256': sha256(movement_library),
        },
    }
    report_path = args.report.resolve() if args.report else output / 'validation.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'PASS: SWAMP Irrlicht host assertions; report={report_path}')


if __name__ == '__main__':
    main()
