from pathlib import Path
import sys,json,hashlib,struct
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_LR,UC_ARM_REG_PC,UC_ARM_REG_SP
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so'
rows=[]
for poison in (0,1,0xa5,0xff):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,0xa00000);u.mem_map(0x1000000,0x200000)
 with original.open('rb') as f:
  elf=ELFFile(f)
  for p in elf.iter_segments():
   if p['p_type']=='PT_LOAD':u.mem_write(p['p_vaddr'],p.data())
 actor=0x1100000;handle=0x1110000;stop=0x1000000;allocs=[];writes=[]
 u.mem_write(actor,bytes([poison])*0x318)
 def hook(uc,a,n,user):
  if a==stop:uc.emu_stop();return
  if a==0x30e6f4:
   size=uc.reg_read(UC_ARM_REG_R0);assert size in (0x318,12);allocs.append(size)
   uc.reg_write(UC_ARM_REG_R0,actor if size==0x318 else handle);uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
  elif a in (0x31167c,0x33ed7c):uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
 def write(uc,access,a,n,value,user):
  if a<=actor+0x87<a+n:writes.append(dict(pc=hex(uc.reg_read(UC_ARM_REG_PC)),address=hex(a),size=n))
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_WRITE,write)
 u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop)
 u.emu_start(0x340ca8,stop,count=20000)
 assert u.reg_read(UC_ARM_REG_R0)==actor and allocs==[0x318,12]
 assert u.mem_read(actor+0x87,1)[0]==poison and not writes
 assert struct.unpack('<i',u.mem_read(actor+0x64,4))[0]==-1
 rows.append(dict(poison=poison,byte87=u.mem_read(actor+0x87,1)[0],allocations=allocs,writes87=writes))
out=dict(validation='PASS',cases=rows,original_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),scope='Whole original LevelConfig factory340ca8 -> operatornew310570 -> CustomAlloc310454 -> LevelConfigC1/ObjectBaseC2. Explicit malloc poison/CString reserve/ConditionData constructor service boundaries. No byte87 write occurs; result preserves caller malloc contents. No claim malloc naturally returns zero.')
dest=root/'port/level-world/reference/level-config-across-rooms-v4/constructor-original-proof.json';dest.write_text(json.dumps(out,indent=2));print(json.dumps(out))
