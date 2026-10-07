"""Whole shipping collision wrappers/filter stores; nav/Refilter are observers.
This is an original instruction receipt, not a native differential claim.
"""
from pathlib import Path
import sys,struct,json,hashlib
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_SP
engine=ROOT/'.local-inputs/libDungeonHunter2.so'
c=Cpu(engine,False,{'functions':[]});owner=c.data+0x1000;physical=c.data+0x4000
vt=c.data+0x8000;s1=c.data+0xa000;s2=c.data+0xb000
trace=[]
def ret(value=0):c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,address,size,unused):
 if address==0x7e7afc:
  shape=c.reg(1);trace.append(['refilter',shape-owner,list(struct.unpack('<3H',uc.mem_read(shape+0x22,6)))]);ret()
 elif address==0x528234:
  sp=uc.reg_read(UC_ARM_REG_SP)
  trace.append(['nav',c.reg(1)-owner,c.reg(2),c.reg(3),struct.unpack('<I',uc.mem_read(sp,4))[0]]);ret()
c.uc.hook_add(UC_HOOK_CODE,hook)
c.pointer(vt+0xb4,0x3a2ee8);c.pointer(vt+0xb8,0x3a2ef0);c.pointer(vt+0xbc,0x3a2efc)
cases=[]
for enabled in (False,True):
 for present in (False,True):
  for primary,secondary in ((False,False),(True,False),(True,True)):
   for disabled in (0,1,2):
    c.uc.mem_write(owner,bytes(0x2000));c.uc.mem_write(physical,bytes(0x40));c.pointer(owner,vt)
    c.pointer(owner+0x2dc,physical if present else 0)
    c.pointer(physical+0x18,s1 if primary else 0);c.pointer(physical+0x1c,s2 if secondary else 0)
    c.uc.mem_write(physical+0x20,struct.pack('<3H',0xfffb,0x1234,0xabcd));c.uc.mem_write(physical+0x26,bytes([disabled]))
    c.uc.mem_write(s1,bytes(0x80));c.uc.mem_write(s2,bytes(0x80));trace.clear()
    c.invoke(0x394a3c if enabled else 0x3949b0,[owner])
    reached=present and (disabled!=0 if enabled else disabled==0)
    expected_filter=[0xfffb,0x1234,0xabcd] if enabled else [0,0,0]
    expected=[]
    if reached:
     if primary:expected.append(['refilter',s1-owner,expected_filter])
     if secondary:expected.append(['refilter',s2-owner,expected_filter])
    expected.append(['nav',0x1c8,int(enabled),0x42480000,0x41a00000])
    assert trace==expected,(enabled,present,primary,secondary,disabled,trace,expected)
    if present:assert c.uc.mem_read(physical+0x26,1)==bytes([0 if enabled else 1])
    cases.append({'enabled':enabled,'body':present,'primary':primary,'secondary':secondary,'initial_disabled':disabled,'calls':list(trace)})
report={'original_sha256':hashlib.sha256(engine.read_bytes()).hexdigest(),'cases':len(cases),'mismatches':0,'whole_original_wrappers':['0x394a3c','0x3949b0'],'whole_original_filters':['0x46ebe4','0x46eb70'],'actual_character_virtual_leaves':['0x3a2ee8','0x3a2ef0','0x3a2efc'],'fixture_boundaries':['NativeWorld::Refilter delivery observer','PF528234 delivery observer; existing navigation differential separately proves its body'],'native_differential':False,'case_receipts':cases}
out=ROOT/'port/level-world/reports/character-collision-lifecycle-v1-original.json';out.write_text(json.dumps(report,indent=2)+'\n');print('PASS',len(cases),'whole original wrapper/filter cases',out)
