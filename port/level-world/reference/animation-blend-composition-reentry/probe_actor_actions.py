"""Original CharAnimator repeat/advance/close callback choreography."""
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
a=c.data+0x10000;owner=a+0x1000;guard=a+0x2000;tableptr=a+0x3000;table=a+0x4000
got=0x3caf4c+8+w(0x3cb2c4);c.pointer(got+w(0x3cb2c8),guard);c.pointer(guard,123);c.pointer(got+w(0x3cb2d4),tableptr);c.pointer(tableptr,table)
trace=[];event_request=-1;event_point=0;observer_result=0
services={0x3136b4,0x3136b8,0x337888,0x3140ec,0x337a88,0x3139ac}
def observe(uc,address,size,data):
 if address in services:ret()
 elif address==0x3a4d5c:
  event=c.reg(1);assert c.reg(2)==0
  trace.append(dict(op='event',event=event,payload=0,pending=c.uc.mem_read(a+0x49,1)[0],closed=c.uc.mem_read(a+0x48,1)[0],queued=w(a+0x50),step=w(a+0x10),repeat=w(a+0xc)))
  if event==event_point and event_request!=-1:c.put(0,a);c.put(1,event_request);uc.reg_write(c.pc,0x3cacb0)
  else:ret(observer_result)
 elif address in (0x3cab38,0x3ca79c):
  trace.append(dict(op='PrepareAnim' if address==0x3ca79c else '_SetAnim',sequence=c.reg(1),depth=c.reg(2),pending=c.uc.mem_read(a+0x49,1)[0],queued=w(a+0x50),step=w(a+0x10),repeat=w(a+0xc)))
  ret(observer_result)
c.uc.hook_add(UC_HOOK_CODE,observe)
records=[]
for action,steps,repeat in (('close',1,0),('advance',2,0),('repeat',1,1),('repeat_forever',1,-1)):
 for initial,event_point,event_request,observer_result in ((-1,0,-1,0),(243,0,-1,0),(-1,0x27,248,0),(-1,0x25,248,1),(-1,0x23,248,0),(-1,0x22,248,1)):
  c.uc.mem_write(a,bytes(0x80));c.uc.mem_write(table,struct.pack('<5I',0,0,steps,0,int(action=='advance')));c.pointer(owner+0x520,0x200);c.pointer(a+4,owner);c.pointer(a+0xc,repeat&0xffffffff);c.pointer(a+0x50,initial&0xffffffff);c.uc.mem_write(a+0x5c,b'\1');c.uc.mem_write(a+0x49,b'\1');trace.clear()
  c.invoke(0x3caf3c,[a]);assert c.uc.mem_read(a+0x49,1)[0]==0 and w(a+0x50)==0xffffffff
  events=[r['event'] for r in trace if r['op']=='event'];select=[r for r in trace if r['op']!='event'];queued=event_request if event_point in events and event_request!=-1 else initial
  if action=='close':assert events==[0x27,0x25,0x22] and len(select)==int(queued!=-1)
  elif action=='advance':assert events==[0x27,0x23] and select[0]['op']=='PrepareAnim' and select[0]['pending']==1,(action,events,select,trace)
  else:
   assert events==[0x27,0x25,0x23] and len(select)==1
   assert select[0]['sequence']==(queued if queued!=-1 else 0) and select[0]['pending']==int(queued==-1)
  if queued!=-1:assert select[-1]['sequence']==queued and select[-1]['pending']==0
  records.append(dict(action=action,initial_request=initial,callback_event=event_point,callback_request=event_request,observer_result=observer_result,trace=list(trace),pending_after=0,queued_after=-1))
# Retained finite root can receive another explicit pending notification.
c.uc.mem_write(a,bytes(0x80));c.uc.mem_write(table,struct.pack('<5I',0,0,1,0,0));c.pointer(a+4,owner);c.pointer(a+0x50,0xffffffff);c.uc.mem_write(a+0x5c,b'\1');event_request=-1;trace.clear()
for repeat_update in range(2):c.uc.mem_write(a+0x49,b'\1');c.invoke(0x3caf3c,[a])
assert [r['event'] for r in trace if r['op']=='event']==[0x27,0x25,0x22,0x27,0x25]
records.append(dict(action='closed_repeated_pending',trace=list(trace),pending_after=0,queued_after=-1))
result={'validation':'PASS','original_sha256':manifest['original_sha256'],'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'original_instruction_cases':len(records),'native_comparisons':0,'records':records,'scope':'Actual CharAnimator Update and synchronous ANIM_Set; resolved depth0 sequence records, owner fields/loggers and _SetAnim/PrepareAnim service observers. Event/service returns0/1 are fixtures; source callsites ignore these returns. Nested scheduler stack and AIS behavior are not supplied.'}
(HERE/'actor-action-probes.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k!='records'}))
