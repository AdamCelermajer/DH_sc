"""Build a compact, indexed entity -> Lua -> native evidence map.

This is a static association atlas. Script membership and call sites are kept
separate from dynamic execution, and native addresses are kept separate from
port readiness. It consumes the canonical atlas ledgers and writes two indexes.
"""
from collections import Counter, defaultdict
from pathlib import Path
import re

from common import read_json, write_json, stable_id, physical_path

IDA_ROOT = '.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so'
RESOURCE_KINDS = {
    'character_animation': 'animation_table',
    'animation_sequence': 'animation_sequence',
    'animation_clip': 'animation_clip',
    'effect_set': 'effect_table',
    'animated_effect': 'effect_file_entry',
    'sound': 'sound_table',
    'projectile': 'projectile_table',
    'projectile_model': 'projectile_model',
}
MEDIA_EXTENSIONS = {
    '.bdae': 'visual_resource_file', '.dae': 'visual_resource_file',
    '.wav': 'sound_file', '.vxn': 'sound_file', '.ogg': 'sound_file',
    '.mp3': 'sound_file', '.mp4': 'video_resource_file',
}
KNOWN_PLATFORM_SOURCES = {
    'android', 'android-extra', 'original-cache', 'port-runtime-original-cache', 'audio-cache',
    'ios', 'ios-packed', 'ios-glive', 'ios-app-resources',
}


def _rows(ctx, name, default=()):
    value = read_json(ctx.out / 'data' / (name + '.json'), list(default))
    if isinstance(value, dict):
        for key in ('rows', name, 'references', 'assets'):
            if isinstance(value.get(key), list):
                return value[key]
        return []
    return value or []


def _script_key(value):
    return str(value or '').replace('\\', '/').casefold().lstrip('./')


def _script_index(scripts):
    by_id = {r['id']: r for r in scripts}
    by_name = defaultdict(list)
    by_site = defaultdict(list)
    for row in scripts:
        by_name[Path(row.get('name', '')).stem.casefold()].append(row)
        site = _script_key(row.get('site'))
        by_site[site].append(row)
        # Configuration files often use a data-relative path while the source
        # catalog uses a corpus-relative path. Keep basename matches explicit.
        by_site[Path(site).name].append(row)
    return by_id, by_name, by_site


def _profile_scripts(profile, by_id, by_name, by_site):
    links = []
    if profile.get('kind') == 'script':
        script_id = profile.get('source', {}).get('script_id')
        script = by_id.get(script_id)
        links.append({'script_id': script_id, 'association': 'SCRIPT_PROFILE_SELF',
                      'status': 'RESOLVED' if script else 'DANGLING_SCRIPT_ID',
                      'site': script.get('site') if script else None,
                      'name': script.get('name') if script else None})
    for script_id in profile.get('scripts', {}).get('script_ids', []):
        script = by_id.get(script_id)
        links.append({'script_id': script_id, 'association': 'PROFILE_SCRIPT_CLOSURE',
                      'status': 'RESOLVED' if script else 'DANGLING_SCRIPT_ID',
                      'site': script.get('site') if script else None,
                      'name': script.get('name') if script else None})
    # SkillTable.Script is an exact authored field, even if a script closure
    # was not built for the profile (for example, ambiguous names).
    table = profile.get('source', {}).get('table')
    script_value = profile.get('root_fields', {}).get('Script')
    if table == 'skills#group1' and isinstance(script_value, str) and script_value:
        candidates = by_name.get(Path(script_value).stem.casefold(), [])
        for script in candidates:
            links.append({'script_id': script['id'], 'association': 'SKILL_TABLE_SCRIPT_FIELD',
                          'status': 'RESOLVED' if len(candidates) == 1 else 'AMBIGUOUS_SCRIPT_STEM',
                          'match_rule': 'UNIQUE_SCRIPT_STEM' if len(candidates) == 1 else 'SCRIPT_STEM_CANDIDATES',
                          'site': script.get('site'), 'name': script.get('name'),
                          'authored_value': script_value})
        if not candidates:
            links.append({'script_id': None, 'association': 'SKILL_TABLE_SCRIPT_FIELD',
                          'status': 'SCRIPT_NOT_FOUND', 'authored_value': script_value})
    return _dedupe_links(links)


def _dedupe_links(links):
    out, seen = [], set()
    for link in links:
        key = (link.get('script_id'), link.get('association'), link.get('status'), link.get('authored_value'))
        if key not in seen:
            seen.add(key)
            out.append(link)
    return out


