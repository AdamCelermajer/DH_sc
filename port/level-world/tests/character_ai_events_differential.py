"""Complete original RaiseAIEvent switches versus genuine optimized ARM64."""
import argparse,itertools,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from character_ai_update_differential import TimelineCpu,words,word,sha

HELPERS={0x3cb77c,0x3df3f0,0x394a3c,0x3d4434,0x3d3d30,0x3d3d4c,0x3d3ff8,0x3d4204,0x3d8038,0x3d808c,0x3d8b28,0x3d8b7c}
class Machine:
 def __init__(self,path,native,manifest):
  self.c=TimelineCpu(path,native,manifest);self.native=native;c=self.c;d=c.data
  self.s=d+0x1000;self.out=d+0x1800;self.owner={2:d+0x2000,3:d+0x2100};self.services=d+0x2800;self.payload=d+0x2900;self.nested_out=d+0x3000;self.nested_payload=d+0x3100;self.svc=d+0x4000
  self.ptrs={0:0,**{i:d+i*0x10000 for i in range(1,17)}};self.ids={v:k for k,v in self.ptrs.items()}
  self.fn={token:self.svc+token*4 for token in range(100,451)};self.fnids={v:k for k,v in self.fn.items()}
  self.tables={4:100,5:200,7:300,8:400};self.ai=self.ptrs[1];self.events=[];self.params=(0,3,1,0,0,0);self.event_id=0;self.fail=0xffffffff
  for tid,base in self.tables.items():
   for i in range(51):c.pointer(self.ptrs[tid]+i*c.word_size,self.fn[base+i])
  if native:
   c.uc.mem_write(self.services,struct.pack('<QQII',0,self.svc,63,0));c.uc.mem_write(self.nested_payload,bytes(24))
  else:
   for oid,fsm,props in ((2,12,14),(3,13,15)):
    self.ids[self.ptrs[oid]+0x4fc]=fsm;self.ids[self.ptrs[oid]+0x560]=props
   c.pointer(self.ptrs[16],self.ptrs[16]+0x100);c.pointer(self.ptrs[16]+0x100,self.fn[450])
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ident(self,value):return self.ids.get(value,self.fnids.get(value,value))
 def snapshot(self):
  c=self.c
  if self.native:
   raw=bytes(c.uc.mem_read(self.s,64));p=list(struct.unpack('<5Q',raw[:40]));p[0]=self.ident(p[0]);p[1]=next(k for k,v in self.owner.items() if v==p[1]);p[2:]=[self.ident(v) for v in p[2:]];result=struct.pack('<5Q',*p)+raw[40:]
   for oid in (2,3):
    raw=bytes(c.uc.mem_read(self.owner[oid],48));result+=struct.pack('<4Q',*(self.ident(v) for v in struct.unpack('<4Q',raw[:32])))+raw[32:]
   return result
  active=word(c,self.ai+0x1c);table=word(c,active) if active else self.ptrs[7]
  result=struct.pack('<5Q',1,self.ident(word(c,self.ai+4)),self.ident(word(c,self.ai)),self.ident(active),self.ident(table))+words(c.uc.mem_read(self.ai+0x18,1)[0],c.uc.mem_read(self.ai+0x4a,1)[0],c.uc.mem_read(0x9a318b,1)[0],0,0,0)
  for oid,ctrl,fsm,props in ((2,10,12,14),(3,11,13,15)):
   controller=word(c,self.ptrs[oid]+0x378);result+=struct.pack('<4Q',oid,self.ident(controller),fsm,props)+words(c.uc.mem_read(controller+9,1)[0],c.uc.mem_read(controller+8,1)[0],0,0)
  return result
 def put(self,raw):
  c=self.c;p=struct.unpack('<5Q',raw[:40]);v=struct.unpack('<6I',raw[40:64])
  if self.native:
   c.uc.mem_write(self.s,struct.pack('<5Q',self.ptrs[p[0]],self.owner[p[1]],*(self.ptrs[x] for x in p[2:]))+raw[40:64])
   for i,oid in enumerate((2,3)):
    part=raw[64+48*i:112+48*i];c.uc.mem_write(self.owner[oid],struct.pack('<4Q',*(self.ptrs[x] for x in struct.unpack('<4Q',part[:32])))+part[32:])
  else:
   c.pointer(self.ai,self.ptrs[p[2]]);c.pointer(self.ai+4,self.ptrs[p[1]]);c.pointer(self.ai+0x1c,self.ptrs[p[3]]);c.pointer(self.ptrs[6],self.ptrs[p[4]])
   c.uc.mem_write(self.ai+0x18,bytes([v[0]]));c.uc.mem_write(self.ai+0x4a,bytes([v[1]]));c.uc.mem_write(0x9a318b,bytes([v[2]]))
   for i,oid in enumerate((2,3)):
    part=raw[64+48*i:112+48*i];controller=self.ptrs[struct.unpack('<Q',part[8:16])[0]];v=struct.unpack('<4I',part[32:]);c.pointer(self.ptrs[oid]+0x378,controller);c.uc.mem_write(controller+9,bytes([v[0]]));c.uc.mem_write(controller+8,bytes([v[1]]))
 def mutation(self,service):
  mode,trigger=self.params[3:5]
  if not mode or trigger!=service:return
  raw=bytearray(self.snapshot())
  if mode==1:struct.pack_into('<Q',raw,8,3)
  elif mode==2:struct.pack_into('<Q',raw,16,5)
  elif mode==3:struct.pack_into('<Q',raw,24,0)
  elif mode==4:struct.pack_into('<Q',raw,24,6);struct.pack_into('<Q',raw,32,8)
  elif mode==5:struct.pack_into('<I',raw,40,255)
  elif mode==6:struct.pack_into('<I',raw,44,255)
  elif mode==7:struct.pack_into('<I',raw,48,255)
  elif mode==8:struct.pack_into('<I',raw,96,255);struct.pack_into('<I',raw,100,255)
  else:raise AssertionError(mode)
  self.put(bytes(raw))
 def record(self,service,operation,subject,callee=0,payload=0,argument=0,event=None):
  value=self.params[0] if service==2 else self.params[1] if service==4 else self.params[2] if service==3 else 0
  request=words(service,operation,self.event_id if event is None else event,argument)+struct.pack('<3Q',subject,callee,payload)
  self.events.append(request+self.snapshot()+words(value));self.mutation(service);return value
 def finish(self,value=0):c=self.c;c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def reenter(self):
  c=self.c
  if self.native:
   for i,value in enumerate((self.nested_out,self.s,0x31,self.nested_payload,self.services)):c.put(i,value)
   c.uc.reg_write(c.pc,c.symbols['dh2_character_ai_event'])
  else:c.put(0,self.ai);c.put(1,0x31);c.put(2,0);c.uc.reg_write(c.pc,0x3cbb34)
 def relay(self,argument):
  c=self.c
  if self.native:
   for i,value in enumerate((self.nested_out,self.s,argument,self.services)):c.put(i,value)
   c.uc.reg_write(c.pc,c.symbols['dh2_character_ai_event_script_timer'])
  else:c.uc.reg_write(c.pc,0x3d0ca0)
 def hook(self,uc,address,size,unused):
  c=self.c
  if self.native:
   if address!=self.svc:return
   service,operation,event,argument,subject,callee,payload=struct.unpack('<4I3Q',c.uc.mem_read(c.reg(2),40))
   value=self.record(service,operation,self.ident(subject),self.ident(callee),self.ident(payload),argument,event);c.uc.mem_write(c.reg(3),words(value))
   if service==self.fail:self.finish(17);return
   if service==1 and operation==0x90 and self.params[5]==1:self.relay(argument);return
   if service==1 and self.params[5]==2:self.reenter();return
   self.finish();return
  if address in self.fnids:
   token=self.fnids[address]
   if token==450:self.finish(self.record(2,0,16,450));return
   service=5 if token>=300 else 1;slot=(token%100)*4
   has_payload=slot in (0x24,0x28,0x2c,0x30,0x34,0x80,0xb0,0xbc,0xc0,0xc4,0xc8,0x20)
   argument=c.reg(1) if slot==0x90 else c.reg(2) if slot in (0xbc,0xc0,0xc4,0xc8,0x20) else 0
   self.record(service,slot,self.ident(c.reg(0)),token,self.ident(c.reg(1)) if has_payload else 0,argument)
   if service==1 and slot==0x90 and self.params[5]==1:self.relay(argument);return
   if service==1 and self.params[5]==2:self.reenter();return
   self.finish();return
  if address in HELPERS:
   payload=self.ident(c.reg(1)) if address==0x3d4434 else 0;self.finish(self.record(3,address,self.ident(c.reg(0)),address,payload));return
  if address==0x3c01ac:self.finish(self.record(4,0,self.ident(c.reg(0))));return
  if address==0x3c5684:self.record(0,0,self.ident(c.reg(0)),payload=self.ident(c.reg(2)),event=c.reg(1));self.finish();return
 def execute(self,raw,event,payload,params):
  self.put(raw);self.event_id=event;self.params=params;self.events=[];c=self.c
  if self.native:
   c.uc.mem_write(self.payload,struct.pack('<QQII',self.ptrs[payload],self.fn[450],0,0));status=c.invoke('dh2_character_ai_event',[self.out,self.s,event,self.payload,self.services]);assert status==0 and word(c,self.out)==7
  else:c.invoke(0x3cbb34,[self.ai,event,self.ptrs[payload]])
  return self.snapshot(),tuple(self.events)

