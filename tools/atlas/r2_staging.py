"""Round-2 staging-gap ledger and source-site scope enumerator.

The original atlas files and all source corpora are read-only.  Scope is kept
as small JSON shards so the tracked output stays well below GitHub's file cap.
Classification is intentionally separate from enumeration: a missing or
dynamic reference stays unresolved until its source rule is proven.
"""
from collections import Counter, defaultdict, deque
import hashlib
import io
import json
import os
from pathlib import Path
import re
import stat
import ast
import sys
import subprocess
import tarfile
import xml.etree.ElementTree as ET

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
ACT2_SCOPE_PATH = Path('r2/act2/scope-current.json')
ACT2_FROZEN_SCOPE_PATH = Path('r2/act2/scope.json')
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


def _fold_path(value):
    return str(value or '').replace('\\', '/').strip('/').casefold()


def _port_uri_candidates(uri, owner=''):
    """Mirror content_path_candidates in content_paths.cpp for ledger evidence."""
    value = str(uri or '').replace('\\', '/')
    if len(value) > 8 and re.match(r'^[A-Za-z]:/data/', value, re.I):
        value = value[3:]
    if value.startswith('/') or ':' in value or '?' in value:
        return []
    value = value.split('#', 1)[0]
    parts = []
    for part in value.split('/'):
        if part in ('', '.'):
            continue
        if part == '..':
            return []
        if part.casefold() in {'iphone', 'ps3', 'debug', 'old'}:
            continue
        parts.append(part)
    if not parts:
        return []
    requested = Path(*parts)
    variants = [requested]
    ext = requested.suffix.casefold()
    if ext in {'.dae', '.xml'}:
        variants.append(requested.with_suffix('.bdae'))
    ns = requested.as_posix().casefold()
    if ns.startswith('original-cache/'):
        ns = ns[len('original-cache/'):]
    if ext == '.tga' and (ns.startswith('data/3d/textures/') or ns.startswith('textures/')) and not requested.name.casefold().startswith('pvr2_'):
        variants.append(requested.parent / ('pvr2_' + requested.name))
    out = []
    def add(path):
        text = Path(os.path.normpath(str(path))).as_posix()
        if text not in out:
            out.append(text)
    owner_path = Path(owner) if owner else Path()
    owner_parent = owner_path.parent if owner else None
    for variant in variants:
        add(variant)
        if owner_parent:
            add(owner_parent / variant)
        raw = variant
        if raw.as_posix().casefold().startswith('original-cache/'):
            raw = Path(raw.as_posix()[len('original-cache/'):])
        add(raw)
        add(Path('original-cache') / raw)
        if raw.parent.as_posix().casefold() == 'data/pydata' and raw.suffix.casefold() == '.bin':
            add(Path('data') / raw.name)
        if not raw.as_posix().casefold().startswith('data/'):
            for folder in ('data', 'data/scene', 'data/3d/modules'):
                add(Path(folder) / raw)
                add(Path('original-cache') / folder / raw)
        name, ext = raw.name, raw.suffix.casefold()
        if ext in {'.tga', '.png', '.pvr', '.btex'}:
            for folder in ('data/3d/textures', 'textures'):
                add(Path(folder) / name)
                add(Path('original-cache') / folder / name)
        elif ext in {'.bdae', '.dae'}:
            for folder in ('data/3d/actors', 'data/3d/models', 'models', 'actors', 'animations', 'worlds'):
                add(Path(folder) / name)
                add(Path('original-cache') / folder / name)
        elif ext in {'.dwld', '.dact', '.xml', '.mlx'}:
            add(Path('worlds') / name)
    return out


def _walk_values(value):
    if isinstance(value, dict):
        yield value
        for child in value.values():
            yield from _walk_values(child)
    elif isinstance(value, list):
        for child in value:
            yield from _walk_values(child)


def _site_uris(row):
    """Extract only authored or atlas-resolved URIs, never name-similarity guesses."""
    out = []
    def add(value, provenance):
        if isinstance(value, str) and value.strip() and (value.startswith(('data/', 'q:/', 'Q:/')) or '.local-inputs/' in value.replace('\\', '/') or Path(value).suffix.casefold() in {'.bdae', '.dae', '.xml', '.mlx', '.wav', '.ogg', '.luac', '.pyscript', '.bin', '.png', '.tga', '.pvr', '.btex'}):
            pair = (value.strip(), provenance)
            if pair not in out:
                out.append(pair)
    for item in _walk_values(row.get('resolves_to') or []):
        if item.get('type') == 'file':
            add(item.get('path'), 'resolved file URI')
        add(item.get('logical_uri'), 'resolved logical URI')
        for key in ('path',):
            val = item.get(key)
            if isinstance(val, str) and val.startswith('.local-inputs/'):
                add(val, 'exact source provenance')
    add(row.get('name'), 'authored name')
    for candidate in row.get('candidates') or []:
        add(candidate, 'atlas candidate')
    return out


def _source_catalog(ctx):
    """Index only source roots that can be read by the v21.8 stage script."""
    specs = [
        ('assets-extra/android/data', 'assets/data', 'android'),
        ('assets-extra/ios/data', 'assets/data', 'ios'),
        ('assets-extra/android/original-cache/data', 'assets/original-cache/data', 'original-cache'),
        ('assets-extra/android/3d', 'assets/data/3d', 'android-extra'),
        ('audio-v34/cache', 'audio-assets/data/sounds', 'audio-cache'),
        ('assets-extra/ios/data/sounds', 'audio-assets/data/sounds', 'ios-audio'),
    ]
    roots = []
    by_key = {}
    for source_root, destination_root, label in specs:
        physical = ctx.inputs / source_root
        if not physical.is_dir():
            continue
        entries = defaultdict(list)
        basenames = defaultdict(list)
        for directory, dirs, files in os.walk(physical):
            dirs.sort(key=str.casefold)
            for filename in sorted(files, key=str.casefold):
                path = Path(directory) / filename
                if path.is_symlink():
                    continue
                try:
                    relative = path.relative_to(physical).as_posix()
                except ValueError:
                    continue
                entries[_fold_path(relative)].append(path)
                basenames[path.name.casefold()].append((relative, path))
        spec = {'source_root': source_root, 'destination_root': destination_root,
                'label': label, 'physical': physical, 'entries': entries, 'basenames': basenames}
        roots.append(spec)
        by_key[source_root.casefold()] = spec
    return roots, by_key


def _manifest_catalog(ctx):
    base = ctx.inputs / 'windows-source-clock-v21.8-staged2'
    manifest_path = base / 'manifest.json'
    if not manifest_path.is_file():
        raise RuntimeError(f'Frozen staged2 manifest missing: {manifest_path}')
    manifest = read_json(manifest_path, {}) or {}
    files = {}
    for entry in manifest.get('files') or []:
        key = _fold_path(entry.get('path'))
        files.setdefault(key, []).append(entry)
    return base, manifest, files


def _ensure_current_act2_scope(ctx):
    current_path = ctx.out / ACT2_SCOPE_PATH
    if current_path.is_file():
        return current_path
    # Scratch regeneration orders C's enumeration before B but C writes its
    # current snapshot in run(); calculate that same source closure here.
    try:
        import r2_act2
        current = r2_act2.enumerate_scope(ctx)
    except Exception:
        frozen = ctx.out / ACT2_FROZEN_SCOPE_PATH
        if frozen.is_file():
            return frozen
        raise
    write_json(current_path, current)
    return current_path


def _direct_source_record(ctx, value, source_roots):
    raw = str(value or '').replace('\\', '/')
    if not raw.startswith('.local-inputs/'):
        return []
    relative = raw[len('.local-inputs/'):]
    matches = []
    for spec in source_roots:
        prefix = spec['source_root'].replace('\\', '/') + '/'
        if relative.casefold().startswith(prefix.casefold()):
            rel = relative[len(prefix):]
            for path in spec['entries'].get(_fold_path(rel), []):
                actual_rel = path.relative_to(spec['physical']).as_posix()
                dest = spec['destination_root'].rstrip('/') + '/' + actual_rel
                destination_root = spec['destination_root']
                if spec['label'] == 'ios' and actual_rel.casefold().startswith('sounds/'):
                    dest = 'audio-assets/data/' + actual_rel
                    destination_root = 'audio-assets/data'
                matches.append({'source_root': spec['source_root'], 'relative': actual_rel,
                                'destination': dest, 'destination_root': destination_root, 'physical': path,
                                'label': spec['label'], 'provenance': 'exact atlas source path'})
    return matches


