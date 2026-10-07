"""Strict compile full staged SWF/shader/cache/budget and existing native caller."""
from pathlib import Path
import hashlib,json,shlex,subprocess
root=Path(__file__).resolve().parents[3];out=root/'port/engine-resources/reports/swf-objects-v43'
baseline=json.loads((out/'baseline-source-sha256.json').read_text())
overlay={'version':0,'case-sensitive':False,'use-external-names':False,'roots':[{'type':'file','name':(root/p).as_posix(),'external-contents':(out/Path(p).name).as_posix()} for p in baseline]}
(out/'android-overlay.json').write_text(json.dumps(overlay,indent=2)+'\n')
results=[]
for abi in ['arm64-v8a','x86_64']:
 commands=json.loads((root/f'port/android-native/app/.cxx/Debug/5a1n3w3m/{abi}/compile_commands.json').read_text())
 model=next(c for c in commands if c['file'].replace('\\','/').endswith('/model_renderer.cpp'))
 original=shlex.split(model['command'],posix=False)
 for source in ['port/android-native/app/src/main/cpp/swf_gpu.cpp','port/android-native/app/src/main/cpp/authored_shader_program.cpp','port/engine-ui/swf_vertex_cache_v36.cpp','port/engine-resources/resource_budget_v37.cpp','port/android-native/app/src/main/cpp/native_app.cpp']:
  command=original.copy();command[command.index('-o')+1]=str(out/(Path(source).stem+'-'+abi+'.o'));command[command.index('-c')+1]=str(root/source)
  command+=['-ivfsoverlay',str(out/'android-overlay.json')]
  p=subprocess.run(command,capture_output=True,text=True,timeout=55)
  staged=out/Path(source).name;receipt=staged if staged.exists() else root/source
  results.append({'source':source,'staged_sha256':hashlib.sha256(receipt.read_bytes()).hexdigest(),'abi':abi,'command':command,'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr})
  (out/'android-compile.json').write_text(json.dumps({'status':'PASS' if p.returncode==0 else 'FAIL','scope':__doc__,'results':results,'overlay':overlay},indent=2)+'\n');print(abi,Path(source).name,p.returncode,p.stderr[:10000])
  if p.returncode:raise SystemExit(p.returncode)
