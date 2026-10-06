"""Execute original AI_IsNeutral over declared handle/faction service fixtures."""
from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
root=Path(__file__).resolve().parents[3]; original=root/'.local-inputs/libDungeonHunter2.so'; results=[]
for relation,kind,null_object,current in [(-1,0,False,False),(0,0,False,False),(1,0,False,False),(2,0,False,False),(None,0,False,False),(0,7,False,False),(0,0,True,False),(0,0,False,True)]:
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,0xc00000);u.mem_map(0x1000000,0x300000)
 with original.open('rb') as f:
  elf=ELFFile(f)
  for p in elf.iter_segments():
   if p['p_type']=='PT_LOAD':u.mem_write(p['p_vaddr'],p.data())
  for section in elf.iter_sections():
   if section['sh_type'] not in ('SHT_REL','SHT_RELA'):continue
   symbols=elf.get_section(section['sh_link'])
   for r in section.iter_relocations():
    v=symbols.get_symbol(r['r_info_sym'])['st_value'];a=r['r_offset'];k=r['r_info_type']
    if v and k in (2,21,22):u.mem_write(a,struct.pack('<I',(v+(struct.unpack('<I',u.mem_read(a,4))[0] if k==2 else 0))&0xffffffff))
 def w(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 ai=0x1100000;owner=0x1101000;target=0x1103000;table=0x1110000;entries=0x1111000;calls=[]
 w(ai+4,owner);w(ai+0x40,target);w(target+0xf4,kind)
 base=0x3d5ab4+word(0x3d5d20)
 w(word(base+word(0x3d5d24)),2)
 w(word(base+word(0x3d5d2c)),table)
 w(table+4,0 if relation is None else 1);w(table+8,entries)
 w(entries+4,1);w(entries+8,relation or 0)
 stop=0x1000000
 def ret(value=None):
  if value is not None:u.reg_write(UC_ARM_REG_R0,value&0xffffffff)
  u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(u,a,size,data):
  if a==stop:u.emu_stop();return
  r0=u.reg_read(UC_ARM_REG_R0)
  if a==0x33dd70:
   calls.append('handle');w(r0,target);ret();return
  if a==0x33ff8c:calls.append('object');ret(0 if null_object else target);return
  if a==0x3a3180:
   calls.append('ownerFaction' if r0==owner else 'targetFaction');ret(0 if r0==owner else 1);return
 u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_ARM_REG_R0,ai);u.reg_write(UC_ARM_REG_R1,0 if current else target);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop)
 u.emu_start(0x3d5a98,stop,count=10000)
 actual=u.reg_read(UC_ARM_REG_R0);expected=int(null_object or kind!=0 or relation is None or relation==0)
 assert actual==expected,(relation,kind,actual,expected)
 if not null_object and kind==0:assert calls==['handle','object','targetFaction','targetFaction','ownerFaction','ownerFaction','ownerFaction','targetFaction']
 results.append(dict(relation=relation,kind=kind,null_object=null_object,current_target=current,result=actual,calls=calls))
(root/'port/level-world/reference/world-touch-target-v1/neutral-original.json').write_text(json.dumps(dict(original_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),cases=results,actual_executed=['AI_IsNeutral3d5a98'],fixture_services=['GetHandle','GetObject(false)','GetFaction'],assertion_branches_verified=False),indent=2)+'\n')
print('PASS eight original neutral relation/default/kind/handle/current-target cases')
