"""Original FastUpdate status instruction region and full RenderFX GotoFrame
versus O2 ARM64. Cached clip services, sprite virtual operations and the imported
integer division runtime are explicit caller projections. Sprite timeline tags,
ActionScript scheduling, potion/skill prefix/tail and full manager are outside.
Zero-divisor cases compare an explicit runtime result, not a recovered handler.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../level-world/tests'))
from visual_timeline_differential import TimelineCpu
from navigation_search_differential import word
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def sint(v):return v if v<0x80000000 else v-0x100000000
class Machine:
 def __init__(self,path,native,manifest):
  self.c=TimelineCpu(path,native,manifest);self.native=native;c=self.c;d=c.data
  self.state=d+0x1000;self.owner=d+0x2000;self.actor=d+0x4000;self.sheet=d+0x6000;self.service=d+0x7000;self.vtable=d+0x8000;self.clips=[d+0x9000+i*0x200 for i in range(5)];c.handler=self.callback;c.uc.hook_add(UC_HOOK_CODE,self.hook)
  if not native:
   for clip in self.clips:c.pointer(clip,self.vtable)
   for off,delta in ((8,32),(0x14c,48),(0x94,64)):c.pointer(self.vtable+off,c.callback+delta)
 def ret(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def fx(self):return word(self.c,self.owner+0x57c) if not self.native else struct.unpack('<Q',self.c.uc.mem_read(self.state+16,8))[0]
 def setfx(self,v):self.c.uc.mem_write(self.state+16,struct.pack('<Q',v)) if self.native else self.c.pointer(self.owner+0x57c,v)
 def event(self,op,index,value=0,other=0,fx=0,clip=0):self.events.append(struct.pack('<4I2Q',op,index,value&0xffffffff,other&0xffffffff,fx,clip))
 def resolve(self,index):
  self.active_index=index;self.captured_fx=self.fx();self.event(1,index,fx=self.captured_fx)
  if self.mutation&(1<<index):
   self.setfx(self.fx()+1);self.c.uc.mem_write(self.sheet+36*4 if self.native else self.actor+0x1088,words(987654321))
  return self.clips[index] if self.mask&(1<<index) else 0
 def divide(self,num,den):
  index=self.divisions;self.divisions+=1
  if not den:self.event(5,index,num);return self.zero
  a,b=sint(num),sint(den);return (abs(a)//abs(b)*(-1 if (a<0)!=(b<0) else 1))&0xffffffff
 def callback(self,address):
  c=self.c
  if self.native:
   op,index,value,other,fx,clip=struct.unpack('<4I2Q',c.uc.mem_read(c.reg(2),32));out=c.reg(3)
   if op==1:result=self.resolve(index);c.uc.mem_write(out,struct.pack('<QiI',result,0,0))
   else:
    self.event(op,index,value,other,fx,self.clips.index(clip)+1 if clip else 0)
    if op==2:c.uc.mem_write(out,struct.pack('<QiI',0,int(bool(self.types&(1<<index))),0))
    elif op==5:c.uc.mem_write(out,struct.pack('<QII',0,self.zero,0))
   c.put(0,1);return
  index=self.clips.index(c.reg(0));op={32:2,48:3,64:4}[address-c.callback]
  self.event(op,index,c.reg(1),fx=self.captured_fx,clip=index+1)
  c.put(0,int(bool(self.types&(1<<index))) if op==2 else 777)
 def hook(self,uc,address,size,unused):
  if self.native:return
  c=self.c
  if address==0x41e208:c.put(0,0);uc.reg_write(c.pc,c.stop)
  elif address==0x427d50:
   index={0xc:0,0x9c:1,0xcc:2,0x3c:3,0x6c:4}[c.reg(0)-self.owner];self.ret(self.resolve(index))
  elif address==0x30e2a4:self.ret(self.divide(c.reg(0),c.reg(1)))
 def run(self,raw):
  c=self.c;v=struct.unpack('<54I',raw);op=v[0];sheet=words(*v[1:45]);fx,self.mask,self.types,self.mutation,self.zero,frame,play,index,present=v[45:];self.events=[];self.divisions=0;self.active_index=index;self.captured_fx=fx
  if self.native:
   c.uc.mem_write(self.sheet,sheet);c.uc.mem_write(self.state,struct.pack('<QIIQ',self.sheet,44,0,fx));c.uc.mem_write(self.service,struct.pack('<QQ',0,c.callback+32))
   args=[self.state,self.service] if op==0 else [self.state,index,fx,self.clips[index] if present else 0,frame,play,self.service]
   result=c.invoke('dh2_ui_hud_player_values' if op==0 else 'dh2_ui_hud_goto_frame',args)
   assert result==0,result;out=bytes(c.uc.mem_read(self.sheet,176))+struct.pack('<Q',self.fx())
  else:
   c.uc.mem_write(self.actor+0xff8,sheet);c.pointer(self.owner+0x57c,fx)
   if op==0:c.put(4,self.owner);c.put(6,self.actor);c.invoke(0x41e0f4,[])
   else:c.invoke(0x7a7d34,[fx,self.clips[index] if present else 0,frame,play])
   out=bytes(c.uc.mem_read(self.actor+0xff8,176))+struct.pack('<Q',self.fx())
  return out,tuple(self.events)
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--gold',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args();manifest=json.loads((ROOT/'reference/hud-player-values/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256'];old=Machine(a.engine,False,manifest);new=Machine(a.library,True,{'functions':[]});rng=random.Random(20261004);records=[];calls=0;zero_cases=0
 def compare(op,sheet,fx,mask,types,mutate,zero,frame=0,play=0,index=0,present=1):
  nonlocal calls,zero_cases
  raw=words(op,*sheet,fx,mask,types,mutate,zero,frame,play,index,present);expected,events=old.run(raw);actual,observed=new.run(raw)
  assert expected==actual,(len(records),'state');assert events==observed,(len(records),events,observed);calls+=len(events);zero_cases+=int(any(struct.unpack_from('<I',x)[0]==5 for x in events));records.append(words(len(raw),len(expected),len(events))+raw+expected+b''.join(events))
 for i in range(1200):
  sheet=[rng.randrange(-0x80000000,0x80000000) for _ in range(44)]
  for cur,maxi in ((36,38),(41,43),(33,34)):
   sheet[cur]=rng.choice((-2147483648,-1000,-1,0,1,49,50,99,100,101,2147483647,rng.randrange(-100000000,100000000)));sheet[maxi]=rng.choice((-2147483648,-101,-100,-1,0,1,2,100,101,2147483647))
  compare(0,sheet,1,rng.randrange(32),rng.randrange(32),rng.randrange(32),rng.choice((-2147483648,-1,0,2147483647)))
 # Observable boundary frames: 0/1/99/100%, negative MP/XP, signed overflow.
 for hp in (-1,0,1,50,99,100,101):
  for mp in (-1,0,1,99,100,101):
   s=[0]*44;s[36]=hp;s[38]=100;s[41]=mp;s[43]=100;s[33]=mp;s[34]=100;compare(0,s,7,31,31,0,0)
 for i in range(320):compare(1,[0]*44,13,31,(i//5)%32,0,0,rng.choice((-2147483648,-1,0,99,100,2147483647)),rng.choice((0,1,2,0xffffffff)),i%5,i%3!=0)
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(words(0x31564448,len(records))+b''.join(records));report=dict(validation='PASS',comparisons=len(records),ordered_services=calls,explicit_zero_runtime_cases=zero_cases,mismatches=0,original_sha256=sha(a.engine),arm64_library_sha256=sha(a.library),gold_sha256=sha(a.gold),source_sha256={p.name:sha(p) for p in (ROOT/'hud_player_values.hpp',ROOT/'hud_player_values.cpp')},scope=__doc__);a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
