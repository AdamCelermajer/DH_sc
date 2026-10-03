"""Validate native class reconstruction and Android base snapshots."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_world_checkpoint import digest,read,log,ROOT,REPO
def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args();local=REPO/'.local-inputs';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libraries=inspect(apk);assert len(libraries)==14
 states=read(local/'actor-states-tests-classes/actor-states-smoke.json');world=read(local/'world-tests-classes/world-smoke.json');oracle_path=REPO/'port/game-data/reports/classes-arm64-differential.json';oracle=read(oracle_path);host=read(local/'classes-host-audit.json')
 assert states['apk_sha256']==world['apk_sha256']==sha
 assert len(states['cases'])==24 and len(world['cases'])==10 and states['base_class_snapshots_verified'] and states['base_class_instance_count']==11 and states['original_class_report_sha256']==digest(oracle_path)
 assert states['independent_idle_instances_verified'] and len(states['three_stage_attacks'])==len(states['one_stage_deaths'])==3
 assert all(world[k] for k in ('frozen_idle_stable','live_idle_changed_pixels','rotation_position_preserved','pause_cancels_movement')) and len(world['movement'])==3
 assert host['native_roundtrip_matches_original'] and host['classes']==260 and host['formulas']==1659 and host['serialized_bytes']==34224 and host['class_level_applications']==1040 and host['character_row_applications']==448 and host['mutations_and_truncations']==3000 and host['recursive_failure_atomic']
 assert oracle['bit_exact_entire_property_sheet_cases']==3120 and oracle['original_character_sheet_cases']==448 and oracle['buff_snapshot_cases']==1560 and oracle['all_224_original_offsets_verified_identity'] and oracle['unmocked_original_getters_setters_group_dispatch']
 assert oracle['arm64_library_sha256']==digest(local/'classes-oracle.so') and oracle['original_sha256']==digest(local/'libDungeonHunter2.so')
 old=read(ROOT/'reports/build-validation-completion.json');assets=dict(old['assets_verified']);base=ROOT/'app/src/main/assets';provenance=read(base/'class-provenance.json');assert provenance['cache_sha256']==old['cache_sha256'] and len(provenance['inputs'])==4
 for row in provenance['inputs']:assets[row['asset']]={'bytes':row['bytes'],'sha256':row['sha256']}
 file=base/'class-provenance.json';assets['class-provenance.json']={'bytes':file.stat().st_size,'sha256':digest(file)};assert len(assets)==107
 with zipfile.ZipFile(apk) as archive:
  actual={n.removeprefix('assets/') for n in archive.namelist() if n.startswith('assets/') and not n.endswith('/')};assert actual==set(assets)
  for name,row in assets.items():
   raw=archive.read('assets/'+name);assert raw==(base/name).read_bytes()==(a.studio/'app/src/main/assets'/name).read_bytes(),name;assert len(raw)==row['bytes'] and hashlib.sha256(raw).hexdigest()==row['sha256'],name
 for name,expected in oracle['input_sha256'].items():assert assets['data/'+name]['sha256']==expected,name
 assert len({r['entry'] for r in provenance['inputs']})==4
 entries={r['archive_entry'] for r in read(base/'texture-provenance.json')['samples']}
 for name in ('worlds/crypt01-provenance.json','actor-provenance.json','animation-provenance.json','actor-state-provenance.json','class-provenance.json'):entries|={r['entry'] for r in read(base/name)['inputs']}
 assert len(entries)==98
 source={}
 for name in ('cpp/CMakeLists.txt','cpp/native_app.cpp','cpp/model_renderer.cpp','cpp/model_renderer.hpp','java/com/example/dh2/MainActivity.java','java/com/example/dh2/NativeBridge.java'):
  file=ROOT/'app/src/main'/name;assert digest(file)==digest(a.studio/'app/src/main'/name),name;source[name]=digest(file)
 for folder in ('actor-states-tests-classes','world-tests-classes'):
  for file in (local/folder).glob('*.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|completion failed|Shader failed|Link failed',log(file)),file
 world_log=log(local/'world-tests-classes/world-lifecycle.log');assert 'Class tables ready | classes 260 | bytes 34224 | cached base snapshots' in world_log
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(s for s in cache.splitlines() if s.startswith(prefix))
 for name in ('classes-android-build.log','classes-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'classes-zipalign.log')
 modules=set(old['module_source_sha256'])|{'game-data/class_tables.hpp','game-data/class_tables.cpp','game-data/CMakeLists.txt','game-data/tests/classes.cpp','game-data/tests/classes_differential.py','game-data/tools/inspect_class_tables.py','game-data/tools/inspect_property_layout.py','level-world/tools/prepare_class_tables.py'}
 validation={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(a.studio/'app/build/outputs/apk/debug/app-debug.apk'),'source_sync_verified':True,'studio_source_sha256':source,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(modules)},'libraries':libraries,'apk_16k_zip_alignment_verified':True,'assets_verified':assets,'original_input_count':old['original_input_count']+4,'cache_sha256':old['cache_sha256'],'classes_asan_ubsan':host,'class_original_arm64_comparisons':{'class_cases':3120,'original_character_cases':448,'buff_cases':1560,'arm64_library_sha256':oracle['arm64_library_sha256'],'report_sha256':digest(oracle_path)},'actor_state_cases':24,'world_cases':10,'prior_completion_checkpoint_apk_sha256':old['apk_sha256'],'cached_base_class_instances_verified':11,'dynamic_property_resolution_implemented':False,'combat_or_ai_implemented':False,'physical_arm64_tested':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 for path,result in ((ROOT/'reports/build-validation-classes.json',validation),(ROOT/'reports/actor-states-smoke-classes.json',states),(ROOT/'reports/world-smoke-classes.json',world),(REPO/'port/game-data/reports/classes-host-audit.json',host),(REPO/'port/game-data/reports/class-input-provenance.json',provenance)):
  path.parent.mkdir(parents=True,exist_ok=True);path.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-classes-{sha[:8]}.apk';checkpoint.parent.mkdir(parents=True,exist_ok=True);checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'assets':len(assets),'original_inputs':validation['original_input_count'],'emulator_cases':34,'native_base_class_instances':11,'goal_status':'active'}))
if __name__=='__main__':main()
