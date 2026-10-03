"""Bind authored floors, live collision and graph construction to both APKs.

Route search, full floor loading/lifecycle and the original movement controller
remain pending. Historical combat results retain their own checkpoint identity.
"""
import argparse
import hashlib
import json
import re
import subprocess
import zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_navigation_checkpoint import ROOT, REPO, digest, read, log
from validate_collision_checkpoint import verify_capture
from validate_floor_source_checkpoint import FLOOR
from validate_selector_checkpoint import fields

RECORDS = {'authored_floor_records':8, 'authored_mesh_constructor_comparisons':8,
 'authored_triangle_comparisons':314, 'authored_bounds_comparisons':8,
 'authored_raised_triangle_comparisons':314, 'clone_transform_comparisons':208,
 'original_copy_mesh_scene_node_calls':208, 'resource_copy_services':208,
 'clone_constructor_services':208, 'mismatches':0}
GRAPH = {'triangle_comparisons':314, 'floor_query_comparisons':1239,
 'original_floor_collision_calls':1239, 'mismatches':0,
 'bounds_from_authored_floor_records':True, 'object_flags_from_authored_floor_records':True,
 'floor_support_answers_are_caller_fixtures':False, 'native_octree_selector_used':True,
 'original_octree_selector_executes':True, 'selector_triangle_order_is_caller_fixture':False,
 'last_graph_counts':[321,778,300,942], 'last_graph_state_fnv1a64':'542a55a83f199cd9'}

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True)
 p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args()
 local=REPO/'.local-inputs';reports_dir=REPO/'port/level-world/reports'
 apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';studio_apk=a.studio/'app/build/outputs/apk/debug/app-debug.apk'
 prior_path=ROOT/'reports/build-validation-floor-source.json';prior=read(prior_path)
 sha=digest(apk);libs=inspect(apk);studio_libs=inspect(studio_apk);assert len(libs)==len(studio_libs)==14
 engine=local/'libDungeonHunter2.so';engine_sha=digest(engine)
 input_sha=digest(local/'crypt-floor-inputs.bin');floor_sha=digest(local/'authored-floors-packaged.json')
 assert floor_sha==digest(local/'authored-floors-studio.json')
 floor=read(local/'authored-floors-packaged.json');assert len(floor['floor'])==314
 assert floor['floor_object_flags']=={str(i):1 for i in range(8)} and len(floor['floor_bounds'])==8
 reports={};library_sha={};asset_library_sha={};references={}
 for tag in ('packaged','studio'):
  library_sha[tag]=digest(local/f'floor-records-{tag}-world-arm64.so')
  asset_library_sha[tag]=digest(local/f'floor-records-{tag}-assets-arm64.so')
  for kind,prefix,expected in [('records','authored-floors',RECORDS),('mesh','floor-source-records',FLOOR),('graph','authored-navigation-records',GRAPH)]:
   r=read(reports_dir/f'{prefix}-{tag}-arm64-differential.json');fields(r,expected)
   assert r['original_sha256']==engine_sha and r['arm64_library_sha256']==library_sha[tag]
   if kind!='graph':assert r['asset_library_sha256']==asset_library_sha[tag]
   if kind=='records':assert r['input_sha256']==input_sha and r['floor_source_sha256']==floor_sha
   else:
    fixture=local/f'{prefix}-{tag}-original.bin';reference=digest(fixture)
    assert r['reference_sha256']==reference
    if kind in references:assert references[kind]==reference
    references[kind]=reference
    if kind=='mesh':assert r['native_attribute_calls']==5100 and reference==prior['reference_sha256']
    else:assert r['floor_source_sha256']==floor_sha
   reports[kind+'-'+tag]=r
 host=read(reports_dir/'authored-floors-host-audit.json')
 fields(host,{'floor_records':8,'mesh_parts':8,'triangle_count':314,'octree_nodes':34,
  'graph_nodes':321,'graph_edges':778,'graph_invalid':300,'graph_validation':942,
  'graph_state_fnv1a64':GRAPH['last_graph_state_fnv1a64'],'source_floor_overrides':0,
  'collision_used_by_height':True,'route_search_used_by_movement':False,'input_sha256':input_sha})
 host_regressions={}
 for name in ('navigation','floor-source','actor-clips','player-attack','player-death','locomotion'):
  r=read(reports_dir/f'floor-records-{name}-host-regression.json')
  assert r['sanitizers']==['address','undefined'];host_regressions[name]=r
 fields(host_regressions['navigation'],{'triangle_comparisons':314,'floor_query_comparisons':1239,
  'native_floor_collision_used':True,'native_octree_selector_used':True,'mismatches':0,'reference_sha256':references['graph']})
 fields(host_regressions['floor-source'],{'mesh_constructor_comparisons':328,'triangle_comparisons':850,
  'floor_tag_comparisons':212,'world_and_floor_bounds_comparisons':512,'storage_rejection_checks':249,'mismatches':0})
 clips=host_regressions['actor-clips'];assert len(clips['clips'])==26
 assert sum(c['poses'] for c in clips['clips'])==45218 and all(c['unsupported']==0 for c in clips['clips'])
 for name,poses in [('player-attack',3475),('player-death',1400)]:
  fields(host_regressions[name],{'every_millisecond_poses':poses,'warrior_skins':4,'finite_skinned_vertices':True,'original_owner_words_compared':896})
 locomotion=host_regressions['locomotion'];assert locomotion['skins']==4 and sum(c['sampled_poses'] for c in locomotion['clips'])==1868
 world_host=read(reports_dir/'native-floors-world-host-audit.json')
 fields(world_host,{'rooms':8,'nodes':146,'navigation_triangles':314,'stairs_steps':220,'synthetic_movement_checks':9,'mutated_descriptors':2000})
 for r in (host,world_host):assert r['sanitizers']==['address','undefined']
 world=read(local/'world-tests-floor-records/world-smoke.json')
 fields(world,{'apk_sha256':sha,'installed_apk_sha256':sha,'floor_geometry_sha256':floor_sha,
  'native_floor_records':8,'native_graph_nodes':321,'native_graph_edges':778,
  'native_selector_collision_used_by_height':True,'original_movement_controller_reconstructed':False,
  'frozen_idle_stable':True,'live_idle_changed_pixels':True,'rotation_position_preserved':True,'pause_cancels_movement':True})
 assert len(world['cases'])==10
 required={'dh2_floor_clone_matrix','dh2_floor_raise_triangles','dh2_floor_mesh_triangles',
  'dh2_floor_source_flags','dh2_floor_source_bounds','dh2_selector_floor','dh2_octree_build','dh2_collision_floor','dh2_nav_triangle'}
 assets={};unchanged=[]
 with zipfile.ZipFile(apk) as z,zipfile.ZipFile(studio_apk) as sz:
  assert set(z.namelist())==set(sz.namelist()) and not any('DungeonHunter2.so' in n for n in z.namelist())
  for name in z.namelist():
   if name.startswith('assets/') and not name.endswith('/'):
    raw=z.read(name);relative=name[7:]
    assert raw==sz.read(name)==(ROOT/'app/src/main/assets'/relative).read_bytes()==(a.studio/'app/src/main/assets'/relative).read_bytes()
    assets[relative]={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}
   elif name.startswith('lib/') and name.endswith('/libdh2_level_world.so'):
    assert required<=exports(z.read(name)) and required<=exports(sz.read(name))
  assert assets==prior['assets_verified'] and len(assets)==126
  for tag,archive in [('packaged',z),('studio',sz)]:
   assert hashlib.sha256(archive.read('lib/arm64-v8a/libdh2_level_world.so')).hexdigest()==library_sha[tag]
   assert hashlib.sha256(archive.read('lib/arm64-v8a/libdh2_scene_materials.so')).hexdigest()==asset_library_sha[tag]
  for row in prior['libraries']:
   if Path(row['path']).name in {'libdh2_engine_textures.so','libdh2_game_data.so'}:
    assert hashlib.sha256(z.read(row['path'])).hexdigest()==row['sha256'];unchanged.append(row['path'])
  for row in prior['studio_libraries']:
   if Path(row['path']).name in {'libdh2_engine_textures.so','libdh2_game_data.so'}:assert hashlib.sha256(sz.read(row['path'])).hexdigest()==row['sha256']
 for r in (host,world_host):
  assert r['bres_sha256']==assets['worlds/crypt.bdae']['sha256'] and r['descriptor_sha256']==assets['worlds/crypt01.dwld']['sha256']
 source={}
 for name,expected in prior['studio_source_sha256'].items():
  value=digest(ROOT/'app/src/main'/name);assert value==digest(a.studio/'app/src/main'/name)
  if name!='cpp/model_renderer.cpp':assert value==expected,name
  source[name]=value
 changed={'android-native/README.md','android-native/ROADMAP.md','android-native/tools/world_smoke.py',
  'level-world/CMakeLists.txt','level-world/README.md','level-world/world.hpp','level-world/world.cpp',
  'level-world/floor_source.hpp','level-world/floor_source.cpp','level-world/tests/floor_source_differential.py',
  'level-world/tests/navigation_differential.py','level-world/tools/build_floor_source_oracle.ps1',
  'level-world/reference/floor-source/NOTES.md','level-world/reference/selector/NOTES.md',
  'scene-materials/scene.hpp','scene-materials/scene.cpp'}
 for name,expected in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==expected,name
 additions={'level-world/floors.hpp','level-world/floors.cpp','level-world/tools/inspect_floors.cpp',
  'level-world/tests/floor_records_differential.py','android-native/tools/validate_floor_records_checkpoint.py'}
 additions|={f.relative_to(REPO/'port').as_posix() for f in (REPO/'port/level-world/reference/floor-records').rglob('*') if f.is_file()}
 # Scene's owned name/property strings change its ABI; capture all module source.
 additions|={f.relative_to(REPO/'port').as_posix() for f in (REPO/'port/scene-materials').rglob('*') if f.is_file() and f.suffix in {'.cpp','.hpp'} and 'build' not in f.parts}
 for folder in ('floor-records','floor-source','selector','navigation','collision','octree'):
  verify_capture(REPO/'port/level-world/reference'/folder/'original-functions.json',engine)
 for name in ('floor-records-repo-build.log','floor-records-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'floor-records-zipalign.log')
 for f in (local/'world-tests-floor-records').glob('*.log'):
  assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|Shader failed|Link failed',log(f)),f
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):
  assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 modules=set(prior['module_source_sha256'])|changed|additions
 result={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(studio_apk),
  'libraries':libs,'studio_libraries':studio_libs,'unchanged_libraries':unchanged,'assets_verified':assets,
  'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],
  'studio_source_sha256':source,'module_source_sha256':{n:digest(REPO/'port'/n) for n in sorted(modules)},
  'floor_reports':reports,'reference_sha256':references,'authored_floor_input_sha256':input_sha,
  'floor_geometry_sha256':floor_sha,'authored_floors_host_audit':host,'world_host_audit':world_host,
  'host_regressions':host_regressions,'world_cases':10,'apk_16k_zip_alignment_verified':True,
  'prior_floor_source_checkpoint':{'apk_sha256':prior['apk_sha256'],'validation_sha256':digest(prior_path)},
  'prior_combat_checkpoint':prior['prior_combat_checkpoint'],
  'actual_crypt_floor_records_built':True,'selector_collision_used_by_gameplay_height':True,
  'graph_constructed_in_gameplay':True,'graph_used_by_route_search':False,
  'original_movement_controller_reconstructed':False,'full_original_floor_loader_reconstructed':False,
  'original_metadata_parser_reconstructed':False,'scene_ownership_is_modern_adapter':True,
  'pending_navigation':'Full metadata/lifecycle, rotated/generated rooms, cross-floor sewing, route search, smoothing, obstacles and original current-floor/movement controller.',
  'physical_arm64_tested':False,'original_gpu_parity_verified':False,
  'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 (ROOT/'reports/build-validation-floor-records.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
 (ROOT/'reports/world-smoke-floor-records.json').write_text(json.dumps(world,indent=2)+'\n',encoding='utf-8')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-floor-records-{sha[:8]}.apk'
 checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'checkpoint':str(checkpoint),
  'floor_records':8,'graph_nodes':321,'graph_edges':778,'world_cases':10,
  'selector_collision_used_by_gameplay_height':True,'graph_used_by_route_search':False,'goal_status':'active'}))

if __name__=='__main__':main()
