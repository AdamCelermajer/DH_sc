"""Bind the bounded Character combat checkpoint to exact current artifacts.

Reads files only; does not build, install, run ADB, create a checkpoint or run
instruction or host tests. Required inputs are final repo/Studio APKs, an
existing checkpoint, current packaged state/blender reports, current sanitized
Playback/event audits, and movement/lifecycle/combat smoke reports. Historical
reports remain evidence for their saved artifacts, not fresh runtime passes.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys
import zipfile

REPO = Path(__file__).resolve().parents[3]
ROOT = REPO / 'port/android-native'
REPORTS = REPO / 'port/level-world/reports'
LOCAL = REPO / '.local-inputs'
ORIGINAL = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
CACHE = '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'


def require(condition, message):
    if not condition:
        raise AssertionError(message)


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def read(path):
    return json.loads(path.read_text(encoding='utf-8-sig'))


def binding(path, value=None):
    return {'path': str(path.resolve()), 'sha256': digest(path),
            'result': read(path) if value is None else value}


def source_bindings(records):
    require(bool(records), 'Missing audited source hashes')
    for name, expected in records.items():
        path = (REPO / name).resolve()
        require(path.is_relative_to(REPO) and digest(path) == expected,
                'Audited source changed: ' + name)


def elf_symbols(raw):
    require(raw[:6] == b'\x7fELF\x02\x01', 'Expected little-endian ELF64')
    offset = struct.unpack_from('<Q', raw, 40)[0]
    stride, count = struct.unpack_from('<HH', raw, 58)
    sections = [struct.unpack_from('<IIQQQQIIQQ', raw, offset+i*stride) for i in range(count)]
    defined, imported = set(), set()
    for section in sections:
        if section[1] != 11:
            continue
        strings = sections[section[6]]
        names = raw[strings[4]:strings[4]+strings[5]]
        for cursor in range(section[4], section[4]+section[5], section[9]):
            name, info, _, index, value, _ = struct.unpack_from('<IBBHQQ', raw, cursor)
            if not name or info >> 4 not in (1, 2):
                continue
            symbol = names[name:names.index(b'\0', name)].decode('ascii')
            (defined if index and value else imported).add(symbol)
    return defined, imported


def library_record(archive, info, apk):
    raw = archive.read(info)
    abi = info.filename.split('/')[1]
    require(abi in ('arm64-v8a', 'x86_64'), 'Unexpected ABI: ' + info.filename)
    require(raw[:6] == b'\x7fELF\x02\x01', 'Expected native ELF64: ' + info.filename)
    require(struct.unpack_from('<H', raw, 18)[0] == {'arm64-v8a': 183, 'x86_64': 62}[abi],
            'ELF machine disagrees with APK ABI: ' + info.filename)
    offset = struct.unpack_from('<Q', raw, 32)[0]
    stride, count = struct.unpack_from('<HH', raw, 54)
    alignments = [struct.unpack_from('<Q', raw, offset+i*stride+48)[0]
                  for i in range(count) if struct.unpack_from('<I', raw, offset+i*stride)[0] == 1]
    require(alignments and min(alignments) >= 16384, 'ELF load alignment: ' + info.filename)
    with apk.open('rb') as stream:
        stream.seek(info.header_offset)
        header = stream.read(30)
    require(header[:4] == b'PK\x03\x04', 'Invalid APK local header')
    name_bytes, extra_bytes = struct.unpack_from('<HH', header, 26)
    data_offset = info.header_offset + 30 + name_bytes + extra_bytes
    require(info.compress_type == zipfile.ZIP_STORED and data_offset % 16384 == 0,
            'Native APK member lacks 16KiB ZIP alignment: ' + info.filename)
    return {'path': info.filename, 'sha256': sha(raw),
            'minimum_load_alignment': min(alignments), 'apk_data_offset': data_offset}


def capture_manifest(path, engine):
    manifest = read(path)
    require(manifest['original_sha256'] == ORIGINAL, 'Wrong original capture manifest')
    require(engine[:5] == b'\x7fELF\x01', 'Expected original ARM32 ELF')
    offset = struct.unpack_from('<I', engine, 28)[0]
    stride, count = struct.unpack_from('<HH', engine, 42)
    loads = [struct.unpack_from('<IIIIIIII', engine, offset+i*stride) for i in range(count)]
    for row in manifest['functions']:
        address, size = int(row['elf_address'], 16), row['size']
        segment = next((item for item in loads if item[0] == 1 and item[2] <= address
                        and address+size <= item[2]+item[4]), None)
        require(segment is not None, 'Capture address outside original file: ' + row['original_symbol'])
        start = segment[1]+address-segment[2]
        require(sha(engine[start:start+size]) == row['sha256'], 'Original capture bytes changed')
    return {'path': str(path.relative_to(REPO)), 'sha256': digest(path),
            'functions_verified': len(manifest['functions'])}


def input_rows(value):
    if isinstance(value, dict):
        entry = value.get('entry', value.get('archive_entry'))
        if entry is not None and 'sha256' in value and 'bytes' in value:
            yield entry, value
        for child in value.values():
            yield from input_rows(child)
    elif isinstance(value, list):
        for child in value:
            yield from input_rows(child)


def verify_authored_cache(cache, assets, provenance):
    require(digest(cache) == CACHE, 'Original cache SHA256 changed')
    sys.path.insert(0, str(REPO / 'port/level-world/tools'))
    sys.path.insert(0, str(Path(__file__).parent))
    from prepare_actors import strings
    from prepare_player_combat import animation_tables
    from bundle_locomotion import constants
    verified = {}
    with zipfile.ZipFile(cache) as archive:
        entries = {}
        for item in archive.infolist():
            entries.setdefault(item.filename, []).append(item)
        for name, manifest in provenance.items():
            require(manifest.get('cache_sha256') == CACHE, 'Cache provenance mismatch: ' + name)
            for entry, row in input_rows(manifest):
                require(len(entries.get(entry, [])) == 1, 'Missing/duplicate original input: ' + entry)
                raw = archive.read(entries[entry][0])
                require(len(raw) == row['bytes'] and sha(raw) == row['sha256'], 'Original input changed: ' + entry)
                if 'asset' in row:
                    require(assets[row['asset']] == {'bytes': len(raw), 'sha256': sha(raw)},
                            'Original asset disagrees with input: ' + row['asset'])
                verified[entry] = {'bytes': len(raw), 'sha256': sha(raw)}
        lower = {}
        for item in archive.infolist():
            lower.setdefault(item.filename.lower(), []).append(item)
        used = set()
        def original(relative):
            key = 'com.gameloft.android.gand.gloftd2ss/files/' + relative.lower()
            found = lower.get(key, [])
            require(len(found) == 1, 'Missing/duplicate original table/clip: ' + relative)
            used.add(found[0].filename)
            return archive.read(found[0]), found[0].filename
        def table(name):
            return original('data/pydata/' + name)[0]
        names, _ = strings(table('character_properties_pyarraynames.bin'))
        fields, _ = strings(table('character_properties_pystructnames.bin'))
        row = names.index('KnightPlayerBase')
        properties = struct.unpack_from('<224i', table('character_properties_pyarray.bin'), 4+row*896)
        table_id = properties[fields.index('AnimTable')]
        sequences, characters = animation_tables(table('animations_pyarray.bin'))
        raw = table('animations_pystructnames.bin')
        for _ in range(4):
            state_names, consumed = strings(raw)
            raw = raw[consumed:]
        paths, _ = strings(table('animations_dictionary_pyarray.bin'))
        cst = constants(table('animations_pycst.bin'))
        mask, stance_count = cst['AnimStancedAnim']['SL__LIST_IPHONE'], cst['AnimStances']['COUNT_IPHONE']
        require((row, table_id, mask, stance_count) == (263, 48, 210, 5), 'Authored Knight table/stance changed')
        clips, states = {}, []
        def collect(index, depth=0):
            require(depth < 3 and 0 <= index < len(sequences), 'Malformed authored sequence redirection')
            for step in sequences[index]['steps']:
                if step['redir'] == 1:
                    collect(step['anim'], depth+1)
                else:
                    require(step['redir'] == 0, 'Unsupported authored redirection')
                    clips[step['anim']] = paths[step['anim']]
        for label, field, bit in (('AttackMoving', 'Attack', 'SL_ATTACK'),
                                  ('AttackStatic', 'AttackStatic', 'SL_ATTACK_STATIC'),
                                  ('Died', 'Died', 'SL_DIED')):
            base = characters[table_id][state_names.index(field)][0]
            enabled = bool(mask & cst['AnimStancedAnim'][bit])
            roots = list(range(base, base+(stance_count if enabled else 1)))
            for root in roots:
                collect(root)
            states.append({'state': label, 'table_field': field, 'base_sequence': base,
                           'stance_enabled': enabled, 'sequences': roots})
        combat = provenance['character-combat-provenance.json']
        require(combat['character'] == 'KnightPlayerBase' and combat['character_row'] == row
                and combat['animation_table'] == table_id and combat['stance_mask'] == mask
                and combat['stance_count'] == stance_count and combat['states'] == states,
                'Combat provenance disagrees with actual authored tables')
        actual = {item['clip_id']: item for item in combat['clips']}
        require(len(actual) == len(combat['clips']) == len(clips) == 64, 'Combat bank must contain64 distinct clips')
        for clip, path in clips.items():
            raw, entry = original(path)
            item = actual[clip]
            expected = {'clip_id': clip, 'asset': 'animations/'+Path(path.replace('\\', '/')).name,
                        'entry': entry, 'bytes': len(raw), 'sha256': sha(raw)}
            require(item == expected and assets[item['asset']] == {'bytes': len(raw), 'sha256': sha(raw)},
                    'Combat clip identity disagrees with original dictionary: ' + str(clip))
        require(len(combat['inputs']) == len(used) == 71
                and {item['entry'] for item in combat['inputs']} == used, 'Combat original-input manifest differs')
    return {'cache_sha256': CACHE, 'unique_original_inputs': len(verified),
            'original_inputs_verified': verified, 'combat_clips': 64, 'combat_inputs': 71,
            'combat_states': states}


def sanitizer_pass(audit):
    result = audit.get('host_audit', audit)
    require(result['mismatches'] == 0, 'Host audit mismatches')
    require(audit.get('validation', 'PASS') == 'PASS' and audit.get('sanitizer_findings', 0) == 0,
            'Failed host audit or sanitizer findings')
    flags = str(audit.get('flags', audit.get('sanitizers', ''))).lower()
    require((audit.get('address_sanitizer') and audit.get('undefined_behavior_sanitizer'))
            or ('address' in flags and 'undefined' in flags), 'ASan/UBSan proof missing')
    if 'source_sha256' in audit:
        source_bindings(audit['source_sha256'])
    return result


def smoke_record(path, apk_sha):
    result = read(path)
    require(result['validation'] == 'PASS', 'Smoke did not pass: ' + str(path))
    require(result['apk_sha256'] == result['installed_apk_sha256'] == apk_sha,
            'Smoke belongs to another APK: ' + str(path))
    require(not result.get('cleanup_error') and not result.get('touch_cleanup_error')
            and not result.get('rotation_cleanup_error'), 'Smoke cleanup failed')
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--studio', type=Path, required=True)
    parser.add_argument('--movement', type=Path, required=True)
    parser.add_argument('--lifecycle', type=Path, required=True)
    parser.add_argument('--combat', type=Path, required=True)
    parser.add_argument('--cache', type=Path, default=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'))
    parser.add_argument('--checkpoint', type=Path)
    parser.add_argument('--state-kernels', type=Path, default=REPORTS/'character-combat-kernel-validation.json')
    parser.add_argument('--playback-host', type=Path, default=REPORTS/'actor-playback-host-audit.json')
    parser.add_argument('--events-host', type=Path, default=REPORTS/'actor-playback-events-host-audit.json')
    parser.add_argument('--build-prefix', default='character-combat-final')
    parser.add_argument('--output', type=Path, default=REPORTS/'character-combat-source-validation.json')
    args = parser.parse_args()
    require(args.output.resolve().is_relative_to(REPO), 'Validation output must remain in workspace')
    report = {'validation': 'FAIL', 'scope': __doc__, 'goal_status': 'active'}
    try:
        apks = {'packaged': ROOT/'app/build/outputs/apk/debug/app-debug.apk',
                'studio': args.studio/'app/build/outputs/apk/debug/app-debug.apk'}
        hashes = {tag: digest(path) for tag, path in apks.items()}
        report['apk_sha256'] = hashes
        checkpoint = args.checkpoint or ROOT/f"build/checkpoints/dh2-native-character-combat-{hashes['packaged'][:8]}.apk"
        require(digest(checkpoint) == hashes['packaged'], 'Existing checkpoint differs from current repo APK')
        engine = (LOCAL/'libDungeonHunter2.so').read_bytes()
        require(sha(engine) == ORIGINAL, 'Original oracle ELF changed')
        captures = [capture_manifest(REPO/'port/level-world/reference/character-state/original-functions.json', engine),
                    capture_manifest(REPO/'port/level-world/reference/animation-blender/manifest.json', engine)]
        sources = {}
        main_root = ROOT/'app/src/main'
        studio_main = args.studio/'app/src/main'
        local_names = {item.relative_to(main_root).as_posix() for item in main_root.rglob('*') if item.is_file()}
        remote_names = {item.relative_to(studio_main).as_posix() for item in studio_main.rglob('*') if item.is_file()}
        excluded_templates = []
        # Studio's AGP scaffold can add this inactive R8 comment template.
        # Active rules, any other extra file, and all production files remain
        # strict mismatches. Record exclusions rather than silently ignoring it.
        for tag, base, names in (('packaged', main_root, local_names), ('studio', studio_main, remote_names)):
            template = 'keepRules/rules.keep'
            if template in names:
                path = base/template
                raw = path.read_bytes()
                lines = raw.decode('utf-8-sig').splitlines()
                require(all(not line.strip() or line.strip().startswith('#') for line in lines),
                        'Active or unknown R8 template content: ' + str(path))
                excluded_templates.append({'project': tag, 'path': str(path.resolve()),
                                           'relative_path': template, 'bytes': len(raw), 'sha256': sha(raw),
                                           'reason': 'Comment-only AGP R8 scaffold with no active rules; excluded from Debug compiled-source inventory.'})
                names.remove(template)
        report['excluded_comment_only_templates'] = excluded_templates
        require(local_names == remote_names, 'Repo/Studio main source/asset inventories differ')
        for name in sorted(local_names):
            if name.startswith('assets/'):
                continue
            raw = (main_root/name).read_bytes()
            require(raw == (studio_main/name).read_bytes(), 'Repo/Studio source differs: ' + name)
            sources[(main_root/name).relative_to(REPO).as_posix()] = sha(raw)
        for stem in ('character_state', 'actor_playback', 'actor_runtime', 'animation_blender',
                     'native_body', 'physical_world', 'actor_rotation', 'move_state'):
            for extension in ('hpp', 'cpp'):
                path = REPO/f'port/level-world/{stem}.{extension}'
                sources[path.relative_to(REPO).as_posix()] = digest(path)
        studio_source = args.studio/'reconstruction-source'
        require(studio_source.is_dir(), 'Missing Studio reconstruction-source dependency checkout')
        for folder in ('engine-textures', 'scene-materials', 'engine-animation', 'engine-skinning',
                       'game-data', 'level-world', 'physics-backend'):
            for path in (REPO/'port'/folder).rglob('*'):
                relative = path.relative_to(REPO)
                if not path.is_file() or any(part in ('build', 'tests', 'tools', 'reports', 'reference', '.cxx')
                                             for part in relative.parts):
                    continue
                if path.suffix not in ('.cpp', '.hpp', '.h', '.c') and path.name != 'CMakeLists.txt':
                    continue
                value = digest(path)
                require(digest(studio_source/relative) == value, 'Studio compiled dependency source differs: ' + relative.as_posix())
                sources[relative.as_posix()] = value
        libraries, assets, provenance = {}, {}, {}
        with zipfile.ZipFile(apks['packaged']) as packaged, zipfile.ZipFile(apks['studio']) as studio:
            require(set(packaged.namelist()) == set(studio.namelist()), 'APK member inventories differ')
            for tag, archive in (('packaged', packaged), ('studio', studio)):
                require(len(archive.namelist()) == len(set(archive.namelist())), 'Duplicate APK members')
                require(not any('DungeonHunter2.so' in name or name.startswith('lib/armeabi')
                                for name in archive.namelist()), 'ARM32 oracle/runtime bundled')
                rows = [library_record(archive, item, apks[tag]) for item in archive.infolist()
                        if item.filename.startswith('lib/') and item.filename.endswith('.so')]
                require(len(rows) == 14 and {row['path'].split('/')[1] for row in rows} == {'arm64-v8a', 'x86_64'},
                        'Unexpected native library inventory')
                libraries[tag] = {row['path']: row for row in rows}
                required = {'dh2_character_attack_speed', 'dh2_character_state_event',
                            'dh2_character_state_transition', 'dh2_character_state_update',
                            'dh2_blender_begin', 'dh2_blender_update_weights', 'dh2_blender_normalize',
                            'dh2_actor_event_handoff', 'dh2_actor_sequence_close'}
                for abi in ('arm64-v8a', 'x86_64'):
                    defined, _ = elf_symbols(archive.read(f'lib/{abi}/libdh2_level_world.so'))
                    require(required <= defined, 'Missing packaged state/event/weight exports')
                    _, imported = elf_symbols(archive.read(f'lib/{abi}/libdh2_native.so'))
                    require({'dh2_character_state_event', 'dh2_character_state_transition',
                             'dh2_character_state_update'} <= imported, 'Live application does not import source state coordinator')
                    require(any(name.startswith('_ZN3dh25actor8Playback4swap') for name in imported),
                            'Live application does not import native Playback Swap')
            for name in packaged.namelist():
                if not name.startswith('assets/') or name.endswith('/'):
                    continue
                key, raw = name[7:], packaged.read(name)
                require(raw == studio.read(name) == (main_root/'assets'/key).read_bytes()
                        == (studio_main/'assets'/key).read_bytes(), 'Authored asset differs: ' + key)
                assets[key] = {'bytes': len(raw), 'sha256': sha(raw)}
                if key.endswith('provenance.json'):
                    provenance[key] = json.loads(raw)
        require(len(assets) == 189 and set(assets) == {name[7:] for name in local_names if name.startswith('assets/')},
                'Expected exactly189 authored assets with matching inventory')
        prior_path = REPORTS/'live-actor-source-validation.json'
        prior = read(prior_path)
        for name, record in prior['assets_verified'].items():
            require(assets[name] == record, 'Earlier authored asset changed: ' + name)
        combat_manifest = provenance['character-combat-provenance.json']
        additions = ({item['asset'] for item in combat_manifest['clips']} - set(prior['assets_verified'])) | {'character-combat-provenance.json'}
        require(set(assets)-set(prior['assets_verified']) == additions, 'Unaccounted asset addition')
        cache_proof = verify_authored_cache(args.cache, assets, provenance)
        kernels = read(args.state_kernels)
        require(kernels['validation'] == 'PASS' and kernels['apk_sha256'] == hashes
                and kernels['original_sha256'] == ORIGINAL, 'Packaged kernel report is stale')
        source_bindings(kernels['source_sha256'])
        golds = {'character-state': (3910, REPO/'port/level-world/reference/character-state/state-reference.bin'),
                 'animation-blender': (2504, LOCAL/'animation-blender-discovery/reference.bin')}
        for tag, modules in kernels['packaged_differentials'].items():
            require(tag in hashes and set(modules) == set(golds), 'Unexpected packaged kernel modules')
            library_sha = libraries[tag]['lib/arm64-v8a/libdh2_level_world.so']['sha256']
            for module, (count, gold) in golds.items():
                item = modules[module]
                path = REPO/item['report']
                leaf = read(path)
                require(digest(path) == item['report_sha256'] and leaf == item['differential'], 'Kernel leaf binding differs')
                require(item['library_sha256'] == leaf['arm64_library_sha256'] == library_sha,
                        'Kernel belongs to another packaged library')
                require(item['reference_sha256'] == leaf['reference_sha256'] == digest(gold), 'Kernel corpus changed')
                require(item['comparisons'] == leaf['comparisons'] == count and leaf['mismatches'] == 0
                        and leaf['original_sha256'] == ORIGINAL, 'Kernel count/original/mismatch failure')
                require(leaf['actual_packaged_library_executed'] and leaf['original_gold_replayed']
                        and not leaf['original_instructions_executed_this_run'], 'Misstated packaged replay provenance')
                source_bindings(leaf['source_sha256'])
                evidence = leaf['original_instruction_evidence']
                original_path = REPO/evidence['original_report']
                original_audit = read(original_path)
                require(digest(original_path) == evidence['original_report_sha256']
                        and digest(REPO/evidence['reference']) == evidence['reference_sha256'] == digest(gold)
                        and original_audit['original_sha256'] == ORIGINAL
                        and original_audit['reference_sha256'] == digest(gold)
                        and original_audit['comparisons'] == count and original_audit['mismatches'] == 0,
                        'Original instruction evidence binding differs')
                if module == 'character-state':
                    require(original_audit['kernel_source_sha256'] == digest(REPO/'port/level-world/character_state.cpp'),
                            'State source differs from original instruction audit')
                    require(leaf['ordered_service_requests'] == 8773 and leaf['malformed_no_mutation_cases'] == 21
                            and leaf['operation_counts'] == {'0': 937, '1': 975, '2': 906, '3': 1010, '4': 82},
                            'Missing ordered source-state service/ABI replay')
                else:
                    require(leaf['atomic_rejection_checks'] == 18
                            and leaf['operation_counts'] == {'0': 613, '1': 685, '2': 526, '3': 680},
                            'Missing blender metadata/ABI replay')
        require(set(kernels['packaged_differentials']) == set(hashes), 'Both APK kernel proofs required')
        hosts = {}
        host_paths = {'character-state': REPORTS/'character-state-host-sanitizers.json',
                      'animation-blender': REPORTS/'animation-blender-host-audit.json',
                      'actor-playback': args.playback_host, 'actor-playback-events': args.events_host,
                      'actor-runtime': REPORTS/'actor-runtime-host-audit.json'}
        for name, path in host_paths.items():
            audit = read(path)
            result = sanitizer_pass(audit)
            hosts[name] = binding(path, audit)
            if name in golds:
                count, gold = golds[name]
                require(audit['reference_sha256'] == digest(gold), 'Host kernel corpus differs')
                require(result.get('original_reference_cases', result.get('original_derived_replay')) == count, 'Host corpus count differs')
        state_host = hosts['character-state']['result']
        require(state_host['ordered_service_requests'] == 8773 and state_host['malformed_no_mutation_cases'] == 21
                and state_host['synchronous_reentry_passed'], 'State ABI/order/reentry audit incomplete')
        playback = hosts['actor-playback']['result']
        source_bindings(playback['source_sha256'])
        require(playback['original_instruction_evidence']['original_sha256'] == ORIGINAL
                and playback['original_instruction_evidence']['corpus_sha256'] == digest(REPO/'port/level-world/reference/actor-playback/composition-fixtures.bin'),
                'Playback original phase corpus changed')
        events = hosts['actor-playback-events']['result']
        source_bindings(events['source_sha256'])
        event_gold = REPO/'port/level-world/reference/actor-playback-events/event-fixtures.bin'
        require(events['original_instruction_evidence']['original_sha256'] == ORIGINAL
                and events['original_instruction_evidence']['corpus_sha256'] == digest(event_gold),
                'Playback event/Swap original corpus changed')
        event_host = events.get('host_audit', events)
        require(event_host['original_swap_cases'] == 144 and event_host['metadata_swaps'] > 0
                and event_host['sequence_closures'] > 0 and event_host['retained_batch_callbacks'] > 0
                and event_host['overshoot_replays'] > 0, 'Playback Swap/closure/reentry audit incomplete')
        movement = smoke_record(args.movement, hashes['packaged'])
        require(all(movement[key] for key in ('scene_then_step_then_actor_markers', 'live_touch_movement',
                    'release_returns_to_authored_idle', 'body_coordinates_match_game')), 'Movement pipeline smoke incomplete')
        require({row['state'] for row in movement['movement']} == {'Walk', 'Run'}, 'Walk/Run smoke cases missing')
        lifecycle = smoke_record(args.lifecycle, hashes['packaged'])
        require(all(lifecycle[key] for key in ('rotation_position_preserved', 'context_counter_resets_verified',
                    'pause_cancels_held_input')) and not lifecycle['unattended_resume_movement']
                and len(lifecycle['rotation_cases']) == 2, 'Lifecycle smoke incomplete')
        combat = smoke_record(args.combat, hashes['packaged'])
        require(all(combat[key] for key in ('enemy_ai_disabled', 'finite_source_idle_closure',
                    'moving_attack_displacement_preserved', 'legacy_second_cursor_completion_absent',
                    'out_of_reach_ui_rejected')), 'Combat smoke incomplete')
        require(set(combat['cases']) == {'stationary', 'moving'}, 'Both combat smoke cases required')
        spec = importlib.util.spec_from_file_location('character_combat_smoke', Path(__file__).with_name('character_combat_smoke.py'))
        smoke_parser = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(smoke_parser)
        combat_logs = {}
        for label, predecessor, root in (('stationary', 3, 243), ('moving', 4, 248)):
            path = args.combat.parent/(label+'.log')
            text = path.read_text()
            require(not smoke_parser.ERROR.search(text), 'Runtime error in combat raw log')
            require(smoke_parser.verify_case(text, predecessor, root, combat['target']['name']) == combat['cases'][label],
                    'Combat report does not match raw state/event/hit log')
            require(not smoke_parser.HIT.search(text[combat['cases'][label]['source_end']['offset']:]), 'Damage after source Idle closure')
            image = args.combat.parent/(label+'-idle.png')
            require(image.read_bytes().startswith(b'\x89PNG\r\n\x1a\n'), 'Missing combat capture')
            combat_logs[label] = {'log_sha256': digest(path), 'screenshot_sha256': digest(image)}
        require(any(hit['hp_after'] < hit['hp_before'] for item in combat['cases'].values() for hit in item['native_hits']), 'Combat did not apply native HP damage')
        builds = {}
        for tag in ('repo', 'studio'):
            path = LOCAL/f'{args.build_prefix}-{tag}-build.log'
            raw = path.read_bytes()
            text = raw.decode('utf-16') if raw[:2] in (b'\xff\xfe', b'\xfe\xff') else raw.decode('utf-8-sig')
            require('BUILD SUCCESSFUL' in text, 'Final build did not pass: ' + tag)
            builds[tag] = {'path': str(path.resolve()), 'sha256': sha(raw)}
        require(hashes == {tag: digest(path) for tag, path in apks.items()}, 'APK changed during validation')
        source_bindings(sources)
        require(all(digest(Path(item['path'])) == item['sha256'] for item in excluded_templates),
                'Excluded comment-only template changed during validation')
        report.update(validation='PASS', libraries=libraries, assets_verified=assets,
                      source_sha256=sources, original_captures=captures, original_cache=cache_proof,
                      kernel_validation=binding(args.state_kernels, kernels), host_audits=hosts,
                      movement=binding(args.movement, movement), lifecycle=binding(args.lifecycle, lifecycle),
                      combat=binding(args.combat, combat), combat_capture_bindings=combat_logs, build_logs=builds,
                      checkpoint={'path': str(checkpoint.resolve()), 'sha256': digest(checkpoint), 'bytes': checkpoint.stat().st_size},
                      prior_live_actor={'report_sha256': digest(prior_path), 'apk_sha256': prior['apk_sha256'],
                                        'scope': 'Historical preserved-asset evidence; its runtime/kernel tests were not freshly replayed here.'},
                      validator_sha256=digest(Path(__file__)), source_sync_verified=True,
                      excluded_comment_only_templates=excluded_templates,
                      apk_16k_zip_and_elf_alignment_verified=True, physical_arm64_tested=False,
                      original_gpu_parity_verified=False, full_game_playable=False,
                      live_source_states=[3, 4, 5, 12], supplied_equipment_stance=0,
                      debug_attack_setup='Debug-build receiver com.example.dh2.DEBUG_PLAYER_ATTACK requires android.permission.DUMP and calls the same native playerAttack bridge without Activity pause or state/position injection.',
                      remaining_boundaries=['Single-slot live Playback; recovered two-slot fade metadata does not implement full typed pose blending.',
                          'Bounded Idle/Move/Attack/Dead states; full Character/Prince AI, general timers and remaining states/services are pending.',
                          'Supplied unarmed stance0 and base properties; original equipment/save/input/target/camera producers remain incomplete.',
                          'Death source is integrated but is not exercised by these two attack smoke cases.',
                          'No whole original frame, original GPU, physical ARM64 device or full-game parity claim.'])
    except Exception as error:
        report['error'] = str(error)
        raise
    finally:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({'validation': 'PASS', 'apk_sha256': report['apk_sha256'], 'assets': 189,
                      'combat_clips': 64, 'source_cases_per_apk': 6414, 'full_game_playable': False}))


if __name__ == '__main__':
    main()
