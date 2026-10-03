"""Bind reconstructed floor validation to APKs and actual authored geometry."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_navigation_checkpoint import ROOT,REPO,digest,read,log
from validate_collision_checkpoint import verify_capture
from validate_selector_checkpoint import fields

PROBE={'floor_pairs':64,'position_valid':64,'accepted':64,'direction_valid':64,'state_fnv1a64':'d60ad48039cc85c6'}
TOTALS={'height':528,'position':573,'direction':768,'segment':484,'floor_queries':2860,'accepted':316,'clamped':210,'equivalent':30,'position_miss':17,'direction_valid':722,'direction_rejected':46,'slide_attempts':171,'successful_slides':149,'parent_requests':264,'parent_changes':64}
def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args();local=REPO/'.local-inputs';rd=REPO/'port/level-world/reports';prior_path=ROOT/'reports/build-validation-navigation-path.json';prior=read(prior_path)
 apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';studio_apk=a.studio/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libs=inspect(apk);studio_libs=inspect(studio_apk);assert len(libs)==len(studio_libs)==14
 engine=local/'libDungeonHunter2.so';engine_sha=digest(engine);reference=digest(local/'navigation-motion-original.bin');linked=digest(local/'navigation-link-original.bin');floor_sha=digest(local/'authored-floors-packaged.json');assert linked==prior['linked_reference_sha256'] and floor_sha==prior['floor_geometry_sha256'];reports={};library_sha={}
 for tag in ('oracle','packaged','studio'):
  library=local/('navigation-motion-oracle.so' if tag=='oracle' else f'navigation-motion-{tag}-world-arm64.so');library_sha[tag]=digest(library);report=read(rd/('navigation-motion-arm64-differential.json' if tag=='oracle' else f'navigation-motion-{tag}-arm64-differential.json'));fixture=local/('navigation-motion-original.bin' if tag=='oracle' else f'navigation-motion-{tag}-original.bin');fields(report,{'comparisons':2353,'mismatches':0,'totals':TOTALS,'crypt_motion_probe':PROBE,'original_sha256':engine_sha,'arm64_library_sha256':library_sha[tag],'linked_reference_sha256':linked,'floor_source_sha256':floor_sha,'reference_sha256':reference});assert digest(fixture)==reference;reports[tag]=report
 host={name:read(rd/f'navigation-{name}-host-audit.json') for name in ('motion','motion-find-regression','motion-path-regression','motion-world-regression','motion-search-regression')}
 fields(host['motion'],{'cases':2353,'height_queries':528,'position_requests':573,'direction_requests':768,'segment_requests':484,'referenced_floor_queries':2860,'atomic_rejection_checks':6,'authored_graph_nodes':335,'authored_graph_edges':838,'reference_sha256':reference})
 fields(host['motion-find-regression'],{'cases':366,'sessions':46,'direct_paths':9,'graph_paths':127,'failed_paths':230,'atomic_rejection_checks':6,'reference_sha256':prior['reference_sha256']['find']})
 fields(host['motion-path-regression'],{'line_comparisons':661,'path_operation_comparisons':4509,'atomic_rejection_checks':3,'reference_sha256':prior['reference_sha256']['path']})
 fields(host['motion-world-regression'],{'cases':525,'sessions':346,'atomic_rejection_checks':5,'reference_sha256':digest(local/'navigation-world-original.bin')})
 fields(host['motion-search-regression'],{'cases':1084,'predicate_call_comparisons':101090,'search_node_record_comparisons':96279,'atomic_rejection_checks':5,'reference_sha256':digest(local/'navigation-search-original.bin')})
 for report in host.values():fields(report,{'mismatches':0,'sanitizers':['address','undefined']})
 world=read(local/'world-tests-navigation-motion/world-smoke.json');fields(world,{'apk_sha256':sha,'installed_apk_sha256':sha,'native_floor_records':8,'native_graph_nodes':335,'native_graph_edges':838,'native_neighbour_floor_relations':14,'native_validation_references':998,'native_floor_motion_used_by_startup_probe':True,'original_position_validation_reconstructed':True,'original_floor_direction_validation_reconstructed':True,'original_obstacle_parent_backend_reconstructed':False,'native_floor_motion_used_by_actor_movement':False,'original_movement_controller_reconstructed':False,'frozen_idle_stable':True,'live_idle_changed_pixels':True,'rotation_position_preserved':True,'pause_cancels_movement':True,'crypt_motion_probe':PROBE});assert len(world['cases'])==10 and world['floor_geometry_sha256']==floor_sha and world['crypt_findpath_probe']==prior['crypt_findpath_probe']
 assets={};unchanged=[];required={'dh2_nav_floor_height','dh2_nav_room_height','dh2_nav_world_height','dh2_nav_validate_position','dh2_nav_validate_direction','dh2_nav_segment_intersect','dh2_nav_find_path','dh2_nav_move_path','dh2_nav_route','dh2_nav_search'}
 with zipfile.ZipFile(apk) as z,zipfile.ZipFile(studio_apk) as sz:
  assert set(z.namelist())==set(sz.namelist()) and not any('DungeonHunter2.so' in n for n in z.namelist())
  for name in z.namelist():
   if name.startswith('assets/') and not name.endswith('/'):
    raw=z.read(name);relative=name[7:];assert raw==sz.read(name)==(ROOT/'app/src/main/assets'/relative).read_bytes()==(a.studio/'app/src/main/assets'/relative).read_bytes();assets[relative]={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}
   elif name.startswith('lib/') and name.endswith('/libdh2_level_world.so'):assert required<=exports(z.read(name)) and required<=exports(sz.read(name))
  assert assets==prior['assets_verified'] and len(assets)==126
  for tag,archive in [('packaged',z),('studio',sz)]:assert hashlib.sha256(archive.read('lib/arm64-v8a/libdh2_level_world.so')).hexdigest()==library_sha[tag]
  for rows,archive in [(prior['libraries'],z),(prior['studio_libraries'],sz)]:
   for row in rows:
    if Path(row['path']).name not in {'libdh2_level_world.so','libdh2_native.so'}:
     assert hashlib.sha256(archive.read(row['path'])).hexdigest()==row['sha256']
     if archive is z:unchanged.append(row['path'])
 for name,report in host.items():
  if name!='motion-path-regression':assert report['bres_sha256']==assets['worlds/crypt.bdae']['sha256'] and report['descriptor_sha256']==assets['worlds/crypt01.dwld']['sha256']
 source={}
 for name,expected in prior['studio_source_sha256'].items():
  value=digest(ROOT/'app/src/main'/name);assert value==digest(a.studio/'app/src/main'/name)
  if name!='cpp/model_renderer.cpp':assert value==expected,name
  source[name]=value
 changed={'android-native/README.md','android-native/ROADMAP.md','android-native/tools/world_smoke.py','level-world/CMakeLists.txt','level-world/README.md','level-world/floors.hpp'}
 for name,expected in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==expected,name
 additions={'level-world/navigation_motion.cpp','level-world/navigation_motion.hpp','level-world/tests/navigation_motion.cpp','level-world/tests/navigation_motion_differential.py','level-world/tools/build_navigation_motion_oracle.ps1','android-native/tools/validate_navigation_motion_checkpoint.py'}
 additions|={f.relative_to(REPO/'port').as_posix() for f in (REPO/'port/level-world/reference/navigation-motion').rglob('*') if f.is_file()};verify_capture(REPO/'port/level-world/reference/navigation-motion/original-functions.json',engine)
 for name in ('navigation-motion-repo-build.log','navigation-motion-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert log(local/'navigation-motion-zipalign.log').count('Verification successful')==2
 marker='Native floor motion probe | floor pairs 64 | position valid 64 | accepted 64 | direction valid 64 | state d60ad48039cc85c6 | controller and dynamic obstacles pending'
 for f in (local/'world-tests-navigation-motion').glob('*.log'):
  text=log(f);assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|Shader failed|Link failed',text),f;assert marker in text,f
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 result={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(studio_apk),'libraries':libs,'studio_libraries':studio_libs,'unchanged_libraries':unchanged,'assets_verified':assets,'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'studio_source_sha256':source,'module_source_sha256':{n:digest(REPO/'port'/n) for n in sorted(set(prior['module_source_sha256'])|changed|additions)},'motion_reports':reports,'reference_sha256':reference,'linked_reference_sha256':linked,'host_audits':host,'crypt_motion_probe':PROBE,'crypt_findpath_probe':prior['crypt_findpath_probe'],'floor_geometry_sha256':floor_sha,'world_cases':10,'apk_16k_zip_alignment_verified':True,'prior_findpath_checkpoint':{'apk_sha256':prior['apk_sha256'],'validation_sha256':digest(prior_path)},'prior_combat_checkpoint':prior['prior_combat_checkpoint'],'original_position_validation_reconstructed':True,'original_floor_direction_validation_reconstructed':True,'original_height_normals_reconstructed':True,'original_special_inclusive_height_queries_reconstructed':True,'floor_motion_used_by_startup_probe':True,'floor_motion_used_by_actor_movement':False,'original_obstacle_parent_backend_reconstructed':False,'original_obstacle_force_reconstructed':False,'original_pfobject_initialization_reconstructed':False,'original_cache_invalidation_lifecycle_reconstructed':False,'original_movement_controller_reconstructed':False,'pending_navigation':'PFObject InitObject/InitObstacle and actor producers; obstacle-parent lists and force/avoidance; cache lifecycle; character controller/root motion and moving pursuit.','physical_arm64_tested':False,'original_gpu_parity_verified':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 (ROOT/'reports/build-validation-navigation-motion.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8');(ROOT/'reports/world-smoke-navigation-motion.json').write_text(json.dumps(world,indent=2)+'\n',encoding='utf-8');checkpoint=ROOT/'build/checkpoints'/f'dh2-native-floor-motion-{sha[:8]}.apk';checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'checkpoint':str(checkpoint),'motion_comparisons_per_arm64_binary':2353,'crypt_motion_probe':PROBE,'world_cases':10,'actor_movement_uses_motion':False,'goal_status':'active'}))
if __name__=='__main__':main()
