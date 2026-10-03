"""Bind the native navigation builder to both APKs and fresh world checks.

The builder is compiled and differentially audited but not used by gameplay.
Earlier combat evidence is retained under its own checkpoint, not relabeled.
"""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from elftools.elf.elffile import ELFFile
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def log(p):
 raw=p.read_bytes();return raw.decode('utf-16') if raw.startswith((b'\xff\xfe',b'\xfe\xff')) else raw.decode('utf-8-sig')
def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args();local=REPO/'.local-inputs';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';studio_apk=a.studio/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);prior_path=ROOT/'reports/build-validation-enemy-ai.json';prior=read(prior_path);libs=inspect(apk);studio_libs=inspect(studio_apk);assert len(libs)==len(studio_libs)==14
 reports={tag:read(REPO/'port/level-world/reports'/name) for tag,name in [('oracle','navigation-arm64-differential.json'),('packaged','navigation-packaged-arm64-differential.json'),('studio','navigation-studio-arm64-differential.json')]};host=read(REPO/'port/level-world/reports/navigation-host-audit.json')
 for tag,r in reports.items():
  assert r['triangle_comparisons']==1880 and r['floor_query_comparisons']==6666 and r['mismatches']==0 and r['batches']==72 and r['reference_sha256']==reports['oracle']['reference_sha256'];assert r['floor_source_sha256']==digest(local/'world-floor.json');assert r['reference_sha256']==digest(local/('navigation-original.bin' if tag=='oracle' else 'navigation-'+tag+'-original.bin'))
 assert host['triangle_comparisons']==1880 and host['floor_query_comparisons']==6666 and host['mismatches']==0 and host['sanitizers']==['address','undefined']
 assert reports['oracle']['arm64_library_sha256']==digest(local/'navigation-oracle.so')
 world=read(local/'world-tests-navigation/world-smoke.json');assert world['apk_sha256']==world['installed_apk_sha256']==sha and len(world['cases'])==10
 assets={};required={'dh2_nav_begin_floor','dh2_nav_triangle'}
 with zipfile.ZipFile(apk) as z,zipfile.ZipFile(studio_apk) as sz:
  assert set(z.namelist())==set(sz.namelist());assert not any('DungeonHunter2.so' in n for n in z.namelist())
  for name in z.namelist():
   if name.startswith('assets/') and not name.endswith('/'):
    raw=z.read(name);relative=name[7:];assert raw==sz.read(name)==(ROOT/'app/src/main/assets'/relative).read_bytes()==(a.studio/'app/src/main/assets'/relative).read_bytes();assets[relative]={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}
   elif name.startswith('lib/') and name.endswith('/libdh2_level_world.so'):assert required<=exports(z.read(name)) and required<=exports(sz.read(name))
  assert assets==prior['assets_verified'];assert hashlib.sha256(z.read('lib/arm64-v8a/libdh2_level_world.so')).hexdigest()==reports['packaged']['arm64_library_sha256'];assert hashlib.sha256(sz.read('lib/arm64-v8a/libdh2_level_world.so')).hexdigest()==reports['studio']['arm64_library_sha256']
  # All other repo ELF binaries are unchanged, including gameplay/AI code.
  unchanged=[]
  for row in prior['libraries']:
   if row['path'].endswith('/libdh2_level_world.so'):continue
   assert hashlib.sha256(z.read(row['path'])).hexdigest()==row['sha256'];unchanged.append(row['path'])
 source={}
 for name in prior['studio_source_sha256']:assert digest(ROOT/'app/src/main'/name)==digest(a.studio/'app/src/main'/name)==prior['studio_source_sha256'][name];source[name]=digest(ROOT/'app/src/main'/name)
 changed={'level-world/CMakeLists.txt','level-world/README.md'}
 for name,expected in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==expected,name
 manifest_path=REPO/'port/level-world/reference/navigation/original-functions.json';manifest=read(manifest_path);assert manifest['original_sha256']==reports['oracle']['original_sha256']==digest(local/'libDungeonHunter2.so')
 with (local/'libDungeonHunter2.so').open('rb') as f:
  elf=ELFFile(f)
  for row in manifest['functions']:
   addr=int(row['elf_address'],16);segment=next(s for s in elf.iter_segments() if s['p_type']=='PT_LOAD' and s['p_vaddr']<=addr< s['p_vaddr']+s['p_filesz']);f.seek(segment['p_offset']+addr-segment['p_vaddr']);assert hashlib.sha256(f.read(row['size'])).hexdigest()==row['sha256']
 for name in ('navigation-android-build-clean.log','navigation-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'navigation-zipalign.log')
 for f in (local/'world-tests-navigation').glob('*.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|Shader failed|Link failed',log(f)),f
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 additions={'level-world/navigation.hpp','level-world/navigation.cpp','level-world/tests/navigation.cpp','level-world/tests/navigation_differential.py','level-world/tools/build_navigation_oracle.ps1','level-world/reference/navigation/NOTES.md','level-world/reference/navigation/original-functions.json','level-world/reference/navigation/reference/original-functions.asm','android-native/tools/validate_navigation_checkpoint.py'};modules=set(prior['module_source_sha256'])|changed|additions
 result={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(studio_apk),'libraries':libs,'studio_libraries':studio_libs,'unchanged_gameplay_libraries':unchanged,'assets_verified':assets,'original_input_count':prior['original_input_count'],'studio_source_sha256':source,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(modules)},'cache_sha256':prior['cache_sha256'],'navigation_reports':reports,'navigation_host_audit':host,'navigation_reference_sha256':digest(local/'navigation-original.bin'),'world_cases':10,'apk_16k_zip_alignment_verified':True,'previous_combat_checkpoint':{'apk_sha256':prior['apk_sha256'],'validation_sha256':digest(prior_path),'automatic_damage_attempts':prior['automatic_enemy_damage_attempts'],'isolated_player_attempts':prior['isolated_player_attack_attempts']},'navigation_builder_compiled':True,'navigation_builder_used_by_gameplay':False,'floor_support_answers_are_caller_fixtures':True,'full_original_enemy_ai_implemented':False,'pending_navigation':'Original floor collision/selector, floor identities/flags, cross-floor sewing, graph search, smoothing, obstacle handling, movement controller/animation integration.','physical_arm64_tested':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 (ROOT/'reports/build-validation-navigation.json').write_text(json.dumps(result,indent=2)+'\n');(ROOT/'reports/world-smoke-navigation.json').write_text(json.dumps(world,indent=2)+'\n');checkpoint=ROOT/'build/checkpoints'/f'dh2-native-navigation-{sha[:8]}.apk';checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'triangle_comparisons_per_arm64_build':1880,'floor_query_comparisons':6666,'world_cases':10,'builder_integrated_into_gameplay':False,'goal_status':'active'}))
if __name__=='__main__':main()
