from pathlib import Path
from elftools.elf.elffile import ELFFile
import json,struct,hashlib
root=Path(__file__).resolve().parents[4];engine=root/'.local-inputs/libDungeonHunter2.so'
with engine.open('rb') as f:
 e=ELFFile(f);symbols=list(e.get_section_by_name('.symtab').iter_symbols())
 def raw(addr,n):
  p=next(v for v in e.iter_segments() if v['p_type']=='PT_LOAD' and v['p_vaddr']<=addr<v['p_vaddr']+v['p_memsz'])
  if addr-p['p_vaddr']>=p['p_filesz']:return bytes(n)
  f.seek(p['p_offset']+addr-p['p_vaddr']);return f.read(n)
 def word(addr):return struct.unpack('<I',raw(addr,4))[0]
 got=(0x3cfc10+8+word(0x3cfd48))&0xffffffff;slot=(got+word(0x3cfd4c))&0xffffffff;target=word(slot)
 def info(addr):return [{'symbol':s.name,'address':hex(s['st_value']),'size':s['st_size']} for s in symbols if s['st_value']<=addr<s['st_value']+s['st_size']]
 out={'original_sha256':hashlib.sha256(engine.read_bytes()).hexdigest(),'got':hex(got),'got_slot':hex(slot),'global_address':hex(target),'global_symbols':info(target),'first_file_byte':raw(target,1).hex(),'literal_words':{'0x3cfd48':hex(word(0x3cfd48)),'0x3cfd4c':hex(word(0x3cfd4c))}}
 (root/'port/level-world/reference/character-ai-frame/debug-producer.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))

