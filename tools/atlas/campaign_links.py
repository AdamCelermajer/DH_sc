"""Source-backed relationship resolver for decoded campaign command operands.

This module adds meaning to serialized string fields only where a recovered
consumer establishes the operand's role. Exact scene-object matches are links
to authored object definitions; their CharacterProperties associations remain a
separate, static template relation and never claim that a command executed.
"""
from __future__ import annotations

from collections import defaultdict
from pathlib import Path
import re
import struct
import xml.etree.ElementTree as ET

from common import read_json, stable_id


# (command kind, serialized field offset): (relationship role, evidence)
# Offsets are the generated-data reader offsets (the command header is at +4).
_STRING_ROLES = {
    (8, 16): ('camera_target_object',
              'IDA Script_SetCameraTarget::Execute 0x460004 passes the authored name to CameraTarget::SetTarget; the camera target performs ObjectManager name lookup (coordination/v20/EVIDENCE-review.md:148).'),
    (19, 24): ('animated_scene_object',
               'IDA Script_PlayAnimByName::Execute resolves the named scene GameObject and calls its visual animator Play; port/level-loader/command_c1_descriptors_v59.inc:22 and port/windows-foundation/features/campaign_host/campaign_host.cpp:679-700.'),
    (20, 32): ('effect_anchor_object',
               'IDA Script_PlayEffect::Execute 0x45fa9c resolves the authored object name as the FX anchor; port/windows-foundation/features/campaign_host/campaign_host.cpp:702-717.'),
    (30, 12): ('character_object_instance',
               'IDA Script_SpawnCharacter::Execute 0x45f400 calls ObjectManager::GetObjectByName(name), converts the result to Character, then calls CharStateMachine::SM_SetSpawnState; coordination/v21.4/B141-report.md:20 and pseudocode-all.c:248020.'),
    (40, 24): ('moving_character_object',
               'IDA Script_MoveActor::Execute 0x45f0a8 resolves field +24 and converts it to Character; coordination/v20/EVIDENCE-review.md:96 and pseudocode-all.c:247897.'),
    (40, 12): ('move_destination_object',
               'IDA Script_MoveActor::Execute 0x45f0a8 resolves field +12 as GameObject and passes it to Cmd_MoveTo; pseudocode-all.c:247897.'),
    (41, 20): ('facing_character_object',
               'IDA Script_LookActor::Execute resolves field +20 as the actor; its +12 target is resolved as a Character first and otherwise as a waypoint; port/windows-foundation/features/campaign_host/campaign_host.cpp:637-659.'),
    (41, 12): ('look_target_object_or_waypoint',
               'IDA/port behavior resolves field +12 to a named actor first, then to a placed waypoint; port/windows-foundation/features/campaign_host/campaign_host.cpp:637-659.'),
    (42, 12): ('visible_character_object',
               'Campaign host kind 42 resolves field +12 to the named Character before applying visibility; port/windows-foundation/features/campaign_host/campaign_host.cpp:661-667.'),
    (43, 12): ('visible_character_object',
               'Campaign host kind 43 resolves field +12 to the named Character before applying visibility; port/windows-foundation/features/campaign_host/campaign_host.cpp:661-667.'),
    (44, 12): ('killed_character_object',
               'IDA Script_KillActor::Execute resolves the named Character and invokes its kill command; coordination/v20/EVIDENCE-review.md:96 and port/windows-foundation/features/campaign_host/campaign_host.cpp:588-593.'),
    (45, 24): ('animated_character_object',
               'IDA Script_PlayActorAnim::Init 0x45a2ec and Execute 0x45e890 resolve field +24 through ObjectManager and Character conversion; port/windows-foundation/features/campaign_host/campaign_host.cpp:595-617.'),
    (46, 12): ('position_waypoint_object',
               'IDA Script_SetActorPosition::Execute 0x45e5c8 resolves field +12 as waypoint and places the field +20 actor there; coordination/v20/EVIDENCE-review.md:96, port/windows-foundation/features/campaign_host/campaign_host.cpp:619-635.'),
    (46, 20): ('positioned_character_object',
               'IDA Script_SetActorPosition::Execute 0x45e5c8 resolves field +20 as the Character to reposition; coordination/v20/EVIDENCE-review.md:96.'),
    (29, 16): ('limbus_character_object',
               'Campaign host kind 29 resolves field +16 to the named Character before its Limbus transition; port/windows-foundation/features/campaign_host/campaign_host.cpp:669-676.'),
    (31, 12): ('idle_character_object',
               'Original campaign world adapter kind 31 resolves field +12 as a named character before setting Idle; port/windows-foundation/original_campaign_world_adapter.cpp:38-45.'),
    (32, 12): ('scripted_character_object',
               'Original campaign world adapter kind 32 resolves field +12 as a named character before setting the scripted flag; port/windows-foundation/original_campaign_world_adapter.cpp:38-45.'),
    (39, 12): ('stopped_character_object',
               'Original campaign world adapter kind 39 resolves field +12 as a named character before stopping it; port/windows-foundation/original_campaign_world_adapter.cpp:38-45.'),
    (64, 16): ('destination_level',
               'IDA Script_ChangeLevel::Execute 0x45cba0 matches field +16 against Arrays::LevelList::m_memberNames, then loads that level; .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:246813-246877 and port/level-loader/command_c1_descriptors_v59.inc:74.'),
}

