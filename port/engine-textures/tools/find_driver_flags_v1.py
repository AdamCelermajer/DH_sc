from pathlib import Path
import sys,json
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3];md=Cs(CS_ARCH_ARM,CS_MODE_ARM);rows=[]
with (root/'.local-inputs/libDungeonHunter2.so').open('rb') as f:
 e=ELFFile(f)
 for s in e.get_section_by_name('.symtab').iter_symbols():
  if not s['st_size'] or s['st_info']['type']!='STT_FUNC':continue
  if not any(k in s.name for k in ('Driver','Application','Device','initAdditionalConfig','ShaderHandlerC')):continue
  a=s['st_value'];p=next(p for p in e.iter_segments() if p['p_vaddr']<=a<p['p_vaddr']+p['p_filesz']);f.seek(p['p_offset']+a-p['p_vaddr']);code=list(md.disasm(f.read(s['st_size']),a))
  hits=[i for i,x in enumerate(code) if x.mnemonic.startswith('str') and '#0x88]' in x.op_str]
  if 'initAdditionalConfig' in s.name or 'ShaderHandlerC' in s.name:hits=[0]
  if hits:rows.append(dict(address=hex(a),size=s['st_size'],symbol=s.name,stores=['\n'.join(f'{x.address:08x}: {x.mnemonic} {x.op_str}' for x in code[max(0,i-4):i+2])for i in hits]))
(root/'port/engine-textures/reference/texture-owner-v1/driver-flags-writers.json').write_text(json.dumps(rows,indent=2)+'\n');print(json.dumps(rows,indent=2))
