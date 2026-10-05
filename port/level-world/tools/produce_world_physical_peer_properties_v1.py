"""Actual current Crypt AnimatedDecor XML inventory; frozen DACT unchanged."""
from pathlib import Path
import hashlib,json,struct,zipfile,xml.etree.ElementTree as ET
from produce_actor_initialization import CACHE,ORIGINAL,text
root=Path(__file__).resolve().parents[3];assets=root/'port/android-native/app/src/main/assets'
descriptor=(assets/'worlds/crypt01.dact').read_bytes();layout=json.loads((assets/'worlds/crypt01-provenance.json').read_text())
cache=Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
sha=lambda x:hashlib.sha256(x).hexdigest()
with cache.open('rb')as f:assert hashlib.file_digest(f,'sha256').hexdigest()==CACHE
assert descriptor[:4]==b'DACT';rows=[];sources=[];xml={}
with zipfile.ZipFile(cache)as z:
 for room,r in enumerate(layout['rooms']):
  matches=[n for n in z.namelist()if n.endswith('/'+r['visual'])];assert len(matches)==1
  raw=z.read(matches[0]);sources.append(dict(room=room,entry=matches[0],sha256=sha(raw)))
  xml[room]={n.get('name'):n for n in ET.fromstring(raw).iter('GameObject')}
 for i in range(struct.unpack_from('<I',descriptor,8)[0]):
  p=16+256*i;kind,room=struct.unpack_from('<II',descriptor,p)
  if kind!=2:continue
  name=descriptor[p+8:p+72].split(b'\0')[0].decode();n=xml[room][name]
  assert n.get('gametype')=='AnimatedDecor' and n.get('_templateName')=='AnimatedDecor'
  assert all(n.get(k)is None for k in ['visible','static','activate_cond','deactivate_cond'])
  rows.append(dict(descriptor_index=i,room=room,name=name,source_type=20,visible_default=1,static_default=1))
assert len(rows)==84
payload=b''.join(struct.pack('<II',r['descriptor_index'],r['room'])+text(r['name']) for r in rows)
binary=struct.pack('<4sIII',b'CPP1',1,80+len(payload),84)+bytes.fromhex(sha(descriptor))+bytes.fromhex(ORIGINAL)+payload
out=root/'port/level-world/reference/character-world-physical-peers-v1';out.mkdir(exist_ok=True)
(out/'crypt01-decor-properties.bin').write_bytes(binary)
manifest=dict(validation='PASS',format='CPP1',source_cache_sha256=CACHE,original_sha256=ORIGINAL,descriptor_sha256=sha(descriptor),binary_sha256=sha(binary),sources=sources,rows=rows,full_AnimatedDecor_InitPost=False)
(out/'crypt01-decor-properties.json').write_text(json.dumps(manifest,indent=2)+'\n');print(json.dumps({k:v for k,v in manifest.items()if k not in ['sources','rows']}))
