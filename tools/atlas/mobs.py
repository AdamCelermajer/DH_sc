"""Joined actor dossiers. Source associations are not runtime behavior claims."""
from collections import Counter, defaultdict, deque
from pathlib import Path
import json
import re

from common import read_json, write_json, markdown_table

PATTERNS = (
    (r'animations_pyarray\.bin\[character=(\d+)\]', 'character_animations'),
    (r'animations_pyarray\.bin\[sequence=(\d+),', 'animation_sequences'),
    (r'skills_pyarray\.bin\[skill=(\d+)\]', 'skills#group1'),
    (r'effects_pyarray\.bin\[character=(\d+)\]', 'character_effects'),
    (r'effects_pyarray\.bin\[set=(\d+),', 'effect_sets'),
    (r'projectiles_pyarray\.bin\[row=(\d+)\]', 'projectiles#group0'),
    (r'sdd_dungeon_hunter_2_iphone_pyarray\.bin\[row=(\d+)\]', 'sdd_dungeon_hunter_2_iphone'),
)
ROOTS = {'model': ('ModelFile', 'ModularVisual'), 'animations': ('AnimTable',),
         'effects': ('Effects',), 'projectiles': ('RangeProjectileID',),
         'skills': ('SkillTree',), 'sounds': ('Sounds',), 'ai': ('AI',)}


def fields(row):
    value = row.get('fields') or {}
    return value.get('fields', value)


def target_key(target):
    if target.get('type') in ('table-field','ios_source_name','ios_stored_constant','ios_table_source'):
        return None
    source = target.get('source_row') or {}
    table = source.get('table', target.get('table'))
    index = source.get('index', target.get('index'))
    if table and isinstance(index, int):
        return table, index
    match = re.fullmatch(r'(AnimTable|EffectsTables|sdd_dungeon_hunter_2_iphone)\[(\d+)\]', str(table))
    if match:
        return {'AnimTable': 'animation_sequences', 'EffectsTables': 'effect_sets',
                'sdd_dungeon_hunter_2_iphone': 'sdd_dungeon_hunter_2_iphone'}[match[1]], int(match[2])
    return None


