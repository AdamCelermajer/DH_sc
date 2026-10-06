from pathlib import Path
import sys,json,struct
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
r=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(r/'port/game-data/tests'))
from aggro_differential import Cpu
rows=[]
for fill in [0,0xa5]:
 c=Cpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});obj=c.data+0x1000;c.uc.mem_write(obj,bytes([fill])*0x1a8);c.invoke(0x398da4,[obj]);word=lambda o:struct.unpack('<I',c.uc.mem_read(obj+o,4))[0]
 assert word(0x104)==3 and [word(4+i*4) for i in range(3)]==[obj+0x130,obj+0x158,obj+0x180]
 members=[]
 for off in [0x130,0x158,0x180]:
  x={'type':word(off+4),'stamp':struct.unpack('<Q',c.uc.mem_read(obj+off+8,8))[0],'previous':word(off+0x10),'current':word(off+0x14),'raw18':word(off+0x18),'changed':c.uc.mem_read(obj+off+0x1c,1)[0],'value':word(off+0x20)}
  assert x['type']==32 and x['value']==0 and x['changed']==int(fill!=0)
  members.append(x)
 rows.append({'allocation_fill':fill,'member_order':[hex(o) for o in [0x130,0x158,0x180]],'members':members,'imports':c.import_calls})
report={'status':'PASS','cases':2,'rows':rows,'scope':'Whole original NetStructTrigger398da4 including NetStructBase8138f4, DeclareMember81324c, conditional SetChanged814f84 and original global sequence mutation. Imported memset semantics supplied by CPU fixture. Old value memory is genuinely read before source zero store; allocation contents affect changed notifications.'}
(r/'port/level-world/reference/trigger-zone-v22/network-constructor-original.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
