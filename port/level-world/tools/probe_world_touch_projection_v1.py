from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so';rows=[]
for hit_floor in (0,0x1103000,0x1104000):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,0xc00000);u.mem_map(0x1000000,0x300000)
 with original.open('rb')as f:
  elf=ELFFile(f)
  for p in elf.iter_segments():
   if p['p_type']=='PT_LOAD':u.mem_write(p['p_vaddr'],p.data())
  for section in elf.iter_sections():
   if section['sh_type']not in('SHT_REL','SHT_RELA'):continue
   symbols=elf.get_section(section['sh_link'])
   for r in section.iter_relocations():
    value=symbols.get_symbol(r['r_info_sym'])['st_value'];a=r['r_offset'];kind=r['r_info_type']
    if value and kind in(2,21,22):u.mem_write(a,struct.pack('<I',(value+(struct.unpack('<I',u.mem_read(a,4))[0]if kind==2 else 0))&0xffffffff))
 def w(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 world=0x1100000;room1=0x1101000;room2=0x1102000;floor1=0x1103000;floor2=0x1104000;masked=0x1105000;vectors=0x1110000;screen=0x1120000;point=0x1120100;calls=[]
 w(world+8,vectors);w(world+0xc,vectors+8);w(vectors,room1);w(vectors+4,room2)
 w(room1+0x30,vectors+16);w(room1+0x34,vectors+24);w(vectors+16,masked);w(vectors+20,floor1)
 w(room2+0x30,vectors+32);w(room2+0x34,vectors+36);w(vectors+32,floor2);w(masked+0x24,0x01000000)
 # Actual source Application/device/SceneManager collision-manager chain;
 # only its virtual camera query is a declared external service fixture.
 base=0x52589c+word(0x52593c);application=word(base+word(0x525940));device=0x1130000;scene=0x1130100;manager=0x1130200;table=0x1130300
 w(application+0x10,device);w(device+0x1c,scene);w(scene+0x2c,manager);w(manager,table);w(table+0x14,0x1001014)
 u.mem_write(screen,struct.pack('<ff',10.9,-20.9));u.mem_write(point,struct.pack('<fff',99,99,99));stop=0x1000000
 def ret(v=None):
  if v is not None:u.reg_write(UC_ARM_REG_R0,v)
  u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(u,a,size,data):
  r0=u.reg_read(UC_ARM_REG_R0)
  if a==stop:u.emu_stop();return
  if a==0x30e4cc:ret(int(struct.unpack('<f',struct.pack('<I',r0))[0])&0xffffffff);return
  if a==0x1001014:
   xy=struct.unpack('<ii',u.mem_read(u.reg_read(UC_ARM_REG_R2),8));assert xy==(10,-20);calls.append(['camera',*xy]);u.mem_write(r0,struct.pack('<ffffff',1,2,3,4,5,6));ret();return
  if a==0x51b874:
   assert struct.unpack('<fff',u.mem_read(u.reg_read(UC_ARM_REG_R1),12))==(1,2,3);calls.append(['floor',hex(r0)]);
   if r0==hit_floor:u.mem_write(u.reg_read(UC_ARM_REG_R3),struct.pack('<fff',7,8,9))
   ret(int(r0==hit_floor));return
 u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_ARM_REG_R0,world);u.reg_write(UC_ARM_REG_R1,screen);u.reg_write(UC_ARM_REG_R2,point);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop);u.emu_start(0x525884,stop,count=10000)
 actual=u.reg_read(UC_ARM_REG_R0);out=struct.unpack('<fff',u.mem_read(point,12));assert actual==bool(hit_floor)and out==((7,8,9)if hit_floor else(99,99,99));assert ['floor',hex(masked)]not in calls
 if hit_floor==floor1:assert ['floor',hex(floor2)]not in calls
 rows.append({'hit_floor':hex(hit_floor),'result':actual,'point':out,'calls':calls})
(root/'port/level-world/reference/world-touch-target-v1/projection-original.json').write_text(json.dumps({'original_sha256':hashlib.sha256(original.read_bytes()).hexdigest(),'cases':rows,'actual_executed':['TranslateScreenToWorld525884','PFWorldGetCollisionAt525800','PFRoomGetCollisionAt5212e0'],'fixture_services':['camera virtual ScreenRay','PFFloor triangleSelector raycast51b874','__aeabi_f2iz finite in-range primitives'],'whole_camera_floor_renderer_verified':False},indent=2)+'\n');print('PASS three original screen/PF room/floor first-hit/filter/truncation cases')
