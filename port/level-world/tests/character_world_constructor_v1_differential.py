"""Original FSM constructors/actor constructor stores and complete CF setter field producer."""
from pathlib import Path
import sys,struct,json,hashlib,itertools
root=Path(__file__).resolve().parents[3];sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R4,UC_ARM_REG_R6,UC_ARM_REG_R8
old=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});new=Cpu(root/'.local-inputs/libcharacter_world_npc_state_owner_v1_oracle.so',True,{'functions':[]})
actor=old.data+0x1000;machine=old.data+0x5000;native=new.data+0x1000;context=new.data+0x2000
word=lambda at:struct.unpack('<I',old.uc.mem_read(at,4))[0]
stop_at=0;levels=[0,0]
def hook(uc,pc,size,user):
 if pc==stop_at:uc.reg_write(old.pc,old.stop)
 elif pc==0x3dedb4:
  old.put(0,levels[0 if old.reg(0)==actor+0x560 else 1]);uc.reg_write(old.pc,uc.reg_read(old.lr))
old.uc.hook_add(UC_HOOK_CODE,hook)
records=[]
for poison in (0,0x33,0xaa,0xff):
 for ctor in (0x3c1ac4,0x3c1b58):
  old.uc.mem_write(machine,bytes([poison])*0x80);old.invoke(ctor,[machine]);assert word(machine+0x20)==word(machine+0x24)==word(machine+0x2c)==0;assert word(machine+0x28)==0xffffffff;assert bytes(old.uc.mem_read(machine+0x3f,1))==b'\0'
  old.uc.mem_write(actor,bytes([poison])*0x1800)
  for start,end,reg,value in ((0x3a96fc,0x3a9704,UC_ARM_REG_R8,0),(0x3a9738,0x3a9744,UC_ARM_REG_R8,0),(0x33f2d8,0x33f2dc,UC_ARM_REG_R6,0xffffffff)):
   old.uc.reg_write(UC_ARM_REG_R4,actor);old.uc.reg_write(reg,value);stop_at=end;old.invoke(start,[]);stop_at=0
  new.uc.mem_write(native,bytes([poison])*8);assert new.invoke('dh2_character_constructor_combat_fields_v1',[native])==0
  expected=bytes(old.uc.mem_read(actor+0x14d0,2))+bytes(old.uc.mem_read(actor+0x14f0,1))+bytes(old.uc.mem_read(machine+0x3f,1))+bytes(old.uc.mem_read(actor+0x110,4));assert bytes(new.uc.mem_read(native,8))==expected
  records.append([poison,hex(ctor),expected.hex()])
shared=(0x3b05b4+word(0x3b0618))&0xffffffff
for al,bl,element,off,magic in itertools.product((-2147483648,-1,0,1,2147483647),(-2147483648,-1,0,1,2147483647),(-1,0,7),(0,1,255),(0,1,255)):
 levels[:]=[al,bl];old.uc.mem_write(shared+0x1c,b'\xaa'*24);new.uc.mem_write(context,b'\xbb'*32)
 old.invoke(0x3b059c,[actor,actor+0x2000,element,off,magic]);assert new.invoke('dh2_world_combat_context_v1',[context,0x100000001,0x200000002,al,bl,element,off,magic])==0
 expected=struct.unpack('<IIiii4B',old.uc.mem_read(shared+0x1c,24));actual=struct.unpack('<QQiii4B',new.uc.mem_read(context,32));assert expected[:2]==(actor,actor+0x2000);assert actual[:2]==(0x100000001,0x200000002);assert expected[2:]==actual[2:],(al,bl,element,off,magic,expected,actual)
report={'validation':'PASS','constructor_cases':len(records),'cf_setter_cases':675,'mismatches':0,'whole_fsm_constructor_instructions':True,'character_ctor_store_fragments_with_source_register_values':True,'whole_cf_setter_instructions_with_explicit_property19_service':True,'cf_previous_context_bytes_poisoned':True,'source_push_death_byte':'Character+53b = FSM+3f, zeroed by C1/C2 word+3c','original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'native_sha256':hashlib.sha256((root/'.local-inputs/libcharacter_world_npc_state_owner_v1_oracle.so').read_bytes()).hexdigest()}
(root/'port/level-world/reports/character-world-constructor-v1-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
