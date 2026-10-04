"""Actual root/sprite advance instructions against O2 native field coordinators.

Movie/AS/tag/listener/GC/drag services are explicit and ordered, not replaced
by stock root.advance. Deferred frame-tag/AS interpretation is a separate core
connection audit; this oracle proves the corpus field branches and live rereads.
"""
import argparse,json,math,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_S0,UC_ARM64_REG_S1
from swf_cursor_input_differential import InputCpu,f32,words,floats,sha
from body_transform_differential import equal
ROOT=Path(__file__).resolve().parents[1]
class Cpu(InputCpu):
 def external(self,uc,a,size,unused):
  name=self.imports.get(a)
  if name=='fmodf':
   x,y=floats(words(uc.reg_read(UC_ARM64_REG_S0),uc.reg_read(UC_ARM64_REG_S1)))if self.arm64 else floats(words(self.reg(0),self.reg(1)))
   v=f32(math.fmod(x,y))if math.isfinite(x)and y!=0 else math.nan
   if self.arm64:uc.reg_write(UC_ARM64_REG_S0,struct.unpack('<I',struct.pack('<f',v))[0])
   else:self.put(0,struct.unpack('<I',struct.pack('<f',v))[0])
   uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,a,size,unused)
class Machine:
 def __init__(self,p,native):
  self.c=c=Cpu(p,native,{'functions':[]});self.native=native;d=c.data;self.s=d+0x1000;self.movie=d+0x2000;self.player=d+0x3000;self.vt=d+0x4000;self.services=d+0x5000;self.def_=d+0x6000;self.list=d+0x7000;self.scratch=d+0x9000
  c.handler=self.callback;c.uc.hook_add(UC_HOOK_CODE,self.hook)
  if native:c.uc.mem_write(self.services,struct.pack('<QQ',0,c.callback+32))
  else:
   c.pointer(self.movie,self.vt);c.pointer(self.s,self.vt);c.pointer(self.def_,self.vt+0x200)
   for off,cb in((8,48),(0x148,64),(0x5c,80),(0x2c,96),(0x44,112),(0x58,128),(0xc8,144)):c.pointer(self.vt+off,c.callback+cb)
   c.pointer(self.vt+0x200+0x38,c.callback+160);c.pointer(self.vt+0x200+0x50,c.callback+176)
 def trace(self,op,value=0,payload=b''):self.events.append(words(op,value)+payload)
 def root_mutate(self,op):
  if self.mutation==1 and op==2:self.loaded(1)
  if self.mutation==2 and op==7:self.loaded(0)
  if self.mutation==3 and op==8:self.float_field(1,0.2)
 def loaded(self,v):self.c.uc.mem_write(self.s+(12 if self.native else 0x84),bytes([v]))
 def float_field(self,i,v):self.c.uc.mem_write(self.s+(i*4 if self.native else 0x8c+i*4),struct.pack('<f',v))
 def sprite_event(self,value):
  self.trace(2,value)
  if self.mutation==1 and value==10:
   self.c.uc.mem_write(self.s+(24 if self.native else 0xec),b'\1');self.c.uc.mem_write(self.s+(25 if self.native else 0x9b),b'\0')
  if self.mutation==2 and value==12:self.c.uc.mem_write(self.s+(27 if self.native else 0xe9),b'\0')
 def queued(self):return struct.unpack('<I',self.c.uc.mem_read(self.s+(40 if self.native else 0xd0),4))[0]
 def queue(self,values):
  c=self.c;c.uc.mem_write(self.list,b''.join(struct.pack('<Q'if self.native else'<I',x)for x in values));c.uc.mem_write(self.s+(40 if self.native else 0xd0),words(len(values)))
 def execute(self,values):
  self.trace(4,len(values),words(*values));self.executions+=1
  if self.executions<self.requeue:self.queue([700+self.executions])
 def callback(self,a):
  c=self.c;cb=a-c.callback
  if self.native:
   q=c.reg(2);op=struct.unpack('<I',c.uc.mem_read(q,4))[0];delta=bytes(c.uc.mem_read(q+4,4))
   if self.mode=='root':
    self.trace(op,0,delta if op in(2,7)else b'');self.root_mutate(op);c.put(0,1)
   else:
    frame,value,actions,count=struct.unpack('<iiQI',c.uc.mem_read(q+8,20));out=c.reg(3)
    if op==2:self.sprite_event(value)
    elif op==4:self.execute(list(struct.unpack('<'+'Q'*count,c.uc.mem_read(actions,count*8))))
    elif op==5:self.trace(5);c.uc.mem_write(out,words(self.frames))
    elif op==7:self.trace(7,frame)
    elif op==9:self.trace(9,0,delta);c.uc.mem_write(out,words(self.children_need))
    else:self.trace(op)
    c.put(0,1)
   return
  if self.mode=='root':
   if cb==48:c.put(0,1);return
   op={64:6,80:7,96:8,112:11}[cb];self.trace(op,0,words(c.reg(1))if op==7 else b'');self.root_mutate(op);return
  if cb==64:self.trace(1)
  elif cb==96:self.sprite_event(c.uc.mem_read(c.reg(1),1)[0])
  elif cb==128:c.put(0,self.s+0x100)
  elif cb==144:self.trace(7,c.reg(1))
  elif cb==160:self.trace(5);c.put(0,self.frames)
  elif cb==176:c.put(0,self.scratch+0x300)
  else:raise AssertionError(cb)
 def hook(self,uc,a,size,unused):
  if self.native:return
  c=self.c;op=None
  if self.mode=='root':
   op={0x773d38:1,0x760d58:2,0x7b7898:3,0x774660:4,0x78069c:5,0x76c808:9,0x760940:10,0x76d2f8:12}.get(a)
   if op:self.trace(op,0,words(c.reg(1))if op==2 else b'');self.root_mutate(op)
  else:
   if a==0x75e3a8:self.trace(3);op=3
   elif a==0x75b810:
    p=c.reg(1);base,n=struct.unpack('<II',c.uc.mem_read(p,8));values=list(struct.unpack('<'+'I'*n,c.uc.mem_read(base,n*4)));self.execute(values);op=4
   elif a in(0x75647c,0x7563f8):self.trace(6);op=6
   elif a==0x781eac:self.trace(8);op=8
   elif a==0x755908:self.trace(9,0,words(c.reg(1)));c.put(0,self.children_need);op=9
   elif a==0x7611f0:self.trace(10);op=10
   # Fixture source array resize, only requests<=32 fit existing local storage.
   elif a==0x77e7f8:
    p=c.reg(0);base=struct.unpack('<I',c.uc.mem_read(p,4))[0];n=c.reg(1);assert n<=32;c.uc.mem_write(p+8,words(n));op=-1
  if op is not None:uc.reg_write(c.pc,uc.reg_read(c.lr))
 def run_root(self,row):
  self.mode='root';c=self.c;self.events=[];self.mutation=row['mutation'];r,f,g,loaded,delta,catch_up=row['fields']
  if self.native:c.uc.mem_write(self.s,struct.pack('<3fB3xQQ',r,f,g,loaded,self.movie,self.player));c.uc.reg_write(UC_ARM64_REG_S0,struct.unpack('<I',struct.pack('<f',delta))[0]);result=c.invoke('dh2_ui_swf_root_frame',[self.s,catch_up,self.services])
  else:
   c.uc.mem_write(self.s+0x84,bytes([loaded]));c.uc.mem_write(self.s+0x8c,struct.pack('<3f',r,f,g));c.pointer(self.s+0x10,self.movie);c.pointer(self.s+0xcc,self.player);c.pointer(self.s+0xc8,self.player+0x200);c.uc.mem_write(self.player+0x204,b'\1');result=c.invoke(0x775304,[self.s,struct.unpack('<I',struct.pack('<f',delta))[0],catch_up])
  out=bytes(c.uc.mem_read(self.s,12))+bytes(c.uc.mem_read(self.s+12,1))if self.native else bytes(c.uc.mem_read(self.s+0x8c,12))+bytes(c.uc.mem_read(self.s+0x84,1))
  if self.native:assert result==0,result
  return out,self.events
 def run_sprite(self,row):
  self.mode='sprite';c=self.c;self.events=[];self.mutation=row['mutation'];self.frames=row['frames'];self.requeue=row['requeue'];self.executions=0;self.children_need=row['children'];current,play,loaded,visible,need,enter=row['fields'];values=row['goto'];delta=row['delta']
  if self.native:c.uc.mem_write(self.s,struct.pack('<QQii4BIQIIQII',self.s,self.def_,current,play,loaded,visible,need,enter,0,self.list,len(values),32,self.scratch,0,32));self.queue(values);c.uc.reg_write(UC_ARM64_REG_S0,struct.unpack('<I',struct.pack('<f',delta))[0]);result=c.invoke('dh2_ui_swf_sprite_frame',[self.s,self.services])
  else:
   c.pointer(self.s,self.vt);c.pointer(self.s+0xa0,self.def_);c.uc.mem_write(self.s+0xe4,struct.pack('<hbbBBB',current,play,0,1,enter,0));c.uc.mem_write(self.s+0xec,bytes([loaded]));c.uc.mem_write(self.s+0x9b,bytes([visible,0,need]));c.pointer(self.s+0xcc,self.list);c.uc.mem_write(self.s+0xd4,words(32));c.uc.mem_write(self.scratch+0x300,words(self.list+0x300,0,0));self.queue(values);result=c.invoke(0x78240c,[self.s,struct.unpack('<I',struct.pack('<f',delta))[0]])
  if self.native:
   current,play=struct.unpack('<ii',c.uc.mem_read(self.s+16,8));loaded,visible,need,enter=c.uc.mem_read(self.s+24,4)
  else:current=struct.unpack('<h',c.uc.mem_read(self.s+0xe4,2))[0];play=struct.unpack('<b',c.uc.mem_read(self.s+0xe6,1))[0];loaded=c.uc.mem_read(self.s+0xec,1)[0];visible=c.uc.mem_read(self.s+0x9b,1)[0];need=c.uc.mem_read(self.s+0x9d,1)[0];enter=c.uc.mem_read(self.s+0xe9,1)[0]
  n=self.queued();values=list(struct.unpack('<'+('Q'if self.native else'I')*n,c.uc.mem_read(self.list,n*(8 if self.native else 4))))
  if self.native:assert result==0,result
  return words(current,play,loaded,visible,need,enter,n,*values),self.events
