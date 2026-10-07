from pathlib import Path
import sys,json
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3];md=Cs(CS_ARCH_ARM,CS_MODE_ARM);rows=[];asm=[]
with (root/'.local-inputs/libDungeonHunter2.so').open('rb') as f:
 e=ELFFile(f)
 for s in e.get_section_by_name('.symtab').iter_symbols():
  if s['st_info']['type']!='STT_FUNC' or not s['st_size']:continue
  if ('Texture' in s.name and any(k in s.name for k in ('create','Create','C1E','C2E','generateMip','setData','updateData','Online','Offline'))) or ('Format' in s.name and ('Properties' in s.name or 'flags' in s.name)) or 'initExtensions' in s.name:
   a=s['st_value'];rows.append(dict(address=hex(a),size=s['st_size'],symbol=s.name))
   if ('Driver' in s.name and ('create' in s.name or 'Create' in s.name or 'initExtensions' in s.name)) or ('CTexture' in s.name and any(k in s.name for k in ('C1E','C2E','generateMip','Online','Offline'))):
    p=next(p for p in e.iter_segments() if p['p_vaddr']<=a<p['p_vaddr']+p['p_filesz']);f.seek(p['p_offset']+a-p['p_vaddr'])
    asm.append(s.name+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}' for i in md.disasm(f.read(s['st_size']),a)))
ref=root/'port/engine-textures/reference/texture-owner-v1';(ref/'upload-leaf-symbols.json').write_text(json.dumps(rows,indent=2)+'\n');(ref/'upload-leaves.asm').write_text('\n\n'.join(asm)+'\n');print(json.dumps(rows,indent=2))
