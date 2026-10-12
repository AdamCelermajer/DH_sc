from pathlib import Path
import sys,json,struct
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
r=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(r/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_SP
c=Cpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});obj=c.data+0x1000;root=c.data+0x2000;vt=c.data+0x3000;calls=[]
def word(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
def hook(uc,a,size,user):
 if a==0x474cac:
  calls.append([c.reg(i) for i in range(4)]+[word(uc.reg_read(UC_ARM_REG_SP))]);uc.reg_write(c.pc,uc.reg_read(c.lr))
c.uc.hook_add(UC_HOOK_CODE,hook)
for ctor in [0x474d30,0x474e44]:
 c.uc.mem_write(obj,bytes(64));c.pointer(root,vt+12);c.pointer(vt,0);c.pointer(root+4,1);c.invoke(ctor,[obj,root,0]);assert word(obj+4)==root and word(root+4)==2
 assert calls[-1][0]==obj and calls[-1][2]==obj and calls[-1][4]==obj
 for fn in [calls[-1][1],calls[-1][3]]:assert bytes(c.uc.mem_read(fn,4))==bytes.fromhex('1eff2fe1'),hex(fn)
report={'status':'PASS','constructors':['AnimControllerC1 474d30','C2 474e44'],'root_reference_after':2,'SetCallbacksOnAll_args':calls,'callback_bodies':'Both exact source endpoints bx lr; return bool in native adapter means delivered, not a fabricated original result','scope':'Original whole normal non-null-root bool=false C1/C2; SetCallbacksOnAll service interception only, root reference mutation executes.'}
(r/'port/level-world/reference/destructible-container-v21/controller-ctor-original.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
