"""Locate direct ARM-state B/BL callers in the hash-identified original ELF."""
import argparse,hashlib,json,struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
EXPECTED='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
def main():
 p=argparse.ArgumentParser();p.add_argument('engine',type=Path);p.add_argument('targets',nargs='+');p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==EXPECTED;targets={int(x,0) for x in a.targets};rows=[]
 with a.engine.open('rb') as f:
  elf=ELFFile(f);segments=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
  for symbol in elf.get_section_by_name('.symtab').iter_symbols():
   start=symbol['st_value'];size=symbol['st_size']
   if symbol['st_info']['type']!='STT_FUNC' or start%4 or not size:continue
   segment=next((s for s in segments if s['p_vaddr']<=start and start+size<=s['p_vaddr']+s['p_filesz']),None)
   if not segment:continue
   f.seek(segment['p_offset']+start-segment['p_vaddr']);raw=f.read(size)
   for offset in range(0,size-3,4):
    instruction=struct.unpack_from('<I',raw,offset)[0]
    if instruction&0x0e000000!=0x0a000000 or instruction>>28==15:continue
    immediate=instruction&0xffffff
    if immediate&0x800000:immediate-=0x1000000
    target=(start+offset+8+immediate*4)&0xffffffff
    if target in targets:rows.append({'caller':symbol.name,'caller_address':hex(start),'caller_size':size,'branch_address':hex(start+offset),'target':hex(target),'link':bool(instruction&0x1000000)})
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps({'original_sha256':EXPECTED,'direct_arm_state_calls':rows},indent=2)+'\n');print(json.dumps(rows,indent=2))
if __name__=='__main__':main()
