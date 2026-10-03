"""Bind authored Player/Basic script-selection inputs to the supplied cache."""
import argparse
import hashlib
import json
import struct
import zipfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def strings(raw):
    at = 4
    out = []
    for _ in range(struct.unpack_from('<I', raw)[0]):
        length = struct.unpack_from('<I', raw, at)[0]
        at += 4
        out.append(raw[at:at+length].decode('ascii'))
        at += length
    assert at == len(raw)
    return out


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--cache', type=Path, default=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'))
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.output.exists():
        raise RuntimeError('Refusing to overwrite authored script input evidence')
    assert sha(args.cache) == '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
    data = REPO / 'port/android-native/app/src/main/assets/data'
    names = strings((data / 'ai_pyarraynames.bin').read_bytes())
    fields = strings((data / 'ai_pystructnames.bin').read_bytes())
    assert fields == ['AttackDelay', 'CombatBeat', 'CombatMusic', 'DelayedLoad', 'Flags',
                      'InteractRadius', 'LeashDistance', 'MeleeRadius', 'OnAggroSFX',
                      'Script', 'SelfFX', 'Trophy', 'Type', 'ViewRadius', 'ViewRadiusNoAggro']
    raw = (data / 'ai_pyarray.bin').read_bytes()
    count = struct.unpack_from('<I', raw)[0]
    assert count == len(names) == 76
    at = 4
    rows = []
    for index in range(count):
        prefix = struct.unpack_from('<iiiBIfffi', raw, at)
        at += 33
        length = struct.unpack_from('<I', raw, at)[0]
        at += 4
        script = raw[at:at+length].decode('ascii')
        at += length
        suffix = struct.unpack_from('<iiiff', raw, at)
        at += 20
        if index in (8, 44):
            rows.append(dict(id=index, name=names[index], serialized_script_bytes=length,
                             **dict(zip(fields, (*prefix, script, *suffix)))))
    assert at == len(raw)
    assert [(r['id'], r['name'], r['Script']) for r in rows] == [(8, 'Basic', ''), (44, 'Player', '__player__')]
    inputs = []
    with zipfile.ZipFile(args.cache) as cache:
        for filename in ('ai_pyarray.bin', 'ai_pyarraynames.bin', 'ai_pystructnames.bin'):
            path = data / filename
            entries = [entry for entry in cache.namelist()
                       if entry.endswith('/' + filename) and cache.read(entry) == path.read_bytes()]
            assert len(entries) == 1, (filename, entries)
            inputs.append(dict(asset=str(path.relative_to(REPO)), cache_entry=entries[0], sha256=sha(path)))
    report = dict(validation='PASS', cache_sha256=sha(args.cache), inputs=inputs,
                  table_rows=count, selected_rows=rows, inspector_sha256=sha(Path(__file__)),
                  scope='Authored cache input inspection only. Row44 script selects AISPlayerIPhone through '
                        'the separately original-audited selector. Row8 fallback depends on the actual owner '
                        'name, whose creation producer is not proven here. No AIS initialization/live APK claim.')
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
