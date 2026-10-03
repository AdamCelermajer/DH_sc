"""Capture animation factory, scale interpolation and application evidence."""
import argparse,hashlib,json
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
ROOT=Path(__file__).resolve().parents[1]
EXPECTED='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
def main():
 p=argparse.ArgumentParser();p.add_argument('engine',type=Path);a=p.parse_args()
 assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==EXPECTED
 addresses={0x611ae0,0x610988,0x62393c,0x628850,0x669e24,0x613294,0x6130d4,0x628834,0x60ff20,0x612d00,0x6286cc};rows=[];assembly=[]
 with a.engine.open('rb') as f:
  elf=ELFFile(f)
  for s in elf.get_section_by_name('.symtab').iter_symbols():
   if s['st_info']['type']!='STT_FUNC' or s['st_value'] not in addresses:continue
   addr=s['st_value'];segment=next(p for p in elf.iter_segments() if p['p_type']=='PT_LOAD' and p['p_vaddr']<=addr<p['p_vaddr']+p['p_filesz'])
   f.seek(segment['p_offset']+addr-segment['p_vaddr']);raw=f.read(s['st_size'])
   rows.append({'original_symbol':s.name,'elf_address':hex(addr),'size':s['st_size'],'sha256':hashlib.sha256(raw).hexdigest(),
    'use':'float-3 interpolation arithmetic reconstructed' if addr==0x628850 else 'track/property/layout evidence; runtime factory ABI not ported'})
   assembly.append('\n# '+s.name+'\n');assembly.extend(f'{i.address:08x}: {i.mnemonic:8} {i.op_str}\n' for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw,addr))
 (ROOT/'reference').mkdir(exist_ok=True)
 (ROOT/'reference/original-functions.asm').write_text(''.join(assembly),encoding='utf-8')
 (ROOT/'original-functions.json').write_text(json.dumps({'original_sha256':EXPECTED,'functions':rows},indent=2)+'\n')
 print(f'Captured {len(rows)} original routines')
if __name__=='__main__':main()
