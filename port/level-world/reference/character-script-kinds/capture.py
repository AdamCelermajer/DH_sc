"""Read original AIS vtables and capture exact additional-kind routines."""
import hashlib,json,struct,subprocess,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1]
engine=REPO/'.local-inputs/libDungeonHunter2.so'
assert hashlib.sha256(engine.read_bytes()).hexdigest()=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
with engine.open('rb') as file:
 elf=ELFFile(file);symbols=list(elf.get_section_by_name('.symtab').iter_symbols());names={s.name:s for s in symbols};by_address={}
 for s in symbols:
  if s['st_info']['type']=='STT_FUNC':by_address.setdefault(s['st_value'],[]).append(s.name)
 def read(addr,size):
  seg=next(x for x in elf.iter_segments() if x['p_type']=='PT_LOAD' and x['p_vaddr']<=addr< x['p_vaddr']+x['p_filesz']);file.seek(seg['p_offset']+addr-seg['p_vaddr']);return file.read(size)
 result=[];addresses={0x3ccbe4,0x3cccf8,0x3ccaf4,0x3cce14,0x3ceeb0,0x3cf04c}
 for name in ['AISDefault','AISMonster','AISFaery','AISExternal','AISPlayer','AISPlayerIPhone']:
  s=names['_ZTV'+str(len(name))+name];raw=read(s['st_value'],s['st_size']);slots=[]
  for offset in range(8,len(raw),4):
   addr=struct.unpack_from('<I',raw,offset)[0];slots.append(dict(slot=hex(offset-8),address=hex(addr),symbols=by_address.get(addr,[])))
   if name in ['AISDefault','AISMonster','AISFaery','AISExternal']:addresses.add(addr)
  result.append(dict(kind=name,vtable=hex(s['st_value']+8),raw_size=len(raw),raw_sha256=hashlib.sha256(raw).hexdigest(),slots=slots))
 constructors=[dict(symbol=s.name,address=hex(s['st_value']),size=s['st_size']) for s in symbols if s['st_info']['type']=='STT_FUNC' and any(s.name.startswith('_ZN'+str(len(k))+k) and ('C1E' in s.name or 'C2E' in s.name or 'D0E' in s.name or 'D1E' in s.name or 'D2E' in s.name) for k in ['AISMonster','AISFaery','AISExternal'])]
 addresses.update(int(s['address'],16) for s in constructors)
 (HERE/'vtables.json').write_text(json.dumps(dict(original_sha256=hashlib.sha256(engine.read_bytes()).hexdigest(),vtables=result,constructors=constructors),indent=2)+'\n')
 subprocess.run([sys.executable,str(ROOT/'tools/capture_reference.py'),str(engine),*(hex(x) for x in sorted(addresses) if x in by_address),'--output',str(HERE)],check=True)
 print(json.dumps(dict(vtables=len(result),unique_functions=len(addresses),constructors=constructors)))
