"""Complete CanUpdate instructions vs O2 ARM64, with explicit synchronous providers."""
import hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
from visual_timeline_differential import TimelineCpu
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
REF=ROOT/'reference/character-can-update'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
class Machine:
 def __init__(self,path,native):
  self.c=TimelineCpu(path,native,json.loads((REF/'original-functions.json').read_text()));self.native=native;c=self.c;d=c.data
  self.owner=d+0x1000;self.visual=[0,d+0x4000,d+0x4100];self.nodes=[0,d+0x5000,d+0x6000]
  self.player=d+0x7000;self.online=d+0x8000;self.services=d+0x9000;self.cb=d+0xa000;self.output=d+0xb000;self.vt=d+0xc000
  self.events=[];self.depth=0;c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def word(self,p):return struct.unpack('<I',self.c.uc.mem_read(p,4))[0]
 def pointer(self,p):return struct.unpack('<Q' if self.native else '<I',self.c.uc.mem_read(p,self.c.word_size))[0]
 def byte(self,p):return self.c.uc.mem_read(p,1)[0]
 def owner_visual(self):return self.pointer(self.owner+(8 if self.native else 0x2d8))
 def node_of(self,v):return self.pointer(v+(0 if self.native else 8))
 def offsets(self):return (16,32,33,34) if self.native else (0x418,0x80,0x2fc,0x1480)
 def snapshot(self):
  link,en,force,interaction=self.offsets();c=self.c
  return (self.visual.index(self.owner_visual()),self.pointer(self.owner+link),self.byte(self.owner+en),self.byte(self.owner+force),self.byte(self.owner+interaction),self.nodes.index(self.node_of(self.visual[1])),*(x for n in self.nodes[1:] for x in (self.word(n+(0 if self.native else 0x118)),self.byte(n+(4 if self.native else 0x200)))))
 def mutate(self,op):
  c=self.c;link,en,force,interaction=self.offsets();mode=self.params[-1]
  if op==0 and mode==1:c.pointer(self.owner+(8 if self.native else 0x2d8),self.visual[2])
  if op==2 and mode==2:c.pointer(self.owner+link,200)
  if op==2 and mode==3:c.uc.mem_write(self.nodes[1]+(0 if self.native else 0x118),words(0))
  if op==2 and mode==4:c.pointer(self.owner+(8 if self.native else 0x2d8),self.visual[2])
  if op==3 and mode==5:c.uc.mem_write(self.owner+interaction,b'\x01')
  if op==4 and mode==6:c.uc.mem_write(self.owner+en,b'\x01')
  if op==4 and mode==7:c.pointer(self.visual[1]+(0 if self.native else 8),self.nodes[2])
  if op==1 and mode==9:c.uc.mem_write(self.owner+en,b'\x00')
 def hook(self,uc,a,size,unused):
  c=self.c
  if self.native:
   if a!=self.cb:return
   op,arg,owner,subject=struct.unpack('<IIQQ',uc.mem_read(c.reg(2),24));assert owner==0x1234
   assert arg==(1 if op==2 else 0) and subject==(0x1350 if op==3 else 0)
   response=c.reg(3)
  else:
   mapping={0x7fd794:0,self.cb+4:1,0x36e478:2,0x33de90:3,self.cb+8:4,0x3a5248:5}
   if a not in mapping:return
   op=mapping[a];arg=1 if op==2 else 0;subject=0x1350 if op==3 else 0
   if op==2:assert c.reg(1)==0 and c.reg(2)==1
   if op==3:assert c.reg(0)==self.owner and c.reg(1)==self.owner+0x12c
  self.events.append((op,arg,subject,*self.snapshot()))
  self.mutate(op)
  p=self.params;value=(p[1],p[2],100 if p[3] else 200,p[6],p[8],p[9])[op]
  if self.depth and op==2:value=100
  if self.depth and op==4:value=0
  if self.native:uc.mem_write(response,struct.pack('<QII',value if op==2 else 0,0 if op==2 else value,0))
  if op==3 and p[-1]==8 and not self.depth:
   self.depth=1;c.pointer(self.owner+(8 if self.native else 0x2d8),self.visual[2]);c.pointer(self.owner+self.offsets()[0],100)
   if self.native:
    uc.mem_write(response,struct.pack('<QII',0,1,0));c.put(0,self.owner);c.put(1,self.services);c.put(2,self.output+4);uc.reg_write(c.pc,c.symbols['dh2_character_can_update'])
   else:c.put(0,self.owner);uc.reg_write(c.pc,0x3a52a4)
   return
  if self.native:c.put(0,0)
  elif op==0:c.put(0,self.online)
  elif op==2:c.uc.mem_write(self.player+0x660,words(value));c.put(0,self.player)
  else:c.put(0,value)
  uc.reg_write(c.pc,uc.reg_read(c.lr))
 def run(self,p):
  self.params=p;self.events=[];self.depth=0;c=self.c
  visual,online,remote,match,culling,force,cull_result,interaction,dead,respawn,enabled,mode=p
  c.uc.mem_write(self.owner,bytes(0x2000));c.uc.mem_write(self.online,bytes(32));c.uc.mem_write(self.online+5,bytes((online,)))
  for i in (1,2):
   c.uc.mem_write(self.nodes[i],bytes(0x300));c.pointer(self.visual[i]+(0 if self.native else 8),self.nodes[i])
   c.uc.mem_write(self.nodes[i]+(0 if self.native else 0x118),words(culling if i==1 else 0));c.uc.mem_write(self.nodes[i]+(4 if self.native else 0x200),b'\xa5')
  if self.native:
   c.uc.mem_write(self.owner,struct.pack('<4Q4BI',0x1234,self.visual[visual],100,0x1350,enabled,force,interaction,0,0));c.uc.mem_write(self.services,struct.pack('<QQII',0,self.cb,63,0));c.uc.mem_write(self.output,words(0xa5,0xa5));assert c.invoke('dh2_character_can_update',[self.owner,self.services,self.output])==0;result=self.word(self.output)
  else:
   c.pointer(self.owner,self.vt);c.pointer(self.vt+0x54,self.cb+4);c.pointer(self.vt+0x34,self.cb+8);c.pointer(self.owner+0x2d8,self.visual[visual]);c.pointer(self.owner+0x418,100)
   for offset,value in ((0x80,enabled),(0x2fc,force),(0x1480,interaction)):c.uc.mem_write(self.owner+offset,bytes((value,)))
   result=c.invoke(0x3a52a4,[self.owner])
  return result,self.snapshot(),tuple(self.events)
