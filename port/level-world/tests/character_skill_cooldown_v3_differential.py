"""Actual cooldown Lua wrapper instructions versus O2 ARM64 callback bodies.

Value.getNumber, live list lookup and owned instance access/write are declared
services. Original tag guards, repeated conversion, IEEE unsigned/signed
conversions and nullable writes execute; actual retained owners/VM/timer expiry
are covered by the separate complete-cache player sanitizer composition.
"""
import argparse,hashlib,json,math,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu as Base,words,bits,signed,floating
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Cpu(Base):
 def external(self,uc,at,size,user):
  n=self.imports.get(at,'')
  if self.arm64 and 'CharacterSkillOwnerV3' in n:self.owner_service(n);return
  return super().external(uc,at,size,user)
class Writer:
 def __init__(self,path,native):
  self.native=native;self.c=Cpu(path,native,{'functions':[]});c=self.c;d=c.data
  self.owner=d+0x1000;self.args=d+0x3000;self.vector=d+0x3100;self.values=d+0x4000;self.out=d+0x7000;self.result=d+0x7100;self.bind=d+0x7200;self.list=d+0x7300;self.state=d+0x7400;self.slots=d+0x7500;self.instances=d+0x8000
  c.owner_service=self.owner_service;c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def word(self,p):return struct.unpack('<I',self.c.uc.mem_read(p,4))[0]
 def ret(self,n=0):self.c.put(0,n);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def owner_service(self,name):
  c=self.c
  if 'list_count' in name:self.trace.append(['list',c.reg(1)]);c.uc.mem_write(c.reg(2),words(self.count));self.ret()
  elif 'set_cooldown' in name:
   kind,index,value=c.reg(1),c.reg(2),c.reg(3);assert index<self.count
   if self.mask&(1<<index):c.uc.mem_write(self.instances+index*32+20,words(value))
   self.ret(1 if self.mask&(1<<index) else 0)
  elif 'state' in name:self.ret(self.state)
  else:raise AssertionError(name)
 def hook(self,uc,at,size,user):
  c=self.c
  if self.native:return
  if at==0x37baf8:self.ret(self.values+112*c.reg(1))
  elif at==0x31bbf0:self.ret(self.word(c.reg(0)+8))
  elif at in (0x3bc5fc,0x3ae5dc):self.trace.append(['list',int(at==0x3ae5dc)]);self.ret(self.list)
 def run(self,spell,values,count,mask):
  c=self.c;self.count=count;self.mask=mask;self.trace=[]
  c.uc.mem_write(self.list,words(0,count,0));c.uc.mem_write(self.result,words(0))
  for i in range(5):
   if self.native:c.uc.mem_write(self.instances+i*32,struct.pack('<QQIifI',self.owner,0,i,0x11223344,0,0));c.uc.mem_write(self.slots+i*8,struct.pack('<Q',self.instances+i*32 if mask&(1<<i) else 0))
   else:c.uc.mem_write(self.instances+i*32,bytes(32));c.pointer(self.instances+i*32+24,0x11223344);c.pointer(self.slots+i*4,self.instances+i*32 if mask&(1<<i) else 0)
  for i,(tag,value) in enumerate(values):
   if self.native:c.uc.mem_write(self.values+i*40,struct.pack('<IIfIQQQ',tag,0,value,int(value!=0) if tag==1 else 0,0,0,0))
   else:c.uc.mem_write(self.values+i*112,bytes(112));c.pointer(self.values+i*112+4,tag);c.pointer(self.values+i*112+8,bits(float(value)))
  if self.native:
   c.uc.mem_write(self.state,struct.pack('<QQIIQII',self.owner,self.slots,count,0,self.slots,count,0));c.uc.mem_write(self.bind,struct.pack('<QQQ',self.owner,0,0))
   name='_ZN3dh29character6skills'+('21spell_set_cooldown_v3' if spell else '21skill_set_cooldown_v3')+'EPvPK16dh2_script_valuejPS3_jPjPcm'
   # Obtain the exact public C++ symbol rather than assume identifier length.
   name=next(n for n in c.symbols if ('spell_set_cooldown_v3' if spell else 'skill_set_cooldown_v3') in n)
   r=c.invoke(name,[self.bind,self.values,len(values),self.out,0,self.result,self.out+256,256]);assert signed(r)==0,(spell,values,count,r)
  else:
   c.pointer(self.owner+(0x488 if spell else 0x47c),self.slots);c.pointer(self.args+4,self.vector);c.uc.mem_write(self.vector,words(self.values,self.values+112*len(values),self.values+112*len(values)));c.invoke(0x3b90e4 if spell else 0x3b97e0,[self.args,self.out,self.owner])
  return self.trace,[self.word(self.instances+i*32+(20 if self.native else 24)) for i in range(5)]
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();old=ROOT/'.local-inputs/libDungeonHunter2.so';o,n=Writer(old,False),Writer(a.library,True);rng=random.Random(20261006);rows=[]
 choices=[-math.inf,-3.,-.1,-0.,0.,.999,1.,2.9,16777216.,2147483648.,4294967296.,math.inf,math.nan]
 for i in range(1024):
  spell=bool(i%2);count=1+i%5;mask=rng.randrange(32)
  if spell:values=[(rng.choice([0,1,3,5]),rng.choice(choices))][:i%3]
  else:
   tag=rng.choice([0,1,3]);index=rng.randrange(count) if tag==3 else rng.choice([0,1]) if tag==1 else 0
   values=[(tag,index),(rng.choice([0,1,3,5]),rng.choice(choices))][:i%4]
  e=o.run(spell,values,count,mask);v=n.run(spell,values,count,mask);assert e==v,(i,spell,values,count,mask,e,v);rows.append(dict(spell=spell,values=[[t,bits(float(v))]for t,v in values],count=count,mask=mask,trace=e[0],words=e[1]))
 ref=ROOT/'port/level-world/reference/character-skill-gameplay-v3';gold=ref/'cooldown-writer-gold.json';gold.write_text(json.dumps(dict(validation='PASS',cases=rows),indent=2)+'\n');report=dict(validation='PASS',cases=len(rows),live_list_requests=sum(len(x['trace'])for x in rows),mismatches=0,original_sha256=sha(old),optimized_arm64_sha256=sha(a.library),gold_sha256=sha(gold),original_instructions_executed=True,optimized_arm64_instructions_executed=True,scope=__doc__)
 target=ROOT/'port/level-world/reports/character-skill-gameplay-v3-cooldown-arm64-differential.json';target.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
