"""Complete source pre-controller startup including the actual shared RB queue.

Character.Update executes continuously from its entry to the real controller
boundary; actual CanUpdate and RB init/insert/erase/destruction execute. Diagnostic,
manager/deferred-load/inventory and final Kill/AIUnload bodies are explicit services.
There is no controller/timer/AI/animator/GameObject or whole-frame claim.
"""
import hashlib,itertools,json,struct
from pathlib import Path
from unicorn import UC_HOOK_CODE
import character_can_update_differential as eligibility
from character_update_startup_differential import Machine as Startup,text
from character_deferred_queue_differential import Machine as Queue,QueueCpu,words
eligibility.TimelineCpu=QueueCpu
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/character-update-queued'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class SharedQueue(Queue):
 def __init__(self,parent):
  self.parent=parent;self.c=parent.c;self.c.machine=self;self.native=parent.native;c=self.c;d=c.data+0x1000000
  self.actor=[d+0x1000+i*0x2000 for i in range(32)]
  self.controllers=[d+0x90000+i*0x100 for i in range(32)];self.controllables=[d+0xa0000+i*0x100 for i in range(32)]
  self.vt=d+0xb0000;self.cb=d+0xc0000;self.services=d+0xd0000;self.iterator=d+0xe0000;self.rows=d+0xe1000;self.count=d+0xe3000;self.level=d+0xe4000
  self.header=0 if self.native else 0x9a2944;self.heap=c.data+0x100000;self.live={};self.node_generation={};self.next_generation=1;self.prefill=0;self.frames=[];self.mode='';self.events=[];self.action=0;self.mutated=False;self.final_key=0;self.ctor=[]
  for i,a in enumerate(self.actor):
   if self.native:c.uc.mem_write(a,struct.pack('<QQ',i+1,0x6000+i+1))
   else:c.pointer(a+0x378,self.controllers[i]);c.pointer(self.controllers[i]+4,self.controllables[i]);c.pointer(self.controllables[i],self.vt)
  if self.native:c.uc.mem_write(self.services,struct.pack('<QQII',0,self.cb,3,0))
  else:c.pointer(self.vt+0x58,self.cb)
  self.actor.extend([0]*(0x1234-len(self.actor)))
  self.actor[0x1233]=d+0xf0000 if self.native else parent.owner
  if self.native:c.uc.mem_write(self.actor[0x1233],struct.pack('<QQ',0x1234,0x4400))
  self.current_controller=d+0xf1000;self.current_controllable=d+0xf1100
  if not self.native:c.pointer(self.current_controller+4,self.current_controllable);c.pointer(self.current_controllable,self.vt)
 def service(self,*args):
  if self.parent.in_run:self.parent.order.append((2,len(self.events)))
  return super().service(*args)
 def hook(self,uc,a,size,unused):
  if not self.native and a==self.cb and self.c.reg(0)==self.current_controllable:
   assert self.c.reg(1)==0 and self.c.reg(2)==1
   if self.service(0,self.kill_owner,0x4400,1,self.kill_key):return
   return self.finish()
  return super().hook(uc,a,size,unused)
