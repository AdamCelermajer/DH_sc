"""Complete original NativeGetPlayerHUDInfos caller vs O2 native producer.
AS object cast/coercion/member/string maintenance and skill/potion/property/
constant/player/saved-option endpoints are explicit delivered fixtures; source
slot prefix, actor guards, percent arithmetic and all ordered writes execute.
"""
import argparse,hashlib,json,math,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../level-world/tests'))
from visual_timeline_differential import TimelineCpu,integer
from navigation_search_differential import word
from aggro_differential import float_bits

def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def sint(v):return v if v<0x80000000 else v-0x100000000
def floating(v):return struct.unpack('<f',words(v))[0]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
NAMES=['PlayerActive','LEVEL','HP_PCT','HP_LOWPCT','MP_PCT','XP_PCT','SPELL_PCT','SPELL_MP','SKILL1_PCT','SKILL1_MP','SKILL2_PCT','SKILL2_MP','SKILL3_PCT','SKILL3_MP','NB_POTIONS','AVAIL_POINTS','TouchToMove']
class Machine:
 def __init__(self,path,native,manifest):
  self.c=TimelineCpu(path,native,manifest);self.native=native;c=self.c;d=c.data;self.input=d+0x1000;self.actor=d+0x2000;self.sheet=d+0x4000;self.services=d+0x5000;self.object=d+0x6000;self.vt=d+0x7000;self.fn=d+0x8000;self.args=d+0x9000;self.env=d+0xa000;c.handler=self.callback;c.uc.hook_add(UC_HOOK_CODE,self.hook)
  if not native:c.pointer(self.object,self.vt);c.pointer(self.vt+0x1c,c.callback+32)
 def ret(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def event(self,op,i=0,v=0,t=0):self.events.append(struct.pack('<4I2Q',op,i&0xffffffff,v&0xffffffff,t,1 if self.present else 0,1))
 def sheetbytes(self):return bytes(self.c.uc.mem_read(self.sheet if self.native else self.actor+0xff8,176))
 def mutate(self,i):
  if self.mutation&(1<<i):
   c=self.c;base=self.sheet if self.native else self.actor+0xff8
   for index in (33,36,41):c.uc.mem_write(base+index*4,words(word(c,base+index*4)^0x98765432))
 def deliver(self,op,i,v,t=0):
  if op==1:self.events.append(struct.pack('<4I2Q',op,i&0xffffffff,v&0xffffffff,t,0,1));return self.actor if self.present else 0
  self.event(op,i,v,t)
  if op==2:return self.slots[i]
  if op in(3,4,5):
   k=self.slots.index(sint(i));return (self.usable[k],self.levels[k],self.fractions[k])[op-3]
  if op==6:return self.fractions[3]
  if op==7:return self.meta[0 if i==19 else 4]
  if op==8:return self.meta[1]
  if op==9:return self.meta[2]
  if op==10:return self.meta[3]
  if op==11:return self.meta[5]
  if op==12:self.mutate(i);return 1
  if op==13:return self.meta[6]
  raise AssertionError(op)
 def callback(self,address):
  c=self.c
  if self.native:
   op,i,v,t,actor,obj=struct.unpack('<4I2Q',c.uc.mem_read(c.reg(1),32));out=c.reg(2);value=self.deliver(op,i,sint(v),t);identity=value if op==1 else 0;fraction=value if op in(5,6) else 0;c.uc.mem_write(out,struct.pack('<QII',identity,0 if op in(1,5,6) else value&0xffffffff,fraction));c.put(0,1)
  else:
   key=self.keys[c.reg(1)];i=NAMES.index(key);p=c.reg(2);t=c.uc.mem_read(p+1,1)[0];v=c.uc.mem_read(p+4,1)[0] if t==1 else int(struct.unpack('<d',c.uc.mem_read(p+4,8))[0]);self.deliver(12,i,v,t);c.put(0,1)
 def hook(self,uc,address,size,unused):
  if self.native:return
  c=self.c;r=c.reg
  if address==0x439cb4:self.ret(self.object)
  elif address==0x797a54:c.uc.mem_write(self.env+0x100,struct.pack('<d',float(sint(self.index))));lo,hi=struct.unpack('<2I',struct.pack('<d',float(sint(self.index))));c.put(0,lo);c.put(1,hi);uc.reg_write(c.pc,uc.reg_read(c.lr))
  elif address==0x797960:self.ret(self.remote)
  elif address==0x30ea24:self.ret(integer(struct.unpack('<d',words(r(0),r(1)))[0]))
  elif address==0x30ed30:
   lo,hi=struct.unpack('<2I',struct.pack('<d',float(sint(r(0)))));c.put(0,lo);c.put(1,hi);uc.reg_write(c.pc,uc.reg_read(c.lr))
  elif address==0x30e2a4:
   num,den=sint(r(0)),sint(r(1));v=self.deliver(13,self.divide_index,num) if not den else (abs(num)//abs(den)*(-1 if (num<0)!=(den<0) else 1));self.divide_index={36:41,41:33,33:36}[self.divide_index];self.ret(v)
  elif address==0x30ed6c:self.ret(float_bits(floating(r(0))*floating(r(1))))
  elif address==0x30e4cc:self.ret(integer(floating(r(0))))
  elif address==0x43c388:self.ret(self.deliver(1,r(0),sint(r(1))))
  elif address==0x3bbe68:self.ret(self.deliver(2,r(1),0))
  elif address==0x3d8358:self.ret(self.deliver(3,r(1),0))
  elif address==0x3bbed0:self.ret(self.deliver(4,r(1),0))
  elif address==0x3d7e88:c.uc.mem_write(r(3),words(self.deliver(5,r(1),sint(r(2)))));self.ret()
  elif address==0x3d7da8:c.uc.mem_write(r(1),words(self.deliver(6,0,0)));self.ret()
  elif address==0x3df6e0:self.ret(self.deliver(7,r(1),0))
  elif address==0x4c4bdc:self.ret(self.deliver(8,0,0))
  elif address==0x3d80b4:self.ret(self.deliver(9,0,0))
  elif address==0x3fc690:self.ret(self.deliver(10,0,0))
  elif address==0x320e44:self.ret(self.deliver(11,0,0))
  elif address==0x413a7c:
   text=bytearray();p=r(1)
   while (b:=c.uc.mem_read(p,1)[0]):text.append(b);p+=1
   self.keys[r(0)]=text.decode();c.uc.mem_write(r(0),bytes(20));self.ret(r(0))
  elif address==0x797124:self.ret()
  elif address==0x797250:self.returned=r(1);self.ret()
 def run(self,raw):
  c=self.c;v=struct.unpack('<71I',raw);nargs,self.index,self.remote,self.present,active,removed=v[:6];self.slots=list(map(sint,v[6:9]));self.usable=list(map(sint,v[9:12]));self.levels=list(map(sint,v[12:15]));self.fractions=v[15:19];self.meta=list(map(sint,v[19:27]));self.mutation=v[26];sheet=words(*v[27:]);self.events=[];self.keys={};self.returned=0;self.divide_index=36
  if self.native:
   c.uc.mem_write(self.sheet,sheet);c.uc.mem_write(self.actor,struct.pack('<QIBBHQ Q',self.sheet,44,active,removed,0,1,0));c.uc.mem_write(self.input,struct.pack('<QiIII',1,sint(self.index),self.remote,nargs,0));c.uc.mem_write(self.services,struct.pack('<QQ',0,c.callback+32));result=c.invoke('dh2_ui_hud_player_infos_v1',[self.input,self.services]);assert result==0,result
  else:
   c.uc.mem_write(self.actor+0xff8,sheet);c.uc.mem_write(self.actor+0x80,bytes((active,removed)));c.pointer(self.env,self.args);c.uc.mem_write(self.args,bytes(48));c.uc.mem_write(self.args+12+1,bytes((5,)));c.pointer(self.args+12+4,self.object);c.uc.mem_write(self.fn,bytes(24));c.pointer(self.fn,self.env+0x200);c.pointer(self.fn+12,self.env);c.pointer(self.fn+16,nargs);c.pointer(self.fn+20,1);c.invoke(0x44e5cc,[self.fn]);assert self.returned==(self.object if nargs in(2,3) else 0)
  return self.sheetbytes(),tuple(self.events)
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--gold',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args();manifest=json.loads((ROOT/'reference/hud-player-infos/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256'];old=Machine(a.engine,False,manifest);new=Machine(a.library,True,{'functions':[]});rng=random.Random(20261005);records=[];calls=0;zero_cases=0
 for i in range(520):
  v=[rng.choice((0,1,2,3,4,0xffffffff)) if i<25 else rng.choice((2,3)),rng.randrange(-4,5),rng.randrange(2),rng.randrange(2) if i<100 else 1,rng.choice((0,1,255)) if i<150 else 1,rng.choice((0,1,255)) if i<100 else 0]
  slots=[rng.choice((-1,7)),rng.choice((-1,9)),rng.choice((-1,11))];v+=slots+[rng.choice((0,1,255,-1)) for _ in range(3)]+[rng.randrange(-5,9) for _ in range(3)]+[float_bits(rng.choice((0.,1.,.375,-.01,3.,math.nan,math.inf,-math.inf,21474836.))) for _ in range(4)]+[rng.choice((-2147483648,-1,0,1,29,2147483647)) for _ in range(7)]+[rng.randrange(1<<17)]
  sheet=[rng.randrange(-2147483648,2147483648) for _ in range(44)]
  for cur,maxi in ((36,38),(41,43),(33,34)):sheet[cur]=rng.choice((-2147483648,-101,-1,0,1,50,101,2147483647));sheet[maxi]=rng.choice((-2147483648,-1,0,1,100,2147483647))
  raw=words(*v,*sheet);expected,events=old.run(raw);actual,observed=new.run(raw);assert expected==actual,(i,'state');assert events==observed,(i,events,observed);calls+=len(events);zero_cases+=int(any(struct.unpack_from('<I',x)[0]==13 for x in events));records.append(words(len(raw),len(expected),len(events))+raw+expected+b''.join(events))
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(words(0x31494850,len(records))+b''.join(records));report=dict(validation='PASS',comparisons=len(records),ordered_services=calls,explicit_zero_runtime_cases=zero_cases,mismatches=0,original_sha256=sha(a.engine),arm64_library_sha256=sha(a.library),gold_sha256=sha(a.gold),source_sha256={p.name:sha(p) for p in (ROOT/'hud_player_infos.hpp',ROOT/'hud_player_infos.cpp')},scope=__doc__);a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
