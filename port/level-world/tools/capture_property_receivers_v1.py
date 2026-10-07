from pathlib import Path
import sys,json,hashlib,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so';ref=root/'port/level-world/reference/canonical-object-factory-v1'
md=Cs(CS_ARCH_ARM,CS_MODE_ARM)
with original.open('rb')as f:
 elf=ELFFile(f);segments=list(elf.iter_segments());symbols={s['st_value']:s for s in elf.get_section_by_name('.symtab').iter_symbols()if s['st_size']and s['st_info']['type']=='STT_FUNC'}
 def raw(a,n):
  p=next(p for p in segments if p['p_vaddr']<=a and a+n<=p['p_vaddr']+p['p_filesz']);f.seek(p['p_offset']+a-p['p_vaddr']);return f.read(n)
 tables=set(int(row['descriptor_vtable'],16)for c in json.loads((ref/'property-declarations-original.json').read_text())['classes']for row in c['declarations']if 'descriptor_vtable'in row)
 # PropertyT<bool>, PropertyT<int> plus source CString/Point3D AddProperty leaves.
 addresses=set();mapping=[]
 for table in sorted(tables):
  slots=struct.unpack('<6I',raw(table,24));mapping.append(dict(vtable=hex(table),slots=[dict(address=hex(a),symbol=symbols[a].name if a in symbols else None)for a in slots]));addresses.update(slots)
 addresses.update(a for a,s in symbols.items()if ('SimpleTypeProperty' in s.name)and any(n in s.name for n in('SetToDefault','SetValue','Clone','FromString')))
 addresses.update(a for a,s in symbols.items()if any(n in s.name for n in('StrTo','StringTo','ReadValue','FromString')))
 output=[]
 for a in sorted(addresses):
  if a not in symbols:continue
  s=symbols[a];output.append('# '+hex(a)+' '+s.name+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}'for i in md.disasm(raw(a,s['st_size']),a)))
 (ref/'property-receivers.asm').write_text('\n\n'.join(output)+'\n');(ref/'property-receiver-vtables.json').write_text(json.dumps(mapping,indent=2)+'\n')
print(json.dumps(mapping,indent=2))
