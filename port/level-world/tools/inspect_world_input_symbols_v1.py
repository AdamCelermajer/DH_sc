from pathlib import Path
import sys,json
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
r=Path(__file__).resolve().parents[3]
rows=[]
with (r/'.local-inputs/libDungeonHunter2.so').open('rb') as f:
 for s in ELFFile(f).get_section_by_name('.symtab').iter_symbols():
  if s['st_info']['type']=='STT_FUNC'and s['st_size']and any(k in s.name for k in ('Input','Click','Screen','PlayerControl','Selected','Intersect'))and 'St4'not in s.name and 'St6'not in s.name:
   rows.append({'address':hex(s['st_value']),'size':s['st_size'],'symbol':s.name})
(r/'port/level-world/reference/world-touch-target-v1/input-symbols.json').write_text(json.dumps(rows,indent=2)+'\n')
for row in rows:print(row['address'],row['size'],row['symbol'])
