from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages');sys.path.insert(0,r'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
from items_differential import Original as Base
from unicorn import UC_HOOK_CODE
class Original(Base):
 def storage(self,uc,address,size,unused):pass # Replace inherited allocator hook; one fixture allocation per reached call.
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='strlen':
   p=self.reg(0);n=0
   while uc.mem_read(p+n,1)!=b'\0':n+=1
   self.returned(n)
  elif name in ('malloc','_Znwj'):
   n=self.reg(0);p=self.heap;self.heap+=(n+15)&~15;uc.mem_write(p,b'\xa5'*max(n,4));self.returned(p)
  elif name in ('free','_ZdlPv'):self.returned(0)
  else:super().external(uc,address,size,unused)
d=Path(__file__).parent;elf=Path(r'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');c=Original(elf,{'functions':[]});event=c.data+0x4000;stub=c.data+0x5000;row=c.data+0x6000;stream=c.data+0x7000;mode='';allocations=[];raw=b'';cursor=0
def ret(value=0):c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def take(n):
 global cursor
 assert 0<=n<=len(raw)-cursor,(cursor,n,len(raw));v=raw[cursor:cursor+n];cursor+=n;return v
def hook(uc,at,size,unused):
 if at in (0x310570,0x31056c):
  n=c.reg(0);hint=c.reg(1);assert n<1024*1024,(mode,cursor,hex(at),hex(n),hex(c.uc.reg_read(c.lr)));p=c.heap;c.heap+=(max(n,4)+15)&~15;uc.mem_write(p,b'\xa5'*max(n,4));allocations.append({'identity':p,'size':n,'hint':hint});ret(p)
 elif at==0x310440:ret()
 elif mode=='read' and at in (0x3364ec,0x313a90):assert c.reg(0)==stream;ret(struct.unpack('<I',take(4))[0])
 elif mode=='read' and at in (0x459090,0x3df1a0):assert c.reg(0)==stream;uc.mem_write(c.reg(1),take(4));ret()
 elif mode=='read' and at==0x317454:assert c.reg(0)==stream and c.reg(3)==0;uc.mem_write(c.reg(1),take(c.reg(2)));ret()
c.uc.hook_add(UC_HOOK_CODE,hook)
types=[]
for kind in range(11):
 c.uc.mem_write(stub,bytes(44));c.pointer(stub+4,kind);allocations.clear();mode='ctor';obj=c.invoke(0x47a3e8,[stub,0,0,0]);mode='';a=next(v for v in allocations if v['identity']==obj);size=a['size'];before=bytes(c.uc.mem_read(obj,size));vtable=c.word(obj);reset=c.word(vtable+0x30);c.uc.mem_write(obj+0x14,b'\x7f')
 if size>32:c.pointer(obj+0x20,0x12345678)
 c.invoke(reset,[obj,0,0,0]);after=bytes(c.uc.mem_read(obj,size));types.append({'type':kind,'size':size,'vtable':hex(vtable),'reset':hex(reset),'constructed_hex':before.hex(),'reset_hex':after.hex(),'allocations':list(allocations)})
(d/'objectives-original-v50.json').write_bytes((json.dumps(types,indent=2)+'\n').encode())
raw=(d/'v2eventmanager_pyarray.bin').read_bytes();cursor=4;count=struct.unpack_from('<I',raw)[0];rows=[];mode='read'
for i in range(count):
 c.uc.mem_write(row,bytes(24));start=cursor;c.invoke(0x4ebc38,[row,stream,0,0],budget=100000);n=c.word(row+8);script=bytes(c.uc.mem_read(c.word(row+0xc),n)).decode();nobj=c.word(row+0x10);stubs=[]
 for j in range(nobj):
  p=c.word(row+0x14)+j*44;words=[c.word(p+off) for off in (4,8,12,32,36,40)];s1=bytes(c.uc.mem_read(c.word(p+0x14),c.word(p+0x10))).decode();s2=bytes(c.uc.mem_read(c.word(p+0x1c),c.word(p+0x18))).decode();stubs.append({'words':words,'string10':s1,'string18':s2})
 rows.append({'index':i,'field4':c.word(row+4),'script':script,'objectives':stubs,'begin':start,'end':cursor})
