"""Execute actual HasMana/UseMana ARM32 and O2 ARM64 property-kernel effects.
Application/player/Debug ownership policies remain declared synchronous inputs.
Costs cover the nonnegative source assertion domain only.
"""
import hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3];sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu,words,signed
OLD=ROOT/'.local-inputs/libDungeonHunter2.so';LIB=ROOT/'.local-inputs/libskill_mana_v5_oracle.so'
class Actor:
 def __init__(self,native):
  self.native=native;self.c=Cpu(LIB if native else OLD,native,{'functions':[]});c=self.c;d=c.data
  self.owner=d+0x1000;self.ext=d+0x3000;self.svc=d+0x4000;self.app=d+0x5000;self.table=d+0x6000;self.debug=d+0x7000;self.view=d+0x8000;self.sheets=d+0x10000;self.out=d+0x9000
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
  if native:c.uc.mem_write(self.svc,struct.pack('<QQ',self.owner,c.stop+0x100))
  else:c.pointer(self.owner,self.table);c.pointer(self.table+0x54,c.stop+0x200)
 def ret(self,value=0):self.c.put(0,value);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def hook(self,uc,at,size,user):
  c=self.c;v=self.v
  if self.native:
   if at!=c.stop+0x100:return
   q,r=c.reg(1),c.reg(2);op,res,subject,name=struct.unpack('<IIQQ',uc.mem_read(q,24));assert not res
   text=c.string(name).decode()if name else None;self.trace.append([op,text])
   if op==5:self.debugname=text
   result=v['app']if op==1 else v['player']if op==2 else v['contains']if op==3 else v['god']if op==6 and self.debugname=='GOD_MANA' else v['tracing']if op==6 else 0
   uc.mem_write(r,struct.pack('<IIQ',result,0,self.debug if op==5 else 0));self.ret();return
  if at==0x3dedb4:assert c.reg(2)==41;self.ret(self.current);return
  if at==0x3e0708:assert c.reg(1)==41;self.current=(self.current+c.reg(2))&0xffffffff;self.ret();return
  mapping={0x7fd794:1,c.stop+0x200:2,0x320e14:3,0x337888:4,0x3140ec:5,0x337a88:6,0x318254:7}
  if at not in mapping:return
  op=mapping[at];text=c.string(c.reg(1)).decode()if op in(3,5)else None
  self.trace.append([op,text])
  if op==5:self.debugname=text
  result=self.app if op==1 else v['player']if op==2 else v['contains']if op==3 else v['god']if op==6 and self.debugname=='GOD_MANA' else v['tracing']if op==6 else c.reg(0)if op==5 else 0
  self.ret(result)
 def run(self,use,v):
  self.v=v;self.trace=[];self.current=v['mana']&0xffffffff;self.debugname=None;c=self.c
  if self.native:
   pointers=[self.sheets+i*0x1000 for i in range(6)]
   for p in pointers:c.uc.mem_write(p,words(*([0]*224)))
   c.uc.mem_write(pointers[1],words(*([8]*224)));c.uc.mem_write(pointers[5]+41*4,words(v['mana']))
   c.uc.mem_write(self.view,struct.pack('<7QII',*pointers,0,0,0))
   c.uc.mem_write(self.ext,struct.pack('<QQ8B',self.owner,self.view,v['bypass'],*([0]*7)))
   c.uc.mem_write(self.out,words(99))
   status=signed(c.invoke('dh2_character_skill_mana_v5',[self.out,self.ext,use,v['cost'],self.svc]));assert status==0
   result=struct.unpack('<I',c.uc.mem_read(self.out,4))[0];current=struct.unpack('<I',c.uc.mem_read(pointers[5]+41*4,4))[0]
  else:
   c.uc.mem_write(self.app+5,bytes([v['app']]));c.uc.mem_write(self.owner+0x14f0,bytes([v['bypass']]))
   result=c.invoke(0x3bdef4 if use else 0x3bd40c,[self.owner,v['cost']]);current=self.current
  return result,current,self.trace
def main():
 o,n=Actor(False),Actor(True);rng=random.Random(2026100572);cases=[]
 for i in range(4096):
  v=dict(mana=signed(rng.getrandbits(32)),cost=rng.randrange(2147483648),app=i%2,player=(i//2)%2,contains=(i//4)%2,god=(i//8)%2,bypass=(i//16)%2,tracing=(i//32)%2)
  if i%3==0:v['mana']=rng.randrange(10000);v['cost']=rng.randrange(10000)
  if i%7==0:v['cost']=0
  use=(i//64)%2
  expected=o.run(use,v);actual=n.run(use,v);assert expected==actual,(i,v,expected,actual)
  cases.append(dict(use=use,input=v,result=expected[0],mana=expected[1],services=expected[2]))
 ref=ROOT/'port/level-world/reference/character-skill-native-v5';ref.mkdir(parents=True,exist_ok=True)
 (ref/'mana-gold-v5.json').write_text(json.dumps(cases,indent=2)+'\n')
 report=dict(validation='PASS',cases=len(cases),ordered_services=sum(len(c['services'])for c in cases),mismatches=0,original_sha256=hashlib.sha256(OLD.read_bytes()).hexdigest(),optimized_arm64_sha256=hashlib.sha256(LIB.read_bytes()).hexdigest(),scope=__doc__)
 (ROOT/'port/level-world/reports/character-skill-mana-v5-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
