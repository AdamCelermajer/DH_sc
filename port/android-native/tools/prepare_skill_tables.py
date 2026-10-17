"""Bundle byte-pinned original Skill/Faery tables from the extracted cache."""
import argparse
import hashlib
import json
from pathlib import Path

INPUTS = {
    'skills_pyarray.bin': 'e336986d5aee78fd1aed7fe7ec43cc8d58fa03d4fd27216779c0e96e47acc2d6',
    'skills_pyarraynames.bin': '31642dc0d8bf11f1ebd13698bbefda4fda0aefa2c727fb4ae9449f2df073fb9f',
    'skills_pystructnames.bin': 'fd882501b0b7af0191272e6802558d1e1ac4031f6237cf5e2c767ebae946f4b9',
    'faeries_pyarray.bin': '52fc30b0070f7ef2ed559bcf610997ac219342c2f6aabd461566c034d75aab9a',
    'faeries_pyarraynames.bin': 'ef422b17f75405685d3447b5717610a070380aecf744a3f861edb8c3948fdcff',
    'faeries_pystructnames.bin': 'f2f2c5dd7fb58074ddf4733a82bc02eb789463399072beb9c8877483b4828b7b',
    'faeries_pycst.bin': '559a9e44bfa3d1c6cbb9b234523a893d241f6021f464e86b6589da7c0d4930d5',
}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cache', type=Path, help='extracted cache/files root')
    parser.add_argument('--project', type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    pending, inputs = {}, {}
    for name, digest in INPUTS.items():
        raw = (args.cache / 'data/pydata' / name).read_bytes()
        assert hashlib.sha256(raw).hexdigest() == digest, name
        pending[name] = raw
        inputs[name] = {'bytes': len(raw), 'sha256': digest}
    assets = args.project / 'app/src/main/assets/data'
    assets.mkdir(parents=True, exist_ok=True)
    for name, raw in pending.items():
        (assets / name).write_bytes(raw)
    report = args.project / 'app/build/skill-tables-data-provenance.json'
    report.parent.mkdir(parents=True, exist_ok=True)
    report.write_text(json.dumps({'validation': 'PASS', 'unchanged_original_data': True,
                                  'inputs': inputs}, indent=2) + '\n')
    print(json.dumps({'validation': 'PASS', 'original_files': len(inputs)}))


if __name__ == '__main__':
    main()
