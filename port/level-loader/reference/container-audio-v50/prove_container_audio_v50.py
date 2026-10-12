from pathlib import Path
import sys,struct,json,hashlib,zipfile
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');sys.path.insert(0,str(root/'port/engine-resources/tests'))
from cpu import Cpu,i32
from unicorn import UC_HOOK_CODE
b=Path(__file__).resolve().parents[2];ref=Path(__file__).parent;elf=root/'.local-inputs/libDungeonHunter2.so';cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();assert sha(elf)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
with zipfile.ZipFile(cache) as z:
 prefix='com.gameloft.android.GAND.GloftD2SS/files/data/pydata/'
 raw=z.read(prefix+'game_objects_pyarray.bin')[3031:5007];names=z.read(prefix+'game_objects_pyarraynames.bin')[1749:3614]
count=struct.unpack_from('<I',raw)[0];assert count==struct.unpack_from('<I',names)[0]==68
rows=[];at=4;np=4
for i in range(count):
 field4,sound=struct.unpack_from('<ii',raw,at);at+=8;physics=raw[at];at+=1;loot,n=struct.unpack_from('<ii',raw,at);at+=8;script=raw[at:at+n];at+=n;tail=struct.unpack_from('<iii',raw,at);at+=12
 n=struct.unpack_from('<I',names,np)[0];np+=4;name=names[np:np+n].decode();np+=n
 rows.append({'index':i,'name':name,'sound_field8':sound})
assert at==len(raw) and np==len(names)
class Oracle(Cpu):
 def text(self,p):
  out=bytearray()
  while self.uc.mem_read(p,1)!=b'\0':out+=self.uc.mem_read(p,1);p+=1
  return bytes(out)
 def external(self,uc,a,n,u):
  name=self.imports.get(a)
  if name=='strcmp':
   x,y=self.text(self.reg(0)),self.text(self.reg(1));self.put(0,0 if x==y else -1 if x<y else 1);uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,a,n,u)
c=Oracle(elf,False,{'functions':[]});actor=c.data+0x1000;vt=c.data+0x4000;table=c.data+0x10000;ptrs=c.data+0x20000;strings=c.data+0x30000;manager=c.data+0x50000;nextmanager=manager+0x1000
word=lambda p:struct.unpack('<I',c.uc.mem_read(p,4))[0]
c.pointer(c.symbols['_ZN6Arrays18OpenableContainers4sizeE'],count);c.pointer(c.symbols['_ZN6Arrays18OpenableContainers7membersE'],table);c.pointer(c.symbols['_ZN6Arrays18OpenableContainers13m_memberNamesE'],ptrs)
c.pointer(actor,vt);c.pointer(vt+0xd8,0x3a17c4)
for row in rows:
 i=row['index'];c.pointer(table+i*40+8,row['sound_field8']);s=row['name'].encode()+b'\0';p=strings+i*512;c.uc.mem_write(p,s);c.pointer(ptrs+i*4,p)
global_manager=c.symbols['_ZN15VoxSoundManager10s_instanceE'];calls=[];mutate=False
def hook(uc,a,n,u):
 if a==0x3a17c4 and mutate:c.pointer(global_manager,nextmanager)
 if a==0x3699fc:
  calls.append({'captured_manager':c.reg(0),'raw_sound_uid':i32(c.reg(1))});c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
c.uc.hook_add(UC_HOOK_CODE,hook)
cases=[]
for row in rows+ [{'index':-1,'name':'missing-container-v50','sound_field8':-1}]:
 p=strings+0x10000;c.uc.mem_write(p,row['name'].encode()+b'\0');c.pointer(actor+0x38c,p)
 actual=i32(c.invoke(0x3a17c4,[actor]));assert actual==row['sound_field8']
 for present in [False,True]:
  for mutate in ([False,True] if present else [False]):
   c.pointer(global_manager,manager if present else 0);calls.clear();c.put(4,actor);c.put(5,0x994a98);c.uc.reg_write(c.sp,c.stack+0xe000)
   c.uc.emu_start(0x39fa38,0x39fa68,count=100000)
   assert c.uc.reg_read(c.pc)==0x39fa68
   assert calls==([{'captured_manager':manager,'raw_sound_uid':row['sound_field8']}] if present else [])
   cases.append({'name':row['name'],'sound_field8':row['sound_field8'],'manager_present':present,'global_changes_during_real_GetSound':mutate,'LoadSound_observer':list(calls)})
receipt={'validation':'PASS','original_sha256':sha(elf),'cache_sha256':sha(cache),'original_GetSound_actual_rows':68,'missing_row_case':1,'container_audio_prefix_cases':len(cases),'all_raw_field8_passed_unchanged':True,'no_generated_Sounds_ordinal_conversion':True,'manager_captured_before_GetSound':True,'real_lookup_GetSound_instructions_execute':True,'observers':'Original Arrays storage populated from exact supplied group5. Imported strcmp leaf exact bytes. Original Container slice39fa38..39fa68 entered with original-established r4 actor/r5 GOT; LoadSound body is observer. Full InitPost and real audio service not executed.','rows':rows,'cases':cases,'whole_InitPost_verified':False,'audio_backend_verified':False}
(b/'reports/container-audio-units-v50-original.json').write_text(json.dumps(receipt,indent=2)+'\n')
print(json.dumps({k:v for k,v in receipt.items() if k not in ['rows','cases']}))
