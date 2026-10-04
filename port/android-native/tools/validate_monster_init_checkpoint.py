"""Bind the five core emulator suites and optional UI shader proof to one APK."""
import argparse
from datetime import datetime, timezone
import hashlib
import json
from pathlib import Path
import zipfile

ROOT = Path(__file__).resolve().parents[3]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def record(path):
    return dict(path=path.resolve().relative_to(ROOT).as_posix(), sha256=sha(path))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('apk', 'capture', 'inspection', 'host', 'output'):
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--scene-objects',action='store_true',help='Bind live scene identity/finalizer source stage')
    parser.add_argument('--player-object',action='store_true',help='Bind actual retained player scene backing')
    parser.add_argument('--owned-target-stage',action='store_true',help='Bind owned settings and live object GetPosition')
    parser.add_argument('--prepared-monster-backings',action='store_true',help='Bind retained CPU animation owners; no live original FSM claim')
    parser.add_argument('--effects-tables-stage',type=Path,help='Bind exact bundled effects tables and retained owners')
    parser.add_argument('--shader-assets-stage',type=Path,help='Bind unmodified shader pack and members')
    parser.add_argument('--shader-proof',type=Path,help='Bind original shader GPU/readback and texture scaling proof')
    parser.add_argument('--ui-assets-stage',type=Path,nargs=3,metavar=('MENU','TEXT','TEXTURE'))
    parser.add_argument('--hud-proof',type=Path,help='Bind authored initial health-panel GPU/lifecycle proof')
    parser.add_argument('--render-proof',type=Path,help='Explicit render proof for a preserved retry after a transport interruption')
    parser.add_argument('--combat-proof',type=Path,help='Explicit combat proof for a preserved bounded real-touch retry')
    parser.add_argument('--bank-proof',type=Path,help='Explicit animation-bank proof for a preserved bounded real-touch retry')
    args = parser.parse_args()
    assert not args.output.exists(), 'Preserve existing checkpoint proof'
    digest = sha(args.apk)
    tag = digest[:8]
    inspection = json.loads(args.inspection.read_text())
    host = json.loads(args.host.read_text())
    assert inspection['validation'] == host['validation'] == 'PASS'
    assert inspection['capture']['sha256'] == sha(args.capture)
    assert inspection['host']['sha256'] == sha(args.host)
    assert host['sanitizer_findings'] == 0 and host['actual_main_CMake_targets']
    assert host['host_audits']['Lua_GC']['checks'] == (454 if args.scene_objects else 451)
    if args.scene_objects:
        assert host['host_audits']['scene_script_objects']['finalizer_target_clear']
        assert host['host_audits']['scene_script_objects']['checks'] == 448
    if args.player_object:
        assert args.scene_objects
        assert host['host_audits']['player_scene_objects']['checks'] == 453
        assert host['host_audits']['player_scene_objects']['original_Lua_player_target']
    if args.owned_target_stage:
        expected_suites = (56+sum(name in host['host_audits'] for name in ('Idle_update','NPC_body','Idle_events','FX_preload','FX_tables','Init_FX','Can_update','Material_color','Particle_parameter','Update_startup','Deferred_script','Deferred_session','Delayed_Idle','Init_vitals','Init_vitals_session','Delayed_Idle_vitals','Deferred_queue','Particle_factory','Source_services','Required_deferred','Update_queued','Update_queued_session','Particle_emission','Faery_tables','Skills','Skills_session'))) if args.prepared_monster_backings else 42
        if args.ui_assets_stage:
            expected_suites += sum(name in host['host_audits'] for name in
                ('GFNT','Shader_sources','Swf_texture','Swf_movie','Freetype_font','Freetype_HUD',
                 'Swf_font_resolver','Swf_font_HUD','UI_viewport','Localization','Localization_connection',
                 'HUD_freetype_font','HUD_freetype_provider','Swf_glyph_lookup','HUD_player_values'))
        assert args.player_object and len(host['host_audits']) == expected_suites
        assert host['host_audits']['object_position']['retained_position_finalizer']
        assert host['host_audits']['owned_target_pipeline']['owned_authored_threat_bits'] == 0x41200000
        assert host['host_audits']['spawn_body']['original_cases'] == 5458
        assert host['host_audits']['kill']['synchronous_reentry_gold_cases'] == 22
    if args.prepared_monster_backings:
        assert args.owned_target_stage and inspection['projects']['packaged']['assets'] == (770 if args.ui_assets_stage else (347 if args.effects_tables_stage else 342) + (35 if args.shader_assets_stage else 0))
        assert host['host_audits']['asset_sha256']['hashlib_fixture_comparisons'] == 1104
        assert host['host_audits']['AI_state_changed']['cases'] == 1578
    with zipfile.ZipFile(args.capture) as archive:
        manifest = json.loads(archive.read('build-capture.json'))
        assert manifest['apks']['packaged']['sha256'] == digest
        assert hashlib.sha256(archive.read('packaged-app-debug.apk')).hexdigest() == digest
    suites = []
    paths = [('render', 'monster-init-smoke.json'), ('movement', 'live-actor-smoke.json'),
             ('lifecycle', 'live-actor-lifecycle-smoke.json'), ('bank', 'prince-bank-smoke.json'),
             ('combat', 'character-combat-smoke.json')]
    for name, filename in paths:
        folder = (('player-scene-live-' if name=='render' else 'player-scene-live-'+name+'-')
                  if args.player_object else ('scene-objects-live-' if name=='render' else 'scene-objects-live-'+name+'-')
                  if args.scene_objects else ('monster-init-render-live-' if name == 'render'
                  else 'monster-init-live-'+name+'-'))+tag
        path = ROOT/'.local-inputs'/folder/filename
        if name == 'bank' and args.bank_proof:
            path = args.bank_proof
        if name == 'render' and args.render_proof:
            path = args.render_proof
        if name == 'combat' and args.combat_proof:
            path = args.combat_proof
        proof = json.loads(path.read_text())
        assert proof['validation'] == 'PASS'
        assert proof['apk_sha256'] == proof['installed_apk_sha256'] == digest
        suites.append(dict(**record(path), validation='PASS', apk_sha256=digest,
                           installed_apk_sha256=digest))
    if args.shader_assets_stage:
        assert args.shader_proof and inspection['shader_assets_stage_sha256'] == sha(args.shader_assets_stage)
        shader = json.loads(args.shader_proof.read_text())
        assert shader['validation'] == 'PASS' and shader['apk_sha256'] == shader['installed_apk_sha256'] == digest
        assert shader['shader_stage_sha256'] == sha(args.shader_assets_stage)
        assert len(shader['cases']) == 7 and shader['shader_pixel_contract_cases_per_context'] == 8
        assert {case['rotation'] for case in shader['cases']} == {0,1}
        for case in shader['cases']:
            assert case['gpu_contracts'] and all(programs == 2 and count == 8 and error <= 2 for programs,count,error in case['gpu_contracts'])
            assert sha(args.shader_proof.parent/case['screenshot']) == case['screenshot_sha256']
        suites.append(dict(**record(args.shader_proof),validation='PASS',apk_sha256=digest,installed_apk_sha256=digest))
    if args.ui_assets_stage:
        assert args.shader_assets_stage and args.hud_proof
        stage = inspection['native_UI_stage']
        assert stage['total_assets'] == 770 and stage['unique_original_resources'] == 385
        assert [row['sha256'] for row in stage['stages']] == [sha(path) for path in args.ui_assets_stage]
        hud = json.loads(args.hud_proof.read_text())
        assert hud['validation'] == 'PASS' and hud['apk_sha256'] == hud['installed_apk_sha256'] == digest
        assert hud['authored_initial_health_panel'] and not hud['live_game_values']
        assert not hud['original_viewport_publication'] and not hud['HUD_input']
        assert [case['label'] for case in hud['cases']] == ['portrait','landscape','resumed']
        assert len({tuple(case['owner']) for case in hud['cases']}) == 1
        for case in hud['cases']:
            assert case['panel_pixels'] > 1000 and case['unique_colors'] > 128
            assert sha(args.hud_proof.parent/case['screenshot']) == case['screenshot_sha256']
        suites.append(dict(**record(args.hud_proof),validation='PASS',apk_sha256=digest,installed_apk_sha256=digest))
    else:
        assert not args.hud_proof
    monster_path = ROOT/suites[0]['path']
    monster = json.loads(monster_path.read_text())
    assert len(monster['initial']) == 11 and monster['initial'] == monster['restored']
    if args.scene_objects:
        assert monster['live_scene_object_bindings']
        assert len(monster['scene_objects_initial']) == 11
        assert monster['scene_objects_initial'] == monster['scene_objects_restored']
    if args.player_object:
        assert monster['live_player_scene_backing']
        assert monster['player_object_initial']['identity'] == '0000000100000001'
        for key in ('identity','CharAI','properties','life','hp','dead','checksum'):
            assert monster['player_object_initial'][key] == monster['player_object_restored'][key]
    if args.owned_target_stage:
        assert monster['live_owned_design_settings'] and monster['live_original_object_GetPosition']
        assert monster['design_settings_initial'] == monster['design_settings_restored']
        assert len(monster['object_positions_initial']) == 11
        assert monster['object_positions_initial'] == monster['object_positions_restored']
    if args.prepared_monster_backings:
        assert monster['retained_monster_CPU_resources'] and monster['independently_hashed_initialization']
        assert len(monster['prepared_backings_initial']) == 11
        assert monster['prepared_backings_initial'] == monster['prepared_backings_restored']
        assert monster['live_monster_native_FSM'] is False
        assert set(monster['rendered_viewports']) == {'portrait','landscape'}
        assert all(row['pixels'] > row['area']*.2 for row in monster['rendered_viewports'].values())
    if args.effects_tables_stage:
        assert monster['retained_effects_native_backing'] and monster['FX_factory_ready'] is False
        assert monster['effects_backing_initial'] == monster['effects_backing_restored']
        assert inspection['effects_tables_stage_sha256'] == monster['effects_tables_stage_sha256'] == sha(args.effects_tables_stage)
    visual = []
    for name in ('portrait.png', 'landscape.png'):
        path = monster_path.parent/name
        assert sha(path) == monster['screenshots'][name]
        visual.append(dict(**record(path), observed='Textured Crypt and player rendered; prototype controls'))
    result = dict(validation='PASS', recorded_at_utc=datetime.now(timezone.utc).isoformat(),
        scope=__doc__, apk=record(args.apk), apk_bytes=args.apk.stat().st_size,
        source_build_capture=dict(**record(args.capture), compiled_input_count=len(manifest['source_sha256'])),
        build_inspection=record(args.inspection), main_linked_host_audit=record(args.host),
        emulator=dict(serial='emulator-5554', api=37, abi='x86_64'), suites=suites,
        original_monster_Init_sessions=11, sessions_retained_after_rotation=11,
        visual_inspection=visual, physical_arm64_verified=False,
        full_game_verified=False, full_enemy_ai_verified=False,
        all_game_assets_bundled=False, source_Application_session_selection=False)
    if args.scene_objects:
        result.update(live_scene_object_bindings=True,stable_CharAI_records_after_rotation=11,
            live_supported_source_registrations_per_monster=32,
            source_host_scoped_finalizer_target_clear_verified=True)
    if args.player_object:
        result.update(live_player_scene_backing=True,stable_native_scene_records=12,
            original_host_Lua_player_target_verified=True)
    if args.owned_target_stage:
        result.update(live_owned_design_settings=True,live_original_object_GetPosition=True,
            full_spawn_and_Kill_backends_verified=False)
    if args.prepared_monster_backings:
        result.update(retained_monster_CPU_resources=11,packaged_prototype_assets=770 if args.ui_assets_stage else (347 if args.effects_tables_stage else 342)+(35 if args.shader_assets_stage else 0),
            independently_hashed_initialization=True,live_monster_native_FSM=False)
    if args.effects_tables_stage:
        result.update(retained_effects_native_backing=True,FX_factory_ready=False,
                      effects_tables_stage=record(args.effects_tables_stage))
    if args.shader_assets_stage:
        result.update(original_shader_sources_bundled=True,authored_UI_shader_GPU_contract_verified=True,
                      original_UI_display_list_verified=False,shader_assets_stage=record(args.shader_assets_stage))
    if args.ui_assets_stage:
        result.update(original_health_panel_GPU_submission_verified=True,
                      retained_original_UI_owner_after_rotation_resume=True,
                      authored_initial_empty_bar_state=True,live_HUD_values_verified=False,
                      original_viewport_publication_verified=False,original_HUD_input_verified=False,
                      UI_assets_stages=[record(path) for path in args.ui_assets_stage])
    args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps(dict(validation='PASS', apk_sha256=digest, emulator_suites=len(suites),
                         original_monster_Init_sessions=11)))


if __name__ == '__main__':
    main()