def main():
 REF.mkdir(exist_ok=True);library=REPO/'.local-inputs/character-can-update/oracle.so'
 o=Machine(REPO/'.local-inputs/libDungeonHunter2.so',False);n=Machine(library,True);rows=[]
 domains=[(0,1),(0,1,255),(0,1),(0,1),(0,1),(0,1),(0,1),(0,1),(0,1),(0,1),(0,1,255)]
 for values in itertools.product(*domains):
  case=(*values,0);want=o.run(case);got=n.run(case);assert want==got,(case,want,got);rows.append((case,want))
 for mode in range(1,10):
  for online,enabled,dead,cull in itertools.product((0,1),(0,1),(0,1),(0,1)):
   case=(1,online,1 if mode==9 else 0,0,1,0,cull,0,dead,0,enabled,mode)
   want=o.run(case);got=n.run(case);assert want==got,(case,want,got);rows.append((case,want))
 gold=b'CUF1'+words(len(rows));events=0
 for p,(result,state,trace) in rows:
  gold+=words(*p,result,*state,len(trace));events+=len(trace)
  for e in trace:gold+=words(*e)
 (REF/'can-update-fixtures.bin').write_bytes(gold)
 files=[ROOT/'character_can_update.hpp',ROOT/'character_can_update.cpp',Path(__file__),ROOT/'tools/build_character_can_update_oracle.ps1']
 report=dict(validation='PASS',comparisons=len(rows),ordered_services=events,mismatches=0,source_sha256={str(p.relative_to(REPO)).replace('\\','/'):sha(p) for p in files},original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),arm64_sha256=sha(library),corpus_sha256=hashlib.sha256(gold).hexdigest(),manifest_sha256=sha(REF/'original-functions.json'),scope=__doc__,upstream_visibility_or_world_producers=False)
 (ROOT/'reports/character-can-update-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('validation','comparisons','ordered_services','mismatches')}))
if __name__=='__main__':main()
