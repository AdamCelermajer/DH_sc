import json,sys
from pathlib import Path
p=Path(__file__).resolve().parents[1]/'reference/character-menu-native-v1'
rows=json.loads((p/'schema-variants-v1.json').read_text())
if len(sys.argv)>1 and sys.argv[1]=='literals':
 for f in json.loads((p/'original-functions.json').read_text())['functions']:
  if len(sys.argv)==2 or f['name'] in sys.argv[2:]:print(f['name'],f['literal_strings'])
 raise SystemExit
if len(sys.argv)>1:
 lines=(Path(sys.argv[3]) if len(sys.argv)>3 else p/'original-functions.asm').read_text().splitlines()
 print('\n'.join(lines[int(sys.argv[1])-1:int(sys.argv[2])]))
 raise SystemExit
for r in rows[:4]:
 print('\n'+r['callback'])
 last=0
 for w in r['writes']:
  n=w['service_prefix']; print(w['key'],w['type'],r['services'][last:n]);last=n
