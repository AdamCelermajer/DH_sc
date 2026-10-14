"""Round-2 native-scope enumeration and call-site evidence.

The module is source-only.  It reads the round-1 atlas ledgers, checks their
Lua call inventory against the supplied plaintext Lua files, and writes only
under ``coordination/atlas/r2``.  Native behavior is not inferred from a name,
address, or identifier match in the port.
"""
from collections import Counter, defaultdict
import json
from pathlib import Path
import re
import hashlib
import html
from collections import Counter, defaultdict

from common import read_json, write_json
import scripts as lua_scripts
import r2_native_contracts


SCOPE_PATH = Path('r2/natives/scope.json')
PROGRESS_PATH = Path('r2/package-a-progress.json')
REFINED_SCOPE_PATH = Path('r2/natives/scope-refined.json')
CONTRACT_ROOT = Path('r2/natives')
INDEX_PATH = Path('r2/natives/INDEX.md')
COMMON_SOURCES = (
    '.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac',
    '.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac',
)
CALL_MODES = {'global', 'object_method', 'qualified_function', 'alias_method', 'dynamic'}
HELPER_MODES = {'local_function', 'imported_script_function', 'script_function',
                 'lua_builtin', 'local_call_unknown', 'parameter_or_shadowed'}


def _data(ctx, name, default=None):
    return read_json(ctx.out / 'data' / (name + '.json'), default)


def _script_path(ctx, site):
    if str(site).startswith('.local-inputs/'):
        return ctx.inputs / str(site)[len('.local-inputs/'):]
    return ctx.root / str(site)


def _display_call(call):
    mode = call.get('mode') or call.get('call_mode') or ''
    name = call.get('name') or ''
    receiver = call.get('receiver')
    if mode in ('object_method', 'alias_method') and receiver:
        return f'{receiver}:{name}'
    if mode in ('qualified_function', 'lua_builtin') and receiver:
        return f'{receiver}.{name}'
    if mode == 'dynamic':
        return '<dynamic>'
    return name


def _read_source(path):
    return Path(path).read_text(encoding='utf-8', errors='replace')


def _ranked_functions(ctx):
    readme = ctx.out / 'README.md'
    if not readme.is_file():
        return []
    text = readme.read_text(encoding='utf-8')
    marker = '## Engine functions to implement next'
    if marker not in text:
        return []
    section = text.split(marker, 1)[1].split('\n## ', 1)[0]
    result = []
    for line in section.splitlines():
        if not line.startswith('| ') or line.startswith('| ---') or line.startswith('| Function'):
            continue
        cells = [cell.strip() for cell in line.strip().strip('|').split('|')]
        if len(cells) < 5:
            continue
        try:
            blocked = json.loads(cells[2])
        except (TypeError, json.JSONDecodeError):
            blocked = []
        result.append({
            'rank': len(result) + 1,
            'name': cells[0],
            'atlas_port_status_reference_only': cells[1],
            'blocked_boss_custom_scripts': blocked,
            'all_scripts': int(cells[3]),
            'call_sites': int(cells[4]),
            'source': 'coordination/atlas/README.md#Engine-functions-to-implement-next',
        })
    return result


