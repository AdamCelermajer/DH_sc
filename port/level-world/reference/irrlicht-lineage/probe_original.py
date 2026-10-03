"""Original GUI receiver filter vs historical stock-source expression only."""
import argparse,hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[4]
sys.path.insert(0,str(ROOT/'port/engine-resources/tests'))
from cpu import Cpu
from unicorn import UC_HOOK_CODE

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 m=json.loads((Path(__file__).parent/'original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==m['original_sha256']
 c=Cpu(a.engine,False,m);env=c.data+0x1000;receiver=c.data+0x2000;vt=c.data+0x2100;event=c.data+0x3000;callback=c.data+0x4000;observed=[];callback_result=0
 c.pointer(receiver,vt);c.pointer(vt+8,callback)
 def hook(uc,address,size,unused):
  if address==callback:
   observed.append((c.reg(0),c.reg(1)));c.put(0,callback_result);uc.reg_write(c.pc,uc.reg_read(c.lr))
 c.uc.hook_add(UC_HOOK_CODE,hook)
 records=[]
 for kind in range(7):
  for present in (0,1):
   for self_caller in (0,1):
    for callback_result in (0,1):
     caller=env+8 if self_caller else receiver
     c.pointer(env+0x1c4,receiver if present else 0);c.uc.mem_write(event,struct.pack('<5I',kind,0,caller,0,0));observed.clear()
     result=c.invoke(0x535710,[env,event])
     expected=bool(present and kind!=1 and kind!=2 and (kind!=0 or not self_caller))
     assert observed==([(receiver,event)] if expected else [])
     assert result==(callback_result if expected else 0)
     records.append({'event_type':kind,'receiver_present':present,'caller_is_environment_interface':self_caller,'callback_result':callback_result,'forwarded':expected,'returned':result})
 assert c.invoke(0x596d08,[env])==1
 report={'validation':'PASS','original_sha256':m['original_sha256'],'gui_filter_original_cases':len(records),'registration_base_return':1,'native_comparisons':0,'mismatches':0,'scope':'CGUIEnvironment::onEvent535710 executed original instructions against the historical stock-source boolean filter. Callback and receiver are bounded fixtures. ISceneNode base registration return also executes. No complete GUI or stock engine ABI/parity claim.','records':records}
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:v for k,v in report.items() if k not in ('records','scope')}))
if __name__=='__main__':main()
