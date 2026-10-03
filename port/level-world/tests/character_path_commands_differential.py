"""Original complete PathTo and LookAt(Point) versus compiled native ARM64.

PathTo executes original list traversal, arithmetic and route-reuse gates;
complete FindPath remains one explicit synchronous service. LookAt executes
the complete original LookTowards with existing consistent IEEE/libm imports.
Finite words compare exactly; arithmetic NaN payloads compare by NaN class.
"""
import argparse,hashlib,itertools,json,math,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from visual_timeline_differential import TimelineCpu
from navigation_differential import equal
from unicorn.arm64_const import UC_ARM64_REG_S0
from aggro_differential import float_bits
from combat_result_differential import floating
class PathCpu(TimelineCpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)!='atanf':return super().external(uc,address,size,unused)
  raw=uc.reg_read(UC_ARM64_REG_S0) if self.arm64 else self.reg(0);result=float_bits(math.atan(floating(raw)))
  if self.arm64:uc.reg_write(UC_ARM64_REG_S0,result)
  else:self.put(0,result)
  self.import_calls['atanf']=self.import_calls.get('atanf',0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Machine:
 def __init__(self,path,native,manifest):
  self.c=PathCpu(path,native,manifest);self.native=native;c=self.c;d=c.data
  self.owner=d+0x10000;self.target=d+0x20000;self.state=d+0x30000;self.services=d+0x40000;self.callback=d+0x50000;self.out=d+0x60000;self.calls=[]
  c.uc.mem_write(self.services,struct.pack('<QQ',0,self.callback));c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def finish(self,value=0):c=self.c;c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def hook(self,uc,address,size,unused):
  c=self.c
  if self.native:
   if address!=self.callback:return
   raw=bytes(uc.mem_read(c.reg(1),32));owner=struct.unpack_from('<Q',raw)[0];assert owner==self.owner
   self.calls.append(struct.pack('<Q',1)+raw[8:]);uc.mem_write(c.reg(2),words(self.answer));self.finish(self.fail)
  elif address==0x52db48:
   assert c.reg(1)==self.owner+0x1c8 and c.reg(2)==self.target
   self.calls.append(struct.pack('<QII',1,c.reg(3),0)+bytes(uc.mem_read(c.reg(2),12))+words(0));self.finish(self.answer)
 def path_to(self,raw,missing=False,fail=0):
  c=self.c;disabled,length,limit,self.answer=struct.unpack('<4I',raw[:16]);self.fail=fail;self.calls=[]
  c.uc.mem_write(self.target,raw[28:40])
  if self.native:
   c.uc.mem_write(self.state,struct.pack('<Q4I',self.owner,disabled,int(bool(length)),limit,0)+raw[16:28]+words(0));c.uc.mem_write(self.out,bytes.fromhex('adbeadde')*4)
   c.uc.mem_write(self.services,struct.pack('<QQ',0,0 if missing else self.callback));result=c.invoke('dh2_character_path_to',[self.out,self.state,self.target,self.services]);out=bytes(c.uc.mem_read(self.out,16))
  else:
   c.uc.mem_write(self.owner+0x84,bytes([disabled]));c.pointer(self.owner+0x26c,limit);c.uc.mem_write(self.owner+0x208,raw[16:28]);head=self.owner+0x200;nodes=[self.owner+0x800+i*16 for i in range(length)];c.pointer(head,nodes[0] if nodes else head)
   for i,node in enumerate(nodes):c.pointer(node,nodes[i+1] if i+1<len(nodes) else head)
   c.invoke(0x3939f0,[self.owner,self.target]);result=0;out=words(1,limit or 30,self.answer,0) if self.calls else bytes(16)
  return result,out,tuple(self.calls)
 def look_at(self,raw):
  c=self.c;alias=struct.unpack_from('<I',raw,28)[0];c.uc.mem_write(self.target,raw[16:28]);self.calls=[]
  if self.native:
   c.uc.mem_write(self.state,raw[:16]);assert c.invoke('dh2_character_look_at_point',[self.state,self.state if alias else self.target])==0;return bytes(c.uc.mem_read(self.state,16))
  c.uc.mem_write(self.owner+0x160,raw[:12]);c.uc.mem_write(self.owner+0x178,raw[12:16]);c.invoke(0x393cec,[self.owner,self.owner+0x160 if alias else self.target]);return bytes(c.uc.mem_read(self.owner+0x160,12))+bytes(c.uc.mem_read(self.owner+0x178,4))
def main():
 p=argparse.ArgumentParser()
 for key in ('engine','library','gold','report'):p.add_argument('--'+key,type=Path,required=True)
 a=p.parse_args();manifest=json.loads((ROOT/'port/level-world/reference/character-path-commands/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256'];old=Machine(a.engine,False,manifest);new=Machine(a.library,True,{'functions':[]});rng=random.Random(0x3939f0);path_records=[];look_records=[];requests=0
 def path(raw):
  nonlocal requests
  expected=old.path_to(raw);actual=new.path_to(raw);assert expected==actual,(len(path_records),raw.hex(),expected,actual)
  _,out,calls=expected;path_records.append(raw+out+words(len(calls))+b''.join(calls));requests+=len(calls)
 for disabled,length,limit,x in itertools.product((0,1,255),(0,1,2,8),(0,1,30,0x7fffffff,0xffffffff),(0.,199.99998474121094,200.,200.00001525878906,201.,-200.,-201.)):
  path(words(disabled,length,limit,rng.choice((0,1,255,0xffffffff)))+struct.pack('<6f',0.,0.,0.,x,0.,0.))
 for _ in range(512):
  path(words(rng.randrange(256),rng.choice((0,1,3)),rng.getrandbits(32),rng.getrandbits(32))+words(*(rng.getrandbits(32) for _ in range(6))))
 # Ensure exceptional distance and source Z-only displacement actually execute
 # the reuse branch (random raw nonzero disabled bytes predominantly skip it).
 for xyz in ((0.,0.,201.),(0.,0.,200.),(math.inf,0.,0.),(math.nan,0.,0.),(3.4e38,3.4e38,3.4e38)):
  path(words(0,2,0,1)+struct.pack('<6f',0.,0.,0.,*xyz))
 def look(raw):
  expected=old.look_at(raw);actual=new.look_at(raw);assert equal(expected,actual),(len(look_records),raw.hex(),expected.hex(),actual.hex());look_records.append(raw+expected)
 for alias,x,y,z in itertools.product((0,1),(-201.,-1.,-0.,0.,1.,201.),(-201.,-1.,-0.,0.,1.,201.),(-13.,0.,17.)):
  look(struct.pack('<7fI',7.,-11.,13.,.37,x+7.,y-11.,z+13.,alias))
 for _ in range(768):
  look(struct.pack('<7fI',*(rng.uniform(-10000,10000) for _ in range(7)),rng.randrange(2)))
 for x,y,z in ((math.nan,0.,0.),(0.,math.nan,0.),(math.inf,0.,0.),(0.,math.inf,0.),(3.4e38,3.4e38,math.nan)):
  look(struct.pack('<7fI',-3.4e38,-3.4e38,0.,.37,x,y,z,0))
 failures=0;base=words(0,0,0,1)+struct.pack('<6f',0.,0.,0.,1.,2.,3.)
 for missing,fail in ((True,0),(False,1),(False,-1)):
  status,out,calls=new.path_to(base,missing=missing,fail=fail);assert status==2 and out==bytes.fromhex('adbeadde')*4 and len(calls)==int(not missing);failures+=1
 blob=b'PCD1'+words(len(path_records))+b''.join(path_records)+words(len(look_records))+b''.join(look_records);a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(blob)
 names=('character_path_commands.hpp','character_path_commands.cpp','navigation_heading.hpp','navigation_heading.cpp','tests/character_path_commands_differential.py','tests/character_path_commands.cpp')
 report={'validation':'PASS','original_sha256':sha(a.engine),'library_sha256':sha(a.library),'corpus_sha256':sha(a.gold),'path_comparisons':len(path_records),'look_comparisons':len(look_records),'find_path_requests':requests,'unavailable_failure_checks':failures,'mismatches':0,'source_sha256':{n:sha(ROOT/'port/level-world'/n) for n in names if (ROOT/'port/level-world'/n).exists()},'scope':__doc__,'original_math_imports':old.c.import_calls,'native_math_imports':new.c.import_calls,'valid_original_ring_projection':'native path_nonempty describes a valid owned ring; corrupt ring traversal is outside contract'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
if __name__=='__main__':main()
