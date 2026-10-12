from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
r=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(r/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});obj=c.data+0x1000;calls=[]
def returned():c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def text(a):
 b=bytearray()
 while c.uc.mem_read(a,1)!=b'\0':b+=c.uc.mem_read(a,1);a+=1
 return b.decode()
def hook(uc,a,size,user):
 if a==0x38c398:calls.append({'GameObjectC1':[c.reg(i) for i in range(4)]});returned()
 elif a==0x3140ec:calls.append({'CString':{'offset':hex(c.reg(0)-obj),'text':text(c.reg(1))}});returned()
 elif a==0x398da4:calls.append({'NetStructTrigger_offset':hex(c.reg(0)-obj)});returned()
c.uc.hook_add(UC_HOOK_CODE,hook);reports=[]
for entry in [0x39b890,0x39b990]:
 calls.clear();c.uc.mem_write(obj,b'\xa5'*0x800);c.invoke(entry,[obj,20]);word=lambda o:struct.unpack('<I',c.uc.mem_read(obj+o,4))[0];byte=lambda o:c.uc.mem_read(obj+o,1)[0]
 words={hex(o):word(o) for o in [0x374,0x378,0x37c,0x384,0x38c,0x398,0x3a0,0x3a4,0x3a8,0x3ac,0x3b4,0x3b8,0x3c0,0x718,0x71c,0x720,0x73c,0x758,0x774,0x790,0x7ac,0x7b0,0x7b8]}
 assert words['0x718']==words['0x71c']==words['0x720']==0xa5a5a5a5
 for o in [0x374,0x378,0x37c,0x384,0x38c,0x398,0x3a0,0x3a4,0x3ac,0x3b4,0x3b8,0x3c0,0x7b0,0x7b8]:assert word(o)==0,hex(o)
 assert word(0x3a8)==1
 for o in [0x73c,0x758,0x774,0x790,0x7ac]:assert word(o)==0xffffffff
 assert [byte(o) for o in [0x380,0x381,0x84,0x28,0xf8,0x3b0,0x3bc,0x7b4,0x7b5]]==[0,1,1,1,4,0,0,0,0]
 assert word(0x100)==obj+0x3c8 and word(0x104)==obj+0x570
 reports.append({'entry':hex(entry),'calls':list(calls),'words':words,'bytes':{hex(o):byte(o) for o in [0x380,0x381,0x84,0x28,0xf8,0x3b0,0x3bc,0x7b4,0x7b5]}})
report={'status':'PASS','original_sha256':hashlib.sha256((r/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'constructors':reports,'scope':'Whole TriggerZone→Trigger→ZoneEx→Zone stores execute. Previously proven GameObject C1, CString construction and separate whole NetStructTrigger C1 are explicit service interceptions. Poison proves all dimensions ctor0 and scope/type/cine unproduced.'}
(r/'port/level-world/reference/trigger-zone-v22/constructor-original.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
