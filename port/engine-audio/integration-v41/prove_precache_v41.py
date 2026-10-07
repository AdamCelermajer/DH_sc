from pathlib import Path
import sys,struct,json,hashlib
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path(__file__).resolve().parents[3];sys.path.insert(0,str(root/'port/engine-resources/tests'))
from cpu import Cpu,i32
from unicorn import UC_HOOK_CODE
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
out=Path(__file__).parent;elf=root/'.local-inputs/libDungeonHunter2.so'
c=Cpu(elf,False,{'functions':[]});word=lambda p:struct.unpack('<I',c.uc.mem_read(p,4))[0]
got=0x369a14+word(0x369b24);disabled=word(got+word(0x369b28))
manager=c.data+0x1000;slots=c.data+0x4000;instance=c.symbols['_ZN15VoxSoundManager10s_instanceE'];initial_instance=word(instance);calls=[];shrink=False
def hook(uc,a,n,u):
 if a==0x8896f4:
  calls.append(['bank_info',c.reg(0),i32(c.reg(1))])
  if shrink:c.pointer(manager+0x1c,0)
  c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
 if a==0x369aac:
  calls.append(['unloaded_slot_reached',i32(c.reg(6))]);uc.reg_write(c.pc,0x369a4c)
 if a==0x310570:
  calls.append(['allocate',c.reg(0),c.reg(1)]);c.put(0,manager);uc.reg_write(c.pc,uc.reg_read(c.lr))
 if a==0x36c7b0:
  calls.append(['constructor',c.reg(0),word(instance)]);uc.reg_write(c.pc,uc.reg_read(c.lr))
 if a in [0x36afb0,0x310440]:
  calls.append(['destroy' if a==0x36afb0 else 'free',c.reg(0)]);uc.reg_write(c.pc,uc.reg_read(c.lr))
c.uc.hook_add(UC_HOOK_CODE,hook)
cases=[]
for off in [0,1]:
 for uid in [-1,0,33,34]:
  for loaded in [False,True]:
   c.uc.mem_write(disabled,bytes([off]));c.pointer(manager+0x1c,33);c.pointer(manager+8,slots);c.pointer(slots+max(0,uid)*4,0x123 if loaded else 0);calls.clear();shrink=False
   c.invoke(0x3699fc,[manager,uid])
   expected=[] if off or uid<0 or uid>33 else [['bank_info',manager+0x64,uid]]+([] if loaded else [['unloaded_slot_reached',uid]])
   assert calls==expected,(uid,calls,expected)
   cases.append(dict(disabled=off,raw_uid=uid,slot_loaded=loaded,observations=list(calls)))
c.uc.mem_write(disabled,b'\0');c.pointer(manager+0x1c,33);shrink=True;calls.clear();c.invoke(0x3699fc,[manager,33]);assert calls==[['bank_info',manager+0x64,33]];shrink=False
construction=[]
for present in [False,True]:
 c.pointer(instance,manager if present else 0);calls.clear();c.invoke(0x36ca88,[])
 expected=[] if present else [['allocate',0xc4,4],['constructor',manager,0]]
 assert calls==expected and word(instance)==manager
 construction.append(dict(present=present,calls=list(calls),published=word(instance)))
deletion=[]
for present in [False,True]:
 c.pointer(instance,manager if present else 0);calls.clear();c.invoke(0x36b048,[])
 assert calls==([['destroy',manager],['free',manager]] if present else [])
 assert word(instance)==(manager if present else 0)
 deletion.append(dict(present=present,calls=list(calls),global_after=word(instance)))
asm=[]
for start,size in [(0x3699fc,0x128),(0x36ca88,0x40),(0x36b048,0x38)]:
 asm += [f'{i.address:08x} {i.mnemonic} {i.op_str}' for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(bytes(c.uc.mem_read(start,size)),start)]
(out/'original-precache-instance.asm').write_text('\n'.join(asm)+'\n')
report=dict(validation='PASS',original_sha256=hashlib.sha256(elf.read_bytes()).hexdigest(),initial_global_instance=initial_instance,load_cases=cases,recheck_after_bank_info=True,construction=construction,deletion=deletion,scope='Original LoadSound gates/bank lookup/cache-slot prefix; loaded and early-return paths whole. Unloaded construction tail skipped to original epilogue. Create/Delete whole with observed allocation/constructor/destructor/free leaves. No real backend or World readiness.',disabled_global=hex(disabled),checks=len(cases)+5)
(out/'original-proof.json').write_text(json.dumps(report,indent=2)+'\n');print('PASS',report['checks'],'original precache/instance cases')
