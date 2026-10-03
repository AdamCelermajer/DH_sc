"""Execute original ARM32 applicator/root/VisualObject routines against ARM64.

Animator type and scene vtable ownership are explicit caller facts; the actual
GetApplicator, ordered delta sum, quaternion operations, node setters and game
setter execute. Absolute matrix/backend transform services are observed.
"""
import argparse,hashlib,json,math,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from navigation_motion_differential import MotionCpu
from navigation_differential import ROOT,equal
from navigation_search_differential import word,words
from subobjects_update_differential import UpdateCpu,Original as OriginalUpdate,state,body,policy,fixture
from aggro_differential import float_bits
from combat_result_differential import floating
from unicorn.arm64_const import UC_ARM64_REG_D0,UC_ARM64_REG_S0,UC_ARM64_REG_S1,UC_ARM64_REG_S2

class CheckedMemory:
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='__memset_chk':
   dst,value,count,capacity=[self.reg(i) for i in range(4)];assert count<=capacity and count<=0x2000000
   if count:uc.mem_write(dst,bytes((value&255,))*count)
   self.put(0,dst);self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,unused)
class NativeCpu(CheckedMemory,MotionCpu):pass
class VisualCpu(CheckedMemory,UpdateCpu):pass
class RotationCpu(CheckedMemory,UpdateCpu):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='dh2_quat_from_euler' and hasattr(self,'math_cpu'):
   # Execute the dependency extracted from the same APK, rather than model
   # quaternion construction as a service. Separate CPU storage keeps both
   # ELF address spaces intact and transports only the actual ABI arguments.
   other=self.math_cpu;out=self.reg(0)
   for reg in (UC_ARM64_REG_S0,UC_ARM64_REG_S1,UC_ARM64_REG_S2):other.uc.reg_write(reg,uc.reg_read(reg))
   other.invoke('dh2_quat_from_euler',[other.data])
   uc.mem_write(out,bytes(other.uc.mem_read(other.data,16)));self.put(0,out)
   self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr));return
  def double(i=0):return struct.unpack('<d',struct.pack('<II',self.reg(i),self.reg(i+1)))[0]
  def putdouble(v):
   raw=struct.unpack('<Q',struct.pack('<d',v))[0]
   if self.arm64:uc.reg_write(UC_ARM64_REG_D0,raw)
   else:self.put(0,raw&0xffffffff);self.put(1,raw>>32)
  if name in ('sin','cos'):
   v=struct.unpack('<d',struct.pack('<Q',uc.reg_read(UC_ARM64_REG_D0)))[0] if self.arm64 else double();putdouble((math.sin if name=='sin' else math.cos)(v))
  elif name=='__aeabi_f2d':putdouble(floating(self.reg(0)))
  elif name=='__aeabi_d2f':self.put(0,float_bits(double()))
  elif name in ('__aeabi_dadd','__aeabi_dsub','__aeabi_dmul','__aeabi_ddiv'):
   a,b=double(),double(2);putdouble(a+b if name.endswith('add') else a-b if name.endswith('sub') else a*b if name.endswith('mul') else a/b)
  else:return super().external(uc,address,size,unused)
  uc.reg_write(self.pc,uc.reg_read(self.lr))

class Rotation:
 def __init__(self,engine,library,manifest,math_library=None):
  self.old=RotationCpu(engine,False,manifest);self.new=RotationCpu(library,True,{'functions':[]});c=self.old;self.visual=c.data+0x1000;self.root=c.data+0x2000;self.vt=c.data+0x3000;self.point=c.data+0x4000;c.pointer(self.visual+8,self.root);c.pointer(self.root,self.vt);c.pointer(self.vt+0x98,c.callback+32);c.pointer(self.vt+0x9c,c.callback+48);c.handler=self.callback;c.uc.hook_add(UC_HOOK_CODE,self.hook)
  if math_library:self.new.math_cpu=RotationCpu(math_library,True,{'functions':[]})
 def callback(self,address):
  c=self.old
  if address==c.callback+32:c.put(0,self.root+0xb8)
  else:c.uc.mem_write(self.root+0xb8,bytes(c.uc.mem_read(c.reg(1),16)))
 def hook(self,uc,address,size,unused):
  if address in (0x47211c,0x470a54):uc.reg_write(self.old.pc,uc.reg_read(self.old.lr))
 def execute(self,point):
  c=self.old;c.uc.mem_write(self.point,point);c.uc.mem_write(self.root+0xb8,bytes(16));c.invoke(0x472874,[self.visual,self.point]);expected=bytes(c.uc.mem_read(self.root+0xb8,16));c=self.new;c.uc.mem_write(c.data,point);assert c.invoke('dh2_visual_rotation',[c.data+128,c.data])==0;return expected,bytes(c.uc.mem_read(c.data+128,16))