class Index:
    def __init__(self, ctx):
        self.ctx = ctx
        self.tables = read_json(ctx.out / 'data/tables.json', [])
        self.rows = {(r['table'], r['index']): r for r in self.tables}
        self.by_id = {r['id']: r for r in self.tables}
        self.refs = read_json(ctx.out / 'data/references.json', [])
        self.ref_by_id = {r['id']: r for r in self.refs}
        self.row_refs = defaultdict(list)
        self.script_refs = defaultdict(list)
        self.asset_refs = defaultdict(list)
        for ref in self.refs:
            context = ref.get('context') or {}
            if context.get('table') and isinstance(context.get('index'), int):
                self.row_refs[context['table'], context['index']].append(ref)
            else:
                for pattern, table in PATTERNS:
                    match = re.search(pattern, ref.get('site', ''))
                    if match:
                        self.row_refs[table, int(match[1])].append(ref)
                        break
            if context.get('script'):
                self.script_refs[context['script']].append(ref)
            file = context.get('file')
            if file:
                self.asset_refs[file].append(ref)
        self.assets = read_json(ctx.out / 'data/assets.json', [])
        self.asset_by_id = {a['id']: a for a in self.assets}
        self.asset_paths = defaultdict(list)
        for asset in self.assets:
            for path in (asset['path'], asset.get('relative', ''), *asset.get('logical_paths', [])):
                if path:
                    norm = path.replace('\\', '/').casefold()
                    self.asset_paths[norm].append(asset)
                    if norm.startswith('data/'):
                        self.asset_paths[norm[5:]].append(asset)
        self.scripts = read_json(ctx.out / 'data/scripts.json', [])
        self.script_by_id = {s['id']: s for s in self.scripts}
        self.script_names = defaultdict(list)
        for script in self.scripts:
            self.script_names[Path(script['name']).stem.casefold()].append(script)
        self.calls = read_json(ctx.out / 'data/calls.json', [])
        self.script_calls = defaultdict(list)
        for call in self.calls:
            self.script_calls[call['script_id']].append(call)
        self.functions = {f['id']: f for f in read_json(ctx.out / 'data/functions.json', [])}

    def files(self, path):
        path = str(path or '').replace('\\', '/').casefold()
        variants = {path, re.sub(r'^data/(?:iphone/)?', '', path)}
        result = {a['id']: a for key in variants for a in self.asset_paths.get(key, [])}
        return list(result.values())

    def walk(self, roots, initial_refs=()):
        queue = deque(roots)
        seen, refs, assets, gaps = set(), {}, {}, []
        asset_queue = deque()

        def add_asset(asset):
            if asset['id'] not in assets:
                assets[asset['id']] = asset
                asset_queue.append(asset)

        def consume(ref):
            if ref['id'] in refs:
                return
            refs[ref['id']] = ref
            for target in ref.get('resolves_to') or []:
                key = target_key(target)
                if key:
                    queue.append(key)
                path = target.get('path')
                if not path and str(target.get('table', '')).lower().startswith('data/'):
                    path = target['table']
                for asset in self.files(path):
                    add_asset(asset)
                for file in target.get('asset_files') or []:
                    for asset in self.files(file.get('path')):
                        add_asset(asset)

        for ref in initial_refs:
            consume(ref)
        while queue or asset_queue:
            if not queue:
                asset = asset_queue.popleft()
                for ref in self.asset_refs[asset['path']]:
                    consume(ref)
                continue
            key = queue.popleft()
            if key in seen:
                continue
            seen.add(key)
            row = self.rows.get(key)
            if row is None:
                gaps.append({'table': key[0], 'index': key[1], 'reason': 'Joined target row is absent.'})
                continue
            if row.get('status') != 'RESOLVED':
                gaps.append({'table': key[0], 'index': key[1], 'reason': row.get('candidate_note') or row.get('reason') or 'Payload schema unresolved.'})
            for asset in self.files(fields(row).get('Path')):
                add_asset(asset)
            if key[0] == 'skills#group0':
                queue.extend(('skills#group1', i) for i in fields(row).get('list_entries', []))
            for ref in self.row_refs[key]:
                consume(ref)
        return {'table_ids': sorted(self.rows[k]['id'] for k in seen if k in self.rows),
                'reference_ids': sorted(refs), 'asset_ids': sorted(assets), 'gaps': gaps,
                'reference_statuses': dict(Counter(r.get('status') for r in refs.values()))}

    def script_closure(self, names):
        queue = deque(names)
        scripts, includes, gaps = {}, [], []
        while queue:
            name = queue.popleft()
            matches = self.script_names.get(Path(name).stem.casefold(), [])
            if len(matches) != 1:
                gaps.append({'script': name, 'reason': 'Exact source stem is absent or ambiguous.',
                             'candidates': [s['site'] for s in matches]})
                continue
            script = matches[0]
            if script['id'] in scripts:
                continue
            scripts[script['id']] = script
            for call in self.script_calls[script['id']]:
                if call['name'] != 'Include' or not call.get('arguments'):
                    continue
                arg = call['arguments'][0]
                literals = [v for v in arg.get('literals', []) if v.get('type') == 'string' and v.get('raw') == arg.get('expression')]
                if len(literals) == 1:
                    includes.append({'source': script['site'], 'line': call['line'], 'target': literals[0]['value']})
                    queue.append(literals[0]['value'])
                else:
                    gaps.append({'script': script['site'], 'line': call['line'], 'reason': 'Dynamic Include expression', 'expression': arg.get('expression')})
        return {'script_ids': sorted(scripts), 'include_edges': includes, 'gaps': gaps}


