from pathlib import Path
import json,struct,hashlib
root=Path(__file__).resolve().parents[3];source=root/'port/level-world/reference/character-clear-aggro/source-probes.json';old=json.loads(source.read_text());assert old['validation']=='PASS';assert old['original_sha256']==hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest()
rows=[];ops={'OnDeAggro':1,'SetTargetNull':2,'CmdStop':3}
for r in old['records']:
 kind,count,identity,forward,reverse,targets,mutation,wrapper=r['input']
 if wrapper:continue
 mask=0
 for event in r['trace']:mask|=1<<ops[event['op']]
 stop=next((t['controller'] for t in r['trace'] if t['op']=='CmdStop'),0)
 rows.append(struct.pack('<9I',identity,forward,reverse,targets,mutation,mask,len(r['after']['outgoing']),len(r['after']['incoming']),stop))
dest=root/'port/level-world/reference/target-cleanup-v40';dest.mkdir(parents=True,exist_ok=True);blob=struct.pack('<I',len(rows))+b''.join(rows);(dest/'clear-original.bin').write_bytes(blob)
(dest/'original-gold.json').write_text(json.dumps({'cases':len(rows),'original_sha256':old['original_sha256'],'original_capture_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'corpus_sha256':hashlib.sha256(blob).hexdigest(),'scope':'Retained original direct ClearAggro contrast captures, actual RB erase/ownership instructions; original OnDeAggro/SetTarget/Stop bodies are supplied endpoints.'},indent=2)+'\n');print(len(rows))
