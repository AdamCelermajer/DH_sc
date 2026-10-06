"""Whole original Point3D34d0b0/34d04c versus ARM64, including IEEE zero/NaN."""
import sys,json,struct,random,math,hashlib
from pathlib import Path
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from character_body_config_differential import Cpu,ROOT
from body_transform_differential import equal
from aggro_differential import float_bits
from combat_result_differential import floating
from unicorn.arm64_const import UC_ARM64_REG_S0
class NormalizeCpu(Cpu):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='sqrtf':
   value=floating(uc.reg_read(UC_ARM64_REG_S0) if self.arm64 else self.reg(0));result=float_bits(math.sqrt(value) if value>=0 else math.nan)
   if self.arm64:uc.reg_write(UC_ARM64_REG_S0,result)
   else:self.put(0,result)
   self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if name=='__aeabi_fdiv':
   a,b=floating(self.reg(0)),floating(self.reg(1))
   result=(a/b if b else (math.copysign(math.inf,a*b) if a else math.nan)) if not(math.isnan(a) or math.isnan(b)) else math.nan
   self.put(0,float_bits(result));self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,unused)
old=NormalizeCpu(Path('.local-inputs/libDungeonHunter2.so'),False,{'functions':[]})
new=NormalizeCpu(Path('.local-inputs/point3d-normalize-v2.so'),True,{'functions':[]})
op=old.data+0x1000;np=new.data+0x1000;rng=random.Random(20261006)
vectors=[(0,0,0),(0x80000000,0,0),(0x3f800000,0,0),(0x40400000,0x40800000,0),(0x7f800000,0,0),(0x7fc01234,0,0),(1,1,1),(0x7f7fffff,0x7f7fffff,0x7f7fffff)]
vectors += [tuple(float_bits(rng.uniform(-1e6,1e6)) for _ in range(3)) for i in range(1000)]
for index,words in enumerate(vectors):
 raw=struct.pack('<3I',*words);old.uc.mem_write(op,raw);new.uc.mem_write(np,raw);old.invoke(0x34d0b0,[op]);assert new.invoke('dh2_point3d_normalize_v2',[np])==np
 expected=bytes(old.uc.mem_read(op,12));actual=bytes(new.uc.mem_read(np,12));assert equal(expected,actual),(index,raw.hex(),expected.hex(),actual.hex())
report={'status':'PASS','comparisons':len(vectors),'mismatches':0,'original_sha256':hashlib.sha256(Path('.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'library_sha256':hashlib.sha256(Path('.local-inputs/point3d-normalize-v2.so').read_bytes()).hexdigest(),'scope':__doc__,'original_import_calls':old.import_calls}
dest=ROOT/'reports/point3d-normalize-v2-original.json';dest.write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
