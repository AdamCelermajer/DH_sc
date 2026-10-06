"""Original CSceneNode5839d8 and mesh585118 constructor transform fields.
The unrelated empty std::string producer is a typed constructor
service boundary. Original ISNode constructor and matrix update run ARM.
"""
from pathlib import Path
import sys,json,struct,importlib.util
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_SP
root=Path(__file__).resolve().parents[3]
sp=importlib.util.spec_from_file_location('decor_probe',root/'port/level-world/tests/decor_scene_differential.py');dep=importlib.util.module_from_spec(sp);sp.loader.exec_module(dep)
manifest=json.loads((root/'port/level-world/reference/decor-scene/original-functions.json').read_text());cpu=dep.Cpu(root/'.local-inputs/libDungeonHunter2.so',False,dep.Dependencies(),manifest)
def ret(v=0):cpu.write_reg(0,v);cpu.uc.reg_write(cpu.pc_reg,cpu.uc.reg_read(cpu.lr_reg))
def hook(uc,a,n,u):
 if a==0x598ee0:ret(cpu.reg(0))
 if a==0x585118:uc.mem_write(uc.reg_read(UC_ARM_REG_SP),struct.pack('<II',args+16,args+32))
cpu.uc.hook_add(UC_HOOK_CODE,hook)
cpu.symbols['group']=0x5839d8;cpu.symbols['mesh']=0x585118
owner=cpu.data+0x1000;args=cpu.data+0x2000
cpu.uc.mem_write(owner,bytes(0x300));cpu.invoke('group',[owner,0xffffffff]);
fields=dict(flags=hex(struct.unpack('<I',bytes(cpu.uc.mem_read(owner+0x11c,4)))[0]),world_words=list(struct.unpack('<16I',bytes(cpu.uc.mem_read(owner+0x24,64)))),box=list(struct.unpack('<6f',bytes(cpu.uc.mem_read(owner+0x130,24)))))
assert fields['flags']=='0x721',fields
assert fields['box']==[-1,-1,-1,1,1,1],fields
cpu.uc.mem_write(owner,bytes(0x300));cpu.uc.mem_write(args,struct.pack('<I10f',0,123,-456,22,0,0,0,1,1,1,1));cpu.invoke('mesh',[owner,args,0xffffffff,args+4])
mesh=dict(flags=hex(struct.unpack('<I',bytes(cpu.uc.mem_read(owner+0x11c,4)))[0]),world=list(struct.unpack('<16f',bytes(cpu.uc.mem_read(owner+0x24,64)))),visible=list(bytes(cpu.uc.mem_read(owner+0x120,2))))
assert mesh['flags']=='0x721' and mesh['visible']==[1,1],mesh
assert mesh['world'][12:15]==[123,-456,22],mesh
out=root/'port/level-world/reference/level-config-module-connection-v2/map-node-constructor-original.json';out.write_text(json.dumps(dict(status='PASS',scope=__doc__,group=fields,mesh_transform=mesh),indent=2)+'\n');print(json.dumps(dict(group=fields,mesh=mesh)))
