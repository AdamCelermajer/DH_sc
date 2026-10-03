import struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
with Path('.local-inputs/libDungeonHunter2.so').open('rb') as f:
 elf=ELFFile(f)
 def read(address,size):
  segment=next(s for s in elf.iter_segments() if s['p_type']=='PT_LOAD' and s['p_vaddr']<=address<s['p_vaddr']+s['p_filesz']);f.seek(segment['p_offset']+address-segment['p_vaddr']);return f.read(size)
 def word(address):return struct.unpack('<I',read(address,4))[0]
 base=0x523dc4+8+word(0x523db8+8+0x3cc)
 print('floor prefixes table',hex(base))
 for i in range(3):
  ptr=word(base+i*4);print(hex(ptr),read(ptr,80).split(b'\0')[0])