def build(ctx):
    import mob_drops
    index = Index(ctx)
    loot_index = mob_drops.build_index(index.tables, index.refs)
    actors = read_json(ctx.out / 'data/actors.json', [])
    bosses = {b['id']: b for b in read_json(ctx.out / 'data/bosses.json', [])}
    profiles = []
    for actor in actors:
        sections = {}
        for section, root_fields in ROOTS.items():
            roots = [(link['table'], link['index']) for f in root_fields
                     if (link := actor.get('field_links', {}).get(f))]
            sections[section] = index.walk(roots)
            sections[section]['root_fields'] = {f: actor['fields'].get(f) for f in root_fields}
            for f in root_fields:
                value = actor['fields'].get(f)
                if isinstance(value, int) and value >= 0 and f not in actor.get('field_links', {}):
                    sections[section]['gaps'].append({'field': f, 'value': value,
                        'reason': 'Positive character field has no resolved table edge.'})
        script_names = []
        for section in ('ai', 'skills'):
            for rid in sections[section]['table_ids']:
                name = fields(index.by_id[rid]).get('Script')
                if isinstance(name, str) and name:
                    script_names.append(name)
        scripts = index.script_closure(script_names)
        script_refs = [r for sid in scripts['script_ids']
                       for r in index.script_refs[index.script_by_id[sid]['site']]]
        sections['script_resources'] = index.walk([], script_refs)
        audio_assets = {aid for data in sections.values() for aid in data['asset_ids']
                        if Path(index.asset_by_id[aid]['path']).suffix.casefold() in ('.wav', '.vxn', '.mp3', '.ogg')}
        audio_refs = {rid for data in sections.values() for rid in data['reference_ids']
                      if 'sound' in index.ref_by_id[rid].get('reference_kind', index.ref_by_id[rid].get('kind', '')).lower()}
        profile = {'id': actor['id'], 'index': actor['index'], 'name': actor['name'],
                   'aliases': ['Mud Lord', 'MudLord', 'Swamp King'] if actor['name'] == 'SwampKing' else [],
                   'role': 'BOSS_FLAGGED' if bosses.get(actor['id'], {}).get('boss_flag_status') == 'PROVEN'
                   else 'BOSS_NAME_CANDIDATE' if actor['id'] in bosses
                   else 'MOB_AI_TYPE_4' if actor.get('ai_type') == 4 else 'OTHER_ACTOR',
                   'act': actor.get('scene_membership') or actor.get('act', 'unplaced'),
                   'act_status': actor.get('act_assignment_status'),
                   'sections': sections, 'scripts': scripts,
                   'audio': {'asset_ids': sorted(audio_assets), 'reference_ids': sorted(audio_refs),
                             'meaning': 'Union of reached character, animation-step and script audio producers; shared script branches remain candidate associations.'},
                   'drops': mob_drops.build(loot_index, index.tables, index.refs, actor)
                   if actor['fields'].get('Loot', -1) >= 0 else {'root': None, 'nodes': [], 'terminal_items': [],
                       'gaps': [], 'cycles': [], 'notes': ['Character Loot field has the absent sentinel; Lua or campaign rewards are separate paths.']},
                   'complete': False,
                   'scope': 'Authored dependencies and candidate resources. Dynamic Lua expressions, runtime admission, overrides and drops are not fully certified.'}
        profiles.append(profile)
    return index, profiles


def source_link(ctx, path, line=None):
    if not path:
        return ''
    if path.startswith('.local-inputs/'):
        target = ctx.inputs / path[len('.local-inputs/'):]
    else:
        target = ctx.root / path.removeprefix('.repo/')
    suffix = ':' + str(line) if line else ''
    return f'[{path}{suffix}](<{target.as_posix()}{suffix}>)'


