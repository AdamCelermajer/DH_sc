"""Static original initialization/default-bank ordering evidence only."""
import hashlib,json
from pathlib import Path
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
from elftools.elf.elffile import ELFFile
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3];image=ROOT/'.local-inputs/libDungeonHunter2.so'
functions=[];assembly=[]
with image.open('rb') as f:
 e=ELFFile(f);symbols=list(e.get_section_by_name('.symtab').iter_symbols())
 for address in (0x3b4d60,0x3c9f4c,0x3c9c7c,0x3c9d80,0x476398,0x62fc90,0x470a54,0x3a59ac,0x472a0c):
  s=next(s for s in symbols if s['st_value']==address and s['st_size'] and s['st_info']['type']=='STT_FUNC')
  p=next(p for p in e.iter_segments() if p['p_vaddr']<=address<p['p_vaddr']+p['p_filesz']);f.seek(p['p_offset']+address-p['p_vaddr']);raw=f.read(s['st_size'])
  functions.append(dict(address=hex(address),symbol=s.name,size=len(raw),sha256=hashlib.sha256(raw).hexdigest()));assembly.append('\n# '+s.name);assembly.extend(f'{i.address:08x}: {i.mnemonic:8} {i.op_str}' for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw,address))
(HERE/'initialization-order.asm').write_text('\n'.join(assembly)+'\n');(HERE/'initialization-order.json').write_text(json.dumps(dict(original_sha256=hashlib.sha256(image.read_bytes()).hexdigest(),functions=functions,scope=__doc__),indent=2)+'\n')
