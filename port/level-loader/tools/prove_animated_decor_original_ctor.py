from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path(r'C:\Users\adamc\Desktop\workspace\DH_sc');sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});obj=c.data+0x1000;storage=c.data+0x3000;calls=[]
def returned():c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def string(address):
 out=bytearray()
 while c.uc.mem_read(address,1)!=b'\0':out+=c.uc.mem_read(address,1);address+=1
 return out.decode()
def hook(uc,address,size,user):
 if address==0x310570:
  assert c.reg(0)==916 and c.reg(1)==0;calls.append({'allocation_bytes':c.reg(0)});c.uc.reg_write(UC_ARM_REG_R0,obj);returned()
 elif address==0x38c398:
  assert c.reg(0)==obj and c.reg(1)==20;calls.append({'actual_GameObject_C1_id':20});returned()
 elif address==0x31167c:
  assert c.reg(0)==obj+0x37c and c.reg(1)==16
  calls.append({'string_allocation_offset':'0x37c','bytes':16})
  c.uc.mem_write(obj+0x38c,struct.pack('<II',storage,storage));c.uc.mem_write(storage,b'\xcc'*16);returned()
 elif address==0x3899d0:
  assert c.reg(0)==obj;calls.append({'qualified_Decor_DeclareProperties':True});returned()
 elif address==0x33ef7c:
  assert c.reg(0)==obj+4 and c.reg(2)==obj+0x37c;calls.append({'additional_string_property':string(c.reg(1)),'offset':'0x37c'});returned()
c.uc.hook_add(UC_HOOK_CODE,hook);reports=[]
for poison in [0,0xa5,0x7f]:
 calls.clear();c.uc.mem_write(obj,bytes([poison])*0x400);c.invoke(0x342600,[])
 assert c.reg(0)==obj
 values={hex(offset):c.uc.mem_read(obj+offset,1)[0] for offset in [0x84,0x375,0x376,0x377,0x378]}
 assert values['0x84']==1 and values['0x375']==0 and values['0x376']==1
 assert values['0x377']==values['0x378']==poison
 assert c.uc.mem_read(storage,1)==b'\0'
 reports.append({'poison':poison,'derived_bytes':values,'animation_empty':True,'calls':list(calls)})
calls.clear();c.invoke(0x389dd4,[obj]);assert calls[-1]['additional_string_property']=='startanim'
result={'validation':'PASS','original_elf_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'original_factory_cases':reports,'property_declaration':list(calls),'scope':'Whole actual342600 factory instruction flow with explicit allocation/GameObject C1/string allocation interception; exact derived stores and string37c/property declaration; no InitPost/animation claim'}
(Path(__file__).parent/'animated-decor-original-ctor-proof.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