def _candidate_source_records(candidate, source_roots, is_sound=False):
    value = candidate.replace('\\', '/').lstrip('/')
    cf = value.casefold()
    records = []
    # A sound URI is consumed by AudioFilesystem at data/sounds/<filename>;
    # staged2's separate audio catalog is rooted at audio-assets.
    if is_sound and cf.startswith('data/sounds/'):
        name = value[len('data/sounds/'):]
        for spec in source_roots:
            if spec['label'] == 'audio-cache':
                rel = name
                for path in spec['entries'].get(_fold_path(rel), []):
                    actual_rel = path.relative_to(spec['physical']).as_posix()
                    if actual_rel != rel:
                        continue # AudioFilesystem lookup is case-sensitive.
                    records.append({'source_root': spec['source_root'], 'relative': actual_rel,
                                    'destination': 'audio-assets/data/sounds/' + actual_rel,
                                    'destination_root': 'audio-assets/data/sounds',
                                    'physical': path, 'label': spec['label'],
                                    'provenance': 'AudioFilesystem exact data/sounds filename'})
            elif spec['label'] == 'ios-audio':
                rel = name
                for path in spec['entries'].get(_fold_path(rel), []):
                    actual_rel = path.relative_to(spec['physical']).as_posix()
                    if actual_rel != rel:
                        continue
                    records.append({'source_root': spec['source_root'], 'relative': actual_rel,
                                    'destination': 'audio-assets/data/sounds/' + actual_rel,
                                    'destination_root': 'audio-assets/data/sounds',
                                    'physical': path, 'label': spec['label'],
                                    'provenance': 'exact iOS data/sounds filename'})
        return records
    if cf.startswith('original-cache/data/'):
        rel = value[len('original-cache/data/'):]
        wanted = {'original-cache'}
    elif cf.startswith('data/'):
        rel = value[len('data/'):]
        wanted = {'android', 'ios'}
        if rel.casefold().startswith('3d/'):
            wanted.add('android-extra')
    elif cf.startswith('original-cache/'):
        rel = value[len('original-cache/'):]
        wanted = {'original-cache'}
    else:
        rel = value
        wanted = {'android', 'ios'}
    for spec in source_roots:
        if spec['label'] not in wanted:
            continue
        root_rel = rel
        if spec['label'] == 'android-extra' and root_rel.casefold().startswith('3d/'):
            root_rel = root_rel[len('3d/'):]
        for path in spec['entries'].get(_fold_path(root_rel), []):
            actual_rel = path.relative_to(spec['physical']).as_posix()
            dest = spec['destination_root'].rstrip('/') + '/' + actual_rel
            records.append({'source_root': spec['source_root'], 'relative': actual_rel,
                            'destination': dest, 'destination_root': spec['destination_root'], 'physical': path,
                            'label': spec['label'], 'provenance': 'port path candidate ' + value})
    # Keep deterministic order. Caller applies content_path_candidates order.
    return records


def _sha(path):
    digest = hashlib.sha256()
    with Path(path).open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(block)
    return digest.hexdigest().upper()


def _typed_nonresource_reason(row):
    rule = str(row.get('rule') or '').casefold()
    kind = str(row.get('kind') or '').casefold()
    reference_kind = str(row.get('reference_kind') or '').casefold()
    if any(token in rule for token in ('not a file', 'not a resource', 'not an actor reference', 'not file-valued')):
        return str(row.get('rule'))
    typed_table_targets = {'table-row', 'source-table-cell', 'table', 'enum'}
    if row.get('resolves_to') and all(isinstance(x, dict) and x.get('type') in typed_table_targets for x in row.get('resolves_to') or []):
        if kind in {'anim', 'effect set', 'effect_set', 'animation_dictionary', 'animated effect', 'faction', 'class', 'loot', 'projectile', 'skill', 'level_name_candidate', 'actor_name_candidate', 'campaign_script_target_index', 'scene_object_condition', 'source_sound_ordinal', 'named_reference', 'numeric_reference', 'symbolic_reference', 'character', 'model', 'sound', 'script', 'data file'}:
            return ('This source site is a typed table key/index and selects structured data; it does not select an additional standalone file for staging. '
                    'The target table row’s file/media fields remain separate scope entries. This is a stage-only disposition, not a claim that the runtime reference is globally resolved.')
    if kind in {'animation event', 'campaign_script_target_index', 'level_name_candidate', 'scene_object_condition', 'source_sound_ordinal'}:
        return f'{row.get("rule") or kind} identifies a callback, transition, condition, or table index; it is not an independent file path.'
    if kind == 'character' and (reference_kind in {'table_row', 'object_template_selector'} or any(token in rule for token in ('charpropsname', 'char_template', 'charatertemplates', 'charactertable'))):
        return 'Authored character name selects a CharacterTable/CharacterTemplateTable row; resource file fields are enumerated from the joined profile separately.'
    if kind == 'data file' and any(token in rule for token in ('selector', 'table', 'skilltable')):
        return 'Typed PyData table selector or table name; this site selects structured data rather than an independent media file.'
    if kind == 'model component' and 'named modular node/icon' in rule:
        return None  # ItemTable ModularModule is consumed by native modular-gear logic; downstream file/node resolution is not established.
    if kind == 'named_reference' and any(token in rule for token in ('duplicates the exact getpystruct', 'duplicates the exact getpycst', 'already represented by its exact')):
        return str(row.get('rule'))
    if kind == 'symbolic_reference' and any(token in rule for token in ('getpystruct exact', 'getpycst resolves', 'getpyoid exact', 'registerskill positional', 'registerskillex argument', 'constant-flow trace and')):
        return 'Exact native namespace/member, constant, table-row, or callback schema key; it is not a standalone file reference.'
    if kind in {'anim', 'effect set', 'effect_set', 'animated effect', 'sound'} and any(token in rule for token in ('animstep field schema', 'field animtable', 'field sounds', 'field swooshsoundfx', 'projectiletable compiled member order', 'chareffecttable field')):
        return 'Typed table index or member reference; the referenced table’s file fields are separate reference sites.'
    if kind == 'script' and (rule == 'xml_attribute' or rule.endswith('field ai')):
        name = str(row.get('name') or '')
        if Path(name.replace('\\', '/')).suffix.casefold() in {'.pyscript', '.luac'}:
            return None
        # A non-path token may be a symbol in a bank, but absent an exact
        # source-to-bank relation it remains unresolved rather than being
        # cleared merely because some other script bank is in scope.
        return None
    if kind == 'script' and 'field spellscript' in rule:
        return 'Faery SpellScript value is a script-bank symbol, not a standalone source file; bank bytes and exact script references are enumerated separately.'
    if kind == 'script' and reference_kind == 'script_file' and 'ai.script stem' in rule:
        return 'AI script stem is a typed script-bank name; the resolved .luac source and its resource references are enumerated at their file sites.'
    if kind == 'campaign_script_target_index' and reference_kind in {'absolute_script_target', 'level_script_target'}:
        return 'Campaign target index is a script-bank command index, not an independent file path.'
    if kind == 'animation event' and reference_kind == 'animation_event':
        return 'Animation event is a callback/token consumed by the engine event dispatcher, not a separate resource file.'
    if kind == 'scene_object_condition' and reference_kind in {'object_activation_condition', 'object_deactivation_condition'}:
        return 'Scene activation condition is a state/quest predicate name, not a file resource.'
    if kind == 'level_name_candidate' and reference_kind == 'destination_level':
        return 'Destination level name is a typed scene-table key; the authored level file is tracked through the transition relation.'
    if kind == 'actor_name_candidate' and reference_kind in {'visible_character_object', 'facing_character_object', 'positioned_character_object', 'character_object_instance', 'animated_character_object', 'camera_target_object', 'position_waypoint_object', 'idle_character_object', 'killed_character_object', 'stopped_character_object', 'moving_character_object', 'look_target_object_or_waypoint'}:
        return 'Campaign actor/object name is a scene object key; its character profile and resource fields are tracked separately.'
    if kind == 'serialized_string' and (reference_kind.endswith(('_object', '_waypoint')) or reference_kind in {'look_target_object_or_waypoint', 'animated_scene_object', 'character_object_instance', 'effect_anchor_object', 'position_waypoint_object'}):
        return 'Campaign object/waypoint name is a typed scene object key, not a resource URI; resource fields are tracked separately.'
    if kind in {'effect set', 'effect_set', 'animated effect', 'anim', 'sound', 'model'} and not any(Path(uri).suffix.casefold() in {'.bdae', '.dae', '.xml', '.mlx', '.wav', '.ogg', '.luac', '.pyscript', '.bin', '.png', '.tga', '.pvr', '.btex'} or uri.startswith(('data/', 'q:/')) for uri, _ in _site_uris(row)):
        if row.get('resolves_to') and any(isinstance(x, dict) and x.get('type') in typed_table_targets for x in row.get('resolves_to') or []):
            return 'Structured table-row lookup; the site is a key/index and does not name a standalone media file.'
    if kind in {'dialog_style_id', 'dialog_text_string_id'}:
        return 'Typed dialog/style string identifier is a lookup key; no independent media file path is encoded at this site.'
    return None


