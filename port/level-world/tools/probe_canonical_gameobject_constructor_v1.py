from pathlib import Path
import sys,json,hashlib,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE,UC_HOOK_MEM_INVALID
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_LR,UC_ARM_REG_PC,UC_ARM_REG_SP
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so';ref=root/'port/level-world/reference/canonical-object-factory-v1';results=[]
for go_type in(7,20):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,0xc00000);u.mem_map(0x1000000,0x300000)
 with original.open('rb')as f:
  elf=ELFFile(f)
  for p in elf.iter_segments():
   if p['p_type']=='PT_LOAD':u.mem_write(p['p_vaddr'],p.data())
  for section in elf.iter_sections():
   if section['sh_type']not in('SHT_REL','SHT_RELA'):continue
   symbols=elf.get_section(section['sh_link'])
   for relocation in section.iter_relocations():
    kind=relocation['r_info_type'];address=relocation['r_offset'];symbol=symbols.get_symbol(relocation['r_info_sym']);value=symbol['st_value']
    if value and kind in(2,21,22):
     addend=struct.unpack('<I',u.mem_read(address,4))[0]if kind==2 else 0
     u.mem_write(address,struct.pack('<I',(value+addend)&0xffffffff))
 actor=0x1100000;heap=[0x1200000];first_handle=heap[0];stop=0x1000000;services=[];tail=[]
 u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop);u.emu_start(0x312e00,stop,count=200)
 u.mem_write(actor,b'\xa5'*0x374)
 def hook(u,address,size,data):
  tail.append(hex(address));tail[:]=tail[-25:]
  if address<0x300000:raise RuntimeError(str(tail))
  if address==stop:u.emu_stop();return
  if address in(0x31167c,0x33ed7c,0x310570,0x310454,0x310440,0x5244e8,0x30eba4,0x30e5b0):
   services.append(hex(address))
   if address in(0x310570,0x310454):
    size=u.reg_read(UC_ARM_REG_R0);u.reg_write(UC_ARM_REG_R0,heap[0]);heap[0]+=(size+0xff)&~0xff
   elif address==0x30e5b0:
    destination=u.reg_read(UC_ARM_REG_R0);value=u.reg_read(UC_ARM_REG_R1)&255;length=u.reg_read(UC_ARM_REG_R2);u.mem_write(destination,bytes([value])*length)
   elif address==0x5244e8:
    p=u.reg_read(UC_ARM_REG_R0);u.mem_write(p+0x10,struct.pack('<2I',p,p));u.mem_write(p,b'\0')
   elif address==0x30eba4:
    a,b=struct.unpack('<2f',struct.pack('<2I',u.reg_read(UC_ARM_REG_R0),u.reg_read(UC_ARM_REG_R1)));u.reg_write(UC_ARM_REG_R0,struct.unpack('<I',struct.pack('<f',a+b))[0])
   u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def invalid(u,access,address,size,value,data):print('invalid',hex(address),tail,services);return False
 u.hook_add(UC_HOOK_MEM_INVALID,invalid);u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_ARM_REG_R0,actor);u.reg_write(UC_ARM_REG_R1,go_type);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop)
 u.emu_start(0x38c398,stop,count=20000)
 def word(o):return struct.unpack('<I',u.mem_read(actor+o,4))[0]
 def floats(o,n):return list(struct.unpack('<'+'f'*n,u.mem_read(actor+o,4*n)))
 assert floats(0x144,6)==[-100.]*3+[100.]*3 and floats(0x12c,6)==floats(0x144,6)
 assert floats(0x120,3)==[0.]*3 and floats(0x160,3)==[0.]*3 and floats(0x16c,3)==[0.]*3
 assert word(0x180)==0 and word(0x270)==0xffffffff and word(0x274)==100
 assert struct.unpack('<III',u.mem_read(first_handle,12))==(0,actor,0xffffffff)
 assert u.mem_read(actor+0x80,1)==b'\xa5' and u.mem_read(actor+0x87,1)==b'\xa5'
 assert word(0x304+0x2c)==actor and word(0x304+0x30)==0
 results.append(dict(go_type=go_type,local_bounds=floats(0x144,6),absolute_bounds=floats(0x12c,6),scale=floats(0x120,3),target_list_ref=hex(word(0x330)),target_list_character=word(0x334),services=services))
(ref/'gameobject-constructor-original.json').write_text(json.dumps(dict(original_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),entry='0x38c398',results=results,
 fixture_services=['CString reserve/empty ctor','Condition ctor (condition fields excluded)','operator new allocation','binary32 float add'],
 actual_source=['ObjectBase ctor','ObjectHandle ctor','PFObject ctor','TargetList ctor/Clear/SetRefObject','GameObject IsCharacter virtual0','UpdateAbsoluteAABB'],production_full_gameobject_verified=False),indent=2)+'\n')
print('PASS two actual GameObject constructor graphs; explicit fixture services')
