from pathlib import Path
import sys,runpy,json,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import *
root=Path(__file__).resolve().parents[3];d=runpy.run_path(str(root/'port/engine-textures/tools/probe_texture_parameters_v1.py'));u=d['u'];obj=d['obj'];driver=d['driver'];stop=d['stop'];events=[];name_result=0;kind=0
def w(a,v):u.mem_write(a,struct.pack('<I',v))
def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
def hook(uc,a,n,x):
 if a==0x30e934:
  assert u.mem_read(obj+0x3f,1)[0]&0x10==0;w(uc.reg_read(UC_ARM_REG_R1),name_result);events.append(['gen',name_result]);uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
 elif a==0x30e1e4:
  events.append(['active',uc.reg_read(UC_ARM_REG_R0)]);uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
 elif a==0x30e7c0:
  active=word(driver+0x268);assert word(driver+0x420+kind*32+active*4)==obj
  assert not u.mem_read(obj+0x3f,1)[0]&8;events.append(['bind',uc.reg_read(UC_ARM_REG_R0),uc.reg_read(UC_ARM_REG_R1),active]);uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
 elif a==0x5b044c:
  assert u.mem_read(obj+0x3f,1)[0]&8;assert word(driver+0x84)==0
  events.append(['update',bool(uc.reg_read(UC_ARM_REG_R1)),u.mem_read(obj+0x3f,1)[0]]);u.mem_write(obj+0x40,b'\0\0');uc.reg_write(UC_ARM_REG_R0,1);uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
u.hook_add(UC_HOOK_CODE,hook);rows=[]
for fmt in (14,27):
 for name_result in (0,77):
  for active_before,current_contains in ((0,False),(3,False),(2,True)):
   events.clear();w(obj+0x34,driver);w(obj+0x38,(fmt<<4)|(3<<12)|(1<<15));w(obj+0x54,0);u.mem_write(obj+0x58,b'\0');u.mem_write(obj+0x3e,b'\12');u.mem_write(obj+0x3f,b'\22');u.mem_write(obj+0x40,struct.pack('<H',0x1ffd));w(driver+0x4c,4);w(driver+0x268,active_before);w(driver+0x84,0);u.mem_write(driver+0x420,bytes(128))
   if current_contains:w(driver+0x420+active_before*4,obj)
   u.reg_write(UC_ARM_REG_R0,obj);u.reg_write(UC_ARM_REG_R1,0);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop);u.emu_start(0x5b5610,stop,count=20000)
   flags=u.mem_read(obj+0x3f,1)[0];result=u.reg_read(UC_ARM_REG_R0);assert word(driver+0x84)==0
   if name_result:assert flags==10 and result==1 and events[-1][0]=='update'
   else:assert flags==18 and result==0 and events==[['gen',0]]
   rows.append(dict(format=fmt,generated_name=name_result,active_before=active_before,current_contains=current_contains,active_after=word(driver+0x268),flags_after=flags,counter_after=word(driver+0x84),events=list(events),result=result))
(root/'port/engine-textures/reference/texture-owner-v1/initial-bind-original-oracle.json').write_text(json.dumps(dict(cases=rows,scope='12 original ARM bindImpl initial control cases, sourceGLgen/bind/cache/directgrid/online-before-update ordering; update(true) continuation fixture, GPU not executed'),indent=2)+'\n');print('PASS',len(rows),'original ARM initial bind cases')
