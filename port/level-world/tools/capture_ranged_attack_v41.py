"""Whole original AI_DoRangeAttack; named capability/search/LookAt/FSM endpoints."""
from pathlib import Path
import sys,json,struct,random,hashlib
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages');sys.path.insert(0,str(root/'port/level-world/tests'))
from navigation_differential import Cpu
from unicorn import UC_HOOK_CODE
engine=root/'.local-inputs/libDungeonHunter2.so';cpu=Cpu(engine,False,{'functions':[]})
owner=cpu.data+0x1000;ai=owner+0x3c8;target=cpu.data+0x4000;candidate=cpu.data+0x6000;other=cpu.data+0x8000;vt=cpu.data+0xa000;state=cpu.data+0xb000;buffer=cpu.data+0xc000;virtual=[cpu.data+0xd000+i*16 for i in range(3)]
for at in virtual:cpu.uc.mem_write(at,bytes.fromhex('1eff2fe1'))
cpu.pointer(vt+0x34,virtual[0]);cpu.pointer(vt+0x128,virtual[1]);cpu.pointer(vt+0x28,virtual[2]);f={};trace=[];debug='';list_handle=0
keys=['dead','blocked','range','attacking','last','heading','found','can_attack','target_dead','player','spec','explicit','current','mutation','maximum','frontal']
def word(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
def ret(v=0):cpu.put(0,v);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
def text(at):return bytes(cpu.uc.mem_read(at,128)).split(b'\0')[0].decode()
def identity(p):return {0:0,owner:1,target:2,candidate:3,other:4}[p]
def record(op,peer=0,a=0,b=0):trace.append((op,peer,a&0xffffffff,b&0xffffffff))
def hook(uc,at,n,_):
 global debug,list_handle
 if at==virtual[0]:record(7 if cpu.reg(0)!=owner else 0,identity(cpu.reg(0)) if cpu.reg(0)!=owner else 0);ret(f['target_dead'] if cpu.reg(0)!=owner else f['dead'])
 elif at==virtual[1]:record(1);uc.mem_write(cpu.reg(1),struct.pack('<i',10));uc.mem_write(cpu.reg(2),struct.pack('<i',f['maximum']));uc.mem_write(cpu.reg(3),struct.pack('<i',4));ret(f['range'])
 elif at==virtual[2]:record(8);ret(f['player'])
 elif at==0x3c02d0:record(2) # execute original SM_GetState/IsAttacking
 elif at==0x3d01ac:record(3,identity(cpu.reg(1)),cpu.reg(2));ret()
 elif at==0x337888:ret()
 elif at==0x3140ec:debug=text(cpu.reg(1));ret()
 elif at==0x337a88:record(4,0,int(debug=='isTracingChar_MeleePotentialTarget'));ret(0)
 elif at==0x318254:ret()
 elif at in (0x4a2240,0x4a191c):ret()
 elif at==0x4a2730:list_handle=cpu.reg(0);record(5,0,word(cpu.uc.reg_read(cpu.sp))) # actual ctor executes
 elif at==0x3d015c:record(9) # actual empty/reset/comparator store executes
 elif at==0x3d67f4:
  record(6)
  if f['mutation']==1:uc.mem_write(owner+0x1b5,b'\1')
  ret(f['can_attack'])
 elif at==0x4c4bdc:
  assert (text(cpu.reg(1)),text(cpu.reg(2)))==('CharacterDesign','Attack_FrontalAngle');record(10,0,f['frontal']);ret(f['frontal'])
 elif at==0x3d0020:
  record(11,0,cpu.reg(1),cpu.reg(2));cpu.pointer(list_handle,buffer);cpu.pointer(list_handle+0x10,buffer+20 if f['found'] else buffer);cpu.pointer(buffer,candidate);ret()
 elif at==0x3d6890:
  record(12,identity(cpu.reg(1)));cpu.pointer(ai+0x40,cpu.reg(1))
  if f['mutation']==2:cpu.pointer(buffer,other)
  ret()
 elif at==0x393d48:record(13,identity(cpu.reg(1)));ret()
 elif at==0x38d18c:record(14);ret()
 elif at==0x3c6488:record(15,identity(cpu.reg(1)),cpu.reg(2));ret()
cpu.uc.hook_add(UC_HOOK_CODE,hook)
rng=random.Random(41);cases=[]
base=[0]*16;base[2]=1;base[9]=1;base[14]=100;base[15]=90
for k in range(14):
 row=base.copy();row[k]=1;cases.append(row)
cases.append(base.copy())
for _ in range(241):
 row=[rng.randrange(2) for _ in range(14)]+[rng.choice([1,100,1000]),rng.choice([90,-3,181])];row[13]=rng.randrange(3);cases.append(row)
records=[]
for row in cases:
 f=dict(zip(keys,row));trace=[];cpu.uc.mem_write(owner,bytes(0x1800));cpu.uc.mem_write(target,bytes(0x1800));cpu.pointer(owner,vt);cpu.pointer(target,vt);cpu.pointer(ai+4,owner);cpu.pointer(owner+0x51c,state);cpu.pointer(state,5 if f['attacking'] else 3);cpu.pointer(owner+0x528,f['blocked']);cpu.uc.mem_write(owner+0x1b5,bytes([f['heading']]));cpu.uc.mem_write(ai+0x78,b'\x37');cpu.uc.mem_write(ai+0x79,bytes([f['last']]));cpu.pointer(ai+0x40,target if f['current'] else 0);cpu.pointer(ai+0x44,target if f['current'] else 0)
 cpu.invoke(0x3d076c,[ai,target if f['explicit'] else 0,f['spec']]);continued=bytes(cpu.uc.mem_read(ai+0x78,1))[0];chosen=identity(word(ai+0x40))
 records.append(struct.pack('<19I',*(x&0xffffffff for x in row),continued,chosen,len(trace))+b''.join(struct.pack('<4I',*x) for x in trace))
dest=root/'port/level-world/reference/ranged-attack-v41';blob=struct.pack('<I',len(records))+b''.join(records);(dest/'ranged-original.bin').write_bytes(blob)
report={'validation':'PASS','cases':len(records),'original_sha256':hashlib.sha256(engine.read_bytes()).hexdigest(),'corpus_sha256':hashlib.sha256(blob).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'scope':__doc__,'original_ctor_sort_and_state_predicate_execute':True,'whole_projectile_runtime':False};(dest/'ranged-original.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
