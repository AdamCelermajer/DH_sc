"""Bind reconstructed mesh extraction, baking, tags and bounds to both APKs.

The full floor loader and gameplay navigation are still separate boundaries.
"""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_navigation_checkpoint import ROOT,REPO,digest,read,log
from validate_collision_checkpoint import verify_capture
from validate_selector_checkpoint import SELECTOR,GRAPH,fields

FLOOR={'mesh_constructor_comparisons':328,'triangle_comparisons':850,'baked_constructor_cases':192,'inconsistent_identity_hint_cases':13,'unindexed_parts':308,'skipped_parts':222,'floor_tag_comparisons':212,'world_and_floor_bounds_comparisons':512,'mismatches':0}

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args()
 local=REPO/'.local-inputs';reports_dir=REPO/'port/level-world/reports';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';studio_apk=a.studio/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk)
 prior_path=ROOT/'reports/build-validation-selector.json';prior=read(prior_path);libs=inspect(apk);studio_libs=inspect(studio_apk);assert len(libs)==len(studio_libs)==14
 engine_sha=digest(local/'libDungeonHunter2.so');reference=digest(local/'floor-source-original.bin');reports={};library_sha={};asset_library_sha={};regressions={}
 for tag in ('oracle','packaged','studio'):
  library=local/('floor-source-oracle.so' if tag=='oracle' else f'floor-source-{tag}-arm64.so');library_sha[tag]=digest(library)
  suffix='-arm64-differential.json' if tag=='oracle' else f'-{tag}-arm64-differential.json';r=read(reports_dir/('floor-source'+suffix));fields(r,FLOOR)
  fixture=local/('floor-source-original.bin' if tag=='oracle' else f'floor-source-{tag}-original.bin');assert r['original_sha256']==engine_sha and r['arm64_library_sha256']==library_sha[tag] and r['reference_sha256']==digest(fixture)==reference
  if tag=='oracle':assert r['asset_library_sha256'] is None and r['native_attribute_calls']==0
  else:
   asset_library_sha[tag]=digest(local/f'floor-source-{tag}-assets-arm64.so');assert r['asset_library_sha256']==asset_library_sha[tag] and r['native_attribute_calls']==5100
   for kind,expected,key in [('selector',SELECTOR,'selector'),('navigation-selector',GRAPH,'graph')]:
    regression=read(reports_dir/f'floor-source-{kind}-{tag}-arm64-differential.json');fields(regression,expected)
    assert regression['original_sha256']==engine_sha and regression['arm64_library_sha256']==library_sha[tag] and regression['floor_source_sha256']==digest(local/'world-floor.json')
    assert regression['reference_sha256']==digest(local/f'floor-source-{kind}-{tag}-original.bin')==prior['reference_sha256'][key];regressions[kind+'-'+tag]=regression
  reports[tag]=r
 host=read(reports_dir/'floor-source-host-audit.json');fields(host,{k:v for k,v in FLOOR.items() if k in ('mesh_constructor_comparisons','triangle_comparisons','floor_tag_comparisons','world_and_floor_bounds_comparisons','mismatches')});assert host['storage_rejection_checks']==249 and host['reference_sha256']==reference and host['sanitizers']==['address','undefined']
 host_regressions={}
 for kind,expected,key in [('selector',SELECTOR,'selector'),('navigation-selector',{'triangle_comparisons':314,'floor_query_comparisons':1239,'native_floor_collision_used':True,'native_octree_selector_used':True,'mismatches':0},'graph')]:
  r=read(reports_dir/f'floor-source-{kind}-host-regression.json');fields(r,expected);assert r['reference_sha256']==prior['reference_sha256'][key] and r['sanitizers']==['address','undefined'];host_regressions[kind]=r
 world=read(local/'world-tests-floor-source/world-smoke.json');assert world['apk_sha256']==world['installed_apk_sha256']==sha and world['floor_geometry_sha256']==digest(local/'world-floor.json') and len(world['cases'])==10
 required={'dh2_floor_mesh_triangles','dh2_floor_source_flags','dh2_floor_source_bounds','dh2_floor_transform_bounds','dh2_selector_inverse','dh2_selector_floor_query','dh2_octree_build','dh2_collision_floor','dh2_nav_triangle'};assets={};unchanged=[]
 with zipfile.ZipFile(apk) as z,zipfile.ZipFile(studio_apk) as sz:
  assert set(z.namelist())==set(sz.namelist()) and not any('DungeonHunter2.so' in name for name in z.namelist())
  for name in z.namelist():
   if name.startswith('assets/') and not name.endswith('/'):
    raw=z.read(name);relative=name[7:];assert raw==sz.read(name)==(ROOT/'app/src/main/assets'/relative).read_bytes()==(a.studio/'app/src/main/assets'/relative).read_bytes();assets[relative]={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}
   elif name.startswith('lib/') and name.endswith('/libdh2_level_world.so'):assert required<=exports(z.read(name)) and required<=exports(sz.read(name))
  assert assets==prior['assets_verified']
  for tag,archive in [('packaged',z),('studio',sz)]:
   assert hashlib.sha256(archive.read('lib/arm64-v8a/libdh2_level_world.so')).hexdigest()==library_sha[tag]
   assert hashlib.sha256(archive.read('lib/arm64-v8a/libdh2_scene_materials.so')).hexdigest()==asset_library_sha[tag]
  for row in prior['libraries']:
   if not row['path'].endswith('/libdh2_level_world.so'):assert hashlib.sha256(z.read(row['path'])).hexdigest()==row['sha256'];unchanged.append(row['path'])
  for row in prior['studio_libraries']:
   if not row['path'].endswith('/libdh2_level_world.so'):assert hashlib.sha256(sz.read(row['path'])).hexdigest()==row['sha256']
 source={}
 for name,expected in prior['studio_source_sha256'].items():assert digest(ROOT/'app/src/main'/name)==digest(a.studio/'app/src/main'/name)==expected;source[name]=expected
 changed={'level-world/CMakeLists.txt','level-world/README.md','level-world/tests/selector_differential.py','level-world/reference/selector/NOTES.md','android-native/README.md'}
 for name,expected in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==expected,name
 for folder in ('floor-source','selector','octree','navigation','collision'):verify_capture(REPO/'port/level-world/reference'/folder/'original-functions.json',local/'libDungeonHunter2.so')
 for name in ('floor-source-android-build.log','floor-source-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'floor-source-zipalign.log')
 for f in (local/'world-tests-floor-source').glob('*.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|Shader failed|Link failed',log(f)),f
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 additions={'level-world/floor_source.hpp','level-world/floor_source.cpp','level-world/tests/floor_source.cpp','level-world/tests/floor_source_differential.py','level-world/tools/build_floor_source_oracle.ps1','android-native/tools/validate_floor_source_checkpoint.py'}
 additions|={p.relative_to(REPO/'port').as_posix() for p in (REPO/'port/level-world/reference/floor-source').rglob('*') if p.is_file()};modules=set(prior['module_source_sha256'])|changed|additions
 result={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(studio_apk),'libraries':libs,'studio_libraries':studio_libs,'unchanged_gameplay_libraries':unchanged,'assets_verified':assets,'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'studio_source_sha256':source,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(modules)},'floor_source_reports':reports,'reference_sha256':reference,'floor_source_host_audit':host,'packaged_regressions':regressions,'host_regressions':host_regressions,'world_cases':10,'apk_16k_zip_alignment_verified':True,'prior_selector_checkpoint':{'apk_sha256':prior['apk_sha256'],'validation_sha256':digest(prior_path)},'prior_combat_checkpoint':prior['prior_combat_checkpoint'],'original_mesh_extraction_and_baking_reconstructed':True,'floor_tag_bit_operations_reconstructed':True,'node_world_and_floor_bounds_arithmetic_reconstructed':True,'full_original_floor_loader_reconstructed':False,'real_floor_metadata_and_clone_services_reconstructed':False,'backend_used_by_gameplay':False,'pending_navigation':'Actual floor records/metadata and mesh-node clone transforms, graph/floor ownership, cross-floor sewing, graph search, smoothing, obstacles and movement integration.','physical_arm64_tested':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 (ROOT/'reports/build-validation-floor-source.json').write_text(json.dumps(result,indent=2)+'\n');(ROOT/'reports/world-smoke-floor-source.json').write_text(json.dumps(world,indent=2)+'\n')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-floor-source-{sha[:8]}.apk';checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'mesh_constructions_per_arm64_binary':328,'triangles_per_arm64_binary':850,'floor_tags_per_arm64_binary':212,'world_and_floor_bounds_per_arm64_binary':512,'world_cases':10,'backend_used_by_gameplay':False,'goal_status':'active'}))
if __name__=='__main__':main()