_NUMERIC_ROLES = {
    (10, 8): ('dialog_actor_id', 'dialogs#group0',
              'Script_StartDialog constructs DialogMsg and passes scalar +8 to DialogMsg::SetActorName; 0x433dd8 indexes Arrays::DialogActors with that value and reads row +16; pseudocode-all.c:248929-248956, 220177-220193.'),
    (11, 8): ('dialog_message_list_id', 'dialogs#group1',
              'Script_StartDialogID passes scalar +8 to GetDialogMsgListById; 0x433f04 bounds-checks Arrays::Dialogs and selects members + 12 * id; pseudocode-all.c:220213-220280, 248959-248985.'),
    (13, 16): ('source_sound_ordinal', 'sdd_dungeon_hunter_2_iphone',
               'Script_PlaySound stores field +16 as the source sound ordinal; campaign_host.cpp:755 and command_c1_descriptors_v59.inc:15.'),
    (14, 16): ('source_sound_ordinal', 'sdd_dungeon_hunter_2_iphone',
               'Script_StopSound stores field +16 as the source sound ordinal; campaign_host.cpp:732 and command_c1_descriptors_v59.inc:16.'),
    (20, 8): ('effect_set', 'effect_sets',
              'Script_PlayEffect field +8 is registered and played as an effects-set index; script_command_init_v62.hpp:86 and campaign_host.cpp:702-717.'),
    (45, 8): ('animation_dictionary', 'animations_dictionary#group0',
              'Script_PlayActorAnim field +8 is resolved through animations_dictionary; campaign_host.cpp:595-617 and actor_clip_manifest.hpp:4.'),
    (45, 12): ('chained_animation_dictionary', 'animations_dictionary#group0',
               'Script_PlayActorAnim field +12 is the chained dictionary index, with uint32 0xFFFFFFFF as the no-chain sentinel; campaign_host.cpp:595-617 and actor_clip_manifest.hpp:4.'),
}

_DIALOG_OPERANDS = {
    (10, 12): ('dialog_style_id',
              'Script_StartDialog::Execute 0x460fe8 constructs DialogMsg with scalar +12 as style; coordination/claude-preview16/OPENING2-report.md:17.'),
    (10, 16): ('dialog_text_string_id',
              'Script_StartDialog::Execute 0x460fe8 constructs DialogMsg with scalar +16 as text string ID; coordination/claude-preview16/OPENING2-report.md:17.'),
}


def _key(value):
    return str(value or '').strip().casefold()


def _index(rows, key_name='name'):
    result = defaultdict(list)
    for row in rows or []:
        key = _key(row.get(key_name))
        if key:
            result[key].append(row)
    return result


def _unique_source_rows(rows):
    """Collapse repeated table copies without hiding distinct duplicate names."""
    unique = {}
    for row in rows or []:
        identity = row.get('id') or (row.get('table'), row.get('index'))
        unique[identity] = row
    return list(unique.values())


def _module_path_index(ctx):
    root = Path(ctx.inputs) / 'assets-extra/android/data/3d/modules'
    if not root.is_dir():
        return root, {}
    return root, {path.as_posix().casefold(): path
                  for path in root.rglob('*.mgp') if path.is_file()}


def _resolve_module_path(value, module_root, modules_by_path):
    normalized = str(value or '').replace('\\', '/')
    marker = normalized.casefold().find('3d/modules/')
    if marker >= 0:
        tail = normalized[marker + len('3d/modules/'):]
        found = modules_by_path.get((module_root / tail).as_posix().casefold())
        if found:
            return found
    # Procedural choices are relative to a scene's MGP folder and may only
    # supply the leaf MGP filename.
    candidate = module_root / normalized.lstrip('/')
    return modules_by_path.get(candidate.as_posix().casefold())


def _scene_module_files(ctx, scene, module_root, modules_by_path):
    """Collect exact source MGPs admitted by this scene's authored references."""
    result = {}
    for obj in scene.get('direct_objects', []) or []:
        path = _resolve_module_path(obj.get('mgp'), module_root, modules_by_path)
        if path:
            result.setdefault(path, set()).add('direct scene module reference')
    folder = str((scene.get('config') or {}).get('folder', '')).replace('\\', '/')
    match = re.search(r'(?:data/)?3d/modules/(.+)$', folder, re.IGNORECASE)
    folder_rel = match.group(1) if match else ''
    for proc in scene.get('procedural_lists', []) or []:
        for choice in proc.get('choices', []) or []:
            gameplay = str(choice.get('gameplay', '')).replace('\\', '/')
            if not gameplay or not folder_rel:
                continue
            tail = gameplay
            marker = tail.casefold().find('3d/modules/')
            if marker >= 0:
                tail = tail[marker + len('3d/modules/'):]
            if not tail.casefold().startswith(folder_rel.casefold() + '/'):
                tail = (folder_rel + '/' + tail if tail.casefold().startswith('mgp/')
                        else folder_rel + '/mgp/' + tail)
            path = _resolve_module_path('3d/modules/' + tail,
                                        module_root, modules_by_path)
            if path:
                result.setdefault(path, set()).add(
                    f"procedural choice {proc.get('name')}")
    return result