def _script_record(script, calls, functions, ctx):
    script_path = physical_path(ctx, script.get('site', ''))
    try:
        source_lines = script_path.read_text(encoding='utf-8-sig', errors='replace').splitlines()
    except (OSError, ValueError):
        source_lines = []
    call_rows = []
    function_ids = set()
    for call in calls:
        if call.get('script_id') != script['id']:
            continue
        function_id = call.get('function_id')
        if function_id:
            function_ids.add(function_id)
        line_no = call.get('line')
        lua_excerpt = (source_lines[line_no - 1].strip()[:500]
                       if isinstance(line_no, int) and 1 <= line_no <= len(source_lines)
                       else None)
        call_rows.append({
            'call_id': call.get('id'), 'function_id': function_id,
            'line': line_no, 'lua_excerpt': lua_excerpt, 'name': call.get('name'),
            'expression': call.get('expression'), 'call_mode': call.get('call_mode'),
            'arguments': call.get('arguments', []),
            'engine_candidate': bool(call.get('engine_candidate')),
            'status': call.get('status'), 'port_status': call.get('port_status'),
        })
    return {
        'id': script['id'], 'name': script.get('name'), 'site': script.get('site'),
        'platform': script.get('platform'), 'format': script.get('format'),
        'status': script.get('status'), 'readiness': script.get('readiness'),
        'call_ids': [r['call_id'] for r in call_rows if r.get('call_id')],
        'function_ids': sorted(function_ids),
        'calls': sorted(call_rows, key=lambda r: (r.get('line') or 0, r.get('call_id') or '')),
        'association_meaning': 'A listed call is authored in this Lua source. Its execution is conditional on runtime path, caller, state, and gates.',
    }


def _ida_address(ctx, address):
    compact = str(address or '').lower().replace('0x', '').zfill(8)
    source = f'{IDA_ROOT}/pseudocode/{compact[:4]}/{compact}.c'
    p = physical_path(ctx, source)
    if p.is_file():
        lines = p.read_text(encoding='utf-8', errors='replace').splitlines()
        return {'address': address, 'pseudocode_path': source,
                'source_line': 1, 'snippet': '\n'.join(lines[:14])[:1800],
                'snippet_kind': 'function_file'}
    all_source = f'{IDA_ROOT}/pseudocode-all.c'
    all_path = physical_path(ctx, all_source)
    if all_path.is_file():
        lines = all_path.read_text(encoding='utf-8', errors='replace').splitlines()
        # The IDA ledger line is stored on each address record and is attached
        # later by _function_record; this fallback only supplies path/data.
        return {'address': address, 'pseudocode_path': all_source,
                'lines': lines, 'snippet_kind': 'aggregate_fallback'}
    return {'address': address, 'pseudocode_path': source,
            'source_line': None, 'snippet': None, 'snippet_kind': 'MISSING_DECOMPILATION'}


def _function_record(ctx, row):
    addresses = []
    for native in row.get('native_addresses', []):
        evidence = _ida_address(ctx, native.get('address'))
        line_no = native.get('source_line')
        if evidence.get('snippet_kind') == 'aggregate_fallback':
            lines = evidence.pop('lines')
            if isinstance(line_no, int) and 1 <= line_no <= len(lines):
                lo = max(0, line_no - 1)
                # Keep snippets bounded; IDA can emit very large bodies.
                excerpt = lines[lo:min(len(lines), lo + 14)]
                evidence.update(source_line=line_no, snippet='\n'.join(excerpt)[:1800])
            else:
                evidence.update(source_line=line_no, snippet=None,
                                snippet_kind='AGGREGATE_LINE_UNAVAILABLE')
            evidence['aggregate_source_line'] = line_no
        else:
            evidence['aggregate_source_line'] = line_no
        for key in ('binding', 'handler', 'signature', 'body_excerpt'):
            if native.get(key) is not None:
                evidence[key] = native[key]
        addresses.append(evidence)
    return {
        'id': row['id'], 'name': row.get('name'), 'mode': row.get('mode'),
        'meaning': row.get('meaning'), 'status': row.get('status'),
        'reason': row.get('reason'), 'candidate_note': row.get('candidate_note'),
        'native_addresses': addresses,
        'native_contract_status': ('CONTRACT_REVIEWED' if addresses and row.get('status') == 'DONE'
                                   else 'BODY_CAPTURED_CONTRACT_INCOMPLETE' if addresses
                                   else 'NO_EXACT_NATIVE_ADDRESS'),
        # The native evidence's presence is independent from port implementation.
        'port_status': row.get('port_status'), 'port_reason': row.get('port_reason'),
        'implementation': row.get('implementation', []),
        'call_count': row.get('call_count', 0),
        'ida_ledger_path': row.get('ida_evidence'),
    }


def _entity_record(source_id, kind, source, script_links, scripts_by_id,
                   call_by_id, function_by_id, notes=None):
    script_links = _dedupe_links(script_links)
    script_ids = sorted({l['script_id'] for l in script_links if l.get('script_id') in scripts_by_id})
    call_ids, function_ids = set(), set()
    for script_id in script_ids:
        sr = scripts_by_id[script_id]
        call_ids.update(sr['call_ids'])
        function_ids.update(sr['function_ids'])
    unresolved = [l for l in script_links if l.get('status') not in ('RESOLVED',)]
    if script_ids:
        association_status = 'SOURCE_SCRIPT_ASSOCIATION_FOUND'
        no_script_reason = None
    elif unresolved:
        association_status = 'SCRIPT_LINK_UNRESOLVED'
        no_script_reason = 'A source association was attempted but did not resolve uniquely in the available script catalog.'
    else:
        association_status = 'NO_SCRIPT_ASSOCIATION_IN_AVAILABLE_SOURCE_JOIN'
        no_script_reason = ('No direct Lua script link was found in this entity/profile join; native engine behavior may be separate.'
                            if kind in ('actor', 'item', 'skill', 'level') else
                            'No direct Lua script association is represented by this profile row.')
    return {
        'id': source_id, 'kind': kind, 'source': source,
        'script_links': script_links, 'script_ids': script_ids,
        'call_ids': sorted(call_ids), 'native_function_ids': sorted(function_ids),
        'script_association_status': association_status,
        'no_script_association_reason': no_script_reason,
        'association_note': 'Function IDs are reachable through listed script associations; this does not prove the script executes for this entity.',
        'unresolved_script_links': unresolved,
        'notes': notes or [],
    }


