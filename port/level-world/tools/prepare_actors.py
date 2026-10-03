"""Bundle authored decors and direct monster records, preserving skipped gates.

Character models resolve through the original CharacterTable/ModelDict.
Idle clips are explicit development selections until CharAnim reconstruction.
"""
import argparse,hashlib,json,struct,zipfile
from pathlib import Path
from inventory import EXPECTED
from prepare_world import vector

TABLES=('character_properties_pyarray.bin','character_properties_pyarraynames.bin','character_properties_pystructnames.bin','character_models_dictionary_pyarray.bin','character_models_dictionary_pyarraynames.bin')
IDLE={'skeleton.bdae':'skeleton_idle_01.bdae','slime_green_v2.bdae':'slime_idle.bdae','slime_red.bdae':'slime_idle.bdae','ghost.bdae':'ghost_idle_01.bdae','ghostdh1.bdae':'ghost_idle_01.bdae','ghostdh1_dark.bdae':'ghost_idle_01.bdae'}
def strings(raw):
 count=struct.unpack_from('<I',raw)[0];offset=4;out=[];assert count<=10000
 for _ in range(count):
  size=struct.unpack_from('<I',raw,offset)[0];offset+=4;assert 0<size<=4096 and offset+size<=len(raw);out.append(raw[offset:offset+size].decode('ascii'));offset+=size
 return out,offset
def images(raw):
 def word(offset):return struct.unpack_from('<I',raw,offset)[0]
 root=word(32);count=word(root+0x4c);items=word(root+0x50);assert count<4096;out=set()
 for i in range(count):
  offset=word(items+i*20+8);assert 0<offset<len(raw);end=raw.index(b'\0',offset,offset+4096);out.add(Path(raw[offset:end].decode().replace('\\','/')).name)
 return out
def fixed(text,width):
 raw=text.encode('ascii');assert 0<len(raw)<width;return raw.ljust(width,b'\0')
def main():
 p=argparse.ArgumentParser();p.add_argument('cache',type=Path);p.add_argument('--project',type=Path,default=Path(__file__).resolve().parents[2]/'android-native');a=p.parse_args()
 with a.cache.open('rb') as f:assert hashlib.file_digest(f,'sha256').hexdigest()==EXPECTED
 assets=a.project/'app/src/main/assets';layout=json.loads((assets/'worlds/crypt01-provenance.json').read_text());assert layout['cache_sha256']==EXPECTED
 records=[];inputs=[];payloads={};aliases=[];skipped=[]
 with zipfile.ZipFile(a.cache) as archive:
  by_name={}
  for entry in archive.infolist():by_name.setdefault(Path(entry.filename).name.lower(),[]).append(entry)
  def read(name,folder):
   destination=folder+'/'+(name.lower() if folder=='textures' else name)
   if destination in payloads:return payloads[destination]
   found=by_name.get(name.lower(),[])
   if not found and folder=='textures':
    found=by_name.get(('pvr2_'+name).lower(),[]);assert len(found)==1,('Missing texture',name)
   if len(found)==1 and Path(found[0].filename).name!=name:aliases.append({'requested':name,'original_entry_name':Path(found[0].filename).name})
   assert len(found)==1,('Nonunique or missing input',name,len(found));raw=archive.read(found[0]);payloads[destination]=raw
   inputs.append({'asset':destination,'entry':found[0].filename,'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()});return raw
  for name in TABLES:read(name,'data')
  names,_=strings(payloads['data/'+TABLES[1]]);fields,_=strings(payloads['data/'+TABLES[2]]);models,_=strings(payloads['data/'+TABLES[3]]);assert len(fields)==224
  raw=payloads['data/'+TABLES[0]];assert struct.unpack_from('<I',raw)[0]==len(names)
  for obj in layout['objects']:
   kind=obj.get('gametype');room=layout['rooms'][obj['room']];clip='';template=''
   if kind=='AnimatedDecor':model=Path(obj['dae'].replace('\\','/')).name;tag=2
   elif kind=='Character' and obj.get('_templateName')=='Monster' and obj.get('charpropsname') and not obj.get('activate_cond') and obj.get('auto_spawn','1')!='0':
    template=obj['charpropsname'];index=names.index(template);values=struct.unpack_from('<224i',raw,4+index*896);properties=dict(zip(fields,values));model_id=properties['ModelFile'];assert 0<=model_id<len(models)
    model=Path(models[model_id]).name;clip=IDLE[model];tag=1
   else:skipped.append({'name':obj['name'],'room':obj['room'],'type':kind,'reason':'scripts, templates, conditional actors or other game object factory pending'});continue
   position=vector(obj['position']);offset=room['position'];position=[position[i]+offset[i] for i in range(3)];rotation=vector(obj['rotation']);scale=vector(obj['scale'])
   if tag==1:scale=[scale[i]*properties['Scale_'+axis]/100 for i,axis in enumerate('XYZ')]
   assert all(0<v<=100 for v in scale)
   records.append({'kind':tag,'room':obj['room'],'name':obj['name'],'character':template,'model':model,'clip':clip,'position':position,'rotation_degrees':rotation,'scale':scale,'animation_table':properties['AnimTable'] if tag==1 else None})
   model_raw=read(model,'actors');textures=images(model_raw)
   if clip:read(clip,'actors')
   for name in sorted(textures):read(name,'textures')
  # DACT,v1,count,reserved. Fixed 256-byte record: kind,room,name[64],
  # character[64],model[64],position/rotation/scale (9 floats),20 reserved bytes.
  data=struct.pack('<4sIII',b'DACT',1,len(records),0)
  for row in records:
   template=row['character'].encode().ljust(64,b'\0');assert len(template)==64
   data+=struct.pack('<II',row['kind'],row['room'])+fixed(row['name'],64)+template+fixed(row['model'],64)+struct.pack('<9f',*row['position'],*row['rotation_degrees'],*row['scale'])+bytes(20)
  assert len(data)==16+len(records)*256
  for name,raw in payloads.items():destination=assets/name;destination.parent.mkdir(parents=True,exist_ok=True);destination.write_bytes(raw)
  (assets/'worlds/crypt01.dact').write_bytes(data)
  report={'cache_sha256':EXPECTED,'descriptor_sha256':hashlib.sha256(data).hexdigest(),'character_table_rows':len(names),'character_fields':len(fields),'model_dictionary_rows':len(models),'records':records,'skipped':skipped,'inputs':inputs,'texture_aliases':aliases,'idle_selection':'explicit development clips; original CharAnim state mapping pending','script_conditions_executed':False}
  (assets/'actor-provenance.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'objects':len(records),'monsters':sum(r['kind']==1 for r in records),'decors':sum(r['kind']==2 for r in records),'skipped':len(skipped),'inputs':len(inputs),'texture_aliases':aliases}))
if __name__=='__main__':main()
