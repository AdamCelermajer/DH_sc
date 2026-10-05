"""Original nonplayer HitFor including distinct player achievement tail.
Game, policy, trophy and full Kill delivery are explicit fixture services;
genuine property/handle kernels and source instruction order execute.
"""
import hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3];sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from character_hit_differential import Audit,w,signed,words,string
OLD=ROOT/'.local-inputs/libDungeonHunter2.so';LIB=ROOT/'.local-inputs/libcharacter_hit_v6_oracle.so'
class Extended(Audit):
 def __init__(self,path,native,manifest,defaults,types):
  super().__init__(path,native,manifest,defaults,types);self.trophy=self.c.data+0x35000
  if not native:
   got=(0x3a8bdc+w(self.c,0x3a921c))&0xffffffff
   self.c.pointer(w(self.c,got+w(self.c,0x3a9244)),self.trophy)
 def extra(self,op,subject=0,target=0,name=''):
  self.trace.append([op,subject,target,name])
  if op==11:return self.config['local']
  if op==12:return self.trophy
  if op==13:return ['power_200dam','power_150dam','power_100dam','power_50dam','power_onehit'].index(name)+10
  if op==15:return self.config['major']
  if op==16:return self.config['minor']
  return 0
 def hook(self,uc,at,size,user):
  c=self.c
  if self.native and at==self.cb:
   op,force,subject,target,name=struct.unpack('<IIQQQ',uc.mem_read(c.reg(2),32))
   if op>=11:
    text=string(c,name).decode()if name else ''
    answer=self.extra(op,8 if subject==self.trophy else self.ident(subject),signed(force)if op==14 else 0,text)
    uc.mem_write(c.reg(3),struct.pack('<Q',answer));self.finish();return
  if not self.native:
   if at==0x3a8e7c:self.extra(12);return
   if at==0x3a8e9c:return # Newly recovered source continuation.
   if at==0x36effc:self.finish(self.extra(11,self.ident(c.reg(1))));return
   if at==0x3a3f70:self.finish(self.extra(13,0,0,string(c,c.reg(0)).decode()));return
   if at==0x3813b8:self.extra(14,8,signed(c.reg(1)));self.finish();return
   if at==0x3a3158:self.finish(self.extra(15,self.ident(c.reg(0))));return
   if at==0x3a3144:self.finish(self.extra(16,self.ident(c.reg(0))));return
  super().hook(uc,at,size,user)
def main():
 manifest=json.loads((ROOT/'.local-inputs/character-hit-discovery/full/original-functions.json').read_text())
 raw=(ROOT/'port/android-native/app/src/main/assets/data/character_properties_pyarray.bin').read_bytes();defaults=list(struct.unpack_from('<224i',raw,4));types=list(struct.unpack_from('<224i',raw,900))
 old=Extended(OLD,False,manifest,defaults,types);new=Extended(LIB,True,{'functions':[]},defaults,types);rng=random.Random(2026100567);cases=[]
 for i in range(4096):
  hp=rng.choice([1000000,200000,256,1]);damage=rng.choice([0,1,49*256,50*256,99*256,100*256,149*256,150*256,199*256,200*256,300*256])
  sheets=[defaults.copy()for _ in range(4)];sheets[1][36]=sheets[3][36]=hp;sheets[3][38]=hp if i%2 else hp+1
  x=dict(dead=0,main_present=1,main_dead=0,god=0,monster=1,online=0,oneshot=0,app_oneshot=0,players=[0],attacker_player=1,character=1,remote=i%2,attacker=3,cached=3,key=5,oldframe=1,frame=1,local=i%3!=0,major=i%4==0,minor=i%5==0)
  old.fixture(sheets,x);new.fixture(sheets,x)
  old.c.invoke(0x3a8bc4,[old.actor,damage,old.enemy])
  status=signed(new.c.invoke('dh2_character_hit_for_v6',[new.out,new.actor,damage,new.attacker,new.bind]));assert status==1,(i,status,new.trace)
  assert old.trace==new.trace,(i,old.trace,new.trace,x)
  assert old.state()==new.state(),(i,old.state(),new.state())
  assert all(bytes(old.c.uc.mem_read(a,896))==bytes(new.c.uc.mem_read(b,896))for a,b in zip(old.sheets,new.sheets)),i
  cases.append({'damage':damage,'hp':hp,'config':x,'trace':old.trace,'after_hp':signed(w(new.c,new.sheets[3]+36*4))})
 ref=ROOT/'port/level-world/reference/character-skill-combat-v6';(ref/'hit-player-gold-v6.json').write_text(json.dumps(cases,indent=2)+'\n')
 report=dict(validation='PASS',cases=len(cases),ordered_services=sum(len(c['trace'])for c in cases),achievement_calls=sum(t[0]==14 for c in cases for t in c['trace']),mismatches=0,original_sha256=hashlib.sha256(OLD.read_bytes()).hexdigest(),optimized_sha256=hashlib.sha256(LIB.read_bytes()).hexdigest(),scope=__doc__,full_kill_backend=False)
 (ROOT/'port/level-world/reports/character-hit-v6-player-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