def render(index, profile):
    ctx = index.ctx
    doc = [f"# {profile['name']}", '',
           f"CharacterProperties row **{profile['index']}**. Role: **{profile['role']}**. Act: {profile['act']} ({profile.get('act_status')}).", '',
           '**Coverage: partial.** ' + profile['scope'], '',
           '[All actor profiles](index.md) · [Resolution rules](../../RULES.md)', '',
           '## Scripts', '',
           markdown_table(['Script source', 'Engine calls ported / total', 'Partial / missing', 'Mapping status'],
             [[source_link(ctx, s['site']), f"{s['readiness']['ported']} / {s['readiness']['engine_calls']}",
               f"{s['readiness']['partial']} / {s['readiness']['missing']}", s['status']]
              for sid in profile['scripts']['script_ids'] for s in [index.script_by_id[sid]]]), '',
           'Includes are followed only for an exact string argument and unique source stem. Listed resources in shared scripts may belong to conditional branches; association does not prove use by this actor.', '']
    if profile['scripts']['gaps']:
        doc += ['Script gaps: ' + json.dumps(profile['scripts']['gaps'], ensure_ascii=False), '']
    for section, data in profile['sections'].items():
        doc += ['## ' + section.replace('_', ' ').title(), '']
        if section == 'sounds':
            doc += ['**All reached audio producers:** character sounds, animation steps and script resources. '
                    'An empty character row does not imply a silent actor. Shared-script branches remain candidates.', '',
                    markdown_table(['Audio source file', 'Cache', 'SHA256'],
                      [[source_link(ctx, a['path']), a['source'], a['sha256']] for aid in profile['audio']['asset_ids']
                       for a in [index.asset_by_id[aid]]]), '']
        if data.get('root_fields'):
            doc += ['Source character fields: `' + json.dumps(data['root_fields'], sort_keys=True) + '`.', '']
        doc += [markdown_table(['Table / row', 'Name', 'Fields', 'Payload status'],
                  [[f"{row['table']}[{row['index']}]", row['name'], fields(row), row['status']]
                   for rid in data['table_ids'] for row in [index.by_id[rid]]]), '',
                markdown_table(['Original source file', 'Cache', 'SHA256'],
                  [[source_link(ctx, a['path']), a['source'], a['sha256']]
                   for aid in data['asset_ids'] for a in [index.asset_by_id[aid]]]), '']
        # Keep every exact consumer site, including missing files, in the dossier.
        doc += [markdown_table(['Reference site', 'Kind / value', 'Resolved target', 'Status / reason'],
                  [[ref['site'], f"{ref.get('reference_kind', ref.get('kind'))}: {ref.get('name')}".replace('\r\n', '\n').replace('\r', '\n'),
                    [(t.get('table'), t.get('index'), t.get('path'), t.get('source_row', {}).get('name')) for t in ref.get('resolves_to') or []],
                    ref['status'] + (' / ' + str(ref.get('candidate_note') or ref.get('reason')) if ref['status'] != 'RESOLVED' else '')]
                   for rid in data['reference_ids'] for ref in [index.ref_by_id[rid]]]), '']
        if data['gaps']:
            doc += ['Payload gaps: ' + json.dumps(data['gaps'], ensure_ascii=False), '']
    drops = profile['drops']
    doc += ['## Drops', '',
            'Authored possibilities; kill/reward admission, RNG, difficulty, player class and player count affect actual results. Raw probability fields are preserved, without claiming unconditional drop percentages. '
            '[Consumer evidence](../../mob-drops-report.md).', '',
            'Root: `' + json.dumps(drops['root'], ensure_ascii=False) + '`.', '',
            markdown_table(['Loot / item-list / power node', 'Name', 'Source fields', 'Edges'],
                [[f"{n['identity']['table']}[{n['identity']['index']}]", n['identity']['name'], n['fields'],
                  [{'field': e['field'], 'target': e['target'], 'status': e['status']} for e in n['edges']]]
                 for n in drops['nodes'] if n['identity']['table'] != 'loot_table#group3']), '',
            markdown_table(['Possible item', 'Item row', 'Item list', 'Raw probability / quantity fields'],
                [[t['item']['name'], t['item']['index'], t['item_list']['name'], t['item_list_entry']]
                 for t in drops['terminal_items']]), '',
            'Graph gaps: `' + json.dumps(drops['gaps']) + '`. Cycles: `' + json.dumps(drops['cycles']) + '`.', '',
            '## Character fields', '', '```json',
            json.dumps(index.rows['character_properties#group0', profile['index']]['fields'], indent=2, sort_keys=True), '```', '']
    return '\n'.join(doc)


