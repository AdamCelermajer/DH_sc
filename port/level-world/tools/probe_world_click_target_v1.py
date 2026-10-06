from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so';ref=root/'port/level-world/reference/world-touch-target-v1';rows=[]
def bits(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def number(w):return struct.unpack('<f',struct.pack('<I',w))[0]
for allowed,casting,using,mode,released,has_actor in [(False,False,False,False,False,True),(True,True,False,False,False,True),(True,True,False,False,True,True),(True,False,True,False,False,True),(True,False,False,False,False,True),(True,False,False,True,False,False),(True,False,False,False,False,False)]:
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,0xc00000);u.mem_map(0x1000000,0x300000)
 with original.open('rb') as f:
  elf=ELFFile(f)
  for p in elf.iter_segments():
   if p['p_type']=='PT_LOAD':u.mem_write(p['p_vaddr'],p.data())
  for section in elf.iter_sections():
   if section['sh_type']not in ('SHT_REL','SHT_RELA'):continue
   symbols=elf.get_section(section['sh_link'])
   for r in section.iter_relocations():
    value=symbols.get_symbol(r['r_info_sym'])['st_value'];a=r['r_offset'];kind=r['r_info_type']
    if value and kind in (2,21,22):u.mem_write(a,struct.pack('<I',(value+(struct.unpack('<I',u.mem_read(a,4))[0]if kind==2 else 0))&0xffffffff))
 actor=0x1100000;npc=0x1110000;manager=0x1120000;node=0x1130000;point=0x1140000;stop=0x1000000;calls=[];key=''
 def w(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def string(a):
  b=b''
  while u.mem_read(a,1)!=b'\0':b+=u.mem_read(a,1);a+=1
  return b.decode()
 head=manager+0x60;w(head,node if has_actor else head);w(node,head);w(node+8,npc)
 w(actor,0x1150000);w(npc,0x1151000);w(0x1151000+0x88,0x1001088);w(0x1150000+0xe4,0x10010e4);w(0x1150000+0xec,0x10010ec)
 u.mem_write(point,struct.pack('<fff',10,20,30));u.mem_write(npc+0x160,struct.pack('<fff',10,20,30));u.mem_write(npc+0x12c,struct.pack('<ffffff',9,19,29,11,21,31));u.mem_write(actor+0x14c8,b'\xa5\xa5\xa5\xa5')
 # Source DesignSettings pointer is loaded through the relocated GOT slot.
 base=0x3adde0+word(0x3ae448);slot=base+word(0x3ae454);global_address=word(slot);w(global_address,0x1160000)
 for offset in (0x2c,0x50,0x54):w(0x1160000+offset,bits(1))
 def ret(v=None):
  if v is not None:u.reg_write(UC_ARM_REG_R0,v&0xffffffff)
  u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(u,a,size,data):
  nonlocal_key=None;r0=u.reg_read(UC_ARM_REG_R0);r1=u.reg_read(UC_ARM_REG_R1)
  if a==stop:u.emu_stop();return
  if a==0x3ad430:calls.append(['allowed']);ret(allowed);return
  if a==0x320e74:calls.append(['mode']);w(r0+0x38,manager);ret(mode);return
  if a==0x3c0334:calls.append(['casting']);ret(casting);return
  if a==0x3c02e8:calls.append(['using']);ret(using);return
  if a==0x3935dc:calls.append(['position',hex(r0)]);ret(r0+0x160);return
  if a==0x1001088:calls.append(['interactive',hex(r0)]);ret(1);return
  if a==0x3d5a98:calls.append(['neutral',hex(r1)]);ret(0);return
  if a==0x3d574c:calls.append(['enemy',hex(r1)]);ret(1);return
  if a in (0x30e3ac,0x30ed6c,0x30eba4,0x30e2f8,0x30e9ac,0x30e4b4):
   x=number(r0);y=number(r1)
   result=bits(x-y)if a==0x30e3ac else bits(x*y)if a==0x30ed6c else bits(x+y)if a==0x30eba4 else int(x>y)if a==0x30e2f8 else int(x<=y)if a==0x30e9ac else int(x>=y)
   ret(result);return
  if a==0x337888:ret();return
  if a==0x3140ec:
   text=string(r1).encode()+b'\0';storage=0x1170000;u.mem_write(storage,text);w(r0+0x14,storage);w(r0+0x10,storage+len(text)-1);ret();return
  if a==0x337a88:calls.append(['debug',string(word(r1+0x14))]);ret(0);return
  if a==0x3139ac:ret();return
  if a==0x3d6890:calls.append(['target',hex(r1),u.reg_read(UC_ARM_REG_R2)]);w(actor+0x3c8+0x40,r1);ret();return
  if a in (0x10010e4,0x10010ec):calls.append(['move',a==0x10010ec]);ret();return
 u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_ARM_REG_R0,actor);u.reg_write(UC_ARM_REG_R1,point);u.reg_write(UC_ARM_REG_R2,released);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop);u.emu_start(0x3addc8,stop,count=40000)
 pending=u.mem_read(actor+0x14c8,1)[0];skill=struct.unpack('<h',u.mem_read(actor+0x14ca,2))[0]
 if not allowed:assert pending==0xa5 and calls==[['allowed']]
 elif casting or using:assert pending==int(not released)and skill==-1
 elif has_actor:assert ['target',hex(npc),0]in calls and u.mem_read(actor+0x413,1)==b'\1'
 elif mode:assert not any(x[0]=='target'for x in calls)
 else:assert ['target','0x0',0]in calls and ['move',False]in calls
 rows.append({'allowed':allowed,'casting':casting,'using':using,'mode':mode,'released':released,'has_actor':has_actor,'pending':pending,'skill':skill,'calls':calls})
(ref/'original-control-oracle.json').write_text(json.dumps({'original_sha256':hashlib.sha256(original.read_bytes()).hexdigest(),'cases':rows,'actual_executed':['CharacterCtrl_Click3addc8','GameObjectIsNearby38ac0c','AI_SyncLastTarget3d49c4'],'fixture_services':['FSM/application gates','actual list actors and DesignSettings backing','target position/interactive/relationships','softfloat primitive ABI','Debug/String','AI_SetTarget whole boundary (separately recovered)','movement virtuals'],'production_touch_binding_verified':False},indent=2)+'\n');print('PASS seven original Ctrl_Click/IsNearby control cases; explicit services and same-field effects')
