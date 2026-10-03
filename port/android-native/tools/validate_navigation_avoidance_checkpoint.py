"""Bind reconstructed avoidance source, original comparisons and both APKs.

Live avoidance is a startup probe. Character movement still uses the existing
supported-floor adapter; physical ARM64 and full-game verification are pending.
"""
import argparse
import hashlib
import json
import re
import subprocess
import zipfile
from pathlib import Path

from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_navigation_checkpoint import ROOT, REPO, digest, read, log
from validate_collision_checkpoint import verify_capture
from validate_selector_checkpoint import fields
from validate_navigation_objects_checkpoint import EXPORTS as OBJECT_EXPORTS

PROBE = {
    'floors': 8, 'force_contributions': 16, 'adjusted': 8,
    'turn_limited': 8, 'state_fnv1a64': '09d1d79d2612c945',
}
TOTALS = {
    'force': 832, 'avoid': 750, 'can_collide': 426, 'force_records': 4524,
    'contributions': 4535, 'adjusted': 220, 'turn_limited': 181, 'gated': 275,
    'registry_keys_compared': 3377,
}
EXPORTS = OBJECT_EXPORTS | {
    'dh2_nav_can_collide', 'dh2_nav_obstacle_force', 'dh2_nav_avoid_obstacles',
}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--studio', type=Path, required=True)
    parser.add_argument('--host-build', default='/home/adampalace/dh2-world-build')
    args = parser.parse_args()
    local = REPO / '.local-inputs'
    report_dir = REPO / 'port/level-world/reports'
    prior_path = ROOT / 'reports/build-validation-navigation-objects.json'
    prior = read(prior_path)
    apk = ROOT / 'app/build/outputs/apk/debug/app-debug.apk'
    studio_apk = args.studio / 'app/build/outputs/apk/debug/app-debug.apk'
    sha = digest(apk)
    libs, studio_libs = inspect(apk), inspect(studio_apk)
    assert len(libs) == len(studio_libs) == 14
    engine = local / 'libDungeonHunter2.so'
    engine_sha = digest(engine)
    reference = digest(local / 'navigation-avoidance-original.bin')
    linked = digest(local / 'navigation-link-original.bin')
    floor_sha = digest(local / 'authored-floors-packaged.json')
    assert linked == prior['linked_reference_sha256']
    assert floor_sha == prior['floor_geometry_sha256']

    reports, library_sha = {}, {}
    for tag in ('oracle', 'packaged', 'studio'):
        suffix = '' if tag == 'oracle' else '-' + tag
        library = local / ('navigation-avoidance-oracle.so' if tag == 'oracle'
                           else f'navigation-avoidance-{tag}-world-arm64.so')
        library_sha[tag] = digest(library)
        report = read(report_dir / f'navigation-avoidance{suffix}-arm64-differential.json')
        fields(report, {
            'comparisons': 2008, 'mismatches': 0, 'totals': TOTALS,
            'crypt_avoidance_probe': PROBE, 'original_sha256': engine_sha,
            'arm64_library_sha256': library_sha[tag],
            'linked_reference_sha256': linked, 'floor_source_sha256': floor_sha,
            'reference_sha256': reference,
            'complete_ordered_forces_and_registry_compared': True,
            'original_physical_filter_and_concrete_virtual_execute': True,
        })
        assert digest(local / f'navigation-avoidance{suffix}-original.bin') == reference
        reports[tag] = report

    host_names = ('avoidance', 'avoidance-objects-regression',
                  'avoidance-motion-regression', 'avoidance-find-regression',
                  'avoidance-path-regression', 'avoidance-world-regression',
                  'avoidance-search-regression')
    host = {name: read(report_dir / f'navigation-{name}-host-audit.json')
            for name in host_names}
    fields(host['avoidance'], {
        'cases': 2008, 'operation_counts': [832, 750, 426],
        **{key: value for key, value in TOTALS.items()
           if key not in ('force', 'avoid', 'can_collide')},
        'atomic_rejection_checks': 8, 'empty_registry_early_gate_checks': 1,
        'authored_graph_nodes': 335, 'authored_graph_edges': 838,
        'reference_sha256': reference,
    })
    for name, old in (
        ('avoidance-objects-regression', 'objects'),
        ('avoidance-motion-regression', 'objects-motion-regression'),
        ('avoidance-find-regression', 'objects-find-regression'),
        ('avoidance-path-regression', 'objects-path-regression'),
        ('avoidance-world-regression', 'objects-world-regression'),
        ('avoidance-search-regression', 'objects-search-regression'),
    ):
        fields(host[name], prior['host_audits'][old])
    for report in host.values():
        fields(report, {'mismatches': 0, 'sanitizers': ['address', 'undefined']})

    world_dir = local / 'world-tests-navigation-avoidance'
    world = read(world_dir / 'world-smoke.json')
    fields(world, {
        'apk_sha256': sha, 'installed_apk_sha256': sha,
        'native_floor_records': 8, 'native_graph_nodes': 335,
        'native_graph_edges': 838, 'native_neighbour_floor_relations': 14,
        'native_validation_references': 998,
        'original_obstacle_force_reconstructed': True,
        'original_obstacle_avoidance_reconstructed': True,
        'original_concrete_physical_collision_filter_reconstructed': True,
        'native_avoidance_used_by_startup_probe': True,
        'native_avoidance_used_by_actor_movement': False,
        'native_obstacle_registry_used_by_actor_movement': False,
        'native_floor_motion_used_by_actor_movement': False,
        'original_actor_navigation_producers_reconstructed': False,
        'original_movement_controller_reconstructed': False,
        'frozen_idle_stable': True, 'live_idle_changed_pixels': True,
        'rotation_position_preserved': True, 'pause_cancels_movement': True,
        'crypt_avoidance_probe': PROBE,
        'physical_arm64_tested': False, 'full_game_playable': False,
    })
    assert len(world['cases']) == 10
    assert world['floor_geometry_sha256'] == floor_sha
    for key in ('crypt_objects_probe', 'crypt_motion_probe', 'crypt_findpath_probe'):
        assert world[key] == prior[key]
    prior_world = read(ROOT / 'reports/world-smoke-navigation-objects.json')
    for key in ('crypt_route_probe', 'crypt_world_route_probe'):
        assert world[key] == prior_world[key]

    assets, unchanged = {}, []
    with zipfile.ZipFile(apk) as z, zipfile.ZipFile(studio_apk) as sz:
        assert set(z.namelist()) == set(sz.namelist())
        assert not any('DungeonHunter2.so' in name for name in z.namelist())
        for name in z.namelist():
            if name.startswith('assets/') and not name.endswith('/'):
                raw, relative = z.read(name), name[7:]
                assert raw == sz.read(name)
                assert raw == (ROOT / 'app/src/main/assets' / relative).read_bytes()
                assert raw == (args.studio / 'app/src/main/assets' / relative).read_bytes()
                assets[relative] = {'bytes': len(raw),
                                    'sha256': hashlib.sha256(raw).hexdigest()}
            elif name.startswith('lib/') and name.endswith('/libdh2_level_world.so'):
                assert EXPORTS <= exports(z.read(name))
                assert EXPORTS <= exports(sz.read(name))
        assert assets == prior['assets_verified'] and len(assets) == 126
        for tag, archive in (('packaged', z), ('studio', sz)):
            assert hashlib.sha256(archive.read(
                'lib/arm64-v8a/libdh2_level_world.so')).hexdigest() == library_sha[tag]
        for rows, archive in ((prior['libraries'], z), (prior['studio_libraries'], sz)):
            for row in rows:
                if Path(row['path']).name not in {'libdh2_level_world.so', 'libdh2_native.so'}:
                    assert hashlib.sha256(archive.read(row['path'])).hexdigest() == row['sha256']
                    if archive is z:
                        unchanged.append(row['path'])
    for name, report in host.items():
        if name != 'avoidance-path-regression':
            assert report['bres_sha256'] == assets['worlds/crypt.bdae']['sha256']
            assert report['descriptor_sha256'] == assets['worlds/crypt01.dwld']['sha256']

    source = {}
    for name, expected in prior['studio_source_sha256'].items():
        value = digest(ROOT / 'app/src/main' / name)
        assert value == digest(args.studio / 'app/src/main' / name)
        if name != 'cpp/model_renderer.cpp':
            assert value == expected, name
        source[name] = value
    changed = {'android-native/README.md', 'android-native/ROADMAP.md',
               'android-native/tools/world_smoke.py', 'level-world/CMakeLists.txt',
               'level-world/README.md'}
    for name, expected in prior['module_source_sha256'].items():
        if name not in changed:
            assert digest(REPO / 'port' / name) == expected, name
    additions = {
        'level-world/navigation_avoidance.cpp', 'level-world/navigation_avoidance.hpp',
        'level-world/tests/navigation_avoidance.cpp',
        'level-world/tests/navigation_avoidance_differential.py',
        'level-world/tools/build_navigation_avoidance_oracle.ps1',
        'android-native/tools/validate_navigation_avoidance_checkpoint.py',
    }
    capture_dir = REPO / 'port/level-world/reference/navigation-avoidance'
    additions |= {f.relative_to(REPO / 'port').as_posix()
                  for f in capture_dir.rglob('*') if f.is_file()}
    verify_capture(capture_dir / 'original-functions.json', engine)
    for name in ('navigation-avoidance-repo-build.log', 'navigation-avoidance-studio-build.log'):
        assert 'BUILD SUCCESSFUL' in log(local / name)
    assert log(local / 'navigation-avoidance-zipalign.log').count('Verification successful') == 2
    marker = ('Native avoidance probe | floors 8 | force contributions 16 | adjusted 8 | '
              'turn limited 8 | state 09d1d79d2612c945 | actor producers and controller pending')
    logs = list(world_dir.glob('*.log'))
    assert logs
    for path in logs:
        text = log(path)
        assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|Shader failed|Link failed', text), path
        assert marker in text, path
    cache = subprocess.run(['wsl', 'cat', args.host_build + '/CMakeCache.txt'],
                           capture_output=True, text=True, check=True).stdout
    for prefix in ('CMAKE_CXX_FLAGS:STRING=', 'CMAKE_EXE_LINKER_FLAGS:STRING=',
                   'CMAKE_SHARED_LINKER_FLAGS:STRING='):
        assert '-fsanitize=address,undefined' in next(
            line for line in cache.splitlines() if line.startswith(prefix))

    result = {
        'apk_sha256': sha, 'apk_bytes': apk.stat().st_size,
        'studio_apk_sha256': digest(studio_apk), 'libraries': libs,
        'studio_libraries': studio_libs, 'unchanged_libraries': unchanged,
        'assets_verified': assets, 'original_input_count': prior['original_input_count'],
        'cache_sha256': prior['cache_sha256'], 'studio_source_sha256': source,
        'module_source_sha256': {n: digest(REPO / 'port' / n) for n in
                                 sorted(set(prior['module_source_sha256']) | changed | additions)},
        'avoidance_reports': reports, 'reference_sha256': reference,
        'linked_reference_sha256': linked, 'host_audits': host,
        'crypt_avoidance_probe': PROBE,
        **{key: prior[key] for key in ('crypt_objects_probe', 'crypt_motion_probe', 'crypt_findpath_probe')},
        'floor_geometry_sha256': floor_sha, 'world_cases': 10,
        'apk_16k_zip_alignment_verified': True,
        'prior_objects_checkpoint': {'apk_sha256': prior['apk_sha256'],
                                     'validation_sha256': digest(prior_path)},
        'prior_combat_checkpoint': prior['prior_combat_checkpoint'],
        'original_obstacle_force_reconstructed': True,
        'original_obstacle_avoidance_reconstructed': True,
        'original_concrete_physical_collision_filter_reconstructed': True,
        'original_init_object_reconstructed': True,
        'original_init_obstacle_reconstructed': True,
        'original_motion_obstacle_defaults_reconstructed': True,
        'full_pfobject_constructor_projection_verified': False,
        'original_obstacle_parent_backend_reconstructed': True,
        'parent_service_attached_to_native_position_validation': True,
        'avoidance_used_by_startup_probe': True,
        'avoidance_used_by_actor_movement': False,
        'original_actor_navigation_producers_reconstructed': False,
        'original_physical_body_construction_reconstructed': False,
        'original_cache_invalidation_lifecycle_reconstructed': False,
        'original_movement_controller_reconstructed': False,
        'nan_payload_or_sign_parity_claimed': False,
        'pending_navigation': 'GameObject capability/radius/obstacle and physical producers, cache lifecycle, controller/root motion and pursuing enemies.',
        'physical_arm64_tested': False, 'original_gpu_parity_verified': False,
        'full_cache_bundled': False, 'full_game_playable': False, 'goal_status': 'active',
    }
    (ROOT / 'reports/build-validation-navigation-avoidance.json').write_text(
        json.dumps(result, indent=2) + '\n', encoding='utf-8')
    (ROOT / 'reports/world-smoke-navigation-avoidance.json').write_text(
        json.dumps(world, indent=2) + '\n', encoding='utf-8')
    checkpoint = ROOT / 'build/checkpoints' / f'dh2-native-avoidance-{sha[:8]}.apk'
    checkpoint.write_bytes(apk.read_bytes())
    assert digest(checkpoint) == sha
    print(json.dumps({'apk_sha256': sha, 'apk_bytes': apk.stat().st_size,
                      'checkpoint': str(checkpoint), 'avoidance_comparisons_per_arm64_binary': 2008,
                      'crypt_avoidance_probe': PROBE, 'world_cases': 10,
                      'actor_movement_uses_avoidance': False, 'goal_status': 'active'}))


if __name__ == '__main__':
    main()
