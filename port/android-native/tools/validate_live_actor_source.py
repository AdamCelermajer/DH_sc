"""Bind current native actor source, APK assets, kernel audits and emulator proof.

Historical checkpoint reports remain evidence for their saved artifacts only.
This validation covers the Crypt locomotion integration, not the complete game.
"""
import argparse
import hashlib
import json
import struct
import zipfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
ROOT = REPO / 'port/android-native'
LOCAL = REPO / '.local-inputs'
REPORTS = REPO / 'port/level-world/reports'


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def read(path):
    return json.loads(path.read_text(encoding='utf-8-sig'))


def build_log(path):
    raw = path.read_bytes()
    return raw.decode('utf-16') if raw[:2] in (b'\xff\xfe', b'\xfe\xff') else raw.decode('utf-8-sig')


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--studio', type=Path, required=True)
    parser.add_argument('--smoke', type=Path, required=True)
    parser.add_argument('--lifecycle', type=Path, required=True)
    parser.add_argument('--cache', type=Path, default=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'))
    args = parser.parse_args()
    paths = {'packaged': ROOT / 'app/build/outputs/apk/debug/app-debug.apk',
             'studio': args.studio / 'app/build/outputs/apk/debug/app-debug.apk'}
    hashes = {tag: sha(path.read_bytes()) for tag, path in paths.items()}
    checkpoint = ROOT / f"build/checkpoints/dh2-native-actor-{hashes['packaged'][:8]}.apk"
    assert checkpoint.is_file() and sha(checkpoint.read_bytes()) == hashes['packaged']
    kernels = read(REPORTS / 'live-actor-kernel-validation.json')
    assert kernels['apk_sha256'] == hashes, 'Kernel audits must bind current APKs'
    smoke = read(args.smoke)
    assert smoke['apk_sha256'] == smoke['installed_apk_sha256'] == hashes['packaged']
    assert smoke['validation'] == 'PASS'
    lifecycle = read(args.lifecycle)
    assert lifecycle['validation'] == 'PASS'
    assert lifecycle['apk_sha256'] == lifecycle['installed_apk_sha256'] == hashes['packaged']
    prior = read(REPORTS / 'physical-movement-source-validation.json')
    assets = {}
    libraries = {}
    with zipfile.ZipFile(paths['packaged']) as packaged, zipfile.ZipFile(paths['studio']) as studio:
        for tag, archive in (('packaged', packaged), ('studio', studio)):
            assert set(archive.namelist()) == set(packaged.namelist())
            rows = []
            for name in archive.namelist():
                if not name.startswith('lib/') or not name.endswith('.so'):
                    continue
                raw = archive.read(name)
                assert name.split('/')[1] in ('arm64-v8a', 'x86_64')
                assert 'DungeonHunter2' not in name and raw[:5] == b'\x7fELF\x02'
                offset = struct.unpack_from('<Q', raw, 32)[0]
                size, count = struct.unpack_from('<HH', raw, 54)
                aligns = [struct.unpack_from('<Q', raw, offset+i*size+48)[0]
                          for i in range(count) if struct.unpack_from('<I', raw, offset+i*size)[0] == 1]
                assert aligns and min(aligns) >= 16384, name
                rows.append({'path': name, 'sha256': sha(raw), 'minimum_load_alignment': min(aligns)})
            assert {row['path'].split('/')[1] for row in rows} == {'arm64-v8a', 'x86_64'}
            libraries[tag] = rows
        for name in packaged.namelist():
            if not name.startswith('assets/') or name.endswith('/'):
                continue
            key = name[7:]
            raw = packaged.read(name)
            assert raw == studio.read(name) == (ROOT / 'app/src/main/assets' / key).read_bytes()
            assert raw == (args.studio / 'app/src/main/assets' / key).read_bytes()
            assets[key] = {'bytes': len(raw), 'sha256': sha(raw)}
        for key, record in prior['assets_verified'].items():
            assert assets[key] == record, ('Earlier authored asset changed', key)
        locomotion = read(ROOT / 'app/src/main/assets/player-locomotion-provenance.json')
        assert locomotion['character_row'] == 263 and locomotion['animation_table'] == 48
        assert locomotion['stance_mask'] == 210 and locomotion['stance_count'] == 5
        assert len(locomotion['clips']) == 9
        for clip in locomotion['clips']:
            assert assets[clip['asset']] == {'bytes': clip['bytes'], 'sha256': clip['sha256']}
        additions = set(assets) - set(prior['assets_verified'])
        assert additions == ({c['asset'] for c in locomotion['clips']} - set(prior['assets_verified'])) | {'player-locomotion-provenance.json'}
    with args.cache.open('rb') as stream:
        assert hashlib.file_digest(stream, 'sha256').hexdigest() == locomotion['cache_sha256']
    with zipfile.ZipFile(args.cache) as cache:
        for row in locomotion['inputs']:
            matches = [entry for entry in cache.infolist() if entry.filename == row['entry']]
            assert len(matches) == 1, ('Missing or duplicate original input', row['entry'])
            raw = cache.read(matches[0])
            assert len(raw) == row['bytes'] and sha(raw) == row['sha256']
    sources = {}
    for stem in ('actor_playback', 'actor_runtime', 'actor_rotation', 'decor_scene', 'character_scene', 'visual_motion'):
        for ext in ('cpp', 'hpp'):
            path = REPO / f'port/level-world/{stem}.{ext}'
            sources[path.relative_to(REPO).as_posix()] = sha(path.read_bytes())
    for filename in ('model_renderer.cpp', 'native_app.cpp'):
        path = ROOT / 'app/src/main/cpp' / filename
        assert path.read_bytes() == (args.studio / 'app/src/main/cpp' / filename).read_bytes()
        sources[path.relative_to(REPO).as_posix()] = sha(path.read_bytes())
    renderer = (ROOT / 'app/src/main/cpp/model_renderer.cpp').read_text()
    assert 'move_x*420*dt' not in renderer
    for tag in ('repo', 'studio'):
        assert 'BUILD SUCCESSFUL' in build_log(LOCAL / f'live-actor-final-{tag}-build.log')
        assert 'Verification successful' in build_log(LOCAL / f'live-actor-{tag}-zipalign.log')
    hosts = {}
    for name in ('actor-playback', 'actor-runtime', 'actor-rotation', 'decor-scene', 'character-scene'):
        hosts[name] = read(REPORTS / f'{name}-host-audit.json')
        audit = hosts[name].get('host_audit', hosts[name])
        assert audit['mismatches'] == 0
    for path, expected in hosts['actor-playback']['source_sha256'].items():
        assert sha((REPO / path).read_bytes()) == expected, ('Playback host source changed', path)
    gold = REPO / 'port/level-world/reference/actor-playback/composition-fixtures.bin'
    assert hosts['actor-playback']['original_instruction_evidence']['corpus_sha256'] == sha(gold.read_bytes())
    report = {'validation': 'PASS', 'apk_sha256': hashes, 'libraries': libraries,
              'checkpoint_apk': str(checkpoint.resolve()), 'checkpoint_bytes': checkpoint.stat().st_size,
              'assets_verified': assets, 'source_sha256': sources, 'host_audits': hosts,
              'packaged_kernel_validation': kernels, 'emulator': smoke, 'lifecycle': lifecycle,
              'original_cache_sha256': locomotion['cache_sha256'],
              'preserved_diagnostics': ['.local-inputs/world-tests-live-actor/live-actor-smoke.json',
                  '.local-inputs/world-tests-live-actor-v2/live-actor-smoke.json',
                  '.local-inputs/world-tests-live-actor-lifecycle/live-actor-lifecycle-smoke.json'],
              'live_pipeline': 'authored scene/root displacement -> genuine world Step -> animator -> source path/rotation/subobjects -> target cache',
              'native_body_count': 83, 'authored_decor_colliders': 82,
              'physical_arm64_tested': False, 'original_gpu_parity_verified': False,
              'full_game_playable': False, 'goal_status': 'active',
              'remaining_boundaries': ['full character FSM and equipment/save producers',
                  'original input, camera and obstacle avoidance', 'full combat animation order and enemy pursuit',
                  'full levels, lighting, audio, UI and saves', 'all game assets and physical ARM64 verification']}
    (REPORTS / 'live-actor-source-validation.json').write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps({'validation': 'PASS', 'assets': len(assets), 'native_libraries': len(libraries['packaged']),
                      'bodies': 83, 'full_game_playable': False}))


if __name__ == '__main__':
    main()
