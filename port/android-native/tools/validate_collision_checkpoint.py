"""Bind native floor collision and graph coupling to both built APKs."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from elftools.elf.elffile import ELFFile
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_navigation_checkpoint import ROOT,REPO,digest,read,log

def verify_capture(path,engine):
 manifest=read(path);assert manifest['original_sha256']==digest(engine)
 with engine.open('rb') as f:
  elf=ELFFile(f)
  for row in manifest['functions']:
   address=int(row['elf_address'],16);segment=next(s for s in elf.iter_segments() if s['p_type']=='PT_LOAD' and s['p_vaddr']<=address<s['p_vaddr']+s['p_filesz']);f.seek(segment['p_offset']+address-segment['p_vaddr']);assert hashlib.sha256(f.read(row['size'])).hexdigest()==row['sha256']

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args();local=REPO/'.local-inputs';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';studio_apk=a.studio/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);prior_path=ROOT/'reports/build-validation-navigation.json';prior=read(prior_path);libs=inspect(apk);studio_libs=inspect(studio_apk);assert len(libs)==len(studio_libs)==14
 reports={};reference={};libraries={};level_reports=REPO/'port/level-world/reports'
 for tag in ('oracle','packaged','studio'):
  libraries[tag]=digest(local/('collision-oracle.so' if tag=='oracle' else 'collision-'+tag+'-arm64.so'))
  for kind,prefix,cases in [('collision','collision',5071),('graph','navigation-collision',314)]:
   name=prefix+('-arm64-differential.json' if tag=='oracle' else '-'+tag+'-arm64-differential.json');r=read(level_reports/name);fixture=local/(prefix+('-original.bin' if tag=='oracle' else '-'+tag+'-original.bin'));assert r['mismatches']==0 and r['arm64_library_sha256']==libraries[tag] and r['reference_sha256']==digest(fixture) and r['floor_source_sha256']==digest(local/'world-floor.json');reports[kind+'-'+tag]=r
   assert r['comparisons' if kind=='collision' else 'triangle_comparisons']==cases
   if kind=='collision':assert r['cases']=={'line':2409,'ray':1400,'floor':1262} and r['hits']=={'line':1469,'ray':735,'floor':937}
   else:assert r['floor_query_comparisons']==r['original_floor_collision_calls']==1239 and not r['floor_support_answers_are_caller_fixtures']
   if kind in reference:assert reference[kind]==r['reference_sha256']
   else:reference[kind]=r['reference_sha256']
  if tag!='oracle':
   r=read(level_reports/('collision-navigation-'+tag+'-regression.json'));assert r['triangle_comparisons']==1880 and r['floor_query_comparisons']==6666 and r['mismatches']==0 and r['arm64_library_sha256']==libraries[tag] and r['reference_sha256']==prior['navigation_reference_sha256']==digest(local/('collision-navigation-'+tag+'-original.bin'));reports['graph-regression-'+tag]=r
 host=read(level_reports/'collision-host-audit.json');graph_host=read(level_reports/'navigation-collision-host-audit.json');regression_host=read(level_reports/'collision-navigation-host-regression.json')
 for r in (host,graph_host,regression_host):assert r['mismatches']==0 and r['sanitizers']==['address','undefined']
 assert host['comparisons']==5071 and graph_host['triangle_comparisons']==314 and graph_host['native_floor_collision_used'] and regression_host['triangle_comparisons']==1880
 world=read(local/'world-tests-collision/world-smoke.json');assert world['apk_sha256']==world['installed_apk_sha256']==sha and len(world['cases'])==10
 assets={};required={'dh2_nav_begin_floor','dh2_nav_triangle','dh2_collision_line','dh2_collision_raycast','dh2_collision_floor','dh2_collision_floor_query'}
 with zipfile.ZipFile(apk) as z,zipfile.ZipFile(studio_apk) as sz:
  assert set(z.namelist())==set(sz.namelist());assert not any('DungeonHunter2.so' in n for n in z.namelist())
  for name in z.namelist():
   if name.startswith('assets/') and not name.endswith('/'):
    raw=z.read(name);relative=name[7:];assert raw==sz.read(name)==(ROOT/'app/src/main/assets'/relative).read_bytes()==(a.studio/'app/src/main/assets'/relative).read_bytes();assets[relative]={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}
   elif name.startswith('lib/') and name.endswith('/libdh2_level_world.so'):assert required<=exports(z.read(name)) and required<=exports(sz.read(name))
  assert assets==prior['assets_verified'];assert hashlib.sha256(z.read('lib/arm64-v8a/libdh2_level_world.so')).hexdigest()==libraries['packaged'];assert hashlib.sha256(sz.read('lib/arm64-v8a/libdh2_level_world.so')).hexdigest()==libraries['studio']
  unchanged=[]
  for row in prior['libraries']:
   if row['path'].endswith('/libdh2_level_world.so'):continue
   assert hashlib.sha256(z.read(row['path'])).hexdigest()==row['sha256'];unchanged.append(row['path'])
 source={}
 for name,expected in prior['studio_source_sha256'].items():assert digest(ROOT/'app/src/main'/name)==digest(a.studio/'app/src/main'/name)==expected;source[name]=expected
 changed={'level-world/CMakeLists.txt','level-world/README.md','level-world/tests/navigation.cpp','level-world/tests/navigation_differential.py','level-world/reference/navigation/NOTES.md'}
 for name,expected in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==expected,name
 for folder in ('navigation','collision'):verify_capture(REPO/'port/level-world/reference'/folder/'original-functions.json',local/'libDungeonHunter2.so')
 for name in ('collision-android-build.log','collision-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'collision-zipalign.log')
 for f in (local/'world-tests-collision').glob('*.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|Shader failed|Link failed',log(f)),f
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 additions={'level-world/collision.hpp','level-world/collision.cpp','level-world/tests/collision.cpp','level-world/tests/collision_differential.py','level-world/tools/build_collision_oracle.ps1','level-world/reference/collision/NOTES.md','level-world/reference/collision/original-functions.json','level-world/reference/collision/reference/original-functions.asm','android-native/tools/validate_collision_checkpoint.py'};modules=set(prior['module_source_sha256'])|changed|additions
 result={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(studio_apk),'libraries':libs,'studio_libraries':studio_libs,'unchanged_gameplay_libraries':unchanged,'assets_verified':assets,'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'studio_source_sha256':source,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(modules)},'collision_and_graph_reports':reports,'collision_host_audit':host,'graph_collision_host_audit':graph_host,'graph_host_regression':regression_host,'reference_sha256':reference,'world_cases':10,'apk_16k_zip_alignment_verified':True,'prior_navigation_checkpoint':{'apk_sha256':prior['apk_sha256'],'validation_sha256':digest(prior_path)},'prior_combat_checkpoint':prior['previous_combat_checkpoint'],'native_floor_collision_compiled':True,'graph_calls_native_floor_collision_in_audits':True,'backend_used_by_gameplay':False,'selector_bvh_and_floor_producers_reconstructed':False,'pending_navigation':'Original COctTreeTriangleSelector BVH selection/order and transforms, real floor identities/flags/bounds, cross-floor sewing, graph search, smoothing, obstacles and movement controller/animation integration.','physical_arm64_tested':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 (ROOT/'reports/build-validation-collision.json').write_text(json.dumps(result,indent=2)+'\n');(ROOT/'reports/world-smoke-collision.json').write_text(json.dumps(world,indent=2)+'\n');checkpoint=ROOT/'build/checkpoints'/f'dh2-native-collision-{sha[:8]}.apk';checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'collision_cases_per_arm64_build':5071,'graph_collision_triangles_per_arm64_build':314,'graph_regression_cases_per_packaged_arm64_build':1880,'world_cases':10,'backend_used_by_gameplay':False,'goal_status':'active'}))
if __name__=='__main__':main()
