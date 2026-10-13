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

ACTS = tuple(f'act_{i}' for i in range(1, 6))


def _clean(v):
    return str(v or '').replace('\\', '/')


def _scene_kind(name):
    if name.startswith('x') and '_backup' in name:
        return 'backup'
    if name.startswith('050_'):
        return 'demo'
    m = re.match(r'^(\d+)', name)
    n = int(m.group(1)) if m else -1
    return 'debug' if 100 <= n <= 130 else ('campaign' if 0 <= n < 100 else 'unplaced')


def _tokens(text):
    value = str(text or '').casefold()
    for compound, parts in {'witchcave': 'witch cave', 'trollcave': 'troll cave',
                            'infectedvillage': 'infected village', 'rootden': 'root den',
                            'red_desert': 'red desert', 'secretbasement': 'secret basement',
                            'light_house': 'light house', 'earthtemple': 'earth temple',
                            'windtemple': 'wind temple', 'firetemple': 'fire temple',
                            'watertemple': 'water temple', 'voidmaze': 'void maze',
                            'darkwood': 'darkwood'}.items():
        value = value.replace(compound, parts)
    return {str(int(token)) if token.isdigit() else token
            for token in re.findall(r'[a-z]+|\d+', value)}


def _derive_act_membership(level_rows, scenes):
    """Assign only scenes supported by the explicit progression evidence.

    Region words, shared tokens, and fast-travel order do not establish act
    membership. The task fixes Act 1 to Swamp and Witch Cave and B155 records
    the next load as 003_darkwood, which is retained as the single Act 2 entry
    point. Everything else remains unplaced pending a unique transition chain.
    """
    del level_rows  # A name/region table match is not boundary evidence.
    explicit = {
        '001_swamp': ('act_1', ['task-directed Act 1: Swamp opening level']),
        '002_swamp_witchcave': ('act_1', ['task-directed Act 1: Witch Cave']),
        '002_swamp_witchcave.rule': ('act_1', ['task-directed Act 1: Witch Cave']),
        '003_darkwood': ('act_2', ['B155 handoff: post-Mud-Lord transition loads 003_darkwood.mlx']),
    }
    out = {}
    for scene in scenes:
        match = explicit.get(scene.get('name', '').casefold()) if scene.get('source_kind') == 'campaign' else None
        out[scene['id']] = match or ('unplaced', ['No unique source-backed transition-chain assignment.'])
    return out, []


def _scene_record(ctx, path):
    stem = path.stem
    record = dict(id=stable_id('scene', stem), name=stem, file=path.name,
                  source=source_site(ctx, path), source_kind=_scene_kind(path.name),
                  act='unplaced', act_status='TODO',
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
                        ('type', 'name', 'gametype', 'scriptFile', '_templateName', 'mgp') if attrs.get(k)})
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


def _module_character_candidates(ctx, scene, actor_by_name, template_by_name, inventory_only=False):
    """Read exact source MGPs referenced by a scene/rule and list character candidates.

    MGP character objects and template names identify authored possible spawns;
    conditional rules, random module selection, spawn probability, and script gates
    mean they are not a runtime roster.
    """
    data_root = Path(ctx.inputs) / 'assets-extra/android/data'
    modules_root = data_root / '3d/modules'
    modules_by_lower = {p.as_posix().casefold(): p for p in modules_root.rglob('*.mgp')} if modules_root.is_dir() else {}
    paths = []
    for obj in scene.get('direct_objects', []):
        mgp = obj.get('mgp', '')
        if mgp:
            normalized = mgp.replace('\\', '/')
            marker = normalized.casefold().find('3d/modules/')
            if marker >= 0:
                candidate = normalized[marker:]
                path = modules_by_lower.get((modules_root / candidate[len('3d/modules/'):]).as_posix().casefold())
                if path: paths.append((path, 'static scene module reference'))
    folder = (scene.get('config') or {}).get('folder', '').replace('\\', '/')
    folder_match = re.search(r'(?:data/)?3d/modules/(.+)$', folder, re.I)
    folder_path = folder_match.group(1) if folder_match else ''
    for proc in scene.get('procedural_lists', []):
        for choice in proc.get('choices', []):
            gameplay = choice.get('gameplay', '')
            if gameplay and folder_path:
                path = modules_by_lower.get((modules_root / folder_path / 'mgp' / gameplay).as_posix().casefold())
                if path: paths.append((path, f"procedural choice {proc.get('name')}"))
    result, seen = [], set()
    for path, relation in paths:
        try:
            root = ET.parse(path).getroot()
        except (ET.ParseError, OSError):
            continue
        for obj in root.iter('GameObject'):
            if obj.get('gametype', '').casefold() != 'character':
                continue
            key = (path.as_posix().casefold(), obj.get('name', ''))
            if key in seen: continue
            seen.add(key)
            cp_name = obj.get('charpropsname', '')
            template = obj.get('char_template', '')
            cp = actor_by_name.get(cp_name.casefold()) if cp_name else None
            template_row = template_by_name.get(template.casefold()) if template else None
            result.append({
                'module': source_site(ctx, path), 'relation': relation,
                'object_name': obj.get('name'), 'charpropsname': cp_name or None,
                'actor_id': cp.get('id') if cp else None,
                'actor_name': cp.get('name') if cp else None,
                'template': template or None,
                'template_id': template_row.get('id') if template_row else None,
                'spawn_prob': obj.get('spawn_prob'), 'char_group': obj.get('char_group'),
                'char_group_role': obj.get('char_group_role'), 'script': obj.get('script'),
                'ai_state': obj.get('ai_state'), 'status': 'TODO' if inventory_only else 'INFERENCE',
                'work_status': 'TODO' if inventory_only else 'DONE',
                'reason': 'Original MGP GameObject gametype=Character and template/CharacterProperties fields; level load, procedural choice and spawn/script conditions are not evaluated.'})
    return result


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


def _actor_table_links(row, cell_refs, table_rows):
    fields = row.get('fields') or {}
    links = {}
    for (table, index, field), ref in cell_refs.items():
        if table != 'character_properties#group0' or index != int(row.get('index', -1)):
            continue
        targets = ref.get('resolves_to') or []
        if not targets:
            continue
        target = targets[0]
        target_table = target.get('table')
        target_index = target.get('index')
        target_row = table_rows.get((target_table, target_index), {})
        links[field] = {'raw': fields.get(field), 'table': target_table,
                        'index': target_index, 'name': target_row.get('name'),
                        'target_id': target_row.get('id'), 'reference_id': ref.get('id'),
                        'status': 'INFERENCE' if len(targets) > 1 else 'RESOLVED'}
    def value(field, *fallbacks):
        if field in links:
            link = links[field]
            label = link.get('name') or f"{link.get('table')}[{link.get('index')}]"
            return f"{label} [{link.get('index')}]"
        raw = _field(row, field, *fallbacks)
        return _str(raw)
    sounds = {key: val for key, val in fields.items()
              if re.search(r'sound|sfx|voice', str(key), re.I) and val not in (None, -1, '')}
    def profile(field):
        link = links.get(field)
        if not link:
            return {}
        target = table_rows.get((link.get('table'), link.get('index')), {})
        return {'name': link.get('name'), 'table': link.get('table'),
                'index': link.get('index'), 'fields': target.get('fields') or {},
                'source_status': target.get('status', 'TODO'),
                'reference_status': cell_refs.get(('character_properties#group0',
                    int(row.get('index', -1)), field), {}).get('status', 'TODO')}
    ai_profile = profile('AI')
    ai_fields = ai_profile.get('fields') or {}
    sound_link = profile('Sounds')
    return {
        'field_links': links,
        'model': value('ModelFile', 'Model', 'ModelName'),
        'animation_table': value('AnimTable', 'AnimationTable'),
        'ai': value('AI', 'AIType', 'AIName'),
        'ai_profile': ai_profile,
        'ai_script': ai_fields.get('Script'),
        'ai_type': ai_fields.get('Type'),
        'skills': value('SkillTree', 'Skills'),
        'loot': value('Loot', 'LootTable'),
        'sound_profile': sound_link,
        'sounds': sounds,
    }


