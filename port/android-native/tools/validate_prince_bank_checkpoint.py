"""Validate frozen Prince-bank APK/build inputs; live checks are separate.

Read-only except for a NEW output report. No builds, instruction execution, ADB,
installation or gameplay are performed. PAB1 is a port metadata format, not an
original engine format; its ordered requests bind the original registration
probe. Host proofs are not packaged instruction or full-bank pose proofs.
"""
import argparse
from io import BytesIO
import json
from pathlib import Path
import re
import struct
import zipfile
from validate_character_combat_source import (
    REPO, ORIGINAL, CACHE, require, sha, digest, read, binding, elf_symbols,
    smoke_record)

ASSET_ROOT = REPO / 'port/android-native/app/src/main/assets'
MANIFEST = ASSET_ROOT / 'data/prince-animation-bank.json'
EXPECTED_MANIFEST = '76633bb4f0ab5f645f4516e407e1f926df61641ea66faa805f68da57f043eb81'
EXPECTED_METADATA = 'c0cf8bfbb804256050e0d1ac326b8388b472c9b3034c8366c992f2056f477b0b'
PROBE = REPO / 'port/engine-animation/reference/prince-registration/probe.json'
ABIS = ('arm64-v8a', 'x86_64')
LIBRARIES = {
    'libdh2_native.so', 'libdh2_level_world.so', 'libdh2_game_data.so',
    'libdh2_engine_animation.so', 'libdh2_scene_materials.so',
    'libdh2_engine_skinning.so', 'libdh2_engine_textures.so'}


def metadata(raw):
    """Independent exact PAB1 reader; compared to the native reader's proof."""
    at = 0
    def take(n):
        nonlocal at
        require(n >= 0 and at+n <= len(raw), 'Truncated PAB1')
        result = raw[at:at+n]; at += n
        return result
    def word():
        return struct.unpack('<I', take(4))[0]
    def text():
        size = word()
        require(0 < size <= 4096, 'Invalid PAB1 text size')
        payload = take(size)
        require(all(32 <= x <= 126 for x in payload), 'Invalid PAB1 text')
        return payload.decode('ascii')
    def hashword():
        result = take(32)
        require(any(result), 'Empty PAB1 hash')
        return result.hex()
    require(0 < len(raw) <= 2*1024*1024, 'Invalid PAB1 span')
    require((word(), word(), word()) == (0x31424150, 1, len(raw)), 'PAB1 header differs')
    policy, table, set_id, template, nr, nq = (word() for _ in range(6))
    require(policy == 1 and 0 < nr <= 65536 and nr <= nq <= 65536, 'PAB1 policy/counts')
    result = {'identity_policy': policy, 'animation_table': table,
              'animation_set_id': set_id, 'template_clip_id': template}
    for key in ('manifest_sha256', 'cache_sha256', 'original_sha256', 'producer_sha256'):
        result[key] = hashword()
    result['character'] = text()
    rows = []
    for _ in range(nr):
        row = {'clip_id': word(), 'bytes': word(), 'sha256': hashword(),
               'authored_path': text(), 'asset': text(), 'cache_entry': text()}
        require(row['clip_id'] <= 0x7fffffff and 0 < row['bytes'] <= 64*1024*1024, 'PAB1 resource size/ID')
        for key in ('authored_path', 'asset', 'cache_entry'):
            path = row[key]
            require('\\' not in path and ':' not in path and all(part not in ('', '.', '..') for part in path.split('/')), 'Invalid PAB1 resource path')
        rows.append(row)
    for key in ('clip_id', 'authored_path', 'asset', 'cache_entry'):
        require(len({row[key] for row in rows}) == nr, 'Duplicate PAB1 '+key)
    requests = [word() for _ in range(nq)]
    first = list(dict.fromkeys(requests))
    require(first == [row['clip_id'] for row in rows] and requests[0] == template and at == len(raw), 'PAB1 order/coverage/trailing bytes')
    result.update(resources=rows, registration_requests=requests, first_unique_resource_order=first)
    return result


