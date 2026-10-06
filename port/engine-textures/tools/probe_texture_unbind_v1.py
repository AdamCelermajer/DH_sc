from pathlib import Path
import sys,runpy,json,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import *
root=Path(__file__).resolve().parents[3]
d=runpy.run_path(str(root/'port/engine-textures/tools/probe_texture_parameters_v1.py'))
u=d['u'];obj=d['obj'];driver=d['driver'];stop=d['stop'];data=driver+0x1000;events=[]
def w(a,v):u.mem_write(a,struct.pack('<I',v))
def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
def hook(uc,a,n,x):
 if a==0x30e8ec:
  assert all(word(driver+0x420+i*4)!=obj for i in range(4))
  events.append(['delete',word(uc.reg_read(UC_ARM_REG_R1))]);uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
u.hook_add(UC_HOOK_CODE,hook);rows=[]
for auto in (False,True):
 for mips in (1,10,32,33):
  for dirty in (0,0x1ffd):
   events.clear();w(obj+0x34,driver);w(obj+0x38,14<<4);w(obj+0x54,77);w(obj+0x30,data)
   u.mem_write(obj+0x3e,bytes([mips]));u.mem_write(obj+0x3f,bytes([24|int(auto)*2]));u.mem_write(obj+0x40,struct.pack('<H',dirty))
   w(driver+0x4c,4);w(driver+0x84,0);u.mem_write(driver+0x420,bytes(128))
   for i in (0,3):w(driver+0x420+i*4,obj)
   u.mem_write(data,bytes(256));pending=data+(mips+1)*4
   u.reg_write(UC_ARM_REG_R0,obj);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop);u.emu_start(0x5b28dc,stop,count=20000)
   flags=u.mem_read(obj+0x3f,1)[0];after=struct.unpack('<H',u.mem_read(obj+0x40,2))[0]
   expected=[1]+[0]*((mips+31)//32-1) if auto else [0xffffffff]*((mips+31)//32)
   actual=[word(pending+i*4) for i in range(len(expected))]
   assert flags==int(auto)*2 and after==((dirty&~2)|0x1ffd) and word(obj+0x54)==0 and actual==expected and word(driver+0x84)==0
   rows.append(dict(auto=auto,mips=mips,dirty_before=dirty,dirty_after=after,flags_after=flags,pending=actual,events=list(events)))
(root/'port/engine-textures/reference/texture-owner-v1/unbind-original-oracle.json').write_text(json.dumps(dict(cases=rows,scope='16 whole original ARM unbindImpl5b28dc 2D cases; actual GL delete boundary fixture'),indent=2)+'\n')
print('PASS',len(rows),'original ARM unbind cases')
