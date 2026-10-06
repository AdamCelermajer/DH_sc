from pathlib import Path
import sys,subprocess
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
with open('.local-inputs/libDungeonHunter2.so','rb') as f:
 syms=list(ELFFile(f).get_section_by_name('.symtab').iter_symbols())
 selected=[s for s in syms if (('CharPropertiesC1' in s.name or 'CharPropertiesC2' in s.name) or s.name in ['_ZN9Character18SafeGetCharPropsIdEv','_ZN14CharProperties18LoadBasePropertiesEi']) and s['st_size']]
 for s in selected:print(hex(s['st_value']),s.name)
out=Path('port/level-world/reference/canonical-character-family-v4');out.mkdir(parents=True,exist_ok=True)
r=subprocess.run([sys.executable,'.local-inputs/audit_reload_components.py',*[hex(s['st_value'])[2:] for s in selected]],capture_output=True,text=True,check=True)
(out/'property-constructor-source.asm').write_text(r.stdout)
