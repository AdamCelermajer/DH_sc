"""Original coupled root/camera instructions vs optimized native ARM64.

Renderer/driver projections and player-global Viewport publication are explicit
services. Actual original arithmetic/conversion/state branches execute; only
post-conversion AS rectangle construction/publication is observed as an endpoint.
"""
import argparse,hashlib,json,math,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../level-world/tests'))
from visual_timeline_differential import TimelineCpu
from navigation_differential import equal
from navigation_search_differential import word
class ViewportCpu(TimelineCpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='__aeabi_fdiv':
   a,b=struct.unpack('<2f',words(self.reg(0),self.reg(1)))
   value=math.nan if math.isnan(a) or math.isnan(b) or (b==0 and a==0) else math.copysign(math.inf,a*math.copysign(1,b)) if b==0 else a/b
   self.put(0,struct.unpack('<I',struct.pack('<f',value))[0]);self.import_calls['__aeabi_fdiv']=self.import_calls.get('__aeabi_fdiv',0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr));return
  super().external(uc,address,size,unused)
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Machine:
 def __init__(self,path,native,manifest):
  self.c=ViewportCpu(path,native,manifest);self.native=native;c=self.c;d=c.data;self.s=d+0x1000;self.movie=d+0x2000;self.camera=d+0x3000;self.service=d+0x4000;self.point=d+0x5000;self.params=d+0x6000;self.render=d+0x7000;self.vt=d+0x8000;self.ref=d+0x9000;self.obj=d+0xa000;c.handler=self.callback
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
  if not native:
   c.pointer(self.render,self.vt);c.pointer(self.vt+0xac,c.callback+32)
   for pc,lit,off in ((0x77560c,0x775d18,0x775d20),(0x773dd4,0x773f48,0x773f4c),(0x773f64,0x774120,0x774124)):
    table=(pc+word(c,lit))&0xffffffff;slot=word(c,table+word(c,off));c.pointer(slot,self.render)
   # Real driver pointer-chain projected from caller owner, no width substitution.
   table=(0x42cda8+word(c,0x42cf28))&0xffffffff;slot=table+word(c,0x42cf2c);chain=[d+0xb000+i*0x100 for i in range(5)];c.pointer(slot,chain[0]);c.pointer(chain[0]+0x10,chain[1]);c.pointer(chain[1]+0x10,chain[2]);c.pointer(chain[2]+0xcc,chain[3]+4);c.pointer(chain[3],chain[4]);self.driver=chain[4]
 def ret(self,value=0):self.c.put(0,value);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def event(self,op,values=(0,0,0,0),rect=bytes(16)):self.events.append(words(op,0,*values)+rect)
 def orientation(self):
  value=self.facts[min(self.queries,1)];self.queries+=1;self.event(1);return value
 def callback(self,address):
  c=self.c
  if not self.native:assert address==c.callback+32;c.put(0,self.orientation());return
  assert address==c.callback+32
  request=bytes(c.uc.mem_read(c.reg(2),40));op=struct.unpack_from('<I',request)[0];self.events.append(request)
  if op==1:
   value=self.facts[min(self.queries,1)];self.queries+=1;c.uc.mem_write(c.reg(3),words(value,0,0,0))
  elif op==2:c.uc.mem_write(c.reg(3),words(*self.facts[2:],0,0))
  c.put(0,1)
 def hook(self,uc,address,size,unused):
  if self.native:return
  c=self.c
  if address==0x775844:
   sp=uc.reg_read(c.sp);rect=bytes(uc.mem_read(sp+0x60,8))+bytes(uc.mem_read(sp+0x58,8));self.event(3,rect=rect);uc.reg_write(c.pc,0x775a9c)
  elif address==0x42ce28:self.event(2)
  elif address==0x77520c:
   sp=uc.reg_read(c.sp);rect=b''.join(bytes(uc.mem_read(sp+off,4)) for off in (0x24,0x1c,0x28,0x20));uc.mem_write(self.point,rect);uc.reg_write(c.pc,0x7752cc)
  elif address in (0x7a9bac,0x7a9b30):
   vals=[c.reg(i) for i in (1,2,3)]+[word(c,uc.reg_read(c.sp))];self.event(4 if address==0x7a9bac else 5,vals);self.ret()
 def load(self,raw):
  c=self.c;op=word_bytes(raw,0);state=raw[4:68];cam=raw[68:108];params=raw[108:124];point=raw[124:132];self.facts=struct.unpack('<4i',raw[132:148]);self.events=[];self.queries=0
  c.uc.mem_write(self.params,params);c.uc.mem_write(self.point,point+bytes(8))
  if self.native:c.uc.mem_write(self.s,state);c.uc.mem_write(self.camera,cam);c.uc.mem_write(self.service,struct.pack('<QQ',0,c.callback+32))
  else:
   c.uc.mem_write(self.s,bytes(0x100));c.pointer(self.s+12,self.movie);c.uc.mem_write(self.movie+0xb4,state[:16]);c.uc.mem_write(self.s+0x14,state[16:52]);c.pointer(self.s+0xc8,self.ref if word_bytes(state,56) else 0);c.pointer(self.s+0xcc,self.obj if word_bytes(state,56) else 0);c.uc.mem_write(self.ref+4,b'\x01');c.uc.mem_write(self.camera,bytes(0x40));c.pointer(self.camera+4,self.obj);c.pointer(self.camera+8,self.obj if word_bytes(cam,16) else 0);c.uc.mem_write(self.camera+12,cam[:16]);c.uc.mem_write(self.camera+0x24,cam[24:40]);c.uc.mem_write(self.driver+12,words(*self.facts[2:]))
  return op
 def snapshot(self):
  c=self.c
  if self.native:return bytes(c.uc.mem_read(self.s,64))+bytes(c.uc.mem_read(self.camera,40))+bytes(c.uc.mem_read(self.point,16))
  state=bytes(c.uc.mem_read(self.movie+0xb4,16))+bytes(c.uc.mem_read(self.s+0x14,36))+words(0)+struct.pack('<Q',int(bool(word(c,self.s+0xcc))))
  cam=bytes(c.uc.mem_read(self.camera+12,16))+struct.pack('<Q',int(bool(word(c,self.camera+8))))+bytes(c.uc.mem_read(self.camera+0x24,16));return state+cam+bytes(c.uc.mem_read(self.point,16))
 def run(self,raw):
  c=self.c;op=self.load(raw)
  if self.native:
   name=('dh2_ui_set_bounds','dh2_ui_set_viewport','dh2_ui_screen_to_logical','dh2_ui_logical_to_screen','dh2_ui_flash_camera_update','dh2_ui_display_rectangle')[op]
   args=([self.s,self.params,word_bytes(raw,108),self.service] if op==0 else [self.s,self.params,self.service] if op==1 else [self.s,self.point,self.service] if op in (2,3,5) else [self.camera,self.s,self.service])
   if op==0:args[2]=word_bytes(raw,148);args[1]=self.params
   assert c.invoke(name,args)==0
  else:
   target=(0x7755f4,0x775d38,0x773dc0,0x773f50,0x42cd84,0x7751bc)[op]
   params=struct.unpack('<4I',raw[108:124]);args=[self.s,*params,word_bytes(raw,148)] if op==0 else [self.s,*params] if op==1 else [self.s,self.point] if op in (2,3) else [self.s] if op==5 else [self.camera]
   try:c.invoke(target,args)
   except Exception:
    print('original failure',op,hex(c.uc.reg_read(c.pc)),[hex(c.reg(i)) for i in range(4)])
    raise
  return self.snapshot(),tuple(self.events)
