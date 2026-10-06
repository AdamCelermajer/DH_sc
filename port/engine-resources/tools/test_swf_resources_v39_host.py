"""Tiny source-bound SwfGpu allocation/failure fixtures, no GLES or emulator."""
from pathlib import Path
import hashlib,json,subprocess,runpy
root=Path(__file__).resolve().parents[3]
runpy.run_path(str(root/'port/engine-resources/tools/prepare_swf_resource_patch_v39.py'))
out=root/'port/engine-resources/reports/swf-resource-v39'
def unix(p):s=str(Path(p).resolve()).replace('\\','/');return '/mnt/'+s[0].lower()+s[2:]
cpp=(out/'swf_gpu.cpp').read_text();header=(out/'swf_gpu.hpp').read_text()
# Visibility only; no changed production fields, storage or control flow.
test_header=header.replace('private:','public:').replace('"../../../../../engine-resources/retained_bytes_v39.hpp"','"'+unix(root/'port/engine-resources/retained_bytes_v39.hpp')+'"')
(out/'test-header.hpp').write_text(test_header)
barrier=cpp[cpp.index('void SwfGpu::barrier_v37'):cpp.index('void SwfGpu::initialize')]
methods=cpp[cpp.index('void SwfGpu::release_texture_v39'):cpp.index('void SwfGpu::primitive')]
(out/'actual-methods.inc').write_text('namespace dh2::android_ui {\n'+barrier+methods+'\n}\n')
sources=['port/engine-resources/tests/swf_resource_consumer_v39.cpp','port/engine-resources/retained_bytes_v39.cpp','port/engine-resources/resource_budget_v37.cpp']
bindings={p.relative_to(root).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in [out/'swf_gpu.hpp',out/'swf_gpu.cpp',out/'test-header.hpp',out/'actual-methods.inc',root/'port/engine-resources/retained_bytes_v39.hpp',*[root/s for s in sources]]}
receipts=[]
for opt in ['O1','O2']:
 exe=out/('fixture-'+opt)
 command=['g++','-std=c++17','-'+opt,'-g','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-fno-omit-frame-pointer','-pthread','-Iport/engine-ui','-Iport/android-native','-Iport/android-native/app/src/main/cpp','-Iport/engine-ui/reports/swf-gpu-host-v38/headers',*sources,'-o',unix(exe)]
 for command in [command,['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',unix(exe)]]:
  p=subprocess.run(['wsl.exe','--cd',unix(root),'--exec',*command],capture_output=True,text=True,timeout=50)
  receipts.append({'command':command,'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr})
  result={'status':'PASS' if p.returncode==0 else 'FAIL','scope':__doc__,'bindings_sha256':bindings,'receipts':receipts}
  (out/'host-receipt.json').write_text(json.dumps(result,indent=2)+'\n');print(p.returncode,p.stdout,p.stderr[:9000])
  if p.returncode:raise SystemExit(p.returncode)
