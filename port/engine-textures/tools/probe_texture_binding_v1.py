from pathlib import Path
import sys,runpy,json,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import *
root=Path(__file__).resolve().parents[3];d=runpy.run_path(str(root/'port/engine-textures/tools/probe_texture_parameters_v1.py'));u=d['u'];obj=d['obj'];driver=d['driver'];stop=d['stop'];events=[]
def w(a,v):u.mem_write(a,struct.pack('<I',v))
def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
def hook(uc,a,n,x):
 if a in (0x30e1e4,0x30e7c0,0x5b044c,0x5fde9c):
  events.append([hex(a),uc.reg_read(UC_ARM_REG_R0),uc.reg_read(UC_ARM_REG_R1)]);uc.reg_write(UC_ARM_REG_R0,1);uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
u.hook_add(UC_HOOK_CODE,hook);rows=[]
for kind in range(4):
 for unit,old,new,dirty,online,active in [(4,0,obj,0,True,0),(3,0,0,0,True,0),(3,0,obj,0,True,0),(3,0,obj,0,True,3),(3,obj,obj,0,True,0),(3,obj,obj,2,True,0),(3,obj,obj,1,True,0),(3,obj,0,0,True,0),(3,0,obj,1,False,0)]:
  events.clear();slot=driver+0x420+kind*32+unit*4;w(slot,old);w(driver+0x4c,4);w(driver+0x268,active);w(driver+0x84,0);w(obj+0x54,99);u.mem_write(obj+0x3f,bytes([8 if online else 0]));u.mem_write(obj+0x40,struct.pack('<H',dirty))
  u.reg_write(UC_ARM_REG_R0,driver);u.reg_write(UC_ARM_REG_R1,unit);u.reg_write(UC_ARM_REG_R2,new);u.reg_write(UC_ARM_REG_R3,kind);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop);u.emu_start(0x5b26f0,stop,count=10000)
  result=u.reg_read(UC_ARM_REG_R0);changed=new!=old
  assert result==(0 if unit>=4 else 1)
  if unit<4:
   assert word(slot)==new and word(driver+0x84)==int(changed and bool(new))
   if changed and new:assert events[-1][0]==('0x5b044c'if online else '0x5fde9c')
   elif new and dirty&0x1ffd:assert events[-1][0]=='0x5b044c'
   else:assert not events
  rows.append(dict(kind=kind,unit=unit,old=old,new=new,dirty=dirty,online=online,active_before=active,active_after=word(driver+0x268),counter_after=word(driver+0x84),result=result,events=list(events)))
(root/'port/engine-textures/reference/texture-owner-v1/binding-original-oracle.json').write_text(json.dumps(dict(cases=rows,scope='36 original ARM whole setTexture5b26f0 control cases; GL/bind/update receiver continuations explicit fixtures, online-clean port domain only production-supported'),indent=2)+'\n');print('PASS',len(rows),'original ARM texture binding cases')
