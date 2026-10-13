"""Compact lookup profiles for the atlas's non-actor entity roots.

Profiles store stable typed IDs and joins only.  Full source rows, references,
and assets stay in the atlas's canonical tables and ledgers.
"""
from collections import defaultdict
from pathlib import Path

from common import read_json, stable_id
import mobs


# These are independent source roots, not claims that every row is used at
# runtime.  Actor-to-skill and loot-to-item ownership is represented by exact
# authored table edges below.
ROOT_TABLES = {
    'skill_list': ('skills#group0',),
    'skill': ('skills#group1',),
    'item': ('loot_table#group3',),
    'item_power_list': ('item_powers#group0',),
    'item_power': ('item_powers#group1',),
    'monopoly_item_power': ('item_powers_monopoly#group0',),
    'faery_list': ('faeries#group0',),
    'faery': ('faeries#group1',),
    'projectile': ('projectiles#group0',),
    'projectile_model': ('projectiles_dictionary#group0',),
    'effect_set': ('effect_sets',),
    'animated_effect': ('effects_dictionary#group0',),
    'animation_sequence': ('animation_sequences',),
    'character_animation': ('character_animations',),
    'animation_clip': ('animations_dictionary#group0',),
    'sound': ('sdd_dungeon_hunter_2_iphone',),
}

EXPLANATIONS = {
    'skills#group0.List': 'Authored SkillListTable entries point to SkillTable row indices; this is class/list membership, not proof of learned or equipped state.',
    'skills#group1.Anim': 'SkillTable animation-sequence index. A negative sentinel means no sequence root.',
    'skills#group1.Script': 'SkillTable Lua source stem. Includes are followed only when the source argument is one exact string and the stem is unique.',
    'skills#group1.SkillIcon': 'Original SWF sprite 79 symbol label; empty selects the authored undefined frame. Symbol labels/frames come from the source_skill_art extraction.',
    'item_powers#group0.List': 'Authored item-power list entries; exact children are ItemPowerTable rows.',
    'item_powers#group1.SpecialEffect': 'Authored effect-set index when nonnegative and resolved by the exact source reference.',
    'item_powers#group1.AttrMonopoly': 'Authored monopoly-table index when nonnegative and resolved by the exact source reference.',
    'loot_table#group3.AudioVisualID': 'ItemTable audiovisual row; that row can resolve pickup/drop sound indices and a visual token.',
    'loot_table#group3.ModularModule': 'Serialized Item.name/ModularModule token consumed by Character::INV_UpdateSkin. Weapon model URI and modular body controller lookups are resolved only through their recovered consumer rules and source assets.',
    'loot_table#group3.SwooshFX': 'Authored effect-set index used by item-related effects where present.',
    'loot_table#group3.SwooshSoundFX': 'Authored sound-row index where present.',
    'loot_audiovisual#group0.AudioDrop': 'Authored sound-row index for a drop cue.',
    'loot_audiovisual#group0.AudioPickup': 'Authored sound-row index for a pickup cue.',
    'loot_audiovisual#group0.Visual': 'Authored item-drop scene token passed to the fixed itemdrops.bdae source scene. Token presence is reported separately from a parsed/verified scene-node binding.',
    'faeries#group1.ModelFile': 'Faery model-dictionary index.',
    'faeries#group1.SpellScript': 'Faery script source stem.',
    'projectiles#group0.Model': 'Projectile model-dictionary index.',
    'projectiles#group0.ImpactFX': 'Projectile impact effect-set index.',
    'projectiles#group0.ExpireFX': 'Projectile expiry effect-set index.',
    'projectiles_dictionary#group0.Path': 'Projectile model source path.',
    'animations_dictionary#group0.Path': 'Animation clip source path.',
    'animation_sequences.Steps': 'Animation-step records carry clip/redirect, FX, sound and timing fields; the source ledger preserves exact references per step.',
    'effect_sets.steps.File': 'Effect-set step points to an animated effect dictionary entry when the source reference resolves.',
}

ITEMDROP_URI = 'data/3d/gameobjects/itemdrops.bdae'
WEAPON_URI_PREFIX = 'data/3d/characters/prince/weapons/'
WEAPON_URI_SUFFIX = '.bdae'
SKILL_ART_REPORT = 'port/windows-foundation/features/skill_ui/source_skill_art.json'
ITEMDROP_VERIFIED_VISUALS = {
    'dummy_itemdrop_potion': {'item_index': 925, 'meaning': 'SOURCE_SUBTREE_VERIFIED'},
    'root_itemdrop_gold_01': {'item_index': 418, 'meaning': 'SOURCE_CONTROLLER_REST_POSE_VERIFIED'},
}


