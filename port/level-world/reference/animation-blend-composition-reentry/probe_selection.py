"""Original selector/controller instructions with resolved-resource services."""
import hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[4];HERE=Path(__file__).parent
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from visual_timeline_differential import TimelineCpu
engine=ROOT/'.local-inputs/libDungeonHunter2.so';manifest=json.loads((HERE/'original-functions.json').read_text())
assert hashlib.sha256(engine.read_bytes()).hexdigest()==manifest['original_sha256']
c=TimelineCpu(engine,False,manifest)
def w(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def ret(value=0):c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def fb(v):return struct.unpack('<I',struct.pack('<f',v))[0]
b=c.data+0x1000;agg=b+0x88;vec=b+0x200;slots=[b+0x300,b+0x500];sv=b+0x700;times=[b+0x800,b+0x900];tv=b+0xa00;service=b+0xb00;controller=b+0xc00;manager=b+0xd00;library=b+0xe00;definition=b+0xf00;descriptor=b+0x1000;meta=b+0x1100;resource=b+0x1200
calls=[];replay_check=False
def snapshot(t):return dict(current_ms=w(t+4),initialized=c.uc.mem_read(t+0x3d,1)[0],ended=c.uc.mem_read(t+0x3c,1)[0],loop=c.uc.mem_read(t+0x18,1)[0],start_ms=w(t+0x10),end_ms=w(t+0x14),pending=c.uc.mem_read(agg+0x30,1)[0],extra_ms=w(agg+0x10))
def observe(uc,address,size,data):
 if address==0x4748b8:ret(b)
 elif address==service:ret(12 if c.reg(0)==b else 11)
 elif address==service+4:ret(times[slots.index(c.reg(0))])
 elif address==service+8:ret(100)
 elif address==service+12:ret(900)
 elif address==service+16:ret(c.uc.mem_read(c.reg(0)+0x3c,1)[0])
 elif address==service+20:calls.append(dict(op='completion_callback',pending=c.uc.mem_read(agg+0x30,1)[0]));ret()
 elif address==service+24:calls.append(dict(op='install_slot_callback'));ret()
 elif address==0x3660f4:ret(resource)
 elif address==0x65f0fc:ret(0)
 elif address==0x65f0b4:ret(definition)
 elif address==0x60e334:ret(manager)
 elif address==0x60fab8:calls.append(dict(op='install_event_callback'));ret()
 elif address==0x35d624:
  calls.append(dict(op='RootNewAnim_service',timeline=snapshot(times[w(b+0x70)])))
  if replay_check:c.put(0,agg);c.put(1,times[w(b+0x70)]);uc.reg_write(c.pc,0x36440c)
  else:ret()
 elif address==0x666f7c:calls.append(dict(op='actual_SetClip',clip=c.reg(1)))
 elif address==0x666f10:calls.append(dict(op='actual_SetTime',time=c.reg(1)))
c.uc.hook_add(UC_HOOK_CODE,observe)
def configure(previous=7,ended=0):
 c.uc.mem_write(b,bytes(0x2000));c.pointer(b,sv);c.pointer(sv+0x24,service);c.pointer(b+0x28,vec);c.pointer(b+0x2c,vec+8);c.pointer(b+0x70,0);c.pointer(b+0x78,0)
 c.pointer(agg+0x10,17);c.uc.mem_write(agg+0x30,b'\1');c.pointer(agg+0x34,service+20)
 for i in (0,1):
  s=slots[i];t=times[i];c.pointer(vec+4*i,s);c.pointer(s,sv);c.pointer(s+0x24,library);c.pointer(s+0x50,previous);c.pointer(sv+0x44,service+4)
  c.pointer(t,tv);c.pointer(t+4,555);c.pointer(t+0x10,100);c.pointer(t+0x14,900);c.uc.mem_write(t+0x18,b'\1');c.uc.mem_write(t+0x3c,bytes([ended,1]));c.pointer(t+0x30,fb(2.));c.pointer(t+0x2c,fb(.1))
 c.pointer(tv+0x10,0x666f7c);c.pointer(tv+0x2c,service+8);c.pointer(tv+0x30,service+12);c.pointer(tv+0x44,service+16);c.pointer(tv+0xc,0x666f10);c.pointer(tv+0x40,0x666c28);c.pointer(tv+0x48,0x666c20)
 c.pointer(sv+0x30,service+24);c.pointer(library+0x3c,1);c.pointer(manager,1);c.pointer(definition,descriptor);c.pointer(descriptor+0x24,descriptor);c.pointer(descriptor+0x20,meta);c.pointer(resource+0x20,7)
 c.pointer(controller+4,0x1234);c.pointer(controller+0x14,0);calls.clear()
records=[]
for previous in (7,5):
 for ended in (0,1):
  for check in (False,True):
   configure(previous,ended);replay_check=check
   try:c.invoke(0x47680c,[controller,243,1,0,0])
   except Exception:print('PC',hex(c.uc.reg_read(c.pc)),'LR',hex(c.uc.reg_read(c.lr)),'calls',calls);raise
   after=snapshot(times[1])
   assert after['initialized']==0 and after['ended']==0 and after['current_ms']==(117 if previous==7 else 100)
   assert after['pending']==(0 if check else 1) and after['extra_ms']==17
   records.append(dict(op='PlayClip',previous=previous,initial_ended=ended,replay_CheckCallback=check,calls=list(calls),after=after,current_slot=w(b+0x70)))
configure();replay_check=False;c.invoke(0x47680c,[controller,0xffffffff,1,0,0]);assert w(b+0x70)==1 and c.uc.mem_read(agg+0x30,1)[0]==1
records.append(dict(op='PlayClip_failed_minus_one',current_slot=w(b+0x70),pending_after=1,extra_after=w(agg+0x10),calls=list(calls)))
# All timeline identities receive SetScale regardless of weights and pending.
scale_calls=[]
def scale_observe(uc,address,size,data):
 if address==0x666c20:scale_calls.append([times.index(c.reg(0)),c.reg(1)])
c.uc.hook_add(UC_HOOK_CODE,scale_observe)
for weights in ((0.,0.),(1.,0.),(0.,1.),(.25,.75)):
 configure();c.pointer(b+0x34,b+0x1400);c.uc.mem_write(b+0x1400,struct.pack('<2f',*weights));scale_calls.clear();c.invoke(0x3666d8,[b,fb(1.375)])
 assert scale_calls==[[0,fb(1.375)],[1,fb(1.375)]] and c.uc.mem_read(agg+0x30,1)[0]==1
 records.append(dict(op='SetScale',weights=weights,calls=list(scale_calls),scales=[w(t+0x30) for t in times],pending_after=1))
result={'validation':'PASS','original_sha256':manifest['original_sha256'],'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'original_instruction_cases':len(records),'native_comparisons':0,'records':records,'service_boundaries':['resource manager/ID/definition resolution','timeline identity and bounds virtuals','event callback installer','RootNewAnim observer; optional nested actual CheckCallback','source AEABI float arithmetic imports'],'scope':'Original PlayClip, Blend, SetCurrentAnimation, setCurrentAnimation, timeline SetClip/SetTime/SetLoop/SetScale, GetApplicator and CheckCallback execute. Root replay and asset resolution are explicit service boundaries.'}
(HERE/'selection-probes.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k not in ('records','service_boundaries')}))
