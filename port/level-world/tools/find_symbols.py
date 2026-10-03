import argparse,json,re
from pathlib import Path
from elftools.elf.elffile import ELFFile
def main():
 p=argparse.ArgumentParser();p.add_argument('engine',type=Path);p.add_argument('pattern');p.add_argument('--output',type=Path,required=True);a=p.parse_args();rows=[]
 with a.engine.open('rb') as f:
  elf=ELFFile(f)
  for s in elf.get_section_by_name('.symtab').iter_symbols():
   if s['st_info']['type']=='STT_FUNC' and re.search(a.pattern,s.name,re.I):rows.append({'symbol':s.name,'address':hex(s['st_value']),'size':s['st_size']})
 a.output.write_text(json.dumps(rows,indent=2)+'\n');print('\n'.join(f'{r["address"]} {r["size"]} {r["symbol"]}' for r in rows[:120]))
if __name__=='__main__':main()
