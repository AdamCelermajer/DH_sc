"""Exact supplied-cache textual dependency inventory; no execution/AI acceptance claim."""
import hashlib,json,re,zipfile
from pathlib import Path
HERE=Path(__file__).resolve().parent
CACHE=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
PREFIX='com.gameloft.android.GAND.GloftD2SS/files/data/scripts/ai/'
def sha(raw):return hashlib.sha256(raw).hexdigest()
assert sha(CACHE.read_bytes())=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
with zipfile.ZipFile(CACHE) as z:
 scripts={n[len(PREFIX):]:z.read(n) for n in z.namelist() if n.startswith(PREFIX) and n.endswith('.luac')}
 rows={}
 for name,raw in scripts.items():
  matches=[]
  for m in re.finditer(rb'\bInclude\s*\(\s*(["\'])([^"\']*)\1',raw):
   requested=m.group(2).decode('utf8','replace');suffix=requested.find('.lua')
   target=requested+'.luac' if suffix<0 else requested if requested[suffix:suffix+5]=='.luac' else requested+'c'
   matches.append(dict(line=raw[:m.start()].count(b'\n')+1,request=requested,normalized=target,present=target in scripts))
  if matches:rows[name]=dict(bytes=len(raw),sha256=sha(raw),literal_includes=matches)
 selected={}
 for root in ('monster.luac','follower.luac','rene.luac'):
  assert root in scripts,root
  closure=[];pending=[root];seen=set()
  while pending:
   name=pending.pop(0)
   if name in seen:continue
   seen.add(name);raw=scripts.get(name)
   if raw is None:closure.append(dict(name=name,missing=True));continue
   row=dict(name=name,sha256=sha(raw),bytes=len(raw),includes=rows.get(name,{}).get('literal_includes',[]))
   row['source_excerpt']=raw[:1800].decode('utf8','replace');closure.append(row)
   pending.extend(m['normalized'] for m in row['includes'])
  selected[root]=closure
 report=dict(cache_sha256=sha(CACHE.read_bytes()),script_sha256=sha(Path(__file__).read_bytes()),ai_files=len(scripts),files_with_literal_includes=len(rows),inventory=rows,selected_crypt_textual_closures=selected,scope=__doc__,limitations=['Regex includes comments and inactive branches; computed Include names are outside this textual inventory.','Source excerpts/dependencies are not proof of execution or complete game bindings.'])
 (HERE/'cache-include-inventory.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps(dict(ai_files=len(scripts),selected={k:[r['name'] for r in v] for k,v in selected.items()})))
