"""Bind the damage/event-decision checkpoint to source and fresh emulator tests.

This checkpoint does not execute hits or mutate target health. Historical event
reader/pose checks are reused only while their source hashes remain unchanged.
"""
import argparse,hashlib,json,re,struct,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_world_checkpoint import digest,read,log,ROOT,REPO

def exports(raw):
 # ELF64 little-endian defined dynamic symbols, independent of build outputs.
 assert raw[:6]==b'\x7fELF\x02\x01'
 offset=struct.unpack_from('<Q',raw,40)[0];stride,count=struct.unpack_from('<HH',raw,58)
 sections=[struct.unpack_from('<IIQQQQIIQQ',raw,offset+i*stride) for i in range(count)];result=set()
 for section in sections:
  if section[1]!=11:continue
  strings=sections[section[6]];names=raw[strings[4]:strings[4]+strings[5]]
  for cursor in range(section[4],section[4]+section[5],section[9]):
   name,info,other,index,value,size=struct.unpack_from('<IBBHQQ',raw,cursor)
   if index and value and info>>4 in (1,2):result.add(names[name:names.index(b'\0',name)].decode('ascii'))
 return result

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args();local=REPO/'.local-inputs';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libraries=inspect(apk);assert len(libraries)==14
 prior=read(ROOT/'reports/build-validation-events.json');states=read(local/'actor-states-tests-combat/actor-states-smoke.json');world=read(local/'world-tests-combat/world-smoke.json');damage_path=REPO/'port/game-data/reports/combat-arm64-differential.json';route_path=REPO/'port/game-data/reports/combat-events-arm64-differential.json';damage=read(damage_path);route=read(route_path);host=read(local/'combat-host-audit.json');vitals=read(local/'vitals-host-combat.json')
 assert states['apk_sha256']==world['apk_sha256']==world['installed_apk_sha256']==sha
 assert len(states['cases'])==24 and len(world['cases'])==10
 for key in ('authored_animation_events_verified','base_class_snapshots_verified','resolved_property_snapshots_verified','spawn_vitals_verified','independent_idle_instances_verified','state5_attack_event_decisions_verified','frozen_attack_events_absent_verified'):assert states[key],key
 assert states['original_combat_event_report_sha256']==digest(route_path) and not states['combat_execution_implemented']
 for key,path in [('original_class_report_sha256','game-data/reports/classes-regression-vitals.json'),('original_property_report_sha256','game-data/reports/properties-arm64-differential.json'),('original_vitals_report_sha256','game-data/reports/vitals-arm64-differential.json'),('original_event_report_sha256','engine-animation/reports/events-arm64-differential.json')]:assert states[key]==digest(REPO/'port'/path)
 assert len(states['three_stage_attacks'])==len(states['one_stage_deaths'])==3
 for row in states['three_stage_attacks']:
  assert len(row['animation_events'])==len(row['combat_actions'])==1
  event=row['animation_events'][0];action=row['combat_actions'][0]
  assert event[1]=='attack_mainhand' and event[-1]==1 and action[:2]==event[:2]
  assert action[2]==1 and action[3] in (0,1) and action[4:]==[0,0,0,-1]
 assert all(not row['animation_events'] and not row['combat_actions'] for row in states['one_stage_deaths'])
 assert len(world['movement'])==3 and all(world[k] for k in ('frozen_idle_stable','live_idle_changed_pixels','rotation_position_preserved','pause_cancels_movement'))
 assert damage['comparisons']==23792 and damage['source_character_damage_cases']==1792 and damage['mismatches']==0 and damage['all_damage_words_and_rng_state_compared']
 assert route['attack_event_decisions_compared']==6000 and route['mismatches']==0 and len(route['runtime_cases'])==16
 for report in (damage,route):assert report['arm64_library_sha256']==digest(local/'combat-oracle.so') and report['original_sha256']==digest(local/'libDungeonHunter2.so')
 assert damage['spawn_reference_sha256']==digest(local/'vitals-original-spawn.bin')
 assert damage['native_host_reference_sha256']==digest(local/'combat-original-characters.bin') and route['native_host_reference_sha256']==digest(local/'combat-original-events.bin')
 assert host['original_character_damage_cases']==1792 and host['original_attack_event_decisions']==6000 and all(host[k] for k in ('all_damage_words_and_rng_verified','invalid_input_atomic','zero_range_and_counter_wrap_verified')) and len(host['unarmed_crypt_projectile_properties'])==4 and set(host['unarmed_crypt_projectile_properties'].values())=={-1}
 assert vitals['uncached_original_character_owner_states']==vitals['two_pass_spawn_original_owner_states']==448 and vitals['properties_compared_per_owner']==896
 assets=prior['assets_verified'];assert len(assets)==107
 combat_exports={}
 with zipfile.ZipFile(apk) as archive:
  names={n.removeprefix('assets/') for n in archive.namelist() if n.startswith('assets/') and not n.endswith('/')};assert names==set(assets)
  for name,row in assets.items():
   raw=archive.read('assets/'+name);assert raw==(ROOT/'app/src/main/assets'/name).read_bytes()==(a.studio/'app/src/main/assets'/name).read_bytes();assert len(raw)==row['bytes'] and hashlib.sha256(raw).hexdigest()==row['sha256'],name
  required={'dh2_combat_random','dh2_combat_bonus','dh2_combat_dot','dh2_combat_damage','dh2_combat_event_route'}
  for abi in ('arm64-v8a','x86_64'):
   defined=exports(archive.read('lib/'+abi+'/libdh2_game_data.so'));assert required<=defined;combat_exports[abi]=sorted(required)
 source={}
 for name in prior['studio_source_sha256']:
  file=ROOT/'app/src/main'/name;assert digest(file)==digest(a.studio/'app/src/main'/name);source[name]=digest(file)
 changed={'game-data/CMakeLists.txt','android-native/tools/actor_states_smoke.py'}
 for name,sha_before in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==sha_before,name
 for folder in ('actor-states-tests-combat','world-tests-combat'):
  for file in (local/folder).glob('*.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|completion failed|event dispatch failed|combat route failed|Shader failed|Link failed',log(file)),file
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(s for s in cache.splitlines() if s.startswith(prefix))
 for name in ('combat-android-build.log','combat-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'combat-zipalign.log')
 modules=set(prior['module_source_sha256'])|{'game-data/combat.cpp','game-data/combat.hpp','game-data/combat_events.cpp','game-data/combat_events.hpp','game-data/tests/combat.cpp','game-data/tests/combat_differential.py','game-data/tests/combat_events_differential.py','game-data/tools/build_combat_oracle.ps1','level-world/tools/inspect_literals.py','android-native/tools/validate_combat_checkpoint.py'}
 for folder in ('combat','combat-events'):
  modules.update(str(f.relative_to(REPO/'port')).replace('\\','/') for f in (REPO/'port/game-data/reference'/folder).rglob('*') if f.is_file())
 validation={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(a.studio/'app/build/outputs/apk/debug/app-debug.apk'),'source_sync_verified':True,'studio_source_sha256':source,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(modules)},'libraries':libraries,'apk_16k_zip_alignment_verified':True,'assets_verified':assets,'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'combat_asan_ubsan':host,'vitals_host_regression':vitals,'damage_original_arm64_comparisons':{'comparisons':damage['comparisons'],'arm64_library_sha256':damage['arm64_library_sha256'],'report_sha256':digest(damage_path)},'attack_event_original_arm64_comparisons':{'comparisons':route['attack_event_decisions_compared'],'arm64_library_sha256':route['arm64_library_sha256'],'report_sha256':digest(route_path)},'actor_state_cases':24,'world_cases':10,'prior_events_checkpoint_apk_sha256':prior['apk_sha256'],'prior_unchanged_animation_host_checks':{'events_asan_ubsan':prior['events_asan_ubsan'],'actor_clip_host_regression':prior['actor_clip_host_regression']},'spawn_vitals_instances_verified':11,'state5_attack_event_decisions_implemented':True,'damage_dot_bonus_and_random_helpers_implemented':True,'full_event_to_state_machine_mapping_implemented':False,'hit_rolls_or_result_application_implemented':False,'combat_execution_implemented':False,'equipped_range_capability_implemented':False,'complete_original_animator_timeline_implemented':False,'physical_arm64_tested':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 validation['packaged_combat_exports_verified']=combat_exports
 for path,result in ((ROOT/'reports/build-validation-combat.json',validation),(ROOT/'reports/actor-states-smoke-combat.json',states),(ROOT/'reports/world-smoke-combat.json',world),(REPO/'port/game-data/reports/combat-host-audit.json',host),(REPO/'port/game-data/reports/vitals-host-combat.json',vitals)):
  path.parent.mkdir(parents=True,exist_ok=True);path.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-combat-{sha[:8]}.apk';checkpoint.parent.mkdir(parents=True,exist_ok=True);checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'assets':len(assets),'original_inputs':validation['original_input_count'],'emulator_cases':34,'original_comparisons':damage['comparisons']+route['attack_event_decisions_compared'],'goal_status':'active'}))
if __name__=='__main__':main()