class Machine(Startup):
 def __init__(self,path,native):
  self.in_run=False;self.order=[];super().__init__(path,native);self.shared=SharedQueue(self)
  self.binding=self.c.data+0x1f000;self.c.uc.hook_add(UC_HOOK_CODE,self.queue_hook)
 def hook(self,uc,a,size,unused):
  before=len(self.events);super().hook(uc,a,size,unused)
  if self.in_run:
   for i in range(before,len(self.events)):self.order.append((1,i))
 def snap(self):
  state=list(super().snap())
  if hasattr(self,'shared'):state[4]=len(self.shared.callback_snapshot())
  return tuple(state)
 def response(self,*args):
  if self.in_run:self.order.append((0,len(self.trace)))
  result=super().response(*args)
  if args[0]==17:return 0,self.clock
  return result
 def prefix(self,uc,a,size,unused):
  if not self.in_run:return
  if hasattr(self,'shared') and not self.native:
   q=self.shared
   if self.in_run and a==0x3ac384:q.mode='evict'
   if self.in_run and a==0x3ac460:q.mode='assign'
   if self.in_run and a in (0x3ac3cc,0x3ac460):return
   if self.in_run and a==0x40570c and q.mode=='evict':return
   if q.frames and a==0x31f594:return
  before=len(self.events);super().prefix(uc,a,size,unused)
  if self.in_run:
   for i in range(before,len(self.events)):self.order.append((1,i))
 def queue_hook(self,uc,a,size,unused):
  q=self.shared
  if not self.in_run:return q.hook(uc,a,size,unused)
  if self.native:return q.hook(uc,a,size,unused)
  if a in (0x708ec0,0x708f00,0x3ac3cc,0x3a7b24,0x3cc9dc,q.cb,q.cb+16):return q.hook(uc,a,size,unused)
  if q.frames and a in (0x3abf9c,0x3ac3f4,0x31f594):return q.hook(uc,a,size,unused)
 def run(self,p,clock=0x80000000,action=0):
  self.p=p;self.phase=0;self.clock=clock;self.trace=[];self.names={};self.stage=99;self.status=0;self.state_calls=0;self.after_load=False;self.order=[];self.in_run=False;c=self.c;q=self.shared
  q.reset(0)
  base=(0,0,0,0,0,0,0,0,0 if p[6] else 1,0,0,0)
  eligibility.Machine.run(self,base);self.params=base;self.events=[];self.trace=[];self.in_can=False
  if self.native:
   c.uc.mem_write(self.start,struct.pack('<6QI4BI',self.owner,self.services,p[10],0x4400,self.stats,0,p[25],p[20],0,0,0,0))
   c.uc.mem_write(self.owner+34,bytes((p[11],)));c.uc.mem_write(self.stats,words(0xfffffffe));c.uc.mem_write(self.start_services,struct.pack('<QQII',0,self.start_cb,(1<<19)-1,0));c.uc.mem_write(self.output,words(99))
   c.uc.mem_write(self.binding,struct.pack('<QQQ',q.header,q.actor[0x1233],q.services))
  else:
   c.pointer(self.vt+0x148,0x3a52a4);c.pointer(self.vt+0x34,self.cb+16);c.pointer(self.vt+0x28,self.cb+20)
   c.pointer(self.owner+0x3e4,p[10]);c.pointer(self.owner+0x378,q.current_controller);c.uc.mem_write(self.owner+0x1480,bytes((p[11],)));c.uc.mem_write(self.owner+0x3ec,bytes((p[20],)));c.uc.mem_write(self.owner+0x1088,words(p[25]));c.pointer(self.app+0x38,self.stats);c.uc.mem_write(self.stats+0x5c,words(0xfffffffe));c.pointer(0x994a98+self.word(0x3acd44),q.header)
  for i in range(p[13]):q.assign(i,0x1234 if p[14] and i==0 else i%32+1)
  q.events=[];q.action=action;q.mutated=False;q.mode='frame';self.in_run=True
  if self.native:
   self.status=c.invoke('dh2_character_update_queued',[self.start,self.start_services,self.binding,self.output]);self.stage=self.word(self.output)
  else:
   c.put(0,self.owner);c.uc.reg_write(c.sp,c.stack+0xe000);c.uc.reg_write(c.lr,c.stop)
   try:c.uc.emu_start(0x3abe98,c.stop,count=2000000)
   except Exception as error:raise AssertionError((p,clock,action,hex(c.uc.reg_read(c.pc)),tuple(hex(c.reg(i)) for i in range(13)),self.trace,q.events)) from error
   assert c.uc.reg_read(c.pc)==c.stop
  result=(self.status,self.stage,self.snap(),tuple(self.trace),tuple(self.events),tuple(q.events),tuple(self.order),q.snapshot())
  self.in_run=False;q.destroy();return result
