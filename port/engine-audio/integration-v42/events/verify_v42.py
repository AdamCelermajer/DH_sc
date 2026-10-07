from pathlib import Path
import json,shlex,subprocess,hashlib
O=Path(__file__).resolve().parent;R=O.parents[3];P=O/'prospective'
results=[]
for abi in ('arm64-v8a','x86_64'):
 db=R/f'port/android-native/app/.cxx/Debug/5a1n3w3m/{abi}/compile_commands.json'
 row=next(e for e in json.loads(db.read_text())if Path(e['file']).name=='model_renderer.cpp')
 args=row.get('arguments')or shlex.split(row['command'],posix=False);args=[a.strip('"')for a in args]
 filtered=[];i=0
 while i<len(args):
  if args[i]in ('-o','-c'):i+=2;continue
  filtered.append(args[i]);i+=1
 args=[filtered[0],'-I'+str(R/'port/android-native/app/src/main/cpp'),*filtered[1:],'-fsyntax-only',str(P/'port/android-native/app/src/main/cpp/model_renderer.cpp')]
 p=subprocess.run(args,cwd=R,capture_output=True,text=True,timeout=60)
 results.append(dict(abi=abi,exit_code=p.returncode,diagnostics=p.stdout+p.stderr,compile_database_sha256=hashlib.sha256(db.read_bytes()).hexdigest()))
 print(abi,'PASS'if not p.returncode else p.stdout+p.stderr,flush=True)
(O/'syntax-receipt.json').write_text(json.dumps(dict(scope='Isolated prospective active model TU syntax only; no app build or shared edits',results=results),indent=2)+'\n')
assert all(not row['exit_code']for row in results)
