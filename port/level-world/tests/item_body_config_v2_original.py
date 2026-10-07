"""Execute original PhysicalObject C2 selected by POItem against new ARM64 definitions.
Debug/string and Box2D allocation/mass endpoints are declared observed fixtures;
all original circle/body/filter/math instructions execute. This is not a full
ItemObject InitAgain or live render proof.
"""
import sys,json,struct,random,hashlib
from pathlib import Path
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from unicorn import UC_HOOK_CODE
from character_body_config_differential import Cpu,config_equal,ROOT
old=Cpu(Path('.local-inputs/libDungeonHunter2.so'),False,json.loads((ROOT/'reference/character-body-config/original-functions.json').read_text()))
new=Cpu(Path('.local-inputs/item-body-oracle-v2.so'),True,{'functions':[]})
owner=old.data+0x1000;physical=old.data+0x4000;body=old.data+0x6000;shape=old.data+0x7000;world=old.data+0xa000
identity=0x200000009abcdef0;out=new.data+0x1000;bounds_at=new.data+0x2000;pos_at=new.data+0x3000
definition=None;shape_definition=None;override=0;events=[]
def returned(v=0):old.put(0,v);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
def hook(uc,address,size,unused):
 global definition,shape_definition
 if address in (0x337888,0x3140ec,0x3139ac,0x31167c):returned()
 elif address==0x337a88:returned(override)
 elif address==0x34bcf0:
  raw=bytes(uc.mem_read(old.reg(1),44));assert struct.unpack_from('<I',raw,16)[0]==physical
  definition=struct.pack('<Q',identity)+raw[:16]+raw[20:40]+struct.pack('<5I',*raw[40:44],0)
  old.uc.mem_write(body,bytes(0x100));events.append(2);returned(body)
 elif address==0x7e1dc8:
  raw=bytes(uc.mem_read(old.reg(1),100));kind=struct.unpack_from('<I',raw,4)[0];assert kind==0
  group=struct.unpack_from('<h',raw,30)[0];category,mask=struct.unpack_from('<2H',raw,26)
  shape_definition=struct.pack('<QII',identity,kind,raw[24])+raw[12:24]+raw[32:44]+bytes(32)+struct.pack('<IiII',0,group,category,mask)
  old.uc.mem_write(shape,bytes(0x80));events.append(3);returned(shape)
 elif address==0x7e1818:events.append(4);returned()
old.uc.hook_add(UC_HOOK_CODE,hook)
rng=random.Random(20261005);comparisons=0
for i in range(514):
 bounds=[rng.uniform(-3000,3000) for _ in range(4)] if i>=2 else [-100,-200,100,200]
 position=[rng.uniform(-3000,3000) for _ in range(2)] if i>=2 else [200,300]
 override=i%2;definition=None;shape_definition=None;events.clear()
 old.uc.mem_write(owner,bytes(0x1800));old.uc.mem_write(physical,bytes(40))
 packed=struct.pack('<4f',*bounds);pose=struct.pack('<2f',*position)
 old.uc.mem_write(owner+0x12c,packed[:8]);old.uc.mem_write(owner+0x138,packed[8:]);old.uc.mem_write(owner+0x160,pose)
 old.invoke(0x46f2f0,[physical,world,owner,0,1,1,0,0xfffffffd,0x40,4,0])
 assert definition is not None and shape_definition is not None and events==[2,3,4]
 radius=bytes(old.uc.mem_read(physical+12,4))
 expected=definition+shape_definition+radius+struct.pack('<4I',1,0,0,4)+struct.pack('<8I',1,2,3,4,0,0,0,0)+bytes(4)
 new.uc.mem_write(bounds_at,packed);new.uc.mem_write(pos_at,pose)
 assert new.invoke('dh2_item_body_oracle_v2',[out,identity,bounds_at,pos_at,override])==1
 actual=bytes(new.uc.mem_read(out,208));assert config_equal(expected,actual),(i,list(struct.unpack('<52I',expected)),list(struct.unpack('<52I',actual)))
 comparisons+=1
report={'status':'PASS','comparisons':comparisons,'original_sha256':hashlib.sha256(Path('.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'library_sha256':hashlib.sha256(Path('.local-inputs/item-body-oracle-v2.so').read_bytes()).hexdigest(),'scope':__doc__,'whole_original_physical_C2_init_addshape_executed':True,'full_ItemInitAgain':False,'original_import_calls':old.import_calls}
p=ROOT/'reports/item-body-config-v2-original.json';p.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
