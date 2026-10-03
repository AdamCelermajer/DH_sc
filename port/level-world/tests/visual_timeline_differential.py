"""Actual CTimelineController instructions versus native ARM64 reconstruction.

Imported AEABI/fmod arithmetic is the same dependency model on both sides.
Callbacks inspect transitional state and can execute actual reentrant methods.
Clip bound virtuals receive explicit authored-bound fixtures.
"""
import argparse,hashlib,json,math,random,struct,time
from pathlib import Path
from unicorn.arm64_const import UC_ARM64_REG_S0,UC_ARM64_REG_S1
from navigation_differential import ROOT,equal
from subobjects_update_differential import UpdateCpu
from navigation_search_differential import word,words
from aggro_differential import float_bits
from combat_result_differential import floating

def integer(f):return 0 if math.isnan(f) else max(-2147483648,min(2147483647,int(f))) if math.isfinite(f) else 2147483647 if f>0 else -2147483648
class TimelineCpu(UpdateCpu):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='fmodf':
   a,b=(floating(uc.reg_read(UC_ARM64_REG_S0)),floating(uc.reg_read(UC_ARM64_REG_S1))) if self.arm64 else (floating(self.reg(0)),floating(self.reg(1)))
   try:v=math.fmod(a,b)
   except ValueError:v=math.nan
   bits=float_bits(v)
   if self.arm64:uc.reg_write(UC_ARM64_REG_S0,bits)
   else:self.put(0,bits)
  elif name=='__aeabi_f2iz':self.put(0,integer(floating(self.reg(0))))
  else:return super().external(uc,address,size,unused)
  self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))

