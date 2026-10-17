"""Verify the bounded source AI units are packaged for both native ABIs.

Export presence and one Lua core establish the build boundary. They do not
claim that the live enemy controllers call these units or complete the game.
Requires pyelftools, also used by the original-function comparison tools.
"""
import argparse
import hashlib
import io
import json
from pathlib import Path
import zipfile

from elftools.elf.elffile import ELFFile

UNITS = {
    'aggro_delay': '_ZN3dh221character_aggro_delay6update',
    'turn_selection': '_ZN3dh217character_ai_turn8evaluate',
    'target_search': 'dh2_aggro_target_',
    'candidate_events': '_ZN3dh232character_aggro_candidate_events7consume',
    'enemy_spotted': '_ZN3dh223character_enemy_spotted16on_enemy_spotted',
    'external_monster_script': '_ZN3dh223monster_external_script7Session8dispatch',
}
TARGETING_UNITS = {
    'target_mutation': 'dh2_character_ai_set_target',
    'faction_relations': '_ZN3dh222character_ai_relations5query',
    'combat_query': '_ZN3dh222character_ai_in_combat8evaluate',
}
ACQUISITION_UNITS = {
    'acquisition_prefix': '_ZN3dh234character_aggro_acquisition_prefix7prepare',
    'monster_retarget': '_ZN3dh226character_monster_retarget6update',
    'sight_distance': '_ZN3dh218character_ai_sight17evaluate_distance',
    'sight_object': '_ZN3dh218character_ai_sight15evaluate_object',
    'ghost_actor_composition': '_ZN3dh216ghost_ai_session12ActorSession19search_and_dispatch',
}
def cpp_prefix(namespace, function):
    return f'_ZN3dh2{len(namespace)}{namespace}{len(function)}{function}'


def nested_cpp_prefix(namespaces, function):
    return '_ZN3dh2' + ''.join(f'{len(name)}{name}' for name in namespaces) + f'{len(function)}{function}'


