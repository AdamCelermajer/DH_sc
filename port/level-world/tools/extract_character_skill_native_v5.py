import json,hashlib,struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
ROOT=Path(__file__).resolve().parents[3];OLD=ROOT/'.local-inputs/libDungeonHunter2.so'
REF=ROOT/'port/level-world/reference/character-skill-native-v5';REF.mkdir(parents=True,exist_ok=True)
with OLD.open('rb')as f:
 elf=ELFFile(f);symbols=list(elf.get_section_by_name('.dynsym').iter_symbols());segments=list(elf.iter_segments());md=Cs(CS_ARCH_ARM,CS_MODE_ARM)
 chosen=[s for s in symbols if s['st_size'] and (s['st_value'] in [0x4a36b0,0x390bf8,0x3b8ed8,0x4000c8,0x400080,0x40019c,0x4001a0,0x3ffe8c] or any(x in s.name for x in ['HasMana','UseMana','HasShield','TargetListBackup','SetTargetListCharacterFilter','SetTargetListObjectFilter','SetTargetListSorting','TargetListSearch','GetTargetListTop','PopTargetList','IsTargetListEmpty','GetAnimStance']))]
 records=[];asm=[]
 for s in chosen:
  address=s['st_value'];size=s['st_size'];seg=next(x for x in segments if x['p_vaddr']<=address<x['p_vaddr']+x['p_filesz']);f.seek(seg['p_offset']+address-seg['p_vaddr']);raw=f.read(size)
  records.append(dict(symbol=s.name,address=hex(address),size=size,sha256=hashlib.sha256(raw).hexdigest()))
  asm.append('\n# '+s.name+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic:8} {i.op_str}'for i in md.disasm(raw,address)))
 (REF/'original-functions.json').write_text(json.dumps(records,indent=2)+'\n');(REF/'original-functions.asm').write_text('\n'.join(asm)+'\n')
 print(f'Extracted {len(records)} source functions')
 def read(address,size):
  seg=next(x for x in segments if x['p_vaddr']<=address<x['p_vaddr']+x['p_filesz']);f.seek(seg['p_offset']+address-seg['p_vaddr']);return f.read(size)
 for literal,pc in [(0x3a54ac,0x3a5420),(0x3a54b0,0x3a5424),(0x390c4c,0x390c18)]:
  address=(pc+struct.unpack('<I',read(literal,4))[0])&0xffffffff
  print(hex(address),read(address,120).split(b'\0')[0])
