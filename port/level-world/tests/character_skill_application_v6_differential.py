"""Original F_ApplyResult order, genuine leech and positive native HitFor bodies.
Policy, reactions, cancellation, text, sound, AI and player lookup are declared
fixture services. This proof does not claim populated-target gameplay.
"""
import hashlib,json,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from character_hit_differential import Audit,words,w,signed,string,packed
OLD=ROOT/'.local-inputs/libDungeonHunter2.so';LIB=ROOT/'.local-inputs/libcharacter_skill_application_v6_oracle.so'
class Application(Audit):
 def __init__(self,path,native,defaults,types):
  super().__init__(path,native,{'functions':[]},defaults,types);d=self.c.data
  self.result=d+0x50000;self.a=d+0x51000;self.b=d+0x52000;self.fields=d+0x53000;self.debug_bind=d+0x54000;self.apply_bind=d+0x55000;self.debug_cb=d+0x56000;self.player=d+0x57000
  self.hit_cb=d+0x58000;self.enemy_view=d+0x59000;self.enemy_sheets=[d+x for x in (0x60000,0x61000,0x62000,0x63000)];self.trophy=d+0x64000;self.positive=False;self.in_hit=False
  if native:
   self.c.uc.mem_write(self.debug_bind,struct.pack('<QQ',0,self.debug_cb));self.c.uc.mem_write(self.apply_bind,struct.pack('<QQQ',0,self.cb,self.debug_bind))
  else:
   got=(0x3b10cc+w(self.c,0x3b1d74))&0xffffffff
   self.c.pointer(w(self.c,got+w(self.c,0x3b1d90))+0x40,self.game)
   for obj in [self.actor,self.enemy]:
    self.c.pointer(obj+0x3c8,self.vt);self.c.pointer(self.vt+0xb4,self.cb+20)
   self.c.uc.mem_write(self.cb+20,words(0xe12fff1e))
   hit_got=(0x3a8bdc+w(self.c,0x3a921c))&0xffffffff
   self.c.pointer(w(self.c,hit_got+w(self.c,0x3a9244)),self.trophy)
 def extra_hit(self,op,subject=0,target=0,name=''):
  self.trace.append([op,subject,target,name]);return self.trophy if op==12 else 0
 def event_apply(self,op,subject=0,word=0,flags=0,name=''):
  self.trace.append([op,subject,signed(word),flags,name])
  if op==1:return 0
  if op==2:return self.config['saved_god']
  if op==3:return self.config['attacker_player']if subject==(3 if self.positive else 1)else 0
  if op==4:return 0
  if op==20:return self.player
  return 0
 def debug_event(self,op,text):
  self.trace.append([100+op,0,0,0,text]);return self.config.get(text,0)
 def hook(self,uc,at,size,user):
  c=self.c
  if self.positive:
   if not self.native and at==0x3a8bc4:self.in_hit=True;self.hit_return=c.uc.reg_read(c.lr)
   if not self.native and self.in_hit and at==self.hit_return:self.in_hit=False
   if self.native and at==self.hit_cb:
    op,force,subject,target,name=struct.unpack('<IIQQQ',uc.mem_read(c.reg(2),32));text=string(c,name).decode()if name else ''
    answer=self.extra_hit(op,8 if subject==self.trophy else self.ident(subject),signed(force)if op==14 else 0,text)if op>=11 else self.event(op,self.ident(subject),self.ident(target),text)
    uc.mem_write(c.reg(3),struct.pack('<Q',answer));self.finish();return
   if not self.native and self.in_hit:
    if at==0x3a8e7c:self.extra_hit(12);return
    if at==0x3a8e9c:return
    if at==0x36effc:self.finish(self.extra_hit(11,self.ident(c.reg(1))));return
    if at==0x3140ec:c.pointer(c.reg(0),c.reg(1));self.finish(c.reg(0));return
    if at==0x318254:self.finish();return
    return Audit.hook(self,uc,at,size,user)
  if self.native:
   if at==self.debug_cb:
    op,_,subject,name=struct.unpack('<IIQQ',uc.mem_read(c.reg(1),24));text=string(c,name).decode()if name else self.tokens.get(subject,'')
    value=self.debug_event(op,text)
    if op==2:value=len(self.tokens)+1;self.tokens[value]=text
    if op==4:self.tokens.pop(subject)
    uc.mem_write(c.reg(2),struct.pack('<Q',value));self.finish();return
   if at==self.cb:
    op,word,flags,_,subject,attacker,target,name,number,_=struct.unpack('<IIIIQQQQfI',uc.mem_read(c.reg(1),56))
    if op==6:word=struct.unpack('<I',struct.pack('<f',number))[0]
    answer=self.event_apply(op,self.ident(subject),word,flags,string(c,name).decode()if name else '')
    uc.mem_write(c.reg(2),struct.pack('<QIf',answer if op==20 else 0,answer if op!=20 else 0,0));self.finish();return
   return
  if at==0x337888:self.debug_event(1,'');self.finish();return
  if at==0x3140ec:
   text=string(c,c.reg(1)).decode();self.debug_event(2,text);c.pointer(c.reg(0),c.reg(1));self.finish(c.reg(0));return
  if at==0x337a88:self.finish(self.debug_event(3,string(c,w(c,c.reg(1))).decode()));return
  if at in [0x3139ac,0x318254]:self.debug_event(4,string(c,w(c,c.reg(0))).decode());self.finish();return
  if at==0x7fd794:self.event_apply(1);self.finish(self.online);return
  if at==0x320e14:self.finish(self.event_apply(2,0,0,0,string(c,c.reg(1)).decode()));return
  if at==self.cb:self.finish(self.event_apply(4,self.ident(c.reg(0))));return
  if at==self.cb+4:self.finish(self.event_apply(3,self.ident(c.reg(0))));return
  if at==self.cb+20:self.event_apply(19,1 if c.reg(0)==self.actor+0x3c8 else 3);self.finish();return
  if at==0x36eea8:self.event_apply(20,self.ident(c.reg(1)));self.finish(self.player);return
  if self.positive and at==0x3b1578:self.event_apply(5);return
  if self.positive and at==0x3d7c68:self.event_apply(6,1,c.reg(2));self.finish();return
  mapping={0x3c5b3c:9,0x3c5c60:10,0x3c5d84:11,0x3c5ea0:12,0x3c5ffc:13,0x3c6144:14,0x3e2a5c:15,0x3bc6b8:16,0x3af77c:17,0x3afee0:18}
  if at in mapping:
   op=mapping[at];subject=(1 if self.positive else 3)if op<16 else self.ident(c.reg(0))if op==16 else 0
   word=c.reg(1)if op in [12,13,14,15]else 0
   flags=c.reg(3)if op==12 else w(c,c.uc.reg_read(c.sp))if op==14 else c.reg(2)if op==11 else 0
   self.event_apply(op,subject,word,flags);self.finish();return
  super().hook(uc,at,size,user)
 def prepare(self,sheets,x,result,combo):
  self.fixture(sheets,x);c=self.c;self.tokens={};self.trace=[]
  self.in_hit=False
  c.uc.mem_write(self.result,struct.pack('<6iIIii',*result))
  if self.native:
   c.uc.mem_write(self.fields,struct.pack('<HBBi',combo,0,0,-1))
   pointers=[self.actor,self.view,0,0,0,0,0,0,self.fields,self.fields+2,self.fields+3,self.fields+4]
   c.uc.mem_write(self.a,struct.pack('<12Q',*pointers));pointers[0]=self.enemy;c.uc.mem_write(self.b,struct.pack('<12Q',*pointers))
   if self.positive:
    for p,s in zip(self.enemy_sheets,sheets):c.uc.mem_write(p,packed(s))
    c.uc.mem_write(self.enemy_view,struct.pack('<7QII',self.defaults,self.types,*self.enemy_sheets,0,0,0));c.uc.mem_write(self.bind,struct.pack('<QQ',0,self.hit_cb))
    pointers=[self.enemy,self.enemy_view,0,0,self.attacker,0,0,0,self.fields,self.fields+2,self.fields+3,self.fields+4];c.uc.mem_write(self.a,struct.pack('<12Q',*pointers))
    pointers[0]=self.actor;pointers[1]=self.view;pointers[3]=self.actor;pointers[4]=0;pointers[5]=self.bind;c.uc.mem_write(self.b,struct.pack('<12Q',*pointers))
  else:
   c.uc.mem_write(self.actor+0x14d0,struct.pack('<H',combo));c.uc.mem_write(self.enemy+0x14f0,bytes(1));c.uc.mem_write(self.enemy+0x110,words(-1));c.uc.mem_write(self.online+5,bytes(1));c.uc.mem_write(self.player+0x670,words(1))
   if self.positive:
    self.enemy_sheets=[self.enemy+0x560+n+4 for n in (8,0x38c,0x710,0xa94)]
    for p,s in zip(self.enemy_sheets,sheets):c.uc.mem_write(p,packed(s))
    sentinel=self.enemy+0x560+0xe18;c.uc.mem_write(sentinel,words(0,0,sentinel,sentinel));c.pointer(self.enemy+0x560+0xe28,0)
    c.uc.mem_write(self.enemy+0x14d0,struct.pack('<H',combo));c.uc.mem_write(self.actor+0x14f0,bytes(1));c.uc.mem_write(self.actor+0x110,words(-1));c.uc.mem_write(self.game+0x6c4,words(1))
 def final_combo(self):return struct.unpack('<H',self.c.uc.mem_read(self.fields if self.native else (self.enemy if self.positive else self.actor)+0x14d0,2))[0]
