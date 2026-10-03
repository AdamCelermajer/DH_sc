"""Original public ANIM_SetStep/SkipNextStep/StopLoop and closed getters.

These metadata adapters are distinct from private _SetAnimStep selection.
The resolved immutable sequence table is an explicit fixture for GetStepCount.
"""
import argparse,hashlib,json,struct
from pathlib import Path
from visual_timeline_differential import TimelineCpu
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--manifest',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args();m=json.loads(a.manifest.read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==m['original_sha256'];c=TimelineCpu(a.engine,False,m)
 actor=c.data+0x1000;tableptr=c.data+0x3000;table=c.data+0x4000
 def word(at):return struct.unpack('<I',c.uc.mem_read(at,4))[0]
 got=0x3c9360+word(0x3c9390);c.pointer(got+word(0x3c9394),tableptr);c.pointer(tableptr,table)
 for seq in range(3):c.uc.mem_write(table+seq*20,words(0,0,seq+2,0,1))
 records=[]
 for depth in range(3):
  for closed in (0,1):
   for stop in (0,1):
    for loops in (-1,0,2):
     for step in (0,1,0xffffffff):
      for op in range(5):
       for arg in (0,1,3,0xffffffff):
        c.uc.mem_write(actor,bytes(0x80));c.pointer(actor+0x2c,depth);c.uc.mem_write(actor+0x48,bytes([closed,0,stop]));frame=actor+8+depth*12;c.uc.mem_write(frame,words(depth,loops,step));c.invoke((0x3c9484,0x3c9464,0x3c948c,0x3c932c,0x3c934c)[op],[actor,arg]);value=c.reg(0) if op>=3 else 0
        records.append(words(depth,closed,stop,loops,step,op,arg,word(frame+4),word(frame+8),c.uc.mem_read(actor+0x4a,1)[0],value))
 blob=b'BSC1'+words(len(records))+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(blob);report={'validation':'PASS','original_sha256':m['original_sha256'],'manifest_sha256':hashlib.sha256(a.manifest.read_bytes()).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'corpus_sha256':hashlib.sha256(blob).hexdigest(),'original_control_cases':len(records),'scope':__doc__};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
