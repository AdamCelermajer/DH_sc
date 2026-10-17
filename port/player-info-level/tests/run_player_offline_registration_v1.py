"""Offline registration callers against original ARM and the selected library.

Full PlayerInfo lifecycle, map internals, platform inputs and joining/HUD are
declared providers in this caller proof; their integration has separate gates.
"""
from __future__ import annotations
import argparse,hashlib,json,os,random,struct,subprocess
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_PC,UC_ARM_REG_LR,UC_ARM_REG_SP
from player_locality_v1_original import image,get,put,signed,ELF_SHA
from run_input_manager_v1 import f32,word,number
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/player-info-level'
PIN=MODULE/'reference/player-offline-registration-v1/original-functions.json'
MANAGER=0x10001000;FALLBACK=MANAGER+8;INPUT=0x10013000;LEVEL=0x10014000;HUD=0x10015000
STOP=0x30000000;GET_PAD=STOP+0x100
FUNCTIONS=[0x36d280,0x378a40,0x36ed0c,0x378c80]
FIELDS=[0x664,0x668,0x66c,0x670,0x674,0x678,0x67c,0x240]
def player(key):return 0x10003000+(key+1)*0x1000
def cases():
 base=[0,0,-1,0,1,0,5,1,0,-1,0,word(0),word(0),word(0),0,0];rows=[]
 def add(**kw):
  row=base.copy()
  for k,v in kw.items():row[int(k[1:])]=v
  rows.append(row)
 for mask in range(16):
  for key in [-2,-1,0,1,2,3,4,2147483647]:add(v5=mask,v1=key)
  for localmask in [0,5,15]:add(v0=2,v5=mask,v6=localmask)
 for mask in [0,1,5,15]:
  for key in [-1,0,1,2,3,9]:
   for local in [0,1,7,255]:add(v0=1,v5=mask,v1=key,v2=17,v3=3,v4=local)
 for count in [-2,0,1,2,4]:
  for mask in [0,1,5,15]:
   for connected in [0,2,10,15]:add(v0=3,v5=mask,v7=count,v8=connected,v9=3)
 for value in [word(-float('inf')),word(-1),word(0),word(.5),word(1),word(float('inf')),0x7fc00001]:
  for edge in [0,1,255]:
   for level in [0,1]:add(v0=3,v5=1,v11=value,v14=edge,v15=level)
 for state in [0,2]:
  for slot in [-1,0,3]:add(v0=3,v5=15,v7=4,v8=15,v9=slot,v10=state,v11=word(1),v15=1)
 rng=random.Random(0x378c80)
 for _ in range(40):add(v0=3,v5=rng.randrange(16),v6=rng.randrange(16),v7=rng.randrange(5),v8=rng.randrange(16),v9=rng.choice([-1,3]),v10=rng.choice([0,2]),v11=rng.choice([word(.49),word(.5),word(.51),0x7fc00000]),v12=word(-1),v13=word(1),v14=rng.randrange(2),v15=rng.randrange(2))
 return rows
