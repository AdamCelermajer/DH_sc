"""Original _LoadProcess state7 ordering and filename/seed branches.
External DebugSwitches, GetOnline, StreamBuffer lifetime, GenerateRandomLevel,
AssignSteam and LoadFile are explicit observers/results. Original branches,
global writes, string operations, counters and ordering execute ARM instructions.
This does not verify actual generator XML output, asset parsing or class effects.
"""
from pathlib import Path
import sys,json,hashlib,itertools
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages');sys.path.insert(0,r'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
from items_differential import Original as Base
class Original(Base):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='strlen':
   start=self.reg(0);length=0
   while uc.mem_read(start+length,1)!=b'\0':length+=1
   self.returned(length)
  elif self.imports.get(address) in ('pthread_mutex_lock','pthread_mutex_unlock'):self.returned(0) # Serialized caller fixture.
  elif self.imports.get(address) in ('malloc','_Znwj'):
   count=self.reg(0);pointer=self.heap;self.heap+=(count+15)&~15;assert self.heap<self.data+0x2000000
   if count:uc.mem_write(pointer,bytes(count))
   self.returned(pointer)
  elif self.imports.get(address)=='__aeabi_uidiv':
   assert self.reg(1)!=0;self.returned(self.reg(0)//self.reg(1))
  elif self.imports.get(address) in ('free','_ZdlPv'):self.returned()
  else:super().external(uc,address,size,unused)
from unicorn import UC_HOOK_CODE
root=Path(__file__).resolve().parents[2];engine=Path(r'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');c=Original(engine,{'functions':[]});level=c.data+0x4000;online=c.data+0x6000;name=c.data+0x7000;events=[];facts={};mode=False;calls=0
got=0x3f69a8+c.word(0x3f7920);global_dc=c.word(got+c.word(0x3f7978));global_e0=c.word(got+c.word(0x3f797c))
def ret(n=0):c.put(0,n);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def read_string(at):
 begin,end=c.word(at+0x14),c.word(at+0x10);assert begin<=end;return bytes(c.uc.mem_read(begin,end-begin)).decode()
def hook(uc,address,size,unused):
 global calls
 if not mode:return
 if address==0x3f6e9c:uc.reg_write(c.pc,c.stop);uc.emu_stop()
 elif address==0x330740:ret(c.data+0x9000)
 elif address==0x337a88:ret(0)
 elif address==0x7fd794:
  events.append({'kind':'online','global_dc':c.word(global_dc),'global_e0':c.word(global_e0)});ret(online)
 elif address==0x316d3c:events.append({'kind':'stream_construct'});ret(c.reg(0))
 elif address==0x3169c4:events.append({'kind':'stream_destroy'});ret()
 elif address==0x3f07f8:
  assert c.reg(0)==level;events.append({'kind':'generate','seed':c.reg(2)});ret(facts['generated'])
 elif address==0x3f02ac:assert c.reg(0)==level;events.append({'kind':'assign_stream'});ret()
 elif address==0x3f3b40:
  assert c.reg(0)==level;calls+=1;events.append({'kind':'load_file','name':read_string(c.reg(1)),'root':read_string(c.reg(2)),'counter138':c.word(level+0x138),'call':calls});ret(int(calls==3))
c.uc.hook_add(UC_HOOK_CODE,hook);cases=[]
for filename,procedural,connected,generated in itertools.product(('001_swamp.mlx','x005_crypt.rnd','nodot','folder/x05_area.mlx','abc.part.mlx'),(0,1),(0,1),(0,1)):
 c.uc.mem_write(level,bytes(0x200));b=filename.encode()+b'\0';c.uc.mem_write(name,b);c.invoke(0x3140ec,[level+0xf8,name,c.data+0x8000])
 c.pointer(level+0x130,7);c.pointer(level+0xdc,77);c.pointer(level+0xe0,99);c.uc.mem_write(level+0xe8,bytes((procedural,)));c.pointer(level+0x138,123);c.pointer(level+0x13c,5);c.uc.mem_write(online+5,bytes((connected,)))
 facts={'generated':generated};events.clear();calls=0;mode=True;c.put(4,level);c.put(5,got);c.invoke(0x3f7204,[],budget=100000);mode=False
 assert c.word(global_dc)==77 and c.word(global_e0)==99;assert c.word(level+0x130)==8 and c.word(level+0x13c)==6 and c.word(level+0x138)==500
 expected_name=filename
 if procedural and not generated:
  expected_name='x'+filename[1:];dot=expected_name.find('.')
  if dot!=-1:expected_name=expected_name[:dot]+'_BACKUP.mlx'
 expected_prefix=[] if not procedural else [{'kind':'online','global_dc':77,'global_e0':99},{'kind':'stream_construct'},{'kind':'generate','seed':99 if connected else 77}]+([{'kind':'assign_stream'}] if generated else[])+[{'kind':'stream_destroy'}]
 assert events[:len(expected_prefix)]==expected_prefix,(filename,procedural,connected,generated,events)
 assert events[len(expected_prefix):]==[{'kind':'load_file','name':expected_name,'root':'Level','counter138':500,'call':i} for i in range(1,4)]
 cases.append({'filename':filename,'procedural':procedural,'online':connected,'generated_fixture':generated,'final_name':read_string(level+0xf8),'events':list(events)})
report={'validation':'PASS','original_sha256':hashlib.sha256(engine.read_bytes()).hexdigest(),'original_stage7_cases':len(cases),'source_ordering_verified':True,'cases':cases,'scope':__doc__,'raw_filename_backup_note':'Original mutates first CString character, then truncates at first dot/appends _BACKUP.mlx only when a dot exists. Native source-only helper modifies resource basename and appends even without a dot: separate adapter, not identical full stage7 domain.','whole_loader_verified':False}
(Path(__file__).parent/'stage-loader-v38-stage7-original.json').write_bytes((json.dumps(report,indent=2)+'\n').encode());print(json.dumps({k:v for k,v in report.items() if k not in ('cases','scope')}))
