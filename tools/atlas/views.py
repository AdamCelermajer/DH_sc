"""Generate Act-organised Original Data Atlas views from the source inventories.

The generator reads only `.local-inputs` and other generated Atlas JSON.  It
never writes to source assets.  Act membership outside the explicitly directed
Act 1 boundary remains labelled INFERENCE until an owner confirms it.
"""
from collections import Counter, defaultdict
import json
import re
import xml.etree.ElementTree as ET
from pathlib import Path

from common import read_json, write_json, markdown_table, stable_id, source_site

# Coarse story-region buckets, explicitly provisional. These groupings help
# navigation but are not evidence of the game's authored act boundaries.
ACT_BUCKETS = {
    'act_1': ('001_swamp', '002_swamp_witchcave'),
    'act_2': ('003_darkwood', '004_root_den', '005_infectedvillage', '006_basement',
              '007_crypt_01', '008_gothicus_castle_01', '009_abbey', '009a_abbey',
              '010_el_lightning', '011_earth_temple_01', '012_earth_temple_02',
              '013_earth_temple_03', '014_el_earth'),
    'act_3': ('015_red_desert_hub', '016_red_desert_cave_01', '017_red_desert_cave_02',
              '018_red_desert_cave_03', '019_light_house_01', '019_light_house_02',
              '019_light_house_03', '020_crypt_02', '021_gothicus_castle_02',
              '022_swamp2', '022_swamp2_trollcave', '023_watertemple', '024_el_water'),
    'act_4': ('025_icy_hub', '026_icy_cavern_01', '027_icy_cavern_02', '028_icy_cavern_03',
              '029_wind_temple', '030_el_air', '031_firetemple', '032_el_fire'),
    'act_5': ('033_voidmaze_01', '034_voidmaze_02', '035_voidmaze_03', '036_underworld_hub',
              '037_machine_rooms_01', '038_dark_temple', '039_dark_temple_bossroom_01'),
}


def _clean(v):
    return str(v or '').replace('\\', '/')


def _scene_kind(name):
    if name.startswith('x') and '_backup' in name:
        return 'backup'
    m = re.match(r'^(\d+)', name)
    n = int(m.group(1)) if m else -1
    return 'debug' if 100 <= n <= 130 else ('campaign' if 0 <= n < 100 else 'unplaced')


def _act_for_scene(stem):
    for act, stems in ACT_BUCKETS.items():
        if stem in stems:
            return act
    return 'unplaced'


def _scene_record(ctx, path):
    stem = path.stem
    record = dict(id=stable_id('scene', stem), name=stem, file=path.name,
                  source=source_site(ctx, path), source_kind=_scene_kind(path.name),
                  act=_act_for_scene(stem), act_status='INFERENCE',
                  membership_confidence='INFERENCE', config={}, direct_objects=[],
                  procedural_lists=[], status='TODO')
    try:
        root = ET.parse(path).getroot()
        if root.tag.lower() == 'level':
            configs = [e for e in root.iter() if e.attrib.get('type', '').lower() == 'level'
                       or e.attrib.get('gametype', '').lower() == 'levelconfig']
            if configs:
                record['config'] = dict(configs[0].attrib)
            for e in root.iter():
                attrs = e.attrib
                if attrs.get('type', '').lower() not in ('', 'level'):
                    record['direct_objects'].append({k: attrs[k] for k in
                        ('type', 'name', 'gametype', 'scriptFile', '_templateName') if attrs.get(k)})
        elif root.tag.lower() == 'rules':
            record['config'] = dict(root.attrib)
            for e in root.iter('list'):
                elems = [{k: v for k, v in x.attrib.items()} for x in e.findall('elem')]
                record['procedural_lists'].append(dict(name=e.get('name'), random=e.get('random'),
                                                       count=len(elems), choices=elems))
    except (ET.ParseError, OSError) as exc:
        record['parse_error'] = str(exc)
        record['status'] = 'UNRESOLVED'
        record['reason'] = 'SOURCE_XML_PARSE_ERROR'
    return record


