from pathlib import Path
import subprocess,json,hashlib
root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');base=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=base/'reference/device-provider-v54';reports=base/'reports';out=r/'device_provider_v54_test';w=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
sources=[r/'device_provider_v54_test.cpp',root/'port/engine-resources/resource_budget_v37.cpp',root/'port/engine-ui/hud_startup_callbacks.cpp']
cmd=['wsl.exe','-d','Ubuntu','--','c++','-std=c++17','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-O1','-pthread','-I'+w(r),'-I'+w(root/'port/engine-resources'),'-I'+w(root/'port/engine-ui')]+[w(s) for s in sources]+['-o',w(out)]
p=subprocess.run(cmd,capture_output=True,text=True);(reports/'device-provider-v54-build.log').write_text(p.stdout+p.stderr);print((p.stdout+p.stderr)[-6000:]);assert p.returncode==0
p=subprocess.run(['wsl.exe','-d','Ubuntu','--',w(out)],capture_output=True,text=True);print(p.stdout+p.stderr);assert p.returncode==0
receipt={'schema':'dh2-device-provider-v54-host','status':'PASS','sources_sha256':{str(s):hashlib.sha256(s.read_bytes()).hexdigest() for s in sources+[r/'actual_device_provider_v54.hpp',r/'native_device_provider_v54.inc']},'stdout':p.stdout,'binary_sha256':hashlib.sha256(out.read_bytes()).hexdigest(),'actual_ledger_and_existing_device_kernel':True,'egl_queries_in_host_are_explicit_fixtures':True,'android_or_whole_character_activation_proven':False,'selected_shared_writes':False}
(reports/'device-provider-v54-verified.json').write_text(json.dumps(receipt,indent=2)+'\n');(r/'build_device_v54.py').write_bytes(Path(__file__).read_bytes())
