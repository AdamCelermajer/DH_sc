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
  raise ValueError('unmapped '+hex(a))
 decoder=Cs(CS_ARCH_ARM,CS_MODE_ARM)
 for address in [0x5134f0,0x487e84,0x489508,0x485aa0]:
  name,size=syms[address]
  lines=['# '+name+' '+hex(address)+' size='+str(size)]
  for ins in decoder.disasm(data(address,size),address):
   suffix=''
   if ins.mnemonic=='bl' and ins.op_str.startswith('#'):
    target=int(ins.op_str[1:],0);suffix=' ; '+names.get(target,'')
   lines.append(f'{ins.address:08x}: {ins.mnemonic} {ins.op_str}{suffix}')
  text='\n'.join(lines)+'\n';open(base+f'original-{address:08x}.asm','w',encoding='utf8').write(text)
  if address==0x5134f0:print(text)
 selected=[{'address':hex(a),'name':name,'size':size} for a,(name,size) in syms.items() if ('ToString' in name or 'ObjToStr' in name or 'ToXML' in name) and ('Property' in name or 'ObjToStr' in name)]
 open(base+'property-emission-symbols.json','w',encoding='utf8').write(json.dumps(selected,indent=2))
 print(json.dumps(selected,indent=2))
 # Exact original PC-relative literals in the two emission roots, with pointer
 # addresses calculated from original LDR literals and ADD PC instruction sites.
 spec=[(0x488be0,0x488e24),(0x488bd8,0x488e28),(0x488bdc,0x488e2c),(0x488c20,0x488e34),(0x488c54,0x488e38),(0x488c70,0x488e3c),(0x488cec,0x488e40),(0x488d48,0x488e48),(0x491e1c,0x492298),(0x491ea8,0x4922a8),(0x49207c,0x4922bc),(0x492080,0x4922c0),(0x4920ec,0x4922cc),(0x4920f8,0x4922d0)]
 literals=[]
 for pc,literal in spec:
  offset=struct.unpack('<I',data(literal,4))[0];pointer=(pc+8+offset)&0xffffffff;raw=data(pointer,256).split(b'\0',1)[0]
  literals.append({'add_pc':hex(pc),'literal':hex(literal),'pointer':hex(pointer),'value':raw.decode('utf8','replace')})
 open(base+'original-emission-literals.json','w',encoding='utf8').write(json.dumps(literals,indent=2));print(json.dumps(literals,indent=2))