from pathlib import Path
import sys,runpy,json,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import *
root=Path(__file__).resolve().parents[3];d=runpy.run_path(str(root/'port/engine-textures/tools/probe_texture_parameters_v1.py'))
u=d['u'];obj=d['obj'];driver=d['driver'];stop=d['stop'];events=[];data=driver+0x1000;vtable=driver+0x2000
def w(a,v):u.mem_write(a,struct.pack('<I',v))
def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
def hook(uc,a,n,x):
 if a==0x30e934:w(uc.reg_read(UC_ARM_REG_R1),77);events.append('generate')
 elif a==0x30e1e4:events.append('active')
 elif a==0x30e7c0:events.append('bind')
 elif a==0x5b044c:
  assert u.mem_read(obj+0x3f,1)[0]&8
  u.mem_write(obj+0x3f,bytes([u.mem_read(obj+0x3f,1)[0]|16]));uc.reg_write(UC_ARM_REG_R0,0);events.append('update_error')
 elif a==0x30e8ec:
  assert all(word(driver+0x420+i*4)!=obj for i in range(4));events.append('delete')
 else:return
 uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
u.hook_add(UC_HOOK_CODE,hook);rows=[]
for fmt in (14,27):
 for active in (0,3):
  events.clear();w(obj,vtable);w(vtable+16,0x5b28dc);w(obj+0x34,driver);w(obj+0x30,data);w(obj+0x38,(fmt<<4)|(3<<12)|(1<<15));w(obj+0x54,0)
  u.mem_write(obj+0x58,b'\0');u.mem_write(obj+0x3e,b'\12');u.mem_write(obj+0x3f,b'\2');u.mem_write(obj+0x40,struct.pack('<H',0x1ffd));w(driver+0x4c,4);w(driver+0x268,active);w(driver+0x84,0);u.mem_write(driver+0x420,bytes(128));u.mem_write(data,bytes(128))
  u.reg_write(UC_ARM_REG_R0,obj);u.reg_write(UC_ARM_REG_R1,0);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop);u.emu_start(0x5b5610,stop,count=20000)
  flags=u.mem_read(obj+0x3f,1)[0];assert flags==18 and word(obj+0x54)==0 and word(data+44)==1 and events[-1]=='delete' and u.reg_read(UC_ARM_REG_R0)==0
  rows.append(dict(format=fmt,active_before=active,flags_after=flags,pending=word(data+44),result=0,events=list(events)))
(root/'port/engine-textures/reference/texture-owner-v1/upload-cleanup-original-oracle.json').write_text(json.dumps(dict(cases=rows,scope='4 original ARM whole bindImpl->real unbindImpl error10 tails, updateData GL-error boundary fixture'),indent=2)+'\n')
print('PASS',len(rows),'original ARM upload cleanup cases')
