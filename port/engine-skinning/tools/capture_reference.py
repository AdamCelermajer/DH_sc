"""Capture exact original skin data consumers and matrix-cache evidence."""
import argparse,hashlib,json
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
ROOT=Path(__file__).resolve().parents[1]
EXPECTED='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
def main():
 p=argparse.ArgumentParser();p.add_argument('engine',type=Path);a=p.parse_args()
 assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==EXPECTED
 addresses={0x6664f0,0x60fa24,0x61ace8,0x664af8,0x66fe34,0x66fab4,0x66ff48,0x664ee0,0x6651a0,0x66f8ec,0x670ec8};rows=[];assembly=[]
 with a.engine.open('rb') as f:
  elf=ELFFile(f)
  for s in elf.get_section_by_name('.symtab').iter_symbols():
   if s['st_info']['type']!='STT_FUNC' or s['st_value'] not in addresses:continue
   addr=s['st_value'];segment=next(p for p in elf.iter_segments() if p['p_type']=='PT_LOAD' and p['p_vaddr']<=addr<p['p_vaddr']+p['p_filesz'])
   f.seek(segment['p_offset']+addr-segment['p_vaddr']);raw=f.read(s['st_size'])
   rows.append({'original_symbol':s.name,'elf_address':hex(addr),'size':s['st_size'],'sha256':hashlib.sha256(raw).hexdigest(),
    'use':'serialized field and algorithm evidence; full runtime class ABI not ported'})
   assembly.append('\n# '+s.name+'\n');assembly.extend(f'{i.address:08x}: {i.mnemonic:8} {i.op_str}\n' for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw,addr))
 (ROOT/'reference').mkdir(exist_ok=True)
 (ROOT/'reference/original-functions.asm').write_text(''.join(assembly),encoding='utf-8')
 (ROOT/'original-functions.json').write_text(json.dumps({'original_sha256':EXPECTED,'functions':rows},indent=2)+'\n')
 print(f'Captured {len(rows)} original routines')
if __name__=='__main__':main()
