"""Execute original dispatcher and progress tail, without supplying stage results.
External stage bodies are deliberately outside this probe; it stops at each
original dispatch target. Progress arithmetic/min counters execute original
instructions; menu/license/global callbacks after the tail do not execute.
"""
from pathlib import Path
import sys,struct,json,hashlib,argparse
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
shared=Path(r'C:/Users/adamc/Desktop/workspace/DH_sc');sys.path.insert(0,str(shared/'port/game-data/tests'))
from items_differential import Original
from unicorn import UC_HOOK_CODE
p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,default=shared/'.local-inputs/libDungeonHunter2.so');p.add_argument('--output',type=Path,default=Path(__file__).parent);a=p.parse_args()
c=Original(a.engine,{'functions':[]});obj=c.data+0x4000
W=lambda *v:struct.pack('<'+'I'*len(v),*(n&0xffffffff for n in v))
entries=[]
for state in range(38):
 at=0x3f6a38+state*4;insn=c.word(at);assert insn>>24==0xea
 imm=insn&0xffffff;imm=imm-(1<<24) if imm&(1<<23) else imm
 entries.append(at+8+4*imm)
targets=set(entries);mode='dispatch';observed=[]
def hook(uc,address,size,unused):
 if mode=='dispatch' and address in targets:observed.append(address);uc.reg_write(c.pc,c.stop);uc.emu_stop()
 elif mode=='progress' and address in (0x3f6ef8,0x3f6f6c):uc.reg_write(c.pc,c.stop);uc.emu_stop()
c.uc.hook_add(UC_HOOK_CODE,hook)
for state,entry in enumerate(entries):
 c.uc.mem_write(obj,bytes(0x200));c.pointer(obj+0x130,state);c.put(4,obj);c.invoke(0x3f6a28,[],budget=1000);assert observed[-1]==entry
mode='progress';rows=[]
for state in range(39):
 for counter,current in ((0,500),(500,0),(-1,17),(17,-1),(2147483647,-2147483648)):
  c.uc.mem_write(obj,bytes(0x200));c.pointer(obj+0x130,state);c.pointer(obj+0x134,counter&0xffffffff);c.pointer(obj+0x138,current&0xffffffff);c.put(4,obj)
  c.invoke(0x3f6e9c,[],budget=1000)
  result=tuple(c.word(obj+off) for off in (0x130,0x30,0x134,0x138));rows.append((state,counter&0xffffffff,current&0xffffffff,*result))
gold=b'L36G'+W(len(entries),len(rows))+W(*entries)+b''.join(W(*row) for row in rows)
a.output.mkdir(parents=True,exist_ok=True);(a.output/'lifecycle_v36_original_gold.bin').write_bytes(gold)
report={'validation':'PASS','original_sha256':hashlib.sha256(a.engine.read_bytes()).hexdigest(),'dispatch_cases':len(entries),'progress_tail_cases':len(rows),'stage_entries':[hex(v) for v in entries],'gold_sha256':hashlib.sha256(gold).hexdigest(),'scope':__doc__,'whole_level_init_verified':False}
(a.output/'lifecycle_v36_original.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('stage_entries','scope')}))
