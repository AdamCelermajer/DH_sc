from pathlib import Path
import sys,json,runpy,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3];d=runpy.run_path(str(root/'port/engine-textures/tools/probe_texture_parameters_v1.py'));u=d['u'];md=Cs(CS_ARCH_ARM,CS_MODE_ARM);rows=[];out=[]
with (root/'.local-inputs/libDungeonHunter2.so').open('rb') as f:
 e=ELFFile(f);syms=list(e.get_section_by_name('.symtab').iter_symbols());byaddress={s['st_value']:s for s in syms if s['st_info']['type']=='STT_FUNC' and s['st_size']}
 for s in syms:
  if s.name.startswith('_ZTV') and 'CCommonGLDriverI' in s.name and '8CTexture' in s.name:
   table=s['st_value']+8
   for slot in (0xc,0x10,0x14,0x18,0x20):
    a=struct.unpack('<I',u.mem_read(table+slot,4))[0];target=byaddress.get(a);rows.append(dict(table=hex(table),slot=hex(slot),function=hex(a),name=target.name if target else None))
    if target:
     p=next(p for p in e.iter_segments() if p['p_vaddr']<=a<p['p_vaddr']+p['p_filesz']);f.seek(p['p_offset']+a-p['p_vaddr']);out.append(target.name+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}'for i in md.disasm(f.read(target['st_size']),a)))
ref=root/'port/engine-textures/reference/texture-owner-v1';(ref/'online-source-symbols.json').write_text(json.dumps(rows,indent=2)+'\n');(ref/'online-source.asm').write_text('\n\n'.join(out)+'\n');print(json.dumps(rows,indent=2))
