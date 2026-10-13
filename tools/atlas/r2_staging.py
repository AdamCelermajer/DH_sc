"""Round-2 staging-gap ledger and source-site scope enumerator.

The original atlas files and all source corpora are read-only.  Scope is kept
as small JSON shards so the tracked output stays well below GitHub's file cap.
Classification is intentionally separate from enumeration: a missing or
dynamic reference stays unresolved until its source rule is proven.
"""
from collections import Counter, defaultdict, deque
import hashlib
import json
from pathlib import Path
import re

from common import read_json, write_json


STAGING = Path('r2/staging')
REF_FILES = ('references', 'references-extra', 'script-references', 'campaign-references')
PLAYER_CLASS_NAMES = {
    'knight', 'knightberserker', 'knightpaladin', 'rogue', 'roguearcher',
    'rogueassassin', 'mage', 'mageillusionist', 'magenecromancer',
}
ACT2_SCENES = {
    '2e1304ff2c8ed98caa94': '003_darkwood',
    'a9e0f80e1d8689ad5593': '036_underworld_hub',
    '9fc3c1a42c6b469bbf26': '037_machine_rooms_01',
}
ACT2_SCOPE_PATH = Path('r2/act2/scope.json')
RESOURCE_KINDS = {'anim', 'animation event', 'animation_dictionary', 'animated effect',
                  'effect set', 'effect_set', 'model', 'model component', 'projectile',
                  'sound', 'texture', 'named_reference', 'numeric_reference', 'symbolic_reference',
                  'serialized_string', 'data file', 'script', 'character', 'item', 'faery',
                  'quest', 'level', 'loot', 'skill', 'item power', 'class', 'faction'}


def _data(ctx, name, default=None):
    return read_json(ctx.out / 'data' / (name + '.json'), default)


def _norm(value):
    return str(value or '').replace('\\', '/').casefold()


def _canonical_source(ctx, value):
    """Convert an exact worktree-local absolute source path to atlas site form."""
    raw = str(value or '').replace('\\', '/')
    offset = raw.casefold().find('/.local-inputs/')
    if offset >= 0:
        return '.local-inputs/' + raw[offset + len('/.local-inputs/'):]
    try:
        return '.local-inputs/' + Path(value).resolve().relative_to(ctx.inputs.resolve()).as_posix()
    except (ValueError, OSError):
        return raw


def _entity_targets(row):
    targets = []
    for edge in row.get('field_edges') or []:
        for target in edge.get('targets') or []:
            if target.get('id'):
                targets.append(target['id'])
    for link in (row.get('field_links') or {}).values():
        if link.get('target_id'):
            targets.append(link['target_id'])
    return targets


def _reference_targets(row):
    found = []
    def walk(value):
        if isinstance(value, dict):
            if str(value.get('type', '')).casefold() == 'table-row' and value.get('id'):
                found.append(value['id'])
            if value.get('id', '').startswith('entity:'):
                found.append(value['id'])
            for child in value.values():
                walk(child)
        elif isinstance(value, list):
            for child in value:
                walk(child)
    walk(row.get('resolves_to'))
    walk(row.get('derived_edges'))
    return found


def _profile_sources(ctx, profile_ids):
    """Return all source file paths attributed to reached resource profiles."""
    ra = _data(ctx, 'resource-attributions', {}) or {}
    assets = ra.get('resource_files_by_id') or {}
    profiles = ra.get('resource_profiles_by_id') or {}
    paths = set()
    for profile_id in profile_ids:
        profile = profiles.get(profile_id) or {}
        for aid in profile.get('asset_ids', []) + profile.get('candidate_asset_ids', []):
            asset = assets.get(aid) or {}
            source = asset.get('source') or {}
            source_path = source.get('path') if isinstance(source, dict) else None
            for path in (asset.get('path'), source_path):
                if path:
                    paths.add(_norm(path))
            for source in asset.get('sources') or []:
                if source.get('path'):
                    paths.add(_norm(source['path']))
            for source in asset.get('android_or_recovered_sources') or []:
                if isinstance(source, dict) and source.get('path'):
                    paths.add(_norm(source['path']))
    return paths


