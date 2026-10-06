"""Whole original OptimizeStatic/updateAbsolutePosition over borrowed TRS nodes.
Factory/root allocation is outside this leaf oracle. Matrices, quaternion
extraction, flags and depth-first linked-child traversal execute original ARM.
"""
from pathlib import Path
import sys,json,struct,importlib.util,random
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from unicorn import UC_HOOK_CODE
root=Path(__file__).resolve().parents[3]
spec=importlib.util.spec_from_file_location('decor_probe',root/'port/level-world/tests/decor_scene_differential.py');dep=importlib.util.module_from_spec(spec);spec.loader.exec_module(dep)
manifest=json.loads((root/'port/level-world/reference/decor-scene/original-functions.json').read_text())
cpu=dep.Cpu(root/'.local-inputs/libDungeonHunter2.so',False,dep.Dependencies(),manifest)
cpu.symbols['optimize']=0x50f220
d=cpu.data;vt=d+0x1000;external=d+0x2000
def word(a,n):cpu.uc.mem_write(a,struct.pack('<I',n))
def pack(v):return struct.pack('<'+'f'*len(v),*v)
for off,fn in [(0x38,0x35a8e0),(0x40,0x598908),(0x9c,0x5970f4),(0xa4,0x59712c),(0xb8,0x597c60)]:word(vt+off,fn)
identity=[1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1]
rng=random.Random(20261006);rows=[]
for index in range(36):
 outer=identity.copy();outer[12:15]=[rng.uniform(-500,500)for _ in range(3)]
 word(external,vt);word(external+0x11c,0x120);cpu.uc.mem_write(external+0x24,pack(outer)+b'\0')
 count=1+index%4;nodes=[]
 for i in range(count):
  p=d+0x3000+i*0x400;cpu.uc.mem_write(p,bytes(0x200));word(p,vt);word(p+0xec,external if i==0 else p-0x400);word(p+0x11c,0x60f)
  t=[rng.uniform(-50,50)for _ in range(3)];q=list(rng.choice([(0,0,0,1),(0,0,0.70710677,0.70710677),(0,1,0,0),(0.70710677,0,0,0.70710677)]));s=[rng.choice([0.5,1,2])for _ in range(3)]
  cpu.uc.mem_write(p+0xac,pack(t+q+s));cpu.uc.mem_write(p+0x24,pack(identity)+b'\1');cpu.uc.mem_write(p+0x68,pack(identity)+b'\1')
  mesh=d+0x6000+i*0x400;cpu.uc.mem_write(mesh,bytes(0x200));word(mesh,vt);word(mesh+0xec,p);word(mesh+0x11c,0x761);cpu.uc.mem_write(mesh+0xac,pack([0,0,0,0,0,0,1,1,1,1]));cpu.uc.mem_write(mesh+0x24,pack(identity)+b'\0');cpu.uc.mem_write(mesh+0x68,pack(identity)+b'\0');word(mesh+0xf4,mesh+0xf4)
  sentinel=p+0xf4;word(sentinel,mesh+4);word(mesh+4,sentinel if i==count-1 else p+0x404)
  if i:word(p+4,p-0x400+0xf4)
  nodes.append(dict(translation=t,quaternion=q,scale=s,parent=i-1))
 cpu.invoke('optimize',[d+0x3000]);expected=[]
 for i in range(count):
  p=d+0x3000+i*0x400
  expected.append(dict(world_words=list(struct.unpack('<16I',bytes(cpu.uc.mem_read(p+0x24,64)))),pose_words=list(struct.unpack('<7I',bytes(cpu.uc.mem_read(p+0xac,28)))),flags=struct.unpack('<I',bytes(cpu.uc.mem_read(p+0x11c,4)))[0]))
 mesh_expected=[]
 for i in range(count):
  p=d+0x6000+i*0x400;mesh_expected.append(dict(pose_words=list(struct.unpack('<7I',bytes(cpu.uc.mem_read(p+0xac,28)))),flags=struct.unpack('<I',bytes(cpu.uc.mem_read(p+0x11c,4)))[0]))
 rows.append(dict(outer=outer,nodes=nodes,expected=expected,mesh_expected=mesh_expected))
(root/'port/level-world/reference/level-config-module-connection-v2/module-static-scene-original.json').write_text(json.dumps(dict(status='CAPTURED',scope=__doc__,cases=rows),indent=2)+'\n')
print('Original OptimizeStatic CAPTURED',len(rows))
