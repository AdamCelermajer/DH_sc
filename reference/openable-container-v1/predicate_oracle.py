import sys,struct,json,hashlib
from pathlib import Path
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_LR
p=Path('.local-inputs/libDungeonHunter2.so');u=Uc(UC_ARCH_ARM,UC_MODE_ARM)
u.mem_map(0x390000,0x20000);u.mem_map(0x200000,0x1000);u.mem_map(0x500000,0x1000)
with p.open('rb') as f:
 e=ELFFile(f)
 for s in e.iter_segments():
  if s['p_type']=='PT_LOAD' and s['p_vaddr']<=0x390000 and 0x3b0000<=s['p_vaddr']+s['p_filesz']:
   f.seek(s['p_offset']+0x390000-s['p_vaddr']);u.mem_write(0x390000,f.read(0x20000));break
checks=0
for state in (-2147483648,-1,0,1,2,3,4,5,2147483647):
 for key in (-2147483648,-1,0,7,2147483647):
  u.mem_write(0x200394,struct.pack('<i',state));u.mem_write(0x200710,struct.pack('<i',key))
  u.reg_write(UC_ARM_REG_R0,0x200000);u.reg_write(UC_ARM_REG_LR,0x500000);u.emu_start(0x3a16a8,0x500000)
  expected=int(((state-3)&0xffffffff)>1 and key!=-1)
  assert u.reg_read(UC_ARM_REG_R0)==expected;(checks:=checks+1)
 for disabled in (0,1,127,255):
  u.mem_write(0x200081,bytes([disabled]));u.reg_write(UC_ARM_REG_R0,0x200000);u.reg_write(UC_ARM_REG_LR,0x500000);u.emu_start(0x39f384,0x500000)
  assert u.reg_read(UC_ARM_REG_R0)==int(not disabled and state==2);checks+=1
out=dict(original_elf_sha256=hashlib.sha256(p.read_bytes()).hexdigest(),source_IsLocked='0x3a16a8',source_IsInteractive='0x39f384',checks=checks,pass_=True,scope='original ARM predicates vs source-translated arithmetic; not compiled whole owner')
Path('reference/openable-container-v1/predicate-oracle.json').write_text(json.dumps(out,indent=2));print(out)
