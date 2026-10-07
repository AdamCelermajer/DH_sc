"""Record original native active-skill combat/effect bodies before authoring."""
import json,hashlib,struct,re
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
ROOT=Path(__file__).resolve().parents[3]
old=ROOT/'.local-inputs/libDungeonHunter2.so'
ref=ROOT/'port/level-world/reference/character-skill-combat-v6'
ref.mkdir(parents=True,exist_ok=True)
with old.open('rb') as f:
 elf=ELFFile(f);segments=list(elf.iter_segments());md=Cs(CS_ARCH_ARM,CS_MODE_ARM)
 records=[];asm=[];calls=[];strings=[];byaddr={s['st_value']:s.name for s in elf.get_section_by_name('.dynsym').iter_symbols() if s['st_size']}
 def read(address,size):
  seg=next(x for x in segments if x['p_vaddr']<=address<x['p_vaddr']+x['p_filesz']);f.seek(seg['p_offset']+address-seg['p_vaddr']);return f.read(size)
 discovery=[]
 for s in elf.get_section_by_name('.dynsym').iter_symbols():
  if s['st_size'] and any(x in s.name for x in ['Skill','Attack','Hurt','Push','Aggro']):discovery.append({'symbol':s.name,'address':hex(s['st_value']),'size':s['st_size']})
  if not s['st_size'] or not (s['st_value']in(0x319228,0x31d194,0x3cde2c,0x469764,0x3cc448,0x3cc418,0x3d8cfc,0x3bbe2c,0x467324,0x3ce044,0x3d8894)or(s['st_info']['type']=='STT_FUNC'and any(x in s.name for x in ['SkillCombatRoll','SkillAttack','AttackEffect','F_CalculateResult','F_ApplyResult','SetCombatants','DotAttack','PushProfilingContext','PopProfilingContext','CancelSneaking','IsSneaking','PlayerStatManager','HitFor','GetTrophyIndex','UnlockTrophy','GetEffectiveThreatPerDamage','RegenHP','RegenMP','AI_AddAggro','AI_SetAggro','AI_GetAggro','OnAggro','OnDeAggro']))):continue
  a=s['st_value'];n=s['st_size'];seg=next(x for x in segments if x['p_vaddr']<=a<x['p_vaddr']+x['p_filesz'])
  f.seek(seg['p_offset']+a-seg['p_vaddr']);raw=f.read(n)
  records.append(dict(symbol=s.name,address=hex(a),size=n,sha256=hashlib.sha256(raw).hexdigest()))
  lines=[];ins=list(md.disasm(raw,a))
  for k,i in enumerate(ins):
   annotation=''
   if i.mnemonic=='bl' and i.op_str.startswith('#0x'):
    annotation=' ; '+byaddr.get(int(i.op_str[1:],16),'')
    calls.append({'caller':s.name,'instruction':hex(i.address),'target':i.op_str[1:],'symbol':annotation[3:]})
   lines.append(f'{i.address:08x}: {i.mnemonic:8} {i.op_str}'+annotation)
   match=re.fullmatch(r'(r\d+), \[pc, #(0x[0-9a-f]+)\]',i.op_str) if i.mnemonic=='ldr' else None
   if match:
    register=match[1];literal=i.address+8+int(match[2],16)
    for following in ins[k+1:k+5]:
     if following.mnemonic=='add' and following.op_str==f'{register}, pc, {register}':
      address=(following.address+8+struct.unpack('<I',read(literal,4))[0])&0xffffffff
      try:
       payload=read(address,160).split(b'\0')[0].decode('ascii')
       if payload and all(32<=ord(c)<127 for c in payload):strings.append({'caller':s.name,'instruction':hex(i.address),'address':hex(address),'text':payload})
      except (StopIteration,UnicodeDecodeError):pass
      break
  asm.append('\n# '+s.name+'\n'+'\n'.join(lines))
 (ref/'original-functions.json').write_text(json.dumps({'original_sha256':hashlib.sha256(old.read_bytes()).hexdigest(),'functions':records},indent=2)+'\n')
 (ref/'original-functions.asm').write_text('\n'.join(asm)+'\n')
 (ref/'symbol-discovery.json').write_text(json.dumps(discovery,indent=2)+'\n')
 (ref/'calls.json').write_text(json.dumps(calls,indent=2)+'\n')
 (ref/'literal-strings.json').write_text(json.dumps(strings,indent=2)+'\n')
 print(json.dumps(records,indent=2))
