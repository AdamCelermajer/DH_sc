"""Bounded WSL host regressions for Item pool virtual40; never calls adb.

The Item factory/graph/current production units are rebuilt from this checkout.
Remaining shared libraries are the declared older host support snapshot, not a
current APK or whole campaign acceptance claim.
"""
from pathlib import Path
import hashlib
import json
import subprocess
import sys
import uuid

ROOT = Path(__file__).resolve().parents[3]
UNIX = '/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
OUT = ROOT / 'port/level-world/reports/item-stage29-set-visible-v1-host'
OUT.mkdir(parents=True, exist_ok=True)
SCRATCH = '/var/tmp/dh2-item-set-visible-' + uuid.uuid4().hex
SUPPORT = '.local-inputs/character-menu-native-v1-host/snapshot'
base = json.loads((ROOT / 'port/level-world/reports/loot-canonical-v44/host.json').read_text())
sources = [p for p in base['source_sha256'] if '/tests/' not in p]
sources += [
    'port/level-world/object_manager_language_registry_v1.cpp',
    'port/level-world/condition_scoped_binding_v72.cpp',
    'port/level-world/condition_data_init_v3.cpp',
    'port/level-world/generic_animation_callbacks_v21.cpp',
    'port/level-world/retained_visual_child_v91.cpp',
    'port/level-world/retained_map_mesh_v93.cpp',
    'port/level-world/native_scene_lights_v113.cpp',
    'port/engine-skinning/skinning.cpp',
    'port/engine-skinning/visual_skin_owner_v6.cpp',
    'port/level-loader/lifecycle_v36.cpp',
    'port/level-world/fresh_inventory_loot_source_v9.cpp',
    'port/level-world/character_loot_item_source_v9.cpp',
    'port/level-world/world_item_init_again_source_v9.cpp',
    'port/level-world/world_item_drop_inventory_v9.cpp',
    'port/game-data/fresh_inventory_owned_v4.cpp',
    'port/level-world/character_loot_drop_award_v8.cpp',
    'port/level-world/world_item_physical_contact_v4.cpp',
    'port/level-world/loot_source_fields_v47.cpp',
    'port/level-world/loot_item_frame_source_v47.cpp',
    'port/level-world/character_use_ooi_v47.cpp',
    'port/level-world/character_state_owner.cpp',
    'port/level-world/character_target_bindings.cpp',
    'port/level-world/loot_item_target_source_v47.cpp',
    'port/level-world/player_save_difficulty_global_v29.cpp',
    'port/engine-ui/character_menu_item_actions_v1.cpp',
    'port/engine-ui/character_menu_inventory_mutation_v1.cpp',
]
sources = list(dict.fromkeys(sources))
cache_args = ['.local-inputs/player-loot-v7/cache', '.local-inputs/loot-world-v8/cache']
visual_args = cache_args + ['.local-inputs/item-visual-cache-v2']
text_args = visual_args + [
    'port/level-world/reference/character-game-design/real-cache-inputs.bin',
    '.local-inputs/loot-v44-host-assets', '.local-inputs/loot-v44-host-private',
]
tests = [
    ('item_stage29_set_visible_v1', visual_args),
    ('world_item_live_owner_v5', visual_args),
    ('world_item_pool_graph_v3', visual_args),
    ('world_item_pool_pf_v4', visual_args),
    ('world_loot_canonical_bindings_v44', text_args),
    ('world_loot_positive_v23', text_args),
    ('character_loot_item_manager_v8', cache_args),
    ('world_item_drop_inventory_v9', cache_args),
    ('loot_root_v47', text_args),
]
flags = ['-std=c++17', '-O0', '-g0', '-Wall', '-Wextra', '-Werror',
         '-Wno-misleading-indentation', '-Wno-missing-field-initializers',
         '-include', 'algorithm',
         '-ffunction-sections', '-fdata-sections', '-fsanitize=address,undefined',
         '-fno-omit-frame-pointer']
for directory in ['level-world', 'game-data', 'level-loader', 'engine-ui',
                  'scene-materials', 'engine-animation', 'engine-skinning',
                  'script-runtime']:
    flags.append('-Iport/' + directory)
flags += ['-isystem', 'port/physics-backend/box2d-2.0.1/Include']
rows = []
report = {'status': 'RUNNING', 'commands': rows,
          'scope': 'Direct Stage29 real cached Item factory/graph precache; '
                   'Level/debug/device/network/Projectile fixture boundaries; '
                   'older shared host support; no SWAMP-authored Item/device claim'}
completed_objects = {}
completed_tests = {}
header_hashes = {str(p.relative_to(ROOT)).replace('\\', '/'): hashlib.sha256(p.read_bytes()).hexdigest()
                 for p in (ROOT / 'port').rglob('*')
                 if p.suffix in ('.hpp', '.h') and not {'reports', 'vendor'}.intersection(p.parts)}
