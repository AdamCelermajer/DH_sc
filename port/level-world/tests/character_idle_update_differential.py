"""Actual Idle OnUpdate/CommonUpdate instructions vs O2 ARM64 coordinator.
Character event/controller bodies and original manager/constant/IsPlayer calls
are required synchronous fixtures. Float imports use the established IEEE oracle.
"""
import argparse,hashlib,json,math,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from navigation_motion_differential import MotionCpu
from body_transform_differential import equal
from aggro_differential import float_bits
from combat_result_differential import floating
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def word(c,p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
class Audit:
 def __init__(self,path,native,manifest):
  self.c=c=MotionCpu(path,native,manifest);self.native=native;d=c.data;self.actors=[d+0x1000+i*0x2000 for i in range(6)];self.controllers=[d+0x20000+i*0x100 for i in range(6)];self.manager=d+0x24000;self.online=d+0x25000;self.players=[d+0x30000+i*0x1000 for i in range(6)];self.stateinfos=d+0x40000;self.callback=d+0x41000;self.services=d+0x42000;self.vtable=d+0x43000;self.trace=[];self.nested=[];self.reentered=False
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
  if native:c.uc.mem_write(self.services,struct.pack('<QQ',0,self.callback));c.uc.mem_map(c.stack-0x10000,0x10000)
  else:
   c.pointer(self.vtable+0x28,self.callback);app=word(c,0x994a98+0x37f4);c.pointer(app+0x2c,d+0x46000);c.pointer(app+0x40,self.manager)
 def ret(self,v=0):self.c.put(0,v&0xffffffff);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def identity(self,p):
  if not p:return 0
  if p in self.actors:return 1+self.actors.index(p)
  if p in self.controllers:return 11+self.controllers.index(p)
  if p==self.manager:return 20
  raise AssertionError(hex(p))
 def snapshot(self):
  c=self.c;return b''.join(words(self.controllers.index(struct.unpack('<Q',c.uc.mem_read(p+8,8))[0]))+bytes(c.uc.mem_read(p+16,20)) if self.native else words(self.controllers.index(word(c,p+0x378)))+bytes(c.uc.mem_read(p+0x160,12))+words(word(c,p+0x55c),c.uc.mem_read(p+0x1b5,1)[0]) for p in self.actors)
 def mutate(self,index):
  if index!=self.x['mutate']:return
  c=self.c
  for i,p in enumerate(self.actors):
   position=words(float_bits(0.25+i*0.5),float_bits(-0.125*i),float_bits(0.125*i));timer=(self.x['delay']+1)&0xffffffff
   if self.native:c.uc.mem_write(p+16,position+words(timer,0,0));c.pointer(p+8,self.controllers[(i+1)%6])
   else:c.uc.mem_write(p+0x160,position);c.pointer(p+0x55c,timer);c.uc.mem_write(p+0x1b5,b'\0');c.pointer(p+0x378,self.controllers[(i+1)%6])
 def event(self,op,arg=0,subject=0,point=bytes(12)):
  index=len(self.trace);self.trace.append([op,arg,self.identity(subject),*struct.unpack('<3I',point)]);self.mutate(index)
  if index==self.x['reentry'] and not self.reentered:
   c=self.c;self.reentered=True;context=c.uc.context_save();stack=c.stack;outer=self.trace;self.trace=[];c.stack-=0x10000
   if self.native:c.uc.mem_write(self.actors[0]+32,words(1))
   else:c.uc.mem_write(self.actors[0]+0x1b5,b'\1')
   try:self.run();self.nested.append(self.trace.copy())
   finally:c.stack=stack;c.uc.context_restore(context);self.trace=outer
 def hook(self,uc,address,size,_):
  c=self.c;x=self.x
  if self.native:
   if address!=self.callback:return
   op,arg,subject,*point,reserved=struct.unpack('<IIQ4I',uc.mem_read(c.reg(2),32));assert not reserved;self.event(op,arg,subject,words(*point));response=0;identity=0
   if op==1:response=x['player']
   elif op==2:response=x['online']
   elif op==3:response=x['delay']
   elif op==4:response=x['count'];identity=self.manager
   elif op==5:identity=self.actors[x['list'][arg]-1] if x['list'][arg] else 0
   elif op==6:response=x['states'][self.actors.index(subject)]
   elif op==7:response=x['distance']
   uc.mem_write(c.reg(3),struct.pack('<QII',identity,response&0xffffffff,0));self.ret();return
  op={self.callback:1,0x7fd794:2,0x36d7a8:4,0x36e744:5,0x3c01ac:6,0x4054e4:8,0x3a4d5c:0}.get(address)
  if address==0x4c4bdc:
   from character_script_selection_differential import string
   key=string(c,c.reg(2));assert string(c,c.reg(1))==b'CharacterDesign';op=3 if key==b'PlayerInterPenetration_Delay' else 7;assert key in [b'PlayerInterPenetration_Delay',b'PlayerInterPenetration_Dist']
  if op is None:return
  subject=c.reg(0) if op in [0,1,8] else c.reg(0)-0x4fc if op==6 else self.manager if op==5 else 0;arg=c.reg(1) if op==5 else 0;point=bytes(uc.mem_read(c.reg(1),12)) if op==8 else bytes(12)
  if op==0:assert c.reg(1)==c.reg(2)==0
  if op==5:assert c.reg(2)==0
  self.event(op,arg,subject,point)
  if op==1:self.ret(x['player'])
  elif op==2:self.ret(self.online)
  elif op==3:self.ret(x['delay'])
  elif op==4:self.ret(x['count'])
  elif op==5:self.ret(self.players[arg])
  elif op==6:return # actual nullable-state getter executes
  elif op==7:self.ret(x['distance'])
  else:self.ret()
 def fixture(self,x):
  self.x=x;self.trace=[];self.nested=[];self.reentered=False;c=self.c;c.uc.mem_write(self.online+5,bytes([x['online']&255]))
  for i,(p,row)in enumerate(zip(self.actors,x['rows'])):
   controller,*position,timer,heading=row
   if self.native:c.uc.mem_write(p,struct.pack('<QQ6I',p,self.controllers[controller],*position,timer,heading,0))
   else:
    c.uc.mem_write(p,bytes(0x1800));c.pointer(p,self.vtable);c.pointer(p+0x378,self.controllers[controller]);c.uc.mem_write(p+0x160,words(*position));c.pointer(p+0x55c,timer);c.uc.mem_write(p+0x1b5,bytes([heading]));c.pointer(p+0x51c,self.stateinfos+16*i if x['states'][i]!=-1 else 0);c.pointer(self.stateinfos+16*i,x['states'][i]&0xffffffff)
  for i,v in enumerate(x['list']):c.pointer(self.players[i]+0x660,self.actors[v-1] if v else 0)
 def run(self):
  if self.native:return self.c.invoke('dh2_character_idle_update',[self.actors[0],self.services])
  self.c.invoke(0x3c0e80,[0,3,self.actors[0],self.actors[0]+0x4fc]);return 1
def main():
 p=argparse.ArgumentParser();p.add_argument('--cases',type=int,default=1800);p.add_argument('--library',type=Path,default=REPO/'.local-inputs/character-idle-update-discovery/oracle.so');a=p.parse_args();start=time.monotonic();ref=ROOT/'reference/character-idle-update';manifest=json.loads((ref/'original-functions.json').read_text());old=Audit(REPO/'.local-inputs/libDungeonHunter2.so',False,manifest);new=Audit(a.library,True,{'functions':[]});rng=random.Random(20261019);records=[];special=[0,0x80000000,1,0x80000001,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc01234]
 for i in range(a.cases):
  x=dict(player=int(i%7!=0),online=int(i%13==0),delay=rng.choice([0,1,100,0xffffffff,0x80000000]),count=rng.choice([-1,0,1,5]),distance=rng.choice([0,1,2,4,25,-25,0x7fffffff,-0x80000000]),list=[rng.randrange(7)for _ in range(5)],states=[rng.choice([-1,3,3,4,12])for _ in range(6)],mutate=rng.choice([-1,-1,0,1,2,3,4,6,8]),reentry=-1,rows=[])
  for j in range(6):x['rows'].append([j,*[rng.choice(special)if i%9==0 else float_bits(rng.uniform(-2,2))for _ in range(3)],rng.choice([0,1,100,0xffffffff,0x80000000]),int(j==0 and i%11==0)])
  if i%30==0:x.update(player=1,online=0,delay=0,count=5,distance=25,list=[2,3,4,5,6],states=[3]*6,mutate=-1);x['rows']=[[j,float_bits(j*.25),0,0,100,0]for j in range(6)]
  if i%120==0:x['reentry']=8
  old.fixture(x);new.fixture(x);assert new.run()==old.run()==1;assert equal(old.snapshot(),new.snapshot()) and old.trace==new.trace and old.nested==new.nested,(i,x,old.trace,new.trace,old.snapshot().hex(),new.snapshot().hex(),old.nested,new.nested)
  records.append(dict(config=x,trace=old.trace,nested=old.nested,state=list(struct.unpack('<36I',old.snapshot()))))
 guards=0
 for n in range(5):
  new.fixture(x);args=[new.actors[0],new.services]
  if n==0:args[0]=0
  elif n==1:args[1]=0
  elif n==2:new.c.uc.mem_write(new.actors[0]+36,words(1))
  elif n==3:new.c.uc.mem_write(new.actors[0]+32,words(256))
  elif n==4:new.c.pointer(new.actors[0],0)
  before=bytes(new.c.uc.mem_read(new.actors[0],40));assert new.c.invoke('dh2_character_idle_update',args)&0xffffffff==0xffffffff and not new.trace and before==bytes(new.c.uc.mem_read(new.actors[0],40));guards+=1
 gold=ref/'idle-fixtures.json';gold.write_text(json.dumps(dict(records=records),separators=(',',':'))+'\n');binary=bytearray(b'IDU1'+words(len(records)))
 for r in records:
  x=r['config'];binary+=words(x['player'],x['online'],x['delay'],x['count'],x['distance'],*x['list'],*x['states'],x['mutate'],x['reentry'],*[v for row in x['rows']for v in row],len(r['trace']),len(r['nested']),*r['state'])
  for t in r['trace']:binary+=words(*t)
  for nested in r['nested']:binary+=words(len(nested));binary+=b''.join(words(*t)for t in nested)
 (ref/'idle-fixtures.bin').write_bytes(binary);build=json.loads(Path(str(a.library)+'.build.json').read_text(encoding='utf-8-sig'));sources=build['source_sha256'];sources[Path(__file__).relative_to(REPO).as_posix()]=sha(Path(__file__))
 report=dict(validation='PASS',original_sha256=manifest['original_sha256'],manifest_sha256=sha(ref/'original-functions.json'),arm64_library_sha256=sha(a.library),source_sha256=sources,comparisons=len(records),ordered_requests=sum(len(r['trace'])+sum(len(n)for n in r['nested'])for r in records),reentry_cases=sum(bool(r['nested'])for r in records),atomic_guards=guards,mismatches=0,gold_sha256=sha(gold),binary_gold_sha256=sha(ref/'idle-fixtures.bin'),original_imports=old.c.import_calls,scope=__doc__,elapsed_seconds=round(time.monotonic()-start,2));(ROOT/'reports/character-idle-update-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