def seed(forced=0,locked=0,blocked=0,active=6,paused=255,seeking=0):
 return struct.pack('<5Q',1,2,4,active,7)+words(paused,seeking,blocked,0,0,0)+struct.pack('<4Q',2,10,12,14)+words(forced,locked,0,0)+struct.pack('<4Q',3,11,13,15)+words(0,0,0,0)

def native_contracts(machine):
 c=machine.c;owner=machine.owner[2]
 regions=[(machine.s,64),(machine.out,16),(machine.payload,24),(machine.services,24),(owner,48),(machine.owner[3],48)]
 def initialize():
  machine.put(seed());machine.events=[];machine.params=(0,3,1,0,0,0);machine.event_id=0x35;machine.fail=0xffffffff
  c.uc.mem_write(machine.payload,struct.pack('<QQII',machine.ptrs[16],machine.fn[450],0,0));c.uc.mem_write(machine.services,struct.pack('<QQII',0,machine.svc,63,0));c.uc.mem_write(machine.out,words(11,12,13,14))
 def args(event=0x35):return [machine.out,machine.s,event,machine.payload,machine.services]
 for kind in range(46):
  initialize();call=args()
  if kind<=3:call[(0,1,3,4)[kind]]=0
  elif kind in (4,5,6,7):call[0]=(machine.s,machine.services,machine.payload,owner)[kind-4]
  elif kind==8:call[1]=machine.services
  elif kind==9:call[4]=machine.s
  elif 10<=kind<=13:call[3]=(machine.out,machine.services,machine.s,owner)[kind-10]
  elif 14<=kind<=17:c.pointer(machine.s+8,(machine.s,machine.services,machine.out,machine.payload)[kind-14])
  elif kind==18:c.pointer(machine.s,0)
  elif kind==19:c.pointer(machine.s+8,0)
  elif 20<=kind<=25:c.uc.mem_write(machine.s+40+4*(kind-20),words(256 if kind<=22 else 1))
  elif 26<=kind<=29:c.pointer(owner+8*(kind-26),0)
  elif 30<=kind<=33:c.uc.mem_write(owner+32+4*(kind-30),words(256 if kind<=31 else 1))
  elif kind==34:c.pointer(machine.services+8,0)
  elif kind==35:c.uc.mem_write(machine.services+16,words(64))
  elif kind==36:c.uc.mem_write(machine.services+20,words(1))
  elif kind in (37,38):c.uc.mem_write(machine.payload+16+4*(kind-37),words(1))
  elif 39<=kind<=43:c.pointer(machine.s+16,(machine.s,machine.services,owner,machine.payload,machine.out)[kind-39])
  elif kind in (44,45):c.pointer(machine.s+32,(machine.s,owner)[kind-44])
  before=[bytes(c.uc.mem_read(address,size)) for address,size in regions]
  assert c.invoke('dh2_character_ai_event',call)==1 and not machine.events,('guard',kind)
  assert before==[bytes(c.uc.mem_read(address,size)) for address,size in regions],('partial reject',kind)
 event_for_service=(0,2,0x35,0x33,0x1d,0x35)
 for service in range(6):
  initialize();machine.event_id=event_for_service[service];c.uc.mem_write(machine.services+16,words(63^(1<<service)))
  symbol='dh2_character_ai_event_script_timer' if service==5 else 'dh2_character_ai_event'
  call=[machine.out,machine.s,0x80000000,machine.services] if service==5 else args(machine.event_id)
  assert c.invoke(symbol,call)==2 and word(c,machine.out)==service+1 and word(c,machine.out+4)==service and not machine.events
 for service in range(6):
  initialize();machine.event_id=event_for_service[service];machine.params=(0,3,1,1,service,0);machine.fail=service
  symbol='dh2_character_ai_event_script_timer' if service==5 else 'dh2_character_ai_event';call=[machine.out,machine.s,0x80000000,machine.services] if service==5 else args(machine.event_id)
  assert c.invoke(symbol,call)==3 and word(c,machine.out)==service+1 and word(c,machine.out+4)==service and len(machine.events)==1
  assert struct.unpack('<Q',c.uc.mem_read(machine.s+8,8))[0]==machine.owner[3]
 for mode in range(3):
  initialize()
  if mode==0:c.pointer(machine.payload+8,0)
  if mode==1:c.pointer(machine.s+16,0)
  if mode==2:c.pointer(machine.s+32,0)
  status=c.invoke('dh2_character_ai_event_script_timer',[machine.out,machine.s,0xffffffff,machine.services]) if mode==2 else c.invoke('dh2_character_ai_event',args())
  assert status==2 and len(machine.events)==(1 if mode==1 else 0)
 initialize();c.pointer(machine.s+24,0);c.pointer(machine.s+32,0);c.uc.mem_write(machine.services+16,words(0))
 assert c.invoke('dh2_character_ai_event_script_timer',[machine.out,machine.s,0xffffffff,machine.services])==0 and word(c,machine.out)==7 and not machine.events
 return {'atomic_rejections':46,'unavailable_services':6,'missing_callables':3,'runtime_failure_prefixes':6,'inactive_relay':True}