def main():
 REF.mkdir(parents=True,exist_ok=True);library=REPO/'.local-inputs/character-update-queued/oracle.so';o=Machine(REPO/'.local-inputs/libDungeonHunter2.so',False);n=Machine(library,True)
 base=[0,0,0,0,0,0,1,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];cases=[]
 # Preserve exact source-order cases from the frozen prefix, completing its
 # formerly unavailable queue branches with real native/original containers.
 historical=json.loads((ROOT/'reference/character-update-startup/startup-prefix-fixtures.json').read_text())
 for row in historical:
  if row['phase']==0:cases.append((row['input'],0x80000000,0))
 for level,size,load,online,delayed,clock in itertools.product((0,29),(0,8,9,24,25),(0,1),(0,1,255),(0,1),(0,8,24,0x7fffffff,0x80000000,0xffffffff)):
  p=base.copy();p[12:21]=level,size,0,load,1,0,0,online,delayed;cases.append((p,clock,0))
 for level,size,action,current,mutation in itertools.product((0,29),(9,25,34),(1,2,3,4),(0,1),(0,2)):
  p=base.copy();p[12:21]=level,size,current,1,1,0,0,0,1;p[21]=mutation;cases.append((p,0x80000000,action))
 rows=[];prefixes=cans=queue_callbacks=entries=0
 for p,clock,action in cases:
  want=o.run(p,clock,action);got=n.run(p,clock,action);assert got==want,(p,clock,action,want,got)
  status,stage,state,trace,can,qevents,order,final=want;assert status==0
  rows.append(dict(input=p,clock=clock,action=action,expected=want));prefixes+=len(trace);cans+=len(can);queue_callbacks+=len(qevents);entries+=len(final)+sum(len(e[-1]) for e in qevents)
 (REF/'queued-prefix-fixtures.json').write_text(json.dumps(rows,separators=(',',':'))+'\n')
 # Host replay uses this explicit JSON-free word format.
 gold=b'CUQ1'+words(len(rows))
 for row in rows:
  status,stage,state,trace,can,qevents,order,final=row['expected'];gold+=words(*row['input'],row['clock'],row['action'],status,stage,*state,len(trace),len(can),len(qevents),len(order),len(final))
  for op,arg,arg2,subject,name,*snapshot in trace:gold+=words(op,arg,arg2,subject,{'':0,'KillPlayerOne':1,'Give50Potions':2,'isABot':3}[name],*snapshot)
  for event in can:gold+=words(*event)
  for op,owner,controller,final_word,key,snapshot in qevents:
   gold+=words(op,owner,controller,final_word,key,len(snapshot))
   for key,owner in snapshot:gold+=words(key,owner)
  for event in order:gold+=words(*event)
  for key,owner in final:gold+=words(key,owner)
 (REF/'queued-prefix-fixtures.bin').write_bytes(gold)
 names=['character_update_queued.hpp','character_update_queued.cpp','tests/character_update_queued_differential.py','tools/build_character_update_queued_oracle.ps1','character_can_update.hpp','character_can_update.cpp','character_deferred_queue.hpp','character_deferred_queue.cpp','tests/character_deferred_queue_differential.py','tests/character_update_startup_differential.py','tests/character_can_update_differential.py']
 report=dict(validation='PASS',comparisons=len(rows),ordered_prefix_services=prefixes,ordered_eligibility_services=cans,ordered_queue_callbacks=queue_callbacks,ordered_entries=entries,mismatches=0,source_sha256={str((ROOT/p).relative_to(REPO)).replace('\\','/'):sha(ROOT/p) for p in names},original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),arm64_sha256=sha(library),binary_corpus_sha256=sha(REF/'queued-prefix-fixtures.bin'),JSON_corpus_sha256=sha(REF/'queued-prefix-fixtures.json'),frozen_manifests={str(p.relative_to(REPO)).replace('\\','/'):sha(p) for p in (ROOT/'reference/character-update-startup/original-functions.json',ROOT/'reference/character-deferred-queue/original-functions.json')},scope=__doc__,whole_Character_frame=False,partial_prefix_replayed=False)
 (ROOT/'reports/character-update-queued-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('validation','comparisons','ordered_prefix_services','ordered_eligibility_services','ordered_queue_callbacks','mismatches')}))
if __name__=='__main__':main()