FRAME_UNITS = {
    'enemy_retention': cpp_prefix('character_enemy_retention', 'update'),
    'target_update': cpp_prefix('character_ai_update_target', 'update'),
    'master_update': cpp_prefix('character_ai_master_update', 'update'),
    'melee_range': cpp_prefix('character_ai_melee_range', 'evaluate_object'),
    'melee_radius': cpp_prefix('character_ai_melee_range', 'get_radius'),
    'interaction_range': cpp_prefix('character_ai_interaction_range', 'evaluate_object'),
    'ais_external_update': cpp_prefix('ais_external_update', 'update'),
    'ais_state_callbacks': cpp_prefix('ais_state_callbacks', 'invoke'),
    'pause_update': cpp_prefix('character_ai_pause_update', 'pause'),
    'collision_persist': cpp_prefix('ais_default_collision_persist', 'persist'),
}
RUNTIME_DEPENDENCY_UNITS = {
    'close_range': cpp_prefix('character_ai_ranged_range', 'evaluate_close'),
    'ranged_range': cpp_prefix('character_ai_ranged_range', 'evaluate_ranged'),
    'character_range_parameters': cpp_prefix('character_range_capability', 'character_parameters'),
    'inventory_range_parameters': cpp_prefix('character_range_capability', 'inventory_parameters'),
    'character_range_capability': cpp_prefix('character_range_capability', 'character_can_range'),
    'inventory_range_capability': cpp_prefix('character_range_capability', 'has_ranged_weapon'),
    'shared_ai_queue': cpp_prefix('character_ai_queue', 'advance'),
    'character_interactive': cpp_prefix('character_interactive', 'evaluate'),
    'character_ai_initialization': cpp_prefix('character_ai_initialization', 'construct'),
    'ais_default_callback_flags': cpp_prefix('ais_external_init_vcb', 'initialize_default'),
    'ais_external_callback_flags': cpp_prefix('ais_external_init_vcb', 'initialize_external'),
}
LIFECYCLE_UNITS = {
    'ais_script_construction': cpp_prefix('ais_external_initialization','construct_char_ai_script'),
    'ais_external_construction': cpp_prefix('ais_external_initialization','construct_external'),
    'ais_character_association': cpp_prefix('ais_external_initialization','set_character'),
    'lua_base_bindings': cpp_prefix('ais_native_bindings','bind_base'),
    'lua_ai_bindings': cpp_prefix('ais_native_bindings','bind_character'),
    'lua_all_bindings': cpp_prefix('ais_native_bindings','bind_all'),
    'character_ai_association': cpp_prefix('character_ai_association','associate'),
    'game_object_native_bindings': cpp_prefix('character_native_bindings','bind_game_object'),
    'character_native_bindings': cpp_prefix('character_native_bindings','bind_character'),
    'room_zone_initial_enrollment': cpp_prefix('room_zone_enrollment','add_initial_object'),
    'object_zone_entered': cpp_prefix('room_zone_enrollment','zone_entered'),
    'object_zone_exited': cpp_prefix('room_zone_enrollment','zone_exited'),
}
LUA_SYMBOL = 'lua_newstate'
INITIALIZATION_UNITS = {
    'host_player_level': cpp_prefix('lua_script_level_queries', 'get_host_player_level'),
    'host_player_difficulty': cpp_prefix('lua_script_level_queries', 'get_host_player_difficulty'),
    'current_level_range': cpp_prefix('lua_script_level_queries', 'get_current_level_range'),
    'character_script_set_level': cpp_prefix('character_script_set_level', 'set_level'),
    'character_regen_hp': cpp_prefix('character_regeneration', 'regen_hp'),
    'character_regen_mp': cpp_prefix('character_regeneration', 'regen_mp'),
    'module_room_zone_bounds': cpp_prefix('module_room_zone_bounds', 'initialize'),
}
DEBUG_PERSISTENCE_UNITS = {
    'debug_switches_load': '_ZN3dh214debug_switches7Runtime4load',
    'debug_switches_get': '_ZN3dh214debug_switches7Runtime10get_switch',
    'debug_switches_set': '_ZN3dh214debug_switches7Runtime10set_switch',
    'debug_switches_write': cpp_prefix('debug_switches_persistence', 'write'),
    'debug_switches_save': cpp_prefix('debug_switches_persistence', 'save'),
}
LEVEL_CONSTRUCTION_UNITS = {'level_construction_fields': cpp_prefix('level_construction_fields', 'initialize')}
NATIVE_MONSTER_DEPENDENCIES = {
    'live_character_level_adapter': cpp_prefix('character_level_runtime', 'Runtime') + '15set_level_fixed',
    'player_level_member_set': cpp_prefix('character_level_member', 'set_value'),
    'player_character_level_setter': cpp_prefix('character_level_member', 'set_character_level'),
    'player_manager_host_level': cpp_prefix('player_manager_host_level', 'get_hosting_level'),
    'player_level_reconciliation': cpp_prefix('player_manager_host_level', 'reconcile_character_level'),
    'ai_update_all_skills': cpp_prefix('character_ai_update_all_skills', 'update'),
}
CHARACTER_LIST_DEPENDENCIES = {
    'character_list_query': 'dh2_aggro_target_search_character_list',
    'object_list_query': 'dh2_aggro_target_search_object_list',
    'manager_character_list_initialize': '_ZN3dh29character5aggro19object_manager_list10initialize',
    'manager_character_list_append': '_ZN3dh29character5aggro19object_manager_list16append_after_add',
    'manager_character_list_remove': '_ZN3dh29character5aggro19object_manager_list19remove_after_remove',
    'manager_object_list_methods': '_ZN3dh29character5aggro19object_manager_list19object_list_methods',
}
INIT_CALLBACK_DEPENDENCIES = {
    'ais_init_callback_callers': cpp_prefix('ais_external_init_callbacks','invoke'),
    'ais_default_init': cpp_prefix('ais_external_init_callbacks','default_init'),
    'ais_default_post': cpp_prefix('ais_external_init_callbacks','default_post'),
    'ais_default_final': cpp_prefix('ais_external_init_callbacks','default_final'),
}
HP_MP_INIT_DEPENDENCIES = {
    'character_hp_mp_init_caller': cpp_prefix('character_init_hp_mp','execute'),
    'native_hp_mp_init_adapter': cpp_prefix('character_level_runtime','Runtime') + '16initialize_hp_mp',
}
GHOST_SKILL_INIT_DEPENDENCIES = {
    'source_set_skills_and_spells': cpp_prefix('character_ai_set_skills_and_spells','prepare'),
    'source_faery_selection': cpp_prefix('character_faery_selection','select'),
    'source_skill_script_constructor': cpp_prefix('character_ai_skill_script_constructor','construct'),
    'source_skill_state_predicates': cpp_prefix('character_skill_state_queries','query'),
    'owned_skill_tables': cpp_prefix('data','load_skill_tables'),
    'owned_faery_tables': cpp_prefix('data','load_faery_tables'),
}
FRAME_FOUNDATION_UNITS = {
    'character_ai_classification': cpp_prefix('character_ai_classification','query'),
    'character_zonability': cpp_prefix('character_zonability','evaluate'),
    'character_update_eligibility': cpp_prefix('character_update_eligibility','evaluate'),
    'session_state_update': cpp_prefix('monster_external_script','Session')+'17call_state_update',
    'session_state_conditions': cpp_prefix('monster_external_script','Session')+'21call_state_conditions',
    'session_resolved_path_load': cpp_prefix('monster_external_script','Session')+'13load_resolved',
    'per_vm_path_cache': cpp_prefix('lua_script_load_once','load_once'),
}
FRAME_BRIDGE_UNITS = {
    'game_object_stop': 'dh2_game_object_stop',
    'character_physics_position': 'dh2_character_is_updating_position_from_physics',
    'character_update_script_scheduler': cpp_prefix('character_update_script_scheduler','run'),
    'session_created_services': cpp_prefix('monster_external_script','Session')+'24install_created_services',
    'session_vm_identity': '_ZNK3dh223monster_external_script7Session11vm_identity',
}
CAMERA_CULLING_UNITS = {
    'camera_plane_extraction': nested_cpp_prefix(('engine_camera', 'frustum'), 'set_from'),
    'camera_bounds': nested_cpp_prefix(('engine_camera', 'frustum_bounds'), 'recalculate'),
    'camera_runtime': nested_cpp_prefix(('engine_camera', 'frustum_runtime'), 'set_from'),
    'camera_plane_intersection': nested_cpp_prefix(('engine_camera', 'plane_intersection'), 'intersect_three_planes'),
    'game_object_set_visible': cpp_prefix('game_object_set_visible', 'set_game_object'),
    'visual_object_set_visible': cpp_prefix('game_object_set_visible', 'set_visual_object'),
    'game_object_disable_zoning': cpp_prefix('game_object_zoning_visibility', 'disable_zoning'),
    'game_object_enable_zoning': cpp_prefix('game_object_zoning_visibility', 'enable_zoning'),
    'visual_object_sync_visibility': cpp_prefix('game_object_zoning_visibility', 'sync_visibility'),
    'object_update_culling': cpp_prefix('object_update_culling', 'evaluate'),
    'object_remote_update_query': cpp_prefix('object_update_culling', 'is_remotely_updated'),
    'object_update_dispatch': cpp_prefix('object_update_dispatch', 'dispatch'),
    'character_culling_composition': cpp_prefix('character_culling_runtime', 'evaluate'),
    'native_character_stop': 'dh2_native_character_stop',
}
CAMERA_CULLING_LIBRARIES = {
    'camera_plane_extraction': 'libdh2_engine_camera.so',
    'camera_bounds': 'libdh2_engine_camera.so',
    'camera_runtime': 'libdh2_engine_camera.so',
    'camera_plane_intersection': 'libdh2_engine_camera.so',
    'game_object_set_visible': 'libdh2_level_world.so',
    'visual_object_set_visible': 'libdh2_level_world.so',
    'game_object_disable_zoning': 'libdh2_level_world.so',
    'game_object_enable_zoning': 'libdh2_level_world.so',
    'visual_object_sync_visibility': 'libdh2_level_world.so',
    'object_update_culling': 'libdh2_level_world.so',
    'object_remote_update_query': 'libdh2_level_world.so',
    'object_update_dispatch': 'libdh2_level_world.so',
    'character_culling_composition': 'libdh2_level_world.so',
    'native_character_stop': 'libdh2_native.so',
}
ADAM_RECONCILIATION_UNITS = {
    'item_instance_quantity': '_ZNK3dh24data14ItemInstanceV115signed_quantity',
    # Both exports come from the single loot_tables_v2.cpp source unit.
    'loot_tables_decode_owner': ('dh2_loot_v2_decode',
                                 nested_cpp_prefix(('data', 'LootTablesV2'), 'load')),
    'fresh_inventory_v4': nested_cpp_prefix(('data', 'FreshInventoryOwnedV4'), 'add_fixed_loot'),
    'item_gear_properties_v5': 'dh2_gear_reset_v5',
    'item_power_tables_v5': 'dh2_item_power_decode_v5',
    'player_gear_effects_v5': nested_cpp_prefix(('data', 'PlayerGearEffectsV5'), 'update_properties'),
    'loot_power_resources_v7': nested_cpp_prefix(('data', 'LootPowerResourcesV7'), 'load'),
    'ais_player_init_vcb': cpp_prefix('ais_player_init_vcb', 'initialize'),
    'character_skill_cooldown_services': (
        cpp_prefix('character_skill_cooldown_services', 'skill'),
        cpp_prefix('character_skill_cooldown_services', 'spell')),
}
ADAM_RECONCILIATION_LIBRARIES = {
    'item_instance_quantity': 'libdh2_game_data.so',
    'loot_tables_decode_owner': 'libdh2_game_data.so',
    'fresh_inventory_v4': 'libdh2_game_data.so',
    'item_gear_properties_v5': 'libdh2_game_data.so',
    'item_power_tables_v5': 'libdh2_game_data.so',
    'player_gear_effects_v5': 'libdh2_game_data.so',
    'loot_power_resources_v7': 'libdh2_game_data.so',
    'ais_player_init_vcb': 'libdh2_level_world.so',
    'character_skill_cooldown_services': 'libdh2_level_world.so',
}
ADAM_RECONCILIATION_SOURCE_PATHS = {
    'item_instance_quantity': ('port/game-data/item_instance.cpp',),
    'loot_tables_decode_owner': ('port/game-data/loot_tables_v2.cpp',),
    'fresh_inventory_v4': ('port/game-data/fresh_inventory_owned_v4.cpp',),
    'item_gear_properties_v5': ('port/game-data/item_gear_properties_v5.cpp',),
    'item_power_tables_v5': ('port/game-data/item_power_tables_v5.cpp',),
    'player_gear_effects_v5': ('port/game-data/player_gear_effects_v5.cpp',),
    'loot_power_resources_v7': ('port/game-data/loot_power_resources_v7.cpp',),
    'ais_player_init_vcb': ('port/level-world/ais_player_init_vcb.cpp',),
    'character_skill_cooldown_services': ('port/level-world/character_skill_cooldown_services.cpp',),
}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apk', type=Path, required=True)
    source = parser.add_mutually_exclusive_group(required=True)
    source.add_argument('--artifact', type=Path,
                        help='Previously verified checkpoint report tied to this APK')
    source.add_argument('--build-capture', type=Path,
                        help='Actual compiler-input capture from capture_native_build.py')
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--require-targeting-units', action='store_true')
    parser.add_argument('--require-acquisition-units', action='store_true')
    parser.add_argument('--require-frame-units', action='store_true')
    parser.add_argument('--require-runtime-dependency-units', action='store_true')
    parser.add_argument('--require-lifecycle-units', action='store_true')
    parser.add_argument('--require-initialization-units', action='store_true')
    parser.add_argument('--require-debug-persistence-units', action='store_true')
    parser.add_argument('--require-level-construction-unit', action='store_true')
    parser.add_argument('--require-native-monster-dependencies', action='store_true')
    parser.add_argument('--require-character-list-dependencies', action='store_true')
    parser.add_argument('--require-init-callback-dependencies', action='store_true')
    parser.add_argument('--require-hp-mp-init-dependencies', action='store_true')
    parser.add_argument('--require-ghost-skill-init-dependencies', action='store_true')
    parser.add_argument('--require-frame-foundation-units', action='store_true')
    parser.add_argument('--require-frame-bridge-units', action='store_true')
    parser.add_argument('--require-camera-culling-units', action='store_true')
    parser.add_argument('--require-adam-reconciliation-units', action='store_true')
    args = parser.parse_args()
    initialization = args.require_initialization_units or args.require_debug_persistence_units or args.require_level_construction_unit or args.require_native_monster_dependencies
    lifecycle = args.require_lifecycle_units or initialization
    runtime_dependencies = args.require_runtime_dependency_units or lifecycle
    frame = args.require_frame_units or runtime_dependencies
    acquisition = args.require_acquisition_units or frame
    units = {**UNITS, **(TARGETING_UNITS if args.require_targeting_units or acquisition else {}),
             **(ACQUISITION_UNITS if acquisition else {}),
             **(FRAME_UNITS if frame else {}),
             **(RUNTIME_DEPENDENCY_UNITS if runtime_dependencies else {}),
             **(LIFECYCLE_UNITS if lifecycle else {}),
             **(INITIALIZATION_UNITS if initialization else {}),
             **(DEBUG_PERSISTENCE_UNITS if args.require_debug_persistence_units else {}),
             **(LEVEL_CONSTRUCTION_UNITS if args.require_level_construction_unit else {}),
             **(NATIVE_MONSTER_DEPENDENCIES if args.require_native_monster_dependencies else {}),
             **(CHARACTER_LIST_DEPENDENCIES if args.require_character_list_dependencies else {}),
             **(INIT_CALLBACK_DEPENDENCIES if args.require_init_callback_dependencies else {}),
             **(HP_MP_INIT_DEPENDENCIES if args.require_hp_mp_init_dependencies else {}),
             **(GHOST_SKILL_INIT_DEPENDENCIES if args.require_ghost_skill_init_dependencies else {}),
             **(FRAME_FOUNDATION_UNITS if args.require_frame_foundation_units else {}),
             **(FRAME_BRIDGE_UNITS if args.require_frame_bridge_units else {}),
             **(CAMERA_CULLING_UNITS if args.require_camera_culling_units else {}),
             **(ADAM_RECONCILIATION_UNITS if args.require_adam_reconciliation_units else {})}
    # Sight has two overload groups; melee caller and radius share one unit.
    source_units = len(units) - int(acquisition) - int(frame) - 5 * int(runtime_dependencies) - 7 * int(lifecycle) - 3 * int(initialization) - 3 * int(args.require_debug_persistence_units) - 2 * int(args.require_native_monster_dependencies)
    source_units -= 4 * int(args.require_character_list_dependencies) + 3 * int(args.require_init_callback_dependencies)
    source_units -= int(args.require_hp_mp_init_dependencies) # adapter already counted in level unit.
    source_units -= int(args.require_ghost_skill_init_dependencies) # skill/faery readers share one source unit.
    source_units -= 3 * int(args.require_frame_foundation_units) # three Session methods extend its existing unit.
    source_units -= 2 * int(args.require_frame_bridge_units) # installer/identity extend the existing Session unit.
    source_units -= len(CAMERA_CULLING_UNITS) * int(args.require_camera_culling_units)
    source_units -= len(ADAM_RECONCILIATION_UNITS) * int(args.require_adam_reconciliation_units)
    raw = args.apk.read_bytes()
    digest = hashlib.sha256(raw).hexdigest()
    if args.artifact:
        artifact_raw = args.artifact.read_bytes()
        artifact = json.loads(artifact_raw)
        assert artifact['validation'] == 'PASS' and artifact['apk_sha256'] == digest
        source_sha256 = artifact['source_sha256']
        artifact_report_sha256 = hashlib.sha256(artifact_raw).hexdigest()
        build_capture_sha256 = None
    else:
        capture_raw = args.build_capture.read_bytes()
        with zipfile.ZipFile(io.BytesIO(capture_raw)) as capture:
            assert capture.testzip() is None, 'Corrupt build capture'
            manifest = json.loads(capture.read('build-capture.json'))
            assert manifest['validation'] == 'BUILD_INPUTS_CAPTURED'
            assert set(capture.namelist()) == set(manifest['entries']) | {'build-capture.json'}, 'Capture entry inventory differs'
            assert len(capture.namelist()) == len(set(capture.namelist())), 'Duplicate capture entries'
            for name, expected in manifest['entries'].items():
                payload = capture.read(name)
                assert len(payload) == expected['bytes'] and hashlib.sha256(payload).hexdigest() == expected['sha256'], ('Capture entry differs', name)
            for path, expected in manifest['source_sha256'].items():
                assert hashlib.sha256(capture.read('source/' + path)).hexdigest() == expected, ('Captured source differs', path)
            packaged = manifest['apks']['packaged']
            assert packaged['sha256'] == digest and packaged['bytes'] == len(raw)
            assert capture.read('packaged-app-debug.apk') == raw
            source_sha256 = manifest['source_sha256']
        artifact_report_sha256 = None
        build_capture_sha256 = hashlib.sha256(capture_raw).hexdigest()
    abis = {}
    with zipfile.ZipFile(io.BytesIO(raw)) as archive:
        for abi in ('arm64-v8a', 'x86_64'):
            exports = {}
            export_libraries = {}
            lua_definitions = []
            debug_backend = []
            native_character_owner = []
            native_libraries = {}
            for path in archive.namelist():
                if not path.startswith(f'lib/{abi}/') or not path.endswith('.so'):
                    continue
                image = archive.read(path)
                elf = ELFFile(io.BytesIO(image))
                assert elf.elfclass == 64
                assert elf['e_machine'] == {'arm64-v8a': 'EM_AARCH64', 'x86_64': 'EM_X86_64'}[abi]
                if args.require_camera_culling_units and Path(path).name in {
                    'libdh2_engine_camera.so', 'libdh2_level_world.so', 'libdh2_native.so'
                }:
                    loads = [segment['p_align'] for segment in elf.iter_segments()
                             if segment['p_type'] == 'PT_LOAD']
                    assert loads and min(loads) >= 16384, (abi, path, loads)
                    native_libraries[Path(path).name] = {
                        'path': path, 'bytes': len(image),
                        'sha256': hashlib.sha256(image).hexdigest(),
                        'load_alignments': loads,
                    }
                if args.require_adam_reconciliation_units and Path(path).name in {
                    'libdh2_game_data.so', 'libdh2_level_world.so'
                }:
                    loads = [segment['p_align'] for segment in elf.iter_segments()
                             if segment['p_type'] == 'PT_LOAD']
                    assert loads and min(loads) >= 16384, (abi, path, loads)
                    native_libraries[Path(path).name] = {
                        'path': path, 'bytes': len(image),
                        'sha256': hashlib.sha256(image).hexdigest(),
                        'load_alignments': loads,
                    }
                symbols = elf.get_section_by_name('.dynsym')
                defined = {s.name for s in symbols.iter_symbols() if s['st_shndx'] != 'SHN_UNDEF'}
                if LUA_SYMBOL in defined:
                    lua_definitions.append(path)
                if path.endswith('/libdh2_native.so') and args.require_native_monster_dependencies:
                    debug_backend = sorted(n for n in defined if n.startswith('_ZN3dh26native11debug_files7Backend10initialize'))
                if path.endswith('/libdh2_native.so') and args.require_character_list_dependencies:
                    native_character_owner = sorted(n for n in defined if n.startswith('_ZN3dh26native14character_list5Owner16enroll_after_add'))
                for unit, prefix in units.items():
                    library = CAMERA_CULLING_LIBRARIES.get(unit) or ADAM_RECONCILIATION_LIBRARIES.get(unit)
                    if library is None:
                        library = 'libdh2_game_data.so' if unit in ('owned_skill_tables', 'owned_faery_tables') else 'libdh2_level_world.so'
                    if path.endswith('/'+library):
                        prefixes = prefix if isinstance(prefix, tuple) else (prefix,)
                        exports[unit] = sorted(n for n in defined
                                               if any(n.startswith(item) for item in prefixes))
                        export_libraries[unit] = path
            assert all(exports.get(unit) for unit in units), (abi, exports)
            expected_target_exports = {
                'dh2_aggro_target_list_init', 'dh2_aggro_target_search', 'dh2_aggro_target_pop',
            }
            if args.require_character_list_dependencies:
                expected_target_exports.update(('dh2_aggro_target_search_character_list', 'dh2_aggro_target_search_object_list'))
            assert set(exports['target_search']) == expected_target_exports, (abi, exports['target_search'])
            assert lua_definitions == [f'lib/{abi}/libdh2_script_runtime.so'], (abi, lua_definitions)
            if args.require_native_monster_dependencies:
                assert debug_backend, (abi, 'real native Debug file backend missing')
            if args.require_character_list_dependencies:
                assert native_character_owner, (abi, 'native owned Character list missing')
            if args.require_camera_culling_units:
                expected_camera_libraries = {
                    'libdh2_engine_camera.so', 'libdh2_level_world.so', 'libdh2_native.so'
                }
                if args.require_adam_reconciliation_units:
                    expected_camera_libraries.add('libdh2_game_data.so')
                assert set(native_libraries) == expected_camera_libraries, (abi, native_libraries)
            if args.require_adam_reconciliation_units:
                for unit, prefix in ADAM_RECONCILIATION_UNITS.items():
                    assert export_libraries.get(unit, '').endswith('/' + ADAM_RECONCILIATION_LIBRARIES[unit]), (abi, unit, export_libraries)
                    prefixes = prefix if isinstance(prefix, tuple) else (prefix,)
                    actual = exports.get(unit, [])
                    assert all(any(name.startswith(wanted) for name in actual)
                               for wanted in prefixes), (abi, unit, prefixes, actual)
                    for source_path in ADAM_RECONCILIATION_SOURCE_PATHS[unit]:
                        assert source_path in source_sha256, (abi, unit, 'source input missing from build capture', source_path)
                expected_libraries = {
                    'libdh2_game_data.so', 'libdh2_level_world.so'
                }
                if args.require_camera_culling_units:
                    expected_libraries |= {'libdh2_engine_camera.so', 'libdh2_native.so'}
                assert expected_libraries <= set(native_libraries), (abi, native_libraries)
            abis[abi] = {'source_unit_exports': exports, 'lua_core_definitions': lua_definitions,
                         'source_unit_export_libraries': export_libraries,
                         'native_debug_file_backend': debug_backend,
                         'native_character_list_owner': native_character_owner,
                         'native_libraries_16k_aligned': native_libraries}
    scope = f'{source_units} bounded source AI/script units compiled and exported for ELF64 ARM64/x86_64; one reused Adam Lua core per ABI. Export verification establishes compilation only; live AI/controller/body behavior requires separate device evidence.'
    if args.require_camera_culling_units:
        scope += f' Also verifies {len(CAMERA_CULLING_UNITS)} camera/culling/visibility/Stop export groups and 16 KiB PT_LOAD alignment for camera, level-world and native libraries.'
    if args.require_adam_reconciliation_units:
        scope += f' Also verifies {len(ADAM_RECONCILIATION_UNITS)} Adam reconciliation export groups in game-data/level-world and 16 KiB PT_LOAD alignment; these are compilation checks, not live inventory/loot behavior.'
    report = {
        'validation': 'PASS', 'apk_sha256': digest, 'apk_bytes': len(raw),
        'artifact_report_sha256': artifact_report_sha256,
        'build_capture_sha256': build_capture_sha256,
        'validation_basis': 'prior verified artifact' if args.artifact else 'captured Gradle/Ninja build inputs and packaged APK',
        'abis': abis, 'source_sha256': source_sha256,
        'compiled_source_units': source_units,
        'camera_culling_export_groups': len(CAMERA_CULLING_UNITS) if args.require_camera_culling_units else 0,
        'adam_reconciliation_export_groups': len(ADAM_RECONCILIATION_UNITS) if args.require_adam_reconciliation_units else 0,
        'adam_reconciliation_source_paths': ADAM_RECONCILIATION_SOURCE_PATHS if args.require_adam_reconciliation_units else {},
        'required_export_groups': len(units),
        'native_ai_wired': False, 'full_game_playable': False,
        'scope': scope,
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'validation': 'PASS', 'apk_sha256': digest, 'abis': list(abis),
                      'source_units': source_units, 'native_ai_wired': False}))


if __name__ == '__main__':
    main()
