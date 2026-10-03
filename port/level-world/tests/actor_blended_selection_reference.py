"""Actual _SetAnim/_SetAnimStep selection24/26 recursion and retained rows.

Logger and leaf visual/audio/equipment producers are explicit services. The
original executes sequence metadata, both callbacks and all redirect decisions.
Callback swaps rewrite compatible immutable sequence identities synchronously.
"""
import argparse,hashlib,json,struct
from pathlib import Path
from unicorn import UC_HOOK_CODE
from visual_timeline_differential import TimelineCpu
from navigation_differential import ROOT

def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args()
 m=json.loads((ROOT/'reference/animation-blend-composition-reentry/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==m['original_sha256']
 c=TimelineCpu(a.engine,False,m);actor=c.data+0x10000;owner=actor+0x1000;guard=actor+0x2000;tableptr=actor+0x3000;table=actor+0x4000;countptr=actor+0x5000;steps=actor+0x6000
 def word(at):return struct.unpack('<I',c.uc.mem_read(at,4))[0]
 def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 got=0x3cab50+word(0x3cac94)
 c.pointer(got+word(0x3cac98),guard);c.pointer(guard,123)
 c.pointer(got+word(0x3cac9c),countptr);c.pointer(countptr,4)
 c.pointer(got+word(0x3caca0),tableptr);c.pointer(tableptr,table)
 # _SetAnimStep obtains the same resolved sequence-table global.
 stepgot=0x3ca7bc+word(0x3cab24);c.pointer(stepgot+word(0x3cab28),tableptr)
 trace=[];fixture={};swapped=False
 def record(op,value):
  depth=word(actor+0x2c);frame=actor+8+depth*12
  trace.append(words(op,value,c.uc.mem_read(actor+0x49,1)[0],c.uc.mem_read(actor+0x48,1)[0],word(actor+0x50),word(frame),word(frame+8),word(frame+4),depth))
 def hook(uc,address,size,unused):
  nonlocal swapped
  if address in (0x3136b4,0x3136b8,0x337888,0x3140ec,0x337a88,0x3139ac):ret(1)
  elif address==0x3a4d5c:
   event=c.reg(1);assert c.reg(2)==0;record(0,event)
   if event==fixture['swap'] and not swapped:
    swapped=True;depth=word(actor+0x2c)
    for d in range(depth+1):c.pointer(actor+8+d*12,word(actor+8+d*12)+1)
    c.pointer(actor+0x4c,word(actor+8+depth*12))
   ret(fixture['result'])
  elif address==0x3ca814:
   # Retained leaf pointer r6, after actual26 and redirect branch. Do not
   # emulate equipment/audio/visual services in this selection-order corpus.
   record(1,word(c.reg(6)+8));uc.reg_write(c.pc,0x3ca7e0)
 c.uc.hook_add(UC_HOOK_CODE,hook)
 records=[];summaries=[]
 for nested in (0,1):
  for loops in (0,2,-1):
   for pending in (0,1):
    for swap in (0,0x24,0x26):
     for result in (0,1):
      fixture={'swap':swap,'result':result};swapped=False;trace.clear();c.uc.mem_write(actor,bytes(0x80));c.pointer(actor+4,owner);c.pointer(actor+0x50,243);c.uc.mem_write(actor+0x49,bytes([pending]));c.uc.mem_write(actor+0x48,b'\1')
      for seq in range(4):
       step=steps+seq*0x100;c.uc.mem_write(table+seq*20,words(0,loops,2,step,1))
       for index in range(2):
        row=bytearray(56);struct.pack_into('<I',row,8,(seq+2 if nested and seq<2 else 955+seq));struct.pack_into('<I',row,0x28,int(nested and seq<2));c.uc.mem_write(step+index*56,bytes(row))
      c.invoke(0x3cab38,[actor,0,0])
      records.append(words(nested,loops,pending,swap,result,len(trace))+b''.join(trace));summaries.append({'nested':nested,'loops':loops,'pending':pending,'swap':swap,'result':result,'trace':[list(struct.unpack('<9I',r)) for r in trace]})
 blob=b'BSS1'+words(len(records))+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(blob)
 report={'validation':'PASS','original_sha256':m['original_sha256'],'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'corpus_sha256':hashlib.sha256(blob).hexdigest(),'original_selection_cases':len(records),'ordered_callbacks':sum(sum(r[0]==0 for r in s['trace']) for s in summaries),'leaf_services':len(records),'scope':__doc__,'records':summaries};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('records','scope')}))
if __name__=='__main__':main()
