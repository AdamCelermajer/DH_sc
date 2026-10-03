"""Bind the touch-driven Prince combat checkpoint to original and live evidence."""
import argparse, hashlib, json, re, subprocess, zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_world_checkpoint import digest, read, log, ROOT, REPO
from validate_combat_checkpoint import exports

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args()
 local=REPO/'.local-inputs';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libraries=inspect(apk);assert len(libraries)==14
 prior=read(ROOT/'reports/build-validation-combat-application.json')
 states=read(local/'actor-states-tests-player-combat/actor-states-smoke.json');world=read(local/'world-tests-player-combat/world-smoke.json');monster=read(local/'combat-application-tests-player-combat/combat-application-smoke.json');live=read(local/'player-combat-tests/player-combat-smoke.json')
 assert states['apk_sha256']==world['apk_sha256']==world['installed_apk_sha256']==monster['apk_sha256']==monster['installed_apk_sha256']==live['apk_sha256']==live['installed_apk_sha256']==sha
 assert len(states['cases'])==24 and len(world['cases'])==10 and monster['native_hits_verified']==18 and len(monster['pairs'])==3
 for key in ('authored_animation_events_verified','base_class_snapshots_verified','resolved_property_snapshots_verified','spawn_vitals_verified','independent_idle_instances_verified','state5_attack_event_decisions_verified','frozen_attack_events_absent_verified','packaged_combat_result_probes_verified','packaged_health_probes_verified','combat_execution_implemented','live_combat_health_application_implemented'):assert states[key],key
 assert states['health_probe_count']==22 and states['combat_result_probe_count']==11 and not states['full_combat_or_ai_implemented']
 assert len(states['three_stage_attacks'])==len(states['one_stage_deaths'])==3
 for row in states['three_stage_attacks']:
  assert len(row['animation_events'])==len(row['combat_actions'])==1;event=row['animation_events'][0];action=row['combat_actions'][0];assert event[1]=='attack_mainhand' and event[-1]==1 and action[:2]==event[:2] and action[2]==1 and action[3] in (0,1,2) and action[4:]==[0,0,0,-1]
 assert all(not row['animation_events'] and not row['combat_actions'] for row in states['one_stage_deaths'])
 for key,path in [('original_class_report_sha256','game-data/reports/classes-regression-vitals.json'),('original_property_report_sha256','game-data/reports/properties-arm64-differential.json'),('original_vitals_report_sha256','game-data/reports/vitals-arm64-differential.json'),('original_event_report_sha256','engine-animation/reports/events-arm64-differential.json'),('original_combat_event_report_sha256','game-data/reports/combat-events-application-arm64-differential.json'),('original_combat_result_report_sha256','game-data/reports/combat-result-arm64-differential.json'),('original_health_report_sha256','game-data/reports/health-arm64-differential.json')]:assert states[key]==digest(REPO/'port'/path)
 assert len(world['movement'])==3 and all(world[k] for k in ('frozen_idle_stable','live_idle_changed_pixels','rotation_position_preserved','pause_cancels_movement'))
 assert world['floor_geometry_sha256']==digest(local/'world-floor.json') and not world['original_pf_parity_verified'] and not world['original_gpu_parity_verified']
 oracle_path=REPO/'port/game-data/reports/player-application-arm64-differential.json';oracle=read(oracle_path);host=read(local/'player-application-host-audit.json');poses=read(local/'player-combat-host-audit.json');regression=read(local/'player-monster-application-regression.json')
 assert oracle['player_attacker'] and oracle['comparisons']==2732 and oracle['synthetic_cases']==1500 and oracle['source_cases']==896 and oracle['mismatches']==0 and oracle['all_owner_sheets_result_actor_states_and_requests_compared'] and oracle['original_service_request_order_verified']
 for key,name in [('arm64_library_sha256','player-application-oracle.so'),('reference_sha256','player-application-original.bin'),('original_sha256','libDungeonHunter2.so'),('spawn_reference_sha256','vitals-original-spawn.bin')]:assert oracle[key]==digest(local/name)
 assert host['original_application_cases']==2732 and host['owner_words_per_case']==1792 and host['result_and_actor_state_words']==20 and host['application_words']==25 and host['invalid_input_atomic']
 assert host['kill_requests']==oracle['counts']['kills']==oracle['core_kill_death_events'] and host['hit_calls']==oracle['counts']['hits'] and host['status_requests']==oracle['counts']['statuses']
 assert regression==prior['combat_application_asan_ubsan']
 assert poses['player_row']==263 and poses['original_owner_words_compared']==896 and poses['hp_raw']==42265 and poses['mp_raw']==6976 and poses['animation_table']==48 and poses['warrior_skins']==4 and poses['every_millisecond_poses']==3475 and poses['melee_event_tracks']==3 and poses['finite_skinned_vertices'] and len(poses['clips'])==9
 assert live['original_player_application_report_sha256']==digest(oracle_path) and live['native_player_attempts_verified']==112 and len(live['attempts'])==112
 for key in ('out_of_reach_rejected','frozen_attack_no_hit','busy_attack_does_not_restart','original_player_sheet_checksum_verified'):assert live[key],key
 assert [r['orientation'] for r in live['rotation_does_not_replay_hits']]==[1,0] and all(r['attempts']==3 for r in live['rotation_does_not_replay_hits']) and live['inflight_pause_resume'] is not None and live['automatic_original_death_clip']==1185
 reference=next(r for r in oracle['live_sequences'] if r['defender']=='Crypt_Skeleton');assert len(reference['attacks'])==112
 signed=lambda v:v if v<0x80000000 else v-0x100000000
 for number,(actual,expected) in enumerate(zip(live['attempts'],reference['attacks']),1):
  words=expected['application_words'];assert actual['attempt']==number and actual['result']==expected['result_after'] and actual['random_after']==expected['random_after'] and actual['hp_before']==signed(words[1]) and actual['hp_after']==signed(words[2]) and actual['combo']==expected['state_after'][2] and actual['dead']==expected['state_after'][5] and actual['status_requests']==words[16]
 assert live['attempts'][-1]['hp_after']==0 and live['attempts'][-1]['dead']==1
 monster_path=REPO/'port/game-data/reports/combat-application-arm64-differential.json';monster_oracle=read(monster_path);assert digest(monster_path)==prior['combat_application_original_arm64_comparisons']['report_sha256'] and monster['original_application_report_sha256']==digest(monster_path)
 assert all(monster[k] for k in ('frozen_attacks_do_not_hit','dead_target_rejection_verified','automatic_original_death_clips_verified'))
 assert [r['orientation'] for r in monster['rotation_preserves_hp_and_does_not_replay_hits']]==[1,0] and monster['inflight_pause_resume_does_not_replay_hits']==[{'case':'inflight_attack_pause_resume','hits':2}]
 for pair,reference in zip(monster['pairs'],monster_oracle['live_sequences']):
  assert pair['frozen_no_hit'] and pair['dead_target_rejected'] and len(pair['attacks'])==len(reference['attacks'])==6 and pair['death_clip'] in (1251,700,1185)
  for actual,expected in zip(pair['attacks'],reference['attacks']):
   words=expected['application_words'];assert actual['result']==expected['result_after'] and actual['random_after']==expected['random_after'] and actual['hp_before']==words[1] and actual['hp_after']==words[2] and actual['combo']==expected['state_after'][2] and actual['dead']==expected['state_after'][5] and actual['status_requests']==words[16]
 assets=dict(prior['assets_verified']);assert len(assets)==107
 provenance_path=ROOT/'app/src/main/assets/player-combat-provenance.json';provenance=read(provenance_path);assert digest(a.cache)==provenance['cache_sha256']==prior['cache_sha256']
 assert provenance['character']=='KnightPlayerBase' and provenance['row']==263 and provenance['class_id']==77 and provenance['animation_table']==48 and provenance['root_sequence']==243 and len(provenance['inputs'])==9
 assert sorted(r['clip_id'] for r in provenance['inputs'])==sorted(r['id'] for r in poses['clips'])
 with zipfile.ZipFile(a.cache) as cache:
  for row in provenance['inputs']:
   raw=cache.read(row['entry']);assert len(raw)==row['bytes'] and hashlib.sha256(raw).hexdigest()==row['sha256'];assets[row['asset']]={'bytes':row['bytes'],'sha256':row['sha256']}
 assets['player-combat-provenance.json']={'bytes':provenance_path.stat().st_size,'sha256':digest(provenance_path)};assert len(assets)==117
 required=set(prior['packaged_combat_exports_verified']['arm64-v8a'])|{'dh2_combat_apply_player_to_monster'};packaged={}
 with zipfile.ZipFile(apk) as archive:
  names={n.removeprefix('assets/') for n in archive.namelist() if n.startswith('assets/') and not n.endswith('/')};assert names==set(assets)
  for name,row in assets.items():
   raw=archive.read('assets/'+name);assert raw==(ROOT/'app/src/main/assets'/name).read_bytes()==(a.studio/'app/src/main/assets'/name).read_bytes();assert len(raw)==row['bytes'] and hashlib.sha256(raw).hexdigest()==row['sha256'],name
  for abi in ('arm64-v8a','x86_64'):assert required<=exports(archive.read('lib/'+abi+'/libdh2_game_data.so'));packaged[abi]=sorted(required)
 source={}
 for name in prior['studio_source_sha256']:
  file=ROOT/'app/src/main'/name;assert digest(file)==digest(a.studio/'app/src/main'/name);source[name]=digest(file)
 changed={'game-data/CMakeLists.txt','game-data/combat_application.cpp','game-data/combat_application.hpp','game-data/tests/combat_application.cpp','game-data/tests/combat_application_differential.py','level-world/CMakeLists.txt'}
 for name,before in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==before,name
 for folder in ('actor-states-tests-player-combat','world-tests-player-combat','combat-application-tests-player-combat','player-combat-tests'):
  for file in (local/folder).glob('*.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|completion failed|event dispatch failed|combat route failed|probe failed|application failed|death animation failed|Shader failed|Link failed',log(file)),file
 for name in ('player-combat-android-build.log','player-combat-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'player-combat-zipalign.log')
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(s for s in cache.splitlines() if s.startswith(prefix))
 modules=set(prior['module_source_sha256'])|changed|{'level-world/tests/player_combat.cpp','level-world/tools/prepare_player_combat.py','android-native/tools/player_combat_smoke.py','android-native/tools/validate_player_combat_checkpoint.py'}
 modules.update(str(f.relative_to(REPO/'port')).replace('\\','/') for f in (REPO/'port/game-data/reference/player-initialization').rglob('*') if f.is_file())
 validation={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(a.studio/'app/build/outputs/apk/debug/app-debug.apk'),'source_sync_verified':True,'studio_source_sha256':source,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(modules)},'libraries':libraries,'apk_16k_zip_alignment_verified':True,'assets_verified':assets,'original_input_count':prior['original_input_count']+9,'cache_sha256':prior['cache_sha256'],'packaged_combat_exports_verified':packaged,'player_application_asan_ubsan':host,'player_pose_asan_ubsan':poses,'monster_application_host_regression':regression,'player_application_original_arm64_comparisons':{'cases':2732,'arm64_library_sha256':oracle['arm64_library_sha256'],'report_sha256':digest(oracle_path),'reference_sha256':oracle['reference_sha256']},'actor_state_cases':24,'world_cases':10,'native_player_attempts_verified':112,'native_monster_hits_verified':18,'native_automatic_deaths_verified':4,'native_android_health_probes_verified':22,'native_android_result_probes_verified':11,'prior_combat_application_checkpoint_apk_sha256':prior['apk_sha256'],'prior_unchanged_helper_evidence':prior['prior_unchanged_helper_evidence'],'prior_combat_result_comparisons':prior['prior_combat_result_comparisons'],'expanded_event_original_comparisons':prior['expanded_event_original_comparisons'],'player_application_scope':oracle['scope'],'player_input_scope':live['input_policy'],'player_base_preset':'KnightPlayerBase fallback; save/class/equipment creation producers pending','player_combat_implemented':True,'original_death_clip_adapter_implemented':True,'full_ai_fsm_or_kill_rewards_implemented':False,'aggro_or_status_services_implemented':False,'physical_arm64_tested':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 for path,result in ((ROOT/'reports/build-validation-player-combat.json',validation),(ROOT/'reports/actor-states-smoke-player-combat.json',states),(ROOT/'reports/world-smoke-player-combat.json',world),(ROOT/'reports/combat-application-smoke-player-combat.json',monster),(ROOT/'reports/player-combat-smoke.json',live),(REPO/'port/game-data/reports/player-monster-application-host-regression.json',regression)):
  path.parent.mkdir(parents=True,exist_ok=True);path.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-player-combat-{sha[:8]}.apk';checkpoint.parent.mkdir(parents=True,exist_ok=True);checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'assets':117,'original_inputs':107,'baseline_emulator_cases':34,'native_player_attempts':112,'native_monster_hits':18,'automatic_deaths':4,'original_player_application_comparisons':2732,'goal_status':'active'}))

if __name__=='__main__':main()
