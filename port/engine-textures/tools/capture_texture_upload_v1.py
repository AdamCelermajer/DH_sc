from pathlib import Path
import sys
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3]
md=Cs(CS_ARCH_ARM,CS_MODE_ARM)
out=[]
with (root/'.local-inputs/libDungeonHunter2.so').open('rb') as f:
 e=ELFFile(f)
 for a,n in [(0x5afff0,1116),(0x5fdae8,288),(0x5aa5f8,688),(0x6dd6dc,88),(0x5f95ac,400),(0x5edbec,384),(0x5edaec,256),(0x6dcd90,128)]:
  p=next(p for p in e.iter_segments() if p['p_vaddr']<=a<p['p_vaddr']+p['p_filesz'])
  f.seek(p['p_offset']+a-p['p_vaddr'])
  out.append('\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}' for i in md.disasm(f.read(n),a)))
 (root/'port/engine-textures/reference/texture-owner-v1/upload-source.asm').write_text('\n\n'.join(out)+'\n')
