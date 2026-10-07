"""Actual original ARM versus optimized native ARM64 cast semantic bodies.
Profiler instrumentation is excluded; actual flags/shared bytes and ordered
Character/Animator/cancel services are compared, with declared fixture stubs.
"""
from pathlib import Path
import sys,struct,json,random,hashlib
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu,words,signed
from unicorn import UC_HOOK_CODE
class Actor:
 def __init__(self,native):
  self.native=native;self.c=Cpu(ROOT/'.local-inputs/faery-cast-v2/libfaery-cast-v2-oracle.so' if native else ROOT/'.local-inputs/libDungeonHunter2.so',native,{'functions':[]});c=self.c;d=c.data
  self.owner=d+0x1000;self.state=d+0x4000;self.fields=d+0x5000;self.svc=d+0x6000
  if native:c.uc.mem_write(self.svc,struct.pack('<QQ',self.owner,c.stop+0x100))
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,v=0):self.c.put(0,v);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def semantic(self,op,value):
  self.trace.append([op,value]);c=self.c
  if op==1 and self.mutate:c.pointer(self.state+4 if self.native else self.owner+0x520,0xfeed5678)
  if op==4:c.uc.mem_write(self.owner+0x412,b'\0')
 def hook(self,uc,at,size,user):
  c=self.c
  if self.native:
   if at==c.stop+0x100:
    q=c.reg(2);op,value,char=struct.unpack('<IIQ',uc.mem_read(q,16));assert char==self.owner;self.semantic(op,value);self.ret()
   return
  if at in (0x337888,0x337a88,0x318254):self.ret();return
  if at==0x3140ec:self.ret(c.reg(0));return
  mapping={0x3a4d5c:1,0x3c0b50:2,0x3c93fc:3,0x3bc6b8:5,0x3c948c:6}
  if at in mapping:self.semantic(mapping[at],c.reg(1) if at!=0x3bc6b8 else 0);self.ret();return
  if at==0x3c3a70:self.semantic(4,0);return
  if at==0x3c932c:self.ret(self.step);return
  if at==0x3c934c:self.ret(5);return
 def body(self,op,flags,gate,heading,mutate):
  c=self.c;self.trace=[];self.mutate=mutate;c.uc.mem_write(self.owner+0x412,bytes([heading]))
  if self.native:
   c.uc.mem_write(self.state,words(-1,flags,gate,*([0]*11)));code=signed(c.invoke('dh2_faery_cast_body_v2',[self.state,self.owner,op,self.svc]));assert code==0
   final=struct.unpack('<I',c.uc.mem_read(self.state+4,4))[0]
  else:
   c.pointer(self.owner+0x520,flags);c.pointer(self.owner+0x528,gate)
   c.invoke([0x3c39d0,0x3c3934,0x3c0024,0x3c0020][op],[0,7,self.owner,self.owner+0x4fc,3,0]);final=struct.unpack('<I',c.uc.mem_read(self.owner+0x520,4))[0]
  return final,c.uc.mem_read(self.owner+0x412,1)[0],self.trace
 def animstep(self,start,step,mode,continued,last):
  c=self.c;self.trace=[];self.mutate=False;self.step=step
  if self.native:
   c.uc.mem_write(self.fields,struct.pack('<iBBH',-1,continued,last,0));assert signed(c.invoke('dh2_faery_cast_step_v2',[self.state,self.fields,self.owner,start,step,mode,self.svc]))==0;result=list(c.uc.mem_read(self.fields+4,2))
  else:
   c.pointer(self.owner+0x3c8+4,self.owner);c.pointer(self.owner+0x4c8,mode);c.uc.mem_write(self.owner+0x3c8+0xd0,bytes([continued,last]));c.invoke(0x3d3dd4 if start else 0x3d3d68,[self.owner+0x3c8]);result=list(c.uc.mem_read(self.owner+0x3c8+0xd0,2))
  return result,self.trace
 def ctrl(self,gate):
  c=self.c
  if self.native:c.pointer(self.state+8,gate);return signed(c.invoke('dh2_faery_ctrl_allowed_v2',[self.state]))
  c.pointer(self.owner+0x528,gate);return c.invoke(0x3ad430,[self.owner])
def main():
 old,native=Actor(False),Actor(True);rng=random.Random(202610055);cases=0;services=0
 for i in range(512):
  args=(i%4,rng.getrandbits(32),rng.getrandbits(32),rng.randrange(256),bool(i%2));expected=old.body(*args);actual=native.body(*args);assert actual==expected,(i,args,expected,actual);cases+=1;services+=len(expected[2])
 for start in (0,1):
  for step in (0,1,2,0xffffffff):
   for mode in (0,1,2,0xffffffff):
    for continued in (0,1,255):
     for last in (0,1,255):
      args=(start,step,mode,continued,last);expected=old.animstep(*args);actual=native.animstep(*args);assert expected==actual,(args,expected,actual);cases+=1
 for i in range(1024):
  mask=rng.getrandbits(32);assert old.ctrl(mask)==native.ctrl(mask);cases+=1
 out=dict(validation='PASS',scope=__doc__,cases=cases,ordered_body_services=services,mismatches=0,original_sha256=hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),native_sha256=hashlib.sha256((ROOT/'.local-inputs/faery-cast-v2/libfaery-cast-v2-oracle.so').read_bytes()).hexdigest())
 (ROOT/'port/android-native/reports/faery-cast-state-v2-arm64-differential.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
if __name__=='__main__':main()
