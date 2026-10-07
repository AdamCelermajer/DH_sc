"""Bounded offline decoding of actual ftrace slices/scheduling, not full TP.

Uses official Perfetto wire IDs; unsupported compression/clock domains fail.
No device/server/process launch or dependency installation.
"""
from pathlib import Path
import argparse,collections,hashlib,json,math,struct,time,re,bisect
LIMIT=8*1024**2
def varint(raw,at):
 value=0
 for shift in range(0,70,7):
  if at>=len(raw):raise ValueError('Truncated protobuf varint')
  b=raw[at];at+=1;value|=(b&127)<<shift
  if not b&128:return value,at
 raise ValueError('Overlong protobuf varint')
def fields(raw):
 at=0;n=0
 while at<len(raw):
  tag,at=varint(raw,at);key,wire=tag>>3,tag&7;n+=1
  if not key or n>1000000:raise ValueError('Invalid/excessive protobuf fields')
  if wire==0:value,at=varint(raw,at)
  elif wire in (1,5):
   size=8 if wire==1 else 4
   if at+size>len(raw):raise ValueError('Truncated fixed field')
   value=raw[at:at+size];at+=size
  elif wire==2:
   size,at=varint(raw,at)
   if size>len(raw)-at:raise ValueError('Truncated length field')
   value=raw[at:at+size];at+=size
  else:raise ValueError('Unsupported protobuf wire '+str(wire))
  yield key,wire,value
def obj(raw):return {k:v for k,w,v in fields(raw)}
def string(raw):return bytes(raw).decode('utf-8',errors='replace').strip()
def packed(raw):
 at=0;out=[]
 while at<len(raw):
  value,at=varint(raw,at);out.append(value)
 return out
def stats(values):
 a=sorted(values)
 if not a:return {'samples':0,'available':False}
 def q(p):return a[max(0,math.ceil(p*len(a))-1)]/1e6
 return {'samples':len(a),'total_ms':sum(a)/1e6,'median_ms':q(.5),'p95_ms':q(.95),'p99_ms':q(.99),'max_ms':a[-1]/1e6}
def merge_ranges(ranges):
 out=[]
 for a,b in sorted(ranges):
  if b<a:raise ValueError('Regressed loss range')
  if out and a<=out[-1][1]:out[-1]=(out[-1][0],max(b,out[-1][1]))
  else:out.append((a,b))
 return out
def overlaps(begin,end,ranges):return any(a<end and b>begin for a,b in ranges)
def normalize(name):
 name=re.sub(r'frame=\d+','frame=*',name)
 name=re.sub(r'\(f:\d+,a:\d+\)','(f:*,a:*)',name)
 return name
