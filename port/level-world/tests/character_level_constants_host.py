"""Genuine native constants table feeding frozen Level source and actual monster Lua initialization."""
import argparse,hashlib,json,subprocess,zipfile,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];SCRATCH=REPO/'.local-inputs/character-level-constants-discovery'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
 p=argparse.ArgumentParser();p.add_argument('--build',default='/home/adampalace/dh2-world-build');p.add_argument('--main-linked');a=p.parse_args();commands=[];SCRATCH.mkdir(parents=True,exist_ok=True)
 def run(*args):
  r=subprocess.run(['wsl.exe','--cd',linux(REPO),*args],capture_output=True,text=True,timeout=60);commands.append(dict(arguments=list(args),returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and not r.stderr.strip(),commands[-1];return r.stdout.strip()
 sources=[ROOT/'character_level.cpp',ROOT/'character_level.hpp',ROOT/'tests/character_level_constants.cpp',Path(__file__),ROOT/'tests/character_level_differential.py',ROOT/'reports/character-level-arm64-differential.json',ROOT/'reference/character-level/level-fixtures.bin',ROOT/'character_property_bindings.hpp']+[REPO/'port/game-data'/x for x in('properties.cpp','properties.hpp','class_tables.cpp','class_tables.hpp')]+[REPO/'port/script-runtime'/x for x in('script_runtime.h','script_runtime.c','script_design_bindings.h','script_scalar_bindings.c','script_function_alias.cpp','script_constants.hpp','script_constants.cpp')]
 before={str(x.relative_to(REPO)):sha(x) for x in sources};runtime=a.build+'/script-runtime/libdh2_script_runtime.so';data=a.build+'/game-data/libdh2_game_data.so';world=a.build+'/libdh2_level_world.so';dependencies={x:run('sha256sum',x).split()[0] for x in(runtime,data,world)}
 cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
 with zipfile.ZipFile(cache) as z:
  for name in('monster.luac','_commons.luac'):(SCRATCH/name).write_bytes(z.read('com.gameloft.android.GAND.GloftD2SS/files/data/scripts/ai/'+name))
  for name in('ai_pycst.bin','design_pycst.bin'):(SCRATCH/name).write_bytes(z.read('com.gameloft.android.GAND.GloftD2SS/files/data/pydata/'+name))
 constants_gold=REPO/'port/script-runtime/reference/design-constants-loader/constants-original-gold.bin';assert sha(constants_gold)=='a0da51c378a8e651f7f0dcca9b3c9a5e09651c978ed662b3015a8cb609b3529f'
 corpus=constants_gold.read_bytes();cursor=0;load_matches=[];wanted={(SCRATCH/n).read_bytes():n for n in('ai_pycst.bin','design_pycst.bin')}
 def word():
  nonlocal cursor
  value=struct.unpack_from('<I',corpus,cursor)[0];cursor+=4;return value
 def span():
  nonlocal cursor
  size=word();assert size<=len(corpus)-cursor;value=corpus[cursor:cursor+size];cursor+=size;return value
 assert word()==0x31545343
 for index in range(word()):
  blob=span();status=word();state=[word() for _ in range(4)];queries=word()
  if blob in wanted:
   name=wanted[blob];assert status==0 and state==({'ai_pycst.bin':[1042,58,7,0],'design_pycst.bin':[3999,152,13,0]}[name]);assert blob==(REPO/'port/android-native/app/src/main/assets/data'/name).read_bytes();load_matches.append(dict(original_load=index,name=name,status=status,state=state,sha256=hashlib.sha256(blob).hexdigest(),original_queries=queries))
  for _ in range(queries):span();span();word()
 assert cursor==len(corpus) and len(load_matches)==2
 flags=['-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror'];exe=a.main_linked or linux(SCRATCH/'level_constants_host')
 if not a.main_linked:run('g++',*flags,linux(ROOT/'character_level.cpp'),linux(ROOT/'tests/character_level_constants.cpp'),'-I'+linux(ROOT),'-L'+a.build,'-ldh2_level_world','-L'+a.build+'/script-runtime','-ldh2_script_runtime','-L'+a.build+'/game-data','-ldh2_game_data','-ldl','-Wl,-rpath,'+a.build,'-Wl,-rpath,'+a.build+'/script-runtime','-Wl,-rpath,'+a.build+'/game-data','-o',exe)
 deps=run('ldd',exe);assert 'not found' not in deps and runtime in deps and data in deps and world in deps and 'libasan.so' in deps and 'libubsan.so' in deps
 gold=ROOT/'reference/character-level/level-fixtures.bin';assets=REPO/'port/android-native/app/src/main/assets/data'
 result=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,linux(gold),linux(SCRATCH/'_commons.luac'),linux(SCRATCH/'monster.luac'),linux(assets),linux(SCRATCH/'ai_pycst.bin'),linux(SCRATCH/'design_pycst.bin')));assert result['validation']=='PASS' and result['property_library']==world and result['constants_library']==runtime and result['real_constant_loads']==2 and result['comparisons']==870
 assert all(sha(REPO/x)==h for x,h in before.items());assert dependencies=={x:run('sha256sum',x).split()[0] for x in dependencies};binaries={**dependencies,exe:run('sha256sum',exe).split()[0]}
 if a.main_linked:binaries[result['level_library']]=run('sha256sum',result['level_library']).split()[0]
 assert sha(constants_gold)=='a0da51c378a8e651f7f0dcca9b3c9a5e09651c978ed662b3015a8cb609b3529f'
 report=dict(validation='PASS',host_audit=result,source_bindings=before,binary_bindings=binaries,gold_sha256=sha(gold),script_bindings={n:sha(SCRATCH/n) for n in('monster.luac','_commons.luac')},constant_bytes={n:sha(SCRATCH/n) for n in('ai_pycst.bin','design_pycst.bin')},constant_loader_gold_sha256=sha(constants_gold),original_load_matches=load_matches,cache_sha256=sha(cache),commands=commands,sanitizer_findings=0,owned_level_compiler_provenance_claim=not bool(a.main_linked),dependency_compiler_provenance_claim=False,scope=__doc__,constant_loader_backend='actual main runtime native table loader/getConstant; no supplied cap callback',debug_backend='explicit caller services',world_input_and_host_receiver_ownership='explicit borrowed test session, no original network/level ownership proof',packaged_APK=False)
 dest=ROOT/('reports/character-level-constants-main-linked-host-audit.json' if a.main_linked else 'reports/character-level-constants-host-audit.json');dest.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',report=str(dest),host_audit=result)))
if __name__=='__main__':main()
