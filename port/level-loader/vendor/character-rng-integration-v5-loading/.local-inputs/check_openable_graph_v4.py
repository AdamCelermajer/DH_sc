import json,subprocess,shlex
from pathlib import Path
repo=Path.cwd()
for abi in ['arm64-v8a','x86_64']:
 p=max((repo/'port/android-native/app/.cxx').glob('**/'+abi+'/compile_commands.json'),key=lambda x:x.stat().st_mtime)
 entry=next(x for x in json.loads(p.read_text()) if x['file'].endswith('model_renderer.cpp'))
 base=entry.get('arguments') or [x.strip('"') for x in shlex.split(entry['command'],posix=False)]
 if '-o' in base:
  i=base.index('-o');del base[i:i+2]
 cmd=[str(repo/'port/level-world/canonical_openable_graph_v4.cpp') if x.replace('\\','/').endswith('/model_renderer.cpp') else x for x in base if x!='-c']+['-fsyntax-only']
 r=subprocess.run(cmd,cwd=entry['directory'],text=True,capture_output=True)
 print(abi,r.returncode,r.stdout,r.stderr)
 if r.returncode:raise SystemExit(r.returncode)
