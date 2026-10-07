from pathlib import Path
import sys,json
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3];out=[];symbols_out=[];md=Cs(CS_ARCH_ARM,CS_MODE_ARM)
with (root/'.local-inputs/libDungeonHunter2.so').open('rb') as f:
 e=ELFFile(f)
 plt=e.get_section_by_name('.plt');rel=e.get_section_by_name('.rel.plt');dyn=e.get_section(rel['sh_link'])
 imports={hex(plt['sh_addr']+20+12*i):dyn.get_symbol(r['r_info_sym']).name for i,r in enumerate(rel.iter_relocations())}
 print({a:imports.get(a) for a in ['0x30e70c','0x30e514','0x30eba4','0x30e964','0x30e910','0x30e25c']})
 for s in e.get_section_by_name('.symtab').iter_symbols():
  if s['st_info']['type']!='STT_FUNC' or not s['st_size']:continue
  if any(k in s.name for k in ('DriverFlag','AdditionalShader','ShaderConfig','ShaderHandlerC','Driver4init','Driver10initialize')) or ('Driver' in s.name and ('init' in s.name or 'Init' in s.name)):print(hex(s['st_value']),s['st_size'],s.name)
  if s['st_value'] not in (0x5ec460,0x5afd40,0x5fe404,0x5b5c40,0x6de5f8,0x5aaf1c,0x5a90b0,0x6e0a3c,0x5b3afc,0x5ae5f0,0x5aab60,0x5fdf74,0x5fde9c,0x5b044c,0x5aa5f8,0x5afff0,0x5fdae8) and not any(k in s.name for k in ('TextureManagerC','6CImageC','CodeShaderManagerC','updateParameters','createTextureFromImage')) and not ('COpenGLES2Driver' in s.name and ('C1E' in s.name or 'C2E' in s.name)):continue
  a=s['st_value'];p=next(p for p in e.iter_segments() if p['p_vaddr']<=a<p['p_vaddr']+p['p_filesz']);f.seek(p['p_offset']+a-p['p_vaddr'])
  symbols_out.append(dict(address=hex(a),size=s['st_size'],symbol=s.name));out.append(s.name+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}' for i in md.disasm(f.read(s['st_size']),a)))
ref=root/'port/engine-textures/reference/texture-owner-v1';ref.mkdir(parents=True,exist_ok=True)
(ref/'original-source.asm').write_text('\n\n'.join(out)+'\n');(ref/'symbols.json').write_text(json.dumps(symbols_out,indent=2)+'\n');print(json.dumps(symbols_out,indent=2))
