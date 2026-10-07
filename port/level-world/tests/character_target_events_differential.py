"""Complete original CharAI target handler prefixes vs O2 ARM64.

Debug/string, target-character/aggro, position and active virtual services are
explicit synchronous boundaries. Sound words are decoded by actual original
AIProps stream instructions from the bundled cache table, never invented IDs.
"""
import argparse,hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/character-target-events'
sys.path.insert(0,str(REPO/'port/game-data/tests'))
from items_differential import Original,words
from navigation_differential import Cpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
METHODS=[0x3d1f60,0x3d1eb4,0x3d2410,0x3d22e4,0x3d1e00,0x3d1d4c,0x3d1ca0,0x3d1bf4]
class Oracle:
 def __init__(self,library):
  self.old=Original(REPO/'.local-inputs/libDungeonHunter2.so',json.loads((REF/'original-functions.json').read_text()));self.new=Cpu(library,True,{'functions':[]})
  c=self.old;self.ai=c.data+0x10000;self.owners=[c.data+0x20000,c.data+0x24000];self.active=[c.data+0x30000,c.data+0x31000];self.vtables=[c.data+0x32000,c.data+0x33000];self.callbacks=[c.data+0x40000+i*32 for i in range(8)];self.position=c.data+0x45000;self.tables=[c.data+0x50000,c.data+0x60000];self.managers=[c.data+0x70000,c.data+0x71000]
  c.blob=(REPO/'port/android-native/app/src/main/assets/data/ai_pyarray.bin').read_bytes();c.cursor=4;self.ai_count=struct.unpack_from('<I',c.blob)[0]
  for i in range(self.ai_count):c.invoke(0x506f3c,[self.tables[0]+68*i,c.stream])
  assert c.cursor==len(c.blob);self.sounds=[self.word(c,self.tables[0]+68*i+36) for i in range(self.ai_count)]
  for i in range(self.ai_count):c.uc.mem_write(self.tables[1]+68*i,bytes(c.uc.mem_read(self.tables[0]+68*(self.ai_count-1-i),68)))
  got=0x3d22fc+self.word(c,0x3d23f8);self.table_global=self.word(c,got+self.word(c,0x3d2408));self.manager_global=self.word(c,got+self.word(c,0x3d240c))
  c.uc.hook_add(UC_HOOK_CODE,self.old_hook)
  n=self.new;self.state=n.data+0x1000;self.target=n.data+0x1100;self.nowners=[n.data+0x1200,n.data+0x1300];self.services=n.data+0x1400;self.callback=n.data+0x1500;self.ntables=[n.data+0x2000,n.data+0x3000]
  n.uc.mem_write(self.ntables[0],words(*self.sounds));n.uc.mem_write(self.ntables[1],words(*reversed(self.sounds)));n.uc.mem_write(self.callback,bytes.fromhex('c0035fd6'));n.uc.hook_add(UC_HOOK_CODE,self.new_hook)
 def word(self,c,p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
 def text(self,c,p):
  b=b''
  while c.uc.mem_read(p+len(b),1)!=b'\0':b+=bytes(c.uc.mem_read(p+len(b),1))
  return b.decode()
 def snapshot(self,native):
  c=self.new if native else self.old
  if native:
   target,active,flags,reserved=struct.unpack('<4Q',c.uc.mem_read(self.state,32));owner=struct.unpack('<Q',c.uc.mem_read(target+8,8))[0];table=struct.unpack('<Q',c.uc.mem_read(self.services+16,8))[0];manager=struct.unpack('<Q',c.uc.mem_read(self.services+32,8))[0]
   return [self.nowners.index(owner)+1,active,flags&255,c.uc.mem_read(target+42,1)[0],self.ntables.index(table),manager]
  return [self.owners.index(self.word(c,self.ai+4))+1,0 if not self.word(c,self.ai+28) else self.active.index(self.word(c,self.ai+28))+31,c.uc.mem_read(self.ai+120,1)[0],c.uc.mem_read(self.ai+76,1)[0],self.tables.index(self.word(c,self.table_global)),self.managers.index(self.word(c,self.manager_global))+41]
 def mutate(self,native,op):
  if self.triggered or self.row[8]!=op:return
  self.triggered=True;c=self.new if native else self.old;mode=self.row[9]
  if mode in (1,2):
   if native:c.uc.mem_write(self.state+8,struct.pack('<Q',0 if mode==1 else 32))
   else:c.pointer(self.ai+28,0 if mode==1 else self.active[1])
  elif mode==3:
   if native:c.uc.mem_write(self.target+8,struct.pack('<Q',self.nowners[1]))
   else:c.pointer(self.ai+4,self.owners[1])
  elif mode==4:
   c.uc.mem_write(self.state+16 if native else self.ai+120,b'\xff');c.uc.mem_write(self.target+42 if native else self.ai+76,b'\xfe')
  elif mode==5:
   self.trace.append([11,*([0]*15)]);saved=c.uc.context_save();stack=c.stack;c.stack=c.uc.reg_read(c.sp)-0x10000
   try:
    if native:assert c.invoke('dh2_character_target_event',[self.state,self.row[0],self.services])==0
    else:c.invoke(METHODS[self.row[0]-10],[self.ai])
   finally:c.stack=stack;c.uc.context_restore(saved)
   self.trace.append([12,*([0]*15)])
  elif mode==6:
   if native:c.uc.mem_write(self.services+16,struct.pack('<Q',self.ntables[1]))
   else:c.pointer(self.table_global,self.tables[1])
  elif mode==7:
   if native:c.uc.mem_write(self.services+32,struct.pack('<Q',42))
   else:c.pointer(self.manager_global,self.managers[1])
 def observe(self,native,entry):
  self.trace.append([*entry,*self.snapshot(native)[:4]]);self.mutate(native,entry[0])
 def old_hook(self,uc,a,size,_):
  c=self.old;op=None;subject=other=sound=flag=integer=0;position=[0]*3;parameters=[0]*2
  if a==0x337888:op=0
  elif a==0x3140ec:
   op=1;assert self.text(c,c.reg(1))=='isTracingCharAIEvents';c.uc.mem_write(c.reg(0),bytes(24));c.pointer(c.reg(0)+20,c.reg(1))
  elif a==0x337a88:op=2;assert self.text(c,self.word(c,c.reg(1)+20))=='isTracingCharAIEvents'
  elif a==0x3139ac:op=3
  elif a in (0x3d5484,0x3d5450,0x3d6d68):
   op={0x3d5484:4,0x3d5450:5,0x3d6d68:6}[a];subject=self.owners.index(c.reg(0)-0x3c8)+1;other=c.reg(1) if op==6 else 0
  elif a==0x3a2fec:op=7;subject=self.owners.index(c.reg(0))+1
  elif a==0x3935dc:op=8;subject=self.owners.index(c.reg(0))+1
  elif a==0x36b5d8:
   op=9;subject=self.managers.index(c.reg(0))+41;sound=c.reg(1);integer=c.reg(3);position=list(struct.unpack('<3I',uc.mem_read(c.reg(2),12)));flag=self.word(c,uc.reg_read(c.sp));parameters=[self.word(c,uc.reg_read(c.sp)+4),self.word(c,uc.reg_read(c.sp)+8)]
  elif a in self.callbacks:op=10;subject=self.active.index(c.reg(0))+31;assert self.callbacks.index(a)==self.row[0]-10
  if op is None:return
  self.observe(False,[op,self.row[0],subject,other,sound,flag,*position,*parameters,integer])
  result={2:self.row[7],4:self.row[4],5:self.row[5],7:self.row[6],8:self.position}.get(op,0)
  if op==8:uc.mem_write(self.position,words(*self.row[10:13]))
  c.returned(result)
 def new_hook(self,uc,a,size,_):
  if a!=self.callback:return
  c=self.new;op,event,subject,other,text,sound,flag,*tail=struct.unpack('<IIQQQII6I',uc.mem_read(c.reg(2),64));position=tail[:3];parameters=tail[3:5];integer=tail[5]
  if op in (1,2,3):assert self.text(c,text)=='isTracingCharAIEvents'
  self.observe(True,[op,event,subject,other,sound,flag,*position,*parameters,integer]);value={2:self.row[7],4:self.row[4],7:self.row[6]}.get(op,0);identity=self.row[5] if op==5 else 0;out=self.row[10:13] if op==8 else [0]*3;uc.mem_write(c.reg(3),struct.pack('<QI3I',identity,value,*out));c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
 def execute(self,row,native):
  self.row=row;self.trace=[];self.triggered=False;c=self.new if native else self.old
  if native:
   for i,p in enumerate(self.nowners):c.uc.mem_write(p,struct.pack('<QHHI',i+1,0,0,0))
   c.uc.mem_write(self.target,struct.pack('<5Q4BI',11,self.nowners[0],0,0,0,1,1,row[3],0,0));c.uc.mem_write(self.state,struct.pack('<4Q',self.target,row[1],row[2],0));c.uc.mem_write(self.services,struct.pack('<QQQIIQ',0,self.callback,self.ntables[0],self.ai_count,0,41));assert c.invoke('dh2_character_target_event',[self.state,row[0],self.services])==0
  else:
   c.uc.mem_write(self.ai,bytes(0x100));c.pointer(self.ai+4,self.owners[0]);c.pointer(self.ai+28,self.active[row[1]-31] if row[1] else 0);c.uc.mem_write(self.ai+120,bytes([row[2]]));c.uc.mem_write(self.ai+76,bytes([row[3]]));c.pointer(self.table_global,self.tables[0]);c.pointer(self.manager_global,self.managers[0])
   for i,p in enumerate(self.active):c.pointer(p,self.vtables[i]);[c.pointer(self.vtables[i]+64+4*j,self.callbacks[j]) for j in range(8)]
   c.invoke(METHODS[row[0]-10],[self.ai])
  return self.snapshot(native),self.trace.copy()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();o=Oracle(a.library);cases=[];base=[10,31,1,77,1,301,44,0,0xffffffff,0,0x3f800000,0x80000000,0x7fc12345]
 for event,active,continued,changed,character,debug in itertools.product(range(10,18),(0,31,32),(0,1,255),(0,77,255),(0,1),(0,1,0xffffffff)):
  v=base.copy();v[:5]=[event,active,continued,changed,character];v[7]=debug;cases.append(v)
 for ai in range(o.ai_count):v=base.copy();v[0]=13;v[6]=ai;cases.append(v)
 for character,identity in itertools.product((0,1,2,255,256,0xffffffff),(0,301,302)):
  v=base.copy();v[0]=12;v[4:6]=[character,identity];cases.append(v)
 for value in (0,0x80000000,0x7f800000,0xff800000,0x7fc12345,0x7f812345,0xffffffff):
  v=base.copy();v[0]=13;v[10:13]=[value,value^0x80000000,value];cases.append(v)
 for event,op,mode in itertools.product(range(10,18),range(11),range(1,8)):
  v=base.copy();v[0]=event;v[8:10]=[op,mode];cases.append(v)
 records=[];calls=0
 for row in cases:
  expected=o.execute(row,False);actual=o.execute(row,True);assert actual==expected,(row,expected,actual);records.append([row,*expected]);calls+=len(expected[1])
 gold=b'CTE1'+words(len(records),o.ai_count,*o.sounds)
 for row,after,trace in records:gold+=words(*row,*after,len(trace),*(x for entry in trace for x in entry))
 (REF/'target-events-fixtures.bin').write_bytes(gold)
 report={'validation':'PASS','comparisons':len(records),'ordered_services':calls,'mismatches':0,'original_sha256':sha(REPO/'.local-inputs/libDungeonHunter2.so'),'arm64_sha256':sha(a.library),'manifest_sha256':sha(REF/'original-functions.json'),'corpus_sha256':hashlib.sha256(gold).hexdigest(),'ai_rows':o.ai_count,'ai_table_sha256':sha(REPO/'port/android-native/app/src/main/assets/data/ai_pyarray.bin'),'source_sha256':{str(x.relative_to(REPO)):sha(x) for x in (ROOT/'character_target_events.hpp',ROOT/'character_target_events.cpp',Path(__file__))},'scope':__doc__};(ROOT/'reports/character-target-events-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
