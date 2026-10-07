"""Execute actual HUD attack receiver event branch and full held-dispatch branch.
Outer cache/Level/PM gates are separately tested native contracts; Cmd methods
are ordered callback observers here, not original controller execution."""
from pathlib import Path
import sys,struct,json,hashlib,random
R=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(R/'port/game-data/tests'))
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6
c=Cpu(R/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
state,event,actor=c.data+0x1000,c.data+0x2000,c.data+0x3000
calls=[];mode=0
def hook(uc,address,size,unused):
 if address==0x418f10:
  uc.reg_write(UC_ARM_REG_R4,event);uc.reg_write(UC_ARM_REG_R5,state);uc.reg_write(UC_ARM_REG_R6,0)
 elif address==0x41a818:
  uc.reg_write(UC_ARM_REG_R5,state);uc.reg_write(UC_ARM_REG_R6,actor)
 elif address==0x418d94 or address==0x41a840:uc.reg_write(c.pc,c.stop)
 elif address in (0x4057fc,0x405b04):
  calls.append((1 if address==0x4057fc else 2,c.reg(0),c.reg(1)));uc.reg_write(c.pc,uc.reg_read(c.lr))
c.uc.hook_add(UC_HOOK_CODE,hook)
rng=random.Random(20261006);records=[]
controller=12345
for i in range(320):
 held=rng.randrange(3);x=rng.randrange(-2000,2001);y=rng.randrange(-2000,2001);ev=rng.choice([0,3,4,5,6,7,8]);ooi=rng.choice([0,98765]);click=rng.randrange(256)
 c.uc.mem_write(state+9,bytes([held]));c.uc.mem_write(state+0x7c,struct.pack('<ii',x,y));c.uc.mem_write(event+8,struct.pack('<I',ev));c.uc.mem_write(event+0x24,b'\x00')
 c.invoke(0x418f10,[])
 after=c.uc.mem_read(state+9,1)[0];px,py=struct.unpack('<ii',c.uc.mem_read(state+0x7c,8));consumed=c.uc.mem_read(event+0x24,1)[0]
 c.uc.mem_write(state+9,bytes([held]));c.uc.mem_write(actor+0x14a4,struct.pack('<I',ooi));c.uc.mem_write(actor+0x413,bytes([click]));c.uc.mem_write(actor+0x378,struct.pack('<I',controller));calls.clear()
 c.invoke(0x41a818,[]);assert len(calls)<=1
 command,ctrl,arg=calls[0] if calls else (0,0,0);after_click=c.uc.mem_read(actor+0x413,1)[0]
 records.append(struct.pack('<IiiIIIIiiIIII',held,x,y,ev,ooi,click,after,px,py,consumed,command,ctrl,arg)+struct.pack('<I',after_click))
out=R/'port/engine-ui/reference/hud-attack-control-v46';out.mkdir(parents=True,exist_ok=True)
blob=struct.pack('<I',len(records))+b''.join(records);(out/'gold.bin').write_bytes(blob)
report={'cases':len(records),'scope':__doc__,'gold_sha256':hashlib.sha256(blob).hexdigest(),'original_sha256':hashlib.sha256((R/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest()}
(R/'port/engine-ui/reports/hud-attack-control-v46-original.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
