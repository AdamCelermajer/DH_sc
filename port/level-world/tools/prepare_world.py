"""Compile original authored level placements into a checked native descriptor.

Retain original XML and provenance alongside the deterministic descriptor.
This does not implement procedural rules or execute gameplay scripts.
"""
import argparse,hashlib,json,math,struct,zipfile
from pathlib import Path
import xml.etree.ElementTree as ET
from inventory import EXPECTED
def vector(text):
 values=[float(v) for v in text.split(',')];assert len(values)==3 and all(math.isfinite(v) for v in values);return values
def main():
 p=argparse.ArgumentParser();p.add_argument('cache',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 with a.cache.open('rb') as f:assert hashlib.file_digest(f,'sha256').hexdigest()==EXPECTED
 a.output.mkdir(parents=True,exist_ok=True);records=[];inputs={}
 with zipfile.ZipFile(a.cache) as z:
  def read(name):
   if name in inputs:return inputs[name]
   matches=[e for e in z.infolist() if Path(e.filename).name==name];assert len(matches)==1,(name,len(matches))
   entry=matches[0];raw=z.read(entry);inputs[name]=raw;records.append({'name':name,'entry':entry.filename,'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()});return raw
  layout=ET.fromstring(read('x07_crypt_backup.mlx'));rooms=[];spawn=None;objects=[]
  for node in layout.findall('GameObject'):
   if node.get('gametype')!='Module':continue
   nodeid=node.attrib['xrefobject']+'-node';position=vector(node.attrib['position']);rotation=vector(node.attrib['rotation']);scale=vector(node.attrib['scale'])
   # Initial authored Crypt layout has no rotation; do not guess Euler order.
   assert rotation==[0,0,0] and scale==[1,1,1]
   gp=Path(node.attrib['mgp'].replace('\\','/')).name;vp=Path(node.attrib['mvp'].replace('\\','/')).name
   game=ET.fromstring(read(gp));visual=ET.fromstring(read(vp));room_index=len(rooms)
   for child in game.findall('GameObject'):
    obj={'room':room_index,**child.attrib};objects.append(obj)
    if child.get('gametype')=='SpawnPoint' and child.get('entrypointID')=='0':
     assert spawn is None;local=vector(child.attrib['position']);spawn=[position[i]+local[i] for i in range(3)]
   for child in visual.findall('GameObject'):
    objects.append({'room':room_index,'visual':True,**child.attrib})
   rooms.append({'name':node.attrib['name'],'node':nodeid,'position':position,'gameplay':gp,'visual':vp})
  assert len(rooms)==8 and spawn is not None
  read('crypt.bdae');read('007_crypt_01.rule.xml')
  # Header DWLD,v1,room count,spawn XYZ; fixed 128-byte room records:
  # UTF-8 node ID[112] then translation XYZ and reserved u32.
  data=struct.pack('<4sII3f',b'DWLD',1,len(rooms),*spawn)
  for room in rooms:
   name=room['node'].encode();assert len(name)<112;data+=name.ljust(112,b'\0')+struct.pack('<3fI',*room['position'],0)
  (a.output/'crypt01.dwld').write_bytes(data)
  for name,raw in inputs.items():(a.output/name).write_bytes(raw)
  report={'cache_sha256':EXPECTED,'layout':'x07_crypt_backup.mlx','compiled_descriptor_sha256':hashlib.sha256(data).hexdigest(),'rooms':rooms,'spawn':spawn,'objects':objects,'inputs':records,'procedural_rules_executed':False}
  (a.output/'crypt01-provenance.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'rooms':len(rooms),'spawn':spawn,'gameplay_visual_objects':len(objects),'inputs':len(inputs)}))
if __name__=='__main__':main()
