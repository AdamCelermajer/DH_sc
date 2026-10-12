from pathlib import Path
import sys,struct,json,hashlib
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');sys.path.insert(0,str(root/'port/engine-resources/tests'))
from cpu import Cpu
from unicorn import UC_HOOK_CODE
b=Path(__file__).resolve().parents[2];elf=root/'.local-inputs/libDungeonHunter2.so';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();assert sha(elf)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
c=Cpu(elf,False,{'functions':[]});menu=c.data+0x1000;multi=c.data+0x2000;got=c.data+0x3000;app=c.data+0x4000;level=c.data+0x5000;vt=c.data+0x6000;callback=c.data+0x1f000
c.pointer(callback,0xe12fff1e);c.pointer(vt+0x10,callback);c.pointer(menu+0xf4,multi);c.pointer(got,app)
files=[c.data+0x7000+i*0x100 for i in range(4)]
for x in files:c.pointer(x,vt)
observed=[];current=0;lookups=0
def hook(uc,address,size,user):
 global lookups
 if address==0x31f594:
  assert c.reg(0)==app;lookups+=1;c.put(0,current);uc.reg_write(c.pc,uc.reg_read(c.lr))
 elif address==callback:
  observed.append((files.index(c.reg(0)),c.reg(1),c.reg(2)));uc.reg_write(c.pc,uc.reg_read(c.lr))
 elif address==0x42ed2c:uc.emu_stop()
c.uc.hook_add(UC_HOOK_CODE,hook)
cases=[]
for argument in [0,1,255]:
 for mask in [0,8,15,7]:
  for current_mode in ['null','not_ready','ready']:
   current=0 if current_mode=='null' else level;c.uc.mem_write(level+0x198,bytes([int(current_mode=='ready')]))
   for i,x in enumerate(files):c.pointer(multi+0x134+i*4,x if mask&(1<<i) else 0)
   observed.clear();lookups=0;c.uc.reg_write(c.sp,c.stack+0xe000)
   for i,v in [(4,got),(5,menu),(6,0),(10,123),(11,argument)]:c.put(i,v)
   c.uc.emu_start(0x42ecac,c.stop,count=4000)
   wanted=[i for i in range(3 if argument else 0,4) if mask&(1<<i) and not(i==3 and current_mode=='not_ready')]
   assert [x[0] for x in observed]==wanted,(argument,mask,current_mode,observed,wanted)
   assert all(x[1:]==(123,0) for x in observed);assert c.uc.reg_read(c.pc)==0x42ed2c
   cases.append({'argument':argument,'loaded_mask':mask,'current_level':current_mode,'updated_slots':wanted,'GetCurrentLevel_observer_calls':lookups})
receipt={'validation':'PASS','source_elf_sha256':sha(elf),'original_slice':['0x42ecac','0x42ed2c'],'cases':cases,'case_count':len(cases),'Update_true_starts_slot3':True,'Update_false_starts_slot0':True,'not_ready_Level198_skips_slot3':True,'movie_update_r2_is_zero':True,'observers':'GetCurrentLevel and virtual movie.Update are explicit caller/service observers. Original branch/slot loads/loop and profiling functions execute. GetDt prefix/input/HUD/FlashAnim/visible registry suffix are not executed. This is not whole MenuManager.Update.','whole_MenuManager_Update_verified':False,'shared_mutated':False}
(b/'reports/menu-movie-loop-v57-original.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps({k:v for k,v in receipt.items() if k!='cases'}))
