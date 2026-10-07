"""Complete base GameObject query bodies and IEEE interaction radius."""
import json,struct,random,math
from character_target_providers_differential import Source,Native,ELF,REF,words
from navigation_differential import equal
from unicorn.arm64_const import UC_ARM64_REG_S0
def run(library):
 old=Source(ELF,json.loads((REF/'generic/original-functions.json').read_text()));new=Native(library,True,{'functions':[]}) if library else None
 owner=old.data+0x80000;out=new.data+0x90000 if new else 0;aabb=out+0x100;records=[];base=[]
 rng=random.Random(20261004);boundary=[0,0x80000000,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc12345,0x7f812345,1,0x7f7fffff]
 for i in range(512):
  raw=words(*(rng.choice(boundary) if i<256 else rng.getrandbits(32) for _ in range(6)));old.uc.mem_write(owner+0x144,raw);expected=words(old.invoke(0x38ad7c,[owner]))
  if new:
   new.uc.mem_write(aabb,raw);assert new.invoke('dh2_gameobject_interaction_radius',[out,aabb])==0;actual=bytes(new.uc.mem_read(out,4));assert equal(actual,expected),(i,raw.hex(),expected.hex(),actual.hex())
  records.append([*struct.unpack('<6I',raw),struct.unpack('<I',expected)[0]])
 for b in range(256):
  old.uc.mem_write(owner+0x2ed,bytes([b]))
  for op,address in enumerate((0x3883b0,0x38ad74,0x33dcd0,0x3883b8),1):
   expected=old.invoke(address,[owner])
   if new:assert new.invoke('dh2_gameobject_target_query',[out,op,b])==0;assert struct.unpack('<I',new.uc.mem_read(out,4))[0]==expected
   base.append([op,b,expected])
 return records,base