def _str(v):
    if isinstance(v, (dict, list)):
        return json.dumps(v, ensure_ascii=False, sort_keys=True)
    return '' if v is None else str(v)


def _actor_ai_summary(actor):
    fields = actor.get('ai_profile', {}).get('fields') or {}
    keys = ('Script', 'Type', 'Flags', 'CombatMusic', 'OnAggroSFX', 'SelfFX', 'Trophy')
    return _str({key: fields[key] for key in keys if key in fields})


def _actor_sound_summary(actor):
    link = actor.get('sound_profile') or {}
    if not link:
        return _str(actor.get('sounds', {}))
    name = link.get('name') or f"{link.get('table')}[{link.get('index')}]"
    return f"{name} (reference {link.get('reference_status', 'TODO')}; source row {link.get('source_status', 'TODO')})"


def _record_actor(row, cell_refs=None, table_rows=None):
    fields = row.get('fields') or {}
    out = dict(id=row.get('id'), index=row.get('index'), name=row.get('name'),
               fields=fields, source_table='character_properties',
               status=row.get('status', 'TODO'), work_status=row.get('work_status', 'TODO'))
    out.update(_actor_table_links(row, cell_refs or {}, table_rows or {}))
    return out


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
    for rel in ('general', 'shared', 'unplaced', 'reports', *ACTS):
        (out / rel).mkdir(parents=True, exist_ok=True)
    source = inputs / 'assets-extra/android/data/scene'
    if not source.is_dir():
        raise RuntimeError(f'Android original scene data not found: {source}')

    tables = read_json(out / 'data/tables.json', []) or []
    tables_extra = read_json(out / 'data/tables-extra.json', []) or []
    extra_by_id = {r.get('id'): r for r in tables_extra}
    for row in tables:
        extra = extra_by_id.get(row.get('id'))
        if extra:
            row['fields'] = extra.get('fields', row.get('fields', {}))
            row['status'] = extra.get('status', row.get('status'))
            row['work_status'] = extra.get('work_status', row.get('work_status'))
    table_rows = {(r.get('table'), r.get('index')): r for r in tables + tables_extra}
    cell_rows = read_json(out / 'data/reference-cells-extra.json', []) or []
    cell_refs = {}
    for ref in cell_rows:
        context = ref.get('context') or {}
        key = (context.get('table'), context.get('index'), context.get('field'))
        if key[0] and key[1] is not None and key[2]:
            cell_refs[key] = ref
    refs = read_json(out / 'data/references.json', []) or []
    source_assets = read_json(out / 'data/assets.json', []) or []
    scripts = read_json(out / 'data/scripts.json', []) or []
    script_refs = read_json(out / 'data/script-references.json', []) or []
    by_table = _table_map(tables)
    scene_records = [_scene_record(ctx, p) for p in sorted(source.iterdir())
                     if p.is_file() and p.suffix.lower() in ('.mlx', '.xml')
                     and p.name != '.nomedia']
    if inventory_only:
        for scene in scene_records:
            scene['act'] = 'unplaced'
    level_table_rows = [r for r in by_table.get('levels', []) if r.get('source_group') in ('group0','group1')]
    level_rows_by_file = {}
    for level_row in level_table_rows:
        level_file = (level_row.get('fields') or {}).get('LevelFile')
        if level_file:
            level_rows_by_file.setdefault(Path(str(level_file)).name.casefold(), []).append(level_row)
    scene_membership, route_regions = _derive_act_membership(level_table_rows, scene_records)
    if not inventory_only:
        for scene in scene_records:
            act, evidence = scene_membership.get(scene['id'], ('unplaced', []))
            scene['act'] = act
            scene['act_status'] = 'INFERENCE' if act != 'unplaced' else 'UNRESOLVED'
            scene['act_reason'] = ('Explicit task/handoff progression evidence; act boundary remains INFERENCE.'
                                   if act != 'unplaced' else
                                   'No unique source-backed transition-chain assignment; left unplaced.')
            scene['act_work_status'] = 'DONE'
            scene['membership_confidence'] = 'LIKELY' if act != 'unplaced' else 'UNRESOLVED'
            scene['membership_evidence'] = evidence
    actor_rows = [_record_actor(r, cell_refs, table_rows) for r in by_table.get('character_properties', []) if r.get('source_group') == 'group0' and int(r.get('index', -1)) < 448]
    actor_by_name = {str(a.get('name', '')).casefold(): a for a in actor_rows}
    template_by_name = {str(r.get('name', '')).casefold(): r for r in by_table.get('character_templates', [])}
    for scene in scene_records:
        scene['module_spawn_candidates'] = _module_character_candidates(ctx, scene, actor_by_name, template_by_name, inventory_only)
    for actor in actor_rows:
        actor['act'] = 'unplaced'
        actor['act_assignment_status'] = 'UNRESOLVED'
        actor['act_assignment_reason'] = 'No source-backed spawn-to-level link established.'

    # Actual level table names are kept beside scene XML names: they are separate
    # original indices and must not be conflated with editor-export filenames.
    scene_level_rows = []
    for sc in scene_records:
        scene_level_rows.append({**sc, 'record_type': 'scene_file',
            'linked_level_rows': [r.get('id') for r in level_rows_by_file.get(sc['file'].casefold(), [])],
            'linked_level_evidence': [r.get('schema_evidence') for r in level_rows_by_file.get(sc['file'].casefold(), [])]})
    scene_by_filename = {s['file'].casefold(): s for s in scene_records}
    table_level_rows = []
    for r in level_table_rows:
        level_file = Path(str((r.get('fields') or {}).get('LevelFile') or '')).name.casefold()
        linked_scene = scene_by_filename.get(level_file)
        table_level_rows.append({**r, 'record_type': 'levels_table_row',
            'act': linked_scene.get('act', 'unplaced') if linked_scene else 'unplaced',
            'act_status': ('INFERENCE' if linked_scene and linked_scene.get('act') != 'unplaced' else 'UNRESOLVED'),
            'source_table': 'levels', 'source': 'pydata/levels_pyarray.bin',
            'status': ('INFERENCE' if linked_scene and linked_scene.get('act') != 'unplaced' else r.get('status', 'UNRESOLVED')),
            'work_status': r.get('work_status', 'DONE'),
            'linked_scene': linked_scene.get('file') if linked_scene else None,
            'link_evidence': r.get('schema_evidence') if linked_scene else None})
    levels = scene_level_rows + table_level_rows

    # Explicit act assignment is required for actual placements. Current scene
    # exports do not contain character spawns (the campaign script files named by
    # scriptFile are outside this Android data capture), so actor rows remain
    # unplaced pending an authoritative spawn link.
    inferred_levels = defaultdict(list)
    for scene in scene_records:
        if scene['act'] != 'unplaced':
            inferred_levels[scene['act']].append(scene['name'])
    membership = {
        'schema': 'atlas-act-membership-v1',
        'boundary_policy': 'All act boundaries are INFERENCE until owner confirmation.',
        'inferences': [
            {'act': act, 'level_stems': sorted(inferred_levels.get(act, [])),
             'confidence': 'LIKELY' if inferred_levels.get(act) else 'UNRESOLVED',
             'evidence': (['coordination/v21.8/ATLAS-CODEX-PROMPT.md: Act 1 is Swamp + Witch Cave and ends at the Darkwood load',
                           'coordination/v21.7/B155-B156-HANDOFF.md: B155 documents Mud Lord completion and 003_darkwood.mlx transition']
                          if act in ('act_1', 'act_2') else []),
             'note': ('Only explicit task scope or the directly documented post-boss load is included; all other scenes remain unplaced.'
                      if inferred_levels.get(act) else 'No unique transition-chain evidence assigns a scene to this act; generated page is intentionally empty.')}
            for act in ACTS
        ],
        'actor_assignments': [],
        'actor_assignment_gap': 'Serialized Script_SpawnCharacter targets are available from paired script binaries, but authored object-name arguments do not automatically identify CharacterProperties IDs. Keep actor rows unplaced until an exact source-backed link or handoff evidence exists.'
    }
    if inventory_only:
        membership['inferences'] = []
        membership['boundary_rows'] = [{'act': f'act_{i}', 'status': 'UNRESOLVED', 'work_status': 'DONE',
            'reason_code': 'OTHER', 'label': 'INFERENCE', 'confidence': 'UNRESOLVED',
            'evidence': [], 'candidates': [], 'reason': 'Inventory checkpoint: original transition-chain boundary analysis is pending.'} for i in range(1,6)]
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
    named_boss_candidates = [a for a in actor_rows if re.search(r'boss|midboss', a['name'] or '', re.I)
                             or (a['name'] or '').casefold() in extra_boss_names]
    def has_original_boss_flag(actor):
        fields = (actor.get('ai_profile') or {}).get('fields') or {}
        try:
            return bool(int(fields.get('Flags') or 0) & 4)
        except (TypeError, ValueError):
            return False
    boss_candidates = list({a['id']: a for a in
        [x for x in actor_rows if has_original_boss_flag(x)] + named_boss_candidates}.values())
    boss_flag_ids = {a['id'] for a in actor_rows if has_original_boss_flag(a)}

    def boss_inventory_row(actor, refreshed=None):
        row = {**(refreshed or actor)}
        is_flagged = row.get('id') in boss_flag_ids or has_original_boss_flag(row)
        row.update(classification=('ORIGINAL_BOSS_FLAG_CANDIDATE' if is_flagged else 'NAME_ONLY_CANDIDATE'),
            classification_basis=('Native HUD boss marker: (CharacterProperties.AI -> AI.Flags & 4) != 0; '
                'predicate source: port/level-world/canonical_character_hud_v62.cpp:14.' if is_flagged else
                'Supplemental BOSS/MIDBOSS name marker; source encounter role is not proven.'),
            boss_flag_status='PROVEN' if is_flagged else 'UNRESOLVED',
            boss_flag_source='port/level-world/canonical_character_hud_v62.cpp:14' if is_flagged else None,
            status=('PROVEN' if is_flagged else 'UNRESOLVED') if not inventory_only else 'TODO',
            work_status='DONE' if not inventory_only else 'TODO',
            reason_code=None if is_flagged or inventory_only else 'OTHER',
            reason=('Original HUD predicate marks the linked AI row as boss-flagged; this is source classification evidence, not an encounter placement.'
                    if is_flagged and not inventory_only else
                    'Name-only candidate lacks the original boss flag predicate.' if not inventory_only else ''))
        return row
    bosses = [boss_inventory_row(a) for a in boss_candidates]

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
    write_json(out / 'data/bosses.json', bosses)
    write_json(out / 'data/act-membership.json', membership)
    import campaign
    campaign_counts = campaign.run(ctx, inventory_only=inventory_only)
    actor_rows = read_json(out / 'data/actors.json', []) or actor_rows
    membership = read_json(out / 'data/act-membership.json', {}) or membership
    if not inventory_only:
        module_actor_evidence = defaultdict(list)
        for scene in scene_records:
            if scene.get('act') == 'unplaced':
                continue
            for candidate in scene.get('module_spawn_candidates', []):
                if candidate.get('actor_id'):
                    module_actor_evidence[candidate['actor_id']].append({
                        'act': scene['act'], 'scene': scene['file'],
                        'module': candidate['module'], 'object_name': candidate['object_name'],
                        'template': candidate.get('template'), 'spawn_prob': candidate.get('spawn_prob'),
                        'status': 'INFERENCE', 'reason': candidate['reason']})
        assignments = membership.setdefault('actor_assignments', [])
        assigned_pairs = {(r.get('actor_id'), r.get('act')) for r in assignments}
        for actor in actor_rows:
            evidences = module_actor_evidence.get(actor.get('id'), [])
            if not evidences:
                continue
            acts = sorted({e['act'] for e in evidences})
            actor['module_spawn_candidates'] = evidences
            actor['act_candidates'] = acts
            if not actor.get('scene_membership') or actor.get('scene_membership') == 'unplaced':
                actor['act'] = acts[0] if len(acts) == 1 else 'shared'
                actor['scene_membership'] = acts[0] if len(acts) == 1 else 'shared'
                actor['shared_acts'] = acts if len(acts) > 1 else []
                actor['act_assignment_status'] = 'INFERENCE'
                actor['act_assignment_reason'] = 'Original modular XML contains matching CharacterProperties reference in a scene module; random/procedural/activation conditions are not evaluated.'
            for evidence in evidences:
                pair = (actor['id'], evidence['act'])
                if pair not in assigned_pairs:
                    assignments.append({'actor_id': actor['id'], 'actor_name': actor.get('name'),
                        'act': evidence['act'], 'status': 'INFERENCE', 'work_status': 'DONE', 'confidence': 'LIKELY',
                        'source': evidence['module'], 'scene': evidence['scene'],
                        'candidate_type': 'conditional_or_static_module_character',
                        'reason': evidence['reason']})
                    assigned_pairs.add(pair)
        write_json(out / 'data/actors.json', actor_rows)
        # The handoff files identify the two Act 1 encounters by CharacterTable
        # row and document the swamp/Cave progression. These are explicit
        # task-evidence assignments; act membership itself stays INFERENCE.
        boss_evidence = {
            'swampking': ('coordination/v21.7/B155-B156-HANDOFF.md',
                          'B155 links CharacterTable SwampKing (row 358) to the Mud Lord encounter and Darkwood transition.'),
            'witch': ('coordination/v21.7/B155-B156-HANDOFF.md',
                      'B156 identifies the Witch encounter in the Witch Cave progression.')}
        assignments = membership.setdefault('actor_assignments', [])
        for actor in actor_rows:
            evidence = boss_evidence.get(str(actor.get('name', '')).casefold())
            if evidence:
                if actor.get('scene_membership') in (None, 'unplaced'):
                    actor.update(act='act_1', scene_membership='act_1', shared_acts=[],
                                 act_assignment_status='INFERENCE', act_assignment_reason=evidence[1])
                handoff_source = evidence[0]
                if not any(r.get('actor_id') == actor.get('id') and r.get('source') == handoff_source
                           for r in assignments):
                    assignments.append({'actor_id': actor['id'], 'actor_name': actor['name'],
                        'act': 'act_1', 'status': 'INFERENCE', 'work_status': 'DONE', 'confidence': 'LIKELY',
                        'source': handoff_source, 'candidate_type': 'handoff_encounter_link',
                        'reason': evidence[1]})
        write_json(out / 'data/actors.json', actor_rows)
        refreshed_actors = {a.get('id'): a for a in actor_rows}
        bosses = [boss_inventory_row(a, refreshed_actors.get(a.get('id'), a)) for a in boss_candidates]
        write_json(out / 'data/bosses.json', bosses)
        membership['actor_assignments'] = assignments
        membership['actor_assignment_gap'] = ('Serialized Script_SpawnCharacter targets are recorded as authored object names; exact table-name matches and task-evidence Act 1 boss rows are linked. '
            'Other actor placement, branch execution, and runtime spawn conditions remain unresolved.')
        membership['boundary_rows'] = [
            {'act': act, 'status': 'UNRESOLVED', 'work_status': 'DONE',
             'reason_code': 'OTHER', 'label': 'INFERENCE',
             'confidence': 'LIKELY' if act == 'act_1' else 'GUESS',
             'reason': 'No authored act marker is decoded; this navigation bucket is an explicit assumption awaiting owner confirmation.',
             'candidates': [],
             'evidence': ['pydata/levels_pyarray.bin ordered fast-travel rows and LevelName rows',
                          'coordination/v21.8/ATLAS-CODEX-PROMPT.md: swamp + Witch Cave -> Mud Lord -> Darkwood scope',
                          'coordination/v21.7/B155-B156-HANDOFF.md: B155 quest completion and 003_darkwood.mlx transition']}
            for act in ACTS]
        membership['level_rows'] = [
            {'scene_id': s['id'], 'scene': s['file'], 'act': s['act'],
             'status': 'INFERENCE' if s['act'] != 'unplaced' else 'UNRESOLVED',
             'work_status': 'DONE',
             'confidence': s.get('membership_confidence', 'UNRESOLVED'),
             'evidence': s.get('membership_evidence', []),
             'reason': ('Explicit task/handoff progression link; act placement remains INFERENCE.'
                        if s['act'] != 'unplaced' else
                        'No unique source LevelName/fast-travel link established, or scene is debug/backup/demo.')}
            for s in scene_records]
        write_json(out / 'data/act-membership.json', membership)

    # Table-backed general navigation pages. These are generated views over the
    # source inventories; unresolved table payloads stay visibly unresolved.
    def grouped(table_names):
        wanted = set(table_names)
        return [r for r in tables if r.get('table') in wanted]
    def table_rows_page(path, title, rows, headers=None, project=None):
        headers = headers or ['Table', 'Index', 'Name', 'Status']
        project = project or (lambda r: [r.get('table'), r.get('index'), r.get('name'), r.get('work_status', r.get('status'))])
        (out / 'general' / path).write_text('# ' + title + '\n\n' + markdown_table(headers, [project(r) for r in rows]) + '\n', encoding='utf-8')

    game_state_rows = grouped({'levels#group0', 'levels#group1', 'worldmap#group0', 'worldmap#group1',
                               'v2quests#group0', 'v2conditions#group0', 'v2eventmanager#group0',
                               'common#group0', 'common_text#group0', 'design#group0', 'design#group1', 'design#group2'})
    table_rows_page('game_state.md', 'Game state, quests, levels and world map', game_state_rows,
                    ['Table', 'Index', 'Name', 'Decoded fields', 'Status'],
                    lambda r: [r.get('table'), r.get('index'), r.get('name'), _str(r.get('fields', {})), r.get('work_status', r.get('status'))])
    skill_rows = grouped({'skills#group0', 'skills#group1', 'item_powers#group0', 'item_powers#group1', 'item_powers_monopoly#group0'})
    table_rows_page('skills.md', 'Skills and item powers', skill_rows,
                    ['Table', 'Index', 'Name', 'Script / fields', 'Status'],
                    lambda r: [r.get('table'), r.get('index'), r.get('name'), _str(r.get('fields', {})), r.get('work_status', r.get('status'))])
    faery_rows = grouped({'faeries#group0', 'faeries#group1'})
    table_rows_page('faeries.md', 'Faeries', faery_rows,
                    ['Table', 'Index', 'Name', 'Model / spell / fields', 'Status'],
                    lambda r: [r.get('table'), r.get('index'), r.get('name'), _str(r.get('fields', {})), r.get('work_status', r.get('status'))])
    item_rows = [r for r in tables if r.get('table', '').startswith(('loot_table#', 'item_powers#', 'item_powers_monopoly#', 'loot_audiovisual#', 'trophies#'))]
    table_rows_page('items.md', 'Items, loot and trophies', item_rows,
                    ['Table', 'Index', 'Name', 'Decoded fields', 'Status'],
                    lambda r: [r.get('table'), r.get('index'), r.get('name'), _str(r.get('fields', {})), r.get('work_status', r.get('status'))])
    fx_rows = grouped({'projectiles#group0', 'projectiles_dictionary#group0', 'effects_dictionary#group0',
                       'effect_sets', 'character_effects', 'animations_dictionary#group0'})
    table_rows_page('projectiles-effects.md', 'Projectiles, effects and animation assets', fx_rows,
                    ['Table', 'Index', 'Name', 'Decoded fields', 'Status'],
                    lambda r: [r.get('table'), r.get('index'), r.get('name'), _str(r.get('fields', {})), r.get('work_status', r.get('status'))])
    audio_rows = [r for r in tables if r.get('table', '').startswith('sounds#') or r.get('table') in ('sdd_dungeon_hunter_2_iphone', 'footstep_effects')]
    table_rows_page('audio.md', 'Audio names, sound rows and event mapping', audio_rows,
                    ['Table', 'Index', 'Name', 'Fields', 'Status'],
                    lambda r: [r.get('table'), r.get('index'), r.get('name'), _str(r.get('fields', {})), r.get('work_status', r.get('status'))])
    script_rows = read_json(out / 'data/scripts.json', []) or []
    table_rows_page('common-scripts.md', 'Common and custom scripts', script_rows,
                    ['Script', 'Source', 'Lines / bytes', 'Engine call sites', 'Readiness', 'Status'],
                    lambda r: [r.get('name'), r.get('site'), f"{r.get('lines', 0)} / {r.get('bytes', 0)}", r.get('engine_call_sites', r.get('call_sites', 0)), _str(r.get('readiness', {})), r.get('work_status', r.get('status'))])
    table_rows_page('bosses.md', 'Boss and named encounter candidates', bosses,
                    ['Index', 'Candidate', 'AI row', 'AI facts', 'Skill tree', 'Loot', 'Original flag status', 'Basis'],
                    lambda r: [r.get('index'), r.get('name'), r.get('ai'), _actor_ai_summary(r),
                               r.get('skills'), r.get('loot'), r.get('boss_flag_status'), r.get('classification_basis')])
    function_rows = read_json(out / 'data/functions.json', []) or []
    ranked_functions = sorted(function_rows, key=lambda r: (-r.get('call_count', 0), r.get('name', '')))
    table_rows_page('native-map.md', 'Native handler and port map', ranked_functions,
                    ['Function', 'Native handler / addresses', 'Port status', 'Call sites', 'Status'],
                    lambda r: [r.get('name'), '; '.join(r.get('native_handlers', [])) + ' ' + ','.join(x.get('address', '') for x in r.get('native_addresses', [])), r.get('port_status'), r.get('call_count'), r.get('work_status', r.get('status'))])
    def count_records(records, status_field='status'):
        states = Counter(r.get(status_field, r.get('status', 'TODO')) for r in records)
        if not states and records: states['TODO'] = len(records)
        return {'total': len(records), 'todo': states.get('TODO', 0),
                'processed': len(records) - states.get('TODO', 0), 'states': dict(states)}
    module_candidate_count = sum(len(s.get('module_spawn_candidates', [])) for s in scene_records)
    mob_actor_rows = [a for a in actor_rows if a.get('ai_type') == 4]
    flagged_boss_rows = [a for a in actor_rows if has_original_boss_flag(a)]
    campaign_records = read_json(out / 'data/campaign.json', []) or []
    campaign_refs = read_json(out / 'data/campaign-references.json', []) or []
    write_json(out / 'counts/views.json', {
        'category': 'ACT-ORGANISED GENERATED VIEWS',
        'work_status': 'DONE',
        'inventory': {
            'scene_level_rows': count_records(scene_level_rows),
            'campaign_scene_rows': count_records([s for s in scene_level_rows if s['source_kind'] == 'campaign']),
            'debug_scene_rows': count_records([s for s in scene_level_rows if s['source_kind'] == 'debug']),
            'backup_scene_rows': count_records([s for s in scene_level_rows if s['source_kind'] == 'backup']),
            'demo_scene_rows': count_records([s for s in scene_level_rows if s['source_kind'] == 'demo']),
            'levels_table_rows': count_records(level_table_rows),
            'character_properties_actor_rows': count_records(actor_rows),
            'mob_actor_rows_ai_type_4': {'total': len(mob_actor_rows), 'status': 'DONE',
                'rule': 'CharacterProperties AI row Type == 4; this is an AI category count, not a runtime spawn count.'},
            'boss_flag_actor_rows': {'total': len(flagged_boss_rows), 'status': 'PROVEN',
                'rule': 'CharacterProperties.AI -> AI.Flags bit 4; native HUD predicate at port/level-world/canonical_character_hud_v62.cpp:14.'},
            'supplemental_name_only_boss_candidates': {'total': sum(1 for a in bosses if a.get('boss_flag_status') != 'PROVEN'),
                'status': 'UNRESOLVED', 'rule': 'BOSS/MIDBOSS name markers outside the source-proven AI flag set.'},
            'boss_candidate_actor_rows': {**count_records(read_json(out / 'data/bosses.json', []) or []),
                'definition': 'BOSS/MIDBOSS name markers or explicitly named encounter actors; candidates only'},
            'dialogue_rows': count_records(by_table.get('dialogs', [])),
            'source_script_rows': count_records(scripts),
            'script_reference_rows': count_records(script_refs),
            'asset_reference_rows': count_records(refs),
            'module_character_candidates': {'total': module_candidate_count, 'status': 'INFERENCE',
                'note': 'Authored MGP candidates; conditional/random module selection and actor scripts are not executed.'},
            'campaign_numeric_references': count_records([r for r in campaign_refs if r.get('rule') == 'CAMPAIGN_TYPED_NUMERIC_OPERAND']),
            'campaign_string_references': count_records([r for r in campaign_refs if r.get('rule') == 'CAMPAIGN_SCRIPT_STRING_FIELD']),
            'serialized_campaign_script_files': {'total': campaign_counts['script_file_rows'],
                'decoded': campaign_counts['decoded_script_files'], 'scripts': campaign_counts['scripts'],
                'commands': campaign_counts['commands'], 'candidate_string_references': campaign_counts['candidate_string_references'],
                'missing_pairs': campaign_counts['missing_pairs'], 'decode_failures': campaign_counts['decode_failures'],
                'states': dict(Counter(r.get('work_status', r.get('status')) for r in campaign_records)),
                'by_source': campaign_counts.get('by_source', {})},
        },
        'boundary_rows': {'total': 5, 'todo': 0, 'processed': 5, 'unresolved': 5, 'label': 'INFERENCE'},
        'actor_act_membership_rows': count_records(actor_rows, 'act_assignment_status'),
        'notes': ['Counts are inventory, not completion claims.',
                  'INFERENCE rows represent generated navigation/candidate memberships, not confirmed boundaries or runtime spawns.',
                  'Boss rows are unresolved classification candidates, not confirmed encounter roles.']
    })
    if inventory_only:
        return {'scene_level_rows': level_inventory, 'levels_table_rows': len(level_table_rows), 'campaign_scene_rows': campaign_count,
                'debug_scene_rows': debug_count, 'backup_scene_rows': backup_count,
                'character_properties_actor_rows': len(actor_rows), 'boss_candidates': len(bosses)}

    # Produce navigable directories. The detail layer is intentionally limited to
    # Act 1 and the next inferred region (Act 2).
    for act in ACTS:
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
            campaign_rows = [r for r in read_json(out / 'data/campaign.json', []) if r.get('act') == act]
            placed_actors = [a for a in actor_rows if a.get('scene_membership') == act]
            placed_bosses = [a for a in bosses if any(x['id'] == a['id'] for x in placed_actors)]
            spawn_target_count = sum(len(r.get('spawn_targets', [])) for r in campaign_rows)
            dialog_command_count = sum(1 for r in campaign_rows for script in r.get('scripts', [])
                                       for command in script.get('commands', [])
                                       if command.get('name') == 'Script_StartDialog')
            module_spawn_rows = [dict(scene=sc['file'], **candidate) for sc in scene_rows
                                  for candidate in sc.get('module_spawn_candidates', [])]
            text += ['## Actors and mobs', '',
                     'CharacterProperties rows are linked from exact source-script names, MGP `charpropsname`, or explicit B155/B156 encounter evidence. Other MGP Character objects and `Script_SpawnCharacter` arguments remain conditional candidates; command branches and runtime execution are not evaluated.', '',
                     markdown_table(['Actor rows', 'Mapped to this act', 'Status'],
                        [[len(actor_rows), len(placed_actors), 'INFERENCE: serialized spawn-command targets']]),
                     f"Serialized Script_SpawnCharacter authored targets: {spawn_target_count}; modular XML Character candidates: {len(module_spawn_rows)}. Both are conditional candidates, not a static runtime roster.", '',
                     '### Modular XML Character candidates', '',
                     'Rows come from MGP files referenced directly by scene modules or by procedural-rule gameplay choices. `charpropsname` is an exact CharacterProperties link where present; `char_template` is kept as a template label when the CharacterProperties link is not decoded. Random module selection and `spawn_prob` are preserved as conditions.', '',
                     markdown_table(['Scene', 'Module source', 'Object', 'CharacterProperties', 'Template', 'Spawn probability', 'AI state', 'Status'],
                        [[m['scene'], m['module'], m.get('object_name'), m.get('actor_name') or m.get('charpropsname'),
                          m.get('template'), m.get('spawn_prob'), m.get('ai_state'), m.get('status')]
                         for m in module_spawn_rows]), '',
                     '### CharacterProperties profiles', '',
                     'This table joins listed actor rows to their original table references. AI script/type values come from the linked AI table row; loot, skills, model and animation remain source-table links. It does not prove the actor spawns in this level.', '',
                     markdown_table(['Actor', 'Model', 'Animation table', 'AI row', 'AI facts', 'Skill tree', 'Loot', 'Sounds'],
                        [[a['name'], a.get('model'), a.get('animation_table'), a.get('ai'),
                          _actor_ai_summary(a), a.get('skills'), a.get('loot'),
                          _actor_sound_summary(a)] for a in placed_actors]), '',
                     '## Bosses', '',
                     'Boss candidates use explicit BOSS/MIDBOSS name markers or named encounter actors. Act 1 encounter rows are linked through B155/B156 handoff evidence; AI script/type is shown in the CharacterProperties profile table. Remaining boss classification and encounter semantics stay unresolved.', '',
                     markdown_table(['Candidate rows', 'Linked spawn targets', 'Status'], [[len(bosses), len(placed_bosses), 'INFERENCE: see campaign references']]), '',
                     '## Dialogues', '',
                     'The dialog table is inventoried globally. Serialized `Script_StartDialog` occurrences are listed as command rows; mapping them to dialog table IDs remains unresolved.', '',
                     f"Dialog table rows: {len(by_table.get('dialogs', []))}; StartDialog commands: {dialog_command_count}.", '',
                     '## Serialized level scripts', '',
                     'The configured `.pyscript` names resolve to original serialized `_pyscriptnames.bin` and `_pyscripts.bin` pairs. Atlas decodes script names, command kinds, and raw fields without executing callbacks.', '',
                     markdown_table(['Scene', 'Script file', 'Scripts', 'Commands', 'Decode status'],
                        [[r.get('scene'), r.get('script_file'), r.get('script_count', 0),
                          r.get('command_count', 0), r.get('status')] for r in campaign_rows]), '',
                     '### Command readiness by native kind', '',
                     'The native factory/reader addresses come from the recovered C1 descriptor map. Port anchors identify current dispatch code only; PARTIAL/UNKNOWN does not establish complete source parity or runtime verification.', '',
                     markdown_table(['Kind', 'Command', 'Count', 'Factory address', 'Reader address', 'Port status', 'Code anchors'],
                        [[c.get('kind'), c.get('name'), c.get('count'), c.get('native_factory_address'),
                          c.get('native_reader_address'), c.get('port_status'), ', '.join(c.get('port_path_anchors', []))]
                         for c in sorted((c for r in campaign_rows for c in r.get('command_readiness', [])),
                                         key=lambda x: (x.get('kind', -1), x.get('name', '')))]), '',
                     '### Actor, dialogue, transition, and audio command details', '',
                     'The table below preserves exact command labels and decoded string operands. A `Script_SpawnCharacter` target is an authored object-name argument; it does not prove which conditional branch ran in gameplay.', '']
            relevant_prefixes = ('Script_SpawnCharacter', 'Script_LookActor', 'Script_SetActorPosition',
                                 'Script_PlayActorAnim', 'Script_HideActor', 'Script_ShowActor',
                                 'Script_MoveActor', 'Script_KillActor', 'Script_PutCharacterInIdle',
                                 'Script_StartDialog', 'Script_WaitDialog', 'Script_ChangeLevel',
                                 'Script_PlaySound', 'Script_StopSound', 'Script_PlayLevelMusic',
                                 'Script_SetLevelState', 'Script_SetWorldMapLocationState',
                                 'Script_OpenDoor', 'Script_CloseDoor')
            summary = Counter()
            for campaign_row in campaign_rows:
                for script in campaign_row.get('scripts', []):
                    for command in script.get('commands', []):
                        if command.get('name') not in relevant_prefixes:
                            continue
                        string_values = [str(v) for v in command.get('fields', {}).values() if isinstance(v, str) and v]
                        summary[(campaign_row.get('scene'), script.get('name'), command.get('name'),
                                 '; '.join(string_values))] += 1
            text += [markdown_table(['Scene', 'Script name', 'Command', 'String operands', 'Count'],
                        [[*key, count] for key, count in sorted(summary.items())]), '',
                     'Raw strings are recorded in `../data/campaign-references.json`. Command labels and recovered field offsets identify spawn, transition, dialogue, and audio command records; runtime branch conditions and dialogue-table IDs remain unresolved.', '']
        detail_assets = asset_pages.get(act, [])
        if act in ('act_1', 'act_2'):
            text += ['## Source-linked asset detail pages', '',
                     'Each page joins the source asset inventory to scene/MGP reference rows and carries the resolver status, confidence, rule, port metadata, and source path.', '',
                     markdown_table(['Asset', 'Stable ID', 'Source status', 'Page'],
                         [[a.get('relative') or a.get('path'), a.get('id'), a.get('status'), f"[details]({link})"]
                          for a, link in detail_assets]), '']
        (target / 'index.md').write_text('\n'.join(text), encoding='utf-8')

    # Unplaced comprises all debug scenes, backups, demo scene, and any scene
    # not covered by an inferred campaign bucket.
    unplaced = [s for s in scene_level_rows if s['source_kind'] in ('debug', 'backup', 'demo') or s['act'] == 'unplaced']
    (out / 'unplaced/index.md').write_text('# Unplaced and non-campaign scenes\n\n' +
        '> Debug levels, backups, demo scenes, and scenes without a provisional campaign bucket are not assigned to an act.\n\n' +
        markdown_table(['File', 'Category', 'Source', 'Status'],
                       [[s['file'], s['source_kind'], s['source'], s['status']] for s in unplaced]),
        encoding='utf-8')

    (out / 'general/README.md').write_text(
        '# Act-organised Original Data Atlas views\n\n'
        '> Generated from original level tables, Android scene exports, and the Atlas source inventories. Act boundaries are not authored in the parsed level table; all assignments shown here are INFERENCE.\n\n'
        '## Browse\n\n'
        + '\n'.join(f'- [Act {i}](../act_{i}/index.md)' for i in range(1, 6))
        + '\n- [Game state, quests and world map](game_state.md)\n- [Skills and item powers](skills.md)\n- [Faeries](faeries.md)\n- [Items, loot and trophies](items.md)\n- [Projectiles, effects and animations](projectiles-effects.md)\n- [Audio](audio.md)\n- [Boss candidates](bosses.md)\n- [Common and custom scripts](common-scripts.md)\n- [Native handler map](native-map.md)\n- [Shared assets](../shared/index.md)\n- [Unplaced scenes](../unplaced/index.md)\n- [Level inventory](levels.md)\n- [Character table inventory](characters.md)\n- [View report](../reports/views.md)\n',
        encoding='utf-8')
    (out / 'general/levels.md').write_text('# Original level inventory\n\n' +
        markdown_table(['File', 'Category', 'Act inference', 'Script', 'Music', 'Direct objects', 'Rule lists'],
            [[s.get('file', s.get('name', '')), s.get('source_kind', 'levels_table_row'), s.get('act', 'unplaced'),
              (s.get('config') or {}).get('scriptFile',''),
              (s.get('config') or {}).get('music', (s.get('config') or {}).get('ambiant_music','')),
              len(s.get('direct_objects', [])), len(s.get('procedural_lists', []))] for s in levels]), encoding='utf-8')
    (out / 'general/characters.md').write_text('# CharacterProperties row inventory\n\n' +
        '> Rows named by serialized `Script_SpawnCharacter` commands are linked as INFERENCE candidates. This is not a runtime spawn confirmation or a complete static mob roster.\n\n' +
        markdown_table(['Index', 'Name', 'Model', 'Animation table', 'AI', 'AI facts', 'Skill tree', 'Loot', 'Sounds', 'Status'],
            [[a['index'], a['name'], a['model'], a['animation_table'], a['ai'], _actor_ai_summary(a),
              a['skills'], a['loot'], _actor_sound_summary(a), a['status']] for a in actor_rows]),
        encoding='utf-8')

    # Per-asset detail pages are limited to assets referenced from the two
    # detail acts. Scene-to-MGP source links are included so nested asset
    # references can be joined without relying on a filename guess.
    scene_by_source = {s['source'].replace('\\', '/'): s for s in scene_records}
    source_act = defaultdict(set)
    for scene in scene_records:
        if scene['act'] not in ('act_1', 'act_2'):
            continue
        scene_source = scene['source'].replace('\\', '/')
        source_act[scene_source].add(scene['act'])
        for candidate in scene.get('module_spawn_candidates', []):
            module = str(candidate.get('module') or '').replace('\\', '/')
            if module:
                source_act[module].add(scene['act'])
    assets_by_path = {str(a.get('path', '')).replace('\\', '/').casefold(): a for a in source_assets}
    refs_by_id = {r.get('id'): r for r in refs}
    asset_act_refs = defaultdict(list)
    target_acts = defaultdict(set)
    act_unresolved_refs = Counter()
    for ref in refs:
        file = str((ref.get('context') or {}).get('file') or '').replace('\\', '/')
        acts = source_act.get(file, set())
        if not acts:
            continue
        if ref.get('status') in ('TODO', 'UNRESOLVED', 'AMBIGUOUS'):
            for act in acts:
                act_unresolved_refs[act] += 1
        targets = ref.get('resolves_to') or []
        if not isinstance(targets, list):
            targets = [targets]
        for target in targets:
            if not isinstance(target, dict):
                continue
            source_rows = target.get('asset_files') or []
            for source_row in source_rows:
                path = str(source_row.get('path') or '').replace('\\', '/')
                asset = assets_by_path.get(path.casefold())
                if not asset:
                    continue
                asset_act_refs[asset['id']].append((ref, target))
                for act in acts:
                    target_acts[asset['id']].add(act)

    asset_pages = defaultdict(list)
    page_by_asset = {}
    for rel in ('act_1/assets', 'act_2/assets', 'shared/assets'):
        generated_dir = out / rel
        if generated_dir.is_dir():
            for stale in generated_dir.glob('*.md'):
                stale.unlink()
    for asset_id, contexts in sorted(asset_act_refs.items()):
        asset = next((a for a in source_assets if a.get('id') == asset_id), None)
        if not asset:
            continue
        acts = sorted(target_acts[asset_id])
        all_ref_ids = set(asset.get('referrers') or [])
        rows_for_asset = {}
        for ref, target in contexts:
            rows_for_asset[ref.get('id')] = (ref, target)
        # Retain the complete source/resolution metadata for source refs that
        # name this asset; list unrelated asset-level referrer IDs separately.
        detail_rows = []
        for ref_id, (ref, target) in sorted(rows_for_asset.items()):
            detail_rows.append([ref_id, ref.get('kind'), ref.get('name'), ref.get('site'),
                ref.get('status'), ref.get('work_status'), ref.get('confidence'), ref.get('rule'),
                ref.get('port_status'), _str(ref.get('implementation')), _str(ref.get('reason')),
                _str(target.get('path'))])
        rel = Path('assets') / (asset_id + '.md')
        page = ['# ' + str(asset.get('relative') or asset.get('path')), '',
            f"Stable asset ID: `{asset_id}`. Source: `{asset.get('path')}`.", '',
            markdown_table(['Fact', 'Value'], [
                ['Source corpus', asset.get('source')], ['Relative path', asset.get('relative')],
                ['Size bytes', asset.get('size')], ['SHA-256', asset.get('sha256')],
                ['Inventory status', asset.get('status')], ['Work status', asset.get('work_status')],
                ['Act uses', ', '.join(acts)], ['Source-linked reference rows', len(detail_rows)],
                ['All inventory referrer IDs', len(all_ref_ids)],
            ]), '', '## Source-linked reference and resolution rows', '',
            markdown_table(['ID', 'Kind', 'Name', 'Site', 'Status', 'Work status', 'Confidence',
                            'Rule', 'Port status', 'Implementation', 'Reason', 'Resolved source path'], detail_rows), '',
            'Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.', '']
        for act in acts:
            actdir = out / act / 'assets'
            actdir.mkdir(parents=True, exist_ok=True)
            (actdir / rel.name).write_text('\n'.join(page), encoding='utf-8')
            asset_pages[act].append((asset, rel.as_posix()))
        page_by_asset[asset_id] = rel.as_posix()
        if len(acts) >= 2:
            shared_dir = out / 'shared' / 'assets'
            shared_dir.mkdir(parents=True, exist_ok=True)
            (shared_dir / rel.name).write_text('\n'.join(page), encoding='utf-8')
            page_by_asset[asset_id] = ('../shared/' + rel.as_posix())
    shared_actors = [a for a in actor_rows if a.get('shared_acts')]
    shared_assets = [(asset_id, asset, acts) for asset_id, acts in target_acts.items() if len(acts) >= 2
                     for asset in source_assets if asset.get('id') == asset_id]
    shared_assets.sort(key=lambda row: (row[1].get('relative', ''), row[0]))
    shared_table = [[a.get('relative') or a.get('path'), asset_id, ', '.join(sorted(acts)),
                     f"[detail](assets/{asset_id}.md)"] for asset_id, a, acts in shared_assets]
    (out / 'shared/index.md').write_text('# Shared assets and actors\n\n' +
        '> Shared assets are the same stable source asset referenced from two or more currently assigned act sources. Act labels remain INFERENCE.\n\n' +
        '## Shared asset detail pages\n\n' +
        (markdown_table(['Asset', 'Stable ID', 'Acts', 'Page'], shared_table) if shared_table else
         'No source asset is currently proven to be referenced from multiple assigned acts. The join was run over scene/MGP source-linked reference rows.\n') + '\n' +
        '## Actors named by spawn commands in multiple acts\n\n' +
        markdown_table(['Actor', 'CharacterProperties row', 'Acts', 'Status'],
            [[a['name'], a.get('index'), a.get('shared_acts'), a.get('act_assignment_status')] for a in shared_actors]) + '\n',
        encoding='utf-8')

    # Build a source-joined gap ranking for Act 2 onward. It includes unresolved
    # references plus campaign command kinds whose port status is only PARTIAL
    # or UNKNOWN; addresses and dispatch anchors stay attached to each row.
    command_gaps = []
    for record in campaign_records:
        act = record.get('act')
        if act not in ('act_2', 'act_3', 'act_4', 'act_5'):
            continue
        for command in record.get('command_readiness', []):
            port_status = command.get('port_status') or 'UNKNOWN'
            if port_status not in ('PARTIAL', 'UNKNOWN', 'UNRESOLVED'):
                continue
            name = command.get('name') or ''
            if not re.search(r'(spawn|boss|actor|kill|changelevel|dialog|sound|anim)', name, re.I):
                continue
            command_gaps.append({'act': act, 'category': 'campaign_engine_command_port_gap',
                'source': record.get('source') or record.get('scene'), 'scene': record.get('scene'),
                'name': name, 'kind': command.get('kind'), 'count': command.get('count', 0),
                'status': command.get('status', 'UNRESOLVED'), 'work_status': command.get('work_status', 'DONE'),
                'port_status': port_status, 'native_factory_address': command.get('native_factory_address'),
                'native_reader_address': command.get('native_reader_address'),
                'port_path_anchors': command.get('port_path_anchors', []),
                'source_descriptor': command.get('descriptor_source'),
                'priority': 0 if port_status in ('UNKNOWN', 'UNRESOLVED') else 1})
    ref_gaps = []
    for ref in refs:
        file = str((ref.get('context') or {}).get('file') or '').replace('\\', '/')
        for act in source_act.get(file, set()):
            if act not in ('act_2', 'act_3', 'act_4', 'act_5') or ref.get('status') not in ('TODO', 'UNRESOLVED', 'AMBIGUOUS'):
                continue
            ref_gaps.append({'act': act, 'category': 'unresolved_source_reference',
                'id': ref.get('id'), 'source': ref.get('site'), 'name': ref.get('name'),
                'kind': ref.get('kind'), 'status': ref.get('status'), 'work_status': ref.get('work_status'),
                'confidence': ref.get('confidence'), 'rule': ref.get('rule'),
                'port_status': ref.get('port_status'), 'implementation': ref.get('implementation'),
                'reason': ref.get('reason'), 'priority': 2})
    ranked_gaps = sorted(command_gaps + ref_gaps,
        key=lambda row: (row['priority'], row['act'], -row.get('count', 0), str(row.get('source', '')), str(row.get('name', ''))))
    act_gap_rows = []
    for act in ACTS:
        act_rows = [r for r in ranked_gaps if r['act'] == act]
        act_gap_rows.append({'act': act, 'unresolved_source_references': act_unresolved_refs[act],
            'incomplete_campaign_engine_commands': sum(r['category'] == 'campaign_engine_command_port_gap' for r in act_rows),
            'unmapped_actor_rows_global': len([a for a in actor_rows if a.get('act_assignment_status') != 'INFERENCE']),
            'unresolved_boss_candidates_global': len(bosses),
            'rank_basis': 'Source-joined incomplete campaign command port anchors and unresolved references; command addresses and ref resolver metadata retained.',
            'status': 'DONE'})
    write_json(out / 'data/act-gaps.json', {
        'by_act': sorted(act_gap_rows, key=lambda row: (-row['unresolved_source_references'],
                                                         -row['incomplete_campaign_engine_commands'], row['act'])),
        'top20_acts_2_plus': ranked_gaps[:20],
        'shared_rule': 'A stable asset ID is shared only when its source-linked references occur in at least two explicitly assigned acts.',
        'shared_asset_count': len(shared_assets),
        'ranking_scope': 'Act 2+ only. Campaign engine command rows include native kind, factory/reader address, descriptor source, port status, and current anchors; unresolved source rows include resolver metadata.',
        'status': 'DONE'
    })

    all_status = Counter(s['status'] for s in scene_level_rows)
    report = ['# Act-organised views report', '',
        'Generated by `tools/atlas/views.py` from original Android scene XML plus the Atlas tables/scripts/references JSON and paired campaign-script binaries. Original inputs are read-only.', '',
        '## Inventory', '',
        markdown_table(['Rows', 'Count', 'Current status'], [
            ['Scene XML level/rule files', len(scene_records), dict(all_status)],
            ['Campaign scene rows (numeric <100; excludes demo)', campaign_count, 'Inventoried; assignments are evidence-limited'],
            ['Demo scene rows', sum(s['source_kind'] == 'demo' for s in scene_records), 'Inventoried; unplaced'],
            ['Debug/test scene rows 100–130', debug_count, 'Inventoried; unplaced'],
            ['Backup scenes xNN', backup_count, 'Inventoried; unplaced'],
            ['Levels table rows', len(level_table_rows), 'Inventoried; act link unresolved except explicit chain evidence'],
            ['CharacterProperties rows', len(actor_rows), 'Inventoried; actor placement remains partial'],
            ['Named boss candidates', len(bosses), 'Candidate classification unresolved'],
            ['Dialog rows', len(by_table.get('dialogs', [])), 'Inventoried; command mapping unresolved'],
            ['Serialized level script files', campaign_counts['script_file_rows'], 'DONE decode coverage; see pair gaps below'],
            ['Serialized level scripts', campaign_counts['scripts'], 'DONE decoded names'],
            ['Serialized commands', campaign_counts['commands'], 'DONE decoded data fields; not executed'],
            ['Campaign string-field references', campaign_counts['candidate_string_references'], 'Target semantics unresolved where not source-backed'],
        ]), '',
        f"Campaign script pairs decoded: {campaign_counts['decoded_script_files']}/{campaign_counts['script_file_rows']}; missing pairs: {campaign_counts['missing_pairs']}; decode failures: {campaign_counts['decode_failures']}.",
        'Exact `Script_SpawnCharacter` field-12 arguments are listed as authored object-name targets. They do not establish that a conditional command ran, and object names are not automatically equated with CharacterProperties rows.', '',
        '## Boundary evidence and limits', '',
        '- **Act 1 grouping is an inference constrained by the task instruction.** The instruction names the swamp + witch cave and says the endpoint is the Mud Lord encounter followed by the Darkwood load. B155 handoff directly documents the Mud Lord quest completion, Gothicus-to-Darkwood transition, and `003_darkwood.mlx` load. B157 report documents a separate Witch Cavern E-transition level-up issue but does not establish the overall campaign act boundary.',
        '- **Act 1 includes only `001_swamp` and `002_swamp_witchcave.rule`**, as directed. B155 records the post-Mud-Lord load of `003_darkwood.mlx`, which is shown as the single Act 2 entry point. No region-name or token-similarity matches assign later scenes.',
        '- The fast-travel table has no explicit act field in its decoded schema. Scenes without a unique source-backed transition-chain link remain unplaced; Act 2 onward is intentionally sparse pending additional chain evidence.',
        '- The direct scene XML exposes level config (script path, music, skybox/light/fog), static editor objects and procedural module lists. Referenced MGP XML supplies authored `gametype=Character` candidates, but random module choice, `spawn_prob`, scripts and activation conditions do not establish a runtime roster.',
        '- Level config script paths end in `.pyscript`; the source corpus stores serialized pairs under `_pyscripts.bin` and `_pyscriptnames.bin`. These are decoded into `data/campaign.json` using the recovered `ScriptReaderV52` stream layout and `script_data_schemas_v52.inc`; commands are not executed.',
        '- `data/campaign-references.json` records serialized string fields and exact actor/level-name candidate matches. These matches do not by themselves establish spawns, transitions, dialog selection, or other runtime semantics.',
        '- Decoder provenance: `port/level-loader/level_script_paths_v51.hpp` documents the source `.pyscript` to paired binary path transform; `port/level-loader/script_manager_owner_v52.cpp` and `script_data_schemas_v52.inc` define the bounded little-endian name/command record layout. `port/windows-foundation/features/campaign_host/actor_clip_manifest.hpp` identifies `Script_SpawnCharacter` field 12 as its actor-name argument.',
        '- Scene `.rule.xml` choices and `.mlx` module references are followed to MGP Character records. Candidate rows preserve exact `charpropsname`, `char_template`, `spawn_prob`, and script/AI state values. Missing template payload decoding and branch execution remain explicit gaps.',
        '- B155/B156 handoff evidence links CharacterProperties rows SwampKing (index 358) and Witch (index 435) to the Act 1 encounter scope. Other actor rows are assigned only as INFERENCE candidates from exact `Script_SpawnCharacter` names or MGP `charpropsname` links. No runtime mob count is claimed.', '',
        '## Act-specific detail availability', '',
        '- `act_1/index.md` and `act_2/index.md` include generated per-level detail sections, because only Act 1 and the immediately following inferred region receive detail pages.',
        '- `act_3/index.md` through `act_5/index.md` are generated tables only.',
        '- Debug levels 100–130, `xNN_*_backup`, and demo level 050 are under `unplaced/`.', '',
        '## Status', '',
        'The view generator is complete for the current evidence set. Scene XML, MGP Character candidates, table joins and available campaign command streams have source-backed results. Act boundaries are labeled INFERENCE and unresolved pending owner confirmation; two configured script pairs are unresolved; actor table links include AI script/type where source rows are available; boss classification, dialogue-table mapping, and runtime command conditions remain open.', '']
    (out / 'reports/views.md').write_text('\n'.join(report), encoding='utf-8')
    return {'scene_level_rows': level_inventory, 'campaign_scene_rows': campaign_count,
            'debug_scene_rows': debug_count, 'backup_scene_rows': backup_count,
            'character_properties_actor_rows': len(actor_rows), 'boss_candidates': len(bosses)}


