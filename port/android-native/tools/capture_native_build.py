"""Freeze current APKs and compiler-recorded repository inputs before further edits.

This captures build provenance, not a gameplay or differential PASS. Dependencies
come from each ABI's actual Ninja dependency database and compile commands.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import zipfile

REPO = Path(__file__).resolve().parents[3]


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--studio', type=Path, required=True)
    p.add_argument('--ninja', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--single-project', action='store_true',
                   help='Capture only the packaged checkout; do not duplicate the same APK as a Studio artifact')
    p.add_argument('--store-apks', action='store_true',
                   help='Store already compressed APK bytes directly to avoid recompressing them')
    p.add_argument('--ui-assets-stage', type=Path, nargs=3, metavar=('MENU', 'TEXT', 'TEXTURE'),
                   help='Capture the three original UI staging receipts and UI build definitions')
    p.add_argument('--ui-font-freeze', type=Path,
                   help='Dedicated additive HUD-font freeze; canonical path used when those modules compiled')
    p.add_argument('--connected-hud-stage',type=Path,
                   help='Versioned world/status connection and exact renderer getter migration receipt')
    a = p.parse_args()
    assert not a.output.exists(), 'Refusing to replace an existing build capture'
    projects = {'packaged': REPO/'port/android-native'}
    if not a.single_project:
        projects['studio'] = a.studio
    sources, commands, entries, artifacts = {}, {}, {}, {}
    ui_receipts = []
    font_freeze = None
    assert not a.ui_font_freeze or a.ui_assets_stage, 'Font freeze requires --ui-assets-stage'
    if a.ui_assets_stage:
        for label, path, count in zip(('menu','text','texture'), a.ui_assets_stage, (44,337,6)):
            raw = path.read_bytes()
            receipt = json.loads(raw)
            assert receipt['validation'] == 'PASS' and receipt['resource_count'] == count
            assert receipt['unmodified_original_bytes']
            archive = 'provenance/ui-assets/'+label+'-stage.json'
            entries[archive] = raw
            ui_receipts.append(dict(stage=label, path=str(path.resolve()), archive=archive,
                                    sha256=sha(raw), resource_count=count))
    for tag, project in projects.items():
        apk = project/'app/build/outputs/apk/debug/app-debug.apk'
        raw = apk.read_bytes()
        artifacts[tag] = {'path': str(apk), 'sha256': sha(raw), 'bytes': len(raw)}
        entries[tag+'-app-debug.apk'] = raw
        commands[tag] = {}
        for abi in ('arm64-v8a', 'x86_64'):
            databases = list((project/'app/.cxx/Debug').glob('*/'+abi+'/compile_commands.json'))
            assert len(databases) == 1, ('Ambiguous compiler database', databases)
            database = databases[0]
            rows = json.loads(database.read_bytes())
            deps = subprocess.run([str(a.ninja), '-C', str(database.parent), '-t', 'deps'],
                                  capture_output=True, text=True, check=True).stdout
            candidates = sorted(set([row['file'] for row in rows] + [line.strip() for line in deps.splitlines() if line.startswith('    ')]))
            used = {}
            for name in candidates:
                path = Path(name).resolve()
                if not path.is_relative_to(REPO):
                    # Studio's app copy is checked against the repository copy.
                    main = (project/'app/src/main').resolve()
                    if not path.is_relative_to(main):
                        continue
                    path = REPO/'port/android-native/app/src/main'/path.relative_to(main)
                    assert path.read_bytes() == Path(name).read_bytes(), 'Studio source differs'
                assert path.is_file(), ('Missing compiler input', path)
                key = path.relative_to(REPO).as_posix()
                payload = path.read_bytes()
                value = sha(payload)
                assert key not in sources or sources[key] == value, 'Source changed during capture'
                sources[key] = value
                used[key] = value
                entries['source/'+key] = payload
            assert any(key.endswith('character_timers.cpp') for key in used), 'Timers missing from compiler inputs'
            assert any(key.endswith('animation_blend.cpp') for key in used), 'Typed blends missing from compiler inputs'
            if ui_receipts:
                assert 'port/engine-ui/swf_movie.cpp' in used, 'Native UI missing from compiler inputs'
                assert any(key.startswith('port/engine-ui/vendor/gameswf1714/') for key in used)
                assert any(key.startswith('port/engine-ui/vendor/freetype-2.3.7-hud/') for key in used)
            commands[tag][abi] = {'database_sha256': sha(database.read_bytes()),
                                  'ninja_dependencies_sha256': sha(deps.encode()),
                                  'repository_inputs': used}
            entries[f'compiler/{tag}-{abi}-commands.json'] = database.read_bytes()
            entries[f'compiler/{tag}-{abi}-dependencies.txt'] = deps.encode()
        for path in (project/'app/src/main').rglob('*'):
            if path.is_file() and 'assets' not in path.relative_to(project/'app/src/main').parts:
                key = path.relative_to(project/'app/src/main').as_posix()
                if key == 'keepRules/rules.keep':
                    assert all(not line.strip() or line.strip().startswith('#') for line in path.read_text().splitlines())
                    continue
                original = REPO/'port/android-native/app/src/main'/key
                payload = path.read_bytes()
                assert original.read_bytes() == payload
                key = original.relative_to(REPO).as_posix()
                sources[key] = sha(payload)
                entries['source/'+key] = payload
    assert set(commands['packaged']['arm64-v8a']['repository_inputs']) == set(commands['packaged']['x86_64']['repository_inputs'])
    if 'studio' in commands:
        for abi in ('arm64-v8a', 'x86_64'):
            assert commands['packaged'][abi]['repository_inputs'] == commands['studio'][abi]['repository_inputs'], 'Repo/Studio compiler inputs differ'
    for folder in ('engine-resources', 'asset-payloads', 'engine-math', 'engine-animation', 'engine-skinning', 'engine-textures', 'game-data', 'level-world', 'physics-backend', 'scene-materials', 'script-runtime'):
        path = REPO/'port'/folder/'CMakeLists.txt'
        if path.is_file():
            key = path.relative_to(REPO).as_posix()
            sources[key] = sha(path.read_bytes())
            entries['source/'+key] = path.read_bytes()
    if ui_receipts:
        definitions=('CMakeLists.txt','gameswf_sources.cmake','freetype237-hud.cmake')
        if a.connected_hud_stage:definitions+=('gameswf_font_overlay_v1.cmake',)
        for name in definitions:
            path = REPO/'port/engine-ui'/name
            key = path.relative_to(REPO).as_posix()
            payload = path.read_bytes()
            assert key not in sources or sources[key] == sha(payload), 'UI source changed during capture'
            sources[key] = sha(payload)
            entries['source/'+key] = payload
        additive = ['port/engine-ui/'+name+suffix for name in
                    ('freetype_bitmap_alpha','hud_freetype_font','swf_hud_freetype_provider')
                    for suffix in ('.cpp','.hpp')]
        if any(key in sources for key in additive):
            path = a.ui_font_freeze or REPO/'port/engine-ui/reference/hud-freetype-font/freeze-manifest.json'
            raw = path.read_bytes()
            frozen = json.loads(raw)
            assert frozen['validation'] == 'PASS'
            mapping = frozen.get('source_sha256', frozen.get('source_and_evidence_sha256'))
            assert mapping, 'Missing additive HUD-font freeze mapping'
            for key in additive:
                assert mapping[key] == sources[key]
                assert all(key in commands[tag][abi]['repository_inputs']
                           for tag in projects for abi in ('arm64-v8a','x86_64'))
            archive = 'provenance/ui-font/freeze-manifest.json'
            entries[archive] = raw
            font_freeze = dict(path=str(path.resolve()),archive=archive,sha256=sha(raw))
            proofs = frozen['proof_sha256']
            expected = {'port/engine-ui/reports/'+name for name in
                        ('hud-freetype-font-host-audit.json','swf-hud-freetype-provider-host-audit.json',
                         'freetype-bitmap-alpha-arm64-differential.json')}
            assert expected <= proofs.keys(), 'Incomplete additive HUD-font proof freeze'
            font_freeze['proofs'] = {}
            for key in sorted(expected):
                proof = (REPO/key).read_bytes()
                assert sha(proof) == proofs[key] and json.loads(proof)['validation'] == 'PASS'
                entry = 'provenance/ui-font/proofs/'+key
                entries[entry] = proof
                font_freeze['proofs'][key] = dict(archive=entry,sha256=sha(proof))
        else:
            assert not a.ui_font_freeze, 'Font freeze supplied but additive modules did not compile'
    project = projects['packaged']
    for name in ('build.gradle.kts', 'settings.gradle.kts', 'gradle.properties', 'gradlew', 'gradlew.bat', 'app/build.gradle.kts'):
        path = project/name
        key = path.relative_to(REPO).as_posix()
        payload = path.read_bytes()
        sources[key] = sha(payload)
        entries['source/'+key] = payload
    manifest = {'scope': __doc__, 'validation': 'BUILD_INPUTS_CAPTURED', 'apks': artifacts,
                'source_sha256': sources, 'compiler_inputs': commands,
                'entries': {name: {'sha256': sha(raw), 'bytes': len(raw)} for name, raw in entries.items()}}
    if a.connected_hud_stage:
        assert ui_receipts,'Connected HUD stage requires original UI receipts'
        raw=a.connected_hud_stage.read_bytes();stage=json.loads(raw)
        assert stage['validation']=='PASS' and stage['renderer_migration']['exact_additive_getter_verified']
        for key,digest in stage['source_sha256'].items():
            assert sources.get(key)==digest,('Connected HUD input missing or changed',key)
        entry='provenance/connected-hud/stage.json';entries[entry]=raw
        manifest['connected_hud_stage']=dict(path=str(a.connected_hud_stage.resolve()),archive=entry,sha256=sha(raw))
        manifest['entries'][entry]=dict(sha256=sha(raw),bytes=len(raw))
        for label in ('host','core_stage'):
            proof=(REPO/stage[label+'_path']).read_bytes()
            assert sha(proof)==stage[label+'_sha256'],('Connected proof changed',label)
            entry='provenance/connected-hud/'+label+'.json';entries[entry]=proof
            manifest['entries'][entry]=dict(sha256=sha(proof),bytes=len(proof))
    if ui_receipts:
        manifest['ui_assets_stages'] = ui_receipts
        manifest['native_UI_scope'] = 'Compiler-input/resource capture only; no live HUD/GPU acceptance inferred'
        if font_freeze:
            manifest['ui_font_freeze'] = font_freeze
    entries['build-capture.json'] = (json.dumps(manifest, indent=2)+'\n').encode()
    a.output.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(a.output, 'w', zipfile.ZIP_DEFLATED) as z:
        for name, raw in sorted(entries.items()):
            z.writestr(name, raw, compress_type=zipfile.ZIP_STORED if a.store_apks and name.endswith('.apk') else zipfile.ZIP_DEFLATED)
    with zipfile.ZipFile(a.output) as z:
        assert z.testzip() is None
    print(json.dumps({'capture': str(a.output.resolve()), 'sha256': sha(a.output.read_bytes()),
                      'source_inputs': len(sources), 'apks': artifacts}))


if __name__ == '__main__':
    main()
