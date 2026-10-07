from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM
from unicorn.arm_const import *
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so';u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,0xc00000);u.mem_map(0x1000000,0x200000)
with original.open('rb')as f:
 for p in ELFFile(f).iter_segments():
  if p['p_type']=='PT_LOAD':u.mem_write(p['p_vaddr'],p.data())
actor=0x1100000;u.mem_write(actor,b'\xa5'*0x1600);u.reg_write(UC_ARM_REG_R4,actor);u.reg_write(UC_ARM_REG_R3,0);u.reg_write(UC_ARM_REG_R8,0);u.reg_write(UC_ARM_REG_R6,0xffffffff)
# Exact ancestor constant stores: r3=0 at3a9584, r8=0 at3a93cc,
# r6=-1 at3a93d8. This executes the source pending-field ctor block only.
u.emu_start(0x3a96b4,0x3a96f4,count=1000)
points=struct.unpack('<ffffff',u.mem_read(actor+0x14b0,24));pending=u.mem_read(actor+0x14c8,1)[0];skill=struct.unpack('<h',u.mem_read(actor+0x14ca,2))[0]
assert points==(0,0,0,0,0,0)and pending==0 and skill==-1
# Whole actual CharAI constructor prefix through field stores; queue insertion
# starting3ced00 is outside this isolated field proof and is not claimed.
u.reg_write(UC_ARM_REG_R0,actor+0x3c8);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,0x1000000);u.emu_start(0x3cebf0,0x3ced00,count=1000)
clicked=u.mem_read(actor+0x413,1)[0];assert clicked==1
report={'original_sha256':hashlib.sha256(original.read_bytes()).hexdigest(),'actual_source_ranges':['3a96b4..3a96f0 pending fields','3cebf0..3cecfc CharAI ctor field prefix'],'entry_register_ancestor_proofs':{'r3':'0 at3a9584','r8':'0 at3a93cc','r6':'-1 at3a93d8'},'points':points,'pending14c8':pending,'skill14ca':skill,'click413':clicked,'whole_Character_ctor_verified':False,'CharAI_global_queue_insertion_verified':False}
(root/'port/level-world/reference/world-touch-target-v1/constructor-original.json').write_text(json.dumps(report,indent=2)+'\n');print('PASS actual source pending-field constructor writes and CharAI click413=1 prefix')
