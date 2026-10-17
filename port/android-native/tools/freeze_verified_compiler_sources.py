"""Preserve the exact repository inputs of a verified native APK.

This archive records compiled source provenance. It does not imply original
instruction parity, complete gameplay, or a standalone build without the
external SDK/toolchain/dependency caches and bundled assets.
"""
import argparse
import hashlib
import json
from pathlib import Path
import zipfile

ROOT = Path(__file__).resolve().parents[3]


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--production-snapshot', type=Path, required=True)
    parser.add_argument('--artifact-validation', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    production = json.loads(args.production_snapshot.read_bytes())
    validation = json.loads(args.artifact_validation.read_bytes())
    assert production['validation'] == validation['validation'] == 'PASS'
    assert production['apk_sha256'] == validation['apk_sha256']
    assert production['source_sha256'] == validation['source_sha256']
    assert production['all_actual_repository_compiler_inputs_unchanged_before_and_after_build']
    assert not args.output.exists(), 'Refusing to replace an existing source archive'
    payloads = {}
    for name, expected in production['source_sha256'].items():
        path = (ROOT/name).resolve()
        assert path.is_relative_to(ROOT), name
        raw = path.read_bytes()
        assert digest(raw) == expected, ('Compiled source changed', name)
        payloads[name] = raw
    manifest = {
        'apk_sha256': production['apk_sha256'],
        'production_snapshot_sha256': digest(args.production_snapshot.read_bytes()),
        'artifact_validation_sha256': digest(args.artifact_validation.read_bytes()),
        'scope': __doc__,
        'entries': {name: {'sha256': digest(raw), 'bytes': len(raw)}
                    for name, raw in sorted(payloads.items())},
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(args.output, 'w', zipfile.ZIP_DEFLATED) as archive:
        for name, raw in sorted(payloads.items()):
            archive.writestr(name, raw)
        archive.writestr('compiled-source-manifest.json', json.dumps(manifest, indent=2)+'\n')
    with zipfile.ZipFile(args.output) as archive:
        assert archive.testzip() is None
        for name, raw in payloads.items():
            assert archive.read(name) == raw
    print(json.dumps({'validation': 'PASS', 'inputs': len(payloads),
                      'bytes': args.output.stat().st_size,
                      'sha256': digest(args.output.read_bytes()),
                      'path': str(args.output.resolve())}))


if __name__ == '__main__':
    main()
