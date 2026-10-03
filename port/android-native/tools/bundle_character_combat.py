"""Bundle authored Android Knight attack/death states and reachable stances.

AttackMoving is the native state label; the serialized table field is Attack.
This prepares assets only, without inventing animation or equipment choices.
"""
import argparse
import hashlib
import json
import struct
import sys
import zipfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(REPO / 'port/level-world/tools'))
from inventory import EXPECTED
from prepare_actors import strings
from prepare_player_combat import animation_tables
from bundle_locomotion import constants


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--cache', type=Path, required=True)
    parser.add_argument('--project', type=Path, action='append')
    args = parser.parse_args()
    with args.cache.open('rb') as stream:
        assert hashlib.file_digest(stream, 'sha256').hexdigest() == EXPECTED
    inputs = {}
    payloads = {}
    states = []
    clips = {}
    with zipfile.ZipFile(args.cache) as archive:
        entries = {}
        for entry in archive.infolist():
            entries.setdefault(entry.filename.lower(), []).append(entry)

        def read(path):
            found = entries.get('com.gameloft.android.gand.gloftd2ss/files/' + path.lower(), [])
            assert len(found) == 1, ('Missing/duplicate original input', path)
            raw = archive.read(found[0])
            record = {'entry': found[0].filename, 'bytes': len(raw), 'sha256': hashlib.sha256(raw).hexdigest()}
            inputs[record['entry']] = record
            return raw, record

        def table(name):
            return read('data/pydata/' + name)[0]

        names, _ = strings(table('character_properties_pyarraynames.bin'))
        fields, _ = strings(table('character_properties_pystructnames.bin'))
        row = names.index('KnightPlayerBase')
        properties = struct.unpack_from('<224i', table('character_properties_pyarray.bin'), 4 + row * 896)
        table_id = properties[fields.index('AnimTable')]
        sequences, characters = animation_tables(table('animations_pyarray.bin'))
        raw = table('animations_pystructnames.bin')
        for _ in range(4):
            state_names, consumed = strings(raw)
            raw = raw[consumed:]
        paths, _ = strings(table('animations_dictionary_pyarray.bin'))
        cst = constants(table('animations_pycst.bin'))
        mask = cst['AnimStancedAnim']['SL__LIST_IPHONE']
        stance_count = cst['AnimStances']['COUNT_IPHONE']
        assert table_id == 48 and row == 263 and mask == 210 and stance_count == 5

        def collect(index, depth=0):
            assert depth < 3 and 0 <= index < len(sequences)
            for step in sequences[index]['steps']:
                if step['redir'] == 1:
                    collect(step['anim'], depth + 1)
                else:
                    assert step['redir'] == 0 and 0 <= step['anim'] < len(paths)
                    clips[step['anim']] = paths[step['anim']]

        for label, field, bit in (('AttackMoving', 'Attack', 'SL_ATTACK'),
                                  ('AttackStatic', 'AttackStatic', 'SL_ATTACK_STATIC'),
                                  ('Died', 'Died', 'SL_DIED')):
            base = characters[table_id][state_names.index(field)][0]
            enabled = bool(mask & cst['AnimStancedAnim'][bit])
            roots = list(range(base, base + (stance_count if enabled else 1)))
            for root in roots:
                collect(root)
            states.append({'state': label, 'table_field': field, 'base_sequence': base,
                           'stance_enabled': enabled, 'sequences': roots})
        clip_records = []
        for clip_id, path in sorted(clips.items()):
            raw, original = read(path)
            filename = Path(path.replace('\\', '/')).name
            assert filename.endswith('.bdae')
            asset = 'animations/' + filename
            assert asset not in payloads or payloads[asset] == raw
            payloads[asset] = raw
            clip_records.append({'clip_id': clip_id, 'asset': asset, **original})
    report = {'cache_sha256': EXPECTED, 'character': 'KnightPlayerBase', 'character_row': row,
              'animation_table': table_id, 'stance_mask': mask, 'stance_count': stance_count,
              'states': states, 'clips': clip_records, 'inputs': list(inputs.values()),
              'scope': 'Authored reachable Android Knight attack/death clips. Full equipment, FSM, blending, FX and audio remain separate source reconstruction.'}
    encoded = (json.dumps(report, indent=2) + '\n').encode()
    additions = {}
    for project in args.project or [REPO / 'port/android-native']:
        assets = project.resolve() / 'app/src/main/assets'
        assert assets.is_dir()
        additions[str(project)] = 0
        for name, raw in payloads.items():
            target = assets / name
            if target.exists():
                assert target.read_bytes() == raw, ('Existing authored asset differs', target)
            else:
                target.write_bytes(raw)
                additions[str(project)] += 1
        (assets / 'character-combat-provenance.json').write_bytes(encoded)
    print(json.dumps({'clips': len(clip_records), 'added_clips': additions,
                      'provenance_sha256': hashlib.sha256(encoded).hexdigest(), 'states': states}))


if __name__ == '__main__':
    main()
