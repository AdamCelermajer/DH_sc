"""Focused source and damaged-input checks for the joined mob lookup."""
from collections import defaultdict
from types import SimpleNamespace
from common import read_json, write_json
import mob_drops
import mob_sounds
import mobs


def run(ctx):
    index = mobs.Index(ctx)
    profiles = {p['index']: p for p in read_json(ctx.out / 'data/mob-profiles.json', [])}
    checks = []

    def check(name, passed, detail):
        checks.append({'name': name, 'passed': bool(passed), 'detail': detail})

    decoded = mob_sounds.build(ctx, index.tables, index.refs)
    sounds = {r['name']: r for r in decoded['rows']}
    check('original CharSounds payload and boundary',
          sounds['Default']['fields'] == {'Die': [], 'Hurt': [], 'ImpactVsFlesh': [], 'ImpactVsMetal': [], 'IsFlesh': True, 'IsMetallic': False}
          and sounds['ArmoredHuman']['fields'] == {'Die': [28], 'Hurt': [29, 30, 31],
               'ImpactVsFlesh': [120, 121, 122], 'ImpactVsMetal': [117, 118, 119], 'IsFlesh': False, 'IsMetallic': True}
          and decoded['consumption']['bytes_consumed'] == 98,
          decoded['consumption'])
    for cp, root, count, script in [(358, 229, 58, 'swampking_core.luac'),
                                   (435, 338, 58, 'witch.luac'), (360, 230, 30, 'monster.luac')]:
        profile = profiles[cp]
        scripts = [index.script_by_id[i]['name'] for i in profile['scripts']['script_ids']]
        check('joined original actor ' + profile['name'],
              profile['drops']['root']['index'] == root
              and len(profile['drops']['terminal_items']) == count and script in scripts
              and not profile['drops']['gaps'] and not profile['complete'],
              {'loot_root': root, 'terminal_item_entries': count, 'scripts': scripts})
    swamp = profiles[358]
    audio = [index.asset_by_id[i]['path'] for i in swamp['audio']['asset_ids']]
    check('Mud Lord animation audio comes from available iOS cache',
          any('/ios/data/sounds/sfx_serpent_attack.wav' in p for p in audio), audio)

    # A model's direct resolved texture edges must be present as files too.
    model = swamp['sections']['model']
    known_assets = set(model['asset_ids'])
    targets = {a['id'] for rid in model['reference_ids'] for ref in [index.ref_by_id[rid]]
               if ref.get('kind') == 'texture' and ref['status'] == 'RESOLVED'
               for target in ref.get('resolves_to') or []
               for file in target.get('asset_files') or [] for a in index.files(file['path'])}
    check('model texture dependencies are included', bool(targets) and targets <= known_assets,
          {'resolved_texture_files': len(targets), 'joined_model_files': len(known_assets)})

    # Deliberate source graph corruption: terminate a cycle and expose a missing list.
    rows = [{'table': 'loot_table#group5', 'index': 0, 'id': 'a', 'name': 'a',
             'fields': {'SubLoots': [1], 'FixedLootEntries': [{'ItemListID': 99}]}},
            {'table': 'loot_table#group5', 'index': 1, 'id': 'b', 'name': 'b', 'fields': {'SubLoots': [0]}}]
    broken = mob_drops.build(None, rows, [], 0)
    check('loot cycles and dangling item list are visible', len(broken['cycles']) == 1 and
          any(g.get('target', {}).get('index') == 99 for g in broken['gaps']), broken['counts'])
    try:
        reader = mob_sounds._Reader(ctx.inputs / 'assets-extra/android/data/pydata/sounds_pyarray.bin')
        reader.take(len(reader.data) + 1)
        rejected = False
    except ValueError:
        rejected = True
    check('truncated character sound bytes are rejected', rejected, 'bounded reader rejects overrun')
    result = {'passed': all(c['passed'] for c in checks), 'tests': checks,
              'scope': 'Static source decoding and integrated generated lookup; no game/runtime certification.'}
    write_json(ctx.out / 'data/mob-verification.json', result)
    return result