def _target_ids(edge):
    for target in edge.get('targets', []):
        target_id = target.get('id')
        if target_id:
            yield target_id


def _edge_quality(status, confidence, target_status=None):
    """Never promote an unresolved or weak reference into an owner claim."""
    statuses = (status, target_status)
    if any(s in ('UNRESOLVED', 'TODO', 'NOT-NEEDED') for s in statuses if s):
        return 'CANDIDATE_UNRESOLVED'
    if any(s == 'AMBIGUOUS' for s in statuses if s):
        return 'CANDIDATE_AMBIGUOUS'
    if status == 'RESOLVED' and (target_status in (None, 'RESOLVED')):
        if confidence == 'PROVEN':
            return 'DECLARED_PROVEN'
        if confidence == 'LIKELY':
            return 'CANDIDATE_LIKELY'
        return 'CANDIDATE_CONFIDENCE_UNKNOWN'
    return 'CANDIDATE_UNPROVEN'


def _logical_media_paths(asset):
    paths = []
    for logical in asset.get('logical_paths', []):
        ext = Path(str(logical)).suffix.casefold()
        if ext in MEDIA_EXTENSIONS:
            paths.append({'path': logical, 'extension': ext,
                          'resource_type': MEDIA_EXTENSIONS[ext],
                          'confidence': asset.get('mapping_confidence'),
                          'provenance': asset.get('metadata_aliases', [])})
    return paths


