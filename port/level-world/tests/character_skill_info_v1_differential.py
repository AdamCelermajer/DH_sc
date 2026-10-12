"""Original complete AI_SkillInfo/GetInfo versus O2 ARM64 coordinator.
Lua Call/result vectors, active-owner projections and timer lookup are explicit
services; actual source branch/arithmetic/reload order executes unchanged.
"""
import argparse,hashlib,json,math,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu,words,bits,floating,signed
def block(v):return words(len(v))+v
class SkillCpu(Cpu):
 def external(self,uc,address,size,user):
  if self.arm64 and address==self.callback+32:self.native_callback(uc,address,size,user);return
  return super().external(uc,address,size,user)
class Machine:
 def __init__(self,path,native):
  self.c=SkillCpu(path,native,{'functions':[]});self.native=native;c=self.c;d=c.data;c.native_callback=self.hook
  self.state=d+0x1000;self.vector=d+0x2000;self.instance=d+0x3000;self.owners=[d+0x4000,d+0x6000];self.active=[d+0x8000,d+0x9000];self.fraction=d+0xa000;self.service=d+0xb000;self.result=d+0xc000;self.results=d+0xd000;self.value=d+0xe000;self.text=d+0xf000
  self.trace=[]
  if not native:c.uc.hook_add(UC_HOOK_CODE,self.hook)
  if not native:c.pointer(0x99f698,1)
 def ret(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def owner_id(self,p):return self.owners.index(p)+1 if p in self.owners else 0
 def active_id(self,p):return self.active.index(p)+1 if p in self.active else 0
 def read(self,p):return struct.unpack('<I',self.c.uc.mem_read(p,4))[0]
 def event(self,op,owner,active=0,value=0):self.trace.append(words(op,owner,active,value))
 def configure(self,v):
  self.v=v;self.trace=[];c=self.c;c.uc.mem_write(self.state,bytes(0x200));c.uc.mem_write(self.instance,bytes(0x80));c.uc.mem_write(self.vector,bytes(16));c.uc.mem_write(self.text,b'skill\0');c.uc.mem_write(self.fraction,words(v[12]));
  for i,o in enumerate(self.owners):c.pointer(o+0x3e4,self.active[i] if v[i+2] else 0)
  if self.native:
   c.uc.mem_write(self.state,struct.pack('<QQIIQQII',self.owners[0],self.vector,2,0,0,0,0,0));c.pointer(self.vector,self.instance if v[0] else 0);c.pointer(self.vector+8,0)
   c.uc.mem_write(self.instance,struct.pack('<QQIifI',self.owners[0],self.text,0,17,0.,0));c.uc.mem_write(self.service,struct.pack('<QQ',0,c.callback+32))
  else:
   c.pointer(self.state+0xb4,self.vector);c.pointer(self.state+0xb8,self.vector+8);c.pointer(self.vector,self.instance if v[0] else 0);c.pointer(self.vector+4,0);c.pointer(self.instance+4,self.owners[0])
  c.uc.mem_write(self.results,words(self.value,self.value,self.value+112*64))
 def mutate(self,phase):
  c=self.c;offset=0 if self.native else 4
  if self.v[13]&(1 if phase==2 else 2):c.pointer(self.instance+offset,self.owners[1])
 def hook(self,uc,address,size,user):
  c=self.c;r=c.reg;v=self.v
  if self.native:
   if address!=c.callback+32:return
   op,level,owner,active,instance,name,timer,res=struct.unpack('<IIQQQQiI',uc.mem_read(r(1),48));assert not res;oid=self.owner_id(owner);aid=self.active_id(active);value=level if op==3 else timer if op==4 else 0;self.event(op,oid,aid,value)
   out=[0]*10
   if op==1:out[0]=self.active[oid-1] if v[oid+1] else 0
   elif op==2:self.mutate(op);out[2]=v[4];out[3]=v[5]
   elif op==3:self.mutate(op);out[2]=v[6];out[3]=v[7];out[4]=v[8];out[5]=v[9]
   elif op==4:out[6]=v[10];out[7]=v[11];out[8]=v[14]
   else:raise AssertionError(op)
   # active is64-bit then source_error/count/type/number/found/elapsed/duration/res.
   uc.mem_write(r(2),struct.pack('<Q8I',out[0],*out[2:]));self.ret();return
  if address in (0x3dacf0,0x3dad6c):
   owner=self.read(self.instance+4);self.event(1,self.owner_id(owner))
  elif address==0x3192b4:uc.mem_write(r(0),bytes(16));self.ret(r(0))
  elif address==0x31b434:uc.mem_write(r(0),bytes(40));c.pointer(r(0)+36,self.results);self.ret(r(0))
  elif address==0x37c390:
   name=c.string(r(1));op=2 if name==b'SetSkill' else 3;assert name in(b'SetSkill',b'OnSkillInfo');oid=self.owner_id(self.read(self.instance+4));self.event(op,oid,self.active_id(r(0)),v[1] if op==3 else 0);self.mutate(op);err,count=(v[4],v[5]) if op==2 else (v[6],v[7]);uc.mem_write(r(3)+8,words(err));c.pointer(self.results,self.value);c.pointer(self.results+4,self.value+count*112);uc.mem_write(self.value+4,words(v[8],v[9]));self.ret()
  elif address==0x3cdd78:self.ret(r(0))
  elif address==0x31c3cc:c.pointer(self.results+4,self.value);self.ret()
  elif address in(0x31b398,0x319228):self.ret()
  elif address==0x3db344:
   # owner timer manager address comes from freshly reread instance+4.
   owner=r(0)-0x3b4;self.event(4,self.owner_id(owner),0,signed(r(1)));uc.mem_write(r(2),words(v[11]));uc.mem_write(r(3),words(v[14]));self.ret(v[10])
 def run(self):
  c=self.c;v=self.v;has_fraction=v[15];before=bytes(c.uc.mem_read(self.fraction,4))
  code=c.invoke('dh2_character_skill_info_v1' if self.native else 0x3d7e88,[self.state,0,v[1],self.fraction if has_fraction else 0,self.service] if self.native else [self.state,0,v[1],self.fraction if has_fraction else 0])
  if self.native:assert code==0,code
  result=bytes(c.uc.mem_read(self.fraction,4));return result,b''.join(self.trace)
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();old=Machine(REPO/'.local-inputs/libDungeonHunter2.so',False);new=Machine(a.library,True);rng=random.Random(20261005);records=[];callbacks=0
 for i in range(640):
  # null second active after receiver mutation is unsafe in source and excluded.
  v=[1,rng.getrandbits(32),rng.randrange(2),1,rng.randrange(4)==0,rng.randrange(5),rng.randrange(4)==0,rng.randrange(5),rng.randrange(9),bits(rng.choice([0.,1.,2.5,-1.,float(2**31),math.nan,math.inf])),rng.randrange(2),rng.choice([0,1,2,199,0xffffffff]),bits(-17.25),rng.randrange(4),rng.choice([0,1,200,0xffffffff]),rng.randrange(2)]
  if i%13==0:v[0]=0;v[15]=1
  old.configure(v);new.configure(v);expected,events=old.run();actual,nevents=new.run();assert events==nevents,(i,v,events,nevents);x,y=floating(struct.unpack('<I',expected)[0]),floating(struct.unpack('<I',actual)[0]);assert actual==expected or math.isnan(x) and math.isnan(y),(i,v,actual,expected);records.append(block(words(*v))+block(expected+words(len(events)//16)+events));callbacks+=len(events)//16
 gold=ROOT/'reference/character-skill-info-v1';gold.mkdir(parents=True,exist_ok=True);(gold/'skill-info-gold.bin').write_bytes(words(0x31494653,len(records))+b''.join(records));report=dict(validation='PASS',cases=len(records),ordered_services=callbacks,mismatches=0,original_instructions_executed=True,optimized_arm64_instructions_executed=True,source_sha256={x.relative_to(REPO).as_posix():hashlib.sha256(x.read_bytes()).hexdigest()for x in [ROOT/'character_skill_info_v1.hpp',ROOT/'character_skill_info_v1.cpp',Path(__file__)]},library_sha256=hashlib.sha256(a.library.read_bytes()).hexdigest(),gold_sha256=hashlib.sha256((gold/'skill-info-gold.bin').read_bytes()).hexdigest(),scope=__doc__);(ROOT/'reports/character-skill-info-v1-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
