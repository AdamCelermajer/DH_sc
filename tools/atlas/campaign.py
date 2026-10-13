"""Read-only decoder for serialized per-level PyScript name/command streams.

The loader transforms a configured `.pyscript` path into sibling
`_pyscripts.bin` and `_pyscriptnames.bin` files. This module decodes those
source records for Atlas inspection; it never runs command callbacks.
"""
from collections import Counter
import re
import struct
from pathlib import Path

from common import read_json, write_json, stable_id, source_site


def _schema_actions(path):
    schemas = {}
    for line in path.read_text(encoding='utf-8').splitlines():
        m = re.match(r'^\s*\{(\d+),\{', line)
        if not m:
            continue
        start = line.find('{{', m.end())
        if start < 0:
            continue
        depth, end = 0, None
        for i in range(start, len(line)):
            if line[i] == '{': depth += 1
            elif line[i] == '}':
                depth -= 1
                if depth == 0:
                    end = i + 1
                    break
        if end is None:
            continue
        actions = [(int(a), int(w), int(c)) for a, w, c in
                   re.findall(r'\{(\d+),(\d+),(\d+)\}', line[start:end])]
        schemas[int(m.group(1))] = actions
    return schemas


def _command_names(path):
    result = {}
    for line in path.read_text(encoding='utf-8').splitlines():
        m = re.match(r'\s*\{(\d+),\{\d+,"([^"]+)"', line)
        if m:
            result[int(m.group(1))] = m.group(2)
    return result


def _command_catalog(descriptors_path, host_path, world_path):
    catalog = {}
    for line_number, line in enumerate(descriptors_path.read_text(encoding='utf-8').splitlines(), 1):
        m = re.search(r'\{(\d+),\{\d+,"([^"]+)",(0x[0-9a-fA-F]+)u?,(0x[0-9a-fA-F]+)u?', line)
        if m:
            catalog[int(m.group(1))] = {'name': m.group(2),
                'native_factory_address': m.group(3), 'native_reader_address': m.group(4),
                'descriptor_source': f'port/level-loader/command_c1_descriptors_v59.inc:{line_number}'}
    anchors = {}
    for path, label in ((host_path, 'port/windows-foundation/features/campaign_host/campaign_host.cpp'),
                        (world_path, 'port/windows-foundation/original_campaign_world_adapter.cpp')):
        for line_number, line in enumerate(path.read_text(encoding='utf-8').splitlines(), 1):
            for m in re.finditer(r'\bcase\s+(\d+)\s*:', line):
                anchors.setdefault(int(m.group(1)), []).append(f'{label}:{line_number}')
    for kind, row in catalog.items():
        row['port_path_anchors'] = anchors.get(kind, [])
        # A visible dispatch case is only a code-path anchor, not proof that all
        # source gates/side effects are ported or runtime verified.
        row['port_status'] = 'PARTIAL' if row['port_path_anchors'] else 'UNKNOWN'
    return catalog


def _signed32(value):
    value = int(value)
    return value - (1 << 32) if value >= (1 << 31) else value


def _u32(data, cursor):
    if cursor + 4 > len(data):
        raise ValueError('truncated 32-bit word')
    return struct.unpack_from('<I', data, cursor)[0], cursor + 4


def _read_names(path):
    data = path.read_bytes()
    count, cursor = _u32(data, 0)
    if count > 8192:
        raise ValueError(f'name count outside native reader bound: {count}')
    names = []
    for _ in range(count):
        size, cursor = _u32(data, cursor)
        if size > 1 << 20 or cursor + size > len(data):
            raise ValueError('truncated or oversize name')
        names.append(data[cursor:cursor+size].decode('utf-8', 'replace'))
        cursor += size
    if cursor != len(data):
        raise ValueError('unexpected name stream tail')
    return names