if '--resume' in sys.argv[1:]:
    prior = json.loads((OUT / 'receipt.json').read_text())
    report['previous_failure'] = prior.get('error', '')
    report['resumed_source_sha256'] = prior.get('source_sha256', {})
    if prior.get('header_sha256') == header_hashes:
        SCRATCH = prior['scratch']
    for row in prior['commands'] if prior.get('header_sha256') == header_hashes else []:
        args = row['argv']
        if row['exit_code'] == 0 and args[0] == 'g++' and '-c' in args:
            source = args[args.index('-c') + 1]
            object_digest = subprocess.run(['wsl.exe', '--exec', 'sha256sum', args[-1]], capture_output=True, text=True, timeout=10)
            if source in sources and prior['source_sha256'].get(source) == hashlib.sha256((ROOT / source).read_bytes()).hexdigest() and object_digest.returncode == 0 and prior.get('object_sha256', {}).get(source) == object_digest.stdout.split()[0]:
                completed_objects[source] = args[-1]
                rows.append(row)
    # Only exact source objects from this same bounded current-source run are
    # reused; changed sources and every test TU are rebuilt. This is not a
    # reusable engine library or current APK attribution shortcut.
    report['resumed_objects'] = list(completed_objects)
    if prior.get('header_sha256') == header_hashes and all(
            prior.get('source_sha256', {}).get(p) == hashlib.sha256((ROOT / p).read_bytes()).hexdigest()
            for p in sources):
        for row in prior['commands']:
            if row['exit_code'] == 0 and row['argv'][0] == 'env':
                name = Path(row['argv'][3]).name
                source = 'port/level-world/tests/' + name + '.cpp'
                if source in prior['source_sha256'] and prior['source_sha256'][source] == hashlib.sha256((ROOT / source).read_bytes()).hexdigest():
                    completed_tests[name] = row
    report['resumed_tests'] = list(completed_tests)
report['scratch'] = SCRATCH
report['header_sha256'] = header_hashes


def run(argv, seconds):
    result = subprocess.run(
        ['wsl.exe', '--cd', UNIX, '--exec', 'timeout', '--kill-after=2',
         str(seconds), *argv], capture_output=True, text=True, timeout=seconds + 10)
    rows.append({'argv': argv, 'exit_code': result.returncode,
                 'stdout': result.stdout, 'stderr': result.stderr})
    (OUT / 'receipt.json').write_text(json.dumps(report, indent=2) + '\n')
    if result.returncode:
        raise RuntimeError(result.stderr or result.stdout or str(argv))
    if result.stdout:
        print(result.stdout, end='', flush=True)


try:
    ida = ROOT / '.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so'
    vtables = json.loads((ida / 'vtables-and-rtti.json').read_text())
    item_vtable = next(v for v in vtables if v['name'] == '_ZTV10ItemObject')
    raw = bytes.fromhex(item_vtable['raw_bytes_hex'])
    selected = int.from_bytes(raw[8 + 0x40:8 + 0x44], 'little')
    assert selected == 0x38b0f0, 'Original Item virtual40 must be GameObject.SetVisible'
    report['original_virtual_proof'] = {
        'vtable': item_vtable['address'], 'vptr_header_bytes': 8,
        'slot': '0x40', 'selected': hex(selected),
        'despawn_callsite': '0x3eaca8', 'spawn_callsite': '0x3eae14',
        'vtable_sha256': hashlib.sha256((ida / 'vtables-and-rtti.json').read_bytes()).hexdigest(),
    }
    run(['mkdir', '-p', SCRATCH + '/objects', SCRATCH + '/libs'], 5)
    libraries = sorted((ROOT / SUPPORT).glob('*.so'))
    assert libraries, 'Required recorded WSL host support libraries'
    run(['cp', '-t', SCRATCH + '/libs',
         *[SUPPORT + '/' + p.name for p in libraries]], 30)
    report['support_sha256'] = {p.name: hashlib.sha256(p.read_bytes()).hexdigest()
                               for p in libraries}
    report['source_sha256'] = {}
    report['object_sha256'] = {}
    objects = []
    for index, source in enumerate(sources):
        obj = SCRATCH + '/objects/' + source.replace('/', '_') + '.o'
        report['source_sha256'][source] = hashlib.sha256((ROOT / source).read_bytes()).hexdigest()
        if source not in completed_objects:
            run(['g++', *flags, '-c', source, '-o', obj], 45)
        digest = subprocess.run(['wsl.exe', '--exec', 'sha256sum', obj], capture_output=True, text=True, timeout=10)
        if digest.returncode:
            raise RuntimeError('Compiled object disappeared before hash verification: ' + obj)
        report['object_sha256'][source] = digest.stdout.split()[0]
        objects.append(obj)
        print('reused' if source in completed_objects else 'compiled', index + 1, '/', len(sources), source, flush=True)
    links = ['-L' + SCRATCH + '/libs', '-ldh2_game_data', '-ldh2_level_world',
             '-ldh2_engine_ui', '-ldh2_scene_materials', '-ldh2_engine_animation',
             '-ldh2_engine_skinning', '-ldh2_script_runtime', '-Wl,--gc-sections',
             '-Wl,-rpath,' + SCRATCH + '/libs']
    for name, arguments in tests:
        source = 'port/level-world/tests/' + name + '.cpp'
        report['source_sha256'][source] = hashlib.sha256((ROOT / source).read_bytes()).hexdigest()
        if name in completed_tests:
            rows.append(completed_tests[name])
            print('retained PASS', name, flush=True)
            continue
        obj, binary = SCRATCH + '/objects/' + name + '.o', SCRATCH + '/' + name
        run(['g++', *flags, '-c', source, '-o', obj], 45)
        run(['g++', *flags, obj, *objects, *links, '-o', binary], 60)
        run(['env', 'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1',
             'UBSAN_OPTIONS=halt_on_error=1', binary, *arguments], 45)
    report['status'] = 'PASS'
except Exception as error:
    report['status'] = 'FAIL'
    report['error'] = str(error)
finally:
    report['changed_during_run'] = [p for p, digest in report.get('source_sha256', {}).items()
                                    if hashlib.sha256((ROOT / p).read_bytes()).hexdigest() != digest]
    (OUT / 'receipt.json').write_text(json.dumps(report, indent=2) + '\n')
print(json.dumps({'status': report['status'], 'receipt': str(OUT / 'receipt.json'),
                  'changed_during_run': report['changed_during_run']}), flush=True)
raise SystemExit(0 if report['status'] == 'PASS' and not report['changed_during_run'] else 1)
