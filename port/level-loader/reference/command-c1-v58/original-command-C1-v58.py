from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages');sys.path.insert(0,r'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
from items_differential import Original as Base
from unicorn import UC_HOOK_CODE,UC_HOOK_MEM_WRITE
class Original(Base):
 def storage(self,*args):pass
 def external(self,uc,at,size,unused):
  name=self.imports.get(at)
  if name in ('malloc','_Znwj'):
   n=self.reg(0);p=self.heap;self.heap+=(max(n,4)+15)&~15;uc.mem_write(p,b'\xa5'*max(n,4));allocations[p]=n;self.returned(p)
  elif name in ('free','_ZdlPv','pthread_mutex_lock','pthread_mutex_unlock'):self.returned(0)
  else:super().external(uc,at,size,unused)
d=Path(__file__).parent;prior=d.parent/'scriptmanager-v52';elf=Path(r'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');c=Original(elf,{'functions':[]});allocations={};writes=[];active=False
def hook(uc,at,size,unused):
 if at in (0x310570,0x31056c):
  n=c.reg(0);assert n<1000000;n=max(n,4);p=c.heap;c.heap+=(n+15)&~15;uc.mem_write(p,b'\xa5'*n);allocations[p]=n;c.returned(p)
 elif at==0x310440:c.returned()
def memory(uc,access,at,size,value,unused):
 if active:writes.append((at,size,value,c.uc.reg_read(c.pc)))
c.uc.hook_add(UC_HOOK_CODE,hook);c.uc.hook_add(UC_HOOK_MEM_WRITE,memory)
matrix=json.loads((prior/'command-main-services-v52.json').read_text());rows=[]
for key,entry in sorted(matrix.items(),key=lambda v:int(v[0])):
 allocations.clear();writes.clear();active=True;p=c.invoke(int(entry['command_factory'],16),[0,0,0,0],budget=100000);active=False;assert p in allocations;(size,before)=(allocations[p],bytes(c.uc.mem_read(p,allocations[p])));defined=[False]*size;stores=[]
 for at,n,v,pc in writes:
  if p<=at and at+n<=p+size:
   for i in range(at-p,at-p+n):defined[i]=True
   stores.append({'offset':at-p,'width':n,'value':v,'source':hex(pc)})
 vptr=c.word(p);init=c.word(vptr);assert init==int(entry['command_Init'],16);assert before[4]==0 and c.word(p+12)==0
 rows.append({'kind':int(key),'class':entry['class'],'factory':entry['command_factory'],'original_size':size,'constructed_hex':before.hex(),'defined_mask':''.join('1' if v else '0' for v in defined),'stores':stores,'Init':hex(init),'C1_only_no_Init':True,'allocator_delivery_fixture':True})
report={'validation':'PASS','engine_sha256':hashlib.sha256(elf.read_bytes()).hexdigest(),'source_C1_classes':len(rows),'allocation_service_fixture':True,'actual_class_Init_Execute_verified':False,'rows':rows};(d/'original-command-C1-v58.json').write_bytes((json.dumps(report,indent=2)+'\n').encode());print(json.dumps({'validation':'PASS','classes':len(rows),'sizes':sorted(set(x['original_size'] for x in rows))}))
