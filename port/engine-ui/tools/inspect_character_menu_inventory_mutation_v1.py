from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
import struct
p=Path(__file__).resolve().parents[3]/'.local-inputs/libDungeonHunter2.so'
with p.open('rb') as f:
 e=ELFFile(f);syms={s['st_value']:s for s in e.get_section_by_name('.symtab').iter_symbols() if s['st_info']['type']=='STT_FUNC'}
 for a in (0x3bb9d8,0x3bba20,0x3fc6a8,0x3fc6c8,0x3fdaf0,0x3ff858,0x3ff200):
  s=syms[a];print(s.name)
  for seg in e.iter_segments():
   if seg['p_type']=='PT_LOAD' and seg['p_vaddr']<=a<a+s['st_size']<=seg['p_vaddr']+seg['p_filesz']:
    f.seek(seg['p_offset']+a-seg['p_vaddr']);raw=f.read(s['st_size']);break
  for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw,a):print(hex(i.address),i.mnemonic,i.op_str)
 def read(a,n):
  for seg in e.iter_segments():
   if seg['p_type']=='PT_LOAD' and seg['p_vaddr']<=a<a+n<=seg['p_vaddr']+seg['p_filesz']:f.seek(seg['p_offset']+a-seg['p_vaddr']);return f.read(n)
 pointer=(0x3a4b5c+8+struct.unpack('<I',read(0x3a4ba8,4))[0])&0xffffffff
 print('transmute achievement',hex(pointer),read(pointer,80).split(b'\0',1)[0])
