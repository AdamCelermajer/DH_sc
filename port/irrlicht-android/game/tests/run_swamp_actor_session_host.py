#!/usr/bin/env python3
"""Build and exercise the bounded SWAMP source actor/physics frame on host."""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys


TESTS = Path(__file__).resolve().parent
GAME = TESTS.parent
IRRLICHT = GAME.parent
REPO = IRRLICHT.parents[1]
PORT = REPO / 'port'
NAVIGATION = PORT / 'navigation'
BOX2D = PORT / 'physics-backend' / 'box2d-2.0.1'
DEFAULT_ASSETS = PORT / 'android-native' / 'app' / 'src' / 'main' / 'assets'
DEFAULT_CACHE = REPO.parent / 'cache' / 'files'
DEFAULT_OUTPUT = IRRLICHT / 'build' / 'swamp-actor-session-host-checks'

ANIMATION_DATA = (
    'animations_pyarray.bin', 'animations_pyarraynames.bin',
    'animations_pystructnames.bin', 'animations_dictionary_pyarraynames.bin',
    'animations_dictionary_pyarray.bin', 'prince-animation-bank.bin',
    'character_properties_pyarray.bin', 'character_properties_pyarraynames.bin',
    'character_properties_pystructnames.bin', 'character_classes_pyarray.bin',
    'character_classes_pyarraynames.bin', 'character_classes_pystructnames.bin',
)

LEVEL_SOURCES = (
    'physical_world.cpp', 'native_body.cpp', 'physical_controls.cpp',
    'body_transform.cpp', 'subobjects_update.cpp', 'actor_runtime.cpp',
    'actor_rotation.cpp', 'character_body_config.cpp',
)


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(block)
    return digest.hexdigest()


def module_from_path(name: str, path: Path):
    spec = importlib.util.spec_from_file_location(name, path)
    if not spec or not spec.loader:
        raise RuntimeError(f'could not load helper module: {path}')
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def run(command: list[str | Path], *, cwd: Path = REPO) -> str:
    args = [str(value) for value in command]
    result = subprocess.run(args, cwd=cwd, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, text=True, check=False)
    if result.returncode:
        quoted = subprocess.list2cmdline(args) if os.name == 'nt' else ' '.join(args)
        raise RuntimeError(f'command exited {result.returncode}: {quoted}\n' +
                           result.stdout[-15000:])
    return result.stdout


def stage_assets(source_root: Path, target_root: Path) -> dict[str, dict[str, object]]:
    bank_path = source_root / 'data/prince-animation-bank.json'
    bank = json.loads(bank_path.read_text(encoding='utf-8'))
    if (bank.get('character') != 'KnightPlayerBase' or
            bank.get('animation_table') != 48 or
            bank.get('animation_set_id') != 12302 or
            bank.get('template_clip_id') != 1111 or
            len(bank.get('resources', [])) != 116 or
            len(bank.get('registration_requests', [])) != 158):
        raise RuntimeError('Prince authored 116/158 bank metadata differs')
    assets = [('models/prince_modular.bdae', None)]
    assets.extend((f'data/{name}', None) for name in ANIMATION_DATA)
    assets.extend((resource['asset'], resource) for resource in bank['resources'])
    report: dict[str, dict[str, object]] = {}
    for relative, expected in assets:
        source = source_root / relative
        target_relative = ('models/prince_modular.bdae' if relative.startswith('models/')
                           else 'dh2/prince-animation/' + relative)
        if not source.is_file():
            raise RuntimeError(f'missing checked Prince input: {source}')
        identity = {'bytes': source.stat().st_size, 'sha256': sha256(source)}
        if expected and (identity['bytes'] != expected['bytes'] or
                         identity['sha256'] != expected['sha256']):
            raise RuntimeError(f'Prince bank asset differs from metadata: {source}')
        target = target_root / target_relative
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)
        if target.stat().st_size != identity['bytes'] or sha256(target) != identity['sha256']:
            raise RuntimeError(f'staged asset differs from input: {source}')
        report[target_relative] = {'source': str(source), **identity}
    return report