def native_library(apk, info, apk_raw):
    raw = apk.read(info)
    abi = info.filename.split('/')[1]
    require(abi in ABIS and raw[:6] == b'\x7fELF\x02\x01' and len(raw) >= 64, 'Expected native little-endian ELF64')
    require(struct.unpack_from('<H', raw, 18)[0] == {'arm64-v8a': 183, 'x86_64': 62}[abi], 'ELF machine/ABI mismatch')
    offset = struct.unpack_from('<Q', raw, 32)[0]
    stride, count = struct.unpack_from('<HH', raw, 54)
    require(stride >= 56 and count and offset+stride*count <= len(raw), 'Invalid ELF program headers')
    segments = []
    for i in range(count):
        kind, flags, file_at, address, _, file_size, memory_size, align = struct.unpack_from('<IIQQQQQQ', raw, offset+i*stride)
        if kind != 1:
            continue
        require(align >= 16384 and align & (align-1) == 0 and file_at % align == address % align, 'ELF LOAD alignment/congruence differs')
        require(file_size <= memory_size and file_at+file_size <= len(raw), 'Invalid ELF LOAD range')
        segments.append({'file_offset': file_at, 'virtual_address': address, 'alignment': align, 'flags': flags})
    require(segments, 'ELF has no LOAD segments')
    local = info.header_offset
    require(apk_raw[local:local+4] == b'PK\x03\x04' and local+30 <= len(apk_raw), 'Invalid APK local header')
    name_bytes, extra_bytes = struct.unpack_from('<HH', apk_raw, local+26)
    data_offset = local+30+name_bytes+extra_bytes
    require(info.compress_type == zipfile.ZIP_STORED and data_offset % 16384 == 0, 'Native APK entry lacks16KiB ZIP alignment')
    return {'sha256': sha(raw), 'bytes': len(raw), 'machine': {'arm64-v8a': 183, 'x86_64': 62}[abi],
            'elf_class': 64, 'load_segments': segments, 'apk_data_offset': data_offset}


def proof_sources(proof, frozen):
    records = proof.get('source_sha256', {})
    require(records, 'Proof source bindings absent')
    bound, supplemental = {}, {}
    for name, expected in records.items():
        if name in frozen:
            require(frozen[name] == expected, 'Proof differs from frozen compiler source: '+name)
            bound[name] = expected
        else:
            path = (REPO/name).resolve()
            require(path.is_relative_to(REPO) and path.is_file() and digest(path) == expected, 'Uncaptured proof source changed: '+name)
            supplemental[name] = expected
    return {'captured_source_sha256': bound, 'supplemental_current_source_sha256': supplemental}


def validate_idle_recreation(recreation):
    # Final smoke contract: [state,sequence,clip], with source-focus fields
    # outside the array. Idle does not use the Attack/Dead explicit restart log.
    restart = recreation['restart']
    require(isinstance(restart, list) and len(restart) == 3 and
            all(isinstance(value, int) for value in restart), 'Bank smoke restart contract differs')
    require(recreation['new_context'] > recreation['previous_context'] and
            recreation['first_frame']['actor_frames'] == 1, 'Bank smoke recreation counters differ')
    require(restart[0] == 3 and restart[1] == recreation['saved_sequence'] and
            restart[1] >= 0 and restart[2] in (1040, 1041), 'Bank smoke saved Idle sequence differs')
    require(int(recreation['flags'], 16) == 0x2380 and
            recreation['service'] == 'actor state service' and
            recreation['restart_source'].startswith('Character animation selected:'),
            'Bank smoke source Idle focus flags/service differ')


