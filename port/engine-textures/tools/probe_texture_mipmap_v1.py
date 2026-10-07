from pathlib import Path
import sys,runpy,json,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import *
root=Path(__file__).resolve().parents[3];d=runpy.run_path(str(root/'port/engine-textures/tools/probe_texture_parameters_v1.py'))
u=d['u'];obj=d['obj'];driver=d['driver'];stop=d['stop'];calls=d['calls'];events=[]
def w(a,v):u.mem_write(a,struct.pack('<I',v))
def hook(uc,a,n,x):
 if a in (0x5b26f0,0x30e1e4,0x30e370):
  events.append([hex(a),uc.reg_read(UC_ARM_REG_R0),uc.reg_read(UC_ARM_REG_R1),uc.reg_read(UC_ARM_REG_R2),uc.reg_read(UC_ARM_REG_R3)]);uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
u.hook_add(UC_HOOK_CODE,hook);rows=[]
for target in range(4):
 for fi in range(6):
  for auto in (False,True):
   for active in (0,3):
    calls.clear();events.clear();w(obj+0x34,driver);w(obj+0x38,target|(fi<<12));u.mem_write(obj+0x3f,bytes([2 if auto else 0]));u.mem_write(obj+0x40,b'\0\0');w(driver+0x4c,4);w(driver+0x268,active)
    u.reg_write(UC_ARM_REG_R0,obj);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop);u.emu_start(0x5b280c,stop,count=10000)
    selected=[0xde1,0x806f,0x8513,0x84f5][target]
    assert events[0][0]=='0x5b26f0' and events[0][2:]==[3,obj,target]
    assert any(x[0]=='0x30e1e4'for x in events)==(active!=3)
    assert any(x[0]=='0x30e370'and x[1]==selected for x in events)
    assert calls==([] if fi>1 else [[selected,0x2801,0x2700],[selected,0x2801,[0x2600,0x2601][fi]]])
    dirty=struct.unpack('<H',u.mem_read(obj+0x40,2))[0];assert dirty==(0 if auto else 2)
    assert struct.unpack('<I',u.mem_read(driver+0x268,4))[0]==3
    rows.append(dict(kind=target,filter=fi,auto=auto,active_before=active,active_after=3,dirty_after=dirty,events=list(events),parameter_calls=list(calls)))
(root/'port/engine-textures/reference/texture-owner-v1/mipmap-original-oracle.json').write_text(json.dumps(dict(cases=rows,scope='96 original ARM whole generateMipmapsImpl5b280c executions; bindTexture and GL calls explicit provider boundaries, GPU not executed'),indent=2)+'\n');print('PASS',len(rows),'original ARM mipmap cases')