def word_bytes(raw,offset):return struct.unpack_from('<I',raw,offset)[0]
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--gold',type=Path,required=True);a=p.parse_args();manifest=json.loads((ROOT/'reference/viewport/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256'];old=Machine(a.engine,False,manifest);new=Machine(a.library,True,{'functions':[]});rng=random.Random(20261004);records=[];counts=[0]*6;callbacks=0
 def compare(op,state,cam,params,point,facts,mode=0):
  nonlocal callbacks
  raw=words(op)+state+cam+struct.pack('<4i',*params)+struct.pack('<2f',*point)+struct.pack('<4i',*facts)+words(mode);expected,events=old.run(raw);actual,calls=new.run(raw)
  assert equal(expected,actual),(len(records),op,mode,params,facts,expected.hex(),actual.hex());assert len(events)==len(calls) and all(equal(x,y) for x,y in zip(events,calls)),(len(records),'events',events,calls)
  counts[op]+=1;callbacks+=len(events);records.append(words(len(raw),len(expected),len(events))+raw+expected+b''.join(events));return expected[:64],expected[64:104]
 for i in range(500):
  rect=rng.choice(((0.,9600.,0.,6400.),(-160.,12640.,-400.,6800.),(0.,0.,0.,0.),(100.,-9000.,20.,-8000.)))
  vp=rng.choice(((0,0,480,320),(17,-31,1080,1920),(0,0,1920,1080),(0,0,0,0)))
  bounds=tuple(rng.randrange(-2000,2500) for _ in range(4));state=struct.pack('<4f8ifIQ',*rect,*vp,*bounds,.75,0,i%2)
  camera=struct.pack('<4fQ4i',-50.25,950.75,-30.75,630.5,i%2,*[rng.randrange(-1500,1500) for _ in range(4)])
  params=rng.choice((vp,(0,0,480,320),(-83,29,1920,1080),(4,6,0,0)));point=(rng.uniform(-10000,10000),rng.uniform(-10000,10000));facts=(i%4,(i//4)%4,vp[2],vp[3])
  for mode in range(3):compare(0,state,camera,params,point,facts,mode)
  for op in range(1,6):compare(op,state,camera,params,point,facts)
 # Exact signed wrap, equal/one-pixel easing, nonfinite film/clip boundaries,
 # zero/negative dimensions and source nonstandard scale-mode passthrough.
 for i in range(96):
  f=rng.choice((0.,-0.,float('nan'),float('inf'),-float('inf'),16777216.,-32768.25))
  rect=(0.,f,0.,rng.choice((f,6400.)))
  vp=rng.choice(((0,0,480,320),(INTMIN:= -2147483648,2147483647,0,-1)))
  state=struct.pack('<4f8ifIQ',*rect,*vp,*vp,1.,0,i%2)
  ca=rng.choice((INTMIN,2147483647,-1,0,1,9,10,11));cb=rng.choice((INTMIN,2147483647,ca,ca+1 if ca<2147483647 else INTMIN))
  camera=struct.pack('<4fQ4i',f,f,f,f,1,ca,cb,cb,ca);facts=(i%5-1,(i//5)%5-1,vp[2],vp[3]);params=rng.choice((vp,(0,0,2147483647,INTMIN)))
  for mode in (-1,0,1,2,99):compare(0,state,camera,params,(f,f),facts,mode)
  for op in range(1,6):compare(op,state,camera,params,(f,f),facts)
 # Stateful orientation/viewport changes: consume original-produced state on
 # subsequent calls rather than reconstructing independent expected formulas.
 state=struct.pack('<4f8ifIQ',0.,9600.,0.,6400.,0,0,480,320,0,0,480,320,1.,0,1)
 camera=struct.pack('<4fQ4i',-120.,960.,-90.,640.,1,0,0,-420,-180)
 for i in range(80):
  facts=(i%4,i%4,1080 if i%2 else 1920,1920 if i%2 else 1080)
  state,camera=compare(4,state,camera,(0,0,0,0),(13.,17.),facts)
  state,camera=compare(1,state,camera,(0,0,*facts[2:]),(13.,17.),facts)
  state,camera=compare(0,state,camera,(-17,29,*facts[2:]),(13.,17.),facts,i%3)
 gold=words(0x31505756,len(records))+b''.join(records);a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(gold)
 report=dict(validation='PASS',comparisons=len(records),operations=counts,ordered_services=callbacks,mismatches=0,original_sha256=sha(a.engine),arm64_library_sha256=sha(a.library),gold_sha256=sha(a.gold),source_sha256={p.name:sha(p) for p in (ROOT/'viewport.hpp',ROOT/'viewport.cpp')},scope=__doc__)
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
