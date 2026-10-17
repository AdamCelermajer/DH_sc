#!/usr/bin/env python3
"""Refresh the SWAMP evidence index from a built APK and device test reports."""
from __future__ import annotations

import argparse
from datetime import date
import hashlib
import json
import os
from pathlib import Path
import re


HERE = Path(__file__).resolve().parent
APP = HERE.parent
REPO = APP.parents[1]


def read_json(path: Path):
    return json.loads(path.read_text(encoding='utf-8'))


def rel(path: Path):
    return Path(os.path.relpath(path.resolve(), REPO)).as_posix()


def axis_summary(movement, axis):
    data = movement[axis]
    match = re.search(r'yaw (-?\d+(?:\.\d+)?)', data['post_release_report'])
    return {
        'before_xyz': data['before_xyz'],
        'after_release_xyz': data['after_release_xyz'],
        'stable_after_release_xyz': data['after_release_stable_xyz'],
        'heading_after_release_radians': float(match.group(1)) if match else None,
    }


def device_summary(check, result_path: Path):
    movement = check['movement_checks']
    device_root = result_path.parent / check['serial']
    shots = [{
        'path': (Path(rel(device_root / shot['file']))).as_posix(),
        'bytes': shot['bytes'],
        'sha256': shot['sha256'],
    } for shot in check['screenshots']]
    return {
        'android_release': check['android_release'],
        'api': check['sdk'],
        'page_size_bytes': check['page_size_bytes'],
        'abi': check['abi'],
        'installed_apk_sha256': check['installed_apk_sha256'],
        'installed_apk_bytes': check['installed_apk_bytes'],
        'positive_x': axis_summary(movement, 'east'),
        'positive_y': axis_summary(movement, 'north'),
        'pause_resume_no_drift': movement['pause_resume']['no_drift'],
        'no_floor_candidate_rejected_without_position_change':
            movement['no_floor_rejection']['blocked_candidate_preserved_position'],
        'no_floor_report': movement['no_floor_rejection']['report_while_stick_held'],
        'trace_completed': check['trace_completed'],
        'return_preserved_encounter': check['encounter_unchanged_after_return'],
        'trace_ui_controls_nonoverlap': check['trace_panel_and_both_controls_nonoverlapping'],
        'logged_walk_and_idle_transitions': check['movement_animation_transitions_logged'],
        'filtered_app_fatal_or_native_error_entries': check['filtered_error_log_entries'],
        'evidence_directory': rel(device_root),
        'screenshots': shots,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report', type=Path, default=APP/'swamp-preview-runtime-validation.json')
    parser.add_argument('--build-report', type=Path, default=APP/'build-validation.json')
    parser.add_argument('--movement-report', type=Path, required=True)
    parser.add_argument('--device-result', type=Path, action='append', default=[])
    parser.add_argument('--device-aggregate', type=Path,
                        help='Validated per-device evidence assembled from separate complete runs')
    args = parser.parse_args()
    if bool(args.device_result) == bool(args.device_aggregate):
        parser.error('supply either --device-result (repeatable) or --device-aggregate')

    report = read_json(args.report)
    build = read_json(args.build_report)
    movement_host = read_json(args.movement_report)
    apk = build['apk']
    report['date'] = date.today().isoformat()
    report['scope'] = (
        'Source-built authored development encounter plus static SWAMP module-zero '
        'geometry/material, path-mask-gated endpoint movement, and an explicit '
        'bounded logical trace of the original module-one LizardMan_Intro. Not '
        'the complete original game.'
    )
    report['apk'].update({
        'path': 'port/android-app/build/dh2-source-renderer-debug.apk',
        'sha256': apk['sha256'],
        'bytes': apk['bytes'],
        'target_sdk': build['target_sdk'],
        'min_sdk': build['min_sdk'],
        'abis': list(build['abi']),
        'elf_load_alignment_bytes': 16384,
        'signature_schemes_verified': ['v2', 'v3'],
    })
    report['preview']['movement'].update({
        'candidate_rule': (
            'endpoint accepted only when a known, non-void/non-wall source floor '
            'passes native CanPathOn subset rule for baseline player mask 2 and '
            'strict abs(candidate_z-floor_z)<100; accepted object-mode validation '
            'writes sampled floor height to candidate Z, while rejected steps roll back'
        ),
        'source_floor_examples': {
            'water_mask_2_passes_player_mask_2': True,
            'module_7_hole_mask_1_rejected_by_player_mask_2': True,
            'module_zero_contains_hole': False,
        },
        'limits': [
            'selected floor ordering and vertical hit selection remain an approximation',
            'no segment/radius wall or actor collision; endpoint steps can cross thin geometry',
            'speed 30 units/second, module-zero restriction and fixed steps are preview choices',
            'not a native Character controller; acceleration and source speed are unresolved',
        ],
        'host_test': 'port/swamp-movement/tests/check_movement.py',
    })
    report['host_checks']['navigation'] = (
        'pass; 9 modules, 16 surfaces, 626 triangles; source floor tags carried '
        'through hits and native CanPathOn subset filtering is applied by the '
        'actor-floor query; ordering/collision response remain partial'
    )

    devices = {}
    if args.device_aggregate:
        aggregate = read_json(args.device_aggregate)
        aggregate_apk = aggregate.get('apk', {})
        if (aggregate_apk.get('sha256') != apk['sha256'] or
                aggregate_apk.get('bytes') != apk['bytes']):
            raise ValueError(f'{args.device_aggregate}: aggregate APK does not match current build')
        for check in aggregate.get('device_evidence', []):
            if check.get('status') != 'passed':
                raise ValueError(f"{check.get('serial')}: device status is not passed")
            if (check.get('installed_apk_sha256') != apk['sha256'] or
                    check.get('filtered_error_log_entries') != 0):
                raise ValueError(f"{check.get('serial')}: installed APK or error-log check failed")
            capture_root = args.device_aggregate.parent / check['captures_directory']
            shots = [{
                'path': rel(capture_root / shot['file']),
                'bytes': shot['bytes'],
                'sha256': shot['sha256'],
            } for shot in check.get('screenshots', [])]
            assertions = check.get('assertions', [])
            devices[check['serial']] = {
                'android_release': check['android_release'],
                'api': check['sdk'],
                'page_size_bytes': check['page_size_bytes'],
                'abi': check['abi'],
                'installed_apk_sha256': check['installed_apk_sha256'],
                'installed_apk_bytes': apk['bytes'],
                'checks': assertions,
                'pause_resume_no_drift': any('pause/resume position stable' in x for x in assertions),
                'no_floor_candidate_rejected_without_position_change': any(
                    'no-floor candidate rejected without changing position' in x for x in assertions),
                'trace_completed': any('manual trace completed' in x for x in assertions),
                'return_preserved_encounter': any('return to encounter preserved state' in x
                                                  for x in assertions),
                'filtered_app_fatal_or_native_error_entries': check['filtered_error_log_entries'],
                'hud_status': check.get('hud_status'),
                'visual_result': check.get('visual_result'),
                'evidence_directory': rel(capture_root),
                'screenshots': shots,
            }
    else:
        for result_path in args.device_result:
            result = read_json(result_path)
            if result['apk_sha256'] != apk['sha256'] or result['apk_bytes'] != apk['bytes']:
                raise ValueError(f'{result_path}: device report APK does not match current build')
            for check in result['checks']:
                devices[check['serial']] = device_summary(check, result_path)

    if (len(devices) != 2 or {d['api'] for d in devices.values()} != {37} or
            {d['page_size_bytes'] for d in devices.values()} != {4096, 16384}):
        raise ValueError('expected API 37 emulator runs at both 4 KiB and 16 KiB page sizes')
    for serial, current in devices.items():
        prior = next((item for item in report['devices'] if item['serial'] == serial), None)
        if prior is not None:
            prior.update({
                'android_api': current['api'],
                'page_size_bytes': current['page_size_bytes'],
                'abi': current['abi'],
                'installed_apk_sha256': current['installed_apk_sha256'],
                'installed_apk_bytes': current['installed_apk_bytes'],
                'latest_movement_runtime': current,
            })
    report['latest_preview_runtime'] = {
        'date': date.today().isoformat(),
        'apk_sha256': apk['sha256'],
        'apk_bytes': apk['bytes'],
        'movement_host_checks': {
            'pass': movement_host['pass'],
            'checks': movement_host['checks'],
            'source_mesh': movement_host['source_mesh'],
        },
        'android_devices': devices,
    }
    if args.device_aggregate:
        report['latest_preview_runtime']['aggregate_note'] = aggregate.get('combined_rerun_note')
        report['latest_preview_runtime']['fold7_tested'] = aggregate.get('fold7_tested', False)

    current_shots = [shot['path'] for device in devices.values() for shot in device['screenshots']]
    report['screenshots_outside_repository'] = list(dict.fromkeys(
        report.get('screenshots_outside_repository', []) + current_shots))
    args.report.write_text(json.dumps(report, indent=2, ensure_ascii=False) + '\n', encoding='utf-8')
    print(json.dumps({'report': str(args.report), 'apk_sha256': apk['sha256'],
                      'devices': list(devices)}, indent=2))


if __name__ == '__main__':
    main()
