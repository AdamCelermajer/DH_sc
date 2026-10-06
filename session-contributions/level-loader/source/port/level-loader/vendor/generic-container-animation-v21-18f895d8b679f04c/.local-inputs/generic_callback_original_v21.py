from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
r=Path(__file__).resolve().parent.parent
sys.path.insert(0,str(r/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
obj=c.data+0x1000;cb=c.data+0x50000;ctx=c.data+0x2000
records=[];calls=[]
def word(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
def hook(uc,address,size,user):
 if address==cb:
  calls.append([c.reg(0),c.reg(1),c.uc.mem_read(obj+0x30,1)[0]])
  # A reentrant source callback can write the pending flag. Original clears it
  # after returning, so this write must NOT survive CheckCallback.
  c.uc.mem_write(obj+0x30,bytes([99]))
  uc.reg_write(c.pc,uc.reg_read(c.lr))
c.uc.hook_add(UC_HOOK_CODE,hook)
for pending in [0,1,2,128,255]:
 for present in [False,True]:
  calls.clear();c.uc.mem_write(obj,bytes(0x80));c.uc.mem_write(obj+0x30,struct.pack('<III',pending,cb if present else 0,ctx))
  c.invoke(0x36440c,[obj,obj+0x60]);after=c.uc.mem_read(obj+0x30,1)[0]
  assert after==(0 if pending and present else pending)
  assert len(calls)==int(bool(pending and present))
  if calls:assert calls[0]==[obj+0x60,ctx,pending]
  records.append({'pending':pending,'callback':present,'after':after,'calls':list(calls)})
report={'status':'PASS','original_sha256':hashlib.sha256((r/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'function':'CheckCallback36440c','cases':len(records),'records':records,'scope':'Whole original pending/null/callback ordering; callback body is an observer with explicit reentrant pending mutation. No container/event/visual success claimed.'}
(r/'port/level-world/reference/destructible-container-v21/check-callback-original.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