def _read_scripts(path, names, schemas, labels):
    data = path.read_bytes()
    count, cursor = _u32(data, 0)
    if count != len(names):
        raise ValueError(f'name/script count mismatch: {len(names)} vs {count}')
    result = []
    for script_index in range(count):
        command_count, cursor = _u32(data, cursor)
        if command_count > 65536:
            raise ValueError('command count outside native reader bound')
        commands = []
        for command_index in range(command_count):
            # The original manager peeks the kind without advancing, then the
            # generated-data reader consumes that same word as field offset 4.
            kind, _ = _u32(data, cursor)
            if kind not in schemas:
                raise ValueError(f'no recovered generated-data schema for kind {kind}')
            values = {}
            for offset, width, count_offset in schemas[kind]:
                if width in (1, 2, 4):
                    if cursor + width > len(data):
                        raise ValueError('truncated command scalar')
                    value = int.from_bytes(data[cursor:cursor+width], 'little')
                    cursor += width
                    values[offset] = value
                else:
                    size = values.get(count_offset)
                    if size is None:
                        raise ValueError(f'missing source length field {count_offset} for kind {kind}')
                    if width == 0:
                        if size > 1 << 20 or cursor + size > len(data):
                            raise ValueError('truncated or oversize command string')
                        values[offset] = data[cursor:cursor+size].decode('utf-8', 'replace')
                        cursor += size
                    elif width == 255:
                        if size > 65536 or cursor + 4*size > len(data):
                            raise ValueError('truncated or oversize integer array')
                        values[offset] = list(struct.unpack_from('<' + 'I'*size, data, cursor)) if size else []
                        cursor += 4*size
                    else:
                        raise ValueError(f'unknown generated-data action width {width}')
            commands.append({'index': command_index, 'kind': kind,
                             'name': labels.get(kind, f'UNKNOWN_COMMAND_{kind}'),
                             'fields': values, 'decode_status': 'DONE'})
        result.append({'index': script_index,
                       'name': names[script_index] if script_index < len(names) else '',
                       'command_count': command_count, 'commands': commands})
    if cursor != len(data):
        raise ValueError('unexpected command stream tail')
    return result