def _fields(row):
    value = row.get('fields') or {}
    return value.get('fields', value)


def _typed(table, index):
    return f'entity:{table}:{int(index)}'


def _edge_target(ref):
    targets = []
    for target in ref.get('resolves_to') or []:
        source = target.get('source_row') or {}
        table = source.get('table', target.get('table'))
        index = source.get('index', target.get('index'))
        if table is not None and isinstance(index, int):
            targets.append({'id': _typed(table, index), 'table': table, 'index': index,
                            'name': source.get('name', target.get('name')),
                            'status': ref.get('status')})
        elif target.get('path') or target.get('asset_files'):
            targets.append({'path': target.get('path'), 'asset_files': [f.get('path') for f in target.get('asset_files') or []],
                            'status': ref.get('status')})
    return targets


def _profile(ctx, index, kind, row, owner_refs=()):
    table, row_index = row['table'], int(row['index'])
    key = (table, row_index)
    joined = index.walk([key])
    row_refs = list(index.row_refs.get(key, []))
    field_refs = []
    for ref in row_refs:
        context = ref.get('context') or {}
        field_refs.append({
            'id': ref['id'], 'field': context.get('field'), 'site': ref.get('site'),
            'kind': ref.get('reference_kind', ref.get('kind')),
            'status': ref.get('status'), 'reason': ref.get('candidate_note') or ref.get('reason'),
            'targets': _edge_target(ref),
        })
    fields = _fields(row)
    root_fields = {}
    for field, value in fields.items():
        if isinstance(value, (str, int, float, bool)) or value is None:
            root_fields[field] = value
    script_names = []
    for field in ('Script', 'SpellScript', 'ScriptName'):
        value = fields.get(field)
        if isinstance(value, str) and value:
            script_names.append(value)
    scripts = index.script_closure(script_names) if script_names else {'script_ids': [], 'include_edges': [], 'gaps': []}
    script_refs = [ref for sid in scripts['script_ids']
                   for ref in index.script_refs.get(index.script_by_id[sid]['site'], [])]
    resources = index.walk([], script_refs) if script_refs else {
        'table_ids': [], 'reference_ids': [], 'asset_ids': [], 'gaps': [], 'reference_statuses': {}}
    audio_ids = sorted({asset_id for aid in joined['asset_ids'] + resources['asset_ids']
                        for asset_id in [aid]
                        if Path(index.asset_by_id[asset_id]['path']).suffix.casefold() in ('.wav', '.vxn', '.mp3', '.ogg')})
    owners = list(owner_refs)
    return {
        'id': _typed(table, row_index),
        'stable_id': stable_id('entity-profile', kind, table, row_index),
        'kind': kind,
        'source': {'table': table, 'index': row_index, 'row_id': row.get('id'),
                   'name': row.get('name'), 'status': row.get('status')},
        'root_fields': root_fields,
        'field_explanations': {f'{table}.{field}': EXPLANATIONS[f'{table}.{field}']
                               for field in fields if f'{table}.{field}' in EXPLANATIONS},
        'field_edges': field_refs,
        'owners': owners,
        'joined': joined,
        'scripts': scripts,
        'script_resources': resources,
        'audio_asset_ids': audio_ids,
        'coverage': 'SOURCE_JOINED_PARTIAL',
        'runtime_use': 'NOT_INFERRED_FROM_AUTHORED_ASSOCIATION',
    }


def _reverse_authored_edges(index):
    """Index exact table-cell references pointing at known source rows."""
    incoming = defaultdict(list)
    for ref in index.refs:
        context = ref.get('context') or {}
        source_table, source_index, field = context.get('table'), context.get('index'), context.get('field')
        if source_table is None or not isinstance(source_index, int):
            continue
        source = index.rows.get((source_table, source_index))
        for target in ref.get('resolves_to') or []:
            sr = target.get('source_row') or {}
            table, row_index = sr.get('table', target.get('table')), sr.get('index', target.get('index'))
            if table is None or not isinstance(row_index, int):
                continue
            incoming[(table, row_index)].append({
                'source': {'id': source.get('id') if source else _typed(source_table, source_index),
                           'table': source_table, 'index': source_index,
                           'name': source.get('name') if source else None},
                'field': field, 'reference_id': ref.get('id'), 'status': ref.get('status'),
                'meaning': 'EXACT_AUTHORED_SOURCE_REFERENCE',
            })
    return incoming