def _membership_scripts(ctx, scripts_by_id, entities):
    """Return Act 1 source roots, preserving the atlas's inference label."""
    membership = _data(ctx, 'act-membership', {}) or {}
    levels = _data(ctx, 'levels', []) or []
    level_roots = [row for row in membership.get('level_rows', [])
                   if row.get('act') == 'act_1']
    actor_roots = [row for row in membership.get('actor_assignments', [])
                   if row.get('act') == 'act_1']
    entity_ids = {row.get('scene_id') for row in level_roots if row.get('scene_id')}
    entity_ids.update(row.get('actor_id') for row in actor_roots if row.get('actor_id'))
    # Atlas levels preserve direct XML/MGP Character candidates, which can be
    # more complete than the older serialized-spawn assignment ledger.
    for level in levels:
        if level.get('act') != 'act_1':
            continue
        for candidate in level.get('module_spawn_candidates') or []:
            if candidate.get('actor_id'):
                entity_ids.add(candidate['actor_id'])
    direct_script_ids = set()
    for entity_id in entity_ids:
        direct_script_ids.update((entities.get(entity_id) or {}).get('script_ids') or [])

    # Module-level script attributes are authored roots even where their value
    # names a callback/resource outside the supplied Lua inventory.  Package B's
    # Act 1 shards are the exact ledger of these scene/module references.
    module_script_refs = []
    shard_root = ctx.out / 'r2/staging/scope/act_1'
    if shard_root.is_dir():
        for path in sorted(shard_root.glob('*.json')):
            payload = read_json(path, {}) or {}
            if payload.get('kind') != 'script':
                continue
            for row in payload.get('rows') or []:
                reasons = row.get('scope_reasons') or []
                scene_reasons = [reason for reason in reasons
                                 if 'scene module source' in str(reason).casefold()
                                 or 'scene source' in str(reason).casefold()]
                if scene_reasons:
                    module_script_refs.append({
                        'name': row.get('name'), 'reference_id': row.get('reference_id') or row.get('id'),
                        'site': row.get('site'), 'source': (row.get('context') or {}).get('file'),
                        'scope_reasons': scene_reasons,
                    })
    act1_scene_files = set()
    act1_module_files = set()
    act1_level_script_refs = []
    for level in levels:
        if level.get('act') != 'act_1':
            continue
        if level.get('source'):
            act1_scene_files.add(level['source'])
        config_script = ((level.get('config') or {}).get('scriptFile')
                         or (level.get('config') or {}).get('script_file'))
        if config_script:
            act1_level_script_refs.append({
                'scene': level.get('name'), 'authored_path': config_script,
                'normalized_source_candidate': '.local-inputs/assets-extra/android/' +
                    config_script.replace('\\', '/').lstrip('/'),
                'status': 'NON_LUA_LEVEL_SCRIPT_SOURCE_UNAVAILABLE',
            })
        for candidate in level.get('module_spawn_candidates') or []:
            if candidate.get('module'):
                act1_module_files.add(candidate['module'])
    for ref in module_script_refs:
        if ref.get('source'):
            act1_module_files.add(ref['source'])
    # Include Lua scripts named by exact module rows when they map to the atlas
    # inventory. Non-Lua object callbacks remain visible as unresolved roots.
    by_name = defaultdict(list)
    for script in scripts_by_id.values():
        by_name[Path(script.get('name') or '').stem.casefold()].append(script['id'])
    unmatched_module_roots = []
    for ref in module_script_refs:
        name = str(ref.get('name') or '').strip()
        ids = by_name.get(Path(name.replace('\\', '/')).stem.casefold(), [])
        if ids:
            direct_script_ids.update(ids)
            ref['matched_script_ids'] = sorted(ids)
        else:
            ref['matched_script_ids'] = []
            unmatched_module_roots.append(ref)

    closure = set(direct_script_ids)
    frontier = list(sorted(closure))
    site_to_id = {row.get('site'): row['id'] for row in scripts_by_id.values()}
    while frontier:
        script_id = frontier.pop()
        script = scripts_by_id.get(script_id) or {}
        imports = script.get('imports') or {}
        for sites in imports.values():
            if isinstance(sites, str):
                sites = [sites]
            for site in sites:
                dependency = site_to_id.get(site)
                if dependency and dependency not in closure:
                    closure.add(dependency)
                    frontier.append(dependency)

    # All supported Prince player skill Lua files are in scope regardless of
    # their Act 1 class availability; their native contracts are reusable.
    player_skill_ids = {
        row['id'] for row in scripts_by_id.values()
        if Path(row.get('site') or '').as_posix().casefold().find('/scripts/skills/') >= 0
        and Path(row.get('name') or '').name.casefold().startswith('prince_')
    }
    common_ids = {site_to_id[path] for path in COMMON_SOURCES if path in site_to_id}
    source_ids = closure | player_skill_ids | common_ids
    tags = {}
    for script_id in source_ids:
        source_tags = []
        if script_id in player_skill_ids:
            source_tags.append('player_skill_lua')
        if script_id in closure:
            source_tags.append('act1_level_boss_script_closure')
        if script_id in common_ids:
            source_tags.append('autoloaded_lua_common_helpers')
        tags[script_id] = source_tags

    # Keep the broader transition closure visible as an audited boundary note;
    # it belongs to Act 2's dossier and must not be mislabeled Act 1.
    transition_note = (
        'MASTER-HANDOFF.md and Package C identify 001_swamp + 002_swamp_witchcave '
        'as the prior Act 1 chain ending at 003_darkwood; the boundary remains inference.'
    )
    return {
        'level_roots': level_roots,
        'actor_assignment_roots': actor_roots,
        'entity_root_ids': sorted(entity_ids),
        'direct_script_ids': sorted(direct_script_ids),
        'script_closure_ids': sorted(closure),
        'player_skill_ids': sorted(player_skill_ids),
        'common_script_ids': sorted(common_ids),
        'source_ids': sorted(source_ids),
        'source_tags': tags,
        'module_script_references': sorted(module_script_refs,
                                           key=lambda row: (str(row.get('site') or ''), str(row.get('name') or ''))),
        'scene_files': sorted(act1_scene_files),
        'module_files': sorted(act1_module_files),
        'level_script_references': sorted(act1_level_script_refs,
                                         key=lambda row: (str(row.get('scene') or ''),
                                                          str(row.get('authored_path') or ''))),
        'unmatched_module_script_roots': sorted(unmatched_module_roots,
                                                key=lambda row: (str(row.get('site') or ''), str(row.get('name') or ''))),
        'transition_note': transition_note,
        'act1_level_rows': [row for row in levels if row.get('act') == 'act_1'],
    }


