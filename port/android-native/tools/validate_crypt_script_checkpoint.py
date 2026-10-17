"""Bind the new native Crypt script artifact to its runtime, assets and sources."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import zipfile
from emulator_smoke import inspect

REPO = Path(__file__).resolve().parents[3]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apk', type=Path, required=True)
    parser.add_argument('--sdk', type=Path, required=True)
    parser.add_argument('--runtime', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--include-acquisition-sources', action='store_true')
    parser.add_argument('--include-frame-sources', action='store_true')
    parser.add_argument('--include-native-owner-sources', action='store_true')
    parser.add_argument('--include-runtime-dependency-sources', action='store_true')
    parser.add_argument('--include-lifecycle-sources', action='store_true')
    parser.add_argument('--include-initialization-sources', action='store_true')
    parser.add_argument('--include-debug-persistence-sources', action='store_true')
    parser.add_argument('--include-level-construction-source', action='store_true')
    parser.add_argument('--include-level-catalogue-assets', action='store_true')
    args = parser.parse_args()
    runtime = json.loads(args.runtime.read_text(encoding='utf-8'))
    digest = sha(args.apk)
    assert runtime['validation'] == 'PASS' and runtime['apk_sha256'] == digest
    assert runtime['installed_apk_sha256'] == digest and runtime['page_size'] == 16384
    assert runtime['actual_touch_contact_triggered_original_script']
    assert runtime['reload_preserves_consumed_trigger_and_script']
    assert runtime['recreation_preserves_script_and_ghost_states_without_replay']
    assert runtime['source_visibility_hides_then_restores_enabled_ghosts']
    assert len(runtime['unbound_gated_combat_target_pairs_rejected']) == 3
    tools = args.sdk / 'build-tools/37.0.0'
    alignment = subprocess.run([str(tools / 'zipalign.exe'), '-c', '-P', '16', '4', str(args.apk.resolve())],
                               check=True, capture_output=True, text=True)
    signing = subprocess.run([str(tools / 'apksigner.bat'), 'verify', '--verbose', str(args.apk.resolve())],
                             check=True, capture_output=True, text=True, env=os.environ.copy())
    paths = [
        'port/android-native/app/src/main/cpp/model_renderer.cpp',
        'port/android-native/app/src/main/java/com/example/dh2/MainActivity.java',
        'port/android-native/app/src/main/cpp/CMakeLists.txt',
        'port/level-world/CMakeLists.txt',
        'port/level-world/crypt_spawn_script_session.cpp',
        'port/level-world/crypt_spawn_script_session.hpp',
        'port/level-world/crypt_spawn_trigger.cpp',
        'port/level-world/crypt_spawn_trigger.hpp',
        'port/level-world/character_factory.cpp',
        'port/level-world/character_state.cpp',
        'port/level-world/character_state.hpp',
        'port/script-runtime/script_runtime.cpp',
        'port/script-runtime/script_runtime.hpp',
        'port/trigger-contact/trigger_contact.cpp',
        'port/trigger-contact/trigger_contact.hpp',
        'port/zone-contact-runtime/zone_geometry.cpp',
        'port/pydata-scripts/native/pydata_scripts.cpp',
        'port/pydata-scripts/native/pydata_scripts.h',
    ]
    ai_units = (
        'character_aggro_delay', 'character_ai_turn',
        'character_aggro_target_search', 'character_aggro_candidate_events',
        'character_enemy_spotted', 'monster_external_script_session',
        'character_ai_set_target', 'character_ai_relations', 'character_ai_in_combat',
    )
    initialization = args.include_initialization_sources or args.include_debug_persistence_sources or args.include_level_construction_source
    lifecycle = args.include_lifecycle_sources or initialization
    runtime_dependencies = args.include_runtime_dependency_sources or lifecycle
    frame = args.include_frame_sources or args.include_native_owner_sources or runtime_dependencies
    acquisition = args.include_acquisition_sources or frame
    if acquisition:
        ai_units += ('character_aggro_acquisition_prefix', 'character_ai_sight', 'ghost_ai_session', 'character_monster_retarget')
    if frame:
        ai_units += ('character_enemy_retention', 'character_ai_update_target', 'character_ai_master_update',
                     'character_ai_melee_range', 'character_ai_interaction_range', 'ais_external_update',
                     'ais_state_callbacks', 'character_ai_pause_update', 'ais_default_collision_persist')
        paths.extend(('port/scene-materials/CMakeLists.txt', 'port/scene-materials/technique_selector.hpp',
                      'port/scene-materials/technique_selector.cpp', 'port/material-bindings/bindings.hpp',
                      'port/material-bindings/bindings.cpp'))
    if runtime_dependencies:
        ai_units += ('character_ai_ranged_range', 'character_range_capability', 'character_ai_queue',
                     'character_interactive', 'character_ai_initialization', 'ais_external_init_vcb')
        paths.extend(('port/scene-materials/render_state_snapshot.hpp',
                      'port/scene-materials/render_state_snapshot.cpp',
                      'port/scene-materials/source_state_conversion.hpp',
                      'port/scene-materials/source_state_conversion.cpp'))
    if lifecycle:
        ai_units += ('ais_external_initialization','ais_native_bindings',
                     'character_ai_association','character_native_bindings','room_zone_enrollment')
        paths.extend(('port/adam-script-runtime/script_runtime.c',
                      'port/adam-script-runtime/script_runtime.h'))
    if initialization:
        ai_units += ('lua_script_level_queries', 'character_script_set_level',
                     'character_regeneration', 'module_room_zone_bounds')
    if args.include_debug_persistence_sources:
        ai_units += ('debug_switches_runtime', 'debug_switches_persistence')
    if args.include_level_construction_source:
        ai_units += ('level_construction_fields',)
    if args.include_native_owner_sources:
        paths.extend(('port/android-native/app/src/main/cpp/ghost_ai_owner.hpp',
                      'port/android-native/app/src/main/cpp/ghost_ai_owner.cpp'))
    if args.include_level_catalogue_assets:
        paths.extend(('port/game-data/level_tables.cpp', 'port/game-data/level_tables.hpp',
                      'port/game-data/CMakeLists.txt', 'port/android-native/tools/prepare_level_tables.py'))
    for unit in ai_units:
        paths.extend(f'port/level-world/{unit}{suffix}' for suffix in ('.hpp', '.cpp'))
    paths.extend((
        'port/random/random.h', 'port/random/random.c',
        'port/lua-numeric/numeric.h', 'port/lua-numeric/numeric.c',
    ))
    assets = {}
    with zipfile.ZipFile(args.apk) as archive:
        for name in archive.namelist():
            if name.startswith('assets/') and not name.endswith('/'):
                raw = archive.read(name)
                local = REPO / 'port/android-native/app/src/main' / name
                assert local.read_bytes() == raw, name
                assets[name.removeprefix('assets/')] = {'bytes': len(raw), 'sha256': hashlib.sha256(raw).hexdigest()}
    assert len(assets) == (243 if acquisition else 240) + 3 * int(args.include_level_catalogue_assets)
    if args.include_level_catalogue_assets:
        provenance_path = REPO / 'port/android-native/app/build/level-table-provenance.json'
        provenance = json.loads(provenance_path.read_text(encoding='utf-8'))
        assert provenance['unchanged_original_data']
        for name, record in provenance['inputs'].items():
            assert assets['data/'+name] == {'bytes': record['bytes'], 'sha256': record['sha256']}
    if acquisition:
        from prepare_monster_ai import CACHE_SHA256, SCRIPTS
        for name, digest_expected in SCRIPTS.items():
            assert assets['scripts/ai/' + name]['sha256'] == digest_expected
        provenance = json.loads((REPO / 'port/android-native/app/src/main/assets/scripts/ai/monster-ai-provenance.json').read_text())
        assert provenance['cache_sha256'] == CACHE_SHA256 and provenance['unchanged_original_source'] is True

    report = {'validation': 'PASS', 'apk_sha256': digest, 'apk_bytes': args.apk.stat().st_size,
              'runtime_report_sha256': sha(args.runtime), 'native_libraries': inspect(args.apk),
              'zip_16k_alignment_verified': alignment.returncode == 0,
              'signing_verification': signing.stdout.strip(), 'assets': assets,
              'source_sha256': {path: sha(REPO / path) for path in paths},
              'host_only_source_not_claimed_live': ['character_template_random', 'swamp_actor_floor_bridge'],
              'full_game_playable': False, 'physical_arm64_phone_tested': False,
              'scope': 'Exact Crypt GhostAmbush01 artifact/source/assets and Android17/16KiB runtime evidence; no whole-function or full-game equivalence claim'}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'validation': 'PASS', 'apk_sha256': digest, 'apk_bytes': report['apk_bytes'],
                      'assets': len(assets), 'libraries': len(report['native_libraries'])}))


if __name__ == '__main__':
    main()
