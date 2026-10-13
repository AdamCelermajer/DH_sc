"""Coverage and referential-integrity checks for entity_native.py outputs."""
from collections import Counter
from pathlib import Path

from common import read_json
import entity_native


def run(ctx):
    native = read_json(ctx.out / 'data/entity-native.json', {})
    resources = read_json(ctx.out / 'data/resource-attributions.json', {})
    scripts = native.get('scripts_by_id', {})
    functions = native.get('functions_by_id', {})
    entities = native.get('entities_by_id', {})
    calls = {c['call_id']: c for s in scripts.values() for c in s.get('calls', [])}
    errors = []
    checks = []

    def check(name, passed, detail):
        checks.append({'name': name, 'passed': bool(passed), 'detail': detail})
        if not passed:
            errors.append(name + ': ' + str(detail))

    expected = {
        'actors': (448, 'actor'), 'skills': (127, 'skill'),
        'items': (1322, 'item'), 'levels': (190, 'level'),
        'profiles': (6404, None), 'scripts': (219, None),
        'native_function_ledger_rows': (288, None),
    }
    count_details = {}
    for count_name, (wanted, kind) in expected.items():
        actual = (sum(r.get('kind') == kind for r in entities.values()) if kind
                  else native.get('counts', {}).get(count_name, 0))
        count_details[count_name] = {'expected': wanted, 'actual': actual}
        check('global ' + count_name + ' coverage', actual == wanted, count_details[count_name])

    check('entity dictionary keys match row IDs', all(k == v.get('id') for k, v in entities.items()),
          {'rows': len(entities)})
    check('script dictionary keys match row IDs', all(k == v.get('id') for k, v in scripts.items()),
          {'rows': len(scripts)})
    check('function dictionary keys match row IDs', all(k == v.get('id') for k, v in functions.items()),
          {'rows': len(functions)})

    missing_calls, missing_functions, bad_entity_refs = [], [], []
    for script in scripts.values():
        for call in script.get('calls', []):
            if call.get('function_id') and call['function_id'] not in functions:
                missing_functions.append((script['id'], call['call_id'], call['function_id']))
    for entity in entities.values():
        for script_id in entity.get('script_ids', []):
            if script_id not in scripts:
                bad_entity_refs.append((entity['id'], 'script', script_id))
        for call_id in entity.get('call_ids', []):
            if call_id not in calls:
                missing_calls.append((entity['id'], call_id))
        for function_id in entity.get('native_function_ids', []):
            if function_id not in functions:
                bad_entity_refs.append((entity['id'], 'function', function_id))
    check('all call/function references resolve', not missing_functions and not missing_calls and not bad_entity_refs,
          {'dangling_call_ids': missing_calls[:20], 'dangling_function_ids': missing_functions[:20],
           'other_dangling_ids': bad_entity_refs[:20]})
    missing_association_explanations = [r['id'] for r in entities.values()
        if (not r.get('script_association_status') or
            (not r.get('script_ids') and not r.get('no_script_association_reason')))]
    check('entities without a script link state the source gap', not missing_association_explanations,
          {'missing': missing_association_explanations[:20], 'count': len(missing_association_explanations)})

    ledger = {r['id']: r for r in entity_native._rows(ctx, 'functions')}
    omitted_addresses = []
    for function_id, source in ledger.items():
        indexed = functions.get(function_id)
        if not indexed:
            omitted_addresses.append({'function_id': function_id, 'issue': 'missing function record'})
            continue
        source_addresses = source.get('native_addresses', [])
        indexed_addresses = indexed.get('native_addresses', [])
        if len(source_addresses) != len(indexed_addresses):
            omitted_addresses.append({'function_id': function_id, 'issue': 'address count differs'})
        for addr in source_addresses:
            if not any(a.get('address') == addr.get('address') and a.get('binding') == addr.get('binding')
                       for a in indexed_addresses):
                omitted_addresses.append({'function_id': function_id, 'address': addr.get('address'),
                                          'binding': addr.get('binding')})
    check('every available ledger handler/address is retained', not omitted_addresses,
          {'issues': omitted_addresses[:30], 'count': len(omitted_addresses)})
    missing_native_snippets = [
        {'function_id': fid, 'address': a.get('address'), 'path': a.get('pseudocode_path'),
         'line': a.get('source_line')}
        for fid, function in functions.items() for a in function.get('native_addresses', [])
        if not a.get('snippet') or not a.get('pseudocode_path') or not a.get('source_line')]
    check('available native addresses retain decompilation path, line, and snippet',
          not missing_native_snippets,
          {'missing': missing_native_snippets[:20], 'count': len(missing_native_snippets)})
    bad_native_locations = []
    for fid, function in functions.items():
        for address in function.get('native_addresses', []):
            source = address.get('pseudocode_path')
            line_no = address.get('source_line')
            path = entity_native.physical_path(ctx, source) if source else None
            if not path or not path.is_file() or not isinstance(line_no, int):
                bad_native_locations.append({'function_id': fid, 'path': source, 'line': line_no,
                                             'issue': 'source file/line missing'})
                continue
            lines = path.read_text(encoding='utf-8', errors='replace').splitlines()
            if line_no < 1 or line_no > len(lines):
                bad_native_locations.append({'function_id': fid, 'path': source, 'line': line_no,
                                             'line_count': len(lines), 'issue': 'line outside named source file'})
    check('native source lines point into the named decompilation files', not bad_native_locations,
          {'bad': bad_native_locations[:20], 'count': len(bad_native_locations)})
    missing_lua_snippets = [
        {'script_id': sid, 'call_id': c.get('call_id'), 'site': script.get('site'), 'line': c.get('line')}
        for sid, script in scripts.items() for c in script.get('calls', [])
        if c.get('line') is not None and not c.get('lua_excerpt')]
    check('available Lua call sites retain a source excerpt', not missing_lua_snippets,
          {'missing': missing_lua_snippets[:20], 'count': len(missing_lua_snippets)})

    supported_kinds = set(entity_native.RESOURCE_KINDS)
    profiles_source = entity_native._rows(ctx, 'entity-profiles')
    required_profiles = {p['id'] for p in profiles_source if p.get('kind') in supported_kinds}
    resource_profiles = resources.get('resource_profiles_by_id', {})
    missing_profiles = sorted(required_profiles - set(resource_profiles))
    extra_profiles = sorted(set(resource_profiles) - required_profiles)
    check('resource profile attribution is exhaustive', not missing_profiles and not extra_profiles,
          {'expected': len(required_profiles), 'indexed': len(resource_profiles),
           'missing': missing_profiles[:20], 'unexpected': extra_profiles[:20]})
    misclassified_owners = [
        {'profile_id': pid, 'edge': edge}
        for pid, profile in resource_profiles.items()
        for edge in profile.get('owners', [])
        if edge.get('classification') != 'DECLARED_PROVEN'
        or edge.get('status') != 'RESOLVED'
        or edge.get('confidence') != 'PROVEN'
    ]
    missing_profile_notes = [pid for pid, profile in resource_profiles.items()
                             if not profile.get('association_note')]
    bad_candidate_paths = [
        {'profile_id': pid, 'path': path}
        for pid, profile in resource_profiles.items()
        for path in profile.get('candidate_resource_paths', [])
        if path.get('classification') == 'DECLARED_PROVEN'
    ]
    candidate_paths_missing_reason = [
        {'profile_id': pid, 'edge_id': path.get('edge_id'), 'path': path.get('path')}
        for pid, profile in resource_profiles.items()
        for path in profile.get('candidate_resource_paths', [])
        if (path.get('reason_code') is None and path.get('reference_reason') is None
            and path.get('candidate_note') is None and path.get('source_gap_note') is None)
    ]
    candidate_paths_missing_gap_note = [
        {'profile_id': pid, 'edge_id': path.get('edge_id'), 'path': path.get('path')}
        for pid, profile in resource_profiles.items()
        for path in profile.get('candidate_resource_paths', [])
        if (not path.get('resolved_asset_ids') and not path.get('candidate_asset_ids')
            and not path.get('source_gap_note'))
    ]
    check('only resolved/proven profile edges are declared owners',
          not misclassified_owners and not missing_profile_notes and not bad_candidate_paths
          and not candidate_paths_missing_reason and not candidate_paths_missing_gap_note,
          {'bad_owners': misclassified_owners[:10], 'missing_association_note': missing_profile_notes[:10],
           'bad_candidates': bad_candidate_paths[:10],
           'candidate_reason_missing': candidate_paths_missing_reason[:10],
           'candidate_source_gap_missing': candidate_paths_missing_gap_note[:10]})

    # Reconcile confirmed missing resource paths against both the reference
    # ledger and the complete indexed asset paths. A FILE_MISSING edge counts
    # only when its authored physical/logical path is absent from that corpus.
    reference_rows = entity_native._rows(ctx, 'references')
    reference_by_id = {r.get('id'): r for r in reference_rows}
    known_asset_paths = set()
    for asset in entity_native._rows(ctx, 'assets'):
        known_asset_paths.update(str(v).casefold() for v in
                                 (asset.get('path'), asset.get('relative'), *asset.get('logical_paths', [])) if v)
    expected_missing_paths = set()
    for source_profile in profiles_source:
        if source_profile.get('kind') not in supported_kinds:
            continue
        for edge in source_profile.get('field_edges', []):
            reference = reference_by_id.get(edge.get('id'), {})
            status = edge.get('status') or reference.get('status')
            reason_code = reference.get('reason_code') or edge.get('reason_code')
            if status != 'UNRESOLVED' or reason_code != 'FILE_MISSING':
                continue
            for target in edge.get('targets', []):
                path = target.get('path')
                if not path:
                    continue
                # Only the exact requested path and its explicit mapped files
                # can satisfy this target. Sibling resolves_to candidates are
                # not evidence that the requested filename exists.
                target_paths = [path, *(target.get('asset_files') or [])]
                if not any(str(candidate).casefold() in known_asset_paths for candidate in target_paths):
                    expected_missing_paths.add((source_profile['id'], edge.get('id'), path))
    actual_missing_paths = {
        (pid, path.get('edge_id'), path.get('path'))
        for pid, profile in resource_profiles.items()
        for path in profile.get('missing_resource_paths', [])
    }
    malformed_missing_paths = [
        {'profile_id': pid, 'path': path}
        for pid, profile in resource_profiles.items()
        for path in profile.get('missing_resource_paths', [])
        if (path.get('reason_code') != 'FILE_MISSING'
            or path.get('source_missing_confirmed') is not True
            or path.get('resolved_asset_ids') or path.get('candidate_asset_ids'))
    ]
    missing_path_counts = resources.get('counts', {})
    check('confirmed missing resource paths match FILE_MISSING references and absent source paths',
          actual_missing_paths == expected_missing_paths and not malformed_missing_paths
          and missing_path_counts.get('missing_resource_path_edges') == len(actual_missing_paths)
          and missing_path_counts.get('confirmed_file_missing_resource_paths') == len(actual_missing_paths),
          {'expected': len(expected_missing_paths), 'actual': len(actual_missing_paths),
           'count_field': missing_path_counts.get('missing_resource_path_edges'),
           'malformed': malformed_missing_paths[:10],
           'missing_rows': sorted(expected_missing_paths - actual_missing_paths)[:10],
           'unexpected_rows': sorted(actual_missing_paths - expected_missing_paths)[:10]})

    media_ext = set(entity_native.MEDIA_EXTENSIONS)
    asset_source = entity_native._rows(ctx, 'assets')
    required_assets = {a['id'] for a in asset_source
                       if Path(a.get('path', '')).suffix.casefold() in media_ext
                       or entity_native._logical_media_paths(a)}
    file_rows = resources.get('resource_files_by_id', {})
    missing_assets = sorted(required_assets - set(file_rows))
    extra_assets = sorted(set(file_rows) - required_assets)
    invalid_status = [a.get('id') for a in file_rows.values()
                      if a.get('association_status') not in {
                          'AUTHORED_RESOURCE_PATH_ASSOCIATION', 'PACKED_LOGICAL_PATH_CANDIDATE',
                          'UNRESOLVED_RESOURCE_PATH_CANDIDATE', 'DECODER_JOIN_ASSOCIATION_ONLY',
                          'SOURCE_DECODER_CONSUMER_ONLY', 'ORPHAN_IN_EXTRACTED_CORPUS'}]
    no_file_note = [a.get('id') for a in file_rows.values() if not a.get('association_note')]
    check('resource file attribution is exhaustive and typed',
          not missing_assets and not extra_assets and not invalid_status and not no_file_note,
          {'expected': len(required_assets), 'indexed': len(file_rows),
           'missing': missing_assets[:20], 'unexpected': extra_assets[:20],
           'untyped': invalid_status[:20], 'missing_association_note': no_file_note[:20]})
    packed_media = [a for a in asset_source if a.get('source') == 'ios-packed'
                    and entity_native._logical_media_paths(a)]
    packed_rows = [file_rows.get(a['id']) for a in packed_media]
    packed_coverage_errors = [a['id'] for a, row in zip(packed_media, packed_rows)
        if not row or row.get('transport_path') != a.get('path')
        or row.get('mapping_confidence') != a.get('mapping_confidence')
        or not row.get('logical_media_paths')
        or row.get('association_status') != 'PACKED_LOGICAL_PATH_CANDIDATE']
    check('packed logical media paths retain transport/provenance as candidates',
          not packed_coverage_errors,
          {'expected_files': len(packed_media), 'indexed_candidates': len(packed_rows) - len(packed_coverage_errors),
           'errors': packed_coverage_errors[:20]})
    packed_owner_claims = [r['id'] for r in packed_rows if r and
                           (r.get('association_status') != 'PACKED_LOGICAL_PATH_CANDIDATE'
                            or r.get('authored_path_profile_ids'))]
    check('packed likely aliases never become exact path owners', not packed_owner_claims,
          {'incorrect': packed_owner_claims[:20], 'count': len(packed_owner_claims)})

    ownerless_runtime_claims = [r.get('id') for r in resource_profiles.values()
                                if r.get('runtime_status') != 'UNPROVEN_RUNTIME_USE']
    file_runtime_claims = [r.get('id') for r in file_rows.values()
                           if r.get('runtime_status') != 'UNPROVEN_RUNTIME_USE']
    check('runtime claims remain explicitly unproven', not ownerless_runtime_claims and not file_runtime_claims,
          {'profile_claims': ownerless_runtime_claims[:20], 'file_claims': file_runtime_claims[:20]})

    sample_names = {'Witch': ('actor', 'Witch'),
                    'Knight': ('actor', 'KnightPlayerBase'),
                    'Bow': ('item', 'Bow12'),
                    'Mud Lord': ('actor', 'SwampKing')}
    sample_results = {}
    for label, (kind, name) in sample_names.items():
        matches = [r for r in entities.values()
                   if r.get('kind') == kind and r.get('source', {}).get('name') == name]
        good = len(matches) == 1
        entity = matches[0] if good else None
        sample_results[label] = ({'id': entity['id'], 'script_ids': entity.get('script_ids', []),
                                  'native_function_ids': entity.get('native_function_ids', []),
                                  'script_association_status': entity.get('script_association_status'),
                                  'no_script_association_reason': entity.get('no_script_association_reason'),
                                  'unresolved_script_links': entity.get('unresolved_script_links', [])}
                                 if entity else {'matches': len(matches)})
        if entity:
            good = all(fid in functions for fid in entity.get('native_function_ids', []))
            if not entity.get('script_ids'):
                good = good and bool(entity.get('association_note'))
        check('sample entity mapping: ' + label, good, sample_results[label])

    result = {'passed': not errors, 'tests': checks,
              'samples': sample_results,
              'scope': 'Static index coverage and dangling-link checks; no original runtime certification.'}
    return result


def coverage_check(ctx):
    result = run(ctx)
    return [] if result['passed'] else [t['name'] + ': ' + str(t['detail'])
                                         for t in result['tests'] if not t['passed']]
