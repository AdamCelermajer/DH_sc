from pathlib import Path
import sys,struct,json,hashlib
r=Path(__file__).resolve().parents[3];sys.path.insert(0,str(r/'port/engine-animation/tests'))
from compiled_transforms_differential import Cpu,relocate,word
cpu=Cpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});raw=(r/'.local-inputs/combat-hit-fx-v1/spell_dh2_hurt_fire.bdae').read_bytes();base=cpu.data+0x10000;relocate(cpu,raw,base);root=word(raw,32);record=word(raw,root+40)+32
matches=[(n,a)for n,a in cpu.symbols.items()if 'CApplyValueExIA4_h' in n and 'Lin1Eh' in n and 'getInstance' in n]
instance=next(a for n,a in matches if n.startswith('_ZZ'));guard=next(a for n,a in matches if n.startswith('_ZGV'));vt=next(a for n,a in cpu.symbols.items()if n.startswith('_ZTV')and 'CApplyValueExIA4_h' in n and 'Lin1Eh' in n)
cpu.pointer(instance,vt+8);cpu.pointer(guard,1);selected=cpu.invoke(0x611ae0,[base+record]);assert selected==instance
node=cpu.data+0x1000;pos=node+0x800;quat=pos+16;scale=quat+16;cpu.uc.mem_write(pos,bytes(16));cpu.uc.mem_write(quat,struct.pack('<4I',0,0,0,0x3f800000));cpu.uc.mem_write(scale,struct.pack('<3I',*[0x3f800000]*3));cpu.invoke(0x599268,[node,123,pos,quat,scale]);assert cpu.invoke(0x5974e4,[node])==0 and cpu.invoke(0x5974f4,[node])==0
report=dict(validation='PASS',actual_fire_factory_executed=True,factory_result='full uchar4 SUseDefaultLerp; not component3 alpha',instance=hex(instance),vtable=hex(vt),actual_node_constructor_executed=True,camera_offset_word=0,rendering_layer=0,scope='Actual getAnimationTrackEx611ae0 chooses fulluchar4 over relocated exact FIRE sampler/default. Initialized singleton is explicit fixture. Whole ISceneNodeC1 and actual getter instructions produce both queue words0.',original_sha256=hashlib.sha256((r/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),asset_sha256=hashlib.sha256(raw).hexdigest())
(r/'port/engine-animation/reports/fire-material-factory-v3-original.json').write_text(json.dumps(report,indent=2)+'\n');print(report)
