"""Capture actual World/Character target ownership and faction source routines."""
import hashlib,json
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3];old=root/'.local-inputs/libDungeonHunter2.so';ref=root/'port/level-world/reference/character-world-target-owner-v1';ref.mkdir(exist_ok=True)
with old.open('rb') as f:
 elf=ELFFile(f);segments=list(elf.iter_segments());symbols=list(elf.get_section_by_name('.dynsym').iter_symbols());names={s['st_value']:s.name for s in symbols if s['st_size']};md=Cs(CS_ARCH_ARM,CS_MODE_ARM);records=[];asm=[]
 for s in symbols:
  if not s['st_size'] or not (s['st_value']in(0x33dd70,0x33ff8c,0x33fdc0,0x3a3180)or any(x in s.name for x in ['AI_IsFriend','AI_IsEnemy','GetTargetPosition','Cmd_LookAt','Ctrl_LookAt','GetTargetNode','_ZN10ObjectBaseC','_ZN10GameObjectC','_ZN9CharacterC','_ZN9Character4Init','_ZN10GameObject4Init','_ZN6CharAIC','RoomZone','RegisterObject','UnregisterObject','AddObject','RemoveObject','_ZN12ObjectHandleC','ObjectManager','GameObject10SetVisible','GameObject9SetZoned'])):continue
  a=s['st_value'];n=s['st_size'];segment=next(p for p in segments if p['p_vaddr']<=a<p['p_vaddr']+p['p_filesz']);f.seek(segment['p_offset']+a-segment['p_vaddr']);raw=f.read(n)
  records.append({'symbol':s.name,'address':hex(a),'size':n,'sha256':hashlib.sha256(raw).hexdigest()});lines=[]
  for i in md.disasm(raw,a):
   annotation=' ; '+names.get(int(i.op_str[1:],16),'') if i.mnemonic=='bl' and i.op_str.startswith('#0x') else ''
   lines.append(f'{i.address:08x}: {i.mnemonic:8} {i.op_str}'+annotation)
  asm.append('\n# '+s.name+'\n'+'\n'.join(lines))
 (ref/'original-functions.asm').write_text('\n'.join(asm)+'\n');(ref/'original-functions.json').write_text(json.dumps({'original_sha256':hashlib.sha256(old.read_bytes()).hexdigest(),'functions':records},indent=2)+'\n');print(json.dumps(records))
