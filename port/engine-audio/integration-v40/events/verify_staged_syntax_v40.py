from pathlib import Path
import json,shlex,subprocess,hashlib
O=Path(__file__).resolve().parent;R=O.parents[3];V=O/'verify-tree'
files=['port/android-native/app/src/main/cpp/model_renderer.cpp','port/level-world/world_item_graph_v3.cpp','port/level-world/world_loot_pickup_v23.cpp','port/level-world/openable_container_owner_v1.cpp','port/level-world/canonical_destructible_container_v16.cpp']
results=[]
for abi in ('arm64-v8a','x86_64'):
 db=R/f'port/android-native/app/.cxx/Debug/5a1n3w3m/{abi}/compile_commands.json';rows=json.loads(db.read_text())
 model=next(e for e in rows if Path(e['file']).name=='model_renderer.cpp')
 for path in files:
  entry=next((e for e in rows if Path(e['file']).name==Path(path).name),model)
  args=entry.get('arguments')or shlex.split(entry['command'],posix=False);args=[a.strip('"')for a in args]
  filtered=[];i=0
  while i<len(args):
   if args[i]in ('-o','-c'):i+=2;continue
   filtered.append(args[i]);i+=1
  args=[filtered[0],'-I'+str(V/'port/level-world'),'-I'+str(R/'port/android-native/app/src/main/cpp'),*filtered[1:],'-fsyntax-only',str(V/path)]
  p=subprocess.run(args,cwd=R,capture_output=True,text=True,timeout=60)
  results.append(dict(abi=abi,file=path,exit_code=p.returncode,diagnostics=p.stdout+p.stderr,compile_database_sha256=hashlib.sha256(db.read_bytes()).hexdigest()))
  print(abi,Path(path).name,'PASS'if p.returncode==0 else 'FAIL',flush=True)
  if p.returncode:print(p.stdout+p.stderr,flush=True)
(O/'staged-syntax-receipt.json').write_text(json.dumps(dict(scope='Syntax-only isolated prospective patch tree; actual cached NDK TU flags. No root build/link/APK.',results=results),indent=2)+'\n')
assert all(r['exit_code']==0 for r in results)
