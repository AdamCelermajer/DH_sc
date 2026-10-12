"""Bind a tested checkpoint to its compiled owner and packaged target-search proof.

Host owner checks cover genuine private VM/alias/session ownership with explicit
gameplay registration providers. Only the target-search proof executes packaged
ARM64 instructions. Live checks remain bounded Crypt prototype checks.
"""
import argparse
from io import BytesIO
import json
from pathlib import Path
import zipfile
from validate_character_combat_source import REPO, binding, digest, elf_symbols, read, require, sha


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--base-validation', type=Path, required=True)
    p.add_argument('--build-capture', type=Path, required=True)
    p.add_argument('--owner-host', type=Path, required=True)
    p.add_argument('--packaged-target-proof', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    require(not a.output.exists(), 'Refusing to replace a validation report')
    base = read(a.base_validation)
    require(base['validation'] == base['build_validation'] == base['live_validation'] == 'PASS', 'Base runtime/build validation incomplete')
    require(set(base['smokes']) == {'movement', 'lifecycle', 'combat', 'bank'}, 'All four runtime suites required')
    checkpoint = Path(base['checkpoint']['path'])
    require(digest(checkpoint) == base['checkpoint']['sha256'], 'Checkpoint changed')
    require(digest(a.build_capture) == base['compiler_capture']['sha256'], 'Compiler capture changed')
    owner = read(a.owner_host)
    checks = owner['host_audit']
    require(owner['validation'] == checks['validation'] == 'PASS' and owner['sanitizer_findings'] == 0, 'Owner host audit failed')
    require(checks['checks'] == 6283 and checks['original_binding_descriptors'] == 300 and checks['private_sessions'] == 2, 'Owner corpus differs')
    require(owner['main_world_library_executed'] and owner['owner_module_in_main_world_library'], 'Owner proof must execute central library')
    require(checks['genuine_trace_alias_timer_bindings'] and checks['actual_close_finalizers'] == 1, 'Genuine binding/close evidence absent')
    require(checks['all_gameplay_globals_implemented'] is False and owner['full_gameplay_namespace_bound'] is False, 'Owner scope exceeds proof')
    require({'AddressSanitizer', 'UndefinedBehaviorSanitizer'} <= set(owner['sanitizers']), 'Owner sanitizers absent')
    require(owner['original_sha256'] == base['original_sha256'], 'Original engine identity differs')
    captured, supplemental = {}, {}
    with zipfile.ZipFile(a.build_capture) as capture:
        manifest = json.loads(capture.read('build-capture.json'))
        for name, expected in owner['source_sha256'].items():
            key = name.replace('\\', '/')
            if key in manifest['source_sha256']:
                require(manifest['source_sha256'][key] == expected and sha(capture.read('source/'+key)) == expected, 'Owner proof differs from captured source: '+key)
                captured[key] = expected
            else:
                path = (REPO/key).resolve()
                require(path.is_relative_to(REPO) and path.is_file() and digest(path) == expected, 'Supplementary owner proof changed: '+key)
                supplemental[key] = expected
        for key in ('port/level-world/character_script_owner.cpp', 'port/level-world/character_script_call_timer.cpp', 'port/level-world/character_target_search.cpp'):
            for tag in ('packaged', 'studio'):
                for abi in ('arm64-v8a', 'x86_64'):
                    require(manifest['compiler_inputs'][tag][abi]['repository_inputs'][key] == manifest['source_sha256'][key], 'Compiled owner/search input missing')
        raw = capture.read('packaged-app-debug.apk')
        require(sha(raw) == base['checkpoint']['sha256'], 'Captured APK differs')
        with zipfile.ZipFile(BytesIO(raw)) as apk:
            for abi in ('arm64-v8a', 'x86_64'):
                symbols, imports = elf_symbols(apk.read('lib/'+abi+'/libdh2_level_world.so'))
                require({'dh2_character_script_constructor_fields', 'dh2_character_script_binding_count',
                         'dh2_character_script_binding', 'dh2_character_script_call_timer',
                         'dh2_target_search', 'dh2_target_list_init', 'dh2_target_pop'} <= symbols, 'Owner/search exports absent')
                require(any('ScriptOwner' in x and 'advance' in x for x in symbols), 'Owned lifecycle export absent')
                require({'dh2_script_vm_create_empty', 'dh2_script_vm_open_source_library', 'dh2_script_alias_clear_contents'} <= imports, 'World must use genuine deferred VM/alias teardown')
                runtime, _ = elf_symbols(apk.read('lib/'+abi+'/libdh2_script_runtime.so'))
                require({'dh2_script_vm_create_empty', 'dh2_script_vm_open_source_library', 'dh2_script_alias_clear_contents'} <= runtime, 'Runtime exports absent')
    target = read(a.packaged_target_proof)
    require(target['validation'] == 'PASS' and target['mismatches'] == 0 and target['actual_packaged_instructions_executed'], 'Packaged target-search proof incomplete')
    require(target['apk_sha256'] == base['checkpoint']['sha256'] and target['build_capture_sha256'] == digest(a.build_capture), 'Target proof differs from checkpoint/capture')
    require((target['comparisons'], target['ordered_callbacks'], target['accepted_records'], target['synchronous_same_list_reentry_cases']) == (412, 26580, 1480, 9), 'Target-search corpus differs')
    member = target['apk_library_member']
    require(target['arm64_library_sha256'] == base['libraries']['packaged'][member]['sha256'], 'Target proof differs from actual packaged DSO')
    require(target['physical_arm64_verified'] is False and target['full_game_verified'] is False, 'Target proof scope exceeds evidence')
    report = dict(base)
    report.update(scope=__doc__, base_runtime_validation=binding(a.base_validation),
                  owner_host_proof=binding(a.owner_host), owner_captured_source_sha256=captured,
                  owner_supplemental_source_sha256=supplemental,
                  packaged_target_search_proof=binding(a.packaged_target_proof),
                  script_owner_scope='Compiled private owner backend; actual central host VM/session/alias checks. Live enemy AI is not connected.',
                  packaged_target_search_scope=target['scope'], validator_sha256=digest(Path(__file__)))
    a.output.parent.mkdir(parents=True, exist_ok=True)
    with a.output.open('x', encoding='utf-8', newline='\n') as f:
        f.write(json.dumps(report, indent=2)+'\n')
    print(json.dumps({'validation': 'PASS', 'build_validation': report['build_validation'],
                      'live_validation': report['live_validation'], 'checkpoint': str(checkpoint)}))


if __name__ == '__main__':
    main()
