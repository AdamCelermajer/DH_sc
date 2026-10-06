from pathlib import Path
import sys,json,hashlib
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so';ref=root/'port/level-world/reference/player-manager-owner-v1';ref.mkdir(parents=True,exist_ok=True)
md=Cs(CS_ARCH_ARM,CS_MODE_ARM);rows=[];text=[]
with original.open('rb') as f:
 elf=ELFFile(f);segments=list(elf.iter_segments());symbols=list(elf.get_section_by_name('.symtab').iter_symbols())
 selected={s['st_value']:s for s in symbols if s['st_info']['type']=='STT_FUNC'and s['st_size']and ('13PlayerManager' in s.name or '10PlayerInfoC' in s.name or s.name.startswith('_ZN6PlayerC')or s.name.startswith('_ZN6Player4Init')or s.name.startswith('_ZN9Character7InitPre')or s['st_value']in(0x37418c,0x378808,0x373bdc,0x3b3d38,0x3b3c34,0x3bb7fc,0x3bb814,0x3bc4d0))}
 for address,s in sorted(selected.items()):
  size=s['st_size'];segment=next(p for p in segments if p['p_vaddr']<=address and address+size<=p['p_vaddr']+p['p_filesz']);f.seek(segment['p_offset']+address-segment['p_vaddr']);code=f.read(size)
  rows.append(dict(address=hex(address),symbol=s.name,size=size,sha256=hashlib.sha256(code).hexdigest()));text.append('# '+hex(address)+' '+s.name+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}'for i in md.disasm(code,address)))
(ref/'original-source.asm').write_text('\n\n'.join(text)+'\n');(ref/'original-functions.json').write_text(json.dumps(dict(original_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),functions=rows),indent=2)+'\n')
for r in rows:print(r['address'],r['size'],r['symbol'])
for s in symbols:
 if 'Character' in s.name and 'InitPre' in s.name:print('INITPRE',hex(s['st_value']),s['st_size'],s.name)
