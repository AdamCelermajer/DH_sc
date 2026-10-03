"""Verify normal class recalculation and spawn HP/MP in the native checkpoint."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_world_checkpoint import digest,read,log,ROOT,REPO

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args();local=REPO/'.local-inputs';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libraries=inspect(apk);assert len(libraries)==14
 states=read(local/'actor-states-tests-vitals/actor-states-smoke.json');world=read(local/'world-tests-vitals/world-smoke.json');oracle_path=REPO/'port/game-data/reports/vitals-arm64-differential.json';oracle=read(oracle_path);class_path=REPO/'port/game-data/reports/classes-regression-vitals.json';cached=read(class_path);host=read(local/'vitals-host-audit.json');prior=read(ROOT/'reports/build-validation-properties.json')
 assert states['apk_sha256']==world['apk_sha256']==world['installed_apk_sha256']==sha
 assert len(states['cases'])==24 and len(world['cases'])==10 and states['base_class_snapshots_verified'] and states['base_class_instance_count']==11
 assert states['resolved_property_snapshots_verified'] and states['resolved_property_instance_count']==11 and states['original_property_report_sha256']==digest(REPO/'port/game-data/reports/properties-arm64-differential.json')
 assert states['spawn_vitals_verified'] and states['spawn_vitals_instance_count']==11 and states['original_vitals_report_sha256']==digest(oracle_path) and states['original_class_report_sha256']==digest(class_path)
 assert states['independent_idle_instances_verified'] and len(states['three_stage_attacks'])==len(states['one_stage_deaths'])==3
 assert all(world[k] for k in ('frozen_idle_stable','live_idle_changed_pixels','rotation_position_preserved','pause_cancels_movement')) and len(world['movement'])==3
 assert host['uncached_original_character_owner_states']==448 and host['two_pass_spawn_original_owner_states']==448 and host['properties_compared_per_owner']==896
 assert (host['crypt_hp_raw'],host['crypt_mp_raw'],host['crypt_first_hp_add_raw'],host['crypt_second_hp_add_raw'])==(12160,5632,12160,1)
 assert all(host[k] for k in ('positive_and_fill_regen_verified','saved_level_influences_uncached_class','recursive_failure_atomic','invalid_view_atomic'))
 assert oracle['uncached_class_entire_owner_state_cases']==2080 and oracle['regeneration_entire_owner_state_cases']==10000 and oracle['full_original_character_class_recalc_cases']==448 and oracle['original_character_two_pass_spawn_vitals_cases']==448 and oracle['original_stat_functions_unmocked']
 assert oracle['positive_regeneration_cases']==4803 and oracle['original_regen_debug_switch_query_blocks_skipped']==6115
 assert oracle['arm64_library_sha256']==cached['arm64_library_sha256']==digest(local/'vitals-oracle.so') and oracle['original_sha256']==digest(local/'libDungeonHunter2.so')
 assert cached['bit_exact_entire_property_sheet_cases']==3120 and cached['original_character_sheet_cases']==448 and cached['buff_snapshot_cases']==1560
 for name in ('uncached','spawn'):
  file=local/f'vitals-original-{name}.bin';assert file.stat().st_size==oracle['references'][name]['bytes']==448*3584 and digest(file)==oracle['references'][name]['sha256']
 regressions={'classes':read(local/'classes-host-regression-vitals.json'),'properties':read(local/'properties-host-regression-vitals.json')};assert regressions['classes']['native_roundtrip_matches_original'] and regressions['classes']['mutations_and_truncations']==3000 and regressions['properties']['original_character_rows_resolved']==448 and regressions['properties']['invalid_input_atomic']
 assets=prior['assets_verified'];base=ROOT/'app/src/main/assets';assert len(assets)==107
 with zipfile.ZipFile(apk) as archive:
  actual={n.removeprefix('assets/') for n in archive.namelist() if n.startswith('assets/') and not n.endswith('/')};assert actual==set(assets)
  for name,row in assets.items():
   raw=archive.read('assets/'+name);assert raw==(base/name).read_bytes()==(a.studio/'app/src/main/assets'/name).read_bytes(),name;assert len(raw)==row['bytes'] and hashlib.sha256(raw).hexdigest()==row['sha256'],name
 for name,expected in oracle['input_sha256'].items():assert assets['data/'+name]['sha256']==expected,name
 source={}
 for name in ('cpp/CMakeLists.txt','cpp/native_app.cpp','cpp/model_renderer.cpp','cpp/model_renderer.hpp','java/com/example/dh2/MainActivity.java','java/com/example/dh2/NativeBridge.java'):
  file=ROOT/'app/src/main'/name;assert digest(file)==digest(a.studio/'app/src/main'/name),name;source[name]=digest(file)
 for folder in ('actor-states-tests-vitals','world-tests-vitals'):
  for file in (local/folder).glob('*.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|completion failed|Shader failed|Link failed',log(file)),file
 world_log=log(local/'world-tests-vitals/world-lifecycle.log');assert len(re.findall(r'Spawn vitals \| .*? \| passes 2',world_log))>=11
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(s for s in cache.splitlines() if s.startswith(prefix))
 for name in ('vitals-android-build.log','vitals-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'vitals-zipalign.log')
 modules=set(prior['module_source_sha256'])|{'game-data/vitals.hpp','game-data/vitals.cpp','game-data/tests/vitals.cpp','game-data/tests/vitals_differential.py','game-data/tools/build_vitals_oracle.ps1','game-data/tools/build_class_oracle.ps1','game-data/reference/vitals/original-functions.json','game-data/reference/vitals/reference/original-functions.asm','android-native/tools/validate_vitals_checkpoint.py'}
 validation={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(a.studio/'app/build/outputs/apk/debug/app-debug.apk'),'source_sync_verified':True,'studio_source_sha256':source,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(modules)},'libraries':libraries,'apk_16k_zip_alignment_verified':True,'assets_verified':assets,'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'vitals_asan_ubsan':host,'class_properties_host_regression':regressions,'vitals_original_arm64_comparisons':{'uncached_classes':2080,'regeneration':10000,'character_recalculation':448,'character_spawn_vitals':448,'arm64_library_sha256':oracle['arm64_library_sha256'],'report_sha256':digest(oracle_path),'discarded_debug_switch_blocks_omitted':True},'cached_class_original_arm64_regression':{'class_cases':3120,'character_cases':448,'report_sha256':digest(class_path)},'actor_state_cases':24,'world_cases':10,'prior_property_checkpoint_apk_sha256':prior['apk_sha256'],'resolved_property_instances_verified':11,'spawn_vitals_instances_verified':11,'normal_uncached_base_class_recalculation_implemented':True,'two_pass_spawn_hp_mp_sequence_implemented':True,'full_original_initpost_revive_or_actor_lifecycle_implemented':False,'gear_buff_producers_implemented':False,'combat_ai_or_regeneration_scheduling_implemented':False,'physical_arm64_tested':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 for path,result in ((ROOT/'reports/build-validation-vitals.json',validation),(ROOT/'reports/actor-states-smoke-vitals.json',states),(ROOT/'reports/world-smoke-vitals.json',world),(REPO/'port/game-data/reports/vitals-host-audit.json',host),(REPO/'port/game-data/reports/classes-host-regression-vitals.json',regressions['classes']),(REPO/'port/game-data/reports/properties-host-regression-vitals.json',regressions['properties'])):
  path.parent.mkdir(parents=True,exist_ok=True);path.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-vitals-{sha[:8]}.apk';checkpoint.parent.mkdir(parents=True,exist_ok=True);checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'assets':len(assets),'original_inputs':validation['original_input_count'],'emulator_cases':34,'spawn_vitals_instances':11,'goal_status':'active'}))
if __name__=='__main__':main()
