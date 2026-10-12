"""Actual stun/scare Focus and event bodies, including scalar random-heading instructions; gameplay and RNG results explicit."""
import hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1];sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu as Base,string
class Cpu(Base):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='__aeabi_i2f':
   value=struct.unpack('<i',struct.pack('<I',self.reg(0)))[0];self.put(0,struct.unpack('<I',struct.pack('<f',float(value)))[0]);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,unused)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
c=Cpu(REPO/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});char=c.data+0x1000;machine=char+0x4fc;controller=c.data+0x7000;body=c.data+0x8000;obj=c.data+0x9000;vt=c.data+0xa000;virtual=c.data+0x1e000;table=c.data+0x10000;trace=[];settings={};draw=0
def w(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
def ret(value=0):c.put(0,value&0xffffffff);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
got=(0x3c6028+8+w(0x3c612c))&0xffffffff;count_global=w(got+w(0x3c6130));table_global=w(got+w(0x3c6134));c.pointer(count_global,20);c.pointer(table_global,table);c.pointer(char,vt);c.pointer(vt+0x28,virtual);c.uc.mem_write(virtual,struct.pack('<I',0xe12fff1e))
for i in range(20):c.pointer(table+i*160+0x8c,1000+i*10);c.pointer(table+i*160+0x7c,2000+i*10)
def hook(uc,address,size,unused):
 global draw
 if address in [0x337888,0x337a88,0x318254]:ret()
 elif address==0x3140ec:uc.mem_write(c.reg(0),bytes(24));ret()
 elif address==0x3a3228:trace.append(dict(service='animation_table_id',stored=settings['index'],flags=w(machine+0x24)))
 elif address==0x4c4bdc:trace.append(dict(service='constant',group=string(c,c.reg(1)).decode(),key=string(c,c.reg(2)).decode()));ret(settings['constant'])
 elif address==0x3a53e0:trace.append(dict(service='stance'));ret(settings['stance'])
 elif address==0x3cacb0:trace.append(dict(service='set_animation',animator=c.reg(0)==char+0x49c,animation=c.reg(1)));ret()
 elif address==virtual:trace.append(dict(service='is_player',character=c.reg(0)==char));ret(settings['player'])
 elif address==0x3c26a0:
  value=settings['draws'][draw];draw+=1;trace.append(dict(service='random',bound=c.reg(0),value=value));ret(value)
 elif address==0x405374:trace.append(dict(service='heading_point',controller=c.reg(0)==controller,xyz=list(struct.unpack('<3I',uc.mem_read(c.reg(1),12)))));ret()
 elif address==0x3bc6b8:
  trace.append(dict(service='cancel_sneaking',character=c.reg(0)==char))
  if settings['mutate']:c.pointer(char+0x2dc,0)
  ret()
 elif address==0x46eae0:trace.append(dict(service='unpin',body=c.reg(0)==body));ret()
c.uc.hook_add(UC_HOOK_CODE,hook);rows=[];variants=[[0,9997,49,50],[9997,0,50,49],[5000,7000,0,99]]
for kind,index,constant,player,present,locked,mutate,variant in itertools.product(range(2),[-1,0,19],[0,0x100,0x200,0xffffffff],range(2),range(2),[0,255],range(2),range(3)):
 settings=dict(index=index,constant=constant,stance=-3 if mutate else 2,player=player,mutate=mutate,draws=variants[variant]);c.pointer(char+0x1000,index&0xffffffff);c.pointer(char+0x378,controller);c.pointer(char+0x2dc,body if present else 0);c.pointer(machine+0x24,0x12345678);c.uc.mem_write(controller+8,bytes([locked]));trace.clear();draw=0;c.invoke(0x3c3cc0 if not kind else 0x3c45c4,[obj,9 if not kind else 8,char,machine,3,0,0]);assert draw==(4 if kind else 0)
 rows.append(dict(kind=kind,operation=0,index=index,constant=constant,stance=settings['stance'],player=player,present=present,locked=locked,mutate=mutate,draws=variants[variant],trace=trace.copy(),final_flags=w(machine+0x24),final_locked=c.uc.mem_read(controller+8,1)[0],final_body=bool(w(char+0x2dc))))
for kind,event,variant in itertools.product(range(2),[0,0x22,0x23,0x2c],range(3)):
 settings=dict(index=0,constant=0,stance=0,player=0,mutate=0,draws=variants[variant]);c.pointer(char+0x378,controller);c.pointer(char+0x2dc,body);c.pointer(machine+0x24,0x12345678);c.uc.mem_write(controller+8,b'\xff');trace.clear();draw=0;c.invoke(0x3c0030 if not kind else 0x3c2b28,[obj,9 if not kind else 8,char,machine,event,0]);assert draw==(4 if kind and event==0x23 else 0)
 rows.append(dict(kind=kind,operation=1,event=event,index=0,constant=0,stance=0,player=0,present=1,locked=255,mutate=0,draws=variants[variant],trace=trace.copy(),final_flags=w(machine+0x24),final_locked=c.uc.mem_read(controller+8,1)[0],final_body=True))
report=dict(validation='PASS',scope=__doc__,original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),probe_sha256=sha(Path(__file__)),manifest_sha256=sha(HERE/'original-functions.json'),original_instructions_executed=True,focus_cases=sum(not r['operation'] for r in rows),event_cases=sum(bool(r['operation']) for r in rows),rows=rows);(HERE/'focus-probe.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',focus_cases=report['focus_cases'],event_cases=report['event_cases'])))
