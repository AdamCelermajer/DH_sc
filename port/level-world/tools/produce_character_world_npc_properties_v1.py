"""Bounded source-visible/static descriptor inputs for the actual Crypt NPCs.
Reads canonical source XML; does not extend frozen CAI1 or DACT artifacts."""
from pathlib import Path
import json,hashlib,struct,zipfile,xml.etree.ElementTree as ET
from produce_actor_initialization import dact_keys,CACHE,ORIGINAL,text,authored
root=Path(__file__).resolve().parents[3]
cache=Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
descriptor=root/'port/android-native/app/src/main/assets/worlds/crypt01.dact'
layout=json.loads((root/'port/android-native/app/src/main/assets/worlds/crypt01-provenance.json').read_text())
sha=lambda b:hashlib.sha256(b).hexdigest()
with cache.open('rb') as f:assert hashlib.file_digest(f,'sha256').hexdigest()==CACHE
keys=dact_keys(descriptor.read_bytes());rows=[];sources=[];xml={}
with zipfile.ZipFile(cache) as z:
 for room in sorted({k['room'] for k in keys}):
  matches=[n for n in z.namelist() if n.endswith('/'+layout['rooms'][room]['gameplay'])];assert len(matches)==1
  raw=z.read(matches[0]);sources.append(dict(room=room,entry=matches[0],bytes=len(raw),sha256=sha(raw)))
  xml[room]={n.get('name'):n for n in ET.fromstring(raw).iter('GameObject')}
 for key in keys:
  node=xml[key['room']][key['name']];assert node.get('gametype')=='Character' and node.get('charpropsname')==key['character'] and node.get('_templateName')=='Monster'
  values={name:node.get(name) for name in ('visible','static')}
  # This bounded source inventory has no authored override. Other actors need
  # actual FromString/template producer support rather than guessed parsing.
  assert all(v is None for v in values.values()),values
  rows.append(dict(**key,authored=values,resolved=dict(visible=1,static=0)))
payload=b''.join(struct.pack('<I',r['room'])+text(r['name'])+text(r['character'])+authored(r['authored']['visible'])+authored(r['authored']['static']) for r in rows)
binary=struct.pack('<4s3I',b'CPO1',1,80+len(payload),11)+bytes.fromhex(sha(descriptor.read_bytes()))+bytes.fromhex(ORIGINAL)+payload
out=root/'port/level-world/reference/character-world-npc-object-v1'
manifest=dict(format='CPO1',version=1,cache_sha256=CACHE,descriptor_sha256=sha(descriptor.read_bytes()),original_sha256=ORIGINAL,
 source_properties=dict(visible=dict(offset='0x80',default=1),static=dict(offset='0x84',default=0)),
 producer_sha256=sha(Path(__file__).read_bytes()),binary_sha256=sha(binary),sources=sources,records=rows,full_XML_factory=False)
for path,raw in [(out/'crypt01-object-properties.bin',binary),(out/'crypt01-object-properties.json',(json.dumps(manifest,indent=2)+'\n').encode())]:
 if path.exists():assert path.read_bytes()==raw,('refusing mismatched existing sidecar',path)
 else:path.write_bytes(raw)
print(json.dumps(dict(validation='PASS',actors=len(rows),source_members=len(sources),visible_overrides=0,static_overrides=0,binary_sha256=sha(binary))))
