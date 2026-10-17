#!/usr/bin/env python3
"""Build and run the module-zero SWAMP-to-actor-floor host integration gate."""
from __future__ import annotations

import argparse
import ctypes as c
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys


TESTS = Path(__file__).resolve().parent
GAME = TESTS.parent
REPO = TESTS.parents[3]
NAVIGATION = REPO / 'port/navigation'

ACTOR_SOURCES = [
    TESTS / 'swamp_actor_floor_bridge_host.cpp',
    GAME / 'swamp_actor_floor_bridge.cpp',
    REPO / 'port/asset-payloads/payloads.cpp',
    REPO / 'port/engine-math/math.cpp',
    REPO / 'port/engine-resources/resources.cpp',
    REPO / 'port/scene-materials/scene.cpp',
    REPO / 'port/level-world/floors.cpp',
    REPO / 'port/level-world/floor_source.cpp',
    REPO / 'port/level-world/octree.cpp',
    REPO / 'port/level-world/selector.cpp',
    REPO / 'port/level-world/collision.cpp',
    REPO / 'port/level-world/navigation.cpp',
    REPO / 'port/level-world/navigation_world.cpp',
    REPO / 'port/level-world/navigation_motion.cpp',
    REPO / 'port/level-world/navigation_search.cpp',
    REPO / 'port/level-world/navigation_path.cpp',
    REPO / 'port/level-world/navigation_controller.cpp',
    REPO / 'port/level-world/navigation_heading.cpp',
    REPO / 'port/level-world/navigation_objects.cpp',
    REPO / 'port/level-world/navigation_producers.cpp',
    REPO / 'port/level-world/navigation_avoidance.cpp',
]


def run(command: list[str | Path]) -> str:
    result = subprocess.run([str(item) for item in command], cwd=REPO,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            text=True, check=False)
    if result.returncode:
        raise RuntimeError('command failed: ' + ' '.join(map(str, command)) +
                           '\n' + result.stdout[-18000:])
    return result.stdout


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def module_from_path(name: str, path: Path):
    spec = importlib.util.spec_from_file_location(name, path)
    if not spec or not spec.loader:
        raise RuntimeError(f'could not load Python host module: {path}')
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def fixed_bytes(text: bytes, size: int) -> bytes:
    if len(text) >= size:
        raise RuntimeError(f'source string too large for neutral view: {text!r}')
    return text + bytes(size - len(text))


def write_source_snapshot(path: Path, navchecks, nav) -> None:
    with path.open('wb') as output:
        output.write(struct.pack('<8sIII', b'DH2SWF01', 1,
                                 int(nav.surface_count), int(nav.triangle_count)))
        surface_format = '<13I128s128s128s128s128s256s'
        for index in range(nav.surface_count):
            surface = nav.surfaces[index]
            output.write(struct.pack(surface_format,
                index, surface.module_index, surface.module_source_record,
                surface.node_record, surface.geometry_index, surface.visible,
                surface.first_triangle, surface.triangle_count,
                surface.vertex_count, surface.primitive_count,
                surface.floor_type_flags, int(surface.floor_type_flags_known),
                int(surface.floor_type_tag_present),
                fixed_bytes(bytes(surface.module_name).split(b'\0', 1)[0], 128),
                fixed_bytes(bytes(surface.source_node_id).split(b'\0', 1)[0], 128),
                fixed_bytes(bytes(surface.source_node_name).split(b'\0', 1)[0], 128),
                fixed_bytes(bytes(surface.source_geometry_id).split(b'\0', 1)[0], 128),
                fixed_bytes(bytes(surface.source_geometry_name).split(b'\0', 1)[0], 128),
                fixed_bytes(bytes(surface.floor_type_tag).split(b'\0', 1)[0], 256)))
        triangle_format = '<9f3I'
        for index in range(nav.triangle_count):
            triangle = nav.triangles[index]
            output.write(struct.pack(triangle_format,
                *triangle.a, *triangle.b, *triangle.c,
                triangle.surface_index, triangle.primitive_index,
                triangle.source_triangle_index))


