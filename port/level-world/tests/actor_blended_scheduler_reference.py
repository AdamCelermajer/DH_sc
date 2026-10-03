"""Original CharAnimator callback/queue traces, including child unwind.

Resolved immutable sequence rows, logger, _SetAnim and _SetAnimStep services
are fixtures. Update and ANIM_Set execute original instructions. This proves
the scheduling boundary, not deep clip/audio/equipment selection services.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
from visual_timeline_differential import TimelineCpu
from navigation_differential import ROOT

def words(*values):return struct.pack('<'+'I'*len(values),*(v&0xffffffff for v in values))
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args()
 manifest=json.loads((ROOT/'reference/animation-blend-composition-reentry/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 c=TimelineCpu(a.engine,False,manifest);actor=c.data+0x10000;owner=actor+0x1000;guard=actor+0x2000;tableptr=actor+0x3000;table=actor+0x4000
 def word(at):return struct.unpack('<I',c.uc.mem_read(at,4))[0]
 def ret(value=0):c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 got=0x3caf54+word(0x3cb2c4);c.pointer(got+word(0x3cb2c8),guard);c.pointer(guard,123);c.pointer(got+word(0x3cb2d4),tableptr);c.pointer(tableptr,table)
 trace=[];fixture={}
 def observe(uc,address,size,unused):
  if address in (0x3136b4,0x3136b8,0x337888,0x3140ec,0x337a88,0x3139ac):ret()
  elif address==0x3a4d5c:
   event=c.reg(1);depth=word(actor+0x2c);frame=actor+8+depth*12;assert c.reg(2)==0
   trace.append(words(0,event,0,uc.mem_read(actor+0x49,1)[0],uc.mem_read(actor+0x48,1)[0],word(actor+0x50),word(frame+8),word(frame+4),depth))
   if event==fixture['callback_event'] and fixture['callback_request']!=-1:c.put(0,actor);c.put(1,fixture['callback_request']);uc.reg_write(c.pc,0x3cacb0)
   else:ret(fixture['observer_result'])
  elif address in (0x3cab38,0x3ca79c):
   depth=word(actor+0x2c);frame=actor+8+depth*12
   # _SetAnimStep has ONE unsigned argument: r2 is caller-clobbered, not depth.
   trace.append(words(1 if address==0x3cab38 else 2,c.reg(1),c.reg(2) if address==0x3cab38 else 0,uc.mem_read(actor+0x49,1)[0],uc.mem_read(actor+0x48,1)[0],word(actor+0x50),word(frame+8),word(frame+4),depth));ret(fixture['observer_result'])
 c.uc.hook_add(UC_HOOK_CODE,observe)
 records=[];summaries=[]
 for root_type,root_count,root_loops,depth,updates in ((0,1,0,0,1),(1,2,0,0,1),(0,1,1,0,1),(0,1,-1,0,1),(0,1,0,0,2),(1,2,0,1,1),(0,1,0,1,1),(0,1,1,1,1)):
  for initial,event_point,event_request,result in ((-1,0,-1,0),(243,0,-1,0),(-1,0x27,248,0),(-1,0x25,248,1),(-1,0x23,248,0),(-1,0x22,248,1)):
   fixture={'callback_event':event_point,'callback_request':event_request,'observer_result':result}
   c.uc.mem_write(actor,bytes(0x80));c.uc.mem_write(table,words(0,root_loops,root_count,0,root_type)+words(0,0,1,0,0));c.pointer(owner+0x520,0x200);c.pointer(actor+4,owner);c.pointer(actor+0xc,root_loops&0xffffffff);c.pointer(actor+0x2c,depth)
   if depth:c.pointer(actor+0x14,1);c.pointer(actor+0x18,0);c.pointer(actor+0x1c,0)
   c.pointer(actor+0x50,initial&0xffffffff);c.uc.mem_write(actor+0x5c,b'\1');trace.clear()
   for _ in range(updates):c.uc.mem_write(actor+0x49,b'\1');c.invoke(0x3caf3c,[actor])
   assert c.uc.mem_read(actor+0x49,1)==b'\0' and word(actor+0x50)==0xffffffff
   header=words(root_type,root_count,root_loops,depth,updates,initial,event_point,event_request,result,len(trace));records.append(header+b''.join(trace))
   summaries.append({'type':root_type,'count':root_count,'loops':root_loops,'depth':depth,'updates':updates,'initial_request':initial,'callback_event':event_point,'callback_request':event_request,'trace':[list(struct.unpack('<9I',t)) for t in trace]})
 blob=b'BPS1'+words(len(records))+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(blob)
 report={'validation':'PASS','original_sha256':manifest['original_sha256'],'corpus_sha256':hashlib.sha256(blob).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'original_scheduling_cases':len(records),'original_nested_unwind_cases':sum(s['depth']!=0 for s in summaries),'callbacks':sum(sum(t[0]==0 for t in s['trace']) for s in summaries),'preparation_services':sum(sum(t[0]!=0 for t in s['trace']) for s in summaries),'scope':__doc__,'records':summaries}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('records','scope')}))
if __name__=='__main__':main()