mode='';assert cursor==len(raw),(cursor,len(raw))
got=0x479e80+c.word(0x479f24);slots={k:c.word(got+c.word(a)) for k,a in [('size',0x479f28),('data',0x479f2c),('names',0x479f30)]}
for p in slots.values():c.pointer(p,0)
raw=(d/'v2eventmanager_pyarray.bin').read_bytes();cursor=0;mode='read';c.invoke(0x4b8c9c,[stream,0,0,0],budget=1000000);assert cursor==len(raw) and c.word(slots['size'])==count,(cursor,len(raw),c.word(slots['size']))
raw=(d/'v2eventmanager_pyarraynames.bin').read_bytes();cursor=0;c.invoke(0x4b1460,[stream,0,0,0],budget=1000000);mode='';assert cursor==len(raw),(cursor,len(raw))
def cstring(at):
 n=0
 while c.uc.mem_read(at+n,1)!=b'\0':n+=1
 return bytes(c.uc.mem_read(at,n)).decode()
names=[cstring(c.word(c.word(slots['names'])+i*4)) for i in range(count)];manager=c.data+0x9000;c.uc.mem_write(manager,b'\xa5'*12);c.invoke(0x479d08,[manager,0,0,0]);manager_c1=[c.word(manager+i*4) for i in range(3)];assert manager_c1==[0,0,0],manager_c1
c.invoke(0x479e5c,[manager,0,0,0],budget=1000000);assert (c.word(manager+4)-c.word(manager))//4==count
events=[]
for i,want in enumerate(rows):
 p=c.word(c.word(manager)+i*4);assert c.word(p)==0 and c.word(p+4)==i and cstring(c.word(p+8))==names[i] and c.uc.mem_read(p+0x18,1)==b'\0';data=c.word(slots['data'])+24*i;assert c.word(p+0x1c)==data and c.word(p+0xc)==len(want['objectives']) and c.word(p+0x14)==c.word(data+0x14)
 objectives=[]
 for j in range(c.word(p+0xc)):
  o=c.word(c.word(p+0x10)+j*4);kind=c.word(o+4);assert kind==want['objectives'][j]['words'][0] and c.word(o+0xc)==c.word(p+0x14)+44*j and c.uc.mem_read(o+0x14,1)==b'\0';objectives.append({'type':kind,'size':types[kind]['size'],'defined_words':{hex(off):c.word(o+off) for off in (4,0xc,0x10,0x20,0x24,0x28,0x2c) if off<types[kind]['size']},'source_reset_complete':True})
 events.append({'id':i,'name':names[i],'state0':c.word(p),'registered18':c.uc.mem_read(p+0x18,1)[0],'objectives':objectives})
saved=bytes(c.uc.mem_read(manager,12));c.invoke(0x479e5c,[manager,0,0,0]);assert saved==bytes(c.uc.mem_read(manager,12))
report={'validation':'PASS','original_engine_sha256':hashlib.sha256(elf.read_bytes()).hexdigest(),'original_objective_constructor_reset_types':len(types),'original_event_reader_rows':len(rows),'original_reader_bytes':cursor,'original_manager_c1':manager_c1,'original_whole_manager_load_events':len(events),'original_nested_objectives':sum(len(v['objectives']) for v in events),'observed_objective_types':sorted(set(s['words'][0] for v in rows for s in v['objectives'])),'original_nonempty_load_noop':True,'stream_delivery_is_fixture':True,'functional_compile_register_effects_verified':False,'types':types,'rows':rows,'events':events}
(d/'original-v50.json').write_bytes((json.dumps(report,indent=2)+'\n').encode())
parts=[struct.pack('<II',0x30564547,len(rows))]
def word(v):return struct.pack('<I',v&0xffffffff)
def text(s):b=s.encode();return word(len(b))+b
for row,event in zip(rows,events):
 parts.append(word(row['field4'])+text(event['name'])+text(row['script'])+word(len(row['objectives'])))
 for stub in row['objectives']:parts.append(b''.join(word(v) for v in stub['words'])+text(stub['string10'])+text(stub['string18']))
(d/'original-v50-gold.bin').write_bytes(b''.join(parts))
print(json.dumps({k:v for k,v in report.items() if k not in ('types','rows','events')}))
