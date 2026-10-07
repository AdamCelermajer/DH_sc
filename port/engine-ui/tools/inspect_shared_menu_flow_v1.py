"""Capture the authored shared UI constructors and exported control behavior."""
from pathlib import Path
import hashlib,json,zlib
import swf_action_decoder_v1 as decoder
ROOT=Path(__file__).resolve().parents[3]
HERE=ROOT/'port/engine-ui/reference/shared-menu-flow-v1'
HERE.mkdir(parents=True,exist_ok=True)
raw=(ROOT/'port/android-native/app/src/main/assets/original-cache/data/menus/dqshared_droid.swf').read_bytes()
b=raw[:8]+zlib.decompress(raw[8:]) if raw[:3]==b'CWS' else raw
tree=ROOT/'.local-inputs/ui-layout-discovery/dqshared_droid-tree.json'
tags=json.loads(tree.read_text())
blocks=[];assembly=[];functions=[];strings=set()
def walk(rows,path):
 for row in rows:
  strings.update(row.get('constants',[]))
  for value in row.get('values',[]):
   if isinstance(value,str):strings.add(value)
   if isinstance(value,dict) and 'text' in value:strings.add(value['text'])
  if 'body' in row:
   functions.append({'name':row['name'],'offset':row['offset'],'path':path,'args':row['args']})
   walk(row['body'],path+'/'+row['name'])
for path,t in decoder.flatten(tags):
 ranges=decoder.clips(b,t) if t['tag'] in (26,70) else [(t['offset']+(2 if t['tag']==59 else 0),t['offset']+t['length'],None,None)]
 for start,end,event,key in ranges:
  rows=decoder.decode(b,start,end,label=path)
  blocks.append({'path':path,'tag':t['tag'],'frame':t['frame'],'offset':start,'event_flags':event,'key':key,'rows':rows})
  assembly.append(f'BLOCK {path} FRAME {t["frame"]} OFFSET {start:08x}')
  assembly.extend(decoder.lines(rows));walk(rows,path)
data={'resource_sha256':hashlib.sha256(raw).hexdigest(),'tree_sha256':hashlib.sha256(tree.read_bytes()).hexdigest(),'blocks':blocks}
(HERE/'authored-actions.json').write_text(json.dumps(data,indent=2,ensure_ascii=False)+'\n',encoding='utf8')
(HERE/'authored-actions.txt').write_text('\n'.join(assembly)+'\n',encoding='utf8')
summary={**{k:v for k,v in data.items() if k!='blocks'},'blocks':len(blocks),'functions':functions,'strings':sorted(strings),'limits':'Authored bytecode capture; no native menu lifecycle claim.'}
(HERE/'authored-flow-index.json').write_text(json.dumps(summary,indent=2,ensure_ascii=False)+'\n',encoding='utf8')
print(json.dumps({'blocks':len(blocks),'functions':len(functions),'strings':len(strings),'resource_sha256':data['resource_sha256']}))
