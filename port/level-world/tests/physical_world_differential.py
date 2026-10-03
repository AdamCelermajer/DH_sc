"""Original PhysicalWorld filter/contact/timing instructions vs native ARM64.

Debug-only logging/string/canary blocks are omitted in contact dispatch; the
original instigator function and both virtual calls execute. Collision policies,
velocity getters and contact recipients are supplied observers. Box2D Step is
an argument observer in timing cases; backend simulation has its own audit.
"""
import argparse,hashlib,json,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from navigation_differential import ROOT,Cpu as Base,float_bits
from combat_result_differential import floating
class Cpu(Base):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='world_callback':return self.world_callback(uc,address,size,unused)
  if self.imports.get(address)=='__aeabi_ui2f':
   self.put(0,float_bits(self.reg(0)&0xffffffff));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,unused)
def main():
 p=argparse.ArgumentParser()
 for name in ('engine','library','reference-output','report'):p.add_argument('--'+name,type=Path,required=True)
 a=p.parse_args();started=time.monotonic();manifest=json.loads((ROOT/'reference/physical-world/original-functions.json').read_text())
 assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});rng=random.Random(20261003)
 os=[old.data+0x1000+i*0x100 for i in range(2)];ob=[old.data+0x2000+i*0x100 for i in range(2)];oo=[old.data+0x3000+i*0x100 for i in range(2)]
 vt=old.data+0x4000;point=old.data+0x5000;world=old.data+0x6000
 ns=[new.data+0x1000+i*16 for i in range(2)];no=[new.data+0x2000+i*0x100 for i in range(2)];np=new.data+0x3000;ndt=new.data+0x4000
 events=[[],[]];filters=[];values=[];accept=[];current_event=0;time_ms=0;step=[];records=[];totals=[0]*6
 contexts=[0xabc00000001,0xabc00000002]
 def returned(cpu,v=0):cpu.put(0,v);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
 def ident(cpu,value):return oo.index(value) if cpu is old else contexts.index(value)
 def callback(cpu,address):
  who=ident(cpu,cpu.reg(0));side=int(cpu is new);offset=address-cpu.callback
  if offset==32:
   other=ident(cpu,cpu.reg(1))
   if cpu is old:
    stack=struct.unpack('<4I',cpu.uc.mem_read(cpu.uc.reg_read(cpu.sp),16));group=cpu.reg(2)&0xffff;category=cpu.reg(3)&0xffff
    f1=struct.pack('<hHHH',group if group<0x8000 else group-0x10000,category,stack[0]&0xffff,1)
    f2=struct.pack('<hHHH',stack[1] if stack[1]<0x80000000 else stack[1]-0x100000000,stack[2]&0xffff,stack[3]&0xffff,1)
   else:f1=bytes(cpu.uc.mem_read(cpu.reg(2),8));f2=bytes(cpu.uc.mem_read(cpu.reg(3),8))
   events[side].append((4,who,other,f1+f2));returned(cpu,accept[who]);return
  if offset==48:
   out=cpu.reg(1);cpu.uc.mem_write(out,values[who][12:20]);events[side].append((5,who,0,b''));returned(cpu);return
  if cpu is old:
   other=ident(cpu,cpu.reg(1));inst=cpu.reg(2) if current_event==3 else cpu.reg(3);payload=b'' if current_event==3 else bytes(cpu.uc.mem_read(cpu.reg(2),8))
  else:
   assert cpu.reg(1)==current_event;other=ident(cpu,cpu.reg(2));inst=cpu.reg(4);payload=b'' if current_event==3 else bytes(cpu.uc.mem_read(cpu.reg(3),8))
  events[side].append((current_event,who,other,payload+struct.pack('<I',inst)));returned(cpu)
 def old_hook(uc,address,size,unused):
  entries={0x34c6c8:(5,0x34,0x34c730),0x34c7f8:(5,0x34,0x34c860),0x34c41c:(5,0x34,0x34c484),0x34c54c:(6,0x20,0x34c5b0)}
  if address in entries:
   reg,local,target=entries[address];old.put(reg,old.reg(1));old.put(10,old.reg(0));uc.reg_write(old.sp,uc.reg_read(old.sp)-local);uc.reg_write(old.pc,target);return
  exits={0x34c7bc:0x34c7d0,0x34c8ec:0x34c900,0x34c510:0x34c524,0x34c5fc:0x34c610}
  if address in exits:uc.reg_write(old.pc,exits[address]);return
  if address in (0x3136b4,0x3136b8):returned(old);return
  if address==0x31f66c:returned(old,time_ms);return
  if address==0x7e8b1c:step.append((old.reg(1),old.reg(2)));returned(old)
 old.uc.hook_add(UC_HOOK_CODE,old_hook)
 new.world_callback=lambda uc,address,size,unused:callback(new,address)
 old.world_callback=lambda uc,address,size,unused:callback(old,address)
 for offset in (32,48,64):
  new.imports[new.callback+offset]='world_callback';old.imports[old.callback+offset]='world_callback'
 for offset in (8,12,16,20,24):old.pointer(vt+offset,old.callback+(32 if offset==8 else 64))
 old.pointer(vt+28,old.callback+48)
 def prepare(present,f,v):
  filters[:]=f;values[:]=v
  for i in range(2):
   old.uc.mem_write(os[i],bytes(0x40));old.uc.mem_write(ob[i],bytes(0x100));old.uc.mem_write(oo[i],bytes(0x40));old.pointer(oo[i],vt)
   old.pointer(os[i]+12,ob[i]);old.pointer(os[i]+44,oo[i] if present&(1<<i) else 0)
   group,category,mask,_=struct.unpack('<hHHH',f[i]);old.uc.mem_write(os[i]+34,struct.pack('<HHh',category,mask,group))
   old.uc.mem_write(ob[i]+4,v[i][:8]);old.uc.mem_write(ob[i]+116,v[i][8:12])
   new.uc.mem_write(no[i],struct.pack('<4Q',contexts[i],new.callback+32,new.callback+64,new.callback+48)+v[i][:12]+bytes(4))
   new.uc.mem_write(ns[i],struct.pack('<Q',no[i] if present&(1<<i) else 0)+f[i])
  old.uc.mem_write(point,struct.pack('<2I',*os)+b'\x00'*64);new.uc.mem_write(np,b''.join(bytes(new.uc.mem_read(n,16)) for n in ns)+bytes(8))
 for ci in range(2500):
  present=ci%4;f=[struct.pack('<hHHH',rng.choice((-4,-1,0,1,2)),rng.randrange(65536),rng.randrange(65536),1) for _ in range(2)]
  if ci%3==0:f[1]=f[0]
  v=[struct.pack('<5f',rng.uniform(-5,5),rng.uniform(-5,5),rng.choice((0,1,2)),rng.uniform(-10,10),rng.uniform(-10,10)) for _ in range(2)]
  accept[:]=[(ci//4)%2,(ci//8)%2];prepare(present,f,v);events[0].clear();events[1].clear()
  expected=old.invoke(0x34c304,[world,*os]);actual=new.invoke('dh2_physical_world_should_collide',ns)
  assert expected==actual and events[0]==events[1],('filter',ci,expected,actual,events)
  records.append(struct.pack('<4I',4,present,*accept)+b''.join(f)+b''.join(v)+bytes(8)+struct.pack('<II',expected,len(events[0]))+encode(events[0]));totals[4]+=1
 for ci in range(3500):
  current_event=ci%4;present=ci%11%4;accept[:]=[1,1];f=[struct.pack('<hHHH',0,1,65535,1)]*2
  special=(0.,-0.,1.,-1.,float('inf'),float('-inf'),float('nan'))
  v=[struct.pack('<5f',*[rng.choice(special) if ci%3==0 else rng.uniform(-50,50) for _ in range(5)]) for _ in range(2)]
  coordinate=struct.pack('<2f',*[rng.choice(special) if ci%3==0 else rng.uniform(-50,50) for _ in range(2)])
  prepare(present,f,v);old.uc.mem_write(point+8,coordinate);new.uc.mem_write(np+32,coordinate);events[0].clear();events[1].clear()
  old.invoke((0x34c6c4,0x34c7f4,0x34c418,0x34c548)[current_event],[world,point]);assert new.invoke('dh2_physical_world_contact',[np,current_event])==0
  assert events[0]==events[1],('contact',ci,events)
  records.append(struct.pack('<4I',current_event,present,*accept)+b''.join(f)+b''.join(v)+coordinate+struct.pack('<II',0,len(events[0]))+encode(events[0]));totals[current_event]+=1
 timing=[]
 for time_ms in (0,1,16,33,1000,-1,-2147483648,2147483647,*[rng.randrange(-2147483648,2147483648) for _ in range(992)]):
  step.clear();old.invoke(0x34bd08,[world]);assert len(step)==1
  assert new.invoke('dh2_physical_world_step_arguments',[ndt,ndt+4,time_ms])==0
  actual=struct.unpack('<II',new.uc.mem_read(ndt,8));assert actual==step[0],('timing',time_ms,step,actual);timing.append(struct.pack('<III',time_ms&0xffffffff,*actual));totals[5]+=1
 gold=struct.pack('<III',0x31575750,len(records),len(timing))+b''.join(records)+b''.join(timing)
 a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(gold)
 report={'comparisons':sum(totals),'operations':totals,'mismatches':0,'ordered_callback_observations':sum(struct.unpack_from('<I',r,84)[0] for r in records),'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(gold).hexdigest(),'original_import_calls':old.import_calls,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
def encode(events):
 return b''.join(struct.pack('<4I',kind,who,other,len(data))+data+bytes(16-len(data)) for kind,who,other,data in events)
if __name__=='__main__':main()