def decode(path,tid,pid):
 if path.stat().st_size>LIMIT:raise ValueError('Input above8MiB bound')
 raw=memoryview(path.read_bytes());prints=[];switches=[];wakings=[];clocks=[];fs=[];service=[]
 count=collections.Counter();bundles=collections.Counter();lost=[];clock_domains=set();compact=[];last_cpu_end={};start=time.monotonic()
 for packetkey,wire,packet in fields(raw):
  if packetkey!=1 or wire!=2:raise ValueError('Unexpected Trace framing')
  count['packets']+=1
  if count['packets']>100000 or time.monotonic()-start>15:raise ValueError('Offline parser packet/time bound')
  for key,w,value in fields(packet):
   count['packet_field_'+str(key)]+=1
   if key in (50,133):raise ValueError('Compressed packets unsupported; use actual trace processor')
   if key==6:
    clock={}
    for k,_,v in fields(value):
     if k==1:
      c=obj(v)
      if c.get(3,0)or c.get(4,1)!=1:raise ValueError('Incremental/custom clock unit unsupported')
      clock[c[1]]=c[2]
    clocks.append(clock)
   elif key in (34,35):
    o={'timestamp_ns':obj(packet).get(8),'fields':{}}
    for k,vw,v in fields(value):
     if key==34 and k==2:
      o.setdefault('cpus',[]).append({str(a):b if isinstance(b,int)else struct.unpack('<d',b)[0]for a,b in obj(v).items()})
     elif key==35 and k==1:o.setdefault('buffers',[]).append({str(a):b for a,b in obj(v).items()if isinstance(b,int)})
     else:o['fields'].setdefault(str(k),[]).append(v if isinstance(v,int)else string(v))
    (fs if key==34 else service).append(o)
   elif key==1:
    b=list(fields(value));cpu=next((v for k,w,v in b if k==1),None);bundles[cpu]+=1
    domain=next((v for k,w,v in b if k==5),0);clock_domains.add(domain)
    if domain!=0:raise ValueError('Non-boot ftrace clock unsupported: '+str(domain))
    event_ts=[obj(v).get(1)for k,w,v in b if k==2]
    event_ts=[t for t in event_ts if t is not None]
    if any(k==3 and v for k,w,v in b):
     first=min(event_ts)if event_ts else None;previous=next((v for k,w,v in b if k==10),last_cpu_end.get(cpu))
     if first is None or previous is None:raise ValueError('Loss interval cannot be bounded')
     lost.append({'cpu':cpu,'bundle':bundles[cpu],'begin_ns':previous,'end_ns':first})
    if event_ts:last_cpu_end[cpu]=max(event_ts)
    for k,vw,v in b:
     if k==8:lost.append({'cpu':cpu,'error':{str(a):b for a,b in obj(v).items()}})
     if k==4:
      c=collections.defaultdict(list)
      for ck,cw,cv in fields(v):c[ck].extend(packed(cv)if cw==2 and ck!=5 else[string(cv)]if ck==5 else[cv])
      required=[c[i]for i in [1,2,3,4,6]]
      if len({len(a)for a in required})!=1:raise ValueError('Compact switch cardinality mismatch')
      stamp=0
      for i,d in enumerate(c[1]):
       stamp+=d;comm=c[5][c[6][i]];compact.append((stamp,cpu,c[3][i],c[2][i],comm))
      stamp=0
      for i,d in enumerate(c[7]):
       stamp+=d
       if c[8][i]==tid:wakings.append((stamp,'compact_waking'))
     if k!=2:continue
     e=obj(v);count['events']+=1
     if count['events']>500000:raise ValueError('Event bound')
     ts=e.get(1);event_tid=e.get(2)
     if 3 in e:
      count['print_events']+=1
      if event_tid==tid:prints.append((ts,string(obj(e[3]).get(2,b''))))
     if 4 in e:
      s=obj(e[4]);switches.append((ts,cpu,s.get(2),s.get(6),s.get(4),string(s.get(1,b'')),string(s.get(5,b''))))
     for wake in (17,20):
      if wake in e and obj(e[wake]).get(2)==tid:wakings.append((ts,'wakeup'if wake==17 else'waking'))
 # Compact switch has no prev_pid: recover ONLY from prior same-CPU next_pid.
 # First switch per CPU is explicitly unknown, not guessed from event emitter.
 last={}
 for ts,cpu,nxt,state,comm in sorted(compact):
  switches.append((ts,cpu,last.get(cpu),nxt,state,'',comm));last[cpu]=nxt
 switches.sort();prints.sort();wakings.sort();slices=[];stack=[];unmatched=0
 loss_ranges=merge_ranges([(r['begin_ns'],r['end_ns'])for r in lost if 'begin_ns'in r]);loss_resets=0;loss_discarded_prints=0;loss_at=0
 for ts,text in prints:
  while loss_at<len(loss_ranges)and ts>=loss_ranges[loss_at][0]:
   stack.clear();loss_resets+=1;loss_at+=1
  if overlaps(ts,ts+1,loss_ranges):loss_discarded_prints+=1;continue
  if text.startswith('B|'):
   parts=text.split('|',2)
   if len(parts)==3:stack.append((ts,parts[2],len(stack)))
  elif text=='E' or text.startswith('E|'):
   if stack:
    begin,name,depth=stack.pop();slices.append({'begin_ns':begin,'end_ns':ts,'name':name,'depth':depth,'duration_ns':ts-begin})
   else:unmatched+=1
 byname=collections.defaultdict(list)
 for s in slices:
  if overlaps(s['begin_ns'],s['end_ns'],loss_ranges):raise ValueError('Invalid slice crosses loss boundary')
  byname[normalize(s['name'])].append(s['duration_ns'])
 # Reconstruct running and switch-out→switch-in spans for ONLY named TID.
 run_begin=None;out_begin=None;run=[];off=[];unknown=[]
 for ts,cpu,prev,nxt,state,pc,nc in switches:
  if prev==tid:
   if run_begin is not None:
    if ts<run_begin:raise ValueError('Scheduler time regression')
    run.append((run_begin,ts))
   else:unknown.append({'at_ns':ts,'kind':'switch_out_without_known_switch_in'})
   run_begin=None;out_begin=(ts,state)
  if nxt==tid:
   if out_begin is not None:off.append((out_begin[0],ts,out_begin[1]))
   else:unknown.append({'at_ns':ts,'kind':'switch_in_without_known_switch_out'})
   out_begin=None;run_begin=ts
 runnable=[];sleep=[];other=[];wake_times=sorted(set(ts for ts,k in wakings))
 raw_scheduler_spans={'running':len(run),'offcpu':len(off)}
 run=[(a,b)for a,b in run if not overlaps(a,b,loss_ranges)]
 off=[(a,b,s)for a,b,s in off if not overlaps(a,b,loss_ranges)]
 for begin,end,state in off:
  if state==0:runnable.append((begin,end))
  else:
   i=bisect.bisect_left(wake_times,begin);wake=wake_times[i]if i<len(wake_times)and wake_times[i]<=end else None
   if wake is not None:
    sleep.append((begin,wake,state));runnable.append((wake,end))
   else:other.append((begin,end,state))
 offsets=[c[6]-c[3]for c in clocks if 3 in c and 6 in c]
 result={'schema':'dh2-offline-ftrace-v42','trace_sha256':hashlib.sha256(raw).hexdigest(),'trace_bytes':len(raw),'tid':tid,'pid':pid,'counts':dict(count),'cpu_bundle_counts':dict(bundles),'ftrace_clock_domains':list(clock_domains),'clock_boot_minus_monotonic_ns':{'samples':len(offsets),'min':min(offsets)if offsets else None,'max':max(offsets)if offsets else None},'ftrace_bundle_loss_or_errors':lost,'excluded_loss_ranges':loss_ranges,'excluded_loss_total_ms':sum(b-a for a,b in loss_ranges)/1e6,'loss_stack_resets':loss_resets,'loss_discarded_prints':loss_discarded_prints,'raw_scheduler_spans_before_loss_exclusion':raw_scheduler_spans,'ftrace_stats':fs,'service_trace_stats':service,'synchronous_GL_TID_slice_stats':{n:stats(a)for n,a in sorted(byname.items())},'unmatched_trace_ends':unmatched,'open_slices_at_end':len(stack),'scheduler_running':stats([b-a for a,b in run]),'scheduler_runnable_wait':stats([b-a for a,b in runnable]),'scheduler_sleep_before_observed_wake':stats([b-a for a,b,s in sleep]),'scheduler_offcpu_without_observed_wake':stats([b-a for a,b,s in other]),'scheduler_end_states':dict(collections.Counter(str(s)for a,b,s in off)),'scheduler_unknown_boundaries':unknown,'parser_elapsed_seconds':time.monotonic()-start,'not_full_trace_processor':True,'FrameTimeline_SQL_acceptance':False,'no_device_calls':True,'whole_trace_percentiles_or_cpu_percentages_accepted':False,'sample_bias':'Only complete spans wholly outside any CPU loss range; incomplete/lost spans excluded, so not representative full workload quantiles.'}
 detail={'slices':slices,'running':run,'runnable':runnable,'sleep':sleep,'other_offcpu':other,'wake_times':wake_times,'print_events':prints}
 return result,detail
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('trace',type=Path);p.add_argument('--tid',type=int,default=3338);p.add_argument('--pid',type=int,default=3319);p.add_argument('--output',type=Path,required=True);args=p.parse_args()
 result,detail=decode(args.trace,args.tid,args.pid);args.output.mkdir(parents=True,exist_ok=True);(args.output/'offline-ftrace.json').write_text(json.dumps(result,indent=2)+'\n');(args.output/'offline-ftrace-detail.json').write_text(json.dumps(detail,separators=(',',':'))+'\n');print(json.dumps({'trace_sha256':result['trace_sha256'],'loss_flags':len(result['ftrace_bundle_loss_or_errors']),'excluded_loss_total_ms':result['excluded_loss_total_ms'],'slice_stats':{n:s for n,s in result['synchronous_GL_TID_slice_stats'].items()if n.startswith('DH2:')or n in ['eglSwapBuffers','dequeueBuffer','queueBuffer','waitForBufferRelease']},'scheduler_running':result['scheduler_running'],'scheduler_runnable_wait':result['scheduler_runnable_wait'],'whole_trace_quantiles_accepted':False},indent=2))
if __name__=='__main__':main()
