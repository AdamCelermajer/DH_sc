"""Original RegenHP/MP instruction order, captured delta and genuine Add kernel."""
import hashlib,json,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from character_hit_differential import Audit,words,w,signed,string
OLD=ROOT/'.local-inputs/libDungeonHunter2.so';LIB=ROOT/'.local-inputs/libcharacter_skill_application_v6_oracle.so'
class Regen(Audit):
 def hook(self,uc,at,size,user):
  c=self.c
  if self.native and at==self.cb:
   op,_,subject,name=struct.unpack('<IIQQ',uc.mem_read(c.reg(1),24))
   text=string(c,name).decode()if name else self.tokens.get(subject,'')
   self.trace.append([op,text]);value=0
   if op==2:value=len(self.tokens)+1;self.tokens[value]=text
   if op==3 and self.mutate is not None:
    for p in [self.sheets[1],self.sheets[3]]:uc.mem_write(p+self.property*4,words(self.mutate))
   if op==4:self.tokens.pop(subject)
   uc.mem_write(c.reg(2),struct.pack('<Q',value));self.finish();return
  if not self.native:
   if at==0x337888:self.trace.append([1,'']);self.finish();return
   if at==0x3140ec:
    text=string(c,c.reg(1)).decode();self.trace.append([2,text]);c.pointer(c.reg(0),c.reg(1));self.finish(c.reg(0));return
   if at==0x337a88:
    self.trace.append([3,string(c,w(c,c.reg(1))).decode()])
    if self.mutate is not None:
     for p in [self.sheets[1],self.sheets[3]]:uc.mem_write(p+self.property*4,words(self.mutate))
    self.finish();return
   if at==0x318254:self.trace.append([4,string(c,w(c,c.reg(0))).decode()]);self.finish();return
  super().hook(uc,at,size,user)
def main():
 raw=(ROOT/'port/android-native/app/src/main/assets/data/character_properties_pyarray.bin').read_bytes();defaults=list(struct.unpack_from('<224i',raw,4));types=list(struct.unpack_from('<224i',raw,900))
 old=Regen(OLD,False,{'functions':[]},defaults,types);new=Regen(LIB,True,{'functions':[]},defaults,types);rng=random.Random(2026100568);cases=[]
 for i in range(4096):
  mp=i%2;prop=41 if mp else 36;maximum_prop=43 if mp else 38
  current=rng.choice([0,1,256,100000,2147483647,-2147483648]);maximum=rng.choice([0,1,256,100000,2147483647,-2147483648]);amount=rng.choice([-2147483648,-1,0,1,256,100000,2147483647]);mutation=rng.choice([0,256,100000])if i%3==0 else None
  sheets=[defaults.copy()for _ in range(4)];sheets[1][prop]=sheets[3][prop]=current;sheets[3][maximum_prop]=maximum
  x=dict(dead=0,main_present=1,main_dead=0,god=0,monster=1,online=0,oneshot=0,app_oneshot=0,players=[0],attacker_player=0,character=1,remote=0,attacker=1,cached=1,key=-7,oldframe=0,frame=1)
  for a in [old,new]:a.fixture(sheets,x);a.tokens={};a.mutate=mutation;a.property=prop
  old.c.invoke(0x3bdbb8 if mp else 0x3bdca4,[old.actor,amount&0xffffffff])
  status=signed(new.c.invoke('dh2_character_skill_regen_v6',[new.view,mp,amount&0xffffffff,new.bind]));assert status==0,(i,status,new.trace)
  assert old.trace==new.trace,(i,old.trace,new.trace)
  assert all(bytes(old.c.uc.mem_read(a,896))==bytes(new.c.uc.mem_read(b,896))for a,b in zip(old.sheets,new.sheets)),(i,current,maximum,amount,mutation)
  cases.append(dict(mp=mp,current=current,maximum=maximum,amount=amount,mutation=mutation,trace=old.trace,after=signed(w(new.c,new.sheets[3]+prop*4))))
 ref=ROOT/'port/level-world/reference/character-skill-combat-v6';gold=ref/'regen-gold-v6.json';gold.write_text(json.dumps(cases,indent=2)+'\n')
 report=dict(validation='PASS',cases=len(cases),ordered_debug_calls=sum(len(c['trace'])for c in cases),mismatches=0,original_sha256=hashlib.sha256(OLD.read_bytes()).hexdigest(),optimized_sha256=hashlib.sha256(LIB.read_bytes()).hexdigest(),gold_sha256=hashlib.sha256(gold.read_bytes()).hexdigest(),scope=__doc__,debug_provider='explicit fixture, source property kernels execute',full_target_application=False)
 (ROOT/'port/level-world/reports/character-skill-regen-v6-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
