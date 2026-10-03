"""Verify recovered world/lifecycle/visual kernels from both real APKs.

Full native Box2D scene replay and native body integration have separate host
and ARM64 oracle audits; their test-only constructors are never shipped.
"""
import argparse,hashlib,json,subprocess,sys,zipfile
from pathlib import Path
REPO=Path(__file__).resolve().parents[3]
LOCAL=REPO/'.local-inputs';REPORTS=REPO/'port/level-world/reports';TESTS=REPO/'port/level-world/tests'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def wsl(p):
 p=p.resolve();return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def run(command):
 r=subprocess.run(command,cwd=REPO,text=True,capture_output=True)
 if r.returncode:raise RuntimeError(f'{command!r}\n{r.stdout}\n{r.stderr}')
 return r.stdout
def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');p.add_argument('--visual-reference',type=Path,required=True);p.add_argument('--asset-samples',type=Path,required=True);p.add_argument('--skip-host',action='store_true');p.add_argument('--skip-host-module',action='append',default=[]);p.add_argument('--skip-packaged-module',action='append',default=[]);a=p.parse_args()
 specs={'physical-world':('physical_world',LOCAL/'physical-world-reference.bin'),'physical-lifecycle':('physical_lifecycle',LOCAL/'physical-lifecycle-reference.bin'),'visual-motion':('visual_motion',a.visual_reference)}
 if not a.skip_host:
  cache=run(['wsl','cat',a.host_build+'/CMakeCache.txt'])
  for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):
   assert '-fsanitize=address,undefined' in next(v for v in cache.splitlines() if v.startswith(prefix))
  for module,(stem,gold) in specs.items():
   if module in a.skip_host_module:continue
   result=json.loads(run(['wsl',a.host_build+'/'+stem+'_audit',wsl(gold)]));result.update(reference_sha256=sha(gold),sanitizers=['address','undefined'])
   (REPORTS/f'physics-backend-{module}-host-audit.json').write_text(json.dumps(result,indent=2)+'\n');print(f'Host {module}: pass',flush=True)
  result=json.loads(run(['wsl',a.host_build+'/native_body_audit']));result['sanitizers']=['address','undefined'];(REPORTS/'physics-backend-native-body-host-audit.json').write_text(json.dumps(result,indent=2)+'\n');print('Host genuine native body: pass',flush=True)
  result=json.loads(run(['wsl',a.host_build+'/native_body_replay_audit',wsl(LOCAL/'native-body-reference.bin')]));result.update(sanitizers=['address','undefined'],reference_sha256=sha(LOCAL/'native-body-reference.bin'));(REPORTS/'physics-backend-native-body-replay-host-audit.json').write_text(json.dumps(result,indent=2)+'\n');print('Host original-derived native body replay: pass',flush=True)
 for tag,apk in [('packaged',REPO/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'),('studio',a.studio/'app/build/outputs/apk/debug/app-debug.apk')]:
  library=LOCAL/f'physics-backend-{tag}-world-arm64.so'
  math_library=LOCAL/f'physics-backend-{tag}-math-arm64.so'
  with zipfile.ZipFile(apk) as z:
   library.write_bytes(z.read('lib/arm64-v8a/libdh2_level_world.so'));math_library.write_bytes(z.read('lib/arm64-v8a/libdh2_scene_materials.so'))
  for module,(stem,gold) in specs.items():
   if tag+':'+module in a.skip_packaged_module:continue
   output=LOCAL/f'physics-backend-{tag}-{module}-reference.bin';report=REPORTS/f'physics-backend-{tag}-{module}-arm64-differential.json'
   args=[sys.executable,str(TESTS/f'{stem}_differential.py'),'--engine',str(LOCAL/'libDungeonHunter2.so'),'--library',str(library),'--reference-output',str(output),'--report',str(report)]
   if module=='visual-motion':args+=['--asset-samples',str(a.asset_samples),'--math-library',str(math_library)]
   run(args);assert sha(output)==sha(gold),(module,tag,'Reference changed');print(f'{tag} {module}: pass',flush=True)
if __name__=='__main__':main()