def run(ctx, inventory_only=False):
    out, inputs = Path(ctx.out), Path(ctx.inputs)
    script_dirs = [inputs / rel for rel in (
        'assets-extra/android/data/pydata/scripts',
        'assets-extra/android/original-cache/data/pydata/scripts',
        'assets-extra/ios/data/pydata/scripts') if (inputs / rel).is_dir()]
    scripts_dir = Path(__file__).resolve().parents[2] / 'port/level-loader'
    schemas = _schema_actions(scripts_dir / 'script_data_schemas_v52.inc')
    descriptors_path = scripts_dir / 'command_c1_descriptors_v59.inc'
    labels = _command_names(descriptors_path)
    command_catalog = _command_catalog(descriptors_path,
        Path(__file__).resolve().parents[2] / 'port/windows-foundation/features/campaign_host/campaign_host.cpp',
        Path(__file__).resolve().parents[2] / 'port/windows-foundation/original_campaign_world_adapter.cpp')
    scenes = [r for r in read_json(out / 'data/levels.json', [])
              if r.get('record_type') == 'scene_file']
    actors = read_json(out / 'data/actors.json', []) or []
    levels = read_json(out / 'data/tables.json', []) or []
    extra_tables = read_json(out / 'data/tables-extra.json', []) or []
    tables = levels + extra_tables
    actor_by_name = {str(a.get('name', '')).casefold(): a for a in actors}
    level_names = {str(r.get('name', '')).casefold(): r
                   for r in levels if r.get('table', '').startswith('levels#group1')}
    sound_rows = {int(r.get('index', -1)): r for r in tables if r.get('table') == 'sdd_dungeon_hunter_2_iphone'}
    anim_rows = {int(r.get('index', -1)): r for r in tables if r.get('table') == 'animations_dictionary#group0'}
    effect_rows = {int(r.get('index', -1)): r for r in tables if r.get('table') == 'effect_sets'}
    by_dir = [{p.name.casefold(): p for p in folder.glob('*_pyscriptnames.bin')}
              for folder in script_dirs]
    commands_by_dir = [{p.name.casefold(): p for p in folder.glob('*_pyscripts.bin')}
                       for folder in script_dirs]
    records, references = [], []
    actor_spawn_evidence = {}
    counts = Counter()
    for scene in scenes:
        config = scene.get('config') or {}
        script_path = str(config.get('scriptFile', ''))
        if not script_path:
            continue
        stem = Path(script_path.replace('\\', '/')).name
        prefix = stem.rsplit('.', 1)[0].casefold()
        pair = None
        for folder, file_map, command_map in zip(script_dirs, by_dir, commands_by_dir):
            possible_names = file_map.get((prefix + '_pyscriptnames.bin').casefold())
            possible_commands = command_map.get((prefix + '_pyscripts.bin').casefold())
            if possible_names and possible_commands:
                pair = (folder, possible_names, possible_commands)
                break
        folder, names_path, commands_path = pair if pair else (None, None, None)
        row = {'id': stable_id('campaign-script-file', scene['id']),
               'scene_id': scene['id'], 'scene': scene.get('file'),
               'act': scene.get('act', 'unplaced'), 'script_file': script_path,
               'source_names': source_site(ctx, names_path) if names_path else None,
               'source_commands': source_site(ctx, commands_path) if commands_path and commands_path.is_file() else None,
               'source_platform': ('android' if 'assets-extra/android/data/' in names_path.as_posix().replace('\\','/')
                                   else 'original-cache' if 'original-cache' in names_path.as_posix().replace('\\','/')
                                   else 'ios') if names_path else None,
               'scripts': [], 'status': 'TODO', 'work_status': 'TODO'}
        if not names_path or not commands_path.is_file():
            row.update(status='TODO' if inventory_only else 'UNRESOLVED',
                       work_status='TODO' if inventory_only else 'UNRESOLVED',
                       reason='Configured script path has no matching serialized name/command pair in supplied Android, recovered-cache, or iOS PyData inputs.')
            records.append(row)
            counts['missing_pairs'] += 1
            continue
        try:
            names = _read_names(names_path)
            decoded = _read_scripts(commands_path, names, schemas, labels)
            for decoded_script in decoded:
                decoded_script['status'] = 'TODO' if inventory_only else 'DONE'
                decoded_script['work_status'] = decoded_script['status']
                for decoded_command in decoded_script['commands']:
                    decoded_command['decode_status'] = 'TODO' if inventory_only else 'DONE'
            row['scripts'] = decoded
            row['script_count'] = len(decoded)
            row['command_count'] = sum(s['command_count'] for s in decoded)
            row['spawn_targets'] = []
            row['command_readiness'] = []
            command_summary = {}
            row['status'] = 'TODO' if inventory_only else 'DONE'
            row['work_status'] = 'TODO' if inventory_only else 'DONE'
            for script in decoded:
                for command in script['commands']:
                    cname = command['name']
                    kind_id = int(command['kind'])
                    native_mapping = command_catalog.get(kind_id, {'name': cname,
                        'port_status': 'UNKNOWN', 'port_path_anchors': []})
                    if inventory_only:
                        native_mapping = {**native_mapping, 'status': 'TODO', 'work_status': 'TODO'}
                    else:
                        native_mapping = {**native_mapping,
                            'status': 'PARTIAL' if native_mapping.get('port_status') == 'PARTIAL' else 'UNRESOLVED',
                            'work_status': 'DONE',
                            'reason_code': None if native_mapping.get('port_status') == 'PARTIAL' else 'OTHER',
                            'reason': None if native_mapping.get('port_status') == 'PARTIAL' else 'No reviewed port dispatch anchor was found in the two indexed command-owner files.',
                            'candidates': native_mapping.get('port_path_anchors', [])}
                    command['native_mapping'] = native_mapping
                    summary = command_summary.setdefault(kind_id, {'kind': kind_id, 'name': cname,
                        'count': 0, **native_mapping})
                    summary['count'] += 1
                    if cname == 'Script_SpawnCharacter':
                        target_name = command['fields'].get(12)
                        target_actor = actor_by_name.get(str(target_name).casefold()) if target_name else None
                        if target_name and not inventory_only:
                            spawn = {'actor_id': target_actor['id'] if target_actor else None,
                                     'actor': target_actor['name'] if target_actor else None,
                                     'target_name': str(target_name),
                                     'scene_id': scene['id'], 'scene': scene.get('file'),
                                     'act': scene.get('act', 'unplaced'),
                                     'script_name': script['name'], 'script_index': script['index'],
                                     'command_index': command['index'], 'field_offset': 12,
                                     'status': 'INFERENCE',
                                     'reason': 'Exact name argument to original Script_SpawnCharacter command; branch execution and runtime spawn are not asserted.'}
                            row['spawn_targets'].append(spawn)
                            if target_actor:
                                actor_spawn_evidence.setdefault(target_actor['id'], []).append(spawn)
                    for offset, value in command['fields'].items():
                        if not isinstance(value, str) or not value:
                            continue
                        match = actor_by_name.get(value.casefold())
                        kind = 'actor_name_candidate' if match else 'serialized_string'
                        target = match.get('id') if match else None
                        if value.casefold() in level_names:
                            kind, target = 'level_name_candidate', level_names[value.casefold()].get('id')
                        exact_candidates = ([{'id': match.get('id'), 'type': 'character_properties', 'name': match.get('name')}]
                                            if match else [])
                        if value.casefold() in level_names:
                            level_match = level_names[value.casefold()]
                            exact_candidates.append({'id': level_match.get('id'), 'type': 'levels',
                                                     'name': level_match.get('name')})
                        ref_id = stable_id('campaign-reference', scene['id'], script['index'], command['index'], offset)
                        ref_kind = ('spawn_actor_object_name' if cname == 'Script_SpawnCharacter' and offset == 12
                                    else kind)
                        ref = {'id': ref_id,
                               'site': source_site(ctx, commands_path) +
                                   f":script[{script['index']}]={script['name']}/command[{command['index']}]={cname}/field@{offset}",
                               'name': value, 'kind': kind, 'rule': 'CAMPAIGN_SCRIPT_STRING_FIELD',
                               'scene_id': scene['id'], 'act': scene.get('act', 'unplaced'),
                               'script_name': script['name'], 'script_index': script['index'],
                               'command_index': command['index'], 'command_kind': command['kind'],
                               'command': cname, 'field_offset': offset, 'value': value,
                               'reference_kind': ref_kind, 'resolves_to': [],
                               'candidates': exact_candidates,
                               'candidate_note': 'Exact table-name string match is a candidate link only; command field semantics require confirmation.',
                               'confidence': 'INFERENCE' if exact_candidates else 'UNKNOWN',
                               'port_status': 'UNKNOWN', 'implementation': None,
                               'status': 'TODO' if inventory_only else 'UNRESOLVED',
                               'work_status': 'TODO' if inventory_only else 'DONE',
                               'reason_code': None if inventory_only else 'OTHER',
                               'reason': 'Serialized command string was decoded; its target role requires command-field evidence.'}
                        references.append(ref)
                    numeric_targets = []
                    if kind_id in (13, 14) and 16 in command['fields']:
                        numeric_targets.append((16, 'source_sound_ordinal', 'sdd_dungeon_hunter_2_iphone', sound_rows,
                            'Descriptor receiver and canonical audio adapter preserve field 16 as the source-row ordinal.'))
                    elif kind_id == 20 and 8 in command['fields']:
                        numeric_targets.append((8, 'effect_set', 'effect_sets', effect_rows,
                            'Campaign host passes Script_PlayEffect field 8 as the source effects set index.'))
                    elif kind_id == 45:
                        for offset in (8, 12):
                            if offset in command['fields']:
                                numeric_targets.append((offset, 'animation_dictionary', 'animations_dictionary#group0', anim_rows,
                                    'Campaign host resolves Script_PlayActorAnim fields 8/12 through animations_dictionary; uint32 0xFFFFFFFF is the no-chain sentinel.'))
                    for offset, kind_name, table_name, target_rows, evidence in numeric_targets:
                        signed_value = _signed32(command['fields'][offset])
                        if kind_name == 'animation_dictionary' and signed_value == -1:
                            continue
                        target = target_rows.get(signed_value)
                        site = (source_site(ctx, commands_path) +
                            f":script[{script['index']}]={script['name']}/command[{command['index']}]={cname}/field@{offset}")
                        references.append({'id': stable_id('campaign-reference', scene['id'], script['index'], command['index'], offset),
                            'site': site, 'name': target.get('name') if target else str(signed_value),
                            'kind': kind_name, 'rule': 'CAMPAIGN_TYPED_NUMERIC_OPERAND',
                            'scene_id': scene['id'], 'act': scene.get('act', 'unplaced'),
                            'script_name': script['name'], 'script_index': script['index'],
                            'command_index': command['index'], 'command_kind': kind_id,
                            'command': cname, 'field_offset': offset, 'value': signed_value,
                            'resolves_to': ([{'id': target.get('id'), 'table': table_name,
                                              'index': signed_value, 'name': target.get('name')}]
                                            if target else []),
                            'candidates': [], 'candidate_note': evidence,
                            'confidence': 'PROVEN' if target else 'UNKNOWN',
                            'port_status': native_mapping.get('port_status', 'UNKNOWN'),
                            'implementation': native_mapping.get('port_path_anchors'),
                            'status': ('TODO' if inventory_only else 'RESOLVED' if target else 'UNRESOLVED'),
                            'work_status': ('TODO' if inventory_only else 'DONE' if target else 'UNRESOLVED'),
                            'reason': None if target else 'ID_OUT_OF_RANGE'})
            row['command_readiness'] = [command_summary[k] for k in sorted(command_summary)]
            counts['decoded_script_files'] += 1
            counts['source_' + str(row['source_platform'])] += 1
            counts['scripts'] += len(decoded)
            counts['commands'] += row['command_count']
        except (OSError, ValueError) as exc:
            row.update(status='UNRESOLVED', work_status='UNRESOLVED', reason=f'Serialized source decode failed: {exc}')
            counts['decode_failures'] += 1
        records.append(row)
    write_json(out / 'data/campaign.json', records)
    write_json(out / 'data/campaign-references.json', references)
    assignments = []
    act_by_actor = {}
    for actor_id, evidence_rows in actor_spawn_evidence.items():
        acts = sorted({r['act'] for r in evidence_rows if r['act'] != 'unplaced'})
        act_by_actor[actor_id] = acts
        for item in evidence_rows:
            assignments.append({**item, 'actor_id': actor_id, 'work_status': 'DONE'})
    for actor in actors:
        acts = act_by_actor.get(actor.get('id'), [])
        if acts:
            actor['scene_membership'] = acts[0] if len(acts) == 1 else 'shared'
            actor['shared_acts'] = acts if len(acts) > 1 else []
            actor['act_assignment_status'] = 'INFERENCE'
            actor['act_assignment_reason'] = 'Named by one or more source Script_SpawnCharacter commands; execution branches/runtime state are not established.'
    write_json(out / 'data/actors.json', actors)
    membership_path = out / 'data/act-membership.json'
    membership = read_json(membership_path, {}) or {}
    membership['actor_assignments'] = assignments
    membership['actor_assignment_gap'] = (
        'Inventory checkpoint: actor-to-level script mapping has not been accepted.' if inventory_only else
        'Serialized level script command targets now identify actors named by Script_SpawnCharacter. '
        'Assignments are INFERENCE because branch execution and runtime spawn conditions are not evaluated; the scene XML still contains no static mob roster.')
    write_json(membership_path, membership)
    return {'category': 'SERIALIZED LEVEL CAMPAIGN SCRIPTS', 'work_status': 'TODO',
            'script_file_rows': len(records), 'decoded_script_files': counts['decoded_script_files'],
            'scripts': counts['scripts'], 'commands': counts['commands'],
            'candidate_string_references': len(references),
            'missing_pairs': counts['missing_pairs'], 'decode_failures': counts['decode_failures'],
            'by_source': {k.removeprefix('source_'): v for k, v in counts.items() if k.startswith('source_')},
            'notes': ['Commands are decoded source records and are not executed.',
                      'Exact table-name matches are candidate references; field semantics remain TODO.']}


