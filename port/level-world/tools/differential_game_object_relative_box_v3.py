"""Whole original SetRelativeAABB control/stores; typed existing absolute-box
and UpdatePFObject services are observed independently of their implementations.
"""
from pathlib import Path
import sys,json,struct,importlib.util,random
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from unicorn import UC_HOOK_CODE
root=Path(__file__).resolve().parents[3]
spec=importlib.util.spec_from_file_location('decor_probe',root/'port/level-world/tests/decor_scene_differential.py');dep=importlib.util.module_from_spec(spec);spec.loader.exec_module(dep)
cpu=dep.Cpu(root/'.local-inputs/libDungeonHunter2.so',False,dep.Dependencies(),json.loads((root/'port/level-world/reference/decor-scene/original-functions.json').read_text()));cpu.symbols['box']=0x38b110
actor=cpu.data+0x1000;input=cpu.data+0x3000;calls=[]
def ret():cpu.uc.reg_write(cpu.pc_reg,cpu.uc.reg_read(cpu.lr_reg))
def hook(uc,address,size,unused):
 if address==0x38aac8:
  calls.append('absolute');box=struct.unpack('<6f',bytes(uc.mem_read(actor+0x144,24)));p=struct.unpack('<3f',bytes(uc.mem_read(actor+0x160,12)));uc.mem_write(actor+0x12c,struct.pack('<6f',*[x+p[i%3]for i,x in enumerate(box)]));ret()
 elif address==0x393ea0:calls.append('pf');ret()
cpu.uc.hook_add(UC_HOOK_CODE,hook)
rng=random.Random(20261005);cases=[]
for i in range(80):
 box=[rng.uniform(-100,100)for _ in range(6)] if i>5 else [0,0,-1,i,i,1]
 p=[rng.uniform(-1000,1000)for _ in range(3)];prior=i%2;alias=i%3==0;calls=[]
 cpu.uc.mem_write(actor,bytes(0x400));cpu.uc.mem_write(actor+0x2f9,bytes([prior]));cpu.uc.mem_write(actor+0x160,struct.pack('<3f',*p));cpu.uc.mem_write(input,struct.pack('<6f',*box))
 pointer=actor+0x144 if alias else input
 if alias:cpu.uc.mem_write(pointer,struct.pack('<6f',*box))
 cpu.invoke('box',[actor,pointer,i%2]);cases.append(dict(input_words=list(struct.unpack('<6I',struct.pack('<6f',*box))),position_words=list(struct.unpack('<3I',struct.pack('<3f',*p))),prior_flat=prior,alias=alias,expected_words=list(struct.unpack('<6I',bytes(cpu.uc.mem_read(actor+0x144,24)))),absolute_words=list(struct.unpack('<6I',bytes(cpu.uc.mem_read(actor+0x12c,24)))),flat=ucbyte if False else cpu.uc.mem_read(actor+0x2f9,1)[0],calls=calls))
out=root/'port/level-world/reference/module-zone-v3/relative-box-original.json';out.write_text(json.dumps(dict(scope=__doc__,cases=cases),indent=2));print('Original SetRelativeAABB CAPTURED',len(cases))