def run(ctx):
    index, profiles = build(ctx)
    folder = ctx.out / 'general/mobs'
    folder.mkdir(parents=True, exist_ok=True)
    for profile in profiles:
        profile['page'] = f"general/mobs/{profile['index']:03d}.md"
        (ctx.out / profile['page']).write_text(render(index, profile), encoding='utf-8', newline='\n')
    write_json(ctx.out / 'data/mob-profiles.json', profiles)
    counts = {'profiles': len(profiles), 'roles': dict(Counter(p['role'] for p in profiles)),
              'fully_certified_profiles': 0,
              'meaning': 'Joined lookup coverage; none certifies every gameplay rule or dynamic dependency.'}
    write_json(ctx.out / 'counts/mobs.json', counts)
    # Actor dossiers are joined views, not additional per-asset detail pages.
    (folder / 'index.md').write_text('# Mob, boss and actor profiles\n\n'
        'Every CharacterProperties row has a joined profile. Coverage remains partial; unresolved payloads and references are shown explicitly.\n\n'
        + markdown_table(['Actor', 'Row', 'Role', 'Act', 'Coverage'],
            [[f"[{p['name']}]({p['index']:03d}.md)", p['index'], p['role'], p['act'], 'PARTIAL'] for p in profiles]),
        encoding='utf-8', newline='\n')
    return counts


def lookup(ctx, query, as_json=False):
    profiles = read_json(ctx.out / 'data/mob-profiles.json', [])
    exact = [p for p in profiles if query.casefold() in
             [p['name'].casefold(), str(p['index']), p['id'], *(a.casefold() for a in p.get('aliases', []))]]
    matches = exact or [p for p in profiles if query.casefold() in p['name'].casefold()]
    if not matches:
        print('No matching character row: ' + query)
        return 1
    if len(matches) > 1:
        print(markdown_table(['Name', 'Row', 'Role'], [[p['name'], p['index'], p['role']] for p in matches]))
        return 0
    profile = matches[0]
    if as_json:
        index = Index(ctx)
        result = {**profile, 'expanded_sections': {name: {**data,
            'tables': [index.by_id[i] for i in data['table_ids']],
            'references': [index.ref_by_id[i] for i in data['reference_ids']],
            'assets': [index.asset_by_id[i] for i in data['asset_ids']]}
            for name, data in profile['sections'].items()},
            'script_records': [index.script_by_id[i] for i in profile['scripts']['script_ids']],
            'script_calls': [call for i in profile['scripts']['script_ids'] for call in index.script_calls[i]],
            'native_functions': [index.functions[fid] for fid in sorted({call['function_id']
                for i in profile['scripts']['script_ids'] for call in index.script_calls[i]
                if call.get('function_id') in index.functions})]}
        print(json.dumps(result, ensure_ascii=False, indent=2))
    else:
        print((ctx.out / profile['page']).read_text(encoding='utf-8'))
    return 0


def coverage_check(ctx):
    profiles = read_json(ctx.out / 'data/mob-profiles.json', [])
    actors = read_json(ctx.out / 'data/actors.json', [])
    errors = []
    if Counter(p.get('id') for p in profiles) != Counter(a['id'] for a in actors):
        errors.append('mob profiles: missing, duplicate or stale actor profile')
    index = Index(ctx)
    for profile in profiles:
        if not (getattr(ctx, 'view_root', ctx.out) / profile.get('page', '')).is_file():
            errors.append('mob profiles: missing page for ' + str(profile.get('name')))
        for section in profile.get('sections', {}).values():
            for field, known in [('table_ids', index.by_id), ('reference_ids', index.ref_by_id), ('asset_ids', index.asset_by_id)]:
                if any(i not in known for i in section.get(field, [])):
                    errors.append('mob profiles: dangling ' + field + ' for ' + str(profile.get('name')))
        if not isinstance(profile.get('drops'), dict):
            errors.append('mob profiles: drop graph missing for ' + str(profile.get('name')))
        if set(profile.get('sections', {})) != set(ROOTS) | {'script_resources'}:
            errors.append('mob profiles: section coverage missing for ' + str(profile.get('name')))
        actor = next((a for a in actors if a['id'] == profile.get('id')), {})
        for section, names in ROOTS.items():
            for name in names:
                link = actor.get('field_links', {}).get(name)
                if link and link.get('target_id') not in profile.get('sections', {}).get(section, {}).get('table_ids', []):
                    errors.append('mob profiles: omitted source root ' + name + ' for ' + str(profile.get('name')))
    return errors
