#!/usr/bin/env python3
"""Refresh tracked summaries from the latest local Android 17 emulator runs."""
from __future__ import annotations

from datetime import date
import json
from pathlib import Path


APP = Path(__file__).resolve().parents[1]
BUILD = APP / 'build'
EXPECTED_PAGES = {4096, 16384}


def read(path: Path):
    return json.loads(path.read_text(encoding='utf-8'))


def result(path: str):
    return read(BUILD / path / 'result.json')


def assert_apk(data, expected):
    if data.get('apk_sha256') != expected['sha256'] or data.get('apk_bytes') != expected['bytes']:
        raise ValueError('device result does not match the current build-validation.json APK')


def validate_pair(items, get_meta):
    if len(items) != 2 or {get_meta(item)['page_size_bytes'] for item in items} != EXPECTED_PAGES:
        raise ValueError('expected exactly API 37 Android 17 runs at 4 KiB and 16 KiB')
    for item in items:
        meta = get_meta(item)
        if meta['android_release'] != '17' or meta['sdk'] != 37 or meta['abi'] != 'x86_64':
            raise ValueError('unexpected emulator release, API, or ABI')


def main():
    built = read(APP / 'build-validation.json')['apk']
    dirs = {
        '16k': 'actor-runtime-api37-16k',
        '4k': 'actor-runtime-api37-4k',
    }
    actors = [result(path) for path in dirs.values()]
    validate_pair(actors, lambda item: {
        'page_size_bytes': item['page_size'], 'android_release': item['android_release'],
        'sdk': item['api_level'], 'abi': 'x86_64',
    })
    actor_rows = []
    for item, evidence_dir in zip(actors, ('actor-runtime-api37-16k', 'actor-runtime-api37-4k')):
        assert_apk(item, built)
        if item['installed_apk_sha256'] != built['sha256'] or item['pairs_tested'] != 18:
            raise ValueError('actor preview run did not test the exact installed APK and all 18 pairs')
        actor_rows.append({
            'serial': item['serial'], 'android_release': item['android_release'],
            'api': item['api_level'], 'page_size_bytes': item['page_size'], 'abi': 'x86_64',
            'installed_apk_sha256': item['installed_apk_sha256'],
            'model_count': item['model_count'], 'clip_count': item['clip_count'],
            'pairs_tested': item['pairs_tested'], 'returned_to_gameplay': item['returned_to_gameplay'],
            'tested_pairs': item['tested_pairs'], 'screenshots': item['screenshots'],
            'evidence_directory': f'build/{evidence_dir}',
        })
    (APP / 'infected-actor-current-apk-runtime-validation.json').write_text(
        json.dumps({'date': date.today().isoformat(), 'apk_sha256': built['sha256'],
                    'apk_bytes': built['bytes'], 'device_results': actor_rows,
                    'fold7_tested': False}, indent=2, ensure_ascii=False) + '\n', encoding='utf-8')

    village = [
        result('village-current-apk-16k-fixed'),
        result('village-current-apk-4k-fixed'),
    ]
    validate_pair(village, lambda item: item['check'])
    village_rows = []
    for item, evidence_dir in zip(village, (
            'village-current-apk-16k-fixed', 'village-current-apk-4k-fixed')):
        assert_apk(item, built)
        check = item['check']
        if (check['installed_apk_sha256'] != built['sha256'] or
                check['filtered_error_log_entries'] != 0 or len(check['cycles']) != 3 or
                not all(cycle['returned'] and cycle['hud_unchanged'] and
                        cycle['orbit'] is not None and
                        cycle['orbit']['sha256'] != cycle['preview']['sha256']
                        for cycle in check['cycles'])):
            raise ValueError(f"{check['serial']}: village preview cycle/orbit/log checks failed")
        village_rows.append({
            'serial': check['serial'], 'android_release': check['android_release'],
            'api': check['sdk'], 'page_size_bytes': check['page_size_bytes'], 'abi': check['abi'],
            'installed_apk_sha256': check['installed_apk_sha256'],
            'cycles': [{'cycle': c['cycle'], 'returned': c['returned'],
                        'hud_unchanged': c['hud_unchanged'], 'preview': c['preview'],
                        'orbit': c['orbit']} for c in check['cycles']],
            'filtered_error_log_entries': check['filtered_error_log_entries'],
            'evidence_directory': f'build/{evidence_dir}',
        })
    (APP / 'infected-village-current-apk-runtime-validation.json').write_text(
        json.dumps({'date': date.today().isoformat(), 'apk_sha256': built['sha256'],
                    'apk_bytes': built['bytes'], 'device_results': village_rows,
                    'fold7_tested': False}, indent=2, ensure_ascii=False) + '\n', encoding='utf-8')

    swamp_runs = [
        result('swamp-current-apk-16k-final2'), result('swamp-current-apk-4k'),
    ]
    def swamp_meta(item):
        c = item['checks'][0]
        return {'page_size_bytes': c['page_size_bytes'], 'android_release': c['android_release'],
                'sdk': c['sdk'], 'abi': c['abi']}
    validate_pair(swamp_runs, swamp_meta)
    swamp_rows = []
    for item, evidence_dir in zip(swamp_runs, (
            'swamp-current-apk-16k-final2', 'swamp-current-apk-4k')):
        assert_apk(item, built)
        if len(item['checks']) != 1:
            raise ValueError('expected one device in each SWAMP result')
        check = item['checks'][0]
        if (check['installed_apk_sha256'] != built['sha256'] or
                check['filtered_error_log_entries'] != 0 or not check['trace_completed'] or
                not check['encounter_unchanged_after_return'] or
                not check['movement_checks']['pause_resume']['no_drift'] or
                not check['movement_checks']['no_floor_rejection']['blocked_candidate_preserved_position']):
            raise ValueError(f"{check['serial']}: SWAMP runtime checks failed")
        check = dict(check)
        check['evidence_directory'] = f'build/{evidence_dir}'
        swamp_rows.append(check)
    (APP / 'swamp-current-apk-runtime-validation.json').write_text(
        json.dumps({'date': date.today().isoformat(), 'apk_sha256': built['sha256'],
                    'apk_bytes': built['bytes'], 'device_results': swamp_rows,
                    'fold7_tested': False}, indent=2, ensure_ascii=False) + '\n', encoding='utf-8')

    lifecycle_dirs = []
    for expected_page_size, candidates in (
            (16384, ('gameplay-lifecycle-api37-16k', 'gameplay-lifecycle-api37-16k-retry1')),
            (4096, ('gameplay-lifecycle-api37-4k',))):
        selected = next((name for name in candidates if
                         (BUILD / name / 'gameplay-activity-lifecycle-runtime-validation.json').is_file() and
                         read(BUILD / name / 'gameplay-activity-lifecycle-runtime-validation.json').get('result') == 'pass' and
                         read(BUILD / name / 'gameplay-activity-lifecycle-runtime-validation.json').get('device', {}).get('page_size_bytes') == expected_page_size),
                        None)
        if selected is None:
            raise FileNotFoundError(f'no lifecycle evidence found in {candidates!r}')
        lifecycle_dirs.append(selected)
    lifecycle_runs = [
        read(BUILD / path / 'gameplay-activity-lifecycle-runtime-validation.json')
        for path in lifecycle_dirs
    ]
    validate_pair(lifecycle_runs, lambda item: {
        'page_size_bytes': item['device']['page_size_bytes'],
        'android_release': item['device']['android_release'],
        'sdk': item['device']['api_level'], 'abi': item['device']['abi'],
    })
    lifecycle_rows = []
    for item, evidence_dir in zip(lifecycle_runs, lifecycle_dirs):
        if item['result'] != 'pass' or item['apk']['candidate_sha256'] != built['sha256']:
            raise ValueError(f"{item['device']['serial']}: lifecycle test did not pass on the current APK")
        if (not item['apk']['installed_matches_candidate'] or
                item['apk']['installed_sha256'] != built['sha256'] or
                not item['checkpoint']['home_resume_sentry_progress_unchanged'] or
                not item['checkpoint']['cold_relaunch_sentry_progress_unchanged'] or
                not item['checkpoint']['cold_relaunch_confirmed_restore'] or
                not item['lifecycle']['home_resume_completed'] or
                not item['lifecycle']['cold_relaunch_completed'] or
                item['lifecycle']['cold_relaunch_pid'] == item['lifecycle']['initial_cold_launch_pid'] or
                item['logcat_cleared']):
            raise ValueError(f"{item['device']['serial']}: lifecycle checkpoint/process checks failed")
        if any(row.get('fatal_error_count', 0) for row in item['app_pid_fatal_errors']):
            raise ValueError(f"{item['device']['serial']}: lifecycle app process logged fatal errors")
        checkpoint = item['checkpoint']
        lifecycle_rows.append({
            'serial': item['device']['serial'],
            'android_release': item['device']['android_release'],
            'api': item['device']['api_level'],
            'page_size_bytes': item['device']['page_size_bytes'],
            'abi': item['device']['abi'],
            'installed_apk_sha256': item['apk']['installed_sha256'],
            'home_resume_completed': item['lifecycle']['home_resume_completed'],
            'cold_relaunch_completed': item['lifecycle']['cold_relaunch_completed'],
            'cold_relaunch_used_new_pid': item['lifecycle']['cold_relaunch_pid'] !=
                item['lifecycle']['initial_cold_launch_pid'],
            'sentry_progress_unchanged': (
                checkpoint['home_resume_sentry_progress_unchanged'] and
                checkpoint['cold_relaunch_sentry_progress_unchanged']),
            'cold_relaunch_confirmed_restore': checkpoint['cold_relaunch_confirmed_restore'],
            'hp_at_initial_hud': int(checkpoint['initial_hud'].split('/', 1)[0].split()[-1]),
            'hp_after_home_resume_hud': int(checkpoint['after_home_resume_hud'].split('/', 1)[0].split()[-1]),
            'hp_after_cold_relaunch_hud': checkpoint['hp_after_cold_relaunch'],
            'app_pid_fatal_error_count': sum(row.get('fatal_error_count', 0)
                                             for row in item['app_pid_fatal_errors']),
            'logcat_cleared': item['logcat_cleared'],
            'evidence_directory': f'build/{evidence_dir}',
        })
    (APP / 'gameplay-activity-current-apk-runtime-validation.json').write_text(
        json.dumps({
            'date': date.today().isoformat(), 'apk_sha256': built['sha256'],
            'apk_bytes': built['bytes'], 'device_results': lifecycle_rows,
            'notes': [
                'The authored encounter sentries remain active while the player is idle; HP may decrease between HUD reads and is not a lifecycle invariant.',
                'The test verifies checkpoint restoration and sentry progress across Home/resume and a force-stop/cold relaunch. It does not claim original-game save compatibility or complete gameplay.',
            ],
            'fold7_tested': False,
        }, indent=2, ensure_ascii=False) + '\n', encoding='utf-8')
    print(json.dumps({'apk_sha256': built['sha256'],
                      'actor_page_sizes': [r['page_size_bytes'] for r in actor_rows],
                      'village_page_sizes': [r['page_size_bytes'] for r in village_rows],
                      'swamp_page_sizes': [r['page_size_bytes'] for r in swamp_rows],
                      'gameplay_lifecycle_page_sizes': [r['page_size_bytes'] for r in lifecycle_rows]}, indent=2))


if __name__ == '__main__':
    main()
