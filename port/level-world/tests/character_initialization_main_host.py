"""Actual CMake DSOs: monster Init ownership, services and Lua object regressions.

World/session projections remain explicit fixtures. This does not establish
live Android execution, full enemy frames, timer expiry or campaign behavior.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import struct
import re

ROOT = Path(__file__).resolve().parents[3]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def linux(path):
    return '/mnt/c/'+str(path.resolve()).replace('\\','/')[3:]


def npc_identity_projection(raw):
    """Preserve every fixture byte except process-local owner/physical identities.

    NPB1 emits actual native pointers. Validate both body user-data fields
    against that row's captured physical identity before canonicalizing them.
    All geometry, pose, bounds, flags and creation requests stay byte-exact.
    """
    result = bytearray(raw)
    magic,count = struct.unpack_from('<2I',raw)
    assert magic == 0x3142504e and count == 11
    at,identities = 8,{}
    for index in range(count):
        assert struct.unpack_from('<I',raw,at)[0] == index
        entries = struct.unpack_from('<I',raw,at+88)[0]
        assert entries <= 128
        at += 92+40*entries
        skins = struct.unpack_from('<I',raw,at)[0]
        assert skins <= 128
        at += 4
        for _ in range(skins):
            joints,boxes = struct.unpack_from('<2I',raw,at)
            assert joints <= 4096 and boxes <= 4096
            at += 8+68*joints+24*boxes
        owner,physical = struct.unpack_from('<2Q',raw,at)
        assert owner and physical and owner != physical
        # The fixture borrows loop-local identity slots, reused across rows.
        # Alpha-renaming preserves every cross-row/role alias exactly while
        # allowing those stack addresses to differ between native processes.
        canonical_owner = identities.setdefault(owner,len(identities)+1)
        canonical_physical = identities.setdefault(physical,len(identities)+1)
        struct.pack_into('<2Q',result,at,canonical_owner,canonical_physical)
        at += 16+128+56
        assert struct.unpack_from('<Q',raw,at)[0] == physical
        assert struct.unpack_from('<Q',raw,at+64)[0] == physical
        struct.pack_into('<Q',result,at,canonical_physical)
        struct.pack_into('<Q',result,at+64,canonical_physical)
        at += 208
    assert at == len(raw)
    return bytes(result)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', default='/home/adampalace/dh2-world-build')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.output.exists():
        raise RuntimeError('Preserve existing main initialization proof')
    world = ROOT/'port/level-world'
    data = ROOT/'port/game-data'
    runtime = ROOT/'port/script-runtime'
    modules = ('character_script_session','character_design_services','character_game_design',
        'character_host_context','character_spatial_bindings','character_target_bindings',
        'gameobject_lua_representation','character_script_owner')
    sources = [world/'CMakeLists.txt', data/'CMakeLists.txt', runtime/'CMakeLists.txt', Path(__file__)]
    for module in modules:
        sources += [world/(module+suffix) for suffix in ('.hpp','.cpp')]
        sources.append(world/'tests'/(module+'.cpp'))
    sources += [world/'tests/character_script_owner_integers.cpp',
                world/'reference/character-game-design/registered_names.inc']
    sources += [runtime/name for name in ('script_runtime.c','script_runtime.h',
        'script_object_bridge.c','script_object_bridge.h','script_object_bridge_internal.h')]
    sources += [runtime/'tests'/(name+'.cpp') for name in
                ('script_callback_scope','script_include','script_vm_ownership')]
    sources += [runtime/'lua/lgc.c', runtime/'lua/ltable.c', runtime/'tests/lua514.cpp']
    sources += [world/'character_timer_effects.hpp',world/'character_timer_effects.cpp',
                world/'tests/character_timer_effects.cpp',world/'tests/character_script_session_targets.cpp']
    sources += [world/(name+suffix) for name in ('character_state','object_identity') for suffix in ('.hpp','.cpp')]
    sources += [world/'tests'/(name+'.cpp') for name in ('character_state','object_identity','object_target_events')]
    sources += [world/(name+suffix) for name in ('character_state_owner','character_buffs') for suffix in ('.hpp','.cpp')]
    sources += [world/'character_state_owner_data.inc']
    sources += [world/'tests'/(name+'.cpp') for name in ('character_state_owner','character_buffs')]
    sources += [world/(name+suffix) for name in ('character_state_owner_behavior','character_state_owner_frame','character_script_commands','character_target_update','character_target_events','character_controller_commands','character_path_commands') for suffix in ('.hpp','.cpp')]
    sources += [world/'tests'/(name+'.cpp') for name in ('character_state_owner_behavior','character_state_owner_frame','character_script_commands','character_target_update','character_target_events')]
    sources += [world/'character_ai_events.hpp',world/'character_ai_events.cpp',world/'tests/character_ai_events.cpp']
    sources += [world/(name+suffix) for name in ('character_dot_attack','character_script_objects','character_state_empty') for suffix in ('.hpp','.cpp')]
    sources += [world/'tests'/(name+'.cpp') for name in ('character_dot_attack','character_script_objects','character_state_empty','character_target_event_route')]
    sources += [world/'reference/character-state-methods/native-methods.inc']
    sources += [world/(name+suffix) for name in ('character_enemy_spotted','character_pre_spawn') for suffix in ('.hpp','.cpp')]
    sources += [world/'tests'/(name+'.cpp') for name in ('character_enemy_spotted','character_pre_spawn','character_target_pipeline')]
    sources += [world/(name+suffix) for name in ('character_hit','character_target_providers') for suffix in ('.hpp','.cpp')]
    sources += [world/'tests/character_hit.cpp',data/'properties.hpp',data/'properties.cpp']
    sources += [world/'character_state_owner_extensions.hpp',world/'character_state_owner_extensions.cpp',
                world/'tests/character_state_owner_extensions.cpp']
    sources += [world/'character_clear_aggro.hpp',world/'character_clear_aggro.cpp',
                world/'tests/character_clear_aggro.cpp',data/'aggro.hpp',data/'aggro.cpp']
    sources += [world/'tests/character_player_objects.cpp']
    sources += [world/'tests/character_object_position.cpp',world/'tests/character_owned_target_pipeline.cpp']
    sources += [world/(name+suffix) for name in ('character_spawn_select','character_spawn_permission','character_spawn_body','character_kill') for suffix in ('.hpp','.cpp')]
    sources += [world/'tests'/(name+'.cpp') for name in ('character_spawn_select','character_spawn_body','character_kill')]
    sources += [data/'design_settings.hpp',data/'design_settings.cpp',data/'tests/design_settings.cpp']
    sources += [world/(name+suffix) for name in ('character_spawn_owner_extensions','character_ai_death','character_cancel_sneaking') for suffix in ('.hpp','.cpp')]
    sources += [world/'tests'/(name+'.cpp') for name in ('character_spawn_owner_extensions','character_ai_death','character_cancel_sneaking')]
    sources += [world/'character_dead_select.hpp',world/'character_dead_select.cpp',world/'tests/character_dead_select.cpp',
                data/'skill_tables.hpp',data/'skill_tables.cpp',data/'tests/skill_tables.cpp']
    sources += [world/(name+suffix) for name in ('character_sneaking_tables','actor_initialization','character_animation_instance') for suffix in ('.hpp','.cpp')]
    sources += [world/'tests'/(name+'.cpp') for name in ('character_sneaking_tables','actor_initialization','character_animation_instance')]
    sources += [data/(name+suffix) for name in ('level_tables','game_design_tables') for suffix in ('.cpp','.hpp')]
    for name in ('character_ai_state_changed','character_ai_state_changed_vm','character_idle_update','character_npc_body','character_idle_events'):
        sources += [world/(name+suffix) for suffix in ('.hpp','.cpp')]
        sources.append(world/'tests'/(name+'.cpp'))
    sources.append(world/'character_idle_event_tables.inc')
    sources += [world/'visual_fx_preload.hpp',world/'visual_fx_preload.cpp',world/'tests/visual_fx_preload.cpp']
    sources += [world/'visual_fx_tables.hpp',world/'visual_fx_tables.cpp',world/'tests/visual_fx_tables.cpp',
                data/'effects_tables.hpp',data/'effects_tables.cpp']
    sources += [world/'character_init_fx.hpp',world/'character_init_fx.cpp',world/'tests/character_init_fx.cpp']
    sources += [world/'character_can_update.hpp',world/'character_can_update.cpp',world/'tests/character_can_update.cpp']
    animation = ROOT/'port/engine-animation'
    sources += [animation/'CMakeLists.txt',animation/'material_color.hpp',animation/'material_color.cpp',animation/'tests/material_color.cpp']
    for name in ('character_deferred_script','character_update_startup'):
        sources += [world/(name+suffix) for suffix in ('.hpp','.cpp')]
        sources.append(world/'tests'/(name+'.cpp'))
    sources += [world/'character_deferred_script_session.cpp',world/'tests/character_deferred_script_session.cpp',
                world/'tests/character_delayed_idle.cpp',world/'character_script_lifecycle.hpp',
                world/'character_script_lifecycle.cpp',world/'tests/character_script_lifecycle.cpp']
    sources += [animation/'particle_parameter.hpp',animation/'particle_parameter.cpp',animation/'tests/particle_parameter.cpp']
    sources += [animation/'particle_factory.hpp',animation/'particle_factory.cpp',animation/'tests/particle_factory.cpp',
                world/'character_deferred_queue.hpp',world/'character_deferred_queue.cpp',world/'tests/character_deferred_queue.cpp']
    sources += [world/'tests/character_script_source_services.cpp',world/'tests/character_deferred_required.cpp']
    sources += [world/'character_update_queued.hpp',world/'character_update_queued.cpp',
                world/'tests/character_update_queued.cpp',world/'tests/character_update_queued_session.cpp',
                animation/'particle_emission.hpp',animation/'particle_emission.cpp',animation/'tests/particle_emission.cpp']
    sources += [data/'faery_tables.hpp',data/'faery_tables.cpp',data/'tests/faery_tables.cpp',
                world/'character_skills.hpp',world/'character_skills.cpp',world/'character_skills_owner.cpp',
                world/'character_skills_session.hpp',world/'character_skills_session.cpp',
                world/'tests/character_skills.cpp',world/'tests/character_skills_session.cpp']
    sources += [world/'character_script_init_vitals.hpp',world/'character_script_init_vitals.cpp',
                world/'character_script_init_vitals_session.cpp',world/'tests/character_script_init_vitals.cpp',
                world/'tests/character_script_init_vitals_session.cpp',world/'tests/character_delayed_idle_vitals.cpp',
                world/'tests/character_script_init_vitals_linked.cpp']
    payloads = ROOT/'port/asset-payloads'
    ui = ROOT/'port/engine-ui'
    scene = ROOT/'port/scene-materials'
    sources += [ui/'CMakeLists.txt',ui/'gfnt.hpp',ui/'gfnt.cpp',ui/'tests/gfnt.cpp',
                scene/'shader_sources.hpp',scene/'shader_sources.cpp',scene/'tests/shader_sources.cpp',
                scene/'swf_texture.hpp',scene/'swf_texture.cpp',scene/'tests/swf_texture.cpp']
    sources += [ui/'gameswf_sources.cmake',ui/'freetype237.cmake',
                ui/'tests/swf_movie.cpp',ui/'tests/freetype_font.cpp']
    for name in ('swf_movie','freetype_font','freetype_glyph_kernel',
                 'swf_freetype_provider','swf_font_geometry'):
        sources += [ui/(name+suffix) for suffix in ('.hpp','.cpp')]
    sources += [ui/'freetype237-hud.cmake']
    for name in ('swf_font_resolver','viewport','localization'):
        sources += [ui/(name+suffix) for suffix in ('.hpp','.cpp')]
        sources.append(ui/'tests'/(name+'.cpp'))
    sources += [ui/'tests/swf_font_resolver_hud.cpp',ui/'tests/localization_connection.cpp']
    for name in ('freetype_bitmap_alpha','hud_freetype_font','swf_hud_freetype_provider',
                 'swf_glyph_lookup','hud_player_values'):
        sources += [ui/(name+suffix) for suffix in ('.hpp','.cpp')]
    connected_modules=('swf_viewport_connection','hud_sprite_timeline','hud_sprite_core',
                       'hud_advance','hud_advance_owner','player_status_hud',
                       'swf_actionscript_connection','renderfx_text_connection',
                       'hud_startup_callbacks','game_option_table_v1','owned_hud_settings_v1',
                       'settings_native_files_v1','settings_language_scene_v1',
                       'hud_manager','hud_manager_core','hud_manager_backends','text_layout_v1',
                       'hud_initialization_v1','hud_initialization_core_v1',
                       'hud_initialization_owned_v1','hud_manager_core_v2',
                       'text_display_v2','text_render_owner_v2','hud_freetype_font_v2',
                       'hud_text_format_v1','hud_text_v1')
    for name in connected_modules:
        sources += [ui/(name+suffix) for suffix in ('.hpp','.cpp')]
    connected_tests=('swf_viewport_connection','hud_sprite_timeline','hud_advance',
                     'gameswf_font_overlay','player_status_hud','swf_actionscript_connection',
                     'hud_startup','owned_settings_v1','settings_language_scene_v1',
                     'hud_manager','hud_manager_core','hud_manager_backends','hud_manager_reentry','text_layout_v1',
                     'hud_initialization_v1','hud_initialization_core_v1',
                     'text_display_v2','text_render_owner_v2','hud_text_format_v1','hud_skill_text_v1')
    sources += [ui/'tests'/(name+'.cpp') for name in connected_tests]
    owned_player_modules=('player_savegame_v1','item_inventory_v1','player_profile_index_v1',
                          'loot_tables_v2','fresh_inventory_v2')
    owned_player_tests=('player_savegame_v1','player_faeries_v1','player_metadata_v1',
                        'item_inventory_v1','inventory_potions_v1','player_profile_index_v1',
                        'loot_tables_v2','fresh_inventory_v2')
    sources += [world/(name+suffix) for name in ('player_initial_grants_v2',) for suffix in ('.hpp','.cpp')]
    sources += [world/'tests/player_initial_grants_v2.cpp']
    formatting_world_modules=('character_skill_info_v1','character_skill_info_session_v1',
                              'character_skill_properties_v1','character_hud_skill_text_v1')
    sources += [world/(name+suffix) for name in formatting_world_modules for suffix in ('.hpp','.cpp')]
    sources += [world/'tests/character_skill_info_v1.cpp',runtime/'script_runtime_return_v1.c',
                runtime/'script_return_observer_v1.h',runtime/'tests/script_first_return_v1.cpp']
    sources += [data/(name+suffix) for name in owned_player_modules for suffix in ('.hpp','.cpp')]
    sources += [data/'tests'/(name+'.cpp') for name in owned_player_tests]
    sources += [ui/'gameswf_font_overlay_v1.cmake',ui/'overlays/font-v1/gameswf_font.cpp']
    sources += [ui/'tests'/(name+'.cpp') for name in
                ('hud_freetype_font','hud_freetype_provider','swf_glyph_lookup','hud_player_values')]
    source_input_modules=('swf_cursor_input','swf_input_geometry','swf_input_policy',
        'swf_input_history','swf_input_connection','swf_input_connection_v2',
        'swf_event_dispatch','swf_event_core','swf_frame_schedule','swf_frame_connection',
        'swf_drag_values','swf_source_movie_v1','swf_input_session_v1','swf_input_session_v2',
        'swf_source_startup_v1')
    for name in source_input_modules:
        sources += [ui/(name+suffix) for suffix in ('.hpp','.cpp') if (ui/(name+suffix)).exists()]
    source_overlay_recipes=('gameswf_input_overlay_v1.cmake','gameswf_frame_overlay_v1.cmake',
        'gameswf_player_lifetime_overlay_v1.cmake','gameswf_loader_lifetime_overlay_v1.cmake',
        'gameswf_source_facade_v1.cmake')
    sources += [ui/name for name in source_overlay_recipes]
    source_overlay_files=[path for folder in ('input-v1','frame-v1','player-lifetime-v1',
        'loader-lifetime-v1','source-facade-v1') for path in (ui/'overlays'/folder).glob('*.cpp')]
    sources += source_overlay_files
    source_input_tests=('swf_input_session_v1','swf_source_session_v2','swf_cursor_input',
        'swf_frame_connection','swf_frame_gold','swf_cursor_input_gold','swf_cursor_gold_libm_v1')
    sources += [ui/'tests'/(name+'.cpp') for name in source_input_tests]
    sources += [ui/'tests/overlays/source-facade-v1/swf_viewport_connection.cpp']
    sources += [ROOT/'port/android-native/app/src/main/cpp'/('original_ui_input_session_v1'+suffix)
                for suffix in ('.hpp','.cpp')]
    sources += [data/('fresh_inventory_owned_v4'+suffix) for suffix in ('.hpp','.cpp')]
    sources += [data/'tests/fresh_inventory_owned_v4.cpp']
    item_data_modules_v5=('item_gear_properties_v5','item_power_tables_v5',
                          'item_presentation_v5','player_gear_effects_v5')
    item_ui_modules_v5=('item_text_varargs_v5','item_text_owner_v5')
    item_data_tests_v5=('item_gear_properties_v5','item_presentation_v5',
        'item_presentation_v5_fixture','item_power_instance_v5_fixture',
        'player_gear_effects_v5','player_gear_cache_v5','player_skin_v5','player_skin_v5_fixture')
    item_ui_tests_v5=('item_text_varargs_v5','item_text_varargs_v5_fixture')
    sources += [data/(name+suffix) for name in item_data_modules_v5 for suffix in ('.hpp','.cpp')]
    sources += [ui/(name+suffix) for name in item_ui_modules_v5 for suffix in ('.hpp','.cpp')]
    sources += [data/'tests'/(name+'.cpp') for name in item_data_tests_v5]
    sources += [ui/'tests'/(name+'.cpp') for name in item_ui_tests_v5]
    player_skill_modules_v2=('character_script_owner_v2', 'character_script_session_v2', 'character_skills_session_v2', 'character_skill_info_session_v2', 'character_script_player_vcb_v2', 'character_current_skill_v2', 'character_player_skills_v2')
    sources += [world/(name+suffix) for name in player_skill_modules_v2 for suffix in ('.hpp','.cpp')]
    script_resource_modules_v1=('character_script_assets_v1','character_current_spell_v1')
    sources += [world/(name+suffix) for name in script_resource_modules_v1 for suffix in ('.hpp','.cpp')]
    sources += [world/'tests'/(name+'.cpp') for name in script_resource_modules_v1]
    sources += [payloads/'zip_asset_pack_v1.hpp',payloads/'zip_asset_pack_v1.cpp']
    sources += [world/'tests'/(name+'.cpp') for name in ('character_skill_session_v2','character_player_skills_v2')]
    skill_integration_v3=json.loads((world/'reference/character-skill-gameplay-v3/integration.json').read_text())
    player_skill_modules_v3=tuple(Path(p).stem for p in skill_integration_v3['production_world_sources'])
    sources += [world/(name+suffix) for name in player_skill_modules_v3 for suffix in ('.hpp','.cpp')]
    sources += [runtime/'script_runtime_return_v3.c',runtime/'script_return_observer_v3.h']
    sources += [ROOT/t['source'] for t in skill_integration_v3['tests']]
    loot_modules_v7=('loot_power_resources_v7','loot_power_creation_v7')
    loot_tests_v7=('loot_power_creation_v7','loot_power_creation_v7_fixture')
    sources += [data/(name+suffix) for name in loot_modules_v7 for suffix in ('.hpp','.cpp')]
    sources += [data/'tests'/(name+'.cpp') for name in loot_tests_v7]
    core_manifest_path = ui/'reference/gameswf-core/vendor-manifest.json'
    core_manifest = json.loads(core_manifest_path.read_text())
    for entry in core_manifest['files']:
        path = ui/'vendor/gameswf1714'/entry['path']
        assert sha(path) == entry['sha256']
        sources.append(path)
    ft_manifest_path = ui/'reference/freetype-font/native-source-manifest-current.json'
    ft_manifest = json.loads(ft_manifest_path.read_text())
    for name,digest in ft_manifest['files'].items():
        path = ui/'vendor/freetype-2.3.7'/name
        assert sha(path) == digest
        sources.append(path)
    hud_ft_manifest_path = ui/'reference/swf-font-resolver/hud-vendor-manifest.json'
    hud_ft_manifest = json.loads(hud_ft_manifest_path.read_text())
    for name,digest in hud_ft_manifest['files'].items():
        path = ui/'vendor/freetype-2.3.7-hud'/name
        assert sha(path) == digest
        sources.append(path)
    sources += [payloads/'sha256.hpp',payloads/'sha256.cpp',payloads/'tests/sha256.cpp',
                ROOT/'port/scene-materials/CMakeLists.txt']
    hashes = {path.relative_to(ROOT).as_posix():sha(path) for path in sources}
    historical_proof_changes = {}
    baseline_root = ROOT/'.local-inputs/script-owner-source-services-baseline'
    baseline_path = baseline_root/'baseline.json'
    baseline = json.loads(baseline_path.read_text())
    migration_files = {
        'port/level-world/character_script_owner.hpp','port/level-world/character_script_owner.cpp',
        'port/level-world/character_script_session.hpp','port/level-world/character_script_session.cpp',
        'port/level-world/character_deferred_script_session.cpp',
        'port/script-runtime/script_runtime.h','port/script-runtime/script_runtime.c'}
    assert set(baseline['source_sha256']) == migration_files
    assert baseline['central74_report_sha256'] == sha(world/'reports/character-queue-particle-factory-main-linked-host-audit.json')
    for key,digest in baseline['source_sha256'].items():
        assert sha(baseline_root/key) == digest
    # Two exact preserved facade baselines only. Current facade bytes must
    # equal the new retained-AS receipt; all prior suites execute below.
    facade_baseline=ROOT/'.local-inputs/swf-movie-live-baseline'
    facade_hashes=json.loads((facade_baseline/'sha256.json').read_text())
    assert facade_hashes=={
        'swf_movie.cpp':'38c5bfcf44cb4e2a0c6ff64ea3688502560afc6c2629105890c9c527c2b564ad',
        'swf_movie.hpp':'6de117225b6e512c5f65d1a43f5d8335a6b8d039f50fb946cd5d04891b246961'}
    for name,digest in facade_hashes.items():
        assert sha(facade_baseline/name)==digest
    as_freeze_path=ui/'reference/swf-actionscript-connection-v1/freeze-manifest.json'
    as_freeze=json.loads(as_freeze_path.read_text())
    assert as_freeze['validation']=='PASS'
    for mapping in ('source_sha256','proof_sha256'):
        for name,digest in as_freeze[mapping].items():
            assert sha(ROOT/name)==digest
    def check_source(name,digest):
        key = name.replace('\\','/')
        current = sha(ROOT/key)
        if current == digest:
            return
        if key.startswith('port/engine-ui/') and key.rsplit('/',1)[-1] in facade_hashes:
            assert key=='port/engine-ui/'+key.rsplit('/',1)[-1]
            assert facade_hashes[key.rsplit('/',1)[-1]]==digest
            assert as_freeze['source_sha256'][key]==current
            historical_proof_changes[key]=dict(historical=digest,current=current,
                retained_baseline=(facade_baseline/key.rsplit('/',1)[-1]).relative_to(ROOT).as_posix(),
                reason='Frozen exact retained typed AS ownership/startup callbacks, quiescent property teardown, source viewport and HUD APIs; current AS/status and all historical facade regressions executed')
            return
        assert key in migration_files and baseline['source_sha256'][key] == digest, (key,digest,current)
        historical_proof_changes[key] = dict(historical=digest,current=current,
            retained_baseline=(baseline_root/key).relative_to(ROOT).as_posix(),
            reason='Additive path/cache/call/InitVCB services and typed required-failure provenance; current Source_services and Required_deferred plus all prior central regressions executed')
    npc_handoff_path = world/'reference/character-npc-body/handoff.json'
    npc_handoff = json.loads(npc_handoff_path.read_text())
    assert npc_handoff['validation'] == 'PASS'
    for key in ('source_sha256','reports_sha256','current_kernel_source_sha256','reference_sha256'):
        for name,digest in npc_handoff[key].items():
            check_source(name,digest)
    proofs = {baseline_path.relative_to(ROOT).as_posix():sha(baseline_path)}
    spell_freeze_path=world/'reference/character-current-spell-v1/freeze-manifest.json'
    spell_freeze=json.loads(spell_freeze_path.read_text())
    assert spell_freeze['validation']=='PASS' and spell_freeze['original_cases']==512
    for name,digest in spell_freeze['files'].items():
        assert sha(ROOT/name)==digest
        proofs[name]=digest
    proofs[spell_freeze_path.relative_to(ROOT).as_posix()]=sha(spell_freeze_path)
    item_freeze_path_v5=data/'reference/player-item-effects-v5/freeze-manifest.json'
    item_freeze_v5=json.loads(item_freeze_path_v5.read_text())
    assert item_freeze_v5['validation']=='PASS'
    for mapping in ('production_source_sha256','source_sha256','proof_sha256','reference_sha256'):
        for name,digest in item_freeze_v5[mapping].items():
            assert sha(ROOT/name)==digest,(name,digest,sha(ROOT/name))
            proofs[name]=digest
    proofs[item_freeze_path_v5.relative_to(ROOT).as_posix()]=sha(item_freeze_path_v5)
    skill_freeze_path_v3=world/'reference/character-skill-gameplay-v3/freeze-manifest.json'
    skill_freeze_v3=json.loads(skill_freeze_path_v3.read_text())
    assert skill_freeze_v3['validation']=='PASS'
    for name,digest in skill_freeze_v3['source_and_evidence_sha256'].items():
        assert sha(ROOT/name)==digest,(name,digest,sha(ROOT/name))
        proofs[name]=digest
    proofs[skill_freeze_path_v3.relative_to(ROOT).as_posix()]=sha(skill_freeze_path_v3)
    loot_freeze_path_v7=data/'reference/loot-power-creation-v7/freeze-manifest.json'
    loot_freeze_v7=json.loads(loot_freeze_path_v7.read_text())
    assert loot_freeze_v7['validation']=='PASS' and loot_freeze_v7['original_cases']==4781
    for mapping in ('production_source_sha256','source_sha256','proof_sha256',
                    'reference_sha256','borrowed_dependency_sha256'):
        for name,digest in loot_freeze_v7[mapping].items():
            assert sha(ROOT/name)==digest,(name,digest,sha(ROOT/name))
            proofs[name]=digest
    proofs[loot_freeze_path_v7.relative_to(ROOT).as_posix()]=sha(loot_freeze_path_v7)
    cache_archive=Path(r'C:\Users\adamc\Downloads\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
    assert sha(cache_archive)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
    proofs[as_freeze_path.relative_to(ROOT).as_posix()]=sha(as_freeze_path)
    proofs.update(as_freeze['proof_sha256'])
    manager_freeze_path=ui/'reference/hud-manager/freeze.json'
    manager_freeze=json.loads(manager_freeze_path.read_text())
    assert manager_freeze['validation']=='FROZEN'
    for name,digest in manager_freeze['files_sha256'].items():
        assert sha(ROOT/name)==digest
        proofs[name]=digest
    proofs[manager_freeze_path.relative_to(ROOT).as_posix()]=sha(manager_freeze_path)
    for freeze_path in (data/'reference/player-inventory-v1/freeze-manifest.json',
                        ui/'reference/text-layout-v1/freeze-manifest.json',
                        ui/'reference/hud-initialization-v1/freeze-manifest.json',
                        ui/'reference/hud-formatting-v1/freeze-manifest.json'):
        freeze=json.loads(freeze_path.read_text())
        assert freeze['validation']=='PASS'
        for name,digest in freeze['source_and_evidence_sha256'].items():
            assert sha(ROOT/name)==digest
            proofs[name]=digest
        proofs[freeze_path.relative_to(ROOT).as_posix()]=sha(freeze_path)
    for freeze_path in (ui/'reference/swf-native-input-frame-v1/freeze-manifest.json',
        ui/'reference/swf-input-session-v1/freeze-manifest.json',
        ui/'reference/swf-source-session-v2/freeze-manifest.json',
        ui/'reference/swf-source-facade-v1/freeze-manifest.json',
        data/'reference/player-inventory-owned-v4/freeze-manifest.json',
        world/'reference/character-skill-session-v2/freeze-manifest.json'):
        frozen=json.loads(freeze_path.read_text())
        assert frozen['validation']=='PASS'
        for group,mapping in frozen.items():
            if group.endswith('sha256') and isinstance(mapping,dict):
                for name,digest in mapping.items():
                    if isinstance(digest,str) and len(digest)==64:
                        assert sha(ROOT/name)==digest,(group,name)
                        proofs[name]=digest
        for old in frozen.get('prior_freezes',[]):
            assert sha(ROOT/old['path'])==old['sha256']
        proofs[freeze_path.relative_to(ROOT).as_posix()]=sha(freeze_path)
    cursor_dso_proof_path=ui/'reports/swf-cursor-dso-gold-v1-host-audit.json'
    cursor_dso_proof=json.loads(cursor_dso_proof_path.read_text())
    assert cursor_dso_proof['validation']=='PASS'
    proofs[cursor_dso_proof_path.relative_to(ROOT).as_posix()]=sha(cursor_dso_proof_path)
    for dirname in ('hud-startup-callbacks','owned-hud-settings-v1'):
        freeze_path=ui/'reference'/dirname/'freeze-manifest.json'
        freeze=json.loads(freeze_path.read_text())
        assert freeze['validation']=='PASS'
        for mapping in ('source_sha256','proof_sha256' if dirname=='hud-startup-callbacks' else 'source_and_evidence_sha256'):
            for name,digest in freeze[mapping].items():
                assert sha(ROOT/name)==digest
                proofs[name]=digest
        proofs[freeze_path.relative_to(ROOT).as_posix()]=sha(freeze_path)
    ui_freeze_path = ui/'reference/gfnt/freeze-manifest.json'
    ui_freeze = json.loads(ui_freeze_path.read_text())
    assert ui_freeze['validation'] == 'FROZEN'
    for mapping in ('source_sha256','reports'):
        for name,digest in ui_freeze[mapping].items():
            check_source(name,digest)
    assert ui_freeze['gold_sha256'] == sha(ui/'reference/gfnt/original-fixtures.bin')
    for name in ui_freeze['reports']:
        assert json.loads((ROOT/name).read_text())['validation'] == 'PASS'
        proofs[name] = sha(ROOT/name)
    proofs[ui_freeze_path.relative_to(ROOT).as_posix()] = sha(ui_freeze_path)
    shader_freeze_path = scene/'reference/shader-sources/freeze.json'
    shader_freeze = json.loads(shader_freeze_path.read_text())
    assert shader_freeze['validation'] == 'PASS' and shader_freeze['source_frozen']
    for name,digest in shader_freeze['sha256'].items():
        check_source(name,digest)
    proofs[shader_freeze_path.relative_to(ROOT).as_posix()] = sha(shader_freeze_path)
    swf_texture_freeze_path = scene/'reference/swf-render-connection/freeze.json'
    swf_texture_freeze = json.loads(swf_texture_freeze_path.read_text())
    assert swf_texture_freeze['validation'] == 'PASS' and swf_texture_freeze['sanitizer_findings'] == 0
    for name,digest in swf_texture_freeze['source_sha256'].items():
        check_source(name,digest)
    proofs[swf_texture_freeze_path.relative_to(ROOT).as_posix()] = sha(swf_texture_freeze_path)
    core_proof_path = ui/'reports/swf-movie-host-audit-v2.json'
    core_proof = json.loads(core_proof_path.read_text())
    assert core_proof['validation'] == 'PASS' and core_proof['sanitizer']['findings'] == 0
    for name,digest in core_proof['source_sha256'].items():
        check_source(name,digest)
    proofs[core_proof_path.relative_to(ROOT).as_posix()] = sha(core_proof_path)
    proofs[core_manifest_path.relative_to(ROOT).as_posix()] = sha(core_manifest_path)
    ft_freeze_path = ui/'reference/freetype-font/freeze-manifest-current.json'
    ft_freeze = json.loads(ft_freeze_path.read_text())
    for mapping in ('source_sha256','proof_sha256'):
        for name,digest in ft_freeze[mapping].items():
            check_source(name,digest)
    for name in ft_freeze['proof_sha256']:
        if '/reports/' in name:
            assert json.loads((ROOT/name).read_text())['validation'] == 'PASS'
    proofs[ft_freeze_path.relative_to(ROOT).as_posix()] = sha(ft_freeze_path)
    proofs[ft_manifest_path.relative_to(ROOT).as_posix()] = sha(ft_manifest_path)
    resolver_freeze_path = ui/'reference/swf-font-resolver/freeze-manifest.json'
    resolver_freeze = json.loads(resolver_freeze_path.read_text())
    assert resolver_freeze['validation'] == 'FROZEN' and resolver_freeze['prior_vendor_unchanged']
    for mapping in ('source_sha256','proof_sha256'):
        for name,digest in resolver_freeze[mapping].items():
            check_source(name,digest)
    proofs[resolver_freeze_path.relative_to(ROOT).as_posix()] = sha(resolver_freeze_path)
    proofs[hud_ft_manifest_path.relative_to(ROOT).as_posix()] = sha(hud_ft_manifest_path)
    locale_freeze_path = ui/'reference/localization/freeze.json'
    locale_freeze = json.loads(locale_freeze_path.read_text())
    assert locale_freeze['validation'] == 'PASS' and locale_freeze['sanitizer_findings'] == 0
    for name,digest in locale_freeze['source_and_evidence_sha256'].items():
        check_source(name,digest)
    proofs[locale_freeze_path.relative_to(ROOT).as_posix()] = sha(locale_freeze_path)
    viewport_proof_path = ui/'reports/viewport-host-audit.json'
    viewport_proof = json.loads(viewport_proof_path.read_text())
    assert viewport_proof['validation'] == 'PASS' and viewport_proof['sanitizer_findings'] == 0
    for name,digest in viewport_proof['source_sha256'].items():
        check_source('port/engine-ui/'+name,digest)
    proofs[viewport_proof_path.relative_to(ROOT).as_posix()] = sha(viewport_proof_path)
    for relative,mappings,status in (
            ('reference/swf-viewport-connection/freeze-manifest.json',('source_sha256','proof_sha256','reference_sha256'),'PASS'),
            ('reference/hud-sprite-timeline/freeze.json',('files',),'PASS'),
            ('reference/gameswf-font-overlay-v1/freeze-manifest.json',('source_sha256','proof_sha256'),'PASS'),
            ('reference/hud-freetype-font/freeze-manifest.json',('source_sha256','test_source_sha256','proof_sha256'),'PASS'),
            ('reference/swf-glyph-lookup/freeze-manifest.json',('source_sha256','proof_sha256'),'FROZEN'),
            ('reference/hud-player-values/freeze.json',('files',),'PASS')):
        path = ui/relative
        frozen = json.loads(path.read_text())
        assert frozen['validation'] == status
        for mapping in mappings:
            for name,digest in frozen[mapping].items():
                check_source(name,digest)
                if '/reports/' in name:
                    assert json.loads((ROOT/name).read_text())['validation'] == 'PASS'
                    proofs[name] = digest
        proofs[path.relative_to(ROOT).as_posix()] = sha(path)
    for proof_path,key in ((world/'reference/character-can-update/handoff.json','source_and_artifact_sha256'),
                           (world/'reference/character-update-startup/handoff.json','source_and_artifact_sha256'),
                           (world/'reference/character-deferred-queue/handoff.json','source_and_proof_sha256'),
                           (world/'reference/character-update-queued/handoff.json','source_and_proof_sha256'),
                           (animation/'reference/fx-material-animation/native-source-freeze.json','source_sha256'),
                           (animation/'reference/fx-particle-factory/native-source-freeze.json','source_and_evidence_sha256'),
                           (animation/'reference/fx-particle-animation/native-source-freeze.json','source_and_evidence_sha256')):
        proof = json.loads(proof_path.read_text())
        assert proof['validation'] == 'PASS'
        for source,digest in proof[key].items():
            check_source(source,digest)
        proofs[proof_path.relative_to(ROOT).as_posix()] = sha(proof_path)
    emission_freeze_path = animation/'reference/fx-particle-emission/native-source-freeze.json'
    emission_freeze = json.loads(emission_freeze_path.read_text())
    assert emission_freeze['validation'] == 'PASS'
    for key in ('source_sha256','evidence_sha256'):
        for name,digest in emission_freeze[key].items():
            check_source(name,digest)
    proofs[emission_freeze_path.relative_to(ROOT).as_posix()] = sha(emission_freeze_path)
    skills_freeze_path = world/'reference/character-skills/freeze-manifest.json'
    skills_freeze = json.loads(skills_freeze_path.read_text())
    assert skills_freeze['validation'] == 'FROZEN'
    for name,digest in skills_freeze['source_sha256'].items():
        check_source(name,digest)
    for name in skills_freeze['reports']:
        path = ROOT/name
        proof = json.loads(path.read_text())
        assert proof['validation'] == 'PASS'
        for source,digest in proof['source_sha256'].items():
            check_source(source,digest)
        if 'host_audit' in proof:
            assert proof['host_audit']['validation'] == 'PASS' and proof['sanitizers']['findings'] == 0
        for source,digest in proof.get('input_sha256',{}).items():
            input_path = ROOT/source if '/' in source else ROOT/'.local-inputs/character-skills'/source
            assert sha(input_path) == digest
        proofs[name] = sha(path)
    proofs[skills_freeze_path.relative_to(ROOT).as_posix()] = sha(skills_freeze_path)
    for name in ('character-deferred-script-host-audit.json','character-deferred-script-arm64-differential.json'):
        path = world/'reports'/name
        proof = json.loads(path.read_text())
        assert proof['validation'] == 'PASS'
        for source,digest in proof['source_sha256'].items():
            check_source(source,digest)
        assert proof.get('mismatches',proof.get('host_audit',{}).get('mismatches')) == 0
        assert proof.get('gold_sha256',proof.get('corpus_sha256')) == sha(world/'reference/character-deferred-script/deferred-fixtures.bin')
        proofs[path.relative_to(ROOT).as_posix()] = sha(path)
    for name in ('character-script-init-vitals-host-audit.json','character-script-init-vitals-arm64-differential.json'):
        path = world/'reports'/name
        proof = json.loads(path.read_text())
        assert proof['validation'] == 'PASS'
        for source,digest in proof['source_sha256'].items():
            check_source(source,digest)
        assert proof.get('mismatches',proof.get('host_audit',{}).get('mismatches')) == 0
        assert proof['gold_sha256'] == sha(world/'reference/character-script-init-services/vitals-fixtures.bin')
        proofs[path.relative_to(ROOT).as_posix()] = sha(path)
    for name in ('character-init-fx-host-audit.json','character-init-fx-arm64-differential.json'):
        proof_path = world/'reports'/name
        proof = json.loads(proof_path.read_text())
        assert proof['validation'] == 'PASS'
        for source,digest in proof['source_sha256'].items():
            check_source(source,digest)
        assert proof.get('mismatches',proof.get('host_audit',{}).get('mismatches')) == 0
        assert proof.get('binary_gold_sha256',proof.get('reference_sha256')) == sha(world/'reference/character-init-fx/init-fx-fixtures.bin')
        proofs[proof_path.relative_to(ROOT).as_posix()] = sha(proof_path)
    idle_events_path = world/'reports/character-idle-events-host-audit.json'
    idle_events_proof = json.loads(idle_events_path.read_text())
    assert idle_events_proof['validation'] == idle_events_proof['audit']['validation'] == 'PASS'
    assert idle_events_proof['sanitizer_findings'] == 0
    assert idle_events_proof['whole_composition_original_instruction_differential'] is False
    for name,digest in idle_events_proof['source_and_input_sha256'].items():
        check_source(name,digest)
    proofs[idle_events_path.relative_to(ROOT).as_posix()] = sha(idle_events_path)
    fx_freeze_path = world/'reference/character-skeleton-fx/native-source-freeze.json'
    fx_freeze = json.loads(fx_freeze_path.read_text())
    assert fx_freeze['validation'] == 'PASS' and fx_freeze['registration_ready']
    assert fx_freeze['native_FX_playback_ready'] is False
    for name,digest in fx_freeze['source_and_proof_sha256'].items():
        check_source(name,digest)
    proofs[fx_freeze_path.relative_to(ROOT).as_posix()] = sha(fx_freeze_path)
    fx_differential_path = world/'reports/visual-fx-preload-arm64-differential.json'
    fx_differential = json.loads(fx_differential_path.read_text())
    assert fx_differential['validation'] == 'PASS' and fx_differential['mismatches'] == 0
    assert fx_differential['binary_gold_sha256'] == sha(world/'reference/character-skeleton-fx/preload-fixtures.bin')
    proofs[fx_differential_path.relative_to(ROOT).as_posix()] = sha(fx_differential_path)
    effects_root = data/'reference/effects-tables'
    effects_proof_path = effects_root/'original-reader-projection.json'
    effects_proof = json.loads(effects_proof_path.read_text())
    assert effects_proof['validation'] == 'PASS'
    assert effects_proof['original_sha256'] == sha(ROOT/'.local-inputs/libDungeonHunter2.so')
    assert effects_proof['script_sha256'] == sha(effects_root/'prepare_original.py')
    assert effects_proof['manifest_sha256'] == sha(effects_root/'original-functions.json')
    assert effects_proof['assembly_sha256'] == sha(effects_root/'original-functions.asm')
    assert effects_proof['canonical_projection_sha256'] == sha(effects_root/'original-reader-projection.bin')
    assert effects_proof['consumed'] == dict(sets=12700,characters=12755,footsteps=12953,dictionary=16770)
    assert len(effects_proof['sets']) == 276 and sum(len(row['steps']) for row in effects_proof['sets']) == 276
    assert len(effects_proof['character_rows']) == 3 and len(effects_proof['footstep_rows']) == 7
    assert len(effects_proof['dictionary_paths']) == 284
    for name,record in effects_proof['inputs'].items():
        assert sha(effects_root/name) == record['sha256']
    for path in (effects_proof_path,effects_root/'prepare_original.py',effects_root/'original-functions.json',effects_root/'original-functions.asm'):
        proofs[path.relative_to(ROOT).as_posix()] = sha(path)
    proofs[npc_handoff_path.relative_to(ROOT).as_posix()] = sha(npc_handoff_path)
    for name,digest in npc_handoff['reports_sha256'].items():
        proof = json.loads((ROOT/name).read_text())
        assert proof['validation'] == 'PASS'
        proofs[name] = digest
    for filename in ('character-script-session-host-audit.json','character-design-services-host-audit.json'):
        path = world/'reports'/filename
        proof = json.loads(path.read_text())
        assert proof['validation'] == 'PASS'
        for name,digest in proof['source_sha256'].items():
            key = name.replace('\\','/')
            if key.startswith('port/level-world/') and key in hashes:
                if hashes[key] != digest:
                    assert filename == 'character-script-session-host-audit.json'
                    assert key in ('port/level-world/character_script_session.cpp',
                                   'port/level-world/character_script_session.hpp')
                    historical_proof_changes[key] = dict(historical=digest,current=hashes[key],
                        reason='Additive caller-owned target/FSM/object/command bindings replayed in the current main CMake audit')
        proofs[path.relative_to(ROOT).as_posix()] = sha(path)
    # Bind newly integrated kernels to their frozen original/O2 comparisons,
    # rather than treating a passing host replay as source equivalence by itself.
    for filename, corpus, digest_field in (
        ('character-enemy-spotted-arm64-differential.json',
         'character-enemy-spotted/enemy-spotted-fixtures.bin','corpus_sha256'),
        ('character-pre-spawn-arm64-differential.json',
         'character-pre-spawn/pre-spawn-fixtures.bin','reference_sha256'),
        ('character-hit-arm64-differential.json',
         'character-hit/hit-fixtures.json','gold_sha256'),
        ('character-state-empty-arm64-differential.json',
         'character-state-methods/empty-fixtures.bin','reference_sha256'),
        ('character-clear-aggro-arm64-differential.json',
         'character-clear-aggro/clear-aggro-fixtures.bin','corpus_sha256'),
        ('character-spawn-select-arm64-differential.json',
         'character-spawn-state/spawn-select-fixtures.bin','reference_sha256'),
        ('character-spawn-permission-arm64-differential.json',
         'character-spawn-state/spawn-permission-fixtures.bin','reference_sha256'),
        ('character-spawn-body-arm64-differential.json',
         'character-spawn-body/spawn-body-fixtures.bin','reference_sha256'),
        ('character-kill-arm64-differential.json',
         'character-kill/kill-fixtures.json','gold_sha256'),
        ('character-ai-death-arm64-differential.json',
         'character-ai-death/death-fixtures.json','gold_sha256'),
        ('character-cancel-sneaking-arm64-differential.json',
         'character-cancel-sneaking/cancel-sneaking-fixtures.bin','corpus_sha256'),
        ('character-dead-select-arm64-differential.json',
         'character-dead-select/dead-select-fixtures.bin','binary_gold_sha256'),
        ('character-ai-state-changed-arm64-differential.json',
         'character-ai-state-changed/state-changed-fixtures.bin','corpus_sha256'),
        ('character-idle-update-arm64-differential.json',
         'character-idle-update/idle-fixtures.bin','binary_gold_sha256')):
        path = world/'reports'/filename
        proof = json.loads(path.read_text())
        assert proof['validation'] == 'PASS' and proof['mismatches'] == 0
        for name,digest in proof['source_sha256'].items():
            check_source(name,digest)
        assert sha(world/'reference'/corpus) == proof[digest_field]
        proofs[path.relative_to(ROOT).as_posix()] = sha(path)
    settings_proof = data/'reports/design-settings-arm64-differential.json'
    settings = json.loads(settings_proof.read_text())
    assert settings['validation'] == 'PASS' and settings['mismatches'] == 0
    assert settings['corpus_sha256'] == sha(data/'reference/design-settings/design-settings-fixtures.bin')
    for name,digest in settings['source_sha256'].items():
        check_source(name,digest)
    for name,digest in settings['input_sha256'].items():
        assert sha(ROOT/'.local-inputs/design-settings'/name) == digest
    proofs[settings_proof.relative_to(ROOT).as_posix()] = sha(settings_proof)
    skills_proof = data/'reports/skill-tables-arm64-differential.json'
    skills = json.loads(skills_proof.read_text())
    assert skills['validation'] == 'PASS' and skills['mismatches'] == 0
    assert skills['corpus_sha256'] == sha(data/'reference/skill-tables/skill-tables-fixtures.bin')
    for name,digest in skills['source_sha256'].items():
        check_source(name,digest)
    for name,digest in skills['input_sha256'].items():
        assert sha(ROOT/'.local-inputs/skill-tables'/name) == digest
    proofs[skills_proof.relative_to(ROOT).as_posix()] = sha(skills_proof)
    for path,key in ((world/'reports/character-ai-state-changed-host-audit.json','source_and_input_sha256'),
                     (world/'reports/character-idle-update-host-audit.json','source_sha256'),
                     (payloads/'reports/sha256-host-audit.json','source_sha256')):
        proof = json.loads(path.read_text())
        assert proof['validation'] == proof['host_audit']['validation'] == 'PASS'
        for name,digest in proof[key].items():
            check_source(name,digest)
        proofs[path.relative_to(ROOT).as_posix()] = sha(path)
    for filename,key in (('actor-initialization-host-audit.json','source_sha256'),
                         ('character-sneaking-tables-host-audit.json','source_and_input_sha256')):
        path = world/'reports'/filename
        proof = json.loads(path.read_text())
        assert proof['validation'] == proof['host_audit']['validation'] == 'PASS'
        if filename == 'actor-initialization-host-audit.json':
            assert proof['sanitizers']['diagnostics'] == 0 and proof['mismatches'] == 0
            assert proof['source_records_compared'] == 11 and proof['source_MGP_members_compared'] == 3
            for name,digest in proof['original_proof_sha256'].items():
                check_source(name,digest)
        else:
            assert proof['sanitizer_findings'] == 0
        for name,digest in proof[key].items():
            check_source(name,digest)
        proofs[path.relative_to(ROOT).as_posix()] = sha(path)
    spawn_owner_proof = world/'reports/character-spawn-owner-extensions-host-audit.json'
    spawn_owner = json.loads(spawn_owner_proof.read_text())
    assert spawn_owner['validation'] == spawn_owner['host_audit']['validation'] == 'PASS'
    assert spawn_owner['host_audit']['mismatches'] == 0
    assert '-fsanitize=address,undefined' in spawn_owner['compiler_command']
    for name,digest in spawn_owner['source_sha256'].items():
        check_source(name,digest)
    proofs[spawn_owner_proof.relative_to(ROOT).as_posix()] = sha(spawn_owner_proof)
    endpoint_proof = world/'reference/character-target-event-route/endpoints-original-probe.json'
    proofs[endpoint_proof.relative_to(ROOT).as_posix()] = sha(endpoint_proof)
    scratch = ROOT/'.local-inputs/character-initialization-main/files'
    scratch.mkdir(parents=True,exist_ok=True)
    common = ROOT/'.local-inputs/character-script-owner-extension/_commons.luac'
    monster = ROOT/'.local-inputs/character-script-owner-extension/monster.luac'
    inputs = world/'reference/character-game-design/real-cache-inputs.bin'
    assets = ROOT/'port/android-native/app/src/main/assets'
    actor_manifest = json.loads((world/'reference/actor-initialization/crypt01-actor-initialization.json').read_text())
    assert sha(assets/'worlds/crypt01.dact') == actor_manifest['descriptor_sha256']
    assert sha(world/'reference/actor-initialization/crypt01-actor-initialization.bin') == actor_manifest['binary_sha256']
    monster_assets = ROOT/'.local-inputs/character-monster-bank-assets/assets'
    suites = {
        'sessions': ('character_script_session_audit',[inputs,common,monster,assets/'worlds/crypt01.dact']),
        'debug': ('character_design_services_audit',[world/'reference/character-design-services/debug-services-fixtures.bin',inputs,scratch,common,monster]),
        'objects': ('gameobject_lua_representation_audit',[world/'reference/gameobject-lua-representation/object-methods-original.bin']),
        'targets': ('character_target_bindings_audit',[world/'reference/character-target-bindings/target-original-gold.bin',assets/'data']),
        'spatial': ('character_spatial_bindings_audit',[world/'reference/character-spatial-bindings/spatial-original-gold.bin']),
        'host': ('character_host_context_audit',[world/'reference/character-host-context/host-context-fixtures.bin',data/'reference/level-tables']),
        'design': ('character_game_design_audit',[inputs,data/'reference/game-design-tables/table-fixtures.bin']),
        'owner_integers': ('character_script_owner_integers_audit',[common]),
        'scope': ('script-runtime/script_callback_scope_audit',[]),
        'Include': ('script-runtime/script_include_audit',[]),
        'VM_ownership': ('script-runtime/script_vm_ownership_audit',[]),
        'Lua_GC': ('script-runtime/lua514_audit',[common,scratch/'weak-table-roundtrip.luac']),
        'target_sessions': ('character_script_session_targets_audit',[inputs,common,monster]),
        'state_bodies': ('character_state_audit',[world/'reference/character-state/state-reference.bin']),
        'object_identity': ('object_identity_audit',[world/'reference/object-identity-lifecycle/identity-fixtures.bin']),
        'object_target_events': ('object_target_events_audit',[world/'reference/object-identity-lifecycle/target-events/dispatch-fixtures.bin']),
        'state_owner': ('character_state_owner_audit',[
            world/'reference/character-monster-state-ownership/state-owner-fixtures.bin',
            world/'reference/character-monster-state-ownership/predicate-fixtures.bin']),
        'buffs': ('character_buffs_audit',[
            ROOT/'.local-inputs/character-buffs-discovery/host-fixtures.bin',assets/'data']),
        'state_owner_behavior': ('character_state_owner_behavior_audit',[world/'reference/character-state/state-reference.bin']),
        'state_owner_frame': ('character_state_owner_frame_audit',[
            world/'reference/character-state/state-reference.bin',
            world/'reference/character-native-fsm/native-update-fixtures.bin']),
        'script_commands': ('character_script_commands_audit',[world/'reference/character-script-commands/command-fixtures.bin']),
        'target_update': ('character_target_update_audit',[world/'reference/character-target-update/target-update-fixtures.bin']),
        'target_event_prefixes': ('character_target_events_audit',[world/'reference/character-target-events/target-events-fixtures.bin']),
        'ai_event_route': ('character_ai_events_audit',[world/'reference/character-ai-events/event-fixtures.bin']),
        'target_event_route': ('character_target_event_route_audit',[world/'reference/character-target-event-route/target-route-fixtures.bin']),
        'dot_attack': ('character_dot_attack_audit',[
            ROOT/'.local-inputs/character-dot-attack-discovery/host-fixtures.bin',
            ROOT/'.local-inputs/character-dot-attack-discovery/genuine-missing-DebugSwitches.savegame']),
        'scene_script_objects': ('character_script_objects_audit',[
            inputs,common,monster,scratch/'SceneObjects-DebugSwitches.savegame']),
        'state_empty': ('character_state_empty_audit',[world/'reference/character-state-methods/empty-fixtures.bin']),
        'enemy_spotted': ('character_enemy_spotted_audit',[
            world/'reference/character-enemy-spotted/enemy-spotted-fixtures.bin']),
        'pre_spawn': ('character_pre_spawn_audit',[
            world/'reference/character-pre-spawn/pre-spawn-fixtures.bin']),
        'target_pipeline': ('character_target_pipeline_audit',[
            inputs,ROOT/'.local-inputs/character-script-owner-discovery/ai-commons-source.luac',
            monster,world/'reference/character-enemy-spotted/enemy-spotted-fixtures.bin']),
        'hit': ('character_hit_audit',[
            ROOT/'.local-inputs/character-hit-discovery/host-fixtures.bin',
            ROOT/'.local-inputs/character-hit-discovery/genuine-missing-DebugSwitches.savegame']),
        'state_owner_extensions': ('character_state_owner_extensions_audit',[
            world/'reference/character-state-methods/empty-fixtures.bin']),
        'clear_aggro': ('character_clear_aggro_audit',[
            world/'reference/character-clear-aggro/clear-aggro-fixtures.bin',inputs,
            ROOT/'.local-inputs/character-script-owner-discovery/ai-commons-source.luac',
            monster,world/'reference/character-enemy-spotted/enemy-spotted-fixtures.bin']),
        'player_scene_objects': ('character_player_objects_audit',[
            inputs,common,monster,scratch/'PlayerObjects-DebugSwitches.savegame']),
        'object_position': ('character_object_position_audit',[
            inputs,common,monster,scratch/'ObjectPosition-DebugSwitches.savegame']),
        'owned_target_pipeline': ('character_owned_target_pipeline_audit',[
            inputs,ROOT/'.local-inputs/character-script-owner-discovery/ai-commons-source.luac',monster,
            ROOT/'.local-inputs/design-settings/design_pyarray.bin',
            ROOT/'.local-inputs/design-settings/design_pyarraynames.bin',
            ROOT/'.local-inputs/design-settings/design_pystructnames.bin']),
        'spawn_select': ('character_spawn_select_audit',[
            world/'reference/character-spawn-state/spawn-select-fixtures.bin',
            world/'reference/character-spawn-state/spawn-permission-fixtures.bin']),
        'spawn_body': ('character_spawn_body_audit',[
            world/'reference/character-spawn-body/spawn-body-fixtures.bin']),
        'kill': ('character_kill_audit',[
            ROOT/'.local-inputs/character-kill-discovery/host-fixtures.bin',assets/'data/v2quests_pycst.bin']),
        'design_settings': ('design_settings_audit',[
            data/'reference/design-settings/design-settings-fixtures.bin',
            ROOT/'.local-inputs/design-settings/design_pyarray.bin',
            ROOT/'.local-inputs/design-settings/design_pyarraynames.bin',
            ROOT/'.local-inputs/design-settings/design_pystructnames.bin']),
        'spawn_owner': ('character_spawn_owner_extensions_audit',[
            world/'reference/character-spawn-body/spawn-body-fixtures.bin',
            world/'reference/character-spawn-state/spawn-select-fixtures.bin',
            world/'reference/character-spawn-state/spawn-permission-fixtures.bin']),
        'AI_death': ('character_ai_death_audit',[
            ROOT/'.local-inputs/character-ai-death-discovery/host-fixtures.bin',
            ROOT/'.local-inputs/character-ai-death-discovery/genuine-missing-DebugSwitches.savegame']),
        'cancel_sneaking': ('character_cancel_sneaking_audit',[
            world/'reference/character-cancel-sneaking/cancel-sneaking-fixtures.bin',
            ROOT/'.local-inputs/character-cancel-sneaking/skills_pyarray.bin']),
        'dead_select': ('character_dead_select_audit',[
            world/'reference/character-dead-select/dead-select-fixtures.bin',assets/'data']),
        'skill_tables': ('skill_tables_audit',[
            data/'reference/skill-tables/skill-tables-fixtures.bin',
            ROOT/'.local-inputs/skill-tables/skills_pyarray.bin',
            ROOT/'.local-inputs/skill-tables/skills_pyarraynames.bin',
            ROOT/'.local-inputs/skill-tables/skills_pystructnames.bin']),
        'sneaking_tables': ('character_sneaking_tables_audit',[
            ROOT/'.local-inputs/skill-tables/skills_pyarray.bin',
            ROOT/'.local-inputs/skill-tables/skills_pyarraynames.bin',
            ROOT/'.local-inputs/skill-tables/skills_pystructnames.bin']),
        'actor_initialization': ('actor_initialization_audit',[
            world/'reference/actor-initialization/crypt01-actor-initialization.bin',assets/'worlds/crypt01.dact']),
        'timer_effects': ('character_timer_effects_audit',[
            ROOT/'.local-inputs/character-timer-effects-discovery/host-fixtures.bin',
            world/'reference/character-timer-effects/cached-reset-fixtures.bin',
            world/'reference/character-timer-effects/owner-query-fixtures.bin',
            assets/'data/character_properties_pyarray.bin',
            ROOT/'.local-inputs/character-timer-effects-discovery/files'])}
    for tag,model in (('skeleton','skeleton'),('slime','slime_green_v2'),('slime-red','slime_green_v2'),('ghost','ghost')):
        suites['monster_animation_'+tag] = ('character_animation_instance_audit',[
            monster_assets/('data/monster-'+tag+'-animation-bank.bin'),
            monster_assets/('actors/'+model+'.bdae'),monster_assets,assets/'data'])
    suites['asset_sha256'] = ('asset_sha256_audit',[
        payloads/'reference/sha256/hashlib-fixtures.bin',assets/'worlds/crypt01.dact'])
    suites['AI_state_changed'] = ('character_ai_state_changed_audit',[
        world/'reference/character-ai-state-changed/state-changed-fixtures.bin'])
    suites['AI_state_changed_VM'] = ('character_ai_state_changed_vm_audit',[
        ROOT/'.local-inputs/character-script-owner-discovery/ai-commons-source.luac'])
    suites['Idle_update'] = ('character_idle_update_audit',[
        world/'reference/character-idle-update/idle-fixtures.bin'])
    npc_output = ROOT/'.local-inputs/character-initialization-main'/args.output.stem
    npc_output.mkdir(parents=True,exist_ok=True)
    npc_fixture,npc_projection = npc_output/'npc-fixtures.bin',npc_output/'npc-projections.json'
    assert not npc_fixture.exists() and not npc_projection.exists(), 'Preserve NPC output proof'
    suites['NPC_body'] = ('character_npc_body_audit',[assets,npc_fixture,npc_projection])
    suites['Idle_events'] = ('character_idle_events_audit',[
        world/'reference/character-game-design/real-cache-inputs.bin',
        ROOT/'.local-inputs/character-script-owner-discovery/ai-commons-source.luac',
        ROOT/'.local-inputs/character-script-owner-extension/monster.luac',
        assets/'worlds/crypt01.dact',monster_assets,assets/'data'])
    fx_empty_files = npc_output/'fx-empty-files'
    fx_empty_files.mkdir()
    suites['FX_preload'] = ('visual_fx_preload_audit',[
        world/'reference/character-skeleton-fx/preload-fixtures.bin',fx_empty_files])
    fx_table_files = npc_output/'fx-tables-empty-files'
    fx_table_files.mkdir()
    suites['FX_tables'] = ('visual_fx_tables_audit',[effects_root,fx_table_files])
    suites['Init_FX'] = ('character_init_fx_audit',[world/'reference/character-init-fx/init-fx-fixtures.bin'])
    suites['Can_update'] = ('character_can_update_audit',[world/'reference/character-can-update/can-update-fixtures.bin'])
    suites['Material_color'] = ('engine-skinning/engine-animation/material_color_audit',[
        animation/'reference/fx-material-animation/material-color-fixtures.bin'])
    suites['Particle_parameter'] = ('engine-skinning/engine-animation/particle_parameter_audit',[
        animation/'reference/fx-particle-animation/particle-parameter-fixtures.bin'])
    suites['Update_startup'] = ('character_update_startup_audit',[
        world/'reference/character-update-startup/startup-prefix-fixtures.bin'])
    suites['Deferred_script'] = ('character_deferred_script_audit',[
        world/'reference/character-deferred-script/deferred-fixtures.bin'])
    suites['Deferred_session'] = ('character_deferred_script_session_audit',[
        world/'reference/character-game-design/real-cache-inputs.bin',
        ROOT/'.local-inputs/character-script-owner-discovery/ai-commons-source.luac',
        ROOT/'.local-inputs/character-script-owner-extension/monster.luac',assets/'worlds/crypt01.dact'])
    suites['Delayed_Idle'] = ('character_delayed_idle_audit',[
        world/'reference/character-game-design/real-cache-inputs.bin',
        ROOT/'.local-inputs/character-script-owner-discovery/ai-commons-source.luac',
        ROOT/'.local-inputs/character-script-owner-extension/monster.luac',
        assets/'worlds/crypt01.dact',monster_assets,assets/'data'])
    suites['Init_vitals'] = ('character_script_init_vitals_audit',[
        world/'reference/character-script-init-services/vitals-fixtures.bin'])
    suites['Init_vitals_session'] = ('character_script_init_vitals_session_audit',suites['Deferred_session'][1])
    suites['Delayed_Idle_vitals'] = ('character_delayed_idle_vitals_audit',suites['Delayed_Idle'][1])
    suites['Deferred_queue'] = ('character_deferred_queue_audit',[
        world/'reference/character-deferred-queue/deferred-queue-fixtures.bin'])
    suites['Particle_factory'] = ('engine-skinning/engine-animation/particle_factory_audit',[
        animation/'reference/fx-particle-factory/particle-factory-fixtures.bin',
        ROOT/'.local-inputs/character-skeleton-fx-assets/zombie_spawn_fx.bdae'])
    suites['Source_services'] = ('character_script_source_services_audit',[
        ROOT/'.local-inputs/character-script-owner-discovery/ai-commons-source.luac'])
    suites['Required_deferred'] = ('character_deferred_required_audit',suites['Deferred_session'][1])
    suites['Update_queued'] = ('character_update_queued_audit',[
        world/'reference/character-update-queued/queued-prefix-fixtures.bin'])
    suites['Update_queued_session'] = ('character_update_queued_session_audit',suites['Delayed_Idle'][1])
    suites['Particle_emission'] = ('engine-skinning/engine-animation/particle_emission_audit',[
        animation/'reference/fx-particle-emission/particle-emission-fixtures.bin',
        ROOT/'.local-inputs/character-skeleton-fx-assets/zombie_spawn_fx.bdae'])
    faery_inputs = [ROOT/'.local-inputs/character-skills'/('faeries_'+suffix+'.bin')
                    for suffix in ('pyarray','pyarraynames','pystructnames')]
    suites['Faery_tables'] = ('faery_tables_audit',[world/'reference/character-skills/faery-fixtures.bin',*faery_inputs])
    suites['Skills'] = ('character_skills_audit',[world/'reference/character-skills/skill-fixtures.bin'])
    suites['Skills_session'] = ('character_skills_session_audit',[
        inputs,common,monster,assets/'worlds/crypt01.dact',
        *[assets/'data'/('skills_'+suffix+'.bin') for suffix in ('pyarray','pyarraynames','pystructnames')],
        *faery_inputs,world/'reference/character-script-update/lua-inputs/skills-commons.luac'])
    suites['GFNT'] = ('engine-ui/gfnt_audit',[
        ui/'reference/gfnt/original-fixtures.bin',ROOT/'.local-inputs/font-text-discovery/sct_font_3.fnt'])
    suites['Shader_sources'] = ('engine-skinning/engine-animation/scene-materials/shader_sources_audit',[
        scene/'reference/shader-sources/shader-source-fixtures.bin',scene/'reference/shader-sources/shaders.pak'])
    suites['Swf_texture'] = ('engine-skinning/engine-animation/scene-materials/swf_texture_audit',[
        scene/'reference/swf-render-connection/swf-texture-fixtures.bin'])
    swf_inputs = ROOT/'.local-inputs/ui-layout-discovery'
    actual_font = ROOT/'port/android-native/app/src/main/assets/original-cache/data/Fontin SmallCaps.ttf'
    font_output = ROOT/'.local-inputs/main-ui-font-audit'
    font_output.mkdir(exist_ok=True)
    suites['Swf_movie'] = ('engine-ui/swf_movie_audit',[swf_inputs])
    suites['Freetype_font'] = ('engine-ui/freetype_font_audit',[
        swf_inputs,actual_font,font_output/'fontin.pgm'])
    suites['Freetype_HUD'] = ('engine-ui/freetype_hud_audit',[
        swf_inputs,actual_font,font_output/'hud-fontin.pgm'])
    font_inputs = ROOT/'.local-inputs/font-text-discovery'
    ui_assets = ROOT/'port/android-native/app/src/main/assets/original-cache/data'
    ui_debug = font_output/'debug-private';ui_debug.mkdir(exist_ok=True)
    assert not (ui_debug/'DebugSwitches.savegame').exists()
    suites['Swf_font_resolver'] = ('engine-ui/swf_font_resolver_audit',[
        ui/'reference/swf-font-resolver/source-fixtures.bin'])
    suites['Swf_font_HUD'] = ('engine-ui/swf_font_resolver_hud_audit',[swf_inputs,font_inputs])
    suites['UI_viewport'] = ('engine-ui/ui_viewport_audit',[ui/'reference/viewport/viewport-gold.bin'])
    suites['Localization'] = ('engine-ui/localization_audit',[ui/'reference/localization/localization-fixtures.bin'])
    suites['Localization_connection'] = ('engine-ui/localization_connection_audit',[
        ui/'reference/localization/localization-connection-fixtures.bin',ui_assets,assets/'data/fonts_pycst.bin',ui_debug])
    suites['HUD_freetype_font'] = ('engine-ui/hud_freetype_font_audit',[
        actual_font,ui_assets/'wqy-zenhei.ttf'])
    suites['HUD_freetype_provider'] = ('engine-ui/hud_freetype_provider_audit',[
        swf_inputs,font_inputs,ui_assets,assets/'data/fonts_pycst.bin'])
    suites['Swf_glyph_lookup'] = ('engine-ui/swf_glyph_lookup_audit',[
        ui/'reference/swf-glyph-lookup/source-fixtures.bin'])
    suites['HUD_player_values'] = ('engine-ui/hud_player_values_audit',[
        ui/'reference/hud-player-values/status-frames-gold.bin'])
    suites['Swf_viewport_connection']=('engine-ui/swf_viewport_connection_audit',[
        ui/'reference/swf-viewport-connection/connection-gold.bin'])
    suites['HUD_sprite_timeline']=('engine-ui/hud_sprite_timeline_audit',[
        ui/'reference/hud-sprite-timeline/scheduling-gold.bin'])
    suites['HUD_advance']=('engine-ui/hud_advance_audit',[
        ui/'reference/hud-sprite-timeline/advance-gold.bin'])
    suites['Gameswf_font_overlay']=('engine-ui/gameswf_font_overlay_audit',[
        swf_inputs,font_inputs,ui_assets,assets/'data/fonts_pycst.bin',
        ui/'reference/swf-glyph-lookup/source-fixtures.bin'])
    suites['Player_status_HUD']=('engine-ui/player_status_hud_audit',[
        swf_inputs,font_inputs,ui_assets,assets/'data/fonts_pycst.bin',assets/'data'])
    suites['Swf_AS_connection']=('engine-ui/swf_actionscript_connection_audit',[swf_inputs])
    suites['HUD_startup']=('engine-ui/hud_startup_audit',[
        ui/'reference/hud-startup-callbacks/source-fixtures.bin'])
    suites['Owned_HUD_settings']=('engine-ui/owned_settings_v1_audit',[
        ui/'reference/owned-hud-settings-v1/fixtures.bin',ROOT/'.local-inputs/design-settings',
        ui_assets/'pydata',ROOT/'.local-inputs/main-owned-hud-settings-v1-files'])
    suites['Settings_language_scene']=('engine-ui/settings_language_scene_v1_audit',[
        ui/'reference/owned-hud-settings-v1/language-scene-fixtures.bin'])
    suites['HUD_manager']=('engine-ui/hud_manager_audit',[ui/'reference/hud-manager/manager-gold.bin'])
    suites['HUD_manager_reentry']=('engine-ui/hud_manager_reentry_audit',[ui/'reference/hud-manager/reentry-gold.bin'])
    suites['HUD_manager_backends']=('engine-ui/hud_manager_backends_audit',[ui/'reference/hud-manager/backends-gold.bin'])
    suites['HUD_manager_core']=('engine-ui/hud_manager_core_audit',[swf_inputs])
    suites['HUD_text_format_v1']=('engine-ui/hud_text_format_v1_audit',[
        ui/'reference/hud-formatting-v1/format-gold.bin'])
    suites['Skill_info_v1']=('character_skill_info_v1_audit',[
        world/'reference/character-skill-info-v1/skill-info-gold.bin'])
    suites['HUD_skill_text_v1']=('engine-ui/hud_skill_text_v1_audit',[
        ui/'reference/hud-formatting-v1/skill-class-gold.bin',assets,
        ROOT/'.local-inputs/hud-formatting-v1/scripts'])
    suites['Script_first_return_v1']=('script-runtime/script_first_return_v1_audit',[])
    suites['Text_layout_v1']=('engine-ui/text_layout_v1_audit',[ui/'reference/text-layout-v1/whole-gold-v2.bin'])
    suites['Text_display_v2']=('engine-ui/text_display_v2_audit',[ui/'reference/text-display-v2/whole-gold-v3.bin'])
    suites['Text_render_owner_v2']=('engine-ui/text_render_owner_v2_audit',[
        font_inputs/'Fontin SmallCaps.ttf',font_inputs/'wqy-zenhei.ttf'])
    suites['Loot_tables_v2']=('game-data/loot_tables_v2_audit',[
        data/'reference/player-creation-v2/fixtures.bin',ROOT/'.local-inputs/items-discovery'])
    suites['Fresh_inventory_v2']=('game-data/fresh_inventory_v2_audit',[
        data/'reference/player-creation-v2/fresh-fixtures.bin',ROOT/'.local-inputs/items-discovery'])
    suites['Player_initial_grants_v2']=('player_initial_grants_v2_audit',[
        world/'reference/player-initial-grants-v2/fixtures.bin',
        world/'reference/player-initial-grants-v2/skill-fixtures.bin',ROOT/'.local-inputs/skill-tables'])
    suites['HUD_initialization_v1']=('engine-ui/hud_initialization_v1_audit',[
        ui/'reference/hud-initialization-v1/wrappers-gold.bin',
        ui/'reference/hud-initialization-v1/options-gold.bin'])
    suites['HUD_initialization_droid_v1']=('engine-ui/hud_initialization_core_v1_audit',[
        swf_inputs,ROOT/'port/android-native/app/src/main/assets'])
    suites['HUD_initialization_base_v1']=('engine-ui/hud_initialization_core_v1_audit',[
        swf_inputs,ROOT/'port/android-native/app/src/main/assets','dqhud.swf'])
    suites['Source_input_session_v1']=('engine-ui/swf_input_session_v1_audit',[])
    suites['Source_input_session_v2']=('engine-ui/swf_source_session_v2_audit',[])
    suites['Source_cursor_input_v1']=('engine-ui/swf_cursor_input_v1_audit',[])
    suites['Source_frame_connection_v1']=('engine-ui/swf_frame_connection_v1_audit',[])
    suites['Source_frame_gold_v1']=('engine-ui/swf_frame_gold_v1_audit',[
        ui/'reference/swf-native-input-frame-v1/frame-drag-gold.bin'])
    suites['Source_cursor_gold_v1']=('engine-ui/swf_cursor_gold_v1_audit',[
        ui/'reference/swf-native-input-frame-v1/input-gold.bin'])
    suites['Fresh_inventory_owned_v4']=('game-data/fresh_inventory_owned_v4_audit',[
        data/'reference/player-inventory-owned-v4/fixtures.bin',
        data/'reference/player-creation-v2/fresh-fixtures.bin',ROOT/'.local-inputs/items-discovery'])
    suites['Skill_session_v2']=('character_skill_session_v2_audit',[
        world/'reference/character-skill-session-v2/source-gold.bin'])
    suites['Player_skills_v2']=('character_player_skills_v2_audit',[
        world/'reference/character-game-design/real-cache-inputs.bin',assets,
        ROOT/'.local-inputs/character-skill-session-v2/cache',
        ui/'reference/hud-formatting-v1/skill-class-gold.bin'])
    suites['Current_spell_v1']=('character_current_spell_v1_audit',[
        world/'reference/character-current-spell-v1/source-gold.bin'])
    suites['Script_assets_v1']=('character_script_assets_v1_audit',[
        linux(cache_archive),ROOT/'.local-inputs/character-script-assets-v1/expected-script-sha.tsv'])
    suites['Player_skills_v3']=('character_player_skills_v3_audit',[
        world/'reference/character-game-design/real-cache-inputs.bin',assets,
        ROOT/'.local-inputs/character-skill-session-v2/cache',
        ui/'reference/hud-formatting-v1/skill-class-gold.bin',linux(cache_archive)])
    suites['Skill_AI_v3']=('character_skill_ai_v3_audit',[
        world/'reference/character-skill-gameplay-v3/ai-gold-v3.bin'])
    suites['Skill_callbacks_v3']=('character_skill_callbacks_v3_audit',[
        world/'reference/character-skill-gameplay-v3/callback-gold.bin'])
    suites['Indexed_runtime_v3']=('script-runtime/script_indexed_return_v3_audit',[])
    item_gold_v5=data/'reference/player-item-effects-v5'
    item_power_cache_v5=ROOT/'.local-inputs/player-item-effects-v5/power-cache'
    item_loot_cache_v5=ROOT/'.local-inputs/items-discovery'
    item_char_cache_v5=ROOT/'.local-inputs/actors'
    item_class_cache_v5=ROOT/'.local-inputs/combat-data'
    item_private_v5=ROOT/'.local-inputs/player-item-effects-v5/private-save'
    assert item_private_v5.is_dir()
    suites['Item_gear_properties_v5']=('game-data/item_gear_properties_v5_audit',[
        item_gold_v5/'gear-fixtures.bin',item_gold_v5/'power-fixtures.bin',item_power_cache_v5])
    suites['Item_presentation_v5']=('game-data/item_presentation_v5_audit',[
        item_gold_v5/'presentation-fixtures.bin',item_gold_v5/'power-instance-fixtures.bin',item_power_cache_v5])
    item_starter_args_v5=[item_gold_v5/'starter-effects-fixtures.bin',item_loot_cache_v5,
        item_power_cache_v5,item_char_cache_v5,item_class_cache_v5]
    suites['Player_gear_effects_v5']=('game-data/player_gear_effects_v5_audit',item_starter_args_v5)
    suites['Player_gear_cache_v5']=('game-data/player_gear_cache_v5_audit',[
        *item_starter_args_v5,assets,item_private_v5])
    suites['Player_skin_v5']=('game-data/player_skin_v5_audit',[
        item_gold_v5/'skin-fixtures.bin',item_loot_cache_v5])
    suites['Item_text_varargs_v5']=('engine-ui/item_text_varargs_v5_audit',[
        ui/'reference/item-text-varargs-v5/fixtures.bin'])
    suites['Loot_power_creation_v7']=('game-data/loot_power_creation_v7_audit',[
        data/'reference/loot-power-creation-v7/fixtures.bin',
        ROOT/'.local-inputs/player-loot-v7/cache',item_char_cache_v5,assets,item_private_v5])
    suites['Player_savegame_v1']=('game-data/player_savegame_v1_audit',[
        data/'reference/player-savegame-v1/fixtures.bin',ROOT/'.local-inputs/skill-tables'])
    suites['Player_faeries_v1']=('game-data/player_faeries_v1_audit',[
        data/'reference/player-savegame-v1/faery-fixtures.bin'])
    suites['Player_metadata_v1']=('game-data/player_metadata_v1_audit',[
        data/'reference/player-savegame-v1/metadata-fixtures.bin',ROOT/'.local-inputs/combat-data/character_classes_pyarraynames.bin'])
    suites['Item_inventory_v1']=('game-data/item_inventory_v1_audit',[
        data/'reference/item-inventory-v1/fixtures.bin',ROOT/'.local-inputs/items-discovery'])
    suites['Inventory_potions_v1']=('game-data/inventory_potions_v1_audit',[
        data/'reference/item-inventory-v1/potion-fixtures.bin',ROOT/'.local-inputs/items-discovery'])
    suites['Player_profile_index_v1']=('game-data/player_profile_index_v1_audit',[
        data/'reference/player-profile-index-v1/fixtures.bin'])
    files = set(path for name,(_,paths) in suites.items()
                for path in (paths[:1] if name=='Lua_GC' else paths) if isinstance(path,Path) and path.is_file())
    files.update(baseline_root/key for key in migration_files)
    files.add(baseline_path)
    files.update(facade_baseline/name for name in (*facade_hashes,'sha256.json'))
    files.update(swf_inputs/name for name in ('dqshared_droid.swf','dqhud_droid.swf','dqshared.swf','dqhud.swf'))
    files.update(path for path in (ROOT/'.local-inputs/design-settings').rglob('*') if path.is_file())
    files.add(actual_font)
    files.update(path for path in (ROOT/'.local-inputs/hud-formatting-v1/scripts').iterdir() if path.is_file())
    files.update(font_inputs/name for name in ('Fontin SmallCaps.ttf','wqy-zenhei.ttf'))
    files.update(path for path in ui_assets.rglob('*') if path.is_file())
    files.update(path for path in (assets/'data').rglob('*') if path.is_file())
    files.update(path for path in monster_assets.rglob('*') if path.is_file())
    files.add(world/'reference/actor-initialization/crypt01-actor-initialization.json')
    files.add(world/'reference/character-hit/hit-fixtures.json')
    files.update(path for path in effects_root.iterdir() if path.suffix == '.bin')
    files.update(path for path in (ROOT/'.local-inputs/character-skill-session-v2/cache').rglob('*') if path.is_file())
    for directory in (item_power_cache_v5,item_loot_cache_v5,item_char_cache_v5,item_class_cache_v5):
        files.update(directory.glob('*.bin'))
    item_host_receipt_v5=json.loads((data/'reports/player-item-effects-v5-host-audit-v3.json').read_text())
    files.update(ROOT/name for name in item_host_receipt_v5['input_sha256'])
    files.update((ROOT/'.local-inputs/player-loot-v7/cache').glob('*.bin'))
    input_hashes = {path.relative_to(ROOT).as_posix():sha(path) for path in files}
    commands = []

    def run(*command,expected_stderr=''):
        process = subprocess.run(['wsl.exe','--cd',linux(ROOT),*command],
                                 capture_output=True,text=True,timeout=120)
        commands.append(dict(arguments=list(command),returncode=process.returncode,
                             stdout=process.stdout,stderr=process.stderr))
        assert process.returncode == 0 and process.stderr.strip() == expected_stderr,commands[-1]
        return process.stdout.strip()

    world_so = args.build+'/libdh2_level_world.so'
    runtime_so = args.build+'/script-runtime/libdh2_script_runtime.so'
    data_so = args.build+'/game-data/libdh2_game_data.so'
    scene_so = args.build+'/engine-skinning/engine-animation/scene-materials/libdh2_scene_materials.so'
    animation_so = args.build+'/engine-skinning/engine-animation/libdh2_engine_animation.so'
    ui_so = args.build+'/engine-ui/libdh2_engine_ui.so'
    binaries = [world_so,runtime_so,data_so,scene_so,animation_so,ui_so,
                args.build+'/engine-ui/libdh2_gameswf_core.a',
                args.build+'/engine-ui/libdh2_freetype237.a']+[args.build+'/'+target for target,_ in suites.values()]

    def binary_hashes():
        return {row.split(maxsplit=1)[1]:row.split()[0] for row in run('sha256sum',*binaries).splitlines()}

    # Every executable is rebuilt against current headers before any binary
    # hash is captured. An additive session input field otherwise leaves a
    # valid old executable with an incompatible C++ layout beside the new DSO.
    run('cmake','--build',args.build,'--target',
        *(target.rsplit('/',1)[-1] for target,_ in suites.values()),'--parallel','6')
    before = binary_hashes()
    compiler = json.loads(run('cat',args.build+'/compile_commands.json'))
    suffixes = ['/level-world/'+name+'.cpp' for name in modules]
    suffixes += ['/script-runtime/'+name for name in ('script_runtime_return_v3.c','script_object_bridge.c')]
    suffixes += ['/level-world/tests/'+name+'.cpp' for name in modules if name!='character_script_owner']
    suffixes += ['/level-world/tests/character_script_owner_integers.cpp']
    suffixes += ['/script-runtime/tests/'+name+'.cpp' for name in ('script_callback_scope','script_include','script_vm_ownership')]
    suffixes += ['/script-runtime/lua/lgc.c','/script-runtime/lua/ltable.c','/script-runtime/tests/lua514.cpp',
                 '/level-world/character_timer_effects.cpp','/level-world/tests/character_timer_effects.cpp',
                 '/level-world/tests/character_script_session_targets.cpp']
    suffixes += ['/level-world/'+name+'.cpp' for name in ('character_state','object_identity')]
    suffixes += ['/level-world/tests/'+name+'.cpp' for name in ('character_state','object_identity','object_target_events')]
    suffixes += ['/level-world/'+name+'.cpp' for name in ('character_state_owner','character_buffs')]
    suffixes += ['/level-world/tests/'+name+'.cpp' for name in ('character_state_owner','character_buffs')]
    suffixes += ['/level-world/'+name+'.cpp' for name in ('character_state_owner_behavior','character_state_owner_frame','character_script_commands','character_target_update','character_target_events','character_controller_commands','character_path_commands')]
    suffixes += ['/level-world/tests/'+name+'.cpp' for name in ('character_state_owner_behavior','character_state_owner_frame','character_script_commands','character_target_update','character_target_events')]
    suffixes += ['/level-world/character_ai_events.cpp','/level-world/tests/character_ai_events.cpp']
    suffixes += ['/level-world/'+name+'.cpp' for name in ('character_dot_attack','character_script_objects','character_state_empty')]
    suffixes += ['/level-world/tests/'+name+'.cpp' for name in ('character_dot_attack','character_script_objects','character_state_empty','character_target_event_route')]
    suffixes += ['/level-world/'+name+'.cpp' for name in ('character_enemy_spotted','character_pre_spawn')]
    suffixes += ['/level-world/tests/'+name+'.cpp' for name in ('character_enemy_spotted','character_pre_spawn','character_target_pipeline')]
    suffixes += ['/level-world/'+name+'.cpp' for name in ('character_hit','character_target_providers')]
    suffixes += ['/level-world/tests/character_hit.cpp','/game-data/properties.cpp']
    suffixes += ['/level-world/character_state_owner_extensions.cpp','/level-world/tests/character_state_owner_extensions.cpp']
    suffixes += ['/level-world/character_clear_aggro.cpp','/level-world/tests/character_clear_aggro.cpp','/game-data/aggro.cpp']
    suffixes += ['/level-world/tests/character_player_objects.cpp']
    suffixes += ['/level-world/tests/character_object_position.cpp','/level-world/tests/character_owned_target_pipeline.cpp']
    suffixes += ['/level-world/'+name+'.cpp' for name in ('character_spawn_select','character_spawn_permission','character_spawn_body','character_kill')]
    suffixes += ['/level-world/tests/'+name+'.cpp' for name in ('character_spawn_select','character_spawn_body','character_kill')]
    suffixes += ['/game-data/design_settings.cpp','/game-data/tests/design_settings.cpp']
    suffixes += ['/level-world/'+name+'.cpp' for name in ('character_spawn_owner_extensions','character_ai_death','character_cancel_sneaking')]
    suffixes += ['/level-world/tests/'+name+'.cpp' for name in ('character_spawn_owner_extensions','character_ai_death','character_cancel_sneaking')]
    suffixes += ['/level-world/character_dead_select.cpp','/level-world/tests/character_dead_select.cpp',
                 '/game-data/skill_tables.cpp','/game-data/tests/skill_tables.cpp']
    suffixes += ['/level-world/'+name+'.cpp' for name in ('character_sneaking_tables','actor_initialization','character_animation_instance')]
    suffixes += ['/level-world/tests/'+name+'.cpp' for name in ('character_sneaking_tables','actor_initialization','character_animation_instance')]
    suffixes += ['/level-world/'+name+'.cpp' for name in ('character_ai_state_changed','character_ai_state_changed_vm','character_idle_update','character_npc_body','character_idle_events')]
    suffixes += ['/level-world/tests/'+name+'.cpp' for name in ('character_ai_state_changed','character_ai_state_changed_vm','character_idle_update','character_npc_body','character_idle_events')]
    suffixes += ['/asset-payloads/sha256.cpp','/asset-payloads/tests/sha256.cpp']
    suffixes += ['/level-world/visual_fx_preload.cpp','/level-world/tests/visual_fx_preload.cpp']
    suffixes += ['/level-world/visual_fx_tables.cpp','/level-world/tests/visual_fx_tables.cpp','/game-data/effects_tables.cpp']
    suffixes += ['/level-world/'+name+'.cpp' for name in ('character_init_fx','character_can_update')]
    suffixes += ['/level-world/tests/'+name+'.cpp' for name in ('character_init_fx','character_can_update')]
    suffixes += ['/engine-animation/material_color.cpp','/engine-animation/tests/material_color.cpp']
    suffixes += ['/engine-animation/particle_parameter.cpp','/engine-animation/tests/particle_parameter.cpp']
    suffixes += ['/level-world/'+name+'.cpp' for name in ('character_deferred_script','character_deferred_script_session','character_update_startup','character_script_lifecycle')]
    suffixes += ['/level-world/tests/'+name+'.cpp' for name in ('character_deferred_script','character_deferred_script_session','character_update_startup','character_delayed_idle')]
    suffixes += ['/level-world/'+name+'.cpp' for name in ('character_script_init_vitals','character_script_init_vitals_session')]
    suffixes += ['/level-world/tests/'+name+'.cpp' for name in ('character_script_init_vitals_linked','character_script_init_vitals_session','character_delayed_idle_vitals')]
    suffixes += ['/level-world/character_deferred_queue.cpp','/level-world/tests/character_deferred_queue.cpp',
                 '/engine-animation/particle_factory.cpp','/engine-animation/tests/particle_factory.cpp']
    suffixes += ['/level-world/tests/character_script_source_services.cpp','/level-world/tests/character_deferred_required.cpp']
    suffixes += ['/level-world/character_update_queued.cpp','/level-world/tests/character_update_queued.cpp',
                 '/level-world/tests/character_update_queued_session.cpp',
                 '/engine-animation/particle_emission.cpp','/engine-animation/tests/particle_emission.cpp']
    suffixes += ['/game-data/faery_tables.cpp','/game-data/tests/faery_tables.cpp',
                 '/level-world/character_skills.cpp','/level-world/character_skills_owner.cpp',
                 '/level-world/character_skills_session.cpp','/level-world/tests/character_skills.cpp',
                 '/level-world/tests/character_skills_session.cpp']
    suffixes += ['/engine-ui/gfnt.cpp','/engine-ui/tests/gfnt.cpp',
                 '/scene-materials/shader_sources.cpp','/scene-materials/tests/shader_sources.cpp',
                 '/scene-materials/swf_texture.cpp','/scene-materials/tests/swf_texture.cpp']
    suffixes += ['/engine-ui/'+name+'.cpp' for name in ('swf_movie','freetype_font',
                 'freetype_glyph_kernel','swf_freetype_provider','swf_font_geometry')]
    suffixes += ['/engine-ui/tests/swf_movie.cpp','/engine-ui/tests/freetype_font.cpp']
    suffixes += ['/engine-ui/'+name+'.cpp' for name in ('swf_font_resolver','viewport','localization')]
    suffixes += ['/engine-ui/tests/'+name+'.cpp' for name in
                 ('swf_font_resolver','swf_font_resolver_hud','viewport','localization','localization_connection')]
    suffixes += ['/engine-ui/'+name+'.cpp' for name in
                 ('freetype_bitmap_alpha','hud_freetype_font','swf_hud_freetype_provider',
                  'swf_glyph_lookup','hud_player_values')]
    suffixes += ['/engine-ui/tests/'+name+'.cpp' for name in
                 ('hud_freetype_font','hud_freetype_provider','swf_glyph_lookup','hud_player_values')]
    suffixes += ['/engine-ui/vendor/freetype-2.3.7-hud/src/base/ftbitmap.c']
    suffixes += ['/engine-ui/vendor/gameswf1714/'+name for name in
                 re.findall(r'\$\{DH2_GAMESWF_ROOT\}/([^\"]+\.cpp)',(ui/'gameswf_sources.cmake').read_text())]
    suffixes.remove('/engine-ui/vendor/gameswf1714/gameswf/gameswf_font.cpp')
    suffixes += ['/engine-ui/overlays/font-v1/gameswf_font.cpp']
    suffixes += ['/engine-ui/'+name+'.cpp' for name in connected_modules]
    suffixes += ['/engine-ui/tests/'+name+'.cpp' for name in connected_tests]
    suffixes += ['/game-data/'+name+'.cpp' for name in owned_player_modules]
    suffixes += ['/level-world/player_initial_grants_v2.cpp','/level-world/tests/player_initial_grants_v2.cpp']
    suffixes += ['/level-world/'+name+'.cpp' for name in formatting_world_modules]
    suffixes += ['/level-world/tests/character_skill_info_v1.cpp','/script-runtime/tests/script_first_return_v1.cpp']
    suffixes += ['/game-data/tests/'+name+'.cpp' for name in owned_player_tests]
    suffixes += ['/engine-ui/vendor/freetype-2.3.7-hud/'+name for name in
                 re.findall(r'\$\{DH2_FT237\}/([^\"]+\.c)',(ui/'freetype237-hud.cmake').read_text())]
    # Selection recipes replace TUs; included frozen bodies stay separately
    # hash-bound, but are never counted as additional compiler invocations.
    for old in ('swf_movie','swf_source_movie_v1','swf_input_session_v1','swf_input_session_v2'):
        suffix='/engine-ui/'+old+'.cpp'
        if suffix in suffixes:suffixes.remove(suffix)
    suffixes += ['/engine-ui/'+name+'.cpp' for name in source_input_modules
                 if name not in ('swf_movie','swf_source_movie_v1','swf_input_session_v1','swf_input_session_v2')]
    suffixes += ['/engine-ui/tests/'+name+'.cpp' for name in source_input_tests]
    suffixes.remove('/engine-ui/tests/swf_viewport_connection.cpp')
    suffixes += ['/engine-ui/tests/overlays/source-facade-v1/swf_viewport_connection.cpp',
        '/android-native/app/src/main/cpp/original_ui_input_session_v1.cpp',
        '/game-data/fresh_inventory_owned_v4.cpp','/game-data/tests/fresh_inventory_owned_v4.cpp']
    for path in source_overlay_files:
        suffixes.append('/'+path.relative_to(ROOT/'port').as_posix())
        if path.parent.name!='source-facade-v1':
            suffixes.remove('/engine-ui/vendor/gameswf1714/gameswf/'+path.name)
    suffixes += ['/level-world/'+name+'.cpp' for name in player_skill_modules_v2]
    suffixes += ['/level-world/tests/'+name+'.cpp' for name in ('character_skill_session_v2','character_player_skills_v2')]
    suffixes += ['/level-world/'+name+'.cpp' for name in script_resource_modules_v1]
    suffixes += ['/level-world/tests/'+name+'.cpp' for name in script_resource_modules_v1]
    suffixes += ['/asset-payloads/zip_asset_pack_v1.cpp']
    suffixes += ['/game-data/'+name+'.cpp' for name in item_data_modules_v5]
    suffixes += ['/engine-ui/'+name+'.cpp' for name in item_ui_modules_v5]
    suffixes += ['/game-data/tests/'+name+'.cpp' for name in item_data_tests_v5]
    suffixes += ['/engine-ui/tests/'+name+'.cpp' for name in item_ui_tests_v5]
    suffixes += ['/level-world/'+name+'.cpp' for name in player_skill_modules_v3]
    suffixes += ['/'+p['source'].removeprefix('port/') for p in skill_integration_v3['tests']]
    suffixes += ['/game-data/'+name+'.cpp' for name in loot_modules_v7]
    suffixes += ['/game-data/tests/'+name+'.cpp' for name in loot_tests_v7]
    records = [record for record in compiler if any(record['file'].endswith(suffix) for suffix in suffixes)]
    # The same genuine font audit is compiled once for raster-only and once for HUD.
    assert len(records) == len(suffixes)+1
    assert all('-fsanitize=address,undefined' in record['command'] for record in records)
    audits, dependencies = {},{}
    for name,(target,paths) in suites.items():
        executable = args.build+'/'+target
        dependencies[name] = run('ldd',executable)
        ui_suites = ('GFNT','Swf_movie','Freetype_font','Freetype_HUD','Swf_font_resolver','Swf_font_HUD','UI_viewport','Localization','Localization_connection',
                     'HUD_freetype_font','HUD_freetype_provider','Swf_glyph_lookup','HUD_player_values',
                     'Swf_viewport_connection','HUD_sprite_timeline','HUD_advance','Gameswf_font_overlay','Player_status_HUD',
                     'Swf_AS_connection','HUD_startup','Owned_HUD_settings','Settings_language_scene',
                     'HUD_manager','HUD_manager_reentry','HUD_manager_core','Text_layout_v1',
                     'HUD_initialization_v1','HUD_initialization_droid_v1','HUD_initialization_base_v1',
                     'Text_display_v2','Text_render_owner_v2','HUD_text_format_v1','HUD_skill_text_v1','Source_input_session_v1','Source_input_session_v2',
                     'Source_cursor_input_v1','Source_frame_connection_v1','Source_frame_gold_v1','Source_cursor_gold_v1',
                     'Item_text_varargs_v5')
        data_suites=('Faery_tables','Player_savegame_v1','Player_faeries_v1','Player_metadata_v1',
                     'Item_inventory_v1','Inventory_potions_v1','Player_profile_index_v1',
                     'Loot_tables_v2','Fresh_inventory_v2','Fresh_inventory_owned_v4',
                     'Item_gear_properties_v5','Item_presentation_v5','Player_gear_effects_v5',
                     'Player_gear_cache_v5','Player_skin_v5','Loot_power_creation_v7')
        required_dso = ui_so if name in ui_suites else world_so if name in ('HUD_manager_backends','Player_initial_grants_v2','Skill_info_v1','Skill_session_v2','Player_skills_v2','Current_spell_v1','Script_assets_v1','Player_skills_v3','Skill_AI_v3','Skill_callbacks_v3') else data_so if name in data_suites else scene_so if name in ('asset_sha256','Material_color','Particle_parameter','Particle_factory','Particle_emission','Shader_sources','Swf_texture') else runtime_so
        assert all(value in dependencies[name] for value in (required_dso,'libasan.so','libubsan.so'))
        assert 'not found' not in dependencies[name]
        if name in ('Player_gear_cache_v5','Loot_power_creation_v7','Player_skills_v3'):
            assert all(value in dependencies[name] for value in (data_so,ui_so,world_so,runtime_so))
        audits[name] = json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1',
            'UBSAN_OPTIONS=halt_on_error=1',executable,*(linux(path) if isinstance(path,Path) else path for path in paths),
            expected_stderr="error: can't create a movie from 'missing'" if name=='Source_input_session_v1' else
                'Unresolved genuine font: Arial' if name=='Freetype_HUD' else
                'wqy P: size=12,mode=1,width=5,rows=8,pitch=1\nsource localization: World Map' if name in ('HUD_freetype_provider','Gameswf_font_overlay') else
                'wqy P: size=12,mode=1,width=5,rows=8,pitch=1\nsource localization: World Map\nsource localization: World Map' if name=='Player_status_HUD' else ''))
        if name in ('HUD_sprite_timeline','HUD_advance'):
            assert audits[name]['mismatches']==0
            audits[name]['validation']='PASS'
            audits[name]['validation_basis']='Exit0 with exact original fixture comparison and empty sanitizer output'
        if name == 'UI_viewport':
            # The frozen viewport executable emits counters rather than a status
            # field. Verify its complete schema after exit0/empty sanitizer output.
            assert audits[name] == dict(comparisons=5200,ordered_services=12683,
                                       failure_reentry_guards=12,mismatches=0)
            audits[name]['validation']='PASS'
            audits[name]['validation_basis']='Exit0, empty sanitizer output and exact complete gold/guard counters'
        if name == 'HUD_player_values':
            assert audits[name] == dict(comparisons=1562,ordered_services=13721,
                                       failure_reentry_guards=15,mismatches=0)
            audits[name]['validation']='PASS'
            audits[name]['validation_basis']='Exit0, empty sanitizer output and exact complete status-region gold/guard counters'
        assert audits[name]['validation'] == 'PASS'
        if not name.startswith('monster_animation_') and name not in ('scope','Include','VM_ownership','Lua_GC','timer_effects','state_bodies','object_identity','object_target_events','state_owner','buffs','state_owner_behavior','state_owner_frame','script_commands','target_update','target_event_prefixes','ai_event_route','target_event_route','dot_attack','scene_script_objects','state_empty','enemy_spotted','pre_spawn','hit','state_owner_extensions','clear_aggro','player_scene_objects','object_position','owned_target_pipeline','spawn_select','spawn_body','design_settings','spawn_owner','AI_death','cancel_sneaking','dead_select','skill_tables','sneaking_tables','actor_initialization','asset_sha256','AI_state_changed','AI_state_changed_VM','Idle_update'):
            if name not in ('NPC_body','Idle_events','FX_preload','FX_tables','Init_FX','Can_update','Material_color','Particle_parameter','Update_startup','Deferred_script','Deferred_session','Delayed_Idle','Init_vitals','Init_vitals_session','Delayed_Idle_vitals','Deferred_queue','Particle_factory','Required_deferred','Update_queued','Update_queued_session','Particle_emission','Faery_tables','Skills','Skills_session','GFNT','Shader_sources','Swf_texture','Swf_movie','Freetype_font','Freetype_HUD'):
                if name not in ui_suites and name not in data_suites and name not in ('HUD_manager_backends','Player_initial_grants_v2','Skill_info_v1','Script_first_return_v1','Skill_session_v2','Player_skills_v2','Current_spell_v1','Script_assets_v1','Player_skills_v3','Skill_AI_v3','Skill_callbacks_v3','Indexed_runtime_v3'):
                    assert audits[name]['runtime_library'] == runtime_so
    assert audits['Lua_GC']['checks'] == 454
    assert audits['Item_gear_properties_v5']['gear_original_cases']==4037
    assert audits['Item_gear_properties_v5']['actual_power_rows']==937
    assert audits['Item_gear_properties_v5']['actual_power_properties']==1224
    assert audits['Item_presentation_v5']['presentation_original_cases']==4031
    assert audits['Item_presentation_v5']['power_instance_original_cases']==1119
    assert audits['Player_skin_v5']['original_skin_cases']==24
    assert audits['Item_text_varargs_v5']['original_cases']==355
    for name in ('Player_gear_effects_v5','Player_gear_cache_v5'):
        assert audits[name]['actual_starter_classes']==3
        assert audits[name]['original_owner_cases']==6 and audits[name]['original_owner_steps']==112
        assert audits[name]['native_property_effects']==64 and audits[name]['native_vitals_effects']==64
    assert audits['Player_gear_cache_v5']['genuine_localized_item_text_deliveries']==128
    assert audits['Player_gear_cache_v5']['actual_power_localized_descriptions']==936
    assert audits['Player_gear_cache_v5']['source_null_power_descriptions_unsupported']==1
    assert audits['Player_gear_cache_v5']['real_localization_file_opens']==4
    # The two object-method finalizer checks now execute under the close scope;
    # before the runtime fix, Lua's close swallowed their rejected callbacks.
    assert audits['target_sessions']['checks'] == 546 and audits['target_sessions']['world_library'] == world_so
    assert audits['target_sessions']['guards'] == 29 and audits['target_sessions']['command_supported_registrations'] == 40
    assert audits['target_sessions']['actual_controller_character_calls'] == 4
    assert audits['state_bodies']['isolated_Focus_Blur_original_cases'] == 937
    assert audits['state_bodies']['isolated_OnEvent_original_cases'] == 690
    assert audits['object_identity']['comparisons'] == 1166
    assert audits['object_target_events']['comparisons'] == 465
    assert audits['state_owner']['gold_cases'] == 1152 and audits['state_owner']['predicate_gold_cases'] == 2016
    assert audits['state_owner']['owned_registry_states'] == 20 and audits['state_owner']['independent_owner_and_guard_checks'] == 31
    assert audits['buffs']['checks'] == 268980 and audits['buffs']['original_gold_cases'] == 528
    assert audits['buffs']['module_library'] == world_so and audits['buffs']['world_library'] == world_so
    assert audits['buffs']['real_timer_expiries'] == 6 and audits['buffs']['data_library'] == data_so
    assert audits['state_owner_behavior']['original_transition_cases'] == 937 and audits['state_owner_behavior']['original_event_cases'] == 975
    assert audits['state_owner_frame']['original_bounded_frame_cases'] == 906
    assert audits['state_owner_frame']['original_outer_frame_cases'] == 1536
    assert audits['state_owner_frame']['other_update_metadata_deliveries'] == 1104
    assert audits['state_owner_frame']['guard_and_prefix_checks'] == 16
    assert audits['script_commands']['original_gold_cases'] == 4196 and audits['script_commands']['checks'] == 170449
    assert audits['script_commands']['module_library'] == world_so
    assert audits['target_update']['comparisons'] == 2225 and audits['target_update']['raise_events'] == 1625
    assert audits['target_event_prefixes']['comparisons'] == 2013 and audits['target_event_prefixes']['genuine_ai_rows'] == 76
    assert audits['ai_event_route']['comparisons'] == 11344
    assert audits['ai_event_route']['dispatcher_library'] == world_so
    assert audits['ai_event_route']['relay_library'] == world_so
    assert audits['target_event_route']['comparisons'] == 1944 and audits['target_event_route']['ordered_services'] == 2096
    assert audits['dot_attack']['original_gold_cases'] == 1800 and audits['dot_attack']['checks'] == 361946
    assert audits['dot_attack']['module_library'] == world_so and audits['dot_attack']['data_library'] == data_so
    assert audits['scene_script_objects']['checks'] == 448 and audits['scene_script_objects']['finalizer_target_clear']
    assert audits['scene_script_objects']['world_library'] == world_so
    assert audits['state_empty']['empty_methods'] == 18 and audits['state_empty']['required_nonempty_methods'] == 46
    assert audits['enemy_spotted']['comparisons'] == 409 and audits['enemy_spotted']['ordered_services'] == 4344
    assert audits['enemy_spotted']['provider_failure_checks'] == 16 and audits['enemy_spotted']['authored_threat_bits'] == 0x41200000
    assert audits['pre_spawn']['original_cases'] == 538 and audits['pre_spawn']['original_ordered_requests'] == 2698
    assert audits['pre_spawn']['guard_prefix_source_invalid_checks'] == 22 and audits['pre_spawn']['nested_interactive_event_checks'] == 1
    assert audits['target_pipeline']['checks'] == 1298 and audits['target_pipeline']['gate_cases'] == 72
    assert audits['target_pipeline']['world_library'] == audits['target_pipeline']['prefix_library'] == world_so
    assert audits['target_pipeline']['nested_source_events'] == 1 and audits['target_pipeline']['finalizer_clear']
    assert audits['hit']['original_gold_cases'] == 2600 and audits['hit']['checks'] == 361256
    assert audits['hit']['module_library'] == audits['hit']['world_library'] == world_so and audits['hit']['data_library'] == data_so
    assert audits['hit']['genuine_nonlethal_HitFor'] == 2 and audits['hit']['genuine_synchronous_reentry'] == 2
    assert audits['hit']['genuine_DoT_HitFor_bridge_calls'] == 1 and not audits['hit']['full_Kill_backend']
    assert audits['state_owner_extensions']['genuine_empty_method_deliveries'] == 18
    assert audits['state_owner_extensions']['required_nonempty_failure_prefixes'] == 43
    assert audits['state_owner_extensions']['module_library'] == world_so
    assert audits['state_owner_extensions']['actual_owner_PreSpawn_Idle_transition']
    assert audits['state_owner_extensions']['single_elapsed_frame'] and audits['state_owner_extensions']['nested_PreSpawn_event'] == 1
    assert audits['clear_aggro']['source_records'] == 1792 and audits['clear_aggro']['ordered_calls'] == 200
    assert audits['clear_aggro']['checks'] == 64700 and audits['clear_aggro']['library'] == world_so
    assert audits['clear_aggro']['genuine_Lua_ClearAggro'] and audits['clear_aggro']['actual_monster_OutSight'] == 1
    assert audits['clear_aggro']['nested_VM_events'] == 1 and audits['clear_aggro']['finalizer_clear']
    assert audits['player_scene_objects']['checks'] == 453 and audits['player_scene_objects']['world_library'] == world_so
    assert audits['player_scene_objects']['actual_KnightPlayerBase'] and audits['player_scene_objects']['original_Lua_player_target']
    assert audits['player_scene_objects']['live_player_dead_and_sight_reads'] and audits['player_scene_objects']['retained_player_record']
    assert audits['player_scene_objects']['finalizer_target_clear']
    assert audits['object_position']['checks'] == 448 and audits['object_position']['world_library'] == world_so
    assert audits['object_position']['capture_before_argument_projection'] and audits['object_position']['position_read_after_argument_projection']
    assert audits['object_position']['retained_position_finalizer'] and audits['object_position']['three_raw_numbers']
    assert audits['owned_target_pipeline']['actual_monster_EnemySpotted'] == 1 and audits['owned_target_pipeline']['owned_authored_threat_bits'] == 0x41200000
    assert audits['owned_target_pipeline']['live_target_position_method'] and audits['owned_target_pipeline']['retained_target_finalizer']
    assert audits['owned_target_pipeline']['world_library'] == world_so and audits['owned_target_pipeline']['data_library'] == data_so
    assert audits['spawn_select']['original_cases'] == 3456 and audits['spawn_select']['permission_original_cases'] == 512
    assert audits['spawn_select']['genuine_TimerStore_owned_StateInfo_compositions'] == 4
    assert audits['spawn_body']['original_cases'] == 5458 and audits['spawn_body']['original_ordered_requests'] == 60104
    assert audits['spawn_body']['genuine_native_target_set_sync_compositions'] == 1
    assert audits['kill']['original_gold_cases'] == 2200 and audits['kill']['synchronous_reentry_gold_cases'] == 22
    assert audits['kill']['checks'] == 484688 and audits['kill']['service_failure_prefixes'] == 836
    assert audits['kill']['module_library'] == audits['kill']['world_library'] == world_so
    assert audits['kill']['data_library'] == data_so and audits['kill']['runtime_library'] == runtime_so
    assert not audits['kill']['full_loot_xp_quest_AI_backends']
    assert audits['design_settings']['record_comparisons'] == 385 and audits['design_settings']['word_comparisons'] == 33484
    assert audits['design_settings']['owned_threat_enemy_prefix_calls'] == 15 and audits['design_settings']['pinned_after_owner_and_input_destruction']
    assert audits['spawn_owner']['checks'] == 83930 and audits['spawn_owner']['actual_PreSpawn_Spawn_Idle_transition']
    assert audits['spawn_owner']['nested_interactive_event'] and audits['spawn_owner']['nested_notification_transition'] and audits['spawn_owner']['elapsed_once']
    assert audits['spawn_owner']['original_Spawn_body_cases'] == 5458 and not audits['spawn_owner']['full_scene_backends']
    assert audits['AI_death']['checks'] == 161578 and audits['AI_death']['service_failure_prefixes'] == 896
    assert audits['AI_death']['original_gold_cases'] == 1600 and audits['AI_death']['synchronous_reentry_cases'] == 20
    assert audits['AI_death']['module_library'] == audits['AI_death']['world_library'] == audits['AI_death']['timer_stop_library'] == world_so
    assert audits['AI_death']['genuine_SM_SetDeadState_failure_prefixes'] == 1 and not audits['AI_death']['full_group_state_aggro_skill_spell_bodies']
    assert audits['cancel_sneaking']['checks'] == 34930 and audits['cancel_sneaking']['cases'] == 976
    assert audits['cancel_sneaking']['actual_cache_cases'] == 152 and audits['cancel_sneaking']['native_guards'] == 15
    assert not audits['cancel_sneaking']['full_DelBuff'] and not audits['cancel_sneaking']['full_skill_VM']
    assert audits['dead_select']['checks'] == 186572 and audits['dead_select']['failure_prefixes'] == 1384
    assert audits['dead_select']['original_cases'] == 2400 and audits['dead_select']['genuine_state_owner_compositions'] == 14
    assert audits['dead_select']['module_library'] == audits['dead_select']['world_library'] == world_so
    assert audits['dead_select']['data_library'] == data_so and audits['dead_select']['runtime_library'] == runtime_so
    assert audits['dead_select']['missing_dead_focus_backends_rejected']
    assert audits['skill_tables']['checks'] == 40212 and audits['skill_tables']['native_guards'] == 14868
    assert audits['skill_tables']['actual_lists'] == 36 and audits['skill_tables']['actual_skills'] == 127
    assert audits['skill_tables']['pin_after_owner_and_inputs_destroyed'] and audits['skill_tables']['owned_CancelSneaking_calls'] == 1
    assert data_so in dependencies['skill_tables'] and world_so in dependencies['skill_tables']
    assert audits['sneaking_tables']['checks'] == 9211 and audits['sneaking_tables']['cases'] == 521
    assert audits['sneaking_tables']['pinned_after_loader_and_inputs_destroyed']
    assert audits['actor_initialization']['checks'] == 4780 and audits['actor_initialization']['atomic_rejections'] == 4724
    init_projection = audits['actor_initialization']['native_projection']
    assert init_projection['records'] == actor_manifest['records'] and init_projection['sources'] == actor_manifest['sources']
    assert init_projection['descriptor_sha256'] == sha(assets/'worlds/crypt01.dact')
    assert audits['asset_sha256']['checks'] == 13268
    assert audits['asset_sha256']['hashlib_fixture_comparisons'] == 1104
    assert audits['asset_sha256']['actual_DACT_sha256'] == sha(assets/'worlds/crypt01.dact')
    assert audits['AI_state_changed']['checks'] == 25641 and audits['AI_state_changed']['cases'] == 1578
    assert audits['AI_state_changed_VM']['checks'] == 737 and audits['AI_state_changed_VM']['scoped_nested_calls'] == 2
    assert audits['Idle_update']['checks'] == 241943 and audits['Idle_update']['original_cases'] == 1800
    assert audits['Idle_update']['linked_FSM_State_IsPlayer_compositions'] == 12
    assert audits['Idle_update']['module_library'] == audits['Idle_update']['world_library'] == audits['Idle_update']['provider_library'] == world_so
    assert audits['NPC_body']['placements'] == 11 and audits['NPC_body']['marker_cases'] == 6 and audits['NPC_body']['skin_cases'] == 5
    assert audits['NPC_body']['genuine_Box2D_bodies_created_and_destroyed'] == 11
    assert audits['NPC_body']['world_library'] == world_so and audits['NPC_body']['data_library'] == data_so
    assert npc_identity_projection(npc_fixture.read_bytes()) == npc_identity_projection((world/'reference/character-npc-body/source-fixtures.bin').read_bytes())
    assert sha(npc_projection) == sha(world/'reference/character-npc-body/source-model-projections.json')
    idle_events = audits['Idle_events']
    assert idle_events['checks'] == 9754 and idle_events['actual_monster_Init'] == 11
    assert idle_events['banks'] == 4 and idle_events['split_phase_pairs'] == 2420
    assert idle_events['generated_animation_events'] == 362 and idle_events['full_AI_router_services'] == 950
    assert idle_events['actual_commons_End_callbacks'] == 68 and idle_events['genuine_DebugSwitches_queries'] == 215
    assert idle_events['failure_checks'] == 8 and idle_events['scoped_recursive_end_callbacks'] == 3
    assert idle_events['same_FSM_before_Init'] and idle_events['per_instance_pose_clock_isolation']
    assert not idle_events['elapsed_dt_incremented'] and not idle_events['full_original_frame']
    assert not idle_events['physics_navigation'] and not idle_events['controller_gate_producers']
    assert all(path in dependencies['Idle_events'] for path in (world_so,data_so,runtime_so,scene_so))
    fx_preload = audits['FX_preload']
    assert fx_preload['checks'] == 46798
    assert fx_preload['module_library'] == fx_preload['world_library'] == world_so
    assert not (fx_empty_files/'DebugSwitches.savegame').exists()
    effects_tables = audits['FX_tables']
    assert effects_tables['checks'] == 1712 and effects_tables['atomic_rejections'] == 650
    assert effects_tables['sets'] == effects_tables['steps'] == 276
    assert effects_tables['character_rows'] == 3 and effects_tables['footstep_rows'] == 7
    assert effects_tables['dictionary_rows'] == 284 and effects_tables['actual_fx77_registrations'] == 200
    assert effects_tables['native_snapshot_pinned_after_inputs_and_loader_destroyed']
    assert effects_tables['FX_factory_or_playback'] is False
    assert effects_tables['world_library'] == world_so and effects_tables['data_library'] == data_so
    assert not (fx_table_files/'DebugSwitches.savegame').exists()
    init_fx = audits['Init_FX']
    assert init_fx['gold_cases'] == 480 and init_fx['checks'] == 24881
    assert init_fx['mismatches'] == 0 and init_fx['ordered_services'] == 4592
    assert init_fx['atomic_guards'] == 8 and init_fx['failed_prefix_checks'] == 2
    assert init_fx['unsupported_factory_checks'] == 1 and init_fx['host_reentry_checks'] == 1
    assert all(path in dependencies['Init_FX'] for path in (world_so,data_so,runtime_so,scene_so))
    can_update = audits['Can_update']
    assert can_update['comparisons'] == 4752 and can_update['ordered_services'] == 13833
    assert can_update['malformed_and_delivery_checks'] == 23 and can_update['mismatches'] == 0
    assert world_so in dependencies['Can_update']
    material_color = audits['Material_color']
    assert material_color['original_gold_cases'] == 7899 and material_color['atomic_guards'] == 13
    assert material_color['mismatches'] == 0 and animation_so in dependencies['Material_color']
    particle = audits['Particle_parameter']
    assert particle['original_gold_cases'] == 4925 and particle['atomic_guards'] == 16
    assert particle['mismatches'] == 0 and animation_so in dependencies['Particle_parameter']
    startup = audits['Update_startup']
    assert startup['comparisons'] == 1404 and startup['ordered_prefix_services'] == 14451
    assert startup['ordered_eligibility_services'] == 2616 and startup['explicit_continuation_stops'] == 9
    assert startup['live_string_checks'] == 7848 and startup['malformed_and_delivery_checks'] == 20
    assert startup['mismatches'] == 0 and world_so in dependencies['Update_startup']
    deferred = audits['Deferred_script']
    assert deferred['gold_cases'] == 1165 and deferred['ordered_services'] == 5773
    assert deferred['atomic_guards'] == 13 and deferred['required_failure_prefixes'] == 5
    assert deferred['mismatches'] == 0 and world_so in dependencies['Deferred_script']
    deferred_session = audits['Deferred_session']
    assert deferred_session['actual_Crypt_monster_load_then_init'] == 11
    assert deferred_session['active_Post_calls'] == 14 and deferred_session['active_Final_calls'] == 13
    assert deferred_session['nested_active_guard_calls'] == 29 and deferred_session['checks'] == 914
    assert deferred_session['required_failure_prefixes'] == 1 and deferred_session['mismatches'] == 0
    delayed_idle = audits['Delayed_Idle']
    assert delayed_idle['actual_delayed_monsters'] == delayed_idle['source_Idle_before_AIS'] == 11
    assert delayed_idle['retained_FSM_and_CPU_owners'] == 11 and delayed_idle['split_phase_pairs'] == 22
    assert not delayed_idle['source_elapsed_incremented'] and not delayed_idle['full_InitPost_or_Update']
    assert delayed_idle['required_vitals_and_skills_are_fixtures'] and delayed_idle['mismatches'] == 0
    for name in ('Deferred_session','Delayed_Idle'):
        assert all(path in dependencies[name] for path in (world_so,data_so,runtime_so,scene_so,animation_so))
    vitals = audits['Init_vitals']
    assert vitals['gold_cases'] == 900 and vitals['ordered_debug_property_services'] == 3333
    assert vitals['atomic_guards'] == 15 and vitals['required_failure_prefixes'] == 2 and vitals['mismatches'] == 0
    vitals_session = audits['Init_vitals_session']
    assert vitals_session['actual_Crypt_session_vitals'] == vitals_session['identity_atomic_guards'] == vitals_session['missing_skills_rejected'] == 11
    assert vitals_session['combat_state_unchanged'] and vitals_session['mismatches'] == 0
    delayed_vitals = audits['Delayed_Idle_vitals']
    assert delayed_vitals['actual_delayed_monsters'] == delayed_vitals['source_Idle_before_AIS'] == delayed_vitals['retained_FSM_and_CPU_owners'] == 11
    assert delayed_vitals['split_phase_pairs'] == 22 and delayed_vitals['source_vitals_and_debug_delivered']
    assert delayed_vitals['combat_state_unchanged'] and delayed_vitals['required_skills_are_fixtures']
    assert not delayed_vitals['source_elapsed_incremented'] and not delayed_vitals['full_InitPost_or_Update'] and delayed_vitals['mismatches'] == 0
    for name in ('Init_vitals','Init_vitals_session','Delayed_Idle_vitals'):
        assert world_so in dependencies[name] and data_so in dependencies[name]
    queue = audits['Deferred_queue']
    assert queue['comparisons'] == 10463 and queue['ordered_callbacks'] == 3729
    assert queue['ordered_entries'] == 317163 and queue['synchronous_reentry_records'] == 110
    assert queue['native_safety_checks'] == 17 and queue['mismatches'] == 0 and world_so in dependencies['Deferred_queue']
    factory = audits['Particle_factory']
    assert factory['original_gold_cases'] == 1850 and factory['ownership_checks'] == 11
    assert factory['additional_contract_checks'] == 4 and factory['mismatches'] == 0
    assert animation_so in dependencies['Particle_factory'] and scene_so in dependencies['Particle_factory']
    source_services = audits['Source_services']
    assert source_services['checks'] == 757 and source_services['private_path_and_cache_owners'] == 2
    assert source_services['required_failure_kinds'] == 4 and source_services['caught_required_failures_rejected']
    assert source_services['arguments_without_fixed_arity_cap'] == 18 and source_services['mismatches'] == 0
    required_deferred = audits['Required_deferred']
    assert required_deferred['required_Post_and_Final_failures'] == 3 and required_deferred['Lua_caught_required_failure_rejected']
    assert required_deferred['native_prefix_preserved'] and required_deferred['no_implicit_retry'] and required_deferred['mismatches'] == 0
    assert all(path in dependencies['Required_deferred'] for path in (world_so,data_so,runtime_so))
    queued = audits['Update_queued']
    assert queued['comparisons'] == 2028 and queued['ordered_prefix_services'] == 26059
    assert queued['ordered_eligibility_services'] == 4248 and queued['ordered_queue_callbacks'] == 804
    assert queued['reentry_records'] == 80 and queued['failure_and_contract_checks'] == 19
    assert queued['mismatches'] == 0 and world_so in dependencies['Update_queued']
    queued_session = audits['Update_queued_session']
    assert queued_session['checks'] == 5721 and queued_session['actual_delayed_monsters'] == 11
    assert queued_session['queued_prefix_calls'] == queued_session['application_stats_increments'] == 22
    assert queued_session['same_native_FSM_state_queries'] == 66 and queued_session['retained_queue_entries'] == 9
    assert queued_session['source_vitals_and_debug_delivered'] and queued_session['required_skills_are_fixtures']
    assert queued_session['CanUpdate_and_Application_producers_are_fixtures'] and queued_session['Kill_and_full_AIUnload_are_fixtures']
    assert not queued_session['full_InitPost_or_Update'] and queued_session['mismatches'] == 0
    assert all(path in dependencies['Update_queued_session'] for path in (world_so,data_so,runtime_so,scene_so,animation_so))
    emission = audits['Particle_emission']
    assert emission['comparisons'] == 1496 and emission['actual_fx_frames'] == 402
    assert emission['emitted_particles'] == [6,7] and emission['retained_resource_and_generation']
    assert emission['atomic_guards'] == 2
    assert all(path in dependencies['Particle_emission'] for path in (scene_so,animation_so))
    faeries = audits['Faery_tables']
    assert faeries['record_comparisons'] == 208 and faeries['list_comparisons'] == 100
    assert faeries['checks'] == 3729 and faeries['atomic_guards'] == 1086
    assert faeries['retained_after_owner_inputs_destroyed'] and faeries['actual_default_five_null_scripts']
    skills_audit = audits['Skills']
    assert skills_audit['comparisons'] == 62 and skills_audit['ordered_requests'] == 898
    assert skills_audit['checks'] == 5750 and skills_audit['nested_reentry_cases'] == 2
    assert skills_audit['required_failure_prefixes'] == 29 and skills_audit['cleanup_keeps_slots']
    assert skills_audit['mismatches'] == 0 and world_so in dependencies['Skills']
    skills_session = audits['Skills_session']
    assert skills_session['checks'] == 743 and skills_session['actual_Crypt_OnInit_property_inventories'] == 11
    assert skills_session['real_ScriptOwner_setup_compositions'] == 11 and skills_session['retained_null_spell_slots'] == 55
    assert skills_session['nonempty_controlled_skill_instances'] == 6 and skills_session['alias_return_projection_checks'] == 18
    assert skills_session['actual_skills_common_VM'] and skills_session['caught_and_uncaught_required_failures_rejected']
    assert skills_session['table_inputs_destroyed_while_pinned'] and not skills_session['whole_authored_skill_scripts']
    assert all(path in dependencies['Skills_session'] for path in (world_so,data_so,runtime_so))
    gfnt = audits['GFNT']
    assert gfnt['comparisons'] == 8532 and gfnt['present_glyphs'] == 272
    assert gfnt['checks'] == 60239 and gfnt['atomic_guards'] == 234
    assert gfnt['owned_snapshot_survives_inputs'] and gfnt['advance_twips_verified']
    assert gfnt['mismatches'] == 0 and ui_so in dependencies['GFNT']
    shader_sources = audits['Shader_sources']
    assert shader_sources['original_gold_cases'] == 860 and shader_sources['stored_pack_members'] == 34
    assert shader_sources['atomic_guards'] == 13 and shader_sources['owned_body_and_pack']
    assert shader_sources['mismatches'] == 0 and scene_so in dependencies['Shader_sources']
    swf_texture = audits['Swf_texture']
    assert swf_texture['comparisons'] == 2879 and swf_texture['atomic_guards'] == 11
    assert swf_texture['mismatches'] == 0 and scene_so in dependencies['Swf_texture']
    movie = audits['Swf_movie']
    connected_as=audits['Swf_AS_connection']
    assert connected_as['typed_arguments']==21 and connected_as['guards']==17
    assert connected_as['original_menu_methods']==20 and connected_as['authored_potion_callbacks']==1
    assert connected_as['limits']['native_game_services_are_host_fixtures']
    startup=audits['HUD_startup']
    assert startup['comparisons']==1946 and startup['ordered_services']==8736
    assert startup['guards']==12 and startup['failure_prefixes']==14 and startup['host_service_reentry_checks']==1
    owned_settings=audits['Owned_HUD_settings']
    assert owned_settings['comparisons']==1038 and owned_settings['additional_checks']==259
    assert owned_settings['atomic_guards']==12 and owned_settings['required_failure_prefixes']==4
    assert owned_settings['real_stdio_missing_and_present'] and owned_settings['mismatches']==0
    language_scene=audits['Settings_language_scene']
    assert language_scene['comparisons']==162 and language_scene['required_failure_and_live_mutation_checks']==3
    assert language_scene['guards']==3 and language_scene['mismatches']==0
    assert audits['HUD_manager']==dict(validation='PASS',cases=650,ordered_services=22584,failure_guards=3494)
    assert audits['HUD_manager_backends']==dict(validation='PASS',comparisons=1408,ordered_services=808,required_failure_prefixes=808,atomic_guards=7)
    assert audits['HUD_manager_reentry']==dict(validation='PASS',cases=48,nested_same_manager_calls=48,ordered_services=3895)
    manager_core=audits['HUD_manager_core']
    assert manager_core['whole_manager_updates']==38 and manager_core['authored_hud_styles']==2
    assert manager_core['ordered_services']==1874 and manager_core['weak_cache_gets']==437
    assert manager_core['source_frame_calls']==203 and manager_core['plain_text_calls']==41
    assert manager_core['ownership_failure_guards']==7
    initialization=audits['HUD_initialization_v1']
    assert initialization['comparisons']==962 and initialization['ordered_services']==7240
    assert initialization['required_failure_prefixes']==8907 and initialization['mismatches']==0
    base_initialization=audits['HUD_initialization_base_v1']
    assert base_initialization['authored_hud_styles']==4 and base_initialization['manager_supported_styles']==4
    assert base_initialization['matching_source_cache_paths']==30
    assert base_initialization['manager_required_cache_rejections']==0
    assert audits['HUD_initialization_droid_v1']['manager_supported_styles']==2
    assert audits['HUD_initialization_droid_v1']['manager_required_cache_rejections']==2
    for name in ('HUD_initialization_base_v1','HUD_initialization_droid_v1'):
        assert world_so in dependencies[name] and data_so in dependencies[name]
    assert world_so in dependencies['HUD_manager_core'] and world_so in dependencies['HUD_manager_backends']
    assert movie['triangle_strips'] == 1116 and movie['line_strips'] == 2102
    assert movie['vertices'] == 24468 and movie['mask_submissions'] == 140
    assert movie['export_requests'] == 5 and movie['native_calls'] == 2
    assert movie['core_error_diagnostics'] == 27 and movie['borrowed_provider_checks'] == 2
    assert movie['reentry_rejections'] == 1
    for name,checks in (('Freetype_font',2284),('Freetype_HUD',2292)):
        font = audits[name]
        assert font['checks'] == checks and font['rasters'] == 570 and font['atomic_guards'] == 5
    hud = audits['Freetype_HUD']
    assert hud['alpha_uploads'] == 238 and hud['nonempty_alpha_uploads'] == 232
    assert hud['bitmap_quads'] == 1032 and hud['font_reads'] == 4 and hud['font_misses'] == 1
    assert hud['core_diagnostics'] == 31
    resolver = audits['Swf_font_resolver']
    assert resolver['original_cases'] == 1152 and resolver['ordered_services'] == 4544
    assert resolver['failure_prefixes'] == 6 and resolver['atomic_guards'] == 9 and resolver['nested_calls'] == 1
    actual_hud = audits['Swf_font_HUD']
    assert actual_hud['alpha_uploads'] == 245 and actual_hud['nonempty_alpha_uploads'] == 239
    assert actual_hud['bitmap_quads'] == 1032 and actual_hud['font_reads'] == 4
    assert actual_hud['genuine_file_opens'] == actual_hud['genuine_file_closes'] == 4
    assert actual_hud['resolved_fonts'] == ['Fontin SmallCaps','Arial']
    assert audits['UI_viewport']['comparisons'] == 5200 and audits['UI_viewport']['failure_reentry_guards'] == 12
    assert audits['Localization']['comparisons'] == 4388 and audits['Localization']['atomic_guards'] == 4
    connection = audits['Localization_connection']
    assert connection['comparisons'] == 260 and connection['ordered_callbacks'] == 4760
    assert connection['actual_text_files'] == 333 and connection['owned_text_strings'] == 41546
    assert connection['guards'] == 5 and connection['opened'] == connection['closed']
    assert connection['real_constants'] and connection['real_DebugSwitches'] and connection['mismatches'] == 0
    packed = audits['HUD_freetype_font']
    assert packed['gray_regression_cases'] == 570 and packed['actual_wqy_glyphs'] == 5415
    assert packed['packed_wqy_glyphs'] == packed['mode1_glyphs'] == 380
    assert packed['mode2_glyphs'] == 5035 and packed['atomic_guards'] == 5
    connected_hud = audits['HUD_freetype_provider']
    assert connected_hud['alpha_uploads'] == 246 and connected_hud['nonempty_alpha_uploads'] == 239
    assert connected_hud['bitmap_quads'] == 1056 and connected_hud['packed_uploads'] == 7
    assert connected_hud['genuine_file_opens'] == connected_hud['genuine_file_closes'] == connected_hud['font_reads']
    assert connected_hud['resolved_fonts'] == ['Fontin SmallCaps','Arial']
    assert connected_hud['localization_calls'] > 0 and connected_hud['text_sheets'] > 0
    lookup = audits['Swf_glyph_lookup']
    assert lookup['original_cases'] == 6912 and lookup['ordered_services'] == 15768
    assert lookup['failure_prefixes'] == 5 and lookup['atomic_guards'] == 6 and lookup['nested_calls'] == 1
    for tag,resources,occurrences,checks in (('skeleton',23,44,81049),('slime',23,37,26329),('slime-red',23,37,26329),('ghost',18,28,130004)):
        audit = audits['monster_animation_'+tag]
        assert audit['checks'] == checks and audit['frames'] == 180
        assert audit['resources'] == resources and audit['registration_occurrences'] == occurrences
        assert audit['source_Idle_Focus'] and audit['per_character_pose_and_clock_isolation']
        assert audit['CPU_backing_retained_after_external_owner_release']
        assert audit['template_start'] == (-133 if tag=='ghost' else 0)
        assert not audit['full_AI_event_delivery'] and not audit['full_physical_frame']
    assert audits['timer_effects']['checks'] == 106022 and audits['timer_effects']['module_library'] == world_so
    assert audits['sessions']['actual_Crypt_monster_initializations'] == 11
    assert audits['sessions']['checks'] == 10239 and audits['sessions']['world_library'] == world_so
    assert audits['debug']['module_library'] == world_so and audits['debug']['checks'] == 7431
    assert audits['objects']['ordered_original_methods'] == 171
    assert audits['targets']['kernel_library'] == world_so
    assert audits['host']['module_library'] == world_so and audits['host']['checks'] == 31004
    assert audits['spatial']['kernel_library'] == world_so
    assert audits['owner_integers']['owner_library'] == world_so and audits['owner_integers']['checks'] == 6979
    assert before == binary_hashes()
    assert hashes == {path.relative_to(ROOT).as_posix():sha(path) for path in sources}
    assert input_hashes == {path.relative_to(ROOT).as_posix():sha(path) for path in files}
    result = dict(validation='PASS',scope=__doc__,host_audits=audits,source_sha256=hashes,
        input_sha256=input_hashes,binary_sha256=before,compiler_records=records,
        underlying_proof_sha256=proofs,linked_dependencies=dependencies,commands=commands,
        historical_proof_source_changes=historical_proof_changes,
        sanitizer_findings=0,actual_main_CMake_targets=True,full_AI_timer_expiry=False,
        source_Application_session_selection=False,live_Android_monster_Init=False,
        packaged_APK=False,full_game_verified=False,physical_arm64_verified=False)
    result['native_UI_scope'] = dict(source_core_revision=1714,freetype_version='2.3.7',
        core_vptr_instrumentation=False,wrapper_vptr_instrumentation=True,
        full_ActionScript_fork_parity=False,GPU_submission_verified=False,
        full_HUD_localization_and_texture_providers=False,
        unresolved_fixture_font_names=['Arial'],resolved_genuine_HUD_font_names=['Fontin SmallCaps','Arial'],
        source_viewport_arithmetic=True,live_original_viewport_publication=False,
        native_string_text_only_connection=True,Application_player_language_owners=False)
    result['native_UI_scope'].update(packed_glyph_correction=True,
        localized_HUD_packed_glyph_replay=True,source_glyph_lookup_projection=True,
        core_provider_first_glyph_order_connected=True,source_HUD_status_frame_producer=True,
        retained_original_status_timeline_connected=True,real_player_property_producers_in_host=True,
        live_original_viewport_publication=True,live_Android_HUD_verified=False,
        retained_typed_AS_callbacks_in_host=True,pre_construction_observer_hook=True,
        native_string_text_only_connection=False,live_game_native_callbacks=False,
        owned_settings_design_table_and_raw_file_reader=True,
        source_language_scene_traversal=True,language_inventory_producers_are_fixtures=True,
        original_full_text_layout=False,HTML_backend=False,
        full_local_player_inventory_and_saved_skills=False)
    result['native_UI_scope'].update(whole_source_HUD_manager_in_host=True,
        retained_weak_HUD_caches=True,source_potion_slot_level_and_cooldown_reads=True,
        live_world_and_script_HUD_producers_are_fixtures=True,
        dynamically_initialized_HUD_styles_2_and_3=False,
        original_root_and_sprite_frame_advance=False)
    result['native_UI_scope'].update(original_root_and_sprite_frame_advance=True,
        source_cursor_input_and_ordered_button_dispatch=True,
        fresh_default_observer_owner_per_facade_load=True,
        generation_pinned_input_sessions=True,source_loader_recreation_safe=True,
        authoritative_inventory_equipment_storage_v4=True,
        live_Android_input_or_full_menu_connection=False)
    result['native_UI_scope'].update(retained_single_authority_player_skills=True,
        genuine_initialized_player_classes=audits['Player_skills_v2']['genuine_player_sessions'],
        genuine_initialized_skill_instances=audits['Player_skills_v2']['source_skill_instances'],
        owned_player_InitVCB_and_saved_row_skill_levels=True,
        actual_authored_skill_cooldown_creation=False,full_gameplay_skill_lifecycle=False)
    result['native_UI_generated_outputs'] = {path.relative_to(ROOT).as_posix():sha(path)
        for path in font_output.iterdir() if path.is_file()}
    result['native_UI_scope'].update(original_text_layout_coordinator_in_host=True,
        text_layout_case_comparisons=audits['Text_layout_v1']['whole_original_cases'],
        owned_savegame_skills_faeries_inventory_and_profile_sections=True,
        fresh_campaign_player_and_initial_gear_producers=False,
        retained_text_layout_and_GPU_connection=False)
    result['native_UI_scope'].update(whole_HUD_initialization_native_wrappers=True,
        source_selected_base_HUD_all_four_styles_in_host=True,
        base_HUD_authored_stage=[1024,768],
        droid_variant_mismatch_preserved=True,
        owned_HUD_save_and_skill_queries=True,
        localization_parseEx_and_skill_info_platform_producers_are_fixtures=True)
    result['native_UI_scope'].update(whole_original_glyph_display_in_host=True,
        glyph_display_original_trace_comparisons=audits['Text_display_v2']['whole_original_cases'],
        real_owned_font_metric_raster_texture_draw_composition=True,
        font_and_GPU_producer_results=audits['Text_render_owner_v2'],
        retained_ActionScript_text_field_migration=False,
        fresh_starting_loot_owned_storage_and_grant_callers_in_host=True,
        full_native_auto_equipment_and_skill_effects=False)
    result['native_UI_scope'].update(original_parseEx_and_owned_localized_skill_text_in_host=True,
        single_call_source_return_observer_in_main_runtime=True,
        source_temporary_skill_properties_and_two_real_cache_scripts=True,
        full_retained_active_skill_session_verified=False,
        formatting_original_cases=audits['HUD_text_format_v1']['cases'],
        skill_info_original_cases=audits['Skill_info_v1']['cases'],
        real_skill_script_cases=audits['HUD_skill_text_v1']['real_skill_script_cases'])
    result['NPC_generated_outputs'] = {path.relative_to(ROOT).as_posix():sha(path) for path in (npc_fixture,npc_projection)}
    result['native_item_effects_v5_scope']=dict(
        same_authoritative_inventory_properties_and_live_buffs=True,
        real_item_power_tables=True,real_cache_localized_item_text=True,
        class_recalculation_and_HP_MP_clamping=True,
        source_Skin_caller_choreography=True,full_visual_Skin_resource_graph=False,
        full_powered_loot_valuation=False,source_integer_string_formatter_domain=True,
        full_floating_varargs_continuation=False,
        real_cache_starting_classes=3,original_same_inventory_cases=6,original_same_inventory_steps=112,
        live_Android_item_effects_connected=False,full_campaign_player_creation=False)
    result['NPC_identity_normalized_fixture_sha256'] = hashlib.sha256(npc_identity_projection(npc_fixture.read_bytes())).hexdigest()
    skill_private_v3=json.loads((world/'reports/character-skill-gameplay-v3-player-host-audit.json').read_text())
    for suite,key in (('Player_skills_v3','player'),('Skill_AI_v3','ai'),('Skill_callbacks_v3','callbacks')):
        assert audits[suite]==skill_private_v3['results'][key],(suite,audits[suite])
    indexed_private_v3=json.loads((world/'reports/character-skill-gameplay-v3-indexed-runtime-host-audit.json').read_text())
    assert audits['Indexed_runtime_v3']==indexed_private_v3['results']['indexed']
    assert audits['Loot_power_creation_v7']==loot_freeze_v7['native_host_result']
    assert sha(cache_archive)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
    result['external_input_sha256']={str(cache_archive):sha(cache_archive)}
    assert not any(p['file'].endswith('/script_runtime_return_v1.c') or
                   p['file'].endswith('/script-runtime/script_runtime.c') for p in compiler)
    result['native_player_skill_gameplay_v3_scope']=dict(
        same_retained_player_VM_properties_saved_skills_faeries_buffs_timers=True,
        complete_original_scripts=219,real_class_sessions=3,
        source_begin_end_use_cancel_focus_event_blur=True,
        source_passive_buff_apply_replace_remove=True,
        source_cooldown_create_expire_same_session=True,
        original_optimized_ARM64_cases=8800,
        full_active_SkillFSM6_animation_targeting_combat=False,
        live_Android_player_skills_connected=False,full_campaign_player_creation=False)
    result['native_loot_power_creation_v7_scope']=dict(
        real_weighted_power_and_quantity_selection=True,
        original_retry_and_ordered_conflict_fallback=True,
        item_value_gold_value_and_name_update=True,
        same_V5_power_definitions_instances_and_real_localization=True,
        same_V4_inventory_and_borrowed_rng=True,
        original_optimized_ARM64_cases=4781,actual_power_lists=121,
        actual_quantity_lists=39,real_cache_powered_items=363,
        full_AddLoot_random_subloot_pickup_merchant=False,
        live_Android_loot_connected=False)
    args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(dict(validation='PASS',suites=len(audits),sanitizer_findings=0,output=str(args.output))))


if __name__ == '__main__':
    main()
