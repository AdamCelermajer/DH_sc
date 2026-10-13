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
                  membership_confidence='GUESS', config={}, direct_objects=[],
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


def _collect_actor_asset_dependencies(actor_rows, table_rows, cell_rows, resolver_rows, source_assets, scripts, audio_rows):
    """Follow exact actor/table references to source assets and keep each edge.

    The walk uses stable table IDs, local row indices, resolved reference cells,
    and exact source paths. It does not infer runtime loading or repair missing
    sound payload schemas.
    """
    rows_by_id = {r.get('id'): r for r in table_rows.values() if r.get('id')}
    rows_by_key = {(r.get('table'), r.get('index')): r for r in table_rows.values()}
    cells_by_key = defaultdict(list)
    for ref in cell_rows:
        context = ref.get('context') or {}
        if context.get('table') is not None and context.get('index') is not None:
            cells_by_key[(context.get('table'), context.get('index'))].append(ref)
    asset_by_key = {}
    for asset in source_assets:
        for value in (asset.get('path'), asset.get('relative')):
            if not value:
                continue
            norm = str(value).replace('\\', '/').casefold()
            asset_by_key[norm] = asset
            asset_by_key[re.sub(r'^data/', '', norm)] = asset
            asset_by_key[re.sub(r'^data/iphone/', '', norm)] = asset
    scripts_by_name = defaultdict(list)
    for script in scripts:
        for key in (script.get('name'), Path(str(script.get('site') or '')).stem):
            if key:
                scripts_by_name[str(key).casefold()].append(script)
    audio_by_name = defaultdict(list)
    audio_by_site = defaultdict(list)
    for row in audio_rows:
        if row.get('name'):
            audio_by_name[str(row['name']).casefold()].append(row)
        if row.get('site'):
            audio_by_site[str(row['site'])].append(row)
    resolver_refs_by_row = defaultdict(list)
    resolver_site_patterns = (
        (re.compile(r'pydata/animations_pyarray\.bin\[character=(\d+)\]\.'), 'character_animations'),
        (re.compile(r'pydata/animations_pyarray\.bin\[sequence=(\d+),step=\d+\]\.'), 'animation_sequences'),
        (re.compile(r'pydata/skills_pyarray\.bin\[skill=(\d+)\]\.'), 'skills#group1'),
        (re.compile(r'pydata/effects_pyarray\.bin\[character=(\d+)\]\.'), 'character_effects'),
        (re.compile(r'pydata/effects_pyarray\.bin\[set=(\d+),step=\d+\]\.'), 'effect_sets'),
        (re.compile(r'pydata/projectiles_pyarray\.bin\[row=(\d+)\]\.'), 'projectiles#group0'),
    )
    for ref in resolver_rows:
        site = str(ref.get('site') or '')
        for pattern, table_name in resolver_site_patterns:
            match = pattern.search(site)
            if match:
                resolver_refs_by_row[(table_name, int(match.group(1)))].append(ref)
                break

    asset_trails = defaultdict(list)
    asset_cell_refs = defaultdict(list)
    gaps = []

    def lookup_asset(path):
        value = str(path or '').replace('\\', '/').casefold()
        candidates = (value, re.sub(r'^data/', '', value), re.sub(r'^data/iphone/', '', value))
        for key in candidates:
            if key in asset_by_key:
                return asset_by_key[key]
        return None

    def trail_row(actor, act, trail, row, via_ref=None):
        return {'actor_id': actor.get('id'), 'actor': actor.get('name'), 'act': act,
            'dependency': trail, 'table': row.get('table'), 'index': row.get('index'),
            'row_id': row.get('id'), 'row_name': row.get('name'),
            'source_status': row.get('status'), 'source_work_status': row.get('work_status'),
            'source_confidence': row.get('confidence'), 'schema_evidence': row.get('schema_evidence') or row.get('source_path') or row.get('source_file'),
            'reference_id': via_ref.get('id') if via_ref else None,
            'reference_status': via_ref.get('status') if via_ref else None,
            'reference_work_status': via_ref.get('work_status') if via_ref else None,
            'reference_rule': via_ref.get('rule') if via_ref else None,
            'reference_reason': via_ref.get('reason') if via_ref else None,
            'port_status': via_ref.get('port_status') if via_ref else None,
            'implementation': via_ref.get('implementation') if via_ref else None}

    expanded_actors = []
    for actor in actor_rows:
        actor_acts = actor.get('shared_acts') or [actor.get('scene_membership')]
        for actor_act in actor_acts:
            if actor_act in ('act_1', 'act_2'):
                expanded_actors.append({**actor, 'scene_membership': actor_act})

    for actor in expanded_actors:
        act = actor.get('scene_membership')
        visited = set()

        def visit(row, trail, via_ref=None, depth=0):
            if not row or depth > 6:
                return
            key = (row.get('table'), row.get('index'))
            if key in visited:
                return
            visited.add(key)
            row_trail = trail_row(actor, act, trail, row, via_ref)
            fields = row.get('fields') or {}
            nested = fields.get('fields') if isinstance(fields, dict) and isinstance(fields.get('fields'), dict) else fields
            path = nested.get('Path') if isinstance(nested, dict) else None
            if path:
                asset = lookup_asset(path)
                if asset:
                    asset_trails[asset['id']].append({**row_trail, 'asset_path_field': path})

            # Follow only resolver rows emitted from the native table decoder.
            # This distinguishes character-state sequence IDs from clip IDs,
            # and animated-effect sets from effect-dictionary IDs.
            table = row.get('table')
            index = row.get('index')
            if (table, index) in resolver_refs_by_row:
                for ref in resolver_refs_by_row[(table, index)]:
                    site = str(ref.get('site') or '')
                    if ref.get('status') in ('UNRESOLVED', 'TODO', 'AMBIGUOUS'):
                        gaps.append({'act': act, 'actor_id': actor.get('id'), 'actor': actor.get('name'),
                            'dependency': trail, 'table': table, 'index': index, 'source': site,
                            'reference_id': ref.get('id'), 'status': ref.get('status'),
                            'work_status': ref.get('work_status'), 'confidence': ref.get('confidence'),
                            'rule': ref.get('rule'), 'reason': ref.get('reason'),
                            'port_status': ref.get('port_status'), 'implementation': ref.get('implementation')})
                    for target in ref.get('resolves_to') or []:
                        if not isinstance(target, dict):
                            continue
                        path_value = target.get('path')
                        target_table = target.get('table') or ''
                        if not path_value and re.match(r'(?i)^data/', str(target_table)):
                            path_value = target_table
                        for source in target.get('asset_files') or []:
                            asset = lookup_asset(source.get('path'))
                            if asset:
                                asset_trails[asset['id']].append({**row_trail,
                                    'dependency': f'{trail} -> {site}', 'reference_id': ref.get('id'),
                                    'reference_status': ref.get('status'), 'reference_work_status': ref.get('work_status'),
                                    'reference_rule': ref.get('rule'), 'reference_reason': ref.get('reason'),
                                    'port_status': ref.get('port_status'), 'implementation': ref.get('implementation'),
                                    'asset_path_field': source.get('path')})
                                asset_cell_refs[asset['id']].append((ref, target))
                        if path_value:
                            asset = lookup_asset(path_value)
                            if asset:
                                asset_trails[asset['id']].append({**row_trail,
                                    'dependency': f'{trail} -> {site}', 'reference_id': ref.get('id'),
                                    'reference_status': ref.get('status'), 'reference_work_status': ref.get('work_status'),
                                    'reference_rule': ref.get('rule'), 'reference_reason': ref.get('reason'),
                                    'port_status': ref.get('port_status'), 'implementation': ref.get('implementation'),
                                    'asset_path_field': path_value})
                                asset_cell_refs[asset['id']].append((ref, target))
                        source_row = target.get('source_row')
                        child_table = source_row.get('table') if isinstance(source_row, dict) else None
                        child_index = source_row.get('index') if isinstance(source_row, dict) else None
                        if child_table == 'sdd_dungeon_hunter_2_iphone' and isinstance(child_index, int):
                            audio_site = f'pydata/sdd_dungeon_hunter_2_iphone_pyarray.bin[row={child_index}]'
                            for audio in audio_by_site.get(audio_site, []):
                                audio_targets = audio.get('resolves_to') or []
                                resolved_files = [source for audio_target in audio_targets
                                    for source in (audio_target.get('asset_files') or [])]
                                for source in resolved_files:
                                    asset = lookup_asset(source.get('path'))
                                    if asset:
                                        asset_trails[asset['id']].append({**row_trail,
                                            'dependency': f'{trail} -> {site} -> exact SDD row -> audio resolver',
                                            'reference_id': audio.get('id'), 'reference_status': audio.get('status'),
                                            'reference_work_status': audio.get('work_status'), 'reference_rule': audio.get('rule'),
                                            'reference_reason': audio.get('reason'), 'port_status': audio.get('port_status'),
                                            'implementation': audio.get('implementation'), 'asset_path_field': source.get('path')})
                                        asset_cell_refs[asset['id']].append((audio, audio_targets[0] if audio_targets else {}))
                                if audio.get('status') in ('UNRESOLVED', 'TODO', 'AMBIGUOUS') or not resolved_files:
                                    gaps.append({'act': act, 'actor_id': actor.get('id'), 'actor': actor.get('name'),
                                        'dependency': f'{trail} -> {site}', 'table': child_table, 'index': child_index,
                                        'name': audio.get('name'), 'source': audio.get('site'),
                                        'reference_id': audio.get('id'), 'status': audio.get('status', 'UNRESOLVED'),
                                        'work_status': audio.get('work_status'), 'confidence': audio.get('confidence'),
                                        'rule': audio.get('rule'), 'reason': audio.get('reason'),
                                        'candidates': audio.get('candidates'), 'port_status': audio.get('port_status'),
                                        'implementation': audio.get('implementation')})
                        table_match = re.search(r'(AnimTable|EffectsTables|sdd_dungeon_hunter_2_iphone)\[(\d+)\]', str(target_table))
                        if table_match:
                            child_table = {'AnimTable': 'animation_sequences', 'EffectsTables': 'effect_sets',
                                           'sdd_dungeon_hunter_2_iphone': 'sdd_dungeon_hunter_2_iphone'}[table_match.group(1)]
                            child_index = int(table_match.group(2))
                        if child_table and isinstance(child_index, int):
                            child = rows_by_key.get((child_table, child_index)) or source_row
                            if child:
                                visit(child, f'{trail} -> {site} -> {child_table}[{child_index}]', ref, depth + 1)

            # Preserve unresolved sound payload status and only follow an exact
            # audio-name edge if that source ledger provides it.
            if str(row.get('table', '')).startswith('sounds#'):
                audio_matches = audio_by_name.get(str(row.get('name') or '').casefold(), [])
                if row.get('status') in ('UNRESOLVED', 'TODO') or not nested:
                    gaps.append({'act': act, 'actor_id': actor.get('id'), 'actor': actor.get('name'),
                        'dependency': trail, 'table': row.get('table'), 'index': row.get('index'),
                        'name': row.get('name'), 'status': row.get('status', 'UNRESOLVED'),
                        'work_status': row.get('work_status', 'DONE'), 'reason': row.get('candidate_note') or row.get('reason'),
                        'audio_candidates': [c for m in audio_matches for c in (m.get('candidates') or [])],
                        'audio_resolution_rows': audio_matches})
                for audio in audio_matches:
                    for target in audio.get('resolves_to') or []:
                        for source in target.get('asset_files') or []:
                            asset = lookup_asset(source.get('path'))
                            if asset:
                                asset_trails[asset['id']].append({**row_trail,
                                    'dependency': trail + ' -> exact audio name row',
                                    'reference_id': audio.get('id'), 'reference_status': audio.get('status'),
                                    'reference_work_status': audio.get('work_status'),
                                    'reference_rule': audio.get('rule'), 'reference_reason': audio.get('reason'),
                                    'port_status': audio.get('port_status'), 'implementation': audio.get('implementation'),
                                    'asset_path_field': source.get('path')})
                                asset_cell_refs[asset['id']].append((audio, target))

            # A skills index row has a structurally decoded list of exact local
            # row indices; follow only those explicit group1 entries.
            if row.get('table') == 'skills#group0':
                entries = nested.get('list_entries', []) if isinstance(nested, dict) else []
                for idx in entries if isinstance(entries, list) else []:
                    child = rows_by_key.get(('skills#group1', idx))
                    if child:
                        visit(child, f'{trail} -> skills#group1[{idx}]', None, depth + 1)

            # AI Script is an exact source name. A script asset is attached only
            # when that name uniquely matches the source inventory.
            if row.get('table') in ('ai#group0', 'skills#group1') and isinstance(nested, dict):
                script_name = nested.get('Script')
                matches = scripts_by_name.get(str(script_name or '').casefold(), [])
                if len(matches) == 1:
                    script = matches[0]
                    asset = lookup_asset(script.get('site'))
                    if asset:
                        asset_trails[asset['id']].append({**row_trail,
                            'dependency': trail + f' -> exact {row.get("table")} Script {script_name}',
                            'reference_id': script.get('id'), 'reference_status': script.get('status'),
                            'reference_work_status': script.get('work_status'), 'reference_rule': f'exact {row.get("table")}.Script to script inventory name',
                            'reference_reason': script.get('reason'), 'port_status': script.get('port_status'),
                            'implementation': script.get('implementation'), 'asset_path_field': script.get('site')})

            for ref in cells_by_key.get(key, []):
                targets = ref.get('resolves_to') or []
                if not isinstance(targets, list):
                    targets = [targets]
                if ref.get('status') in ('UNRESOLVED', 'TODO', 'AMBIGUOUS'):
                    gaps.append({'act': act, 'actor_id': actor.get('id'), 'actor': actor.get('name'),
                        'dependency': trail, 'table': row.get('table'), 'index': row.get('index'),
                        'field': (ref.get('context') or {}).get('field'), 'reference_id': ref.get('id'),
                        'status': ref.get('status'), 'work_status': ref.get('work_status'),
                        'confidence': ref.get('confidence'), 'rule': ref.get('rule'),
                        'reason': ref.get('reason'), 'port_status': ref.get('port_status'),
                        'implementation': ref.get('implementation')})
                for target in targets:
                    if not isinstance(target, dict):
                        continue
                    for source in target.get('asset_files') or []:
                        asset = lookup_asset(source.get('path'))
                        if asset:
                            asset_trails[asset['id']].append({**row_trail,
                                'dependency': f"{trail} -> {ref.get('context', {}).get('field')}",
                                'reference_id': ref.get('id'), 'reference_status': ref.get('status'),
                                'reference_work_status': ref.get('work_status'), 'reference_rule': ref.get('rule'),
                                'reference_reason': ref.get('reason'), 'port_status': ref.get('port_status'),
                                'implementation': ref.get('implementation'), 'asset_path_field': source.get('path')})
                            asset_cell_refs[asset['id']].append((ref, target))
                    if target.get('type') == 'table-row':
                        child = rows_by_key.get((target.get('table'), target.get('index')))
                        if not child and isinstance(target.get('source_row'), dict):
                            child = target['source_row']
                        if child:
                            visit(child, f"{trail} -> {ref.get('context', {}).get('field')} -> {child.get('table')}[{child.get('index')} ]",
                                  ref, depth + 1)

        for field, link in (actor.get('field_links') or {}).items():
            target = rows_by_id.get(link.get('target_id'))
            if not target and link.get('table') is not None and link.get('index') is not None:
                target = rows_by_key.get((link.get('table'), link.get('index')))
            if target:
                visit(target, f"CharacterProperties[{actor.get('index')}].{field} -> {target.get('table')}[{target.get('index')} ]",
                      {'id': link.get('reference_id'), 'status': link.get('status'),
                       'work_status': 'DONE', 'rule': 'actor field link from exact source cell',
                       'port_status': None, 'implementation': None})
            elif link.get('status') in ('UNRESOLVED', 'TODO', 'AMBIGUOUS'):
                gaps.append({'act': act, 'actor_id': actor.get('id'), 'actor': actor.get('name'),
                    'dependency': f"CharacterProperties[{actor.get('index')}].{field}",
                    'table': link.get('table'), 'index': link.get('index'), 'name': link.get('name'),
                    'reference_id': link.get('reference_id'), 'status': link.get('status'),
                    'work_status': 'DONE', 'reason': 'Exact actor field reference remains unresolved.'})

    return asset_trails, asset_cell_refs, gaps


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
    by_table = _table_map(list(table_rows.values()))
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
            scene['membership_confidence'] = 'LIKELY' if act != 'unplaced' else 'GUESS'
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
             'confidence': 'LIKELY' if inferred_levels.get(act) else 'GUESS',
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
            'reason_code': 'OTHER', 'label': 'INFERENCE', 'confidence': 'GUESS',
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
    named_boss_ids = {a['id'] for a in named_boss_candidates}

    def boss_inventory_row(actor, refreshed=None):
        row = {**(refreshed or actor)}
        is_flagged = row.get('id') in boss_flag_ids or has_original_boss_flag(row)
        row.update(classification=('ORIGINAL_BOSS_FLAG_CANDIDATE' if is_flagged else 'NAME_ONLY_CANDIDATE'),
            classification_basis=('Native HUD boss marker: (CharacterProperties.AI -> AI.Flags & 4) != 0; '
                'predicate source: port/level-world/canonical_character_hud_v62.cpp:14.' if is_flagged else
                'Supplemental BOSS/MIDBOSS name marker; source encounter role is not proven.'),
            name_marker_candidate=row.get('id') in named_boss_ids,
            name_marker_status='UNRESOLVED' if row.get('id') in named_boss_ids else 'NOT-NEEDED',
            name_marker_reason=('Name-based candidate retained as supplemental context; native flag evidence controls the source-proven marker.'
                                if row.get('id') in named_boss_ids else None),
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
             'confidence': s.get('membership_confidence', 'GUESS'),
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
                    ['Index', 'Candidate', 'AI row', 'AI facts', 'Skill tree', 'Loot', 'Original flag status', 'Name marker note', 'Basis'],
                    lambda r: [r.get('index'), r.get('name'), r.get('ai'), _actor_ai_summary(r),
                               r.get('skills'), r.get('loot'), r.get('boss_flag_status'), r.get('name_marker_reason'), r.get('classification_basis')])
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
    supplemental_named_count = sum(1 for a in bosses if a.get('name_marker_candidate') and a.get('boss_flag_status') != 'PROVEN')
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
            'supplemental_name_marked_boss_candidates': {'total': sum(1 for a in bosses if a.get('name_marker_candidate')),
                'status': 'DONE', 'rule': 'Preserves all 26 name-marker candidates as notes; classification relies on the AI flag predicate where available.'},
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
                  f'{len(flagged_boss_rows)} rows match the source-proven AI.Flags bit-4 HUD marker; {supplemental_named_count} name-only candidates remain supplemental, and encounter placement is not inferred from the marker.']
    })
    if inventory_only:
        return {'scene_level_rows': level_inventory, 'levels_table_rows': len(level_table_rows), 'campaign_scene_rows': campaign_count,
                'debug_scene_rows': debug_count, 'backup_scene_rows': backup_count,
                'character_properties_actor_rows': len(actor_rows), 'boss_candidates': len(bosses)}

    # Produce navigable directories. The detail layer is intentionally limited to
    # Act 1 and the next inferred region (Act 2).
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

    # Close actor-to-asset dependencies through exact CharacterProperties
    # links, table row references, and source paths. This adds models, actor
    # animation clips, effects, skills, AI scripts, and resolvable sound edges.
    audio_rows = read_json(out / 'data/audio.json', []) or []
    actor_asset_trails, actor_asset_refs, actor_dependency_gaps = _collect_actor_asset_dependencies(
        actor_rows, table_rows, cell_rows, refs, source_assets, scripts, audio_rows)
    for asset_id, trails in actor_asset_trails.items():
        for trail in trails:
            target_acts[asset_id].add(trail['act'])
    for asset_id, dependent_refs in actor_asset_refs.items():
        for ref, target in dependent_refs:
            if ref.get('id'):
                asset_act_refs[asset_id].append((ref, target))

    asset_pages = defaultdict(list)
    page_by_asset = {}
    for rel in ('act_1/assets', 'act_2/assets', 'shared/assets'):
        generated_dir = out / rel
        if generated_dir.is_dir():
            for stale in generated_dir.glob('*.md'):
                stale.unlink()
    for asset_id in sorted(set(asset_act_refs) | set(actor_asset_trails)):
        contexts = asset_act_refs.get(asset_id, [])
        asset = next((a for a in source_assets if a.get('id') == asset_id), None)
        if not asset:
            continue
        acts = sorted(target_acts[asset_id])
        all_ref_ids = set(asset.get('referrers') or [])
        rows_for_asset = {}
        for ref, target in contexts:
            if not ref:
                continue
            rows_for_asset[ref.get('id')] = (ref, target)
        # Retain the complete source/resolution metadata for source refs that
        # name this asset; list unrelated asset-level referrer IDs separately.
        rel = Path('assets') / (asset_id + '.md')
        actor_trails = actor_asset_trails.get(asset_id, [])
        unique_trails = {}
        for trail in actor_trails:
            trail_key = (trail.get('actor_id'), trail.get('dependency'), trail.get('row_id'),
                         trail.get('reference_id'), trail.get('asset_path_field'))
            unique_trails[trail_key] = trail
        trail_rows = [[t.get('actor'), t.get('act'), t.get('dependency'),
            f"{t.get('table')}[{t.get('index')}] {t.get('row_name') or ''}",
            t.get('source_status'), t.get('source_work_status'), t.get('source_confidence'),
            t.get('schema_evidence'), t.get('reference_id'), t.get('reference_status'),
            t.get('reference_work_status'), t.get('reference_rule'), t.get('reference_reason'),
            t.get('port_status'), _str(t.get('implementation')), t.get('asset_path_field')]
            for t in unique_trails.values()]
        # Include exact resolver rows reached from actor table cells even when
        # the asset path was first reached through a nested table row.
        for ref, target in actor_asset_refs.get(asset_id, []):
            rows_for_asset[ref.get('id')] = (ref, target)
        detail_rows = []
        for ref_id, (ref, target) in sorted(rows_for_asset.items()):
            detail_rows.append([ref_id, ref.get('kind'), ref.get('name'), ref.get('site'),
                ref.get('status'), ref.get('work_status'), ref.get('confidence'), ref.get('rule'),
                ref.get('port_status'), _str(ref.get('implementation')), _str(ref.get('reason')),
                _str(target.get('path'))])
        page = ['# ' + str(asset.get('relative') or asset.get('path')), '',
            f"Stable asset ID: `{asset_id}`. Source: `{asset.get('path')}`.", '',
            markdown_table(['Fact', 'Value'], [
                ['Source corpus', asset.get('source')], ['Relative path', asset.get('relative')],
                ['Size bytes', asset.get('size')], ['SHA-256', asset.get('sha256')],
                ['Inventory status', asset.get('status')], ['Work status', asset.get('work_status')],
                ['Act uses', ', '.join(acts)], ['Source-linked reference rows', len(detail_rows)],
                ['Actor dependency trails', len(trail_rows)],
                ['All inventory referrer IDs', len(all_ref_ids)],
            ]), '', '## Source-linked reference and resolution rows', '',
            markdown_table(['ID', 'Kind', 'Name', 'Site', 'Status', 'Work status', 'Confidence',
                            'Rule', 'Port status', 'Implementation', 'Reason', 'Resolved source path'], detail_rows), '',
            'Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.', '',
            '## Actor table dependency trails', '',
            markdown_table(['Actor', 'Act', 'Dependency path', 'Exact source row', 'Row status', 'Work status',
                'Confidence', 'Schema evidence', 'Reference ID', 'Reference status', 'Reference work status',
                'Resolver rule', 'Reason', 'Port status', 'Implementation', 'Resolved asset path'], trail_rows), '',
            'These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.', '']
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
                     f"The source HUD predicate `(AI.Flags & 4) != 0` is proven for {len(flagged_boss_rows)} CharacterProperties-linked AI rows. {supplemental_named_count} name-only markers remain supplemental unresolved candidates. This source marker does not establish encounter placement or runtime spawn conditions.", '',
                     markdown_table(['Boss flag rows', 'Name-only supplements', 'Act-linked candidates', 'Placement status'],
                         [[len(flagged_boss_rows), supplemental_named_count, len(placed_bosses), 'INFERENCE / unresolved runtime placement']]), '',
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
    campaign_scene_by_id = {s.get('id'): s for s in scene_records}
    for ref in campaign_refs:
        act = ref.get('act')
        scene = campaign_scene_by_id.get(ref.get('scene_id'))
        if act not in ('act_2', 'act_3', 'act_4', 'act_5') or not scene or ref.get('status') not in ('TODO', 'UNRESOLVED', 'AMBIGUOUS'):
            continue
        confidence = ref.get('confidence')
        if confidence not in ('PROVEN', 'LIKELY', 'GUESS'):
            confidence = 'GUESS'
        ref_gaps.append({'act': act, 'category': 'unresolved_campaign_script_reference',
            'id': ref.get('id'), 'source': ref.get('site'), 'scene': scene.get('file'),
            'name': ref.get('name'), 'kind': ref.get('kind'), 'command': ref.get('command'),
            'field_offset': ref.get('field_offset'), 'status': ref.get('status'),
            'work_status': ref.get('work_status'), 'confidence': confidence,
            'rule': ref.get('rule'), 'port_status': ref.get('port_status'),
            'implementation': ref.get('implementation'), 'reason': ref.get('reason'), 'priority': 2})
    for gap in actor_dependency_gaps:
        act = gap.get('act')
        if act not in ('act_2', 'act_3', 'act_4', 'act_5'):
            continue
        confidence = gap.get('confidence')
        if confidence not in ('PROVEN', 'LIKELY', 'GUESS'):
            confidence = 'GUESS'
        ref_gaps.append({**gap, 'category': 'unresolved_actor_table_dependency',
            'confidence': confidence, 'priority': 2})
    ranked_gaps = sorted(command_gaps + ref_gaps,
        key=lambda row: (row['priority'], row['act'], -row.get('count', 0), str(row.get('source', '')), str(row.get('name', ''))))
    act_gap_rows = []
    for act in ACTS:
        act_rows = [r for r in ranked_gaps if r['act'] == act]
        unresolved_campaign_count = sum(1 for r in campaign_refs if r.get('act') == act and r.get('status') in ('TODO', 'UNRESOLVED', 'AMBIGUOUS'))
        unresolved_actor_count = sum(1 for r in actor_dependency_gaps if r.get('act') == act and r.get('status') in ('TODO', 'UNRESOLVED', 'AMBIGUOUS'))
        act_gap_rows.append({'act': act, 'unresolved_source_references': act_unresolved_refs[act] + unresolved_campaign_count + unresolved_actor_count,
            'incomplete_campaign_engine_commands': sum(r['category'] == 'campaign_engine_command_port_gap' for r in act_rows),
            'unmapped_actor_rows_global': len([a for a in actor_rows if a.get('act_assignment_status') != 'INFERENCE']),
            'unresolved_boss_candidates_global': supplemental_named_count,
            'unresolved_boss_candidates_status': 'UNRESOLVED',
            'boss_flag_proven_global': len(flagged_boss_rows),
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
            ['Boss markers', f'{len(flagged_boss_rows)} flag-proven; {supplemental_named_count} name-only supplements', 'Marker predicate proven; encounter placement unresolved'],
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
        f"The view generator is complete for the current evidence set. Scene XML, MGP Character candidates, table joins and available campaign command streams have source-backed results. Act boundaries remain INFERENCE pending owner confirmation; two configured script pairs are unresolved. The native boss-flag predicate is proven for {len(flagged_boss_rows)} AI-linked rows, with {supplemental_named_count} name-only supplements; encounter placement, dialogue-table mapping, and runtime command conditions remain unresolved.", '']
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
