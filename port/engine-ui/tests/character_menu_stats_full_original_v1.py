"""Whole original four IncStat bodies and base reload/class/224 resolution.
Only actual caller tables, empty buff containers and Debug I/O are supplied;
original property reads/writes/recalc/class traversal execute unmocked ARM32.
"""
import json,struct,sys,hashlib
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
sys.path.insert(0,str(ROOT/'port/game-data/tools'))
from character_menu_native_v1_original import Original,words
from inspect_class_tables import parse
m=Original();m.full_stats=True
c=m.c;uc=c.uc;owner=m.character+0x560;c.pointer(owner+4,m.character)
data=ROOT/'port/android-native/app/src/main/assets/data'
raw=(data/'character_properties_pyarray.bin').read_bytes();count=struct.unpack_from('<I',raw)[0]
default=list(struct.unpack_from('<224i',raw,4));types=list(struct.unpack_from('<224i',raw,900))
rules=json.loads((ROOT/'port/game-data/reference/properties/property-rules.json').read_text())
rows=c.data+0x400000
for i in range(count):uc.mem_write(rows+i*900,bytes(4)+raw[4+i*896:4+(i+1)*896])
c.pointer(int(rules['character_array_global'],16),rows)
got=(0x3df260+m.word(0x3df298))&0xffffffff;c.pointer(m.word(got+m.word(0x3df29c)),count)
classes=parse(data)['rows'];cr=c.data+0x600000;cp=c.data+0x640000
got=(0x3e2e34+m.word(0x3e3008))&0xffffffff
c.pointer(m.word(got+m.word(0x3e300c)),len(classes));c.pointer(m.word(got+m.word(0x3e3010)),cr)
for i,row in enumerate(classes):
 entries=row['entries'];uc.mem_write(cr+i*12,words(0,len(entries),cp))
 for f in entries:uc.mem_write(cp,struct.pack('<i5i',0,*f));cp+=24
sheets=[owner+x for x in (8,0x38c,0x710,0xa94)]
sentinel=owner+0xe18;uc.mem_write(sentinel,words(0,0,sentinel,sentinel));c.pointer(owner+0xe28,0)
def read_state():return b''.join(m.read(p+4,896) for p in sheets)
cases=[]
for actor in (263,290,325):
 uc.mem_write(m.character+0x13c8,struct.pack('<h',actor))
 for stat,label in enumerate(('Str','Dex','End','Nrg')):
  for points in (-1,0,1,2,8388607):
   for sheet in sheets:uc.mem_write(sheet,bytes(4)+struct.pack('<224i',*default))
   m.active=True
   try:
    c.invoke(0x3e087c,[owner,actor]);c.invoke(0x3e0808,[owner,148,points]);initial=read_state()
    m.stat_entry=m.entries['_ZN9Character10IncStat'+label+'Ev'];m.services=[]
    c.invoke(m.stat_entry,[m.character]);final=read_state()
    assert [s[0] for s in m.services]==['_ZN13DebugSwitches4loadEv','_ZN13DebugSwitches9GetSwitchERKSs'],m.services
   finally:m.active=False
   cases.append(words(actor,stat,points)+initial+final)
   if len(cases)%10==0:print(json.dumps(dict(original_completed_cases=len(cases))),flush=True)
blob=words(0x31534d43,len(cases))+b''.join(cases)
output=ROOT/'port/engine-ui/reference/character-menu-native-v1/stat-full-gold-v1.bin'
output.write_bytes(blob)
print(json.dumps(dict(validation='PASS',whole_original_cases=len(cases),gold_sha256=hashlib.sha256(blob).hexdigest(),property_class_and_base_reload_unmocked=True)))
