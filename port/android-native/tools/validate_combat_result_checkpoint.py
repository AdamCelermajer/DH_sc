"""Validate packaged native hit/result calculation and fresh actor/world checks."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_world_checkpoint import digest,read,log,ROOT,REPO
from validate_combat_checkpoint import exports

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args();local=REPO/'.local-inputs';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libraries=inspect(apk);assert len(libraries)==14
 prior=read(ROOT/'reports/build-validation-combat.json');states=read(local/'actor-states-tests-combat-result/actor-states-smoke.json');world=read(local/'world-tests-combat-result/world-smoke.json');oracle_path=REPO/'port/game-data/reports/combat-result-arm64-differential.json';oracle=read(oracle_path);host=read(local/'combat-result-host-audit.json')
 assert states['apk_sha256']==world['apk_sha256']==world['installed_apk_sha256']==sha and len(states['cases'])==24 and len(world['cases'])==10
 for key in ('authored_animation_events_verified','base_class_snapshots_verified','resolved_property_snapshots_verified','spawn_vitals_verified','independent_idle_instances_verified','state5_attack_event_decisions_verified','frozen_attack_events_absent_verified','packaged_combat_result_probes_verified'):assert states[key],key
 assert states['combat_result_probe_count']==11 and states['original_combat_result_report_sha256']==digest(oracle_path) and not states['combat_execution_implemented']
 for key,path in [('original_class_report_sha256','game-data/reports/classes-regression-vitals.json'),('original_property_report_sha256','game-data/reports/properties-arm64-differential.json'),('original_vitals_report_sha256','game-data/reports/vitals-arm64-differential.json'),('original_event_report_sha256','engine-animation/reports/events-arm64-differential.json'),('original_combat_event_report_sha256','game-data/reports/combat-events-arm64-differential.json')]:assert states[key]==digest(REPO/'port'/path)
 assert len(states['three_stage_attacks'])==len(states['one_stage_deaths'])==3
 for row in states['three_stage_attacks']:
  assert len(row['animation_events'])==len(row['combat_actions'])==1;event=row['animation_events'][0];action=row['combat_actions'][0]
  assert event[1]=='attack_mainhand' and event[-1]==1 and action[:2]==event[:2] and action[2]==1 and action[3] in (0,1) and action[4:]==[0,0,0,-1]
 assert all(not row['animation_events'] and not row['combat_actions'] for row in states['one_stage_deaths'])
 assert len(world['movement'])==3 and all(world[k] for k in ('frozen_idle_stable','live_idle_changed_pixels','rotation_position_preserved','pause_cancels_movement'))
 assert oracle['comparisons']==9792 and oracle['melee_request_cases']==2000 and oracle['total_comparisons']==11792 and oracle['mismatches']==0 and oracle['all_ten_result_words_and_rng_state_compared']
 assert oracle['arm64_library_sha256']==digest(local/'combat-result-oracle.so') and oracle['original_sha256']==digest(local/'libDungeonHunter2.so') and oracle['native_host_reference_sha256']==digest(local/'combat-result-original-characters.bin') and oracle['spawn_reference_sha256']==digest(local/'vitals-original-spawn.bin')
 assert host['original_result_cases']==9792 and host['result_words_per_case']==10 and host['random_state_verified'] and host['invalid_input_atomic'] and host['outcomes_observed']==oracle['all_outcomes_observed'] and all(host['outcomes_observed'])
 assets=prior['assets_verified'];assert len(assets)==107;combat_exports={}
 with zipfile.ZipFile(apk) as archive:
  names={n.removeprefix('assets/') for n in archive.namelist() if n.startswith('assets/') and not n.endswith('/')};assert names==set(assets)
  for name,row in assets.items():
   raw=archive.read('assets/'+name);assert raw==(ROOT/'app/src/main/assets'/name).read_bytes()==(a.studio/'app/src/main/assets'/name).read_bytes();assert len(raw)==row['bytes'] and hashlib.sha256(raw).hexdigest()==row['sha256'],name
  required=set(prior['packaged_combat_exports_verified']['arm64-v8a'])|{'dh2_combat_result','dh2_combat_melee'}
  for abi in ('arm64-v8a','x86_64'):assert required<=exports(archive.read('lib/'+abi+'/libdh2_game_data.so'));combat_exports[abi]=sorted(required)
 source={}
 for name in prior['studio_source_sha256']:
  file=ROOT/'app/src/main'/name;assert digest(file)==digest(a.studio/'app/src/main'/name);source[name]=digest(file)
 changed={'game-data/CMakeLists.txt','game-data/tools/build_combat_oracle.ps1','android-native/tools/actor_states_smoke.py'}
 for name,before in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==before,name
 for folder in ('actor-states-tests-combat-result','world-tests-combat-result'):
  for file in (local/folder).glob('*.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|completion failed|event dispatch failed|combat route failed|Shader failed|Link failed',log(file)),file
 for name in ('combat-result-android-build.log','combat-result-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'combat-result-zipalign.log')
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(s for s in cache.splitlines() if s.startswith(prefix))
 modules=set(prior['module_source_sha256'])|{'game-data/combat_result.cpp','game-data/combat_result.hpp','game-data/tests/combat_result.cpp','game-data/tests/combat_result_differential.py','android-native/tools/validate_combat_result_checkpoint.py','level-world/tools/inspect_virtuals.py'}
 modules.update(str(f.relative_to(REPO/'port')).replace('\\','/') for f in (REPO/'port/game-data/reference/combat-result').rglob('*') if f.is_file())
 validation={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(a.studio/'app/build/outputs/apk/debug/app-debug.apk'),'source_sync_verified':True,'studio_source_sha256':source,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(modules)},'libraries':libraries,'apk_16k_zip_alignment_verified':True,'assets_verified':assets,'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'packaged_combat_exports_verified':combat_exports,'combat_result_asan_ubsan':host,'combat_result_original_arm64_comparisons':{'result_cases':9792,'melee_request_cases':2000,'total':11792,'arm64_library_sha256':oracle['arm64_library_sha256'],'report_sha256':digest(oracle_path)},'actor_state_cases':24,'world_cases':10,'native_android_result_probes_verified':11,'prior_combat_checkpoint_apk_sha256':prior['apk_sha256'],'prior_unchanged_helper_evidence':{k:prior[k] for k in ('combat_asan_ubsan','vitals_host_regression','damage_original_arm64_comparisons','attack_event_original_arm64_comparisons','prior_unchanged_animation_host_checks')},'full_hit_and_status_result_calculation_implemented':True,'melee_request_construction_implemented':True,'alternate_melee_buff_removal_implemented':False,'combat_result_application_implemented':False,'target_selection_or_full_ai_implemented':False,'combat_execution_implemented':False,'physical_arm64_tested':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 for path,result in ((ROOT/'reports/build-validation-combat-result.json',validation),(ROOT/'reports/actor-states-smoke-combat-result.json',states),(ROOT/'reports/world-smoke-combat-result.json',world),(REPO/'port/game-data/reports/combat-result-host-audit.json',host)):
  path.parent.mkdir(parents=True,exist_ok=True);path.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-combat-result-{sha[:8]}.apk';checkpoint.parent.mkdir(parents=True,exist_ok=True);checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'assets':len(assets),'emulator_cases':34,'original_comparisons':11792,'goal_status':'active'}))
if __name__=='__main__':main()
