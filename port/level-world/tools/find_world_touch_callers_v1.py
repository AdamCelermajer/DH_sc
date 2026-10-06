from pathlib import Path
import sys,json,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
root=Path(__file__).resolve().parents[3];rows=[];targets={0x4051a8,0x525884,0x50ed8c,0x6c3ca4,0x3addc8}
with (root/'.local-inputs/libDungeonHunter2.so').open('rb') as f:
 elf=ELFFile(f);segments=[p for p in elf.iter_segments() if p['p_type']=='PT_LOAD']
 image={p['p_vaddr']:p.data() for p in segments}
 for s in elf.get_section_by_name('.symtab').iter_symbols():
  a=s['st_value'];n=s['st_size']
  if s['st_info']['type']!='STT_FUNC'or not n or a&3:continue
  p=next((p for p in segments if p['p_vaddr']<=a and a+n<=p['p_vaddr']+p['p_filesz']),None)
  if p is None:continue
  b=image[p['p_vaddr']][a-p['p_vaddr']:a-p['p_vaddr']+n]
  for i in range(0,len(b)-3,4):
   w=struct.unpack_from('<I',b,i)[0]
   if w&0x0f000000!=0x0b000000:continue
   off=(w&0xffffff)*4;off=off-0x4000000 if off&0x2000000 else off
   destination=(a+i+8+off)&0xffffffff
   if destination in targets:rows.append({'caller':s.name,'address':hex(a),'size':n,'site':hex(a+i),'target':hex(destination)})
(root/'port/level-world/reference/world-touch-target-v1/callers.json').write_text(json.dumps(rows,indent=2)+'\n')
print(json.dumps(rows,indent=2))
