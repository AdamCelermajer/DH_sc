"""Sanitized property callback/genuine Lua monster proof, borrowed nonproperty services explicit."""
import argparse,hashlib,json,subprocess,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];SCRATCH=REPO/'.local-inputs/character-property-bindings-discovery'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
 p=argparse.ArgumentParser();p.add_argument('--runtime-build',default='/home/adampalace/dh2-world-build/script-runtime');p.add_argument('--data-build',default='/home/adampalace/dh2-world-build/game-data');p.add_argument('--main-linked');a=p.parse_args();commands=[]
 def run(*args):
  r=subprocess.run(['wsl.exe','--cd',linux(REPO),*args],capture_output=True,text=True,timeout=60);commands.append(dict(arguments=list(args),returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and not r.stderr.strip(),commands[-1];return r.stdout.strip()
 sources=[ROOT/'character_property_bindings.cpp',ROOT/'character_property_bindings.hpp',ROOT/'tests/character_property_bindings.cpp',Path(__file__),ROOT/'tests/character_property_bindings_differential.py',ROOT/'reference/character-property-bindings/property-bindings-fixtures.bin',ROOT/'reports/character-property-bindings-arm64-differential.json']
 sources+=[REPO/'port/script-runtime'/x for x in ('script_runtime.c','script_runtime.h','script_scalar_bindings.c','script_scalar_bindings.h','script_function_alias.cpp','script_function_alias.h')]+[REPO/'port/game-data'/x for x in ('data.cpp','data.hpp','properties.cpp','properties.hpp','class_tables.cpp','class_tables.hpp')]
 before={str(x.relative_to(REPO)):sha(x) for x in sources};runtime=a.runtime_build+'/libdh2_script_runtime.so';data=a.data_build+'/libdh2_game_data.so';dependencies={x:run('sha256sum',x).split()[0] for x in (runtime,data)}
 cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
 with zipfile.ZipFile(cache) as z:
  for name in ('monster.luac','_commons.luac'):(SCRATCH/name).write_bytes(z.read('com.gameloft.android.GAND.GloftD2SS/files/data/scripts/ai/'+name))
 flags=['-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror'];exe=a.main_linked or linux(SCRATCH/'property_bindings_host')
 if not a.main_linked:
  run('g++',*flags,linux(ROOT/'character_property_bindings.cpp'),linux(ROOT/'tests/character_property_bindings.cpp'),'-I'+linux(ROOT),'-L'+a.runtime_build,'-ldh2_script_runtime','-L'+a.data_build,'-ldh2_game_data','-ldl','-Wl,-rpath,'+a.runtime_build,'-Wl,-rpath,'+a.data_build,'-o',exe)
 deps=run('ldd',exe);assert 'not found' not in deps and runtime in deps and data in deps and 'libasan.so' in deps and 'libubsan.so' in deps
 gold=ROOT/'reference/character-property-bindings/property-bindings-fixtures.bin';assets=REPO/'port/android-native/app/src/main/assets/data'
 result=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,linux(gold),linux(SCRATCH/'_commons.luac'),linux(SCRATCH/'monster.luac'),linux(assets)));assert result['validation']=='PASS'
 assert all(sha(REPO/x)==h for x,h in before.items());assert dependencies=={x:run('sha256sum',x).split()[0] for x in dependencies}
 bindings={**dependencies,exe:run('sha256sum',exe).split()[0]}
 if a.main_linked:bindings[result['binding_library']]=run('sha256sum',result['binding_library']).split()[0]
 report=dict(validation='PASS',host_audit=result,source_bindings=before,binary_bindings=bindings,corpus_sha256=sha(gold),script_bindings={name:sha(SCRATCH/name) for name in ('monster.luac','_commons.luac')},asset_bindings={str(p.relative_to(REPO)):sha(p) for p in assets.glob('character_*') if p.name.startswith(('character_properties_','character_classes_'))},cache_sha256=sha(cache),commands=commands,sanitizer_findings=0,compiler_provenance_claim=not bool(a.main_linked),dependency_compiler_provenance_claim=False,scope=__doc__,whole_original_VM_differential=False,packaged_APK=False)
 dest=ROOT/('reports/character-property-bindings-main-linked-host-audit.json' if a.main_linked else 'reports/character-property-bindings-host-audit.json');dest.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',report=str(dest),host_audit=result)))
if __name__=='__main__':main()
