"""Complete original _UpdateTarget instructions against native ARM64 coordinator.

SM, interaction/dead/sight/range queries and Character RaiseEvent are explicit
synchronous services, not supplied successful target/range acceptance policy.
"""
import argparse,hashlib,itertools,json,struct,sys,random
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/character-target-update'
sys.path.insert(0,str(REPO/'port/game-data/tests'))
from items_differential import Original,words
from navigation_differential import Cpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Oracle:
 def __init__(self,library):
  self.old=Original(REPO/'.local-inputs/libDungeonHunter2.so',json.loads((REF/'original-functions.json').read_text()));self.new=Cpu(library,True,{'functions':[]})
  c=self.old;self.ai=c.data+0x8000;self.owners=[c.data+0x10000,c.data+0x14000];self.objects=[c.data+0x20000,c.data+0x24000];self.vtables=[c.data+0x30000+i*0x100 for i in range(4)];self.callbacks=[c.data+0x40000+i*32 for i in range(3)];c.uc.hook_add(UC_HOOK_CODE,self.old_hook)
  n=self.new;self.state=n.data+0x1000;self.nowners=[n.data+0x1100,n.data+0x1200];self.svc=n.data+0x1300;self.callback=n.data+0x1400;n.uc.mem_write(self.callback,bytes.fromhex('c0035fd6'));n.uc.mem_write(self.svc,struct.pack('<QQ',0,self.callback));n.uc.hook_add(UC_HOOK_CODE,self.new_hook)
 def old_id(self,p):return [0,1,2,3,4,11][[0,*self.owners,*self.objects,self.ai].index(p)]
 def old_ptr(self,i):return dict(zip((0,1,2,3,4,11),(0,*self.owners,*self.objects,self.ai)))[i]
 def mutation(self,native,op):
  if self.row[14]!=op or self.triggered:return
  self.triggered=True;c=self.new if native else self.old;mode=self.row[15]
  if mode==1:
   if native:c.uc.mem_write(self.state+24,struct.pack('<Q',0))
   else:c.pointer(self.ai+0x40,0)
  elif mode==2:
   if native:c.uc.mem_write(self.state+24,struct.pack('<Q',4))
   else:c.pointer(self.ai+0x40,self.objects[1])
  elif mode==3:
   if native:c.uc.mem_write(self.state+8,struct.pack('<Q',self.nowners[1]))
   else:c.pointer(self.ai+4,self.owners[1])
  elif mode==4:
   if native:c.uc.mem_write(self.state+40,b'\xff\xff')
   else:c.uc.mem_write(self.ai+0x48,b'\xff\xff')
  elif mode==5:
   self.trace.append([11,0,0,0]);saved=c.uc.context_save();stack=c.stack;c.stack=c.uc.reg_read(c.sp)-0x10000
   try:
    if native:assert c.invoke('dh2_character_target_update',[self.state,self.svc])==0
    else:c.invoke(0x3cb908,[self.ai])
   finally:c.stack=stack;c.uc.context_restore(saved)
   self.trace.append([12,0,0,0])
 def observe(self,native,op,event,subject,other):
  self.trace.append([op,event,subject,other]);self.counts[op]=self.counts.get(op,0)+1
  value={0:self.row[0],1:self.row[1],2:self.row[3 if self.counts[op]==1 else 4],3:self.row[5],4:self.row[6],5:self.row[7],6:self.row[8],7:self.row[9],8:self.row[10],9:self.row[11],10:0}[op]
  self.mutation(native,op);return value
 def old_hook(self,uc,a,size,unused):
  c=self.old;op=None;event=0;other=0
  direct={0x3c0230:0,0x3c01c0:1,0x3a2fec:3,0x3d4ed8:5,0x3d63d8:7,0x3d6604:8,0x3d6188:9,0x3a4d5c:10}
  if a in direct:
   op=direct[a];subject=self.old_id(c.reg(0)-0x4fc if op in (0,1) else c.reg(0));other=self.old_id(c.reg(2) if op==10 else c.reg(1)) if op in (5,7,8,9,10) else 0;event=c.reg(1) if op==10 else 0
  elif a in self.callbacks:
   op=(2,4,6)[self.callbacks.index(a)];subject=self.old_id(c.reg(0));other=self.old_id(c.reg(1)) if op==2 else 0
  if op is not None:c.returned(self.observe(False,op,event,subject,other))
 def new_hook(self,uc,a,size,unused):
  if a!=self.callback:return
  c=self.new;op,event,subject,other=struct.unpack('<IIQQ',uc.mem_read(c.reg(2),24));value=self.observe(True,op,event,subject,other);uc.mem_write(c.reg(3),words(value));c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
 def execute(self,row,native):
  self.row=row;self.trace=[];self.counts={};self.triggered=False;c=self.new if native else self.old
  if native:
   for i,p in enumerate(self.nowners):c.uc.mem_write(p,struct.pack('<QHHI',i+1,55,0,0))
   c.uc.mem_write(self.state,struct.pack('<6Q',11,self.nowners[0],4,3 if row[2] else 0,4,(row[12]&255)|((row[13]&255)<<8)|(77<<16)))
   assert c.invoke('dh2_character_target_update',[self.state,self.svc])==0
   ai,owner,candidate,target,last,flags=struct.unpack('<6Q',c.uc.mem_read(self.state,48));after=[self.nowners.index(owner)+1,candidate,target,last,flags&255,(flags>>8)&255,(flags>>16)&255]
  else:
   c.uc.mem_write(self.ai,bytes(0x100));c.pointer(self.ai+4,self.owners[0]);c.pointer(self.ai+0x3c,self.objects[1]);c.pointer(self.ai+0x40,self.objects[0] if row[2] else 0);c.pointer(self.ai+0x44,self.objects[1]);c.uc.mem_write(self.ai+0x48,bytes([row[12]&255,row[13]&255]));c.uc.mem_write(self.ai+0x4c,b'M')
   for i,obj in enumerate([*self.owners,*self.objects]):c.pointer(obj,self.vtables[i]);c.pointer(self.vtables[i]+0x88,self.callbacks[0]);c.pointer(self.vtables[i]+0x34,self.callbacks[1]);c.pointer(self.vtables[i]+0x124,self.callbacks[2])
   c.invoke(0x3cb908,[self.ai]);after=[self.old_id(c.word(self.ai+4)),self.old_id(c.word(self.ai+0x3c)),self.old_id(c.word(self.ai+0x40)),self.old_id(c.word(self.ai+0x44)),c.uc.mem_read(self.ai+0x48,1)[0],c.uc.mem_read(self.ai+0x49,1)[0],c.uc.mem_read(self.ai+0x4c,1)[0]]
  return after,self.trace.copy()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();o=Oracle(a.library);cases=[];base=[0,0,1,1,1,44,0,1,0,0,0,0,1,1,0xffffffff,0]
 for present,interactive,interactive2,dead,sight,canrange,close,ranged,melee,oldalive,oldsight in itertools.product((0,1),repeat=11):
  v=base.copy();v[2:5]=[present,interactive,interactive2];v[6:14]=[dead,sight,canrange,close,ranged,melee,oldalive,oldsight];cases.append(v)
 for spawn,limbus in itertools.product((0,1,255),repeat=2):v=base.copy();v[0:2]=[spawn,limbus];cases.append(v)
 for slot in (3,4,6,7,8,9,10,11,12,13):
  for raw in (2,127,255,256,257,0xffffffff):v=base.copy();v[slot]=raw;v[12:14]=[0,0];cases.append(v)
 for op,mode in itertools.product(range(11),range(1,6)):
  if op==3 and mode==1:continue
  for ranged in (0,1):v=base.copy();v[8]=ranged;v[12:14]=[0,0];v[14:16]=[op,mode];cases.append(v)
 records=[];calls=0;events=0
 for row in cases:
  expected=o.execute(row,False);actual=o.execute(row,True);assert actual==expected,(row,expected,actual);records.append([row,*expected]);calls+=len(expected[1]);events+=sum(x[0]==10 for x in expected[1])
 gold=b'CTU1'+words(len(records))
 for row,after,trace in records:gold+=words(*row,*after,len(trace),*(x for entry in trace for x in entry))
 (REF/'target-update-fixtures.bin').write_bytes(gold)
 report={'validation':'PASS','comparisons':len(records),'ordered_services':calls,'raise_events':events,'mismatches':0,'original_sha256':sha(REPO/'.local-inputs/libDungeonHunter2.so'),'arm64_sha256':sha(a.library),'manifest_sha256':sha(REF/'original-functions.json'),'corpus_sha256':hashlib.sha256(gold).hexdigest(),'source_sha256':{str(x.relative_to(REPO)):sha(x) for x in (ROOT/'character_target_update.hpp',ROOT/'character_target_update.cpp',Path(__file__))},'scope':__doc__};(ROOT/'reports/character-target-update-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
