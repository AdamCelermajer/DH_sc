"""Full original target setter/wrappers vs optimized ARM64, with live service reentry.

Debug storage/string allocation and ReturnValues append are explicit services.
GetCharAIId, real Character.IsDead, AI_IsInSight/GetTargetPosition and actual
AIProps stream decoding execute original instructions. GetTarget compares its
projection-entry identity only; no original GameObject Lua representation claim.
"""
import argparse,hashlib,itertools,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_S0
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/character-target-bindings';ASSETS=REPO/'port/android-native/app/src/main/assets/data'
sys.path.insert(0,str(REPO/'port/game-data/tests'))
from items_differential import Original,words
from visual_timeline_differential import TimelineCpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def fword(v):return struct.unpack('<I',struct.pack('<f',v))[0]
class Source(Original):
 pass
class Machine:
 def __init__(self,library,native,radii=None):
  self.c=TimelineCpu(library,True,{'functions':[]}) if native else Source(library,json.loads((REF/'original-functions.json').read_text()));self.native=native;c=self.c;d=c.data
  self.s=d+0x10000;self.objects=[0]+[d+0x20000+i*0x2000 for i in range(4)];self.ns=[0]+[0x100000000+i*0x10000 for i in range(1,5)];self.owner_views=[0]+[d+0x40000+i*0x100 for i in range(4)];self.args=d+0x50000;self.vector=d+0x50100;self.values=d+0x50200;self.output=d+0x51000;self.returned=d+0x52000;self.bindings=d+0x53000;self.services=d+0x53100;self.callback=d+0x54000;self.vt=d+0x55000;self.dead_stub=self.vt+0x100;self.error=d+0x56000;self.sight_first=d+0x57000;self.sight_second=d+0x57100;self.active=False
  if native:
   c.uc.mem_write(self.callback,bytes.fromhex('c0035fd6'));c.uc.mem_write(self.services,struct.pack('<QQ',0,self.callback));self.radii=radii
  else:
   self.table=d+0x60000;c.blob=(ASSETS/'ai_pyarray.bin').read_bytes();c.cursor=4;self.radii=[]
   for i in range(struct.unpack_from('<I',c.blob)[0]):c.invoke(0x506f3c,[self.table+68*i,c.stream]);self.radii.append(self.word(self.table+68*i+60))
   assert c.cursor==len(c.blob)
   got=0x3a3000+self.word(0x3a301c);c.pointer(self.word(got+self.word(0x3a3020)),len(self.radii))
   got=0x3a3038+self.word(0x3a304c);p=d+0x70000;c.pointer(got+self.word(0x3a3050),p);c.pointer(p,self.table)
   c.pointer(self.vt+0x34,self.dead_stub);c.uc.mem_write(self.dead_stub,words(0xea000000|(((0x3a2ed4-self.dead_stub-8)//4)&0xffffff)))
   c.pointer(self.args+4,self.vector)
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def word(self,p):return struct.unpack('<I',self.c.uc.mem_read(p,4))[0]
 def ret(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def text(self,p):
  b=b''
  while self.c.uc.mem_read(p+len(b),1)!=b'\0':b+=bytes(self.c.uc.mem_read(p+len(b),1))
  return b
 def ident(self,p):
  if not p:return 0
  return self.ns.index(p) if self.native else self.objects.index(p)
 def pointer(self,index):return self.ns[index] if self.native else self.objects[index]
 def snapshot(self):
  c=self.c
  if self.native:
   _,owner,cand,target,last,alive,sight,changed,_,_=struct.unpack('<5Q4BI',c.uc.mem_read(self.s,48));oi=self.owner_views.index(owner)
   return [oi,self.ident(cand),self.ident(target),self.ident(last),alive,sight,changed,self.word(self.owner_views[1]+8)&65535,self.word(self.owner_views[4]+8)&65535]
  return [self.ident(self.word(self.s+4)),self.ident(self.word(self.s+0x3c)),self.ident(self.word(self.s+0x40)),self.ident(self.word(self.s+0x44)),*c.uc.mem_read(self.s+0x48,2),c.uc.mem_read(self.s+0x4c,1)[0],self.word(self.objects[1]+0x14d0)&65535,self.word(self.objects[4]+0x14d0)&65535]
 def stamp(self,op,subject=0):self.trace.append([op,subject,*self.snapshot()])
 def native_ai_id(self,subject):
  c=self.c;c.uc.mem_write(self.sight_first+1024,words(0,self.row[19 if subject==1 else 20]))
  saved=c.uc.context_save();stack=c.stack;c.stack=c.uc.reg_read(c.sp)-0x10000
  try:
   assert c.invoke('dh2_character_target_ai_id',[self.output+1024,self.sight_first+1024,len(self.radii)])==0
   return self.word(self.output+1024)
  finally:c.stack=stack;c.uc.context_restore(saved)
 def change(self,op):
  c=self.c;mask=self.row[15]
  def write_target(code):c.pointer(self.s+24 if self.native else self.s+0x40,self.pointer(code))
  if op==1:
   if mask&1:write_target(3)
   if mask&2:c.pointer(self.s+8 if self.native else self.s+4,self.owner_views[4] if self.native else self.objects[4])
   if mask&4:c.pointer(self.s+32 if self.native else self.s+0x44,self.pointer(3))
  if op==3:
   if mask&8:write_target(3)
   if mask&16:write_target(0)
  if op==4 and mask&32:c.pointer(self.s+8 if self.native else self.s+4,self.owner_views[4] if self.native else self.objects[4])
  trigger={1:1,2:3,3:1,4:6}.get(self.row[16])
  if trigger==op and not self.nested:
   self.nested=True;self.stamp(7);saved=c.uc.context_save();stack=c.stack;c.stack=c.uc.reg_read(c.sp)-0x10000
   try:
    if self.native:assert c.invoke('dh2_character_ai_set_target',[self.s,self.pointer(3),0 if self.row[16]==3 else 1,self.services])==0
    else:c.invoke(0x3d6890,[self.s,self.pointer(3),0 if self.row[16]==3 else 1])
   finally:c.stack=stack;c.uc.context_restore(saved)
   self.stamp(8)
 def hook(self,uc,address,size,_):
  if not self.active:return
  c=self.c
  if not self.native:
   if address==0x337888:self.stamp(0);self.ret()
   elif address==0x3140ec:
    c.uc.mem_write(c.reg(0),bytes(24));c.pointer(c.reg(0)+20,c.reg(1));self.ret(c.reg(0))
   elif address in (0x318254,0x708f00,0x310440):self.ret()
   elif address==0x337a88:
    name=self.text(self.word(c.reg(1)+20));assert name in (b'IsTracingCharAITarget',b'isTracingCharAITarget');op=1 if name[0]==73 else 6;self.stamp(op);self.change(op);self.ret(self.row[11 if op==1 else 12])
   elif address==0x3a2fec:self.stamp(2,self.ident(c.reg(0)))
   elif address==self.dead_stub:self.stamp(3,self.ident(c.reg(0)));self.change(3)
   elif address==0x3d4ed8:self.stamp(4,self.ident(c.reg(1)));self.change(4)
   elif address==0x37c7e4:self.results=[1,c.reg(1)];self.ret()
   elif address==0x37c9f8:self.results=[7,self.ident(c.reg(1))];self.ret()
  elif address==self.callback:
   op,_,subject,text=struct.unpack('<IIQQ',c.uc.mem_read(c.reg(2),24));subject=self.ident(subject);traceop=6 if op==1 and self.text(text)[0]!=73 else op;self.stamp(traceop,subject);self.change(traceop);value=0
   if op==1:value=self.row[11 if traceop==1 else 12]
   elif op==2:value=self.native_ai_id(subject)
   elif op==3:value=self.row[13 if subject==2 else 14] if subject in (2,3) else 0
   elif op==4:
    current=self.snapshot();subject=subject or current[2];owner=current[0]
    if subject:
     def pos(index):
      bit=(self.row[21]>>(2*(index-1)))&3;at=23+(12 if bit==3 else 0)+(index-1)*3;return self.row[at:at+3]
     c.uc.mem_write(self.sight_first,words(*pos(owner)));c.uc.mem_write(self.sight_second,words(*pos(subject)));ai=self.native_ai_id(owner)
     self.stamp(2,owner) # Actual AI_IsInSight's radius getter calls GetCharAIId.
     saved=c.uc.context_save();stack=c.stack;c.stack=c.uc.reg_read(c.sp)-0x10000
     try:c.uc.reg_write(UC_ARM64_REG_S0,self.radii[ai]);assert c.invoke('dh2_character_target_sight',[self.output+512,self.sight_first,self.sight_second])==0;value=self.word(self.output+512)
     finally:c.stack=stack;c.uc.context_restore(saved)
   c.uc.mem_write(c.reg(3),words(value));self.ret()
 def execute(self,row):
  self.row=row;self.trace=[];self.results=[];self.nested=False;c=self.c;self.active=False
  if self.native:
   for i in range(1,5):c.uc.mem_write(self.owner_views[i],struct.pack('<QHHI',self.pointer(i),row[9 if i==1 else 10],0,0))
   c.uc.mem_write(self.s,struct.pack('<5Q4BI',0x200000000,self.owner_views[1],*[self.pointer(x) for x in row[3:6]],*row[6:9],0,0));c.uc.mem_write(self.bindings,struct.pack('<6Q',self.s,0,self.callback,0,0,0))
   for i in range(max(row[18],1)):c.uc.mem_write(self.values+40*i,struct.pack('<4I3Q',row[17],0,0,0,0,0,self.pointer(row[1])))
  else:
   c.uc.mem_write(self.s,bytes(0x100));c.pointer(self.s+4,self.objects[1]);c.pointer(self.s+0x3c,self.pointer(row[3]));c.pointer(self.s+0x40,self.pointer(row[4]));c.pointer(self.s+0x44,self.pointer(row[5]));c.uc.mem_write(self.s+0x48,bytes(row[6:8]));c.uc.mem_write(self.s+0x4c,bytes([row[8]]))
   for i in range(1,5):
    p=self.objects[i];c.uc.mem_write(p,bytes(0x1800));c.pointer(p,self.vt);c.uc.mem_write(p+0x14d0,struct.pack('<H',row[9 if i==1 else 10]));c.pointer(p+0xffc,row[19 if i==1 else 20]);c.uc.mem_write(p+0x1449,bytes([row[13 if i==2 else 14] if i in (2,3) else 0]));c.uc.mem_write(p+0x160,words(*row[23+(i-1)*3:26+(i-1)*3]));c.uc.mem_write(p+0x184,words(*row[35+(i-1)*3:38+(i-1)*3]));flags=(row[21]>>(2*(i-1)))&3;c.pointer(p+0x180,123 if flags&1 else 0);c.uc.mem_write(p+0x80,bytes([1 if flags&2 else 0]))
   c.pointer(self.vector,self.values);c.pointer(self.vector+4,self.values+row[18]*144)
   for i in range(max(row[18],1)):c.uc.mem_write(self.values+144*i,bytes(144));c.pointer(self.values+144*i+4,row[17]);c.pointer(self.values+144*i+0x6c,self.pointer(row[1]))
  self.active=True;op=row[0]
  if self.native:
   if op==0:result=c.invoke('dh2_character_ai_set_target',[self.s,self.pointer(row[1]),row[2],self.services])
   elif op==1:result=c.invoke('dh2_character_clear_target',[self.s,self.services])
   elif op==2:result=c.invoke('dh2_character_target_set_values',[self.bindings,self.values,row[18]])
   elif op==3:
    result=c.invoke('dh2_character_target_has_lua',[self.bindings,self.values,row[18],self.output,1,self.returned,self.error,256]);self.results=[self.word(self.output),self.word(self.output+12)]
   else:result=c.invoke('dh2_character_target_identity',[self.output,self.s]);self.results=[7,self.ident(struct.unpack('<Q',c.uc.mem_read(self.output,8))[0])]
   assert result==0
  else:
   owner=self.objects[1];c.pointer(owner+0x408,self.pointer(row[4]))
   if op==0:c.invoke(0x3d6890,[self.s,self.pointer(row[1]),row[2]])
   elif op==1:
    # Wrapper owns embedded AI; move the configured state to that true owner.
    saved=self.s;self.s=owner+0x3c8;c.uc.mem_write(self.s,bytes(c.uc.mem_read(saved,0x100)));c.invoke(0x3b5690,[self.args,self.output,owner]);c.uc.mem_write(saved,bytes(c.uc.mem_read(self.s,0x100)));self.s=saved
   elif op==2:
    saved=self.s;self.s=owner+0x3c8;c.uc.mem_write(self.s,bytes(c.uc.mem_read(saved,0x100)));c.invoke(0x3b8f38,[self.args,self.output,owner]);c.uc.mem_write(saved,bytes(c.uc.mem_read(self.s,0x100)));self.s=saved
   else:c.invoke(0x3b6f50 if op==3 else 0x3b6c7c,[self.args,self.output,owner])
  self.active=False;return self.results,self.snapshot(),self.trace
def cases(radii):
 base=[0,2,0,0,0,3,127,255,99,1234,5678,0,1,0,1,0,0,7,1,40,68,0,0]+[fword(x) for x in (0,0,0,100,0,0,10000,0,0,2000,0,0,10,0,0,110,0,0,10010,0,0,2010,0,0)]
 rows=[]
 for op,incoming,target,last,debug,mode in itertools.product(range(5),(0,2,3),(0,2,3),(0,2,3),(0,1),(0,1,255)):
  r=base.copy();r[0:3]=[op,incoming,mode];r[4:6]=[target,last];r[11]=debug;rows.append(r)
 for op,kind,count,identity in itertools.product((2,3,4),range(9),(0,1,3),(0,2)):
  r=base.copy();r[0]=op;r[1]=identity;r[17:19]=[kind,count];rows.append(r)
 for mutation,nested,incoming,target,debug in itertools.product(range(64),range(5),(0,2),(0,2),(0,1)):
  r=base.copy();r[15:17]=[mutation,nested];r[1]=incoming;r[4]=target;r[11]=debug;rows.append(r)
 for dead,flags,ai in itertools.product((0,1,2,127,255),(0,0x55,0xaa,0xff),(0,8,40,68,76,0xffffffff)):
  r=base.copy();r[13]=dead;r[21]=flags;r[19]=ai;rows.append(r)
 rng=random.Random(20261004)
 for i in range(300):
  r=base.copy();r[19]=rng.choice((0,8,40,68));r[21]=rng.randrange(256);r[23:]=[rng.choice((0,0x80000000,0x7f800000,0xff800000,0x7fc01234,fword(rng.uniform(-100000,100000)))) for _ in range(24)];rows.append(r)
 # Exact source strict sight boundary with real Monster radius and cached node.
 for bits in (radii[40]-1,radii[40],radii[40]+1):
  r=base.copy();r[26]=bits;rows.append(r)
 return rows
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,default=REPO/'.local-inputs/character-target-bindings-discovery/libcharacter_target_bindings.so');a=p.parse_args();build=json.loads((a.library.parent/'arm64-build.json').read_text(encoding='utf-8-sig'));assert sha(a.library)==build['library_sha256'];assert all(sha(REPO/k)==v for k,v in build['source_bindings'].items())
 old=Machine(REPO/'.local-inputs/libDungeonHunter2.so',False);new=Machine(a.library,True,old.radii);rows=cases(old.radii);records=[];callbacks=0
 for i,row in enumerate(rows):
  expected=old.execute(row);actual=new.execute(row)
  assert actual==expected,(i,row[:23],expected,actual)
  results,state,trace=expected;records.append(words(*row)+words(len(results),*results,*state,len(trace))+b''.join(words(*x) for x in trace));callbacks+=len(trace)
 gold=b'CTB1'+words(len(rows),47,len(old.radii),*old.radii)+b''.join(records);dest=REF/'target-original-gold.bin';dest.write_bytes(gold)
 assert sha(a.library)==build['library_sha256'];assert all(sha(REPO/k)==v for k,v in build['source_bindings'].items())
 report=dict(validation='PASS',original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),compiler_inputs=build,manifest_sha256=sha(REF/'original-functions.json'),gold_sha256=sha(dest),cases=len(rows),ordered_services=callbacks,original_ai_rows=len(old.radii),asset_sha256=sha(ASSETS/'ai_pyarray.bin'),source_sha256={str(Path(__file__).relative_to(REPO)):sha(Path(__file__))},mismatches=0,scope=__doc__,full_GetTarget_Lua_projection=False,whole_world_registry=False)
 (ROOT/'reports/character-target-bindings-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('validation','cases','ordered_services','original_ai_rows','mismatches')}))
if __name__=='__main__':main()

