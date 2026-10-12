"""Actual active gates/default bodies/External end callback vs optimized ARM64.

Nonempty subclass virtuals and External LuaScript.Call are explicit services;
the actual owned private VM/alias composition is a separate sanitized audit.
"""
import itertools,json,struct,sys,hashlib
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from visual_timeline_differential import TimelineCpu
REF=ROOT/'reference/character-ai-state-changed'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(*x):return struct.pack('<'+'I'*len(x),*(v&0xffffffff for v in x))
def text(c,p):
 b=b''
 while bytes(c.uc.mem_read(p+len(b),1))!=b'\0':b+=bytes(c.uc.mem_read(p+len(b),1))
 return b.decode()
class Machine:
 def __init__(self,path,native):
  self.c=TimelineCpu(path,native,json.loads((REF/'original-functions.json').read_text()));self.native=native;c=self.c;d=c.data
  self.s=d+0x1000;self.ai=d+0x2000;self.actors=[0,d+0x3000,d+0x4000];self.vts=[0,d+0x5000,d+0x6000];self.services=d+0x7000;self.cb=d+0x8000;self.script=d+0x9000;self.target_services=d+0xa000
  self.events=[];self.key=0;self.mode=0;self.depth=0;self.op=0
  c.uc.mem_write(self.services,struct.pack('<QQII',0,self.cb,32,0));c.uc.mem_write(self.target_services,struct.pack('<QQ',0,self.cb+8));c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def finish(self,status=0):c=self.c;c.put(0,status);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def state(self):
  c=self.c
  if self.native:
   active,vt=struct.unpack('<QQ',c.uc.mem_read(self.s+24,16));return (self.actors.index(active),self.vts.index(vt))
  active=struct.unpack('<I',c.uc.mem_read(self.ai+0x1c,4))[0];vt=struct.unpack('<I',c.uc.mem_read(active,4))[0] if active else self.vts[1];return(self.actors.index(active),self.vts.index(vt))
 def mutate(self):
  c=self.c
  if self.mode==1:
   if self.native:c.pointer(self.s+24,0)
   else:c.pointer(self.ai+0x1c,0)
  elif self.mode in (2,3,4):
   if self.native:c.pointer(self.s+24,self.actors[2]);c.pointer(self.s+32,self.vts[2])
   else:c.pointer(self.ai+0x1c,self.actors[2])
  if self.mode in (3,4) and not self.depth:
   self.depth=1
   if self.native:
    name='dh2_character_ai_state_changed' if self.op==0 else 'dh2_character_ai_end_anim'
    args=[self.s,0x80000000,0xffffffff,self.services] if self.op==0 else [self.s,self.services]
    for i,v in enumerate(args):c.put(i,v)
    c.uc.reg_write(c.pc,c.symbols[name])
   else:
    c.put(0,self.ai);c.put(1,0x80000000);c.put(2,0xffffffff);c.uc.reg_write(c.pc,0x3d0bec if self.op==0 else 0x3d0ce8)
   return True
  return False
 def hook(self,uc,a,size,unused):
  c=self.c
  if self.native and a==self.cb:
   service,slot,event,old,receiver,key,new=struct.unpack('<4I3Q',uc.mem_read(c.reg(2),40));assert service==5
   if slot==0x98 and key==0x3dccd0:
    uc.mem_write(self.script,struct.pack('<QII',receiver,0,0));c.put(0,self.script);c.put(1,self.target_services);uc.reg_write(c.pc,c.symbols['dh2_character_ais_external_end_anim']);return
   self.events.append((slot,self.actors.index(receiver),key,new,old));uc.mem_write(c.reg(3),words(0xa5))
   if self.mutate():return
   self.finish();return
  if self.native and a==self.cb+8:
   receiver,name,arg,count,kind=struct.unpack('<3Q2I',uc.mem_read(c.reg(1),32));assert(arg,count,kind)==(0,0,0)
   self.events.append(('call0',self.actors.index(receiver),text(c,name)));self.finish();return
  if not self.native and a in (self.cb,self.cb+4):
   receiver=c.reg(0);slot=0x20 if self.op==0 else 0x98;new,old=(c.reg(1),c.reg(2)) if self.op==0 else (0,0)
   self.events.append((slot,self.actors.index(receiver),0x70000001,new,old))
   if self.mutate():return
   self.finish();return
  if not self.native and a==0x37c514:
   self.events.append(('call0',self.actors.index(c.reg(0)),text(c,c.reg(1))));self.finish()
 def run(self,op,active,key,next_,old,mode=0,mask=0):
  self.events=[];self.mode=mode;self.depth=0;self.op=op;c=self.c;slot=0x20 if op==0 else 0x98
  empty=0x3dbe8c if op==0 else 0x3dbeec
  for i in (1,2):
   actual_key=key if i==1 or mode==4 else empty
   c.uc.mem_write(self.vts[i],bytes(51*c.word_size));c.pointer(self.vts[i]+(slot//4)*c.word_size,actual_key if self.native or actual_key in (empty,0x3dccd0) else self.cb)
   if not self.native:c.pointer(self.actors[i],self.vts[i])
  if self.native:c.uc.mem_write(self.s,struct.pack('<5Q6I',self.ai,0,0,self.actors[active],self.vts[1],255,255,255,0,0,0))
  else:c.pointer(self.ai+0x1c,self.actors[active]);c.pointer(self.actors[1]+0xb8,mask)
  if op==2:
   if self.native:
    c.uc.mem_write(self.script,struct.pack('<QII',self.actors[1],mask,0));assert c.invoke('dh2_character_ais_external_end_anim',[self.script,self.target_services])==0
   else:c.invoke(0x3dccd0,[self.actors[1]])
  elif self.native:
   name='dh2_character_ai_state_changed' if op==0 else 'dh2_character_ai_end_anim';args=[self.s,next_,old,self.services] if op==0 else [self.s,self.services];assert c.invoke(name,args)==0
  else:c.invoke(0x3d0bec if op==0 else 0x3d0ce8,[self.ai,next_,old])
  return self.state(),tuple(self.events)
def main():
 library=REPO/'.local-inputs/character-ai-state-changed/oracle.so';o=Machine(REPO/'.local-inputs/libDungeonHunter2.so',False);n=Machine(library,True);rows=[]
 values=[0,1,3,4,12,19,0x7fffffff,0x80000000,0xffffffff]
 tables=json.loads((REF/'vtables.json').read_text())['tables']
 for table in tables:
  for active,next_,old in itertools.product((0,1),values,values):
   case=(0,active,table['slot_keys'][8],next_,old,0,0);want=o.run(*case);got=n.run(*case);assert got==want,(case,want,got);rows.append((case,want))
 for active,next_,old,mode in itertools.product((0,1),values,values,range(5)):
  case=(0,active,0x70000001,next_,old,mode,0);want=o.run(*case);got=n.run(*case);assert got==want,(case,want,got);rows.append((case,want))
 for active,key,mode in itertools.product((0,1),(0x3dbeec,0x70000001,0x3dccd0),range(4)):
  case=(1,active,key,0,0,mode,0);want=o.run(*case);got=n.run(*case);assert got==want,(case,want,got);rows.append((case,want))
 # Source External wrapper executes the base no-op, then actual literal call.
 # The whole Lua call/allocator ABI is covered by prior alias/runtime proofs.
 for mask in [*range(256),0xffffffff,0x80000000]:
  case=(2,1,0x3dccd0,0,0,0,mask);want=o.run(*case);got=n.run(*case);assert got==want,(case,want,got);rows.append((case,want))
 gold=b'ASC1'+words(len(rows));callbacks=0
 for case,(state,events) in rows:
  gold+=words(*case,*state,len(events));callbacks+=len(events)
  for e in events:
   if e[0]=='call0':gold+=words(1,e[1],0,0,0,0)
   else:gold+=words(0,e[1],e[0],e[2],e[3],e[4])
 (REF/'state-changed-fixtures.bin').write_bytes(gold)
 sources=[ROOT/'character_ai_state_changed.cpp',ROOT/'character_ai_state_changed.hpp',Path(__file__),ROOT/'character_ai_events.hpp',ROOT/'object_identity.hpp']
 report=dict(validation='PASS',comparisons=len(rows),ordered_callbacks=callbacks,mismatches=0,original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),arm64_sha256=sha(library),corpus_sha256=hashlib.sha256(gold).hexdigest(),source_sha256={p.relative_to(REPO).as_posix():sha(p) for p in sources},manifest_sha256=sha(REF/'original-functions.json'),vtables_sha256=sha(REF/'vtables.json'),scope=__doc__,actual_source_default_tables=3,whole_private_VM_arm64=False)
 (ROOT/'reports/character-ai-state-changed-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',comparisons=len(rows),ordered_callbacks=callbacks)))
if __name__=='__main__':main()
