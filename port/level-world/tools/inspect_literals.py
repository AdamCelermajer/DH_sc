"""Resolve direct PC-relative literal strings in hash-identified ARM routines."""
import argparse,hashlib,json,struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
from capstone.arm import ARM_OP_MEM,ARM_OP_REG,ARM_REG_PC
EXPECTED='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
def main():
 p=argparse.ArgumentParser();p.add_argument('engine',type=Path);p.add_argument('addresses',nargs='+');p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==EXPECTED
 wanted={int(x,0) for x in a.addresses};rows=[]
 with a.engine.open('rb') as f:
  elf=ELFFile(f);loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
  def read(at,size):
   segment=next((s for s in loads if s['p_vaddr']<=at and at+size<=s['p_vaddr']+s['p_filesz']),None)
   if not segment:return b''
   f.seek(segment['p_offset']+at-segment['p_vaddr']);return f.read(size)
  dis=Cs(CS_ARCH_ARM,CS_MODE_ARM);dis.detail=True
  for symbol in elf.get_section_by_name('.symtab').iter_symbols():
   if symbol['st_info']['type']!='STT_FUNC' or symbol['st_value'] not in wanted:continue
   pending={};literals=[]
   for i in dis.disasm(read(symbol['st_value'],symbol['st_size']),symbol['st_value']):
    ops=i.operands
    if i.mnemonic=='ldr' and len(ops)==2 and ops[0].type==ARM_OP_REG and ops[1].type==ARM_OP_MEM and ops[1].mem.base==ARM_REG_PC:
     at=i.address+8+ops[1].mem.disp;raw=read(at,4)
     if len(raw)==4:pending[ops[0].reg]=(struct.unpack('<I',raw)[0],i.address,at)
     continue
    if i.mnemonic=='add' and len(ops)==3 and all(o.type==ARM_OP_REG for o in ops) and ops[1].reg==ARM_REG_PC and ops[2].reg in pending:
     value,load,field=pending[ops[2].reg];target=(i.address+8+value)&0xffffffff;raw=read(target,256);end=raw.find(b'\0');text=raw[:end] if end>=0 else b''
     if text and all(32<=c<127 for c in text):literals.append({'load_instruction':hex(load),'add_instruction':hex(i.address),'literal_field':hex(field),'literal_word':hex(value),'target':hex(target),'text':text.decode('ascii')})
    for reg in i.regs_access()[1]:pending.pop(reg,None)
   rows.append({'symbol':symbol.name,'address':hex(symbol['st_value']),'size':symbol['st_size'],'strings':literals})
 result={'original_sha256':EXPECTED,'functions':rows,'scope':'Direct PC-relative string literals; not control-flow or indirect/GOT evaluation'};a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
