"""Compile staged complete HUD owner and CPU helper; no APK/device operations."""
from pathlib import Path
import hashlib,json,subprocess
root=Path(__file__).resolve().parents[3];out=root/'port/engine-resources/reports/swf-resource-v39'
ndk=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64')
include=['port/android-native/app/src/main/cpp','port/android-native','port/engine-ui','port/engine-math','port/scene-materials','port/engine-textures','port/level-world','port/engine-skinning','port/engine-animation','port/game-data','port/script-runtime','port/engine-ui/overlays/edit-text-v1']
results=[]
for abi,target in [('arm64-v8a','aarch64-none-linux-android24'),('x86_64','x86_64-none-linux-android24')]:
 for source in [root/'port/engine-resources/retained_bytes_v39.cpp',out/'swf_gpu.cpp']:
  command=[str(ndk/'bin/clang++.exe'),'--target='+target,'--sysroot='+str(ndk/'sysroot'),'-std=c++17','-O2','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror',*['-I'+str(root/p) for p in include],'-c',str(source),'-o',str(out/(source.stem+'-'+abi+'.o'))]
  run=subprocess.run(command,capture_output=True,text=True,timeout=45)
  results.append({'source':source.relative_to(root).as_posix(),'sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'abi':abi,'command':command,'exit_code':run.returncode,'stdout':run.stdout,'stderr':run.stderr})
  (out/'android-compile.json').write_text(json.dumps({'status':'PASS' if run.returncode==0 else 'FAIL','scope':__doc__,'results':results},indent=2)+'\n')
  print(abi,source.name,run.returncode,run.stderr[:8000])
  if run.returncode:raise SystemExit(run.returncode)
