"""Actual static queue initialization, inline time registration and RB erase vs O2.

The original Character caller is entered only at bounded queue instruction
windows. Actual insert/rebalance/erase/free/destructor bodies execute. Allocation,
current-Level, Ctrl_Kill receiver and full AI_UnLoadScript are declared services.
This is not the whole Character frame or Application/static-init construction.
"""
import hashlib,json,random,struct
from pathlib import Path
from unicorn import UC_HOOK_CODE
from visual_timeline_differential import TimelineCpu
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/character-deferred-queue'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def signed(v):return v if v<0x80000000 else v-0x100000000
class QueueCpu(TimelineCpu):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name in ('_Znwm','_Znwj'):
   self.put(0,self.machine.allocate(self.reg(0),False));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if name in ('_ZdlPv','_ZdlPvm'):
   self.machine.free(self.reg(0));self.put(0,0);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  super().external(uc,address,size,unused)
class Machine:
 def __init__(self,path,native):
  self.c=QueueCpu(path,native,json.loads((REF/'original-functions.json').read_text()));self.c.machine=self;self.native=native;c=self.c;d=c.data
  self.actor=[d+0x1000+i*0x2000 for i in range(32)];self.controllers=[d+0x90000+i*0x100 for i in range(32)];self.controllables=[d+0xa0000+i*0x100 for i in range(32)];self.vt=d+0xb0000;self.cb=d+0xc0000;self.services=d+0xd0000;self.iterator=d+0xe0000;self.rows=d+0xe1000;self.count=d+0xe3000;self.level=d+0xe4000
  self.header=0x9a2944 if not native else 0;self.heap=d+0x100000;self.live={};self.node_generation={};self.next_generation=1;self.prefill=0;self.frames=[];self.mode='';self.events=[];self.action=0;self.mutated=False;self.final_key=0;self.ctor=[]
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
  for i,a in enumerate(self.actor):
   if native:c.uc.mem_write(a,struct.pack('<QQ',i+1,0x6000+i+1))
   else:c.pointer(a+0x378,self.controllers[i]);c.pointer(self.controllers[i]+4,self.controllables[i]);c.pointer(self.controllables[i],self.vt)
  if not native:c.pointer(self.vt+0x58,self.cb)
  c.uc.mem_write(self.services,struct.pack('<QQII',0,self.cb,3,0)) if native else None
 def word(self,p):return struct.unpack('<I',self.c.uc.mem_read(p,4))[0]
 def allocate(self,size,original):
  assert 0<size<4096
  p=self.heap;self.heap+=(size+15)&~15;self.c.uc.mem_write(p,bytes((self.prefill,))*size);self.live[p]=size
  if original:self.node_generation[p]=self.next_generation;self.next_generation+=1
  return p
 def free(self,p):assert p in self.live,(hex(p),self.mode,self.action,self.events,list(map(hex,self.live)));del self.live[p]
 def finish(self,value=0):c=self.c;c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def nodes(self):
  found=[]
  def walk(p):
   if not p:return
   walk(self.word(p+8));found.append(p);walk(self.word(p+12))
  walk(self.word(self.header+4));assert len(found)==self.word(self.header+16)
  assert self.word(self.header+8)==(found[0] if found else self.header)
  assert self.word(self.header+12)==(found[-1] if found else self.header)
  return found
 def snapshot(self):
  c=self.c
  if self.native:
   assert c.invoke('dh2_character_deferred_queue_snapshot',[self.header,self.rows,1024,self.count])==0
   return tuple((key,owner) for key,reserved,owner in struct.iter_unpack('<IIQ',bytes(c.uc.mem_read(self.rows,16*self.word(self.count)))))
  return tuple((self.word(p+16),self.actor.index(self.word(p+20))+1) for p in self.nodes())
 def callback_snapshot(self):
  # Run the real public snapshot on a private nested stack, preserving the
  # outer callback's registers and stack bytes.
  if not self.native:return self.snapshot()
  context=self.c.uc.context_save();saved_mode=self.mode;saved_stack=self.c.stack
  self.mode='snapshot';self.c.stack=saved_stack-0x3000
  result=self.c.invoke('dh2_character_deferred_queue_snapshot',[self.header,self.rows,1024,self.count]);assert result==0
  self.c.stack=saved_stack
  data=tuple((key,owner) for key,reserved,owner in struct.iter_unpack('<IIQ',bytes(self.c.uc.mem_read(self.rows,16*self.word(self.count)))))
  self.c.uc.context_restore(context);self.mode=saved_mode;return data
 def restore(self):
  c=self.c;context,mode=self.frames.pop();c.uc.context_restore(context);self.mode=mode;self.finish()
 def redirect(self,op,key=0,owner=1,final=1):
  c=self.c;self.frames.append((c.uc.context_save(),self.mode));c.uc.reg_write(c.sp,c.stack+0xc000);c.uc.reg_write(c.lr,self.cb+16);self.mode=op
  if self.native:
   if op=='assign':args=[self.header,key,self.actor[owner-1],0];name='dh2_character_deferred_queue_assign'
   elif op=='unload':
    c.uc.mem_write(self.iterator+32,struct.pack('<QiIQ',self.header,0,0,0));args=[self.header,self.actor[owner-1],self.iterator+32,final,self.services];name='dh2_character_deferred_queue_unload'
   else:args=[self.header,0,self.services];name='dh2_character_deferred_queue_evict'
   for i,v in enumerate(args):c.put(i,v)
   c.uc.reg_write(c.pc,c.symbols[name])
  elif op=='assign':
   c.put(0,key);c.put(4,self.actor[owner-1]);c.put(5,0x994a98);c.put(9,self.word(0x3acd44));c.uc.reg_write(c.pc,0x3ac460)
  elif op=='unload':
   c.pointer(self.iterator+32,self.header);c.put(0,self.actor[owner-1]);c.put(1,self.iterator+32);c.put(2,final);c.uc.reg_write(c.pc,0x3a7b24)
  else:
   self.level_word=0;c.put(5,0x994a98);c.uc.reg_write(c.pc,0x3ac384)
 def service(self,op,owner,controller,final,key):
  c=self.c;self.events.append((op,owner,controller,final,key,self.callback_snapshot()))
  if not self.mutated and ((op==0 and self.action in (1,2,3)) or (op==1 and self.action==4)):
   self.mutated=True
   if self.action==1:self.redirect('assign',key,32);return True
   if self.action==2:self.redirect('assign',0x80000000,31);return True
   if self.action==3:
    # The captured source RB node must remain alive across Cmd_Kill. Delete a
    # genuinely unrelated owner's first node; deleting this node is source UB.
    unrelated=next(o for k,o in self.events[-1][-1] if o!=owner)
    self.redirect('unload',owner=unrelated,final=255);return True
   if self.action==4:self.redirect('evict');return True
  return False
 def hook(self,uc,a,size,unused):
  c=self.c
  if a==self.cb+16:assert self.frames;self.restore();return
  if self.native:
   if a!=self.cb:return
   op,final,owner,controller,key,reserved=struct.unpack('<IIQQiI',uc.mem_read(c.reg(2),32));assert reserved==0 and c.reg(1)==self.header
   if self.service(op,owner,controller,final,key&0xffffffff):return
   self.finish();return
  if a==0x708ec0:assert self.word(c.reg(0))==24;self.finish(self.allocate(24,True));return
  if a==0x708f00:assert c.reg(1)==24;self.free(c.reg(0));self.finish();return
  if a==0x30e304 and self.mode=='ctor':
   self.ctor.append((c.reg(0),c.reg(1),c.reg(2)));self.finish();return
  if a==0x3aac10 and self.mode=='ctor':c.uc.reg_write(c.pc,c.stop);return
  if a==0x3abf9c and self.mode=='assign':
   if self.frames:self.restore()
   else:c.uc.reg_write(c.pc,c.stop)
   return
  if a==0x31f594:
   c.uc.mem_write(self.level+0x3c,words(self.level_word));self.finish(self.level);return
  if a==0x3ac3cc:self.kill_owner=self.actor.index(self.word(c.reg(11)+20))+1;self.kill_key=self.word(c.reg(11)+16)
  if a==self.cb:
   assert c.reg(1)==0 and c.reg(2)==1
   controller=0x6000+self.controllables.index(c.reg(0))+1
   if self.service(0,self.kill_owner,controller,1,self.kill_key):return
   self.finish();return
  if a==0x3a7b24:
   token=self.word(c.reg(1));actor=c.reg(0)
   if token==self.header:
    token=next((n for n in self.nodes() if self.word(n+20)==actor),self.header)
   self.final_key=self.word(token+16) if token!=self.header else 0
  if a==0x3cc9dc:
   owner=self.actor.index(c.reg(0)-0x3c8)+1
   if self.service(1,owner,0,c.reg(1),self.final_key):return
   self.finish();return
  if a==0x3ac3f4 and self.mode=='evict':
   if self.frames:self.restore()
   else:c.uc.reg_write(c.pc,c.stop)
 def reset(self,prefill):
  c=self.c;self.prefill=prefill;self.events=[];self.frames=[];self.action=0;self.mutated=False;self.ctor=[]
  self.heap=c.data+0x100000;self.live={};self.node_generation={};self.next_generation=1
  if self.native:self.header=c.invoke('dh2_character_deferred_queue_create',[]);assert self.header
  else:
   self.mode='ctor';c.uc.mem_write(self.header,bytes((prefill,))*24);c.put(4,0);c.put(5,0x994a98);c.put(6,0x999000)
   c.invoke(0x3aabe0,[]);assert self.ctor==[(self.header,0x3a7968,0x999000)]
   assert self.word(self.header+4)==0 and self.word(self.header+16)==0
   assert c.uc.mem_read(self.header,1)==b'\0'
  assert self.snapshot()==()
 def assign(self,key,owner):
  c=self.c;self.mode='assign'
  if self.native:assert c.invoke('dh2_character_deferred_queue_assign',[self.header,key,self.actor[owner-1],0])==0
  else:c.put(4,self.actor[owner-1]);c.put(5,0x994a98);c.put(9,self.word(0x3acd44));c.invoke(0x3ac460,[key])
 def unload(self,owner,key,final,explicit):
  c=self.c;self.mode='unload'
  if self.native:
   if explicit:assert c.invoke('dh2_character_deferred_queue_find',[self.header,key,self.iterator])==0
   else:c.uc.mem_write(self.iterator,struct.pack('<QiIQ',self.header,0,0,0))
   assert c.invoke('dh2_character_deferred_queue_unload',[self.header,self.actor[owner-1],self.iterator,final,self.services])==0
  else:
   token=next((n for n in self.nodes() if self.word(n+16)==key),self.header) if explicit else self.header
   c.pointer(self.iterator,token);c.invoke(0x3a7b24,[self.actor[owner-1],self.iterator,final])
 def evict(self,level,action):
  c=self.c;self.mode='evict';self.action=action;self.mutated=False;self.level_word=level
  if self.native:assert c.invoke('dh2_character_deferred_queue_evict',[self.header,level,self.services])==0
  else:c.put(5,0x994a98);c.invoke(0x3ac384,[])
 def destroy(self):
  c=self.c;self.mode='destroy'
  if self.native:assert c.invoke('dh2_character_deferred_queue_destroy',[self.header])==0
  else:c.invoke(0x3a7968,[self.header]);assert self.snapshot()==()
  assert not self.live
