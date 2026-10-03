"""Read-only textual VFTable inventory of the supplied cache; no script execution claim."""
import hashlib,json,re,zipfile
from pathlib import Path
HERE=Path(__file__).resolve().parent
cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
digest=hashlib.sha256(cache.read_bytes()).hexdigest()
assert digest=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
rows=[];count=0;commons=None
with zipfile.ZipFile(cache) as z:
 for name in z.namelist():
  if '/scripts/' not in name or not name.endswith('.luac'):continue
  count+=1;raw=z.read(name);findings=[]
  for operation in ('AddToVFTable','PushVFTable','PopVFTable'):
   for match in re.finditer(operation.encode(),raw):
    line=raw[:match.start()].count(b'\n');findings.append({'name':operation,'line':line+1,'excerpt':raw.splitlines()[line].decode('utf8','replace')})
  if name.endswith('/scripts/ai/_commons.luac'):commons={'sha256':hashlib.sha256(raw).hexdigest(),'alias_names_present':bool(findings)}
  if findings:rows.append({'cache_entry':name,'sha256':hashlib.sha256(raw).hexdigest(),'findings':findings})
assert commons and not commons['alias_names_present']
report={'cache_sha256':digest,'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        'scripts_inspected':count,'scripts_with_alias_names':len(rows),'commons':commons,
        'scope':'Textual inventory of exact supplied cache. Names are not proof of executed control-flow or exhaustive dynamic name creation.',
        'observations':rows}
(HERE/'cache-alias-inventory.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ('scripts_inspected','scripts_with_alias_names','commons')}))
