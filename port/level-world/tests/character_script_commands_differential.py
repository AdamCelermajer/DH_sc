"""Original command wrapper instructions versus optimized ARM64 source.

Controller commands and heading queries are synchronous services. Numeric,
boolean, nil and identity Value getters execute original instructions; string
getNumber is an explicit temporary-Lua conversion fixture. Path nodes are
caller storage, with actual original traversal. No live controller/path claim.
"""
import hashlib,itertools,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/game-data/tests'))
from items_differential import Original
from navigation_differential import Cpu,equal
from aggro_differential import float_bits
from combat_result_differential import floating
REF=ROOT/'reference/character-script-commands'
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class NativeCpu(Cpu):
 def external(self,uc,address,size,unused):
  if address in (self.callback+32,self.callback+48):return
  return super().external(uc,address,size,unused)
class OriginalCpu(Original):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='__aeabi_ui2f':
   self.put(0,float_bits(float(self.reg(0))));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,unused)
class Probe:
 def __init__(self,path,native):
  self.native=native;self.c=NativeCpu(path,True,{'functions':[]}) if native else OriginalCpu(path,json.loads((REF/'original-functions.json').read_text()))
  c=self.c;d=c.data;self.state=d+0x1000;self.args=d+0x3000;self.vector=d+0x4000;self.values=d+0x5000
  self.owner=d+0x10000;self.controller=d+0x20000;self.target=d+0x30000;self.other=d+0x40000
  self.services=d+0x6000;self.returned=d+0x7000;self.out=d+0x8000;self.heading=d+0x9000;self.axis=d+0xa000
  self.nodes=d+0xb000;self.tags={0:0,self.owner:1,self.controller:2,self.target:3,self.other:4};self.active=False
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,value=0):c=self.c;c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def record(self,service,subject,target=0,point=bytes(12)):
  self.calls.append(words(service,self.tags[subject],self.tags[target])+point)
 def conversion(self,type,bits,boolean,identity):
  if type in (1,3):return float_bits(float(boolean)) if type==1 else bits
  if type in (2,7):return float_bits(float(self.source_ids.get(identity,identity)))
  if type==4:return bits # explicit string getNumber fixture
  return 0
 def hook(self,uc,address,size,unused):
  if not self.active:return
  c=self.c
  if self.native and address==c.callback+32:
   service,reserved,subject,target,x,y,z,reserved1=struct.unpack('<IIQQ4I',uc.mem_read(c.reg(2),40));assert not reserved and not reserved1
   self.record(service,subject,target,words(x,y,z))
   if service==5:c.pointer(c.reg(3),subject+0x160)
   elif service==6:c.pointer(c.reg(3),self.heading)
   self.ret()
  elif self.native and address==c.callback+48:
   at=c.reg(1);index=(at-self.values)//40;assert at==self.values+index*40
   type,bits,boolean,identity=self.descriptors[index];self.conversions.append(index)
   uc.mem_write(c.reg(2),words(self.conversion(type,bits,boolean,identity)));self.ret()
  elif not self.native and address in (0x40559c,0x405540,0x40542c,0x4054e4,0x405b04):
   service={0x40559c:0,0x405540:1,0x40542c:2,0x4054e4:3,0x405b04:4}[address]
   self.record(service,c.reg(0),c.reg(1) if service in (1,4) else 0,bytes(uc.mem_read(c.reg(1),12)) if service in (2,3) else bytes(12));self.ret()
  elif not self.native and address==0x3935dc:self.record(5,c.reg(0)) # real uncached position getter executes
  elif not self.native and address==0x393ae4:
   self.record(6,c.reg(0));uc.mem_write(c.reg(1),bytes(uc.mem_read(self.heading,12)));self.ret()
  elif not self.native and address==0x31bbf0:
   index=(c.reg(0)-self.values)//112;self.conversions.append(index)
   if self.descriptors[index][0]==4:self.ret(self.descriptors[index][1])
  elif not self.native and address==0x37c7e4:self.return_values=[c.reg(1)];self.ret()
 def execute(self,op,desc,target_present,path_count,points):
  c=self.c;self.descriptors=desc;self.calls=[];self.conversions=[];self.return_values=[]
  ownerpos,targetpos,heading,axis=points;c.uc.mem_write(self.owner,bytes(0x1800));c.uc.mem_write(self.target,bytes(0x1800))
  c.uc.mem_write(self.owner+0x160,ownerpos);c.uc.mem_write(self.target+0x160,targetpos);c.uc.mem_write(self.heading,heading);c.uc.mem_write(self.axis,axis)
  ids={0:0,3:self.target,4:self.other};values=[]
  for type,bits,boolean,identity in desc:
   if self.native:values.append(struct.pack('<4I3Q',type,0,bits,boolean,0,0,ids.get(identity,identity)))
   else:
    raw=bytearray(112);struct.pack_into('<II',raw,4,type,float_bits(float(boolean)) if type==1 else bits);struct.pack_into('<I',raw,108,ids.get(identity,identity));values.append(raw)
  if values:c.uc.mem_write(self.values,b''.join(values))
  if self.native:
   c.uc.mem_write(self.state,struct.pack('<5Q2I',self.owner,self.controller,self.target if target_present else 0,self.owner+0x160,self.axis,path_count,0));c.uc.mem_write(self.services,struct.pack('<3Q',0,c.callback+32,c.callback+48));c.uc.mem_write(self.out,bytes(40));c.uc.mem_write(self.returned,words(0xdead))
   self.active=True;result=c.invoke('dh2_character_script_command',[self.state,op,self.values if desc else 0,len(desc),self.services,self.out,1,self.returned]);self.active=False
   assert result==1,(op,desc,result)
   count=struct.unpack('<I',c.uc.mem_read(self.returned,4))[0]
   if count:self.return_values=[struct.unpack('<I',c.uc.mem_read(self.out+12,4))[0]]
  else:
   c.pointer(self.args+4,self.vector);c.pointer(self.vector,self.values);c.pointer(self.vector+4,self.values+112*len(desc));c.pointer(self.owner+0x378,self.controller);c.pointer(self.owner+0x408,self.target if target_present else 0)
   c.pointer(self.owner+0x200,self.nodes if path_count else self.owner+0x200)
   for i in range(path_count):c.pointer(self.nodes+8*i,self.nodes+8*(i+1) if i+1<path_count else self.owner+0x200)
   c.uc.mem_write(0x99f878,axis)
   self.active=True;c.invoke((0x3b56b4,0x3ba71c,0x3bada8,0x3b9f44,0x3b91a0,0x38e98c)[op],[self.args,self.out,self.owner]);self.active=False
  return self.calls,self.conversions,self.return_values
