from pathlib import Path
import sys,struct,json,bisect
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
from capstone.arm import ARM_OP_REG,ARM_OP_IMM,ARM_OP_MEM,ARM_REG_PC
root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');ref=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/device-provider-v54');flags={0x9f6403:'HTC',0x9f6404:'SHARP',0x9a5b59:'Multiplayer'}
with (root/'.local-inputs/libDungeonHunter2.so').open('rb') as f:
 e=ELFFile(f);sec=e.get_section_by_name('.text');code=sec.data();start=sec['sh_addr'];segments=[(p['p_vaddr'],p.data()) for p in e.iter_segments() if p['p_type']=='PT_LOAD'];symbols=sorted([(s['st_value'],s.name) for s in e.get_section_by_name('.symtab').iter_symbols() if s['st_info']['type']=='STT_FUNC' and s['st_value']]);addrs=[a for a,n in symbols]
def rd(a):
 for at,data in segments:
  if at<=a and a+4<=at+len(data):return struct.unpack_from('<I',data,a-at)[0]
 return None
cs=Cs(CS_ARCH_ARM,CS_MODE_ARM);cs.detail=True;cs.skipdata=True;regs={};rows=[]
for ins in cs.disasm(code,start):
 if not ins.id:regs={};continue
 regs[ARM_REG_PC]=ins.address+8;o=ins.operands;m=ins.mnemonic
 before=regs.copy();new=None
 if m.startswith('ldr') and len(o)>1 and o[0].type==ARM_OP_REG and o[1].type==ARM_OP_MEM:
  mem=o[1].mem;a=before.get(mem.base)
  if a is not None and (not mem.index or mem.index in before):
   a=(a+mem.disp+(before.get(mem.index,0) if mem.index else 0))&0xffffffff
   if m=='ldr':new=rd(a)
   if a in flags:
    i=bisect.bisect_right(addrs,ins.address)-1;rows.append({'pc':hex(ins.address),'operation':m,'target':flags[a],'symbol':symbols[i][1],'symbol_address':hex(symbols[i][0]),'instruction':ins.op_str})
 elif m.startswith('str') and len(o)>1 and o[1].type==ARM_OP_MEM:
  mem=o[1].mem;a=before.get(mem.base)
  if a is not None and not mem.index:
   a=(a+mem.disp)&0xffffffff
   if a in flags:
    i=bisect.bisect_right(addrs,ins.address)-1;rows.append({'pc':hex(ins.address),'operation':m,'target':flags[a],'symbol':symbols[i][1],'symbol_address':hex(symbols[i][0]),'source_value':before.get(o[0].reg),'instruction':ins.op_str})
 elif m=='mov' and len(o)>1:new=o[1].imm if o[1].type==ARM_OP_IMM else before.get(o[1].reg)
 elif m in ['add','sub'] and len(o)==3:
  a=before.get(o[1].reg);b=o[2].imm if o[2].type==ARM_OP_IMM else before.get(o[2].reg)
  if a is not None and b is not None:new=(a+b if m=='add' else a-b)&0xffffffff
 for r in ins.regs_access()[1]:regs.pop(r,None)
 if new is not None and o and o[0].type==ARM_OP_REG:regs[o[0].reg]=new
 if m in ['bl','blx']:
  # Caller-clobbers use architecture register IDs; retain callee-saved base.
  from capstone.arm import ARM_REG_R0,ARM_REG_R1,ARM_REG_R2,ARM_REG_R3
  for r in [ARM_REG_R0,ARM_REG_R1,ARM_REG_R2,ARM_REG_R3]:regs.pop(r,None)
(ref/'flag-static-xrefs-v54.json').write_text(json.dumps({'method':'linear constant propagation candidate xrefs, must validate containing source body; not exhaustive CFG proof','xrefs':rows},indent=2)+'\n');print(json.dumps(rows,indent=2))
