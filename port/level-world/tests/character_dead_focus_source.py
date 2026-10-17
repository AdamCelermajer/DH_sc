"""Scoped static original evidence; this does not dynamically execute callees."""
from __future__ import annotations
import hashlib,json,struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
ELF_SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
ADDRESSES=[0x3c4d50,0x3bc6b8,0x3bc690,0x3dedb4,0x3bc5c0,0x3bc5fc,0x3bc784,0x3d84e0,0x3a4068,0x3a40e4,0x3a40b0,0x3e0af8,
 0x3a9340,0x3aa1b4,0x3cebf0,0x3ced50,0x3cfbf4,0x3d1050,0x3dce64,0x3dc798,
 0x3d8894,0x3d8a04,0x37280c]
def capture(path:Path,out:Path):
 blob=path.read_bytes();assert hashlib.sha256(blob).hexdigest()==ELF_SHA
 with path.open('rb') as stream:
  elf=ELFFile(stream);symbols=[s for s in elf.get_section_by_name('.symtab').iter_symbols() if s['st_info']['type']=='STT_FUNC' and s['st_size']]
  def raw(a,n):
   g=next(g for g in elf.iter_segments() if g['p_type']=='PT_LOAD' and g['p_vaddr']<=a and a+n<=g['p_vaddr']+g['p_filesz']);o=g['p_offset']+a-g['p_vaddr'];return blob[o:o+n]
  rows=[];assembly=[];cs=Cs(CS_ARCH_ARM,CS_MODE_ARM)
  for a in ADDRESSES:
   s=next(s for s in symbols if int(s['st_value'])==a);n=int(s['st_size']);body=raw(a,n)
   rows.append({'original_symbol':s.name,'elf_address':hex(a),'size':n,'sha256':hashlib.sha256(body).hexdigest()})
   assembly.append(f'\n{s.name} @{a:08x}, bytes{n}, sha256={rows[-1]["sha256"]}')
   for i in cs.disasm(body,a):
    if a!=0x37280c or 0x373448<=i.address<=0x37357c or 0x3738a8<=i.address<=0x3738d0 or 0x37396c<=i.address<=0x373974:
     assembly.append(f'{i.address:08x}  {i.mnemonic:8} {i.op_str}')
  section=elf.get_section_by_name('.text');text=section.data();base=int(section['sh_addr']);xrefs=[]
  for offset in range(0,len(text)-3,4):
   w=struct.unpack_from('<I',text,offset)[0]
   if w&0x0e000000!=0x0a000000:continue
   imm=w&0x00ffffff;imm=imm-(1<<24) if imm&(1<<23) else imm
   target=(base+offset+8+imm*4)&0xffffffff
   if target not in (0x3d8894,0x3d8a04):continue
   caller=next((s for s in symbols if int(s['st_value'])<=base+offset<int(s['st_value'])+int(s['st_size'])),None)
   xrefs.append({'callsite':hex(base+offset),'target':hex(target),'instruction':'BL' if w&0x01000000 else 'B','caller':caller.name if caller else None,'caller_address':hex(int(caller['st_value'])) if caller else None})
 report={'original_sha256':ELF_SHA,'functions':rows,'direct_text_xrefs':xrefs,'claims':{
  'byte415':'Character+415 is embedded CharAI+4d; C2 3cec6c/C1 3cedcc write constructor1.',
  'fx_zero':'Character C2 3a963c/3a964c/3a9674 writes self1484/state148c/highlight14a0 zero.',
  'dead_order':'CSDead focus explicitly loads and queries two Debug switches before flags; CancelSneaking removes player buff146 then writes byte415=1 before reading resolvedSneak198.',
  'player_manager':'ManageCharacters calls UpdateAllSkills only after changed skill member400/continuation and valid signed received levels cause SG_SetSkillLevel in slots0..29. It is not blanket cadence.',
  'frame':'CharAI Update gates paused, controller forced/blocked/locked, flags520 bit100, zonable/zoned/inzone; then target/master/aggro and virtual OnUpdate. AISExternal calls default, flagged Lua OnUpdate, stateUpdate, conditions. Neither directly calls UpdateAllSkills.',
  'kill_continuation':'Current native application already reaches dead/HP0 Kill prefix; full Player kill count/trophy/online continuation remains open, and must not reenter full Kill after dead1.'},
 'scope':'Static original bytes and direct ARM B/BL xrefs only. No indirect-xref completeness, full Character/PlayerManager execution, native AI frame, native positive FX or Kill continuation claim.'}
 out.mkdir(parents=True,exist_ok=True);(out/'original-functions.json').write_text(json.dumps(report,indent=2)+'\n');(out/'original-functions.asm').write_text('\n'.join(assembly)+'\n');return report