def _classify_site(ctx, row, source_roots, manifest_files):
    base_class = {'reference_id': row['reference_id'], 'act': row['act'], 'kind': row.get('kind'),
                  'site': row.get('site'), 'confidence': row.get('confidence', 'GUESS'),
                  'name': row.get('name'), 'reference_kind': row.get('reference_kind'),
                  'rule': row.get('rule'), 'reason_code': row.get('reason_code'),
                  'scope_reasons': row.get('scope_reasons', [])}
    if row.get('scope_retired'):
        return {**base_class, 'classification': 'NOT_NEEDED',
                'reason': 'This reference was in immutable Step 0 scope but is no longer reachable in the current source-evidenced C closure; retained to preserve the original ledger.',
                'evidence': {'retained_from_step0': True, 'current_scope': ACT2_SCOPE_PATH.as_posix(),
                             'source_status': row.get('status'), 'source_reason': row.get('reason')},
                'port_candidates': [], 'source_candidates': [], 'selection_files': []}
    nonresource = _typed_nonresource_reason(row)
    if nonresource:
        return {**base_class, 'classification': 'NOT_NEEDED', 'reason': nonresource,
                'evidence': {'rule': row.get('rule'), 'resolves_to': row.get('resolves_to')},
                'port_candidates': [], 'source_candidates': [], 'selection_files': []}

    value = str(row.get('name') or '').replace('\\', '/')
    value_suffix = Path(value).suffix.casefold()
    rule_fold = str(row.get('rule') or '').casefold()
    site_fold = str(row.get('site') or '').casefold()
    if str(row.get('kind') or '').casefold() == 'script' and value_suffix == '.pyscript' and rule_fold == 'xml_attribute':
        return {**base_class, 'classification': 'UNRESOLVED',
                'reason': 'Scene scriptFile names a standalone .pyscript target that is absent from the extracted corpora; script commands also exist in PyData banks, but no exact source-to-bank or executed-file rule proves that this missing standalone path is genuinely absent runtime content.',
                'evidence': {'rule': row.get('rule'), 'name': row.get('name'),
                             'limitation': 'Packed and compiled PyData banks are not materialized as standalone .pyscript files by the stage script.'},
                'port_candidates': [], 'source_candidates': [], 'selection_files': []}

    if str(row.get('kind') or '').casefold() == 'model component' and 'named modular node/icon' in rule_fold:
        return {**base_class, 'classification': 'UNRESOLVED',
                'reason': 'ItemTable ModularModule is consumed by native modular-equipment logic, but the controller/category-to-asset rule is incomplete: VisualObject::GetModularModuleId (IDA address 0x474568) probes a #<module>-mesh-skin token in a requested category, while the existing audit does not decode the category descriptor. No exact staged path is established; do not infer a file from a matching basename.',
                'evidence': {'rule': row.get('rule'), 'name': row.get('name'),
                             'original_consumer': 'IDA VisualObject::GetModularModuleId 0x474568; coordination/atlas/data/item-resource-links.json field ModularModule audit',
                             'known_limit': 'Controller token presence in character model BRES is not an exact mapping from this item row to a stageable standalone file.'},
                'port_candidates': [], 'source_candidates': [], 'selection_files': []}

    if (str(row.get('kind') or '').casefold() == 'script' and rule_fold == 'xml_attribute' and
            value_suffix not in {'.pyscript', '.luac'}):
        return {**base_class, 'classification': 'UNRESOLVED',
                'reason': 'This MGP/XML script attribute names a script symbol, but no exact source-bank row or loaded-file relation was proven for this object site. Other campaign/script-bank rows in scope do not establish that this name resolves here.',
                'evidence': {'rule': row.get('rule'), 'name': row.get('name'),
                             'source_site': row.get('site'),
                             'attempted_linkage': 'exact source owner + script symbol to bank/profile mapping; no exact joined target is present on this indexed row'},
                'port_candidates': [], 'source_candidates': [], 'selection_files': []}

    is_sound = str(row.get('kind')).casefold() in {'sound', 'source_sound_ordinal'}
    uri_pairs = _site_uris(row)
    owner = str((row.get('context') or {}).get('file') or '')
    owner_norm = owner.replace('\\', '/')
    if '/data/' in owner_norm.casefold():
        owner = 'data/' + owner_norm.casefold().split('/data/', 1)[1]
    else:
        owner = ''
    candidates = []
    source_records = []
    staged_hits = []
    for uri, provenance in uri_pairs:
        direct = _direct_source_record(ctx, uri, source_roots)
        for record in direct:
            source_records.append(record)
            if _fold_path(record['destination']) in manifest_files:
                staged_hits.append({'uri': uri, 'candidate': record['destination'], 'provenance': provenance})
        if uri.startswith('.local-inputs/'):
            # Exact source provenance is sufficient to inspect bytes, but port
            # destinations still come from the documented content/audio rule.
            continue
        port = _port_uri_candidates(uri, owner)
        for candidate_index, candidate in enumerate(port):
            if candidate not in candidates:
                candidates.append(candidate)
            if is_sound and candidate.casefold().startswith('data/sounds/'):
                dests = ['audio-assets/' + candidate]
            else:
                dests = ['assets/' + candidate]
            for dest in dests:
                if _fold_path(dest) in manifest_files:
                    staged_hits.append({'uri': uri, 'candidate': dest, 'provenance': provenance})
            found = _candidate_source_records(candidate, source_roots, is_sound=is_sound)
            for record in found:
                record['candidate_order'] = candidate_index
            source_records.extend(found)

    rule = str(row.get('rule') or '')
    confidence = str(row.get('confidence') or 'GUESS').upper()
    exact_rule = bool(row.get('implementation') or row.get('field_semantics', {}).get('status') == 'PROVEN' or confidence == 'PROVEN' or
                      'bdae material texture basename' in rule.casefold() or
                      ('rule' in rule.casefold() and 'resolver' in rule.casefold()) or
                      (str(row.get('kind')).casefold() == 'sound' and rule.casefold() == 'bres_event_sfx_prefix' and row.get('reference_kind') == 'named_animation_sound'))
    explicit_file_uri = any(source in {'resolved file URI', 'resolved logical URI', 'exact source provenance'}
                            for _, source in uri_pairs)
    file_role_proven = exact_rule or (row.get('status') == 'RESOLVED' and confidence == 'PROVEN' and explicit_file_uri)
    binary_string_unknown = ('binary_file_string' in rule.casefold() or
                             'requires schema evidence' in str(row.get('reason') or '').casefold())
    if binary_string_unknown:
        return {**base_class, 'classification': 'UNRESOLVED',
                'reason': 'Serialized/file-like text is present, but its consumer role is not proven; matching files are diagnostic candidates only.',
                'evidence': {'rule': rule or None, 'original_reason': row.get('reason'),
                             'staged_matches_considered_only': staged_hits[:8],
                             'source_bytes_considered_only': [{'source_root': r['source_root'], 'relative': r['relative']} for r in source_records[:8]]},
                'port_candidates': candidates[:32], 'source_candidates': [], 'selection_files': []}
    if (staged_hits or source_records) and not file_role_proven:
        return {**base_class, 'classification': 'UNRESOLVED',
                'reason': 'A path-like value has candidate files, but no proven consumer rule ties this site to their bytes; no alias was inferred.',
                'evidence': {'rule': rule or None, 'candidates_only': True,
                             'staged_matches': staged_hits[:8],
                             'original_matches': [{'source_root': r['source_root'], 'relative': r['relative']} for r in source_records[:8]]},
                'port_candidates': candidates[:32], 'source_candidates': [], 'selection_files': []}

    # Deduplicate source records by exact source-root/relative/destination tuple.
    unique = {}
    for rec in source_records:
        unique.setdefault((rec['source_root'].casefold(), _fold_path(rec['relative']), _fold_path(rec['destination'])), rec)
    source_records = list(unique.values())
    source_records.sort(key=lambda rec: (rec.get('candidate_order', -1),
                                         0 if rec['label'] in {'android', 'audio-cache'} else 1,
                                         rec['destination'].casefold(), rec['source_root'].casefold()))
    if staged_hits:
        hit = staged_hits[0]
        matches = manifest_files.get(_fold_path(hit['candidate']), [])
        if len(matches) > 1:
            return {**base_class, 'classification': 'AMBIGUOUS',
                    'reason': 'The staged manifest contains multiple case-folded destination matches, which the port resolver rejects.',
                    'evidence': {'candidate': hit['candidate'], 'manifest_entries': matches},
                    'port_candidates': candidates[:32], 'source_candidates': [], 'selection_files': []}
        return {**base_class, 'classification': 'RESOLVED_STAGED',
                'reason': 'A proven source/content URI resolves to a file listed in the frozen staged2 manifest.',
                'evidence': {'uri': hit['uri'], 'manifest_path': hit['candidate'], 'manifest_entry': manifest_files[_fold_path(hit['candidate'])][0], 'provenance': hit['provenance']},
                'port_candidates': candidates[:32], 'source_candidates': [], 'selection_files': []}

    # Candidates sourced from exact original trees and absent from base manifest.
    existing = [record for record in source_records if Path(record['physical']).is_file()]
    destinations = defaultdict(list)
    for record in existing:
        destinations[_fold_path(record['destination'])].append(record)
    if existing:
        dest_key = next(iter(destinations))
        choices = destinations[dest_key]
        same_root_paths = defaultdict(set)
        for choice in choices:
            same_root_paths[choice['source_root'].casefold()].add(str(choice['physical']).casefold())
        if any(len(paths) > 1 for paths in same_root_paths.values()):
            return {**base_class, 'classification': 'AMBIGUOUS',
                    'reason': 'A source directory contains multiple case-folded matches for the same port candidate; the port resolver rejects this path.',
                    'evidence': {'destination': choices[0]['destination'], 'choices': [{'source_root': c['source_root'], 'relative': c['relative'], 'physical': str(c['physical'])} for c in choices]},
                    'port_candidates': candidates[:32], 'source_candidates': [], 'selection_files': []}
        hashes = {(Path(c['physical']).stat().st_size, _sha(c['physical'])) for c in choices}
        if len(hashes) > 1:
            return {**base_class, 'classification': 'AMBIGUOUS',
                    'reason': 'Multiple original corpora provide different bytes for the same port destination; no source version was selected.',
                    'evidence': {'destination': choices[0]['destination'], 'choices': [{'source_root': c['source_root'], 'relative': c['relative'], 'size': Path(c['physical']).stat().st_size, 'sha256': _sha(c['physical'])} for c in choices]},
                    'port_candidates': candidates[:32], 'source_candidates': [], 'selection_files': []}
        chosen = choices[0]
        return {**base_class, 'classification': 'ORIGINAL_EXISTS_NOT_STAGED',
                'reason': 'Exact source bytes exist in an original corpus and map to a port-resolvable destination absent from staged2.',
                'evidence': {'source_root': chosen['source_root'], 'source_relative': chosen['relative'], 'source_label': chosen['label'], 'destination': chosen['destination'], 'destination_root': chosen.get('destination_root'), 'size': Path(chosen['physical']).stat().st_size, 'sha256': _sha(chosen['physical']), 'provenance': chosen['provenance']},
                'port_candidates': candidates[:32], 'source_candidates': [], 'selection_files': []}

    reason_code = str(row.get('reason_code') or row.get('reason') or '').casefold()
    filelike = bool(uri_pairs)
    nearest = []
    basenames = {Path(uri.replace('\\', '/').split('#', 1)[0]).name.casefold()
                 for uri, _ in uri_pairs if Path(uri.replace('\\', '/').split('#', 1)[0]).name}
    for spec in source_roots:
        for basename in sorted(basenames):
            for rel, path in spec['basenames'].get(basename, [])[:4]:
                nearest.append({'source_root': spec['source_root'], 'relative': rel,
                                'reason': 'same basename only; not treated as a resolution'})
                if len(nearest) >= 12:
                    break
            if len(nearest) >= 12:
                break
        if len(nearest) >= 12:
            break
    if filelike and exact_rule and candidates and nearest:
        state = 'UNRESOLVED'
        reason = 'Exact port candidates are absent, but same-basename files exist at other paths; no alias was inferred.'
    elif filelike and exact_rule and candidates and reason_code in {'file_missing', 'name_no_file'}:
        if str(row.get('kind') or '').casefold() == 'texture':
            state = 'UNRESOLVED'
            reason = 'The exact extracted-source texture target is absent, but packed iOS .dat resources are not decoded into standalone files for this check; absence from every corpus is unproven.'
        else:
            state = 'ABSENT_ALL_CORPORA'
            reason = 'The exact authored file target and port candidates were checked in every indexed, stageable extracted source root; no file or same-basename candidate exists.'
    elif filelike and exact_rule and candidates:
        state = 'PORT_RULE_ABSENT'
        reason = 'A proven port file-resolution rule gives a concrete expected path absent from staged2; no exact original source bytes were joined to this site.'
    else:
        state = 'UNRESOLVED'
        reason = 'No exact, proven file mapping or typed non-resource rule applies; the source site remains unresolved.'
    return {**base_class, 'classification': state, 'reason': reason,
            'evidence': {'rule': rule or None, 'original_status': row.get('status'), 'original_reason_code': row.get('reason_code'), 'name': row.get('name'), 'checked_roots': [s['source_root'] for s in source_roots], 'nearest_candidates': nearest},
            'port_candidates': candidates[:32], 'source_candidates': [], 'selection_files': []}


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
    current_scope_path = _ensure_current_act2_scope(ctx)
    act2_scope_input = read_json(current_scope_path, {}) or {}
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
        'act2_scope_input': current_scope_path.relative_to(ctx.out).as_posix() if act2_scope_input else None,
        'act2_edges': act2_scope_input.get('edges', []),
        'source_parse_repairs': act2_scope_input.get('source_parse_repairs', []),
        'act2_scope_counts': act2_scope_input.get('counts', {}),
        'act2_frontier': act2_scope_input.get('frontier', []),
        'act2_uncertain_edges': act2_scope_input.get('uncertain_edges', []),
        'campaign_rows': {act: sum(1 for r in campaign_refs if (r.get('act') == 'act_1' if act == 'act_1' else r.get('scene_id') in act2_scene_ids)) for act in ('act_1', 'act_2_inferred')},
        'actor_rows': {act: len(scoped_actor_ids[act]) for act in ('act_1', 'act_2_inferred')},
        'scene_module_source_files': {act: len(source_paths_by_act[act]) for act in ('act_1', 'act_2_inferred')},
        'reached_source_files_without_round1_reference_rows': sorted(
            p for paths in source_paths_by_act.values() for p in paths if not refs_by_file.get(p)),
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


