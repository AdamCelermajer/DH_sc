from pathlib import Path
import sys,json,hashlib
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so';ref=root/'port/level-world/reference/world-touch-target-v1';ref.mkdir(parents=True,exist_ok=True)
md=Cs(CS_ARCH_ARM,CS_MODE_ARM);rows=[];text=[]
with original.open('rb') as f:
 elf=ELFFile(f);segments=list(elf.iter_segments());symbols=list(elf.get_section_by_name('.symtab').iter_symbols())
 for s in sorted(symbols,key=lambda s:s['st_value']):
  if s['st_info']['type']!='STT_FUNC' or not s['st_size']:continue
  if not any(k in s.name for k in ('Pick','Touch','ScreenRay','ScreenTo','ObjectOfInterest','OOI','SetTarget','RayFrom','GetRay','SelectTarget','Ctrl_Click','Cmd_Click','GetWorldPosFromScreen'))and s['st_value'] not in (0x38ac0c,0x3ad430,0x3c0334,0x3c02e8,0x3d5a98,0x3d574c,0x41a780,0x405f48,0x4054e4,0x405318,0x405260,0x3d49c4,0x3a9340,0x3cebf0,0x320e74,0x320e44,0x525800,0x5212e0,0x51b874):continue
  a=s['st_value'];size=s['st_size'];p=next(p for p in segments if p['p_vaddr']<=a and a+size<=p['p_vaddr']+p['p_filesz']);f.seek(p['p_offset']+a-p['p_vaddr']);code=f.read(size);rows.append({'address':hex(a),'symbol':s.name,'size':size});text.append('# '+hex(a)+' '+s.name+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}'for i in md.disasm(code,a)))
(ref/'original-source.asm').write_text('\n\n'.join(text)+'\n');(ref/'original-functions.json').write_text(json.dumps({'original_sha256':hashlib.sha256(original.read_bytes()).hexdigest(),'functions':rows},indent=2)+'\n')
for r in rows:print(r['address'],r['size'],r['symbol'])
