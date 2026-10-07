"""Execute whole original CalcMeshBox no-marker branches; capture native gold.
SceneManager queries, actual mesh virtual30/parent virtual90 are typed inputs.
Float helpers are common IEEE services; original aggregation and transform run.
"""
from pathlib import Path
import sys,json,struct,importlib.util,random
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R5
root=Path(__file__).resolve().parents[3]
def load(name,path):
 s=importlib.util.spec_from_file_location(name,path);m=importlib.util.module_from_spec(s);s.loader.exec_module(m);return m
dep=load('decor_probe',root/'port/level-world/tests/decor_scene_differential.py')
manifest=json.loads((root/'port/level-world/reference/decor-scene/original-functions.json').read_text())
cpu=dep.Cpu(root/'.local-inputs/libDungeonHunter2.so',False,dep.Dependencies(),manifest)
cpu.symbols['box']=0x47211c
data=cpu.data
visual,node,nodevt,app,device,manager,manvt,meshvt,parentvt,items,callbacks=[data+i for i in (0x1000,0x2000,0x3000,0x4000,0x5000,0x6000,0x7000,0x8000,0x9000,0xa000,0xb000)]
def word(a,n):cpu.uc.mem_write(a,struct.pack('<I',n))
def pack(v):return struct.pack('<'+'f'*len(v),*v)
def ret(value):cpu.write_reg(0,value);cpu.uc.reg_write(cpu.pc_reg,cpu.uc.reg_read(cpu.lr_reg))
for i in range(4):cpu.uc.mem_write(callbacks+i*4,struct.pack('<I',0xe12fff1e))
word(nodevt+0x40,callbacks);word(manvt+0x20,callbacks+4);word(meshvt+0x30,callbacks+8);word(parentvt+0x90,callbacks+12)
word(app+0x10,device);word(device+0x1c,manager);word(manager,manvt)
case=[];queries=[]
def hook(uc,address,size,unused):
 if address==0x472418:uc.reg_write(UC_ARM_REG_R5,app);uc.reg_write(cpu.pc_reg,0x47241c)
 elif address==callbacks:ret(node+0x68)
 elif address==callbacks+4:
  kind=cpu.reg(1);queries.append(kind);selection=[i for i,m in enumerate(case)if m['animated']==(kind==0x73656164)]
  for k,i in enumerate(selection):word(items+k*4,data+0xc000+i*0x400)
  p=cpu.reg(2);word(p,items if selection else 0);word(p+4,items+len(selection)*4 if selection else 0);word(p+8,items+len(selection)*4 if selection else 0);ret(0)
 elif address==callbacks+8:ret(cpu.reg(0)+0x130)
 elif address==callbacks+12:ret(cpu.reg(0)+0xc8)
 elif address==0x597290:ret(cpu.reg(0)+0x200)
 elif address==0x310450:ret(0)
cpu.uc.hook_add(UC_HOOK_CODE,hook)
identity=[1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1]
rng=random.Random(20261005);cases=[]
for index in range(66):
 case=[];queries=[]
 count=0 if index==0 else 1+index%5
 for i in range(count):
  case.append(dict(animated=bool((i+index)%3==0),local_box=[rng.uniform(-100,0)for _ in range(3)]+[rng.uniform(0,100)for _ in range(3)],parent_scale=[rng.choice([-2,-1,0,0.5,1,3])for _ in range(3)]))
 matrix=identity.copy();matrix[0]=rng.uniform(-2,2);matrix[5]=rng.uniform(-2,2);matrix[10]=rng.uniform(-2,2);matrix[4]=rng.uniform(-1,1);matrix[1]=rng.uniform(-1,1);matrix[12:15]=[999,-222,55]
 cpu.uc.mem_write(visual,bytes(0x100));word(visual+8,node);word(node,nodevt);cpu.uc.mem_write(node+0x68,pack(matrix)+b'\0')
 for i,m in enumerate(case):
  mesh=data+0xc000+i*0x400;word(mesh,meshvt);word(mesh+0x200,parentvt);cpu.uc.mem_write(mesh+0x130,pack(m['local_box']));cpu.uc.mem_write(mesh+0x200+0xc8,pack(m['parent_scale']))
 cpu.invoke('box',[visual]);expected=bytes(cpu.uc.mem_read(visual+0x10,24));cases.append(dict(meshes=case,root=matrix,expected_words=list(struct.unpack('<6I',expected)),queries=queries))
out=root/'port/level-world/reference/level-config-module-connection-v2/module-mesh-box-original.json'
out.write_text(json.dumps(dict(status='CAPTURED',cases=cases,scope=__doc__),indent=2)+'\n')
print('Original CalcMeshBox fallback CAPTURED',len(cases),'cases')