def _load_scene_objects(ctx, tables=None):
    """Index all exact MGP GameObject definitions referenced by each scene.

    The records identify authored source definitions, not runtime instances.
    MGP membership is also conditional for procedural choices.
    """
    scenes = read_json(Path(ctx.out) / 'data/levels.json', []) or []
    actors = read_json(Path(ctx.out) / 'data/actors.json', []) or []
    actor_by_name = _index(_unique_source_rows(actors))
    character_properties_by_name = _index(_unique_source_rows([row for row in tables or []
        if str(row.get('table', '')).startswith('character_properties#group0')]))
    template_by_name = _index(_unique_source_rows([row for row in tables or []
        if str(row.get('table', '')).startswith('character_templates#')]))
    module_root, modules_by_path = _module_path_index(ctx)
    result = defaultdict(lambda: defaultdict(list))
    for scene in scenes:
        if scene.get('record_type') != 'scene_file':
            continue
        scene_id = scene.get('id')
        module_files = _scene_module_files(ctx, scene, module_root, modules_by_path)
        for path, module_relations in module_files.items():
            try:
                root = ET.parse(path).getroot()
            except (ET.ParseError, OSError):
                continue
            module_source = '.local-inputs/' + path.relative_to(Path(ctx.inputs)).as_posix()
            for ordinal, obj in enumerate(root.iter('GameObject')):
                object_name = str(obj.get('name', '')).strip()
                if not object_name:
                    continue
                properties = dict(obj.attrib)
                gametype = properties.get('gametype', '')
                cp_name = properties.get('charpropsname', '')
                template_name = properties.get('char_template', '')
                cp_matches = (character_properties_by_name.get(_key(cp_name), []) or
                              actor_by_name.get(_key(cp_name), [])) if cp_name else []
                template_matches = template_by_name.get(_key(template_name), []) if template_name else []
                definition = {
                    'id': stable_id('campaign-scene-object-definition', scene_id,
                                    module_source, ordinal),
                    'type': 'scene_object_definition',
                    'name': object_name,
                    'scene_id': scene_id,
                    'module': module_source,
                    'source_file': module_source,
                    'object_ordinal': ordinal,
                    'gametype': gametype,
                    'properties': properties,
                    'module_relations': sorted(module_relations),
                    'module_membership': {
                        'status': ('SCENE_REFERENCED' if 'direct scene module reference' in module_relations
                                   else 'CONDITIONAL_PROCEDURAL_CHOICE'),
                        'direct_reference': 'direct scene module reference' in module_relations,
                        'procedural_choices': sorted(relation for relation in module_relations
                                                     if relation != 'direct scene module reference'),
                    },
                    'definition_evidence': 'Exact GameObject element parsed from an MGP referenced by this scene; this is an authored definition, not a runtime object instance.',
                }
                # char_template is an explicit alternate character source. Do
                # not reinterpret charpropsname as a CharacterProperties edge
                # on a template-based definition.
                if template_name:
                    definition['character_template'] = {
                        'name': template_name,
                        'status': 'RESOLVED' if len(template_matches) == 1 else 'UNRESOLVED',
                        'target_id': template_matches[0].get('id') if len(template_matches) == 1 else None,
                        'target_table': template_matches[0].get('table') if len(template_matches) == 1 else None,
                        'target_index': template_matches[0].get('index') if len(template_matches) == 1 else None,
                        'evidence': 'Explicit MGP char_template property; it overrides CharacterProperties name interpretation in this resolver.'}
                elif cp_name:
                    definition['character_properties'] = {
                        'name': cp_name,
                        'status': 'RESOLVED' if len(cp_matches) == 1 else 'AMBIGUOUS' if cp_matches else 'UNRESOLVED',
                        'target_id': cp_matches[0].get('id') if len(cp_matches) == 1 else None,
                        'target_table': cp_matches[0].get('table', 'character_properties#group0') if len(cp_matches) == 1 else None,
                        'target_index': cp_matches[0].get('index') if len(cp_matches) == 1 else None,
                        'evidence': 'Explicit MGP charpropsname property with no char_template override.'}
                object_name_key = _key(object_name)
                result[scene_id][object_name_key].append(definition)
    return result


def object_records(ctx, tables=None):
    """Return every named GameObject definition in admitted scene MGP sources.

    These are authored definitions, not claims about runtime instances. The
    returned records preserve the full source identity, properties, and direct
    versus conditional procedural membership assembled by
    :func:`_load_scene_objects`. Ordering is stable for graph and report use.
    """
    records = []
    for by_name in _load_scene_objects(ctx, tables).values():
        for definitions in by_name.values():
            records.extend(definitions)
    return sorted(records, key=lambda row: (
        str(row.get('scene_id') or '').casefold(),
        str(row.get('source_file') or '').casefold(),
        int(row.get('object_ordinal', -1)),
        str(row.get('name') or '').casefold(),
        str(row.get('id') or '')))


_CHARACTER_ROLES = {
    'character_object_instance', 'moving_character_object',
    'facing_character_object', 'visible_character_object',
    'killed_character_object', 'animated_character_object',
    'positioned_character_object', 'limbus_character_object',
    'idle_character_object', 'scripted_character_object',
    'stopped_character_object',
}


def _scene_object_target(scene_id, object_name, scene_objects,
                         role_name=None, reference=None):
    hits = scene_objects.get(scene_id, {}).get(_key(object_name), [])
    candidates = []
    for definition in hits:
        target = dict(definition)
        if reference:
            target['related_field_refs'] = [{'id': reference.get('id'),
                'role': role_name, 'field_offset': reference.get('field_offset'),
                'command': reference.get('command')}]
        candidates.append(target)
    return candidates


