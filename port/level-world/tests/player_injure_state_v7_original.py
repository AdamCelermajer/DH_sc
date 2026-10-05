"""Original ARM SM_SetInjureState and optimized ARM64 V7 ordered differential.
Animation table, constant, stance and event dispatch are explicit fixtures;
source field writes, gate arithmetic and source instruction order execute.
"""
from pathlib import Path
import sys,struct,itertools,json,hashlib
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu,words,signed
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R7,UC_ARM_REG_R10
class Actor:
 def __init__(self,native):
  self.native=native;self.c=Cpu(ROOT/'.local-inputs/libinjure-v7-oracle.so'if native else ROOT/'.local-inputs/libDungeonHunter2.so',native,{'functions':[]})
  c=self.c;self.owner=c.data+0x1000;self.state=c.data+0x4000;self.borrow=c.data+0x5000;self.services=c.data+0x6000;self.tabledata=c.data+0x7000
  if native:
   c.uc.mem_write(self.borrow,struct.pack('<QQQ',self.owner,self.state,self.owner+0x14fc))
   c.uc.mem_write(self.services,struct.pack('<'+'Q'*12,self.owner,*[c.stop+0x100+n*16 for n in range(11)]))
  else:
   c.pointer(self.owner,self.owner+0x1800);c.pointer(self.owner+0x1800+0x28,0x3a49f0)
   c.pointer(self.state+4,self.owner)
   c.pointer(0x9a6440,2);c.pointer(0x9a6444,self.tabledata)
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def word(self,address,value):self.c.uc.mem_write(address,words(value))
 def hook(self,uc,at,size,user):
  c=self.c
  if self.native and c.stop+0x100<=at<c.stop+0x100+11*16:
   n=(at-c.stop-0x100)//16
   if n==0:self.trace.append(['player']);uc.mem_write(c.reg(2),b'\1');self.ret()
   elif n==1:self.trace.append(['table']);self.word(c.reg(2),self.table);self.ret()
   elif n==2:uc.mem_write(c.reg(2),bytes([self.table<2]));self.word(c.reg(3),42);self.ret()
   elif n==3:self.trace.append(['constant']);self.word(c.reg(3),self.stanced);self.ret()
   elif n==4:self.trace.append(['stance']);self.word(c.reg(2),self.stance);self.ret()
   elif n==5:self.trace.append(['event',c.reg(1),c.reg(2)]);self.ret()
   elif n==6:self.trace.append(['transition',c.reg(1),c.reg(2),c.reg(3)]);self.ret()
   elif n==7:self.trace.append(['debug',uc.mem_read(c.reg(1),100).split(b'\0')[0].decode()]);self.ret()
   elif n==8:self.trace.append(['animation',signed(c.reg(1))]);self.ret()
   elif n==9:self.trace.append(['cancel']);self.ret()
   elif n==10:self.trace.append(['look',self.look]);self.ret()
   else:raise AssertionError(n)
   return
  if not self.native:
   if getattr(self,'ticking',False) and at==0x3ac070:uc.reg_write(c.pc,c.stop);return
   if at==0x31f66c:self.ret(self.dt);return
   if at==0x3a49f0:self.trace.append(['player']);self.ret(1)
   elif at==0x3a3228:self.trace.append(['table']);self.ret(self.table)
   elif at==0x4c4bdc:self.trace.append(['constant']);self.ret(self.stanced)
   elif at==0x3a53e0:self.trace.append(['stance']);self.ret(self.stance)
   elif at==0x3c5684:self.trace.append(['event',c.reg(1),c.reg(2)]);self.ret()
   elif at==0x3c1938:self.trace.append(['transition',c.reg(1),c.reg(2),c.reg(3)]);self.ret()
   elif at==0x337888:self.ret()
   elif at==0x3140ec:self.tokens[c.reg(0)]=uc.mem_read(c.reg(1),100).split(b'\0')[0].decode();self.ret(c.reg(0))
   elif at==0x337a88:self.trace.append(['debug',self.tokens[c.reg(1)]]);self.ret()
   elif at==0x318254:self.ret()
   elif at==0x3c0b50:self.trace.append(['animation',signed(c.reg(1))]);self.ret()
   elif at==0x3bc6b8:self.trace.append(['cancel']);self.ret()
   elif at==0x4052bc:self.trace.append(['look',c.reg(1)]);self.ret()
 def run(self,gate,table,stanced,stance,direct):
  c=self.c;self.trace=[];self.table=table;self.stanced=stanced;self.stance=stance
  c.uc.mem_write(self.owner+0x14fc,struct.pack('<f',gate));offset=self.state+(48 if self.native else 0x28);self.word(offset,-99)
  if self.native:assert signed(c.invoke('dh2_injure_set_v7',[self.borrow,0x12345678,direct,self.services]))==1
  else:
   for n in range(2):self.word(self.tabledata+n*0xa0+0x3c,42)
   c.invoke(0x3c5d84,[self.state,0x12345678,direct])
  return bytes(c.uc.mem_read(self.owner+0x14fc,4)),signed(struct.unpack('<I',c.uc.mem_read(offset,4))[0]),self.trace
 def body(self,focus,flags,look):
  c=self.c;self.trace=[];self.tokens={};self.look=look
  offset=self.state+4 if self.native else self.owner+0x520;self.word(offset,flags);c.pointer(self.owner+0x408,look)
  if self.native:assert signed(c.invoke('dh2_injure_focus_v7'if focus else'dh2_injure_blur_v7',[self.borrow,self.services]))==1
  else:c.invoke(0x3c33e8 if focus else 0x3c4ba4,[0,11,self.owner,self.state,3,0])
  return bytes(c.uc.mem_read(offset,4)),self.trace
 def tick(self,gate,dt):
  c=self.c;self.ticking=True;self.dt=dt;c.uc.mem_write(self.owner+0x14fc,struct.pack('<f',gate))
  if self.native:assert signed(c.invoke('dh2_injure_tick_v7',[self.owner+0x14fc,dt]))==1
  else:
   c.uc.reg_write(UC_ARM_REG_R4,self.owner);c.uc.reg_write(UC_ARM_REG_R7,0x14fc)
   c.uc.reg_write(UC_ARM_REG_R5,c.data+0x9000);c.uc.reg_write(UC_ARM_REG_R10,0)
   c.pointer(c.data+0x9000,self.owner);c.invoke(0x3ac058,[])
  self.ticking=False;return bytes(c.uc.mem_read(self.owner+0x14fc,4))
def main():
 old,native=Actor(False),Actor(True);cases=0;calls=0
 for args in itertools.product((-1.,0.,1.,3000.,float('nan')),(-1,0,1,2),(0,0x1000),(-1,0,3),(0,1)):
  expected=old.run(*args);actual=native.run(*args)
  assert expected==actual,(args,expected,actual);cases+=1;calls+=len(expected[2])
 for args in itertools.product((0,1),(0,0xffffffff,0x2b41),(0,0x12345678)):
  expected=old.body(*args);actual=native.body(*args)
  assert expected==actual,(args,expected,actual);cases+=1;calls+=len(expected[1])
 for args in itertools.product((-1.,0.,0.5,1.,3000.,float('nan')),(0,1,16,3001,0xffffffff)):
  expected=old.tick(*args);actual=native.tick(*args)
  assert expected==actual,(args,expected,actual);cases+=1
 report=dict(validation='PASS',cases=cases,ordered_calls=calls,mismatches=0,scope=__doc__,original_sha256=hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),native_sha256=hashlib.sha256((ROOT/'.local-inputs/libinjure-v7-oracle.so').read_bytes()).hexdigest())
 (ROOT/'port/android-native/reports/player-injure-state-v7-original.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
