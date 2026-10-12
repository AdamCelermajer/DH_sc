"""Original spatial wrappers/helpers versus optimized native64 instructions.

Named lookup/cast and return-vector append are explicit services. Original Args,
Value.getString/getUserData and wrapper arithmetic execute. Borrowed coordinate
mutations occur during lookup; all finite outputs are exact, copied NaNs exact,
arithmetic NaNs class-equivalent only. No complete manager/Scene/backend proof.
"""
import argparse,hashlib,itertools,json,random,struct
from pathlib import Path
from unicorn import UC_HOOK_CODE
from visual_timeline_differential import TimelineCpu
WORLD=Path(__file__).resolve().parents[1];ROOT=WORLD.parents[1];REF=WORLD/'reference/character-spatial-bindings'
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def fw(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def nan(b):return (b&0x7f800000)==0x7f800000 and b&0x7fffff
def equal(op,a,b):return len(a)==len(b) and all(x==y or op!=0 and nan(x) and nan(y) for x,y in zip(a,b))
class Machine:
 def __init__(self,path,native,manifest):
  self.c=TimelineCpu(path,native,manifest);self.native=native;c=self.c;d=c.data
  self.args=d+0x1000;self.vector=d+0x1100;self.records=d+0x2000;self.texts=[d+0x3000,d+0x3100];self.objects=[0,d+0x4000,d+0x4400,d+0x4800];self.positions=[0]+[p if native else p+0x160 for p in self.objects[1:]]
  self.bindings=d+0x6000;self.lookup=d+0x7000;self.userdata=d+0x7100;self.output=d+0x8000;self.returned=d+0x9000;self.error=d+0xa000;self.manager=d+0xb000;self.active=False
  if native:
   for p in [self.lookup,self.userdata]:c.uc.mem_write(p,bytes.fromhex('c0035fd6'))
  else:
   c.pointer(self.args+4,self.vector)
   # Original PIC/GOT singleton receiver, read from actual linked literals.
   got=0x393470+self.word(0x3935d0);global_pointer=self.word(got+self.word(0x3935d8));c.pointer(global_pointer+0x38,self.manager)
   got=0x391454+self.word(0x39165c);global_pointer=self.word(got+self.word(0x391664));c.pointer(global_pointer+0x38,self.manager)
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def word(self,p):return int.from_bytes(self.c.uc.mem_read(p,4),'little')
 def text(self,p):
  b=bytearray()
  while True:
   v=self.c.uc.mem_read(p+len(b),1)[0]
   if not v:return bytes(b)
   b.append(v);assert len(b)<256
 def ret(self,value=0):c=self.c;c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def mutate(self,bit):
  if self.row['mutation']&bit:
   for i in range(3):self.c.uc.mem_write(self.positions[i+1],words(*self.row['replacement'][i*3:i*3+3]))
 def named(self,text):
  at=self.lookups;assert at<2 and text==bytes.fromhex(self.row['names'][at]).split(b'\0')[0];code=self.row['plan'][at];self.trace.extend([[0,at,code],[1,at,code]]);self.mutate(1<<at);self.lookups+=1;return code
 def hook(self,uc,address,size,_):
  if not self.active:return
  c=self.c
  if not self.native:
   if address==0x34aca0:
    assert c.reg(1)==self.manager and c.reg(3)==0xffffffff;assert self.word(c.uc.reg_read(c.sp))==self.word(c.uc.reg_read(c.sp)+4)==0
    code=self.named(self.text(c.reg(2)));c.pointer(c.reg(0),self.objects[code]);self.ret()
   elif address==0x33fee4:self.ret(self.word(c.reg(0)))
   elif address==0x31b5a0:
    identity=self.word(c.reg(0)+0x6c)
    if identity:code=self.objects.index(identity);self.trace.append([2,0,code]);self.mutate(4)
   elif address==0x37ccbc:self.result.append(c.reg(1));self.ret()
  elif address==self.lookup:
   assert c.reg(0)==self.bindings;code=self.named(self.text(c.reg(1)));c.pointer(c.reg(2),self.positions[code]);self.ret()
  elif address==self.userdata:
   assert c.reg(0)==self.bindings;identity=c.reg(1);code=(identity-UINT64_BASE)//0x1000;assert code in (1,2,3) and identity==UINT64_BASE+code*0x1000;self.trace.append([2,0,code]);self.mutate(4);c.pointer(c.reg(2),self.positions[code]);self.ret()
 def execute(self,row):
  self.row=row;self.trace=[];self.result=[];self.lookups=0;c=self.c
  for i in range(3):c.uc.mem_write(self.positions[i+1],words(*row['positions'][i*3:i*3+3]))
  for p,text in zip(self.texts,row['names']):c.uc.mem_write(p,bytes.fromhex(text)+b'\0')
  n=row['count'];op=row['op']
  if self.native:
   c.uc.mem_write(self.bindings,struct.pack('<5Q',self.positions[3],self.bindings,self.lookup,self.userdata,0))
   for i in range(n):c.uc.mem_write(self.records+40*i,words(row['kinds'][i],0,fw(99.5),0)+struct.pack('<3Q',self.texts[min(i,1)],len(bytes.fromhex(row['names'][min(i,1)])),UINT64_BASE+row['identities'][i]*0x1000 if row['identities'][i] else 0))
   c.uc.mem_write(self.output,bytes(120));c.uc.mem_write(self.returned,words(0xdeadbeef));self.active=True
   assert c.invoke(('dh2_character_get_position','dh2_character_get_distance_from','dh2_character_get_distance_between')[op],[self.bindings,self.records,n,self.output,3,self.returned,self.error,256])==0
   count=self.word(self.returned);self.result=[self.word(self.output+40*i+8) for i in range(count)]
  else:
   c.uc.mem_write(self.vector,words(self.records,self.records+112*n,self.records+112*n))
   for i in range(n):
    p=self.records+112*i;c.uc.mem_write(p,bytes(112));c.pointer(p+4,row['kinds'][i]);c.pointer(p+0x20,self.texts[min(i,1)]);c.pointer(p+0x6c,self.objects[row['identities'][i]])
   self.active=True;c.invoke((0x38e700,0x393444,0x391438)[op],[self.args,self.output,self.objects[3]])
  self.active=False;state=[self.word(p+4*j) for p in self.positions[1:] for j in range(3)];return self.result,self.trace,state
UINT64_BASE=0xfedcba9800000000
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,default=ROOT/'.local-inputs/character-spatial-bindings/oracle.so');a=p.parse_args()
 manifest=json.loads((REF/'original-functions.json').read_text());manifest['functions']+=json.loads((REF/'value-helpers/original-functions.json').read_text())['functions'];original=ROOT/'.local-inputs/libDungeonHunter2.so';assert sha(original)==manifest['original_sha256']
 old=Machine(original,False,manifest);new=Machine(a.library,True,{'functions':[]});rows=[];counts=[0]*3;services=0;mutations=0
 base=[fw(v) for v in (3,4,12,1,2,3,-1,-2,-3)];changed=[fw(v) for v in (11,17,23,-2,-5,-7,31,37,41)]
 def compare(op,count,kinds=(4,4,7),identities=(1,2,3),plan=(1,2),mutation=0,positions=base,replacement=changed,names=(b'first',b'second')):
  nonlocal services,mutations
  row=dict(op=op,count=count,kinds=list(kinds),identities=list(identities),plan=list(plan),mutation=mutation,positions=positions,replacement=replacement,names=[s.hex() for s in names]);expected=old.execute(row);actual=new.execute(row)
  assert equal(op,expected[0],actual[0]) and expected[1:]==actual[1:],(len(rows),row,expected,actual)
  counts[op]+=1;services+=len(expected[1]);mutations+=int(expected[2]!=positions);row.update(result=expected[0],services=expected[1],after=expected[2]);rows.append(row)
 for op in range(3):
  for count,k0,k1 in itertools.product(range(4),range(9),range(9)):compare(op,count,(k0,k1,7))
 for first,second,mutation in itertools.product(range(4),range(4),range(8)):compare(2,3,plan=(first,second),mutation=mutation)
 for kind,identity,mutation in itertools.product((4,7),range(4),range(8)):compare(1,2,(kind,7,0),(identity,2,0),plan=(identity,2),mutation=mutation)
 for names in [(b'',b'empty'),(b'first\0ignored',b'second\0tail'),(b'\xff\x80',b'\xfe'),(b'x'*127,b'y'*127)]:compare(2,2,names=names)
 edges=[0,0x80000000,1,0x007fffff,0x3f800000,0xbf800000,0x4b800001,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc12345,0x7f812345,0xffc54321]
 for b in edges:
  for axis in range(9):
   points=base.copy();points[axis]=b
   for op in range(3):compare(op,2,positions=points)
 rng=random.Random(0x393444)
 for i in range(400):
  points=[rng.getrandbits(32) for _ in range(9)]
  for op in range(3):compare(op,2,positions=points,plan=(i%4,(i//4)%4),mutation=i%8,replacement=[rng.getrandbits(32) for _ in range(9)])
 gold=REF/'spatial-original-gold.json';gold.write_text(json.dumps(dict(format='spatial-source-v1',cases=len(rows),rows=rows),separators=(',',':'))+'\n')
 corpus=words(0x31425053,len(rows))
 for row in rows:
  corpus+=words(row['op'],row['count'],*row['kinds'],*row['identities'],*row['plan'],row['mutation'],*row['positions'],*row['replacement'])
  for text in row['names']:raw=bytes.fromhex(text);corpus+=words(len(raw))+raw
  corpus+=words(len(row['result']),*row['result'],*row['after'],len(row['services']))+b''.join(words(*s) for s in row['services'])
 binary=REF/'spatial-original-gold.bin';binary.write_bytes(corpus)
 bindings=[WORLD/'character_spatial_bindings.hpp',WORLD/'character_spatial_bindings.cpp',Path(__file__),REF/'original-functions.json',REF/'value-helpers/original-functions.json',ROOT/'port/script-runtime/script_runtime.h']
 report=dict(validation='PASS',comparisons=len(rows),by_operation=counts,ordered_logical_lookup_cast_services=services,mutating_resolver_cases=mutations,mismatches=0,original_sha256=sha(original),library_sha256=sha(a.library),gold_json_sha256=sha(gold),gold_binary_sha256=sha(binary),source_sha256={p.relative_to(ROOT).as_posix():sha(p) for p in bindings},source_wrappers_and_value_helpers_executed=True,optimized_ARM64=True,native64_identity_fixture=hex(UINT64_BASE+0x1000),NaN_policy='Copied GetPosition words exact including NaN payload; arithmetic NaN class only; all other words exact.',explicit_services=['Original ObjectManager name lookup plus GameObject conversion, normalized against one combined genuine caller service.','Original native ReturnValues vector pushNumber, preserving exact float32 input words.','Original imported AEABI floating subtraction/multiply/add pure arithmetic contract.','Native64 userdata→position resolver models original direct GameObject pointer reads.'],full_manager_or_world_proof=False,whole_VM_differential=False,packaged_APK=False,scope=__doc__)
 (WORLD/'reports/character-spatial-bindings-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['validation','comparisons','ordered_logical_lookup_cast_services','mutating_resolver_cases','mismatches']}))
if __name__=='__main__':main()
