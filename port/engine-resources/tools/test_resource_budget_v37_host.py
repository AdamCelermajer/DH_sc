"""Tiny isolated CPU tests; no GL, emulator, stress allocation or shared rebuild."""
from pathlib import Path
import hashlib,json,subprocess
root=Path(__file__).resolve().parents[3]
unix='/mnt/'+str(root.resolve())[0].lower()+str(root.resolve())[2:].replace('\\','/')
out=root/'port/engine-resources/reports/resource-budget-v37';out.mkdir(parents=True,exist_ok=True)
source_root=root/'port/android-native/app/src/main/cpp/renderer_authored_effect_scene_v5.inc'
text=source_root.read_text();start=text.index('static void release_effect_buffers_v38');end=text.index('dh2::textures::TextureDriverOptionsOwnerV1 effect_driver_options_v4;')
helper=text[start:end];(out/'fx-production-helper-v38.inc').write_text(helper)
source_binding={'source':source_root.relative_to(root).as_posix(),'source_sha256':hashlib.sha256(source_root.read_bytes()).hexdigest(),'helper_sha256':hashlib.sha256(helper.encode()).hexdigest()}
base=['port/engine-resources/resource_budget_v37.cpp']
receipts=[]
for name,test in [('core','resource_budget_v37.cpp'),('actual-fx-consumer','resource_budget_fx_consumer_v38.cpp')]:
 for opt in ['O1','O2']:
  sources=['port/engine-resources/tests/'+test,*base]
  exe='port/engine-resources/reports/resource-budget-v37/'+name+'-test-'+opt
  commands=[['g++','-std=c++17','-'+opt,'-g','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-fno-omit-frame-pointer','-pthread',*sources,'-o',exe],
           ['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1','./'+exe]]
  for command in commands:
   p=subprocess.run(['wsl.exe','--cd',unix,'--exec',*command],capture_output=True,text=True,timeout=40)
   receipts.append({'case':name,'command':command,'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr})
   result={'status':'PASS' if p.returncode==0 else 'FAIL','scope':__doc__,'receipts':receipts,'actual_source_binding':source_binding,
           'source_sha256':{s:hashlib.sha256((root/s).read_bytes()).hexdigest() for s in [*base,'port/engine-resources/resource_budget_v37.hpp','port/engine-resources/tests/resource_budget_v37.cpp','port/engine-resources/tests/resource_budget_fx_consumer_v38.cpp','port/engine-resources/tools/test_resource_budget_v37_host.py']}}
   (out/'host-receipt.json').write_text(json.dumps(result,indent=2)+'\n')
   print(p.returncode,p.stdout,p.stderr[:7000])
   if p.returncode:raise SystemExit(p.returncode)
