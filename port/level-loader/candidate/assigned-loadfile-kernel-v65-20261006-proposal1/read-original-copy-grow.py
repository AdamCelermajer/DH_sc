import sys
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
elf_path=r'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so'
with open(elf_path,'rb') as handle:
 elf=ELFFile(handle);syms={}
 for section in elf.iter_sections():
  if section.name in ('.dynsym','.symtab'):
   for sym in section.iter_symbols():
    value=int(sym['st_value']);size=int(sym['st_size'])
    if size and value:syms[value]=(sym.name,size)
 def source(addr,size):
  for segment in elf.iter_segments():
   start=int(segment['p_vaddr']);end=start+int(segment['p_filesz'])
   if start<=addr and addr+size<=end:
    handle.seek(int(segment['p_offset'])+addr-start);return handle.read(size)
  raise RuntimeError('address outside retained file-backed ELF segments')
 decoder=Cs(CS_ARCH_ARM,CS_MODE_ARM);todo=[0x317014,0x31692c];seen=set();output=[]
 while todo and len(seen)<8:
  addr=todo.pop(0)
  if addr in seen:continue
  seen.add(addr)
  if addr not in syms:raise RuntimeError('missing exact symbol size '+hex(addr))
  name,size=syms[addr]
  if size>4096:raise RuntimeError('bounded function size exceeded')
  output.append('\n# '+name+' '+hex(addr)+' size='+str(size))
  for ins in decoder.disasm(source(addr,size),addr):
   output.append(f'{ins.address:08x}: {ins.mnemonic} {ins.op_str}')
   if ins.mnemonic=='bl' and ins.op_str.startswith('#'):
    target=int(ins.op_str[1:],0)
    if 0x316000<=target<0x318000 and target in syms and target not in seen and target not in todo:todo.append(target)
 text='\n'.join(output)+'\n'
 with open(r'C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/candidate/assigned-loadfile-kernel-v65-20261006-proposal1/original-copy-grow-source.asm','w',encoding='utf8') as out:out.write(text)
 print(text)