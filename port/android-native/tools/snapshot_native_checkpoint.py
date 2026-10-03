"""Preserve a verified native checkpoint's source and bundled assets locally.

Audited source/asset entries must match their saved validation hashes. Extra
Gradle and resource/math build dependencies are identified separately; they
are captured build inputs, not additional original-instruction parity claims.
Never includes the original ARM32 engine or local tool/runtime installations.
"""
import argparse
import hashlib
import json
from pathlib import Path
import zipfile

REPO = Path(__file__).resolve().parents[3]


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--validation', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--build-capture', type=Path, help='Use frozen compiler inputs when later worktree edits have begun')
    args = parser.parse_args()
    report = json.loads(args.validation.read_text(encoding='utf-8-sig'))
    assert report['validation'] == 'PASS'
    checkpoint = Path(report['checkpoint']['path'])
    assert digest(checkpoint.read_bytes()) == report['checkpoint']['sha256']
    entries = {}
    records = {}
    captured = {}
    if args.build_capture:
        expected = report.get('build_capture') or report['compiler_capture']
        assert digest(args.build_capture.read_bytes()) == expected['sha256']
        with zipfile.ZipFile(args.build_capture) as archive:
            captured = {name.removeprefix('source/'): archive.read(name)
                        for name in archive.namelist() if name.startswith('source/')}

    def add(path, scope, expected=None):
        path = path.resolve()
        assert path.is_relative_to(REPO), path
        name = path.relative_to(REPO).as_posix()
        raw = captured.get(name)
        if raw is None:
            raw = path.read_bytes()
        actual = digest(raw)
        assert expected is None or actual == expected, ('Source changed', name)
        entries[name] = raw
        records[name] = {'sha256': actual, 'bytes': len(raw), 'scope': scope}

    for name, expected in report['source_sha256'].items():
        add(REPO/name, 'Frozen checkpoint compiler input; algorithm parity is bounded by the validation scope' if captured else 'Audited checkpoint source', expected)
    add(args.validation, 'Saved checkpoint validation')
    # These are additional compile inputs named by scene-materials CMake.
    for directory in ('engine-resources', 'asset-payloads', 'engine-math'):
        for path in (REPO/'port'/directory).iterdir():
            if path.is_file() and path.suffix in ('.cpp', '.hpp', '.h'):
                add(path, 'Captured resource/math build dependency')
    project = REPO/'port/android-native'
    for name in ('build.gradle.kts', 'settings.gradle.kts', 'gradle.properties',
                 'gradlew', 'gradlew.bat', '.gitignore', 'README.md', 'ROADMAP.md'):
        add(project/name, 'Captured build setup/documentation')
    for path in (project/'gradle').rglob('*'):
        if path.is_file():
            add(path, 'Captured Gradle build setup')
    # Preserve exact original authored assets from the verified APK itself.
    assets = report['assets_verified']
    if 'packaged' in assets:
        assets = assets['packaged']
    with zipfile.ZipFile(checkpoint) as apk:
        for name, expected in assets.items():
            raw = apk.read('assets/'+name)
            assert digest(raw) == expected['sha256'] and len(raw) == expected['bytes']
            target = 'port/android-native/app/src/main/assets/'+name
            entries[target] = raw
            records[target] = {**expected, 'scope': 'Verified bundled APK asset'}
    manifest = {'checkpoint_sha256': report['checkpoint']['sha256'],
                'validation_sha256': digest(args.validation.read_bytes()),
                'entries': records,
                'scope': 'Local source/asset snapshot. External Android SDK/NDK/JDK/Gradle dependency caches and original oracle are not bundled.'}
    assert not args.output.exists(), 'Refusing to replace an existing snapshot'
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(args.output, 'w', zipfile.ZIP_DEFLATED) as archive:
        for name, raw in sorted(entries.items()):
            archive.writestr(name, raw)
        archive.writestr('checkpoint-source-manifest.json', json.dumps(manifest, indent=2)+'\n')
    with zipfile.ZipFile(args.output) as archive:
        assert archive.testzip() is None
    print(json.dumps({'snapshot': str(args.output.resolve()), 'entries': len(entries),
                      'bytes': args.output.stat().st_size, 'sha256': digest(args.output.read_bytes())}))


if __name__ == '__main__':
    main()
