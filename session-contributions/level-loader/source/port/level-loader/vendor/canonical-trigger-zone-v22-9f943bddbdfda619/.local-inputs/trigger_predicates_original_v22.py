from pathlib import Path
import sys,json,struct
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
r=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(r/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});obj=c.data+0x1000;dt=0;called=0;rows=[];timers=[]
def hook(uc,a,size,user):
 global called
 if a==0x31f66c:called+=1;c.put(0,dt);uc.reg_write(c.pc,uc.reg_read(c.lr))
c.uc.hook_add(UC_HOOK_CODE,hook)
for count in [-1,0,1,2,2147483647]:
 for activated in [-1,0,1,2]:
  for timer in [-1,0,1]:
   for enabled in [0,1]:
    c.uc.mem_write(obj,bytes(0x800));c.pointer(obj+0x3a8,count&0xffffffff);c.pointer(obj+0x3b4,activated&0xffffffff);c.pointer(obj+0x3b8,timer&0xffffffff);c.uc.mem_write(obj+0x8a,bytes([enabled]));result=c.invoke(0x3987ac,[obj]);rows.append([count,activated,timer,enabled,result])
for timer in [-1,0,1,2147483647]:
 for dt in [0,1,5]:
  called=0;c.pointer(obj+0x3b8,timer&0xffffffff);c.invoke(0x3987fc,[obj]);after=struct.unpack('<i',c.uc.mem_read(obj+0x3b8,4))[0];timers.append([timer,dt,after,called])
report={'status':'CAPTURED','can_activate':rows,'update_timer':timers,'scope':'Whole source CanActivate3987ac (120 cases) and UpdateTimer3987fc (12 cases); App delta getter service observer only.'}
(r/'port/level-world/reference/trigger-zone-v22/predicates-original.json').write_text(json.dumps(report,indent=2))
(r/'port/level-world/tests/trigger_predicates_original_v22.inc').write_text('\n'.join('predicate_case('+','.join(map(str,row))+');' for row in rows)+'\n'+'\n'.join('timer_case('+','.join(map(str,row))+');' for row in timers));print('source predicates',len(rows),'timers',len(timers))