def _reference_rows(ctx):
    rows = {}
    duplicates = Counter()
    for name in REF_FILES:
        for row in (_data(ctx, name, []) or []):
            rid = row.get('id')
            if not rid:
                continue
            duplicates[rid] += 1
            prev = rows.get(rid)
            if prev is None or len(json.dumps(row, ensure_ascii=False)) > len(json.dumps(prev, ensure_ascii=False)):
                rows[rid] = row
    return rows, duplicates


def _add_ref(ids, rid, reason):
    if rid:
        ids.setdefault(str(rid), set()).add(reason)


def _all_site_rows(ctx):
    refs, duplicates = _reference_rows(ctx)
    refs_by_file = defaultdict(list)
    refs_by_script_stem = defaultdict(list)
    for ref in refs.values():
        context = ref.get('context') or {}
        file_path = _norm(context.get('file') or ref.get('source_file'))
        if file_path:
            refs_by_file[file_path].append(ref)
        script_path = str(context.get('script') or '')
        if script_path:
            refs_by_script_stem[Path(script_path).stem.casefold()].append(ref)
    entities = _data(ctx, 'entity-profiles', []) or []
    entity_by_id = {row.get('id'): row for row in entities if row.get('id')}
    actors = _data(ctx, 'actors', []) or []
    campaign = _data(ctx, 'campaign', []) or []
    campaign_refs = _data(ctx, 'campaign-references', []) or []
    levels = _data(ctx, 'levels', []) or []
    membership = _data(ctx, 'act-membership', {}) or {}

    # The Act 2 edge closure is authored by Package C. Its act label remains an
    # inference; exact command targets and scene IDs are preserved separately.
    act2_scope_input = read_json(ctx.out / ACT2_SCOPE_PATH, {}) or {}
    act2_scene_ids = set()
    act2_scene_names = {}
    act2_scene_sources = {}
    act1_scene_ids = {str(level.get('id')) for level in levels
                      if level.get('act') == 'act_1' and level.get('id')}
    for item in act2_scope_input.get('levels') or []:
        if not isinstance(item, dict):
            continue
        scene_id = item.get('scene_id') or item.get('id')
        if not scene_id:
            continue
        scene_id = str(scene_id)
        if item.get('membership_candidate') == 'act_1_backlink':
            act1_scene_ids.add(scene_id)
        else:
            # C's closure includes the inferred start and unclassified reached
            # scenes; retain the distinction in metadata instead of claiming
            # every reached scene is narratively Act 2.
            act2_scene_ids.add(scene_id)
        act2_scene_names[scene_id] = str(item.get('stem') or item.get('scene') or item.get('name') or scene_id)
        if item.get('source') or item.get('source_path'):
            act2_scene_sources[scene_id] = item.get('source') or item.get('source_path')
    if not act2_scene_ids or not act2_scope_input.get('levels'):
        raise RuntimeError(f'Authoritative Package C scope missing/incomplete: {ctx.out / ACT2_SCOPE_PATH}')
    act2_scope_source = act2_scope_input.get('stopping_rule') or act2_scope_input.get('scope_label') or (
        'Package C authored transition closure rooted at 003_darkwood; act membership remains inferred/unknown.')
    for level in levels:
        if level.get('act') == 'act_2_inferred' and level.get('id'):
            act2_scene_ids.add(str(level['id']))

    ids_by_act = {'act_1': {}, 'act_2_inferred': {}}
    entity_queue_by_act = {'act_1': deque(), 'act_2_inferred': deque()}
    reached_entities = {'act_1': set(), 'act_2_inferred': set()}
    asset_paths_by_act = {'act_1': set(), 'act_2_inferred': set()}

    scoped_actor_ids = {'act_1': {a.get('id') for a in actors if a.get('act') == 'act_1'},
                        'act_2_inferred': {a.get('id') for a in actors if a.get('act') == 'act_2_inferred'}}
    for level in levels:
        level_act = 'act_1' if level.get('act') == 'act_1' or str(level.get('id')) in act1_scene_ids else ('act_2_inferred' if str(level.get('id')) in act2_scene_ids else None)
        if level_act:
            for candidate in level.get('module_spawn_candidates') or []:
                if candidate.get('actor_id'):
                    scoped_actor_ids[level_act].add(candidate['actor_id'])
    actors_by_id = {a.get('id'): a for a in actors}
    for act, actor_ids in scoped_actor_ids.items():
        for actor_id in actor_ids:
            actor = actors_by_id.get(actor_id) or {}
            if not actor:
                continue
            why = f"CharacterProperties row {actor.get('index')} {actor.get('name')} ({actor_id}); membership {act}"
            for field, link in (actor.get('field_links') or {}).items():
                _add_ref(ids_by_act[act], link.get('reference_id'), why + f" -> {field}")
                if link.get('target_id'):
                    entity_queue_by_act[act].append((link['target_id'], why + f" -> {field}"))
            # AI names are authored table links. Only exact same-name script profiles
            # are followed; string candidates from unrelated scripts are not promoted.
            ai_name = str(actor.get('ai_script') or '').casefold()
            if ai_name:
                for p in entities:
                    src = p.get('source') or {}
                    if p.get('kind') == 'script' and str(src.get('name') or '').casefold() == ai_name:
                        entity_queue_by_act[act].append((p['id'], why + f" -> AI script table name {actor.get('ai_script')}"))

    # Campaign-level source rows contain the exact scope labels assigned by the
    # original atlas. Act 2 uses the explicit transition closure from Package C.
    for row in campaign_refs:
        scene_id = row.get('scene_id')
        act = row.get('act')
        if act == 'act_1' or str(scene_id) in act1_scene_ids:
            target_act = 'act_1'
        elif scene_id in act2_scene_ids:
            target_act = 'act_2_inferred'
        else:
            continue
        _add_ref(ids_by_act[target_act], row.get('id'),
                 f"campaign {row.get('scene_id')} {row.get('script_name')} command[{row.get('command_index')}] {row.get('field_offset')}")
        for target_id in _reference_targets(row):
            entity_queue_by_act[target_act].append((target_id, f"campaign target from {row.get('site')}"))

    # Direct scene and script rows preserve canonical content chains even when
    # they have no file-valued reference (e.g. dialogue IDs / quest state fields).
    source_paths_by_act = {'act_1': set(), 'act_2_inferred': set()}
    # Every reached scene's canonical binary sources and module variants are
    # explicit Package-C evidence. Include their indexed sites even when a
    # module has no static actor candidate.
    for scene in act2_scope_input.get('levels') or []:
        scene_id = str(scene.get('scene_id') or scene.get('id') or '')
        act = 'act_1' if scene_id in act1_scene_ids else ('act_2_inferred' if scene_id in act2_scene_ids else None)
        if not act:
            continue
        for source in (scene.get('source'), scene.get('script_source_commands'), scene.get('script_source_names')):
            if not source:
                continue
            canonical = _canonical_source(ctx, source)
            source_paths_by_act[act].add(_norm(canonical))
            for ref in refs_by_file.get(_norm(canonical), []):
                _add_ref(ids_by_act[act], ref.get('id'), f"C reached scene source {scene.get('scene')} {canonical}")
    for module in act2_scope_input.get('module_sources') or []:
        scene_id = str(module.get('scene_id') or '')
        act = 'act_1' if scene_id in act1_scene_ids else ('act_2_inferred' if scene_id in act2_scene_ids else None)
        source = module.get('path')
        if not act or not source:
            continue
        canonical = _canonical_source(ctx, source)
        source_paths_by_act[act].add(_norm(canonical))
        for ref in refs_by_file.get(_norm(canonical), []):
            _add_ref(ids_by_act[act], ref.get('id'), f"C module source {module.get('scene')} {canonical}")
    for edge in act2_scope_input.get('edges') or []:
        scene_id = str(edge.get('source_scene_id') or '')
        act = 'act_1' if scene_id in act1_scene_ids else ('act_2_inferred' if scene_id in act2_scene_ids else None)
        source = edge.get('source_path') or edge.get('source_site')
        if act and source:
            canonical = _canonical_source(ctx, source)
            source_paths_by_act[act].add(_norm(canonical))
            for ref in refs_by_file.get(_norm(canonical), []):
                _add_ref(ids_by_act[act], ref.get('id'),
                         f"C source edge {edge.get('source_scene')} -> {edge.get('target_scene')} via {canonical}")
    for level in levels:
        act = 'act_1' if level.get('act') == 'act_1' or str(level.get('id')) in act1_scene_ids else ('act_2_inferred' if str(level.get('id')) in act2_scene_ids else None)
        if not act:
            continue
        scene = level.get('source') or ''
        if scene:
            source_paths_by_act[act].add(_norm(scene))
            for ref in refs_by_file.get(_norm(scene), []):
                _add_ref(ids_by_act[act], ref.get('id'), f"scene source {scene}")
        # The MGP path on a direct module candidate is the exact extracted source
        # path. These source files contain their own model/texture/effect sites.
        for candidate in level.get('module_spawn_candidates') or []:
            module = candidate.get('module')
            if not module:
                continue
            source_paths_by_act[act].add(_norm(module))
            for ref in refs_by_file.get(_norm(module), []):
                _add_ref(ids_by_act[act], ref.get('id'), f"scene module source {module}")

    # All authored skills from the nine Prince class/subclass skill lists, all
    # faeries, and the complete item catalog are deliberately in scope for both
    # acts because these are player-wide resources available across the chain.
    for profile in entities:
        kind = profile.get('kind')
        if kind == 'skill':
            owner_names = {str((owner.get('source') or {}).get('name') or '').casefold()
                           for owner in profile.get('owners') or []}
            include = bool(owner_names & PLAYER_CLASS_NAMES)
            why = 'Prince player skill row owned by ' + ', '.join(sorted(owner_names & PLAYER_CLASS_NAMES))
        elif kind == 'faery':
            include, why = True, 'faery resource catalog row'
        elif kind == 'item':
            include, why = True, 'item resource catalog row'
        else:
            continue
        if include:
            for act in ('act_1', 'act_2_inferred'):
                entity_queue_by_act[act].append((profile.get('id'), why))

    # Walk authored field links and collect script call sites as well as the
    # resource profiles they directly reference. The graph is finite and keyed
    # by exact atlas entity IDs; no filename alias is guessed here.
    while any(entity_queue_by_act.values()):
        for act, queue in entity_queue_by_act.items():
            if not queue:
                continue
            profile_id, why = queue.popleft()
            if not profile_id or profile_id in reached_entities[act]:
                continue
            reached_entities[act].add(profile_id)
            profile = entity_by_id.get(profile_id)
            if not profile:
                continue
            for edge in profile.get('field_edges') or []:
                _add_ref(ids_by_act[act], edge.get('id'), why + ' -> ' + str(edge.get('site') or profile_id))
                for target in edge.get('targets') or []:
                    entity_queue_by_act[act].append((target.get('id'), why + ' -> ' + str(edge.get('site') or profile_id)))
            sr = profile.get('script_resources') or {}
            for rid in sr.get('reference_ids') or []:
                _add_ref(ids_by_act[act], rid, why + ' -> owned script reference')
            scripts = profile.get('scripts') or {}
            for rid in scripts.get('script_ids') or []:
                entity_queue_by_act[act].append((rid, why + ' -> owned script'))
                sp = entity_by_id.get(rid) or {}
                src = sp.get('source') or {}
                if src.get('name'):
                    # Exact original script inventory site, if present.
                    for srrow in refs_by_script_stem.get(str(src['name']).casefold(), []):
                        _add_ref(ids_by_act[act], srrow.get('id'), why + ' -> exact owned script ' + src['name'])

    # Use actual package/source attribution IDs for nested BDAE/audio string
    # sites. A resource profile may lack file bytes; those remain represented by
    # its table field edges and later receive an explicit missing/ambiguous state.
    for act in ('act_1', 'act_2_inferred'):
        asset_paths_by_act[act] = _profile_sources(ctx, reached_entities[act])
        for rid, row in refs.items():
            file_path = _norm((row.get('context') or {}).get('file') or row.get('source_file'))
            if file_path and file_path in asset_paths_by_act[act]:
                _add_ref(ids_by_act[act], rid, 'nested reference site inside attributed resource file')

    # Keep every site, including non-resource references, but mark unsupported
    # atlas rows explicitly. These are not silently counted as corpus-absent.
    site_rows = {}
    for act, id_map in ids_by_act.items():
        for rid, reasons in id_map.items():
            row = refs.get(rid)
            if row is None:
                row = {'id': rid, 'site': None, 'kind': 'unindexed_reference', 'name': None,
                       'status': 'UNRESOLVED', 'reason_code': 'REFERENCE_ID_NOT_IN_ATLAS_INDEX',
                       'reason': 'Source owner points to a reference ID absent from the joined round-1 reference datasets.'}
            out = {k: row.get(k) for k in ('id', 'site', 'kind', 'name', 'status', 'reason_code', 'reason',
                                             'rule', 'confidence', 'reference_kind', 'context', 'candidates',
                                             'resolves_to', 'source_file', 'source_table', 'source_index',
                                             'source_name', 'target_table', 'target_index', 'target_name') if k in row}
            out.update(reference_id=rid, act=act, state='TODO', work_status='TODO',
                       scope_reasons=sorted(reasons), confidence=row.get('confidence', 'GUESS'))
            site_rows[(act, rid)] = out
    meta = {
        'schema': 'atlas-r2-staging-scope-v1',
        'act2_source': act2_scope_source,
        'act2_scene_ids': sorted(act2_scene_ids),
        'act2_levels': [act2_scene_names.get(sid, ACT2_SCENES.get(sid, sid)) for sid in sorted(act2_scene_ids)],
        'act2_scope_input': ACT2_SCOPE_PATH.as_posix() if act2_scope_input else None,
        'act2_edges': act2_scope_input.get('edges', []),
        'act2_uncertain_edges': act2_scope_input.get('uncertain_edges', []),
        'campaign_rows': {act: sum(1 for r in campaign_refs if (r.get('act') == 'act_1' if act == 'act_1' else r.get('scene_id') in act2_scene_ids)) for act in ('act_1', 'act_2_inferred')},
        'actor_rows': {act: len(scoped_actor_ids[act]) for act in ('act_1', 'act_2_inferred')},
        'scene_module_source_files': {act: len(source_paths_by_act[act]) for act in ('act_1', 'act_2_inferred')},
        'entity_profiles_reached': {act: len(v) for act, v in reached_entities.items()},
        'duplicate_reference_ids_across_round1_inputs': sum(1 for n in duplicates.values() if n > 1),
        'duplicate_input_rows': sum(n - 1 for n in duplicates.values() if n > 1),
        'known_limits': [
            'Act membership remains inferred as recorded by the atlas / transition evidence.',
            'Campaign command strings are exact authored fields; branch execution and runtime selection are not inferred.',
            'Player skills, faeries, and items are assigned to both acts because they are cross-act player resources.',
            'Only exact atlas entity/profile joins are followed; dynamic native-generated names remain explicit unresolved scope rows where indexed.',
        ],
    }
    return site_rows, meta


