"""Bind the event checkpoint to native source, original assets and fresh tests."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_world_checkpoint import digest,read,log,ROOT,REPO

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args();local=REPO/'.local-inputs';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libraries=inspect(apk);assert len(libraries)==14
 prior=read(ROOT/'reports/build-validation-vitals.json');states=read(local/'actor-states-tests-events/actor-states-smoke.json');world=read(local/'world-tests-events/world-smoke.json');oracle_path=REPO/'port/engine-animation/reports/events-arm64-differential.json';oracle=read(oracle_path);host=read(local/'events-host-audit.json');poses=read(local/'actor-clips-host-events.json')
 assert states['apk_sha256']==world['apk_sha256']==world['installed_apk_sha256']==sha
 assert len(states['cases'])==24 and len(world['cases'])==10
 assert all(states[k] for k in ('authored_animation_events_verified','base_class_snapshots_verified','resolved_property_snapshots_verified','spawn_vitals_verified','independent_idle_instances_verified'))
 assert states['actor_animation_event_tracks']==5 and states['original_event_report_sha256']==digest(oracle_path)
 for key,path in [('original_class_report_sha256','game-data/reports/classes-regression-vitals.json'),('original_property_report_sha256','game-data/reports/properties-arm64-differential.json'),('original_vitals_report_sha256','game-data/reports/vitals-arm64-differential.json')]:assert states[key]==digest(REPO/'port'/path)
 assert len(states['three_stage_attacks'])==len(states['one_stage_deaths'])==3
 assert all(len(row['animation_events'])==1 and row['animation_events'][0][1]=='attack_mainhand' and row['animation_events'][0][-1]==1 for row in states['three_stage_attacks'])
 assert all(not row['animation_events'] for row in states['one_stage_deaths'])
 assert len(world['movement'])==3 and all(world[k] for k in ('frozen_idle_stable','live_idle_changed_pixels','rotation_position_preserved','pause_cancels_movement'))
 assert oracle['comparisons']==30775 and oracle['callback_events_compared']==109462 and oracle['mismatches']==0 and len(oracle['authored_tracks'])==5 and oracle['synthetic_tracks']==105
 assert oracle['arm64_library_sha256']==digest(local/'events-oracle.so') and oracle['original_sha256']==digest(local/'libDungeonHunter2.so')
 assert host=={'original_bdae_files':38,'authored_tracks':5,'authored_groups':5,'callback_events':5,'mutations':5000,'move_ownership_verified':True,'invalid_input_rejected':True}
 assert poses['clip_count']==26 and poses['every_millisecond_poses']==45218 and poses['unsupported_tracks']==0 and poses['finite_skinned_vertices']
 assets=prior['assets_verified'];assert len(assets)==107
 with zipfile.ZipFile(apk) as archive:
  names={n.removeprefix('assets/') for n in archive.namelist() if n.startswith('assets/') and not n.endswith('/')};assert names==set(assets)
  for name,row in assets.items():
   raw=archive.read('assets/'+name);assert raw==(ROOT/'app/src/main/assets'/name).read_bytes()==(a.studio/'app/src/main/assets'/name).read_bytes();assert len(raw)==row['bytes'] and hashlib.sha256(raw).hexdigest()==row['sha256'],name
  for row in oracle['authored_tracks']:assert hashlib.sha256(archive.read('assets/actors/'+row['file'])).hexdigest()==row['sha256']
 source={}
 for name in prior['studio_source_sha256']:
  file=ROOT/'app/src/main'/name;assert digest(file)==digest(a.studio/'app/src/main'/name);source[name]=digest(file)
 changed={'engine-animation/animation.cpp','engine-animation/animation.hpp','android-native/tools/actor_states_smoke.py'}
 for name,sha_before in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==sha_before,name
 for folder in ('actor-states-tests-events','world-tests-events'):
  for file in (local/folder).glob('*.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|completion failed|event dispatch failed|Shader failed|Link failed',log(file)),file
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(s for s in cache.splitlines() if s.startswith(prefix))
 for name in ('events-android-build.log','events-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'events-zipalign.log')
 modules=set(prior['module_source_sha256'])|{'engine-animation/CMakeLists.txt','engine-animation/events.hpp','engine-animation/events.cpp','engine-animation/event_track.cpp','engine-animation/tests/events_audit.cpp','engine-animation/tests/events_differential.py','engine-animation/tools/build_events_oracle.ps1','engine-animation/tools/inspect_events.py','engine-animation/reference/events/original-functions.json','engine-animation/reference/events/dispatch.asm','engine-animation/reference/events/binding.asm','engine-animation/reference/events/lookup.asm','level-world/tools/find_calls.py','android-native/tools/validate_events_checkpoint.py'}
 validation={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(a.studio/'app/build/outputs/apk/debug/app-debug.apk'),'source_sync_verified':True,'studio_source_sha256':source,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(modules)},'libraries':libraries,'apk_16k_zip_alignment_verified':True,'assets_verified':assets,'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'events_asan_ubsan':host,'actor_clip_host_regression':poses,'events_original_arm64_comparisons':{'comparisons':oracle['comparisons'],'callback_events_compared':oracle['callback_events_compared'],'arm64_library_sha256':oracle['arm64_library_sha256'],'report_sha256':digest(oracle_path)},'actor_state_cases':24,'world_cases':10,'prior_vitals_checkpoint_apk_sha256':prior['apk_sha256'],'spawn_vitals_instances_verified':11,'authored_animation_events_implemented':True,'per_actor_event_cursor_implemented':True,'complete_original_animator_timeline_implemented':False,'callback_mutation_or_reentrancy_verified':False,'event_to_state_machine_mapping_implemented':False,'attack_damage_ai_or_regeneration_scheduling_implemented':False,'physical_arm64_tested':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 for path,result in ((ROOT/'reports/build-validation-events.json',validation),(ROOT/'reports/actor-states-smoke-events.json',states),(ROOT/'reports/world-smoke-events.json',world),(REPO/'port/engine-animation/reports/events-host-audit.json',host),(REPO/'port/level-world/reports/actor-clips-host-events.json',poses)):
  path.parent.mkdir(parents=True,exist_ok=True);path.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-events-{sha[:8]}.apk';checkpoint.parent.mkdir(parents=True,exist_ok=True);checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'assets':len(assets),'original_inputs':validation['original_input_count'],'emulator_cases':34,'original_comparisons':oracle['comparisons'],'goal_status':'active'}))
if __name__=='__main__':main()
