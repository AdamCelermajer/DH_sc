from pathlib import Path
import sys,json,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
import hashlib,subprocess
out=Path('port/level-world/reference/application-spawn-random-v4');out.mkdir(parents=True,exist_ok=True)
with open('.local-inputs/libDungeonHunter2.so','rb') as f:
 e=ELFFile(f);syms=list(e.get_section_by_name('.symtab').iter_symbols())
 selected=[s for s in syms if s.name in ['_ZN6Random6s_seedE','_ZN6Random12s_syncedSeedE','_ZN6Random21s_debugRandomCountersE']]
 data={'elf_sha256':hashlib.sha256(Path('.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'globals':[]}
 for s in selected:
  section=e.get_section(s['st_shndx']);data['globals'].append({'symbol':s.name,'address':hex(s['st_value']),'size':s['st_size'],'section':section.name,'section_type':section['sh_type'],'initial_zero':section['sh_type']=='SHT_NOBITS'})
 assert all(g['initial_zero'] for g in data['globals'])
 (out/'source-global-initialization.json').write_text(json.dumps(data,indent=2))
 functions=['3136dc','33ff90','33ddb4','32d79c']
 for s in syms:
  if 'CheckSpawnProbability' in s.name: functions.append(hex(s['st_value'])[2:])
 for s in syms:
  if 'Random' in s.name or ('Application' in s.name and 'C1' in s.name):print(hex(s['st_value']),s['st_size'],s.name)
 for a in [0x99f89c,0x99f8a4]:
  seg=next(s for s in e.iter_segments() if s['p_type']=='PT_LOAD' and s['p_vaddr']<=a<s['p_vaddr']+s['p_memsz'])
  if a>=seg['p_vaddr']+seg['p_filesz']:print(hex(a),'BSS zero');continue
  f.seek(seg['p_offset']+a-seg['p_vaddr']);print(hex(a),f.read(8).hex())
capture=subprocess.run([sys.executable,'.local-inputs/audit_reload_components.py',*functions],capture_output=True,text=True,check=True)
(out/'original-functions.asm').write_text(capture.stdout)
