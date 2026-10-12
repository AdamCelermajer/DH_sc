from pathlib import Path
import sys,json,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
r=Path(__file__).resolve().parents[1];out=r/'port/level-world/reference/trigger-zone-v22';out.mkdir(exist_ok=True)
with (r/'.local-inputs/libDungeonHunter2.so').open('rb') as f:
 e=ELFFile(f);segs=[s for s in e.iter_segments() if s['p_type']=='PT_LOAD'];rows=[];dis=Cs(CS_ARCH_ARM,CS_MODE_ARM)
 for s in e.get_section_by_name('.symtab').iter_symbols():
  a=int(s['st_value']);n=int(s['st_size'])
  if s['st_info']['type']!='STT_FUNC':continue
  if 'TriggerZone' in s.name or ('4Zone' in s.name and not 'ZoneManager' in s.name) or '7Trigger' in s.name or '6ZoneEx' in s.name or a in [0x398da4,0x3980a8,0x38b6d4,0x38b518,0x81324c,0x8138f4,0x814f84]:
   for seg in segs:
    if seg['p_vaddr']<=a<seg['p_vaddr']+seg['p_filesz']:b=seg.data()[a-seg['p_vaddr']:a-seg['p_vaddr']+n];break
   rows.append({'name':s.name,'address':hex(a),'size':n,'sha256':hashlib.sha256(b).hexdigest(),'instructions':[f'{i.address:08x} {i.mnemonic} {i.op_str}' for i in dis.disasm(b,a)]})
 (out/'source-functions.json').write_text(json.dumps(rows,indent=2));print(json.dumps([{k:v for k,v in row.items() if k!='instructions'} for row in rows],indent=2))
