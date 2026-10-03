"""Actual outer CharAI dispatcher, including composed original OnUpdate/AIS calls."""
import argparse,hashlib,itertools,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from character_ai_update_differential import Machine as UpdateMachine,words,word,sha
class Machine(UpdateMachine):
 def __init__(self,path,native,manifest):
  super().__init__(path,native,manifest);c=self.c;d=c.data
  self.frame=d+0x8000;self.frame_out=d+0x9000;self.owners={2:d+0xa000,9:d+0xa080};self.frame_svc=d+0xb000;self.frame_services=d+0xc000;self.bridge=d+0xd000;self.script=d+0xe000;self.script_services=d+0xe100
  self.frame_events=[];self.frame_params=(0,0,0,0,0,3);self.inside=False;self.frame_fail=0xffffffff
  if not native:self.ai=self.ptrs[2]+0x3c8;c.pointer(self.ai,self.vt);c.pointer(self.vt+0x18,0x3d1050)
  else:
   c.uc.mem_write(self.frame_services,struct.pack('<QQII',self.bridge,self.frame_svc,31,0));c.uc.mem_write(self.bridge,struct.pack('<6Q',self.frame,self.s,self.services,self.script,self.script_services,self.out));c.uc.mem_write(self.script_services,struct.pack('<QQ',self.bridge,self.frame_svc+32))
  self.global_blocked=0x9a318b
 def owner_bytes(self,id):
  c=self.c
  if self.native:
   raw=bytes(c.uc.mem_read(self.owners[id],48));p=struct.unpack('<2Q',raw[:16]);return struct.pack('<2Q',*(self.ids[v] for v in p))+raw[16:]
  p=self.ptrs[id];ctrl=word(c,p+0x378)
  return struct.pack('<2Q',id,self.ids[ctrl])+words(word(c,p+0x520),c.uc.mem_read(ctrl+9,1)[0],c.uc.mem_read(ctrl+8,1)[0],c.uc.mem_read(p+0x2ee,1)[0],c.uc.mem_read(p+0x2f0,1)[0],c.uc.mem_read(p+0x88,1)[0],0,0)
 def snapshot_frame(self):
  c=self.c
  if self.native:
   raw=bytes(c.uc.mem_read(self.frame,32));owner=struct.unpack('<Q',raw[8:16])[0];id=next(i for i,v in self.owners.items() if v==owner);state=struct.pack('<2Q',1,id)+raw[16:]
   raw=bytes(c.uc.mem_read(self.script,48));p=struct.unpack('<4Q',raw[:32]);script=struct.pack('<4Q',*(self.ids[v] for v in p))+raw[32:]
  else:
   owner=word(c,self.ai+4);id=self.ids[owner];state=struct.pack('<2Q',1,id)+words(c.uc.mem_read(self.ai+0x18,1)[0],c.uc.mem_read(self.global_blocked,1)[0],0,0)
   script=struct.pack('<4Q',1,2,self.ids[word(c,self.ptrs[2]+0x3c8+4)],3)+words(word(c,self.active+0xbc),c.uc.mem_read(self.ptrs[2]+0x3c8+0x18,1)[0],0,0)
  return state+self.owner_bytes(2)+self.owner_bytes(9)+script
 def put_frame(self,raw):
  c=self.c;id=struct.unpack('<Q',raw[8:16])[0]
  if self.native:
   c.uc.mem_write(self.frame,struct.pack('<2Q',self.ptrs[1],self.owners[id])+raw[16:32])
   for i,id in enumerate((2,9)):
    value=raw[32+48*i:80+48*i];p=struct.unpack('<2Q',value[:16]);c.uc.mem_write(self.owners[id],struct.pack('<2Q',*(self.ptrs[v] for v in p))+value[16:])
   value=raw[128:];p=struct.unpack('<4Q',value[:32]);c.uc.mem_write(self.script,struct.pack('<4Q',*(self.ptrs[v] for v in p))+value[32:])
  else:
   id=struct.unpack('<Q',raw[8:16])[0];c.pointer(self.ai+4,self.ptrs[id]);c.uc.mem_write(self.ai+0x18,bytes([struct.unpack('<I',raw[16:20])[0]]));c.uc.mem_write(self.global_blocked,bytes([struct.unpack('<I',raw[20:24])[0]]))
   for i,id in enumerate((2,9)):
    value=raw[32+48*i:80+48*i];p=struct.unpack('<2Q',value[:16]);v=struct.unpack('<8I',value[16:]);owner=self.ptrs[id];ctrl=self.ptrs[p[1]]
    c.pointer(owner+0x378,ctrl);c.pointer(owner+0x520,v[0]);c.uc.mem_write(ctrl+9,bytes([v[1]]));c.uc.mem_write(ctrl+8,bytes([v[2]]));c.uc.mem_write(owner+0x2ee,bytes([v[3]]));c.uc.mem_write(owner+0x2f0,bytes([v[4]]));c.uc.mem_write(owner+0x88,bytes([v[5]]))
   c.pointer(self.active+0x98,self.ptrs[2]);c.pointer(self.active+0xbc,struct.unpack('<I',raw[160:164])[0])
 def event(self,service,subject,argument=0,position=None,value=0):
  self.frame_events.append(words(service,argument)+struct.pack('<Q',subject)+(position or bytes(12))+words(0)+self.snapshot_frame()+words(value))
 def mutate_frame(self,service):
  mode,trigger=self.frame_params[1:3]
  if not mode or trigger!=service:return
  raw=bytearray(self.snapshot_frame())
  if mode==1:struct.pack_into('<Q',raw,8,9);struct.pack_into('<Q',raw,144,9)
  elif mode==2:struct.pack_into('<I',raw,60,255) # captured owner2.zoned
  elif mode==3:struct.pack_into('<I',raw,64,0) # captured owner2.in_zone
  elif mode==4:struct.pack_into('<I',raw,16,255);struct.pack_into('<I',raw,164,255)
  elif mode==5:struct.pack_into('<I',raw,20,255)
  elif mode==6:struct.pack_into('<I',raw,48,0)
  elif mode==7:struct.pack_into('<Q',raw,8,9);struct.pack_into('<Q',raw,144,9);struct.pack_into('<I',raw,60,255);struct.pack_into('<I',raw,64,0)
  elif mode==8:struct.pack_into('<Q',raw,8,9);struct.pack_into('<Q',raw,144,9);struct.pack_into('<I',raw,108,255);struct.pack_into('<I',raw,112,0)
  else:raise AssertionError(mode)
  self.put_frame(bytes(raw))
 def sync_update(self):
  if not self.native:return
  c=self.c;raw=self.snapshot_frame();id=struct.unpack('<Q',raw[8:16])[0];owner=raw[32+(id==9)*48:80+(id==9)*48];v=struct.unpack('<8I',owner[16:]);seed=struct.pack('<6Q',1,id,0,0,7,8)+words(self.frame_params[5],1,v[3],0,0x80000000,0x7fc01234,0x7f800000,0);self.put_state(seed)
 def request(self,service,subject,argument=0,position=None):
  value=self.params[0] if service==3 else self.params[1] if service==5 else 0;self.event(16+service,subject,argument,position,value);return value
 def hook(self,uc,address,size,unused):
  c=self.c
  if self.native and address==self.frame_svc:
   service,_,subject=struct.unpack('<IIQ',c.uc.mem_read(c.reg(2),16));value=self.frame_params[0] if service==0 else 0;self.event(service,self.ids[subject],value=value);self.mutate_frame(service);c.uc.mem_write(c.reg(3),words(value))
   if service==self.frame_fail:self.finish(17);return
   if service==4 and self.frame_params[3]:self.sync_update();self.inside=True;c.uc.reg_write(c.pc,c.symbols['dh2_ai_frame_fixture_on_update']);return
   self.finish();return
  if not self.native:
   if address==0x3dc798 and self.frame_params[3]:self.request(0,1);return
   if address==0x3d1050:self.inside=True
   if address==self.svc+16 and not self.inside:self.event(0,self.ids[c.reg(0)],value=self.frame_params[0]);self.mutate_frame(0);self.finish(self.frame_params[0]);return
   mapping={0x3cb908:1,0x3cc5a4:2,0x3cf3f0:3}
   if address in mapping:self.event(mapping[address],1);self.mutate_frame(mapping[address]);self.finish();return
   if address==0x3d1050:
    self.event(4,1);self.mutate_frame(4)
    if not self.frame_params[3]:self.inside=False;self.finish();return
   if address in (0x3dbe24,0x40559c):
    service=32 if address==0x3dbe24 else 33;subject=self.ids[c.reg(0)-0x3b4] if service==32 else self.ids[c.reg(0)];self.event(service,subject,1000 if service==32 else 0)
    if service==33:c.pointer(self.ptrs[2]+0x1500,self.frame_params[5])
    self.finish();return
  if self.native and address==self.svc:
   service,arg,subject=struct.unpack('<IIQ',c.uc.mem_read(c.reg(2),16))
   if service==0 and self.frame_params[3]:
    self.request(0,self.ids[subject],arg,bytes(c.uc.mem_read(c.reg(2)+16,12)));c.uc.reg_write(c.pc,c.symbols['dh2_ai_frame_fixture_selected']);return
  if self.native and address==self.frame_svc+32:
   service,_,_,_,subject,_=struct.unpack('<4I2Q',c.uc.mem_read(c.reg(2),32));paused=word(c,self.script+36);c.uc.mem_write(self.frame+16,words(paused));self.event(32+service,self.ids[subject],1000 if service==0 else 0)
   if service==1:c.uc.mem_write(self.s+48,words(self.frame_params[5]))
   self.finish();return
  super().hook(uc,address,size,unused)
 def execute_frame(self,raw,params):
  c=self.c;self.frame_params=params;self.params=(params[0],0,0,0);self.inside=False;self.frame_events=[]
  seed=struct.pack('<6Q',1,2,0,0,7,8)+words(params[5],1,0,0,0x80000000,0x7fc01234,0x7f800000,0)
  self.put_state(seed)
  if not self.native:
   # Second owner projection is genuinely different memory and remains alive.
   saved_ai=self.ai;c.pointer(self.ptrs[9]+0x4fc+0x20,self.ptrs[9]+0x1500);c.pointer(self.ptrs[9]+0x1500,params[5]);c.pointer(self.ptrs[9],self.vt);c.pointer(self.ptrs[9]+0x2d8,self.ptrs[7]);c.uc.mem_write(self.ptrs[9]+0x1450,words(0x80000000,0x7fc01234,0x7f800000));self.ai=saved_ai
   self.c.pointer(self.active,0x966cd8 if params[3] else self.vt);self.c.pointer(self.vt+0x18,0x3d1050)
  self.put_frame(raw)
  if self.native:
   self.c.uc.mem_write(self.services,struct.pack('<QQII',self.bridge,self.svc,255,0));status=self.c.invoke('dh2_character_ai_frame',[self.frame_out,self.frame,self.frame_services]);assert status==0 and word(self.c,self.frame_out)==6
  else:self.c.invoke(0x3cfbf4,[self.ai])
  return self.snapshot_frame(),tuple(self.frame_events)
