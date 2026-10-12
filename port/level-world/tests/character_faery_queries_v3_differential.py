"""Original equipped-faery scalar wrappers versus O2 ARM64.

Saved selection, actual Faery-row resolution and saved-level helpers are
declared ordered services here; complete owned cache/VM composition is separate.
The original element wrapper actually loads row+8, not an inferred type ID.
"""
import hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu,words,signed
OLD=ROOT/'.local-inputs/libDungeonHunter2.so'
LIB=ROOT/'.local-inputs/character-skill-gameplay-v3/libskill_gameplay_v3_oracle.so'
REF=ROOT/'port/level-world/reference/character-skill-gameplay-v3'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Query:
 def __init__(self,native):
  self.native=native;self.c=Cpu(LIB if native else OLD,native,{'functions':[]});c=self.c;self.owner=c.data+0x1000;self.out=self.owner+0x2000;self.row=self.owner+0x3000;self.svc=self.owner+0x4000
  if native:c.uc.mem_write(self.svc,struct.pack('<QQ',self.owner,c.stop+0x100))
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,v):self.c.put(0,v);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def hook(self,uc,at,size,user):
  c=self.c
  if self.native and at==c.stop+0x100:
   q,r=c.reg(1),c.reg(2);op,id,diff,res,owner=struct.unpack('<IIiIQ',uc.mem_read(q,24));assert owner==self.owner and diff==-1 and not res
   self.trace.append([op,id,diff]);uc.mem_write(r,words(self.selected if op==1 else self.value));self.ret(0);return
  if self.native:return
  if at==0x3bb98c:assert c.reg(0)==self.owner and signed(c.reg(1))==-1;self.trace.append([1,0,-1]);self.ret(self.selected)
  elif at==0x3aeac0:assert c.reg(0)==self.owner;self.trace.append([4,c.reg(1),-1]);self.ret(self.row)
  elif at==0x3bbc18:assert c.reg(0)==self.owner and signed(c.reg(2))==-1;self.trace.append([3,c.reg(1),-1]);self.ret(self.value)
  elif at==0x37cb24:assert c.reg(0)==self.out;self.answer=signed(c.reg(1));self.ret(0)
 def run(self,kind,selected,value):
  c=self.c;self.selected=selected;self.value=value;self.trace=[];self.answer=None;c.uc.mem_write(self.row+8,words(value));c.uc.mem_write(self.out,words(0x12345678))
  if self.native:
   code=c.invoke('dh2_character_faery_element_v3' if kind==0 else 'dh2_character_faery_level_v3',[self.out,self.owner,self.svc]);assert code==1;self.answer=signed(struct.unpack('<I',c.uc.mem_read(self.out,4))[0])
  else:c.invoke(0x3b6dc4 if kind==0 else 0x3b6df8,[self.owner+0x100,self.out,self.owner])
  return self.answer,self.trace
def main():
 o,n=Query(False),Query(True);rng=random.Random(202610043);cases=[]
 for i in range(768):
  inp=[i%2,rng.choice([0,1,4,0xffffffff,0x80000000]),rng.choice([-2147483648,-1,0,1,65535,2147483647,rng.randrange(-10000,10000)])]
  expected=o.run(*inp);actual=n.run(*inp);assert expected==actual,(inp,expected,actual);cases.append(dict(input=inp,answer=expected[0],trace=expected[1]))
 gold=REF/'faery-query-gold-v3.json';gold.write_text(json.dumps(cases,indent=2)+'\n')
 files=['port/level-world/character_faery_element_v3.hpp','port/level-world/character_faery_element_v3.cpp','port/level-world/tests/character_faery_queries_v3_differential.py']
 result=dict(validation='PASS',cases=len(cases),ordered_services=1536,mismatches=0,original_sha256=sha(OLD),optimized_arm64_sha256=sha(LIB),gold_sha256=sha(gold),source_sha256={p:sha(ROOT/p)for p in files},scope=__doc__)
 report=ROOT/'port/level-world/reports/character-skill-gameplay-v3-faery-arm64-differential.json';report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(validation='PASS',cases=768,ordered_services=1536)))
if __name__=='__main__':main()
