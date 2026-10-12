"""Restore omitted build inputs in a NEW checkpoint source archive.

The existing archive and APK remain immutable. The recovered CMake bytes must
match the supplemental source hash in the checkpoint's saved timer proof.
Other missing files come only from the hash-bound original compiler capture.
This repairs source packaging; it is not a new gameplay/build parity claim.
"""
import argparse
import hashlib
import json
from pathlib import Path
import zipfile


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--snapshot', type=Path, required=True)
    p.add_argument('--validation', type=Path, required=True)
    p.add_argument('--cmake', type=Path, required=True)
    p.add_argument('--build-capture', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    assert not a.output.exists(), 'Refusing to replace an existing snapshot'
    report = json.loads(a.validation.read_text())
    assert report['validation'] == 'PASS'
    capture_binding = report.get('build_capture') or report['compiler_capture']
    assert sha(a.build_capture.read_bytes()) == capture_binding['sha256']
    with zipfile.ZipFile(a.build_capture) as z:
        capture = json.loads(z.read('build-capture.json'))
        captured = {}
        for name in z.namelist():
            if name.startswith('source/'):
                value = z.read(name)
                assert sha(value) == capture['entries'][name]['sha256']
                captured[name.removeprefix('source/')] = value
    key = 'port/script-runtime/CMakeLists.txt'
    expected = report['script_timer_source_bindings']['supplemental_current_source_sha256'][key]
    raw = a.cmake.read_bytes()
    assert sha(raw) == expected, 'Recovered build setup differs from saved proof'
    with zipfile.ZipFile(a.snapshot) as z:
        assert z.testzip() is None
        assert key not in z.namelist(), 'This snapshot already has the setup'
        entries = {name: z.read(name) for name in z.namelist()}
    manifest_key = 'checkpoint-source-manifest.json'
    manifest = json.loads(entries[manifest_key])
    assert manifest['checkpoint_sha256'] == report['checkpoint']['sha256']
    assert manifest['validation_sha256'] == sha(a.validation.read_bytes())
    for name, record in manifest['entries'].items():
        assert sha(entries[name]) == record['sha256'] and len(entries[name]) == record['bytes']
    entries[key] = raw
    manifest['entries'][key] = {'sha256': expected, 'bytes': len(raw),
                               'scope': 'Exact checkpoint build setup bound by saved timer proof'}
    added = [key]
    for name, value in captured.items():
        if name in entries:
            # Documentation added separately by the original snapshot isn't
            # compiler input; captured source must nevertheless remain exact.
            assert entries[name] == value, ('Frozen input differs', name)
            continue
        assert not name.startswith(('/', '\\')) and ':' not in name and '..' not in Path(name).parts
        entries[name] = value
        manifest['entries'][name] = {'sha256': sha(value), 'bytes': len(value),
                                   'scope': 'Exact omitted build input from hash-bound compiler capture'}
        added.append(name)
    manifest['source_archive_repair'] = {'scope': __doc__,
                                       'previous_snapshot_sha256': sha(a.snapshot.read_bytes()),
                                       'build_capture_sha256': sha(a.build_capture.read_bytes()),
                                       'added': added}
    entries[manifest_key] = (json.dumps(manifest, indent=2)+'\n').encode()
    with zipfile.ZipFile(a.output, 'w', zipfile.ZIP_DEFLATED) as z:
        for name, data in sorted(entries.items()):
            z.writestr(name, data)
    with zipfile.ZipFile(a.output) as z:
        assert z.testzip() is None and z.read(key) == raw
    print(json.dumps({'validation': 'PASS', 'snapshot': str(a.output.resolve()),
                      'entries': len(entries), 'bytes': a.output.stat().st_size,
                      'sha256': sha(a.output.read_bytes()), 'apk_changed': False}))


if __name__ == '__main__':
    main()
