"""Actual source effect producers; Character AI row, constants, timer and registered-state transition services explicit."""
import hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1];sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu,string
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
c=Cpu(REPO/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});char=c.data+0x1000;machine=char+0x4fc;row=c.data+0x6000;table=c.data+0x10000;trace=[];settings={}
def w(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
def ret(value=0):c.put(0,value&0xffffffff);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
got=(0x3c6028+8+w(0x3c612c))&0xffffffff;count_global=w(got+w(0x3c6130));table_global=w(got+w(0x3c6134));c.pointer(table_global,table)
for i in range(20):c.pointer(table+i*160+0x8c,1000+i*10);c.pointer(table+i*160+0x7c,2000+i*10)
def hook(uc,address,size,unused):
 if address==0x3a3024:trace.append(dict(service='boss_query',character=c.reg(0)==char));ret(row)
 elif address==0x3a3228:trace.append(dict(service='animation_table_id',stored=struct.unpack('<i',struct.pack('<I',w(char+0x1000)))[0]))
 elif address==0x3dbe24:
  trace.append(dict(service='timer_start',character=c.reg(0)==char+0x3b4,duration=c.reg(1),repeat=c.reg(2),event=c.reg(3),payload=w(c.uc.reg_read(c.sp)),mask_before=w(machine+0x2c)))
  if settings['mutate']:c.pointer(machine+0x2c,0x40000000)
  ret(0xffffffff)
 elif address==0x4c4bdc:trace.append(dict(service='constant',group=string(c,c.reg(1)).decode(),key=string(c,c.reg(2)).decode(),animation_before=struct.unpack('<i',struct.pack('<I',w(machine+0x28)))[0]));ret(settings['constant'])
 elif address==0x3a53e0:trace.append(dict(service='stance',character=c.reg(0)==char));ret(settings['stance'])
 elif address in [0x3c1938,0x3c5684]:
  if address==0x3c1938:trace.append(dict(service='force_state',state=c.reg(1),event=c.reg(2),payload=c.reg(3),animation=w(machine+0x28),mask=w(machine+0x2c)))
  else:trace.append(dict(service='raise_state_event',event=c.reg(1),payload=c.reg(2),animation=w(machine+0x28),mask=w(machine+0x2c)))
  if settings['mutate']:c.pointer(machine+0x24,0x100)
  ret()
c.uc.hook_add(UC_HOOK_CODE,hook);rows=[]
for kind,boss,index,mask,constant,mode,force,mutate in itertools.product(range(2),range(2),[-1,0,19,20],[0,2,4,6],[0,0x100,0x200,0xffffffff],range(2),range(2),range(2)):
 settings=dict(constant=constant,stance=-3 if mutate else 2,mutate=mutate);count=20;c.pointer(count_global,count);c.pointer(char+0x1000,index&0xffffffff);c.pointer(row+0x14,boss*4);c.pointer(machine+4,char);c.pointer(machine+0x24,0x12345000);c.pointer(machine+0x28,0x87654321);c.pointer(machine+0x2c,mask);trace.clear();duration=0xffffffff if mutate else 250;payload=c.data+0x20000
 c.invoke(0x3c5ffc if kind==0 else 0x3c6144,[machine,duration,mode,payload,force]);resolved=index if 0<=index<count else 17;pending=2 if kind==0 else 4;stance_mask=0x200 if kind==0 else 0x100;animation_base=(1000 if kind==0 else 2000)+resolved*10
 expected_flags=0x12345000 if boss else (0x100 if mutate else 0x12345000)|((0x800 if kind==0 else 0x400) if mode else 0)
 assert w(machine+0x24)==expected_flags
 if boss:assert w(machine+0x28)==0x87654321 and w(machine+0x2c)==mask and len(trace)==1
 else:
  assert w(machine+0x28)==(animation_base+(settings['stance'] if constant&stance_mask else 0))&0xffffffff
  assert w(machine+0x2c)==(mask if mask&pending else (0x40000000 if mutate else mask)|pending)
  timers=[t for t in trace if t['service']=='timer_start'];assert len(timers)==int(not(mask&pending))
  assert all(t['repeat']==t['payload']==0 and t['event']==(0x2b if kind==0 else 0x2c) for t in timers)
 rows.append(dict(kind=kind,boss=boss,index=index,count=count,mask=mask,constant=constant,mode=mode,force=force,mutate=mutate,stance=settings['stance'],duration=duration,trace=trace.copy(),final_flags=w(machine+0x24),final_animation=w(machine+0x28),final_mask=w(machine+0x2c)))
rejects=[]
for kind,count,index in itertools.product(range(2),[0,1,17],[-1,0,16,17]):
 settings=dict(constant=0,stance=0,mutate=0);c.pointer(count_global,count);c.pointer(char+0x1000,index&0xffffffff);c.pointer(row+0x14,0);c.pointer(machine+4,char);c.pointer(machine+0x24,0x11111111);c.pointer(machine+0x28,0x22222222);c.pointer(machine+0x2c,0);trace.clear();c.invoke(0x3c5ffc if kind==0 else 0x3c6144,[machine,123,0,0,0]);resolved=index if 0<=index<count else 17
 accepted=resolved<count;assert len(trace)>2 if accepted else len(trace)==2
 rejects.append(dict(kind=kind,count=count,index=index,resolved=resolved,accepted=accepted,trace=trace.copy(),final_flags=w(machine+0x24),final_animation=w(machine+0x28),final_mask=w(machine+0x2c)))
result=dict(validation='PASS',scope=__doc__,original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),probe_sha256=sha(Path(__file__)),manifest_sha256=sha(HERE/'original-functions.json'),original_instructions_executed=True,producer_cases=len(rows),table_gate_cases=len(rejects),services=['GetCharAI supplies actual row; IsBoss bit test executes','actual GetCharAnimTableId executes, count and table storage fixture','constant lookup result','GetAnimStance return','timer allocation/start boundary; return UINT_MAX intentionally ignored by actual caller','registered-state force/event transition with reentrant field mutation'],rows=rows,table_gates=rejects)
(HERE/'effect-probe.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(validation='PASS',producer_cases=len(rows),table_gates=len(rejects))))
