from pathlib import Path
import sys,json,hashlib,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_LR,UC_ARM_REG_PC,UC_ARM_REG_SP
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so';ref=root/'port/level-world/reference/canonical-object-factory-v1'
results=[]
for source_type in (0,7,20):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,0xa00000);u.mem_map(0x1000000,0x200000)
 with original.open('rb') as f:
  elf=ELFFile(f)
  for p in elf.iter_segments():
   if p['p_type']=='PT_LOAD':u.mem_write(p['p_vaddr'],p.data())
 actor=0x1100000;handle=0x1110000;stop=0x1000000;calls=[]
 u.mem_write(actor,b'\xa5'*0x120);u.mem_write(handle,b'\xa5'*12)
 def hook(u,address,size,data):
  if address==stop:u.emu_stop();return
  if address in (0x31167c,0x33ed7c,0x310570):
   calls.append(dict(address=hex(address),argument=hex(u.reg_read(UC_ARM_REG_R0))))
   if address==0x310570:u.reg_write(UC_ARM_REG_R0,handle)
   u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_ARM_REG_R0,actor);u.reg_write(UC_ARM_REG_R1,source_type);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop)
 u.emu_start(0x33f15c,stop,count=10000)
 def word(offset):return struct.unpack('<I',u.mem_read(actor+offset,4))[0]
 fields={hex(o):u.mem_read(actor+o,1)[0]for o in(0x80,0x81,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0xf0,0xf1)}
 actual_handle=struct.unpack('<III',u.mem_read(handle,12))
 assert word(0x64)==0xffffffff and word(0xf4)==source_type and word(0x2c)==handle
 assert actual_handle==(0,actor,0xffffffff)
 assert fields['0x80']==0xa5 and fields['0x83']==0xa5 and fields['0x87']==0xa5
 assert fields['0x8a']==1 and fields['0x84']==0 and fields['0x81']==0
 results.append(dict(source_type=source_type,fields=fields,shared_handle=list(actual_handle),calls=calls))
 report=dict(original_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),entry='0x33f15c',cases=results,
  fixture_services=['CString reserve31167c retaining constructor inline pointer','Condition constructor33ed7c (condition fields excluded)','operator new310570 returns retained 12-byte allocation'],
  actual_source_leaf='ObjectHandle constructor33f50c executed',production_factory_verified=False)
 (ref/'object-base-constructor-original.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS three actual ObjectBase source constructor cases; fixture services declared')
