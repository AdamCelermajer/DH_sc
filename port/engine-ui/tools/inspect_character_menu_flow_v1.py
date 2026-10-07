"""Decode original in-game screen flow without executing or fabricating calls."""
from pathlib import Path
import hashlib,json,zlib,struct
import swf_action_decoder_v1 as decoder
ROOT=Path(__file__).resolve().parents[3]
HERE=ROOT/'port/engine-ui/reference/character-menu-flow-v1'
HERE.mkdir(parents=True,exist_ok=True)
stem='dqcharmenu_droid'
raw=(ROOT/'port/android-native/app/src/main/assets/original-cache/data/menus'/f'{stem}.swf').read_bytes()
assert hashlib.sha256(raw).hexdigest()=='43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0'
b=raw[:8]+zlib.decompress(raw[8:]) if raw[:3]==b'CWS' else raw
tree=ROOT/'.local-inputs/ui-layout-discovery'/f'{stem}-tree.json'
ts=json.loads(tree.read_text())
blocks=[];assembly=[];functions=[];labels=[];strings=set()
def walk(rows,path):
 for row in rows:
  if 'constants' in row:strings.update(row['constants'])
  for value in row.get('values',[]):
   if isinstance(value,str):strings.add(value)
   if isinstance(value,dict) and 'text' in value:strings.add(value['text'])
  if 'body' in row:
   functions.append({'name':row['name'],'offset':row['offset'],'path':path,'args':row['args']})
   walk(row['body'],path+'/'+row['name'])
def collect_labels(tags,path='root'):
 for t in tags:
  if t['tag']==43:labels.append({'path':path,'frame':t['frame'],'label':decoder.string(b,t['offset'])[0]})
  if 'children' in t:collect_labels(t['children'],path+'/sprite'+str(t['id']))
collect_labels(ts)
for path,t in decoder.flatten(ts):
 if t['tag'] in (26,70):
  for start,end,event,key in decoder.clips(b,t):
   rows=decoder.decode(b,start,end,label=path+'/clipaction')
   blocks.append({'path':path,'tag':t['tag'],'frame':t['frame'],'offset':start,'event_flags':event,'key':key,'placement_depth':t['placement_depth'],'rows':rows})
   assembly.append(f'CLIP {path} FRAME {t["frame"]} OFFSET {start:08x}')
   assembly.extend(decoder.lines(rows));walk(rows,path)
  continue
 start=t['offset']+(2 if t['tag']==59 else 0)
 rows=decoder.decode(b,start,t['offset']+t['length'],label=path)
 blocks.append({'path':path,'tag':t['tag'],'frame':t['frame'],'offset':start,'rows':rows})
 assembly.append(f'BLOCK {path} FRAME {t["frame"]} OFFSET {start:08x}')
 assembly.extend(decoder.lines(rows));walk(rows,path)
data={'original_sha256':hashlib.sha256(raw).hexdigest(),'tree_sha256':hashlib.sha256(tree.read_bytes()).hexdigest(),'blocks':blocks}
(HERE/'authored-actions.json').write_text(json.dumps(data,indent=2,ensure_ascii=False)+'\n',encoding='utf8')
(HERE/'authored-actions.txt').write_text('\n'.join(assembly)+'\n',encoding='utf8')
summary={'validation':'PASS','resource_sha256':data['original_sha256'],'tree_sha256':data['tree_sha256'],'blocks':len(blocks),'functions':functions,'labels':labels,'strings':sorted(strings),'limits':'Read-only authored action/label evidence; no live screen or original native whole-menu execution'}
(HERE/'authored-flow-index.json').write_text(json.dumps(summary,indent=2,ensure_ascii=False)+'\n',encoding='utf8')
print(json.dumps({'validation':'PASS','blocks':len(blocks),'functions':len(functions),'labels':len(labels),'strings':len(strings)}))