def enumerate_scope(ctx):
    site_rows, meta = _all_site_rows(ctx)
    root = ctx.out / STAGING / 'scope'
    files = []
    counts = {}
    expected_paths = set()
    for act in ('act_1', 'act_2_inferred'):
        by_kind = defaultdict(list)
        for (row_act, _), row in site_rows.items():
            if row_act == act:
                by_kind[str(row.get('kind') or 'unknown')].append(row)
        counts[act] = {}
        for kind, values in sorted(by_kind.items()):
            values.sort(key=lambda x: (str(x.get('site') or ''), x['reference_id']))
            # One shard per kind; the largest expected kind is under the 40 MB cap.
            safe_kind = re.sub(r'[^a-z0-9_-]+', '_', kind.casefold()).strip('_') or 'unknown'
            safe_kind += '-' + hashlib.sha1(kind.encode('utf-8')).hexdigest()[:6]
            path = root / act / f'{safe_kind}.json'
            write_json(path, {'schema': 'atlas-r2-staging-scope-shard-v1', 'act': act,
                              'kind': kind, 'count': len(values), 'rows': values})
            relative = path.relative_to(ctx.out).as_posix()
            files.append(relative)
            expected_paths.add(path.resolve())
            counts[act][kind] = len(values)
    # Remove only obsolete generated JSON shards inside this owned scope folder.
    # The directory is a package-B output root and contains no source corpus.
    for old_path in root.rglob('*.json') if root.exists() else []:
        if old_path.resolve() not in expected_paths:
            old_path.unlink()
    total = len(site_rows)
    id_act_counts = Counter(rid for (_, rid) in site_rows)
    unique_sites = len(id_act_counts)
    shared_sites = sum(1 for count in id_act_counts.values() if count > 1)
    families = ('anim', 'sound', 'effect', 'projectile', 'model', 'script', 'other/nonresource')
    family_counts = {}
    for act in ('act_1', 'act_2_inferred'):
        counter = Counter()
        for (row_act, _), row in site_rows.items():
            if row_act != act:
                continue
            kind = str(row.get('kind') or '').casefold()
            ref_kind = str(row.get('reference_kind') or '').casefold()
            if kind in {'anim', 'animation event', 'animation_dictionary', 'animation clip', 'animation sequence'}:
                family = 'anim'
            elif kind in {'sound', 'source_sound_ordinal'}:
                family = 'sound'
            elif kind in {'effect set', 'effect_set', 'animated effect'}:
                family = 'effect'
            elif kind == 'projectile':
                family = 'projectile'
            elif kind in {'model', 'model component', 'character', 'texture'}:
                family = 'model'
            elif kind == 'script' or ref_kind == 'script':
                family = 'script'
            else:
                family = 'other/nonresource'
            counter[family] += 1
        family_counts[act] = {family: counter.get(family, 0) for family in families}
    progress = {
        'schema': 'atlas-r2-package-progress-v1', 'package': 'B-staging-gap-ledger',
        'phase': 'STEP0_SCOPE_ENUMERATION', 'total': total, 'processed': 0,
        'status_counts': {'TODO': total}, 'scope_files': sorted(files),
        'counts_by_act_kind': counts, 'counts_by_act_family': family_counts,
        'unique_reference_sites': unique_sites, 'act_reference_pairs': total,
        'shared_reference_sites_across_acts': shared_sites,
        'act2_scope': meta,
        'scope_policy': 'One row per canonical source reference ID per act; no classification completed yet.',
    }
    write_json(ctx.out / 'r2' / 'package-b-progress.json', progress)
    write_json(ctx.out / STAGING / 'scope-index.json', {
        'schema': 'atlas-r2-staging-scope-index-v1', 'total': total,
        'unique_reference_sites': unique_sites, 'act_reference_pairs': total,
        'shared_reference_sites_across_acts': shared_sites,
        'files': sorted(files), 'counts_by_act_kind': counts,
        'counts_by_act_family': family_counts, 'act2_scope': meta,
    })
    return progress


