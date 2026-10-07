from pathlib import Path
import sys,json,hashlib,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so';ref=root/'port/level-world/reference/animated-decor-v1';ref.mkdir(parents=True,exist_ok=True)
results=[]
for name,found,accepted,physical in [('',False,True,False),('randomall',False,False,True),('RANDOMALL',False,True,False),('activate',True,True,False),('activate',True,False,True),('missing',False,False,False)]:
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,0xc00000);u.mem_map(0x1000000,0x300000)
 with original.open('rb') as f:
  elf=ELFFile(f)
  for p in elf.iter_segments():
   if p['p_type']=='PT_LOAD':u.mem_write(p['p_vaddr'],p.data())
  for section in elf.iter_sections():
   if section['sh_type'] not in ('SHT_REL','SHT_RELA'):continue
   symbols=elf.get_section(section['sh_link'])
   for r in section.iter_relocations():
    s=symbols.get_symbol(r['r_info_sym']);value=s['st_value'];a=r['r_offset'];kind=r['r_info_type']
    if value and kind in (2,21,22):u.mem_write(a,struct.pack('<I',(value+(struct.unpack('<I',u.mem_read(a,4))[0] if kind==2 else 0))&0xffffffff))
 actor=0x1100000;visual=0x1110000;timeline=0x1120000;table=0x1130000;buffer=0x1140000;stop=0x1000000;calls=[]
 def w(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def string(a):
  b=b''
  while u.mem_read(a,1)!=b'\0':b+=u.mem_read(a,1);a+=1
  return b.decode()
 for o,addr in [(0x10,0x1001010),(0x14,0x1001014),(0x1c,0x100101c),(0x20,0x1001020),(0x2c,0x100102c)]:w(table+o,addr)
 w(visual+0x38,timeline);w(timeline,table);u.mem_write(visual+0x28,bytes([physical]))
 def ret(v=None):
  if v is not None:u.reg_write(UC_ARM_REG_R0,v)
  u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(u,a,size,data):
  r0=u.reg_read(UC_ARM_REG_R0);r1=u.reg_read(UC_ARM_REG_R1)
  if a==stop:u.emu_stop();return
  if a==0x310570:ret(actor if r0==0x394 else 0x1150000);return
  if a==0x38c398:
   assert r0==actor and r1==20;calls.append(['base_ctor',r1]);w(actor+0x2d8,visual);ret(actor);return
  if a==0x31167c:w(actor+0x38c,buffer);w(actor+0x390,buffer);ret();return
  if a==0x388a98:calls.append(['decor_init_post']);ret();return
  if a==0x3109e0:
   assert string(r1)=='idle';u.mem_write(buffer,b'idle\0');w(actor+0x38c,buffer+4);calls.append(['assign','idle']);ret();return
  if a==0x30e6e8:ret(0 if string(r0).lower()==string(r1).lower() else 1);return
  if a==0x1001010:calls.append(['count',r1]);ret(4);return
  if a==0x388c58:calls.append(['random_max',r0]);ret(2);return
  if a==0x1001014:calls.append(['exists',string(r1)]);ret(found);return
  if a==0x100101c:calls.append(['play_index',r1,u.reg_read(UC_ARM_REG_R2),u.reg_read(UC_ARM_REG_R3),word(u.reg_read(UC_ARM_REG_SP))]);ret(accepted);return
  if a==0x1001020:calls.append(['play_name',string(r1),u.reg_read(UC_ARM_REG_R2)]);ret(accepted);return
  if a==0x100102c:calls.append(['completion',hex(r1),u.reg_read(UC_ARM_REG_R2)==actor,u.reg_read(UC_ARM_REG_R3),word(u.reg_read(UC_ARM_REG_SP))==actor]);ret();return
  if a==0x470a54:calls.append(['sync']);ret();return
  if a==0x388a2c:calls.append(['podecor_ctor',u.reg_read(UC_ARM_REG_R2)==actor]);ret();return
  if a==0x394bf8:calls.append(['set_physical',u.reg_read(UC_ARM_REG_R2)]);ret();return
  if a==0x1001030:calls.append(['update']);ret();return
 u.hook_add(UC_HOOK_CODE,hook)
 def run(a,r0=actor,r1=0):
  u.reg_write(UC_ARM_REG_R0,r0);u.reg_write(UC_ARM_REG_R1,r1);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop);u.emu_start(a,stop,count=10000)
 run(0x342600,0);assert u.mem_read(actor+0x375,2)==b'\0\1' and u.mem_read(actor+0x84,1)==b'\1';assert string(buffer)==''
 ctor=list(calls);calls.clear();u.mem_write(buffer,name.encode()+b'\0');w(actor+0x38c,buffer+len(name));w(actor,0x1160000);w(0x1160000+0x2c,0x1001030)
 run(0x389128)
 assert u.mem_read(actor+0x10c,1)==b'\1' and calls[0]==['decor_init_post'] and calls[-1]==['update']
 if name.lower()=='randomall':assert ['random_max',3] in calls and any(x[0]=='completion'and x[1]=='0x388cec'for x in calls),calls
 elif found and accepted:assert ['play_name',name,1] in calls
 else:assert ['play_index',0,1,0,0] in calls
 assert sum(x[0]=='podecor_ctor' for x in calls)==int(physical) # Decor service here is explicit fixture, not whole body
 results.append({'name':name,'found':found,'accepted':accepted,'visual_physical28':physical,'constructor_calls':ctor,'init_post_calls':calls})
(ref/'original-control-oracle.json').write_text(json.dumps({'original_sha256':hashlib.sha256(original.read_bytes()).hexdigest(),'cases':results,'fixture_services':['base constructor38c398','CString reserve/assign','DecorInitPost388a98','timeline methods','shared Random388c58','VisualSync/PODecor/SetPhysical/Update'],'actual_executed':['factory342600 derived stores','AnimatedDecorInitPost389128','MeetCondition38ab60'],'production_initialization_verified':False},indent=2)+'\n')
print('PASS six original AnimatedDecor constructor/InitPost control cases; explicit external service fixtures')
