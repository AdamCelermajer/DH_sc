"""Bind independent actor scheduling to its APK, source, inputs and fresh checks."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_world_checkpoint import digest,read,log,ROOT,REPO

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args()
 local=REPO/'.local-inputs';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libraries=inspect(apk);assert len(libraries)==14
 states=read(local/'actor-states-tests-recovered-completion/actor-states-smoke.json');objects=read(local/'objects-tests-completion/objects-smoke.json');world=read(local/'world-tests-completion/world-smoke.json')
 assert states['apk_sha256']==objects['apk_sha256']==world['apk_sha256']==sha
 assert (len(states['cases']),len(objects['cases']),len(world['cases']))==(24,22,10)
 assert len(states['three_stage_attacks'])==len(states['one_stage_deaths'])==3 and states['independent_idle_instances_verified']
 assert all(len(row['events'])==3 and row['events'][-1][1]==0 for row in states['three_stage_attacks'])
 assert all(len(row['events'])==1 and row['events'][-1][1]==0 for row in states['one_stage_deaths'])
 assert all(objects[k] for k in ('frozen_stable','live_animation_changes','rotation_reloads_objects','resume_reloads_objects'))
 assert all(world[k] for k in ('frozen_idle_stable','live_idle_changed_pixels','rotation_position_preserved','pause_cancels_movement')) and len(world['movement'])==3
 source={}
 for name in ('cpp/CMakeLists.txt','cpp/native_app.cpp','cpp/model_renderer.cpp','cpp/model_renderer.hpp','java/com/example/dh2/MainActivity.java','java/com/example/dh2/NativeBridge.java'):
  file=ROOT/'app/src/main'/name;assert digest(file)==digest(a.studio/'app/src/main'/name),name;source[name]=digest(file)
 old=read(ROOT/'reports/build-validation-states.json');assets=dict(old['assets_verified']);assets_root=ROOT/'app/src/main/assets';provenance=read(assets_root/'actor-state-provenance.json')
 assert provenance['cache_sha256']==old['cache_sha256'] and len(provenance['inputs'])==26 and len(provenance['states'])==12
 for row in provenance['inputs']:
  name=row['asset'];expected={'bytes':row['bytes'],'sha256':row['sha256']}
  if name in assets:assert assets[name]==expected,name
  assets[name]=expected
 file=assets_root/'actor-state-provenance.json';assets['actor-state-provenance.json']={'bytes':file.stat().st_size,'sha256':digest(file)}
 with zipfile.ZipFile(apk) as archive:
  actual={n.removeprefix('assets/') for n in archive.namelist() if n.startswith('assets/') and not n.endswith('/')};assert actual==set(assets)
  for name,row in assets.items():
   raw=archive.read('assets/'+name);assert raw==(assets_root/name).read_bytes()==(a.studio/'app/src/main/assets'/name).read_bytes(),name;assert len(raw)==row['bytes'] and hashlib.sha256(raw).hexdigest()==row['sha256'],name
 assert len(assets)==102
 base=read(assets_root/'texture-provenance.json');level=read(assets_root/'worlds/crypt01-provenance.json');actor=read(assets_root/'actor-provenance.json');animation=read(assets_root/'animation-provenance.json')
 entries={row['archive_entry'] for row in base['samples']}|{row['entry'] for row in level['inputs']}|{row['entry'] for row in actor['inputs']}|{row['entry'] for row in animation['inputs']}|{row['entry'] for row in provenance['inputs']};assert len(entries)==94
 scheduler=read(local/'completion-scheduler-audit.json');clips=read(local/'completion-clips-audit.json');differential=read(REPO/'port/game-data/reports/completion-arm64-differential.json')
 assert scheduler['original_starts']==759 and scheduler['completion_calls']==6966 and scheduler['three_monster_attacks_completed']==9 and scheduler['recursive_redirect_rejected_atomically']
 assert scheduler['death_completes'] and scheduler['idle_reselection_calls']==200 and scheduler['finite_sequence_callbacks']==9 and scheduler['nested_parent_callbacks']==4
 assert clips['clip_count']==26 and clips['every_millisecond_poses']==45218 and clips['finite_skinned_vertices'] and clips['unsupported_tracks']==0
 assert differential['bit_exact_decision_step_and_loop_cases']==3000 and differential['arm64_library_sha256']==digest(local/'completion-oracle.so')
 assert differential['original_sha256']==digest(local/'libDungeonHunter2.so')
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(s for s in cache.splitlines() if s.startswith(prefix))
 for name in ('completion-android-build-final.log','completion-studio-build-final.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'completion-zipalign-final.log')
 for folder in ('actor-states-tests-recovered-completion','objects-tests-completion','world-tests-completion'):
  for path in (local/folder).glob('*.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|completion failed|Shader failed|Link failed',log(path)),path
 lifecycle=log(local/'objects-tests-completion/objects-lifecycle.log')
 for model,count in (('skeleton.bdae',10),('slime_green_v2.bdae',10),('ghost.bdae',6)):assert f'Actor clip bank ready | {model} | clips {count} | independent instance clocks' in lifecycle
 assert 'WATCHDOG KILLING SYSTEM PROCESS' in log(local/'completion-stall-system2.log')
 modules=set(old['module_source_sha256'])|{'game-data/animation_scheduler.hpp','game-data/animation_scheduler.cpp','game-data/CMakeLists.txt','level-world/CMakeLists.txt','level-world/objects.hpp','level-world/objects.cpp','game-data/tests/animation_scheduler.cpp','level-world/tests/actor_clips.cpp','level-world/tools/prepare_actor_states.py'}
 validation={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(a.studio/'app/build/outputs/apk/debug/app-debug.apk'),'source_sync_verified':True,'studio_source_sha256':source,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(modules)},'libraries':libraries,'apk_16k_zip_alignment_verified':True,'assets_verified':assets,'original_input_count':len(entries),'cache_sha256':old['cache_sha256'],'scheduler_asan_ubsan':scheduler,'actor_clips_asan_ubsan':clips,'completion_original_arm64_comparisons':differential,'actor_state_cases':24,'object_cases':22,'world_cases':10,'prior_state_checkpoint_apk_sha256':old['apk_sha256'],'independent_monster_clocks':True,'decor_resource_clock_shared':True,'original_completion_decision_loops_redirect_unwind_implemented':True,'development_rng_seed':1,'frame_timing_adapter_original_parity_verified':False,'blending_or_event_root_motion_dispatch_implemented':False,'combat_or_ai_implemented':False,'physical_arm64_tested':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active','emulator_test_recovery':'First fresh-launch actor suite stopped during an API 37 WindowManager system_server watchdog. Cause unestablished. Cold boot followed by single-top commands with lifecycle intent handoff; final suites use this APK.'}
 for path,result in ((ROOT/'reports/build-validation-completion.json',validation),(ROOT/'reports/actor-states-smoke.json',states),(ROOT/'reports/objects-smoke-completion.json',objects),(ROOT/'reports/world-smoke-completion.json',world),(REPO/'port/game-data/reports/animation-scheduler-host-audit.json',scheduler),(REPO/'port/level-world/reports/actor-clips-host-audit.json',clips),(REPO/'port/level-world/reports/actor-state-input-provenance.json',provenance)):
  path.parent.mkdir(parents=True,exist_ok=True);path.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-completion-{sha[:8]}.apk';checkpoint.parent.mkdir(parents=True,exist_ok=True);checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'assets':len(assets),'original_inputs':len(entries),'emulator_cases':56,'goal_status':'active'}))
if __name__=='__main__':main()
