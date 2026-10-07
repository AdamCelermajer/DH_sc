"""Strict component ABI compile of texture adapters/readers; no APK/device."""
from pathlib import Path
import hashlib,json,subprocess
root=Path(__file__).resolve().parents[3];out=root/'port/engine-resources/reports/texture-resource-v40'
ndk=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64')
# Keep actual source bodies untouched while making quoted root-header imports
# resolve to the staged API globally (including OriginalCache's ZIP import).
overlay={'version':0,'case-sensitive':False,'use-external-names':False,'roots':[
 {'type':'file','name':(root/p).as_posix(),'external-contents':(out/Path(p).name).as_posix()}
 for p in ['port/asset-payloads/zip_asset_pack_v1.hpp','port/android-native/app/src/main/cpp/original_cache_assets_v1.hpp']]}
(out/'android-overlay.json').write_text(json.dumps(overlay,indent=2)+'\n')
includes=['port/android-native','port/android-native/app/src/main/cpp','port/engine-textures']
sources=[root/'port/engine-resources/admitted_cpu_bytes_v40.cpp',root/'port/engine-resources/tests/texture_resource_consumer_v40.cpp',out/'zip_asset_pack_v1.cpp',out/'original_cache_assets_v1.cpp']
results=[]
for abi,target in [('arm64-v8a','aarch64-none-linux-android24'),('x86_64','x86_64-none-linux-android24')]:
 for source in sources:
  command=[str(ndk/'bin/clang++.exe'),'--target='+target,'--sysroot='+str(ndk/'sysroot'),'-ivfsoverlay',str(out/'android-overlay.json'),'-std=c++17','-O2','-Wall','-Wextra','-Werror','-fno-fast-math','-ffp-contract=off',*['-I'+str(root/p) for p in includes],'-c',str(source),'-o',str(out/(source.stem+'-'+abi+'.o'))]
  p=subprocess.run(command,capture_output=True,text=True,timeout=45)
  results.append({'source':source.relative_to(root).as_posix(),'sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'abi':abi,'command':command,'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr})
  (out/'android-compile.json').write_text(json.dumps({'status':'PASS' if p.returncode==0 else 'FAIL','scope':__doc__,'results':results,'overlay':overlay},indent=2)+'\n')
  print(abi,source.name,p.returncode,p.stderr[:9000])
  if p.returncode:raise SystemExit(p.returncode)
