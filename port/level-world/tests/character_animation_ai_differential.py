"""Original AI animation-consumer instructions vs optimized ARM64 source.

Live resolved scheduler/AI/controller queries are named synchronous services.
Ordered requests, service-entry state and final state compare exactly.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from visual_timeline_differential import TimelineCpu

def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def word(c,p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def fb(f):return struct.unpack('<I',struct.pack('<f',f))[0]
ADDRESSES=(0x3d4204,0x3d3ff8,0x3d4120,0x3d4044,0x3d3e44,0x3d3dd4,0x3d3d68)
class Machine:
 def __init__(self,path,native,manifest):
  self.c=TimelineCpu(path,native,manifest);self.native=native;c=self.c;d=c.data
  self.s=d+0x1000;self.owner=d+0x4000;self.ctrl=d+0xa000;self.target=d+0x9000;self.other=d+0xb000;self.service=d+0xc000;self.position=d+0xe000;self.services=d+0xf000
  self.ids={self.owner:1,self.ctrl:2,self.target:3,self.other:4};self.ptrs={v:k for k,v in self.ids.items()}
  self.calls=[];c.uc.hook_add(UC_HOOK_CODE,self.hook)
  if native:c.uc.mem_write(self.services,struct.pack('<QQ',d+0xf100,self.service))
  else:
   c.pointer(self.s,d+0x8000);c.pointer(d+0x8000+0xa4,self.service+4)
   for p in (self.target,self.other):c.pointer(p,d+0x9200)
   c.pointer(d+0x9200+0x34,self.service);c.pointer(self.owner,d+0x9400);c.pointer(d+0x9400+0x124,self.service+8)
 def snapshot(self):
  c=self.c
  if self.native:
   raw=bytearray(c.uc.mem_read(self.s,96))
   for i in range(4):struct.pack_into('<Q',raw,8*i,self.ids.get(struct.unpack_from('<Q',raw,8*i)[0],0))
   return bytes(raw)
  ptrs=[word(c,p) for p in (self.s+4,self.owner+0x378,self.s+0x40,self.owner+0x408)]
  vals=[word(c,self.owner+0x4c8),word(c,self.owner+0x520),c.uc.mem_read(self.s+0x4a,1)[0],c.uc.mem_read(self.s+0x4b,1)[0],word(c,self.s+0x74)]
  vals += [c.uc.mem_read(self.s+x,1)[0] for x in (0x78,0x79,0x7a,0xd0,0xd1)]
  vals += [struct.unpack('<b',c.uc.mem_read(self.owner+0x14a8,1))[0]&0xffffffff,0]
  return struct.pack('<4Q',*(self.ids.get(p,0) for p in ptrs))+words(*vals)+bytes(c.uc.mem_read(self.owner+0x1a8,12))+words(0)
 def put_state(self,raw):
  c=self.c;ptrs=struct.unpack_from('<4Q',raw);vals=struct.unpack_from('<12I',raw,32)
  if self.native:
   blob=bytearray(raw)
   for i,p in enumerate(ptrs):struct.pack_into('<Q',blob,8*i,self.ptrs.get(p,0))
   c.uc.mem_write(self.s,bytes(blob));return
  for address,p in zip((self.s+4,self.owner+0x378,self.s+0x40,self.owner+0x408),ptrs):c.pointer(address,self.ptrs.get(p,0))
  for address,v in zip((self.owner+0x4c8,self.owner+0x520,self.s+0x74),(vals[0],vals[1],vals[4])):c.pointer(address,v)
  for address,v in zip((self.s+0x4a,self.s+0x4b,self.s+0x78,self.s+0x79,self.s+0x7a,self.s+0xd0,self.s+0xd1,self.owner+0x14a8),(vals[2],vals[3],*vals[5:11])):c.uc.mem_write(address,bytes([v&255]))
  c.uc.mem_write(self.owner+0x1a8,raw[80:92])
 def ret(self,value=0):self.c.put(0,value);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def mutation(self,service):
  if service!=self.params[8]:return
  blob=bytearray(self.snapshot());kind=self.params[7]
  if kind==1:struct.pack_into('<i',blob,32,7);struct.pack_into('<i',blob,48,-7)
  elif kind==2:struct.pack_into('<3f',blob,80,9,-8,7)
  elif kind in (3,4):struct.pack_into('<Q',blob,16,0);struct.pack_into('<i',blob,72,8 if kind==4 else 0)
  elif kind==5:struct.pack_into('<Q',blob,16,4)
  elif kind==6:struct.pack_into('<i',blob,48,987)
  elif kind==7:struct.pack_into('<I',blob,52,1)
  self.put_state(bytes(blob))
 def record(self,service,arg=0,subject=None,payload=0):
  if subject is None:subject=self.owner
  canonical=self.ids.get(subject,0);normalized=self.ids.get(payload,payload)
  request=words(service,arg,0,0)+struct.pack('<QQ',canonical,normalized)
  self.calls.append(request+self.snapshot());self.mutation(service)
  return {0:self.params[0],1:self.params[1],2:self.params[2],3:self.params[3],5:self.params[6],9:self.params[4],10:self.params[5]}.get(service,0)
 def hook(self,uc,address,size,data):
  c=self.c
  if self.native:
   if address!=self.service:return
   assert c.reg(0)==c.data+0xf100 and c.reg(1)==self.s
   req=c.reg(2);response=c.reg(3);service,arg,r0,r1,subject,payload=struct.unpack('<4I2Q',uc.mem_read(req,32));assert (r0,r1)==(0,0)
   result=self.record(service,arg,subject,payload);uc.mem_write(response,words(result,*self.params[9:12]));self.ret();return
  direct={0x3c01ac:0,0x3c932c:1,0x3c934c:2,0x3a346c:3,0x3935dc:4,0x3d4c9c:5,0x405540:6,0x4052bc:7,0x3d8d70:11,0x3c9484:12,0x3c9464:13,0x3c948c:14,0x3a4d5c:15,self.service:9,self.service+4:8,self.service+8:10}
  if address not in direct:return
  service=direct[address];arg=c.reg(1) if service in (8,12,14,15) else 0
  subject=c.reg(0) if service in (4,6,7,9) else self.owner
  payload=c.reg(1) if service in (6,7) else c.reg(2) if service==15 else 0
  result=self.record(service,arg,subject,payload)
  if service==4:uc.mem_write(self.position,words(*self.params[9:12]));result=self.position
  self.ret(result)
 def execute(self,op,state,params):
  self.params=params;self.calls=[];self.put_state(state)
  result=self.c.invoke('dh2_character_animation_ai',[self.s,op,self.services]) if self.native else self.c.invoke(ADDRESSES[op],[self.s])
  return result,self.snapshot(),tuple(self.calls)

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,default=ROOT/'.local-inputs/libDungeonHunter2.so');p.add_argument('--library',type=Path,default=ROOT/'.local-inputs/character-animation-ai-discovery/oracle.so');p.add_argument('--report',type=Path,default=ROOT/'port/level-world/reports/character-animation-ai-arm64-differential.json');p.add_argument('--gold',type=Path,default=ROOT/'port/level-world/reference/character-animation-ai/consumer-fixtures.bin');a=p.parse_args()
 manifest=json.loads((ROOT/'port/level-world/reference/character-animation-ai/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=Machine(a.engine,False,manifest);new=Machine(a.library,True,{'functions':[]});rng=random.Random(0x2627);records=[];requests=0
 def compare(op,state,params):
  nonlocal requests
  expected=old.execute(op,state,params);actual=new.execute(op,state,params)
  assert expected==actual,(len(records),op,struct.unpack('<16I',params if isinstance(params,bytes) else words(*params)),expected,actual)
  result,after,calls=expected;assert result==1;requests+=len(calls)
  records.append(words(op)+state+words(*params)+after+words(len(calls))+b''.join(calls))
 def state(depth=0,target=3,continued=0,stop=0,seeking=1,flags=0,style=0):
  return struct.pack('<4Q',1,2,target,4)+words(depth,flags,seeking,0,-3,continued,2,3,4,stop,style,0)+words(fb(0.),fb(0.),fb(0.),0)
 def params(st=5,step=0,count=3,combo=1,target_return=0,can_range=1,radius=4.,mutation=0,service=0,point=(3.,0.,0.)):
  return [st&0xffffffff,step&0xffffffff,count&0xffffffff,combo,target_return,can_range,fb(radius),mutation,service,*map(fb,point),1,0,0,0]
 for op in range(7):
  for i in range(280):
   depth=rng.choice((0,1,2,-1));n=rng.choice((0,1,2,3,4,0x80000000));step=rng.choice((0,1,n-1,n-2,-1,7));mutation=rng.randrange(8);service=rng.choice((0,1,2,4,5,7,9,11))
   compare(op,state(depth,rng.choice((0,3)),rng.choice((0,1,2,255)),rng.choice((0,1,255)),rng.choice((0,1,2)),rng.choice((0,0x1000,0xffffffff)),rng.choice((-128,0,8,127))),params(rng.choice((-1,3,4,5,6,7,12)),step,n,rng.choice((0,1,2)),rng.choice((0,1,0xffffffff)),rng.choice((0,1,2)),rng.choice((0.,4.,9.,float('nan'),float('inf'))),mutation,service))
 # Exercise callbacks reachable in each branch explicitly, not merely randomly.
 for op in (3,4,5,6):
  for depth in (0,1,2):
   for step in (0,1,2,3):
    for continued in (0,1,2,255):
     compare(op,state(depth,3,continued,1),params(step=step,mutation=6,service=7))
 for target in (0,3):
  for result in (0,1):
   for style in (0,8):
    for mutation in (0,3,4):compare(4,state(1,target,1,0,style=style),params(step=1,count=3,target_return=result,mutation=mutation,service=9))
 for radius,point in ((4.,(2.,0.,0.)),(4.,(1.,0.,0.)),(4.,(3.,0.,0.)),(float('nan'),(3.,0.,0.)),(4.,(float('inf'),0.,0.)),(4.,(float('nan'),0.,0.)),(float('inf'),(1e30,1e30,1e30))):
  for mutation,service in ((0,0),(2,4),(5,5)):compare(2,state(),params(radius=radius,point=point,mutation=mutation,service=service))
 # Pure source table producer and HasComboAttack execute their real lookups.
 c=old.c;owner=old.owner;bank=c.data+0x20000;seq=c.data+0x24000;countptr=c.data+0x28000;tablecountptr=countptr+4
 base=0x3a3478+8+word(c,0x3a34e0)
 ptrbank=countptr+8;ptrseq=countptr+12;c.pointer(base+word(c,0x3a34e4),ptrbank);c.pointer(ptrbank,bank);c.pointer(base+word(c,0x3a34e8),countptr);c.pointer(base+word(c,0x3a34ec),ptrseq);c.pointer(ptrseq,seq)
 tablebase=0x3a3238+8+word(c,0x3a325c);c.pointer(tablebase+word(c,0x3a3260),tablecountptr)
 scalar=[]
 # Separate CPU avoids the consumer's modeled HasCombo entry hook.
 q=TimelineCpu(a.engine,False,manifest);qw=lambda p:word(q,p)
 for p0,p1 in ((base+qw(0x3a34e4),ptrbank),(ptrbank,bank),(base+qw(0x3a34e8),countptr),(base+qw(0x3a34ec),ptrseq),(ptrseq,seq),(tablebase+qw(0x3a3260),tablecountptr)):q.pointer(p0,p1)
 for attack in (-1,0,1,5,10):
  for count in (0,1,6,10):
   for typ in (0,1,2):
    q.pointer(owner+0x1000,0);q.pointer(tablecountptr,1);q.pointer(countptr,count);q.pointer(bank+4,attack&0xffffffff)
    if attack>=0:q.pointer(seq+20*attack+16,typ)
    expected=q.invoke(0x3a346c,[owner]);actual=new.c.invoke('dh2_character_animation_has_combo',[attack&0xffffffff,count,typ]);assert expected==actual;scalar.append(['combo',attack,count,typ,expected])
 for prop in (-1,0,1,17,48,100):
  for count in (0,1,18,49,100):
   q.pointer(owner+0x1000,prop&0xffffffff);q.pointer(tablecountptr,count);expected=q.invoke(0x3a3228,[owner]);actual=new.c.invoke('dh2_character_animation_table_id',[prop&0xffffffff,count]);assert expected==actual;scalar.append(['table',prop,count,expected])
 scalar_blob=words(len(scalar))+b''.join(words(0,r[1],r[2],r[3],r[4]) if r[0]=='combo' else words(1,r[1],r[2],0,r[3]) for r in scalar)
 blob=b'AAI1'+words(len(records))+b''.join(records)+scalar_blob;a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(blob)
 # Validation rejection must precede any state writes or borrowed callbacks.
 rejects=0
 def reject(raw,op=0,null_state=False,null_service=False,missing_invoke=False,overlapping=False):
  nonlocal rejects
  new.put_state(raw);before=bytes(new.c.uc.mem_read(new.s,96));new.calls=[]
  new.c.uc.mem_write(new.services,struct.pack('<QQ',new.c.data+0xf100,0 if missing_invoke else new.service))
  result=new.c.invoke('dh2_character_animation_ai',[0 if null_state else new.s,op,0 if null_service else new.s if overlapping else new.services])
  assert result&0xffffffff==0xffffffff and bytes(new.c.uc.mem_read(new.s,96))==before and not new.calls
  rejects+=1
 reject(state(),null_state=True);reject(state(),null_service=True);reject(state(),missing_invoke=True);reject(state(),op=7);reject(state(),overlapping=True)
 for offset,value,width in ((0,0,8),(8,0,8),(76,1,4),(92,1,4),(40,256,4),(44,256,4),(52,256,4),(56,256,4),(60,256,4),(64,256,4),(68,256,4),(72,128,4)):
  raw=bytearray(state());struct.pack_into('<Q' if width==8 else '<I',raw,offset,value);reject(bytes(raw))
 sources=['port/level-world/character_animation_ai.hpp','port/level-world/character_animation_ai.cpp','port/level-world/tests/character_animation_ai_differential.py','port/level-world/tests/character_animation_ai.cpp']
 report={'validation':'PASS','original_sha256':manifest['original_sha256'],'library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'corpus_sha256':hashlib.sha256(blob).hexdigest(),'source_sha256':{f:hashlib.sha256((ROOT/f).read_bytes()).hexdigest() for f in sources},'consumer_comparisons':len(records),'ordered_service_requests':requests,'scalar_comparisons':len(scalar),'atomic_rejection_checks':rejects,'scalar_records':scalar,'mismatches':0,'scope':'Actual original seven consumers/dispatchers vs optimized native ARM64. Scheduler/HasCombo/controller/target/AIS/character-event service boundaries are fixtures with live mutation, ordered entry-state comparison and >4GiB identities. Pure HasCombo and table-ID original lookup instructions independently execute. Full AIS/game-world ownership remains outside this kernel audit.'}
 report['reference_sha256']={str(f.relative_to(ROOT)).replace('\\','/'):hashlib.sha256(f.read_bytes()).hexdigest() for f in sorted((ROOT/'port/level-world/reference/character-animation-ai').rglob('*')) if f.is_file() and f.suffix in ('.json','.asm')}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='scalar_records'}))
if __name__=='__main__':main()