def main():
 library=REPO/'.local-inputs/character-script-commands/oracle.so';engine=REPO/'.local-inputs/libDungeonHunter2.so'
 manifest=json.loads((REF/'original-functions.json').read_text());assert sha(engine)==manifest['original_sha256']
 old,new=Probe(engine,False),Probe(library,True);new.source_ids={3:old.target,4:old.other};rows=[];requests=0
 rng=random.Random(0x3ba71c);templates=[(0,0,0,0),(1,0,0,0),(1,0,1,0),(2,0,0,3),(7,0,0,4),(2,0,0,0),(3,float_bits(1.25),0,0),(3,float_bits(-3.5),0,0),(4,float_bits(2.75),0,0),(5,0,0,0)]
 def compare(op,args,target,path,points):
  nonlocal requests
  a=old.execute(op,args,target,path,points);b=new.execute(op,args,target,path,points)
  assert a[1:]==b[1:],(op,args,a,b)
  assert len(a[0])==len(b[0]) and all(x[:12]==y[:12] and equal(x[12:],y[12:]) for x,y in zip(a[0],b[0])),(op,args,a,b)
  rows.append(words(op,len(args),target,path)+b''.join(words(*v) for v in args)+b''.join(points)+words(len(a[0]))+b''.join(a[0])+words(len(a[1]),*a[1])+words(len(a[2]),*a[2]));requests+=len(a[0])
 fixed=[struct.pack('<3f',*p) for p in ((10,-20,3),(4,5,-7),(0.8,-0.6,0),(0,0,1))]
 for op in range(6):
  for target,path in itertools.product((0,1),(0,1,3)):
   if op not in (1,2):compare(op,[],target,path,fixed)
   for value in templates:compare(op,[value],target,path,fixed);compare(op,[value,templates[6]],target,path,fixed)
 for op in (1,2):
  for a,b,c,relative in itertools.product(templates[:9],templates[:9],templates[:9],(False,True)):
   args=[a,b,c]+([templates[2]] if relative else []);compare(op,args,1,0,fixed)
  for bits in (0,0x80000000,1,0x7f800000,0xff800000,0x7fc01234):
   for relative in (False,True):compare(op,[(3,bits,0,0),(3,float_bits(1),0,0),(3,float_bits(-1),0,0)]+([templates[2]] if relative else []),1,0,fixed)
  for _ in range(256):
   points=[struct.pack('<3f',*(rng.uniform(-10000,10000) for _ in range(3))) for _ in range(4)]
   args=[(3,float_bits(rng.uniform(-1000,1000)),0,0) for _ in range(3)]+[templates[2]];compare(op,args,1,0,points)
 gold=REF/'command-fixtures.bin';gold.write_bytes(b'SCM1'+words(len(rows))+b''.join(rows))
 report=dict(validation='PASS',cases=len(rows),ordered_requests=requests,original_sha256=sha(engine),arm64_sha256=sha(library),gold_sha256=sha(gold),source_sha256={str(p.relative_to(REPO)):sha(p) for p in (ROOT/'character_script_commands.hpp',ROOT/'character_script_commands.cpp',Path(__file__))},scope=__doc__,mismatches=0)
 (ROOT/'reports/character-script-commands-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
