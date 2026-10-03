"""Bind original world-coordinate routing and native asset ownership to APKs."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_navigation_checkpoint import ROOT,REPO,digest,read,log
from validate_collision_checkpoint import verify_capture
from validate_selector_checkpoint import fields

TOTALS={'direct':11,'graph':109,'failed':405,'floor_queries':981,'cache_rejections':32}
PROBE={'floor_pairs':64,'successful':64,'direct':0,'graph':64,'segments':2086,'state_fnv1a64':'3a3ab2a724cc383b'}

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args()
 local=REPO/'.local-inputs';report_dir=REPO/'port/level-world/reports';prior_path=ROOT/'reports/build-validation-navigation-search.json';prior=read(prior_path)
 apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';studio_apk=a.studio/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libs=inspect(apk);studio_libs=inspect(studio_apk);assert len(libs)==len(studio_libs)==14
 engine=local/'libDungeonHunter2.so';engine_sha=digest(engine);reference=digest(local/'navigation-world-original.bin');linked=digest(local/'navigation-link-original.bin');floor_sha=digest(local/'authored-floors-packaged.json');assert linked==prior['linked_reference_sha256'] and floor_sha==prior['floor_geometry_sha256']
 reports={};library_sha={}
 for tag in ('oracle','packaged','studio'):
  library=local/('navigation-world-oracle.so' if tag=='oracle' else f'navigation-world-{tag}-world-arm64.so');library_sha[tag]=digest(library)
  r=read(report_dir/('navigation-world-arm64-differential.json' if tag=='oracle' else f'navigation-world-{tag}-arm64-differential.json'));fields(r,{'comparisons':525,'mismatches':0});fields(r['totals'],TOTALS);fields(r['crypt_world_route_probe'],PROBE)
  fixture=local/('navigation-world-original.bin' if tag=='oracle' else f'navigation-world-{tag}-original.bin');assert r['reference_sha256']==digest(fixture)==reference
  assert r['original_sha256']==engine_sha and r['arm64_library_sha256']==library_sha[tag] and r['linked_reference_sha256']==linked and r['floor_source_sha256']==floor_sha;reports[tag]=r
 host=read(report_dir/'navigation-world-host-audit.json');fields(host,{'cases':525,'sessions':346,'direct_routes':11,'graph_routes':109,'failed_routes':405,'referenced_floor_queries':981,'atomic_rejection_checks':5,'authored_graph_nodes':335,'authored_graph_edges':838,'mismatches':0,'reference_sha256':reference,'sanitizers':['address','undefined']})
 core=read(report_dir/'navigation-world-search-host-regression.json');fields(core,{'cases':1084,'predicate_call_comparisons':101090,'search_node_record_comparisons':96279,'successful_searches':237,'authored_crypt_cases':256,'atomic_rejection_checks':5,'mismatches':0,'reference_sha256':prior['reference_sha256'],'sanitizers':['address','undefined']})
 world=read(local/'world-tests-navigation-world/world-smoke.json');fields(world,{'apk_sha256':sha,'installed_apk_sha256':sha,'native_floor_records':8,'native_graph_nodes':335,'native_graph_edges':838,'native_neighbour_floor_relations':14,'native_validation_references':998,'native_floor_sewing_used_by_level_load':True,'native_route_search_used_by_gameplay':False,'native_graph_node_search_used_by_startup_probe':True,'native_world_coordinate_search_used_by_startup_probe':True,'world_endpoint_selection_reconstructed':True,'native_selector_collision_used_by_height':True,'original_movement_controller_reconstructed':False,'original_smoothing_reconstructed':False,'original_findpath_reconstructed':False,'frozen_idle_stable':True,'live_idle_changed_pixels':True,'rotation_position_preserved':True,'pause_cancels_movement':True});fields(world['crypt_world_route_probe'],PROBE);assert world['crypt_route_probe']==prior['crypt_route_probe'] and len(world['cases'])==10 and world['floor_geometry_sha256']==floor_sha
 assets={};unchanged=[];required={'dh2_nav_route','dh2_nav_world_collision','dh2_nav_search','dh2_nav_link','dh2_nav_triangle','dh2_selector_floor','dh2_octree_build'}
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
 for r in (host,core):assert r['bres_sha256']==assets['worlds/crypt.bdae']['sha256'] and r['descriptor_sha256']==assets['worlds/crypt01.dwld']['sha256']
 source={}
 for name,expected in prior['studio_source_sha256'].items():
  value=digest(ROOT/'app/src/main'/name);assert value==digest(a.studio/'app/src/main'/name)
  if name!='cpp/model_renderer.cpp':assert value==expected,name
  source[name]=value
 changed={'android-native/README.md','android-native/ROADMAP.md','android-native/tools/world_smoke.py','level-world/CMakeLists.txt','level-world/README.md','level-world/floors.cpp','level-world/floors.hpp'}
 for name,expected in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==expected,name
 additions={'level-world/navigation_world.cpp','level-world/navigation_world.hpp','level-world/tests/navigation_world.cpp','level-world/tests/navigation_world_differential.py','level-world/tools/build_navigation_world_oracle.ps1','android-native/tools/validate_navigation_world_checkpoint.py'}
 additions|={f.relative_to(REPO/'port').as_posix() for f in (REPO/'port/level-world/reference/navigation-world').rglob('*') if f.is_file()}
 verify_capture(REPO/'port/level-world/reference/navigation-world/original-functions.json',engine)
 for name in ('navigation-world-repo-build.log','navigation-world-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'navigation-world-zipalign.log')
 for f in (local/'world-tests-navigation-world').glob('*.log'):
  text=log(f);assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|Shader failed|Link failed',text),f
  assert 'Native world route probe | floor pairs 64 | successful 64 | direct 0 | graph 64 | segments 2086 | state 3a3ab2a724cc383b | smoothing and movement pending' in text,f
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 result={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(studio_apk),'libraries':libs,'studio_libraries':studio_libs,'unchanged_libraries':unchanged,'assets_verified':assets,'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'studio_source_sha256':source,'module_source_sha256':{n:digest(REPO/'port'/n) for n in sorted(set(prior['module_source_sha256'])|changed|additions)},'navigation_world_reports':reports,'reference_sha256':reference,'linked_reference_sha256':linked,'navigation_world_host_audit':host,'search_host_regression':core,'crypt_world_route_probe':PROBE,'floor_geometry_sha256':floor_sha,'world_cases':10,'apk_16k_zip_alignment_verified':True,'prior_graph_search_checkpoint':{'apk_sha256':prior['apk_sha256'],'validation_sha256':digest(prior_path)},'prior_combat_checkpoint':prior['prior_combat_checkpoint'],'world_coordinate_search_used_by_startup_probe':True,'world_coordinate_search_used_by_actor_movement':False,'original_world_endpoint_selection_reconstructed':True,'original_ordinary_collision_traversal_reconstructed':True,'original_radius_capability_predicates_reconstructed':True,'original_failed_pair_cache_behavior_reconstructed':True,'original_cache_invalidation_lifecycle_reconstructed':False,'original_movement_controller_reconstructed':False,'original_metadata_parser_reconstructed':False,'original_full_findpath_reconstructed':False,'original_smoothing_reconstructed':False,'pending_navigation':'Original PFObject initialization, failed-cache lifecycle, complete FindPath, smoothing, waypoints/movement controller, obstacle avoidance, metadata and generated-room lifecycle.','physical_arm64_tested':False,'original_gpu_parity_verified':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 (ROOT/'reports/build-validation-navigation-world.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8');(ROOT/'reports/world-smoke-navigation-world.json').write_text(json.dumps(world,indent=2)+'\n',encoding='utf-8')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-world-routes-{sha[:8]}.apk';checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'checkpoint':str(checkpoint),'world_route_cases_per_arm64_binary':525,'crypt_world_route_probe':PROBE,'world_cases':10,'actor_movement_uses_world_routes':False,'goal_status':'active'}))
if __name__=='__main__':main()
