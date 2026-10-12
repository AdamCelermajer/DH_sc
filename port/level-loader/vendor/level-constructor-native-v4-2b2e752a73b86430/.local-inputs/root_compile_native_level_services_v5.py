from pathlib import Path
import hashlib,json,os,shlex,subprocess
root=Path(__file__).resolve().parent.parent
out=root/'port/level-loader/reports/native-level-services-v5';out.mkdir(parents=True,exist_ok=True)
temp=out/'compiler-temp';temp.mkdir(exist_ok=True);environment=dict(os.environ,TMP=str(temp),TEMP=str(temp))
sources=['port/level-loader/level_constructor_v3.cpp','port/level-loader/canonical_level_context_v1.cpp','port/level-loader/level_constructor_bindings_v4.cpp','port/level-world/application_services_owner_v5.cpp','port/android-native/app/src/main/cpp/native_app.cpp','port/android-native/app/src/main/cpp/model_renderer.cpp']
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
rows=[]
for abi in ['arm64-v8a','x86_64']:
 db=json.loads((root/f'port/android-native/app/.cxx/Debug/5a1n3w3m/{abi}/compile_commands.json').read_text())
 entry=next(e for e in db if e['file'].replace('\\','/').endswith('/model_renderer.cpp'))
 args=entry.get('arguments') or [s.strip('"') for s in shlex.split(entry['command'],posix=False)]
 flags=[]
 for i,s in enumerate(args[1:],1):
  if s.startswith(('--target=','--sysroot=','-I','-isystem','-D')):flags.append(s)
  elif args[i-1]=='-isystem':flags.append(s)
 for source in sources:
  command=[args[0],*flags,'-std=c++17','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fsyntax-only',str(root/source)]
  run=subprocess.run(command,capture_output=True,env=environment,timeout=55)
  (out/(abi+'-'+Path(source).stem+'.log')).write_bytes(run.stdout+run.stderr)
  print(abi,source,run.returncode,flush=True);assert run.returncode==0,run.stderr.decode(errors='replace')
  rows.append({'abi':abi,'source':source,'sha256':sha(root/source),'status':'PASS'})
(out/'compile-receipt.json').write_text(json.dumps({'status':'PASS','translation_units':rows,'live_app_updated':False,'whole_Application_PostInit':False},indent=2)+'\n')