def global_records(ctx):
    """Return the fully decoded original global ScriptManager bank.

    Records use graph-compatible IDs ``campaign:global:<index>`` for scripts
    and ``command:global:<script-index>:<command-index>`` for commands, while
    retaining stable source IDs, exact name/payload ordinals, source files,
    source sites, decoded fields, and command counts. The global name and
    command streams must both decode with the recovered campaign schemas or
    the API returns an empty list. This is source inventory, not execution.
    """
    pydata = (Path(ctx.inputs) / 'assets-extra/android/data/pydata')
    names_path = pydata / 'scripts_pyscriptnames.bin'
    commands_path = pydata / 'scripts_pyscripts.bin'
    if not names_path.is_file() or not commands_path.is_file():
        return []
    try:
        # Reuse the campaign reader and its generated-data command schemas;
        # names and payload records retain their exact native ordinal pairing.
        from campaign import _command_names, _read_names, _read_scripts, _schema_actions
        loader_root = Path(__file__).resolve().parents[2] / 'port/level-loader'
        schemas = _schema_actions(loader_root / 'script_data_schemas_v52.inc')
        labels = _command_names(loader_root / 'command_c1_descriptors_v59.inc')
        names = _read_names(names_path)
        decoded = _read_scripts(commands_path, names, schemas, labels)
    except (OSError, ValueError, ImportError, struct.error):
        return []
    source = '.local-inputs/' + names_path.relative_to(Path(ctx.inputs)).as_posix()
    command_source = '.local-inputs/' + commands_path.relative_to(Path(ctx.inputs)).as_posix()
    records = []
    for i, name in enumerate(names):
        script = decoded[i]
        script_site = f'{source}:script[{i}]={name}'
        commands = []
        for command in script.get('commands', []):
            command_index = int(command['index'])
            command_site = (f'{command_source}:script[{i}]={name}/'
                            f"command[{command_index}]={command['name']}")
            commands.append({**command,
                'id': stable_id('campaign-global-command', command_source,
                                i, command_index),
                'graph_id': f'command:global:{i}:{command_index}',
                'payload_ordinal': command_index,
                'site': command_site,
                'source_file': command_source,
                'decode_status': 'DONE'})
        records.append({'id': stable_id('campaign-global-script', source, i),
            'graph_id': f'campaign:global:{i}',
            'type': 'global_campaign_script', 'name': name,
            'index': i, 'name_ordinal': i, 'payload_ordinal': i,
            'manager_script_index': i, 'command_count': script.get('command_count'),
            'commands': commands,
            'site': script_site, 'source_file': source,
            'source_names': source, 'source_commands': command_source,
            'status': 'DONE', 'work_status': 'DONE',
            'definition_evidence': 'Exact decoded global name/command pair, loaded before the configured level script bank.'})
    return records


def global_references(ctx, tables):
    """Resolve only source-proven operands in the original global script bank.

    The bank has no unique level scene context, so scene-object name operands
    retain their proven consumer role but are not joined to a level's MGP
    definitions. Runtime selector symbols and typed table/index operands can
    still be represented exactly. Returns ``(refs, relations)`` and never calls
    :func:`resolve` or its scene-script target loader.
    """
    table_by_index = defaultdict(lambda: defaultdict(list))
    for row in tables or []:
        table_by_index[str(row.get('table', ''))][str(row.get('index'))].append(row)

    refs = []
    relations = []
    for script in global_records(ctx):
        for command in script.get('commands') or []:
            try:
                command_kind = int(command.get('kind'))
                command_index = int(command.get('index'))
            except (TypeError, ValueError):
                continue
            for raw_offset, value in (command.get('fields') or {}).items():
                try:
                    offset = int(raw_offset)
                except (TypeError, ValueError):
                    continue
                numeric = _NUMERIC_ROLES.get((command_kind, offset))
                dialog = _DIALOG_OPERANDS.get((command_kind, offset))
                string = _STRING_ROLES.get((command_kind, offset))
                if not (numeric or dialog or string):
                    continue
                ref = {'id': stable_id('global-campaign-reference',
                                       script.get('index'), command_index, offset),
                       'site': command.get('site', script.get('site')) + f'/field@{offset}',
                       'name': str(value), 'value': value,
                       'kind': 'global_campaign_operand',
                       'rule': 'GLOBAL_CAMPAIGN_COMMAND_FIELD',
                       'scene_id': 'global', 'script_name': script.get('name'),
                       'script_index': script.get('index'),
                       'command_index': command_index,
                       'command_kind': command_kind,
                       'command': command.get('name'), 'field_offset': offset,
                       'source_file': command.get('source_file'),
                       'status': 'UNRESOLVED', 'work_status': 'DONE',
                       'reason_code': 'OTHER', 'reason': None,
                       'candidates': [], 'resolves_to': [], 'confidence': 'PROVEN'}
                if numeric:
                    role_name, table_name, evidence = numeric
                    try:
                        number = int(value)
                    except (TypeError, ValueError):
                        number = None
                    ref.update(reference_kind=role_name,
                               field_semantics={'status': 'PROVEN', 'role': role_name,
                                                'table': table_name, 'evidence': evidence},
                               candidate_note=evidence)
                    # The command consumer's UINT32_MAX chained-animation value
                    # is a sentinel, not a reference to a missing row.
                    if role_name == 'chained_animation_dictionary' and number == -1:
                        ref.update(status='RESOLVED', reason_code=None,
                                   field_semantics={**ref['field_semantics'],
                                                    'sentinel': 'NO_CHAIN'})
                    else:
                        rows = table_by_index.get(table_name, {}).get(str(number), []) if number is not None else []
                        unique = {row.get('id') or (row.get('table'), row.get('index')): row
                                  for row in rows}
                        targets = [{'id': row.get('id'), 'type': 'table-row',
                                    'table': table_name, 'index': number,
                                    'name': row.get('name')}
                                   for row in unique.values()]
                        ref['candidates'] = targets
                        ref['resolves_to'] = targets if len(targets) == 1 else []
                        if len(targets) == 1:
                            ref.update(status='RESOLVED', reason_code=None)
                            relations.append({'source_reference_id': ref['id'],
                                              'relation': role_name,
                                              'target': targets[0],
                                              'evidence': evidence})
                        elif len(targets) > 1:
                            ref['reason'] = 'Multiple source rows share this exact numeric index.'
                        else:
                            ref['reason'] = f'No {table_name} source row has numeric index {number}.'
                elif dialog:
                    role_name, evidence = dialog
                    ref.update(reference_kind=role_name,
                               field_semantics={'status': 'PROVEN', 'role': role_name,
                                                'evidence': evidence},
                               candidate_note=evidence,
                               reason='The native operand role is proven, but no source row with this ID is represented in the supplied Atlas tables.')
                else:
                    role_name, evidence = string
                    ref.update(reference_kind=role_name,
                               field_semantics={'status': 'PROVEN', 'role': role_name,
                                                'evidence': evidence},
                               candidate_note=evidence)
                    if _key(value) in {'localplayer', 'highestthreatplayer', 'player'}:
                        symbol = {'id': stable_id('campaign-runtime-symbol', _key(value)),
                                  'type': 'runtime_actor_symbol', 'name': str(value),
                                  'evidence': evidence}
                        ref['candidates'] = [symbol]
                        ref['resolves_to'] = [symbol]
                        ref.update(status='RESOLVED', reason_code=None, reason=None)
                        relations.append({'source_reference_id': ref['id'],
                                          'relation': role_name,
                                          'target': symbol, 'evidence': evidence})
                    else:
                        ref['reason'] = ('The global command operand has no unique level scene context; '
                                         'it is not joined to a scene-object definition.')
                refs.append(ref)
    return refs, relations


