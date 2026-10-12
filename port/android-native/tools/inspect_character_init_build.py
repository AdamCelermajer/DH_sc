"""Inspect captured native character initialization builds, without live claims."""
import argparse
import hashlib
import io
import json
import re
from pathlib import Path
import zipfile
from elftools.elf.elffile import ELFFile
from validate_prince_bank_checkpoint import native_library

ROOT = Path(__file__).resolve().parents[3]


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def ui_assets(paths, assets):
    """Validate immutable stage overlap, physical case, and all three indices."""
    union, receipts = {}, []
    cache = '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
    asset_root = ROOT/'port/android-native/app/src/main/assets'
    for label, path, count, prior, total, index in zip(
            ('menu','text','texture'), paths, (44,337,6), (382,427,765), (427,765,770),
            ('index.json','text-index.json','texture-index.json')):
        raw = path.read_bytes()
        receipt = json.loads(raw)
        assert receipt['validation'] == 'PASS' and receipt['cache_sha256'] == cache
        assert receipt['unmodified_original_bytes'] and receipt['resource_count'] == count
        assert receipt['prior_assets_preserved'] == prior and receipt['assets_per_project'] == total
        assert len(receipt['resources']) == count
        for key, row in receipt['resources'].items():
            assert key == row['uri'].casefold() and row['asset'] == 'original-cache/'+row['uri']
            assert row['cache_member'] == 'com.gameloft.android.GAND.GloftD2SS/files/'+row['uri']
            assert assets['assets/'+row['asset']] == row['sha256']
            assert (asset_root/row['asset']).stat().st_size == row['bytes']
            assert key not in union or union[key] == row, 'Conflicting UI resource overlap'
            union[key] = row
        index_path = asset_root/'original-cache'/index
        assert sha(index_path.read_bytes()) == receipt['index_sha256']
        index_data = json.loads(index_path.read_bytes())
        assert index_data['cache_sha256'] == cache and index_data['resources'] == receipt['resources']
        receipts.append(dict(stage=label, sha256=sha(raw), path=str(path.resolve()),
                             resource_count=count, index_sha256=receipt['index_sha256']))
    assert len(union) == 385 and len(assets) == 770
    # The catalog is actually included by original_ui_assets.cpp, not a runtime
    # promise based only on the stage receipt. Compare every compiled row.
    catalog = ROOT/'port/android-native/app/src/main/cpp/original_ui_asset_catalog.inc'
    rows = re.findall(r'\{"([^"\\]+)","([^"\\]+)","([^"\\]+)","([0-9a-f]{64})",(\d+)u\}', catalog.read_text())
    actual = {key:dict(uri=uri, asset=asset, sha256=digest, bytes=int(size))
              for key,uri,asset,digest,size in rows}
    expected = {key:{field:row[field] for field in ('uri','asset','sha256','bytes')}
                for key,row in union.items()}
    assert len(rows) == 385 and actual == expected, 'Compiled UI catalog differs from receipts'
    return dict(stages=receipts, unique_original_resources=385, total_assets=770,
                catalog_sha256=sha(catalog.read_bytes()), original_bytes_unmodified=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--capture', type=Path, required=True)
    parser.add_argument('--host', type=Path, required=True)
    parser.add_argument('--monster-backings-stage', type=Path)
    parser.add_argument('--effects-tables-stage', type=Path)
    parser.add_argument('--shader-assets-stage', type=Path)
    parser.add_argument('--ui-assets-stage', type=Path, nargs=3, metavar=('MENU','TEXT','TEXTURE'))
    parser.add_argument('--ui-font-freeze', type=Path,
                        help='Dedicated additive HUD-font freeze; required only if those new modules compiled')
    parser.add_argument('--connected-hud-stage',type=Path,
                        help='Versioned connected status/viewport and exact renderer getter migration proof')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.output.exists():
        raise RuntimeError('Preserve existing character-init build proof')
    host = json.loads(args.host.read_text())
    assert host['validation'] == 'PASS'
    asset_root = ROOT/'port/android-native/app/src/main/assets'
    assets = {'assets/'+path.relative_to(asset_root).as_posix():sha(path.read_bytes())
              for path in asset_root.rglob('*') if path.is_file()}
    stage = None
    if args.monster_backings_stage:
        stage = json.loads(args.monster_backings_stage.read_text())
        assert stage['validation'] == 'PASS' and stage['assets_per_project'] == 342
        assert 'asset_sha256' in host['host_audits'] and 'AI_state_changed_VM' in host['host_audits']
        assert all(assets['assets/'+name] == digest for name,digest in stage['asset_sha256'].items())
    effects_stage = None
    if args.effects_tables_stage:
        effects_stage = json.loads(args.effects_tables_stage.read_text())
        assert stage and effects_stage['validation'] == 'PASS' and effects_stage['assets_per_project'] == 347
        assert effects_stage['prior_monster_stage_sha256'] == sha(args.monster_backings_stage.read_bytes())
        assert 'FX_tables' in host['host_audits']
        assert all(assets['assets/'+name] == digest for name,digest in effects_stage['asset_sha256'].items())
    shader_stage = None
    if args.shader_assets_stage:
        shader_stage = json.loads(args.shader_assets_stage.read_text())
        assert effects_stage and shader_stage['validation'] == 'PASS'
        assert shader_stage['shader_member_count'] == 34 and len(shader_stage['asset_sha256']) == 35
        assert shader_stage['original_bytes_unmodified'] and shader_stage['pack_sha256'] == '365a4d3c432454c44208ebb484c7a472a3a4534a0c5c9e77a7a90f3b87b1b5c0'
        assert all(assets['assets/'+name] == digest for name,digest in shader_stage['asset_sha256'].items())
    ui_stage = None
    if args.ui_assets_stage:
        assert shader_stage, 'UI stage requires the source shader stage'
        ui_stage = ui_assets(args.ui_assets_stage, assets)
        for name in ('GFNT','Shader_sources','Swf_texture','Swf_movie','Freetype_font',
                     'Swf_font_resolver','Swf_font_HUD','UI_viewport','Localization','Localization_connection'):
            assert host['host_audits'][name]['validation'] == 'PASS', name
    else:
        assert not args.ui_font_freeze, 'Font freeze requires --ui-assets-stage'
        assert len(assets) == (347 if effects_stage else 342 if stage else 266 if 'design_settings' in host['host_audits'] else 263) + (35 if shader_stage else 0)
    required = ['port/level-world/'+name+'.cpp' for name in (
        'character_script_owner','character_game_design','character_script_session',
        'character_spatial_bindings','character_host_context','character_design_services',
        'character_target_bindings','gameobject_lua_representation',
        'object_identity','character_timer_effects','character_state_owner',
        'character_buffs','character_state_owner_behavior','character_state_owner_frame',
        'character_script_commands','character_target_update','character_target_events',
        'character_ai_events','character_script_objects','character_dot_attack',
        'character_state_empty')]
    required += ['port/script-runtime/script_runtime.c',
                 'port/script-runtime/script_object_bridge.c',
                 'port/script-runtime/lua/lgc.c',
                 'port/script-runtime/lua/ltable.c',
                 'port/game-data/level_tables.cpp']
    stage_exports = set()
    for audit, module, export in (
        ('enemy_spotted','character_enemy_spotted','dh2_character_enemy_spotted'),
        ('pre_spawn','character_pre_spawn','dh2_character_pre_spawn_body'),
        ('hit','character_hit','dh2_character_hit_for'),
        ('state_owner_extensions','character_state_owner_extensions','dh2_character_state_owner_extensions_method'),
        ('clear_aggro','character_clear_aggro','dh2_character_clear_aggro'),
        ('spawn_select','character_spawn_select','dh2_character_spawn_select'),
        ('spawn_body','character_spawn_body','dh2_character_spawn_body'),
        ('kill','character_kill','dh2_character_kill'),
        ('spawn_owner','character_spawn_owner_extensions','dh2_character_spawn_owner_extensions_method'),
        ('AI_death','character_ai_death','dh2_character_ai_on_died'),
        ('cancel_sneaking','character_cancel_sneaking','dh2_character_cancel_sneaking'),
        ('dead_select','character_dead_select','dh2_character_dead_select')):
        if audit in host['host_audits']:
            required.append('port/level-world/'+module+'.cpp')
            stage_exports.add(export)
    if 'state_owner_extensions' in host['host_audits']:
        stage_exports.add('dh2_character_state_owner_extensions_update')
    if 'clear_aggro' in host['host_audits']:
        stage_exports.update({'dh2_character_clear_aggro_bind','dh2_character_clear_aggro_scoped'})
    if 'spawn_select' in host['host_audits']:
        required.append('port/level-world/character_spawn_permission.cpp')
        stage_exports.add('dh2_character_pre_spawn_permission')
    if 'kill' in host['host_audits']:
        stage_exports.add('dh2_character_ctrl_kill')
    if 'design_settings' in host['host_audits']:
        required.append('port/game-data/design_settings.cpp')
    if 'spawn_owner' in host['host_audits']:
        stage_exports.update({'dh2_character_spawn_owner_extensions_update','dh2_character_spawn_owner_select'})
    if 'skill_tables' in host['host_audits']:
        required.append('port/game-data/skill_tables.cpp')
    for audit,module in (('sneaking_tables','character_sneaking_tables'),
                         ('actor_initialization','actor_initialization'),
                         ('monster_animation_skeleton','character_animation_instance'),
                         ('Idle_update','character_idle_update'),
                         ('NPC_body','character_npc_body'),
                         ('Idle_events','character_idle_events'),
                         ('FX_preload','visual_fx_preload'),
                         ('FX_tables','visual_fx_tables'),
                         ('Init_FX','character_init_fx'),
                         ('Can_update','character_can_update'),
                         ('Update_startup','character_update_startup'),
                         ('Deferred_script','character_deferred_script'),
                         ('Deferred_session','character_deferred_script_session'),
                         ('Init_vitals','character_script_init_vitals'),
                         ('Init_vitals_session','character_script_init_vitals_session'),
                         ('Deferred_queue','character_deferred_queue')):
        if audit in host['host_audits']:
            required.append('port/level-world/'+module+'.cpp')
    if 'FX_tables' in host['host_audits']:
        required.append('port/game-data/effects_tables.cpp')
    if 'Init_FX' in host['host_audits']:
        stage_exports.update({'dh2_character_init_fx_register','dh2_character_init_fx_negative_grab'})
    if 'Can_update' in host['host_audits']:
        stage_exports.add('dh2_character_can_update')
    if 'Material_color' in host['host_audits']:
        required.append('port/engine-animation/material_color.cpp')
    if 'Particle_parameter' in host['host_audits']:
        required.append('port/engine-animation/particle_parameter.cpp')
    if 'Update_startup' in host['host_audits']:
        stage_exports.update({'dh2_character_update_startup','dh2_character_update_after_controller'})
    if 'Deferred_script' in host['host_audits']:
        stage_exports.add('dh2_character_deferred_script')
    if 'Init_vitals' in host['host_audits']:
        stage_exports.add('dh2_character_script_init_vitals')
    if 'Init_vitals_session' in host['host_audits']:
        stage_exports.add('dh2_character_script_session_init_service')
    if 'Deferred_queue' in host['host_audits']:
        stage_exports.update({'dh2_character_deferred_queue_create','dh2_character_deferred_queue_assign',
                              'dh2_character_deferred_queue_unload','dh2_character_deferred_queue_evict'})
    if 'Particle_factory' in host['host_audits']:
        required.append('port/engine-animation/particle_factory.cpp')
    if 'Particle_emission' in host['host_audits']:
        required.append('port/engine-animation/particle_emission.cpp')
    if 'Update_queued' in host['host_audits']:
        required.append('port/level-world/character_update_queued.cpp')
        stage_exports.add('dh2_character_update_queued')
    if 'Faery_tables' in host['host_audits']:
        required.append('port/game-data/faery_tables.cpp')
    if 'Skills_session' in host['host_audits']:
        required += ['port/level-world/character_skills.cpp','port/level-world/character_skills_owner.cpp',
                     'port/level-world/character_skills_session.cpp']
        stage_exports.update({'dh2_character_skills_configure','dh2_character_skills_update','dh2_character_skills_cleanup'})
    if 'AI_death' in host['host_audits']:
        stage_exports.add('dh2_character_ai_set_dead')
    if 'cancel_sneaking' in host['host_audits']:
        stage_exports.add('dh2_character_cancel_skill')
    if 'AI_state_changed' in host['host_audits']:
        required += ['port/level-world/character_ai_state_changed.cpp',
                     'port/level-world/character_ai_state_changed_vm.cpp']
        stage_exports.update({'dh2_character_ai_state_changed','dh2_character_ai_end_anim',
                              'dh2_character_ais_external_end_anim'})
    if 'asset_sha256' in host['host_audits']:
        required.append('port/asset-payloads/sha256.cpp')
    if 'Idle_update' in host['host_audits']:
        stage_exports.add('dh2_character_idle_update')
    if 'FX_preload' in host['host_audits']:
        stage_exports.update({'dh2_fx_register_set','dh2_fx_register_effect','dh2_fx_debug_preload_service'})
    ui_required = []
    if ui_stage:
        required += ['port/engine-ui/'+name+'.cpp' for name in
                     ('gfnt','swf_movie','freetype_font','freetype_glyph_kernel',
                      'swf_freetype_provider','swf_font_geometry','swf_font_resolver','viewport','localization')]
        required += ['port/scene-materials/shader_sources.cpp','port/scene-materials/swf_texture.cpp']
        for module in ('native_app','authored_shader_program','swf_gpu','original_ui_assets','original_ui_session'):
            ui_required.append('port/android-native/app/src/main/cpp/'+module+'.cpp')
            if module != 'native_app':
                ui_required.append('port/android-native/app/src/main/cpp/'+module+'.hpp')
        ui_required.append('port/android-native/app/src/main/cpp/original_ui_asset_catalog.inc')
    projects, matches = {}, {}
    connected=None
    if args.connected_hud_stage:
        assert ui_stage,'Connected stage requires UI receipts'
        connected=json.loads(args.connected_hud_stage.read_bytes())
        assert connected['validation']=='PASS' and connected['renderer_migration']['exact_additive_getter_verified']
        for name in ('Swf_viewport_connection','HUD_sprite_timeline','HUD_advance','Gameswf_font_overlay','Player_status_HUD'):
            assert host['host_audits'][name]['validation']=='PASS'
        assert host['host_audits']['Player_status_HUD']['actual_property_changes_and_refill']
        required += ['port/engine-ui/'+name+'.cpp' for name in
                     ('swf_viewport_connection','hud_sprite_timeline','hud_sprite_core','hud_advance','hud_advance_owner','player_status_hud')]
        required.append('port/engine-ui/overlays/font-v1/gameswf_font.cpp')
    with zipfile.ZipFile(args.capture) as capture:
        manifest = json.loads(capture.read('build-capture.json'))
        if connected:
            receipt=manifest['connected_hud_stage']
            assert receipt['sha256']==sha(args.connected_hud_stage.read_bytes())==sha(capture.read(receipt['archive']))
            for key,digest in connected['source_sha256'].items():
                assert manifest['source_sha256'].get(key)==digest,('Connected source missing',key)
                assert sha((ROOT/key).read_bytes())==digest,('Connected source changed',key)
                if key in host['source_sha256']:assert host['source_sha256'][key]==digest
            ui_required += [key for key in connected['source_sha256'] if key.endswith(('.cpp','.hpp','.h','.inc'))]
        if ui_stage:
            captured_stages = manifest['ui_assets_stages']
            assert [(row['stage'],row['sha256']) for row in captured_stages] == [
                (row['stage'],row['sha256']) for row in ui_stage['stages']]
            for row in captured_stages:
                assert sha(capture.read(row['archive'])) == row['sha256']
            for name in ('CMakeLists.txt','gameswf_sources.cmake','freetype237-hud.cmake'):
                key = 'port/engine-ui/'+name
                assert manifest['source_sha256'][key] == sha((ROOT/key).read_bytes())
            # These modules are an explicitly separate proof stage; they need
            # not be present in the older main host report.
            additive = ['port/engine-ui/'+name+suffix for name in
                        ('freetype_bitmap_alpha','hud_freetype_font','swf_hud_freetype_provider')
                        for suffix in ('.cpp','.hpp')]
            if any(key in manifest['source_sha256'] for key in additive):
                path = args.ui_font_freeze or ROOT/'port/engine-ui/reference/hud-freetype-font/freeze-manifest.json'
                frozen = json.loads(path.read_bytes())
                assert frozen['validation'] == 'PASS'
                captured_font = manifest['ui_font_freeze']
                assert captured_font['sha256'] == sha(path.read_bytes()) == sha(capture.read(captured_font['archive']))
                mapping = frozen.get('source_sha256', frozen.get('source_and_evidence_sha256'))
                assert mapping, 'Missing additive font source freeze mapping'
                for key in additive:
                    assert mapping[key] == manifest['source_sha256'][key] == sha((ROOT/key).read_bytes())
                expected_proofs = {'port/engine-ui/reports/'+name for name in
                                  ('hud-freetype-font-host-audit.json','swf-hud-freetype-provider-host-audit.json',
                                   'freetype-bitmap-alpha-arm64-differential.json')}
                assert expected_proofs == captured_font['proofs'].keys()
                for key,record in captured_font['proofs'].items():
                    raw = capture.read(record['archive'])
                    assert sha(raw) == record['sha256'] == frozen['proof_sha256'][key]
                    proof = json.loads(raw)
                    assert proof['validation'] == 'PASS'
                    for source,digest in proof.get('source_sha256',{}).items():
                        if source in additive:
                            assert digest == mapping[source], 'Font report source binding changed'
                ui_required += additive
                ui_stage['additive_font_freeze'] = dict(path=str(path.resolve()),sha256=sha(path.read_bytes()),
                    source_sha256={key:mapping[key] for key in additive}, proof_sha256=frozen['proof_sha256'],
                    main93_host_coverage_claimed=False)
            elif args.ui_font_freeze:
                raise AssertionError('Font freeze supplied but additive modules did not compile')
        if stage:
            actual=manifest['source_sha256']['port/android-native/app/src/main/cpp/model_renderer.cpp']
            historical=(effects_stage or stage)['renderer_sha256']
            if connected:
                migration=connected['renderer_migration']
                assert migration['historical_sha256']==historical and migration['current_sha256']==actual
            else:assert actual==historical
        for name, record in manifest['entries'].items():
            raw = capture.read(name)
            assert len(raw) == record['bytes'] and sha(raw) == record['sha256']
        for name, digest in host['source_sha256'].items():
            key = name.replace('\\','/')
            if key in manifest['source_sha256']:
                # A new additive HUD provider may extend the UI CMake target.
                # Its dedicated freeze and compiler inputs are checked above;
                # the older main report does not prove the modified build file.
                if ui_stage and 'additive_font_freeze' in ui_stage and key == 'port/engine-ui/CMakeLists.txt' and manifest['source_sha256'][key] != digest:
                    ui_stage['historical_host_build_definition'] = dict(path=key,host_sha256=digest,
                        captured_sha256=manifest['source_sha256'][key],reason='Additive HUD-font integration; separate freeze')
                    continue
                assert manifest['source_sha256'][key] == digest, key
                matches[key] = digest
        assert all(name in matches for name in required)
        for tag in ('packaged','studio'):
            for abi in ('arm64-v8a','x86_64'):
                used = manifest['compiler_inputs'][tag][abi]['repository_inputs']
                assert all(name in used for name in required)
                if ui_stage:
                    assert all(used[key] == sha((ROOT/key).read_bytes()) for key in ui_required)
                    assert any(key.startswith('port/engine-ui/vendor/gameswf1714/') for key in used)
                    assert any(key.startswith('port/engine-ui/vendor/freetype-2.3.7-hud/') for key in used)
                if shader_stage:
                    for name in ('native_app.cpp','authored_shader_program.cpp','authored_shader_program.hpp'):
                        key = 'port/android-native/app/src/main/cpp/'+name
                        assert used[key] == sha((ROOT/key).read_bytes())
            raw = capture.read(tag+'-app-debug.apk')
            assert sha(raw) == manifest['apks'][tag]['sha256']
            with zipfile.ZipFile(io.BytesIO(raw)) as apk:
                libraries, exports = {}, {}
                for info in apk.infolist():
                    if not info.filename.startswith('lib/') or not info.filename.endswith('.so'):
                        continue
                    libraries[info.filename] = native_library(apk,info,raw)
                    elf = ELFFile(io.BytesIO(apk.read(info)))
                    exports[info.filename] = {symbol.name for symbol in elf.get_section_by_name('.dynsym').iter_symbols()
                                              if symbol['st_shndx'] != 'SHN_UNDEF'}
                assert len(libraries) == (18 if ui_stage else 16)
                if ui_stage:
                    assert {name.split('/')[1] for name in libraries} == {'arm64-v8a','x86_64'}
                    assert {name.split('/')[-1] for name in libraries if '/arm64-v8a/' in name} == {
                        name.split('/')[-1] for name in libraries if '/x86_64/' in name}
                assert not any('/armeabi' in name or 'DungeonHunter2' in name for name in libraries)
                for abi in ('arm64-v8a','x86_64'):
                    world = exports['lib/'+abi+'/libdh2_level_world.so']
                    runtime = exports['lib/'+abi+'/libdh2_script_runtime.so']
                    data = exports['lib/'+abi+'/libdh2_game_data.so']
                    if 'asset_sha256' in host['host_audits']:
                        scene = exports['lib/'+abi+'/libdh2_scene_materials.so']
                        assert any('sha256' in name for name in scene)
                    assert {'dh2_character_get_position','dh2_character_host_context_query',
                            'dh2_character_debug_load','dh2_character_debug_level_service',
                            'dh2_character_ai_set_target','dh2_character_set_level',
                            'dh2_character_state_owner_transition',
                            'dh2_character_state_owner_event',
                            'dh2_character_state_owner_behavior_bind',
                            'dh2_character_state_owner_frame',
                            'dh2_character_script_command',
                            'dh2_character_target_update',
                            'dh2_character_target_event',
                            'dh2_character_ai_event','dh2_character_dot_calculate_result',
                            'dh2_character_dot_apply','dh2_character_state_empty_body'} <= world
                    assert any('CharacterScriptSession' in name for name in world)
                    assert any('CharacterScriptObjects' in name for name in world)
                    assert stage_exports <= world
                    for audit,symbol in (('sneaking_tables','SneakingTables'),
                                         ('actor_initialization','load_actor_initialization'),
                                         ('monster_animation_skeleton','CharacterAnimationInstance'),
                                         ('NPC_body','CharacterNpcBodyModel'),
                                         ('Idle_events','CharacterIdleEvents'),
                                         ('Deferred_session','CharacterDeferredScript'),
                                         ('FX_tables','PreloadBacking')):
                        if audit in host['host_audits']:
                            assert any(symbol in name for name in world)
                    assert {'dh2_script_vm_set_source_objects','dh2_script_vm_bind_source_objects',
                            'dh2_script_constants_get','dh2_script_int_bind'} <= runtime
                    assert {'dh2_level_decode_record','dh2_fast_travel_decode_record'} <= data
                    if 'design_settings' in host['host_audits']:
                        assert {'dh2_design_settings_decode_record','dh2_design_settings_decode_table'} <= data
                    if 'skill_tables' in host['host_audits']:
                        assert {'dh2_skill_decode_record','dh2_skill_decode_list','dh2_skill_tables_measure'} <= data
                    if 'FX_tables' in host['host_audits']:
                        assert any('EffectsTables' in name for name in data)
                    if 'Material_color' in host['host_audits']:
                        animation = exports['lib/'+abi+'/libdh2_engine_animation.so']
                        assert {'dh2_material_alpha_key','dh2_material_alpha_between','dh2_material_alpha_delta',
                                'dh2_material_color_blend','dh2_material_color_set'} <= animation
                    if 'Particle_parameter' in host['host_audits']:
                        animation = exports['lib/'+abi+'/libdh2_engine_animation.so']
                        assert {'dh2_particle_parameter_key','dh2_particle_parameter_between',
                                'dh2_particle_parameter_delta_key','dh2_particle_parameter_delta_between',
                                'dh2_particle_parameter_blend','dh2_particle_parameter_apply'} <= animation
                    if 'Particle_factory' in host['host_audits']:
                        animation = exports['lib/'+abi+'/libdh2_engine_animation.so']
                        assert any('ParticleGenerationOwner' in name for name in animation)
                        assert any('decode_particle_emitter' in name for name in animation)
                        assert any('initialize_particle_generation' in name for name in animation)
                    if 'Particle_emission' in host['host_audits']:
                        animation = exports['lib/'+abi+'/libdh2_engine_animation.so']
                        assert any('ParticleEmissionOwner' in name for name in animation)
                        assert any('ParticleAnimationResource' in name for name in animation)
                    if 'Faery_tables' in host['host_audits']:
                        assert {'dh2_faery_decode_record','dh2_faery_decode_list','dh2_faery_tables_measure'} <= data
                    if 'Source_services' in host['host_audits']:
                        assert {'dh2_script_vm_call_source_status_objects','dh2_script_vm_required_failure_epoch'} <= runtime
                    if shader_stage:
                        app = exports['lib/'+abi+'/libdh2_native.so']
                        assert any('android_ui' in name and 'validate_pixels' in name for name in app)
                    if ui_stage:
                        ui = exports['lib/'+abi+'/libdh2_engine_ui.so']
                        assert {'dh2_gfnt_raster','dh2_freetype_glyph_layout','dh2_swf_font_resolve',
                                'dh2_ui_set_bounds','dh2_ui_set_viewport','dh2_ui_screen_to_logical',
                                'dh2_ui_logical_to_screen','dh2_ui_display_rectangle','dh2_ui_flash_camera_update'} <= ui
                        for symbol in ('SwfMovie','SwfFreetypeProvider','FreetypeFont','Localization'):
                            assert any(symbol in name for name in ui), symbol
                        if 'additive_font_freeze' in ui_stage:
                            assert 'dh2_freetype_bitmap_alpha' in ui
                            for symbol in ('HudFreetypeFont','SwfHudFreetypeProvider'):
                                assert any(symbol in name for name in ui), symbol
                        if connected:
                            assert {'dh2_ui_hud_player_values','dh2_ui_hud_sprite_goto_v1','dh2_ui_hud_sprite_play_v1','dh2_ui_hud_notify_v1'} <= ui
                            for symbol in ('PlayerStatusHud','SwfViewportConnection','HudAdvanceOwner','hud_goto','hud_bind'):
                                assert any(symbol in name for name in ui),symbol
                        app = exports['lib/'+abi+'/libdh2_native.so']
                        assert {'Java_com_example_dh2_NativeBridge_loadOriginalHealthPanel',
                                'Java_com_example_dh2_NativeBridge_consumeOriginalUiError'} <= app
                        for symbol in ('OriginalUiSession','OriginalUiAssets','SwfGpu'):
                            assert any(symbol in name for name in app), symbol
                        scene = exports['lib/'+abi+'/libdh2_scene_materials.so']
                        for symbol in ('swf_texture_filename','swf_texture_archive_key','swf_solid_color','swf_bitmap_color','swf_texture_gl_wrap'):
                            assert any(symbol in name for name in scene), symbol
                assert {name:sha(apk.read(name)) for name in apk.namelist() if name.startswith('assets/')} == assets
                projects[tag] = dict(apk=manifest['apks'][tag], native_libraries=libraries,
                                     assets=len(assets), required_exports_present=True)
    result = dict(validation='PASS', scope=__doc__, build_validation='PASS', live_validation='NOT_RUN',
        capture=dict(path=str(args.capture.resolve()),sha256=sha(args.capture.read_bytes())),
        host=dict(path=str(args.host.resolve()),sha256=sha(args.host.read_bytes())),
        host_compiler_source_matches=matches, asset_sha256=assets, projects=projects,
        original_ARM32_library_bundled=False, physical_arm64_verified=False,
        full_enemy_AI=False, full_game_verified=False, all_game_assets_bundled=False)
    if stage:
        result['monster_backings_stage_sha256'] = sha(args.monster_backings_stage.read_bytes())
    if effects_stage:
        result['effects_tables_stage_sha256'] = sha(args.effects_tables_stage.read_bytes())
    if shader_stage:
        result['shader_assets_stage_sha256'] = sha(args.shader_assets_stage.read_bytes())
        result['original_UI_display_list_verified'] = False
    if ui_stage:
        result['native_UI_stage'] = ui_stage
        result['native_UI_compiler_inputs'] = {name:manifest['source_sha256'][name] for name in sorted(ui_required)}
        result['native_UI_scope'] = 'Actual compiler/APK resource/export inspection; no GPU, live HUD or original full-core parity inferred'
    if connected:
        result['connected_hud_stage_sha256']=sha(args.connected_hud_stage.read_bytes())
        result['renderer_migration']=connected['renderer_migration']
    args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(dict(validation='PASS',live='NOT_RUN',assets=len(assets),
        projects={name:value['apk']['sha256'] for name,value in projects.items()})))


if __name__ == '__main__':
    main()
