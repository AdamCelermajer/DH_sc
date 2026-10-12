"""Original GetCurrentSpellInfo choreography vs optimized native ARM64.

SG_GetCurrentFaerieId, GetCharFaery validation, SG_GetFaerieLevel and
ReturnValues.pushInteger are declared synchronous services. No full original
savegame/table helper execution or real Lua VM is inferred from this oracle.
"""
import hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu,words,signed
OLD=ROOT/'.local-inputs/libDungeonHunter2.so'
LIB=ROOT/'.local-inputs/character-current-spell-v1/libcurrent_spell_v1_oracle.so'
REF=ROOT/'port/level-world/reference/character-current-spell-v1'
REPORT=ROOT/'port/level-world/reports/character-current-spell-v1-arm64-differential.json'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Original:
 def __init__(self):
  self.c=Cpu(OLD,False,{'functions':[]});self.owner=self.c.data+0x1000;self.out=self.owner+0x2000
  self.c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,v):self.c.put(0,v);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def hook(self,uc,address,size,user):
  c=self.c
  if address==0x3bb98c:
   assert c.reg(0)==self.owner and signed(c.reg(1))==-1
   self.trace.extend([1,0,0xffffffff,1]);self.ret(self.first if len(self.trace)==4 else self.second)
  elif address==0x3aeac0:
   assert c.reg(0)==self.owner;self.trace.extend([2,c.reg(1),0xffffffff,1]);self.ret(self.owner+0x4000)
  elif address==0x3bbc18:
   assert c.reg(0)==self.owner and signed(c.reg(2))==-1
   self.trace.extend([3,c.reg(1),0xffffffff,1]);self.ret(self.level)
  elif address==0x37cb24:
   assert c.reg(0)==self.out;self.answer=signed(c.reg(1));self.ret(0)
 def run(self,first,second,level):
  self.first,self.second,self.level=first,second,level;self.trace=[];self.answer=None
  self.c.invoke(0x3b6e30,[self.owner+0x100,self.out,self.owner]);return self.answer,self.trace
class Native:
 def __init__(self):
  self.c=Cpu(LIB,True,{'functions':[]});self.input=self.c.data+0x1000;self.out=self.input+0x1000
 def run(self,first,second,level):
  c=self.c;c.uc.mem_write(self.input,words(first,second,level,0,*([0]*16)));c.uc.mem_write(self.out,words(0x12345678))
  r=c.invoke('dh2_current_spell_oracle_v1',[self.out,self.input]);assert r==1
  answer=signed(struct.unpack('<I',c.uc.mem_read(self.out,4))[0]);calls=struct.unpack('<I',c.uc.mem_read(self.input+12,4))[0]
  assert calls==4;trace=list(struct.unpack('<16I',c.uc.mem_read(self.input+16,64)));return answer,trace
def main():
 assert not REPORT.exists(),'Preserve accepted receipts';assert sha(OLD)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
 REF.mkdir(exist_ok=True)
 with OLD.open('rb') as f:
  elf=ELFFile(f);symbols={s['st_value']:s for s in elf.get_section_by_name('.dynsym').iter_symbols() if s['st_value']}
  captured=[]
  for address in (0x3b6e30,0x3bb98c,0x3aeac0,0x3ae5a0,0x3bbc18,0x466700):
   s=symbols[address];size=s['st_size'];seg=next(v for v in elf.iter_segments() if v['p_vaddr']<=address<v['p_vaddr']+v['p_filesz'])
   raw=seg.data()[address-seg['p_vaddr']:address-seg['p_vaddr']+size]
   captured.append(dict(address=address,name=s.name,bytes=raw.hex(),sha256=hashlib.sha256(raw).hexdigest()))
 (REF/'source-capture.json').write_text(json.dumps(dict(original_sha256=sha(OLD),functions=captured),indent=2)+'\n')
 o,n=Original(),Native();rng=random.Random(202610041);rows=[];binary=words(0x31505343,512)
 for i in range(512):
  values=[rng.randrange(5),rng.randrange(5),rng.choice([-1,0,1,65535,rng.randrange(65536)])]
  expected=o.run(*values);actual=n.run(*values);assert actual==expected,(i,values,expected,actual)
  rows.append(dict(input=values,level=expected[0],trace=expected[1]));binary+=words(*values,expected[0],*expected[1])
 (REF/'source-gold.bin').write_bytes(binary)
 (REF/'source-gold.json').write_text(json.dumps(rows,indent=2)+'\n')
 paths=[ROOT/p for p in ('port/level-world/character_current_spell_v1.hpp','port/level-world/character_current_spell_v1.cpp','port/level-world/tests/character_current_spell_v1_oracle.cpp','port/level-world/tests/character_current_spell_v1_differential.py')]
 result=dict(validation='PASS',cases=512,ordered_source_services=2048,mismatches=0,
  original_instructions_executed=True,optimized_arm64_instructions_executed=True,
  original_sha256=sha(OLD),library_sha256=sha(LIB),gold_sha256=sha(REF/'source-gold.bin'),
  source_capture_sha256=sha(REF/'source-capture.json'),source_sha256={p.relative_to(ROOT).as_posix():sha(p) for p in paths},scope=__doc__)
 REPORT.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:result[k] for k in ('validation','cases','ordered_source_services','mismatches')}))
if __name__=='__main__':main()
