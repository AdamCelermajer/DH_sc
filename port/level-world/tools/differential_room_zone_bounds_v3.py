"""Whole original Zone::InitBounds397594 with physical380=false; whole
SetPosition/SetDestination/UpdateAbsoluteAABB run with source-null peers.
"""
from pathlib import Path
import sys,json,struct,importlib.util,random
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
root=Path(__file__).resolve().parents[3]
spec=importlib.util.spec_from_file_location('decor_probe',root/'port/level-world/tests/decor_scene_differential.py');dep=importlib.util.module_from_spec(spec);spec.loader.exec_module(dep)
cpu=dep.Cpu(root/'.local-inputs/libDungeonHunter2.so',False,dep.Dependencies(),json.loads((root/'port/level-world/reference/decor-scene/original-functions.json').read_text()));cpu.symbols['zone']=0x397594
actor=cpu.data+0x1000;input=cpu.data+0x3000;rng=random.Random(20261005);cases=[]
for i in range(60):
 box=[rng.uniform(-25000,0)for _ in range(3)]+[rng.uniform(0,25000)for _ in range(3)]
 raw=struct.pack('<6f',*box);cpu.uc.mem_write(actor,bytes(0x500));cpu.uc.mem_write(input,raw);cpu.invoke('zone',[actor,input])
 output={str(hex(a)):list(struct.unpack('<'+'I'*n,bytes(cpu.uc.mem_read(actor+a,n*4))))for a,n in [(0x374,3),(0x144,6),(0x12c,6),(0x160,3),(0x1a8,3)]}
 cases.append(dict(input_words=list(struct.unpack('<6I',raw)),output=output))
out=root/'port/level-world/reference/module-zone-v3/zone-bounds-original.json';out.write_text(json.dumps(dict(scope=__doc__,cases=cases),indent=2));print('Original Zone InitBounds CAPTURED',len(cases))