class Original:
 def __init__(self,engine,manifest):
  self.c=UpdateCpu(engine,False,manifest);c=self.c;d=c.data
  self.root=d+0x1000;self.animated=d+0x2000;self.secondary=d+0x3000;self.helper=d+0x4000;self.vtable=d+0x5000;self.app=d+0x6000;self.input=d+0x7000;self.list=d+0x10000;self.anim=d+0x20000;self.av=d+0x30000
  c.handler=self.callback;c.pointer(self.vtable+0xa0,0x597124);c.pointer(self.vtable+0xa4,0x59712c);c.pointer(self.vtable+0xb8,c.callback+32);c.pointer(self.av+0x24,c.callback+48)
 def callback(self,address):
  c=self.c
  if address==c.callback+48:c.put(0,word(c,c.reg(0)+4))
  elif address==c.callback+32:self.updates+=1
  else:raise AssertionError(hex(address))
 def delta(self,op,raw,t,point):
  c=self.c;c.uc.mem_write(self.app+0x14,raw);c.uc.mem_write(self.input,point);c.invoke(0x364444 if op==0 else 0x3644cc,[self.app,t,self.input]);return bytes(c.uc.mem_read(self.app+0x14,28))
 def displace(self,raw,deltas):
  c=self.c;flags=struct.unpack_from('<5I',raw,76);presence=flags[0];self.updates=0
  for node,point,flag in ((self.root,raw[:12],flags[1]),(self.animated,raw[40:52],flags[2]),(self.secondary,raw[52:64],flags[3]),(self.helper,raw[64:76],flags[4])):
   c.uc.mem_write(node,bytes(0x300));c.pointer(node,self.vtable);c.uc.mem_write(node+0xac,point);c.pointer(node+0x11c,flag)
  c.uc.mem_write(self.root+0xb8,raw[12:40]);c.pointer(self.root+0x1f0,self.animated);c.pointer(self.root+0x1f4,self.secondary if presence&2 else 0);c.pointer(self.root+0x1f8,self.helper if presence&1 else 0)
  count=len(deltas)//12;sentinel=self.root+0xfc;c.pointer(sentinel,self.list if count else sentinel)
  for i in range(count):
   node=self.list+i*16;anim=self.anim+i*512;kind=11+i%5;c.pointer(node,self.list+(i+1)*16 if i+1<count else sentinel);c.pointer(node+8,anim);c.pointer(anim,self.av);c.pointer(anim+4,kind);c.uc.mem_write(anim+{11:0x58,12:0x88,13:0x58,14:0xd8,15:0x84}[kind]+0x24,deltas[i*12:(i+1)*12])
  moved=c.invoke(0x35cf48,[self.root,0])
  out=bytes(c.uc.mem_read(self.root+0xac,12))+raw[12:40]+b''.join(bytes(c.uc.mem_read(n+0xac,12)) for n in (self.animated,self.secondary,self.helper))+words(presence,*(word(c,n+0x11c) for n in (self.root,self.animated,self.secondary,self.helper)))
  assert self.updates==int(bool(presence&1));return words(moved)+out

