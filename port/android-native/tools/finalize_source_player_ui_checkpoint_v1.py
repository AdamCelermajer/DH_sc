"""Finalize packaging plus existing player/HUD regression, not full menus/gameplay."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
PROJECT = ROOT / 'port/android-native'


def sha(path):
    digest = hashlib.sha256()
    with path.open('rb') as source:
        while block := source.read(1024 * 1024):
            digest.update(block)
    return digest.hexdigest()


def main():
    capture_path = PROJECT / 'reports/native-source-player-ui-136e924a-build-capture.json'
    live_path = PROJECT / 'reports/native-source-player-ui-136e924a-live-v1/connected-player-hud-smoke.json'
    library_path = PROJECT / 'reports/native-source-player-ui-library-build-v1.json'
    output = PROJECT / 'reports/native-source-player-ui-136e924a-checkpoint-validation.json'
    assert not output.exists(), 'Preserve accepted checkpoint validation'
    capture = json.loads(capture_path.read_text())
    live = json.loads(live_path.read_text())
    library = json.loads(library_path.read_text())
    assert capture['validation'] == 'BUILD_INPUTS_CAPTURED'
    assert live['validation'] == library['validation'] == 'PASS'
    checkpoint = Path(capture['checkpoint']['path'])
    digest = sha(checkpoint)
    assert digest == capture['checkpoint']['sha256'] == live['apk_sha256'] == live['installed_apk_sha256']
    assert capture['main_host_report']['suites'] == library['main_host_suites'] == 138
    host_path = ROOT / capture['main_host_report']['path']
    assert sha(host_path) == capture['main_host_report']['sha256'] == library['main_host_report_sha256']
    host = json.loads(host_path.read_text())
    assert host['validation'] == 'PASS' and host['sanitizer_findings'] == 0
    assert len(host['host_audits']) == 138
    assert set(live['cases']) == {'default_scene', 'touch_movement', 'authored_damage', 'context_resume', 'developer_drawer'}
    assert len(live['full_cache_native_probes']) == 6
    for name, value in capture['source_sha256'].items():
        assert sha(ROOT / name) == value, ('Captured source drift', name)
    for name, value in library['source_sha256'].items():
        assert sha(ROOT / name) == value, ('Inspected source drift', name)
    for abi, result in library['android_abis'].items():
        for metadata in result['libraries'].values():
            assert sha(ROOT / metadata['path']) == metadata['sha256']
    stripped = PROJECT / 'app/build/intermediates/stripped_native_libs/debug/stripDebugDebugSymbols/out'
    for name, value in capture['libraries'].items():
        assert sha(stripped / name) == value['sha256'], ('Packaged library differs from stripped build', name)
    assert len(capture['libraries']) == len(live['libraries']) == 18
    for value in live['libraries']:
        assert capture['libraries'][value['path']]['sha256'] == value['sha256']
    snapshot = Path(capture['source_snapshot']['path'])
    assert sha(snapshot) == capture['source_snapshot']['sha256']
    receipts = {path.relative_to(ROOT).as_posix(): sha(path) for path in (capture_path, live_path, library_path, host_path)}
    result = dict(validation='PASS',
        scope='Whole original UI frame/input/session, authoritative inventory and retained player-skill systems are built in native libraries and packaged. Existing player/status scene passes emulator regression. Full Android menu and skill gameplay connections are not established.',
        checkpoint=capture['checkpoint'], source_snapshot=capture['source_snapshot'],
        source_sha256=capture['source_sha256'], compiler_inputs=capture['compiler_inputs'],
        assets_verified=capture['assets_verified'], libraries=capture['libraries'],
        receipts_sha256=receipts, main_host_suites=138, sanitizer_findings=0,
        emulator_cases=live['cases'], native_cache_probes=live['full_cache_native_probes'],
        one_APK=True, bundled_cache_files=6833, external_cache_install=False,
        ARM32_engine_bundled=False, source_UI_frame_input_engine=True,
        owned_inventory_and_player_skill_engine=True,
        live_new_menus=False, live_full_inventory_or_skill_gameplay=False,
        retained_source_edit_text_connection=False, full_enemy_AI=False,
        audio_and_campaign_saves_complete=False, full_game_verified=False,
        physical_arm64_verified=False)
    output.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(dict(validation='PASS', checkpoint=str(checkpoint), sha256=digest,
        main_host_suites=138, emulator_cases=list(live['cases']), output=str(output))))


if __name__ == '__main__':
    main()
