"""Bind object integration checks to one APK and current source/input hashes."""
import argparse,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_world_checkpoint import digest,read,log,ROOT,REPO
def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args();local=REPO/'.local-inputs';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libraries=inspect(apk)
 tests={'objects-smoke.json':local/'objects-tests-final/objects-smoke.json','world-smoke-objects.json':local/'world-tests-objects/world-smoke.json','model-smoke-objects.json':local/'model-tests-objects/model-smoke.json','character-smoke-objects.json':local/'prince-tests-objects/animation-smoke.json','texture-regression-objects.json':local/'texture-tests-objects/emulator-smoke.json'}
 results={name:read(path) for name,path in tests.items()};assert all(r['apk_sha256']==sha for r in results.values()),'Mixed APK checkpoints'
 objects=results['objects-smoke.json'];world=results['world-smoke-objects.json'];assert len(objects['cases'])==22 and len({r['model'] for r in objects['cases']})==12
 assert (objects['objects'],objects['monsters'],objects['decors'],objects['resource_count'])==(95,11,84,12)
 assert all(objects[k] for k in ('frozen_stable','live_animation_changes','rotation_reloads_objects','resume_reloads_objects'))
 assert len(world['cases'])==10 and len(world['movement'])==3 and all(world[k] for k in ('frozen_idle_stable','live_idle_changed_pixels','rotation_position_preserved','pause_cancels_movement'))
 assert len(results['model-smoke-objects.json']['cases'])==9 and len(results['texture-regression-objects.json']['cases'])==7
 character=results['character-smoke-objects.json'];assert len(character['cases'])==5 and character['pose_times_ms']==[100,1700] and character['skinning_implemented'] and character['live_animation_changed_pixels'] and character['frozen_pose_stable']
 source={}
 for name in ('cpp/CMakeLists.txt','cpp/native_app.cpp','cpp/model_renderer.cpp','cpp/model_renderer.hpp','java/com/example/dh2/MainActivity.java','java/com/example/dh2/NativeBridge.java'):
  file=ROOT/'app/src/main'/name;assert digest(file)==digest(a.studio/'app/src/main'/name),name;source[name]=digest(file)
 module_source={name:digest(REPO/'port'/name) for name in ('game-data/CMakeLists.txt','game-data/data.hpp','game-data/data.cpp','level-world/CMakeLists.txt','level-world/world.cpp','level-world/world.hpp','level-world/objects.cpp','level-world/objects.hpp','engine-animation/animation.hpp','engine-animation/animation.cpp','engine-skinning/skinning.hpp','engine-skinning/skinning.cpp')}
 assets_root=ROOT/'app/src/main/assets';base=read(assets_root/'texture-provenance.json');level=read(assets_root/'worlds/crypt01-provenance.json');actor=read(assets_root/'actor-provenance.json');assets={};entries=set()
 assert base['cache_sha256']==level['cache_sha256']==actor['cache_sha256'];assert len(actor['records'])==95 and len(actor['skipped'])==71 and len(actor['inputs'])==32
 def add(name,row,entry):
  if name in assets:assert assets[name]=={'bytes':row['bytes'],'sha256':row['sha256']},name
  assets[name]={'bytes':row['bytes'],'sha256':row['sha256']};entries.add(entry)
 for row in base['samples']:
  name=row['name'];folder='models' if name in ('prince_modular.bdae','candle_flame.bdae','main_menu_charactere_swamp.bdae') else 'animations' if name.startswith('prince_') else 'textures';add(folder+'/'+name,row,row['archive_entry'])
 for row in level['inputs']:add('worlds/'+row['name'],row,row['entry'])
 for row in actor['inputs']:add(row['asset'],row,row['entry'])
 for name,expected in (('worlds/crypt01.dwld',level['compiled_descriptor_sha256']),('worlds/crypt01.dact',actor['descriptor_sha256'])):
  file=assets_root/name;assert digest(file)==expected;assets[name]={'bytes':file.stat().st_size,'sha256':digest(file)}
 for name in ('texture-provenance.json','actor-provenance.json','worlds/crypt01-provenance.json'):
  file=assets_root/name;assets[name]={'bytes':file.stat().st_size,'sha256':digest(file)}
 with zipfile.ZipFile(apk) as archive:
  actual={n.removeprefix('assets/') for n in archive.namelist() if n.startswith('assets/') and not n.endswith('/')};assert actual==set(assets),(actual-set(assets),set(assets)-actual)
  for name,row in assets.items():
   raw=archive.read('assets/'+name);assert raw==(assets_root/name).read_bytes()==(a.studio/'app/src/main/assets'/name).read_bytes(),name;assert len(raw)==row['bytes'] and __import__('hashlib').sha256(raw).hexdigest()==row['sha256'],name
 assert len(assets)==72 and len(entries)==66,(len(assets),len(entries))
 host=read(local/'objects-host-audit.json');data=read(local/'game-data-host-audit.json');oracle=read(local/'actors-arm64-differential.json');prince=read(local/'actors-prince-host-audit.json');locomotion=read(local/'actors-locomotion-audit.json');animation=read(local/'actors-animation-audit.txt')
 assert host['sampled_poses']==44769 and host['descriptor_mutations']==3000 and host['unbound_tracks']==3 and host['unsupported_tracks']==3 and host['strict_missing_target_rejected'] and host['malformed_unbound_key_rejected']
 assert (data['characters'],data['fields'],data['models'],data['mutations_and_truncations'])==(448,224,116,2000)
 assert oracle['three_matrix_palette_bit_exact_cases']==200 and oracle['zero_influence_cases']==10
 assert prince['controllers']==173 and prince['mutations']==1000 and sum(r['sampled_poses'] for r in locomotion['clips'])==1868 and animation['mutated_inputs']==5000
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(s for s in cache.splitlines() if s.startswith(prefix))
 for name in ('actors-android-build.log','actors-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name),name
 assert 'Verification successful' in log(local/'actors-zipalign.log')
 for name in ('objects-tests-final/objects-lifecycle.log','world-tests-objects/world-lifecycle.log','world-tests-objects/world-boundary.log','prince-tests-objects/animation.log'):
  assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed',log(local/name)),name
 reports=ROOT/'reports'
 for name,result in results.items():(reports/name).write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
 validation={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(a.studio/'app/build/outputs/apk/debug/app-debug.apk'),'repo_and_studio_builds_passed':True,'studio_source_sha256':source,'module_source_sha256':module_source,'libraries':libraries,'apk_16k_zip_alignment_verified':True,'assets_verified':assets,'original_input_count':len(entries),'cache_sha256':base['cache_sha256'],'objects_asan_ubsan':host,'data_asan_ubsan':data,'prince_asan_ubsan':prince,'locomotion_asan_ubsan':locomotion,'animation_asan_ubsan':animation,'original_instruction_comparisons':oracle,'object_screenshots':22,'world_screenshots':10,'preview_screenshots':9,'character_screenshots':5,'texture_cases':7,'prior_emulator_system_failure':'API 37 system_server watchdog; app/SystemUI/phone reported DeadSystemException. Retained local log; cold restart before the passing final suite. Root cause not established.','physical_arm64_tested':False,'combat_or_ai_implemented':False,'original_gpu_parity_verified':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 (reports/'build-validation-objects.json').write_text(json.dumps(validation,indent=2)+'\n',encoding='utf-8')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-objects-{sha[:8]}.apk';checkpoint.parent.mkdir(parents=True,exist_ok=True);checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'assets':len(assets),'original_inputs':len(entries),'source_sync_verified':True,'goal_status':'active'}))
if __name__=='__main__':main()
