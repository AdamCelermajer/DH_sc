from pathlib import Path
import sys,struct,json,hashlib
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages');sys.path.insert(0,str(root/'port/engine-animation/tests'))
from particle_factory_differential import FactoryCpu
from unicorn import UC_HOOK_CODE
c=FactoryCpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
frame=c.data+4096;array=frame+256;values=array+256;receiver=values+256;vt=receiver+256;service=vt+256;actor=service+256;buffer=actor+0x2000;result=buffer+256
trace=[];pending='';live=True;mutate=False
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,a,n,user):
 global pending
 if a==0x43c388:trace.append(['player',c.reg(0),c.reg(1)]);ret(actor if live else 0)
 elif a==0x508ef4:
  trace.append(['parse',c.string(c.reg(2)).decode(),c.reg(3)]);c.pointer(c.reg(1)+20,buffer)
  if mutate:c.pointer(actor+0x39c,23)
  ret(1)
 elif a==0x413a7c:pending=c.string(c.reg(1)).decode();uc.mem_write(c.reg(0),bytes(20));ret(c.reg(0))
 elif a==service:
  ptr=c.reg(2);kind=uc.mem_read(ptr+1,1)[0];value=struct.unpack('<d',uc.mem_read(ptr+4,8))[0]if kind==2 else c.string(struct.unpack('<I',uc.mem_read(ptr+4,4))[0]).decode();trace.append(['member',pending,value]);ret(0)
 elif a==0x797350:uc.mem_write(c.reg(0),bytes([0,4,0,0])+struct.pack('<I',c.reg(1))+bytes(4));ret()
 elif a==0x797250:uc.mem_write(c.reg(0),bytes([0,5,0,0])+struct.pack('<I',c.reg(1))+bytes(4));ret()
 elif a==0x797a54:c.put(0,0);c.put(1,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
 elif a==0x797960:ret(1)
 elif a in (0x31167c,0x3139ac,0x797124):ret(c.reg(0))
c.uc.hook_add(UC_HOOK_CODE,hook)
c.pointer(frame,result);c.pointer(frame+12,array);c.pointer(array,values);c.pointer(receiver,vt);c.pointer(vt+0x1c,service);c.uc.mem_write(buffer,b'actual StringManager output\0')
rows=[]
for argc in (1,2,3,4):
 for is_live in (False,True):
  live=is_live;mutate=argc==2;trace.clear();c.pointer(actor+0x39c,17);c.pointer(frame+16,argc);c.pointer(frame+20,argc-1)
  c.uc.mem_write(values,bytes(12*argc));c.uc.mem_write(result,bytes([0,0,0,0])+bytes(8))
  if argc==2:c.uc.mem_write(values,bytes([0,5,0,0])+struct.pack('<I',receiver)+bytes(4))
  if argc==3:c.uc.mem_write(values,bytes([0,1,0,0])+struct.pack('<I',1)+bytes(4))
  c.invoke(0x44a5c4,[frame]);expected=[['player',0,int(argc==3)]]
  if live:
   expected.append(['parse','^d',17])
   if argc==2:expected += [['member','Gold',23.],['member','GoldString','actual StringManager output']]
  assert trace==expected,(argc,live,trace)
  kind=c.uc.mem_read(result+1,1)[0];assert kind==(5 if argc==2 else 4)if live else kind==0
  rows.append(dict(argc=argc,live=live,result_kind=kind,trace=trace.copy()))
out=dict(validation='PASS',cases=rows,original_sha256=hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),scope='Whole original NativeInvGetPlayerGold ARM wrapper; explicit AS conversion/member/result, CString/player and StringManager parse provider fixtures. Actual formatter is separately retained HudText508ef4 production binding.')
dest=root/'port/engine-ui/reference/character-menu-gold-v4';dest.mkdir(exist_ok=True);(dest/'original-proof.json').write_text(json.dumps(out,indent=2));print(json.dumps(out))
