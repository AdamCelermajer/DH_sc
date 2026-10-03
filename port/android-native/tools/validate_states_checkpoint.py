"""Bind native animation-table integration to its own APK and fresh checks."""
import argparse,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_world_checkpoint import digest,read,log,ROOT,REPO
def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args();local=REPO/'.local-inputs';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libraries=inspect(apk)
 assert len(libraries)==14
 objects=read(local/'objects-tests-states/objects-smoke.json');world=read(local/'world-tests-states/world-smoke.json')
 assert objects['apk_sha256']==world['apk_sha256']==sha
 assert len(objects['cases'])==22 and len(world['cases'])==10 and len(world['movement'])==3
 assert all(objects[k] for k in ('frozen_stable','live_animation_changes','rotation_reloads_objects','resume_reloads_objects'))
 assert all(world[k] for k in ('frozen_idle_stable','live_idle_changed_pixels','rotation_position_preserved','pause_cancels_movement'))
 lifecycle=log(local/'objects-tests-states/objects-lifecycle.log')
 assert 'Animation tables ready | sequences 785 | characters 80 | clip paths 1447 | bytes 72812' in lifecycle
 for model,table,sequence,clip,speed in (('skeleton.bdae',62,596,1194,'1.0000'),('slime_green_v2.bdae',64,613,1258,'1.0000'),('ghost.bdae',24,210,706,'1.3000')):
  assert re.search(r'Original idle selected \| '+re.escape(model)+rf' \| table {table} \| sequence {sequence} \| step \d+ \| clip {clip} \| \S+ \| speed '+speed,lifecycle)
 source={}
 for name in ('cpp/CMakeLists.txt','cpp/native_app.cpp','cpp/model_renderer.cpp','cpp/model_renderer.hpp','java/com/example/dh2/MainActivity.java','java/com/example/dh2/NativeBridge.java'):
  file=ROOT/'app/src/main'/name;assert digest(file)==digest(a.studio/'app/src/main'/name);source[name]=digest(file)
 old=read(ROOT/'reports/build-validation-objects.json');assets=dict(old['assets_verified']);assets_root=ROOT/'app/src/main/assets';animation=read(assets_root/'animation-provenance.json')
 assert len(animation['inputs'])==10 and (animation['sequence_count'],animation['character_count'],animation['clip_path_count'],animation['serialized_bytes'])==(785,80,1447,72812)
 assert animation['cache_sha256']==old['cache_sha256']
 for row in animation['inputs']:
  expected={'bytes':row['bytes'],'sha256':row['sha256']};name=row['asset']
  if name in assets:assert assets[name]==expected
  assets[name]=expected
 file=assets_root/'animation-provenance.json';assets['animation-provenance.json']={'bytes':file.stat().st_size,'sha256':digest(file)}
 with zipfile.ZipFile(apk) as archive:
  actual={n.removeprefix('assets/') for n in archive.namelist() if n.startswith('assets/') and not n.endswith('/')};assert actual==set(assets)
  for name,row in assets.items():
   raw=archive.read('assets/'+name);assert raw==(assets_root/name).read_bytes()==(a.studio/'app/src/main/assets'/name).read_bytes();assert len(raw)==row['bytes'] and __import__('hashlib').sha256(raw).hexdigest()==row['sha256']
 assert len(assets)==80
 base=read(assets_root/'texture-provenance.json');level=read(assets_root/'worlds/crypt01-provenance.json');actor=read(assets_root/'actor-provenance.json')
 entries={row['archive_entry'] for row in base['samples']}|{row['entry'] for row in level['inputs']}|{row['entry'] for row in actor['inputs']}|{row['entry'] for row in animation['inputs']}
 assert len(entries)==73
 table=read(local/'states-animation-tables-audit.json');idle=read(local/'states-idle-options-audit.json');random=read(REPO/'port/game-data/reports/animation-random-arm64-differential.json')
 assert table['native_roundtrip_matches_original'] and table['consumed']==72812 and table['mutations_and_truncations']==3000 and table['recursive_redirect_rejected']
 assert idle['every_millisecond_poses']==24103 and idle['original_idle_choices']==5 and idle['finite_skinned_vertices']
 assert random['bit_exact_random_index_and_state_cases']==10000 and random['arm64_library_sha256']==digest(local/'animation-selection-oracle.so')
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(s for s in cache.splitlines() if s.startswith(prefix))
 for name in ('states-android-build.log','states-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'states-zipalign.log')
 for name in ('objects-tests-states/objects-lifecycle.log','world-tests-states/world-lifecycle.log','world-tests-states/world-boundary.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed',log(local/name))
 modules=set(old['module_source_sha256'])|{'game-data/animation_tables.hpp','game-data/animation_tables.cpp','game-data/animation_selection.cpp'}
 validation={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(a.studio/'app/build/outputs/apk/debug/app-debug.apk'),'source_sync_verified':True,'studio_source_sha256':source,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(modules)},'libraries':libraries,'apk_16k_zip_alignment_verified':True,'assets_verified':assets,'original_input_count':73,'cache_sha256':old['cache_sha256'],'table_asan_ubsan':table,'idle_poses_asan_ubsan':idle,'random_original_arm64_comparisons':random,'object_cases':22,'world_cases':10,'prior_full_regression_apk_sha256':old['apk_sha256'],'idle_initial_selection_and_speed_native':True,'development_rng_seed':1,'shared_object_clock':True,'animation_completion_and_blending_implemented':False,'combat_or_ai_implemented':False,'physical_arm64_tested':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 for path,result in ((ROOT/'reports/build-validation-states.json',validation),(ROOT/'reports/objects-smoke-states.json',objects),(ROOT/'reports/world-smoke-states.json',world),(REPO/'port/game-data/reports/animation-tables-host-audit.json',table),(REPO/'port/level-world/reports/idle-options-host-audit.json',idle),(REPO/'port/game-data/reports/animation-input-provenance.json',animation)):
  path.parent.mkdir(parents=True,exist_ok=True);path.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-states-{sha[:8]}.apk';checkpoint.parent.mkdir(parents=True,exist_ok=True);checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'assets':len(assets),'emulator_cases':32,'goal_status':'active'}))
if __name__=='__main__':main()
