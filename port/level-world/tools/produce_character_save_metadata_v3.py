from pathlib import Path
exec(Path(__file__).with_name('produce_level_savegame_backend_v2.py').read_text().split('records=[]')[0])
from unicorn.arm_const import UC_ARM_REG_R4
out=root/'port/level-world/reference/object-save-restore-v3'
storage=cpu.data+0x30000
uc.mem_write(storage,bytes([0xa5])*0x1800)
cpu.write_reg(0,storage);cpu.write_reg(1,0)
uc.reg_write(cpu.sp_reg,cpu.stack+0xe000)
uc.emu_start(0x3df084,0x3df0dc,count=100)
tags=[word(storage+offset) for offset in [8,0x38c,0x710,0xa94]]
assert tags==[0x96bbc0]*4,tags
poses=[]
for first,last in [(0x3aa46c,0x3aa49c),(0x3a95f8,0x3a9628)]:
 uc.mem_write(storage,bytes([0xa5])*0x1800)
 uc.reg_write(UC_ARM_REG_R4,storage);cpu.write_reg(3,0)
 uc.emu_start(first,last,count=100)
 raw=bytes(uc.mem_read(storage+0x1468,24))
 assert raw==bytes(24),raw.hex()
 poses.append({'entry':hex(first),'end':hex(last),'respawn_bytes':raw.hex()})
(out/'metadata-constructor-original.json').write_text(json.dumps({'status':'PASS','original_elf_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'tags':tags,'poses':poses,'scope':'CharProperties C2 prefix executes its actual literal tag producer and all four raw sheet header stores. Character C1/C2 respawn field-store blocks execute with the actual preceding mov r3,0 value; other constructor dependencies are not claimed.'},indent=2)+'\n')
print('Original property tags and both Character respawn constructor store blocks PASS')
