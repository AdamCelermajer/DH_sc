from pathlib import Path
import sys,struct,json,hashlib,subprocess
root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');base=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');ref=base/'reference/device-provider-v54'
sys.path[:0]=[str(root/'port/game-data/tests'),'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages']
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});u=c.uc
def word(a,v):u.mem_write(a,struct.pack('<I',v))
def rd(a):return struct.unpack('<I',u.mem_read(a,4))[0]
got=(rd(0x381728)+0x3816bc)&0xffffffff
flags=[rd(got+rd(a)) for a in [0x38172c,0x381730,0x381734]]
appcell=rd(got+rd(0x381738))
print('flags',list(map(hex,flags)),'appcell',hex(appcell))
app=c.data+0x1000;owner=app+0x100;driver=app+0x200
word(appcell+0x10,owner);word(owner+0x10,driver)
trace=[]
u.hook_add(UC_HOOK_CODE,lambda uc,a,s,x:trace.append(a) if a in [0x5aefa4,0x5b90ac,0x3816f8] else None)
rows=[]
# Concrete derived ctor executes its original vtable producer; base ctor is
# deliberately a required engine service leaf, not a simulated GL activation.
def base_leaf(uc,a,s,x):
 if a==0x5b5c40:uc.reg_write(c.pc,uc.reg_read(c.lr))
hook=u.hook_add(UC_HOOK_CODE,base_leaf)
ctor_rows=[]
for ctor in [0x5b5d78,0x5b5e00]:
 for poison in [0,0xa5,0x7f]:
  u.mem_write(driver,bytes([poison])*128);c.invoke(ctor,[driver,owner]);assert rd(driver)==0x977b20
  ctor_rows.append({'ctor':hex(ctor),'poison':poison,'produced_vtable_address_point':hex(rd(driver)),'base_programmable_driver_ctor':'explicit required service leaf; no GL activation'})
u.hook_del(hook)
for vtable,getter,value in [(0x977b18,0x5aefa4,1),(0x977d40,0x5b90ac,0)]:
 word(driver,vtable+8)
 assert rd(vtable+8+0x5c)==getter
 for fs in [[0,0,0],[1,0,0],[0,1,0],[0,0,1],[255,0,0],[0,0,255]]:
  for a,v in zip(flags,fs):u.mem_write(a,bytes([v]))
  trace.clear();c.invoke(0x38174c,[]);result=c.reg(0)
  expected=0 if any(fs) else value
  assert result==expected
  assert (getter in trace)==(not any(fs))
  rows.append({'driver_vtable':hex(vtable),'getter':hex(getter),'flags':fs,'result':result,'genuine_driver_body_called':getter in trace})
elf=root/'.local-inputs/libDungeonHunter2.so';sdk=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin')
for a,n in [(0x5b5d78,0x34),(0x5b5e00,0x34),(0x5b5dac,0x54),(0x6a0454,0x98)]:
 asm=subprocess.run([str(sdk/'llvm-objdump.exe'),'--disassemble','--demangle','--start-address='+hex(a),'--stop-address='+hex(a+n),str(elf)],capture_output=True,text=True,check=True).stdout;(ref/(hex(a)+'.asm')).write_text(asm)
receipt={'schema':'original-device-provider-v54','status':'PASS','elf_sha256':hashlib.sha256(elf.read_bytes()).hexdigest(),'flags':[{'name':n,'address':hex(a),'scalar_initializer':0,'storage':'BSS byte'} for n,a in zip(['SHARP_DEVICES','HTC_DEVICES','Is_In_Multiplayer_Mode'],flags)],'static_App_object':hex(appcell),'App_device_driver_offsets':['0x10','0x10'],'concrete_driver_ctor_producer_cases':ctor_rows,'whole_original_cases':rows,'fixture_boundary':'Global flag bytes and static App/device/driver borrowed pointer chain supplied as fixtures; original Device body and actual original driver vtable/getter execute unchanged. No Android/device activation proof.'}
(ref/'original-device-v54.json').write_text(json.dumps(receipt,indent=2)+'\n');(ref/'prove_device_v54.py').write_bytes(Path(__file__).read_bytes());print('PASS original Device whole body12 exact3flag cases original GLES2/Null virtual bodies')
