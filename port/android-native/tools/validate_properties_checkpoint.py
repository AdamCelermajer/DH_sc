"""Bind original property resolution, native sanitizer checks and fresh APK tests."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_world_checkpoint import digest,read,log,ROOT,REPO

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args();local=REPO/'.local-inputs';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libraries=inspect(apk);assert len(libraries)==14
 states=read(local/'actor-states-tests-properties/actor-states-smoke.json');world=read(local/'world-tests-properties/world-smoke.json');oracle_path=REPO/'port/game-data/reports/properties-arm64-differential.json';oracle=read(oracle_path);host=read(local/'properties-host-audit.json');prior=read(ROOT/'reports/build-validation-classes.json')
 assert states['apk_sha256']==world['apk_sha256']==world['installed_apk_sha256']==sha
 assert len(states['cases'])==24 and len(world['cases'])==10 and states['base_class_snapshots_verified'] and states['base_class_instance_count']==11
 assert states['resolved_property_snapshots_verified'] and states['resolved_property_instance_count']==11 and states['original_property_report_sha256']==digest(oracle_path)
 assert states['original_class_report_sha256']==digest(REPO/'port/game-data/reports/classes-arm64-differential.json')
 assert states['independent_idle_instances_verified'] and len(states['three_stage_attacks'])==len(states['one_stage_deaths'])==3
 assert all(world[k] for k in ('frozen_idle_stable','live_idle_changed_pixels','rotation_position_preserved','pause_cancels_movement')) and len(world['movement'])==3
 assert host['original_character_rows_resolved']==448 and host['properties_per_sheet']==224 and all(host[k] for k in ('original_default_type_rows_verified','default_collision_and_wrapping_verified','buff_group_and_insertion_precedence_verified','fixed_point_health_mutations_verified','runtime_only_recalc_preserved','invalid_input_atomic'))
 assert oracle['bit_exact_property_resolutions']==119168 and oracle['bit_exact_mutation_entire_state_cases']==7168 and oracle['original_character_rows_resolved']==448 and oracle['original_property_functions_and_container_traversal_unmocked']
 assert oracle['buff_group_counts_tested']==[0,1,3,6] and oracle['buff_deque_sizes_tested']==[0,1,2,5,32,40]
 assert oracle['arm64_library_sha256']==digest(local/'properties-oracle.so') and oracle['original_sha256']==digest(local/'libDungeonHunter2.so')
 assert oracle['original_character_resolved_reference_sha256']==digest(local/'properties-original-rows.bin')
 assets=prior['assets_verified'];base=ROOT/'app/src/main/assets';assert len(assets)==107
 with zipfile.ZipFile(apk) as archive:
  actual={n.removeprefix('assets/') for n in archive.namelist() if n.startswith('assets/') and not n.endswith('/')};assert actual==set(assets)
  for name,row in assets.items():
   raw=archive.read('assets/'+name);assert raw==(base/name).read_bytes()==(a.studio/'app/src/main/assets'/name).read_bytes(),name;assert len(raw)==row['bytes'] and hashlib.sha256(raw).hexdigest()==row['sha256'],name
 for name,expected in oracle['input_sha256'].items():assert assets['data/'+name]['sha256']==expected,name
 rules=read(REPO/'port/game-data/reference/properties/property-rules.json');assert rules['original_sha256']==oracle['original_sha256'] and rules['input_sha256']==oracle['input_sha256'] and len(rules['rows'])==224 and rules['default_row_index']==0 and rules['type_row_index']==1
 source={}
 for name in ('cpp/CMakeLists.txt','cpp/native_app.cpp','cpp/model_renderer.cpp','cpp/model_renderer.hpp','java/com/example/dh2/MainActivity.java','java/com/example/dh2/NativeBridge.java'):
  file=ROOT/'app/src/main'/name;assert digest(file)==digest(a.studio/'app/src/main'/name),name;source[name]=digest(file)
 for folder in ('actor-states-tests-properties','world-tests-properties'):
  for file in (local/folder).glob('*.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|completion failed|Shader failed|Link failed',log(file)),file
 world_log=log(local/'world-tests-properties/world-lifecycle.log');assert 'Property rules ready | defaults row 0 | types row 1 | properties 224 | supplied sheets only' in world_log
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(s for s in cache.splitlines() if s.startswith(prefix))
 for name in ('properties-android-build.log','properties-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'properties-zipalign.log')
 modules=set(prior['module_source_sha256'])|{'game-data/properties.hpp','game-data/properties.cpp','game-data/tests/properties.cpp','game-data/tests/properties_differential.py','game-data/tools/inspect_property_rules.py','game-data/tools/build_properties_oracle.ps1','game-data/reference/properties/original-functions.json','game-data/reference/properties/property-rules.json','game-data/reference/properties/reference/original-functions.asm','android-native/tools/actor_states_smoke.py','android-native/tools/world_smoke.py','android-native/tools/validate_properties_checkpoint.py'}
 validation={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(a.studio/'app/build/outputs/apk/debug/app-debug.apk'),'source_sync_verified':True,'studio_source_sha256':source,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(modules)},'libraries':libraries,'apk_16k_zip_alignment_verified':True,'assets_verified':assets,'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'properties_asan_ubsan':host,'property_original_arm64_comparisons':{'resolutions':119168,'mutations':7168,'raw_character_sheets':448,'arm64_library_sha256':oracle['arm64_library_sha256'],'report_sha256':digest(oracle_path)},'actor_state_cases':24,'world_cases':10,'prior_class_checkpoint_apk_sha256':prior['apk_sha256'],'cached_base_class_instances_verified':11,'resolved_property_instances_verified':11,'supplied_sheet_and_ordered_buff_resolution_implemented':True,'gear_buff_producers_or_uncached_class_resolution_implemented':False,'spawn_health_initialization_implemented':False,'combat_or_ai_implemented':False,'physical_arm64_tested':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 for path,result in ((ROOT/'reports/build-validation-properties.json',validation),(ROOT/'reports/actor-states-smoke-properties.json',states),(ROOT/'reports/world-smoke-properties.json',world),(REPO/'port/game-data/reports/properties-host-audit.json',host)):
  path.parent.mkdir(parents=True,exist_ok=True);path.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-properties-{sha[:8]}.apk';checkpoint.parent.mkdir(parents=True,exist_ok=True);checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'assets':len(assets),'original_inputs':validation['original_input_count'],'emulator_cases':34,'native_property_instances':11,'goal_status':'active'}))
if __name__=='__main__':main()
