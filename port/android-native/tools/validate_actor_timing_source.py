"""Bind recovered movement/scene-body/timeline kernels to current APKs."""
import argparse
import hashlib
import json
import subprocess
import sys
import zipfile
from pathlib import Path
from run_actor_timing_checks import REPO, LOCAL, REPORTS, SPECS, comparisons, sha
from validate_combat_checkpoint import exports


def read(path):
    return json.loads(path.read_text())


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--studio', type=Path, required=True)
    args = parser.parse_args()
    subprocess.run([sys.executable, str(Path(__file__).with_name('validate_physics_backend_source.py')),
                    '--studio', str(args.studio), '--build-prefix', 'actor-timing',
                    '--world-output', str(LOCAL / 'world-tests-actor-timing/world-smoke.json')],
                   cwd=REPO, check=True)
    prior = read(REPORTS / 'physics-backend-source-validation.json')
    apks = {'packaged': REPO / 'port/android-native/app/build/outputs/apk/debug/app-debug.apk',
            'studio': args.studio / 'app/build/outputs/apk/debug/app-debug.apk'}
    required = {'dh2_move_policy', 'dh2_move_speed', 'dh2_move_rotation_speed', 'dh2_move_focus_begin',
                'dh2_decor_body_config', 'dh2_decor_level_world_bounds', 'dh2_decor_marker_mesh_box',
                'dh2_timeline_update', 'dh2_timeline_notify', 'dh2_timeline_extra', 'dh2_timeline_replay'}
    verified = {}
    for tag, apk in apks.items():
        assert sha(apk) == prior['apk_sha256'][tag]
        with zipfile.ZipFile(apk) as archive:
            raw = archive.read('lib/arm64-v8a/libdh2_level_world.so')
        library_sha = hashlib.sha256(raw).hexdigest()
        assert required <= exports(raw) and sha(LOCAL / f'actor-timing-{tag}-world-arm64.so') == library_sha
        verified[tag] = {}
        for module, (stem, gold, count) in SPECS.items():
            native = read(REPORTS / f'actor-timing-{tag}-{module}-arm64-differential.json')
            host = read(REPORTS / f'actor-timing-{module}-host-audit.json')
            assert comparisons(native) == comparisons(host) == count
            assert native['mismatches'] == host['mismatches'] == 0
            assert native.get('native_sha256', native.get('arm64_library_sha256')) == library_sha
            assert native['original_sha256'] == sha(LOCAL / 'libDungeonHunter2.so')
            corpus_sha = native.get('corpus_sha256', native.get('reference_sha256'))
            assert corpus_sha == host['reference_sha256'] == sha(gold)
            assert corpus_sha == sha(LOCAL / f'actor-timing-{tag}-{module}-reference.bin')
            assert host['sanitizers'] == ['address', 'undefined']
            verified[tag][module] = native
    sources = {}
    for stem in ('move_state', 'decor_body_config', 'visual_timeline'):
        for extension in ('hpp', 'cpp'):
            path = REPO / f'port/level-world/{stem}.{extension}'
            sources[path.relative_to(REPO).as_posix()] = sha(path)
    frame_notes = REPO / 'port/level-world/reference/frame-order/NOTES.md'
    report = {
        'apk_sha256': prior['apk_sha256'],
        'source_sha256': sources,
        'physics_backend_validation_sha256': sha(REPORTS / 'physics-backend-source-validation.json'),
        'frame_order_trace_sha256': sha(frame_notes),
        'packaged_differentials': verified,
        'new_cases_per_apk': sum(item[2] for item in SPECS.values()),
        'previous_packaged_cases_per_apk': 13642 + 21903,
        'real_asset_root_samples_per_apk': 3567,
        'world_cases': prior['world_cases'],
        'all_required_assets_bundled': False,
        'live_actor_uses_recovered_movement_pipeline': False,
        'live_actor_movement_policy': prior['live_actor_movement_policy'],
        'physical_arm64_tested': False,
        'original_gpu_parity_verified': False,
        'full_game_playable': False,
        'checkpoint_apk_created': False,
        'goal_status': 'active',
    }
    (REPORTS / 'actor-timing-source-validation.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({'validation': 'PASS', 'new_cases_per_apk': report['new_cases_per_apk'],
                      'world_cases': report['world_cases'], 'live_pipeline_integrated': False}))


if __name__ == '__main__':
    main()
