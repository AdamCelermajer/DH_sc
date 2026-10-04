"""Complete original goto_frame/set_play_state caller instructions vs O2 ARM64.
Frame tags/reverse helpers, renderer sound lookup, stream pause and dirty weak
ownership notification are required service projections, not implemented AS/GL.
Source pending action copies/append and STOP/notification ordering execute.
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
def sint(x):return x if x<0x80000000 else x-0x100000000
class Machine:
 def __init__(self,path,native,manifest):
  self.c=TimelineCpu(path,native,manifest);self.native=native;c=self.c;d=c.data;self.state=d+0x1000;self.definition=d+0x2000;self.vt=d+0x3000;self.dvt=d+0x4000;self.pending=d+0x5000;self.queued=d+0x6000;self.service=d+0x7000;self.sound=d+0x8000;self.svt=d+0x9000;self.trampoline=d+0xb000;c.handler=self.callback;c.uc.hook_add(UC_HOOK_CODE,self.hook);c.uc.mem_write(self.trampoline,words(0xd63f0200 if native else 0xe12fff3c,0))
  if not native:
   c.pointer(self.state,self.vt);c.pointer(self.vt+0xc8,c.callback+48);c.pointer(self.definition,self.dvt);c.pointer(self.dvt+0x38,c.callback+32);c.pointer(self.sound,self.svt);c.pointer(self.svt+0x38,c.callback+64)
 def ret(self,x=0):c=self.c;c.put(0,x);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def fields(self):
  c=self.c
  if self.native:return struct.unpack('<3iI',c.uc.mem_read(self.state,16))[:3],word(c,self.state+40),word(c,self.state+56)
  frame=struct.unpack('<h',c.uc.mem_read(self.state+0xe4,2))[0];play=struct.unpack('<b',c.uc.mem_read(self.state+0xe6,1))[0];return (frame,play,sint(word(c,self.definition+0x20))),word(c,self.state+0xc0),word(c,self.state+0xd0)
 def snapshot(self):
  fields,pending,queued=self.fields();return words(*fields,pending,queued,*[self.ptrword(self.pending,i) if i<pending else 0 for i in range(16)],*[self.ptrword(self.queued,i) if i<queued else 0 for i in range(16)])
 def ptrword(self,ptr,i):return struct.unpack('<Q' if self.native else '<I',self.c.uc.mem_read(ptr+i*self.c.word_size,self.c.word_size))[0]
 def setcount(self,pending,n):self.c.uc.mem_write(self.state+(40 if pending else 56) if self.native else self.state+(0xc0 if pending else 0xd0),words(n))
 def putaction(self,i,value):self.c.uc.mem_write(self.pending+i*self.c.word_size,struct.pack('<Q' if self.native else '<I',value&0xffffffff))
 def setframeplay(self,frame,play):self.c.uc.mem_write(self.state,struct.pack('<2i',frame,play)) if self.native else self.c.uc.mem_write(self.state+0xe4,struct.pack('<Hb',frame&65535,play))
 def event(self,op,frame=0,only=0,sound=0):self.events.append(struct.pack('<IIiiQQ',op,0,frame,only,1,sound)+self.snapshot())
 def tags(self,op,frame,only):
  self.event(op,frame,only);_,n,_=self.fields();flag=2 if op==3 and not only else 1 if op==3 else 4
  if self.flags&flag and n<8:self.putaction(n,0x10000+(frame&65535));self.setcount(True,n+1)
  if self.flags&8:self.setframeplay(-2,0)
 def callback(self,address):
  c=self.c
  if self.native:
   op,_,frame,only,clip,sound=struct.unpack('<IIiiQQ',c.uc.mem_read(c.reg(2),32));assert clip==1
   if op in(2,3):self.tags(op,frame,only)
   else:
    self.event(op,frame,only,sound)
    if op==1:
     if self.flags&16:self.setframeplay(-1,2)
     c.uc.mem_write(c.reg(3),struct.pack('<QiI',0,self.frame_count,0))
    elif op==5:c.uc.mem_write(c.reg(3),struct.pack('<QiI',2 if self.has_sound else 0,0,0))
   c.put(0,1)
   if op==3:self.nest()
   return
  delta=address-c.callback
  if delta==32:
   self.event(1)
   if self.flags&16:self.setframeplay(-1,2)
   c.put(0,self.frame_count)
  elif delta==48:self.tags(3,sint(c.reg(1)),sint(c.reg(2)));c.put(0,0);self.nest()
  elif delta==64:self.event(6,sint(c.reg(1)),sint(c.reg(2)),2);c.put(0,0)
  else:raise AssertionError(address)
 def hook(self,uc,address,size,unused):
  if address==self.trampoline+4:
   uc.context_restore(self.saved_context);uc.reg_write(self.c.pc,self.saved_return);return
  if self.native:return
  c=self.c
  if address==0x77f390:self.tags(2,sint(c.reg(1)),0);self.ret()
  elif address==0x7750e8:self.event(4);self.ret()
  elif address==0x77cba0:self.event(5);self.ret(self.sound if self.has_sound else 0)
 def nest(self):
  if not self.flags&32 or self.nested:return
  self.nested=True;c=self.c;self.saved_return=c.uc.reg_read(c.lr);self.saved_context=c.uc.context_save();c.put(0,self.state);c.put(1,1)
  if self.native:c.put(2,self.service);c.put(16,c.symbols['dh2_ui_hud_sprite_goto_v1'])
  else:c.put(12,0x781a3c)
  c.uc.reg_write(c.lr,self.trampoline)
 def run(self,raw):
  c=self.c;v=struct.unpack('<42I',raw);op,current,play,stream,target,self.frame_count,self.has_sound,self.flags,n,q=v[:10];self.events=[];self.nested=False;pending=v[10:26];queued=v[26:42]
  c.uc.mem_write(self.pending,struct.pack('<' + ('Q' if self.native else 'I')*16,*pending));c.uc.mem_write(self.queued,struct.pack('<' + ('Q' if self.native else 'I')*16,*queued))
  if self.native:
   c.uc.mem_write(self.state,struct.pack('<3iI2QQIIQII',sint(current),sint(play),sint(stream),0,1,3,self.pending,n,16,self.queued,q,16));c.uc.mem_write(self.service,struct.pack('<QQ',0,c.callback+32));result=c.invoke('dh2_ui_hud_sprite_goto_v1' if op==0 else 'dh2_ui_hud_sprite_play_v1',[self.state,target,self.service])
  else:
   c.pointer(self.state+0xa0,self.definition);c.pointer(self.definition+0x20,stream);c.uc.mem_write(self.state+0xbc,words(self.pending,n,16,0,self.queued,q,16));c.uc.mem_write(self.state+0xe4,struct.pack('<HB',current&65535,play&255));result=c.invoke(0x781a3c if op==0 else 0x77fe10,[self.state,target]);result=result if op==0 else 0
  return words(result)+self.snapshot(),tuple(self.events)
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--gold',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args();manifest=json.loads((ROOT/'reference/hud-sprite-timeline/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256'];old=Machine(a.engine,False,manifest);new=Machine(a.library,True,{'functions':[]});rng=random.Random(20261004);records=[];calls=0
 def compare(op,current,play,stream,target,count,sound,flags,n,q):
  nonlocal calls
  raw=words(op,current,play,stream,target,count,sound,flags,n,q,*[100+i for i in range(16)],*[200+i for i in range(16)]);expected,events=old.run(raw);actual,observed=new.run(raw);assert expected==actual,(len(records),expected.hex(),actual.hex());assert events==observed,(len(records),[(x[:32].hex(),x[32:].hex()) for x in events],[(x[:32].hex(),x[32:].hex()) for x in observed]);calls+=len(events);records.append(words(len(raw),len(expected),len(events))+raw+expected+b''.join(events))
 for i in range(1100):
  count=rng.choice((0,1,20,100,101,103));current=rng.choice((-2,-1,0,1,19,49,99,102));target=rng.choice((-2147483648,-1,0,1,19,49,99,100,102,103,2147483647));compare(0,current,rng.choice((-128,-1,0,1,2,127)),rng.choice((-1,0,7)),target,count,i%2,rng.randrange(32),i%4,(i//4)%4)
 for i in range(500):compare(1,rng.randrange(103),rng.choice((-128,-1,0,1,2,127)),rng.choice((-2147483648,-1,0,7,2147483647)),rng.choice((-2147483648,-1,0,1,2,127,128,255,256,2147483647)),103,i%2,0,i%4,(i//4)%4)
 # Explicit valid traversals over real HP/MP/XP/Distress/Hurt frame counts,
 # with pending actions and old batch entries carried through the fork path.
 for count in (100,100,101,103,103):
  for target in (0,1,49,99,count-1):compare(0,0,0,-1,target,count,0,2,2,3);compare(0,count-1,0,-1,target,count,0,2,2,3)
 for i in range(80):compare(0,i%2,0,-1,49 if i%2 else 99,103,0,32|2,i%4,(i//4)%4)
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(words(0x31545348,len(records))+b''.join(records));report=dict(validation='PASS',comparisons=len(records),ordered_services=calls,mismatches=0,original_sha256=sha(a.engine),arm64_library_sha256=sha(a.library),gold_sha256=sha(a.gold),source_sha256={p.name:sha(p) for p in (ROOT/'hud_sprite_timeline.hpp',ROOT/'hud_sprite_timeline.cpp')},scope=__doc__);a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