def _scope_fingerprint(site_rows):
    stable = []
    for (act, reference_id), row in sorted(site_rows.items(), key=lambda item: (item[0][0], item[0][1])):
        stable.append({key: row.get(key) for key in (
            'act', 'reference_id', 'kind', 'site', 'name', 'rule', 'confidence',
            'reference_kind', 'scope_reasons', 'scope_retired')})
    payload = json.dumps(stable, ensure_ascii=False, sort_keys=True, separators=(',', ':')).encode('utf-8')
    return hashlib.sha256(payload).hexdigest()


def enumerate_scope(ctx, site_rows_override=None, meta_override=None):
    if site_rows_override is None:
        site_rows, meta = _all_site_rows(ctx)
    else:
        site_rows, meta = site_rows_override, meta_override or {}
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
    scope_fingerprint = _scope_fingerprint(site_rows)
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
        'scope_fingerprint': scope_fingerprint,
        'counts_by_act_kind': counts, 'counts_by_act_family': family_counts,
        'unique_reference_sites': unique_sites, 'act_reference_pairs': total,
        'shared_reference_sites_across_acts': shared_sites,
        'act2_scope': meta,
        'scope_policy': 'One row per canonical source reference ID per act; no classification completed yet.',
    }
    # Package C and read-only discovery can call enumerate_scope while Package
    # B is already classified. Preserve completed B state when this exact scope
    # is unchanged; a fingerprint mismatch deliberately exposes stale output.
    progress_path = ctx.out / 'r2' / 'package-b-progress.json'
    previous_progress = read_json(progress_path, {}) or {}
    if not (previous_progress.get('phase') == 'CLASSIFIED_AND_PRECHECKED' and
            previous_progress.get('processed') == previous_progress.get('total') == total and
            previous_progress.get('scope_fingerprint') == scope_fingerprint):
        write_json(progress_path, progress)
    write_json(ctx.out / STAGING / 'scope-index.json', {
        'schema': 'atlas-r2-staging-scope-index-v1', 'total': total,
        'scope_fingerprint': scope_fingerprint,
        'unique_reference_sites': unique_sites, 'act_reference_pairs': total,
        'shared_reference_sites_across_acts': shared_sites,
        'files': sorted(files), 'counts_by_act_kind': counts,
        'counts_by_act_family': family_counts, 'act2_scope': meta,
    })
    return progress


def _read_scope_records(ctx, index=None):
    index = index or read_json(ctx.out / STAGING / 'scope-index.json', {}) or {}
    records = []
    for relative in index.get('files') or []:
        payload = read_json(ctx.out / relative, {}) or {}
        records.extend(payload.get('rows') or [])
    return records


def _step0_previous_ids(ctx):
    step0 = read_json(ctx.out / 'r2/STEP0.json', {}) or {}
    package = ((step0.get('packages') or {}).get('B') or {})
    try:
        commit = subprocess.run(['git', 'log', '-1', '--format=%H', '--',
                                  'coordination/atlas/r2/STEP0.json'], cwd=ctx.root,
                                capture_output=True, text=True, check=True).stdout.strip()
        archived = subprocess.run(['git', 'archive', commit, '--',
                                   'coordination/atlas/r2/staging/scope'], cwd=ctx.root,
                                  capture_output=True, check=True).stdout
        rows = []
        with tarfile.open(fileobj=io.BytesIO(archived), mode='r:*') as archive:
            for member in archive.getmembers():
                if not member.isfile() or '/staging/scope/' not in member.name:
                    continue
                stream = archive.extractfile(member)
                payload = json.loads(stream.read().decode('utf-8'))
                rows.extend(payload.get('rows') or [])
        if rows:
            return rows, package
    except (subprocess.SubprocessError, OSError, tarfile.TarError, json.JSONDecodeError):
        pass
    index = read_json(ctx.out / STAGING / 'scope-index.json', {}) or {}
    return _read_scope_records(ctx, index), index