def execute(data,row,words):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
 for base,size in [(0x10000000,0x40000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
 u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP)
 keys={i for i in range(4) if row[5]&(1<<i)};trace=[];temporary=0
 def default(p):
  for off in FIELDS:put(u,p+off,-1 if off not in [0x66c,0x240] else 1 if off==0x66c else 0)
  put(u,p+0x660,0);put(u,p+0x680,0)
 def tree():
  ordered=sorted(keys);header=MANAGER+0x690;nodes=[player(k)-0x18 for k in ordered]
  for node,key in zip(nodes,ordered):
   put(u,node+16,key)
   for off in [4,8,12]:put(u,node+off,0)
  def build(start,end,parent):
   if start==end:return 0
   mid=(start+end)//2;node=nodes[mid];put(u,node+4,parent)
   put(u,node+8,build(start,mid,node));put(u,node+12,build(mid+1,end,node));return node
  put(u,header+4,build(0,len(nodes),header));put(u,header+8,nodes[0] if nodes else header)
  put(u,header+12,nodes[-1] if nodes else header);put(u,MANAGER+0x6a0,len(nodes))
 default(FALLBACK)
 for key in keys:
  p=player(key);default(p);put(u,p+0x664,row[9]);put(u,p+0x668,10+key)
  u.mem_write(p+0x66c,bytes([7 if row[6]&(1<<key) else 0]))
  for off,value in [(0x670,42+key),(0x674,55+key),(0x678,66+key),(0x67c,77+key)]:put(u,p+off,value)
  u.mem_write(p+0x240,bytes([row[10]]))
 tree();put(u,INPUT,INPUT+0x100);put(u,INPUT+0x108,GET_PAD);put(u,INPUT+12,row[7])
 for index in range(4):
  p=0x10020000+index*0x1000;u.mem_write(p+0x758,bytes([int(bool(row[8]&(1<<index)))]))
  for off,value in [(0x1d8,row[11]),(0x1e0,row[12]),(0x1e4,row[13])]:put(u,p+off,value)
  u.mem_write(p+0x1e8,bytes([row[14]]))
 def ret(value):u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def text(p):return bytes(u.mem_read(p,64)).split(b'\0',1)[0].decode()
 external={0x7fd794,0x37418c,0x378544,0x378808,0x371294,0x36dfb0,0x34dda4,GET_PAD,0x36f4b4,0x31f594,0x42ca8c,0x42cb8c,0x30eba4,0x30ed6c,0x30e4b4,0x30ed30,0x7ad7e8,0x797124,0x36f40c}
 def hook(machine,address,size,context):
  nonlocal temporary
  if address==STOP:machine.emu_stop();return
  a,b,c,d=[machine.reg_read(r) for r in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]]
  if address not in external:
   assert any(start<=address<start+size for start,size in zip(FUNCTIONS,[100,340,412,820])) or 0x34d754<=address<0x34d75c,hex(address)
   words.add(address)
  if address==0x7fd794:trace.append([1,0,0]);machine.mem_write(0x10017005,b'\0');ret(0x10017000)
  elif address==0x37418c:trace.append([2,0,0]);temporary=a;default(a);ret(a)
  elif address==0x378544:
   assert a==MANAGER+0x690;key=signed(get(u,b));trace.append([3,key,0])
   if key not in keys:keys.add(key);default(player(key));tree()
   ret(player(key))
  elif address==0x378808:
   assert b==temporary;key=(a-0x10003000)//0x1000-1;trace.append([4,key,0])
   for off in FIELDS:
    length=1 if off in [0x66c,0x240] else 4;machine.mem_write(a+off,bytes(machine.mem_read(b+off,length)))
   ret(a)
  elif address==0x371294:assert a==temporary;trace.append([5,0,0]);ret(a)
  elif address==0x36dfb0:
   key=signed(b);trace.append([6,key,c]);ret(player(key) if key in keys else FALLBACK)
  elif address==0x34dda4:trace.append([7,0,0]);ret(INPUT)
  elif address==0x34d754:trace.append([8,signed(get(u,INPUT+12)),0])
  elif address==GET_PAD:trace.append([9,b,0]);assert b<4;ret(0x10020000+b*0x1000)
  elif address==0x36f4b4:trace.append([10,b,signed(c)]);ret(0)
  elif address==0x31f594:trace.append([11,int(bool(row[15])),0]);ret(LEVEL if row[15] else 0)
  elif address==0x42ca8c:ret(HUD+0x100)
  elif address==0x42cb8c:trace.append([12,0,0]);ret(HUD)
  elif address in [0x30eba4,0x30ed6c]:ret(word(f32(number(a)+number(b)) if address==0x30eba4 else f32(number(a)*number(b))))
  elif address==0x30e4b4:ret(int(number(a)>=number(b)))
  elif address==0x30ed30:
   trace.append([13,signed(a),0]);lo,hi=struct.unpack('<II',struct.pack('<d',float(signed(a))))
   machine.reg_write(UC_ARM_REG_R1,hi);ret(lo)
  elif address==0x7ad7e8:
   assert a==HUD and text(b)=='menu_HUD_0' and text(c)=='onNewPlayerLocal'
   assert machine.mem_read(d+1,1)==b'\2';trace.append([14,0,0]);ret(1)
  elif address==0x797124:trace.append([15,0,0]);ret(0)
  elif address==0x36f40c:
   key=(a-0x10003000)//0x1000-1;trace.append([16,key,b]);machine.mem_write(a+0x240,bytes([b]));ret(0)
 u.hook_add(UC_HOOK_CODE,hook);mode=row[0]
 u.reg_write(UC_ARM_REG_R0,MANAGER);u.reg_write(UC_ARM_REG_R1,row[1]&0xffffffff)
 u.reg_write(UC_ARM_REG_R2,row[2]&0xffffffff);u.reg_write(UC_ARM_REG_R3,row[3]&0xffffffff);put(u,0x20008000,row[4])
 u.emu_start(FUNCTIONS[mode],STOP+4,count=30000);assert u.reg_read(UC_ARM_REG_PC)==STOP
 result=[]
 for key in sorted(keys):
  p=player(key);values=[machine_value(u,p+off,off) for off in FIELDS];result.append([key,*values])
 return dict(found=bool(u.reg_read(UC_ARM_REG_R0)) if mode==0 else False,players=result,trace=trace)
