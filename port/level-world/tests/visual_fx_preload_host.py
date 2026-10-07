"""Original preload gold plus actual missing-file DebugSwitches DSO adapter.
Private complete transitive DSO snapshot; no shared build or source mutation.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
 p=argparse.ArgumentParser();p.add_argument('--main-linked',action='store_true');p.add_argument('--dependencies',type=Path);p.add_argument('--output',type=Path);a=p.parse_args();scratch=REPO/'.local-inputs/character-skeleton-fx-discovery';deps=a.dependencies or scratch/('main-linked-dependencies'if a.main_linked else'dependencies');deps.resolve().relative_to((REPO/'.local-inputs').resolve());deps.mkdir(parents=True,exist_ok=True);files=scratch/'files';files.mkdir(exist_ok=True);assert not(files/'DebugSwitches.savegame').exists();commands=[]
 def run(*args):
  r=subprocess.run(['wsl','--cd',linux(REPO),*args],capture_output=True,text=True,timeout=120);commands.append(dict(arguments=args,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and not r.stderr.strip(),commands[-1];return r.stdout.strip()
 central='/home/adampalace/dh2-world-build/libdh2_level_world.so';private_world=deps/'libdh2_level_world.so';dependency_manifest=deps/'snapshot.json'
 if not dependency_manifest.exists():
  paths=[central]+re.findall(r'=> (/home/adampalace/dh2-world-build/\S+)',run('ldd',central));snapshot={}
  for source in paths:
   before=run('sha256sum',source).split()[0];target=deps/Path(source).name;run('cp',source,linux(target));assert sha(target)==before;snapshot[Path(source).name]={'original_path':source,'sha256':before}
  assert all(run('sha256sum',v['original_path']).split()[0]==v['sha256']for v in snapshot.values())
  dependency_manifest.write_text(json.dumps(snapshot,indent=2)+'\n')
 snapshot=json.loads(dependency_manifest.read_text());assert all(sha(deps/n)==v['sha256']for n,v in snapshot.items())
 flags=['-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-Wno-misleading-indentation'];links=['-L'+linux(deps),'-ldh2_level_world','-Wl,-rpath,'+linux(deps)];module=scratch/'libvisual_fx_preload_audit.so';exe=scratch/'host';sources=[ROOT/n for n in ['visual_fx_preload.hpp','visual_fx_preload.cpp','tests/visual_fx_preload.cpp','tests/visual_fx_preload_host.py']];source_hashes={x.relative_to(REPO).as_posix():sha(x)for x in sources}
 if a.main_linked:module=private_world;module_links=[]
 else:run('g++',*flags,'-shared','-fPIC',linux(ROOT/'visual_fx_preload.cpp'),*links,'-o',linux(module));module_links=['-L'+linux(scratch),'-lvisual_fx_preload_audit']
 run('g++',*flags,linux(ROOT/'tests/visual_fx_preload.cpp'),*module_links,*links,'-Wl,-rpath,'+linux(scratch),'-ldl','-o',linux(exe));gold=ROOT/'reference/character-skeleton-fx/preload-fixtures.bin';env=['env','LD_LIBRARY_PATH='+linux(deps)+':'+linux(scratch),'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1'];audit=json.loads(run(*env,linux(exe),linux(gold),linux(files)));assert audit['validation']=='PASS'and audit['module_library']==linux(module)and audit['world_library']==linux(private_world);dependencies=run(*env,'ldd',linux(exe));assert all(linux(deps/n)in dependencies for n in snapshot);assert all(sha(deps/n)==v['sha256']for n,v in snapshot.items())and all(sha(REPO/n)==v for n,v in source_hashes.items())
 arm=ROOT/'reports/visual-fx-preload-arm64-differential.json';proof=json.loads(arm.read_text());assert proof['validation']=='PASS'and proof['binary_gold_sha256']==sha(gold);assert all(sha(REPO/n)==v for n,v in proof['source_sha256'].items())
 report=dict(validation='PASS',host_audit=audit,source_sha256=source_hashes,private_snapshot=snapshot,binary_sha256={str(x.relative_to(REPO)):sha(x)for x in [module,exe]},original_sha256=proof['original_sha256'],gold_sha256=sha(gold),arm64_report_sha256=sha(arm),sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer'],sanitizer_findings=0,main_world_library_executed=True,module_in_main_world=a.main_linked,FX_factory_or_rendering_executed=False,packaged_APK=False,scope=__doc__,commands=commands,linked_dependencies=dependencies);out=a.output or ROOT/('reports/visual-fx-preload-main-linked-host-audit.json'if a.main_linked else'reports/visual-fx-preload-host-audit.json');out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',host=audit)))
if __name__=='__main__':main()