def _dry_preflight(ctx, selection_path, folder):
    """Execute the real staging-script preflight AST through the line before mkdir."""
    source_path = ctx.root / 'coordination/v21.8/stage_assets_v21_8.py'
    source = source_path.read_text(encoding='utf-8')
    source_hash = _sha(source_path)
    tree = ast.parse(source, filename=str(source_path))
    main = next(node for node in tree.body if isinstance(node, ast.FunctionDef) and node.name == 'main')
    body = []
    for node in main.body:
        call = node.value if isinstance(node, ast.Expr) else None
        func = call.func if isinstance(call, ast.Call) else None
        if isinstance(func, ast.Attribute) and func.attr == 'mkdir' and isinstance(func.value, ast.Name) and func.value.id == 'target':
            break
        body.append(node)
    else:
        raise RuntimeError('Stage preflight extraction did not find target.mkdir boundary')
    main.body = body + [ast.Return(value=ast.Constant(value=None))]
    ast.fix_missing_locations(tree)
    namespace = {'__name__': 'stage_assets_r2_preflight'}
    exec(compile(tree, str(source_path), 'exec'), namespace)
    target = ctx.inputs / folder
    if target.exists():
        raise RuntimeError(f'Dry preflight scratch target already exists; refusing to run: {target}')
    old_argv = sys.argv
    sys.argv = [str(source_path), '--inputs', str(ctx.inputs), '--base', 'windows-source-clock-v21.8-staged2',
                '--selection', str(selection_path), '--folder', folder]
    try:
        namespace['main']()
    finally:
        sys.argv = old_argv
    if target.exists():
        raise RuntimeError(f'Stage preflight unexpectedly created its target: {target}')
    base, _, _ = _manifest_catalog(ctx)
    base_manifest = base / 'manifest.json'
    return {'selection': 'coordination/atlas/' + selection_path.relative_to(ctx.out).as_posix(),
            'selection_out_relative': selection_path.relative_to(ctx.out).as_posix(), 'passed': True,
            'target_created': False, 'stage_script': source_path.relative_to(ctx.root).as_posix(),
            'stage_script_sha256': source_hash,
            'selection_sha256': _sha(selection_path),
            'base_manifest': base_manifest.relative_to(ctx.inputs).as_posix(),
            'base_manifest_sha256': _sha(base_manifest),
            'boundary': 'executed real stage_assets_v21_8.py main through all base/selection preflight checks, stopping before target.mkdir'}


def _known_answer_report(ctx):
    atlas_check_path = ctx.out / 'data' / 'check.json'
    atlas_check = read_json(atlas_check_path, {}) or {}
    known = {str(item.get('name')): item for item in atlas_check.get('known_answers') or []}
    sound = known.get('45 distinct sounds.xml filenames absent from the combined original corpus') or {}
    faery = known.get('sfx_faerie_ice.wav is absent from original data') or {}
    detail = str(sound.get('detail') or '')
    match = re.search(r"combined_source_missing=(\d+).*combined_missing_names=(\[[^\]]*\])", detail)
    missing_names = []
    if match:
        try:
            missing_names = ast.literal_eval(match.group(2))
        except (SyntaxError, ValueError):
            missing_names = []
    passed = bool(sound.get('passed') and match and match.group(1) == '45' and len(missing_names) == 45 and faery.get('passed'))
    return {'schema': 'atlas-r2-staging-known-answers-v1', 'source': 'data/check.json',
            'passed': passed,
            'tests': [
                {'name': '45 sounds.xml filenames remain absent', 'passed': bool(sound.get('passed') and match and match.group(1) == '45' and len(missing_names) == 45),
                 'count': len(missing_names), 'names': missing_names, 'source_detail': detail},
                {'name': 'sfx_faerie_ice.wav absent from original data', 'passed': bool(faery.get('passed')),
                 'source_detail': faery.get('detail')},
            ],
            'packed_limit': 'Package B selections use extracted regular files only. Packed iOS .dat resources are not treated as stageable paths or as proven absent unless separately materialized and mapped.'}


def _write_classification_report(ctx, progress, metrics, status_counts, selection_files, preflights, known_answers):
    current_path = ctx.out / ACT2_SCOPE_PATH
    current = read_json(current_path, {}) or {}
    levels = current.get('levels') or []
    edges = current.get('edges') or []
    modules = current.get('module_sources') or []
    families = progress.get('counts_by_act_family') or {}
    family_lines = []
    for act in ('act_1', 'act_2_inferred'):
        vals = families.get(act, {})
        family_lines.append('| `' + act + '` | ' + ' | '.join(str(vals.get(k, 0)) for k in ('anim', 'sound', 'effect', 'projectile', 'model', 'script', 'other/nonresource')) + ' |')
    metric_lines = []
    for act in ('act_1', 'act_2_inferred'):
        metric_lines.append('| `' + act + '` | ' + str(metrics['before_source_unresolved_by_act'].get(act, 0)) + ' | ' +
                            str(metrics['after_source_unresolved_by_act'].get(act, 0)) + ' | ' +
                            str(metrics['staging_ready_by_act'].get(act, 0)) + ' | ' +
                            str(metrics['already_staged_by_act'].get(act, 0)) + ' |')
    selections = []
    selected_path_count = 0
    for path in selection_files:
        payload = read_json(path, {}) or {}
        count = sum(len(values) for values in (payload.get('groups') or {}).values())
        selected_path_count += count
        selections.append(f"- `{path.relative_to(ctx.out).as_posix()}`: {count} source paths; `{payload.get('source_root')}` → `{payload.get('destination_root')}`.")
    status_text = ', '.join(f'{key}={status_counts.get(key, 0)}' for key in
                            ('RESOLVED_STAGED', 'ORIGINAL_EXISTS_NOT_STAGED', 'PORT_RULE_ABSENT', 'ABSENT_ALL_CORPORA', 'AMBIGUOUS', 'UNRESOLVED', 'NOT_NEEDED'))
    report = f'''# Package B classification and staging selections

Package B has classified **{progress.get('processed', 0):,}/{progress.get('total', 0):,} Act/reference pairs**. The current inventory represents **{progress.get('unique_reference_sites', 0):,} unique canonical reference IDs**, of which **{progress.get('shared_reference_sites_across_acts', 0):,}** occur in both act labels. Raw kind shards remain available in `scope/` and `classification/`; aggregate families follow.

## Scope and evidence

The immutable Step 0 baseline is **28,555 pairs / 22,607 unique IDs**. Package C's current authored-source closure is `{ACT2_SCOPE_PATH.as_posix()}` ({len(levels)} scene rows, {len(edges)} source edges, {len(modules)} module-source variants). It adds **{(progress.get('scope_expansion') or {}).get('added_pairs', 0):,} reference pairs**; `scope-expansion.json` retains the prior IDs/counts and the added rows' initial `TODO` state. The two Act 1 backlink scenes and `act_2_inferred` membership remain explicitly labeled in the source shards; this is not a claim that every reached scene is narratively Act 2.

The source audit is in `STAGING-RULES.md`. The content resolver hash is `B5C1D517CFC194987D4E912A5535B0FE37CC184AE5F7AE4698EAF5B7ADEEAEEB`; the frozen staging script hash used for preflight is `{preflights[0].get('stage_script_sha256') if preflights else 'not run'}`. The source resolver is evidence for the port's candidate rules, not proof that an untyped serialized string is executed. Top unresolved rows preserve recorded mob, level, skill, or source-file chains in `top50-blocking-references.json`.

## Per-act family scope

| Act label | Anim | Sound | Effect | Projectile | Model | Script | Other/nonresource |
|---|---:|---:|---:|---:|---:|---:|---:|
{chr(10).join(family_lines)}

## Source-resolution and staging metrics

“Before unresolved” is the atlas source row's original `UNRESOLVED` status. “After unresolved” includes Package B `UNRESOLVED`, `AMBIGUOUS`, `PORT_RULE_ABSENT`, and `ABSENT_ALL_CORPORA`. These metrics count act/reference pairs. “Ready to stage” counts pairs whose exact original regular file is absent from staged2 and included in a selection; “already staged” counts pairs whose destination is present in the manifest. Shared pairs can refer to the same selected path.

| Act label | Before unresolved | After unresolved | Ready to stage | Already staged |
|---|---:|---:|---:|---:|
{chr(10).join(metric_lines)}

Terminal classification totals: **{status_text}**. No resource row is called `ABSENT_ALL_CORPORA` unless an exact rule and concrete target support that conclusion. Unresolved native/dynamic, serialized, packed-only, or embedded-script cases stay unresolved with the evidence limit stated on each row.

## Selections and preflight

{chr(10).join(selections) if selections else '- No new source paths were selected.'}

The selections contain **{selected_path_count} unique physical source paths** total; this is lower than the number of ready-to-stage pairs because several rows share one file.

The selected paths have no duplicate source path across files/groups and no destination already in the base manifest. The preflight executed `stage_assets_v21_8.py`'s original `main` through base-manifest hashing, selection validation, source checks, and duplicate/destination checks, stopping before `target.mkdir`. **{sum(1 for x in preflights if x.get('passed') and not x.get('target_created'))}/{len(preflights)}** checks passed; `preflight.json` records `mutated_staged_or_base=false`. This validates copy inputs only; it does not create an output package. Packed iOS `.dat` resources are not offered as ready-to-stage regular files; materialization or extraction remains an explicit blocker when relevant.

## Known-answer preservation and remaining limits

`known-answers.json` reproduces the atlas evidence that **45** distinct `sounds.xml` filenames have no file in the combined original source corpus and that `sfx_faerie_ice.wav` is absent. These global catalog checks are separate from the scoped reference-pair metrics. The ledger does not invent filename aliases or infer execution from matching serialized text. The source parse repairs that expanded Package C's closure are recorded in `scope-expansion.json`; new rows remain traceable to their source sites and confidence.
'''
    (ctx.out / STAGING / 'CLASSIFICATION-AND-SELECTIONS.md').write_text(report, encoding='utf-8')


