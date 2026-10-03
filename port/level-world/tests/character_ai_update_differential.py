"""Complete original CharAI::OnUpdate ordering with explicit synchronous services."""
import argparse,hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from visual_timeline_differential import TimelineCpu
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def word(c,p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Machine:
 def __init__(self,path,native,manifest):
  self.c=TimelineCpu(path,native,manifest);self.native=native;c=self.c;d=c.data
  self.s=d+0x1000;self.out=d+0x2000;self.svc=d+0x3000;self.services=d+0x4000;self.ai=d+0x5000;self.vt=d+0x6000;self.active=d+0x7000
  self.ptrs={0:0,**{i:d+0x10000*i for i in range(1,13)}};self.ids={v:k for k,v in self.ptrs.items()};self.events=[];self.params=(0,0,0,0);self.producer=False;self.char_type=1
  c.uc.mem_write(self.services,struct.pack('<QQII',0,self.svc,255,0))
  if not native:
   c.pointer(self.active,self.vt);c.pointer(self.vt+0x18,self.svc)
   c.pointer(self.vt+0xc4,self.svc+16);c.pointer(self.vt+0x34,self.svc+32);c.pointer(self.vt+0xa4,self.svc+48)
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def snapshot(self):
  c=self.c
  if self.native:
   raw=bytes(c.uc.mem_read(self.s,80));p=struct.unpack('<6Q',raw[:48]);return struct.pack('<6Q',*(self.ids[v] for v in p))+raw[48:]
  owner=word(c,self.ai+4);visual=word(c,owner+0x2d8);node=word(c,visual+8) if visual else 0;machine=word(c,owner+0x4fc+0x20)
  p=(1 if word(c,self.ai+0x1c) else 0,self.ids[owner],self.ids[word(c,owner+0x408)],self.ids[word(c,owner+0x418)],self.ids[visual],self.ids[node])
  state=word(c,machine) if machine else self.state_ignored
  return struct.pack('<6Q',*p)+words(state,bool(machine),c.uc.mem_read(owner+0x2ee,1)[0],c.uc.mem_read(owner+0x85,1)[0])+bytes(c.uc.mem_read(owner+0x1450,12))+words(0)
 def put_state(self,raw):
  raw=bytes(raw);c=self.c;p=struct.unpack('<6Q',raw[:48]);v=struct.unpack('<8I',raw[48:]);self.state_ignored=v[0]
  if self.native:c.uc.mem_write(self.s,struct.pack('<6Q',*(self.ptrs[i] for i in p))+raw[48:]);return
  owner=self.ptrs[p[1]];visual=self.ptrs[p[4]];node=self.ptrs[p[5]];c.pointer(self.ai+4,owner);c.pointer(self.ai+0x1c,self.active if p[0] else 0)
  c.pointer(owner,self.vt);c.pointer(owner+0x408,self.ptrs[p[2]]);c.pointer(owner+0x418,self.ptrs[p[3]]);c.pointer(owner+0x2d8,visual)
  if visual:c.pointer(visual+8,node)
  if node:c.pointer(node,self.vt)
  c.pointer(owner+0x4fc+0x20,owner+0x1500 if v[1] else 0);c.pointer(owner+0x1500,v[0]);c.uc.mem_write(owner+0x2ee,bytes([v[2]]));c.uc.mem_write(owner+0x85,bytes([v[3]]));c.uc.mem_write(owner+0x1450,raw[64:76]);c.uc.mem_write(owner+0x1449,bytes([self.params[1]&255]))
 def mutation(self,service):
  mode,trigger=self.params[2:];raw=bytearray(self.snapshot())
  if mode==0 or trigger!=service:return
  if mode==1:struct.pack_into('<Q',raw,8,9) # owner reloaded after callbacks
  elif mode==2:struct.pack_into('<Q',raw,32,10);struct.pack_into('<Q',raw,40,11)
  elif mode==3:struct.pack_into('<I',raw,60,255)
  elif mode==4:raw[64:76]=words(0x80000000,0x7fc12345,0x7f800000)
  elif mode==5:struct.pack_into('<i',raw,48,4)
  elif mode==6:struct.pack_into('<Q',raw,16,5)
  elif mode==7:struct.pack_into('<Q',raw,40,0)
  elif mode==8:struct.pack_into('<I',raw,56,255)
  elif mode==9:struct.pack_into('<I',raw,60,0)
  else:raise AssertionError(mode)
  self.put_state(raw)
 def request(self,service,subject,argument=0,position=None):
  value=self.params[0] if service==3 else self.params[1] if service==5 else 0
  request=words(service,argument)+struct.pack('<Q',subject)+(position or bytes(12))+words(0)
  self.events.append(request+self.snapshot()+words(value));self.mutation(service);return value
 def finish(self,value=0):
  c=self.c;c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def hook(self,uc,address,size,unused):
  c=self.c
  if self.native:
   if address!=self.svc:return
   request=bytes(c.uc.mem_read(c.reg(2),32));service,arg,subject=struct.unpack('<IIQ',request[:16]);result=self.request(service,self.ids[subject],arg,request[16:28]);c.uc.mem_write(c.reg(3),words(result));self.finish();return
  if self.producer:
   if address==0x3a3024:c.pointer(self.s+0x838,self.char_type);self.finish(self.s+0x800);return
   if address in (0x3a36e4,0x3a2ed4):self.request(3 if address==0x3a36e4 else 5,self.ids[c.reg(0)]);return
  if address==self.svc:self.finish(self.request(0,1));return
  if address in (self.svc+16,self.svc+32):self.finish(self.request(3 if address==self.svc+16 else 5,self.ids[c.reg(0)]));return
  if address==self.svc+48:self.finish(self.request(7,self.ids[c.reg(0)],position=bytes(c.uc.mem_read(c.reg(1),12))));return
  mapping={0x38c600:1,0x4713d0:2,0x38c790:4,0x393db4:6}
  if address in mapping:
   service=mapping[address];self.finish(self.request(service,self.ids[c.reg(0)],c.reg(2) if service==6 else 0,bytes(c.uc.mem_read(c.reg(1),12)) if service==6 else None))
 def execute(self,raw,params):
  self.events=[];self.params=params;self.put_state(raw)
  if not self.native:
   self.c.pointer(self.vt+0xc4,0x3a36e4 if self.producer else self.svc+16);self.c.pointer(self.vt+0x34,0x3a2ed4 if self.producer else self.svc+32);self.c.pointer(self.vt+0x28,0x3a49f0)
  if self.native:
   self.c.uc.mem_write(self.services,struct.pack('<QQII',0,self.svc,255,0));status=self.c.invoke('dh2_character_ai_update',[self.out,self.s,self.services]);assert status==0;out=struct.unpack('<4I',self.c.uc.mem_read(self.out,16));assert out==(9,struct.unpack('<I',self.events[-1][:4])[0] if self.events else 0xffffffff,len(self.events),0)
  else:self.c.invoke(0x3d1050,[self.ai])
  return self.snapshot(),tuple(self.events)
def state(active,current,targets,zoned,flag,visual,machine=1):
 return struct.pack('<6Q',active,2,5 if targets&1 else 0,6 if targets&2 else 0,7 if visual else 0,8 if visual==2 else 0)+words(current,machine,zoned,flag,0x80000000,0x7fc01234,0x7f800000,0)
def main():
 p=argparse.ArgumentParser(description=__doc__)
 for name in ('engine','library','gold','report'):p.add_argument('--'+name,type=Path,required=True)
 a=p.parse_args();manifest=json.loads((ROOT/'port/level-world/reference/character-ai-update/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256']
 old=Machine(a.engine,False,manifest);new=Machine(a.library,True,{'functions':[]});records=[];events=0;service_counts=[0]*8
 def compare(raw,params):
  nonlocal events
  expected=old.execute(raw,params);actual=new.execute(raw,params);assert expected==actual,(len(records),params,raw.hex(),expected,actual)
  after,calls=expected;events+=len(calls)
  for call in calls:service_counts[struct.unpack('<I',call[:4])[0]]+=1
  records.append(raw+words(*params)+after+words(len(calls))+b''.join(calls))
 for active,current,targets,zoned,flag,visual,queries in itertools.product((0,1),(-1,0,3,4,12,13,17,18,19),(0,1,2,3),(0,1,255),(0,255),(0,1,2),((0,0),(1,0),(1,1))):compare(state(active,current,targets,zoned,flag,visual),(*queries,0,0))
 for current in (-1,3,17,18,2147483647):compare(state(1,current,0,0,0,2,0),(0xffffffff,0xffffffff,0,0))
 for trigger in range(8):
  for mutation in range(1,10):
   for raw in (state(1,3,0,0,255 if mutation==9 else 0,2),state(1,4,0,0,0,2)):
    compare(raw,(0x80000000,0,mutation,trigger))
 # Complete full32-bit virtual query results and raw IEEE payloads.
 for zonable,dead in itertools.product((0,1,2,255,0x80000000,0xffffffff),repeat=2):compare(state(1,3,0,0,0,2),(zonable,dead,0,0))
 # Genuine Character v+c4/+34 producer bodies and nested IsPlayer/IsFaerie
 # execute. Only GetCharAI's resolved row pointer is a supplied table service.
 old.producer=True;producer_cases=0
 for char_type,current,dead in itertools.product((1,2,3),(3,17,4),(0,1,255)):
  old.char_type=char_type;compare(state(1,current,0,0,0,2),(int(char_type==2),dead,0,0));producer_cases+=1
 old.producer=False
 blob=b'CAU1'+words(len(records))+b''.join(records);a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(blob)
 native_guards=0
 def reject(raw,svc=None,null=None,alias=None):
  nonlocal native_guards
  new.put_state(raw);new.c.uc.mem_write(new.out,b'R'*16);new.events=[];new.c.uc.mem_write(new.services,svc or struct.pack('<QQII',0,new.svc,255,0));before=bytes(new.c.uc.mem_read(new.s,80));args=[new.out,new.s,new.services]
  if null is not None:args[null]=0
  if alias=='state':args[0]=new.s
  if alias=='services':args[0]=new.services
  if alias=='input':args[2]=new.s
  status=new.c.invoke('dh2_character_ai_update',args);assert status==1 and before==bytes(new.c.uc.mem_read(new.s,80)) and bytes(new.c.uc.mem_read(new.out,16))==b'R'*16 and not new.events;native_guards+=1
 raw=state(1,3,0,0,0,2)
 for null in range(3):reject(raw,null=null)
 for alias in ('state','services','input'):reject(raw,alias=alias)
 for offset,value,width in ((8,0,8),(52,2,4),(56,256,4),(60,256,4),(76,1,4)):
  bad=bytearray(raw);struct.pack_into('<Q' if width==8 else '<I',bad,offset,value);reject(bytes(bad))
 for svc in (struct.pack('<QQII',0,0,255,0),struct.pack('<QQII',0,new.svc,256,0),struct.pack('<QQII',0,new.svc,255,1)):reject(raw,svc=svc)
 unavailable=0
 for service in range(8):
  seed=state(1,4 if service in (1,2) else 3,0,0,0,2);new.put_state(seed);new.params=(1,0,0,0);new.events=[];new.c.uc.mem_write(new.services,struct.pack('<QQII',0,new.svc,255^(1<<service),0));status=new.c.invoke('dh2_character_ai_update',[new.out,new.s,new.services]);assert status==2 and word(new.c,new.out)==service+1 and word(new.c,new.out+4)==service;unavailable+=1
 sources=['port/level-world/character_ai_update.hpp','port/level-world/character_ai_update.cpp','port/level-world/tests/character_ai_update_differential.py','port/level-world/tests/character_ai_update.cpp','port/level-world/tools/build_character_ai_update_oracle.ps1']
 report={'validation':'PASS','original_sha256':manifest['original_sha256'],'library_sha256':sha(a.library),'corpus_sha256':sha(a.gold),'comparisons':len(records),'ordered_service_requests':events,'service_counts':service_counts,'character_virtual_producer_cases':producer_cases,'atomic_rejection_checks':native_guards,'unavailable_service_checks':unavailable,'mismatches':0,'source_sha256':{f:sha(ROOT/f) for f in sources if (ROOT/f).exists()},'reference_sha256':{str(f.relative_to(ROOT)).replace('\\','/'):sha(f) for f in sorted((ROOT/'port/level-world/reference/character-ai-update').rglob('*')) if f.is_file() and f.suffix in ('.json','.asm')},'scope':__doc__+' Actual state predicates/GetState execute;27cases also execute actual Character IsZonable/IsDead/IsPlayer/IsFaerie against a supplied resolved AI row. Eight backend boundaries remain explicit, not missing-service no-ops; owner/visual/state/target/IEEE projection mutations are synchronous. No complete AIS/Lua/zoning/position backend or outer CharAI::Update parity claim.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
