"""Actual AISDefault collision control/counter instructions versus native ARM64.

Original SM_GetState/SM_IsMoving(false)/ApplicationGetDt execute. IsCharacter,
IsPlayer, AI_IsEnemy, CancelSneaking and AI_SetTarget are explicit synchronous
services; their ordered arguments and live transitional snapshots are compared.
"""
import argparse,hashlib,json,random,struct
from pathlib import Path
from navigation_differential import Cpu
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
KEY=0x100000000
AIS=0x2001000;OWNER=0x2002000;OTHER=0x2006000;COLLIDER=0x200c000;TARGET=0x200a000;PREFERRED=0x200b000
class ServiceCpu(Cpu):
 def external(self,uc,address,size,unused):
  # These pages belong to this fixture, not the inherited aggro fixture.
  if address in (self.callback+16,self.callback+32):return
  return super().external(uc,address,size,unused)
class Machine:
 def __init__(self,path,native,manifest):
  self.c=ServiceCpu(path,native,manifest);self.native=native;c=self.c;self.s=c.data+0x1000;self.o=c.data+0x10000;self.services=c.data+0x11000;self.vt=c.data+0x12000;self.fsm=c.data+0x13000;self.app=c.data+0x14000;self.req=c.data+0x15000
  self.events=[];self.indices=[0,0,0];self.depth=0;self.mutated=False
  if native:c.uc.mem_write(self.services,struct.pack('<QQ',0,c.callback+16))
  else:
   c.pointer(self.vt+0x24,c.callback+16);c.pointer(self.vt+0x28,c.callback+32)
   def word(at):return struct.unpack('<I',c.uc.mem_read(at,4))[0]
   got=0x3dbfd4+word(0x3dc124);c.pointer(got+word(0x3dc128),self.app)
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def snapshot(self):
  c=self.c
  if self.native:return bytes(c.uc.mem_read(self.s,72)),bytes(c.uc.mem_read(self.o,16))
  def w(at):return struct.unpack('<I',c.uc.mem_read(at,4))[0]
  owner=w(AIS+0x98);key=lambda p:p+KEY if p else 0
  state=struct.pack('<6Q6I',key(AIS),key(owner),key(owner+0x3c8),key(owner+0x4fc),key(w(owner+0x408)),key(w(owner+0x418)),w(AIS+0xbc),w(AIS+0xc0),c.uc.mem_read(owner+0x3e0,1)[0],w(self.fsm),w(self.app+0x74),w(self.app+0x8c))
  return state,struct.pack('<QII',key(COLLIDER),w(COLLIDER+0xf4),0)
 def configure(self,state,object,persist,answers,mutation):
  self.events=[];self.indices=[0,0,0];self.depth=0;self.mutated=False;self.persist=persist;self.answers=answers;self.mutation=mutation;self.write(state,object)
 def write(self,state,object):
  c=self.c
  if self.native:c.uc.mem_write(self.s,state);c.uc.mem_write(self.o,object);return
  values=struct.unpack('<6Q6I',state);owner=values[1]-KEY
  for ptr in (OWNER,OTHER,COLLIDER):c.pointer(ptr,self.vt)
  c.pointer(AIS+0x98,owner);c.uc.mem_write(AIS+0xbc,words(values[6],values[7]));c.uc.mem_write(owner+0x3e0,bytes([values[8]]));c.pointer(owner+0x408,values[4]-KEY if values[4] else 0);c.pointer(owner+0x418,values[5]-KEY if values[5] else 0);c.pointer(owner+0x4fc+0x20,self.fsm);c.uc.mem_write(self.fsm,words(values[9]));c.uc.mem_write(self.app+0x74,words(values[10]));c.uc.mem_write(self.app+0x8c,words(values[11]));c.uc.mem_write(COLLIDER+0xf4,object[8:12])
 def invoke(self):
  c=self.c;symbol='dh2_character_script_collision' if self.native else 0x3dbfa0;args=[self.s,self.o,self.persist,self.services] if self.native else [AIS,COLLIDER,self.persist]
  if not self.depth:return c.invoke(symbol,args)
  # Reentrant source calls use their own stack below the suspended caller.
  entry_sp=c.uc.reg_read(c.sp)-0x400;c.uc.reg_write(c.sp,entry_sp);c.uc.reg_write(c.lr,c.stop)
  for i,v in enumerate(args):c.put(i,v)
  address=c.symbols[symbol] if isinstance(symbol,str) else symbol;c.uc.emu_start(address,c.stop,count=1000000)
  assert c.uc.reg_read(c.pc)==c.stop and c.uc.reg_read(c.sp)==entry_sp
  return c.reg(0)
 def service(self,request):
  c=self.c;service=struct.unpack_from('<I',request)[0];state,object=self.snapshot();response=0
  if service<3:
   index=self.indices[service];self.indices[service]+=1;response=self.answers[service*2+min(index,1)]
  self.events.append(request+state+object+words(self.depth,response))
  if not self.depth and not self.mutated and ((self.mutation in (1,4,7) and service==0) or (self.mutation==2 and service==1) or (self.mutation==3 and service==2) or (self.mutation in (5,6,8) and service in (3,4))):
   self.mutated=True;values=list(struct.unpack('<6Q6I',state));obj=list(struct.unpack('<QII',object))
   if self.mutation==1:values[4]=KEY+COLLIDER
   elif self.mutation==2:values[5]=values[4]
   elif self.mutation==3:values[6]=0xfffffff0;values[10]=(values[10]+1)&0xffffffff;values[11]=0x20
   elif self.mutation==4:values[1]=KEY+OTHER;values[2]=KEY+OTHER+0x3c8;values[3]=KEY+OTHER+0x4fc;values[8]=0;obj[1]=21
   elif self.mutation==5:values[4]=KEY+COLLIDER
   elif self.mutation==6:values[8]=1
   if self.mutation in (7,8):
    context=c.uc.context_save();self.depth+=1;self.invoke();self.depth-=1;c.uc.context_restore(context)
   else:self.write(struct.pack('<6Q6I',*values),struct.pack('<QII',*obj))
  return response
 def hook(self,uc,address,size,unused):
  c=self.c
  if self.native:
   if address!=c.callback+16:return
   assert c.reg(1)==self.s and c.reg(2)==self.o;request=bytes(uc.mem_read(c.reg(3),32))
  else:
   mapping={c.callback+16:0,c.callback+32:1,0x3d574c:2,0x3bc6b8:3,0x3d6890:4}
   if address not in mapping:return
   service=mapping[address];subject=c.reg(0)+KEY;target=c.reg(1)+KEY if service in (2,4) and c.reg(1) else 0;arg=c.reg(2) if service==4 else 0;request=struct.pack('<IIQQII',service,arg,subject,target,0,0)
  response=self.service(request);c.put(0,response);uc.reg_write(c.pc,uc.reg_read(c.lr))

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--manifest',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);a=p.parse_args();m=json.loads(a.manifest.read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==m['original_sha256'];old=Machine(a.engine,False,m);new=Machine(a.library,True,{'functions':[]});rng=random.Random(20261003);records=[];callbacks=0;reentry=0
 for i in range(4096):
  state=struct.pack('<6Q6I',KEY+AIS,KEY+OWNER,KEY+OWNER+0x3c8,KEY+OWNER+0x4fc,rng.choice((0,KEY+TARGET,KEY+COLLIDER)),rng.choice((0,KEY+TARGET,KEY+PREFERRED)),rng.choice((0,198,199,200,0xfffffff0,0xffffffff)),rng.choice((0,1,0xffffffff)),rng.choice((0,0,1,255)),rng.choice((0,4,4,19,19,20,0xffffffff)),rng.choice((0,1,2,0xffffffff)),rng.choice((0,1,16,200,0x80000000,0xffffffff)))
  object=struct.pack('<QII',KEY+COLLIDER,rng.choice((0,1,2,3,21,22,0xffffffff)),0);persist=rng.choice((0,1,255));answers=[rng.choice((0,1,255)) for _ in range(6)];mutation=i%9
  for machine in (old,new):machine.configure(state,object,persist,answers,mutation);result=machine.invoke();assert not machine.native or result==1
  assert new.snapshot()==old.snapshot(),(i,'state',old.snapshot(),new.snapshot());assert new.events==old.events,(i,'ordered services',old.events,new.events)
  final,final_object=old.snapshot();records.append(state+object+words(persist,*answers,mutation)+final+final_object+words(len(old.events))+b''.join(old.events));callbacks+=len(old.events);reentry+=any(struct.unpack_from('<I',e,120)[0] for e in old.events)
 blob=b'SCC1'+words(len(records))+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(blob)
 report={'validation':'PASS','original_sha256':m['original_sha256'],'arm64_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'manifest_sha256':hashlib.sha256(a.manifest.read_bytes()).hexdigest(),'corpus_sha256':hashlib.sha256(blob).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'source_sha256':{n:hashlib.sha256((REPO/n).read_bytes()).hexdigest() for n in ('port/level-world/character_script_collision.hpp','port/level-world/character_script_collision.cpp')},'comparisons':len(records),'ordered_callbacks':callbacks,'nested_callback_cases':reentry,'mismatches':0,'scope':__doc__};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
