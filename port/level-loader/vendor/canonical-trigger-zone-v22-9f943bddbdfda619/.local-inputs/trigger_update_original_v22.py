from pathlib import Path
import sys,json,struct
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
r=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(r/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});obj=c.data+0x1000;manager=c.data+0x5000;player=c.data+0x8000;character=c.data+0xa000;calls=[];touch=0;players=1
def word(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
def returned(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,a,size,user):
 if a==0x36e478:returned(player)
 elif a==0x7fd794:returned(c.data+0x6000)
 elif a==0x398838:calls.append('TriggerUpdate');returned()
 elif a==0x3987f8:returned(touch)
 elif a==0x39b2a0:calls.append(['Script',c.reg(1)]);returned()
 elif a==0x39b354:calls.append('ShowMarker');returned()
 elif a==0x39b2f4:calls.append('HideMarker');returned()
c.uc.hook_add(UC_HOOK_CODE,hook)
# Source GOT Application singleton: calculate literal relocation at39b744/748.
base=(word(0x39b744)+0x39b468+8)&0xffffffff;got=base+word(0x39b748);app=word(got);c.pointer(app+0x40,manager);c.pointer(player+0x660,character)
rows=[]
for touch,players,all_script,on,off in [(0,1,-1,10,11),(1,1,-1,10,11),(0,2,20,10,11),(1,2,20,10,11),(2,2,20,10,11)]:
 for active,one in [(0,0),(1,1)]:
  calls.clear();c.uc.mem_write(obj,bytes(0x800));c.pointer(obj+0x8a,1);c.pointer(manager+0x6c4,players);c.uc.mem_write(c.data+0x6000,bytes(16));c.pointer(obj+0x3a8,1);c.pointer(obj+0x73c,on);c.pointer(obj+0x758,off);c.pointer(obj+0x774,all_script&0xffffffff);c.pointer(obj+0x790,0xffffffff);c.uc.mem_write(obj+0x7b4,bytes([active,one]));c.invoke(0x39b458,[obj]);rows.append({'touching':touch,'players':players,'all_script':all_script,'active_before':active,'one_before':one,'active_after':c.uc.mem_read(obj+0x7b4,1)[0],'one_after':c.uc.mem_read(obj+0x7b5,1)[0],'activation_count':word(obj+0x3b4),'calls':list(calls)})
report={'status':'CAPTURED','cases':len(rows),'rows':rows,'scope':'Whole original TriggerZone.Update decision/store sequence over offline/local-only=false domain; actual PlayerManager/Online/TriggerUpdate/touch count/Script/Marker are service observers. Not an application acceptance.'}
(r/'port/level-world/reference/trigger-zone-v22/update-original.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