def machine_value(u,address,off):return u.mem_read(address,1)[0] if off in [0x66c,0x240] else signed(get(u,address))
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--library',type=Path,required=True);a=p.parse_args()
 data,names,_=image(a.original);pins=json.loads(PIN.read_text());words=set()
 for pin in pins['functions']:
  start=int(pin['elf_address'],0);symbol=names[pin['original_symbol']]
  assert (symbol['st_value'],symbol['st_size'])==(start,pin['size'])
  assert hashlib.sha256(data[start:start+pin['size']]).hexdigest()==pin['sha256']
 rows=cases();expected=[execute(data,row,words) for row in rows];a.output.mkdir(parents=True,exist_ok=True)
 inputs=a.output/'inputs.txt';inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n',encoding='utf-8')
 exe=a.output/'registration_host.exe';subprocess.run([a.compiler,'-std=c++17','-Wall','-Wextra','-Werror',str(MODULE/'tests/player_offline_registration_v1_host.cpp'),str(a.library),'-o',str(exe)],check=True)
 env=os.environ.copy();dlls=sorted(a.library.parent.parent.rglob('*.dll'));env['PATH']=os.pathsep.join([*(str(p.parent) for p in dlls),env['PATH']])
 policies=a.output/'failure-policies.json'
 native=json.loads(subprocess.run([str(exe),str(inputs),str(policies)],env=env,check=True,capture_output=True,text=True).stdout)
 mismatches=[dict(index=i,row=rows[i],original=old,native=new) for i,(old,new) in enumerate(zip(expected,native)) if old!=new]
 (a.output/'raw-comparison.json').write_text(json.dumps(dict(rows=rows,original=expected,native=native,mismatches=mismatches),indent=2)+'\n',encoding='utf-8')
 assert len(native)==len(expected) and not mismatches,mismatches[:2]
 integration=a.output/'registry_composition.exe'
 subprocess.run([a.compiler,'-std=c++17','-Wall','-Wextra','-Werror',str(MODULE/'tests/player_offline_registry_v1_host.cpp'),str(a.library),'-o',str(integration)],check=True)
 composition=json.loads(subprocess.run([str(integration)],env=env,check=True,capture_output=True,text=True).stdout)
 commands=json.loads((a.library.parent.parent/'compile_commands.json').read_text());selected=[c for c in commands if Path(c['file']).name=='player_offline_registration_v1.cpp'];assert len(selected)==1
 sources=['player_offline_registration_v1.cpp','player_offline_registration_v1.hpp','player_offline_registry_v1.cpp','player_offline_registry_v1.hpp','tests/player_offline_registration_v1_host.cpp','tests/player_offline_registry_v1_host.cpp','tests/run_player_offline_registration_v1.py','reference/player-offline-registration-v1/original-functions.json']
 report=dict(validation='PASS',cases=len(rows),distinct_original_words=len(words),selected_commands=selected,source_sha256={s:hashlib.sha256((MODULE/s).read_bytes()).hexdigest() for s in sources},binary_sha256={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in [exe,integration,*dlls]},scope='Offline branches of original membership/AddPlayer/renumber/CheckLocalControllers callers execute, including actual nested AddPlayer and renumber bodies. Lifecycle/map/input/joining/HUD boundaries are declared fixtures in the ARM caller comparison. Separate selected composition connects actual reconstructed lifecycle/input/map/slot/hosting modules. Network branches and complete Character/gameplay registration remain outside this gate.',original_sha256=ELF_SHA,failure_policies=json.loads(policies.read_text()),selected_composition=composition)
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items() if k not in ['selected_commands','source_sha256','binary_sha256']}))
if __name__=='__main__':main()
