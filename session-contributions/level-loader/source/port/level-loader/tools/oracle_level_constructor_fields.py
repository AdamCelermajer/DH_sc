"""Actual ARM LevelC1 direct stores through3f3270 only; constructor helpers are declared fixture boundaries."""
import pathlib,sys,hashlib,struct,json,subprocess
root=pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
sys.path.insert(0,str(root.parent/'dependencies'))
sys.path.insert(0,str(root/'port/engine-math/tests'))
from differential import Cpu
from unicorn import UC_HOOK_CODE
elf=pathlib.Path(r'C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so')
assert hashlib.sha256(elf.read_bytes()).hexdigest()=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
class Imports:
 def call(self,c,name):raise AssertionError('unmodeled import '+name)
c=Cpu(elf,False,Imports(),{'functions':[]})
symbol=next(name for name,address in c.symbols.items() if address==0x3f3128 and 'C1' in name)
level=c.data+0x1000;name=c.data+0x3000;entry_sp=c.stack+0xe000
calls=[];cutoff=[]
helpers={0x33805c:'EventManager constructor',0x37c584:'LuaScript constructor',0x3140ec:'std::string constructor'}
def hook(uc,at,size,unused):
 if at in helpers:
  calls.append(helpers[at]);uc.reg_write(c.pc_reg,uc.reg_read(c.lr_reg))
 elif at==0x3f3274:
  cutoff.append(at);uc.emu_stop()
c.uc.hook_add(UC_HOOK_CODE,hook)
fields={'config38':(0x38,False),'music11c':(0x11c,True),'safezone120':(0x120,True),'ambient124':(0x124,True),'word150':(0x150,False)}
rows=[]
for poison in (0,0x5a,0xa5,0xff):
 for argument in (0,0xdeadbeef):
  c.uc.mem_write(level,bytes([poison])*0x200);c.uc.mem_write(name,b'001_swamp\0')
  c.uc.mem_write(entry_sp,struct.pack('<6I',argument,17,1,0,0xffffffff,2))
  c.uc.reg_write(c.sp_reg,entry_sp);c.uc.reg_write(c.lr_reg,c.stop)
  for i,value in enumerate((level,name,argument,42)):c.write_reg(i,value)
  calls.clear();cutoff.clear();c.uc.emu_start(c.symbols[symbol],c.stop,count=10000)
  assert cutoff==[0x3f3274] and calls==list(helpers.values()),(cutoff,calls)
  actual={key:struct.unpack('<i' if signed else '<I',c.uc.mem_read(level+offset,4))[0] for key,(offset,signed) in fields.items()}
  assert actual=={'config38':0,'music11c':-1,'safezone120':-1,'ambient124':-1,'word150':0},actual
  rows.append({'poison_byte':poison,'argument':argument,'fields':actual})
out=pathlib.Path(__file__).parent
objdump=pathlib.Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\bin\llvm-objdump.exe')
asm=subprocess.check_output([str(objdump),'--disassemble','--demangle','--start-address=0x3f3128','--stop-address=0x3f34c0',str(elf)])
(out/'level-constructor-3f3128.asm').write_bytes(asm)
report={'validation':'PASS','scope':__doc__,'engine_sha256':hashlib.sha256(elf.read_bytes()).hexdigest(),
 'symbol':symbol,'entry':'0x3f3128','cutoff_before':'0x3f3274','helpers_declared_as_fixtures':list(helpers.values()),
 'direct_stores':{'config38':'0x3f319c','music11c':'0x3f3244','safezone120':'0x3f3248','ambient124':'0x3f324c','word150':'0x3f3270'},
 'cases':rows,'capture_sha256':hashlib.sha256(asm).hexdigest(),'oracle_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
 'cpu_helper_sha256':hashlib.sha256(pathlib.Path(sys.modules['differential'].__file__).read_bytes()).hexdigest(),
 'full_level_constructor_verified':False}
(out/'level-constructor-fields-original.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'validation':'PASS','prefix_cases':len(rows),'symbol':symbol,'fields':rows[0]['fields']}))