def run(ctx):
    """Classify source sites and build exact v21.8 selection files after Step 0 release."""
    prior_rows, prior_index = _step0_previous_ids(ctx)
    prior_pairs = {(row.get('act'), row.get('reference_id')) for row in prior_rows}
    prior_ids = sorted({rid for _, rid in prior_pairs if rid})
    current_index = read_json(ctx.out / STAGING / 'scope-index.json', {}) or {}
    current_rows = _read_scope_records(ctx, current_index)
    preserve_rows = {(row.get('act'), row.get('reference_id')): row for row in prior_rows}
    preserve_rows.update({(row.get('act'), row.get('reference_id')): row for row in current_rows})
    expansion_path = ctx.out / STAGING / 'scope-expansion.json'
    prior_expansion = read_json(expansion_path, {}) or {}
    site_rows, meta = _all_site_rows(ctx)
    for old in preserve_rows.values():
        key = (old.get('act'), old.get('reference_id'))
        if key not in site_rows:
            retired = dict(old)
            retired['scope_retired'] = True
            retired.setdefault('scope_reasons', []).append('immutable Step0 reference retained after current C closure no longer includes its owner')
            site_rows[key] = retired
    # Preserve the committed Step-0 inventory before regenerating for C's
    # source-evidenced scope expansion. New rows start in TODO in this record.
    new_pairs = sorted(set(site_rows) - prior_pairs)
    if not prior_expansion and new_pairs:
        write_json(expansion_path, {
            'schema': 'atlas-r2-scope-expansion-v1',
            'evidence_source': ACT2_SCOPE_PATH.as_posix(),
            'evidence_sha256': hashlib.sha256((ctx.out / ACT2_SCOPE_PATH).read_bytes()).hexdigest().upper(),
            'source_parse_repairs': meta.get('source_parse_repairs', []),
            'initial_state': 'TODO',
            'previous_step0': {'total_act_reference_pairs': prior_index.get('total'),
                               'unique_reference_sites': prior_index.get('unique_reference_sites'),
                               'previous_reference_ids': prior_ids,
                               'previous_act_reference_pairs': [{'act': act, 'reference_id': rid} for act, rid in sorted(prior_pairs)]},
            'added_pairs': [{'act': act, 'reference_id': rid, 'kind': site_rows[(act, rid)].get('kind'),
                             'site': site_rows[(act, rid)].get('site'), 'state': 'TODO',
                             'confidence': site_rows[(act, rid)].get('confidence', 'GUESS'),
                             'reference_kind': site_rows[(act, rid)].get('reference_kind'),
                             'scope_reasons': site_rows[(act, rid)].get('scope_reasons', []),
                             'source_evidence': {'rule': site_rows[(act, rid)].get('rule'),
                                                 'reason_code': site_rows[(act, rid)].get('reason_code')}}
                            for act, rid in new_pairs],
            'added_pairs_count': len(new_pairs),
            'note': 'Only references made reachable by Package C source-parse repairs are added; Step 0 remains immutable.'})
        prior_expansion = read_json(expansion_path, {}) or {}
    elif prior_expansion:
        previous = prior_expansion.setdefault('previous_step0', {})
        previous.setdefault('total_act_reference_pairs', prior_index.get('total'))
        previous.setdefault('unique_reference_sites', prior_index.get('unique_reference_sites'))
        previous.setdefault('previous_reference_ids', prior_ids)
        previous.setdefault('previous_act_reference_pairs', [{'act': act, 'reference_id': rid} for act, rid in sorted(prior_pairs)])
        for item in prior_expansion.get('added_pairs') or []:
            row = site_rows.get((item.get('act'), item.get('reference_id')))
            if row:
                item['confidence'] = row.get('confidence', item.get('confidence', 'GUESS'))
                item['reference_kind'] = row.get('reference_kind')
                item['scope_reasons'] = row.get('scope_reasons', [])
                item['source_evidence'] = {'rule': row.get('rule'), 'reason_code': row.get('reason_code')}
        known = {(x.get('act'), x.get('reference_id')) for x in prior_expansion.get('added_pairs') or []}
        additions = []
        for act, rid in new_pairs:
            if (act, rid) not in known:
                row = site_rows[(act, rid)]
                additions.append({'act': act, 'reference_id': rid, 'kind': row.get('kind'),
                                  'site': row.get('site'), 'state': 'TODO', 'confidence': row.get('confidence', 'GUESS'),
                                  'reference_kind': row.get('reference_kind'), 'scope_reasons': row.get('scope_reasons', []),
                                  'source_evidence': {'rule': row.get('rule'), 'reason_code': row.get('reason_code')}})
        if additions:
            prior_expansion['added_pairs'].extend(additions)
            prior_expansion['added_pairs_count'] = len(prior_expansion['added_pairs'])
        write_json(expansion_path, prior_expansion)
    # Regenerate scope rows against the now-frozen expanded C evidence, then
    # classify from the exact current staged2 manifest and original inputs.
    enumerate_scope(ctx, site_rows_override=site_rows, meta_override=meta)
    scope_index = read_json(ctx.out / STAGING / 'scope-index.json', {}) or {}
    source_roots, root_by_key = _source_catalog(ctx)
    base, manifest, manifest_files = _manifest_catalog(ctx)
    classified = []
    classified_by_key = {}
    for row in _read_scope_records(ctx, scope_index):
        result = _classify_site(ctx, row, source_roots, manifest_files)
        if result['classification'] == 'ORIGINAL_EXISTS_NOT_STAGED':
            evidence = result.get('evidence') or {}
            root_key = str(evidence.get('source_root') or '').casefold()
            rel = evidence.get('source_relative')
            dest = evidence.get('destination')
            if root_key in root_by_key and rel and dest and _fold_path(dest) not in manifest_files:
                root_spec = root_by_key[root_key]
                candidates = root_spec['entries'].get(_fold_path(rel), [])
                physical = next((p for p in candidates if p.is_file()), None)
                if physical:
                    result['_selection'] = {'source_root': root_spec['source_root'], 'relative': rel,
                                            'destination': dest, 'destination_root': evidence.get('destination_root') or root_spec['destination_root'], 'physical': str(physical)}
        classified.append(result)
        classified_by_key[(row.get('act'), row.get('reference_id'))] = result

    # A staged path is selected once across both act selections. Shared IDs
    # point at the one compatible selection JSON rather than duplicating a file.
    selection_owner = {}
    selections = defaultdict(lambda: defaultdict(set))
    selection_destinations = defaultdict(dict)
    for result in classified:
        chosen = result.pop('_selection', None)
        if not chosen:
            continue
        path_key = (chosen['source_root'].casefold(), _fold_path(chosen['relative']))
        owner = selection_owner.get(path_key)
        if owner is None:
            owner = result['act']
            selection_owner[path_key] = owner
        label = re.sub(r'[^a-z0-9]+', '-', chosen['source_root'].casefold()).strip('-')
        selection_id = (owner, label, chosen['source_root'], chosen['destination_root'])
        selections[selection_id]['package_b_scope'].add(chosen['relative'])
        selection_destinations[selection_id][chosen['relative']] = chosen['destination']
        filename = f"asset-selection-r2-{owner}-{label}.json"
        result['selection_files'] = [f'r2/staging/selections/{filename}']
        result['evidence']['selection_destination'] = chosen['destination']
    selection_dir = ctx.out / STAGING / 'selections'
    for old in selection_dir.glob('asset-selection-r2-*.json') if selection_dir.exists() else []:
        old.unlink()
    selection_files = []
    selected_global = set()
    for (owner, label, source_root, destination_root), groups in sorted(selections.items()):
        dest_label = re.sub(r'[^a-z0-9]+', '-', destination_root.casefold()).strip('-')
        filename = f'asset-selection-r2-{owner}-{label}-{dest_label}.json'
        path = selection_dir / filename
        group_payload = {}
        for group, paths in sorted(groups.items()):
            values = sorted(paths, key=str.casefold)
            group_payload[group] = values
        payload = {'schema': 'v21.8-asset-selection-1', 'source_root': source_root,
                   'destination_root': destination_root,
                   'note': f'Round-2 Package B exact-source additions. Source root: {source_root}; destination follows the frozen v21.8 stage script. Selection files are split by source root because the script accepts one source_root per selection.',
                   'groups': group_payload}
        write_json(path, payload)
        for group, values in group_payload.items():
            for rel in values:
                key = (source_root.casefold(), _fold_path(rel))
                if key in selected_global:
                    raise RuntimeError(f'Duplicate source path across selection files: {key}')
                selected_global.add(key)
        selection_files.append(path)

    preflights = []
    for i, path in enumerate(selection_files):
        folder = f'__atlas_r2_stage_preflight_no_write_{i}__'
        preflights.append(_dry_preflight(ctx, path, folder))

    # Emit per-act/per-kind classification shards and collect metrics.
    output_root = ctx.out / STAGING / 'classification'
    expected = set()
    by_kind = defaultdict(list)
    for result in classified:
        by_kind[(result['act'], str(result.get('kind') or 'unknown'))].append(result)
    classification_files = []
    act_kind = defaultdict(Counter)
    act_status = defaultdict(Counter)
    for (act, kind), rows in sorted(by_kind.items()):
        rows.sort(key=lambda r: (str(r.get('site') or ''), r['reference_id']))
        safe_kind = re.sub(r'[^a-z0-9_-]+', '_', kind.casefold()).strip('_') or 'unknown'
        safe_kind += '-' + hashlib.sha1(kind.encode('utf-8')).hexdigest()[:6]
        path = output_root / act / f'{safe_kind}.json'
        for row in rows:
            act_kind[(act, kind)][row['classification']] += 1
            act_status[act][row['classification']] += 1
        write_json(path, {'schema': 'atlas-r2-staging-classification-shard-v1', 'act': act,
                          'kind': kind, 'count': len(rows), 'rows': rows})
        expected.add(path.resolve())
        classification_files.append(path.relative_to(ctx.out).as_posix())
    for old in output_root.rglob('*.json') if output_root.exists() else []:
        if old.resolve() not in expected:
            old.unlink()

    # Update Step0 expansion rows with terminal classifications while retaining
    # their exact original TODO record.
    if prior_expansion:
        added = []
        for item in prior_expansion.get('added_pairs') or []:
            match = classified_by_key.get((item.get('act'), item.get('reference_id')))
            enriched = dict(item)
            if match:
                enriched.update(classification=match['classification'], reason=match['reason'], confidence=match['confidence'])
            added.append(enriched)
        prior_expansion['added_pairs'] = added
        prior_expansion['classified_state_counts'] = dict(Counter(x.get('classification', 'TODO') for x in added))
        write_json(expansion_path, prior_expansion)

    # Before/after metrics explicitly separate original-atlas unresolved state
    # from exact staged readiness.
    before_unresolved = Counter()
    after_unresolved = Counter()
    staged_ready = Counter()
    already_staged = Counter()
    metrics_by_act_kind = defaultdict(lambda: {'before_source_unresolved': 0, 'after_source_unresolved': 0,
                                                'staging_ready': 0, 'already_staged': 0, 'total': 0})
    kind_to_family = {}
    def family_for(kind):
        k = str(kind or '').casefold()
        if k in {'anim', 'animation_dictionary', 'animation event'}:
            return 'anim'
        if k in {'sound', 'source_sound_ordinal'}:
            return 'sound'
        if k in {'effect set', 'effect_set', 'animated effect'}:
            return 'effect'
        if k == 'projectile':
            return 'projectile'
        if k in {'model', 'model component', 'character', 'texture'}:
            return 'model'
        if k == 'script':
            return 'script'
        return 'other/nonresource'
    metrics_by_act_family = defaultdict(lambda: {'before_source_unresolved': 0, 'after_source_unresolved': 0,
                                                 'staging_ready': 0, 'already_staged': 0, 'total': 0})
    for row in _read_scope_records(ctx, scope_index):
        key = (row['act'], str(row.get('kind') or 'unknown'))
        metric = metrics_by_act_kind[key]
        family_metric = metrics_by_act_family[(row['act'], family_for(row.get('kind')))]
        metric['total'] += 1
        family_metric['total'] += 1
        if row.get('status') == 'UNRESOLVED' or row.get('work_status') == 'UNRESOLVED':
            metric['before_source_unresolved'] += 1
            family_metric['before_source_unresolved'] += 1
            before_unresolved[row['act']] += 1
        result = classified_by_key[(row['act'], row['reference_id'])]
        if result['classification'] in {'UNRESOLVED', 'AMBIGUOUS', 'PORT_RULE_ABSENT', 'ABSENT_ALL_CORPORA'}:
            metric['after_source_unresolved'] += 1
            family_metric['after_source_unresolved'] += 1
            after_unresolved[row['act']] += 1
        if result['classification'] == 'ORIGINAL_EXISTS_NOT_STAGED':
            metric['staging_ready'] += 1
            staged_ready[row['act']] += 1
            family_metric['staging_ready'] += 1
        elif result['classification'] == 'RESOLVED_STAGED':
            metric['already_staged'] += 1
            already_staged[row['act']] += 1
            family_metric['already_staged'] += 1

    blockers = [row for row in classified if row['classification'] not in {'RESOLVED_STAGED', 'NOT_NEEDED'}]
    priority = {'UNRESOLVED': 0, 'AMBIGUOUS': 1, 'PORT_RULE_ABSENT': 2,
                'ABSENT_ALL_CORPORA': 3, 'ORIGINAL_EXISTS_NOT_STAGED': 4}
    blockers.sort(key=lambda r: (priority.get(r['classification'], 9), -len(r.get('scope_reasons') or []),
                                 str(r.get('site') or ''), r['reference_id']))
    top50 = [{'act': r['act'], 'reference_id': r['reference_id'], 'kind': r.get('kind'),
              'site': r.get('site'), 'name': r.get('evidence', {}).get('name'),
              'classification': r['classification'], 'reason': r['reason'],
              'referrer_chains': r.get('scope_reasons', []), 'port_candidates': r.get('port_candidates', []),
              'confidence': r.get('confidence')} for r in blockers[:50]]
    write_json(ctx.out / STAGING / 'top50-blocking-references.json', {
        'schema': 'atlas-r2-staging-top-blockers-v1', 'ranking': 'unresolved/ambiguous/missing source states first, then number of recorded referrer chains',
        'total_blocking_pairs': len(blockers), 'rows': top50})
    metrics = {'schema': 'atlas-r2-staging-metrics-v1', 'source_unresolved_definition': 'original status UNRESOLVED before classification; after means UNRESOLVED, AMBIGUOUS, PORT_RULE_ABSENT, or ABSENT_ALL_CORPORA.',
               'staging_ready_definition': 'ORIGINAL_EXISTS_NOT_STAGED rows with an exact source path, destination, hash, and selection entry.',
               'already_staged_definition': 'RESOLVED_STAGED rows whose destination is in the frozen staged2 manifest.',
               'before_source_unresolved_by_act': dict(before_unresolved),
               'after_source_unresolved_by_act': dict(after_unresolved), 'staging_ready_by_act': dict(staged_ready),
               'already_staged_by_act': dict(already_staged),
               'by_act_kind': {f'{act}:{kind}': v for (act, kind), v in sorted(metrics_by_act_kind.items())},
               'by_act_family': {f'{act}:{family}': v for (act, family), v in sorted(metrics_by_act_family.items())}}
    write_json(ctx.out / STAGING / 'before-after-metrics.json', metrics)

    total = len(classified)
    status_counts = dict(Counter(row['classification'] for row in classified))
    selection_rel = [p.relative_to(ctx.out).as_posix() for p in selection_files]
    known_answers = _known_answer_report(ctx)
    write_json(ctx.out / STAGING / 'known-answers.json', known_answers)
    progress = {'schema': 'atlas-r2-package-progress-v1', 'package': 'B-staging-gap-ledger',
                'phase': 'CLASSIFIED_AND_PRECHECKED', 'total': total, 'processed': total,
                'status_counts': status_counts, 'scope_files': scope_index.get('files', []),
                'scope_fingerprint': scope_index.get('scope_fingerprint'),
                'classification_files': sorted(classification_files), 'selection_files': selection_rel,
                'preflight_count': len(preflights), 'preflights_passed': sum(1 for p in preflights if p['passed']),
                'counts_by_act_kind': scope_index.get('counts_by_act_kind', {}),
                'counts_by_act_family': scope_index.get('counts_by_act_family', {}),
                'unique_reference_sites': scope_index.get('unique_reference_sites'),
                'act_reference_pairs': total, 'shared_reference_sites_across_acts': scope_index.get('shared_reference_sites_across_acts'),
                'before_after_metrics': metrics, 'act2_scope': meta,
                'scope_expansion': {'path': 'r2/staging/scope-expansion.json', 'added_pairs': len(prior_expansion.get('added_pairs', [])) if prior_expansion else len(new_pairs)},
                'known_answers_preserved': known_answers}
    write_json(ctx.out / 'r2/package-b-progress.json', progress)
    write_json(ctx.out / STAGING / 'preflight.json', {'schema': 'atlas-r2-stage-preflight-v1',
                'staged2_manifest': str((base / 'manifest.json').relative_to(ctx.inputs)).replace('\\', '/'),
                'selection_files': selection_rel, 'checks': preflights,
                'mutated_staged_or_base': False})
    _write_classification_report(ctx, progress, metrics, status_counts, selection_files, preflights, known_answers)
    return progress