def build_source_snapshot(cache: Path, output: Path, bridge_helper) -> tuple[Path, Path, str]:
    source_build = output / 'source-navigation'
    run([sys.executable, NAVIGATION / 'build.py', '--output', source_build])
    library = source_build / ('libdh2_navigation_host.dll' if sys.platform == 'win32'
                              else 'libdh2_navigation_host.so')
    navchecks = module_from_path('swamp_session_navigation_checks',
                                 NAVIGATION / 'tests/check_navigation.py')
    loaded = bridge_helper.import_source_navigation(cache, library, navchecks)
    world, dll, level, storage, bres, scene, nav, expected, water, cache_inputs = loaded
    snapshot = output / 'source-navigation.bin'
    bridge_helper.write_source_snapshot(snapshot, navchecks, nav)
    dll.dh2_nav_free(__import__('ctypes').byref(nav))
    dll.dh2_world_free(__import__('ctypes').byref(level))
    cache_identity = {
        'data/scene/001_swamp.mlx': hashlib.sha256(cache_inputs[0]).hexdigest(),
        'data/3d/modules/swamp/swamp.bdae': hashlib.sha256(cache_inputs[1]).hexdigest(),
    }
    del world, storage, bres, scene, expected, water
    return snapshot, library, json.dumps(cache_identity, sort_keys=True)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--assets', type=Path, default=DEFAULT_ASSETS)
    parser.add_argument('--cache', type=Path, default=DEFAULT_CACHE)
    parser.add_argument('--cxx', default='g++')
    parser.add_argument('--output', type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args()
    source_root = args.assets.resolve(strict=True)
    cache = args.cache.resolve(strict=True)
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    compiler = shutil.which(args.cxx)
    if not compiler:
        parser.error('host C++ compiler not found: ' + args.cxx)
    if not BOX2D.is_dir():
        parser.error('pinned Box2D 2.0.1 sources are absent: ' + str(BOX2D))

    staged_root = output / 'prince-input'
    staged_assets = stage_assets(source_root, staged_root)
    bridge_helper = module_from_path('swamp_session_bridge_host_helper',
        TESTS / 'run_swamp_actor_floor_bridge_host.py')
    snapshot, source_library, cache_hashes_json = build_source_snapshot(
        cache, output, bridge_helper)

    source_paths = [
        TESTS / 'swamp_actor_session_host.cpp',
        GAME / 'swamp_actor_session.cpp',
        GAME / 'swamp_actor_floor_bridge.cpp',
    ]
    prince_helper = module_from_path('swamp_session_prince_host_helper',
        TESTS / 'run_prince_host.py')
    source_paths.extend(p for p in prince_helper.SOURCES
                        if p.name != 'prince_actor_host.cpp')
    bridge_sources = bridge_helper.ACTOR_SOURCES
    source_paths.extend(p for p in bridge_sources
                        if p.name != 'swamp_actor_floor_bridge_host.cpp')
    source_paths.extend(PORT / 'level-world' / name for name in LEVEL_SOURCES)
    source_paths = list(dict.fromkeys(source_paths))
    missing = [str(path) for path in source_paths if not path.is_file()]
    if missing:
        raise RuntimeError('missing session C++ input(s):\n' + '\n'.join(missing))

    objects_dir = output / 'objects'
    if objects_dir.exists():
        shutil.rmtree(objects_dir)
    objects_dir.mkdir(parents=True)
    compile_flags = [
        '-std=c++17', '-O1', '-Wall', '-Wextra', '-Werror',
        '-Wno-misleading-indentation', '-Wno-unused-parameter',
        '-Wno-unused-value', '-fexceptions', '-fno-fast-math',
        '-ffp-contract=off', '-include', 'cstring',
        '-I', GAME, '-I', PORT / 'level-world', '-I', PORT,
        '-I', PORT / 'android-app', '-I', BOX2D / 'Include',
    ]
    objects: list[Path] = []
    compile_commands: list[list[str]] = []

    for index, source in enumerate(source_paths):
        obj = objects_dir / f'repo-{index:03d}.o'
        flags = list(compile_flags)
        if source.name in ('physical_world.cpp', 'native_body.cpp'):
            flags.extend(['-Dfinite=_finite', '-include', 'float.h'])
        command = [compiler, *map(str, flags), '-c', str(source), '-o', str(obj)]
        run(command)
        objects.append(obj)
        compile_commands.append(command)

    box2d_sources = sorted((BOX2D / 'Source').rglob('*.cpp'))
    if not box2d_sources:
        raise RuntimeError('pinned Box2D source tree has no C++ sources')
    for index, source in enumerate(box2d_sources):
        obj = objects_dir / f'box2d-{index:03d}.o'
        command = [compiler, '-std=c++17', '-O1', '-w', '-fno-fast-math',
                   '-ffp-contract=off', '-Dfinite=_finite', '-include', 'float.h',
                   '-include', 'cstring', '-I', str(BOX2D / 'Include'),
                   '-c', str(source), '-o', str(obj)]
        run(command)
        objects.append(obj)
        compile_commands.append(command)

    executable = output / ('swamp-actor-session-host.exe' if os.name == 'nt'
                           else 'swamp-actor-session-host')
    link_command = [compiler, *map(str, objects), '-o', str(executable)]
    run(link_command)
    test_output = run([executable, staged_root, snapshot])
    line = next((value for value in test_output.splitlines()
                 if value.startswith('{') and value.endswith('}')), None)
    if not line:
        raise RuntimeError('session host test did not emit structured result:\n' + test_output)
    result = json.loads(line)
    if (result.get('validation') != 'PASS' or
            result.get('frames') != result.get('world_steps') or
            result.get('source_state_idle') != 3 or
            result.get('source_state_move') != 4 or
            result.get('idle_body_pinned') is not True or
            result.get('move_body_unpinned') is not True or
            result.get('source_body_present_bound') is not True or
            result.get('move_focus_unpins_body') is not True or
            result.get('move_blur_call_order') != ['stop', 'pin'] or
            result.get('body_service_calls_before_shutdown') != 4 or
            result.get('duplicate_body_binding_rejected') is not True or
            result.get('live_character_reload_rejected') is not True or
            result.get('body_services_detached_before_world_teardown') is not True or
            result.get('post_teardown_character_update_safe') is not True or
            result.get('move_direction_checked_while_moving') is not True or
            result.get('released_input_returns_idle_and_pins_body') is not True or
            result.get('source_path_segments_after_release') != 0 or
            result.get('source_path_requested_after_release') != 0 or
            result.get('actual_source_properties') is not True or
            not isinstance(result.get('resolved_collision_scale_property'), int) or
            not isinstance(result.get('body_radius_source'), (int, float)) or
            result.get('body_radius_source', 0) <= 0 or
            result.get('source_x_stop_y_free_frames', 0) <= 0 or
            result.get('source_y_boundary_redirect_frames', 0) <= 0 or
            result.get('source_y_redirected_x_motion_frames', 0) <= 0 or
            result.get('source_y_free_axis_then_boundary_slide') is not True):
        raise RuntimeError('session host output omitted a source-frame/body assertion: ' + line)

    report = {
        **result,
        'host_build': True,
        'compiler': str(Path(compiler).resolve()),
        'compile_commands': compile_commands,
        'link_command': link_command,
        'test_output': test_output.strip(),
        'cache_input_sha256': json.loads(cache_hashes_json),
        'source_navigation_snapshot_sha256': sha256(snapshot),
        'source_navigation_library_sha256': sha256(source_library),
        'staged_prince_assets': staged_assets,
        'source_sha256': {path.relative_to(REPO).as_posix(): sha256(path)
                          for path in source_paths},
        'header_sha256': {
            (GAME / 'swamp_actor_session.hpp').relative_to(REPO).as_posix():
                sha256(GAME / 'swamp_actor_session.hpp'),
            (GAME / 'swamp_actor_floor_bridge.hpp').relative_to(REPO).as_posix():
                sha256(GAME / 'swamp_actor_floor_bridge.hpp'),
            (GAME / 'prince_character_runtime.hpp').relative_to(REPO).as_posix():
                sha256(GAME / 'prince_character_runtime.hpp'),
        },
        'box2d_source_count': len(box2d_sources),
        'box2d_license': str(PORT / 'physics-backend' / 'box2d-2.0.1' / 'License.txt'),
        'host_executable': {
            'path': str(executable), 'bytes': executable.stat().st_size,
            'sha256': sha256(executable),
        },
        'runtime_scope': (
            'Real source cache module-zero floors with source flags/path mask, Prince source Idle/Move ' +
            'Coordinator and 116/158 animation bank, actor_runtime root-driven movement, source ' +
            'Character focus/blur pin-unpin-stop calls bound to NativeWorld body create/teardown, host-only.'
        ),
        'limits': [
            'No APK build, emulator, or Android runtime was exercised.',
            'The host test loads the actual resolved 224-property Prince sheet and derives owner '
            'bounds/collision scale from it; its level-wide Box2D bounds remain an explicit host fixture.',
            'The host axis trace demonstrates free +Y followed by source PF heading redirection and '
            'edge-slide motion on the selected module-zero boardwalk; it does not model every SWAMP surface.',
            'No SWAMP props, other-module seams, camera, AI, combat, or script manager is composed.',
            'Script-blocked timers are not represented by PrinceCharacterRuntime and are not simulated.',
        ],
    }
    report_path = output / 'validation.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print('PASS: SWAMP source actor session host checks; report=' + str(report_path))
    print(json.dumps(report))
    return 0


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except (OSError, ValueError, RuntimeError) as error:
        print('ERROR: ' + str(error), file=sys.stderr)
        raise SystemExit(1)
