"""Original CharAnimator::Update decision range versus compiled ARM64 core.

The character event-dispatch call is modeled as a no-op. Execution stops
before original event logging, selection dispatch or parent unwind; those
operations are outside the bit-exact comparison scope.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../engine-resources/tests'))
from cpu import Cpu
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args()
 manifest=json.loads((ROOT/'reference/completion/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});result=[];boundaries={0x3cb06c:0,0x3cb1e4:1,0x3cb174:2}
 def stop(uc,address,size,unused):
  if address in boundaries:result.append(boundaries[address]);uc.reg_write(old.pc,old.stop)
 def event(uc,address,size,unused):uc.reg_write(old.pc,uc.reg_read(old.lr))
 old.uc.hook_add(UC_HOOK_CODE,stop,begin=0x3cb018,end=0x3cb258)
 old.uc.hook_add(UC_HOOK_CODE,event,begin=0x3a4d5c,end=0x3a4d5c)
 rng=random.Random(20261002);actions={0:0,1:0,2:0}
 for i in range(3000):
  count=rng.randrange(1,101);step=rng.randrange(count);loops=(-1,0,1,2,7)[i%5];kind=(-1,0,1,2)[i%4]
  obj,sequence=old.data+0x1000,old.data+0x2000;old.uc.mem_write(obj,b'\0'*0x100);old.uc.mem_write(obj+12,struct.pack('<iI',loops,step));old.uc.mem_write(sequence+8,struct.pack('<III',count,0,kind&0xffffffff))
  old.uc.reg_write(old.sp,old.stack+0xe000)
  for reg,value in {4:obj,7:sequence,8:obj,10:0}.items():old.put(reg,value)
  result.clear();old.uc.emu_start(0x3cb018,old.stop,count=10000);assert len(result)==1,(i,result)
  sp,lp=new.data+0x1000,new.data+0x2000;new.uc.mem_write(sp,struct.pack('<I',step));new.uc.mem_write(lp,struct.pack('<i',loops))
  actual=new.invoke('dh2_animation_complete',[kind&0xffffffff,count,sp,lp])
  expected_step=bytes(old.uc.mem_read(obj+16,4));expected_loops=bytes(old.uc.mem_read(obj+12,4))
  assert actual==result[0] and bytes(new.uc.mem_read(sp,4))==expected_step and bytes(new.uc.mem_read(lp,4))==expected_loops,(i,kind,count,step,loops,result,actual)
  actions[actual]+=1
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'original_instruction_entry':'0x3cb018','bit_exact_decision_step_and_loop_cases':3000,'actions':{'repeat':actions[0],'advance':actions[1],'finish':actions[2]},'scope':'isolated original completion decision instructions; character event dispatch no-op; stops before selection, event logging and parent unwind; full animation/GPU parity not claimed'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
