"""Whole original LightSetManagerC1 and name lookup; actual CString/libc leaves only."""
from pathlib import Path
import sys,json,struct,importlib.util
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
from unicorn import UC_HOOK_CODE
root=Path(__file__).resolve().parents[3]
spec=importlib.util.spec_from_file_location('decor_probe',root/'port/level-world/tests/decor_scene_differential.py');dep=importlib.util.module_from_spec(spec);spec.loader.exec_module(dep)
cpu=dep.Cpu(root/'.local-inputs/libDungeonHunter2.so',False,dep.Dependencies(),json.loads((root/'port/level-world/reference/decor-scene/original-functions.json').read_text()))
for a in [0x40d7d4,0x40c3cc]:cpu.symbols[str(a)]=a
obj=cpu.data+0x1000;query=cpu.data+0x1800;heap=cpu.data+0x2000;next_heap=heap;calls=[]
def word(a,v):cpu.uc.mem_write(a,struct.pack('<I',v))
def readword(a):return struct.unpack('<I',cpu.uc.mem_read(a,4))[0]
def cstr(a):return bytes(cpu.uc.mem_read(a,256)).split(b'\0')[0]
def ret(v=0):cpu.write_reg(0,v);cpu.uc.reg_write(cpu.pc_reg,cpu.uc.reg_read(cpu.lr_reg))
def hook(uc,a,size,user):
 global next_heap
 if a==0x31167c:
  receiver=cpu.reg(0);assert cpu.reg(1)==16;pointer=next_heap;next_heap+=256;word(receiver+0x10,pointer);word(receiver+0x14,pointer);cpu.uc.mem_write(pointer,b'\0'*256);calls.append('CString reserve16');ret(receiver)
 elif a==0x30de54:ret(len(cstr(cpu.reg(0))))
 elif a==0x3109e0:
  receiver,start,end=cpu.reg(0),cpu.reg(1),cpu.reg(2);data=bytes(cpu.uc.mem_read(start,end-start));assert len(data)<16;pointer=readword(receiver+0x14);cpu.uc.mem_write(pointer,data+b'\0');word(receiver+0x10,pointer+len(data));calls.append('CString assign '+data.decode());ret(receiver)
 elif a==0x30e31c:
  left,right=cstr(cpu.reg(0)),cstr(cpu.reg(1));ret(0 if left==right else -1 if left<right else 1)
cpu.uc.hook_add(UC_HOOK_CODE,hook)
cpu.uc.mem_write(obj,b'\xcd'*0x1a4);cpu.invoke(str(0x40d7d4),[obj])
names=[cstr(readword(obj+0x18+24*i)).decode() for i in range(4)]
assert names==['PlayerLight','SceneLight','CameraLight','MonsterLight']
rows=[]
for text,expected in [('PlayerLight',0),('SceneLight',1),('CameraLight',2),('MonsterLight',3),('',0),('sceneLight',0),('absent',0),('SceneLight\0X',1)]:
 pointer=cpu.data+0x1900;cpu.uc.mem_write(pointer,text.encode()+b'\0');word(query+0x14,pointer);value=cpu.invoke(str(0x40c3cc),[obj,query]);assert value==expected;rows.append({'name':text,'expected':value})
out=root/'port/level-world/reference/module-zone-v3/light-set-names-original.json';out.write_text(json.dumps({'scope':__doc__,'ctor_names':names,'ctor_calls':calls,'query_cases':rows},indent=2));print('Original whole LightSetManager ctor/name query PASS',len(rows))
