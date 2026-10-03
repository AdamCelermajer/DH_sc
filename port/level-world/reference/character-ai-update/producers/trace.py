from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
import json,hashlib
root=Path(__file__).resolve().parents[5];engine=root/'.local-inputs/libDungeonHunter2.so';records=[];vt=[]
with engine.open('rb') as f:
 e=ELFFile(f);symbols=list(e.get_section_by_name('.symtab').iter_symbols());by={s['st_value']:s.name for s in symbols if s['st_info']['type']=='STT_FUNC'}
 def raw(addr,n):
  seg=next(v for v in e.iter_segments() if v['p_type']=='PT_LOAD' and v['p_vaddr']<=addr<v['p_vaddr']+v['p_filesz']);f.seek(seg['p_offset']+addr-seg['p_vaddr']);return f.read(n)
 import struct
 for s in symbols:
  if s.name.startswith('_ZTV') and any(k in s.name for k in ('Prince','Character','GameObject')):
   data=raw(s['st_value'],s['st_size']);vt.append({'symbol':s.name,'address':hex(s['st_value']),'sha256':hashlib.sha256(data).hexdigest(),'slots':{hex(i):{'address':hex(struct.unpack('<I',data[8+i:12+i])[0]),'symbol':by.get(struct.unpack('<I',data[8+i:12+i])[0])} for i in (0x28,0x34,0xc4) if 12+i<=len(data)}})
  if s['st_info']['type']!='STT_FUNC' or not any(k in s.name for k in ('Char','Spawn','GameObject')):continue
  data=raw(s['st_value'],s['st_size']);ins=list(Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(data,s['st_value']));hits=[i for i in ins if any(v in i.op_str for v in ('#0x1450','#0x1454','#0x1458','#0x1440'))]
  if hits:records.append({'symbol':s.name,'address':hex(s['st_value']),'size':s['st_size'],'sha256':hashlib.sha256(data).hexdigest(),'hits':[{'address':hex(i.address),'instruction':i.mnemonic+' '+i.op_str} for i in hits]})
out={'original_sha256':hashlib.sha256(engine.read_bytes()).hexdigest(),'vtables':vt,'saved_position_xrefs':records};p=root/'port/level-world/reference/character-ai-update/producers/xrefs.json';p.write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