def validate_occurrence_proof(proof):
    host = proof['host_audit']
    require(proof['validation'] == host['validation'] == 'PASS' and
            proof['sanitizer_findings'] == host['sanitizer_findings'] == 0,
            'Occurrence host proof failed')
    expected = {'resources': 116, 'occurrences': 158, 'constructor_slot_checks': 2,
                'original_bounds_checks': 158, 'source_mapped_selections': 17,
                'original_playclip_comparisons': 72, 'root_history_checks': 102,
                'zero_track_engine': 155, 'atomic_rejections': 11}
    require(all(host.get(key) == value for key, value in expected.items()), 'Occurrence host corpus incomplete')
    require(host.get('slot0_authored', 0) >= 1 and host.get('slot1_authored', 0) >= 1 and
            host.get('registration_destroyed_before_playback') is True,
            'Occurrence both-slot/registration lifetime proof incomplete')
    require(proof['original_sha256'] == ORIGINAL and proof['original_instruction_cases'] == 72 and
            proof.get('sources_unchanged_through_replay') is True, 'Occurrence original/source binding differs')
    require({'AddressSanitizer', 'UndefinedBehaviorSanitizer'} <= set(proof['sanitizers']), 'Occurrence sanitizers absent')
    regressions = proof['legacy_regression']
    require(set(regressions) == {'static', 'dynamic', 'scheduler', 'selection', 'controls'} and
            all(row['validation'] == 'PASS' and row['mismatches'] == 0 for row in regressions.values()),
            'Occurrence legacy original-bound regression failed')


