"""Produce CAI1 for the current eleven DACT Crypt monsters from actual cache XML.

This is an additive bounded port format, not the shipping XML/property loader.
Authored numeric fields outside this source inventory are explicitly rejected.
"""
import argparse,hashlib,json,struct,zipfile,xml.etree.ElementTree as ET
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
CACHE='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
ORIGINAL='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
CAPTURE='dbff29cffe8c865eed384a0907722dee9ecf6dd272096ebcbab3cbee544cc2e7'
FIELDS=('ai_state','ai_state_visible','auto_spawn','spawn_delay','spawn_view_radius','char_group','char_group_role')
sha=lambda b:hashlib.sha256(b).hexdigest()
def text(s):
 b=s.encode('ascii');assert len(b)<=4096 and all(32<=c<=126 for c in b);return struct.pack('<I',len(b))+b
def authored(s):return struct.pack('<I',s is not None)+text(s or '')
def dact_keys(raw):
 assert len(raw)>=16 and struct.unpack_from('<4sIII',raw)==(b'DACT',1,(len(raw)-16)//256,0) and (len(raw)-16)%256==0
 out=[]
 def fixed(b):
  end=b.index(0);assert not any(b[end:]);return b[:end].decode('ascii')
 for offset in range(16,len(raw),256):
  kind,room=struct.unpack_from('<II',raw,offset)
  if kind==1:out.append(dict(room=room,name=fixed(raw[offset+8:offset+72]),character=fixed(raw[offset+72:offset+136])))
 assert len(out)==11 and len({(r['room'],r['name']) for r in out})==11
 return out
def write(path,raw,verify):
 if path.exists():assert path.read_bytes()==raw,('Existing artifact differs; refusing overwrite',str(path))
 else:
  assert not verify,('Missing verified artifact',str(path));path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(raw)
def produce(cache,descriptor,provenance):
 with cache.open('rb') as f:assert hashlib.file_digest(f,'sha256').hexdigest()==CACHE
 raw=descriptor.read_bytes();keys=dact_keys(raw);layout=json.loads(provenance.read_text());assert layout['cache_sha256']==CACHE
 capture=ROOT/'reference/character-live-owner-readiness/original-functions.json';assert sha(capture.read_bytes())==CAPTURE
 sources=[];records=[];payload=bytearray()
 with zipfile.ZipFile(cache) as z:
  source_by_room={};xml={}
  for room in sorted({k['room'] for k in keys}):
   suffix='/'+layout['rooms'][room]['gameplay'];matches=[n for n in z.namelist() if n.endswith(suffix)];assert len(matches)==1
   entry=matches[0];b=z.read(entry);source_by_room[room]=len(sources);source=dict(room=room,bytes=len(b),cache_entry=entry,sha256=sha(b));sources.append(source)
   xml[room]={n.get('name'):n for n in ET.fromstring(b).iter('GameObject')}
   payload+=struct.pack('<II',room,len(b))+bytes.fromhex(source['sha256'])+text(entry)
  assert len(sources)==3
  for key in keys:
   room,name,character=key['room'],key['name'],key['character'];n=xml[room][name]
   assert n.get('gametype')=='Character' and n.get('charpropsname')==character and n.get('_templateName')=='Monster'
   a={field:n.get(field) for field in FIELDS};assert all(a[field] is None for field in FIELDS[1:5])
   assert a['ai_state'] in (None,'Idle') and a['char_group'] in (None,'') and a['char_group_role'] in (None,'Normal')
   v=dict(ai_state=a['ai_state'] or '',ai_state_visible=True,auto_spawn=True,spawn_delay=[0,0],spawn_view_radius=0.0,char_group=a['char_group'] or '',char_group_role=a['char_group_role'] or '',preset_state=3)
   row=dict(**key,source_index=source_by_room[room],requested_template=n.get('_templateName'),authored=a,resolved=v);records.append(row)
   payload+=struct.pack('<II',room,row['source_index'])+text(name)+text(character)+authored(row['requested_template'])+b''.join(authored(a[field]) for field in FIELDS)
   payload+=text(v['ai_state'])+struct.pack('<IIiiIi',1,1,0,0,0,3)+text(v['char_group'])+text(v['char_group_role'])
 assert sum(r['authored']['ai_state'] is None for r in records)==6 and sum(r['authored']['ai_state']=='Idle' for r in records)==5
 header=struct.pack('<4s7I',b'CAI1',1,160+len(payload),11,3,7,0,0)+b''.join(bytes.fromhex(h) for h in (CACHE,sha(raw),ORIGINAL,CAPTURE))
 binary=header+payload
 manifest=dict(format='CAI1',version=1,scope=__doc__,binary_sha256=sha(binary),binary_bytes=len(binary),cache_sha256=CACHE,descriptor_sha256=sha(raw),original_sha256=ORIGINAL,default_capture_sha256=CAPTURE,world_provenance_sha256=sha(provenance.read_bytes()),producer_sha256=sha(Path(__file__).read_bytes()),sources=sources,records=records,full_original_XML_factory_parity=False,scene_initialization=False)
 return binary,manifest
def main():
 p=argparse.ArgumentParser();p.add_argument('--cache',type=Path,required=True);p.add_argument('--descriptor',type=Path,default=REPO/'port/android-native/app/src/main/assets/worlds/crypt01.dact');p.add_argument('--world-provenance',type=Path,default=REPO/'port/android-native/app/src/main/assets/worlds/crypt01-provenance.json');p.add_argument('--output',type=Path,required=True);p.add_argument('--manifest',type=Path,required=True);p.add_argument('--verify-only',action='store_true');a=p.parse_args()
 binary,manifest=produce(a.cache,a.descriptor,a.world_provenance)
 write(a.output,binary,a.verify_only);write(a.manifest,(json.dumps(manifest,indent=2)+'\n').encode(),a.verify_only)
 print(json.dumps(dict(validation='PASS',records=11,source_members=3,binary_bytes=len(binary),binary_sha256=sha(binary),descriptor_sha256=manifest['descriptor_sha256'],full_original_XML_factory_parity=False)))
if __name__=='__main__':main()
