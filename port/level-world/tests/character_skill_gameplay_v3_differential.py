"""Actual five CharAISkillScript bodies versus optimized native choreography.

ReturnValues construction/erase/destruction, LuaScript calls, and Value.getBool
are declared logical services. This does not execute the original Lua engine.
It verifies two receiver reloads, all return cardinalities, callback statuses,
selected first/second Value and release order. Real same-owner VM proof follows.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu,words,signed
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
ADDRESSES=[0x3da8b8,0x3da794,0x3da6c0,0x3da9dc,0x3db16c]
ACTIVE_LOADS=[0x3da8ec,0x3da960,0x3da7c8,0x3da83c,0x3da6f4,0x3da760,0x3daa10,0x3daa84,0x3db1a0,0x3db214]
class Callback:
 def __init__(self,path,native):
  self.c=Cpu(path,native,{'functions':[]});self.native=native;c=self.c;d=c.data
  self.instance=d+0x1000;self.owner=d+0x2000;self.owner2=d+0x4000;self.out=d+0x6000;self.svc=d+0x6100;self.vec=d+0x7000;self.values=d+0x8000;self.name=d+0x9000
  self.c.uc.hook_add(UC_HOOK_CODE,self.hook)
  if native:c.uc.mem_write(self.svc,struct.pack('<QQ',self.owner,c.stop+0x100))
 def ret(self,value=0):self.c.put(0,value);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def populate(self,p,count,error):
  c=self.c;c.pointer(p+8,error);c.pointer(p+0x24,self.vec);c.uc.mem_write(self.vec,words(self.values,self.values+count*112,self.values+count*112))
 def hook(self,uc,at,size,user):
  c=self.c
  if self.native and at==c.stop+0x100:
   q=c.reg(1);r=c.reg(2);op,index=struct.unpack('<II',uc.mem_read(q,8));owner,active,instance,name,token=struct.unpack('<QQQQQ',uc.mem_read(q+8,40));self.service(op,index,owner,active,name,token,r);self.ret();return
  if self.native:return
  if at in ACTIVE_LOADS:
   self.trace.append([1,0,1 if c.reg(3)==self.owner else 2,0]);return
  if at==0x31b434:self.populate(c.reg(0),0,0);self.ret();return
  if at==0x37c390:
   self.trace.append([2,0,1,1]);self.populate(c.reg(3),self.set_count,self.set_error);c.pointer(self.instance+4,self.owner2);c.pointer(self.owner2+0x3e4,self.active2);self.ret();return
  if at==0x31c3cc:self.trace.append([3,0,2,0]);c.uc.mem_write(self.vec,words(self.values,self.values,self.values));self.ret();return
  if at==0x37c494:
   self.trace.append([4,0,2,2]);self.populate(c.reg(2),self.call_count,self.call_error);self.ret();return
  if at==0x31bc80:
   index=(c.reg(0)-self.values)//112;self.trace.append([5,index,2,0]);self.ret(self.booleans[index]);return
  if at==0x31b398:
   self.trace.append([6,0,2 if self.initial else 1,0]);self.ret();return
 def service(self,op,index,owner,active,name,token,r):
  owner_id=1 if owner==self.owner else 2;active_id=1 if active==0x1111 else 2 if active==0x2222 else 0
  self.trace.append([op,index,owner_id,active_id]);c=self.c
  values=[0,0,0,0,0,0]
  if op==1:values[0]=self.initial if owner_id==1 else self.active2
  if op==2:
   values[1]=self.values;values[2]=self.set_error;values[3]=self.set_count;c.pointer(self.instance,self.owner2)
  if op==4:values[1]=self.values;values[2]=self.call_error;values[3]=self.call_count
  if op==5:values[4]=self.booleans[index]
  c.uc.mem_write(r,struct.pack('<QQIIII',*values))
 def run(self,op,initial,set_error,set_count,call_error,call_count,boolean0,boolean1):
  self.initial=0x1111 if initial else 0;self.active2=0x2222;self.set_error=set_error;self.set_count=set_count;self.call_error=call_error;self.call_count=call_count;self.booleans=[boolean0,boolean1];self.trace=[];c=self.c
  c.uc.mem_write(self.name,b'skill\0');c.uc.mem_write(self.out,words(0x12345678))
  if self.native:
   c.uc.mem_write(self.instance,struct.pack('<QQIifI',self.owner,self.name,0,-1,0,0));r=c.invoke('dh2_character_skill_callback_v3',[self.out,self.instance,op,self.svc]);assert signed(r)==0;answer=struct.unpack('<I',c.uc.mem_read(self.out,4))[0]
  else:
   c.pointer(self.instance+4,self.owner);c.pointer(self.owner+0x3e4,self.initial);c.pointer(self.owner2+0x3e4,self.active2);r=c.invoke(ADDRESSES[op],[self.instance]);answer=r if op!=2 else 0
  return answer,self.trace
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();old=REPO/'.local-inputs/libDungeonHunter2.so';o,n=Callback(old,False),Callback(a.library,True);records=[]
 for op in range(5):
  for initial in range(2):
   for set_error in range(2):
    for set_count in [0,1,3]:
     for call_error in range(2):
      for call_count in [0,1,2,4]:
       for b in range(4):
        v=[op,initial,set_error,set_count,call_error,call_count,b&1,b>>1];e=o.run(*v);actual=n.run(*v);assert e==actual,(v,e,actual);records.append(dict(input=v,answer=e[0],trace=e[1]))
 ref=ROOT/'reference/character-skill-gameplay-v3';gold=ref/'callback-gold.json';gold.write_text(json.dumps(dict(validation='PASS',cases=records),indent=2)+'\n');raw=words(0x334c4b53,len(records))
 for r in records:raw+=words(*r['input'],r['answer'],len(r['trace']))+b''.join(words(*t)for t in r['trace'])
 (ref/'callback-gold.bin').write_bytes(raw)
 report=dict(validation='PASS',cases=len(records),ordered_services=sum(len(r['trace'])for r in records),mismatches=0,original_sha256=sha(old),optimized_arm64_sha256=sha(a.library),original_instructions_executed=True,optimized_arm64_instructions_executed=True,gold_sha256=sha(gold),source_sha256={p.relative_to(REPO).as_posix():sha(p)for p in [Path(__file__),ROOT/'character_skill_callbacks_v3.cpp',ROOT/'character_skill_callbacks_v3.hpp']},scope=__doc__)
 (ROOT/'reports/character-skill-gameplay-v3-callback-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