def validate(a):
    require(digest(MANIFEST) == EXPECTED_MANIFEST, 'Canonical Prince JSON changed')
    canonical = read(MANIFEST)
    probe = read(PROBE)
    require(probe['validation'] == 'PASS' and digest(PROBE) == canonical['producer_sha256'], 'Original registration producer binding differs')
    require(canonical['original_sha256'] == probe['original_sha256'] == ORIGINAL and digest(REPO/'.local-inputs/libDungeonHunter2.so') == ORIGINAL, 'Original oracle changed')
    require(canonical['cache_sha256'] == probe['cache_sha256'] == CACHE, 'Original cache identity differs')
    require(canonical['registration_requests'] == [row['clip_id'] for row in probe['registration_calls']], 'Source158 request order differs')
    require(len(canonical['resources']) == 116 and len(canonical['registration_requests']) == 158, 'Prince bank cardinality differs')
    projection = probe['current17_projection']
    require(len(projection) == 17 and all(canonical['registration_requests'].index(row['clip_id']) == row['first_library_index'] for row in projection), 'Original17 first indices differ')
    for row, original in zip(canonical['resources'], probe['resources']):
        require((row['clip_id'], row['bytes'], row['sha256'], row['authored_path'], row['cache_entry']) ==
                (original['clip_id'], original['bytes'], original['sha256'], original['path'], original['entry']), 'Original resource record differs')
    report = {'scope': __doc__, 'goal_status': 'active', 'physical_arm64_verified': False,
              'full_game_verified': False, 'full_bank_original_pose_parity': False,
              'original_sha256': ORIGINAL, 'cache_sha256': CACHE,
              'original_registration_probe': binding(PROBE), 'original_current17_projection': projection}
    with zipfile.ZipFile(a.capture) as capture:
        capture_names = capture.namelist()
        require(len(capture_names) == len(set(capture_names)) and capture.testzip() is None, 'Capture duplicate/corrupt entries')
        frozen = json.loads(capture.read('build-capture.json'))
        require(frozen['validation'] == 'BUILD_INPUTS_CAPTURED', 'Wrong compiler capture')
        for name, expected in frozen['entries'].items():
            raw = capture.read(name)
            require(len(raw) == expected['bytes'] and sha(raw) == expected['sha256'], 'Capture entry changed: '+name)
        sources = frozen['source_sha256']
        expected_libraries = set(LIBRARIES)
        script_timer_binding = None
        if a.script_timer_host:
            timer_proof = read(a.script_timer_host)
            audit = timer_proof['host_audit']
            require(timer_proof['validation'] == audit['validation'] == 'PASS' and timer_proof['sanitizer_findings'] == 0, 'Native script/timer host audit failed')
            require(audit['source_ai_event35_composed'] and audit['inactive_ais_gate_verified'] and audit['actual_common_loaded'], 'Native timer dispatch composition missing')
            require((audit['timer_expiries'], audit['actual_timer_id_reads'], audit['selected_ais_vm_calls']) == (7, 7, 6), 'Native timer composition corpus differs')
            require(audit['native_storage_failures_protected'] and audit['owner_drift_rejected'] and audit['return_projection_timer_start'], 'Native timer error/projection checks missing')
            require(audit['whole_ai_ownership_claimed'] is False, 'Timer host proof scope exceeds fixture')
            require({'AddressSanitizer', 'UndefinedBehaviorSanitizer'} <= set(timer_proof['sanitizers']), 'Script/timer sanitizers missing')
            script_timer_binding = proof_sources(timer_proof, sources)
            require({'port/level-world/character_script_timers.cpp', 'port/level-world/character_ai_events.cpp', 'port/script-runtime/script_runtime.c', 'port/script-runtime/script_game_bindings.c', 'port/script-runtime/lua/lvm.c'} <= sources.keys(), 'Native script/timer compiler inputs missing')
            expected_libraries.add('libdh2_script_runtime.so')
        for name, expected in sources.items():
            require(sha(capture.read('source/'+name)) == expected, 'Frozen source entry changed: '+name)
        required_inputs = {
            'port/android-native/app/src/main/cpp/model_renderer.cpp',
            'port/game-data/animation_bank.cpp', 'port/game-data/animation_bank.hpp',
            'port/level-world/actor_blended_playback.cpp', 'port/level-world/actor_blended_playback.hpp',
            'port/engine-animation/animation.cpp', 'port/engine-animation/animation.hpp',
            'port/engine-animation/animation_registration.cpp', 'port/engine-animation/animation_registration.hpp',
            'port/engine-animation/angle_interpreter.cpp', 'port/engine-animation/animation_blend.cpp',
            'port/level-world/animation_blender.cpp', 'port/scene-materials/scene.cpp',
            'port/engine-math/math.cpp'}
        commands = frozen['compiler_inputs']
        for tag in ('packaged', 'studio'):
            for abi in ABIS:
                row = commands[tag][abi]; inputs = row['repository_inputs']
                require(required_inputs <= inputs.keys(), 'Missing actual bank/blended compiler dependency: '+tag+'/'+abi+' '+str(sorted(required_inputs-inputs.keys())))
                require(all(sources[name] == value for name, value in inputs.items()), 'Compiler source capture mismatch')
                require(sha(capture.read(f'compiler/{tag}-{abi}-commands.json')) == row['database_sha256'], 'Compiler command capture changed')
                require(sha(capture.read(f'compiler/{tag}-{abi}-dependencies.txt')) == row['ninja_dependencies_sha256'], 'Ninja dependency capture changed')
        require(commands['packaged']['arm64-v8a']['repository_inputs'] == commands['packaged']['x86_64']['repository_inputs'], 'ABI compiler source sets differ')
        for abi in ABIS:
            require(commands['packaged'][abi]['repository_inputs'] == commands['studio'][abi]['repository_inputs'], 'Repo/Studio compiler source bindings differ')
        hashes = {tag: frozen['apks'][tag]['sha256'] for tag in ('packaged', 'studio')}
        require(digest(a.checkpoint) == hashes['packaged'], 'Checkpoint APK differs from frozen build')
        libraries, all_assets, bank_records = {}, {}, {}
        for tag in ('packaged', 'studio'):
            apk_raw = capture.read(tag+'-app-debug.apk')
            require(sha(apk_raw) == hashes[tag] and len(apk_raw) == frozen['apks'][tag]['bytes'], 'Frozen APK changed: '+tag)
            with zipfile.ZipFile(BytesIO(apk_raw)) as apk:
                names = apk.namelist()
                require(len(names) == len(set(names)) and apk.testzip() is None, 'APK duplicate/corrupt entries')
                require(not any('dungeonhunter2.so' in name.lower() for name in names), 'Original engine bundled')
                native = {info.filename: info for info in apk.infolist() if info.filename.startswith('lib/') and not info.is_dir()}
                require(set(native) == {f'lib/{abi}/{name}' for abi in ABIS for name in expected_libraries}, 'Native inventory differs; ARM32/other ABI forbidden')
                libraries[tag] = {name: native_library(apk, info, apk_raw) for name, info in native.items()}
                for abi in ABIS:
                    data_symbols, _ = elf_symbols(apk.read(f'lib/{abi}/libdh2_game_data.so'))
                    world_symbols, _ = elf_symbols(apk.read(f'lib/{abi}/libdh2_level_world.so'))
                    _, native_imports = elf_symbols(apk.read(f'lib/{abi}/libdh2_native.so'))
                    for kernel in ('load_animation_bank', 'animation_resource', 'animation_resource_index', 'animation_resource_identity'):
                        require(any(kernel in name for name in data_symbols), 'Metadata reader/export absent: '+kernel)
                    require(any('BlendedPlayback' in name and 'compile_dynamic' in name and 'RegistrationSet' in name for name in world_symbols), 'Occurrence coordinator bridge missing')
                    require(any('load_animation_bank' in name for name in native_imports) and any('BlendedPlayback' in name for name in native_imports), 'Application does not import metadata/blended APIs')
                    if a.script_timer_host:
                        script_symbols, _ = elf_symbols(apk.read(f'lib/{abi}/libdh2_script_runtime.so'))
                        _, world_imports = elf_symbols(apk.read(f'lib/{abi}/libdh2_level_world.so'))
                        require({'dh2_character_script_bind_timers', 'dh2_character_ai_event', 'dh2_character_ai_event_script_timer'} <= world_symbols, 'World script/timer exports missing')
                        require({'dh2_script_vm_create', 'dh2_script_vm_call_discard_source', 'dh2_script_game_on_timer'} <= script_symbols, 'Source Lua exports missing')
                        require({'dh2_script_game_bind', 'dh2_script_vm_bind_source_values'} <= world_imports, 'World does not import actual source Lua bindings')
                current = {name[7:]: {'bytes': len(apk.read(name)), 'sha256': sha(apk.read(name))} for name in names if name.startswith('assets/') and not name.endswith('/')}
                all_assets[tag] = current
                manifest_raw = apk.read('assets/data/prince-animation-bank.json')
                require(sha(manifest_raw) == EXPECTED_MANIFEST and manifest_raw == MANIFEST.read_bytes(), 'Packaged canonical JSON differs')
                metadata_raw = apk.read('assets/data/prince-animation-bank.bin')
                require(len(metadata_raw) == 31839 and sha(metadata_raw) == EXPECTED_METADATA, 'Packaged PAB1 differs')
                decoded = metadata(metadata_raw)
                for key, value in canonical.items():
                    if key != 'scope':
                        require(decoded[key] == value, 'PAB1/source field differs: '+key)
                require(decoded['manifest_sha256'] == EXPECTED_MANIFEST and decoded['identity_policy'] == 1, 'PAB1 source/identity binding')
                verified = {}
                for row in canonical['resources']:
                    raw = apk.read('assets/'+row['asset'])
                    require(len(raw) == row['bytes'] and sha(raw) == row['sha256'], 'Packaged resource differs: '+row['asset'])
                    verified[row['asset']] = {'bytes': len(raw), 'sha256': sha(raw)}
                model = probe['model_property']; model_raw = apk.read('assets/models/prince_modular.bdae')
                require(len(model_raw) == model['bytes'] and sha(model_raw) == model['sha256'], 'Authored35-node Prince model differs')
                bank_records[tag] = {'resources': verified, 'registration_requests': decoded['registration_requests'],
                                     'template_clip_id': decoded['template_clip_id'], 'identity_policy': 1,
                                     'metadata_sha256': sha(metadata_raw), 'manifest_sha256': sha(manifest_raw)}
        require(all_assets['packaged'] == all_assets['studio'], 'Repo/Studio authored asset sets differ')
        event_instruction_binding = None
        if a.ai_event_arm64_proof:
            event = read(a.ai_event_arm64_proof)
            require(event['validation'] == 'PASS' and event['original_sha256'] == ORIGINAL and event['mismatches'] == 0, 'Packaged AI event instruction proof failed')
            require((event['comparisons'], event['ordered_service_requests'], event['composed_script_timer_cases'], event['original_reentry_cases']) == (11344, 13381, 100, 6), 'Packaged AI event instruction corpus differs')
            require(event['library_sha256'] == libraries['packaged']['lib/arm64-v8a/libdh2_level_world.so']['sha256'], 'AI event proof does not execute actual packaged ARM64 world library')
            event_instruction_binding = proof_sources(event, sources)
        metadata_proof = read(a.metadata_proof)
        host = metadata_proof['host_audit']
        require(metadata_proof['validation'] == host['validation'] == 'PASS' and host['mismatches'] == metadata_proof['sanitizer_findings'] == 0, 'Native metadata host proof failed')
        require((host['resources'], host['registration_requests'], host['lookup_checks'], host['atomic_rejection_checks'], host['owned_input_checks']) == (116, 158, 277, 1918, 1), 'Native metadata corpus incomplete')
        require(metadata_proof['source_manifest_sha256'] == EXPECTED_MANIFEST and metadata_proof['binary_sha256'] == EXPECTED_METADATA and metadata_proof['source_resource_records_compared'] == 116 and metadata_proof['source_ordered_requests_compared'] == 158, 'Native metadata/source report differs')
        require({'AddressSanitizer', 'UndefinedBehaviorSanitizer'} <= set(metadata_proof['sanitizers']), 'Metadata sanitizers missing')
        require(metadata_proof['original_registration_producer']['sha256'] == digest(PROBE), 'Native metadata source producer differs')
        metadata_binding = proof_sources(metadata_proof, sources)
        bank_host = read(a.bank_host); checks = bank_host['checks']
        require(bank_host['validation'] == checks['validation'] == 'PASS' and checks['sanitizer_findings'] == 0, 'Full-bank host proof failed')
        require((checks['resources'], checks['registration_requests'], checks['original_current17_mappings'], checks['compiled_clips'], checks['ordered_targets'], checks['samples']) == (116, 158, 17, 158, 83, 26228), 'Full-bank host corpus differs')
        require(checks['sample_after_player_destruction'] and checks['default_designation_does_not_append'] and bank_host['manifest_sha256'] == EXPECTED_MANIFEST and bank_host['original_registration_probe']['sha256'] == digest(PROBE), 'Full-bank lifetime/source binding differs')
        bank_binding = proof_sources(bank_host, sources)
        occurrence = read(a.occurrence_host)
        validate_occurrence_proof(occurrence)
        occurrence_binding = proof_sources(occurrence, sources)
        live, live_checks = {}, {}
        for label in ('movement', 'lifecycle', 'combat'):
            path = getattr(a, label)
            if path:
                result = smoke_record(path, hashes['packaged'])
                live[label] = binding(path)
                live_checks[label] = result
        if 'movement' in live_checks:
            require(all(live_checks['movement'].get(key) is True for key in ('scene_then_step_then_actor_markers', 'live_touch_movement', 'release_returns_to_authored_idle', 'body_coordinates_match_game')), 'Movement smoke incomplete')
        if 'lifecycle' in live_checks:
            require(all(live_checks['lifecycle'].get(key) is True for key in ('rotation_position_preserved', 'context_counter_resets_verified', 'pause_cancels_held_input')) and live_checks['lifecycle'].get('unattended_resume_movement') is False, 'Lifecycle smoke incomplete')
        if 'combat' in live_checks:
            require(live_checks['combat'].get('finite_source_idle_closure') is True and set(live_checks['combat'].get('cases', {})) == {'stationary', 'moving'} and live_checks['combat'].get('moving_attack_displacement_preserved') is True, 'Combat smoke incomplete')
        bank_smoke_assets = {}
        if a.bank_smoke:
            smoke = smoke_record(a.bank_smoke, hashes['packaged'])
            require(not any(value for key, value in smoke.items() if key.endswith('cleanup_error')), 'Bank smoke cleanup failed')
            require(str(smoke.get('serial', '')).startswith('emulator-') and smoke.get('install_requested') is False, 'Expected named emulator/externally installed bank smoke')
            require(smoke['bank_manifest_sha256'] == EXPECTED_MANIFEST and smoke['bank_binary_sha256'] == EXPECTED_METADATA and
                    (smoke['packaged_resource_count'], smoke['packaged_registration_count']) == (116, 158), 'Bank smoke metadata identity differs')
            require(all(smoke.get(key) is True for key in ('freeze_body_pose_clocks_preserved', 'resume_advances', 'touch_movement_verified', 'finite_attack_closure_observed')), 'Bank smoke recorded checks incomplete')
            require(smoke.get('observed_slots') == [0, 1] and smoke.get('actor_phase') == 5 and smoke.get('bank_ready_markers', 0) >= 2 and smoke.get('context_count', 0) >= 2 and smoke.get('blended_event_markers', 0) > 0, 'Bank smoke slots/context/phase evidence incomplete')
            require(all(smoke.get(key) is False for key in ('physical_arm64_phone_tested', 'original_full_ai_verified', 'original_full_frame_parity_verified', 'full_game_playable')), 'Bank smoke claims exceed bounded checkpoint')
            require({row['state'] for row in smoke['movement']} == {'Walk', 'Run'}, 'Bank smoke missing Walk/Run')
            snapshots = smoke['freeze_snapshots']
            require(len(snapshots) == 2 and len(snapshots[0]) == 9 and snapshots[0] == snapshots[1] and str(snapshots[0][0]) == '1', 'Bank smoke frozen snapshots differ')
            recreation = smoke['recreation']
            validate_idle_recreation(recreation)
            attack = smoke['attack_events']
            require(any(int(row[0], 16) == 0x28 for row in attack) and any(int(row[0], 16) == 0x22 for row in attack), 'Bank smoke trigger/closure absent')
            root = a.bank_smoke.resolve().parent
            for row in smoke['screenshots']:
                path = (root/row['path']).resolve()
                require(path.is_relative_to(root) and digest(path) == row['sha256'], 'Bank smoke screenshot binding differs')
                bank_smoke_assets[row['path']] = {'sha256': digest(path), 'bytes': path.stat().st_size}
            require(len(bank_smoke_assets) >= 6, 'Bank smoke screenshot set incomplete')
            for name in ('prince-bank.log', 'adb-transcript.json'):
                path = root/name
                require(path.is_file(), 'Bank smoke raw artifact missing: '+name)
                bank_smoke_assets[name] = {'sha256': digest(path), 'bytes': path.stat().st_size}
            ready_rows = re.findall(r'Prince blended bank ready \| resources (\d+) \| occurrences (\d+) \| targets (\d+) \| template (-?\d+) \| engine (-?\d+) \| game clip (-?\d+)', (root/'prince-bank.log').read_text(encoding='utf-8'))
            require(len(ready_rows) == smoke['bank_ready_markers'] and len(ready_rows) >= 2 and
                    all(tuple(map(int, row)) == (116, 158, 83, 1111, 0, 1111) for row in ready_rows), 'Raw bank library0/default readiness differs')
            if 'bank_ready' in smoke:
                require(smoke['bank_ready'] == {'resources': 116, 'occurrences': 158, 'targets': 83, 'template': 1111, 'engine': 0, 'game_clip': 1111}, 'Bank smoke readiness fields differ')
            live['bank'] = binding(a.bank_smoke)
        if a.require_live:
            require(set(live) == {'movement', 'lifecycle', 'combat', 'bank'}, 'All4 live reports required')
        changed = [name for name, expected in sources.items() if not (REPO/name).is_file() or digest(REPO/name) != expected]
        report.update(validation='PASS', build_validation='PASS', asset_validation='PASS',
                      live_validation='PASS' if len(live) == 4 else 'PARTIAL_PASS' if live else 'NOT_RUN',
                      checkpoint={'path': str(a.checkpoint.resolve()), 'sha256': hashes['packaged'], 'bytes': a.checkpoint.stat().st_size},
                      apk_sha256=hashes, compiler_capture={'path': str(a.capture.resolve()), 'sha256': digest(a.capture)},
                      source_sha256=sources, compiler_inputs=commands, current_worktree_changed_since_capture=changed,
                      libraries=libraries, assets_verified=all_assets, prince_bank=bank_records,
                      metadata_reader_host_proof=binding(a.metadata_proof), metadata_source_bindings=metadata_binding,
                      full_bank_host_integration=binding(a.bank_host), bank_host_source_bindings=bank_binding,
                      occurrence_host_integration=binding(a.occurrence_host),
                      occurrence_source_bindings=occurrence_binding,
                      script_timer_host_proof=binding(a.script_timer_host) if a.script_timer_host else None,
                      script_timer_source_bindings=script_timer_binding,
                      script_timer_scope='Source Lua backend and native timer adapter are packaged; host event35 composition uses caller-owned AIS identities. Live LuaManager/selected AIS ownership remains unfinished.' if a.script_timer_host else None,
                      packaged_ai_event_arm64_proof=binding(a.ai_event_arm64_proof) if a.ai_event_arm64_proof else None,
                      packaged_ai_event_source_bindings=event_instruction_binding,
                      occurrence_proof_scope='Genuine production host DSOs and focused original PlayClip/reference regressions; not packaged-instruction or complete-frame/pose/GPU parity.',
                      smokes=live, bank_smoke_artifacts=bank_smoke_assets,
                      validator_sha256=digest(Path(__file__)),
                      limits=['PAB1 is a native port metadata format; cache identity tokens are not original pointers.',
                              'Host metadata/bank proofs do not execute the packaged native instructions.',
                              'All-bank original sampled-pose parity and GPU parity are not established.',
                              'Live reports, if supplied, prove their recorded cases on the bound APK only.',
                              'Observed slot IDs0/1 do not independently establish fade-weight or original-pose parity.',
                              'Complete CharAI/AIS/Lua/inventory/target service ownership remains unfinished.',
                              'No physical ARM64 device or complete-game claim.'])
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--capture', type=Path, required=True)
    parser.add_argument('--checkpoint', type=Path, required=True)
    parser.add_argument('--metadata-proof', type=Path, default=REPO/'port/game-data/reports/animation-bank-host-audit.json')
    parser.add_argument('--bank-host', type=Path, default=REPO/'port/engine-animation/reports/prince-bank-current-integration-host.json')
    parser.add_argument('--occurrence-host', type=Path, default=REPO/'port/level-world/reports/actor-blended-occurrences-host-audit.json')
    parser.add_argument('--script-timer-host', type=Path)
    parser.add_argument('--ai-event-arm64-proof', type=Path)
    for label in ('movement', 'lifecycle', 'combat'):
        parser.add_argument('--'+label, type=Path)
    parser.add_argument('--bank-smoke', type=Path)
    parser.add_argument('--require-live', action='store_true')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    require(not args.output.exists(), 'Refusing to replace an existing validation')
    try:
        result = validate(args)
    except Exception as error:
        result = {'validation': 'FAIL', 'scope': __doc__, 'error': str(error),
                  'build_validation': 'NOT_ESTABLISHED', 'live_validation': 'NOT_ESTABLISHED',
                  'physical_arm64_verified': False, 'full_game_verified': False,
                  'validator_sha256': digest(Path(__file__))}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps({key: result[key] for key in ('validation', 'build_validation', 'live_validation')}, indent=2))
    if result['validation'] != 'PASS':
        raise SystemExit(result['error'])


if __name__ == '__main__':
    main()
