"""Selected AISPlayerIPhone virtual executes original AISDefault OnUpdate.

Timer Start and complete controller Cmd_Stop remain synchronous services.
Actual AI_PauseUpdate and selected dispatch prefix execute original instructions.
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
class Machine:
 def __init__(self,path,native,manifest):
  self.c=TimelineCpu(path,native,manifest);self.native=native;c=self.c;d=c.data
  self.s=d+0x1000;self.svc=d+0x2000;self.services=d+0x3000;self.ais=d+0x4000;self.ai=d+0x5000
  self.ptrs={0:0,1:d+0x10000,2:d+0x20000,3:d+0x30000,4:d+0x40000,5:d+0x50000};self.ids={v:k for k,v in self.ptrs.items()}
  c.uc.mem_write(self.services,struct.pack('<QQ',d+0x6000,self.svc))
  # Real selected vtable is installed, rather than substituting a callback.
  if not native:c.pointer(self.ais,0x966cd0+8)
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def snapshot(self):
  c=self.c
  if self.native:
   raw=bytes(c.uc.mem_read(self.s,48));p=struct.unpack('<4Q',raw[:32]);return struct.pack('<4Q',*(self.ids[v] for v in p))+raw[32:]
  owner=word(c,self.ais+0x98);ai_owner=word(c,owner+0x3c8+4);ctrl=word(c,owner+0x378)
  return struct.pack('<4Q',1 if word(c,self.ai+0x1c) else 0,self.ids[owner],self.ids[ai_owner],self.ids[ctrl])+words(word(c,self.ais+0xbc),c.uc.mem_read(owner+0x3c8+0x18,1)[0],0,0)
 def put_state(self,raw):
  c=self.c;p=struct.unpack('<4Q',raw[:32]);v=struct.unpack('<4I',raw[32:])
  if self.native:c.uc.mem_write(self.s,struct.pack('<4Q',*(self.ptrs[i] for i in p))+raw[32:])
  else:
   c.pointer(self.ai+0x1c,self.ais if p[0] else 0);c.pointer(self.ais+0x98,self.ptrs[p[1]]);c.pointer(self.ptrs[p[1]]+0x3c8+4,self.ptrs[p[2]]);c.pointer(self.ptrs[p[1]]+0x378,self.ptrs[p[3]]);c.pointer(self.ais+0xbc,v[0]);c.uc.mem_write(self.ptrs[p[1]]+0x3c8+0x18,bytes([v[1]]))
 def request(self,service,duration=0,repeat=0,event=0,subject=0,user=0):
  self.calls.append(words(service,duration,repeat,event)+struct.pack('<QQ',subject,user)+self.snapshot())
  if service==self.params[1]:
   raw=bytearray(self.snapshot());mutation=self.params[0]
   if mutation==1:struct.pack_into('<Q',raw,24,5)
   elif mutation==2:struct.pack_into('<I',raw,32,300)
   elif mutation==3:struct.pack_into('<I',raw,36,0)
   elif mutation==4:
    struct.pack_into('<Q',raw,8,4);struct.pack_into('<Q',raw,24,5)
   self.put_state(bytes(raw))
 def finish(self):
  c=self.c;c.put(0,17);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def hook(self,uc,address,size,unused):
  c=self.c
  if self.native:
   if address!=self.svc:return
   v=struct.unpack('<4I2Q',c.uc.mem_read(c.reg(2),32));self.request(*v[:4],self.ids[v[4]],self.ids[v[5]]);self.finish();return
  if address==0x3d1078 and self.dispatch:c.uc.reg_write(c.pc,0x3d10c0);return
  if address==0x3dbe24:
   assert c.reg(1)==1000 and c.reg(2)==0 and c.reg(3)==0x31
   user=word(c,c.uc.reg_read(c.sp));assert user==0
   self.request(0,1000,0,0x31,self.ids[c.reg(0)-0x3b4],0);self.finish()
  elif address==0x40559c:self.request(1,subject=self.ids[c.reg(0)]);self.finish()
 def execute(self,dispatch,raw,params):
  self.put_state(raw);self.params=params;self.calls=[];self.dispatch=dispatch
  self.c.invoke('dh2_character_script_update' if self.native else 0x3d1050 if dispatch else 0x3dc798,[self.s,dispatch,self.services] if self.native else [self.ai if dispatch else self.ais])
  return self.snapshot(),tuple(self.calls)
def main():
 p=argparse.ArgumentParser()
 for key in ('engine','library','gold','report'):p.add_argument('--'+key,type=Path,required=True)
 a=p.parse_args();manifest=json.loads((ROOT/'port/level-world/reference/character-script-update/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256']
 old=Machine(a.engine,False,manifest);new=Machine(a.library,True,{'functions':[]});records=[];requests=0;rng=random.Random(0xbc18)
 def state(counter,active=1,paused=0):return struct.pack('<4Q',active,2,2,3)+words(counter,paused,0,0)
 for dispatch in (0,1):
  for active in (0,1):
   for counter in (0,1,198,199,200,201,1000,0x7fffffff,0x80000000,0xffffffff):
    for paused in (0,1,255):
     for mutation in range(5):
      params=(mutation,rng.randrange(2));s=state(counter,active,paused);expected=old.execute(dispatch,s,params);actual=new.execute(dispatch,s,params);assert expected==actual,(len(records),dispatch,counter,params,expected,actual)
      after,events=expected;requests+=len(events);records.append(words(dispatch)+s+words(*params)+after+words(len(events))+b''.join(events))
 expiry=[]
 for paused in (0,1,255):
  s=state(200,1,paused);old.put_state(s);new.put_state(s);old.calls=[];new.calls=[]
  old.c.invoke(0x3cbb34,[old.ptrs[2]+0x3c8,0x31,0]);result=new.c.invoke('dh2_character_script_pause_expired',[new.s]);expected=old.snapshot();actual=new.snapshot()
  assert result==1 and expected==actual and not old.calls and not new.calls
  expiry.append(s+expected)
 blob=b'ASU1'+words(len(records))+b''.join(records)+words(len(expiry))+b''.join(expiry);a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(blob)
 rejects=0
 def reject(raw,dispatch=0,missing=False,nullstate=False,nullservice=False,alias=False):
  nonlocal rejects
  new.put_state(raw);before=bytes(new.c.uc.mem_read(new.s,48));new.calls=[];new.c.uc.mem_write(new.services,struct.pack('<QQ',0,0 if missing else new.svc))
  result=new.c.invoke('dh2_character_script_update',[0 if nullstate else new.s,dispatch,0 if nullservice else new.s if alias else new.services]);assert result&0xffffffff==0xffffffff and before==bytes(new.c.uc.mem_read(new.s,48)) and not new.calls;rejects+=1
 reject(state(200),nullstate=True);reject(state(200),nullservice=True);reject(state(200),missing=True);reject(state(200),alias=True);reject(state(200),dispatch=2)
 for offset,value,width in ((8,0,8),(16,0,8),(24,0,8),(36,256,4),(40,1,4),(44,1,4)):
  raw=bytearray(state(200));struct.pack_into('<Q' if width==8 else '<I',raw,offset,value);reject(bytes(raw))
 expiry_rejects=0
 for offset,value,width in ((8,0,8),(16,0,8),(24,0,8),(36,256,4),(40,1,4),(44,1,4)):
  raw=bytearray(state(200));struct.pack_into('<Q' if width==8 else '<I',raw,offset,value);new.put_state(bytes(raw));before=bytes(new.c.uc.mem_read(new.s,48));new.calls=[]
  result=new.c.invoke('dh2_character_script_pause_expired',[new.s]);assert result&0xffffffff==0xffffffff and before==bytes(new.c.uc.mem_read(new.s,48)) and not new.calls;expiry_rejects+=1
 assert new.c.invoke('dh2_character_script_pause_expired',[0])&0xffffffff==0xffffffff;expiry_rejects+=1
 sources=['port/level-world/character_script_update.hpp','port/level-world/character_script_update.cpp','port/level-world/tests/character_script_update_differential.py','port/level-world/tests/character_script_update.cpp']
 report={'validation':'PASS','original_sha256':manifest['original_sha256'],'library_sha256':sha(a.library),'corpus_sha256':sha(a.gold),'comparisons':len(records),'ordered_service_requests':requests,'pause_expiry_comparisons':len(expiry),'atomic_rejection_checks':rejects,'pause_expiry_atomic_rejections':expiry_rejects,'mismatches':0,'source_sha256':{f:sha(ROOT/f) for f in sources if (ROOT/f).exists()},'reference_sha256':{str(f.relative_to(ROOT)).replace('\\','/'):sha(f) for f in sorted((ROOT/'port/level-world/reference/character-script-update').rglob('*')) if f.is_file() and f.suffix in ('.json','.asm','.luac')},'scope':'Full selected AISDefault OnUpdate and actual AI_PauseUpdate/selected-iPhone-vtable dispatch prefix execute. Timer31 executes actual RaiseAIEvent and returns without forwarding. Timer Start and Cmd_Stop are explicit synchronous services; remainder of CharAI OnUpdate, collision producer and Lua backend remain separate.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
