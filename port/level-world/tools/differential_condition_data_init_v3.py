"""Whole original ConditionDataC1/C2 and Init empty/Invalid paths.
CString reserve and libc strcmp are explicit allocation/string leaf services;
no condition table, compiler, predicate or successful nonempty backend is faked.
"""
from pathlib import Path
import sys,json,struct,importlib.util
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
from unicorn import UC_HOOK_CODE
root=Path(__file__).resolve().parents[3]
spec=importlib.util.spec_from_file_location('decor_probe',root/'port/level-world/tests/decor_scene_differential.py');dep=importlib.util.module_from_spec(spec);spec.loader.exec_module(dep)
cpu=dep.Cpu(root/'.local-inputs/libDungeonHunter2.so',False,dep.Dependencies(),json.loads((root/'port/level-world/reference/decor-scene/original-functions.json').read_text()))
for a in [0x33ed7c,0x33edd8,0x33eb28]:cpu.symbols[str(a)]=a
obj=cpu.data+0x1000;buffer=cpu.data+0x2000;calls=[]
def w(a,v):cpu.uc.mem_write(a,struct.pack('<I',v))
def ret(v=0):cpu.write_reg(0,v);cpu.uc.reg_write(cpu.pc_reg,cpu.uc.reg_read(cpu.lr_reg))
def cstr(a):return bytes(cpu.uc.mem_read(a,256)).split(b'\0')[0]
def hook(uc,a,size,user):
 if a==0x31167c:
  calls.append('CStringReserve');assert cpu.reg(0)==obj+4 and cpu.reg(1)==16;w(obj+0x14,buffer);w(obj+0x18,buffer);ret()
 elif a==0x30e31c:
  calls.append('strcmp');left,right=cstr(cpu.reg(0)),cstr(cpu.reg(1));ret(0 if left==right else -1 if left<right else 1)
cpu.uc.hook_add(UC_HOOK_CODE,hook)
rows=[]
for ctor in [0x33ed7c,0x33edd8]:
 calls.clear();cpu.uc.mem_write(obj,b'\xcd'*40);cpu.uc.mem_write(buffer,b'\xcd'*256);cpu.invoke(str(ctor),[obj]);assert bytes(cpu.uc.mem_read(obj+0x1c,5))==bytes(5);assert cstr(buffer)==b''
 rows.append({'constructor':hex(ctor),'compiled':0,'tested':0,'name':'','calls':list(calls)})
 for name in ['', 'Invalid']:
  calls.clear();data=name.encode()+b'\0';cpu.uc.mem_write(buffer,data);w(obj+0x14,buffer+(len(name)));w(obj+0x18,buffer);w(obj+0x1c,0x11223344);cpu.uc.mem_write(obj+0x20,b'\x55');cpu.invoke(str(0x33eb28),[obj]);assert bytes(cpu.uc.mem_read(obj+0x1c,5))==b'\x44\x33\x22\x11\x55'
  rows.append({'constructor':hex(ctor),'init_name':name,'compiled_preserved':0x11223344,'tested_preserved':85,'calls':list(calls)})
out=root/'port/level-world/reference/module-zone-v3/condition-data-original.json';out.write_text(json.dumps({'scope':__doc__,'checks':len(rows),'cases':rows},indent=2));print('Original ConditionData ctor/empty/Invalid PASS',len(rows))
