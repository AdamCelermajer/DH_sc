"""Read-only NPC physical-input source/authoring capture. No XML factory replay."""
import hashlib,json,struct,zipfile,xml.etree.ElementTree as ET
from pathlib import Path
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
from elftools.elf.elffile import ELFFile
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
ELF=ROOT/'.local-inputs/libDungeonHunter2.so'
CACHE=Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
ASSETS=ROOT/'port/android-native/app/src/main/assets'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(ELF)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
assert sha(CACHE)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
functions=[];assembly=[]
with ELF.open('rb') as f:
 e=ELFFile(f);symbols=list(e.get_section_by_name('.symtab').iter_symbols())
 for address in (0x33f310,0x33f014,0x3a2fec,0x3a3024,0x3a3054,0x3a49f0,0x3b4088,0x3a4398):
  s=next(s for s in symbols if s['st_value']==address and s['st_size'] and s['st_info']['type']=='STT_FUNC')
  segment=next(p for p in e.iter_segments() if p['p_vaddr']<=address<p['p_vaddr']+p['p_filesz'])
  f.seek(segment['p_offset']+address-segment['p_vaddr']);raw=f.read(s['st_size'])
  functions.append(dict(address=hex(address),symbol=s.name,size=len(raw),sha256=hashlib.sha256(raw).hexdigest()))
  assembly.append('\n# '+s.name);assembly.extend(f'{i.address:08x}: {i.mnemonic:8} {i.op_str}' for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw,address))
(HERE/'original-functions.asm').write_text('\n'.join(assembly)+'\n')
(HERE/'original-functions.json').write_text(json.dumps(dict(original_sha256=sha(ELF),functions=functions),indent=2)+'\n')
provenance=json.loads((ASSETS/'actor-provenance.json').read_text());placements=[p for p in provenance['records'] if p['kind']==1]
rooms=json.loads((ASSETS/'worlds/crypt01-provenance.json').read_text())['rooms']
authoring=[];source_members=[]
with zipfile.ZipFile(CACHE) as z:
 for room in sorted(set(p['room'] for p in placements)):
  member=next(n for n in z.namelist() if n.endswith('/'+rooms[room]['gameplay']));raw=z.read(member)
  source_members.append(dict(room=room,member=member,sha256=hashlib.sha256(raw).hexdigest()))
  for p in (p for p in placements if p['room']==room):
   node=next(n for n in ET.fromstring(raw).iter('GameObject') if n.get('name')==p['name'])
   authoring.append(dict(room=room,name=p['name'],character=p['character'],model=p['model'],attributes=dict(node.attrib),DACT_position=p['position'],DACT_rotation=p['rotation_degrees'],DACT_scale=p['scale']))
assert len(authoring)==11
# This raw table inspection is discovery only; the native audit uses genuine
# decoded tables and property resolver/class kernels.
def strings(raw):
 count=struct.unpack_from('<I',raw)[0];at=4;out=[]
 for _ in range(count):
  n=struct.unpack_from('<I',raw,at)[0];at+=4;out.append(raw[at:at+n].decode('ascii'));at+=n
 return out
data=ASSETS/'data';names=strings((data/'character_properties_pyarraynames.bin').read_bytes());fields=strings((data/'character_properties_pystructnames.bin').read_bytes());raw=(data/'character_properties_pyarray.bin').read_bytes()
rows=[]
for name in sorted(set(p['character'] for p in placements)):
 i=names.index(name);row=struct.unpack_from('<224i',raw,4+896*i)
 rows.append(dict(character=name,row=i,raw_properties={fields[j]:row[j] for j in (1,12,13,14,16)}))
ai_names=strings((data/'ai_pyarraynames.bin').read_bytes());ai_raw=(data/'ai_pyarray.bin').read_bytes();at=4;ai_rows=[]
for i in range(struct.unpack_from('<I',ai_raw)[0]):
 at+=12+1+4+12+4;n=struct.unpack_from('<I',ai_raw,at)[0];at+=4;script=ai_raw[at:at+n].decode('ascii');at+=n
 self_fx,trophy,kind=struct.unpack_from('<3i',ai_raw,at);at+=12+8
 if i in (40,68):ai_rows.append(dict(row=i,name=ai_names[i],script=script,type=kind))
assert at==len(ai_raw)
report=dict(original_sha256=sha(ELF),cache_sha256=sha(CACHE),descriptor_sha256=sha(ASSETS/'worlds/crypt01.dact'),source_members=source_members,authoring=authoring,raw_rows=rows,AI_rows=ai_rows,scope='Static ARM source and direct XML/raw table inspection; no executed original factory or pose claim.')
(HERE/'source-inputs.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(rows=rows,static_attributes=[p['attributes'].get('static') for p in authoring])))
