"""Freeze current APKs and compiler-recorded repository inputs before further edits.

This captures build provenance, not a gameplay or differential PASS. Dependencies
come from each ABI's actual Ninja dependency database and compile commands.
"""
import argparse
import hashlib
import json
import re
from pathlib import Path
import subprocess
import zipfile

REPO = Path(__file__).resolve().parents[3]


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--studio', type=Path,
                   help='Optional Android Studio project; omit only with --no-studio')
    p.add_argument('--no-studio', action='store_true',
                   help='Capture the packaged repository project only; make no Studio parity claim')
    p.add_argument('--ninja', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    if a.no_studio == bool(a.studio):
        p.error('pass exactly one of --studio PATH or --no-studio')
    assert not a.output.exists(), 'Refusing to replace an existing build capture'
    projects = {'packaged': REPO/'port/android-native'}
    if a.studio:
        projects['studio'] = a.studio
    sources, commands, entries, artifacts = {}, {}, {}, {}
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
                path = Path(name)
                path = (path if path.is_absolute() else database.parent/path).resolve()
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
            commands[tag][abi] = {'database_sha256': sha(database.read_bytes()),
                                  'ninja_dependencies_sha256': sha(deps.encode()),
                                  'repository_inputs': used}
            entries[f'compiler/{tag}-{abi}-commands.json'] = database.read_bytes()
            entries[f'compiler/{tag}-{abi}-dependencies.txt'] = deps.encode()
            # Preserve every CMake file from the actual configure graph,
            # including the sole Lua runtime and nested dependency libraries.
            rerun = next(line for line in (database.parent/'build.ninja').read_text().splitlines()
                         if line.startswith('build build.ninja: RERUN_CMAKE '))
            for token in re.findall(r'(?:\$[^\r\n]|[^\s])+', rerun.partition('RERUN_CMAKE ')[2]):
                decoded = re.sub(r'\$(.)', r'\1', token)
                if not decoded.endswith(('CMakeLists.txt','.cmake')):
                    continue
                path = Path(decoded)
                path = (path if path.is_absolute() else database.parent/path).resolve()
                if not path.is_relative_to(REPO):
                    continue
                key = path.relative_to(REPO).as_posix()
                if '/.cxx/' in key or '/build/' in key:
                    continue
                payload = path.read_bytes()
                assert key not in sources or sources[key] == sha(payload), 'Configure input changed during capture'
                sources[key] = sha(payload)
                entries['source/'+key] = payload
        for path in (project/'app/src/main').rglob('*'):
            if path.is_file() and 'assets' not in path.relative_to(project/'app/src/main').parts:
                key = path.relative_to(project/'app/src/main').as_posix()
                if any(part in {'build', '.cxx', '__pycache__'} for part in path.relative_to(project/'app/src/main').parts):
                    continue
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
    if a.studio:
        for abi in ('arm64-v8a', 'x86_64'):
            assert commands['packaged'][abi]['repository_inputs'] == commands['studio'][abi]['repository_inputs'], 'Repo/Studio compiler inputs differ'
    for folder in ('engine-resources', 'asset-payloads', 'engine-math', 'engine-animation', 'engine-skinning', 'engine-textures', 'engine-camera', 'game-data', 'level-world', 'physics-backend', 'scene-materials'):
        path = REPO/'port'/folder/'CMakeLists.txt'
        if path.is_file():
            key = path.relative_to(REPO).as_posix()
            sources[key] = sha(path.read_bytes())
            entries['source/'+key] = path.read_bytes()
    project = projects['packaged']
    for name in ('build.gradle.kts', 'settings.gradle.kts', 'gradle.properties', 'gradlew', 'gradlew.bat', 'app/build.gradle.kts', 'gradle/libs.versions.toml'):
        path = project/name
        entries['source/'+path.relative_to(REPO).as_posix()] = path.read_bytes()
        sources[path.relative_to(REPO).as_posix()] = sha(path.read_bytes())
    manifest = {'scope': __doc__, 'validation': 'BUILD_INPUTS_CAPTURED',
                'studio_parity_checked': bool(a.studio), 'apks': artifacts,
                'source_sha256': sources, 'compiler_inputs': commands,
                'entries': {name: {'sha256': sha(raw), 'bytes': len(raw)} for name, raw in entries.items()}}
    entries['build-capture.json'] = (json.dumps(manifest, indent=2)+'\n').encode()
    a.output.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(a.output, 'w', zipfile.ZIP_DEFLATED) as z:
        for name, raw in sorted(entries.items()):
            z.writestr(name, raw)
    with zipfile.ZipFile(a.output) as z:
        assert z.testzip() is None
    print(json.dumps({'capture': str(a.output.resolve()), 'sha256': sha(a.output.read_bytes()),
                      'source_inputs': len(sources), 'apks': artifacts}))


if __name__ == '__main__':
    main()
