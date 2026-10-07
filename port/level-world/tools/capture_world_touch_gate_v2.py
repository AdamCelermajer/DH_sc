from pathlib import Path
import sys
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3];md=Cs(CS_ARCH_ARM,CS_MODE_ARM);out=[]
with (root/'.local-inputs/libDungeonHunter2.so').open('rb') as f:
 e=ELFFile(f)
 for s in e.get_section_by_name('.symtab').iter_symbols():
  if s['st_value'] not in (0x3c034c,0x3c0378):continue
  a=s['st_value'];p=next(p for p in e.iter_segments() if p['p_vaddr']<=a<p['p_vaddr']+p['p_filesz']);f.seek(p['p_offset']+a-p['p_vaddr'])
  out.append(s.name+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}' for i in md.disasm(f.read(s['st_size']),a)))
text='\n\n'.join(out);print(text);(root/'port/level-world/reference/world-touch-target-v1/controller-state-gates.asm').write_text(text+'\n')