class Machine:
 def __init__(self,path,native,manifest):
  self.c=TimelineCpu(path,native,manifest);c=self.c;self.native=native;d=c.data;self.s=d+0x1000;self.comp=d+0x2000;self.vt=d+0x3000;self.services=d+0x4000;self.helper=d+0x5000;self.events=[];c.handler=self.callback
  if not native:
   c.pointer(self.vt+0x2c,c.callback+48);c.pointer(self.vt+0x30,c.callback+64);c.pointer(self.vt+0xc,0x666f10)
 def snapshot(self):
  c=self.c
  if self.native:return bytes(c.uc.mem_read(self.s,56))
  raw=bytes(c.uc.mem_read(self.s+4,4))+bytes(c.uc.mem_read(self.s+0x10,8))+words(c.uc.mem_read(self.s+0x18,1)[0])+bytes(c.uc.mem_read(self.s+0x1c,24))+words(bool(word(c,self.s+0x34)))+bytes(c.uc.mem_read(self.s+0x38,4))+words(c.uc.mem_read(self.s+0x3c,1)[0],c.uc.mem_read(self.s+0x3d,1)[0]);assert len(raw)==56;return raw
 def completion(self):
  c=self.c
  return bytes(c.uc.mem_read(self.comp,8)) if self.native else bytes(c.uc.mem_read(self.comp+0x68,4))+words(c.uc.mem_read(self.comp+0x88,1)[0])
 def configure(self,raw,params,completion,callbacks):
  c=self.c;self.params=params;self.events=[]
  if self.native:c.uc.mem_write(self.s,raw);c.uc.mem_write(self.comp,completion);c.uc.mem_write(self.services,struct.pack('<QQ',0,c.callback+32 if callbacks else 0))
  else:
   c.uc.mem_write(self.s,bytes(0x80));c.pointer(self.s,self.vt);c.uc.mem_write(self.s+4,raw[:4]);c.pointer(self.s+8,c.callback+32 if callbacks else 0);c.uc.mem_write(self.s+0x10,raw[4:12]);c.uc.mem_write(self.s+0x18,bytes((word_bytes(raw,12),)));c.uc.mem_write(self.s+0x1c,raw[16:40]);c.pointer(self.s+0x34,123 if word_bytes(raw,40) else 0);c.uc.mem_write(self.s+0x38,raw[44:48]);c.uc.mem_write(self.s+0x3c,bytes((word_bytes(raw,48),word_bytes(raw,52))));c.uc.mem_write(self.comp+0x68,completion[:4]);c.uc.mem_write(self.comp+0x88,bytes((word_bytes(completion,4),)))
 def callback(self,address):
  c=self.c
  if address in (c.callback+48,c.callback+64):c.put(0,self.params[1 if address==c.callback+48 else 2]);return
  assert address==c.callback+32;self.events.append(self.snapshot()+self.completion());action=self.params[2]
  if action==0:return
  if self.native:
   if action==1:c.put(0,self.comp);c.put(1,self.s);target='dh2_timeline_notify'
   elif action==2:c.put(0,self.s);c.put(1,self.params[1]);target='dh2_timeline_jump'
   elif action==3:c.put(0,self.s);c.put(1,self.params[1]);c.put(2,self.params[3]);c.put(3,1);target='dh2_timeline_range'
   elif action==4:c.put(0,self.s);c.uc.reg_write(UC_ARM64_REG_S0,self.params[1]);target='dh2_timeline_scale'
   else:raise AssertionError(action)
   c.uc.reg_write(c.pc,c.symbols[target])
  else:
   if action==1:c.put(0,self.comp);c.put(1,self.s);target=0x366224
   elif action==2:c.put(0,self.s);c.put(1,self.params[1]);target=0x666f10
   elif action==3:c.put(0,self.s);c.put(1,self.params[1]);c.put(2,self.params[3]);c.put(3,1);target=0x666c38
   elif action==4:c.put(0,self.s);c.put(1,self.params[1]);target=0x666c20
   else:raise AssertionError(action)
   c.uc.reg_write(c.pc,target)
 def execute(self,op):
  c=self.c;p=self.params
  if self.native:
   names=('dh2_timeline_update','dh2_timeline_jump','dh2_timeline_init','dh2_timeline_range','dh2_timeline_clip','dh2_timeline_loop','dh2_timeline_scale','dh2_timeline_notify','dh2_timeline_extra')
   args=([self.s,p[0],self.services],[self.s,p[0]],[self.s,p[0],p[1]],[self.s,p[0],p[1],p[2]],[self.s,p[0],p[1],p[2]],[self.s,p[0]],[self.s],[self.comp,self.s if p[3] else 0],[self.comp,self.s if p[3] else 0])[op]
   if op==6:c.uc.reg_write(UC_ARM64_REG_S0,p[0])
   assert c.invoke(names[op],args)==0
  else:
   addresses=(0x667104,0x666f10,0x666f04,0x666c38,0x666f7c,0x666c28,0x666c20,0x366224,0x3c90f8)
   args=([self.s,p[0]],[self.s,p[0]],[self.s,p[0],p[1]],[self.s,p[0],p[1],p[2]],[self.s,p[0]],[self.s,p[0]],[self.s,p[0]],[self.comp,self.s if p[3] else 0],[self.comp,self.s if p[3] else 0])[op]
   if op==8:c.uc.mem_write(self.comp+0x44,bytes(c.uc.mem_read(self.comp+0x68,4)))
   c.invoke(addresses[op],args)
   if op==8:c.uc.mem_write(self.comp+0x68,bytes(c.uc.mem_read(self.comp+0x44,4)))
  return self.snapshot()+self.completion(),tuple(self.events)

# Base callback dispatcher normally restores PC to LR. Reentrant original and
# native methods replace PC explicitly; preserve that branch after dispatch.
class ReentrantCpu(TimelineCpu):
 def external(self,uc,address,size,unused):
  if self.callback+32<=address<=self.callback+240:
   self.handler(address)
   if uc.reg_read(self.pc)==address:uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)
