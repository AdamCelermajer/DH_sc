"""Original Character.Update startup/post-controller prefixes vs O2 ARM64.

Stops at the controller/timers boundary or a declared unowned queue/bot branch;
does not execute or claim the whole Character frame. Deeper debug strings,
property/inventory/manager/deferred-load queries are explicit synchronous services.
Frozen actual CanUpdate executes on both sides with its own provider trace.
"""
import hashlib,itertools,json,random,struct
from pathlib import Path
from unicorn import UC_HOOK_CODE
from character_can_update_differential import Machine as Eligibility,words
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/character-update-startup'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def text(c,p):
 b=b''
 while c.uc.mem_read(p+len(b),1)!=b'\0':b+=c.uc.mem_read(p+len(b),1)
 return b.decode('ascii')
class Machine(Eligibility):
 def hook(self,uc,address,size,unused):
  if not self.native and address==0x7fd794 and (getattr(self,'phase',False) or getattr(self,'after_load',False)):return
  super().hook(uc,address,size,unused)
 def __init__(self,path,native):
  super().__init__(path,native);c=self.c;d=c.data
  self.start=d+0x11000;self.start_services=d+0x12000;self.start_cb=d+0x13000;self.stats=d+0x14000;self.level=d+0x15000;self.queue=d+0x16000;self.entry=d+0x17000;self.string=d+0x18000;self.player_projection=d+0x19000
  self.app=self.word(0x994a98+0x37f4) if not native else 0
  self.names={};self.trace=[];self.state_calls=0;self.stage=99;self.status=0;c.uc.hook_add(UC_HOOK_CODE,self.prefix)
 def snap(self):
  c=self.c
  if self.native:return(self.word(self.start+48),self.pointer(self.start+16),self.byte(self.owner+34),self.word(self.stats),self.word(self.queue),self.byte(self.start+52))
  return(self.word(self.owner+0x1088),self.word(self.owner+0x3e4),self.byte(self.owner+0x1480),self.word(self.stats+0x5c),self.word(self.queue+0x10),self.byte(self.owner+0x3ec))
 def response(self,op,arg,arg2,subject,name):
  c=self.c;p=self.p
  self.trace.append((op,arg,arg2,subject,name,*self.snap()))
  value=0;identity=0
  if op==2:value=p[{'KillPlayerOne':0,'Give50Potions':1,'isABot':2}[name]]
  elif op==4:
   character=0x1234 if p[4] else 0x2222
   if self.phase:character=0x7777 if p[24] else 0
   if self.native:c.uc.mem_write(self.player_projection,struct.pack('<QQI4B',0x5500,character,p[23],p[22],0,0,0));identity=self.player_projection
   else:c.uc.mem_write(self.player+0x660,words(self.owner if character==0x1234 else 0x7777 if character else 0));identity=self.player
  elif op==5:
   if self.native:c.uc.mem_write(self.start+48,words(arg2))
   else:c.uc.mem_write(self.owner+0x1088,words(arg2))
  elif op==6:value=p[5]
  elif op==8:value=p[3]
  elif op==10:
   value=p[7+min(self.state_calls,2)];self.state_calls+=1
   if p[21]==1 and self.state_calls==2:
    if self.native:c.uc.mem_write(self.owner+34,b'\x01')
    else:c.uc.mem_write(self.owner+0x1480,b'\x01')
  elif op==11:value=p[12]
  elif op==12:
   value=p[15]
   if p[21]==2:
    if self.native:c.pointer(self.start+16,0xabc)
    else:c.pointer(self.owner+0x3e4,0xabc)
  elif op in (13,14,15):value=p[16+op-13]
  elif op==16:value=p[19]
  elif op==17:value=0x80000000
  elif op==18:
   if self.native:c.uc.mem_write(self.player_projection,struct.pack('<QQI4B',0x5500,0x1234,p[23],p[22],0,0,0));identity=self.player_projection
   else:c.uc.mem_write(self.player+0x66c,bytes((p[22],)));c.uc.mem_write(self.player+0x678,words(p[23]));identity=self.player
  return identity,value
 def prefix(self,uc,a,size,unused):
  c=self.c
  if self.native:
   if a!=self.start_cb:return
   op,arg,arg2,reserved,owner,subject,namep=struct.unpack('<4I3Q',uc.mem_read(c.reg(2),40));assert reserved==0 and owner==0x1234
   name=text(c,namep) if namep else '';identity,value=self.response(op,arg,arg2,subject,name)
   uc.mem_write(c.reg(3),struct.pack('<QII',identity,value,0));c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr));return
  if a==0x3a52a4:self.in_can=True
  if a==0x3abf64:self.in_can=False
  if a==self.cb+16 and self.in_can:
   self.events.append((4,0,0,*self.snapshot()));c.put(0,self.params[8]);uc.reg_write(c.pc,uc.reg_read(c.lr));return
  # Original prefix stopping boundaries: these are not fabricated source returns.
  if a==0x3abfc0 and not self.phase:self.stage=1;uc.reg_write(c.pc,c.stop);return
  if a==0x3ac204 and not self.phase:self.stage=0;uc.reg_write(c.pc,c.stop);return
  if (not self.phase and a in (0x3ac3cc,0x3ac460)) or (self.phase and a==0x3ac68c and c.reg(7)):
   self.status=2;uc.reg_write(c.pc,c.stop);return
  if a==0x3ac014 and self.phase:self.stage=2;uc.reg_write(c.pc,c.stop);return
  op=None;arg=0;arg2=0;subject=0;name=''
  if a==0x337888:op=0
  elif a==0x3140ec:op=1;name=text(c,c.reg(1));self.names[c.reg(0)]=name
  elif a==0x337a88:op=2;name=self.names[c.reg(1)]
  elif a==0x318254:op=3;name=self.names[c.reg(0)]
  elif a==0x36e744:op=4;arg=c.reg(1);arg2=c.reg(2)
  elif a==0x3e07a0:op=5;arg=c.reg(1);arg2=c.reg(2)
  elif a==self.cb+16:op=6
  elif a==0x40570c:op=7;arg=c.reg(2);subject=0x4400
  elif a==self.cb+20:op=8
  elif a==0x3ffc40:op=9;arg=c.reg(1)
  elif a==0x3c01ac:op=10
  elif a==0x31f594:op=11
  elif a==0x3cf3a4:op=12;arg=c.reg(1)
  elif a==0x3a3064:op=13
  elif a==0x3a3144:op=14
  elif a==0x3a3158:op=15
  elif a==0x7fd794 and self.phase or a==0x7fd794 and getattr(self,'after_load',False):op=16
  elif a==0x60b0cc:op=17
  elif a==0x36eea8:op=18;arg=c.reg(2)
  if op is None:return
  if op==12:self.after_load=True
  identity,value=self.response(op,arg,arg2,subject,name)
  if op==11:c.uc.mem_write(self.level+0x3c,words(value));value=self.level
  elif op==16:c.uc.mem_write(self.online+5,bytes((value,)));value=self.online
  elif op in (4,18):value=identity
  elif op==1:value=c.reg(0)
  c.put(0,value);uc.reg_write(c.pc,uc.reg_read(c.lr))
 def run(self,p,phase):
  self.p=p;self.phase=phase;self.trace=[];self.names={};self.stage=99;self.status=0;self.state_calls=0;self.after_load=False;c=self.c
  eligible=p[6];base=(0,0,0,0,0,0,0,0,0 if eligible else 1,0,0,0)
  # Configure the actual eligibility projection and callbacks without calling it.
  super().run(base);self.events=[];self.params=base;self.trace=[];self.in_can=False
  if self.native:
   c.uc.mem_write(self.start,struct.pack('<6QI4BI',self.owner,self.services,p[10],0x4400,self.stats,self.queue,p[25],p[20],0,0,0,0))
   c.uc.mem_write(self.owner+34,bytes((p[11],)));c.uc.mem_write(self.stats,words(0xfffffffe));c.uc.mem_write(self.queue,struct.pack('<IIQ',p[13],0,p[14]));c.uc.mem_write(self.start_services,struct.pack('<QQII',0,self.start_cb,(1<<19)-1,0));c.uc.mem_write(self.output,words(99))
   self.status=c.invoke('dh2_character_update_after_controller' if phase else 'dh2_character_update_startup',[self.start,self.start_services,self.output]);self.stage=self.word(self.output)
  else:
   c.pointer(self.vt+0x148,0x3a52a4);c.pointer(self.vt+0x34,self.cb+16);c.pointer(self.vt+0x28,self.cb+20)
   # IsDead virtual within actual CanUpdate shares the same source provider.
   c.pointer(self.owner+0x3e4,p[10]);c.pointer(self.owner+0x378,0x4400);c.uc.mem_write(self.owner+0x1480,bytes((p[11],)));c.uc.mem_write(self.owner+0x3ec,bytes((p[20],)))
   c.uc.mem_write(self.owner+0x1088,words(p[25]));c.pointer(self.app+0x38,self.stats);c.uc.mem_write(self.stats+0x5c,words(0xfffffffe));c.uc.mem_write(self.queue,words(0,0,self.entry if p[14] else self.queue,0,p[13]));c.pointer(0x994a98+self.word(0x3acd44),self.queue)
   if phase:
    c.put(4,self.owner);c.put(5,0x994a98);c.put(7,self.word(0x3acd04));c.put(9,self.string);c.put(11,self.app)
   else:c.put(0,self.owner)
   # Deliberate partial-frame stop preserves the actual prefix stack; the full
   # Character epilogue and downstream frame bodies are outside this oracle.
   c.uc.reg_write(c.sp,c.stack+0xe000);c.uc.reg_write(c.lr,c.stop)
   c.uc.emu_start(0x3abfd4 if phase else 0x3abe98,c.stop,count=1000000)
   assert c.uc.reg_read(c.pc)==c.stop
  return self.status,self.stage,self.snap(),tuple(self.trace),tuple(self.events)