def build_resource_attributions(ctx):
    profiles = _rows(ctx, 'entity-profiles')
    assets = _rows(ctx, 'assets')
    tables = _rows(ctx, 'tables')
    references = _rows(ctx, 'references')
    reference_by_id = {r.get('id'): r for r in references}
    profile_by_id = {p['id']: p for p in profiles}
    profile_by_row = {p.get('source', {}).get('row_id'): p for p in profiles
                      if p.get('source', {}).get('row_id')}
    table_by_id = {r.get('id'): r for r in tables}
    asset_by_path = defaultdict(list)
    asset_by_logical_path = defaultdict(list)
    for asset in assets:
        asset_by_path[asset.get('path', '').casefold()].append(asset)
        asset_by_path[asset.get('relative', '').casefold()].append(asset)
        for logical in asset.get('logical_paths', []):
            asset_by_logical_path[str(logical).casefold()].append(asset)
    profiles_by_table_id = defaultdict(list)
    profiles_by_asset_id = defaultdict(list)
    profiles_by_authored_asset_path = defaultdict(list)
    profiles_by_candidate_asset_path = defaultdict(list)
    profiles_by_authored_logical_path = defaultdict(list)
    profiles_by_candidate_logical_path = defaultdict(list)
    for profile in profiles:
        for table_id in profile.get('joined', {}).get('table_ids', []):
            profiles_by_table_id[table_id].append(profile['id'])
        for asset_id in profile.get('joined', {}).get('asset_ids', []):
            profiles_by_asset_id[asset_id].append(profile['id'])
        for edge in profile.get('field_edges', []):
            reference = reference_by_id.get(edge.get('id'), {})
            edge_status = edge.get('status') or reference.get('status')
            edge_confidence = reference.get('confidence') or edge.get('confidence')
            for target in edge.get('targets', []):
                quality = _edge_quality(edge_status, edge_confidence, target.get('status'))
                path = target.get('path')
                if path:
                    if quality == 'DECLARED_PROVEN':
                        profiles_by_authored_logical_path[str(path).casefold()].append(profile['id'])
                    else:
                        profiles_by_candidate_logical_path[str(path).casefold()].append(profile['id'])
                for asset_path in target.get('asset_files') or []:
                    exact_assets = asset_by_path.get(asset_path.casefold(), [])
                    packed_likely = any(a.get('source') == 'ios-packed' and
                                        a.get('mapping_confidence') == 'LIKELY'
                                        for a in exact_assets)
                    if quality == 'DECLARED_PROVEN' and not packed_likely:
                        profiles_by_authored_asset_path[asset_path.casefold()].append(profile['id'])
                    else:
                        profiles_by_candidate_asset_path[asset_path.casefold()].append(profile['id'])
    incoming = defaultdict(list)
    for owner in profiles:
        for edge in owner.get('field_edges', []):
            for target_id in _target_ids(edge):
                if target_id in profile_by_id:
                    reference = reference_by_id.get(edge.get('id'), {})
                    status = edge.get('status') or reference.get('status')
                    confidence = reference.get('confidence') or edge.get('confidence')
                    incoming[target_id].append({
                        'source_id': owner['id'], 'source_kind': owner.get('kind'),
                        'source_name': owner.get('source', {}).get('name'),
                        'field': edge.get('field'), 'site': edge.get('site'),
                        'edge_id': edge.get('id'), 'association': 'PROFILE_FIELD_TARGET',
                        'status': status, 'confidence': confidence,
                        'reason': reference.get('reason') or edge.get('reason'),
                        'reason_code': reference.get('reason_code') or edge.get('reason_code'),
                        'candidate_note': (reference.get('candidate_note') or edge.get('candidate_note')
                                           or edge.get('reason')),
                        'target_status': next((t.get('status') for t in edge.get('targets', [])
                                               if t.get('id') == target_id), None),
                        'classification': _edge_quality(status, confidence,
                            next((t.get('status') for t in edge.get('targets', [])
                                  if t.get('id') == target_id), None)),
                    })
    # CharacterProperties references are a separate exact table-field layer.
    actors = _rows(ctx, 'actors')
    for actor in actors:
        for field, link in actor.get('field_links', {}).items():
            target = profile_by_row.get(link.get('target_id'))
            if target:
                reference = reference_by_id.get(link.get('reference_id'), {})
                status = link.get('status') or reference.get('status')
                confidence = link.get('confidence') or reference.get('confidence')
                incoming[target['id']].append({
                    'source_id': actor['id'], 'source_kind': 'actor',
                    'source_name': actor.get('name'), 'field': field,
                    'site': f"character_properties#group0[{actor.get('index')}].{field}",
                    'association': 'ACTOR_AUTHORED_FIELD_TARGET',
                    'status': status, 'confidence': confidence,
                    'reason': reference.get('reason') or link.get('reason'),
                    'reason_code': reference.get('reason_code') or link.get('reason_code'),
                    'candidate_note': (reference.get('candidate_note') or link.get('candidate_note')
                                       or link.get('reason')),
                    'classification': _edge_quality(status, confidence, link.get('target_status')),
                })

    asset_by_id = {a['id']: a for a in assets}

    resource_rows = []
    for profile in profiles:
        kind = profile.get('kind')
        if kind not in RESOURCE_KINDS:
            continue
        direct_paths = []
        candidate_paths = []
        missing_paths = []
        candidate_path_assets = set()
        asset_ids = set(profile.get('joined', {}).get('asset_ids', []))
        for edge in profile.get('field_edges', []):
            for target in edge.get('targets', []):
                path = target.get('path')
                if not path:
                    continue
                reference = reference_by_id.get(edge.get('id'), {})
                status = edge.get('status') or reference.get('status')
                confidence = reference.get('confidence') or edge.get('confidence')
                quality = _edge_quality(status, confidence, target.get('status'))
                matched = target.get('asset_files') or []
                reference_reason = reference.get('reason') or edge.get('reason')
                reason_code = reference.get('reason_code') or edge.get('reason_code')
                candidate_note = (reference.get('candidate_note') or edge.get('candidate_note')
                                  or edge.get('reason'))
                path_info = {'field': edge.get('field'), 'site': edge.get('site'),
                             'path': path, 'edge_id': edge.get('id'),
                             'status': status, 'confidence': confidence,
                             'target_status': target.get('status'),
                             'classification': quality, 'asset_files': matched,
                             'reference_reason': reference_reason,
                             'reason_code': reason_code,
                             'candidate_note': candidate_note}
                resolved_files, candidate_files = [], []
                if quality == 'DECLARED_PROVEN':
                    for asset_path in matched:
                        for asset in asset_by_path.get(asset_path.casefold(), []):
                            if asset.get('source') == 'ios-packed' and asset.get('mapping_confidence') == 'LIKELY':
                                candidate_files.append(asset)
                            else:
                                resolved_files.append(asset)
                    # A target logical URI can resolve to packed iOS bytes. The
                    # logical name is exact; its package alias keeps that
                    # physical-file association at LIKELY confidence.
                    if not resolved_files and not candidate_files:
                        candidate_files.extend(asset_by_logical_path.get(str(path).casefold(), []))
                else:
                    for asset_path in matched:
                        candidate_files.extend(asset_by_path.get(asset_path.casefold(), []))
                    if not candidate_files:
                        candidate_files.extend(asset_by_logical_path.get(str(path).casefold(), []))
                path_info['resolved_asset_ids'] = sorted({a['id'] for a in resolved_files})
                path_info['candidate_asset_ids'] = sorted({a['id'] for a in candidate_files})
                # Missing means the source reference explicitly records
                # FILE_MISSING and the exact authored path has no matching
                # physical or logical asset anywhere in this indexed corpus.
                # It remains a candidate relationship; missing bytes do not
                # establish an owner or runtime use.
                # Use this exact authored target and its explicit asset_files
                # list only. Other resolves_to paths can be sibling candidates
                # (for example a similarly named sound) and must not make a
                # missing requested filename look present.
                source_paths = [path, *matched]
                source_matches = {a['id'] for source_path in source_paths
                                  for a in (asset_by_path.get(str(source_path).casefold(), [])
                                            + asset_by_logical_path.get(str(source_path).casefold(), []))}
                source_missing_confirmed = bool(
                    reason_code == 'FILE_MISSING'
                    and status == 'UNRESOLVED'
                    and not source_matches
                    and not path_info['resolved_asset_ids']
                    and not path_info['candidate_asset_ids'])
                path_info['source_missing_confirmed'] = source_missing_confirmed
                if source_missing_confirmed:
                    path_info['source_path_status'] = 'SOURCE_FILE_MISSING'
                    path_info['source_gap_note'] = (
                        'Reference ledger marks FILE_MISSING, and no exact physical or logical path match '
                        'exists in the indexed source/cache asset inventory. This does not establish ownership.')
                elif source_matches or path_info['resolved_asset_ids'] or path_info['candidate_asset_ids']:
                    path_info['source_path_status'] = 'SOURCE_PATH_MATCH_FOUND'
                    path_info['source_gap_note'] = None
                else:
                    path_info['source_path_status'] = 'NO_INDEXED_SOURCE_MATCH_UNPROVEN'
                    path_info['source_gap_note'] = (
                        candidate_note or
                        f'No matching physical or logical asset path was found in indexed source/cache assets; '
                        f'reference status/reason does not establish a confirmed FILE_MISSING result '
                        f'({reason_code or status or "unknown"}).')
                if quality == 'DECLARED_PROVEN':
                    if resolved_files:
                        path_info['asset_file_association_status'] = 'EXACT_SOURCE_FILE_PATH'
                    elif candidate_files:
                        path_info['asset_file_association_status'] = 'PACKED_LOGICAL_PATH_CANDIDATE'
                    else:
                        path_info['asset_file_association_status'] = 'NO_AVAILABLE_ASSET_BYTES'
                    direct_paths.append(path_info)
                    for asset in resolved_files:
                        asset_ids.add(asset['id'])
                if quality != 'DECLARED_PROVEN':
                    candidate_paths.append(path_info)
                if source_missing_confirmed:
                    missing_paths.append({**path_info, 'missing_reason': 'FILE_MISSING'})
                if candidate_files:
                    for asset in candidate_files:
                        # This is a likely package/logical-URI mapping only.
                        # Keep the bytes visible as candidates, not as exact
                        # authored file ownership.
                        candidate_path_assets.add(asset['id'])
        owner_edges = sorted(incoming.get(profile['id'], []), key=lambda x: (x['source_kind'] or '', x['source_name'] or '', x['field'] or ''))
        owners = [e for e in owner_edges if e.get('classification') == 'DECLARED_PROVEN']
        owner_candidates = [e for e in owner_edges if e.get('classification') != 'DECLARED_PROVEN']
        table_ids = profile.get('joined', {}).get('table_ids', [])
        table_rows = [table_by_id[t] for t in table_ids if t in table_by_id]
        shared_table_ids = [t for t in table_ids if len(profiles_by_table_id.get(t, [])) > 1]
        shared = bool(shared_table_ids)
        if owners:
            association_status = 'DECLARED_OWNER_ASSOCIATION'
        elif owner_candidates:
            association_status = 'OWNER_CANDIDATE_ASSOCIATION'
        elif table_ids:
            association_status = 'SHARED_TABLE_ASSOCIATION_ONLY' if shared else 'SOURCE_TABLE_ASSOCIATION_ONLY'
        else:
            association_status = 'NO_OWNER_OR_TABLE_ASSOCIATION'
        present_assets = [asset_by_id[a] for a in sorted(asset_ids) if a in asset_by_id]
        platforms = sorted({a.get('source') for a in present_assets if a.get('source')})
        association_note = (
            'At least one incoming field edge is RESOLVED with PROVEN confidence; candidate edges are preserved separately.'
            if owners else
            'Incoming field edges exist, but none meets the RESOLVED plus PROVEN threshold; inspect owner_candidates.'
            if owner_candidates else
            'No direct incoming declared owner edge is present. The table association records decoder/source membership only.'
            if table_ids else
            'No incoming declared owner or source-table association was found in the available profile joins.'
        )
        resource_rows.append({
            'id': profile['id'], 'kind': kind, 'resource_type': RESOURCE_KINDS[kind],
            'source': profile.get('source'), 'association_status': association_status,
            'association_note': association_note,
            'owners': owners, 'owner_candidates': owner_candidates,
            'source_table_ids': table_ids,
            'shared_table_profile_ids': sorted({owner_id for tid in shared_table_ids
                                                for owner_id in profiles_by_table_id.get(tid, [])
                                                if owner_id != profile['id']}),
            'source_table_names': [{'id': t.get('id'), 'table': t.get('table'),
                                    'index': t.get('index'), 'name': t.get('name')}
                                   for t in table_rows],
            'declared_resource_paths': direct_paths,
            'candidate_resource_paths': candidate_paths,
            'missing_resource_paths': missing_paths,
            'asset_ids': sorted(asset_ids),
            'candidate_asset_ids': sorted(candidate_path_assets),
            'available_platform_sources': platforms,
            'platform_status': ('PLATFORM_UNKNOWN' if not platforms else
                                'RECORDED_PLATFORM_SOURCE_AVAILABLE' if set(platforms) <= KNOWN_PLATFORM_SOURCES
                                else 'SOURCE_PLATFORM_PARTIAL_OR_UNKNOWN'),
            'runtime_status': 'UNPROVEN_RUNTIME_USE',
            'runtime_note': 'Static authored association or file presence does not establish runtime selection, admission, rendering, or playback.',
        })

    file_rows = []
    for asset in assets:
        transport_path = asset.get('path', '')
        physical_ext = Path(transport_path).suffix.casefold()
        logical_media = _logical_media_paths(asset)
        if physical_ext not in MEDIA_EXTENSIONS and not logical_media:
            continue
        logical_extensions = sorted({p['extension'] for p in logical_media})
        ext = physical_ext if physical_ext in MEDIA_EXTENSIONS else (logical_extensions[0] if logical_extensions else physical_ext)
        resource_types = sorted({MEDIA_EXTENSIONS[e] for e in ([physical_ext] if physical_ext in MEDIA_EXTENSIONS else []) + logical_extensions})
        packed_likely = asset.get('source') == 'ios-packed' and asset.get('mapping_confidence') == 'LIKELY' and bool(logical_media)
        # Assets are decoder-only when their only incoming users are inventory
        # row source markers. Explicit source/table paths are kept separately.
        refs = asset.get('referrers', [])
        decoder_refs = sorted(r for r in refs if str(r).startswith('source:'))
        owner_profiles = profiles_by_asset_id.get(asset['id'], [])
        authored_path_profiles = (profiles_by_authored_asset_path.get(transport_path.casefold(), [])
                                  if not packed_likely else [])
        logical_profiles = {pid for logical in logical_media
                            for pid in profiles_by_authored_logical_path.get(logical['path'].casefold(), [])}
        candidate_path_profiles = set(profiles_by_candidate_asset_path.get(transport_path.casefold(), []))
        candidate_path_profiles.update(pid for logical in logical_media
                                       for pid in profiles_by_candidate_logical_path.get(logical['path'].casefold(), []))
        if packed_likely:
            candidate_path_profiles.update(logical_profiles)
        else:
            authored_path_profiles = sorted(set(authored_path_profiles) | logical_profiles)
        if authored_path_profiles:
            relation = 'AUTHORED_RESOURCE_PATH_ASSOCIATION'
            association_note = 'An exact resolved/proven authored path maps to these source bytes; runtime use remains unproven.'
        elif packed_likely:
            relation = 'PACKED_LOGICAL_PATH_CANDIDATE'
            association_note = 'The logical media URI maps to this packed transport file with LIKELY package mapping confidence; this is not a proven physical owner or runtime admission.'
        elif candidate_path_profiles:
            relation = 'UNRESOLVED_RESOURCE_PATH_CANDIDATE'
            association_note = 'A source resource path points toward this file, but the edge is unresolved, ambiguous, or below PROVEN confidence.'
        elif owner_profiles:
            relation = 'DECODER_JOIN_ASSOCIATION_ONLY'
            association_note = 'The file appears through a source decoder/profile join; no exact resolved/proven authored resource path is asserted.'
        elif decoder_refs:
            relation = 'SOURCE_DECODER_CONSUMER_ONLY'
            association_note = 'Only source inventory/decoder referrer IDs were found; no resource owner is established.'
        else:
            relation = 'ORPHAN_IN_EXTRACTED_CORPUS'
            association_note = 'No source referrer, profile decoder join, or resolved/proven authored resource path was found in the extracted corpus.'
        present = bool(asset.get('size', 0))
        file_rows.append({
            'id': asset['id'], 'path': transport_path, 'transport_path': transport_path,
            'physical_extension': physical_ext, 'logical_media_paths': logical_media,
            'extension': ext, 'resource_types': resource_types,
            'resource_type': resource_types[0] if len(resource_types) == 1 else 'MULTIPLE_MEDIA_TYPES',
            'mapping_confidence': asset.get('mapping_confidence'),
            'decode_confidence': asset.get('decode_confidence'),
            'package_relative': asset.get('package_relative'),
            'package_version': asset.get('package_version'),
            'source': asset.get('source'),
            'size': asset.get('size'), 'sha256': asset.get('sha256'),
            'byte_status': 'BYTES_PRESENT' if present else 'MISSING_OR_EMPTY_BYTES',
            'association_status': relation,
            'association_note': association_note,
            'resource_profile_ids': sorted(set(owner_profiles)),
            'authored_path_profile_ids': sorted(set(authored_path_profiles)),
            'candidate_path_profile_ids': sorted(candidate_path_profiles),
            'decoder_referrers': decoder_refs,
            'orphan_reason': (asset.get('reason') or association_note)
                             if relation == 'ORPHAN_IN_EXTRACTED_CORPUS' else None,
            'platform_status': ('RECORDED_PLATFORM_SOURCE_AVAILABLE'
                                if asset.get('source') in KNOWN_PLATFORM_SOURCES
                                else 'PLATFORM_UNKNOWN'),
            'runtime_status': 'UNPROVEN_RUNTIME_USE',
        })
    return {
        'schema_version': 1,
        'meaning': 'Static source/resource association only; runtime use remains unproven.',
        'resource_profiles_by_id': {r['id']: r for r in resource_rows},
        'resource_files_by_id': {r['id']: r for r in file_rows},
        'counts': {
            'resource_profiles': len(resource_rows),
            'resource_profile_kinds': dict(Counter(r['kind'] for r in resource_rows)),
            'profile_associations': dict(Counter(r['association_status'] for r in resource_rows)),
            'proven_owner_profiles': sum(bool(r['owners']) for r in resource_rows),
            'profiles_with_owner_candidates': sum(bool(r['owner_candidates']) for r in resource_rows),
            'owner_candidate_edges': sum(len(r['owner_candidates']) for r in resource_rows),
            'resource_files': len(file_rows),
            'file_types': dict(Counter(t for r in file_rows for t in r['resource_types'])),
            'file_associations': dict(Counter(r['association_status'] for r in file_rows)),
            'byte_status': dict(Counter(r['byte_status'] for r in file_rows)),
            'platform_unknown_profiles': sum(r['platform_status'] == 'PLATFORM_UNKNOWN' for r in resource_rows),
            'platform_unknown_files': sum(r['platform_status'] == 'PLATFORM_UNKNOWN' for r in file_rows),
            'missing_resource_path_edges': sum(len(r['missing_resource_paths']) for r in resource_rows),
            'confirmed_file_missing_resource_paths': sum(
                p.get('source_missing_confirmed') is True
                for r in resource_rows for p in r['missing_resource_paths']),
            'candidate_resource_path_edges': sum(len(r['candidate_resource_paths']) for r in resource_rows),
            'candidate_paths_without_source_match': sum(
                p.get('source_path_status') == 'NO_INDEXED_SOURCE_MATCH_UNPROVEN'
                for r in resource_rows for p in r['candidate_resource_paths']),
            'candidate_paths_with_source_gap_notes': sum(
                bool(p.get('source_gap_note'))
                for r in resource_rows for p in r['candidate_resource_paths']),
            'candidate_resource_path_evidence': dict(Counter(
                f"{p.get('status') or 'STATUS_UNKNOWN'}|{p.get('confidence') or 'CONFIDENCE_UNKNOWN'}|{p.get('target_status') or 'TARGET_STATUS_UNKNOWN'}"
                for r in resource_rows for p in r['candidate_resource_paths'])),
            'packed_logical_media_files': sum(bool(r['logical_media_paths']) for r in file_rows),
            'packed_logical_media_types': dict(Counter(p['resource_type'] for r in file_rows for p in r['logical_media_paths'])),
        },
    }


