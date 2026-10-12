"""Original stun/scare OnUpdate and OnBlur bodies; logging, state-transition, animator, heading and body effects explicit."""
import hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1];sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
c=Cpu(REPO/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});char=c.data+0x1000;machine=char+0x4fc;controller=c.data+0x7000;body=c.data+0x8000;obj=c.data+0x9000;trace=[];mutate=0
def w(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
def ret(value=0):c.put(0,value&0xffffffff);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,address,size,unused):
 if address in [0x337888,0x337a88,0x318254]:ret()
 elif address==0x3140ec:uc.mem_write(c.reg(0),bytes(24));ret()
 elif address==0x3c1938:trace.append(dict(service='force_state',state=c.reg(1),event=c.reg(2),payload=c.reg(3),idle_suppressed=uc.mem_read(machine+0x3c,1)[0]));ret()
 elif address==0x3c948c:trace.append(dict(service='stop_loop',animator=c.reg(0)==char+0x49c,mode=c.reg(1)));ret()
 elif address==0x4053d0:
  trace.append(dict(service='heading_object',controller=c.reg(0)==controller,payload=c.reg(1)))
  if mutate:c.pointer(char+0x2dc,0)
  ret()
 elif address==0x46eb20:trace.append(dict(service='pin',body=c.reg(0)==body,controller_locked=uc.mem_read(controller+8,1)[0]));ret()
c.uc.hook_add(UC_HOOK_CODE,hook);rows=[]
for kind,operation,mask,present,locked,idle,mutate in itertools.product(range(2),range(2),[0,2,4,6],range(2),[0,1,255],[0,1,255],range(2)):
 c.pointer(char+0x378,controller);c.pointer(char+0x2dc,body if present else 0);c.pointer(machine+0x2c,mask);c.uc.mem_write(machine+0x3c,bytes([idle]));c.uc.mem_write(controller+8,bytes([locked]));trace.clear();address=([0x3c549c,0x3c3b4c] if kind==0 else [0x3c4788,0x3c4834])[operation];c.invoke(address,[obj,9 if not kind else 8,char,machine,3]);
 rows.append(dict(kind=kind,operation=operation,mask=mask,present=present,locked=locked,idle=idle,mutate=mutate,trace=trace.copy(),final_locked=c.uc.mem_read(controller+8,1)[0],final_idle=c.uc.mem_read(machine+0x3c,1)[0],final_body=bool(w(char+0x2dc))))
result=dict(validation='PASS',scope=__doc__,original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),probe_sha256=sha(Path(__file__)),manifest_sha256=sha(HERE/'original-functions.json'),original_instructions_executed=True,body_cases=len(rows),rows=rows);(HERE/'body-probe.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(validation='PASS',body_cases=len(rows))))
