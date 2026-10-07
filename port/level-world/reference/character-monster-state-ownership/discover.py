import bisect,hashlib,json,struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[4]
HERE=Path(__file__).resolve().parent
ELF=ROOT/'.local-inputs/libDungeonHunter2.so'
assert hashlib.sha256(ELF.read_bytes()).hexdigest()=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
with ELF.open('rb') as f:
 e=ELFFile(f)
 symbols=sorted((s['st_value'],s['st_size'],s.name) for s in e.get_section_by_name('.symtab').iter_symbols() if s['st_info']['type']=='STT_FUNC')
 addresses=[s[0] for s in symbols]
 targets={0x3c7318,0x3c1938,0x3c1600,0x3a5784,0x3c7b18}
 rows=[]
 for section in e.iter_sections():
  if not section['sh_flags']&4:continue
  raw=section.data()
  for i,(w,) in enumerate(struct.iter_unpack('<I',raw[:len(raw)//4*4])):
   if w&0x0f000000!=0x0b000000:continue
   call=section['sh_addr']+i*4
   target=call+8+(((w&0xffffff)^0x800000)-0x800000)*4
   if target not in targets:continue
   caller=symbols[max(0,bisect.bisect_right(addresses,call)-1)]
   rows.append(dict(call=hex(call),target=hex(target),caller=caller[2],caller_address=hex(caller[0])))
 (HERE/'callers.json').write_text(json.dumps(rows,indent=2)+'\n')
 print(json.dumps([r for r in rows if int(r['target'],16)!=0x3c7b18],indent=2))