def run(ctx):
    """Current stage is deterministic scope generation; classification follows root's Step-0 release."""
    return enumerate_scope(ctx)


def check(ctx):
    errors = []
    tests = []
    progress = read_json(ctx.out / 'r2' / 'package-b-progress.json', {}) or {}
    scope_index = read_json(ctx.out / STAGING / 'scope-index.json', {}) or {}
    files = scope_index.get('files') or []
    seen = set()
    rows = 0
    for relative in files:
        path = ctx.out / relative
        if not path.is_file():
            errors.append(f'missing scope shard: {relative}')
            continue
        if path.stat().st_size >= 40_000_000:
            errors.append(f'scope shard exceeds 40 MB: {relative}')
        payload = read_json(path, {}) or {}
        for row in payload.get('rows') or []:
            key = (payload.get('act'), row.get('reference_id'))
            if key in seen:
                errors.append(f'duplicate site in scope shards: {key}')
            seen.add(key)
            rows += 1
            if row.get('state') != 'TODO' or row.get('work_status') != 'TODO':
                errors.append(f'Step-0 row is not TODO: {key}')
    if rows != progress.get('total') or rows != scope_index.get('total'):
        errors.append(f'scope count mismatch: rows={rows}, progress={progress.get("total")}, index={scope_index.get("total")}')
    tests.append({'name': 'scope_shards_exist_are_small_unique_and_all_todo', 'rows': rows,
                  'files': len(files), 'passed': not errors})
    return {'passed': not errors, 'tests': tests, 'errors': errors}
