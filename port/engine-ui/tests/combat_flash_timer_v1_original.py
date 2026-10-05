"""Whole original FlashAnimManager::Update against optimized native owner.
The same twelve contexts are compared; level/dt/frame-count are explicit fixtures.
"""
from pathlib import Path
import sys,struct,itertools,json,hashlib
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu,words,signed
from unicorn import UC_HOOK_CODE
class Run:
 def __init__(self,native):
  self.native=native;self.c=Cpu(ROOT/'.local-inputs/libcombat-flash-timer-v1-oracle.so'if native else ROOT/'.local-inputs/libDungeonHunter2.so',native,{'functions':[]});c=self.c
  self.manager=c.data+0x1000;self.styles=c.data+0x3000;self.clip=c.data+0x4000;self.level=c.data+0x5000
  if not native:
   c.pointer(self.manager+0x3c4,self.styles);c.pointer(self.styles,self.clip);c.pointer(self.clip,self.clip+0x400);c.pointer(self.clip+0x400+0x13c,c.stop+0x200);c.uc.mem_write(self.level+0x130,words(2))
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,v):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def hook(self,uc,at,size,user):
  if self.native:return
  if at==0x31f594:self.ret(self.level)
  elif at==0x31f66c:self.ret(self.dt)
  elif at==self.c.stop+0x200:self.calls+=1;self.ret(self.frames)
 def run(self,frame,timer,flags,dt,frames):
  c=self.c;self.dt=dt;self.frames=frames;self.calls=0
  rows=[v for n in range(12)for v in (frame+n%3,timer-n,flags if n%2==0 else 0)]
  if self.native:
   c.uc.mem_write(self.manager,words(*rows));calls=signed(c.invoke('dh2_combat_flash_timer_v1',[self.manager,dt,33,frames]));out=list(struct.unpack('<36i',c.uc.mem_read(self.manager,144)))
  else:
   for n in range(12):c.uc.mem_write(self.manager+n*0x50+0xc,words(*rows[n*3:n*3+3],0,0))
   c.invoke(0x4138ac,[self.manager]);calls=self.calls;out=[]
   for n in range(12):out+=list(struct.unpack('<3i',c.uc.mem_read(self.manager+n*0x50+0xc,12)))
  return out,calls
def main():
 old,native=Run(False),Run(True);cases=0
 for args in itertools.product((0,3,9),(0,32,33,34,99),(0,1,3),(0,1,33,66,132),(1,4,12)):
  a=old.run(*args);b=native.run(*args);assert a==b,(args,a,b);cases+=1
 report=dict(validation='PASS',cases=cases,mismatches=0,scope=__doc__,original_sha256=hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),native_sha256=hashlib.sha256((ROOT/'.local-inputs/libcombat-flash-timer-v1-oracle.so').read_bytes()).hexdigest())
 (ROOT/'port/android-native/reports/combat-flash-timer-v1-original.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
