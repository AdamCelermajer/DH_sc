from pathlib import Path
from elftools.elf.elffile import ELFFile
import struct
root=Path(__file__).resolve().parents[3]
with (root/'.local-inputs/libDungeonHunter2.so').open('rb') as stream:
 elf=ELFFile(stream)
 symbols=list(elf.get_section_by_name('.symtab').iter_symbols())
 def read(address,n=4):
  for segment in elf.iter_segments():
   if segment['p_vaddr']<=address and address+n<=segment['p_vaddr']+segment['p_filesz']:
    stream.seek(segment['p_offset']+address-segment['p_vaddr']);return stream.read(n)
  raise ValueError(hex(address))
 base=(0x3fa6e8+8+struct.unpack('<I',read(0x3fa708))[0])&0xffffffff
 slot=base+struct.unpack('<I',read(0x3fa70c))[0]
 address=struct.unpack('<I',read(slot))[0]
 print('color GOT',hex(slot),hex(address),[s.name for s in symbols if s['st_value']==address])
 for section in elf.iter_sections():
  if section['sh_type']=='SHT_REL':
   for r in section.iter_relocations():
    if r['r_offset']==slot:
     sym=elf.get_section(section['sh_link']).get_symbol(r['r_info_sym']);print('color relocation',sym.name,hex(sym['st_value']))
 for s in symbols:
  if s['st_value'] in (0x76b820,0x752ba8,0x30ed6c,0x30e4cc,0x36effc,0x3fe448,0x3fe164,0x3fa17c):print(hex(s['st_value']),s.name)
 for s in symbols:
  if s.name=='_ZTV9Character':
   for segment in elf.iter_segments():
    if segment['p_vaddr']<=s['st_value']<segment['p_vaddr']+segment['p_filesz']:
     stream.seek(segment['p_offset']+s['st_value']-segment['p_vaddr']+8+0x138)
     for offset in (0x138,0x13c,0x140,0x144):
      a=struct.unpack('<I',stream.read(4))[0]
      print('Character virtual',hex(offset),hex(a),[x.name for x in symbols if x['st_value']==a and x['st_size']])
 for s in elf.get_section_by_name('.symtab').iter_symbols():
  if any(x in s.name for x in ('AutoEquip','SortByEquip','CanEquipAtSlot','GetItemColor','CanBeEquippedAtSlot')):
   print(hex(s['st_value']),s.name,s['st_size'])
