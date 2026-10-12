import sys,struct,json
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
base=r'C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/candidate/procedural-stream-serializer-v66-20261006-working/'
with open(r'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so','rb') as f:
 elf=ELFFile(f);syms={}
 for sec in elf.iter_sections():
  if sec.name in ('.dynsym','.symtab'):
   for s in sec.iter_symbols():
    a=int(s['st_value']);n=int(s['st_size'])
    if a and n:syms[a]=(s.name,n)
 def data(a,n):
  for p in elf.iter_segments():
   start=int(p['p_vaddr']);end=start+int(p['p_filesz'])
   if start<=a and a+n<=end:f.seek(int(p['p_offset'])+a-start);return f.read(n)
  raise ValueError(hex(a))
 a=0x310454;name,size=syms[a];d=Cs(CS_ARCH_ARM,CS_MODE_ARM);lines=['# '+name+' '+hex(a)+' size='+str(size)]
 for i in d.disasm(data(a,size),a):lines.append(f'{i.address:08x}: {i.mnemonic} {i.op_str}')
 text='\n'.join(lines)+'\n';open(base+'original-customalloc.asm','w').write(text);print(text)
 spec=[(0x30eee4,0x30eef8),(0x30efec,0x30f00c),(0x30eea8,0x30eeb0),(0x30eebc,0x30eec4),(0x51365c,0x513670)]
 out=[]
 for pc,literal in spec:
  offset=struct.unpack('<I',data(literal,4))[0];ptr=(pc+8+offset)&0xffffffff
  out.append({'add_pc':hex(pc),'pointer':hex(ptr),'value':data(ptr,256).split(b'\0',1)[0].decode('utf8','replace')})
 print(json.dumps(out,indent=2));open(base+'original-property-format-literals.json','w').write(json.dumps(out,indent=2))