def seed(paused=0,forced=0,locked=0,blocked=0,flags=0x100,zoned=0,in_zone=0):
 state=struct.pack('<2Q',1,2)+words(paused,blocked,0,0)
 state+=struct.pack('<2Q',2,3)+words(flags,forced,locked,zoned,in_zone,17,0,0)
 state+=struct.pack('<2Q',9,10)+words(0x100,0,0,0,1,19,0,0)
 return state+struct.pack('<4Q',1,2,2,3)+words(0,paused,0,0)

def native_contracts(machine):
 c=machine.c;owner=machine.owners[2]
 regions=[(machine.frame,32),(machine.frame_out,16),(machine.frame_services,24),(owner,48),(machine.owners[9],48)]
 def initialize():
  machine.put_frame(seed());machine.frame_events=[];machine.frame_params=(0,0,0,0,0,3);machine.frame_fail=0xffffffff
  c.uc.mem_write(machine.frame_services,struct.pack('<QQII',machine.bridge,machine.frame_svc,31,0));c.uc.mem_write(machine.frame_out,words(11,12,13,14))
 guards=0
 for kind in range(27):
  initialize();args=[machine.frame_out,machine.frame,machine.frame_services]
  if kind<3:args[kind]=0
  elif kind==3:args[0]=machine.frame
  elif kind==4:args[0]=machine.frame_services
  elif kind==5:args[2]=machine.frame
  elif kind in (6,7,8):c.pointer(machine.frame+8,[machine.frame,machine.frame_services,machine.frame_out][kind-6])
  elif kind==9:c.pointer(machine.frame,0)
  elif kind==10:c.pointer(machine.frame+8,0)
  elif kind in (11,12,13,14):c.uc.mem_write(machine.frame+16+(kind-11)*4,words(256 if kind<13 else 1))
  elif kind in (15,16):c.pointer(owner+(kind-15)*8,0)
  elif 17<=kind<=23:c.uc.mem_write(owner+20+(kind-17)*4,words(256 if kind<=21 else 1))
  elif kind==24:c.pointer(machine.frame_services+8,0)
  elif kind==25:c.uc.mem_write(machine.frame_services+16,words(32))
  elif kind==26:c.uc.mem_write(machine.frame_services+20,words(1))
  before=[bytes(c.uc.mem_read(address,size)) for address,size in regions]
  assert c.invoke('dh2_character_ai_frame',args)==1 and not machine.frame_events,('guard',kind)
  assert before==[bytes(c.uc.mem_read(address,size)) for address,size in regions],('non-atomic',kind)
  guards+=1
 for service in range(5):
  initialize();c.uc.mem_write(machine.frame_services+16,words(31^(1<<service)))
  assert c.invoke('dh2_character_ai_frame',[machine.frame_out,machine.frame,machine.frame_services])==2
  assert word(c,machine.frame_out)==service+1 and word(c,machine.frame_out+8)==service and len(machine.frame_events)==service
 for service in range(5):
  initialize();machine.frame_params=(0,1,service,0,0,3);machine.frame_fail=service
  assert c.invoke('dh2_character_ai_frame',[machine.frame_out,machine.frame,machine.frame_services])==3
  assert word(c,machine.frame_out)==service+1 and word(c,machine.frame_out+8)==service and len(machine.frame_events)==service+1
  assert struct.unpack('<Q',c.uc.mem_read(machine.frame+8,8))[0]==machine.owners[9]
 machine.frame_fail=0xffffffff
 return {'atomic_rejections':guards,'unavailable_services':5,'runtime_failure_prefixes':5}
