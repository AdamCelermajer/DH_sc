"""Actual LevelC1 prefix module fields; string/Lua/EventManager calls are declared fixture boundaries."""
import pathlib,sys,hashlib,struct,json
root=pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
sys.path.insert(0,str(root.parent/'dependencies'));sys.path.insert(0,str(root/'port/engine-math/tests'))
from differential import Cpu
from unicorn import UC_HOOK_CODE
elf=pathlib.Path(r'C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();assert sha(elf)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
class Imports:
 def call(self,c,name):raise AssertionError('unmodeled import '+name)
c=Cpu(elf,False,Imports(),{'functions':[]});level=c.data+0x1000;name=c.data+0x3000;entry_sp=c.stack+0xe000
helpers={0x33805c:'EventManager constructor',0x37c584:'LuaScript constructor',0x3140ec:'std::string constructor',0x3109e0:'std::string assign',0x37b574:'LuaScript Load'};calls=[];cutoff=[]
def hook(uc,at,size,unused):
 if at in helpers:calls.append(helpers[at]);c.write_reg(0,0);uc.reg_write(c.pc_reg,uc.reg_read(c.lr_reg))
 elif at==0x3f32d8:cutoff.append(at);uc.emu_stop()
c.uc.hook_add(UC_HOOK_CODE,hook)
rows=[]
for poison in (0,0x5a,0xa5,0xff):
 for argument in (0,0xdeadbeef):
  c.uc.mem_write(level,bytes([poison])*0x200);c.uc.mem_write(name,b'001_swamp\0');c.uc.mem_write(entry_sp,struct.pack('<6I',argument,17,1,0,0xffffffff,2))
  c.uc.reg_write(c.sp_reg,entry_sp);c.uc.reg_write(c.lr_reg,c.stop)
  for i,value in enumerate((level,name,argument,42)):c.write_reg(i,value)
  calls.clear();cutoff.clear();c.uc.emu_start(0x3f3128,c.stop,count=10000)
  assert cutoff==[0x3f32d8] and calls==['EventManager constructor','LuaScript constructor','std::string constructor','std::string assign','LuaScript Load','LuaScript Load'],(cutoff,calls)
  fields={'module_offset160':list(struct.unpack('<3f',c.uc.mem_read(level+0x160,12))),'object_module_id18c':struct.unpack('<i',c.uc.mem_read(level+0x18c,4))[0]}
  assert fields=={'module_offset160':[0.,0.,0.],'object_module_id18c':-1},fields
  rows.append({'poison_byte':poison,'argument':argument,'fields':fields})
report={'validation':'PASS','scope':__doc__,'engine_sha256':sha(elf),'entry':'0x3f3128','cutoff_before':'0x3f32d8',
 'direct_stores':{'module_offset160':['0x3f3228','0x3f3230','0x3f3234'],'object_module_id18c':'0x3f32d4'},'helpers_declared_as_fixtures':helpers,'cases':rows,
 'oracle_sha256':sha(pathlib.Path(__file__)),'cpu_helper_sha256':sha(pathlib.Path(sys.modules['differential'].__file__)),
 'full_level_constructor_verified':False,'gameplay_verified':False}
(pathlib.Path(__file__).parent/'level-module-fields-original.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'validation':'PASS','prefix_cases':len(rows),'module_fields':rows[0]['fields']}))
