"""Actual bounded constructor/default stores; not a whole XML/asset factory."""
from pathlib import Path
import sys,json,hashlib,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R7,UC_ARM_REG_LR
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3];engine=root/'.local-inputs/libDungeonHunter2.so';out=root/'port/level-world/reference/character-world-physical-peers-v1';out.mkdir(exist_ok=True)
with engine.open('rb')as f:
 e=ELFFile(f);segments=list(e.iter_segments());symbols=list(e.get_section_by_name('.dynsym').iter_symbols())
 def raw(a,n):
  seg=next(s for s in segments if s['p_vaddr']<=a and a+n<=s['p_vaddr']+s['p_filesz']);f.seek(seg['p_offset']+a-seg['p_vaddr']);return f.read(n)
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0x300000,0x300000);u.mem_map(0x10000000,0x10000)
 def execute(a,n):u.mem_write(a,raw(a,n));u.emu_start(a,a+n,count=100)
 records=[]
 for previous in [0,1,2,255]:
  obj=0x10000000;prop=obj+0x1000;u.mem_write(obj,bytes([0xa5])*0x400);u.mem_write(obj+0x80,bytes([previous]));
  execute(0x342614,4);source_type=u.reg_read(UC_ARM_REG_R1);assert source_type==20
  u.reg_write(UC_ARM_REG_R4,obj);u.reg_write(UC_ARM_REG_R7,source_type);execute(0x33f488,4)
  execute(0x342630,4);execute(0x342660,4)
  before=u.mem_read(obj+0x80,1)[0];assert before==previous
  u.mem_write(prop+4,struct.pack('<I',0x80));u.mem_write(prop+0x20,b'\x01');u.reg_write(UC_ARM_REG_R0,prop);u.reg_write(UC_ARM_REG_R1,obj);u.reg_write(UC_ARM_REG_LR,0x33de90);execute(0x33de80,16)
  fields=[source_type,u.mem_read(obj+0x84,1)[0],u.mem_read(obj+0x80,1)[0]];assert fields==[20,1,1];records.append(dict(previous_visible=previous,constructor_visible=before,type_f4=fields[0],static84=fields[1],default_visible80=fields[2]))
 methods={0x342600,0x33f310,0x33f014,0x33e4ac,0x33de80,0x389dd4,0x3899d0,0x46e6bc,0x3883c4,0x3883c8,0x3883cc,0x3883d0,0x46fb4c,0x46fe68,0x46ff6c,0x46fd28,0x46fb14,0x3884f8}
 md=Cs(CS_ARCH_ARM,CS_MODE_ARM);lines=[];functions=[]
 for s in symbols:
  if s['st_value'] not in methods or not s['st_size']:continue
  code=raw(s['st_value'],s['st_size']);functions.append(dict(address=hex(s['st_value']),name=s.name,bytes=len(code),sha256=hashlib.sha256(code).hexdigest()));lines.append('# '+s.name)
  lines.extend(f'{i.address:08x}: {i.mnemonic} {i.op_str}'for i in md.disasm(code,s['st_value']))
 (out/'original-peer-producers.asm').write_text('\n'.join(lines)+'\n')
report=dict(validation='PASS',original_sha256=hashlib.sha256(engine.read_bytes()).hexdigest(),constructor_default_store_cases=records,functions=functions,whole_constructor=False,allocator_string_XML_factory=False,source_visible_default_argument='ObjectBase DeclareProperties33f03c r3=1; SimpleTypeProperty bool +20 then SetToDefaultValue33de80',source_decor_contact_bodies='3883c4/c8/cc/d0 bx lr',source_player_contact='POCharacter requires actual Character RaiseEvent; no accepted empty provider',source_AnimatedDecor_IsZonable='3884f8 tailcalls MeetCondition38ab60; base byte shortcut unavailable')
(out/'producer-original-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items()if k!='functions'}))
