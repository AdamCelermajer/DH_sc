"""Tiny source-bound generic GPU buffer, FX capacity and real skin fixtures."""
from pathlib import Path
import hashlib,json,runpy,subprocess
root=Path(__file__).resolve().parents[3];runpy.run_path(str(root/'port/engine-resources/tools/prepare_buffer_patch_v41.py'))
out=root/'port/engine-resources/reports/buffer-resource-v41';layout=out/'test-layout'
def unix(p):s=str(Path(p).resolve()).replace('\\','/');return '/mnt/'+s[0].lower()+s[2:]
copies={
'engine-resources/resource_budget_v37.hpp':out/'resource_budget_v37.hpp',
'engine-resources/resource_budget_v37.cpp':out/'resource_budget_v37.cpp',
'engine-resources/cpu_vector_capacity_v41.hpp':root/'port/engine-resources/cpu_vector_capacity_v41.hpp',
'level-world/authored_fx_geometry_packet_v7.hpp':out/'authored_fx_geometry_packet_v7.hpp',
'level-world/authored_fx_geometry_packet_v7.cpp':out/'authored_fx_geometry_packet_v7.cpp'}
for path,source in copies.items():
 destination=layout/path;destination.parent.mkdir(parents=True,exist_ok=True);destination.write_bytes(source.read_bytes())
sources=['port/engine-resources/tests/buffer_resource_consumer_v41.cpp',(layout/'engine-resources/resource_budget_v37.cpp').relative_to(root).as_posix(),(layout/'level-world/authored_fx_geometry_packet_v7.cpp').relative_to(root).as_posix(),'port/engine-skinning/skinning.cpp']
paths=[*sources,*[str((layout/p).relative_to(root)).replace('\\','/') for p in copies],'port/android-native/app/src/main/cpp/renderer_buffer_budget_v41.inc','port/engine-resources/cpu_vector_capacity_v41.hpp']
bindings={p:hashlib.sha256((root/p).read_bytes()).hexdigest() for p in paths}
receipts=[]
for opt in ['O1','O2']:
 exe=out/('fixture-'+opt)
 command=['g++','-std=c++17','-'+opt,'-g','-Wall','-Wextra','-Werror','-ffunction-sections','-fdata-sections','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-fast-math','-ffp-contract=off','-pthread','-Iport/level-world','-Iport/game-data','-Iport/scene-materials','-Iport/engine-skinning','-Iport/engine-animation','-Iport/script-runtime','-Iport/engine-ui/reports/swf-gpu-host-v38/headers',*sources,'-Wl,--gc-sections','-o',unix(exe)]
 for command in [command,['timeout','--signal=TERM','--kill-after=2','20s','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',unix(exe)]]:
  p=subprocess.run(['wsl.exe','--cd',unix(root),'--exec',*command],capture_output=True,text=True,timeout=55)
  receipts.append({'command':command,'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr})
  (out/'host-receipt.json').write_text(json.dumps({'status':'PASS' if p.returncode==0 else 'FAIL','scope':__doc__,'bindings_sha256':bindings,'receipts':receipts},indent=2)+'\n')
  print(p.returncode,p.stdout,p.stderr[:9000])
  if p.returncode:raise SystemExit(p.returncode)
