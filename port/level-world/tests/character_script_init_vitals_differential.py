"""Actual _InitHpMp/RegenHP/MP and property bodies vs optimized native source.
Shared debug/string bodies are named synchronous services, never silent omissions.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-resources/tests'))
from cpu import Cpu,i32
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def pack(v):return struct.pack('<224i',*v)
class VitalCpu(Cpu):
 def external(self,uc,address,size,unused):
  if address==self.callback:return
  return super().external(uc,address,size,unused)
class Machine:
 def __init__(self,path,native,manifest):
  c=self.c=VitalCpu(path,native,manifest);self.native=native;d=c.data
  self.character=d+0x1000;self.owner=self.character+0x560;self.view=d+0x8000;self.binding=d+0x8100;self.out=d+0x8200;self.sheets=[d+0x10000+i*0x1000 for i in range(6)];self.phase=36
  if not native:
   self.sheets[1]=self.sheets[0]+900
   c.pointer(0x9a645c,self.sheets[0]);self.props=[self.owner+x for x in (8,0x38c,0x710,0xa94)]
   sentinel=self.owner+0xe18;c.uc.mem_write(sentinel,struct.pack('<4I',0,0,sentinel,sentinel));c.pointer(self.owner+0xe28,0)
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def snapshot(self):
  return b''.join(bytes(self.c.uc.mem_read(p,896)) for p in self.sheets[2:]) if self.native else b''.join(bytes(self.c.uc.mem_read(p+4,896)) for p in self.props)
 def value(self,index):return i32(struct.unpack('<I',self.c.uc.mem_read((self.sheets[5] if self.native else self.props[3]+4)+index*4,4))[0])
 def event(self,op,amount=0):
  self.calls.append(struct.pack('<3I4i',op,self.phase,amount&0xffffffff,*(self.value(i) for i in (36,41,38,43))))
  if len(self.calls)==self.params[0]:
   index,value=self.params[1:3];at=(self.sheets[5] if self.native else self.props[3]+4)+index*4;self.c.uc.mem_write(at,struct.pack('<I',value&0xffffffff))
 def returned(self,value=0):self.c.put(0,value);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def hook(self,uc,address,size,unused):
  c=self.c
  if self.native:
   if address==c.symbols['dh2_property_add']:self.phase=c.reg(1);self.event(4,c.reg(2));return
   if address!=c.callback:return
   op=c.reg(1)
   if op==1 and any(struct.unpack('<2I',r[:8])==(4,36) for r in self.calls):self.phase=41
   if op==3:assert bytes(uc.mem_read(c.reg(2),32)).split(b'\0')[0]==b'isTracingChar_Stats'
   self.event(op);uc.mem_write(c.reg(3),struct.pack('<I',0));self.returned();return
  if address in (0x3bdca4,0x3bdbb8):
   self.phase=36 if address==0x3bdca4 else 41;self.before[self.phase]=self.value(self.phase);return
  if address==0x3b3a80:self.after_hp=self.value(36);return
  if address==0x337888:self.event(1);self.returned();return
  if address==0x337a88:self.event(3);self.returned();return
  if address==0x3140ec:
   name=bytes(uc.mem_read(c.reg(1),32)).split(b'\0')[0];assert name==b'isTracingChar_Stats';self.returned();return
  if address==0x318254:self.returned();return
  if address==0x3e0708:self.event(4,c.reg(2));return
 def execute(self,sheets,params):
  self.calls=[];self.params=params;self.before={};self.after_hp=None
  c=self.c
  if self.native:
   for p,s in zip(self.sheets,sheets):c.uc.mem_write(p,pack(s))
   hp,mx=self.value(36),self.value(38);delta=mx
   if i32((hp+delta)&0xffffffff)>mx:delta=i32((mx-hp)&0xffffffff)
   self.phase=36 if delta>0 else 41
   c.uc.mem_write(self.view,struct.pack('<7QII',*self.sheets,0,0,0));c.uc.mem_write(self.binding,struct.pack('<4Q',0xabcdef0123456789,self.view,0,c.callback));assert c.invoke('dh2_character_script_init_vitals',[self.out,self.binding])==1
   result=bytes(c.uc.mem_read(self.out,24))
  else:
   for p,s in zip(self.sheets[:2],sheets[:2]):c.uc.mem_write(p,bytes(4)+pack(s))
   for p,s in zip(self.props,sheets[2:]):c.uc.mem_write(p,bytes(4)+pack(s))
   c.invoke(0x3b3a70,[self.character]);adds={36:0,41:0}
   for row in self.calls:
    op,phase,delta=struct.unpack('<3I',row[:12])
    if op==4:adds[phase]=i32(delta)
   result=struct.pack('<6i',adds[36],self.before[36],self.after_hp,adds[41],self.before[41],self.value(41))
  return self.snapshot(),result,tuple(self.calls)
def main():
 p=argparse.ArgumentParser()
 for key in ('engine','library','gold','report'):p.add_argument('--'+key,type=Path,required=True)
 a=p.parse_args();capture=ROOT/'port/level-world/reference/character-script-init-services/original-functions.json';manifest=json.loads(capture.read_text());assert sha(a.engine)==manifest['original_sha256']
 old=Machine(a.engine,False,manifest);new=Machine(a.library,True,{'functions':[]});raw=(ROOT/'port/android-native/app/src/main/assets/data/character_properties_pyarray.bin').read_bytes();defaults=list(struct.unpack_from('<224i',raw,4));types=list(struct.unpack_from('<224i',raw,900));rng=random.Random(0x3b3a70);records=[];calls=0;mutations=0
 for case in range(900):
  sheets=[list(defaults),list(types),*[list(defaults) for _ in range(4)]]
  for i in (36,38,41,43):sheets[5][i]=rng.choice((-2147483648,-1,0,1,256,10000,2147483647)) if case<150 else rng.randrange(-2147483648,2147483648)
  params=(0,0,0) if case<700 else (rng.randrange(1,7),rng.choice((36,38,41,43)),rng.randrange(-2147483648,2147483648))
  expected=old.execute(sheets,params);actual=new.execute(sheets,params);assert expected==actual,(case,params,expected[1:],actual[1:])
  after,result,events=expected;calls+=len(events);mutations+=int(params[0]>0 and len(events)>=params[0]);records.append(b''.join(pack(s) for s in sheets)+struct.pack('<3I',*(v&0xffffffff for v in params))+after+result+struct.pack('<I',len(events))+b''.join(events))
 blob=b'VSI1'+struct.pack('<I',len(records))+b''.join(records);a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(blob)
 source=['port/level-world/character_script_init_vitals.hpp','port/level-world/character_script_init_vitals.cpp','port/level-world/tests/character_script_init_vitals_differential.py','port/game-data/properties.cpp','port/game-data/class_tables.cpp']
 report={'validation':'PASS','original_sha256':sha(a.engine),'library_sha256':sha(a.library),'gold_sha256':sha(a.gold),'comparisons':len(records),'ordered_debug_property_services':calls,'live_debug_mutation_cases':mutations,'mismatches':0,'source_sha256':{s:sha(ROOT/s) for s in source},'original_capture_sha256':sha(capture),'scope':'Actual _InitHpMp→RegenHP→RegenMP and complete source property reads/AddProperty; named synchronous debug load/query/string services. Captured positive delta survives Debug mutations, later MP reads are live. No actual skill creation, whole InitScriptProcess, package or live proof.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
