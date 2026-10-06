"""Execute exact GSLevel.Update386630 envelope. Level.Update/Debug/Menu are
explicit callback fixtures; original GS branches, Level.Load3instructions,
real GOT global stores, strings, menu gate and same field34 rereads execute ARM.
No whole Level loading, GameSM state selection or online body is certified.
"""
from pathlib import Path
import sys,itertools,json,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
sys.path.insert(0,r'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
from items_differential import Original as Base
from unicorn import UC_HOOK_CODE
class Original(Base):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='strlen':
   p=self.reg(0);n=0
   while uc.mem_read(p+n,1)!=b'\0':n+=1
   self.returned(n)
  elif name in ('malloc','_Znwj'):
   n=self.reg(0);p=self.heap;self.heap+=(n+15)&~15;assert self.heap<self.data+0x2000000
   if n:uc.mem_write(p,bytes(n))
   self.returned(p)
  elif name in ('free','_ZdlPv','pthread_mutex_lock','pthread_mutex_unlock'):self.returned(0)
  else:super().external(uc,address,size,unused)
root=Path(__file__).resolve().parents[2];elf=Path(r'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so')
c=Original(elf,{'functions':[]});gs=c.data+0x4000;level=c.data+0x5000;other=c.data+0x6000;mode=False;events=[];fact={};debug_keys=set()
got=0x386648+c.word(0x3867fc);app=c.word(got+c.word(0x386800));flag=c.word(got+c.word(0x38680c));word=c.word(got+c.word(0x386814));debug=c.word(got+c.word(0x386808))
def ret(value=0):c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def string(at):
 begin,end=c.word(at+0x14),c.word(at+0x10);return bytes(c.uc.mem_read(begin,end-begin)).decode()
def hook(uc,at,size,unused):
 if not mode:return
 if at==0x337888:events.append([1,c.word(word),uc.mem_read(flag,1)[0],0]);ret()
 elif at==0x337a88:debug_keys.add(string(c.reg(1)));events.append([2,fact['debug'],0,0]);ret(fact['debug'])
 elif at==0x337ddc:debug_keys.add(string(c.reg(1)));events.append([3,c.reg(2),0,0]);ret()
 elif at==0x3ef218:events.append([4,1,0,0]) # actual Level.Load stores130=0.
 elif at==0x3f82d8:
  assert c.reg(0)==level and c.reg(1)==0;events.append([5,1,c.reg(1),0]);c.pointer(level+0x130,fact['update'])
  if fact['mutation']:c.pointer(gs+0x34,other)
  ret()
 elif at==0x42ca8c:events.append([6,0,0,0]);ret(c.data+0x7000)
 elif at==0x42ea04:events.append([7,c.reg(1),0,0]);ret()
c.uc.hook_add(UC_HOOK_CODE,hook);cases=[]
inputs=[]
for loading,paused,present,byte145,phase,enabled in itertools.product((0,1,2,3,4,5,0xffffffff),(0,1),(0,1),(0,1),(0,1,2,26,27,37,38,39,0x80000000,0xffffffff),(0,1)):
 if not paused and not present and loading in (2,3,4):continue # Original unguarded NULL deref domain.
 inputs.append([loading,paused,present,byte145,phase,enabled,phase,0,38])
for loading in (3,4):
 for altphase in (0,2,26,27,38,0xffffffff):inputs.append([loading,0,1,0,0,1,37,1,altphase])
for inp in inputs:
 loading,paused,present,byte145,phase,enabled,updated,mutation,altphase=inp
 c.uc.mem_write(gs,bytes(0x50));c.uc.mem_write(level,bytes(0x200));c.uc.mem_write(other,bytes(0x200));c.pointer(gs+0x38,loading);c.pointer(gs+0x34,level if present else 0);c.uc.mem_write(gs+0x3c,b'\x7f')
 c.pointer(level+0x130,phase);c.pointer(other+0x130,altphase);c.uc.mem_write(level+0x145,bytes((byte145,)));c.uc.mem_write(other+0x145,bytes((byte145,)));c.uc.mem_write(app+0xec,bytes((paused,)));c.pointer(word,0x13579bdf);c.uc.mem_write(flag,b'\x7e')
 fact={'debug':enabled,'update':updated,'mutation':mutation};events.clear();mode=True;c.invoke(0x386630,[gs,c.data+0x8000,0,0],budget=100000);mode=False
 out=[c.word(gs+0x38),c.word(level+0x130),c.word(other+0x130),c.word(word),c.uc.mem_read(flag,1)[0],c.uc.mem_read(gs+0x3c,1)[0],1 if c.word(gs+0x34)==level else 2 if c.word(gs+0x34)==other else 0]
 cases.append({'input':inp,'output':out,'events':list(events)})
assert len(debug_keys)==1,debug_keys
key=next(iter(debug_keys));assert len(key)==13,key
report={'validation':'PASS','entry':'0x386630','engine_sha256':hashlib.sha256(elf.read_bytes()).hexdigest(),'original_cases':len(cases),'debug_key':key,'got':got,'actual_application':app,'original_debug_flag_global':flag,'original_debug_word_global':word,'callbacks_are_fixtures':True,'whole_level_init_verified':False,'scope':__doc__,'cases':cases}
(Path(__file__).parent/'gslevel-update-v44-original.json').write_bytes((json.dumps(report,indent=2)+'\n').encode())
rows=[str(len(cases))]
for case in cases:
 rows.append(' '.join(map(str,case['input']+case['output']+[len(case['events'])]+[n for event in case['events'] for n in event])))
(Path(__file__).parent/'gslevel-update-v44-gold.txt').write_bytes(('\n'.join(rows)+'\n').encode())
print(json.dumps({k:v for k,v in report.items() if k not in ('cases','scope')}))