def coverage_check(ctx):
    out = Path(ctx.out)
    campaign = read_json(out / 'data/campaign.json', []) or []
    levels = [r for r in read_json(out / 'data/levels.json', []) if r.get('record_type') == 'scene_file']
    expected = {r['id'] for r in levels if (r.get('config') or {}).get('scriptFile')}
    actual = {r.get('scene_id') for r in campaign}
    ids = [r.get('id') for r in campaign]
    errors = []
    if actual != expected:
        errors.append(f'campaign: scene script coverage differs (missing={sorted(expected-actual)[:5]}, stale={sorted(actual-expected)[:5]})')
    if len(ids) != len(set(ids)):
        errors.append('campaign: duplicate source script-file IDs')
    if any(not r.get('scene_id') or not r.get('script_file') or not r.get('status') for r in campaign):
        errors.append('campaign: row missing scene/script path/status')
    for r in campaign:
        if r.get('status') == 'UNRESOLVED' and not r.get('reason'):
            errors.append(f"campaign: unresolved script pair lacks reason for {r.get('scene')}")
        if r.get('status') == 'DONE':
            if r.get('script_count') != len(r.get('scripts', [])):
                errors.append(f"campaign: script count mismatch for {r.get('scene')}")
            if r.get('command_count') != sum(s.get('command_count', 0) for s in r.get('scripts', [])):
                errors.append(f"campaign: command count mismatch for {r.get('scene')}")
    return errors