class Native:
 def __init__(self,library):self.c=NativeCpu(library,True,{'functions':[]});self.s=self.c.data+0x1000;self.input=self.c.data+0x10000;self.request=self.c.data+0x20000
 def delta(self,op,raw,t,point):
  c=self.c;c.uc.mem_write(self.s,raw);c.uc.mem_write(self.input,point);assert c.invoke(('dh2_visual_calculate_delta','dh2_visual_reset_delta')[op],[self.s,t,self.input])==0;return bytes(c.uc.mem_read(self.s,28))
 def displace(self,raw,deltas):
  c=self.c;c.uc.mem_write(self.s,raw);c.uc.mem_write(self.input,deltas or bytes(12));c.uc.mem_write(self.request,struct.pack('<QQII',self.s,self.input,len(deltas)//12,0));moved=c.invoke('dh2_visual_displace',[self.request]);return words(moved)+bytes(c.uc.mem_read(self.s,96))

class VisualOriginal(OriginalUpdate):
 def __init__(self,engine,manifest):
  super().__init__(engine,manifest);c=self.c;self.node=c.data+0xe000;self.nv=c.data+0xf000;c.pointer(self.nv+0xa0,0x597124);c.pointer(self.nv+0xa4,0x59712c);c.pointer(self.nv+0xb8,c.callback+144)
 def callback(self,address):
  if address==self.c.callback+144:self.event(19,self.fdata(self.node+0xac));self.c.put(0,0)
  else:super().callback(address)
 def hook(self,uc,address,size,unused):
  if address in (0x47124c,0x470cb8):return
  super().hook(uc,address,size,unused)
 def execute_visual(self,op,s,b,root,flags,present,owner):
  self.configure(s,b,policy(visual=flags&2,aux=flags&1),fixture(body=present));c=self.c;c.pointer(self.visual+4,self.game if owner else 0);c.pointer(self.visual+8,self.node if root else 0);c.pointer(self.node,self.nv);c.uc.mem_write(self.node+0xac,root[:12] if root else bytes(12));c.pointer(self.node+0x11c,struct.unpack_from('<I',root,80)[0] if root else 0)
  self.active=True;c.invoke(0x47124c if op==2 else 0x470cb8,[self.visual]);self.active=False;ss,bb,_,events=self.snapshot();return ss+bb+(self.fdata(self.node+0xac)+words(word(c,self.node+0x11c)) if root else bytes(16)),events

class VisualNative:
 def __init__(self,library):
  self.c=VisualCpu(library,True,{'functions':[]});c=self.c;d=c.data;self.s=d+0x1000;self.b=d+0x2000;self.r=d+0x3000;self.t=d+0x4000;self.srv=d+0x5000;self.req=d+0x6000;c.handler=self.callback
 def callback(self,address):
  c=self.c;e=c.reg(1);payload=bytes(c.uc.mem_read(c.reg(2),12));self.events.append((e,payload))
  if e==6:c.uc.mem_write(self.b+4,payload[:8]);c.uc.mem_write(self.b+12,payload[8:12])
  c.put(0,0)
 def visual(self,op,s,b,root,flags,present,owner):
  c=self.c;self.events=[];c.uc.mem_write(self.s,s);c.uc.mem_write(self.b,b);c.uc.mem_write(self.r,root or bytes(96));c.uc.mem_write(self.t,bytes(16));c.uc.mem_write(self.srv,struct.pack('<QQ',0,c.callback+144));c.uc.mem_write(self.req,struct.pack('<5Q2I',self.s if owner else 0,self.r if root else 0,self.b if present else 0,self.t,self.srv,flags,0));assert c.invoke('dh2_visual_apply_position' if op==2 else 'dh2_visual_sync_position',[self.req])==0
  return bytes(c.uc.mem_read(self.s,128))+bytes(c.uc.mem_read(self.b,48))+(bytes(c.uc.mem_read(self.r,12))+bytes(c.uc.mem_read(self.r+80,4)) if root else bytes(16)),tuple(self.events)

def root(position=(100.,200.,300.),quaternion=(0.,0.,0.,1.),scale=(1.,1.,1.),animated=(7.,11.,13.),presence=1):return struct.pack('<19f5I',*position,*quaternion,*scale,*animated,17.,19.,23.,29.,31.,37.,presence,0x20,0x10,0x40,0x80)
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--asset-samples',type=Path);p.add_argument('--math-library',type=Path);a=p.parse_args();start=time.monotonic();manifest=json.loads((ROOT/'reference/visual-motion/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];old=Original(a.engine,manifest);new=Native(a.library);vo=VisualOriginal(a.engine,manifest);vn=VisualNative(a.library);rotation=Rotation(a.engine,a.library,manifest,a.math_library);rng=random.Random(20261015);records=[];counts={'calculate':0,'reset':0,'displace':0,'apply':0,'sync':0,'rotation':0,'ordered_calls':0,'asset_samples':0}
 def compare(op,input,expected,actual,events=()):
  assert equal(expected,actual),(len(records),op,expected.hex(),actual.hex());records.append(words(op,len(input),len(expected),len(events))+input+expected+b''.join(words(e)+v for e,v in events));counts[('calculate','reset','apply','sync','displace','rotation')[op]]+=1
 for i in range(128):
  point=struct.pack('<3f',*(rng.uniform(-6.3,6.3) for _ in range(3)));compare(5,point,*rotation.execute(point))
 for i in range(480):
  raw=words(rng.randrange(4))+struct.pack('<6f',*(rng.uniform(-1e4,1e4) for _ in range(6)));point=struct.pack('<3f',*(rng.uniform(-1e4,1e4) for _ in range(3)));t=rng.randrange(4)
  for op in (0,1):compare(op,raw+words(t)+point,old.delta(op,raw,t,point),new.delta(op,raw,t,point))
 for i in range(620):
  q=(0,0,0,1) if i%5==0 else tuple(rng.uniform(-1,1) for _ in range(4));s=tuple(rng.choice((0.,1.,-1.,.01,100.)) for _ in range(3));rr=root(quaternion=q,scale=s,presence=i%4);ds=struct.pack('<'+'f'*(i%8*3),*(rng.uniform(-200,200) for _ in range(i%8*3)));compare(4,rr+words(len(ds)//12)+ds,old.displace(rr,ds),new.displace(rr,ds))
 # Exceptional values preserve original zero comparisons and sign-bit flips.
 for v in (0.,-0.,float('nan'),float('inf'),-float('inf'),1e-40):
  for presence in range(4):
   rr=root(animated=(v,-0.,v),presence=presence);ds=struct.pack('<3f',v,v,v);compare(4,rr+words(1)+ds,old.displace(rr,ds),new.displace(rr,ds))
 for owner in (0,1):
  for present in (0,1):
   for hasroot in (0,1):
    for flags in range(4):
     for op in (2,3):
      ss=state(position=(41.,43.,47.),aux=(53.,59.,61.));bb=body();rr=root() if hasroot else b'';expected,events=vo.execute_visual(op,ss,bb,rr,flags,present,owner);actual,calls=vn.visual(op,ss,bb,rr,flags,present,owner);assert len(calls)==len(events) and all(e==f and equal(x,y) for (e,x),(f,y) in zip(events,calls)),(op,flags,events,calls);counts['ordered_calls']+=len(events);compare(op,ss+bb+(rr or bytes(96))+words(flags,present,owner,hasroot),expected,actual,events)
 if a.asset_samples:
  blob=a.asset_samples.read_bytes();assert blob[:4]==b'VRSA';n=struct.unpack_from('<I',blob,4)[0];assert len(blob)==8+n*44
  for i in range(n):
   clip,ms,timestamp,previous_timestamp=struct.unpack_from('<4I',blob,8+i*44);prev=blob[24+i*44:36+i*44];point=blob[36+i*44:48+i*44];reset=struct.unpack_from('<I',blob,48+i*44)[0];raw=words(previous_timestamp)+prev+bytes(12);op=int(bool(reset));expected=old.delta(op,raw,timestamp,point);actual=new.delta(op,raw,timestamp,point);assert equal(expected,actual);rr=root(animated=struct.unpack('<3f',point));assert equal(old.displace(rr,expected[16:28]),new.displace(rr,actual[16:28]));counts['asset_samples']+=1
 output=b'VMG1'+words(len(records))+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(output);report={'original_sha256':manifest['original_sha256'],'native_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'corpus_sha256':hashlib.sha256(output).hexdigest(),'counts':counts,'cases':len(records),'mismatches':0,'seconds':round(time.monotonic()-start,3),'fidelity':'Actual original root/applicator/game and visual routines. AbsolutePosition and SetXForm services observed; timeline/animator type/ownership fixtures. Full animator blending, culling and frame scheduling remain external.'};report['math_dependency_sha256']=hashlib.sha256(a.math_library.read_bytes()).hexdigest() if a.math_library else None;a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
if __name__=='__main__':main()