def _data_path(value):
    """Normalize source URIs without discarding the suffix or basename."""
    value = str(value or '').replace('\\', '/').strip().casefold()
    value = value.removeprefix('data/')
    return value.lstrip('/')


def _asset_path_index(assets):
    paths = defaultdict(list)
    seen = defaultdict(set)
    for asset in assets:
        for value in (asset.get('path'), asset.get('relative'), *(asset.get('logical_paths') or [])):
            normalized = _data_path(value)
            if normalized and asset.get('id') not in seen[normalized]:
                seen[normalized].add(asset.get('id'))
                paths[normalized].append(asset)
    return paths


def _source_file(ctx, asset):
    path = str(asset.get('path') or '')
    if path.startswith('.local-inputs/'):
        return ctx.inputs / path[len('.local-inputs/'):]
    if path.startswith('.repo/'):
        return ctx.root / path[len('.repo/'):]
    return ctx.root / path


def _asset_summary(asset):
    return {'id': asset['id'], 'path': asset.get('path'), 'relative': asset.get('relative'),
            'logical_paths': asset.get('logical_paths') or [], 'source': asset.get('source'),
            'sha256': asset.get('sha256')}


def _resource_edge(source, field, kind, status, target=None, evidence='', note=None):
    identity = ''
    if target:
        identity = (target.get('id') or target.get('uri') or target.get('controller_uri')
                    or (target.get('template_asset') or {}).get('id')
                    or next((a.get('id') for a in target.get('assets') or []), ''))
    edge = {'id': stable_id('derived-entity-resource', source, field, kind,
                            identity),
            'source': source, 'field': field, 'kind': kind, 'status': status,
            'evidence': evidence}
    if target is not None:
        edge['target'] = target
    if note:
        edge['note'] = note
    return edge