def check(ctx):
    errors = []
    tests = []
    progress = read_json(ctx.out / 'r2' / 'package-b-progress.json', {}) or {}
    scope_index = read_json(ctx.out / STAGING / 'scope-index.json', {}) or {}
    class_files = progress.get('classification_files') or []
    seen = set()
    rows = []
    for relative in class_files:
        path = ctx.out / relative
        if not path.is_file():
            errors.append(f'missing classification shard: {relative}')
            continue
        if path.stat().st_size >= 40_000_000:
            errors.append(f'classification shard exceeds 40 MB: {relative}')
        payload = read_json(path, {}) or {}
        for row in payload.get('rows') or []:
            key = (payload.get('act'), row.get('reference_id'))
            if key in seen:
                errors.append(f'duplicate classified site: {key}')
            seen.add(key)
            rows.append(row)
            if row.get('classification') not in {'RESOLVED_STAGED', 'ORIGINAL_EXISTS_NOT_STAGED', 'PORT_RULE_ABSENT', 'ABSENT_ALL_CORPORA', 'AMBIGUOUS', 'UNRESOLVED', 'NOT_NEEDED'}:
                errors.append(f'incomplete classification: {key}')
            if row.get('classification') in {'UNRESOLVED', 'NOT_NEEDED', 'PORT_RULE_ABSENT', 'ABSENT_ALL_CORPORA', 'AMBIGUOUS'} and not row.get('reason'):
                errors.append(f'missing classification reason: {key}')
            if row.get('classification') == 'NOT_NEEDED':
                evidence = row.get('evidence') or {}
                targets = evidence.get('resolves_to') or []
                if (targets and all(isinstance(x, dict) and x.get('type') in {'table-row', 'source-table-cell', 'table', 'enum'} for x in targets)
                        and ('stage-only disposition' not in str(row.get('reason') or '').casefold() or
                             'not a claim that the runtime reference is globally resolved' not in str(row.get('reason') or '').casefold())):
                    errors.append(f'typed table-key NOT_NEEDED lacks explicit stage-only scope: {key}')
                if (row.get('kind') == 'model component' and 'named modular node/icon' in str(row.get('rule') or '').casefold()):
                    errors.append(f'modular equipment controller was incorrectly cleared as NOT_NEEDED: {key}')
                if (row.get('kind') == 'script' and str(row.get('rule') or '').casefold() == 'xml_attribute'
                        and Path(str(row.get('name') or '')).suffix.casefold() not in {'.pyscript', '.luac'}):
                    errors.append(f'script symbol lacks exact bank linkage but was cleared as NOT_NEEDED: {key}')
            if row.get('confidence') not in {'PROVEN', 'LIKELY', 'GUESS'}:
                errors.append(f'invalid confidence: {key}: {row.get("confidence")}')
    tests.append({'name': 'all_scope_rows_classified_with_evidence', 'rows': len(rows), 'passed': not errors})
    if len(rows) != scope_index.get('total') or len(rows) != progress.get('total'):
        errors.append(f'classification coverage mismatch: rows={len(rows)} scope={scope_index.get("total")} progress={progress.get("total")}')
    if not scope_index.get('scope_fingerprint') or progress.get('scope_fingerprint') != scope_index.get('scope_fingerprint'):
        errors.append('classification progress is stale relative to the current scope fingerprint')

    seen_selection = set()
    seen_destinations = set()
    selected_destinations = {}
    selected_source_hashes = {}
    base, manifest, manifest_files = _manifest_catalog(ctx)
    expected_selected = {}
    for row in rows:
        if row.get('classification') != 'ORIGINAL_EXISTS_NOT_STAGED':
            continue
        evidence = row.get('evidence') or {}
        key = (str(evidence.get('source_root') or '').casefold(), _fold_path(evidence.get('source_relative')))
        expected = (str(evidence.get('destination') or ''), str(evidence.get('sha256') or '').casefold(), evidence.get('size'))
        if key in expected_selected and expected_selected[key] != expected:
            errors.append(f'conflicting classified source evidence for selected path: {key}')
        expected_selected[key] = expected
    for relative in progress.get('selection_files') or []:
        path = ctx.out / relative
        if not path.is_file():
            errors.append(f'missing selection file: {relative}')
            continue
        selection = read_json(path, {}) or {}
        if selection.get('schema') != 'v21.8-asset-selection-1' or not selection.get('source_root') or not selection.get('destination_root'):
            errors.append(f'invalid stage_assets selection schema: {relative}')
            continue
        for group, paths in (selection.get('groups') or {}).items():
            for rel in paths:
                key = (selection['source_root'].casefold(), _fold_path(rel))
                if key in seen_selection:
                    errors.append(f'duplicate path across selection files/groups: {key}')
                seen_selection.add(key)
                dest_root = (selection.get('group_destinations') or {}).get(group, selection['destination_root']).strip('/')
                dest = dest_root + '/' + rel
                if _fold_path(dest) in manifest_files:
                    errors.append(f'selection destination already exists in base: {dest}')
                dest_key = _fold_path(dest)
                if dest_key in seen_destinations:
                    errors.append(f'duplicate destination across selection files/groups: {dest}')
                seen_destinations.add(dest_key)
                selected_destinations[key] = dest
                src = ctx.inputs / selection['source_root'] / Path(rel)
                if not src.is_file() or src.is_symlink():
                    errors.append(f'selection source is missing or not regular: {src}')
                else:
                    selected_source_hashes[key] = (_sha(src).casefold(), src.stat().st_size)
                expected = expected_selected.get(key)
                if expected is None:
                    errors.append(f'selection contains a source with no ORIGINAL_EXISTS_NOT_STAGED classification: {key}')
                elif dest != expected[0]:
                    errors.append(f'selection destination differs from classified destination: {key}: {dest} != {expected[0]}')
                elif src.is_file() and not src.is_symlink() and (selected_source_hashes.get(key) != (expected[1], expected[2])):
                    errors.append(f'selection source bytes differ from classified SHA/size evidence: {key}')
    tests.append({'name': 'stage_selection_schema_unique_paths_and_new_destinations', 'files': len(progress.get('selection_files') or []), 'passed': not errors})
    unselected = []
    for row in rows:
        if row.get('classification') != 'ORIGINAL_EXISTS_NOT_STAGED':
            continue
        evidence = row.get('evidence') or {}
        key = (str(evidence.get('source_root') or '').casefold(), _fold_path(evidence.get('source_relative')))
        if not evidence.get('destination') or selected_destinations.get(key) != evidence.get('destination'):
            unselected.append((row.get('act'), row.get('reference_id'), key))
    if unselected:
        errors.append(f'original-present classification lacks an exact selection/destination: {unselected[:5]}')
    unknown_classified_paths = sorted(set(expected_selected) - seen_selection)
    if unknown_classified_paths:
        errors.append(f'classified original-present paths missing from selections: {unknown_classified_paths[:5]}')
    tests.append({'name': 'every_original_present_gap_has_exact_selection', 'rows': sum(1 for r in rows if r.get('classification') == 'ORIGINAL_EXISTS_NOT_STAGED'),
                  'passed': not unselected})
    preflight = read_json(ctx.out / STAGING / 'preflight.json', {}) or {}
    checks = preflight.get('checks') or []
    checks_by_selection = {c.get('selection_out_relative') or c.get('selection'): c for c in checks if c.get('selection_out_relative') or c.get('selection')}
    for relative in progress.get('selection_files') or []:
        selection_path = ctx.out / relative
        receipt = checks_by_selection.get(relative)
        if not receipt:
            errors.append(f'no preflight receipt binds selection: {relative}')
            continue
        if not selection_path.is_file() or receipt.get('selection_sha256') != _sha(selection_path):
            errors.append(f'preflight receipt selection hash mismatch: {relative}')
        base_relative = preflight.get('staged2_manifest')
        base_path = ctx.inputs / Path(str(base_relative or ''))
        if (not base_path.is_file() or receipt.get('base_manifest') != base_relative or
                receipt.get('base_manifest_sha256') != _sha(base_path)):
            errors.append(f'preflight receipt base-manifest hash mismatch: {relative}')
        stage_script = ctx.root / str(receipt.get('stage_script') or '')
        if (not stage_script.is_file() or receipt.get('stage_script_sha256') != _sha(stage_script)):
            errors.append(f'preflight receipt stage-script hash mismatch: {relative}')
    if len(checks) != len(progress.get('selection_files') or []) or any(not c.get('passed') or c.get('target_created') for c in checks):
        errors.append('stage_assets real-script scratch preflight missing, failed, or created output')
    if preflight.get('mutated_staged_or_base') is not False:
        errors.append('preflight mutation flag is not false')
    tests.append({'name': 'real_stage_script_preflight_stopped_before_write', 'checks': len(checks), 'passed': len(checks) == len(progress.get('selection_files') or []) and all(c.get('passed') and not c.get('target_created') for c in checks)})
    known_answers = read_json(ctx.out / STAGING / 'known-answers.json', {}) or {}
    if not known_answers.get('passed'):
        errors.append('Package B known-answer checks failed: 45 missing sounds.xml filenames and/or sfx_faerie_ice.wav absence')
    tests.append({'name': 'required_original_sound_absence_known_answers', 'passed': bool(known_answers.get('passed')),
                  'checks': known_answers.get('tests', [])})
    expansion = read_json(ctx.out / STAGING / 'scope-expansion.json', {}) or {}
    if expansion.get('initial_state') != 'TODO' or not expansion.get('previous_step0', {}).get('previous_act_reference_pairs'):
        errors.append('scope expansion does not preserve the immutable Step 0 rows and initial TODO state')
    tests.append({'name': 'immutable_step0_scope_expansion_preserved',
                  'passed': expansion.get('initial_state') == 'TODO' and bool(expansion.get('previous_step0', {}).get('previous_act_reference_pairs')),
                  'added_pairs': expansion.get('added_pairs_count')})
    if progress.get('processed') != progress.get('total') or 'TODO' in progress.get('status_counts', {}):
        errors.append('package progress still contains unprocessed/TODO classification rows')
    return {'passed': not errors, 'tests': tests, 'errors': errors}
