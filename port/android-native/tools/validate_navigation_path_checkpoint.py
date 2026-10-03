"""Bind reconstructed FindPath/smoothing/lifecycle to the native APK checkpoint."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_navigation_checkpoint import ROOT,REPO,digest,read,log
from validate_collision_checkpoint import verify_capture
from validate_selector_checkpoint import fields

PROBE={'floor_pairs':64,'successful':64,'owned':64,'segments':2086,'state_fnv1a64':'9356b419cae2bc57'}
TOTALS={'direct':9,'graph':127,'failed':230,'floor_queries':669,'cached_failures':45,'replaced_paths':131}
OPS=[0,643,322,321,325,965,1933]
KINDS=[6,2,296,103,104,150]
def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args();local=REPO/'.local-inputs';report_dir=REPO/'port/level-world/reports';prior_path=ROOT/'reports/build-validation-navigation-world.json';prior=read(prior_path)
 apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';studio_apk=a.studio/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libs=inspect(apk);studio_libs=inspect(studio_apk);assert len(libs)==len(studio_libs)==14
 engine=local/'libDungeonHunter2.so';engine_sha=digest(engine);linked=digest(local/'navigation-link-original.bin');floor_sha=digest(local/'authored-floors-packaged.json');assert linked==prior['linked_reference_sha256'] and floor_sha==prior['floor_geometry_sha256']
 reports={};references={};library_sha={}
 for tag in ('oracle','packaged','studio'):
  library=local/('navigation-path-oracle.so' if tag=='oracle' else f'navigation-path-{tag}-world-arm64.so');library_sha[tag]=digest(library);reports[tag]={}
  for kind in ('path','find'):
   report=read(report_dir/(f'navigation-{kind}-arm64-differential.json' if tag=='oracle' else f'navigation-{kind}-{tag}-arm64-differential.json'));fixture=local/(f'navigation-{kind}-original.bin' if tag=='oracle' else f'navigation-{kind}-{tag}-original.bin');reference=digest(fixture)
   assert report['reference_sha256']==reference==digest(local/f'navigation-{kind}-original.bin');assert report['original_sha256']==engine_sha and report['arm64_library_sha256']==library_sha[tag] and report['linked_reference_sha256']==linked;fields(report,{'mismatches':0})
   if kind=='path':fields(report,{'line_comparisons':661,'path_operation_comparisons':4509,'sessions':323,'operation_counts':OPS,'line_classifications':KINDS,'owned_edge_destructors_execute':True})
   else:fields(report,{'comparisons':366,'sessions':46,'totals':TOTALS,'crypt_findpath_probe':PROBE,'floor_source_sha256':floor_sha})
   references[kind]=reference;reports[tag][kind]=report
 host={name:read(report_dir/f'navigation-{name}-host-audit.json') for name in ('path','find','world-regression','search-regression')}
 fields(host['path'],{'line_comparisons':661,'path_operation_comparisons':4509,'line_classifications':KINDS,'operation_counts':OPS,'atomic_rejection_checks':3,'reference_sha256':references['path']})
 fields(host['find'],{'cases':366,'sessions':46,'direct_paths':9,'graph_paths':127,'failed_paths':230,'referenced_floor_queries':669,'atomic_rejection_checks':6,'authored_graph_nodes':335,'authored_graph_edges':838,'reference_sha256':references['find']})
 fields(host['world-regression'],{'cases':525,'sessions':346,'atomic_rejection_checks':5,'reference_sha256':prior['reference_sha256']})
 fields(host['search-regression'],{'cases':1084,'predicate_call_comparisons':101090,'search_node_record_comparisons':96279,'atomic_rejection_checks':5,'reference_sha256':digest(local/'navigation-search-original.bin')})
 for report in host.values():fields(report,{'mismatches':0,'sanitizers':['address','undefined']})
 world=read(local/'world-tests-navigation-path/world-smoke.json');fields(world,{'apk_sha256':sha,'installed_apk_sha256':sha,'native_floor_records':8,'native_graph_nodes':335,'native_graph_edges':838,'native_neighbour_floor_relations':14,'native_validation_references':998,'native_findpath_used_by_startup_probe':True,'original_smoothing_reconstructed':True,'original_findpath_reconstructed':True,'original_waypoint_path_step_reconstructed':True,'native_findpath_used_by_actor_movement':False,'original_movement_controller_reconstructed':False,'frozen_idle_stable':True,'live_idle_changed_pixels':True,'rotation_position_preserved':True,'pause_cancels_movement':True,'crypt_findpath_probe':PROBE});assert len(world['cases'])==10 and world['floor_geometry_sha256']==floor_sha
 assets={};unchanged=[];required={'dh2_nav_find_path','dh2_nav_smooth_path','dh2_nav_move_path','dh2_nav_calc_waypoint','dh2_nav_past_waypoint','dh2_nav_drop_path','dh2_nav_path_length','dh2_nav_line_intersection','dh2_nav_route','dh2_nav_search'}
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
  if name!='path':assert report['bres_sha256']==assets['worlds/crypt.bdae']['sha256'] and report['descriptor_sha256']==assets['worlds/crypt01.dwld']['sha256']
 source={}
 for name,expected in prior['studio_source_sha256'].items():
  value=digest(ROOT/'app/src/main'/name);assert value==digest(a.studio/'app/src/main'/name)
  if name!='cpp/model_renderer.cpp':assert value==expected,name
  source[name]=value
 changed={'android-native/README.md','android-native/ROADMAP.md','android-native/tools/world_smoke.py','level-world/CMakeLists.txt','level-world/README.md','level-world/floors.cpp','level-world/floors.hpp'}
 for name,expected in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==expected,name
 additions={'level-world/navigation_path.cpp','level-world/navigation_path.hpp','level-world/tests/navigation_path.cpp','level-world/tests/navigation_path_differential.py','level-world/tests/navigation_find.cpp','level-world/tests/navigation_find_differential.py','level-world/tools/build_navigation_path_oracle.ps1','android-native/tools/validate_navigation_path_checkpoint.py'}
 additions|={f.relative_to(REPO/'port').as_posix() for f in (REPO/'port/level-world/reference/navigation-path').rglob('*') if f.is_file()};verify_capture(REPO/'port/level-world/reference/navigation-path/original-functions.json',engine)
 for name in ('navigation-path-repo-build.log','navigation-path-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert log(local/'navigation-path-zipalign.log').count('Verification successful')==2
 marker='Native FindPath probe | floor pairs 64 | successful 64 | owned 64 | segments 2086 | state 9356b419cae2bc57 | position controller pending'
 for f in (local/'world-tests-navigation-path').glob('*.log'):
  text=log(f);assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|Shader failed|Link failed',text),f;assert marker in text,f
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 result={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(studio_apk),'libraries':libs,'studio_libraries':studio_libs,'unchanged_libraries':unchanged,'assets_verified':assets,'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'studio_source_sha256':source,'module_source_sha256':{n:digest(REPO/'port'/n) for n in sorted(set(prior['module_source_sha256'])|changed|additions)},'path_reports':reports,'reference_sha256':references,'linked_reference_sha256':linked,'host_audits':host,'crypt_findpath_probe':PROBE,'floor_geometry_sha256':floor_sha,'world_cases':10,'apk_16k_zip_alignment_verified':True,'prior_world_checkpoint':{'apk_sha256':prior['apk_sha256'],'validation_sha256':digest(prior_path)},'prior_combat_checkpoint':prior['prior_combat_checkpoint'],'original_findpath_timing_disabled_reconstructed':True,'original_smoothing_reconstructed':True,'original_waypoint_path_step_reconstructed':True,'findpath_used_by_startup_probe':True,'findpath_used_by_actor_movement':False,'original_pfobject_initialization_reconstructed':False,'original_cache_invalidation_lifecycle_reconstructed':False,'original_movement_controller_reconstructed':False,'original_metadata_parser_reconstructed':False,'pending_navigation':'ValidatePosition/ValidateDirection, obstacle response, PFObject producers, cache lifecycle, original character movement/controller and moving pursuit.','physical_arm64_tested':False,'original_gpu_parity_verified':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 (ROOT/'reports/build-validation-navigation-path.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8');(ROOT/'reports/world-smoke-navigation-path.json').write_text(json.dumps(world,indent=2)+'\n',encoding='utf-8');checkpoint=ROOT/'build/checkpoints'/f'dh2-native-findpath-{sha[:8]}.apk';checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'checkpoint':str(checkpoint),'findpath_requests_per_arm64_binary':366,'line_cases_per_arm64_binary':661,'path_operations_per_arm64_binary':4509,'crypt_findpath_probe':PROBE,'world_cases':10,'actor_movement_uses_findpath':False,'goal_status':'active'}))
if __name__=='__main__':main()
