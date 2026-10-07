from pathlib import Path
import sys,json,hashlib
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so';ref=root/'port/level-world/reference/level-config-module-connection-v2';ref.mkdir(parents=True,exist_ok=True)
md=Cs(CS_ARCH_ARM,CS_MODE_ARM)
with original.open('rb')as f:
 elf=ELFFile(f);segments=list(elf.iter_segments());symbols={s['st_value']:s for s in elf.get_section_by_name('.symtab').iter_symbols()if s['st_size']and s['st_info']['type']=='STT_FUNC'}
 def raw(a,n):
  p=next(p for p in segments if p['p_vaddr']<=a and a+n<=p['p_vaddr']+p['p_filesz']);f.seek(p['p_offset']+a-p['p_vaddr']);return f.read(n)
 addresses={0x3f150c,0x340ca8,0x340f98,0x3f28ec,0x388b20,0x38a88c,0x38a38c,0x37ba84,0x523c14,0x340f74,
            0x522844,0x521eb8,0x5239e8,0x39771c,0x393db4,0x597180,0x50eeb4,0x350e5c,0x3524a0,
            0x3596f8,0x35a0e4,0x5970f4,0x59712c,0x50e46c,0x393600,0x38aac8,0x61c878,0x61c6f4,0x61c290,0x61be14,0x61c214,0x60e54c}
 addresses.update(a for a,s in symbols.items() if 'ISceneNodeC' in s.name)
 addresses.update({0x520b40,0x319158,0x51cfb8,0x597c60,0x47295c,0x35a8e0,0x34b724,0x598908,0x61b2f4,0x5839d8})
 addresses.update({0x31f594,0x386190,0x3860bc,0x386818,0x385ed8})
 addresses.update({0x50f89c,0x596ec4,0x597004,0x5971e0,0x598864})
 addresses.update({0x585118})
 addresses.update(a for a,s in symbols.items() if 'LevelC' in s.name)
 addresses.update(a for a,s in symbols.items() if 'CStrProps' in s.name)
 addresses.update(a for a,s in symbols.items() if 'UserProperties' in s.name)
 addresses.update(a for a,s in symbols.items() if 'CalcMeshBox' in s.name)
 addresses.update(a for a,s in symbols.items() if ('RoomZone' in s.name and any(x in s.name for x in ('C1','C2','InitWithBounding','InitPost'))))
 addresses.update(a for a,s in symbols.items() if ('PFRoom' in s.name and any(x in s.name for x in ('C1','C2','LoadScene','ExtendBoundingBox','Init'))))
 addresses.update(a for a,s in symbols.items() if any(x in s.name for x in ('OptimizeStatic','InitWithBoundingBox','LoadNode')))
 addresses.update(a for a,s in symbols.items() if ('Zone' in s.name and any(x in s.name for x in ('C1E','C2E')) and s['st_size']<1000))
 output=[];rows=[]
 for a in sorted(addresses):
  s=symbols[a];rows.append(dict(address=hex(a),size=s['st_size'],symbol=s.name));output.append('# '+hex(a)+' '+s.name+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}'for i in md.disasm(raw(a,s['st_size']),a)))
 (ref/'original-source.asm').write_text('\n\n'.join(output)+'\n');(ref/'original-functions.json').write_text(json.dumps(dict(original_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),functions=rows),indent=2)+'\n')
 print(json.dumps(rows,indent=2))