def _helper_definitions(ctx, source_ids, scripts_by_id):
    """Trace common Lua functions to their direct calls with source lines."""
    definitions = []
    by_name = defaultdict(list)
    for site in COMMON_SOURCES:
        path = _script_path(ctx, site)
        if not path.is_file():
            continue
        lines = _read_source(path).splitlines()
        starts = []
        for line_no, line in enumerate(lines):
            match = re.match(r'^\s*(?:local\s+)?function\s+([A-Za-z_][\w.:]*)\s*\(', line)
            if match:
                starts.append((line_no, match.group(1)))
        for index, (start, name) in enumerate(starts):
            end = starts[index + 1][0] if index + 1 < len(starts) else len(lines)
            body_calls = []
            for source_line in range(start, end):
                line = lines[source_line]
                if source_line == start:
                    line = re.sub(r'^\s*(?:local\s+)?function\s+[A-Za-z_][\w.:]*\s*\([^)]*\)', '', line)
                for match in re.finditer(r'(?<![\w.])([A-Za-z_][\w]*)\s*\(', line):
                    called = match.group(1)
                    if called in {'if', 'for', 'while', 'function'}:
                        continue
                    body_calls.append({'name': called, 'line': source_line + 1,
                                       'source': site})
            record = {'name': name, 'source': site, 'line': start + 1,
                      'end_line': end, 'direct_calls': body_calls}
            definitions.append(record)
            by_name[name].append(record)

    # Only wrapper chains reached by in-scope calls are active scope evidence.
    calls = _data(ctx, 'calls', []) or []
    active = set()
    for call in calls:
        if call.get('script_id') in source_ids and call.get('name') in by_name:
            active.add(call['name'])
    changed = True
    while changed:
        changed = False
        for name in tuple(active):
            for definition in by_name.get(name, []):
                for direct in definition['direct_calls']:
                    if direct['name'] in by_name and direct['name'] not in active:
                        active.add(direct['name'])
                        changed = True
    active_defs = [definition for definition in definitions if definition['name'] in active]
    return active_defs, by_name


def _port_matches(ctx, names):
    # Identifier presence is evidence to inspect, not proof of semantic parity.
    normalized = {name: name.split(':')[-1].split('.')[-1].removesuffix('__') for name in names}
    hits = lua_scripts._port_search(ctx.root, set(normalized.values()))
    return {name: hits.get(symbol, []) for name, symbol in normalized.items()}


