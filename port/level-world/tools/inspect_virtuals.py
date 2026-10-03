"""Resolve ARM32 Itanium vtable entries from the hash-identified original ELF."""
import argparse,hashlib,json,struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
EXPECTED='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
def main():
 p=argparse.ArgumentParser();p.add_argument('engine',type=Path);p.add_argument('symbol');p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==EXPECTED
 with a.engine.open('rb') as stream:
  elf=ELFFile(stream);assert elf.elfclass==32 and elf.little_endian
  symbols=list(elf.get_section_by_name('.symtab').iter_symbols());matches=[s for s in symbols if s.name==a.symbol and s['st_shndx']!='SHN_UNDEF'];assert len(matches)==1
  table=matches[0];start,size=table['st_value'],table['st_size'];assert size>=8 and size%4==0
  segment=next(s for s in elf.iter_segments() if s['p_type']=='PT_LOAD' and s['p_vaddr']<=start and start+size<=s['p_vaddr']+s['p_filesz']);stream.seek(segment['p_offset']+start-segment['p_vaddr']);raw=stream.read(size);words=list(struct.unpack('<'+'I'*(size//4),raw));relocated=[]
  for section in elf.iter_sections():
   if section['sh_type']!='SHT_REL':continue
   linked=elf.get_section(section['sh_link'])
   for r in section.iter_relocations():
    at=r['r_offset']
    if not start<=at<start+size:continue
    assert (at-start)%4==0;index=(at-start)//4;kind=r['r_info_type']
    if kind==2:
     sym=linked.get_symbol(r['r_info_sym']);assert sym['st_shndx']!='SHN_UNDEF';words[index]=(words[index]+sym['st_value'])&0xffffffff;relocated.append(hex(at))
    else:assert kind==23,('Unhandled relocation',kind,hex(at))
  names={}
  for s in symbols:
   if s['st_info']['type']=='STT_FUNC' and s['st_shndx']!='SHN_UNDEF' and s['st_value']:names.setdefault(s['st_value'],set()).add(s.name)
  # A class vtable symbol may also contain secondary base-subobject tables.
  # Stop before their repeated RTTI/offset-to-top headers.
  end=next((i for i in range(2,len(words)-1) if words[i+1]==words[1] and (words[i]==0 or words[i]&0x80000000)),len(words))
  rows=[{'offset':hex(i*4),'pointer':hex(word),'symbols':sorted(names.get(word,set()))} for i,word in enumerate(words[2:end])]
  result={'original_sha256':EXPECTED,'vtable_symbol':a.symbol,'address':hex(start),'address_point':hex(start+8),'bytes':size,'raw_sha256':hashlib.sha256(raw).hexdigest(),'absolute_relocations_applied':relocated,'primary_slots':len(rows),'secondary_bytes':size-end*4,'entries':rows}
  a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k not in ('entries','absolute_relocations_applied')}))
if __name__=='__main__':main()