def resolve_item_resources(ctx, refs, tables, assets):
    """Return compact derived edges for source-proven item/skill resources.

    This is intentionally supplemental to the canonical reference ledger: it
    applies recovered consumer rules where the table reference extractor has
    no exact target row. Full row/asset bodies remain in the canonical data.
    Candidates are never promoted to runtime use or a selected class model.
    """
    rows = {(r.get('table'), int(r['index'])): r for r in tables
            if isinstance(r.get('index'), int)}
    assets_by_path = _asset_path_index(assets)
    # Recover exact character->model dictionary ownership from typed table
    # references. This preserves every owner/candidate instead of selecting a
    # class or treating a shared prince model as the only legal body.
    body_models = defaultdict(list)
    for ref in refs:
        context = ref.get('context') or {}
        if context.get('table') != 'character_properties#group0' or context.get('field') != 'ModelFile':
            continue
        for target in ref.get('resolves_to') or []:
            source_row = target.get('source_row') or {}
            table = source_row.get('table', target.get('table'))
            model_index = source_row.get('index', target.get('index'))
            if table != 'character_models_dictionary#group0' or not isinstance(model_index, int):
                continue
            character = rows.get(('character_properties#group0', context.get('index')))
            model = rows.get((table, model_index))
            body_models[model_index].append({
                'character': {'id': character.get('id') if character else _typed('character_properties#group0', context.get('index')),
                              'index': context.get('index'), 'name': character.get('name') if character else None},
                'model': {'id': model.get('id') if model else _typed(table, model_index),
                          'table': table, 'index': model_index,
                          'name': model.get('name') if model else None},
            })

    model_asset_candidates = []
    for (table, model_index), model in rows.items():
        if table != 'character_models_dictionary#group0':
            continue
        model_path = _fields(model).get('Path')
        if not isinstance(model_path, str) or not model_path:
            continue
        for asset in assets_by_path.get(_data_path(model_path), []):
            if Path(asset.get('path', '')).suffix.casefold() != '.bdae':
                continue
            model_asset_candidates.append((model, model_path, asset,
                                           body_models.get(model_index, [])))

    body_blob_cache = {}

    def itemdrop_assets():
        return assets_by_path.get(_data_path(ITEMDROP_URI), [])

    edges = []
    # Item ModularModule is Item.name at original record offset 0x50. Source
    # INV_UpdateSkin calls SetWeaponSkin for its first two weapon slots; that
    # consumer constructs exactly this URI. The suffix/prefix are not guesses.
    for (table, row_index), row in rows.items():
        if table != 'loot_table#group3':
            continue
        module = _fields(row).get('ModularModule')
        source = {'id': _typed(table, row_index), 'row_id': row.get('id'),
                  'table': table, 'index': row_index, 'name': row.get('name')}
        if isinstance(module, str) and module.startswith(('MC_RWeapon_', 'MC_LWeapon_')):
            uri = WEAPON_URI_PREFIX + module + WEAPON_URI_SUFFIX
            candidates = assets_by_path.get(_data_path(uri), [])
            found = [_asset_summary(a) for a in candidates]
            edges.append(_resource_edge(source, 'ModularModule', 'weapon_model',
                'RESOLVED_SOURCE_URI' if found else 'SOURCE_URI_ASSET_MISSING',
                {'uri': uri, 'assets': found} if found else {'uri': uri},
                'IDA Character::INV_UpdateSkin 0x3a999c gates weapon slots; VisualObject::SetWeaponSkin 0x473cd8 constructs data/3d/characters/prince/weapons/<name>.bdae.',
                'The Item row describes an authored module; this edge does not prove the item is generated, equipped, or selected at runtime.'))

        controller_id = ('#' + module + '-mesh-skin'
                         if isinstance(module, str) and module
                         and not module.startswith(('MC_RWeapon_', 'MC_LWeapon_')) else None)
        body_matches = []
        for model, model_path, asset, owners in model_asset_candidates if controller_id else []:
            asset_key = asset['id']
            if asset_key not in body_blob_cache:
                try:
                    body_blob_cache[asset_key] = _source_file(ctx, asset).read_bytes()
                except OSError:
                    body_blob_cache[asset_key] = None
            blob = body_blob_cache[asset_key]
            if blob is None or controller_id.encode('utf-8') not in blob:
                continue
            body_matches.append({'asset': _asset_summary(asset),
                                 'model': {'id': model.get('id') or _typed(model['table'], model['index']),
                                           'table': model['table'], 'index': model['index'],
                                           'name': model.get('name'), 'path': model_path},
                                 'owners': owners})
        # Module strings in source BRES controller libraries prove a module
        # candidate exists in that model. Category-local membership is not
        # asserted here because this helper does not decode the scene descriptor.
        if body_matches and controller_id:
            edges.append(_resource_edge(source, 'ModularModule', 'modular_body_controller_candidate',
                'SOURCE_BRES_CONTROLLER_ID_PRESENT',
                {'controller_uri': controller_id, 'models': body_matches},
                'IDA VisualObject::GetModularModuleId 0x474568 probes #<module>-mesh-skin within its requested category; controller strings were matched byte-exactly in character model BRES files. The category descriptor itself is not decoded by this helper.',
                'Candidate models/character owners are retained together. Character::InitPost 0x3b4d60 admits the direct equipment initializer only for IsLocalPlayer; model defaults and class selection are separate.'))
        elif controller_id:
            edges.append(_resource_edge(source, 'ModularModule', 'modular_body_controller_candidate',
                'CONTROLLER_NOT_FOUND_IN_AVAILABLE_BODY_MODELS',
                {'controller_uri': controller_id},
                'IDA VisualObject::GetModularModuleId 0x474568 derives #<module>-mesh-skin. No candidate character-model BDAE contains that exact controller string.',
                'Does not rule out a model absent from the supplied asset corpus or a version-specific alternate model.'))

        audiovisual_id = _fields(row).get('AudioVisualID')
        if isinstance(audiovisual_id, int) and audiovisual_id >= 0:
            av_row = rows.get(('loot_audiovisual#group0', audiovisual_id))
            visual = _fields(av_row).get('Visual') if av_row else None
            if isinstance(visual, str) and visual:
                available_itemdrop_assets = itemdrop_assets()
                if not available_itemdrop_assets:
                    edges.append(_resource_edge(source, 'AudioVisualID.Visual', 'itemdrop_visual_token',
                        'TEMPLATE_ASSET_MISSING',
                        {'audiovisual': {'id': av_row.get('id') if av_row else _typed('loot_audiovisual#group0', audiovisual_id),
                                         'index': audiovisual_id}, 'visual_token': visual,
                         'template_uri': ITEMDROP_URI},
                        'Recovered ItemObject::InitOnce 0x3ece80 uses fixed data/3D/GameObjects/itemdrops.bdae and passes AudioVisual.Visual to the scene consumer.',
                        'The source template is unavailable in the supplied asset inventory.'))
                for asset in available_itemdrop_assets:
                    try:
                        key = asset['id']
                        if key not in body_blob_cache:
                            body_blob_cache[key] = _source_file(ctx, asset).read_bytes()
                        blob = body_blob_cache[key]
                    except OSError:
                        blob = None
                    present = blob is not None and visual.encode('utf-8') in blob
                    verified = ITEMDROP_VERIFIED_VISUALS.get(visual.casefold())
                    if (verified and verified.get('item_index') == row_index
                            and asset.get('source') == 'android'
                            and asset.get('sha256') == 'c4d783a23a158b4e52687d8ed718c584ee5ad90a325e0111c8da95ae86937610'):
                        status = verified['meaning']
                    else:
                        status = 'SOURCE_BRES_TOKEN_PRESENT' if present else 'SOURCE_BRES_TOKEN_UNRESOLVED'
                    edges.append(_resource_edge(source, 'AudioVisualID.Visual', 'itemdrop_visual_token',
                        status,
                        {'audiovisual': {'id': av_row.get('id') if av_row else _typed('loot_audiovisual#group0', audiovisual_id),
                                         'index': audiovisual_id},
                         'visual_token': visual,
                         'template_uri': ITEMDROP_URI,
                         'template_asset': _asset_summary(asset)},
                        'Recovered ItemObject::InitOnce 0x3ece80 uses fixed data/3D/GameObjects/itemdrops.bdae and passes AudioVisual.Visual to the scene consumer. Exact token byte presence is only a candidate node lookup except Potion0[925] and GoldStack01[418], which have source-scene receipts.',
                        'Token presence does not prove parsed subtree selection, pose, rendering, or runtime drop admission.'))

        icon_name = _fields(row).get('IconName')
        if not icon_name:
            edges.append(_resource_edge(source, 'IconName', 'item_icon', 'SOURCE_FIELD_EMPTY',
                evidence='Original ItemTable inventory audit: all 1,322 IconName entries are empty.',
                note='No icon asset is inferred from item name or equipment category.'))

    # Skill icon labels are symbols in the original SWF, not standalone image
    # filenames. Preserve the exact symbol/frame and source file identity.
    art_path = ctx.root / SKILL_ART_REPORT
    art = read_json(art_path, {}) if art_path.is_file() else {}
    icon_labels = {str(k).casefold(): int(v) for k, v in (art.get('icon_labels') or {}).items()}
    for (table, row_index), row in rows.items():
        if table != 'skills#group1':
            continue
        fields = _fields(row)
        icon = fields.get('SkillIcon')
        if not isinstance(icon, str):
            continue
        source = {'id': _typed(table, row_index), 'row_id': row.get('id'),
                  'table': table, 'index': row_index, 'name': row.get('name')}
        label = icon if icon else 'undefined'
        frame = icon_labels.get(label.casefold())
        status = 'RESOLVED_SWF_SYMBOL_FRAME' if frame is not None else 'SWF_SYMBOL_UNRESOLVED'
        art_source = str(art.get('source') or '').replace('\\', '/')
        source_asset_uri = ('.repo/' + art_source.lstrip('/') if art_source and not art_source.startswith(('.repo/', '.local-inputs/'))
                            else art_source)
        source_assets = assets_by_path.get(_data_path(source_asset_uri), [])
        # `source_skill_art.json` is a tracked catalog, while the SWF itself
        # is an inventoried .repo asset; graph.files() resolves the latter.
        edges.append(_resource_edge(source, 'SkillIcon', 'skill_icon_symbol', status,
            {'asset_uri': source_asset_uri, 'assets': [_asset_summary(a) for a in source_assets],
             'art_catalog_path': '.repo/' + SKILL_ART_REPORT,
             'sha256': art.get('sha256'), 'sprite_id': art.get('icon_sprite'),
             'label': label, 'frame': frame},
            'Original skill presenter matches SkillIcon to a label in dqcharmenu_droid.swf sprite 79; empty strings select authored label undefined. source_skill_art.json preserves the extracted label/frame map.',
            'A symbol/frame join does not assert that the skill is learned or currently displayed.'))

    derived_refs = [{'id': stable_id('entity-resource-reference', edge['id']),
                     'source': edge['source'], 'context': {'table': edge['source']['table'],
                         'index': edge['source']['index'], 'field': edge['field']},
                     'reference_kind': edge['kind'], 'status': edge['status'],
                     'resolves_to': [edge['target']] if edge.get('target') else [],
                     'evidence': edge['evidence'], 'derived_edge_id': edge['id']}
                    for edge in edges]
    return {'references': derived_refs, 'derived_edges': edges,
            'coverage': {'items': sum(1 for r in tables if r.get('table') == 'loot_table#group3'),
                         'skill_icon_rows': sum(1 for r in tables if r.get('table') == 'skills#group1'),
                         'derived_edges': len(edges)},
            'runtime_use': 'NOT_INFERRED_FROM_AUTHORED_ASSOCIATION'}