def run(ctx):
    scripts = _rows(ctx, 'scripts')
    calls = _rows(ctx, 'calls')
    functions = _rows(ctx, 'functions')
    profiles = _rows(ctx, 'entity-profiles')
    actors = _rows(ctx, 'actors')
    levels = _rows(ctx, 'levels')
    mob_profiles = _rows(ctx, 'mob-profiles')
    scripts_by_id, by_name, by_site = _script_index(scripts)
    script_records = {r['id']: _script_record(r, calls, functions, ctx) for r in scripts}
    function_records = {r['id']: _function_record(ctx, r) for r in functions}
    call_by_id = {c['id']: c for c in calls}

    entities = []
    mob_by_id = {r.get('id'): r for r in mob_profiles}
    for actor in actors:
        links = []
        mob = mob_by_id.get(actor['id'], {})
        for script_id in mob.get('scripts', {}).get('script_ids', []):
            script = scripts_by_id.get(script_id)
            links.append({'script_id': script_id, 'association': 'ACTOR_AI_SCRIPT_CLOSURE',
                          'status': 'RESOLVED' if script else 'DANGLING_SCRIPT_ID',
                          'site': script.get('site') if script else None,
                          'name': script.get('name') if script else None,
                          'ai_field': actor.get('ai')})
        ai_script = actor.get('ai_script')
        if ai_script:
            candidates = by_name.get(Path(ai_script).stem.casefold(), [])
            for script in candidates:
                links.append({'script_id': script['id'], 'association': 'ACTOR_AI_SCRIPT_FIELD',
                              'status': 'RESOLVED' if len(candidates) == 1 else 'AMBIGUOUS_SCRIPT_STEM',
                              'match_rule': 'UNIQUE_SCRIPT_STEM' if len(candidates) == 1 else 'SCRIPT_STEM_CANDIDATES',
                              'site': script.get('site'), 'name': script.get('name'),
                              'authored_value': ai_script})
            if not candidates:
                links.append({'script_id': None, 'association': 'ACTOR_AI_SCRIPT_FIELD',
                              'status': 'SCRIPT_NOT_FOUND', 'authored_value': ai_script})
        entities.append(_entity_record(actor['id'], 'actor',
            {'index': actor.get('index'), 'name': actor.get('name'), 'table': 'character_properties#group0',
             'row_id': actor.get('id'), 'ai': actor.get('ai')}, links,
             script_records, call_by_id, function_records,
             ['AI script closure is an authored/source join; runtime spawn and branch execution are not proven.']))

    for level in levels:
        links = []
        authored = _script_key(level.get('config', {}).get('scriptFile'))
        if authored:
            candidates = list({r['id']: r for r in by_site.get(authored, []) + by_site.get(Path(authored).name, [])}.values())
            for script in candidates:
                links.append({'script_id': script['id'], 'association': 'LEVEL_CONFIG_SCRIPTFILE',
                              'status': 'RESOLVED' if len(candidates) == 1 else 'AMBIGUOUS_SOURCE_PATH',
                              'match_rule': 'UNIQUE_BASENAME' if len(candidates) == 1 else 'BASENAME_CANDIDATES',
                              'site': script.get('site'), 'name': script.get('name'), 'authored_value': level['config'].get('scriptFile')})
            if not candidates:
                links.append({'script_id': None, 'association': 'LEVEL_CONFIG_SCRIPTFILE',
                              'status': 'SCRIPT_NOT_FOUND', 'authored_value': level['config'].get('scriptFile')})
        entities.append(_entity_record(level['id'], 'level',
            {'name': level.get('name'), 'file': level.get('file'), 'source': level.get('source'),
             'config_script_file': level.get('config', {}).get('scriptFile')}, links,
             script_records, call_by_id, function_records,
             ['Level script association follows the authored config scriptFile field.']))

    for profile in profiles:
        links = _profile_scripts(profile, scripts_by_id, by_name, by_site)
        entities.append(_entity_record(profile['id'], profile.get('kind', 'profile'),
            profile.get('source', {}), links, script_records, call_by_id, function_records,
            ['Profile-to-script is source association; it does not establish runtime selection.']))

    entity_index = {r['id']: r for r in entities}
    script_to_entities = defaultdict(list)
    function_to_entities = defaultdict(list)
    for entity in entities:
        for script_id in entity['script_ids']:
            script_to_entities[script_id].append(entity['id'])
        for function_id in entity['native_function_ids']:
            function_to_entities[function_id].append(entity['id'])

    idapath = physical_path(ctx, f'{IDA_ROOT}/pseudocode-all.c')
    idasym = physical_path(ctx, f'{IDA_ROOT}/functions.jsonl')
    native_index = {
        'schema_version': 1,
        'meaning': 'Static source relationships. Lua membership, native handler presence, port status, and runtime execution are separate claims.',
        'native_corpus': {
            'platform': 'Android APK ELF export',
            'export_date': '2026-10-07',
            'pseudocode_path': f'{IDA_ROOT}/pseudocode-all.c',
            'pseudocode_available': idapath.is_file(),
            'function_catalog_path': f'{IDA_ROOT}/functions.jsonl',
            'function_catalog_available': idasym.is_file(),
            'no_iOS_native_binary_claimed': True,
        },
        'entities_by_id': {r['id']: r for r in entities},
        'scripts_by_id': script_records,
        'functions_by_id': function_records,
        'indexes': {
            'entity_ids': sorted(entity_index),
            'script_ids': {k: sorted(v) for k, v in sorted(script_to_entities.items())},
            'function_ids': {k: sorted(v) for k, v in sorted(function_to_entities.items())},
        },
        'counts': {
            'entity_rows': len(entities),
            'entity_kinds': dict(Counter(r['kind'] for r in entities)),
            'actors': sum(r['kind'] == 'actor' for r in entities),
            'levels': sum(r['kind'] == 'level' for r in entities),
            'items': sum(r['kind'] == 'item' for r in entities),
            'skills': sum(r['kind'] == 'skill' for r in entities),
            'profiles': len(profiles), 'scripts': len(script_records),
            'calls': len(calls), 'native_function_ledger_rows': len(function_records),
            'functions_with_addresses': sum(bool(r['native_addresses']) for r in function_records.values()),
            'functions_without_addresses': sum(not r['native_addresses'] for r in function_records.values()),
            'addresses_with_decompilation_snippets': sum(bool(a.get('snippet')) for r in function_records.values() for a in r['native_addresses']),
            'addresses_without_decompilation_snippets': sum(not a.get('snippet') for r in function_records.values() for a in r['native_addresses']),
            'entities_with_native_functions': sum(bool(r['native_function_ids']) for r in entities),
            'entity_script_links': sum(len(r['script_links']) for r in entities),
            'entity_unresolved_script_links': sum(len(r['unresolved_script_links']) for r in entities),
            'entity_script_association_status': dict(Counter(r['script_association_status'] for r in entities)),
            'lua_calls_with_source_excerpt': sum(bool(c.get('lua_excerpt')) for s in script_records.values() for c in s['calls']),
            'lua_calls_without_source_excerpt': sum(not c.get('lua_excerpt') for s in script_records.values() for c in s['calls']),
        },
    }
    resource_index = build_resource_attributions(ctx)
    write_json(ctx.out / 'data/entity-native.json', native_index)
    write_json(ctx.out / 'data/resource-attributions.json', resource_index)
    return {'entity_native': native_index['counts'], 'resource_attributions': resource_index['counts']}