def main():
 parser=argparse.ArgumentParser(description=__doc__)
 for name in ('engine','library','gold','report'):parser.add_argument('--'+name,type=Path,required=True)
 args=parser.parse_args();ref=ROOT/'port/level-world/reference/character-ai-events';manifest=json.loads((ref/'original-functions.json').read_text());assert sha(args.engine)==manifest['original_sha256']
 old=Machine(args.engine,False,manifest);new=Machine(args.library,True,{'functions':[]});records=[];requests=0;counts=[0]*6;composed=0;reentrant=0
 def compare(raw,event,payload,params):
  nonlocal requests,composed,reentrant
  expected=old.execute(raw,event,payload,params);actual=new.execute(raw,event,payload,params)
  assert expected==actual,(len(records),hex(event),params,'old',expected,'new',actual)
  after,events=expected;requests+=len(events);composed+=params[5]==1;reentrant+=params[5]==2
  for entry in events:counts[struct.unpack('<I',entry[:4])[0]]+=1
  records.append(raw+words(event,payload,*params)+after+words(len(events))+b''.join(events))
 for event,forced,locked,blocked,payload,result in itertools.product((*range(65),0x80000000,0xffffffff),(0,1,255),(0,1,255),(0,1,255),(0,16),(0,1,0xffffffff)):
  compare(seed(forced,locked,blocked),event,payload,(result,result,result,0,0,0))
 for event,trigger,mutation in itertools.product((2,9,0x1d,0x22,0x26,0x2a,0x35,0x3f),range(6),range(1,9)):
  compare(seed(),event,16,(0x81234567,0x87654321,1,mutation,trigger,0))
 for active,payload,timer,mutation in itertools.product((0,6),(0,16),(0,1,0x7fffffff,0x80000000,0xffffffff),range(5)):
  compare(seed(active=active),0x35,payload,(timer,3,1,mutation,2,1))
 for event in (2,7,9,0x22,0x2a,0x37):compare(seed(),event,16,(0,3,1,0,0,2))
 contracts=native_contracts(new)
 blob=b'CAE1'+words(len(records))+b''.join(records);args.gold.parent.mkdir(parents=True,exist_ok=True);args.gold.write_bytes(blob)
 source=[ROOT/'port/level-world/character_ai_events.hpp',ROOT/'port/level-world/character_ai_events.cpp',Path(__file__)]
 report={'validation':'PASS','original_sha256':manifest['original_sha256'],'library_sha256':sha(args.library),'corpus_sha256':sha(args.gold),'comparisons':len(records),'ordered_service_requests':requests,'service_counts':counts,'composed_script_timer_cases':composed,'original_reentry_cases':reentrant,'mismatches':0,'source_sha256':{str(p.relative_to(ROOT)).replace('\\','/'):sha(p) for p in source},'scope':__doc__+' Private deep helper/FSM/AI virtual bodies are explicit service fixtures. Composed35 executes actual original OnScriptTimer active gate and native relay; selected AIS/VM remains an explicit service. Reentry cases execute actual nested event31 stores. No complete FSM/Lua/timer/frame backend claim.'}
 report['native_caller_contracts']=contracts
 report['reference_sha256']={str(p.relative_to(ROOT)).replace('\\','/'):sha(p) for p in [ref/'original-functions.json',ref/'reference/original-functions.asm',ref/'dispatch-producer.json',ref/'discover.py']}
 args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))

if __name__=='__main__':main()
