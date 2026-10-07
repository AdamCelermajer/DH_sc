from pathlib import Path
import sys,struct,json,hashlib
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
sys.path.insert(0,str(root/'port/engine-animation/tests'))
from particle_factory_differential import FactoryCpu
from unicorn import UC_HOOK_CODE
cpu=FactoryCpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
def word(a):return struct.unpack('<I',cpu.uc.mem_read(a,4))[0]
def ret(v=0):cpu.put(0,v);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
trace=[];phase='ctor';mapping=cpu.data+0x1000;block=mapping+0x100;manager=block+0x1000;frame=manager+0x200;array=frame+0x100;value=array+0x100
def hook(uc,a,n,user):
 if phase=='ctor':
  if a==0x3293c8:trace.append(['map_alloc',cpu.reg(1)]);ret(mapping)
  elif a==0x708ec0:trace.append(['block_alloc',word(cpu.reg(0))]);ret(block)
  elif a==0x30e304:trace.append(['atexit_registered',cpu.reg(0)]);ret()
  elif a==0x434bc8:uc.reg_write(cpu.pc,cpu.stop)
 elif a==0x42ca8c:ret(manager)
 elif a==0x797960:trace.append(['to_bool',cpu.reg(0)]);ret(boolean)
cpu.uc.hook_add(UC_HOOK_CODE,hook)
# Exact original guarded tutorial Singleton/deque constructor region. Other
# global initializers are outside this bounded proof. Allocator/atexit delivery
# is explicit; all field stores and start/end production execute original ARM.
cpu.put(5,0x994a98);cpu.invoke(0x434ae8,[])
queue=cpu.symbols['_ZN9SingletonI18MenuMessageManagerI19CharMenuTutorialMsgLi1EEE6s_instE']
assert word(queue+4)==block and word(queue+0x14)==block
assert word(queue+8)==block and word(queue+0x18)==block and word(queue+0xc)==block+0x68 and word(queue+0x1c)==block+0x68
assert word(cpu.symbols['_ZN19CharMenuTutorialMsg14s_SkipFuncNameE'])==0
rows=[{'operation':'source_constructor_region','empty':True,'trace':trace.copy(),'source_start_end_same':True}]
phase='wrappers';cpu.uc.mem_write(frame,bytes(32));cpu.pointer(frame+12,array);cpu.pointer(array,value);cpu.pointer(frame+20,0)
for argc in(0,1,2,3):
 for boolean in(0,1):
  trace.clear();cpu.pointer(frame+16,argc);cpu.uc.mem_write(manager+0x110,b'\xa5');cpu.invoke(0x43a124,[frame])
  result=cpu.uc.mem_read(manager+0x110,1)[0];assert result==(boolean if argc==1 else 1);assert len(trace)==int(argc==1)
  rows.append({'operation':'NativeSetMultitouch','argc':argc,'actual_to_bool':boolean,'field110':result,'trace':trace.copy()})
trace.clear();before=bytes(cpu.uc.mem_read(queue,44));cpu.invoke(0x442838,[frame]);assert bytes(cpu.uc.mem_read(queue,44))==before and not trace
rows.append({'operation':'whole_NativeSkip_empty','unchanged':True})
for boolean in(0,1):
 trace.clear();cpu.pointer(frame+16,1);cpu.invoke(0x43aa18,[frame]);assert trace==[['to_bool',value]]
 rows.append({'operation':'whole_NativeShowStatusBar','actual_to_bool':boolean,'application_leaf':'31f668 bx lr','trace':trace.copy()})
out={'validation':'PASS','cases':len(rows),'rows':rows,'original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'scope':'actual tutorial guarded constructor region plus whole NativeSetMultitouch/NativeSkip empty/NativeShowStatusBar ARM bodies; allocator, atexit, manager getter and AS boolean are explicit providers. Not original positive tutorial localization/setter/Invoke proof.'}
p=root/'port/engine-ui/reference/character-menu-application-v4/original-receiver-proof.json';p.write_text(json.dumps(out,indent=2));print(json.dumps(out))