def enumerate_scope(ctx):
    """Create the deterministic package-A scope, call ledger, and Step-0 counts."""
    scripts = _data(ctx, 'scripts', []) or []
    calls = _data(ctx, 'calls', []) or []
    functions = _data(ctx, 'functions', []) or []
    native = _data(ctx, 'entity-native', {}) or {}
    scripts_by_id = {row['id']: row for row in scripts if row.get('id')}
    entities = native.get('entities_by_id') or {}
    funcs_by_id = {row['id']: row for row in functions if row.get('id')}
    funcs_by_name = {row.get('name'): row for row in functions if row.get('name')}
    ranked = _ranked_functions(ctx)
    act1 = _membership_scripts(ctx, scripts_by_id, entities)
    source_ids = set(act1['source_ids'])

    active_helpers, helper_by_name = _helper_definitions(ctx, source_ids, scripts_by_id)
    helper_names = set(helper_by_name)
    scripts_in_scope = [scripts_by_id[sid] for sid in source_ids if sid in scripts_by_id]
    scripts_in_scope.sort(key=lambda row: (row.get('site') or '', row.get('name') or ''))
    scoped_calls = [call for call in calls if call.get('script_id') in source_ids]
    scoped_calls.sort(key=lambda row: (row.get('site') or '', row.get('line') or 0,
                                       row.get('id') or ''))

    # Independent scan of every indexed plaintext Lua source. Scope validation
    # compares this parse to the atlas ledger, including each prince_ source.
    scan_rows = []
    calls_by_script = defaultdict(list)
    for call in calls:
        calls_by_script[call.get('script_id')].append(call)
    for script in sorted(scripts, key=lambda row: row.get('site') or ''):
        path = _script_path(ctx, script.get('site') or '')
        if not path.is_file():
            scan_rows.append({'script_id': script.get('id'), 'source': script.get('site'),
                              'status': 'SOURCE_MISSING', 'ledger_calls': len(calls_by_script[script.get('id')]),
                              'parsed_calls': 0})
            continue
        parsed, _, _, _ = lua_scripts._parse_script(_read_source(path))
        ledger = calls_by_script.get(script.get('id'), [])
        def signature(row, parsed_row=False):
            if parsed_row:
                display = _display_call(row)
                return (row.get('line'), display, re.sub(r'\s+', '', row.get('expression') or ''))
            return (row.get('line'), row.get('name'), re.sub(r'\s+', '', row.get('expression') or ''))
        parsed_keys = Counter(signature(row, True) for row in parsed)
        ledger_keys = Counter(signature(row) for row in ledger)
        missing = list((parsed_keys - ledger_keys).elements())
        extra = list((ledger_keys - parsed_keys).elements())
        scan_rows.append({'script_id': script.get('id'), 'source': script.get('site'),
                          'status': 'MATCH' if not missing and not extra else 'MISMATCH',
                          'ledger_calls': sum(ledger_keys.values()), 'parsed_calls': sum(parsed_keys.values()),
                          'missing_ledger_signatures': [list(key) for key in sorted(missing, key=str)],
                          'extra_ledger_signatures': [list(key) for key in sorted(extra, key=str)]})

    # Bindings and exact port identifier mentions are attached after collecting
    # the union. Raw source bodies are never copied into generated outputs.
    registration_map = lua_scripts._ida_registration_map(ctx.inputs)
    ranked_by_name = {row['name']: row for row in ranked}
    candidate_calls = []
    for call in scoped_calls:
        mode = call.get('call_mode')
        is_lua = (mode in HELPER_MODES or call.get('name') in helper_names)
        if call.get('engine_candidate') and not is_lua:
            candidate_calls.append(call)
    source_names = sorted({row.get('name') for row in candidate_calls if row.get('name')})
    names = list(ranked_by_name)
    names.extend(name for name in source_names if name not in ranked_by_name)
    port_hits = _port_matches(ctx, names)

    # Collect all source call sites globally for each native name. Function IDs
    # catch aliases sharing a binding; exact names retain global source coverage.
    all_calls_by_name = defaultdict(list)
    all_calls_by_function = defaultdict(list)
    for call in calls:
        all_calls_by_name[call.get('name')].append(call)
        if call.get('function_id'):
            all_calls_by_function[call['function_id']].append(call)
    scoped_by_name = defaultdict(list)
    for call in candidate_calls:
        scoped_by_name[call.get('name')].append(call)

    wrapper_edges = defaultdict(list)
    for definition in active_helpers:
        for direct in definition.get('direct_calls') or []:
            wrapper_edges[definition['name']].append(direct)

    def paths_to_native(wrapper_name, native_name, seen=()):
        if wrapper_name in seen:
            return []
        found = []
        for direct in wrapper_edges.get(wrapper_name, []):
            next_name = direct['name']
            if next_name == native_name:
                found.append(([wrapper_name, next_name], [direct]))
            elif next_name in wrapper_edges:
                for suffix, source_steps in paths_to_native(next_name, native_name,
                                                            seen + (wrapper_name,)):
                    found.append(([wrapper_name] + suffix, [direct] + source_steps))
        return found

    rows = []
    for name in names:
        row_calls = scoped_by_name.get(name, [])
        fids = {call.get('function_id') for call in row_calls if call.get('function_id')}
        function = funcs_by_name.get(name)
        if not function and len(fids) == 1:
            function = funcs_by_id.get(next(iter(fids)))
        if function:
            fids.add(function.get('id'))
        global_calls_by_id = {call.get('id'): call for fid in fids if fid
                              for call in all_calls_by_function.get(fid, [])}
        for call in all_calls_by_name.get(name, []):
            global_calls_by_id[call.get('id')] = call
        global_sites = []
        for call in sorted(global_calls_by_id.values(), key=lambda item: (
                item.get('site') or '', item.get('line') or 0, item.get('id') or '')):
            global_sites.append({
                'call_id': call.get('id'), 'source': call.get('site'),
                'script': (scripts_by_id.get(call.get('script_id')) or {}).get('name'),
                'line': call.get('line'), 'name': call.get('name'),
                'call_mode': call.get('call_mode'), 'expression': call.get('expression'),
                'arguments': call.get('arguments') or [], 'function_id': call.get('function_id'),
                'source_membership': act1['source_tags'].get(call.get('script_id'), []),
            })
        wrapper_traces = []
        wrapper_call_sites = {}
        for wrapper_name in sorted(wrapper_edges):
            for path_names, source_steps in paths_to_native(wrapper_name, name):
                wrapper_traces.append({
                    'wrapper_path': path_names,
                    'source_steps': source_steps,
                    'confidence': 'PROVEN_SOURCE_CALL_CHAIN',
                })
                for call in all_calls_by_name.get(wrapper_name, []):
                    wrapper_call_sites[call.get('id')] = {
                        'call_id': call.get('id'), 'source': call.get('site'),
                        'script': (scripts_by_id.get(call.get('script_id')) or {}).get('name'),
                        'line': call.get('line'), 'name': call.get('name'),
                        'expression': call.get('expression'), 'arguments': call.get('arguments') or [],
                        'via': path_names,
                    }
        registration_names = {name, name.split(':')[-1].split('.')[-1]}
        bindings = []
        for public_name in sorted(registration_names):
            for entry in registration_map.get(public_name, []):
                bindings.append({
                    'public_name': public_name,
                    'binding': entry.get('binding'),
                    'handler': entry.get('handler'),
                    'address': entry.get('address'),
                    'source_line': entry.get('line'),
                })
        address_candidates = [{key: address.get(key) for key in
                               ('address', 'binding', 'handler', 'source_line')}
                              for address in (function.get('native_addresses', []) if function else [])]
        port = port_hits.get(name, [])
        rank = ranked_by_name.get(name)
        rows.append({
            'name': name, 'state': 'TODO',
            'scope_sources': (['Engine functions to implement next'] if rank else []) +
                             (['player_skill_lua_or_act1_level_boss_closure'] if row_calls else []),
            'rank': rank.get('rank') if rank else None,
            'ranking': rank,
            'ledger_function_id': function.get('id') if function else None,
            'atlas_port_status_reference_only': (rank.get('atlas_port_status_reference_only')
                                                 if rank else function.get('port_status') if function else 'UNKNOWN'),
            'ida_bindings': bindings,
            'ida_binding_status': 'FOUND' if bindings else 'NOT-FOUND-NEEDS-TRACE',
            'atlas_handler_candidates': function.get('native_handlers', []) if function else [],
            'atlas_address_candidates': address_candidates,
            'current_port_identifier_matches': port,
            'current_port_semantic_status': 'REVIEW_REQUIRED' if port else 'NO_IDENTIFIER_MATCH_FOUND',
            'wrapper_traces': wrapper_traces,
            'calls_through_lua_wrappers': sorted(wrapper_call_sites.values(),
                                                 key=lambda item: (item.get('source') or '',
                                                                   item.get('line') or 0,
                                                                   item.get('call_id') or '')),
            'scope_source_sites': [{
                'call_id': call.get('id'), 'source': call.get('site'),
                'script': (scripts_by_id.get(call.get('script_id')) or {}).get('name'),
                'line': call.get('line'), 'name': call.get('name'),
                'expression': call.get('expression'), 'arguments': call.get('arguments') or [],
                'call_mode': call.get('call_mode'),
            } for call in row_calls],
            'all_original_call_sites': global_sites,
        })

    call_ledger = []
    for call in scoped_calls:
        script = scripts_by_id.get(call.get('script_id')) or {}
        mode = call.get('call_mode')
        helper = mode in HELPER_MODES or call.get('name') in helper_names
        call_ledger.append({
            'call_id': call.get('id'), 'script_id': call.get('script_id'),
            'source': call.get('site'), 'script': script.get('name'),
            'source_sets': act1['source_tags'].get(call.get('script_id'), []),
            'line': call.get('line'), 'name': call.get('name'), 'call_mode': mode,
            'classification': 'LUA_HELPER_OR_WRAPPER' if helper else
                              'ENGINE_NATIVE_CANDIDATE' if call.get('engine_candidate') else 'NON_ENGINE_CALL',
            'expression': call.get('expression'), 'arguments': call.get('arguments') or [],
            'function_id': call.get('function_id'), 'imported_from': call.get('imported_from') or [],
        })

    helper_roots = {call.get('name') for call in scoped_calls if call.get('name') in helper_by_name}
    wrapper_map = []
    for definition in active_helpers:
        wrapper_map.append({
            **definition,
            'status': 'LUA_WRAPPER_OR_HELPER',
            'used_in_scope': definition['name'] in helper_roots or any(
                direct['name'] in helper_roots for direct in definition.get('direct_calls', [])),
        })

    scripts_meta = [{
        'script_id': script['id'], 'name': script.get('name'), 'source': script.get('site'),
        'source_sets': act1['source_tags'].get(script['id'], []),
        'format': script.get('format'), 'imports': script.get('imports') or {},
    } for script in scripts_in_scope]
    counts = {
        'ranked_functions': len(ranked),
        'player_skill_lua_scripts': len(act1['player_skill_ids']),
        'act1_level_roots': len(act1['level_roots']),
        'act1_actor_assignment_rows': len(act1['actor_assignment_roots']),
        'act1_entity_roots_unique': len(act1['entity_root_ids']),
        'act1_direct_lua_script_roots': len(act1['direct_script_ids']),
        'act1_lua_import_closure': len(act1['script_closure_ids']),
        'act1_module_script_references': len(act1['module_script_references']),
        'act1_module_script_refs_unmatched_to_lua': len(act1['unmatched_module_script_roots']),
        'act1_scene_files': len(act1['scene_files']),
        'act1_module_files': len(act1['module_files']),
        'act1_level_script_references': len(act1['level_script_references']),
        'autoloaded_common_scripts': len(act1['common_script_ids']),
        'source_lua_scripts': len(scripts_meta),
        'scoped_source_call_sites': len(call_ledger),
        'scoped_engine_native_candidate_sites': sum(row['classification'] == 'ENGINE_NATIVE_CANDIDATE'
                                                    for row in call_ledger),
        'scoped_lua_helper_wrapper_sites': sum(row['classification'] == 'LUA_HELPER_OR_WRAPPER'
                                               for row in call_ledger),
        'distinct_native_rows': len(rows),
        'all_lua_scripts_independently_scanned': len(scan_rows),
        'all_original_call_sites_in_rows': sum(len(row['all_original_call_sites']) for row in rows),
    }
    payload = {
        'schema': 'atlas-r2-native-scope-v2',
        'phase': 'STEP0_SCOPE_ENUMERATION',
        'scope_definition': {
            'ranked_functions': 'Every row from the round-1 README Engine functions to implement next table, preserving rank order.',
            'player_skill_lua': 'Every plaintext Lua source under android/data/scripts/skills named prince_*, including all classes/subclasses.',
            'act1_level_boss_lua': 'Act 1 scene and actor roots plus exact scene/module script references, matched Lua scripts and imported script closure. Scene membership is inference; unmatched module script identifiers remain listed as source gaps.',
            'global_call_sites': 'Every matching call in the round-1 all-script call ledger, joined by function ID and exact Lua public name; includes source call expressions and literal argument records.',
            'port_status': 'Exact current source identifier matches are discovery evidence only. Each contract page still needs source inspection before PORTED/PARTIAL/MISSING is assigned.',
        },
        'sources': {
            'ranked_table': {'path': 'coordination/atlas/README.md', 'heading': 'Engine functions to implement next', 'rows': ranked},
            'player_skill_root': '.local-inputs/assets-extra/android/data/scripts/skills/',
            'act1_membership_source': 'coordination/atlas/data/act-membership.json',
            'act1_membership_confidence': 'INFERENCE per the atlas; MASTER-HANDOFF/Package C describe Swamp + Witch Cave ending at 003_darkwood.',
            'act1_transition_note': act1['transition_note'],
            'act1_level_roots': act1['level_roots'],
            'act1_actor_assignment_roots': act1['actor_assignment_roots'],
            'act1_module_script_references': act1['module_script_references'],
            'act1_scene_files': act1['scene_files'],
            'act1_module_files': act1['module_files'],
            'act1_level_script_references': act1['level_script_references'],
            'unmatched_module_script_roots': act1['unmatched_module_script_roots'],
            'source_lua_scripts': scripts_meta,
        },
        'lua_helper_wrapper_traces': wrapper_map,
        'native_rows': rows,
        'scoped_source_call_ledger': call_ledger,
        'independent_source_scan': scan_rows,
        'counts': counts,
        'state': 'TODO for every native row; investigation and behavior specs are not complete.',
    }
    write_json(ctx.out / SCOPE_PATH, payload)
    progress = {
        'schema': 'atlas-r2-package-progress-v1', 'package': 'A-native-contract-specs',
        'phase': 'STEP0_SCOPE_ENUMERATION', 'total': len(rows), 'processed': 0,
        'status_counts': {'TODO': len(rows), 'DONE': 0, 'UNRESOLVED': 0, 'NOT-NEEDED': 0},
        'scope_files': [SCOPE_PATH.as_posix()],
        'counts': counts,
        'expected_contract_pages': len(rows),
        'contract_status': 'Not started; awaiting root Step 0 release.',
    }
    write_json(ctx.out / PROGRESS_PATH, progress)
    return progress


