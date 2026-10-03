"""Original-only reentry/selection audit. All service fixtures are named in output."""
import hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[4]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from visual_timeline_differential import TimelineCpu

HERE=Path(__file__).parent
engine=ROOT/'.local-inputs/libDungeonHunter2.so'
manifest=json.loads((HERE/'original-functions.json').read_text())
assert hashlib.sha256(engine.read_bytes()).hexdigest()==manifest['original_sha256']
records=[]
def machine():return TimelineCpu(engine,False,manifest)
def w(c,p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def raw(c,p,n):return bytes(c.uc.mem_read(p,n)).hex()
def ret(c,value=0):c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(c,fn):c.uc.hook_add(UC_HOOK_CODE,fn)
def fb(v):return struct.unpack('<I',struct.pack('<f',v))[0]

# Callback clear happens after return; null callbacks retain completion.
for pending,callback,reassert in ((0,1,0),(1,0,0),(1,1,0),(1,1,1)):
 c=machine();app=c.data+0x1000;service=app+0x100;calls=[]
 c.pointer(app+0x34,service if callback else 0);c.pointer(app+0x38,0x12345678);c.uc.mem_write(app+0x30,bytes([pending]))
 def observe(uc,address,size,data):
  if address==service:
   calls.append([c.reg(0),c.reg(1),uc.mem_read(app+0x30,1)[0]])
   if reassert:uc.mem_write(app+0x30,b'\1')
   ret(c)
 hook(c,observe);c.invoke(0x36440c,[app,0x23456789]);after=c.uc.mem_read(app+0x30,1)[0]
 assert after==(pending if not callback else 0)
 records.append(dict(op='CheckCallback',pending=pending,callback=callback,reassert=reassert,calls=calls,pending_after=after))

# Incoming-only ResetDelta: zero initialized sample survives absent root index.
for current in (0,1):
 for reference,root,write in ((0,-1,0),(1,-1,0),(1,2,0),(1,2,1)):
  c=machine();b=c.data+0x1000;agg=b+0x88;vec=b+0x200;slots=[b+0x300,b+0x500];sv=b+0x800;times=[b+0xa00,b+0xb00];service=b+0xc00;calls=[]
  c.pointer(b+0x28,vec);c.pointer(b+0x2c,vec+8);c.pointer(b+0x70,current);c.pointer(agg+8,reference);c.pointer(agg+12,root&0xffffffff);c.pointer(agg+0x3c,b)
  c.uc.mem_write(agg+0x30,b'\1');c.pointer(agg+0x10,99)
  for i in (0,1):
   c.pointer(vec+4*i,slots[i]);c.pointer(slots[i],sv);c.pointer(times[i]+0x10,266+i)
   c.uc.mem_write(slots[i]+0x58,bytes([0x55+i])*0x3c)
  c.pointer(sv+0x24,service);c.pointer(sv+0x44,service+4);c.pointer(sv+0x7c,service+8)
  def observe(uc,address,size,data):
   if address==service:ret(c,11)
   elif address==service+4:ret(c,times[slots.index(c.reg(0))])
   elif address==service+8:
    calls.append([slots.index(c.reg(0)),c.reg(1),c.reg(2),raw(c,c.reg(3),12)])
    if write:uc.mem_write(c.reg(3),struct.pack('<3f',2,4,8))
    ret(c)
  before=[raw(c,s+0x58,0x3c) for s in slots];hook(c,observe);c.invoke(0x366a68,[agg,1001]);after=[raw(c,s+0x58,0x3c) for s in slots]
  assert after[1-current]==before[1-current]
  if reference:
   p=slots[current]+0x58;assert w(c,p+0x14)==1001 and bytes(c.uc.mem_read(p+0x18,12))==struct.pack('<3f',*( (2,4,8) if write else (0,0,0))) and bytes(c.uc.mem_read(p+0x24,12))==bytes(12)
  else:assert after==before
  assert c.uc.mem_read(agg+0x30,1)[0]==1 and w(c,agg+0x10)==99
  records.append(dict(op='ResetDelta',current=current,reference=reference,root=root,sample_writes=write,calls=calls,before=before,after=after,aggregate_pending_after=1,aggregate_extra_after=99))

# Current timeline identity gates callback, including live current slot switch.
for current in (0,1):
 for ending in (0,1):
  c=machine();b=c.data+0x1000;vec=b+0x200;slots=[b+0x300,b+0x400];sv=b+0x500;times=[b+0x600,b+0x700];service=b+0x800
  c.pointer(b+0x28,vec);c.pointer(b+0x70,current);c.pointer(sv+0x44,service)
  for i in (0,1):
   c.pointer(vec+4*i,slots[i]);c.pointer(slots[i],sv);c.pointer(times[i]+4,266);c.pointer(times[i]+0x1c,fb(1.5));c.pointer(times[i]+0x2c,fb(1.));c.pointer(b+0x98,99)
  def observe(uc,address,size,data):
   if address==service:ret(c,times[slots.index(c.reg(0))])
  hook(c,observe);c.invoke(0x366628,[b,times[ending]]);pending=c.uc.mem_read(b+0xb8,1)[0];extra=w(c,b+0x98)
  assert (pending,extra)==((1,766) if current==ending else (0,99))
  records.append(dict(op='HandleAnimEnding',current=current,ending=ending,pending_after=pending,extra_after=extra))

# ANIM_Set queues last request while pending without resetting speed/completion.
for pending in (0,1):
 c=machine();a=c.data+0x1000;calls=[];c.uc.mem_write(a+0x49,bytes([pending]));c.pointer(a+0x40,fb(1.375));c.pointer(a+0x50,0xffffffff)
 def observe(uc,address,size,data):
  if address==0x3cab38:calls.append([c.reg(1),w(c,a+0x40),uc.mem_read(a+0x49,1)[0]]);ret(c)
 hook(c,observe)
 for seq in (243,248):c.invoke(0x3cacb0,[a,seq])
 assert w(c,a+0x50)==(248 if pending else 0xffffffff) and w(c,a+0x40)==(fb(1.375) if pending else fb(1.))
 records.append(dict(op='ANIM_Set',pending=pending,calls=calls,queued_sequence=w(c,a+0x50),speed_after=w(c,a+0x40)))

# Finite closure with prequeued request and nested actual ANIM_Set on22.
for initial_pending,callback_sequence,steps,callback_event,service_return in ((-1,-1,1,0x22,0),(243,-1,1,0x22,0),(243,248,1,0x22,0),(243,-1,2,0x23,0),(243,248,2,0x27,0),(243,248,2,0x23,0),(243,248,2,0x23,1)):
 c=machine();a=c.data+0x10000;owner=a+0x1000;guard=a+0x2000;tableptr=a+0x3000;table=a+0x4000;events=[];select=[]
 got=0x3caf4c+8+w(c,0x3cb2c4);c.pointer(got+w(c,0x3cb2c8),guard);c.pointer(guard,123);c.pointer(got+w(c,0x3cb2d4),tableptr);c.pointer(tableptr,table)
 c.uc.mem_write(table,struct.pack('<5I',0,0,steps,0,int(steps>1)));c.pointer(owner+0x520,0x200);c.pointer(a+4,owner);c.pointer(a+0x50,initial_pending&0xffffffff);c.uc.mem_write(a+0x5c,b'\1');c.uc.mem_write(a+0x49,b'\1')
 services={0x3136b4,0x3136b8,0x337888,0x3140ec,0x337a88,0x3139ac}
 def observe(uc,address,size,data):
  if address in services:ret(c)
  elif address==0x3a4d5c:
   event=c.reg(1);events.append([event,uc.mem_read(a+0x48,1)[0],uc.mem_read(a+0x49,1)[0],w(c,a+0x50),w(c,a+0x10)])
   assert c.reg(2)==0
   if event==callback_event and callback_sequence!=-1:
    c.put(0,a);c.put(1,callback_sequence);uc.reg_write(c.pc,0x3cacb0)
   else:ret(c)
  elif address in (0x3cab38,0x3ca79c):select.append([hex(address),c.reg(1),uc.mem_read(a+0x49,1)[0],w(c,a+0x50),w(c,a+0x10)]);ret(c,service_return)
 hook(c,observe);c.invoke(0x3caf3c,[a]);assert c.uc.mem_read(a+0x49,1)[0]==0 and w(c,a+0x50)==0xffffffff
 if steps==1:assert [e[0] for e in events]==[0x27,0x25,0x22]
 else:assert [e[0] for e in events]==[0x27,0x23] and [s[0] for s in select]==['0x3ca79c','0x3cab38'],(events,select)
 if initial_pending!=-1:assert select[-1][1]==(callback_sequence if callback_sequence!=-1 else initial_pending) and select[-1][2]==0
 records.append(dict(op='CharAnimatorUpdate',initial_pending=initial_pending,callback_sequence=callback_sequence,callback_event=callback_event,service_return=service_return,steps=steps,events=events,selections=select,pending_after=0,queued_after=-1))

result={'validation':'PASS','original_sha256':manifest['original_sha256'],'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'original_instruction_cases':len(records),'native_comparisons':0,'records':records,'service_boundaries':['Timeline identity/type virtuals','Reset root sample optional XYZ write','resolved sequence table and owner fields','logging services','Character RaiseEvent observer; actual ANIM_Set invoked synchronously on22','CharAnimator _SetAnim/PrepareAnim observed; scheduler selection implementation outside this probe'],'scope':'Original ARM32 instructions only; synthetic resolved fields/service fixtures. No native parity or asset factory claim.'}
(HERE/'source-probes.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({k:v for k,v in result.items() if k not in ('records','service_boundaries')}))
