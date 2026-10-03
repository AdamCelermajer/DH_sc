"""Bind original-verified native octree code and regressions to both APKs."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_navigation_checkpoint import ROOT,REPO,digest,read,log
from validate_collision_checkpoint import verify_capture

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args()
 local=REPO/'.local-inputs';reports_dir=REPO/'port/level-world/reports';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';studio_apk=a.studio/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk)
 prior_path=ROOT/'reports/build-validation-collision.json';prior=read(prior_path);libs=inspect(apk);studio_libs=inspect(studio_apk);assert len(libs)==len(studio_libs)==14
 reports={};library_sha={};reference_sha=None;floor_sha=digest(local/'world-floor.json');engine_sha=digest(local/'libDungeonHunter2.so')
 expected={'build_comparisons':74,'node_comparisons':875,'partitioned_triangle_comparisons':3235,'box_comparisons':2294,'selected_triangle_comparisons':10341,'mismatches':0}
 for tag in ('oracle','packaged','studio'):
  library_sha[tag]=digest(local/('octree-oracle.so' if tag=='oracle' else 'octree-'+tag+'-arm64.so'))
  suffix='-arm64-differential.json' if tag=='oracle' else '-'+tag+'-arm64-differential.json';r=read(reports_dir/('octree'+suffix));fixture=local/('octree-original.bin' if tag=='oracle' else 'octree-'+tag+'-original.bin')
  for key,value in expected.items():assert r[key]==value,(tag,key)
  assert r['arm64_library_sha256']==library_sha[tag] and r['original_sha256']==engine_sha and r['floor_source_sha256']==floor_sha and r['reference_sha256']==digest(fixture)
  if reference_sha is None:reference_sha=r['reference_sha256']
  else:assert r['reference_sha256']==reference_sha
  reports[tag]=r
 regressions={}
 for tag in ('packaged','studio'):
  for kind,stem,reference_key,count_key,count in [('collision','octree-collision','collision','comparisons',5071),('graph','octree-navigation-collision','graph','triangle_comparisons',314)]:
   r=read(reports_dir/(stem+'-'+tag+'-regression.json'));fixture=local/(stem+'-'+tag+'-original.bin')
   assert r[count_key]==count and r['mismatches']==0 and r['original_sha256']==engine_sha and r['arm64_library_sha256']==library_sha[tag] and r['floor_source_sha256']==floor_sha
   assert r['reference_sha256']==digest(fixture)==prior['reference_sha256'][reference_key]
   if kind=='collision':assert r['cases']=={'line':2409,'ray':1400,'floor':1262} and r['hits']=={'line':1469,'ray':735,'floor':937}
   else:assert r['floor_query_comparisons']==r['original_floor_collision_calls']==1239 and not r['floor_support_answers_are_caller_fixtures']
   regressions[kind+'-'+tag]=r
 host=read(reports_dir/'octree-host-audit.json');collision_host=read(reports_dir/'octree-collision-host-regression.json');graph_host=read(reports_dir/'octree-navigation-collision-host-regression.json')
 for key,value in expected.items():assert host[key]==value,key
 assert host['bounded_query_checks']==6882 and host['storage_rejection_checks']==134 and host['empty_tree_checks']==2 and host['reference_sha256']==reference_sha
 for r,key,fixture in [(host,None,'octree-original.bin'),(collision_host,'collision','collision-original.bin'),(graph_host,'graph','navigation-collision-original.bin')]:
  assert r['mismatches']==0 and r['sanitizers']==['address','undefined'] and r['reference_sha256']==digest(local/fixture)
  if key:assert r['reference_sha256']==prior['reference_sha256'][key]
 assert collision_host['comparisons']==5071 and graph_host['triangle_comparisons']==314 and graph_host['floor_query_comparisons']==1239 and graph_host['native_floor_collision_used']
 world=read(local/'world-tests-octree/world-smoke.json');assert world['apk_sha256']==world['installed_apk_sha256']==sha and world['floor_geometry_sha256']==floor_sha and len(world['cases'])==10
 required={'dh2_octree_build','dh2_octree_box','dh2_nav_begin_floor','dh2_nav_triangle','dh2_collision_line','dh2_collision_raycast','dh2_collision_floor','dh2_collision_floor_query'};assets={}
 with zipfile.ZipFile(apk) as z,zipfile.ZipFile(studio_apk) as sz:
  assert set(z.namelist())==set(sz.namelist()) and not any('DungeonHunter2.so' in n for n in z.namelist())
  for name in z.namelist():
   if name.startswith('assets/') and not name.endswith('/'):
    raw=z.read(name);relative=name[7:];assert raw==sz.read(name)==(ROOT/'app/src/main/assets'/relative).read_bytes()==(a.studio/'app/src/main/assets'/relative).read_bytes();assets[relative]={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}
   elif name.startswith('lib/') and name.endswith('/libdh2_level_world.so'):assert required<=exports(z.read(name)) and required<=exports(sz.read(name))
  assert assets==prior['assets_verified']
  assert hashlib.sha256(z.read('lib/arm64-v8a/libdh2_level_world.so')).hexdigest()==library_sha['packaged'] and hashlib.sha256(sz.read('lib/arm64-v8a/libdh2_level_world.so')).hexdigest()==library_sha['studio']
  unchanged=[]
  for row in prior['libraries']:
   if row['path'].endswith('/libdh2_level_world.so'):continue
   assert hashlib.sha256(z.read(row['path'])).hexdigest()==row['sha256'];unchanged.append(row['path'])
  for row in prior['studio_libraries']:
   if not row['path'].endswith('/libdh2_level_world.so'):assert hashlib.sha256(sz.read(row['path'])).hexdigest()==row['sha256']
 source={}
 for name,expected_sha in prior['studio_source_sha256'].items():assert digest(ROOT/'app/src/main'/name)==digest(a.studio/'app/src/main'/name)==expected_sha;source[name]=expected_sha
 changed={'level-world/CMakeLists.txt','level-world/README.md','level-world/reference/collision/NOTES.md'}
 for name,expected_sha in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==expected_sha,name
 for folder in ('octree','navigation','collision'):verify_capture(REPO/'port/level-world/reference'/folder/'original-functions.json',local/'libDungeonHunter2.so')
 for name in ('octree-android-build.log','octree-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'octree-zipalign.log')
 for f in (local/'world-tests-octree').glob('*.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|Shader failed|Link failed',log(f)),f
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 additions={'level-world/octree.hpp','level-world/octree.cpp','level-world/tests/octree.cpp','level-world/tests/octree_differential.py','level-world/tools/build_octree_oracle.ps1','level-world/reference/octree/NOTES.md','level-world/reference/octree/original-functions.json','level-world/reference/octree/reference/original-functions.asm','android-native/tools/validate_octree_checkpoint.py','android-native/README.md'}
 modules=set(prior['module_source_sha256'])|changed|additions
 result={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(studio_apk),'libraries':libs,'studio_libraries':studio_libs,'unchanged_gameplay_libraries':unchanged,'assets_verified':assets,'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'studio_source_sha256':source,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(modules)},'octree_reports':reports,'octree_reference_sha256':reference_sha,'collision_graph_regressions':regressions,'octree_host_audit':host,'collision_host_regression':collision_host,'graph_collision_host_regression':graph_host,'world_cases':10,'apk_16k_zip_alignment_verified':True,'prior_collision_checkpoint':{'apk_sha256':prior['apk_sha256'],'validation_sha256':digest(prior_path)},'prior_combat_checkpoint':prior['prior_combat_checkpoint'],'native_octree_compiled':True,'identity_octree_construction_and_box_order_reconstructed':True,'octree_used_by_collision':False,'backend_used_by_gameplay':False,'selector_transforms_and_floor_producers_reconstructed':False,'pending_navigation':'Selector mesh extraction/transform setup, octree/collision coupling, original floor identities/flags/bounds, cross-floor sewing, graph search, smoothing, obstacles and movement controller integration.','physical_arm64_tested':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 (ROOT/'reports/build-validation-octree.json').write_text(json.dumps(result,indent=2)+'\n');(ROOT/'reports/world-smoke-octree.json').write_text(json.dumps(world,indent=2)+'\n')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-octree-{sha[:8]}.apk';checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'octree_builds_per_arm64_binary':74,'octree_box_queries_per_arm64_binary':2294,'collision_regression_cases_per_packaged_arm64_binary':5071,'graph_collision_triangles_per_packaged_arm64_binary':314,'world_cases':10,'octree_used_by_collision':False,'goal_status':'active'}))
if __name__=='__main__':main()