def run(ctx):
    """Keep Step-0 scope sealed and regenerate source contracts alongside it."""
    if not (ctx.out / SCOPE_PATH).is_file():
        enumerate_scope(ctx)
    return r2_native_contracts.run(ctx)


def check(ctx):
    errors = []
    tests = []
    path = ctx.out / SCOPE_PATH
    payload = read_json(path, {}) or {}
    rows = payload.get('native_rows') or []
    progress = read_json(ctx.out / PROGRESS_PATH, {}) or {}
    todo = sum(row.get('state') == 'TODO' for row in rows)
    tests.append({'name': 'sealed_step0_scope_preserved', 'rows': len(rows),
                  'todo_rows': todo, 'passed': bool(rows) and todo == len(rows)})
    if not rows or todo != len(rows):
        errors.append(f'initial Step-0 scope must remain TODO-only: rows={len(rows)}, todo={todo}')
    ranked = _ranked_functions(ctx)
    ranked_names = {row.get('name') for row in rows if row.get('rank')}
    missing_ranked = [row['name'] for row in ranked if row['name'] not in ranked_names]
    tests.append({'name': 'every_ranked_engine_function_in_scope', 'expected': len(ranked),
                  'actual': len(ranked) - len(missing_ranked), 'missing': missing_ranked[:20],
                  'passed': not missing_ranked})
    if missing_ranked:
        errors.append('ranked functions missing from scope: ' + ', '.join(missing_ranked[:20]))
    scans = payload.get('independent_source_scan') or []
    bad_scans = [row for row in scans if row.get('status') != 'MATCH']
    prince_scans = [row for row in scans if Path(row.get('source') or '').name.casefold().startswith('prince_')]
    tests.append({'name': 'all_lua_source_calls_match_atlas_ledger', 'scripts': len(scans),
                  'mismatches': len(bad_scans), 'prince_scripts': len(prince_scans),
                  'passed': bool(scans) and not bad_scans})
    if bad_scans:
        errors.append(f'{len(bad_scans)} Lua scripts differ between independent parse and atlas call ledger')
    if len(prince_scans) < 1:
        errors.append('no prince_* player skill scripts were independently scanned')
    scope_calls = payload.get('scoped_source_call_ledger') or []
    all_calls = _data(ctx, 'calls', []) or []
    source_ids = {row.get('script_id') for row in payload.get('sources', {}).get('source_lua_scripts', [])}
    expected_scope_calls = sum(call.get('script_id') in source_ids for call in all_calls)
    tests.append({'name': 'scoped_call_ledger_complete', 'expected': expected_scope_calls,
                  'actual': len(scope_calls), 'passed': expected_scope_calls == len(scope_calls)})
    if expected_scope_calls != len(scope_calls):
        errors.append('scoped call ledger does not include all calls from enumerated source scripts')
    global_missing = []
    for row in rows:
        function_id = row.get('ledger_function_id')
        name = row.get('name')
        expected = {call.get('id') for call in all_calls
                    if call.get('name') == name or (function_id and call.get('function_id') == function_id)}
        actual = {call.get('call_id') for call in row.get('all_original_call_sites') or []}
        if expected - actual:
            global_missing.append({'name': name, 'missing': len(expected - actual)})
    tests.append({'name': 'global_original_callsite_collection_complete',
                  'functions_checked': len(rows), 'missing_function_sites': len(global_missing),
                  'passed': not global_missing})
    if global_missing:
        errors.append(f'global call-site collection missed calls for {len(global_missing)} native rows')
    limit = 40_000_000
    size = path.stat().st_size if path.is_file() else 0
    tests.append({'name': 'scope_output_below_40mb', 'size_bytes': size,
                  'passed': bool(size and size < limit)})
    if not size or size >= limit:
        errors.append(f'native scope output missing or exceeds 40 MB: {size}')
    refined_path = ctx.out / REFINED_SCOPE_PATH
    refined = read_json(refined_path, {}) or {}
    refined_rows = refined.get('rows') or []
    expected_names = {row.get('name') for row in rows}
    refined_names = {row.get('name') for row in refined_rows}
    refinement_ok = bool(refined_rows) and expected_names == refined_names
    tests.append({'name': 'source_refinement_covers_sealed_scope', 'expected': len(rows),
                  'actual': len(refined_rows), 'passed': refinement_ok})
    if not refinement_ok:
        errors.append('source refinement must cover every sealed Step-0 row by name')
    page_root = ctx.out / CONTRACT_ROOT
    refined_names = [row.get('name') for row in refined_rows]
    expected_pages = {re.sub(r'[^A-Za-z0-9_.-]+', '_', name or 'unknown') + '.md'
                      for name in refined_names}
    pages = [page_root / name for name in sorted(expected_pages) if (page_root / name).is_file()]
    required = ('Binding and handler', 'Lua argument mapping and units',
                'Recovered behavior, gates, state, timers, events and return',
                'Lua wrapper/helper traces', 'Current frozen port source', 'Known answers',
                'Source-backed behavior cases',
                'Original Lua call sites', 'Related natives', 'Unresolved reasons', 'Evidence boundary')
    bad_pages = []
    for page in pages:
        content = page.read_text(encoding='utf-8', errors='replace')
        missing = [section for section in required if f'## {section}' not in content]
        if missing:
            bad_pages.append({'page': page.name, 'missing_sections': missing})
        if '**PROVEN:**' in content and 'Evidence:' not in content:
            bad_pages.append({'page': page.name, 'error': 'proven claim has no evidence marker'})
    page_ok = len(pages) == len(expected_pages) == len(rows) and not bad_pages and (ctx.out / INDEX_PATH).is_file()
    tests.append({'name': 'all_contract_pages_have_required_sections',
                  'expected': len(rows), 'actual': len(pages), 'bad_pages': bad_pages[:20],
                  'passed': page_ok})
    if not page_ok:
        errors.append(f'contract page set incomplete or invalid: pages={len(pages)}, scope={len(rows)}')
    status_counts = progress.get('status_counts') or {}
    progress_ok = (progress.get('total') == len(rows)
                   and progress.get('processed') == status_counts.get('DONE', 0) + status_counts.get('UNRESOLVED', 0) + status_counts.get('NOT-NEEDED', 0)
                   and status_counts.get('DONE', 0) + status_counts.get('UNRESOLVED', 0) + status_counts.get('NOT-NEEDED', 0) + status_counts.get('TODO', 0) == len(rows))
    tests.append({'name': 'package_progress_rollup_complete', 'passed': progress_ok,
                  'total': progress.get('total'), 'processed': progress.get('processed'),
                  'status_counts': status_counts})
    if not progress_ok:
        errors.append('package-A progress counts must reconcile all rows; remaining TODO indicates unaudited linked handlers')
    tick = chr(96)
    unreferenced = []
    for page in pages:
        content = page.read_text(encoding='utf-8', errors='replace')
        for line in content.splitlines():
            if '**PROVEN:**' in line and ('Evidence:' not in line or not re.search(tick + r'[^' + tick + r']+' + tick, line)):
                unreferenced.append(page.name)
                break
    tests.append({'name': 'proven_answers_have_source_references',
                  'bad_pages': unreferenced[:20], 'passed': not unreferenced})
    if unreferenced:
        errors.append('PROVEN claims need a concrete source reference in every page')
    return {'passed': not errors, 'tests': tests, 'errors': errors}
