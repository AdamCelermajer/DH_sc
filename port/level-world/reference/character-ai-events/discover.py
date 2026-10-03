"""Decode exact source switch indices and CharAI callable identities."""
from pathlib import Path
import hashlib,json,struct
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
ROOT=Path(__file__).resolve().parents[4]
engine=ROOT/'.local-inputs/libDungeonHunter2.so'
with engine.open('rb') as stream:
 elf=ELFFile(stream)
 def raw(address,size):
  segment=next(p for p in elf.iter_segments() if p['p_type']=='PT_LOAD' and p['p_vaddr']<=address< p['p_vaddr']+p['p_filesz'])
  stream.seek(segment['p_offset']+address-segment['p_vaddr']);return stream.read(size)
 symbols=list(elf.get_section_by_name('.symtab').iter_symbols())
 def table(address,count,first):
  return [{'event':first+i,'entry':hex(instruction.address),'target':instruction.op_str.removeprefix('#')} for i,instruction in enumerate(Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw(address,count*4),address))]
 values=struct.unpack('<51I',raw(0x966798,204))
 virtuals=[{'slot':i*4,'address':hex(value),'symbol':next(s.name for s in symbols if s['st_value']==value and s['st_info']['type']=='STT_FUNC')} for i,value in enumerate(values)]
 result={'original_sha256':hashlib.sha256(engine.read_bytes()).hexdigest(),'first_switch':table(0x3cbb58,64,0),'gated_switch':table(0x3cbd40,59,4),'ai_virtual_address_point':'0x966798','virtuals':virtuals}
 (Path(__file__).parent/'dispatch-producer.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({'first':64,'gated':59,'virtuals':51,'original_sha256':result['original_sha256']}))
