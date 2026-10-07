from pathlib import Path
import sys,struct,json,hashlib
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so'
rows=[]
with original.open('rb') as f:
 e=ELFFile(f);segments=[(p['p_vaddr'],p.data()) for p in e.iter_segments() if p['p_type']=='PT_LOAD'];relocations=[]
 for sec in e.iter_sections():
  if sec['sh_type']!='SHT_REL':continue
  syms=e.get_section(sec['sh_link'])
  for rel in sec.iter_relocations():
   v=syms.get_symbol(rel['r_info_sym'])['st_value'];a=rel['r_offset'];k=rel['r_info_type']
   if v and k in (2,21,22):relocations.append((a,v,k))
for target in range(4):
 for fi in range(6):
  for wrap in range(8):
   u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,0xc00000);u.mem_map(0x1000000,0x300000)
   for a,raw in segments:u.mem_write(a,raw)
   for a,v,k in relocations:u.mem_write(a,struct.pack('<I',(v+(struct.unpack('<I',u.mem_read(a,4))[0] if k==2 else 0))&0xffffffff))
   obj=0x1100000;driver=0x1110000;stop=0x1000000;calls=[]
   def w(a,v):u.mem_write(a,struct.pack('<I',v))
   parameters=target|(fi<<12)|(1<<15)|(wrap<<18)|(wrap<<21)
   w(obj+0x34,driver);w(obj+0x38,parameters);u.mem_write(obj+0x40,struct.pack('<H',0x1ffd));w(driver+0x9c,0x80)
   def hook(uc,a,n,x):
    if a==0x30e910:
     calls.append([uc.reg_read(UC_ARM_REG_R0),uc.reg_read(UC_ARM_REG_R1),uc.reg_read(UC_ARM_REG_R2)])
     uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
   u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_ARM_REG_R0,obj);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop)
   u.emu_start(0x5afd40,stop,count=5000)
   dirty=struct.unpack('<H',u.mem_read(obj+0x40,2))[0]
   expected_target=[0xde1,0x806f,0x8513,0x84f5][target];wf=[0x2901,0x812f,0x812f,0x812f,0x2901,0x1401,0x1403,0][wrap]
   expected=[[expected_target,0x2801,[0x2600,0x2601,0x2700,0x2701,0x2702,0x2703][fi]],[expected_target,0x2800,0x2601],[expected_target,0x2802,wf],[expected_target,0x2803,wf],[expected_target,0x2803,wf]]
   assert calls==expected,(target,fi,wrap,calls,expected)
   assert dirty==1
   rows.append(dict(parameters=parameters,dirty_before=0x1ffd,capabilities=0x80,dirty_after=dirty,calls=calls))
calls.clear();diagnostics=[];format_row=[]
def compressed_hook(uc,a,n,x):
 if a==0x5afe90:
  for fmt in (14,27):format_row.append(dict(format=fmt,row_hex=bytes(uc.mem_read(uc.reg_read(UC_ARM_REG_R1)+fmt*40,40)).hex(),flags=struct.unpack('<I',uc.mem_read(uc.reg_read(UC_ARM_REG_R1)+fmt*40,4))[0]))
 if a==0x60b034:
  diagnostics.append(uc.reg_read(UC_ARM_REG_R0));uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
u.hook_add(UC_HOOK_CODE,compressed_hook)
w(obj+0x38,(27<<4)|(3<<12)|(1<<15));u.mem_write(obj+0x3f,b'\2');u.mem_write(obj+0x40,struct.pack('<H',4));w(driver+0x9c,0)
u.reg_write(UC_ARM_REG_R0,obj);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop);u.emu_start(0x5afd40,stop,count=5000)
assert calls==[[0xde1,0x2801,0x2600]] and diagnostics==[3]
assert (struct.unpack('<I',u.mem_read(obj+0x38,4))[0]>>12)&7==0
report=dict(original_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),cases=rows,compressed_pvrtc4_rgba=dict(engine_format=27,source_row_hex=format_row,calls=calls,diagnostics=diagnostics),scope='193 original ARM updateParameters executions; target/filter/wrap tables, duplicate wrap-T capability branch and actual compressed format27 downgrade; source callbacks are GL recording boundaries, not GPU execution')
(root/'port/engine-textures/reference/texture-owner-v1/parameters-original-oracle.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS',len(rows)+1,'original ARM sampler cases')
