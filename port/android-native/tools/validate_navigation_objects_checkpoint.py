"""Bind recovered object/registry source to both APKs and fresh live checks."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_navigation_checkpoint import ROOT,REPO,digest,read,log
from validate_collision_checkpoint import verify_capture
from validate_selector_checkpoint import fields

PROBE={'floor_pairs':64,'accepted':64,'registered':64,'relocated':56,'state_fnv1a64':'22a98dfc4102db51'}
TOTALS={'defaults':1,'init_object':207,'init_obstacle':462,'parent_change':107,'set_flying':81,'set_swimming':88,'position':146,'policy_defaults':1,'floor_queries':477,'registry_entries_compared':9598,'registry_keys_compared':2188}
EXPORTS={'dh2_nav_object_defaults','dh2_nav_motion_policy_defaults','dh2_nav_object_set_flying','dh2_nav_object_set_swimming','dh2_nav_object_is_flying','dh2_nav_object_is_swimming','dh2_nav_init_object','dh2_nav_init_obstacle','dh2_nav_change_obstacle_parent','dh2_nav_validate_object_position','dh2_nav_validate_position','dh2_nav_find_path','dh2_nav_route','dh2_nav_search'}

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args();local=REPO/'.local-inputs';rd=REPO/'port/level-world/reports';prior_path=ROOT/'reports/build-validation-navigation-motion.json';prior=read(prior_path)
 apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';studio_apk=a.studio/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libs=inspect(apk);studio_libs=inspect(studio_apk);assert len(libs)==len(studio_libs)==14
 engine=local/'libDungeonHunter2.so';engine_sha=digest(engine);reference=digest(local/'navigation-objects-original.bin');linked=digest(local/'navigation-link-original.bin');floor_sha=digest(local/'authored-floors-packaged.json');assert linked==prior['linked_reference_sha256'] and floor_sha==prior['floor_geometry_sha256'];reports={};library_sha={}
 for tag in ('oracle','packaged','studio'):
  library=local/('navigation-objects-oracle.so' if tag=='oracle' else f'navigation-objects-{tag}-world-arm64.so');library_sha[tag]=digest(library);report=read(rd/('navigation-objects-arm64-differential.json' if tag=='oracle' else f'navigation-objects-{tag}-arm64-differential.json'));fixture=local/('navigation-objects-original.bin' if tag=='oracle' else f'navigation-objects-{tag}-original.bin')
  fields(report,{'comparisons':1093,'mismatches':0,'totals':TOTALS,'crypt_objects_probe':PROBE,'original_sha256':engine_sha,'arm64_library_sha256':library_sha[tag],'linked_reference_sha256':linked,'floor_source_sha256':floor_sha,'reference_sha256':reference,'full_registry_and_all_object_fields_compared':True,'original_map_and_deque_instructions_execute':True,'parent_service_attached_to_position_validation':True,'original_world_height_limit':100.});assert digest(fixture)==reference;reports[tag]=report
 host={name:read(rd/f'navigation-{name}-host-audit.json') for name in ('objects','objects-motion-regression','objects-find-regression','objects-path-regression','objects-world-regression','objects-search-regression')}
 fields(host['objects'],{'cases':1093,'operation_counts':[TOTALS[n] for n in ('defaults','init_object','init_obstacle','parent_change','set_flying','set_swimming','position','policy_defaults')],'referenced_floor_queries':477,'registry_entries_compared':9598,'registry_keys_compared':2188,'atomic_rejection_checks':9,'authored_graph_nodes':335,'authored_graph_edges':838,'reference_sha256':reference})
 for name,old in [('objects-motion-regression','motion'),('objects-find-regression','motion-find-regression'),('objects-path-regression','motion-path-regression'),('objects-world-regression','motion-world-regression'),('objects-search-regression','motion-search-regression')]:fields(host[name],prior['host_audits'][old])
 for report in host.values():fields(report,{'mismatches':0,'sanitizers':['address','undefined']})
 world=read(local/'world-tests-navigation-objects/world-smoke.json');fields(world,{'apk_sha256':sha,'installed_apk_sha256':sha,'native_floor_records':8,'native_graph_nodes':335,'native_graph_edges':838,'native_neighbour_floor_relations':14,'native_validation_references':998,'native_object_initialization_used_by_startup_probe':True,'original_motion_obstacle_defaults_reconstructed':True,'original_init_object_reconstructed':True,'original_init_obstacle_reconstructed':True,'original_obstacle_parent_backend_reconstructed':True,'native_obstacle_registry_used_by_actor_movement':False,'native_floor_motion_used_by_actor_movement':False,'original_obstacle_force_reconstructed':False,'original_movement_controller_reconstructed':False,'frozen_idle_stable':True,'live_idle_changed_pixels':True,'rotation_position_preserved':True,'pause_cancels_movement':True,'crypt_objects_probe':PROBE});assert len(world['cases'])==10 and world['floor_geometry_sha256']==floor_sha and world['crypt_motion_probe']==prior['crypt_motion_probe'] and world['crypt_findpath_probe']==prior['crypt_findpath_probe']
 assets={};unchanged=[]
 with zipfile.ZipFile(apk) as z,zipfile.ZipFile(studio_apk) as sz:
  assert set(z.namelist())==set(sz.namelist()) and not any('DungeonHunter2.so' in n for n in z.namelist())
  for name in z.namelist():
   if name.startswith('assets/') and not name.endswith('/'):
    raw=z.read(name);relative=name[7:];assert raw==sz.read(name)==(ROOT/'app/src/main/assets'/relative).read_bytes()==(a.studio/'app/src/main/assets'/relative).read_bytes();assets[relative]={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}
   elif name.startswith('lib/') and name.endswith('/libdh2_level_world.so'):assert EXPORTS<=exports(z.read(name)) and EXPORTS<=exports(sz.read(name))
  assert assets==prior['assets_verified'] and len(assets)==126
  for tag,archive in [('packaged',z),('studio',sz)]:assert hashlib.sha256(archive.read('lib/arm64-v8a/libdh2_level_world.so')).hexdigest()==library_sha[tag]
  for rows,archive in [(prior['libraries'],z),(prior['studio_libraries'],sz)]:
   for row in rows:
    if Path(row['path']).name not in {'libdh2_level_world.so','libdh2_native.so'}:
     assert hashlib.sha256(archive.read(row['path'])).hexdigest()==row['sha256'],row['path']
     if archive is z:unchanged.append(row['path'])
 for name,report in host.items():
  if name!='objects-path-regression':assert report['bres_sha256']==assets['worlds/crypt.bdae']['sha256'] and report['descriptor_sha256']==assets['worlds/crypt01.dwld']['sha256']
 source={}
 for name,expected in prior['studio_source_sha256'].items():
  value=digest(ROOT/'app/src/main'/name);assert value==digest(a.studio/'app/src/main'/name)
  if name!='cpp/model_renderer.cpp':assert value==expected,name
  source[name]=value
 changed={'android-native/README.md','android-native/ROADMAP.md','android-native/tools/world_smoke.py','level-world/CMakeLists.txt','level-world/README.md'}
 for name,expected in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==expected,name
 additions={'level-world/navigation_objects.cpp','level-world/navigation_objects.hpp','level-world/tests/navigation_objects.cpp','level-world/tests/navigation_objects_differential.py','level-world/tools/build_navigation_objects_oracle.ps1','android-native/tools/validate_navigation_objects_checkpoint.py'}
 additions|={f.relative_to(REPO/'port').as_posix() for f in (REPO/'port/level-world/reference/navigation-objects').rglob('*') if f.is_file()};verify_capture(REPO/'port/level-world/reference/navigation-objects/original-functions.json',engine)
 for name in ('navigation-objects-repo-build.log','navigation-objects-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert log(local/'navigation-objects-zipalign.log').count('Verification successful')==2
 marker='Native obstacle registry probe | floor pairs 64 | accepted 64 | registered 64 | relocated 56 | state 22a98dfc4102db51 | forces and controller pending'
 for f in (local/'world-tests-navigation-objects').glob('*.log'):
  text=log(f);assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|Shader failed|Link failed',text),f;assert marker in text,f
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 result={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(studio_apk),'libraries':libs,'studio_libraries':studio_libs,'unchanged_libraries':unchanged,'assets_verified':assets,'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'studio_source_sha256':source,'module_source_sha256':{n:digest(REPO/'port'/n) for n in sorted(set(prior['module_source_sha256'])|changed|additions)},'objects_reports':reports,'reference_sha256':reference,'linked_reference_sha256':linked,'host_audits':host,'crypt_objects_probe':PROBE,'crypt_motion_probe':prior['crypt_motion_probe'],'crypt_findpath_probe':prior['crypt_findpath_probe'],'floor_geometry_sha256':floor_sha,'world_cases':10,'apk_16k_zip_alignment_verified':True,'prior_motion_checkpoint':{'apk_sha256':prior['apk_sha256'],'validation_sha256':digest(prior_path)},'prior_combat_checkpoint':prior['prior_combat_checkpoint'],'original_init_object_reconstructed':True,'original_init_obstacle_reconstructed':True,'original_motion_obstacle_defaults_reconstructed':True,'full_pfobject_constructor_projection_verified':False,'original_obstacle_parent_backend_reconstructed':True,'parent_service_attached_to_native_position_validation':True,'objects_used_by_startup_probe':True,'objects_used_by_actor_movement':False,'original_obstacle_force_reconstructed':False,'original_actor_navigation_producers_reconstructed':False,'original_cache_invalidation_lifecycle_reconstructed':False,'original_movement_controller_reconstructed':False,'pending_navigation':'Obstacle force/avoidance, GameObject capability/radius/obstacle producers, cache lifecycle, controller/root motion and pursuing enemies.','physical_arm64_tested':False,'original_gpu_parity_verified':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 (ROOT/'reports/build-validation-navigation-objects.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8');(ROOT/'reports/world-smoke-navigation-objects.json').write_text(json.dumps(world,indent=2)+'\n',encoding='utf-8');checkpoint=ROOT/'build/checkpoints'/f'dh2-native-obstacle-registry-{sha[:8]}.apk';checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'checkpoint':str(checkpoint),'objects_comparisons_per_arm64_binary':1093,'crypt_objects_probe':PROBE,'world_cases':10,'actor_movement_uses_registry':False,'goal_status':'active'}))
if __name__=='__main__':main()
