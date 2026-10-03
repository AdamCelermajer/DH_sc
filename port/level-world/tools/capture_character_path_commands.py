"""Capture owned original GameObject path/look/stop bodies by actual ELF symbol."""
import argparse,hashlib,json
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
ROOT=Path(__file__).resolve().parents[3]
EXPECTED='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,default=ROOT/'.local-inputs/libDungeonHunter2.so');a=p.parse_args();assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==EXPECTED
 out=ROOT/'port/level-world/reference/character-path-commands';out.mkdir(parents=True,exist_ok=True);wanted={0x3939f0,0x393cec,0x3938f8,0x393b1c};rows=[];asm=[];md=Cs(CS_ARCH_ARM,CS_MODE_ARM)
 with a.engine.open('rb') as f:
  elf=ELFFile(f);loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD'];symbols={}
  for sec in elf.iter_sections():
   if sec['sh_type'] in ('SHT_SYMTAB','SHT_DYNSYM'):
    for symbol in sec.iter_symbols():
     if symbol['st_value'] in wanted and symbol['st_info']['type']=='STT_FUNC':symbols[symbol['st_value']]=symbol
  assert set(symbols)==wanted
  for addr,sym in sorted(symbols.items()):
   size=sym['st_size'];segment=next(s for s in loads if s['p_vaddr']<=addr and addr+size<=s['p_vaddr']+s['p_filesz']);raw=segment.data()[addr-segment['p_vaddr']:addr-segment['p_vaddr']+size]
   rows.append({'original_symbol':sym.name,'elf_address':hex(addr),'size':size,'sha256':hashlib.sha256(raw).hexdigest()});asm.append('\n# '+sym.name+'\n'+ '\n'.join(f'{i.address:08x}: {i.mnemonic:8} {i.op_str}' for i in md.disasm(raw,addr)))
 (out/'original-functions.json').write_text(json.dumps({'original_sha256':EXPECTED,'functions':rows},indent=2)+'\n');(out/'original-functions.asm').write_text('\n'.join(asm)+'\n');print(json.dumps(rows))
if __name__=='__main__':main()
