from pathlib import Path
import sys,struct,json,hashlib
r=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(r/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
class ConstructorCpu(Cpu):
 def external(self,uc,a,z,u):
  global heap
  if self.imports.get(a) in ('pthread_mutex_lock','pthread_mutex_unlock'):
   self.put(0,0);uc.reg_write(self.pc,uc.reg_read(self.lr)) # explicit single-thread synchronization primitive fixture
  elif self.imports.get(a) in ('_Znwj','_Znaj'):
   n=self.reg(0);p=heap;heap+=(n+4095)&~4095;self.uc.mem_write(p,b'\xa5'*n);self.put(0,p);uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif self.imports.get(a) in ('_ZdlPv','_ZdaPv'):self.put(0,0);uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif self.imports.get(a)=='__aeabi_uidiv':assert self.reg(1)!=0;self.put(0,self.reg(0)//self.reg(1));uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,a,z,u)
c=ConstructorCpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
heap=c.data+0x100000;allocations=[];primitive_allocations=[]
def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,a,z,u):
 global heap
 if a==0x310570:
  n=c.reg(0);assert n<=0x374 and c.reg(1)==0,(hex(n),c.reg(1),hex(c.uc.reg_read(c.lr)))
  p=heap;heap+=0x1000;c.uc.mem_write(p,b'\xa5'*n)
  if n==0x374:allocations.append(p)
  else:primitive_allocations.append({'size':n,'caller':hex(c.uc.reg_read(c.lr))})
  ret(p)
 elif a==0x31167c:assert c.reg(1)<=16,(c.reg(1),hex(c.uc.reg_read(c.lr)));ret() # sufficient inline16 capacity primitive fixture
c.uc.hook_add(UC_HOOK_CODE,hook)
snapshots=[]
for _ in range(3):
 p=c.invoke(0x3410a4,[]);assert p==allocations[-1] and word(p+0xf4)==20 and c.uc.mem_read(p+0x84,1)==b'\1'
 assert c.invoke(0x340134,[p])==c.invoke(0x34013c,[p])==c.invoke(0x340144,[p])==c.invoke(0x34014c,[p,0])==0
 vptr=word(p);slots={hex(offset):hex(word(vptr+offset)) for offset in [0x18,0x1c,0x58]}
 assert slots=={'0x18':'0x38cee8','0x1c':'0x38be5c','0x58':'0x38cd48'}
 snapshots.append({'identity':hex(p),'go_id':word(p+0xf4),'static84':c.uc.mem_read(p+0x84,1)[0],'inherited_virtuals':slots,'constructor_snapshot_sha256':hashlib.sha256(c.uc.mem_read(p,0x374)).hexdigest()})
p=r/'port/level-world/reference/swamp-families-v14';report={'status':'PASS','cases':snapshots,'original_sha256':hashlib.sha256((r/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'scope':'Whole Dummy factory3410a4 plus original GameObject/ObjectBase constructor graph execute on poisoned allocation, four derived fixed virtuals and inherited DeclareProperties/InitPost/InitFinal vptr slots. Heap and inline CString reserve are explicit primitive fixtures.'};(p/'dummy-constructor-original.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
