from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path(r'C:\Users\adamc\Desktop\workspace\DH_sc');sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
obj=c.data+0x1000;heapstart=c.data+0x4000;heap=heapstart;mode='ctor';calls=[];properties={}
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
  if mode=='ctor':assert size==1936;out=obj
  else:assert size in (36,44,56);out=heap;heap+=0x100;c.uc.mem_write(out,b'\xa5'*size)
  calls.append({'allocation':size});ret(out)
 elif address==0x38c398:
  assert c.reg(0)==obj and c.reg(1)==20;calls.append({'base_GO_ID':20});ret()
 elif address==0x31167c:
  target=c.reg(0);assert c.reg(1)==16;c.pointer(target+0x10,target);c.pointer(target+0x14,target);ret()
 elif address==0x3116e8:
  target,begin,end=[c.reg(i) for i in range(3)];assert begin==end
  c.pointer(target+0x10,target);c.pointer(target+0x14,target);c.uc.mem_write(target,b'\0');ret()
 elif address==0x3139ac:ret() # explicit temporary string storage fixture
 elif address==0x38cee8:
  assert c.reg(0)==obj;calls.append({'inherited_GameObject_properties':True});ret()
 elif address==0x3140ec:ret() # descriptor name storage; AddProperty receives original CString
 elif address==0x513ce4:
  assert c.reg(0)==obj+4;name=text(c.reg(1));p=c.reg(2);offset=word(p+4)+4
  if offset==0x374:default=list(struct.unpack('<fff',c.uc.mem_read(p+0x20,12)))
  elif offset in (0x3a8,0x3ac):default=struct.unpack('<i',c.uc.mem_read(p+0x20,4))[0]
  elif offset==0x3b0:default=c.uc.mem_read(p+0x20,1)[0]
  else:
   assert offset in (0x718,0x734,0x750,0x76c);default=''
  properties[name]={'source_offset':offset,'default':default};ret()
c.uc.hook_add(UC_HOOK_CODE,hook);cases=[]
for poison in (0,0xa5,0x7f):
 calls.clear();heap=heapstart;c.uc.mem_write(obj,bytes([poison])*1936);c.invoke(0x340f0c,[]);assert c.reg(0)==obj
 fields={'dimensions':list(struct.unpack('<fff',c.uc.mem_read(obj+0x374,12))),'physical380':c.uc.mem_read(obj+0x380,1)[0],'trigger381':c.uc.mem_read(obj+0x381,1)[0],'colzone384':word(obj+0x384),'static84':c.uc.mem_read(obj+0x84,1)[0],'updating85':c.uc.mem_read(obj+0x85,1)[0],'characters3a0':word(obj+0x3a0),'players3a4':word(obj+0x3a4),'count3a8':word(obj+0x3a8),'delay3ac':word(obj+0x3ac),'reset3b0':c.uc.mem_read(obj+0x3b0,1)[0],'activated3b4':word(obj+0x3b4),'timer3b8':word(obj+0x3b8),'local_only3bc':c.uc.mem_read(obj+0x3bc,1)[0],'touching3c0':word(obj+0x3c0),'network100':word(obj+0x100)-obj,'network104':word(obj+0x104)-obj,'checkpoint28':c.uc.mem_read(obj+0x28,1)[0],'networkf8':c.uc.mem_read(obj+0xf8,1)[0],'data_id730':word(obj+0x730),'script_id74c':word(obj+0x74c),'script_id768_unproduced':word(obj+0x768)==int.from_bytes(bytes([poison])*4,'little'),'byte784':c.uc.mem_read(obj+0x784,1)[0],'condition788':word(obj+0x788)}
 assert fields=={'dimensions':[0.0]*3,'physical380':0,'trigger381':1,'colzone384':0,'static84':0,'updating85':1,'characters3a0':0,'players3a4':0,'count3a8':1,'delay3ac':0,'reset3b0':0,'activated3b4':0,'timer3b8':0,'local_only3bc':0,'touching3c0':0,'network100':0x3c8,'network104':0x570,'checkpoint28':1,'networkf8':4,'data_id730':0xffffffff,'script_id74c':0xffffffff,'script_id768_unproduced':True,'byte784':0,'condition788':0},fields
 strings={hex(o):c.uc.mem_read(obj+o,1)[0]==0 for o in (0x718,0x734,0x750,0x76c)};assert all(strings.values())
 tree={'color388':c.uc.mem_read(obj+0x388,1)[0],'parent38c':word(obj+0x38c),'left390':word(obj+0x390)-obj,'right394':word(obj+0x394)-obj,'size398':word(obj+0x398)}
 assert tree=={'color388':0,'parent38c':0,'left390':0x388,'right394':0x388,'size398':0},tree
 nets=[]
 for offset in (0x3c8,0x570):
  n=obj+offset;count=word(n+0x104);declared=[word(n+4+i*4)-n for i in range(3)]
  assert count==3 and declared==[0x130,0x158,0x180],(count,declared)
  members=[{'type':word(n+m+4),'backing':word(n+m+0x20),'changed':c.uc.mem_read(n+m+0x1c,1)[0]} for m in declared]
  assert all(m['type']==32 and m['backing']==0 for m in members),members
  nets.append({'offset':offset,'count104':count,'declaration_offsets':declared,'members':members})
 cases.append({'poison':poison,'fields':fields,'empty_strings':strings,'contact_tree':tree,'network':nets,'calls':list(calls)})
virtuals={hex(i*4):hex(word(word(obj)+i*4)) for i in range(55)}
assert '0x38cd48' in virtuals.values(),'TriggerObject inherited InitFinal differs'
assert virtuals['0x18']=='0x399d70','TriggerObject property virtual differs'
mode='properties';properties.clear();calls.clear();heap=heapstart;c.invoke(0x399d70,[obj])
report={'validation':'PASS','original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'factory_cases':cases,'properties':properties,'virtuals':virtuals,'scope':'Original whole TriggerObject/Trigger/ZoneEx/Zone and both whole NetStructTrigger constructors execute. GameObject and allocator/string services are explicit interceptions. Activation, conditions, table/visual initialization and engine interaction remain required.'}
out=Path(r'C:\Users\adamc\.codex\visualizations\2026\10\04\01a108c2-0dd8-7842-bf2f-bc28dbbe94f5\trigger-object-original-v28.json');out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
