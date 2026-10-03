"""Bind original floor sewing and live post-load graph to both built APKs."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_navigation_checkpoint import ROOT,REPO,digest,read,log
from validate_collision_checkpoint import verify_capture
from validate_selector_checkpoint import fields
from validate_floor_records_checkpoint import RECORDS,GRAPH

SEWING={'cases':259,'initial_triangle_comparisons':2147,'logical_snapshot_comparisons':1643,
 'link_calls':869,'forced_nodes_created':812,'mismatches':0,'distance_threshold_passes':516,
 'vertical_threshold_passes':404,'postload_caller_order_comparisons':259}
CRYPT={'nodes':335,'edges':838,'validation_references':998,'state_fnv1a64':'57999e27060699df','neighbour_floor_relations':14}

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args()
 local=REPO/'.local-inputs';report_dir=REPO/'port/level-world/reports';prior_path=ROOT/'reports/build-validation-floor-records.json';prior=read(prior_path)
 apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';studio_apk=a.studio/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk)
 libs=inspect(apk);studio_libs=inspect(studio_apk);assert len(libs)==len(studio_libs)==14
 engine=local/'libDungeonHunter2.so';engine_sha=digest(engine);floor_sha=digest(local/'authored-floors-packaged.json');input_sha=digest(local/'crypt-linked-floor-inputs.bin')
 assert floor_sha==prior['floor_geometry_sha256'] and input_sha==prior['authored_floor_input_sha256']
 reference=digest(local/'navigation-link-original.bin');reports={};library_sha={};asset_library_sha={};builder_reference=None
 connectivity=read(report_dir/'navigation-link-connectivity-audit.json');fields(connectivity,{'reference_sha256':reference,'nodes':335,'edges':838,'graph_components':[{'nodes':335,'floors':list(range(8))}]});assert len(connectivity['actual_cross_floor_edge_pairs'])==14
 for tag in ('oracle','packaged','studio'):
  library=local/('navigation-link-oracle.so' if tag=='oracle' else f'navigation-link-{tag}-world-arm64.so');library_sha[tag]=digest(library)
  r=read(report_dir/('navigation-link-arm64-differential.json' if tag=='oracle' else f'navigation-link-{tag}-arm64-differential.json'));fields(r,SEWING);fields(r['crypt'],CRYPT)
  fixture=local/('navigation-link-original.bin' if tag=='oracle' else f'navigation-link-{tag}-original.bin')
  assert r['reference_sha256']==digest(fixture)==reference and r['original_sha256']==engine_sha and r['arm64_library_sha256']==library_sha[tag] and r['floor_source_sha256']==floor_sha
  reports['sewing-'+tag]=r
  if tag!='oracle':
   asset_library_sha[tag]=digest(local/f'navigation-link-{tag}-assets-arm64.so')
   r=read(report_dir/f'navigation-link-floor-records-{tag}-arm64-differential.json');fields(r,RECORDS)
   assert r['arm64_library_sha256']==library_sha[tag] and r['asset_library_sha256']==asset_library_sha[tag] and r['original_sha256']==engine_sha and r['input_sha256']==input_sha
   assert r['floor_source_sha256']==digest(local/f'navigation-link-{tag}-floors.json')==floor_sha;reports['floor-records-'+tag]=r
   r=read(report_dir/f'navigation-link-builder-{tag}-arm64-differential.json');fields(r,GRAPH)
   assert r['arm64_library_sha256']==library_sha[tag] and r['original_sha256']==engine_sha and r['floor_source_sha256']==floor_sha
   assert r['reference_sha256']==digest(local/f'navigation-link-builder-{tag}-original.bin')==prior['reference_sha256']['graph'];builder_reference=r['reference_sha256'];reports['builder-'+tag]=r
 host=read(report_dir/'navigation-link-host-audit.json');fields(host,{'cases':259,'logical_snapshot_comparisons':877,'link_calls':869,'floor_postloads':8,'atomic_rejection_checks':2,'crypt_nodes':335,'crypt_edges':838,'crypt_state_fnv1a64':CRYPT['state_fnv1a64'],'mismatches':0,'reference_sha256':reference})
 authored=read(report_dir/'navigation-link-authored-floors-host-audit.json');fields(authored,{'floor_records':8,'mesh_parts':8,'triangle_count':314,'octree_nodes':34,'graph_nodes':335,'graph_edges':838,'graph_invalid_archive':300,'graph_validation':998,'linked_state_fnv1a64':CRYPT['state_fnv1a64'],'neighbour_floor_relations':14,'floor_sewing_completed':True,'collision_used_by_height':True,'route_search_used_by_movement':False,'input_sha256':input_sha})
 world_host=read(report_dir/'navigation-link-world-host-audit.json');fields(world_host,{'rooms':8,'nodes':146,'navigation_triangles':314,'stairs_steps':220,'synthetic_movement_checks':9,'mutated_descriptors':2000})
 builder_host=read(report_dir/'navigation-link-builder-host-regression.json');fields(builder_host,{'triangle_comparisons':314,'floor_query_comparisons':1239,'native_floor_collision_used':True,'native_octree_selector_used':True,'mismatches':0,'reference_sha256':builder_reference})
 for r in (host,authored,world_host,builder_host):assert r['sanitizers']==['address','undefined']
 world=read(local/'world-tests-navigation-link/world-smoke.json');fields(world,{'apk_sha256':sha,'installed_apk_sha256':sha,'floor_geometry_sha256':floor_sha,'native_floor_records':8,'native_graph_nodes':335,'native_graph_edges':838,'native_neighbour_floor_relations':14,'native_validation_references':998,'native_floor_sewing_used_by_level_load':True,'native_route_search_used_by_gameplay':False,'native_selector_collision_used_by_height':True,'original_movement_controller_reconstructed':False,'frozen_idle_stable':True,'live_idle_changed_pixels':True,'rotation_position_preserved':True,'pause_cancels_movement':True});assert len(world['cases'])==10
 assets={};unchanged=[];required={'dh2_nav_link','dh2_nav_bounds_overlap','dh2_nav_triangle','dh2_selector_floor','dh2_octree_build','dh2_collision_floor','dh2_floor_clone_matrix','dh2_floor_raise_triangles'}
 with zipfile.ZipFile(apk) as z,zipfile.ZipFile(studio_apk) as sz:
  assert set(z.namelist())==set(sz.namelist()) and not any('DungeonHunter2.so' in n for n in z.namelist())
  for name in z.namelist():
   if name.startswith('assets/') and not name.endswith('/'):
    raw=z.read(name);relative=name[7:];assert raw==sz.read(name)==(ROOT/'app/src/main/assets'/relative).read_bytes()==(a.studio/'app/src/main/assets'/relative).read_bytes()
    assets[relative]={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}
   elif name.startswith('lib/') and name.endswith('/libdh2_level_world.so'):assert required<=exports(z.read(name)) and required<=exports(sz.read(name))
  assert assets==prior['assets_verified'] and len(assets)==126
  for tag,archive in [('packaged',z),('studio',sz)]:
   assert hashlib.sha256(archive.read('lib/arm64-v8a/libdh2_level_world.so')).hexdigest()==library_sha[tag]
   assert hashlib.sha256(archive.read('lib/arm64-v8a/libdh2_scene_materials.so')).hexdigest()==asset_library_sha[tag]
  for rows,archive in [(prior['libraries'],z),(prior['studio_libraries'],sz)]:
   for row in rows:
    if Path(row['path']).name not in {'libdh2_level_world.so','libdh2_native.so'}:
     assert hashlib.sha256(archive.read(row['path'])).hexdigest()==row['sha256']
     if archive is z:unchanged.append(row['path'])
 for r in (authored,world_host):assert r['bres_sha256']==assets['worlds/crypt.bdae']['sha256'] and r['descriptor_sha256']==assets['worlds/crypt01.dwld']['sha256']
 source={}
 for name,expected in prior['studio_source_sha256'].items():
  value=digest(ROOT/'app/src/main'/name);assert value==digest(a.studio/'app/src/main'/name)
  if name!='cpp/model_renderer.cpp':assert value==expected,name
  source[name]=value
 changed={'android-native/README.md','android-native/ROADMAP.md','android-native/tools/world_smoke.py','level-world/CMakeLists.txt','level-world/README.md','level-world/floors.cpp','level-world/floors.hpp','level-world/navigation.cpp','level-world/navigation.hpp','level-world/world.cpp','level-world/tools/inspect_floors.cpp','level-world/reference/floor-records/NOTES.md','level-world/reference/navigation/NOTES.md'}
 for name,expected in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==expected,name
 additions={'level-world/tests/navigation_link.cpp','level-world/tests/navigation_link_differential.py','level-world/tools/navigation_state.hpp','android-native/tools/validate_navigation_link_checkpoint.py'}
 additions|={f.relative_to(REPO/'port').as_posix() for f in (REPO/'port/level-world/reference/navigation-link').rglob('*') if f.is_file()}
 for folder in ('navigation-link','floor-records','floor-source','selector','navigation','collision','octree'):verify_capture(REPO/'port/level-world/reference'/folder/'original-functions.json',engine)
 for name in ('navigation-link-repo-build.log','navigation-link-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'navigation-link-zipalign.log')
 for f in (local/'world-tests-navigation-link').glob('*.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|Shader failed|Link failed',log(f)),f
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 result={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(studio_apk),'libraries':libs,'studio_libraries':studio_libs,'unchanged_libraries':unchanged,'assets_verified':assets,'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'studio_source_sha256':source,'module_source_sha256':{n:digest(REPO/'port'/n) for n in sorted(set(prior['module_source_sha256'])|changed|additions)},'navigation_link_reports':reports,'reference_sha256':reference,'builder_reference_sha256':builder_reference,'authored_floor_input_sha256':input_sha,'floor_geometry_sha256':floor_sha,'navigation_link_host_audit':host,'authored_floors_host_audit':authored,'world_host_audit':world_host,'builder_host_regression':builder_host,'world_cases':10,'apk_16k_zip_alignment_verified':True,'prior_floor_records_checkpoint':{'apk_sha256':prior['apk_sha256'],'validation_sha256':digest(prior_path)},'prior_combat_checkpoint':prior['prior_combat_checkpoint'],'actual_crypt_floor_records_built':True,'selector_collision_used_by_gameplay_height':True,'graph_constructed_in_gameplay':True,'cross_floor_sewing_used_by_level_load':True,'original_postload_overlap_and_call_order_verified':True,'graph_used_by_route_search':False,'original_movement_controller_reconstructed':False,'full_original_floor_loader_reconstructed':False,'original_metadata_parser_reconstructed':False,'scene_ownership_is_modern_adapter':True,'invalid_geometry_archive_is_modern_adapter':True,'pending_navigation':'Full metadata/lifecycle, rotated/generated rooms, route search, endpoint selection, smoothing, obstacles and original current-floor/movement controller.','physical_arm64_tested':False,'original_gpu_parity_verified':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 result['linked_graph_connectivity_audit']=connectivity
 (ROOT/'reports/build-validation-navigation-link.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8');(ROOT/'reports/world-smoke-navigation-link.json').write_text(json.dumps(world,indent=2)+'\n',encoding='utf-8')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-floor-sewing-{sha[:8]}.apk';checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'checkpoint':str(checkpoint),'sewing_cases_per_arm64_binary':259,'sewing_calls_per_arm64_binary':869,'crypt_nodes':335,'crypt_edges':838,'neighbour_floor_relations':14,'world_cases':10,'route_search_used_by_gameplay':False,'goal_status':'active'}))
if __name__=='__main__':main()