def main():
 library=REPO/'.local-inputs/character-update-startup/oracle.so';o=Machine(REPO/'.local-inputs/libDungeonHunter2.so',False);n=Machine(library,True);rows=[]
 # kill,give,bot,isplayer,match,dead,eligible,state0,state1,state2,active,
 # interaction,level,count,first,load,monster,mini,boss,online,delayed,
 # mutation,botflag,index,target,hp
 base=[0,0,0,0,0,0,1,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
 cases=[]
 for state,active,interaction,load,monster,mini,boss,online in itertools.product((0,2,3,12,0xffffffff),(0,0xabc),(0,1),(0,1),(0,1),(0,1),(0,1),(0,1)):
  p=base.copy();p[7:10]=[state]*3;p[10]=active;p[11]=interaction;p[15:20]=load,monster,mini,boss,online;cases.append((p,0))
 for kill,give,player,match,hp,dead,eligible in itertools.product((0,1),(0,1),(0,1),(0,1),(0,1,199,200,0x80000000,0xffffffff),(0,1),(0,1)):
  p=base.copy();p[0:2]=kill,give;p[3:7]=player,match,dead,eligible;p[10]=0xabc;p[25]=hp;cases.append((p,0))
 for state0,state1,state2,mode in itertools.product((0,3),(2,3,12),(0,2,3,12),(0,1,2)):
  p=base.copy();p[7:10]=state0,state1,state2;p[21]=mode;cases.append((p,0))
 for level,count,first in itertools.product((0,29),(0,8,9,24,25),(0,1)):
  p=base.copy();p[12:15]=level,count,first;cases.append((p,0))
 for load,monster,mini,boss,online,delayed in itertools.product((0,1),(0,1),(0,1),(0,1),(0,1,255),(0,1)):
  p=base.copy();p[15:21]=load,monster,mini,boss,online,delayed;cases.append((p,0))
 for bot,online,player,flag,index,target in itertools.product((0,1),(0,1,255),(0,1),(0,1),(0,1,2,0xffffffff),(0,1)):
  p=base.copy();p[2]=bot;p[3]=player;p[19]=online;p[22:25]=flag,index,target;cases.append((p,1))
 for p,phase in cases:
  want=o.run(p,phase);got=n.run(p,phase);assert want==got,(p,phase,want,got);rows.append(dict(input=p,phase=phase,expected=want))
 (REF/'startup-prefix-fixtures.json').write_text(json.dumps(rows,separators=(',',':'))+'\n')
 gold=b'CUS1'+words(len(rows));prefixes=0;eligibility=0;unavailable=0
 for row in rows:
  status,stage,snapshot,trace,can_trace=row['expected'];unavailable+=bool(status);prefixes+=len(trace);eligibility+=len(can_trace)
  gold+=words(*row['input'],row['phase'],status,stage,*snapshot,len(trace),len(can_trace))
  for op,arg,arg2,subject,name,*state in trace:
   gold+=words(op,arg,arg2,subject,{'':0,'KillPlayerOne':1,'Give50Potions':2,'isABot':3}[name],*state)
  for event in can_trace:gold+=words(*event)
 (REF/'startup-prefix-fixtures.bin').write_bytes(gold)
 paths=[ROOT/'character_update_startup.hpp',ROOT/'character_update_startup.cpp',Path(__file__),ROOT/'tools/build_character_update_startup_oracle.ps1',ROOT/'character_can_update.hpp',ROOT/'character_can_update.cpp',ROOT/'tests/character_can_update_differential.py']
 report=dict(validation='PASS',comparisons=len(rows),ordered_prefix_services=prefixes,ordered_eligibility_services=eligibility,explicit_continuation_stops=unavailable,mismatches=0,original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),arm64_sha256=sha(library),corpus_sha256=sha(REF/'startup-prefix-fixtures.json'),binary_corpus_sha256=sha(REF/'startup-prefix-fixtures.bin'),source_sha256={p.relative_to(REPO).as_posix():sha(p) for p in paths},manifest_sha256=sha(REF/'original-functions.json'),scope=__doc__,whole_character_frame=False,owned_queue=False)
 (ROOT/'reports/character-update-startup-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
