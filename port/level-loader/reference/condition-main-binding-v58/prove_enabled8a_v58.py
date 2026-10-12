from pathlib import Path
import sys,struct,json,hashlib
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE,UC_HOOK_MEM_WRITE
base=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');ref=base/'reference/condition-main-binding-v58';elf=root/'.local-inputs/libDungeonHunter2.so'
assert hashlib.sha256(elf.read_bytes()).hexdigest()=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
class Oracle(Cpu):
 def text(self,p):
  out=bytearray()
  while self.uc.mem_read(p,1)!=b'\0':out+=self.uc.mem_read(p,1);p+=1
  return bytes(out)
 def external(self,uc,a,size,u):
  name=self.imports.get(a)
  if name in ['__aeabi_fcmplt','__aeabi_fmul']:
   left=struct.unpack('<f',struct.pack('<I',self.reg(0)))[0];right=struct.unpack('<f',struct.pack('<I',self.reg(1)))[0]
   value=int(left<right) if name=='__aeabi_fcmplt' else struct.unpack('<I',struct.pack('<f',left*right))[0]
   self.put(0,value);uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif name in ['strcmp','strstr']:
   x,y=self.reg(0),self.reg(1);left,right=self.text(x),self.text(y)
   if name=='strcmp':value=0 if left==right else -1 if left<right else 1
   else:
    at=left.find(right);value=x+at if at>=0 else 0
   self.put(0,value);uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,a,size,u)
c=Oracle(elf,False,{'functions':[]});actor=c.data+0x1000;heap=c.data+0x50000;allocs=[];writes=[];type_value=0;isplayer_reads=[]
def word(a,v):c.uc.mem_write(a,struct.pack('<I',v&0xffffffff))
def read(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def allocate(n):
 global heap
 p=heap;heap+=(n+15)&~15;c.uc.mem_write(p,bytes(n));return p
def hook(uc,a,size,u):
 if a==0x31167c:
  p=c.reg(0);n=c.reg(1);buf=allocate(n);word(p+0x10,buf);word(p+0x14,buf);allocs.append({'kind':'CString allocation leaf','receiver_offset':hex(p-actor),'bytes':n});ret()
 elif a==0x310570:
  n=c.reg(0);p=allocate(n);allocs.append({'kind':'operator new leaf','bytes':n});ret(p)
 elif a==0x3a3054:ret(type_value) # explicit type-domain provider
 elif a==0x3a4a14:isplayer_reads.append({'pc':hex(a),'source_offset':'0x44','name_begin':hex(read(actor+0x44)),'archetype_begin':hex(read(actor+0x5c))})
def memwrite(uc,access,a,size,v,u):
 if a-actor in [0x28,0x10c,0xec,0xa8,0xac,0xcc,0xd0,0x370,0x2d8]:writes.append({'pc':hex(c.uc.reg_read(c.pc)),'offset':hex(a-actor),'bytes':size,'value':hex(v)})
c.uc.hook_add(UC_HOOK_CODE,hook);c.uc.hook_add(UC_HOOK_MEM_WRITE,memwrite)

cases=[]
for ctor in [0x33f15c,0x33f310]:
 for poison in [0,0xa5,0x7f]:
  c.uc.mem_write(actor,bytes([poison])*0x1f90);c.invoke(ctor,[actor,0]);value=c.uc.mem_read(actor+0x8a,1)[0];assert value==1
  cases.append({'constructor':hex(ctor),'poison':poison,'enabled8a':value,'source_store':'33f254 C1 /33f408 C2 STRB1','disable_f1':c.uc.mem_read(actor+0xf1,1)[0]})
out={'schema':'dh2-enable8a-original-v58','status':'PASS','elf_sha256':hashlib.sha256(elf.read_bytes()).hexdigest(),'whole_ctor_cases':cases,'scope':'Original whole ObjectBase C1/C2 execute; only allocation leaves supplied as in frozen V48. Fresh original enabled8a=1, disablef1=0; no enable event/Eval executed.'}
(ref/'enabled8a-original-v58.json').write_text(json.dumps(out,indent=2)+'\n');print('PASS original wholeC1/C2 enabled8a1/disablef10 sixpoisons; no field alias assumption')
