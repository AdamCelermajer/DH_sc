"""Original pending/active AIS lifecycle instructions vs native ARM64.

Opaque allocation, Lua/skills, owner/property/timer and virtual bodies are
synchronous services; source stores/reloads/gates/call order execute directly.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from visual_timeline_differential import TimelineCpu
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def word(c,p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
ENTRY=(0x3ccfe4,0x3cf1f0,0x3cb314,0x3ce7c0,0x3cf3a4,0x3d12b0,0x3cfde4,0x3cfd7c,0x3d0b80,0x3d0ba4)
class Machine:
 def __init__(self,path,native,manifest):
  self.c=TimelineCpu(path,native,manifest);self.native=native;c=self.c;d=c.data
  self.s=d+0x1000;self.svc=d+0x2000;self.services=d+0x3000;self.vt=d+0x4000;self.avt=d+0x5000;self.ovt=d+0x6000
  self.ptrs={0:0,1:d+0x10000,2:d+0x20000,3:d+0x30000,4:d+0x40000,5:d+0x50000,6:d+0x60000};self.ids={v:k for k,v in self.ptrs.items()}
  c.uc.mem_write(self.services,struct.pack('<QQ',d+0x7000,self.svc))
  if not native:
   c.pointer(self.s,self.vt)
   for slot,addr in ((8,0x3d12b0),(12,0x3d0b80),(16,0x3d0ba4),(20,0x3d11bc)):c.pointer(self.vt+slot,addr)
   for i in (2,3,5):
    c.pointer(self.ptrs[i],self.avt)
   for slot,service in ((4,1),(8,21),(12,22),(16,23),(20,26),(0xcc,11)):c.pointer(self.avt+slot,self.svc+4*(service+1))
   for i in (1,6):c.pointer(self.ptrs[i],self.ovt)
   c.pointer(self.ovt+0x28,self.svc+4*9);c.pointer(self.ovt+0x34,self.svc+4*18)
   c.uc.mem_write(self.ptrs[4],b'external_fixture\0')
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def snapshot(self):
  c=self.c
  if self.native:
   raw=bytes(c.uc.mem_read(self.s,64));p=struct.unpack('<4Q',raw[:32]);return struct.pack('<4Q',*(self.ids[v] for v in p))+raw[32:]
  p=[word(c,self.s+o) for o in (4,0x1c,0x20,0x30)]
  return struct.pack('<4Q',*(self.ids[v] for v in p))+words(word(c,self.s+0x28),word(c,self.s+0x10),word(c,self.s+0x14),c.uc.mem_read(self.s+0x24,1)[0],c.uc.mem_read(self.s+0x2c,1)[0],0,0,0)
 def put_state(self,raw):
  c=self.c;p=struct.unpack('<4Q',raw[:32]);v=struct.unpack('<8I',raw[32:])
  if self.native:c.uc.mem_write(self.s,struct.pack('<4Q',*(self.ptrs[i] for i in p))+raw[32:])
  else:
   for i in (2,3,5):c.pointer(self.ptrs[i],self.avt)
   for o,i in zip((4,0x1c,0x20,0x30),p):c.pointer(self.s+o,self.ptrs[i])
   for o,i in zip((0x28,0x10,0x14),v[:3]):c.pointer(self.s+o,i)
   c.uc.mem_write(self.s+0x24,bytes([v[3]]));c.uc.mem_write(self.s+0x2c,bytes([v[4]]))
 def request(self,service,a=0,b=0,subject=0,payload=0):
  row=words(service,a,b,0)+struct.pack('<QQ',subject,payload)+self.snapshot();self.calls.append(row)
  p=self.params
  if service==p[6]:
   raw=bytearray(self.snapshot());kind=p[5]
   if kind==1:struct.pack_into('<Q',raw,16,5)
   elif kind==2:struct.pack_into('<Q',raw,16,0)
   elif kind==3:struct.pack_into('<Q',raw,8,5)
   elif kind==4:struct.pack_into('<Q',raw,24,0)
   elif kind==5:struct.pack_into('<I',raw,44,0)
   elif kind==6:struct.pack_into('<Q',raw,0,6)
   elif kind==7:struct.pack_into('<i',raw,40,-1)
   elif kind==8:struct.pack_into('<I',raw,32,6)
   elif kind==9:struct.pack_into('<I',raw,48,0)
   self.put_state(bytes(raw))
  values={8:p[0],9:p[1],17:p[2],19:p[3] if a==0x33 else p[4],20:100 if b==0x33 else 101}
  if service==3:
   raw=bytearray(self.snapshot());struct.pack_into('<Q',raw,16,3);self.put_state(bytes(raw))
  return values.get(service,0),self.ptrs[3] if service==2 else 0
 def finish(self,word_value=0,identity=0):
  c=self.c;c.put(0,identity or word_value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def hook(self,uc,address,size,unused):
  c=self.c
  if self.native:
   if address!=self.svc:return
   r=c.reg(2);raw=bytes(c.uc.mem_read(r,32));v=struct.unpack('<4I2Q',raw)
   assert v[3]==0
   w,i=self.request(v[0],v[1],v[2],self.ids[v[4]],self.ids[v[5]])
   c.uc.mem_write(c.reg(3),words(w,0)+struct.pack('<Q',i))
   if self.params[7] and v[0] in (10,15,16):
    for r,value in enumerate((self.s,{10:5,15:8,16:9}[v[0]],0,self.services)):c.put(r,value)
    c.uc.reg_write(c.pc,c.symbols['dh2_character_script_lifecycle']);return
   self.finish();return
  if address==self.entry and self.first:self.first=False;return
  if self.svc+4<=address<=self.svc+4*27:
   service=(address-self.svc)//4-1;subject=self.ids[c.reg(0)];self.request(service,subject=subject);self.finish(self.params[0] if service==8 else self.params[2] if service==17 else 0);return
  mapping={0x3d11bc:0,0x3cf04c:3,0x3d90f8:4,0x3d8f2c:5,0x3cb854:9,0x3d12b0:10,0x3b3a70:12,0x3ce044:13,0x3d8894:14,0x3d0b80:15,0x3d0ba4:16,0x3db2d8:18,0x4c4bdc:19,0x3dbe24:20,0x3d8ae0:24,0x3d8a98:25}
  if address==0x310570:
   assert c.reg(0)==0xd8;w,i=self.request(2,0xd8,1);self.finish(w,i);return
  if address in (0x3d8fb0,0x3ccf9c):self.finish(c.reg(0));return
  if address==0x3cdf7c:
   raw=self.snapshot();pending,name=struct.unpack_from('<QQ',raw,16)
   if name:self.request(7,subject=pending,payload=name)
   self.finish();return
  if address==0x37b574:
   self.request(6,subject=self.ids[c.reg(0)]);self.finish();return
  if address not in mapping:return
  service=mapping[address];a=b=subject=payload=0
  if service==4:subject,payload=self.ids[c.reg(0)],self.ids[c.reg(1)]
  elif service==5:subject=self.ids[c.reg(0)]
  elif service==9:a=c.reg(1)
  elif service==12:subject=self.ids[c.reg(0)]
  elif service==18:a=c.reg(1);subject=self.ids[c.reg(0)-0x3b4]
  elif service==19:
   name=bytes(c.uc.mem_read(c.reg(2),16)).split(b'\0')[0];a=0x33 if name==b'AI_Tick' else 0x34;assert name in (b'AI_Tick',b'DoT_Tick')
  elif service==20:
   a=c.reg(1);b=c.reg(3);subject=self.ids[c.reg(0)-0x3b4];assert c.reg(2)==0xffffffff and word(c,c.uc.reg_read(c.sp))==0
  w,i=self.request(service,a,b,subject,payload)
  if self.params[7] and service in (10,15,16):return
  self.finish(w,i)
 def execute(self,op,raw,params,arg):
  self.put_state(raw);self.params=params;self.calls=[];self.entry=ENTRY[op];self.first=True
  result=self.c.invoke('dh2_character_script_lifecycle' if self.native else ENTRY[op],[self.s,op,arg,self.services] if self.native else [self.s,arg])
  return (result&0xffffffff if self.native or op==4 else 1),self.snapshot(),tuple(self.calls)
def main():
 p=argparse.ArgumentParser()
 for key in ('engine','library','gold','report'):p.add_argument('--'+key,type=Path,required=True)
 a=p.parse_args();manifest=json.loads((ROOT/'port/level-world/reference/character-script-lifecycle/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256']
 old=Machine(a.engine,False,manifest);new=Machine(a.library,True,{'functions':[]});records=[];calls=0;rng=random.Random(0x1c20)
 def state(active=0,pending=3,step=0,delayed=0,scripted=0,name=0,t33=-1,t34=-1):return struct.pack('<4Q',1,active,pending,name)+words(step,t33,t34,delayed,scripted,0,0,0)
 def compare(op,s,params,arg=0):
  nonlocal calls
  try:expected=old.execute(op,s,params,arg);actual=new.execute(op,s,params,arg)
  except Exception:
   print('Case',len(records),'op',op,'state',struct.unpack('<4Q8I',s),'params',params,'PC',hex(old.c.uc.reg_read(old.c.pc)));raise
  assert expected==actual,(len(records),op,params,arg,expected,actual)
  result,after,events=expected;calls+=len(events);records.append(words(op,arg)+s+words(*params)+words(result)+after+words(len(events))+b''.join(events))
 # Source legal states and every operation; virtual receivers remain alive.
 for op in range(10):
  for i in range(140):
   step=rng.choice((0,1,2,3,4,5,6,7,0x7fffffff));delayed=rng.choice((0,1,255));params=[rng.choice((0,1,2)),rng.choice((0,1,2,7)),rng.choice((0,1,2)),rng.choice((0,10,1000,0xffffffff)),rng.choice((0,20,2000)),0,0,0]
   pending=3 if op==2 else rng.choice((0,3)) if op in (0,5,7,8,9) or step in (0,6,7,0x7fffffff) else 3
   compare(op,state(rng.choice((0,2)),pending,step,delayed,rng.choice((0,1,255)),rng.choice((0,4)),rng.choice((-1,0,12)),rng.choice((-1,0,13))),params,rng.randrange(2))
 # Deliberate callback mutation at relevant source reload/captured-local sites.
 for op,service,mutations in ((0,0,(1,2)),(0,1,(1,)),(1,3,(8,9)),(1,4,(1,)),(2,10,(1,4)),(3,13,(3,)),(5,17,(1,)),(5,19,(6,7)),(6,21,(3,)),(7,24,(3,)),(7,18,(1,)),(8,22,(3,))):
  for mutation in mutations:
   compare(op,state(2,3,0,0,1,4,12,13),[0,7,0,10,20,mutation,service,0],1)
 # Execute source Init/InitPost/InitFinal wrappers recursively, preserving the
 # real outer frame rather than using a separate invoke/reset of its stack.
 for op in (1,2,3,4):
  for dead in (0,1):
   for final in (0,1):
    for mutation,service in ((0,0),(1,21),(3,22),(6,19)):
     compare(op,state(0 if op==4 else 2,3,0,0,1,0,12,13),[0,7,dead,10,20,mutation,service,1],final)
 blob=b'ASL1'+words(len(records))+b''.join(records);a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(blob)
 rejects=0
 def reject(s,op=0,arg=0,nullstate=False,nullservice=False,missing=False,alias=False):
  nonlocal rejects
  new.put_state(s);before=bytes(new.c.uc.mem_read(new.s,64));new.calls=[];new.c.uc.mem_write(new.services,struct.pack('<QQ',0,0 if missing else new.svc))
  result=new.c.invoke('dh2_character_script_lifecycle',[0 if nullstate else new.s,op,arg,0 if nullservice else new.s if alias else new.services]);assert result&0xffffffff==0xffffffff and before==bytes(new.c.uc.mem_read(new.s,64)) and not new.calls;rejects+=1
 reject(state(),nullstate=True);reject(state(),nullservice=True);reject(state(),missing=True);reject(state(),alias=True);reject(state(),op=10);reject(state(),arg=2)
 for offset,value,width in ((0,0,8),(32,0xffffffff,4),(44,256,4),(48,256,4),(52,1,4),(56,1,4),(60,1,4)):
  raw=bytearray(state());struct.pack_into('<Q' if width==8 else '<I',raw,offset,value);reject(bytes(raw))
 source=['port/level-world/character_script_lifecycle.hpp','port/level-world/character_script_lifecycle.cpp','port/level-world/tests/character_script_lifecycle_differential.py','port/level-world/tests/character_script_lifecycle.cpp']
 report={'validation':'PASS','original_sha256':manifest['original_sha256'],'library_sha256':sha(a.library),'corpus_sha256':sha(a.gold),'comparisons':len(records),'ordered_service_requests':calls,'atomic_rejection_checks':rejects,'mismatches':0,'source_sha256':{f:sha(ROOT/f) for f in source if (ROOT/f).exists()},'reference_sha256':{str(f.relative_to(ROOT)).replace('\\','/'):sha(f) for f in sorted((ROOT/'port/level-world/reference/character-script-lifecycle').rglob('*')) if f.is_file() and f.suffix in ('.json','.asm')},'scope':'Original lifecycle wrappers and staged promotion execute; allocation/AIS virtual/Lua/skills/property/timer bodies are named synchronous services. No whole AIS backend, package or live-game claim.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
