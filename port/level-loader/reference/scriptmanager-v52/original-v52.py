from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages');sys.path.insert(0,r'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
from items_differential import Original as Base
from unicorn import UC_HOOK_CODE
class Original(Base):
 def storage(self,*args):pass
 def external(self,uc,at,size,unused):
  name=self.imports.get(at)
  if name=='strlen':self.returned(len(cstring(self.reg(0)).encode()))
  elif name in ('malloc','_Znwj'):
   n=self.reg(0);p=self.heap;self.heap+=(max(n,4)+15)&~15;uc.mem_write(p,b'\xa5'*max(n,4));allocations[p]=n;self.returned(p)
  elif name in ('free','_ZdlPv','pthread_mutex_lock','pthread_mutex_unlock'):self.returned(0)
  else:super().external(uc,at,size,unused)
d=Path(__file__).parent;primitive_helpers={int(k):v for k,v in json.loads((d/'primitive-helpers.json').read_text()).items()};elf=Path(r'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');c=Original(elf,{'functions':[]});manager=c.data+0x4000;filename=c.data+0x5000;stream=c.data+0x6000;allocations={};raw=b'';cursor=0;mode='';dataobj=0;data_start=0;records=[];read_events=[];pending=None;reads=[];boundaries=[];stop_factory=False;factory_addresses={int(x['command_factory'],16) for x in json.loads((d/'factory-table.json').read_text())['entries']}
def ret(v=0):c.returned(v)
def cstring(at):
 b=bytearray()
 while c.uc.mem_read(at+len(b),1)!=b'\0':b.extend(c.uc.mem_read(at+len(b),1))
 return bytes(b).decode()
def take(n,advance=True):
 global cursor
 assert 0<=n<=len(raw)-cursor,(mode,cursor,n,len(raw),hex(c.uc.reg_read(c.pc)))
 b=raw[cursor:cursor+n]
 if advance:cursor+=n
 return b
def hook(uc,at,size,unused):
 global dataobj,data_start,pending
 if stop_factory and at in factory_addresses:
  uc.reg_write(c.sp,c.stack+0xe000);uc.reg_write(c.pc,c.stop)
 elif at in (0x310570,0x31056c):
  n=c.reg(0);assert n<1000000,n;p=c.heap;c.heap+=(max(n,4)+15)&~15;uc.mem_write(p,b'\xa5'*max(n,4));allocations[p]=n;ret(p)
 elif at==0x310440:ret()
 elif mode in ('commands','names'):
  if at in (0x45b2c0,0x45ad9c):c.put(0,stream);uc.reg_write(c.pc,0x45b2d8 if mode=='commands' else 0x45adb4)
  elif at in (0x45b2f4,0x45add0):
   c.put(5 if mode=='commands' else 4,c.uc.reg_read(c.sp)+(0x70 if mode=='commands' else 0x48));uc.reg_write(c.pc,0x45b314 if mode=='commands' else 0x45adf0)
  elif at in (0x3172d8,0x3169c4):ret(c.reg(0))
  elif at==0x45b50c:
   dataobj=c.reg(0);data_start=cursor;reads.clear();pending={'kind':struct.unpack_from('<I',raw,cursor)[0],'begin':cursor,'data_identity':dataobj,'data_size':allocations[dataobj],'data_reader':hex(c.word(c.word(dataobj)+12)),'data_C1_hex':bytes(uc.mem_read(dataobj,allocations[dataobj])).hex()}
  elif at==0x45b520:
   pending['end']=cursor;pending['reads']=list(reads);pending['data_hex']=bytes(uc.mem_read(dataobj,pending['data_size'])).hex();records.append(pending.copy());dataobj=0
  elif at==0x45b550:
   command=c.reg(0);assert c.word(command+8)==pending['kind'] and c.word(command+12)==pending['data_identity'];records[-1]['command_init']=hex(c.word(c.word(command)));records[-1]['command_identity']=command;records[-1]['command_size']=allocations[command];records[-1]['command_c1_hex']=bytes(uc.mem_read(command,allocations[command])).hex();uc.reg_write(c.pc,0x45b55c) # required actual Init is OBSERVATION FIXTURE, not executed.
  elif at in (0x459090,0x3df1a0):
   dest=c.reg(1);b=take(4);uc.mem_write(dest,b)
   if dataobj:reads.append({'helper':hex(at),'offset':dest-dataobj if dataobj<=dest<dataobj+allocations[dataobj] else None,'width':4,'raw':b.hex(),'destination':hex(dest)})
   ret()
  elif at in primitive_helpers:
   n=primitive_helpers[at]['width'];dest=c.reg(1);b=take(n);uc.mem_write(dest,b)
   if dataobj:reads.append({'helper':hex(at),'offset':dest-dataobj if dataobj<=dest<dataobj+allocations[dataobj] else None,'width':n,'raw':b.hex(),'destination':hex(dest)})
   ret()
  elif at in primitive_helpers:
   n=primitive_helpers[at]['width'];dest=c.reg(1);b=take(n);uc.mem_write(dest,b)
   if dataobj:reads.append({'helper':hex(at),'offset':dest-dataobj if dataobj<=dest<dataobj+allocations[dataobj] else None,'width':n,'raw':b.hex(),'destination':hex(dest)})
   ret()
  elif at==0x459140:uc.mem_write(c.reg(1),take(4,False));ret()
  elif at in (0x3364ec,0x313a90):
   b=take(4)
   if dataobj:reads.append({'helper':hex(at),'offset':None,'width':4,'raw':b.hex(),'destination':None})
   ret(struct.unpack('<I',b)[0])
  elif at==0x316494:
   n=c.reg(2);b=take(n);uc.mem_write(c.reg(1),b)
   if dataobj:reads.append({'helper':hex(at),'offset':c.reg(1)-dataobj if dataobj<=c.reg(1)<dataobj+allocations[dataobj] else None,'width':n,'raw':b.hex(),'destination':hex(c.reg(1))})
   ret(n)
  elif at==0x317454:
   n=c.reg(2);b=take(n);uc.mem_write(c.reg(1),b)
   if dataobj:reads.append({'helper':hex(at),'offset':None,'width':n,'raw':b.hex(),'destination':hex(c.reg(1))})
   ret()