def _global_script_targets(ctx):
    """Index the global scripts loaded before each level's local bank."""
    return [{k: record[k] for k in ('id', 'type', 'name', 'index',
                'manager_script_index', 'command_count', 'source_file',
                'source_names', 'source_commands', 'definition_evidence')}
            for record in global_records(ctx)]


def _campaign_script_targets(scene_row, global_scripts):
    """Index decoded local scripts only when their source pair was admitted."""
    if (scene_row.get('status') != 'DONE' or
            not scene_row.get('source_names') or
            not scene_row.get('source_commands')):
        return []
    global_count = len(global_scripts)
    targets = []
    for script in scene_row.get('scripts', []) or []:
        if script.get('status') != 'DONE' or script.get('work_status') != 'DONE':
            continue
        try:
            index = int(script.get('index'))
        except (TypeError, ValueError):
            continue
        targets.append({'id': stable_id('campaign-level-script', scene_row.get('scene_id'),
                                        scene_row.get('source_names'), index),
            'type': 'campaign_script', 'name': script.get('name'),
            'scene_id': scene_row.get('scene_id'), 'scene': scene_row.get('scene'),
            'index': index,
            'manager_script_index': (global_count + index) if global_scripts else None,
            'source_names': scene_row.get('source_names'),
            'source_commands': scene_row.get('source_commands'),
            'definition_evidence': ('Exact decoded per-level script name/command pair; level loader appends this bank after the global scripts.'
                if global_scripts else 'Exact decoded per-level script name/command pair; the global script name bank is absent, so its manager-index offset is unknown.')})
    return targets


def _level_target(name, level_index):
    hits = level_index.get(_key(name), [])
    # Primary and supplemental Atlas table files can contain the same source
    # row. Collapse that duplicate by stable source-row identity; distinct row
    # identities with the same name remain genuinely ambiguous.
    unique = {}
    for row in hits:
        target = {'id': row.get('id'), 'type': 'levels', 'name': row.get('name'),
                  'table': row.get('table'), 'index': row.get('index')}
        unique[target['id'] or (target['table'], target['index'])] = target
    return list(unique.values())


