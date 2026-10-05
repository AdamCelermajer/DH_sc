"""Whole NativeChangeRolloverInputBehavior plus actual SetInputBehavior instructions.
AS numeric/boolean, imported EABI d2iz and singleton are controlled synchronous
services. Their algorithm is not reconstructed or claimed by this comparison.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_D0
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-animation/tests'))
from compiled_transforms_differential import Cpu,words
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Machine:
 def __init__(self,path,native):
  self.c=Cpu(path,native,{'functions':[]});self.native=native;c=self.c;d=c.data
  self.call=d+0x1000;self.ncall=d+0x1100;self.values=d+0x1200;self.services=d+0x1300;self.binding=d+0x1400;self.manager=d+0x2000;self.array=d+0x2400
  self.render=[d+0x3000+i*0x200 for i in range(4)];self.cb=[d+0x4000+i*16 for i in range(4)];self.tramp=d+0x4100
  for p in self.cb+[self.tramp]:c.uc.mem_write(p,words([0xd65f03c0]if native else[0xe12fff1e]))
  self.logs=[];c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,x=0):c=self.c;c.put(0,x);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def flags(self):return [struct.unpack('<I',self.c.uc.mem_read(p+(8 if self.native else 0xf8),4))[0]for p in self.render]
 def mutate(self,op):
  if self.mutation&(1<<(op-1)):
   i=(op+1)%4;p=self.render[i]+(8 if self.native else 0xf8);self.c.uc.mem_write(p,words([self.flags()[i]^0xab123456]))
  if op==3 and self.mutation&16:self.remap=True;self.write_binding()
 def write_binding(self):
  c=self.c
  for i in range(4):c.pointer((self.binding if self.native else self.array+0x134)+i*(8 if self.native else 4),self.render[(i+1)%4]if self.remap else self.render[i])
 def emit(self,op,value=0):
  if op==1:lo,hi=struct.unpack('<II',struct.pack('<d',self.number if not self.in_nested else 2.75));idx=0
  else:lo,hi=value&0xffffffff,0;idx=0
  self.logs.append((op,idx,lo,hi,*self.flags()));self.mutate(op)
 def hook(self,uc,a,z,u):
  c=self.c;n=self.native
  if a==self.tramp:
   self.in_nested=False;c.uc.reg_write(c.lr,self.saved_lr);self.ret();return
  if n:
   if a not in self.cb:return
   op=self.cb.index(a)+1;assert c.reg(0)==0xfedcba9876543210
   if op==1:
    token=int.from_bytes(uc.mem_read(c.reg(1),8),'little');assert token in(0xf123456789abcdef,0xf123456789abcdf1)
    uc.mem_write(c.reg(2),struct.pack('<d',2.75 if self.in_nested else self.number))
   elif op==2:
    v=struct.unpack('<d',struct.pack('<Q',uc.reg_read(UC_ARM64_REG_D0)))[0];assert v==(2.75 if self.in_nested else self.number);uc.mem_write(c.reg(1),words([int(v)]))
   elif op==3:uc.mem_write(c.reg(2),words([1 if self.in_nested else self.enabled]))
   else:c.pointer(c.reg(1),self.binding)
   self.emit(op,(2 if self.in_nested else int(self.number))if op==2 else (1 if self.in_nested else self.enabled)if op==3 else 0)
  else:
   if a==0x797a54:
    assert c.reg(0) in(self.values+12,self.values+60)
    self.emit(1);lo,hi=struct.unpack('<II',struct.pack('<d',2.75 if self.in_nested else self.number));c.put(0,lo);c.put(1,hi);uc.reg_write(c.pc,uc.reg_read(c.lr));return
   elif a==0x30ea24:
    v=struct.unpack('<d',words([c.reg(0),c.reg(1)]))[0];self.emit(2,int(v));self.ret(int(v));return
   elif a==0x797960:
    assert c.reg(0)in(self.values,self.values+48);self.emit(3,1 if self.in_nested else self.enabled);c.put(0,1 if self.in_nested else self.enabled)
   elif a==0x42ca8c:self.emit(4);self.ret(self.manager);return
   elif a==0x7a7c98 and not c.reg(0):self.invalid=True;uc.emu_stop();return
   else:return
  if op==3 if n else a==0x797960:
   if self.nested and not self.did_nested:
    self.did_nested=True;self.in_nested=True;self.saved_lr=uc.reg_read(c.lr);c.put(0,self.ncall)
    if n:c.put(1,self.services)
    uc.reg_write(c.lr,self.tramp);uc.reg_write(c.pc,c.symbols['dh2_menu_rollover_input_v1']if n else 0x43cdac);return
  self.ret(0 if n else c.reg(0))
 def run(self,number,enabled,mutation,nested,flags):
  c=self.c;n=self.native;self.number=number;self.enabled=enabled;self.mutation=mutation;self.nested=nested;self.did_nested=False;self.in_nested=False;self.invalid=False;self.remap=False;self.logs=[]
  for p,x in zip(self.render,flags):c.uc.mem_write(p,bytes(0x100));c.uc.mem_write(p+(8 if n else 0xf8),words([x]))
  self.write_binding()
  if n:
   c.uc.mem_write(self.values,struct.pack('<QIIQII',0xf123456789abcdef,0,0,0xf123456789abcdf0,0,0));c.uc.mem_write(self.values+48,struct.pack('<QIIQII',0xf123456789abcdf1,0,0,0xf123456789abcdf2,0,0))
   c.uc.mem_write(self.call,struct.pack('<QII',self.values,2,0));c.uc.mem_write(self.ncall,struct.pack('<QII',self.values+48,2,0));c.uc.mem_write(self.services,struct.pack('<5Q',0xfedcba9876543210,*self.cb))
   rc=c.invoke('dh2_menu_rollover_input_v1',[self.call,self.services]);rc=rc if rc<0x80000000 else rc-0x100000000
  else:
   for fn,values in((self.call,self.values),(self.ncall,self.values+48)):
    c.uc.mem_write(fn,bytes(32));c.pointer(fn+12,fn+32);c.pointer(fn+32,values);c.uc.mem_write(fn+16,words([2,1]));c.pointer(fn,self.values+0x100)
   sentinel=bytes.fromhex('fa030b001122334455667788');c.uc.mem_write(self.values+0x100,sentinel);c.pointer(self.manager+0xf4,self.array)
   c.invoke(0x43cdac,[self.call]);assert bytes(c.uc.mem_read(self.values+0x100,12))==sentinel;rc=-3 if self.invalid else 0
  return self.flags(),rc,self.logs
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);p.add_argument('--gold',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args();old=Machine(ROOT/'.local-inputs/libDungeonHunter2.so',False);new=Machine(a.library,True);rng=random.Random(43_172);records=[];calls=nested=invalid=0
 for i in range(1536):
  number=rng.choice((-2147483648.,-5.75,-1.,-.75,-0.,0.,.75,1.,1.9,2.,2.9,3.,3.9,4.,2147483647.));enabled=rng.choice((0,1,255));mutation=i%32;nest=i%31==0;flags=[rng.getrandbits(32)for _ in range(4)]
  expected=old.run(number,enabled,mutation,nest,flags);actual=new.run(number,enabled,mutation,nest,flags);assert expected==actual,(i,expected,actual);out,rc,logs=expected;calls+=len(logs);nested+=old.did_nested;invalid+=rc==-3
  records.append(struct.pack('<d7I4IiI',number,enabled,mutation,int(nest),*flags,*out,rc,len(logs))+b''.join(words(x)for x in logs))
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(words([0x31524d4d,len(records)])+b''.join(records))
 src=['port/engine-ui/menu_rollover_input_v1.hpp','port/engine-ui/menu_rollover_input_v1.cpp','port/engine-ui/tests/menu_rollover_input_v1_differential.py','port/engine-ui/menu_stack_v1.hpp','port/engine-ui/character_menu_queries_owner_v1.hpp']
 report=dict(validation='PASS',comparisons=len(records),ordered_services=calls,actual_nested_calls=nested,source_invalid_renderer_prefixes=invalid,mismatches=0,original_sha256=sha(ROOT/'.local-inputs/libDungeonHunter2.so'),arm64_library_sha256=sha(a.library),gold_sha256=sha(a.gold),source_sha256={s:sha(ROOT/s)for s in src},AS_and_EABI_conversion_algorithms_reconstructed=False,scope=__doc__)
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
