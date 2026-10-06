from pathlib import Path
import sys,json,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM
from unicorn.arm_const import UC_ARM_REG_SP,UC_ARM_REG_LR
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so';ref=root/'port/level-world/reference/canonical-object-factory-v1'
with original.open('rb')as f:
 elf=ELFFile(f);symbols=list(elf.get_section_by_name('.symtab').iter_symbols());md=Cs(CS_ARCH_ARM,CS_MODE_ARM)
 found=[s for s in symbols if 'Vec3f_Origin'in s.name or ('GLOBAL'in s.name and any(n in s.name.lower()for n in('math','vec','point')))]
 rows=[]
 for s in found:
  sec=elf.get_section(s['st_shndx'])if isinstance(s['st_shndx'],int)else None
  rows.append(dict(symbol=s.name,address=hex(s['st_value']),size=s['st_size'],section=sec.name if sec else str(s['st_shndx'])))
 s=next(s for s in symbols if s['st_value']==0x312e00);sec=elf.get_section(s['st_shndx']);f.seek(sec['sh_offset']+s['st_value']-sec['sh_addr']);code=f.read(s['st_size'])
 text='\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}'for i in md.disasm(code,s['st_value']))
 (ref/'vec3-origin-initializer.asm').write_text(text+'\n');print(text)
 (ref/'vec3-origin-symbols.json').write_text(json.dumps(rows,indent=2)+'\n')
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,0xa00000);u.mem_map(0x1000000,0x100000)
 for segment in elf.iter_segments():
  if segment['p_type']=='PT_LOAD':u.mem_write(segment['p_vaddr'],segment.data())
 u.mem_write(0x99f854,b'\xa5'*48);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,0x1000000)
 u.emu_start(0x312e00,0x1000000,count=200)
 values={hex(a):list(struct.unpack('<3f',u.mem_read(a,12)))for a in(0x99f854,0x99f860,0x99f86c,0x99f878)}
 assert values=={'0x99f854':[0.,0.,0.],'0x99f860':[1.,0.,0.],'0x99f86c':[0.,1.,0.],'0x99f878':[0.,0.,1.]}
 (ref/'point3d-global-original.json').write_text(json.dumps(dict(entry='0x312e00',values=values,fixtures=[],actual_initializer_executed=True),indent=2)+'\n');print('actual global initializer PASS '+json.dumps(values))