def import_source_navigation(cache: Path, library: Path, navchecks):
    world, dll = navchecks.load_world_bindings(library)
    U = c.c_uint32
    dll.dh2_nav_build_swamp.restype = U
    dll.dh2_nav_build_swamp.argtypes = [c.POINTER(navchecks.Navigation),
        c.POINTER(world.SourceLevel), c.POINTER(world.scene.Scene),
        c.POINTER(navchecks.Diagnostic)]
    dll.dh2_nav_free.restype = None
    dll.dh2_nav_free.argtypes = [c.POINTER(navchecks.Navigation)]
    dll.dh2_nav_query_actor_floor.restype = U
    dll.dh2_nav_query_actor_floor.argtypes = [c.POINTER(navchecks.Navigation),
        c.c_float, c.c_float, c.c_float, c.c_float, c.c_float, U,
        c.POINTER(navchecks.Hit), c.POINTER(c.c_bool)]

    level_bytes = (cache / 'data/scene/001_swamp.mlx').read_bytes()
    catalogue_bytes = (cache / 'data/3d/modules/swamp/swamp.bdae').read_bytes()
    level = world.SourceLevel()
    world_diagnostic = world.Diagnostic()
    status = dll.dh2_world_import_level(c.byref(level), b'SWAMP',
        b'data/scene/001_swamp.mlx', level_bytes, len(level_bytes),
        c.byref(world_diagnostic))
    if status != 0:
        raise RuntimeError('SWAMP MLX import failed: ' + bytes(world_diagnostic.message).decode(errors='replace'))
    bres_storage = c.create_string_buffer(catalogue_bytes)
    bres, scene = world.scene.Bres(), world.scene.Scene()
    if dll.dh2_bres_open(c.byref(bres), bres_storage, len(catalogue_bytes)) != 0:
        raise RuntimeError('SWAMP catalogue BRES parse failed')
    if dll.dh2_scene_open(c.byref(scene), c.byref(bres)) != 0:
        raise RuntimeError('SWAMP catalogue scene parse failed')
    nav = navchecks.Navigation()
    diagnostic = navchecks.Diagnostic()
    status = dll.dh2_nav_build_swamp(c.byref(nav), c.byref(level), c.byref(scene),
                                     c.byref(diagnostic))
    if status != 0:
        raise RuntimeError('SWAMP navigation build failed: ' + bytes(diagnostic.message).decode(errors='replace'))

    def query(point, distance, mask):
        hit = navchecks.Hit()
        found = c.c_bool()
        status = dll.dh2_nav_query_actor_floor(c.byref(nav), *point,
            distance, 1.0e-6, mask, c.byref(hit), c.byref(found))
        if status != 0:
            raise RuntimeError('SWAMP source actor-floor query failed')
        result = {'found': bool(found.value)}
        if found.value:
            surface = nav.surfaces[hit.surface_index]
            result.update({'source_surface': int(hit.surface_index),
                           'module_index': int(surface.module_index),
                           'flags': int(hit.floor_type_flags),
                           'height': float(hit.height),
                           'node_id': bytes(surface.source_node_id).split(b'\0', 1)[0].decode()})
        return result

    water_sample = None
    for surface_index in range(nav.surface_count):
        surface = nav.surfaces[surface_index]
        if surface.module_index != 0 or surface.floor_type_flags != 2:
            continue
        for triangle_index in range(surface.first_triangle,
                                    surface.first_triangle + surface.triangle_count):
            triangle = nav.triangles[triangle_index]
            point = tuple((float(triangle.a[axis]) + float(triangle.b[axis])
                           + float(triangle.c[axis])) / 3.0 for axis in range(3))
            hit = query(point, 1.0, 2)
            if hit['found'] and hit['source_surface'] == surface_index \
                    and hit['flags'] == 2:
                water_sample = {'point': point, 'source_surface': surface_index,
                                'mask2': hit, 'mask0': query(point, 1.0, 0)}
                break
        if water_sample:
            break
    if not water_sample:
        raise RuntimeError('Could not find an actual module-zero point whose source floor query selects water with mask 0x2')
    if water_sample['mask0']['found'] \
            and water_sample['mask0']['source_surface'] == water_sample['source_surface']:
        raise RuntimeError('Source path mask zero incorrectly admitted the water-required surface')

    expected = {
        'start': query((1090.75, -212.202, 258.0), 10.0, 2),
        'moved': query((1091.75, -212.202, 255.0), 10.0, 2),
        'water': water_sample['mask2'],
        'water_mask0': water_sample['mask0'],
        'outside': query((5000.0, 0.0, 255.0), 1000.0, 2),
    }
    snapshot_nav = nav
    return world, dll, level, bres_storage, bres, scene, snapshot_nav, expected, water_sample, (level_bytes, catalogue_bytes)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', required=True, type=Path,
                        help='Extracted supplied cache containing SWAMP BRES/MLX files')
    parser.add_argument('--cxx', default='g++')
    parser.add_argument('--output', type=Path,
                        default=REPO / 'port/irrlicht-android/build/swamp-actor-floor-bridge-host-checks')
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    cache = args.cache.resolve(strict=True)
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    compiler = shutil.which(args.cxx)
    if not compiler:
        parser.error('host C++ compiler not found: ' + args.cxx)

    navchecks = module_from_path('swamp_actor_floor_source_checks',
        NAVIGATION / 'tests/check_navigation.py')
    source_build = output / 'source-navigation'
    run([sys.executable, NAVIGATION / 'build.py', '--output', source_build])
    source_library = source_build / ('libdh2_navigation_host.dll' if sys.platform == 'win32'
                                     else 'libdh2_navigation_host.so')
    loaded = import_source_navigation(cache, source_library, navchecks)
    world, dll, level, bres_storage, bres, scene, nav, expected, water_sample, cache_inputs = loaded
    snapshot = output / 'source-navigation.bin'
    write_source_snapshot(snapshot, navchecks, nav)

    executable = output / ('swamp-actor-floor-bridge.exe' if sys.platform == 'win32'
                           else 'swamp-actor-floor-bridge')
    compile_command: list[str | Path] = [
        compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
        '-fno-fast-math', '-ffp-contract=off',
        '-I', GAME, '-I', REPO / 'port', '-I', REPO / 'port/android-app',
        *ACTOR_SOURCES, '-o', executable,
    ]
    run(compile_command)
    output_text = run([executable, snapshot,
        *(format(value, '.9g') for value in water_sample['point']),
        str(water_sample['source_surface'])])
    report_line = next((line for line in output_text.splitlines()
                        if line.startswith('{') and line.endswith('}')), None)
    if not report_line:
        raise RuntimeError('bridge host test did not produce structured PASS output:\n' + output_text)
    bridge = json.loads(report_line)
    if bridge.get('validation') != 'PASS' or bridge.get('module') != 0 \
            or bridge.get('path_mask') != 2 or bridge.get('source_floor_queries') != 3 \
            or bridge.get('rejection_checks') != 7:
        raise RuntimeError('bridge host result is outside the expected source scope: ' + report_line)
    for label in ('start', 'moved', 'water'):
        source_hit = expected[label]
        actor_hit = bridge[label]
        if not source_hit['found'] or source_hit['module_index'] != 0:
            raise RuntimeError(f'Original SWAMP source {label} fixture is not module-zero eligible: {source_hit}')
        if not actor_hit['found'] or actor_hit['source_surface'] != source_hit['source_surface'] \
                or actor_hit['flags'] != source_hit['flags'] \
                or actor_hit['node_id'] != source_hit['node_id'] \
                or abs(actor_hit['height'] - source_hit['height']) > 1.0e-4:
            raise RuntimeError(f'Actor bridge {label} result differs from original source query: source={source_hit}; actor={actor_hit}')
    if expected['outside']['found'] or bridge['outside']['found']:
        raise RuntimeError('Source or actor bridge accepted the outside test coordinate')

    dll.dh2_nav_free(c.byref(nav))
    dll.dh2_world_free(c.byref(level))
    report = {
        **bridge,
        'host_build': True,
        'compiler': compiler,
        'source_queries': expected,
        'comparison': 'initial, one-unit moved, and water source-surface/flags/node/height match within 1e-4; mask0 does not admit the water surface and actor runtime rejects it; outside point rejected by both',
        'source_cache_sha256': {
            'data/scene/001_swamp.mlx': hashlib.sha256(cache_inputs[0]).hexdigest(),
            'data/3d/modules/swamp/swamp.bdae': hashlib.sha256(cache_inputs[1]).hexdigest(),
        },
        'source_library_sha256': sha256(source_library),
        'source_snapshot_sha256': sha256(snapshot),
        'executable_sha256': sha256(executable),
        'source_sha256': {p.relative_to(REPO).as_posix(): sha256(p)
                          for p in ACTOR_SOURCES + [
                              GAME / 'swamp_actor_floor_bridge.hpp',
                              GAME / 'swamp_actor_floor_bridge.cpp',
                              GAME / 'swamp_source_navigation_view.hpp']},
        'host_executable': str(executable),
    }
    report_path = args.report.resolve() if args.report else output / 'validation.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print('PASS: SWAMP module-zero actor-floor bridge host checks; report=' + str(report_path))
    print(json.dumps(report))


if __name__ == '__main__':
    main()