TimelineCpu=ReentrantCpu
def word_bytes(raw,offset):return struct.unpack_from('<I',raw,offset)[0]
def state(start=0,end=800,current=0,loop=0,scale=1.,last=0.,initialized=0,ended=0,library=0):return struct.pack('<3iI6fIi2I',current,start,end,loop,0.,float(start)/1000,float(end-start)/1000,last,float(current)/1000,scale,library,17,ended,initialized)
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);a=p.parse_args();started=time.monotonic();manifest=json.loads((ROOT/'reference/visual-timeline/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];old=Machine(a.engine,False,manifest);new=Machine(a.library,True,{'functions':[]});records=[];rng=random.Random(20261016);counts=[0]*9;callbacks=0
 def compare(op,s,params,comp=struct.pack('<iI',13,0),cb=0):
  nonlocal callbacks
  params=tuple(params);old.configure(s,params,comp,cb);new.configure(s,params,comp,cb);expected,events=old.execute(op);actual,calls=new.execute(op);assert equal(expected,actual),(len(records),op,s.hex(),params,expected.hex(),actual.hex());assert len(events)==len(calls) and all(equal(x,y) for x,y in zip(events,calls)),(len(records),'callback',events,calls);counts[op]+=1;callbacks+=len(events);inp=s+words(*(v&0xffffffff for v in params))+comp+words(cb);records.append(words(op,len(inp),len(expected),len(events))+inp+expected+b''.join(events));return expected[:56],expected[56:]
 for i in range(600):
  start=rng.randrange(-2000,2000);end=start+rng.randrange(-10,4000);current=rng.randrange(start-300,end+500) if end>start-300 else start;s=state(start,end,current,rng.choice((0,1,255)),rng.choice((0.,1.,-1.,.5,1.375,100.)),rng.uniform(-2,2),i%2,i%7==0,i%3==0)
  for op,params in ((0,(rng.randrange(-200000,200000),rng.randrange(-1000,1000),i%5,rng.randrange(0,3000))),(1,(rng.randrange(-100000,100000),0,0,0)),(2,(start-5,end+7,0,0)),(3,(start-11,end+13,i%2,0)),(4,(rng.randrange(-100,100),start,end,0)),(5,(i%256,0,0,0)),(6,(float_bits(rng.choice((0.,1.,-1.,.25,137.))),0,0,0)),(7,(0,0,0,i%3!=0)),(8,(0,0,0,i%3!=0))):compare(op,s,params,cb=op==0)
 # Stateful absolute-time traces: boundaries, backwards time, scale changes,
 # multi-period wraps, initial baseline and callback mutation/reentrancy.
 for loop in (0,1):
  for scale in (1.,-1.,0.,1.375):
   for action in range(5):
    s=state(loop=loop,scale=scale);comp=struct.pack('<iI',17,0)
    for ms in (0,16,799,800,801,816,1600,1601,4001,3999,-1,0,2000):s,comp=compare(0,s,(ms,100,action,1200),comp,1)
 for start,end in ((0,0),(1000,0),(-2147483648,2147483647),(2147483647,-2147483648)):
  for loop in (0,1):
   for ms in (0,1,-1,2147483647,-2147483648):compare(0,state(start,end,0,loop,initialized=1),(ms,0,0,0),cb=1)
 # Exceptional inputs use original AEABI saturation and unordered comparisons.
 for value in (float('nan'),float('inf'),-float('inf'),-0.,1e-40):
  for loop in (0,1):compare(0,state(loop=loop,scale=value,initialized=1),(16,0,0,0),cb=1)
 blob=b'VTG1'+words(len(records))+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(blob);report={'original_sha256':manifest['original_sha256'],'native_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'corpus_sha256':hashlib.sha256(blob).hexdigest(),'comparisons':len(records),'operation_counts':dict(zip(('update','jump','init','range','clip','loop','scale','notify','extra'),counts)),'callback_snapshots':callbacks,'mismatches':0,'seconds':round(time.monotonic()-started,3),'scope':'Actual original timeline/AEABI instructions, explicit resolved clip-bound virtual fixtures. Reentrant callback methods execute actual instructions. Original Animator end notification and CharAnimator extra-time calculation execute. Full character sequence selection remains external.'};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
if __name__=='__main__':main()
