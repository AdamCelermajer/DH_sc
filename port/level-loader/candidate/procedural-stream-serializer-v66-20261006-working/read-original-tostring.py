import sys,struct,json
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
base=r'C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/candidate/procedural-stream-serializer-v66-20261006-working/'
with open(r'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so','rb') as f:
 elf=ELFFile(f);syms={};names={}
 for sec in elf.iter_sections():
  if sec.name in ('.dynsym','.symtab'):
   for s in sec.iter_symbols():
    a=int(s['st_value']);n=int(s['st_size'])
    if a:names[a]=s.name
    if a and n:syms[a]=(s.name,n)
 def data(a,n):
  for p in elf.iter_segments():
   start=int(p['p_vaddr']);end=start+int(p['p_filesz'])
   if start<=a and a+n<=end:f.seek(int(p['p_offset'])+a-start);return f.read(n)
  raise ValueError(hex(a))
 decoder=Cs(CS_ARCH_ARM,CS_MODE_ARM);output=[]
 for a in [0x510fe0,0x33e218,0x3f3858,0x389980,0x30eec8,0x30efb0,0x30eea0,0x30eeb4,0x30ee60]:
  name,size=syms[a];lines=['\n# '+name+' '+hex(a)+' size='+str(size)]
  for i in decoder.disasm(data(a,size),a):
   suffix=''
   if i.mnemonic=='bl' and i.op_str.startswith('#'):suffix=' ; '+names.get(int(i.op_str[1:],0),'')
   lines.append(f'{i.address:08x}: {i.mnemonic} {i.op_str}{suffix}')
  output.extend(lines)
 text='\n'.join(output)+'\n';open(base+'original-property-tostring.asm','w',encoding='utf8').write(text);print(text)