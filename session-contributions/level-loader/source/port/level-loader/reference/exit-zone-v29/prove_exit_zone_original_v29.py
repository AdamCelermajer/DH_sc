from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path(r'C:\Users\adamc\Desktop\workspace\DH_sc');sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});obj=c.data+0x1000;heapstart=c.data+0x5000;heap=heapstart;mode='ctor';properties={};calls=[]
def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def text(p):
 b=bytearray()
 while c.uc.mem_read(p,1)!=b'\0':b+=c.uc.mem_read(p,1);p+=1
 return b.decode()
def ret(v=None):
 if v is not None:c.put(0,v)
 c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,address,size,user):
 global heap
 if address==0x310570:
  size=c.reg(0);assert c.reg(1)==0
  if mode=='ctor':assert size==2080;out=obj
  else:assert size in (36,44,56);out=heap;heap+=0x100;c.uc.mem_write(out,b'\xa5'*size)
  calls.append({'allocation':size});ret(out)
 elif address==0x38c398:assert c.reg(0)==obj and c.reg(1)==14;calls.append({'base_GO_ID':14});ret()
 elif address==0x31167c:
  target=c.reg(0);assert c.reg(1)==16;c.pointer(target+0x10,target);c.pointer(target+0x14,target);ret()
 elif address==0x3116e8:
  target,begin,end=[c.reg(i) for i in range(3)];assert begin==end;c.pointer(target+0x10,target);c.pointer(target+0x14,target);c.uc.mem_write(target,b'\0');ret()
 elif address==0x3139ac:ret()
 elif address==0x38cee8:assert c.reg(0)==obj;calls.append({'inherited_GameObject_properties':True});ret()
 elif address==0x3140ec:ret()
 elif address==0x39c0a8:
  calls.append({'parent_InitPost':True});ret()
 elif address==0x39c2c8:
  calls.append({'lookup':text(c.reg(0))});ret(lookup_result)
 elif address==0x513ce4:
  assert c.reg(0)==obj+4;name=text(c.reg(1));p=c.reg(2);offset=word(p+4)+4
  if offset==0x374:default=list(struct.unpack('<fff',c.uc.mem_read(p+0x20,12)))
  elif offset in (0x3a8,0x3ac,0x718,0x71c,0x720,0x7d8,0x7f4,0x7f8,0x7fc):default=struct.unpack('<i',c.uc.mem_read(p+0x20,4))[0]
  elif offset==0x3b0:default=c.uc.mem_read(p+0x20,1)[0]
  else:
   assert offset in (0x724,0x740,0x75c,0x778,0x794,0x7bc,0x7dc,0x800),(name,hex(offset));default=''
  properties[name]={'source_offset':offset,'default':default};ret()
c.uc.hook_add(UC_HOOK_CODE,hook);cases=[]
for poison in (0,0xa5,0x7f):
 c.uc.mem_write(obj,bytes([poison])*2080);calls.clear();c.invoke(0x340ea0,[]);assert c.reg(0)==obj
 fields={'data7d4':word(obj+0x7d4),'empty7dc':c.uc.mem_read(obj+0x7dc,1)[0]==0,'empty800':c.uc.mem_read(obj+0x800,1)[0]==0,'byte818':c.uc.mem_read(obj+0x818,1)[0],'byte819':c.uc.mem_read(obj+0x819,1)[0],'network100':word(obj+0x100)-obj,'network104':word(obj+0x104)-obj,'physical380':c.uc.mem_read(obj+0x380,1)[0],'trigger381':c.uc.mem_read(obj+0x381,1)[0],'static84':c.uc.mem_read(obj+0x84,1)[0]}
 assert fields=={'data7d4':0xffffffff,'empty7dc':True,'empty800':True,'byte818':0,'byte819':0,'network100':0x3c8,'network104':0x570,'physical380':0,'trigger381':1,'static84':1},fields
 unproduced={hex(offset):word(obj+offset)==int.from_bytes(bytes([poison])*4,'little') for offset in (0x7d8,0x7f4,0x7f8,0x7fc)};assert all(unproduced.values()),unproduced
 assert word(word(obj)+0x18)==0x39c8f8
 cases.append({'poison':poison,'fields':fields,'unproduced':unproduced,'calls':list(calls)})
mode='properties';heap=heapstart;properties.clear();c.invoke(0x39c8f8,[obj])
init_cases=[]
for name,lookup_result,expected in [('',17,99),('SWAMP_02',17,17),('MissingLevel',0xffffffff,0xffffffff)]:
 mode='init';calls.clear();value=name.encode()+b'\0';storage=c.data+0x9000;c.uc.mem_write(storage,value)
 c.pointer(obj+0x7ec,storage+len(name));c.pointer(obj+0x7f0,storage);c.pointer(obj+0x7d4,99)
 c.invoke(0x39c8c8,[obj]);assert word(obj+0x7d4)==expected
 assert calls==([{'parent_InitPost':True},{'lookup':name}] if name else [{'parent_InitPost':True}]),calls
 init_cases.append({'name':name,'result7d4':word(obj+0x7d4),'calls':list(calls)})
report={'validation':'PASS','original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'factory_cases':cases,'properties':properties,'init_post_tail_cases':init_cases,'scope':'Original whole Exit/TriggerZone/Trigger/ZoneEx/Zone factory constructors and DeclareProperties execute; inherited GameObject and allocator/string services explicit interceptions. Transition execution unported.'}
out=Path(r'C:\Users\adamc\.codex\visualizations\2026\10\04\01a108c2-0dd8-7842-bf2f-bc28dbbe94f5\exit-zone-original-v29.json');out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
