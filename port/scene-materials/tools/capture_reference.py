"""Capture original routines used as layout evidence for checked scene views."""
import argparse,hashlib,json
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
ROOT=Path(__file__).resolve().parents[1]
EXPECTED='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
def main():
 p=argparse.ArgumentParser();p.add_argument('engine',type=Path);a=p.parse_args()
 assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==EXPECTED
 addresses={0x60e1d4,0x631ce8,0x6323d0,0x634520,0x61b2f4,0x61b9e8,0x61c290,0x61aeb8,0x60e54c,0x598908,0x597c60}
 rows=[];assembly=[]
 with a.engine.open('rb') as f:
  elf=ELFFile(f)
  for s in elf.get_section_by_name('.symtab').iter_symbols():
   if s['st_info']['type']!='STT_FUNC' or s['st_value'] not in addresses:continue
   addr=s['st_value'];segment=next(p for p in elf.iter_segments() if p['p_type']=='PT_LOAD' and p['p_vaddr']<=addr< p['p_vaddr']+p['p_filesz'])
   f.seek(segment['p_offset']+addr-segment['p_vaddr']);raw=f.read(s['st_size'])
   rows.append({'original_symbol':s.name,'elf_address':hex(addr),'size':s['st_size'],'sha256':hashlib.sha256(raw).hexdigest(),'use':'field/layout evidence; full routine not ported'})
   assembly.append('\n# '+s.name+'\n')
   assembly.extend(f'{i.address:08x}: {i.mnemonic:8} {i.op_str}\n' for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw,addr))
 (ROOT/'reference').mkdir(exist_ok=True)
 (ROOT/'reference/original-functions.asm').write_text(''.join(assembly),encoding='utf-8')
 (ROOT/'original-functions.json').write_text(json.dumps({'original_sha256':EXPECTED,'functions':rows},indent=2)+'\n')
 print([(r['elf_address'],r['size']) for r in rows])
if __name__=='__main__':main()
