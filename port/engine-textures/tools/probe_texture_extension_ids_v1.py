from pathlib import Path
import runpy,struct,json,sys
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import *
root=Path(__file__).resolve().parents[3]
d=runpy.run_path(str(root/'port/engine-textures/tools/probe_texture_parameters_v1.py'))
u=d['u'];stop=d['stop'];address=0x1120000
def string(a):
 out=b''
 while True:
  b=bytes(u.mem_read(a,1))
  if b==b'\0':return out
  out+=b;a+=1
def hook(uc,a,n,x):
 if a==0x30e31c:
  left=string(uc.reg_read(UC_ARM_REG_R0));right=string(uc.reg_read(UC_ARM_REG_R1))
  uc.reg_write(UC_ARM_REG_R0,0 if left==right else (1 if left>right else 0xffffffff));uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
u.hook_add(UC_HOOK_CODE,hook);rows=[]
for name in ('GL_IMG_texture_compression_pvrtc','GL_EXT_texture_compression_s3tc','GL_EXT_texture_filter_anisotropic','GL_OES_texture_npot','GL_OES_texture_3D','GL_OES_rgb8_rgba8'):
 u.mem_write(address,name.encode()+b'\0');u.reg_write(UC_ARM_REG_R0,address);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop);u.emu_start(0x6dd6dc,stop,count=20000)
 value=u.reg_read(UC_ARM_REG_R0);rows.append(dict(name=name,source_id=value,driver_word_offset=hex(0x7b8+(value//32)*4) if value!=65535 else None,bit=hex(1<<(value%32)) if value!=65535 else None))
u.reg_write(UC_ARM_REG_R0,0);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop);u.emu_start(0x6de848,stop,count=1000);table=u.reg_read(UC_ARM_REG_R0)
for index in (87,392,435,437):
 name=string(struct.unpack('<I',u.mem_read(table+index*4,4))[0]).decode();rows.append(dict(name=name,source_id=index,driver_word_offset=hex(0x7b8+(index//32)*4),bit=hex(1<<(index%32))))
(root/'port/engine-textures/reference/texture-owner-v1/extension-original-oracle.json').write_text(json.dumps(rows,indent=2)+'\n');print(json.dumps(rows,indent=2))
