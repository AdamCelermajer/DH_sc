"""Source literal/dependency inventory; subsequent instruction probes live here."""
import json,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
ROOT=Path(__file__).resolve().parents[1]; REPO=ROOT.parents[1]
def main():
 e=ELFFile(open(REPO/'.local-inputs/libDungeonHunter2.so','rb'))
 def read(at,n):
  for s in e.iter_segments():
   if s['p_vaddr']<=at< s['p_vaddr']+s['p_filesz']:return s.data()[at-s['p_vaddr']:at-s['p_vaddr']+n]
  return bytes(n)
 def word(at):return struct.unpack('<I',read(at,4))[0]
 def string(at):return read(at,300).split(b'\0')[0].decode('utf8',errors='replace')
 files=[ROOT/'reference/localization/original-functions.json',*[REPO/'.local-inputs/localization-discovery'/s/'original-functions.json' for s in ['','extra','format']]]
 rows=[]
 for p in files:
  for f in json.loads(p.read_text())['functions']:
   addr=f.get('address',f.get('elf_address'));addr=int(addr,0) if isinstance(addr,str) else addr;md=Cs(CS_ARCH_ARM,CS_MODE_ARM);regs={};out=[]
   for ins in md.disasm(read(addr,f['size']),addr):
    # PC-relative literal loaded then added to PC, or GOT literal dereference.
    args=ins.op_str.split(', ')
    if ins.mnemonic=='ldr' and '[pc, #' in ins.op_str:
     off=int(ins.op_str.split('#')[1].split(']')[0],0);regs[args[0]]=word(ins.address+8+off)
    elif ins.mnemonic=='add' and len(args)==3 and args[1]=='pc' and args[2] in regs:
     value=(ins.address+8+regs[args[2]])&0xffffffff;regs[args[0]]=value
     text=string(value)
     if text and all(ord(c)>=32 or c in '\n\t' for c in text):out.append({'instruction':hex(ins.address),'address':hex(value),'text':text})
   rows.append({'symbol':f.get('symbol',f.get('original_symbol')),'address':hex(addr),'literals':out})
 raw=(REPO/'port/android-native/app/src/main/assets/original-cache/data/pydata/common_text_pyarray.bin').read_bytes()
 report={'source_literals':rows,'common_text_prefix':raw[:100].hex(),'common_text_bytes':len(raw)}
 p=REPO/'.local-inputs/localization-discovery/literals.json';p.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
if __name__=='__main__':main()