def resolve(ctx, refs, tables, assets):
    """Update campaign string refs with source-proven field roles and joins.

    Returns ``(updated_refs, proven_relations)``. ``assets`` is accepted to
    keep the Atlas stage API stable; object instances are indexed from the
    parsed level/MGP records in ``ctx.out`` because file assets alone do not
    encode their contained GameObject names.
    """
    del assets  # explicitly not a name index: filenames are not object names
    refs = [dict(ref) for ref in (refs or [])]
    known_ids = {ref.get('id') for ref in refs}
    level_index = _index([r for r in (tables or [])
                          if str(r.get('table', '')).startswith('levels#group1')])
    table_by_index = defaultdict(lambda: defaultdict(list))
    for row in tables or []:
        table_by_index[str(row.get('table', ''))][str(row.get('index'))].append(row)
    scene_objects = _load_scene_objects(ctx, tables)
    relations = []
    condition_index = _index([r for r in (tables or [])
                              if str(r.get('table', '')).startswith('v2conditions#group0')])
    condition_evidence = ('ObjectBase::DeclareProperties binds activate_cond to ConditionData +144 '
        'and deactivate_cond to +180 (pseudocode-all.c:41550-41562); TestEnableCondition +140 '
        'and TestDisableCondition +176 evaluate those named condition lists (0x33e6d4 and '
        '0x33e61c; pseudocode-all.c:41138-41199).')
    # A named MGP condition resolves to the exact condition row. Its typed
    # quest/level/world-map payload remains the separate decoded table edge.
    for by_name in scene_objects.values():
        for definitions in by_name.values():
            for definition in definitions:
                for field_name, role_name in (
                        ('activate_cond', 'object_activation_condition'),
                        ('deactivate_cond', 'object_deactivation_condition')):
                    condition_name = str((definition.get('properties') or {}).get(field_name, '')).strip()
                    if not condition_name or condition_name.casefold() in {'invalid', 'none'}:
                        continue
                    definition.setdefault('condition_links', {})[field_name] = {
                        'name': condition_name, 'status': 'UNRESOLVED',
                        'evidence': condition_evidence}
                    matches = condition_index.get(_key(condition_name), [])
                    ref_id = stable_id('scene-object-condition-reference',
                                       definition.get('id'), field_name)
                    if ref_id not in known_ids:
                        candidates = [{'id': row.get('id'), 'type': 'table-row',
                                       'table': row.get('table'), 'index': row.get('index'),
                                       'name': row.get('name'), 'fields': row.get('fields')}
                                      for row in matches]
                        if len(candidates) == 1:
                            ref_status = 'RESOLVED'
                            reason = None
                            resolved = candidates
                        elif len(candidates) > 1:
                            ref_status = 'UNRESOLVED'
                            reason = 'Multiple exact v2conditions rows share this condition name.'
                            resolved = []
                        else:
                            ref_status = 'UNRESOLVED'
                            reason = 'The authored condition name has no exact v2conditions row in the supplied Atlas tables.'
                            resolved = []
                        refs.append({'id': ref_id,
                            'site': (f"{definition.get('source_file')}[GameObject#{definition.get('object_ordinal')}]"
                                     f"={definition.get('name')}.{field_name}"),
                            'name': condition_name, 'value': condition_name,
                            'kind': 'scene_object_condition',
                            'rule': 'SCENE_OBJECT_CONDITION',
                            'scene_id': definition.get('scene_id'),
                            'source_definition_id': definition.get('id'),
                            'source_file': definition.get('source_file'),
                            'object_ordinal': definition.get('object_ordinal'),
                            'property': field_name, 'reference_kind': role_name,
                            'field_semantics': {'status': 'PROVEN', 'role': role_name,
                                'table': 'v2conditions#group0', 'evidence': condition_evidence},
                            'candidate_note': condition_evidence, 'confidence': 'PROVEN',
                            'status': ref_status, 'work_status': 'DONE',
                            'reason_code': None if reason is None else 'OTHER',
                            'reason': reason, 'candidates': candidates,
                            'resolves_to': resolved})
                        known_ids.add(ref_id)
                    if len(matches) == 1:
                        target = {'id': matches[0].get('id'), 'type': 'table-row',
                                  'table': matches[0].get('table'),
                                  'index': matches[0].get('index'),
                                  'name': matches[0].get('name'),
                                  'fields': matches[0].get('fields')}
                        definition['condition_links'][field_name].update(
                            status='RESOLVED', target_id=target['id'], target=target)
                        relations.append({'source_definition_id': definition.get('id'),
                            'source_reference_id': ref_id,
                            'relation': role_name, 'field': field_name,
                            'target': target, 'evidence': condition_evidence})
                    elif len(matches) > 1:
                        definition['condition_links'][field_name]['status'] = 'AMBIGUOUS'
                    else:
                        definition['condition_links'][field_name]['reason'] = (
                            'The authored condition name has no exact v2conditions row in the supplied Atlas tables.')

    global_scripts = _global_script_targets(ctx)
    global_by_index = {row['index']: row for row in global_scripts}

    # Dialog operands are numeric and were not previously emitted as generic
    # references. Keep their proven operand roles visible while leaving the
    # localized text/style/actor rows unresolved unless an indexed source table
    # actually supplies that ID.
    campaign_rows = read_json(Path(ctx.out) / 'data/campaign.json', []) or []
    for scene_row in campaign_rows:
        local_script_targets = _campaign_script_targets(scene_row, global_scripts)
        local_by_index = {row['index']: row for row in local_script_targets}
        for script in scene_row.get('scripts', []) or []:
            for command in script.get('commands', []) or []:
                try:
                    kind_id = int(command.get('kind'))
                except (TypeError, ValueError):
                    continue
                if kind_id == 0 and command.get('decode_status') == 'DONE':
                    fields = command.get('fields') or {}
                    try:
                        absolute = bool(int(fields.get('8', 0)))
                    except (TypeError, ValueError):
                        absolute = False
                    try:
                        count = int(fields.get('16', 0))
                    except (TypeError, ValueError):
                        count = 0
                    raw_targets = (fields.get('20') or []) if count else [fields.get('12')]
                    if count and not isinstance(raw_targets, (list, tuple)):
                        raw_targets = []
                    source_evidence = (
                        'Script_ExecScript::Execute 0x4607f4 chooses one random field +20 array entry '
                        'when count +16 is nonzero, otherwise uses field +12; it adds dword_9A5FE4 '
                        'when byte +8 is false, then calls StartScript(..., a3, 1) '
                        '(pseudocode-all.c:248640-248674). Level::_LoadScripts loads the global bank '
                        'before the configured per-level bank, whose name/command pair is appended '
                        '(0x3f3a58 and 0x3f38a0, pseudocode-all.c:176890-176990).')
                    for array_index, raw_value in enumerate(raw_targets):
                        try:
                            target_index = int(raw_value)
                        except (TypeError, ValueError):
                            continue
                        if absolute:
                            target = global_by_index.get(target_index)
                            if target is None:
                                target = next((candidate for candidate in local_script_targets
                                    if candidate.get('manager_script_index') == target_index), None)
                        else:
                            target = local_by_index.get(target_index)
                        ref_id = stable_id('campaign-reference', scene_row.get('scene_id'),
                                           script.get('index'), command.get('index'),
                                           12 if not count else 20, array_index if count else '')
                        if ref_id in known_ids:
                            continue
                        target_candidates = [target] if target else []
                        local_admitted = bool(local_script_targets)
                        if target:
                            status, reason = 'RESOLVED', None
                        elif absolute and not global_scripts:
                            status, reason = 'UNRESOLVED', 'The global script name bank is not present or failed its bounded native decode.'
                        elif absolute:
                            status, reason = 'UNRESOLVED', f'No global or admitted scene-local script has absolute manager index {target_index}.'
                        elif not local_admitted:
                            status, reason = 'UNRESOLVED', 'The scene has no admitted DONE per-level script name/command pair for this local script index.'
                        else:
                            status, reason = 'UNRESOLVED', f'No admitted per-level script row has local index {target_index}.'
                        relation_role = 'absolute_script_target' if absolute else 'level_script_target'
                        refs.append({'id': ref_id,
                            'site': ((scene_row.get('source_commands') or 'campaign script') +
                                     f":script[{script.get('index')}]={script.get('name')}/command[{command.get('index')}]={command.get('name')}/field@{20 if count else 12}" +
                                     (f'[{array_index}]' if count else '')),
                            'name': str(raw_value), 'value': raw_value,
                            'kind': 'campaign_script_target_index',
                            'rule': 'CAMPAIGN_EXEC_SCRIPT_TARGET',
                            'scene_id': scene_row.get('scene_id'), 'act': scene_row.get('act'),
                            'script_name': script.get('name'), 'script_index': script.get('index'),
                            'command_index': command.get('index'), 'command_kind': 0,
                            'command': command.get('name'), 'field_offset': 20 if count else 12,
                            'array_index': array_index if count else None,
                            'target_namespace': 'manager_absolute' if absolute else 'scene_local',
                            'runtime_selection': 'random_one_from_array' if count else 'single_index',
                            'reference_kind': relation_role,
                            'field_semantics': {'status': 'PROVEN',
                                'role': relation_role, 'evidence': source_evidence},
                            'candidate_note': source_evidence, 'confidence': 'PROVEN',
                            'port_status': 'UNKNOWN', 'implementation': None,
                            'status': status, 'work_status': 'DONE',
                            'reason_code': None if status == 'RESOLVED' else 'OTHER',
                            'reason': reason, 'resolves_to': target_candidates,
                            'candidates': target_candidates})
                        known_ids.add(ref_id)
                        if target:
                            relations.append({'source_reference_id': ref_id,
                                'relation': relation_role, 'target': target,
                                'runtime_selection': 'random_one_from_array' if count else 'single_index',
                                'evidence': source_evidence})
                for offset, value in (command.get('fields') or {}).items():
                    try:
                        offset = int(offset)
                    except (TypeError, ValueError):
                        continue
                    role = _DIALOG_OPERANDS.get((kind_id, offset))
                    if not role:
                        continue
                    ref_id = stable_id('campaign-reference', scene_row.get('scene_id'),
                                       script.get('index'), command.get('index'), offset)
                    if ref_id in known_ids:
                        continue
                    role_name, evidence = role
                    refs.append({'id': ref_id,
                        'site': ((scene_row.get('source_commands') or 'campaign script') +
                                 f":script[{script.get('index')}]={script.get('name')}/command[{command.get('index')}]={command.get('name')}/field@{offset}"),
                        'name': str(value), 'value': value, 'kind': role_name,
                        'rule': 'CAMPAIGN_TYPED_DIALOG_OPERAND',
                        'scene_id': scene_row.get('scene_id'), 'act': scene_row.get('act'),
                        'script_name': script.get('name'), 'script_index': script.get('index'),
                        'command_index': command.get('index'), 'command_kind': kind_id,
                        'command': command.get('name'), 'field_offset': offset,
                        'reference_kind': role_name, 'resolves_to': [], 'candidates': [],
                        'field_semantics': {'status': 'PROVEN', 'role': role_name,
                                            'evidence': evidence},
                        'candidate_note': evidence, 'confidence': 'PROVEN',
                        'port_status': 'UNKNOWN', 'implementation': None,
                        'status': 'UNRESOLVED', 'work_status': 'DONE',
                        'reason_code': 'OTHER',
                        'reason': 'The native operand role is proven, but the supplied Atlas tables do not expose the corresponding dialog message, localized text, style, or actor-ID source row.'})
                    known_ids.add(ref_id)

    for ref in refs:
        command_kind = ref.get('command_kind')
        try:
            command_kind = int(command_kind)
        except (TypeError, ValueError):
            continue
        try:
            offset = int(ref.get('field_offset'))
        except (TypeError, ValueError):
            continue

        numeric_role = _NUMERIC_ROLES.get((command_kind, offset))
        if numeric_role:
            role_name, table_name, evidence = numeric_role
            try:
                number = int(ref.get('value'))
            except (TypeError, ValueError):
                number = None
            # The native actor-animation Init explicitly treats UINT32_MAX as
            # a no-chain value. It is not a missing dictionary reference.
            if role_name == 'chained_animation_dictionary' and number == -1:
                ref['field_semantics'] = {'status': 'PROVEN', 'role': role_name,
                                          'sentinel': 'NO_CHAIN', 'evidence': evidence}
                ref['candidate_note'] = evidence
                ref['confidence'] = 'PROVEN'
                continue
            rows = table_by_index.get(table_name, {}).get(str(number), []) if number is not None else []
            unique = {row.get('id'): row for row in rows}
            targets = [{'id': row.get('id'), 'type': 'table-row', 'table': table_name,
                        'index': number, 'name': row.get('name')}
                       for row in unique.values()]
            ref['reference_kind'] = role_name
            ref['field_semantics'] = {'status': 'PROVEN', 'role': role_name,
                                      'table': table_name, 'evidence': evidence}
            ref['candidate_note'] = evidence
            ref['confidence'] = 'PROVEN'
            ref['candidates'] = targets
            ref['resolves_to'] = targets if len(targets) == 1 else []
            if len(targets) == 1:
                ref.update(status='RESOLVED', reason=None, reason_code=None)
                relations.append({'source_reference_id': ref.get('id'),
                                  'relation': role_name, 'target': targets[0],
                                  'evidence': evidence})
            elif len(targets) > 1:
                ref.update(status='UNRESOLVED', reason_code='OTHER',
                           reason='Multiple source rows share this exact numeric index.')
            else:
                ref.update(status='UNRESOLVED', reason_code='OTHER',
                           reason=f'No {table_name} source row has numeric index {number}.')
            continue

        dialog_role = _DIALOG_OPERANDS.get((command_kind, offset))
        if dialog_role:
            role_name, evidence = dialog_role
            ref.update(reference_kind=role_name,
                       field_semantics={'status': 'PROVEN', 'role': role_name,
                                        'evidence': evidence},
                       candidate_note=evidence, confidence='PROVEN',
                       status='UNRESOLVED', reason_code='OTHER',
                       reason='The native operand role is proven, but no source row with this ID is represented in the supplied Atlas tables.')
            continue

        role = _STRING_ROLES.get((command_kind, offset))
        if not role:
            continue

        role_name, evidence = role
        ref['reference_kind'] = role_name
        ref['field_semantics'] = {'status': 'PROVEN', 'role': role_name,
                                  'evidence': evidence}
        ref['candidate_note'] = evidence
        ref['confidence'] = 'PROVEN'

        if role_name == 'destination_level':
            targets = _level_target(ref.get('value', ref.get('name')), level_index)
            ref['candidates'] = targets
            ref['resolves_to'] = targets if len(targets) == 1 else []
            if len(targets) == 1:
                ref.update(status='RESOLVED', reason=None, reason_code=None)
                relations.append({'source_reference_id': ref.get('id'),
                                  'relation': role_name, 'target': targets[0],
                                  'evidence': evidence})
            else:
                ref.update(status='UNRESOLVED', reason_code='OTHER',
                    reason=('No exact level-list name match was present in the supplied Atlas tables.'
                            if not targets else 'Multiple exact level-list rows match; target remains ambiguous.'))
            continue

        # Player/actor selectors are runtime symbols and are not level MGP
        # object instances. Keep them explicit without fabricating an MGP row.
        value = str(ref.get('value', ref.get('name', '')))
        if _key(value) in {'localplayer', 'highestthreatplayer', 'player'}:
            symbol = {'id': stable_id('campaign-runtime-symbol', _key(value)),
                      'type': 'runtime_actor_symbol', 'name': value,
                      'evidence': evidence}
            ref['candidates'] = [symbol]
            ref['resolves_to'] = [symbol]
            ref.update(status='RESOLVED', reason=None, reason_code=None)
            relations.append({'source_reference_id': ref.get('id'),
                              'relation': role_name, 'target': symbol,
                              'evidence': evidence})
            continue

        definitions = _scene_object_target(ref.get('scene_id'), value,
                                            scene_objects, role_name, ref)
        targets = ([target for target in definitions
                    if _key(target.get('gametype')) == 'character']
                   if role_name in _CHARACTER_ROLES else definitions)
        ref['candidates'] = definitions
        ref['resolves_to'] = targets if len(targets) == 1 else []
        if len(definitions) == 1 and len(targets) == 1:
            ref.update(status='RESOLVED', reason=None, reason_code=None)
            relations.append({'source_reference_id': ref.get('id'),
                              'relation': role_name, 'target': targets[0],
                              'evidence': evidence})
            cp_link = (targets[0].get('character_properties') or {})
            if cp_link.get('target_id'):
                relations.append({'source_definition_id': targets[0].get('id'),
                                  'relation': 'character_properties',
                                  'target': {'id': cp_link['target_id'],
                                             'type': 'table-row',
                                             'table': cp_link.get('target_table'),
                                             'index': cp_link.get('target_index'),
                                             'name': cp_link.get('name')},
                                  'evidence': cp_link.get('evidence')})
            template_link = targets[0].get('character_template') or {}
            if template_link.get('target_id'):
                relations.append({'source_definition_id': targets[0].get('id'),
                                  'relation': 'character_template',
                                  'target': {'id': template_link['target_id'],
                                             'type': 'table-row',
                                             'table': template_link.get('target_table'),
                                             'index': template_link.get('target_index'),
                                             'name': template_link.get('name')},
                                  'evidence': template_link.get('evidence')})
        elif len(definitions) > 1:
            ref.update(status='UNRESOLVED', reason_code='OTHER',
                       reason='Several exact GameObject definitions in this scene share the target name; runtime name lookup cannot be narrowed to one source definition.')
        elif definitions:
            ref.update(status='UNRESOLVED', reason_code='OTHER',
                       reason='Exact same-scene GameObject definitions exist, but none has a gametype admitted by this command consumer.')
        else:
            ref.update(status='UNRESOLVED', reason_code='OTHER',
                       reason='The source consumer role is proven, but no exact GameObject definition was found in the scene-referenced MGP files. This does not prove the original runtime lookup fails.')

    return refs, relations
