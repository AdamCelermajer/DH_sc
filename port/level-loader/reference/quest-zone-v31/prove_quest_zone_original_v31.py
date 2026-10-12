from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path(r'C:\Users\adamc\Desktop\workspace\DH_sc');sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});obj=c.data+0x1000;heap=c.data+0x5000;mode='ctor';properties={};calls=[]
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
  if mode=='ctor':assert size==904;out=obj
  else:assert size==44;out=heap;heap+=0x100;c.uc.mem_write(out,b'\xa5'*size)
  calls.append({'allocation':size});ret(out)
 elif address==0x38c398:assert c.reg(0)==obj and c.reg(1)==20;calls.append({'base_GO_ID':20});ret()
 elif address==0x38cee8:assert c.reg(0)==obj;calls.append({'inherited_GameObject_properties':True});ret()
 elif address==0x31167c:
  target=c.reg(0);assert c.reg(1)==16;c.pointer(target+0x10,target);c.pointer(target+0x14,target);ret()
 elif address==0x3116e8:
  target,begin,end=[c.reg(i) for i in range(3)];assert begin==end;c.pointer(target+0x10,target);c.pointer(target+0x14,target);c.uc.mem_write(target,b'\0');ret()
 elif address in (0x3139ac,0x3140ec):ret()
 elif address==0x513ce4:
  assert c.reg(0)==obj+4;name=text(c.reg(1));p=c.reg(2);offset=word(p+4)+4
  assert name=='dimensions' and offset==0x374
  default=list(struct.unpack('<fff',c.uc.mem_read(p+0x20,12)));properties[name]={'source_offset':offset,'default':default};ret()
c.uc.hook_add(UC_HOOK_CODE,hook);cases=[]
for poison in (0,0xa5,0x7f):
 c.uc.mem_write(obj,bytes([poison])*904);calls.clear();c.invoke(0x340f50,[]);assert c.reg(0)==obj
 fields={'dimensions374':list(struct.unpack('<fff',c.uc.mem_read(obj+0x374,12))),'physical380':c.uc.mem_read(obj+0x380,1)[0],'trigger381':c.uc.mem_read(obj+0x381,1)[0],'colzone384':word(obj+0x384),'static84':c.uc.mem_read(obj+0x84,1)[0]}
 assert fields=={'dimensions374':[0,0,0],'physical380':1,'trigger381':0,'colzone384':0,'static84':1},fields
 declare=word(word(obj)+0x18);assert declare==0x397df8,hex(declare)
 cases.append({'poison':poison,'fields':fields,'declare_properties':hex(declare),'calls':list(calls)})
mode='properties';calls.clear();c.invoke(0x397df8,[obj]);assert properties=={'dimensions':{'source_offset':0x374,'default':[200,200,200]}}
report={'validation':'PASS','original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'factory_cases':cases,'properties':properties,'scope':'Whole original QuestMoveInZone and Zone constructors and Zone DeclareProperties; inherited GameObject ctor/properties and allocator intercepted; quest collision execution remains a main-session service.'}
out=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc\port\level-loader\reference\quest-zone-v31');out.mkdir(exist_ok=True)
(out/'quest-zone-original-v31.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