def invoke(fn,args):
 try:c.invoke(fn,args,budget=5000000)
 except Exception:
  print(json.dumps({'pc':hex(c.uc.reg_read(c.pc)),'lr':hex(c.uc.reg_read(c.lr)),'cursor':cursor,'registers':[hex(c.reg(i)) for i in range(8)],'raw_peek':raw[max(0,cursor-16):cursor+16].hex(),'pending':pending,'records':records[-2:]}),flush=True);raise
c.uc.hook_add(UC_HOOK_CODE,hook)
c.uc.mem_write(manager,b'\xa5'*52);c.invoke(0x455980,[manager,0,0,0]);initial=bytes(c.uc.mem_read(manager,52)).hex();loads=[]
for stem,common in [('scripts',1),('001_swamp',0)]:
 for part,fn in [('pyscripts',0x45b264),('pyscriptnames',0x45ad40)]:
  path=d/(stem+'_'+part+'.bin');raw=path.read_bytes();cursor=0;records.clear();dataobj=0;mode='commands' if part=='pyscripts' else 'names';c.uc.mem_write(filename,(path.name+'\0').encode());invoke(fn,[manager,filename,common,0]);mode='';assert cursor==len(raw),(path,cursor,len(raw));loads.append({'asset':path.name,'common':common,'bytes_consumed':cursor,'records':list(records),'common8':c.word(manager+8),'command_lists':(c.word(manager+0x1c)-c.word(manager+0x18))//12,'contexts':(c.word(manager+0x10)-c.word(manager+0xc))//12,'names':(c.word(manager+0x28)-c.word(manager+0x24))//4})
  print(json.dumps({'asset':path.name,'cursor':cursor,'records':len(records),'kinds':sorted(set(x['kind'] for x in records))}),flush=True)
names=[cstring(c.word(c.word(manager+0x24)+4*i)) for i in range(70)];contexts=[[c.word(c.word(manager+0xc)+12*i+j) for j in (0,4,8)] for i in range(70)];assert all(x==[0xffffffff,0,2] for x in contexts),contexts
c.pointer(manager+4,0x12345678);c.uc.mem_write(manager+0x30,b'\x77');c.invoke(0x45a1c8,[manager,0,0,0],budget=5000000);unloaded={'common8':c.word(manager+8),'current0':c.word(manager),'field4':c.word(manager+4),'byte30':c.uc.mem_read(manager+0x30,1)[0],'commands':(c.word(manager+0x1c)-c.word(manager+0x18))//12,'contexts':(c.word(manager+0x10)-c.word(manager+0xc))//12,'names':(c.word(manager+0x28)-c.word(manager+0x24))//4};assert unloaded=={'common8':0,'current0':0xffffffff,'field4':0x12345678,'byte30':0x77,'commands':0,'contexts':0,'names':0},unloaded
c.invoke(0x455980,[manager,0,0,0]);raw=(d/'scripts_pyscripts.bin').read_bytes();cursor=0;mode='commands';stop_factory=True;invoke(0x45b264,[manager,filename,1,0]);mode='';stop_factory=False
required_prefix={'cursor':cursor,'common8':c.word(manager+8),'command_lists':(c.word(manager+0x1c)-c.word(manager+0x18))//12,'contexts':(c.word(manager+0x10)-c.word(manager+0xc))//12,'first_count0':c.word(c.word(manager+0x18)),'array_allocated':bool(c.word(c.word(manager+0x18)+8)),'first_command_factory_not_executed':True}
assert required_prefix=={'cursor':8,'common8':15,'command_lists':15,'contexts':15,'first_count0':4,'array_allocated':True,'first_command_factory_not_executed':True},required_prefix
report={'validation':'PASS','required_command_factory_stop_prefix':required_prefix,'original_whole_unload_storage':unloaded,'engine_sha256':hashlib.sha256(elf.read_bytes()).hexdigest(),'manager_C1_hex':initial,'loads':loads,'names':names,'contexts':contexts,'actual_Command_Init_not_executed':True,'cache_stream_services_are_fixtures':True,'whole_command_execution_verified':False}
(d/'original-v52.json').write_bytes((json.dumps(report,indent=2)+'\n').encode());print(json.dumps({'validation':'PASS','scripts':70,'commands':sum(len(x['records']) for x in loads),'kinds':sorted(set(v['kind'] for x in loads for v in x['records']))}))
