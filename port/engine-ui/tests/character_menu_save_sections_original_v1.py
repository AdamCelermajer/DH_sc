"""Original named save callbacks plus actual scalar/string stream wrappers.
Only the terminal IStreamBase virtual write is a byte sink boundary."""
import sys, json, struct
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
from character_menu_native_v1_original import Original, ROOT

class Sections(Original):
 def hook(self, uc, address, size, userdata):
  if self.active and address == 0x439cb4:
   assert self.c.reg(0) == self.stream
   amount = self.c.reg(2); assert self.c.reg(3) == 0 and amount <= 4096
   self.writes.append(bytes(uc.mem_read(self.c.reg(1), amount)))
   self.ret(amount); return
  super().hook(uc, address, size, userdata)

m=Sections(); saved=m.alloc(512); m.stream=m.alloc(32); vtable=m.alloc(128)
m.c.pointer(m.stream,vtable); m.c.pointer(vtable+0x1c,0x439cb4)
cases=[]
cache_names=(ROOT/'port/android-native/app/src/main/assets/data/skills_pyarraynames.bin').read_bytes();offset=0
def cache_word():
 global offset
 value=struct.unpack_from('<I',cache_names,offset)[0];offset+=4;return value
def cache_name_list():
 global offset
 rows=[]
 for _ in range(cache_word()):
  size=cache_word();rows.append(cache_names[offset:offset+size]);offset+=size
 return rows
list_names=cache_name_list();skill_names=cache_name_list()
assert skill_names and all(b'\0' not in name for name in skill_names)
got=(0x469e84+m.word(0x46a094))&0xffffffff
name_global=m.alloc(4);name_array=m.alloc(len(skill_names)*4)
for row,name in enumerate(skill_names):
 pointer=m.alloc(len(name)+1);m.c.uc.mem_write(pointer,name+b'\0');m.c.pointer(name_array+row*4,pointer)
m.c.pointer(name_global,name_array);m.c.pointer(got+m.word(0x46a09c),name_global)
canary=m.alloc(4);m.c.pointer(canary,0x1337);m.c.pointer(got+m.word(0x46a098),canary)
def run(section,address,expected,inputs):
 m.writes=[];m.active=True
 try:m.c.invoke(address,[m.stream,saved],budget=200000)
 finally:m.active=False
 actual=b''.join(m.writes);assert actual==expected,(section,inputs,actual.hex(),expected.hex())
 cases.append(dict(section=section,inputs=inputs,writes=[x.hex() for x in m.writes],bytes=actual.hex()))
for name in ('','Knight','MagePlayerBase','RoguePlayerBase'):
 raw=name.encode()+b'\0';p=m.alloc(len(raw));m.c.uc.mem_write(p,raw)
 m.c.pointer(saved+0x28,p+len(raw)-1);m.c.pointer(saved+0x2c,p)
 run('PNAM',0x4688c0,struct.pack('<i',len(raw))+raw,dict(name=name))
for level in (-1,0,1,50,2147483647):
 m.c.pointer(saved+0x30,level&0xffffffff)
 run('PLVL',0x468930,struct.pack('<i',level),dict(level=level))
faeries=m.alloc(64)
for tier in range(3):m.c.pointer(saved+0x94+tier*4,faeries);m.c.pointer(saved+0xa0+tier*4,5)
for current in ((0,0,0),(0,1,4),(4,3,2),(-1,0,1)):
 for tier,value in enumerate(current):m.c.pointer(saved+0xac+tier*4,value&0xffffffff)
 run('CFEE',0x468bf0,struct.pack('<iii',*current),dict(current=current))
 expected=b''
 for tier,value in enumerate(current):
  pointer=m.alloc(20);m.c.pointer(saved+0x94+tier*4,pointer)
  expected+=struct.pack('<iI',value,5)
  for row in range(5):
   level=tier*100+row;state=(tier+row)%2
   m.c.uc.mem_write(pointer+row*4,struct.pack('<BBH',state,0,level))
   expected+=struct.pack('<HB',level,state)
 run('FAES',0x468f18,expected,dict(current=current))
maps=m.alloc(48)
for set_index in range(2):
 header=maps+set_index*24;m.c.pointer(header+8,header);m.c.pointer(header+12,header);m.c.pointer(header+16,0)
m.c.pointer(saved+0x88,maps)
for rows in ([(0,0)],[(0,1),(1,10)],[(5,65535),(20,32768),(len(skill_names)-1,1)]):
 skills=m.alloc(len(rows)*8);m.c.pointer(saved+0x80,skills);m.c.pointer(saved+0x84,len(rows));expected=struct.pack('<i',len(rows))
 for index,(skill,level) in enumerate(rows):
  m.c.uc.mem_write(skills+index*8,struct.pack('<I H BB',skill,level,0,0));name=skill_names[skill]+b'\0';expected+=struct.pack('<i',len(name))+name+struct.pack('<H',level)
 expected+=struct.pack('<II',0,0)
 run('SKIL',0x469e6c,expected,dict(rows=rows))
(ROOT/'port/engine-ui/reference/character-menu-native-v1/save-sections-gold-v1.json').write_text(json.dumps(dict(boundary_fixtures=['terminal IStreamBase.write byte sink'],cases=cases),indent=2)+'\n')
binary=struct.pack('<I',len(cases))
for case in cases:
 raw=bytes.fromhex(case['bytes']);binary+=case['section'].encode()+struct.pack('<I',len(raw))+raw
(ROOT/'port/engine-ui/reference/character-menu-native-v1/save-sections-gold-v1.bin').write_bytes(binary)
print(json.dumps(dict(validation='PASS',original_named_sections=len(cases),actual_scalar_and_string_wrappers=True,file_persistence_unproved=True)))