def _table_map(tables):
    out = defaultdict(list)
    for row in tables or []:
        # Shared table decoder keeps physically distinct struct groups in the
        # table label; expose both the base table and its original group.
        key = row.get('table', '')
        row.setdefault('source_group', key.partition('#')[2] or 'group0')
        out[key.partition('#')[0]].append(row)
    return out


def _field(row, *names):
    fields = row.get('fields') or {}
    folded = {str(k).casefold(): v for k, v in fields.items()}
    for name in names:
        if name.casefold() in folded:
            return folded[name.casefold()]
    return None


def _str(v):
    if isinstance(v, (dict, list)):
        return json.dumps(v, ensure_ascii=False, sort_keys=True)
    return '' if v is None else str(v)


def _record_actor(row):
    fields = row.get('fields') or {}
    return dict(id=row.get('id'), index=row.get('index'), name=row.get('name'),
                model=_str(_field(row, 'ModelFile', 'Model', 'ModelName')),
                animation_table=_str(_field(row, 'AnimTable', 'AnimationTable')),
                ai=_str(_field(row, 'AI', 'AIType', 'AIName')),
                skills=_str(_field(row, 'SkillTree', 'Skills')),
                loot=_str(_field(row, 'Loot', 'LootTable')),
                fields=fields, source_table='character_properties',
                status=row.get('status', 'TODO'), work_status=row.get('work_status', 'TODO'))


def _act_rows(ctx, act, scenes, actor_rows, scripts, refs, tables):
    rows = [s for s in scenes if s['act'] == act and s['source_kind'] == 'campaign']
    by_name = {r['name']: r for r in actor_rows}
    relevant_names = set()
    for sc in rows:
        relevant_names.add(sc['name'])
        for item in sc['direct_objects']:
            if item.get('name'):
                relevant_names.add(item['name'])
        for proc in sc['procedural_lists']:
            for choice in proc['choices']:
                for v in choice.values():
                    if isinstance(v, str): relevant_names.add(v)
    # Script-derived actor membership requires a proven source-level spawn site.
    # Preserve all unknown actor rows in the data file; never assign by name guess.
    actor_list = []
    for a in actor_rows:
        if a.get('scene_membership') == act:
            actor_list.append(a)
    return rows, actor_list


def _table_page(title, rows, headers, getrow):
    return '# ' + title + '\n\n' + markdown_table(headers, [getrow(x) for x in rows])


