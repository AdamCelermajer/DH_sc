"""Bind native transformed selector/collision/graph code to both built APKs."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_navigation_checkpoint import ROOT,REPO,digest,read,log
from validate_collision_checkpoint import verify_capture

SELECTOR={'matrix_inverse_comparisons':1007,'singular_inverses':57,'octree_builds':32,'selector_box_comparisons':3072,'selected_triangle_comparisons':31246,'coupled_ray_comparisons':3072,'coupled_floor_comparisons':3072,'hits':{'ray':1086,'floor':1306},'mismatches':0}
GRAPH={'triangle_comparisons':314,'floor_query_comparisons':1239,'original_floor_collision_calls':1239,'original_octree_selector_executes':True,'native_octree_selector_used':True,'selector_triangle_order_is_caller_fixture':False,'floor_support_answers_are_caller_fixtures':False,'mismatches':0}

def fields(report,expected):
 for key,value in expected.items():assert report[key]==value,(key,report[key],value)

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args()
 local=REPO/'.local-inputs';reports_dir=REPO/'port/level-world/reports';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';studio_apk=a.studio/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk)
 prior_path=ROOT/'reports/build-validation-octree.json';prior=read(prior_path);libs=inspect(apk);studio_libs=inspect(studio_apk);assert len(libs)==len(studio_libs)==14
 reports={};library_sha={};references={};floor_sha=digest(local/'world-floor.json');engine_sha=digest(local/'libDungeonHunter2.so')
 for tag in ('oracle','packaged','studio'):
  library_sha[tag]=digest(local/('selector-oracle.so' if tag=='oracle' else 'selector-'+tag+'-arm64.so'))
  for kind,stem,expected in [('selector','selector',SELECTOR),('graph','navigation-selector',GRAPH)]:
   suffix='-arm64-differential.json' if tag=='oracle' else '-'+tag+'-arm64-differential.json';r=read(reports_dir/(stem+suffix));fixture=local/(stem+('-original.bin' if tag=='oracle' else '-'+tag+'-original.bin'))
   fields(r,expected);assert r['arm64_library_sha256']==library_sha[tag] and r['original_sha256']==engine_sha and r['floor_source_sha256']==floor_sha and r['reference_sha256']==digest(fixture)
   if kind in references:assert r['reference_sha256']==references[kind]
   else:references[kind]=r['reference_sha256']
   if kind=='selector':assert r['node_modes']=={'absent':1024,'dynamic':1024,'baked':1024} and r['extra_matrix_cases']==768 and r['original_matrix_getters']==6024
   else:assert r['floor_summaries']==prior['collision_graph_regressions']['graph-packaged']['floor_summaries']
   reports[kind+'-'+tag]=r
 regressions={};old_navigation=read(ROOT/'reports/build-validation-navigation.json')
 specs=[('octree','selector-octree',{'build_comparisons':74,'node_comparisons':875,'box_comparisons':2294,'mismatches':0},prior['octree_reference_sha256']),('collision','selector-collision',{'comparisons':5071,'cases':{'line':2409,'ray':1400,'floor':1262},'hits':{'line':1469,'ray':735,'floor':937},'mismatches':0},prior['collision_graph_regressions']['collision-packaged']['reference_sha256']),('navigation','selector-navigation',{'triangle_comparisons':1880,'floor_query_comparisons':6666,'mismatches':0},old_navigation['navigation_reference_sha256'])]
 for tag in ('packaged','studio'):
  for kind,stem,expected,reference in specs:
   r=read(reports_dir/(stem+'-'+tag+'-arm64-differential.json'));fixture=local/(stem+'-'+tag+'-original.bin');fields(r,expected)
   assert r['arm64_library_sha256']==library_sha[tag] and r['original_sha256']==engine_sha and r['floor_source_sha256']==floor_sha and r['reference_sha256']==digest(fixture)==reference;regressions[kind+'-'+tag]=r
 host=read(reports_dir/'selector-host-audit.json');graph_host=read(reports_dir/'navigation-selector-host-audit.json');fields(host,SELECTOR);assert host['storage_rejection_checks']==65
 fields(graph_host,{'triangle_comparisons':314,'floor_query_comparisons':1239,'native_floor_collision_used':True,'native_octree_selector_used':True,'mismatches':0})
 host_regressions={}
 for kind,stem,expected,reference in specs:
  r=read(reports_dir/(stem+'-host-regression.json'));fields(r,expected);assert r['mismatches']==0 and r['sanitizers']==['address','undefined'] and r['reference_sha256']==reference;host_regressions[kind]=r
 for kind,r,fixture in [('selector',host,'selector-original.bin'),('graph',graph_host,'navigation-selector-original.bin')]:assert r['sanitizers']==['address','undefined'] and r['reference_sha256']==digest(local/fixture)==references[kind]
 world=read(local/'world-tests-selector/world-smoke.json');assert world['apk_sha256']==world['installed_apk_sha256']==sha and world['floor_geometry_sha256']==floor_sha and len(world['cases'])==10
 required={'dh2_selector_inverse','dh2_selector_triangles','dh2_selector_raycast','dh2_selector_floor','dh2_selector_floor_query','dh2_octree_build','dh2_octree_box','dh2_nav_begin_floor','dh2_nav_triangle','dh2_collision_line','dh2_collision_raycast','dh2_collision_floor','dh2_collision_floor_query'};assets={}
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
 changed={'level-world/CMakeLists.txt','level-world/README.md','level-world/reference/collision/NOTES.md','level-world/reference/octree/NOTES.md','level-world/tests/navigation.cpp','level-world/tests/navigation_differential.py','level-world/tests/collision_differential.py','android-native/README.md'}
 for name,expected_sha in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==expected_sha,name
 for folder in ('selector','octree','navigation','collision'):verify_capture(REPO/'port/level-world/reference'/folder/'original-functions.json',local/'libDungeonHunter2.so')
 for name in ('selector-android-build.log','selector-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'selector-zipalign.log')
 for f in (local/'world-tests-selector').glob('*.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|Shader failed|Link failed',log(f)),f
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 additions={'level-world/selector.hpp','level-world/selector.cpp','level-world/tests/selector.cpp','level-world/tests/selector_differential.py','level-world/tools/build_selector_oracle.ps1','level-world/reference/selector/NOTES.md','level-world/reference/selector/original-functions.json','level-world/reference/selector/reference/original-functions.asm','android-native/tools/validate_selector_checkpoint.py'}
 modules=set(prior['module_source_sha256'])|changed|additions
 result={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(studio_apk),'libraries':libs,'studio_libraries':studio_libs,'unchanged_gameplay_libraries':unchanged,'assets_verified':assets,'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'studio_source_sha256':source,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(modules)},'selector_graph_reports':reports,'reference_sha256':references,'packaged_regressions':regressions,'selector_host_audit':host,'graph_selector_host_audit':graph_host,'host_regressions':host_regressions,'world_cases':10,'apk_16k_zip_alignment_verified':True,'prior_octree_checkpoint':{'apk_sha256':prior['apk_sha256'],'validation_sha256':digest(prior_path)},'prior_combat_checkpoint':prior['prior_combat_checkpoint'],'native_selector_compiled':True,'selector_query_and_output_transforms_reconstructed':True,'octree_used_by_collision':True,'native_octree_used_in_graph_audits':True,'backend_used_by_gameplay':False,'mesh_extraction_baking_and_floor_producers_reconstructed':False,'pending_navigation':'Original selector mesh extraction/baking, floor identities/flags/bounds, cross-floor sewing, graph search, smoothing, obstacles and movement controller integration.','physical_arm64_tested':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 (ROOT/'reports/build-validation-selector.json').write_text(json.dumps(result,indent=2)+'\n');(ROOT/'reports/world-smoke-selector.json').write_text(json.dumps(world,indent=2)+'\n')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-selector-{sha[:8]}.apk';checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'selector_queries_per_arm64_binary':9216,'inverse_cases_per_arm64_binary':1007,'graph_triangles_per_arm64_binary':314,'graph_support_queries_per_arm64_binary':1239,'world_cases':10,'octree_used_by_collision':True,'backend_used_by_gameplay':False,'goal_status':'active'}))
if __name__=='__main__':main()
