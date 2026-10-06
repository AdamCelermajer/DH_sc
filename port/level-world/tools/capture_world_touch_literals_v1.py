from pathlib import Path
import sys,json,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
root=Path(__file__).resolve().parents[3];ref=root/'port/level-world/reference/world-touch-target-v1'
with (root/'.local-inputs/libDungeonHunter2.so').open('rb') as f:
 elf=ELFFile(f);segments=list(elf.iter_segments())
 def raw(a,n):
  p=next(p for p in segments if p['p_vaddr']<=a and a+n<=p['p_vaddr']+p['p_filesz']);f.seek(p['p_offset']+a-p['p_vaddr']);return f.read(n)
 def word(a):return struct.unpack('<I',raw(a,4))[0]
 def string(a):
  out=b''
  while raw(a,1)!=b'\0':out+=raw(a,1);a+=1
  return out.decode()
 literals=[]
 for name,base,literal in [('selected_debug',0x3ae06c,0x3ae45c),('no_target_option',0x3ae200,0x3ae464),('move_debug',0x3ae240,0x3ae468),('object_selected_debug',0x3ae310,0x3ae46c),('special_type',0x3ae0ec,0x3ae460),('using_dpad_option',0x320e84,0x320e90)]:
  address=(base+word(literal))&0xffffffff;literals.append({'name':name,'address':hex(address),'value':string(address)})
 plt=elf.get_section_by_name('.plt');rel=elf.get_section_by_name('.rel.plt');symbols=elf.get_section(rel['sh_link']);entries=[]
 for i,r in enumerate(rel.iter_relocations()):
  a=plt['sh_addr']+20+i*12
  if a in (0x30e31c,0x30e9ac,0x30e4b4,0x30e6e8,0x30e2f8):entries.append({'address':hex(a),'symbol':symbols.get_symbol(r['r_info_sym']).name,'got':hex(r['r_offset'])})
(ref/'original-literals.json').write_text(json.dumps({'literals':literals,'plt':entries},indent=2)+'\n');print(literals);print(entries)
