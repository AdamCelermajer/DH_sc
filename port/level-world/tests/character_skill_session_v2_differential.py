"""Player InitVCB and numeric CurrentSkill wrapper: original instructions versus O2 ARM64.

Original GetCharSkillListId/GetCharSkill and SG_GetSkillLevel execute on declared
borrowed source tables. Alias-map membership and ReturnValues.pushInteger remain
logical services; separate real-VM audit supplies owned aliases and saved rows.
Unsafe original index/assertion continuation is excluded, then tested as native
required failure by the sanitized host audit. No whole original Lua VM claim.
"""
import argparse, hashlib, json, math, random, struct, sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_S0
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu, words, bits, floating, signed
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class ProofCpu(Cpu):
 def external(self,uc,address,size,user):
  name=self.imports.get(address)
  if self.arm64 and name=='dh2_script_alias_contains':self.alias();return
  return super().external(uc,address,size,user)
class VCB:
 def __init__(self,p,native):
  self.c=ProofCpu(p,native,{'functions':[]});self.native=native;self.obj=self.c.data+0x1000;self.flags=self.obj if native else self.obj+0xb8;self.c.alias=self.alias
  if not native:self.c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,v):self.c.put(0,v);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def alias(self):
  c=self.c;name=c.string(c.reg(1)).decode();self.trace.append([name,struct.unpack('<I',c.uc.mem_read(self.flags,4))[0]])
  pos=['OnTargetHit','OnTargetMissed','OnKill'].index(name)
  if self.mutation&(1<<pos):c.uc.mem_write(self.flags,words(0xaabbcc00+pos))
  self.ret((self.mask>>pos)&1)
 def hook(self,uc,address,size,user):
  if address==0x37c2a0:self.alias()
 def run(self,initial,mask,mutation):
  self.mask=mask;self.mutation=mutation;self.trace=[];c=self.c;c.uc.mem_write(self.flags,words(initial))
  r=c.invoke('dh2_character_script_player_vcb_v2' if self.native else 0x3dd884,[self.flags,self.obj+0x300] if self.native else [self.obj])
  if self.native:assert r==0
  return struct.unpack('<I',c.uc.mem_read(self.flags,4))[0],self.trace
