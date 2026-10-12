"""Record direct ARM BL callers and locally decoded source instruction windows.
This inventory does not infer indirect calls or order different loader cases.
"""
import bisect,hashlib,json,struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
HERE=Path(__file__).resolve().parent;REPO=HERE.parents[3]
def main():
 path=REPO/'.local-inputs/libDungeonHunter2.so';original=hashlib.sha256(path.read_bytes()).hexdigest();assert original=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
 with path.open('rb')as f:
  e=ELFFile(f);symbols=sorted((s['st_value'],s['st_value']+s['st_size'],s.name)for s in e.get_section_by_name('.symtab').iter_symbols()if s['st_info']['type']=='STT_FUNC'and s['st_size']);starts=[s[0]for s in symbols];t=e.get_section_by_name('.text');raw=t.data();base=t['sh_addr'];result=[];cs=Cs(CS_ARCH_ARM,CS_MODE_ARM);asm=[]
  for i,(v,)in enumerate(struct.iter_unpack('<I',raw[:len(raw)//4*4])):
   if v&0x0f000000!=0x0b000000:continue
   delta=v&0xffffff;delta=delta-0x1000000 if delta&0x800000 else delta;addr=base+i*4;target=addr+8+delta*4
   if target in[0x496bd8,0x495a88,0x4967e8,0x494e3c]:
    s=symbols[bisect.bisect_right(starts,addr)-1];result.append({'call':hex(addr),'target':hex(target),'caller':s[2],'caller_address':hex(s[0]),'caller_size':s[1]-s[0]});begin=max(s[0],addr-32);end=min(s[1],addr+20);block=raw[begin-base:end-base];asm.append('\n# '+s[2]+' direct call '+hex(addr)+'\n');asm.extend(f'{x.address:08x}: {x.mnemonic:8} {x.op_str}\n'for x in cs.disasm(block,begin))
  (HERE/'callers.json').write_text(json.dumps({'original_sha256':original,'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'direct_bl_callers':result,'scope':__doc__},indent=2)+'\n');(HERE/'callers.asm').write_text(''.join(asm))
 print(json.dumps({'validation':'PASS','direct_calls':len(result)}))
if __name__=='__main__':main()