def main():
 library=REPO/'.local-inputs/character-deferred-queue/oracle.so';o=Machine(REPO/'.local-inputs/libDungeonHunter2.so',False);n=Machine(library,True);rng=random.Random(20261004);records=[];cases=0
 for prefill in (0,1,85,170,255):
  for batch in range(28):
   o.reset(prefill);n.reset(prefill);cases+=1;records.append((0,prefill,0,0,0,(),()))
   keys=[0,1,0x7fffffff,0x80000000,0xffffffff,0xfffffffe,128,24,8,*[rng.getrandbits(32) for _ in range(25)]]
   rng.shuffle(keys)
   for i,key in enumerate(keys):
    owner=30 if i%7==0 else i%28+1
    for machine in (o,n):machine.assign(key,owner)
    want=o.snapshot();got=n.snapshot();assert got==want,(prefill,batch,key,want,got);cases+=1;records.append((1,key,owner,0,0,want,()))
   for i in range(12):
    key=rng.choice(keys);owner=rng.randrange(1,33)
    for machine in (o,n):machine.assign(key,owner)
    want=o.snapshot();assert n.snapshot()==want;cases+=1;records.append((1,key,owner,0,0,want,()))
   action=batch%5;level=29 if batch%3==0 else 0
   o.events=[];n.events=[]
   for machine in (o,n):machine.evict(level,action)
   want=(o.snapshot(),tuple(o.events));got=(n.snapshot(),tuple(n.events));assert got==want,(action,want,got);cases+=1;records.append((3,level,action,0,0,*want))
   for i in range(24):
    key=rng.choice(keys);owner=rng.randrange(1,33);final=rng.choice((0,1,255,0xffffffff));explicit=i%2
    o.events=[];n.events=[]
    for machine in (o,n):machine.unload(owner,key,final,explicit)
    want=(o.snapshot(),tuple(o.events));got=(n.snapshot(),tuple(n.events));assert got==want,(key,owner,want,got);cases+=1;records.append((2,owner,key,final,explicit,*want))
   for machine in (o,n):machine.destroy()
   cases+=1;records.append((4,0,0,0,0,(),()))
 # Prove strict threshold edges independently of the larger reentry corpus.
 for level in (0,29,0xffffffff):
  for size in (0,8,9,24,25):
   o.reset(0);n.reset(0);cases+=1;records.append((0,0,0,0,0,(),()))
   for i in range(size):
    for machine in (o,n):machine.assign(i,i%32+1)
    want=o.snapshot();assert n.snapshot()==want;cases+=1;records.append((1,i,i%32+1,0,0,want,()))
   o.events=[];n.events=[]
   for machine in (o,n):machine.evict(level,0)
   want=(o.snapshot(),tuple(o.events));assert (n.snapshot(),tuple(n.events))==want
   cases+=1;records.append((3,level,0,0,0,*want))
   for machine in (o,n):machine.destroy()
   cases+=1;records.append((4,0,0,0,0,(),()))
 gold=b'CDQ1'+words(len(records));callbacks=0;rows=0
 for op,a,b,d,e,state,events in records:
  gold+=words(op,a,b,d,e,len(state),len(events));rows+=len(state)
  for key,owner in state:gold+=words(key,owner)
  for event in events:
   service,owner,controller,final,key,snapshot=event;gold+=words(service,owner,controller,final,key,len(snapshot));callbacks+=1
   rows+=len(snapshot)
   for key,owner in snapshot:gold+=words(key,owner)
 (REF/'deferred-queue-fixtures.bin').write_bytes(gold)
 paths=[ROOT/'character_deferred_queue.hpp',ROOT/'character_deferred_queue.cpp',Path(__file__),ROOT/'tools/build_character_deferred_queue_oracle.ps1']
 report=dict(validation='PASS',comparisons=cases,ordered_callbacks=callbacks,ordered_entries=rows,mismatches=0,original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),arm64_sha256=sha(library),corpus_sha256=hashlib.sha256(gold).hexdigest(),source_sha256={p.relative_to(REPO).as_posix():sha(p) for p in paths},manifest_sha256=sha(REF/'original-functions.json'),scope=__doc__,whole_Character_frame=False,whole_Application_constructor=False)
 (ROOT/'reports/character-deferred-queue-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('validation','comparisons','ordered_callbacks','ordered_entries','mismatches')}))
if __name__=='__main__':main()