def main():
 p=argparse.ArgumentParser(description=__doc__)
 for name in ('engine','library','gold','report'):p.add_argument('--'+name,type=Path,required=True)
 a=p.parse_args();manifest=json.loads((ROOT/'port/level-world/reference/character-ai-frame/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256'];old=Machine(a.engine,False,manifest);new=Machine(a.library,True,{'functions':[]});records=[];requests=0;composed=0
 def compare(raw,params):
  nonlocal requests,composed
  expected=old.execute_frame(raw,params);actual=new.execute_frame(raw,params)
  assert expected==actual,(len(records),params,'state',[i for i,(x,y) in enumerate(zip(expected[0],actual[0])) if x!=y],'oldcodes',[struct.unpack('<I',x[:4])[0] for x in expected[1]],'newcodes',[struct.unpack('<I',x[:4])[0] for x in actual[1]],'eventdiffs',[[i for i,(x,y) in enumerate(zip(a,b)) if x!=y] for a,b in zip(expected[1],actual[1])])
  after,events=expected;requests+=len(events);composed+=bool(params[3]);records.append(raw+words(*params)+after+words(len(events))+b''.join(events));return after
 for paused,forced,locked,blocked,flags,zoned,in_zone,zonable in itertools.product((0,1,255),(0,1,255),(0,1,255),(0,1,255),(0,0x100,0xffffffff),(0,1,255),(0,1,255),(0,1,0xffffffff)):
  compare(seed(paused,forced,locked,blocked,flags,zoned,in_zone),(zonable,0,0,0,0,3))
 for service,mutation,zonable in itertools.product(range(5),range(1,9),(0,1)):
  compare(seed(in_zone=1),(zonable,mutation,service,0,0,3))
 pause_skips=0;expiry_cases=0
 for counter,stop_state,zonable,zoned,in_zone in itertools.product((0,199,200,0xffffffff),(3,4),(0,1),(0,1),(0,1)):
  raw=bytearray(seed(zoned=zoned,in_zone=in_zone));struct.pack_into('<I',raw,160,counter);params=(zonable,0,0,1,counter,stop_state);after=compare(bytes(raw),params)
  if struct.unpack('<I',after[16:20])[0]:
   paused_after=compare(after,(zonable,0,0,1,0,stop_state));assert paused_after==after;pause_skips+=1
   old.c.invoke(0x3cbb34,[old.ai,0x31,0]);assert new.c.invoke('dh2_character_script_pause_expired',[new.script])==1;new.c.uc.mem_write(new.frame+16,words(word(new.c,new.script+36)))
   expired=old.snapshot_frame();assert expired==new.snapshot_frame() and word(old.c,old.active+0xbc)==0;expiry_cases+=1
   compare(expired,(zonable,0,0,1,0,stop_state))
 contracts=native_contracts(new)
 blob=b'CAF1'+words(len(records))+b''.join(records);a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(blob)
 report={'validation':'PASS','original_sha256':manifest['original_sha256'],'library_sha256':sha(a.library),'corpus_sha256':sha(a.gold),'comparisons':len(records),'composed_original_cases':composed,'pause_skip_frames':pause_skips,'pause_expiry_cases':expiry_cases,'ordered_service_requests':requests,'mismatches':0,'source_sha256':{str(f.relative_to(ROOT)).replace('\\','/'):sha(f) for f in [ROOT/'port/level-world/character_ai_frame.hpp',ROOT/'port/level-world/character_ai_frame.cpp',Path(__file__),ROOT/'port/level-world/tests/character_ai_frame_bridge.cpp',ROOT/'port/level-world/character_ai_update.cpp',ROOT/'port/level-world/character_script_update.cpp']},'scope':__doc__+' Target/master/aggro, owner virtual and timer/Stop leaf dependencies are explicit. Original release profiler no-op bodies execute. Composed cases execute actual outer/fullOnUpdate/AISDefault/pause instructions and native equivalents; no Lua/target/aggro/world/complete frame backend claim.'}
 report['native_caller_contracts']=contracts
 ref=ROOT/'port/level-world/reference/character-ai-frame'
 report['reference_sha256']={str(f.relative_to(ROOT)).replace('\\','/'):sha(f) for f in [ref/'original-functions.json',ref/'reference/original-functions.asm',ref/'debug-producer.json',ref/'producers/original-functions.json',ref/'producers/reference/original-functions.asm']}
 report['composed_instruction_evidence']={name:sha(ROOT/'port/level-world/reports'/name) for name in ['character-ai-update-arm64-differential.json','character-script-update-arm64-differential.json']}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