def main():
 raw=(ROOT/'port/android-native/app/src/main/assets/data/character_properties_pyarray.bin').read_bytes();defaults=list(struct.unpack_from('<224i',raw,4));types=list(struct.unpack_from('<224i',raw,900))
 old=Application(OLD,False,defaults,types);new=Application(LIB,True,defaults,types);rng=random.Random(2026100569);cases=[]
 for i in range(6144):
  positive=i>=4096
  for a in [old,new]:a.positive=positive
  sheets=[defaults.copy()for _ in range(4)]
  for p,maxp in [(36,38),(41,43)]:sheets[1][p]=sheets[3][p]=10000;sheets[3][maxp]=100000
  for prop in [140,143,146,185,187,189]:sheets[3][prop]=rng.choice([0,256,3000])
  x=dict(dead=0,main_present=1,main_dead=0,god=0,monster=1,online=0,oneshot=0,app_oneshot=0,players=[0],attacker_player=i%2,character=1,remote=0,attacker=1,cached=1,key=-7,oldframe=0,frame=1,saved_god=i%11==0,NoDamages=i%7==0,GOD=i%9==0)
  result=[rng.choice([0,-1]),-1,-1,-1,rng.choice([-1,0,256]),rng.choice([-1,0,256]),rng.getrandbits(9),rng.choice([0,0x18000000,0x20000000,0x15000]),-1,-1];combo=rng.randrange(65536)
  if positive:
   x.update(attacker=3,cached=3,key=5,local=0,major=0,minor=0);result[0]=rng.choice([1,256,1000]);result[6]&=~8
  for a in [old,new]:a.prepare(sheets,x,result,combo)
  old.c.invoke(0x3b10b4,[old.result,old.enemy if positive else old.actor,old.actor if positive else old.enemy,0])
  status=signed(new.c.invoke('dh2_character_skill_apply_result_v6',[new.out,new.result,new.a,new.b,new.apply_bind]));assert status==0,(i,status,new.trace)
  assert old.trace==new.trace,(i,old.trace,new.trace,result,x)
  assert old.final_combo()==new.final_combo(),(i,'combo')
  assert all(bytes(old.c.uc.mem_read(a,896))==bytes(new.c.uc.mem_read(b,896))for a,b in zip(old.sheets,new.sheets)),(i,'property')
  if positive:assert all(bytes(old.c.uc.mem_read(a,896))==bytes(new.c.uc.mem_read(b,896))for a,b in zip(old.enemy_sheets,new.enemy_sheets)),(i,'attacker property')
  cases.append(dict(result=result,combo=combo,trace=old.trace,final_combo=old.final_combo()))
 ref=ROOT/'port/level-world/reference/character-skill-combat-v6';gold=ref/'application-gold-v6.json';gold.write_text(json.dumps(cases,indent=2)+'\n')
 report=dict(validation='PASS',cases=len(cases),positive_amount_cases=2048,ordered_services=sum(len(x['trace'])for x in cases),mismatches=0,original_sha256=hashlib.sha256(OLD.read_bytes()).hexdigest(),optimized_sha256=hashlib.sha256(LIB.read_bytes()).hexdigest(),gold_sha256=hashlib.sha256(gold.read_bytes()).hexdigest(),scope=__doc__,full_target_application=False)
 (ROOT/'port/level-world/reports/character-skill-application-v6-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
