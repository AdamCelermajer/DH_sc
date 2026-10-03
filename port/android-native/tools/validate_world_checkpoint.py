"""Bind the authored-level checkpoint to source, input hashes and test APK."""
import argparse,hashlib,json,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def read(path):
 raw=path.read_bytes();return json.loads(raw.decode('utf-16' if raw.startswith((b'\xff\xfe',b'\xfe\xff')) else 'utf-8-sig'))
def log(path):
 raw=path.read_bytes();return raw.decode('utf-16' if raw.startswith((b'\xff\xfe',b'\xfe\xff')) else 'utf-8-sig')
def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args()
 local=REPO/'.local-inputs';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libraries=inspect(apk)
 tests={'world-smoke.json':local/'world-tests-final/world-smoke.json','model-smoke-world.json':local/'model-tests-world/model-smoke.json','character-smoke-world.json':local/'prince-tests-world/animation-smoke.json','candle-smoke-world.json':local/'candle-tests-world/animation-smoke.json','texture-regression-world.json':local/'texture-tests-world/emulator-smoke.json'}
 records={name:read(path) for name,path in tests.items()};assert all(row['apk_sha256']==sha for row in records.values()),'Mixed APK checkpoints'
 world=records['world-smoke.json'];assert len(world['cases'])==10 and len(world['movement'])==3
 assert all(world[key] for key in ('frozen_idle_stable','live_idle_changed_pixels','rotation_position_preserved','pause_cancels_movement'))
 assert records['character-smoke-world.json']['pose_times_ms']==[100,1700] and records['character-smoke-world.json']['skinning_implemented']
 assert len(records['model-smoke-world.json']['cases'])==9 and len(records['texture-regression-world.json']['cases'])==7
 source={}
 for name in ['cpp/CMakeLists.txt','cpp/native_app.cpp','cpp/model_renderer.cpp','cpp/model_renderer.hpp','java/com/example/dh2/MainActivity.java','java/com/example/dh2/NativeBridge.java']:
  file=ROOT/'app/src/main'/name;other=a.studio/'app/src/main'/name;assert digest(file)==digest(other),name;source[name]=digest(file)
 assets_root=ROOT/'app/src/main/assets';provenance=read(assets_root/'texture-provenance.json');level=read(assets_root/'worlds/crypt01-provenance.json');assets=[]
 assert len(provenance['samples'])==19 and len(level['inputs'])==19 and len(level['rooms'])==8 and len(level['objects'])==166 and level['procedural_rules_executed'] is False
 assert level['cache_sha256']==provenance['cache_sha256']
 with zipfile.ZipFile(apk) as archive:
  for row in provenance['samples']:
   folder='models' if row['name'] in {'candle_flame.bdae','main_menu_charactere_swamp.bdae','prince_modular.bdae'} else 'animations' if row['name'] in {'prince_menu_idle_knight.bdae','prince_idle_shield.bdae','prince_walk_1hand.bdae'} else 'textures'
   name=f'assets/{folder}/{row["name"]}';raw=archive.read(name);assert len(raw)==row['bytes'] and hashlib.sha256(raw).hexdigest()==row['sha256'];assets.append(name)
  for row in level['inputs']:
   name='assets/worlds/'+row['name'];raw=archive.read(name);assert len(raw)==row['bytes'] and hashlib.sha256(raw).hexdigest()==row['sha256'];assets.append(name)
  descriptor=archive.read('assets/worlds/crypt01.dwld');assert hashlib.sha256(descriptor).hexdigest()==level['compiled_descriptor_sha256'];assets.append('assets/worlds/crypt01.dwld')
  for name in ('worlds/crypt01-provenance.json','texture-provenance.json'):
   assert archive.read('assets/'+name)==(assets_root/name).read_bytes();assert digest(assets_root/name)==digest(a.studio/'app/src/main/assets'/name)
  assets.append('assets/worlds/crypt01-provenance.json')
  assert sorted(n for n in archive.namelist() if n.startswith(('assets/models/','assets/animations/','assets/textures/','assets/worlds/')))==sorted(assets)
  for name in assets:
   relative=name.removeprefix('assets/');assert archive.read(name)==(a.studio/'app/src/main/assets'/relative).read_bytes(),relative
 assert len(assets)==40
 for name in ('world-android-build.log','world-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name),name
 assert 'Verification successful' in log(local/'world-zipalign.log')
 host=read(local/'world-host-audit.json');assert host['rooms']==8 and host['visual_instances']==97 and host['triangles']==14251 and host['navigation_triangles']==314 and host['mutated_descriptors']==2000 and host['synthetic_movement_checks']==9
 locomotion=read(local/'locomotion-host-audit.json');assert locomotion['skins']==4 and [r['tracks'] for r in locomotion['clips']]==[23,27] and sum(r['sampled_poses'] for r in locomotion['clips'])==1868 and all(r['skipped']==0 for r in locomotion['clips'])
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for line in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):
  assert '-fsanitize=address,undefined' in next(s for s in cache.splitlines() if s.startswith(line)),line
 reports=ROOT/'reports';reports.mkdir(exist_ok=True);world_reports=REPO/'port/level-world/reports';world_reports.mkdir(exist_ok=True)
 for name,value in records.items():(reports/name).write_text(json.dumps(value,indent=2)+'\n',encoding='utf-8')
 (world_reports/'host-audit.json').write_text(json.dumps(host,indent=2)+'\n',encoding='utf-8');(world_reports/'locomotion-audit.json').write_text(json.dumps(locomotion,indent=2)+'\n',encoding='utf-8')
 inputs={'cache_sha256':level['cache_sha256'],'compiled_descriptor_sha256':level['compiled_descriptor_sha256'],'world_inputs':level['inputs'],'locomotion_inputs':[r for r in provenance['samples'] if r['name'] in {'prince_idle_shield.bdae','prince_walk_1hand.bdae'}],'layout':level['layout'],'rooms':level['rooms'],'original_spawn':level['spawn'],'preserved_object_count':len(level['objects']),'procedural_rules_executed':False}
 (world_reports/'input-provenance.json').write_text(json.dumps(inputs,indent=2)+'\n',encoding='utf-8')
 validation={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(a.studio/'app/build/outputs/apk/debug/app-debug.apk'),'repo_and_studio_gradle_builds_passed':True,'studio_source_sha256':source,'world_source_sha256':{name:digest(REPO/'port/level-world'/name) for name in ('world.cpp','world.hpp','CMakeLists.txt')},'libraries':libraries,'apk_16k_zip_alignment_verified':True,'bundled_assets_verified':assets,'original_input_count':38,'cache_sha256':level['cache_sha256'],'host_asan_ubsan':host,'locomotion_asan_ubsan':locomotion,'world_screenshot_cases':10,'emulator_model_captures':9,'emulator_texture_cases':7,'physical_arm64_tested':False,'original_pf_parity_verified':False,'original_gpu_parity_verified':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 (reports/'build-validation-world.json').write_text(json.dumps(validation,indent=2)+'\n',encoding='utf-8');print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'assets':len(assets),'original_inputs':38,'source_sync_verified':True,'goal_status':'active'}))
if __name__=='__main__':main()
