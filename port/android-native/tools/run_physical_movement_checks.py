"""Run original-derived checks against both APKs and the shared sanitizer build."""
import argparse,hashlib,json,subprocess,sys,zipfile
from pathlib import Path
REPO=Path(__file__).resolve().parents[3]
TESTS=REPO/'port/level-world/tests';REPORTS=REPO/'port/level-world/reports';LOCAL=REPO/'.local-inputs'
MODULES={
 'physical-controls':('physical_controls',LOCAL/'physical-controls-reference.bin'),
 'body-transform':('body_transform',LOCAL/'body-transform-reference.bin'),
 'subobjects-update':('subobjects_update',REPO/'port/level-world/reference/subobjects-update/service-fixtures.bin'),
 'controller-physical':('controller_physical',LOCAL/'controller-physical-reference.bin'),
 'character-body-config':('character_body_config',LOCAL/'character-body-config-reference.bin'),
}
def wsl_path(path):return '/mnt/'+path.drive[0].lower()+path.as_posix()[2:]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def run(command):
 result=subprocess.run(command,cwd=REPO,text=True,capture_output=True)
 if result.returncode:raise RuntimeError(f'{command!r}\n{result.stdout}\n{result.stderr}')
 return result.stdout
def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');p.add_argument('--skip-host',action='store_true');a=p.parse_args()
 cache=run(['wsl','cat',a.host_build+'/CMakeCache.txt'])
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):
  assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 for module,(stem,gold) in (MODULES.items() if not a.skip_host else []):
  result=json.loads(run(['wsl',a.host_build+'/'+stem+'_audit',wsl_path(gold)]))
  if 'cases' in result:result['comparisons']=result.pop('cases')
  result.update(reference_sha256=sha(gold),sanitizers=['address','undefined'])
  (REPORTS/f'physical-movement-{module}-host-audit.json').write_text(json.dumps(result,indent=2)+'\n')
  print(f'Host {module}: {result["comparisons"]} pass',flush=True)
 for module,arguments in ([] if a.skip_host else [('controller',['.local-inputs/navigation-controller-original.bin','port/android-native/app/src/main/assets/worlds/crypt.bdae','port/android-native/app/src/main/assets/worlds/crypt01.dwld']),('heading',['.local-inputs/navigation-heading-original.bin'])]):
  result=json.loads(run(['wsl',a.host_build+'/navigation_'+module+'_audit',*[wsl_path(REPO/path) for path in arguments]]))
  if 'cases' in result:result['comparisons']=result.pop('cases')
  result.update(reference_sha256=sha(REPO/arguments[0]),sanitizers=['address','undefined'])
  (REPORTS/f'physical-movement-{module}-host-regression.json').write_text(json.dumps(result,indent=2)+'\n')
  print(f'Host {module}: {result["comparisons"]} pass',flush=True)
 for tag,apk in [('packaged',REPO/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'),('studio',a.studio/'app/build/outputs/apk/debug/app-debug.apk')]:
  library=LOCAL/f'physical-movement-{tag}-world-arm64.so'
  with zipfile.ZipFile(apk) as archive:library.write_bytes(archive.read('lib/arm64-v8a/libdh2_level_world.so'))
  for module,(stem,gold) in MODULES.items():
   args=[sys.executable,str(TESTS/f'{stem}_differential.py'),'--engine',str(LOCAL/'libDungeonHunter2.so'),'--library',str(library),
     '--report',str(REPORTS/f'{module}-{tag}-arm64-differential.json'),'--reference-output',str(LOCAL/f'{module}-{tag}-reference.bin')]
   if module=='subobjects-update':args+=['--floor',str(LOCAL/'authored-floors-packaged.json'),'--linked-reference',str(LOCAL/'navigation-link-original.bin'),'--floor-library',str(library)]
   run(args);assert sha(LOCAL/f'{module}-{tag}-reference.bin')==sha(gold)
   print(f'{tag} {module}: pass',flush=True)
  for module in ('controller','heading'):
   args=[sys.executable,str(TESTS/f'navigation_{module}_differential.py'),'--engine',str(LOCAL/'libDungeonHunter2.so'),'--library',str(library),
     '--report',str(REPORTS/f'physical-movement-{module}-{tag}-regression.json'),'--reference-output',str(LOCAL/f'physical-movement-{module}-{tag}-reference.bin')]
   if module=='controller':args+=['--floor',str(LOCAL/'authored-floors-packaged.json'),'--linked-reference',str(LOCAL/'navigation-link-original.bin')]
   run(args);assert sha(LOCAL/f'physical-movement-{module}-{tag}-reference.bin')==sha(LOCAL/f'navigation-{module}-original.bin')
   print(f'{tag} {module} regression: pass',flush=True)
if __name__=='__main__':main()
