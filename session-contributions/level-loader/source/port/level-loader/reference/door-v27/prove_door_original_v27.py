from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path(r'C:\Users\adamc\Desktop\workspace\DH_sc');sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
obj=c.data+0x1000;heapstart=c.data+0x4000;heap=heapstart;calls=[];mode='ctor';properties={}
def ret(value=None):
 if value is not None:c.put(0,value)
 c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def text(p):
 b=bytearray()
 while c.uc.mem_read(p,1)!=b'\0':b+=c.uc.mem_read(p,1);p+=1
 return b.decode()
def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def hook(uc,address,size,user):
 global heap
 if address==0x310570:
  size=c.reg(0);assert c.reg(1)==0
  if mode=='ctor':assert size==1744;out=obj
  else:assert size in (56,36,44);out=heap;heap+=0x100;c.uc.mem_write(out,b'\xa5'*size)
  calls.append({'allocation':size});ret(out)
 elif address==0x38c398:
  assert c.reg(0)==obj and c.reg(1)==2;calls.append({'base_GO_ID':2});ret()
 elif address==0x31167c:
  target=c.reg(0);assert c.reg(1)==16
  c.pointer(target+0x10,target);c.pointer(target+0x14,target)
  calls.append({'string_allocate16':target-obj if target<obj+1744 else 'property-temporary'});ret()
 elif address==0x3116e8:
  target,begin,end=[c.reg(i) for i in range(3)];assert begin==end
  c.pointer(target+0x10,target);c.pointer(target+0x14,target);c.uc.mem_write(target,b'\0');ret()
 elif address==0x38cee8:
  assert c.reg(0)==obj;calls.append({'inherited_GameObject_declarations':True});ret()
 elif address==0x3140ec:
  calls.append({'property_name':text(c.reg(1))});ret()
 elif address==0x513ce4:
  assert c.reg(0)==obj+4;name=text(c.reg(1));p=c.reg(2);offset=word(p+4)+4
  default=list(struct.unpack('<fff',c.uc.mem_read(p+0x20,12))) if name=='dimensions' else '' if offset==0x388 else c.uc.mem_read(p+0x20,1)[0]
  properties[name]={'source_offset':offset,'default':default};ret()
c.uc.hook_add(UC_HOOK_CODE,hook);cases=[]
for poison in (0,0xa5,0x7f):
 heap=heapstart;calls.clear();c.uc.mem_write(obj,bytes([poison])*1744)
 c.invoke(0x340824,[]);assert c.reg(0)==obj
 fields={'dimensions':list(struct.unpack('<fff',c.uc.mem_read(obj+0x374,12))),'physical380':c.uc.mem_read(obj+0x380,1)[0],'trigger381':c.uc.mem_read(obj+0x381,1)[0],'static84':c.uc.mem_read(obj+0x84,1)[0],'colzone384':word(obj+0x384),'description_empty388':c.uc.mem_read(obj+0x388,1)[0]==0,'open3a4':c.uc.mem_read(obj+0x3a4,1)[0],'collision3a5':c.uc.mem_read(obj+0x3a5,1)[0],'state3a8':word(obj+0x3a8),'byte3ac':c.uc.mem_read(obj+0x3ac,1)[0],'net100':word(obj+0x100)-obj,'net104':word(obj+0x104)-obj,'checkpoint28':c.uc.mem_read(obj+0x28,1)[0],'networkf8':c.uc.mem_read(obj+0xf8,1)[0],'table_id3a0_unproduced':word(obj+0x3a0)==int.from_bytes(bytes([poison])*4,'little')}
 assert fields=={'dimensions':[0.0]*3,'physical380':0,'trigger381':1,'static84':1,'colzone384':0,'description_empty388':True,'open3a4':0,'collision3a5':1,'state3a8':0,'byte3ac':0,'net100':0x3b0,'net104':0x540,'checkpoint28':1,'networkf8':3,'table_id3a0_unproduced':True},fields
 nets=[]
 for offset in (0x3b0,0x540):
  n=obj+offset
  members=[]
  for rel in (0x130,0x150,0x170):
   members.append({'type':word(n+rel+4),'backing':c.uc.mem_read(n+rel+0x1d,1)[0],'changed':c.uc.mem_read(n+rel+0x1c,1)[0],'sequence':int.from_bytes(c.uc.mem_read(n+rel+8,8),'little'),'raw140':word(n+rel+0x10),'raw144':word(n+rel+0x14),'raw148':word(n+rel+0x18)})
  assert [m['type'] for m in members]==[1,1,1] and all(m['backing']==0 for m in members),members
  count=word(n+0x104);declared=[word(n+4+i*4)-n for i in range(3)]
  assert count==3 and declared==[0x130,0x150,0x170],(count,declared)
  nets.append({'offset':offset,'members':members,'count104':count,'declaration_offsets':declared})
 cases.append({'poison':poison,'fields':fields,'network':nets,'calls':list(calls)})
mode='properties';calls.clear();properties.clear();heap=heapstart;c.invoke(0x3e8990,[obj])
report={'validation':'PASS','original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'factory_cases':cases,'properties':properties,'property_calls':calls,'scope':'Actual Door factory/Zone and both whole NetStructDoor constructors execute including original NetStruct base, DeclareMember and SetChanged. GameObject base and string/allocator services explicit interceptions; lifecycle/behavior unported.'}
out=Path(__file__).with_name('door-original-v27.json');out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
