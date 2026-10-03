"""Bind recovered concrete obstacle producers to both APKs and live probes."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_navigation_checkpoint import ROOT,REPO,digest,read,log
from validate_collision_checkpoint import verify_capture
from validate_selector_checkpoint import fields
from validate_navigation_avoidance_checkpoint import EXPORTS as AVOIDANCE_EXPORTS

PROBE={'floors':8,'registered':8,'physical_radius_updates':8,'state_fnv1a64':'30c1f3ac1bf81145'}
TOTALS={'updates':1233,'class_counts':[239,315,225,215,239],'null_user':767,'physical_radius':267,'bounds_radius':199,'obstacle_calls':381,'floor_queries':24,'registry_entries_compared':4314,'registry_keys_compared':1893}
EXPORTS=AVOIDANCE_EXPORTS|{'dh2_nav_producer_traits','dh2_nav_update_game_object'}

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args()
 local=REPO/'.local-inputs';rd=REPO/'port/level-world/reports';prior_path=ROOT/'reports/build-validation-navigation-avoidance.json';prior=read(prior_path)
 apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';studio_apk=a.studio/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libs=inspect(apk);studio_libs=inspect(studio_apk);assert len(libs)==len(studio_libs)==14
 engine=local/'libDungeonHunter2.so';engine_sha=digest(engine);reference=digest(local/'navigation-producers-original.bin');linked=digest(local/'navigation-link-original.bin');floor_sha=digest(local/'authored-floors-packaged.json');assert linked==prior['linked_reference_sha256'] and floor_sha==prior['floor_geometry_sha256']
 reports={};library_sha={}
 for tag in ('oracle','packaged','studio'):
  library=local/('navigation-producers-oracle.so' if tag=='oracle' else f'navigation-producers-{tag}-world-arm64.so');library_sha[tag]=digest(library);suffix='' if tag=='oracle' else '-'+tag
  report=read(rd/f'navigation-producers{suffix}-arm64-differential.json')
  fields(report,{'comparisons':1238,'trait_comparisons':5,'mismatches':0,'totals':TOTALS,'crypt_producers_probe':PROBE,'original_sha256':engine_sha,'arm64_library_sha256':library_sha[tag],'linked_reference_sha256':linked,'floor_source_sha256':floor_sha,'reference_sha256':reference,'full_registry_and_all_object_fields_compared':True,'original_concrete_vtable_dispatch_executes':True,'obstacle_before_radius_update_verified':True})
  assert digest(local/f'navigation-producers{suffix}-original.bin')==reference;reports[tag]=report
 host={name:read(rd/f'navigation-{name}-host-audit.json') for name in ('producers','producers-avoidance-regression','producers-objects-regression','producers-motion-regression','producers-find-regression','producers-path-regression','producers-world-regression','producers-search-regression')}
 fields(host['producers'],{'cases':1233,'comparisons':1238,'trait_comparisons':5,'totals':TOTALS,'atomic_rejection_checks':10,'null_user_early_gate_checks':1,'authored_graph_nodes':335,'authored_graph_edges':838,'reference_sha256':reference})
 for name,old in [('producers-avoidance-regression','avoidance'),('producers-objects-regression','avoidance-objects-regression'),('producers-motion-regression','avoidance-motion-regression'),('producers-find-regression','avoidance-find-regression'),('producers-path-regression','avoidance-path-regression'),('producers-world-regression','avoidance-world-regression'),('producers-search-regression','avoidance-search-regression')]:fields(host[name],prior['host_audits'][old])
 for report in host.values():fields(report,{'mismatches':0,'sanitizers':['address','undefined']})
 world_dir=local/'world-tests-navigation-producers';world=read(world_dir/'world-smoke.json')
 fields(world,{'apk_sha256':sha,'installed_apk_sha256':sha,'native_floor_records':8,'native_graph_nodes':335,'native_graph_edges':838,'native_neighbour_floor_relations':14,'native_validation_references':998,'original_update_pfobject_reconstructed':True,'original_concrete_obstacle_producers_reconstructed':True,'original_physical_radius_conversion_reconstructed':True,'native_actor_producers_used_by_startup_probe':True,'native_actor_producers_used_by_actor_movement':False,'original_physical_body_construction_reconstructed':False,'original_actor_navigation_producers_reconstructed':False,'original_movement_controller_reconstructed':False,'native_avoidance_used_by_actor_movement':False,'frozen_idle_stable':True,'live_idle_changed_pixels':True,'rotation_position_preserved':True,'pause_cancels_movement':True,'crypt_producers_probe':PROBE,'physical_arm64_tested':False,'full_game_playable':False})
 assert len(world['cases'])==10 and world['floor_geometry_sha256']==floor_sha
 old_world=read(ROOT/'reports/world-smoke-navigation-avoidance.json')
 for key in ('crypt_route_probe','crypt_world_route_probe','crypt_findpath_probe','crypt_motion_probe','crypt_objects_probe','crypt_avoidance_probe'):assert world[key]==old_world[key]
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
  if name!='producers-path-regression':assert report['bres_sha256']==assets['worlds/crypt.bdae']['sha256'] and report['descriptor_sha256']==assets['worlds/crypt01.dwld']['sha256']
 source={}
 for name,expected in prior['studio_source_sha256'].items():
  value=digest(ROOT/'app/src/main'/name);assert value==digest(a.studio/'app/src/main'/name)
  if name!='cpp/model_renderer.cpp':assert value==expected,name
  source[name]=value
 changed={'android-native/README.md','android-native/ROADMAP.md','android-native/tools/world_smoke.py','level-world/CMakeLists.txt','level-world/README.md'}
 for name,expected in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==expected,name
 additions={'level-world/navigation_producers.cpp','level-world/navigation_producers.hpp','level-world/tests/navigation_producers.cpp','level-world/tests/navigation_producers_differential.py','level-world/tools/build_navigation_producers_oracle.ps1','android-native/tools/validate_navigation_producers_checkpoint.py'}
 capture=REPO/'port/level-world/reference/navigation-producers';additions|={f.relative_to(REPO/'port').as_posix() for f in capture.rglob('*') if f.is_file()};verify_capture(capture/'original-functions.json',engine)
 for name in ('navigation-producers-repo-build.log','navigation-producers-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert log(local/'navigation-producers-zipalign.log').count('Verification successful')==2
 marker='Native actor producer probe | floors 8 | registered 8 | physical radius updates 8 | state 30c1f3ac1bf81145 | physical construction and controller pending'
 logs=list(world_dir.glob('*.log'));assert logs
 for path in logs:
  text=log(path);assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|Shader failed|Link failed',text),path;assert marker in text,path
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 result={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(studio_apk),'libraries':libs,'studio_libraries':studio_libs,'unchanged_libraries':unchanged,'assets_verified':assets,'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'studio_source_sha256':source,'module_source_sha256':{n:digest(REPO/'port'/n) for n in sorted(set(prior['module_source_sha256'])|changed|additions)},'producers_reports':reports,'reference_sha256':reference,'linked_reference_sha256':linked,'host_audits':host,'crypt_producers_probe':PROBE,**{k:prior[k] for k in ('crypt_avoidance_probe','crypt_objects_probe','crypt_motion_probe','crypt_findpath_probe')},'floor_geometry_sha256':floor_sha,'world_cases':10,'apk_16k_zip_alignment_verified':True,'prior_avoidance_checkpoint':{'apk_sha256':prior['apk_sha256'],'validation_sha256':digest(prior_path)},'prior_combat_checkpoint':prior['prior_combat_checkpoint'],'original_update_pfobject_reconstructed':True,'original_concrete_obstacle_producers_reconstructed':True,'original_physical_radius_conversion_reconstructed':True,'original_obstacle_force_reconstructed':True,'original_obstacle_avoidance_reconstructed':True,'original_concrete_physical_collision_filter_reconstructed':True,'original_obstacle_parent_backend_reconstructed':True,'parent_service_attached_to_native_position_validation':True,'producer_used_by_startup_probe':True,'producer_used_by_actor_movement':False,'original_actor_navigation_producers_reconstructed':False,'original_physical_body_construction_reconstructed':False,'original_cache_invalidation_lifecycle_reconstructed':False,'original_movement_controller_reconstructed':False,'pending_navigation':'Physical body/filter/bounds and capability/initialization producers, cache lifecycle, moving controller/root motion and pursuing enemies.','physical_arm64_tested':False,'original_gpu_parity_verified':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 (ROOT/'reports/build-validation-navigation-producers.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8');(ROOT/'reports/world-smoke-navigation-producers.json').write_text(json.dumps(world,indent=2)+'\n',encoding='utf-8')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-actor-producers-{sha[:8]}.apk';checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'checkpoint':str(checkpoint),'producer_comparisons_per_arm64_binary':1238,'crypt_producers_probe':PROBE,'world_cases':10,'actor_movement_uses_producers':False,'goal_status':'active'}))
if __name__=='__main__':main()
