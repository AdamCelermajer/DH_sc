"""Validate the saved native timing APK against its frozen compiler inputs.

The live Prince still supplies empty equipment and its authored player AI row.
Timer/stance/state kernels and typed contributions have bounded instruction
proofs. Complete AI, two-slot live blending and physical ARM64 are unfinished.
Parallel later source edits are explicitly separated from this saved build.
"""
import argparse
import json
from pathlib import Path
import zipfile
from validate_character_combat_source import (
    REPO, ROOT, REPORTS, ORIGINAL, read, digest, sha, require, binding,
    library_record, elf_symbols, verify_authored_cache, smoke_record)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--capture', type=Path, required=True)
    p.add_argument('--checkpoint', type=Path, required=True)
    p.add_argument('--movement', type=Path, required=True)
    p.add_argument('--lifecycle', type=Path, required=True)
    p.add_argument('--combat', type=Path, required=True)
    p.add_argument('--cache', type=Path, default=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'))
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    require(not a.output.exists(), 'Refusing to replace an existing validation')
    report = {'validation': 'FAIL', 'scope': __doc__, 'goal_status': 'active'}
    try:
        with zipfile.ZipFile(a.capture) as capture:
            manifest = json.loads(capture.read('build-capture.json'))
            require(manifest['validation'] == 'BUILD_INPUTS_CAPTURED', 'Wrong build capture')
            for name, expected in manifest['entries'].items():
                raw = capture.read(name)
                require(len(raw) == expected['bytes'] and sha(raw) == expected['sha256'], 'Capture entry changed: '+name)
            sources = manifest['source_sha256']
            for name, expected in sources.items():
                require(sha(capture.read('source/'+name)) == expected, 'Frozen source differs: '+name)
            for tag in ('packaged', 'studio'):
                for abi in ('arm64-v8a', 'x86_64'):
                    inputs = manifest['compiler_inputs'][tag][abi]['repository_inputs']
                    require(all(sources[name] == value for name, value in inputs.items()), 'Compiler source binding differs')
                    for stem in ('character_timers.cpp', 'character_stance.cpp', 'character_state.cpp', 'animation_blend.cpp'):
                        require(any(name.endswith('/'+stem) for name in inputs), 'Missing actual compiler input: '+stem)
            hashes = {tag: value['sha256'] for tag, value in manifest['apks'].items()}
            require(digest(a.checkpoint) == hashes['packaged'], 'Saved checkpoint differs')
            library_records, assets, provenance = {}, {}, {}
            from io import BytesIO
            for tag in ('packaged', 'studio'):
                raw = capture.read(tag+'-app-debug.apk')
                require(sha(raw) == hashes[tag], 'Captured APK differs')
                # library_record needs the exact APK local ZIP header bytes.
                artifact = Path(manifest['apks'][tag]['path'])
                require(digest(artifact) == hashes[tag], 'Current APK path differs from frozen build')
                with zipfile.ZipFile(BytesIO(raw)) as apk:
                    names = apk.namelist()
                    require(len(names) == len(set(names)), 'Duplicate APK members')
                    require(not any('DungeonHunter2.so' in name or name.startswith('lib/armeabi') for name in names), 'ARM32 engine bundled')
                    rows = [library_record(apk, item, artifact) for item in apk.infolist() if item.filename.startswith('lib/') and item.filename.endswith('.so')]
                    require(len(rows) == 14 and {row['path'].split('/')[1] for row in rows} == {'arm64-v8a', 'x86_64'}, 'Wrong native inventory')
                    library_records[tag] = {row['path']: row for row in rows}
                    for abi in ('arm64-v8a', 'x86_64'):
                        world, _ = elf_symbols(apk.read(f'lib/{abi}/libdh2_level_world.so'))
                        animation, _ = elf_symbols(apk.read(f'lib/{abi}/libdh2_engine_animation.so'))
                        _, native = elf_symbols(apk.read(f'lib/{abi}/libdh2_native.so'))
                        require({'dh2_character_timer_start', 'dh2_character_timers_update', 'dh2_character_anim_stance', 'dh2_character_state_event'} <= world, 'World kernel exports missing')
                        require({'dh2_character_timer_start', 'dh2_character_timers_update', 'dh2_character_anim_stance'} <= native, 'Live application does not import timing/stance kernels')
                        require({'dh2_animation_blend_scalar', 'dh2_animation_blend_vector3', 'dh2_animation_blend_quaternion'} <= animation, 'Typed kernels missing')
                    current = {}
                    for name in names:
                        if name.startswith('assets/') and not name.endswith('/'):
                            payload = apk.read(name)
                            key = name[7:]
                            current[key] = {'bytes': len(payload), 'sha256': sha(payload)}
                            if tag == 'packaged' and key.endswith('provenance.json'):
                                provenance[key] = json.loads(payload)
                    require(len(current) == 189, 'Wrong authored asset count')
                    if tag == 'packaged':
                        assets = current
                    else:
                        require(assets == current, 'Repo/Studio assets differ')
        original = REPO/'.local-inputs/libDungeonHunter2.so'
        require(digest(original) == ORIGINAL, 'Original oracle changed')
        prior_path = REPORTS/'character-combat-source-validation.json'
        prior = read(prior_path)
        require(prior['validation'] == 'PASS' and prior['assets_verified'] == assets, 'Earlier authored assets changed')
        cache = verify_authored_cache(a.cache, assets, provenance)
        timing = {}
        for tag in ('packaged', 'studio'):
            path = REPORTS/f'character-timing-{tag}-arm64-differential.json'
            proof = read(path)
            require(proof['validation'] == 'PASS' and proof['apk_sha256'] == hashes[tag] and proof['original_sha256'] == ORIGINAL, 'Wrong timing proof')
            require(proof['library_sha256'] == library_records[tag]['lib/arm64-v8a/libdh2_level_world.so']['sha256'], 'Wrong timing ELF')
            require(all(sources[name] == value for name, value in proof['source_sha256'].items()), 'Timing proof source differs from frozen build')
            t, s = proof['timing_differential'], proof['prior_state_differential']
            require(t['mismatches'] == s['mismatches'] == 0 and t['timer_update_cases'] == 1856 and t['timer_source_reentry_cases'] == 4 and t['stance_cases'] == 576 and t['gate_projection_cases'] == 30 and t['expiry_chain_cases'] == 16 and s['comparisons'] == 3910 and s['ordered_service_requests'] == 8773, 'Timing/state corpus incomplete')
            require(t['timer_malformed_no_mutation_cases'] == 10 and t['stance_malformed_no_mutation_cases'] == 5 and s['malformed_no_mutation_cases'] == 21, 'Timing/state guards incomplete')
            timing[tag] = binding(path)
        typed = {}
        for tag in ('packaged', 'studio'):
            path = REPO/'port/engine-animation/reports'/f'animation-blend-{tag}-arm64-differential.json'
            proof = read(path)
            differential = proof['differential']
            require(proof['validation'] == 'PASS' and proof['apk_sha256'] == hashes[tag] and differential['mismatches'] == 0 and differential['comparisons'] == 5786 and differential['atomic_rejection_checks'] == 36, 'Typed packaged proof incomplete')
            require(proof['actual_packaged_library_executed'] and differential['genuine_packaged_slerp_calls'] > 0 and differential['packaged_math_unique_instructions'] > 0, 'Packaged math was not executed')
            for name, value in proof['source_sha256'].items():
                if name in sources:
                    require(sources[name] == value, 'Typed proof source differs from frozen build')
            for member, expected in proof['libraries'].items():
                require(library_records[tag][member]['sha256'] == expected['sha256'], 'Typed dependency ELF differs')
            typed[tag] = binding(path)
        hosts = {}
        for stem, base in (('character-timers', 'port/level-world'), ('character-stance', 'port/level-world'), ('character-state-timers', 'port/level-world'), ('animation-blend', 'port/engine-animation')):
            name = stem+('-host-audit.json' if stem == 'animation-blend' else '-host-sanitizers.json')
            path = REPO/base/'reports'/name
            proof = read(path)
            require(proof['validation'] == 'PASS', 'Host audit failed')
            flags = str(proof.get('host_flags', proof.get('sanitizers', ''))).lower()
            require('address' in flags and 'undefined' in flags, 'Host sanitizers absent')
            for key, value in proof['source_sha256'].items():
                key = key.replace('\\', '/')
                key = key if key.startswith('port/') else base+'/'+key
                require((sources[key] if key in sources else digest(REPO/key)) == value, 'Host proof source changed: '+key)
            results = [proof[key] for key in ('host_sanitizer_audit', 'prior_corpus_host_replay', 'new_gate_and_chain_host_replay') if key in proof]
            require(all(result['mismatches'] == 0 for result in results) and proof.get('mismatches', 0) == 0, 'Host mismatches')
            hosts[stem] = binding(path)
        movement = smoke_record(a.movement, hashes['packaged'])
        lifecycle = smoke_record(a.lifecycle, hashes['packaged'])
        combat = smoke_record(a.combat, hashes['packaged'])
        require(all(movement[key] for key in ('scene_then_step_then_actor_markers', 'live_touch_movement', 'release_returns_to_authored_idle', 'body_coordinates_match_game')), 'Movement smoke incomplete')
        require(all(lifecycle[key] for key in ('rotation_position_preserved', 'context_counter_resets_verified', 'pause_cancels_held_input')) and not lifecycle['unattended_resume_movement'], 'Lifecycle smoke incomplete')
        require(combat['finite_source_idle_closure'] and set(combat['cases']) == {'stationary', 'moving'} and combat['moving_attack_displacement_preserved'], 'Combat smoke incomplete')
        changed = [name for name, value in sources.items() if not (REPO/name).is_file() or digest(REPO/name) != value]
        report.update(validation='PASS', checkpoint={'path': str(a.checkpoint.resolve()), 'sha256': hashes['packaged'], 'bytes': a.checkpoint.stat().st_size},
                      apk_sha256=hashes, source_sha256=sources, source_scope='Frozen actual compiler inputs in the bound build capture; subsequent current worktree changes are listed separately.',
                      current_worktree_changed_since_capture=changed,
                      build_capture={'path': str(a.capture.resolve()), 'sha256': digest(a.capture)},
                      assets_verified=assets, libraries=library_records, original_cache=cache,
                      timing_kernels=timing, typed_kernels=typed, host_audits=hosts,
                      smokes={'movement': binding(a.movement), 'lifecycle': binding(a.lifecycle), 'combat': binding(a.combat)},
                      prior_checkpoint=binding(prior_path),
                      limits=['Live two-slot blending not yet integrated.', 'Full AI expired callback, scripts and inventory query producers unfinished.', 'Current live Prince attacks did not exercise a nonzero attack-delay expiry.', 'Physical ARM64 Android device not tested.', 'Complete game, UI, audio, saves and full asset cache unfinished.'])
    finally:
        a.output.parent.mkdir(parents=True, exist_ok=True)
        a.output.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps({'validation': report['validation'], 'checkpoint': report['checkpoint'], 'changed_current_sources': report['current_worktree_changed_since_capture']}))


if __name__ == '__main__':
    main()
