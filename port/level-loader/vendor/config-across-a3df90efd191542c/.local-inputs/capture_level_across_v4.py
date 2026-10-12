from pathlib import Path
import sys,json
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
dest=Path('port/level-world/reference/level-config-across-rooms-v4');dest.mkdir(parents=True,exist_ok=True)
with Path('.local-inputs/libDungeonHunter2.so').open('rb') as f:
 e=ELFFile(f);segments=list(e.iter_segments());symbols={s['st_value']:s for s in e.get_section_by_name('.symtab').iter_symbols() if s['st_size'] and s['st_info']['type']=='STT_FUNC'}
 def raw(a,n):
  p=next(p for p in segments if p['p_vaddr']<=a and a+n<=p['p_vaddr']+p['p_filesz']);f.seek(p['p_offset']+a-p['p_vaddr']);return f.read(n)
 addresses={0x310454,0x310570,0x310648,0x340ca8,0x3f51a8,0x3f4910,0x3f4b9c,0x34aca0,0x34b270,0x33f15c,0x33f310}
 addresses.update(a for a,s in symbols.items() if 'LevelConfig' in s.name and any(x in s.name for x in ('Init','Enable','Properties')))
 rows=[];meta=[]
 for a in sorted(addresses):
  s=symbols.get(a);n=s['st_size'] if s else 100;meta.append(dict(address=hex(a),symbol=s.name if s else 'PLT',size=n))
  rows.append('# '+hex(a)+' '+(s.name if s else 'PLT')+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}' for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw(a,n),a)))
 (dest/'original.asm').write_text('\n\n'.join(rows));(dest/'symbols.json').write_text(json.dumps(meta,indent=2))
 print(json.dumps(meta,indent=2))