class Skill:
 def __init__(self,p,native):
  self.c=ProofCpu(p,native,{'functions':[]});self.native=native;c=self.c;d=c.data
  self.character=d+0x1000;self.arguments=d+0x3000;self.vector=d+0x3100;self.value=d+0x3200;self.lists=d+0x4000;self.ids=d+0x5000;self.skills=d+0x6000;self.save=d+0x9000;self.rows=d+0xa000;self.view=d+0xb000;self.savedview=d+0xb100;self.out=d+0xb200
  if not native:
   c.pointer(0x99f698,0);c.uc.hook_add(UC_HOOK_CODE,self.hook)
   # Actual three GOT projections used by recovered helpers (derive literals).
   def got(pc,literal,offset_literal):return pc+8+self.read(literal)+self.read(offset_literal)
   self.list_data_slot=got(0x3bc608,0x3bc624,0x3bc628)
   self.list_count_slot=got(0x3bc5d0,0x3bc5f4,0x3bc5f8)
   self.skills_slot=got(0x3bc7a0,0x3bc840,0x3bc84c)
   for slot,addr in [(self.list_data_slot,d+0xc000),(self.list_count_slot,d+0xc100),(self.skills_slot,d+0xc200)]:c.pointer(slot,addr)
   c.pointer(d+0xc000,self.lists);c.pointer(d+0xc100,8);c.pointer(d+0xc200,self.skills)
 def read(self,p):return struct.unpack('<I',self.c.uc.mem_read(p,4))[0]
 def hook(self,uc,address,size,user):
  if address==0x37cb24:self.answer=signed(self.c.reg(1));self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def run(self,selected,count,index,has_saved,levels,present):
  c=self.c;self.answer=None;c.uc.mem_write(self.out,words(0x12345678))
  for i in range(8):
   at=self.ids+i*64;c.uc.mem_write(at,words(*range(count)))
   c.uc.mem_write(self.lists+i*(16 if self.native else 12),struct.pack('<QII',at,count,0) if self.native else words(0,count,at))
  c.uc.mem_write(self.rows,b''.join(struct.pack('<iHBB',i,lv,0,0)for i,lv in enumerate(levels)))
  if self.native:
   c.uc.mem_write(self.savedview,struct.pack('<QII',self.rows,len(levels),0));c.uc.mem_write(self.view,struct.pack('<QIIiIQ',self.lists,8,20,selected,0,self.savedview if has_saved else 0));c.uc.reg_write(UC_ARM64_REG_S0,bits(index))
   r=c.invoke('dh2_character_current_skill_level_v2',[self.out,self.view,present]);assert signed(r)==present,(r,index)
   return signed(self.read(self.out)) if present else None
  c.pointer(self.character+0x1068,selected&0xffffffff);c.pointer(self.character+0x14e8,self.save if has_saved else 0);c.pointer(self.save+0x80,self.rows);c.pointer(self.save+0x84,len(levels));c.pointer(self.arguments+4,self.vector);c.uc.mem_write(self.vector,words(self.value,self.value+112*present,self.value+112));c.uc.mem_write(self.value+4,words(3,bits(index)));c.invoke(0x3b8f9c,[self.arguments,self.out,self.character]);return self.answer
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();old=REPO/'.local-inputs/libDungeonHunter2.so';rng=random.Random(20261004)
 ov,nv=VCB(old,False),VCB(a.library,True);vcb=[]
 for i in range(256):
  values=[rng.getrandbits(32),i%8,(i//8)%8];expected=ov.run(*values);actual=nv.run(*values);assert actual==expected,(values,expected,actual);vcb.append(dict(input=values,flags=expected[0],trace=expected[1]))
 os,ns=Skill(old,False),Skill(a.library,True);skills=[]
 for i in range(768):
  count=rng.randrange(1,16);index=rng.randrange(count)+rng.choice([0.,.125,.999]);index=math.nan if i%41==0 else index;levels=[rng.randrange(65536)for _ in range(count)];values=[rng.choice([-2147483648,-1,0,1,3,7,8,2147483647]),count,index,i%3!=0,levels,i%17!=0];e=os.run(*values);r=ns.run(*values);assert e==r,(i,values,e,r);skills.append(dict(selected=values[0],count=count,index_bits=bits(index),has_saved=values[3],levels=levels,present=values[5],answer=e))
 dst=ROOT/'reference/character-skill-session-v2';dst.mkdir(parents=True,exist_ok=True);gold=dst/'source-gold.json';gold.write_text(json.dumps(dict(vcb=vcb,current_skill=skills),indent=2)+'\n')
 binary=words(0x324b5350,len(vcb),len(skills))
 for row in vcb:binary+=words(*row['input'],row['flags'])
 for row in skills:binary+=words(row['selected'],row['count'],row['index_bits'],row['has_saved'],row['present'],row['answer'] if row['answer'] is not None else 0x12345678,*row['levels'])
 (dst/'source-gold.bin').write_bytes(binary)
 sources=[ROOT/'character_script_player_vcb_v2.cpp',ROOT/'character_script_player_vcb_v2.hpp',ROOT/'character_current_skill_v2.cpp',ROOT/'character_current_skill_v2.hpp',Path(__file__)]
 report=dict(validation='PASS',player_vcb_cases=len(vcb),alias_calls=len(vcb)*3,current_skill_cases=len(skills),mismatches=0,original_instructions_executed=True,optimized_arm64_instructions_executed=True,original_sha256=sha(old),library_sha256=sha(a.library),gold_sha256=sha(gold),source_sha256={x.relative_to(REPO).as_posix():sha(x)for x in sources},scope=__doc__)
 target=ROOT/'reports/character-skill-session-v2-arm64-differential.json';target.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
