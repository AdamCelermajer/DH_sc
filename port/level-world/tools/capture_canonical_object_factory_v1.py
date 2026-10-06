from pathlib import Path
import sys,json,hashlib,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so';ref=root/'port/level-world/reference/canonical-object-factory-v1';ref.mkdir(parents=True,exist_ok=True)
md=Cs(CS_ARCH_ARM,CS_MODE_ARM);rows=[];text=[]
with original.open('rb') as f:
 elf=ELFFile(f);segments=list(elf.iter_segments());symbols=list(elf.get_section_by_name('.symtab').iter_symbols())
 def raw(address,n):
  segment=next(p for p in segments if p['p_vaddr']<=address and address+n<=p['p_vaddr']+p['p_filesz']);f.seek(segment['p_offset']+address-segment['p_vaddr']);return f.read(n)
 def word(a):return struct.unpack('<I',raw(a,4))[0]
 def string(a):
  out=b''
  while raw(a,1)!=b'\0':out+=raw(a,1);a+=1
  return out.decode('utf-8')
 table=0x34b588+8+word(0x34b710);catalog=[]
 for index in range(33):
  name,factory=struct.unpack('<II',raw(table+8*index,8));catalog.append(dict(index=index,name=string(name),factory=hex(factory)))
 selected={s['st_value']:s for s in symbols if s['st_info']['type']=='STT_FUNC'and s['st_size']and (any(x in s.name for x in('13ObjectManager','17OpenableContainer','13AnimatedDecor','8Treasure','11PropertyMap'))or (any(x in s.name for x in('10ObjectBase','10GameObject','9Character'))and any(x in s.name for x in('DeclareProperties','C1E','C2E','GetName','GetType')))or s['st_value']in [int(c['factory'],16)for c in catalog]or s['st_value']in(0x33fc88,0x33f50c,0x33dd2c,0x34ac18,0x33e6d4,0x33ddc8,0x510b4c,0x513aa8,0x5124d4,0x38aac8,0x4a191c,0x524644))}
 for address,s in sorted(selected.items()):
  size=s['st_size'];segment=next(p for p in segments if p['p_vaddr']<=address and address+size<=p['p_vaddr']+p['p_filesz']);f.seek(segment['p_offset']+address-segment['p_vaddr']);code=f.read(size)
  rows.append(dict(address=hex(address),symbol=s.name,size=size,sha256=hashlib.sha256(code).hexdigest()));text.append('# '+hex(address)+' '+s.name+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}'for i in md.disasm(code,address)))
(ref/'original-source.asm').write_text('\n\n'.join(text)+'\n');(ref/'original-functions.json').write_text(json.dumps(dict(original_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),functions=rows),indent=2)+'\n')
(ref/'factory-catalog.json').write_text(json.dumps(dict(address=hex(table),entries=catalog),indent=2)+'\n')
print(json.dumps(catalog))
for r in rows:print(r['address'],r['size'],r['symbol'])
