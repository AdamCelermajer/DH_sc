"""Actual original manager synchronous service reentry vs O2 native.
An explicit provider invokes SlowUpdate on the SAME live manager during a
reached elapsed/cache/text/ally callback. ARM register context and independent
nested stack are saved by the desktop service harness, not by production.
"""
import argparse,json,hashlib
from pathlib import Path
from hud_manager_differential import Machine,ROOT
class Reentry(Machine):
 def deliver(self,op,*args,**kwargs):
  result=super().deliver(op,*args,**kwargs)
  if op==self.trigger and not self.did_nested:
   self.did_nested=True;c=self.c;context=c.uc.context_save();stack=c.stack;c.stack-=0x5000
   try:
    if self.native:assert c.invoke('dh2_ui_hud_manager_v1',[self.s,4,self.svc],budget=20000000)==0
    else:c.invoke(0x41de54,[self.s],budget=20000000)
   finally:c.stack=stack;c.uc.context_restore(context)
  return result
 def run(self,cfg):self.did_nested=False;return super().run(cfg)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--gold',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();manifest=json.loads((ROOT/'reference/hud-player-infos/manager/original-functions.json').read_text());old=Reentry(a.engine,False,manifest);new=Reentry(a.library,True,{'functions':[]});rows=[]
 source=json.loads(a.gold.read_text())['rows']
 for trigger in (2,6,11,38):
  candidates=[r for r in source if any(c[0]==trigger for c in r['calls'])][:12]
  for row in candidates:
   cfg=row['input'][:];cfg[102]=0;old.trigger=new.trigger=trigger;x,events=old.run(cfg);y,calls=new.run(cfg);assert x==y,(trigger,cfg,x,y)
   if events!=calls:
    k=next((i for i,(x,y) in enumerate(zip(events,calls)) if x!=y),min(len(events),len(calls)));raise AssertionError((trigger,k,events[max(0,k-2):k+3],calls[max(0,k-2):k+3]))
   assert old.did_nested==new.did_nested;rows.append(dict(trigger=trigger,input=cfg,after=x,calls=events,nested=old.did_nested))
 a.output.mkdir(parents=True,exist_ok=True);gold=a.output/'reentry-gold.json';gold.write_text(json.dumps(dict(validation='PASS',rows=rows),separators=(',',':'))+'\n');report=dict(validation='PASS',cases=len(rows),nested_same_manager_calls=sum(r['nested'] for r in rows),ordered_services=sum(len(r['calls']) for r in rows),original_sha256=sha(a.engine),library_sha256=sha(a.library),gold_sha256=sha(gold),scope=__doc__);(a.output/'reentry-report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
