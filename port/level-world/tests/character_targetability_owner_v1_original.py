"""Original nested CharAI ctor prefix and whole SetIsTargetable argument gates.
Actual Lua argument access/getBool are named fixtures; all branch/store code
executes the original ARM ELF. Native host test covers same World projection.
"""
from pathlib import Path
import sys,struct,itertools,json,hashlib
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu,words
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R4,UC_ARM_REG_R1
c=Cpu(ROOT/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
actor=c.data+0x1000;args=c.data+0x3000;vector=c.data+0x3100;value=c.data+0x3200
mode='';boolean=0;calls=[]
def ret(v):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,at,size,user):
 if mode=='ctor' and at in (0x3cec7c,0x3ceddc):uc.reg_write(c.pc,c.stop)
 if mode=='set' and at==0x37baf8:assert c.reg(1)==0;calls.append('Arguments[0]');ret(value)
 if mode=='set' and at==0x31bc80:calls.append('Value.getBool');ret(boolean)
c.uc.hook_add(UC_HOOK_CODE,hook)
cases=0
for start in (0x3cec18,0x3ced78):
 for initial in (0,1,255):
  mode='ctor';c.uc.mem_write(actor+0x415,bytes([initial]));c.uc.reg_write(UC_ARM_REG_R4,actor+0x3c8);c.uc.reg_write(UC_ARM_REG_R1,0)
  c.invoke(start,[]);assert bytes(c.uc.mem_read(actor+0x415,1))==b'\1';cases+=1
for count,kind,boolean,initial in itertools.product((0,1,2),(0,1,3),(0,1),(0,1)):
 mode='set';calls=[];c.uc.mem_write(actor+0x415,bytes([initial]));c.pointer(args+4,vector)
 c.uc.mem_write(vector,words(value,value+44*count,value+44*count));c.uc.mem_write(value+4,words(kind))
 c.invoke(0x3b7a64,[args,0,actor]);answer=bytes(c.uc.mem_read(actor+0x415,1))[0]
 expected=boolean if count and kind==1 else initial
 assert answer==expected,(count,kind,boolean,initial,answer,expected)
 assert calls==(['Arguments[0]','Value.getBool']if count and kind==1 else[]);cases+=1
report=dict(validation='PASS',cases=cases,mismatches=0,scope=__doc__,original_sha256=hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),ctor_source=['Character+3c8 calls CharAI C2 at3aa210','CharAI C2 mov1 at3cec18 and store+4d at3cec6c','CharAI C1 duplicate3ced78/3cedcc'],sole_field='WorldActor.character.interactive415')
(ROOT/'port/android-native/reports/character-targetability-owner-v1-original.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
