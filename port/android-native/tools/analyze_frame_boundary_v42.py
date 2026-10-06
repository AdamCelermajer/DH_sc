"""Analyze recorded callback boundaries; never labels cadence presented FPS."""
import argparse,json,math
from pathlib import Path
def summary(values):
 values=sorted(values)
 if not values:return {'samples':0,'available':False}
 def percentile(p):return values[min(len(values)-1,max(0,math.ceil(p*len(values))-1))]/1e6
 return {'samples':len(values),'median_ms':percentile(.5),'p95_ms':percentile(.95),'p99_ms':percentile(.99),'max_ms':values[-1]/1e6}
def analyze(record):
 if record.get('schema')!='dh2-frame-boundary-v42':raise ValueError('Wrong schema')
 rows=record['rows']
 if not 0<=len(rows)<=512 or record['frames']!=len(rows):raise ValueError('Frame count outside bound/mismatch')
 totals=[];native=[];pre=[];tail=[];cpu=[];cadence=[];gap=[]
 previous=None
 for row in rows:
  if len(row)!=6 or any(type(v)is not int for v in row):raise ValueError('Expected six integer counters')
  begin,nb,ne,end,cb,ce=row
  if not 0<begin<=nb<=ne<=end:raise ValueError('Unfinished/unordered callback; no success inference')
  if previous:
   if begin<previous[3]:raise ValueError('Overlapping/regressed GL callback')
   cadence.append(begin-previous[0]);gap.append(begin-previous[3])
  totals.append(end-begin);native.append(ne-nb);pre.append(nb-begin);tail.append(end-ne)
  if cb>=0 and ce>=cb:cpu.append(ce-cb)
  previous=row
 return {'schema':'dh2-frame-attribution-v42','tid':record['tid'],'callback_wall':summary(totals),'native_wall':summary(native),'java_pre_native_wall':summary(pre),'java_post_native_wall':summary(tail),'guest_thread_cpu':summary(cpu),'callback_start_cadence':summary(cadence),'intercallback_unattributed_gap':summary(gap),'actual_egl_swap_ms':None,'actual_presented_fps':None,'gpu_duration_ms':None,'host_qemu_paging':None,'interpretation':'Gap includes framework/driver swap, scheduling and waits until trace evidence separates them. Callback wall and cadence are not presentation; neither proves GPU-bound behavior.'}
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('record',type=Path);p.add_argument('--output',type=Path);args=p.parse_args()
 if args.record.stat().st_size>256*1024:raise ValueError('Input exceeds bounded marker report size')
 result=json.dumps(analyze(json.loads(args.record.read_text())),indent=2)+'\n'
 if args.output:args.output.write_text(result)
 else:print(result,end='')
if __name__=='__main__':main()