def main():
 p=argparse.ArgumentParser();[p.add_argument('--'+k,type=Path,required=True)for k in('engine','library','gold','report')];a=p.parse_args();old=Machine(a.engine,False);new=Machine(a.library,True);rng=random.Random(20261005);records=[];services=0
 for k in range(2400):
  if k<1200:
   row={'mode':'root','mutation':k%4,'fields':[rng.choice([0.,1.,-.3,.08]),rng.choice([1/30,.1,.2]),rng.choice([0.,2.,.01,-.2]),k%2,rng.choice([0.,.016,.5,-.03]),k%2]};e,t=old.run_root(row);v,c=new.run_root(row)
  else:
   frames=rng.randrange(1,12);row={'mode':'sprite','mutation':k%3,'fields':[rng.randrange(frames),rng.choice([0,1,-1]),k%2,(k//2)%2,rng.randrange(2),(k//3)%2],'frames':frames,'goto':list(range(100,100+rng.randrange(6))),'requeue':rng.choice([0,1,2,3,12,13]),'children':k%3==0,'delta':rng.choice([0.,.016,-.1,.1])};e,t=old.run_sprite(row);v,c=new.run_sprite(row)
  assert equal(e[:12],v[:12])and e[12:]==v[12:] if row['mode']=='root' else e==v,(k,row,e.hex(),v.hex());assert t==c,(k,row,[x.hex()for x in t],[x.hex()for x in c]);records.append({'input':row,'state':e.hex(),'trace':[x.hex()for x in t]});services+=len(t)
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_text(json.dumps(records,separators=(',',':'))+'\n');report={'validation':'PASS','comparisons':len(records),'root_comparisons':1200,'sprite_comparisons':1200,'ordered_services':services,'original_sha256':sha(a.engine),'arm64_library_sha256':sha(a.library),'gold_sha256':sha(a.gold),'mismatches':0,'source_sha256':{str(x.relative_to(ROOT.parents[1])).replace('\\','/'):sha(x)for x in[ROOT/'swf_frame_schedule.hpp',ROOT/'swf_frame_schedule.cpp',Path(__file__)]},'scope':__doc__};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()