def run(ctx, inventory_only=False):
    root = Path(ctx.root)
    out = Path(ctx.out)
    inputs = Path(ctx.inputs)
    source = inputs / 'assets-extra/android/data/scene'
    if not source.is_dir():
        raise RuntimeError(f'Android original scene data not found: {source}')

    tables = read_json(out / 'data/tables.json', []) or []
    refs = read_json(out / 'data/references.json', []) or []
    scripts = read_json(out / 'data/scripts.json', []) or []
    script_refs = read_json(out / 'data/script-references.json', []) or []
    by_table = _table_map(tables)
    scene_records = [_scene_record(ctx, p) for p in sorted(source.iterdir())
                     if p.is_file() and p.suffix.lower() in ('.mlx', '.xml')
                     and p.name != '.nomedia']
    if inventory_only:
        for scene in scene_records:
            scene['act'] = 'unplaced'
    actor_rows = [_record_actor(r) for r in by_table.get('character_properties', []) if r.get('source_group') == 'group0' and int(r.get('index', -1)) < 448]
    for actor in actor_rows:
        actor['act'] = 'unplaced'
        actor['act_assignment_status'] = 'TODO'
        actor['act_assignment_reason'] = 'No source-backed spawn-to-level link established.'

    # Actual level table names are kept beside scene XML names: they are separate
    # original indices and must not be conflated with editor-export filenames.
    level_table_rows = [r for r in by_table.get('levels', []) if r.get('source_group') in ('group0','group1')]
    scene_level_rows = []
    for sc in scene_records:
        scene_level_rows.append({**sc, 'record_type': 'scene_file',
            'linked_level_rows': [r.get('id') for r in level_table_rows
                if sc['name'].casefold() in r.get('name', '').casefold()
                or r.get('name', '').casefold() in sc['name'].casefold()]})
    table_level_rows = []
    for r in level_table_rows:
        table_level_rows.append({**r, 'record_type': 'levels_table_row',
            'act': 'unplaced', 'act_status': 'INFERENCE',
            'source_table': 'levels', 'source': 'pydata/levels_pyarray.bin',
            'status': r.get('status', 'TODO'), 'work_status': r.get('work_status', 'TODO')})
    levels = scene_level_rows + table_level_rows

    # Explicit act assignment is required for actual placements. Current scene
    # exports do not contain character spawns (the campaign script files named by
    # scriptFile are outside this Android data capture), so actor rows remain
    # unplaced pending an authoritative spawn link.
    membership = {
        'schema': 'atlas-act-membership-v1',
        'boundary_policy': 'All act boundaries are INFERENCE until owner confirmation.',
        'inferences': [
            {'act': 'act_1', 'level_stems': list(ACT_BUCKETS['act_1']), 'confidence': 'INFERENCE',
             'evidence': ['coordination/v21.8/ATLAS-CODEX-PROMPT.md: Act 1 is swamp + witch cave ending at Mud Lord and Darkwood load',
                          'coordination/v21.7/B155-B156-HANDOFF.md: B155 documents Mud Lord encounter and Gothicus-to-Darkwood load',
                          'docs/BUGS-AND-IMPLEMENTATION.md: B157 references cave transition behavior'],
             'note': 'Task-directed grouping. The source data and report support the transition, but do not label an authored act boundary.'},
            {'act': 'act_2', 'level_stems': list(ACT_BUCKETS['act_2']), 'confidence': 'INFERENCE',
             'evidence': ['numbered scene export names and geographic map labels; transition chain remains under investigation'],
             'note': 'Provisional geographic bucket; no authored act marker established.'},
            {'act': 'act_3', 'level_stems': list(ACT_BUCKETS['act_3']), 'confidence': 'INFERENCE',
             'evidence': ['numbered scene export names and geographic map labels; transition chain remains under investigation'],
             'note': 'Provisional geographic bucket; no authored act marker established.'},
            {'act': 'act_4', 'level_stems': list(ACT_BUCKETS['act_4']), 'confidence': 'INFERENCE',
             'evidence': ['numbered scene export names and geographic map labels; transition chain remains under investigation'],
             'note': 'Provisional geographic bucket; no authored act marker established.'},
            {'act': 'act_5', 'level_stems': list(ACT_BUCKETS['act_5']), 'confidence': 'INFERENCE',
             'evidence': ['numbered scene export names and geographic map labels; transition chain remains under investigation'],
             'note': 'Provisional geographic bucket; no authored act marker established.'},
        ],
        'actor_assignments': [],
        'actor_assignment_gap': 'No direct character spawn records are present in the supplied scene XML. Level scriptFile paths point to .pyscript files not included in assets-extra/android/data; procedural module files contain geometry blocks but no actor rows. Keep CharacterProperties rows unplaced until a level-to-script/module spawn link is evidenced.'
    }
    if inventory_only:
        membership['inferences'] = []
        membership['boundary_rows'] = [{'act': f'act_{i}', 'status': 'TODO', 'confidence': 'UNKNOWN',
            'evidence': [], 'reason': 'Transition-chain / source-act analysis has not begun.'} for i in range(1,6)]
        membership['level_rows'] = [{'scene_id': s['id'], 'scene': s['file'], 'act': None,
            'status': 'TODO', 'reason': 'Act membership not mapped during the inventory checkpoint.'}
            for s in scene_records]

    # Count rows before any curation or mapping. Work status remains TODO for the
    # integration lead to accept after generator/output review.
    level_inventory = len(scene_records)
    debug_count = sum(s['source_kind'] == 'debug' for s in scene_records)
    backup_count = sum(s['source_kind'] == 'backup' for s in scene_records)
    campaign_count = sum(s['source_kind'] == 'campaign' for s in scene_records)
    # Inventory candidates use explicit BOSS/MIDBOSS labels in CharacterProperties
    # names plus the named custom encounter actors in the atlas brief. This is a
    # candidate list, not an assertion that each row's AI script has been linked.
    extra_boss_names = {'balrog','darkqueenform1','darkqueenform2','edwardboss','flaminite',
        'lighthouse_emissary','madruk_a','madruk_b','resurrectinggod','roottroll',
        'roottroll_midboss','sandworm','sandworm2','sandwormqueen','swampking','witch'}
    boss_candidates = [a for a in actor_rows if re.search(r'boss|midboss', a['name'] or '', re.I)
                       or (a['name'] or '').casefold() in extra_boss_names]
    bosses = boss_candidates

    # Preserve initial all-TODO status during the inventory checkpoint. On
    # ordinary regeneration, scene metadata is DONE only when the XML parsed;
    # table rows keep the source table mapper's status.
    if inventory_only:
        for row in levels:
            row['status'] = 'TODO'; row['work_status'] = 'TODO'
        for row in actor_rows:
            row['status'] = 'TODO'; row['work_status'] = 'TODO'
    else:
        for row in scene_level_rows:
            row['status'] = 'DONE' if not row.get('parse_error') else 'UNRESOLVED'
            row['work_status'] = row['status']
    write_json(out / 'data/levels.json', levels)
    write_json(out / 'data/actors.json', actor_rows)
    write_json(out / 'data/bosses.json', [{**a, 'classification':'BOSS_CANDIDATE',
        'classification_basis':'explicit BOSS/MIDBOSS name or named encounter actor; AI link and act placement TODO',
        'status':'TODO', 'work_status':'TODO'} for a in boss_candidates])
    write_json(out / 'data/act-membership.json', membership)
    write_json(out / 'counts/views.json', {
        'category': 'ACT-ORGANISED GENERATED VIEWS',
        'work_status': 'TODO',
        'inventory': {
            'scene_level_rows': {'total': level_inventory, 'todo': level_inventory},
            'campaign_scene_rows': {'total': campaign_count, 'todo': campaign_count},
            'debug_scene_rows': {'total': debug_count, 'todo': debug_count},
            'backup_scene_rows': {'total': backup_count, 'todo': backup_count},
            'levels_table_rows': {'total': len(level_table_rows), 'todo': len(level_table_rows)},
            'character_properties_actor_rows': {'total': len(actor_rows), 'todo': len(actor_rows)},
            'boss_candidate_actor_rows': {'total': len(boss_candidates), 'todo': len(boss_candidates), 'definition': 'BOSS/MIDBOSS name markers or explicitly named encounter actors; candidates only'},
            'boss_candidates_by_custom_core_ai_link': {'total': len(bosses), 'todo': len(bosses),
                                                        'classification': 'candidate; must confirm table link'},
            'dialogue_rows': {'total': len(by_table.get('dialogs', [])), 'todo': len(by_table.get('dialogs', []))},
            'source_script_rows': {'total': len(scripts), 'todo': len(scripts)},
            'script_reference_rows': {'total': len(script_refs), 'todo': len(script_refs)},
            'asset_reference_rows': {'total': len(refs), 'todo': len(refs)},
        },
        'boundary_rows': {'total': 5, 'todo': 5},
        'actor_act_membership_rows': {'total': len(actor_rows), 'todo': len(actor_rows)},
        'notes': ['Counts are inventory, not completion claims.',
                  'Rows remain TODO pending owner integration/acceptance.',
                  'Boss count is only a candidate count from explicit custom-core links.']
    })
    if inventory_only:
        return {'scene_level_rows': level_inventory, 'levels_table_rows': len(level_table_rows), 'campaign_scene_rows': campaign_count,
                'debug_scene_rows': debug_count, 'backup_scene_rows': backup_count,
                'character_properties_actor_rows': len(actor_rows), 'boss_candidates': len(bosses)}

    # Produce navigable directories. The detail layer is intentionally limited to
    # Act 1 and the next inferred region (Act 2).
    for act in ACT_BUCKETS:
        target = out / act
        target.mkdir(parents=True, exist_ok=True)
        scene_rows = [s for s in scene_level_rows if s['act'] == act]
        text = ['# ' + act.replace('_', ' ').title(), '',
                '> Act membership is INFERENCE. See `../data/act-membership.json`; owner confirmation is pending.', '',
                '## Levels', '',
                markdown_table(['File', 'Format', 'Level script', 'Music', 'Scene objects', 'Procedural lists', 'Source status'],
                    [[s['file'], s['source_kind'], s['config'].get('scriptFile', ''),
                      s['config'].get('music', s['config'].get('ambiant_music', '')),
                      len(s['direct_objects']), len(s['procedural_lists']), s['status']] for s in scene_rows]),
                '', '## Level detail', '']
        for s in scene_rows:
            text += [f"### `{s['file']}`", '', f"Source: `{s['source']}`. Parsed level configuration: direct XML evidence.", '',
                     markdown_table(['Field', 'Value'], [[k, v] for k, v in s['config'].items()]), '']
            if s['direct_objects']:
                text += ['Direct scene objects (not character spawns):', '',
                         markdown_table(['Type', 'Name', 'Game type', 'Script'],
                            [[o.get('type',''),o.get('name',''),o.get('gametype',''),o.get('scriptFile','')]
                             for o in s['direct_objects']]), '']
            if s['procedural_lists']:
                text += ['Procedural rule lists (module choices; does not imply loaded actor roster):', '',
                         markdown_table(['List', 'Random', 'Choices'],
                            [[p['name'], p['random'], p['count']] for p in s['procedural_lists']]), '']
        if act in ('act_1', 'act_2'):
            text += ['## Actors and mobs', '',
                     'Actor table rows are indexed in `../data/actors.json`. No scene-specific actor assignment is made until a source spawn link is available.', '',
                     markdown_table(['Actor rows', 'Mapped to this act', 'Status'],
                        [[len(actor_rows), 0, 'TODO: placement link missing']]), '',
                     '## Bosses', '',
                     'Boss candidates require an explicit CharacterProperties AI-to-custom-core script link; see `../data/actors.json`.', '',
                     markdown_table(['Candidate rows', 'Status'], [[len(bosses), 'TODO: verify classification and act placement']]), '',
                     '## Dialogues', '',
                     'The dialog table is inventoried globally. Per-act dialogue assignment remains TODO until a level/campaign script reference is resolved.', '',
                     f"Dialog table rows: {len(by_table.get('dialogs', []))}.", '',
                     '## Act-specific scripts', '',
                     'Level `scriptFile` values are recorded above. Referenced .pyscript files are not present in the supplied Android data capture; Lua AI/object scripts are indexed separately by the script category.', '']
        (target / 'index.md').write_text('\n'.join(text), encoding='utf-8')

    # Unplaced comprises all debug scenes, backups, demo scene, and any scene
    # not covered by an inferred campaign bucket.
    unplaced = [s for s in scene_level_rows if s['source_kind'] in ('debug', 'backup') or s['act'] == 'unplaced'
                or s['name'].startswith('050_')]
    (out / 'unplaced/index.md').write_text('# Unplaced and non-campaign scenes\n\n' +
        '> Debug levels, backups, demo scenes, and scenes without a provisional campaign bucket are not assigned to an act.\n\n' +
        markdown_table(['File', 'Category', 'Source', 'Status'],
                       [[s['file'], s['source_kind'], s['source'], s['status']] for s in unplaced]),
        encoding='utf-8')

    (out / 'general/levels.md').write_text('# Original level inventory\n\n' +
        markdown_table(['File', 'Category', 'Act inference', 'Script', 'Music', 'Direct objects', 'Rule lists'],
            [[s['file'], s['source_kind'], s['act'], s['config'].get('scriptFile',''),
              s['config'].get('music', s['config'].get('ambiant_music','')),
              len(s['direct_objects']), len(s['procedural_lists'])] for s in levels]), encoding='utf-8')
    (out / 'general/characters.md').write_text('# CharacterProperties row inventory\n\n' +
        '> These are original table rows, not a verified roster of mobs spawned in any one level. Placement is intentionally unassigned until a level-to-spawn link is recovered.\n\n' +
        markdown_table(['Index', 'Name', 'Model', 'Animation table', 'AI', 'Skill tree', 'Loot', 'Status'],
            [[a['index'], a['name'], a['model'], a['animation_table'], a['ai'], a['skills'], a['loot'], a['status']] for a in actor_rows]),
        encoding='utf-8')
    (out / 'shared/index.md').write_text('# Shared assets\n\n' +
        '> Shared membership requires evidence that the same resolved asset is referenced from two or more act groups. The current scene exports identify level music, script paths, and procedural module names; they do not establish actor-spawn reuse. This view will be filled by the reference-resolution merge.\n\n' +
        'Current confirmed cross-act assets: 0. Inventory status: TODO pending reference join.\n', encoding='utf-8')

    all_status = Counter(s['status'] for s in scene_level_rows)
    report = ['# Act-organised views report', '',
        'Generated by `tools/atlas/views.py` from Android original scene XML plus the Atlas tables/scripts/references JSON. Original inputs are read-only.', '',
        '## Inventory', '',
        markdown_table(['Rows', 'Count', 'Current status'], [
            ['Scene XML level/rule files', len(levels), dict(all_status)],
            ['Campaign scene rows (numeric <100; includes demo)', campaign_count, 'TODO'],
            ['Debug/test scene rows 100–130', debug_count, 'TODO'],
            ['Backup scenes xNN', backup_count, 'TODO'],
            ['Levels table rows', len(level_table_rows), 'TODO'],
            ['CharacterProperties rows', len(actor_rows), 'TODO'],
            ['Boss candidates via custom AI core link', len(bosses), 'TODO; classification pending'],
            ['Dialog rows', len(by_table.get('dialogs', [])), 'TODO'],
        ]), '',
        '## Boundary evidence and limits', '',
        '- **Act 1 grouping is an inference constrained by the task instruction.** The instruction names the swamp + witch cave and says the endpoint is the Mud Lord encounter followed by the Darkwood load. B155 handoff directly documents the Mud Lord quest completion, Gothicus-to-Darkwood transition, and `003_darkwood.mlx` load. B157 report documents a separate Witch Cavern E-transition level-up issue but does not establish the overall campaign act boundary.',
        '- **Acts 2–5 are provisional geographic/numbered buckets**, recorded with `INFERENCE` confidence in `data/act-membership.json`. They are not asserted as original authored acts.',
        '- The direct scene XML exposes level config (script path, music, skybox/light/fog), static editor objects and procedural module lists. It does not expose character placements in these files.',
        '- Level config script paths end in `.pyscript` and are not present as files under `assets-extra/android/data`; the recovered input includes 226 Lua/source files elsewhere. Do not equate an unavailable `.pyscript` path to a missing behavior.',
        '- Scene `.rule.xml` list choices are procedural module references, not proof all choices load in a given run. `.mlx` module inspection sees `Block` geometry only; no actor placement links were found in that inspected subset.',
        '- Actor rows remain in the global character index until a source-backed spawn-to-level mapping exists. No mob counts per act or boss assignment is claimed from row names alone.', '',
        '## Act-specific detail availability', '',
        '- `act_1/index.md` and `act_2/index.md` include generated per-level detail sections, because only Act 1 and the immediately following inferred region receive detail pages.',
        '- `act_3/index.md` through `act_5/index.md` are generated tables only.',
        '- Debug levels 100–130, `xNN_*_backup`, and demo level 050 are under `unplaced/`.', '',
        '## Status', '',
        'All inventory statuses remain TODO pending root integration and acceptance. This report records the rows and provenance, not completion of all level, actor, boss, dialogue, transition, or loot semantics.', '']
    (out / 'reports/views.md').write_text('\n'.join(report), encoding='utf-8')
    return {'scene_level_rows': level_inventory, 'campaign_scene_rows': campaign_count,
            'debug_scene_rows': debug_count, 'backup_scene_rows': backup_count,
            'character_properties_actor_rows': len(actor_rows), 'boss_candidates': len(bosses)}