def coverage_check(ctx):
    """Independently verify the source-backed level/actor inventory coverage."""
    out, inputs = Path(ctx.out), Path(ctx.inputs)
    scene_dir = inputs / 'assets-extra/android/data/scene'
    expected_scenes = {p.name for p in scene_dir.iterdir()
                       if p.is_file() and p.suffix.lower() in ('.mlx', '.xml')
                       and p.name != '.nomedia'} if scene_dir.is_dir() else set()
    levels = read_json(out / 'data/levels.json', []) or []
    actors = read_json(out / 'data/actors.json', []) or []
    membership = read_json(out / 'data/act-membership.json', {}) or {}
    errors = []
    scene_rows = [r for r in levels if r.get('record_type') == 'scene_file']
    actual_scenes = [r.get('file') for r in scene_rows]
    if set(actual_scenes) != expected_scenes:
        errors.append(f'views: source scene coverage differs (missing={sorted(expected_scenes-set(actual_scenes))[:5]}, stale={sorted(set(actual_scenes)-expected_scenes)[:5]})')
    if len(actual_scenes) != len(set(actual_scenes)):
        errors.append('views: duplicate scene-file rows')
    if any(not r.get('id') or not r.get('source') or not r.get('status') for r in scene_rows):
        errors.append('views: scene row missing stable id, source, or status')
    name_file = inputs / 'assets-extra/android/data/pydata/character_properties_pyarraynames.bin'
    raw_names = name_file.read_bytes() if name_file.is_file() else b''
    missing_names = [r.get('name') for r in actors
                     if not r.get('name') or r['name'].encode('utf-8') not in raw_names]
    if len(actors) != 448 or missing_names:
        errors.append(f'views: CharacterProperties source-name coverage mismatch (rows={len(actors)}, missing={missing_names[:5]})')
    if len({r.get('id') for r in actors}) != len(actors):
        errors.append('views: duplicate actor ids')
    if any(not r.get('id') or not r.get('name') or not r.get('status') for r in actors):
        errors.append('views: actor row missing stable id, name, or status')
    for row in membership.get('boundary_rows', []):
        if row.get('status') not in ('TODO', 'DONE', 'UNRESOLVED', 'NOT-NEEDED'):
            errors.append(f"views: invalid boundary status for {row.get('act')}")
        if row.get('status') == 'UNRESOLVED' and not row.get('reason'):
            errors.append(f"views: unresolved boundary lacks reason for {row.get('act')}")
    import campaign
    errors.extend(campaign.coverage_check(ctx))
    return errors
