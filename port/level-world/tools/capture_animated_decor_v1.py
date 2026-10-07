from pathlib import Path
import sys,json,hashlib,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so';ref=root/'port/level-world/reference/animated-decor-v1';ref.mkdir(parents=True,exist_ok=True)
md=Cs(CS_ARCH_ARM,CS_MODE_ARM);rows=[];text=[]
with original.open('rb') as f:
 elf=ELFFile(f);segments=list(elf.iter_segments());symbols=list(elf.get_section_by_name('.symtab').iter_symbols())
 def raw(a,n):
  p=next(p for p in segments if p['p_vaddr']<=a and a+n<=p['p_vaddr']+p['p_filesz']);f.seek(p['p_offset']+a-p['p_vaddr']);return f.read(n)
 def word(a):return struct.unpack('<I',raw(a,4))[0]
 def string(a):
  out=b''
  while raw(a,1)!=b'\0':out+=raw(a,1);a+=1
  return out.decode('utf-8')
 literals=[{'use':'random_all_token','address':hex(0x389184+word(0x38932c)),'value':string(0x389184+word(0x38932c))}, {'use':'empty_start_animation_assignment','address':hex(0x389318+word(0x389338)),'value':string(0x389318+word(0x389338))}]
 for s in sorted(symbols,key=lambda s:s['st_value']):
  if s['st_info']['type']!='STT_FUNC' or not s['st_size']:continue
  if not any(k in s.name for k in ('13AnimatedDecor','5Decor','15POAnimatedDecor')) and s['st_value'] not in (0x388a98,0x388a2c,0x38ab60,0x388c58,0x31167c,0x3109e0):continue
  a=s['st_value'];code=raw(a,s['st_size']);rows.append({'address':hex(a),'symbol':s.name,'size':s['st_size']});text.append('# '+hex(a)+' '+s.name+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}' for i in md.disasm(code,a)))
(ref/'original-source.asm').write_text('\n\n'.join(text)+'\n');(ref/'original-functions.json').write_text(json.dumps({'original_sha256':hashlib.sha256(original.read_bytes()).hexdigest(),'functions':rows},indent=2)+'\n')
(ref/'original-literals.json').write_text(json.dumps(literals,indent=2)+'\n');print(literals)
print('\n'.join(str(r) for r in rows))
