from pathlib import Path
import sys,struct,json,hashlib
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
sys.path.insert(0,str(root/'port/engine-animation/tests'))
from particle_factory_differential import FactoryCpu
from unicorn import UC_HOOK_CODE
cpu=FactoryCpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
node=cpu.data+0x1000;controller=node+0x400
vtable=cpu.symbols['_ZTV13RootSceneNode'];rows=[];trace=[]
def word(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
def hook(uc,a,z,u):
 if a in (0x597094,0x598658,0x31d584):trace.append(hex(a))
cpu.uc.hook_add(UC_HOOK_CODE,hook)
for mode in (0,1):
 trace.clear();cpu.uc.mem_write(node,bytes(0x214));cpu.pointer(node,vtable+0x1c);cpu.pointer(node+0x20c,vtable+0x124);cpu.pointer(node+0x210,1)
 # Explicit preconstructed source Root fixture: constructor-count store1 and
 # actual constructor-empty ISceneNode intrusive animator-list headers.
 for offset in (0xfc,0x104):cpu.pointer(node+offset,node+offset);cpu.pointer(node+offset+4,node+offset)
 cpu.uc.mem_write(controller,bytes(8));cpu.invoke(0x474d30,[controller,node,mode]);assert word(controller+4)==node and word(node+0x210)==2
 after_ctor=trace.copy();cpu.invoke(0x474684,[controller]);assert word(node+0x210)==1
 assert after_ctor==(['0x597094']if mode==0 else ['0x598658'])and trace[-1]=='0x31d584'
 rows.append({'mode':mode,'ctor_trace':after_ctor,'complete_trace':trace.copy(),'root_reference_after_ctor':2,'root_reference_after_dtor':1})
out={'validation':'PASS','cases':2,'rows':rows,'original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),
 'scope':'whole original AnimController C1/D1 and getAnimators/removeAnimators/drop against explicit preconstructed Root source fields; not whole Root factory/C1 or final Root deletion proof'}
p=root/'port/level-world/reference/module-root-lifecycle-v4/controller-original.json';p.write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out))