def build(ctx, index=None):
    """Build compact profiles for skills/items and shared media/script roots.

    `index` is an optional existing :class:`mobs.Index`; passing it avoids
    rereading the canonical source ledgers when the atlas build already owns
    one. The returned objects contain source IDs and relationships, not copies
    of the canonical table/reference/asset payloads.
    """
    index = index or mobs.Index(ctx)
    incoming = _reverse_authored_edges(index)
    row_kinds = {table: kind for kind, tables in ROOT_TABLES.items() for table in tables}
    profiles = []
    for row in index.tables:
        kind = row_kinds.get(row.get('table'))
        if kind:
            profiles.append(_profile(ctx, index, kind, row, incoming.get((row['table'], int(row['index'])), [])))

    # Script-only roots have no table row; references/resources still use the
    # same source index and exact script stem resolution rules as mob profiles.
    for script in index.scripts:
        refs = index.script_refs.get(script['site'], [])
        resources = index.walk([], refs)
        profiles.append({
            'id': f"script:{script['id']}", 'stable_id': stable_id('entity-profile', 'script', script['id']),
            'kind': 'script', 'source': {'script_id': script['id'], 'site': script['site'],
                'name': script.get('name'), 'status': script.get('status')},
            'root_fields': {}, 'field_explanations': {}, 'field_edges': [], 'owners': [],
            'joined': {'table_ids': [], 'reference_ids': sorted(r['id'] for r in refs),
                       'asset_ids': [], 'gaps': [], 'reference_statuses': {}},
            'scripts': index.script_closure([script['name']]), 'script_resources': resources,
            'audio_asset_ids': sorted({aid for aid in resources['asset_ids']
                if Path(index.asset_by_id[aid]['path']).suffix.casefold() in ('.wav', '.vxn', '.mp3', '.ogg')}),
            'coverage': 'SOURCE_JOINED_PARTIAL', 'runtime_use': 'NOT_INFERRED_FROM_AUTHORED_ASSOCIATION',
        })

    # Skill list membership is an explicit source edge. Record reverse owners
    # for each SkillTable row without treating it as equipped/unlocked state.
    list_rows = {r['index']: r for r in index.tables if r.get('table') == 'skills#group0'}
    owners = defaultdict(list)
    for list_index, list_row in list_rows.items():
        for position, skill_index in enumerate(_fields(list_row).get('list_entries', [])):
            owners[int(skill_index)].append({'source': {'id': list_row.get('id'),
                'table': 'skills#group0', 'index': list_index, 'name': list_row.get('name')},
                'field': f'List[{position}]', 'meaning': 'EXACT_AUTHORED_SKILL_LIST_MEMBERSHIP'})
    by_id = {p['id']: p for p in profiles}
    for skill_index, refs in owners.items():
        profile = by_id.get(_typed('skills#group1', skill_index))
        if profile:
            profile['owners'].extend(refs)

    # Keep the generated profile payload compact; the root graph can consume
    # the complete derived references/edges through resolve_item_resources().
    resource_graph = resolve_item_resources(ctx, index.refs, index.tables, index.assets)
    resource_ids = defaultdict(list)
    for edge in resource_graph['derived_edges']:
        resource_ids[edge['source']['id']].append(edge['id'])
    for profile in profiles:
        profile['derived_resource_edge_ids'] = sorted(resource_ids.get(profile['id'], []))
    return profiles
