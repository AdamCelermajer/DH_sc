"""Whole LVLS reader and both saved-state setters versus original ARM.
Real original instructions resolve names, consume six groups, validate source
states, read assertion mode and mutate the six retained Save arrays. Stream
primitives, temporary string lifetime, strcmp and assertion logging are explicit
observing providers. Original unsafe array/null stores are stopped at their
reached store instruction and compared as native failure prefixes. Difficulty
outside0..2, malformed streams, limits and provider errors are separate native
policies. This does not validate a native SG_Load/InitPost binding.
"""
from __future__ import annotations
import argparse,hashlib,json,os,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R6,UC_ARM_REG_SP,UC_ARM_REG_LR,UC_ARM_REG_PC
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put
PIN=MODULE/'reference/player-saved-level-states-v1/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
FUNCTIONS=[('_ZN14PlayerSavegame17__LoadLevelStatesEP11IStreamBasePv',0x46a9cc,648),('_ZN14PlayerSavegame16SG_SetLevelStateEiii',0x466e48,504),('_ZN14PlayerSavegame17SG_SetMapLocStateEiii',0x466b18,504)]
SAVE=0x10001000;STREAM=0x10003000;STOP=0x30000000;COUNTS=[0x9a65e4,0x9a66f8];NAMES=[0x9a65ec,0x9a6700];ASSERT=0x99f914
NAME_ARRAY=[0x10008000,0x10008100];TEXT=0x1000c000;ARRAY=0x10010000
def sw(value):return struct.unpack('<i',struct.pack('<I',value&0xffffffff))[0]
def pins(data,names):
 rows=[]
 for symbol,at,size in FUNCTIONS:
  assert(names[symbol]['st_value'],names[symbol]['st_size'])==(at,size)
  rows.append(dict(original_symbol=symbol,elf_address=hex(at),size=size,sha256=hashlib.sha256(data[at:at+size]).hexdigest()))
 return dict(original_sha256=SHA,functions=rows,table_globals={'_ZN6Arrays9LevelList4sizeE':'0x9a65e4','_ZN6Arrays9LevelList13m_memberNamesE':'0x9a65ec','_ZN6Arrays8WorldMap4sizeE':'0x9a66f8','_ZN6Arrays8WorldMap13m_memberNamesE':'0x9a6700','gAssertLevel':'0x99f914'},scope=__doc__)
def word(value):return struct.pack('<I',value&0xffffffff)
def text(value):raw=value.encode()+b'\0';return word(len(raw))+raw
def payload(groups):
 result=b''
 for entries in groups:
  if isinstance(entries,int):result+=word(entries)
  else:result+=word(len(entries))+b''.join(text(name)+word(value) for name,value in entries)
 return result
def cases():
 result=[]
 def add(groups,levels=3,world=3,mode=0,mutation=0):result.append(([0,0,0,0,0,levels,world,mode,mutation],payload(groups)))
 add([[],[],[],[],[],[]]);add([-1,-2147483648,0,0,0,0])
 for counts in [(0,0),(0,3),(3,0),(1,1),(3,3)]:
  for name in ['L0','L1','L2','absent','l0']:
   add([[(name,1)],[],[],[('W2',2),('absent',-9)],[],[]],*counts)
 for d in range(6):
  for value in [-2147483648,-1,0,1,2,3,2147483647]:
   for mode in [0,1,2]:
    groups=[[] for _ in range(6)];groups[d]=[(('L' if d<3 else 'W')+'1',value)];add(groups,mode=mode)
 for d in range(6):
  groups=[[] for _ in range(6)];groups[d]=[(('L' if d<3 else 'W')+'0',0),(('L' if d<3 else 'W')+'0',1),('missing',99)];add(groups)
 for mode in [0,1,2]:add([[('L2',3)],[],[],[],[],[]],mode=mode,mutation=1)
 for table in [0,1]:
  for id in [-1,0,2,3]:
   for value in [-1,0,1,2,3]:
    for mode in [0,1,2]:
     for difficulty in [0,2]:result.append(([1,table,id,value,difficulty,3,3,mode,0],b''))
  result.append(([1,table,-1,-1,0,3,3,1,3],b''))
 return result
class Original:
 def __init__(self,path):
  assert hashlib.sha256(path.read_bytes()).hexdigest()==SHA
  data,names,_=image(path);self.pins=pins(data,names);assert self.pins==json.loads(PIN.read_text())
  for name,address in self.pins['table_globals'].items():assert names[name]['st_value']==int(address,0)
  self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);self.u.mem_map(0,len(data));self.u.mem_write(0,data)
  for base,size in [(0x10000000,0x30000),(0x20000000,0x10000),(STOP,0x1000)]:self.u.mem_map(base,size)
  self.words={at:set() for _,at,_ in FUNCTIONS};self.u.hook_add(UC_HOOK_CODE,self.code);self.u.hook_add(UC_HOOK_MEM_READ,self.read);self.u.hook_add(UC_HOOK_MEM_WRITE,self.write)
 def ret(self,value=0):u=self.u;u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def stream(self,length):
  self.read_calls+=1;assert self.cursor+length<=len(self.payload),'fixture primitive escaped defined stream'
  raw=self.payload[self.cursor:self.cursor+length];self.cursor+=length;return raw
 def code(self,u,address,size,context):
  r0=u.reg_read(UC_ARM_REG_R0);r1=u.reg_read(UC_ARM_REG_R1)
  if address==STOP:u.emu_stop();return
  if address in [0x38b758,0x461da8,0x31167c,0x3139ac,0x30e31c,0x30e004]:
   if address==0x38b758:assert r0==STREAM;u.mem_write(r1,self.stream(4));self.ret()
   elif address==0x461da8:
    assert r0==STREAM;self.string_reads+=1;length=struct.unpack('<I',self.stream(4))[0];raw=self.stream(length);assert raw[-1:]==b'\0';u.mem_write(TEXT,raw);put(u,r1+0x10,TEXT);put(u,r1+0x14,TEXT);self.ret()
   elif address==0x31167c:put(u,r0+0x10,TEXT);put(u,r0+0x14,TEXT);self.ret()
   elif address==0x3139ac:self.ret()
   elif address==0x30e31c:
    assert r0==TEXT;table,index=self.strings[r1];self.events.append([4,table,index,0]);self.comparisons+=1
    left=bytes(u.mem_read(TEXT,128)).split(b'\0',1)[0];right=bytes(u.mem_read(r1,128)).split(b'\0',1)[0]
    if self.mutation==1 and not self.changed and table==0:put(u,COUNTS[0],1);self.changed=True
    self.ret(0 if left==right else 1)
   else:
    caller=u.reg_read(UC_ARM_REG_LR)-4;mapping={0x466f34:(0,0),0x466ff8:(0,1),0x466f84:(0,2),0x466fc4:(0,3),0x466c04:(1,0),0x466cc8:(1,1),0x466c54:(1,2),0x466c94:(1,3)}
    table,condition=mapping[caller];line=get(u,u.reg_read(UC_ARM_REG_SP));self.events.append([6,table*4+condition,caller,line]);self.logs+=1
    if self.mutation==3:put(u,ASSERT,2)
    self.ret()
   return
  assert any(at<=address<at+size for _,at,size in FUNCTIONS),f'unscoped original {address:#x}'
  for _,at,size in FUNCTIONS:
   if at<=address<at+size:self.words[at].add(address)
  if address in [0x46aaec,0x46abcc]:self.completed+=1
  if address in [0x466ed8,0x466ba8]:
   id=sw(u.reg_read(UC_ARM_REG_R6));pointer=u.reg_read(UC_ARM_REG_R3)
   if id<0 or id>=3 or pointer not in self.arrays:self.failed=True;u.emu_stop()
 def read(self,u,access,address,size,value,context):
  if address in COUNTS:table=COUNTS.index(address);self.events.append([2,table,get(u,address),0]);self.counts+=1
  elif address in NAMES:table=NAMES.index(address);self.events.append([3,table,0,0]);self.name_arrays+=1
  elif address==ASSERT:self.events.append([5,get(u,address),0,0]);self.modes+=1
 def write(self,u,access,address,size,value,context):
  if address==0:self.failed=True;u.emu_stop();return
  if any(p<=address<p+12 for p in self.arrays):self.stores+=1
 def execute(self,row):
  fields,self.payload=row;kind,table,id,value,difficulty,levels,world,mode,self.mutation=fields;u=self.u
  self.cursor=self.read_calls=self.string_reads=self.counts=self.name_arrays=self.comparisons=self.modes=self.logs=self.stores=self.completed=0;self.events=[];self.strings={};self.changed=self.failed=False;self.arrays=[]
  u.mem_write(SAVE,bytes(0x198));put(u,ASSERT,mode)
  for t,count in enumerate([levels,world]):
   put(u,COUNTS[t],count);put(u,NAMES[t],NAME_ARRAY[t])
   for index in range(3):
    p=0x10009000+t*0x1000+index*0x100;u.mem_write(p,(('L' if t==0 else 'W')+str(index)).encode()+b'\0');put(u,NAME_ARRAY[t]+index*4,p);self.strings[p]=(t,index)
   for d in range(3):
    p=ARRAY+t*0x300+d*0x100;self.arrays.append(p);put(u,SAVE+(0x68 if t==0 else 0x74)+d*4,p)
    for index in range(3):put(u,p+index*4,100+t*50+d*10+index)
  if kind:
   u.reg_write(UC_ARM_REG_R0,SAVE);u.reg_write(UC_ARM_REG_R1,id&0xffffffff);u.reg_write(UC_ARM_REG_R2,value&0xffffffff);u.reg_write(UC_ARM_REG_R3,difficulty&0xffffffff);entry=FUNCTIONS[1+table][1]
  else:u.reg_write(UC_ARM_REG_R0,STREAM);u.reg_write(UC_ARM_REG_R1,SAVE);entry=FUNCTIONS[0][1]
  u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP);u.emu_start(entry,STOP+4,count=50000)
  assert self.failed or u.reg_read(UC_ARM_REG_PC)==STOP
  values=[int(self.failed),self.cursor,self.read_calls,self.string_reads,self.counts,self.name_arrays,self.comparisons,self.modes,self.logs,self.stores,self.completed]
  result=struct.pack('<11I',*values)+b''.join(bytes(u.mem_read(p,12)) for p in self.arrays)+word(len(self.events))+b''.join(struct.pack('<4I',*e) for e in self.events)
  return result
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--library',type=Path);p.add_argument('--write-pins',action='store_true');a=p.parse_args()
 if a.write_pins:
  assert hashlib.sha256(a.original.read_bytes()).hexdigest()==SHA;data,names,_=image(a.original);PIN.parent.mkdir(parents=True,exist_ok=True);PIN.write_text(json.dumps(pins(data,names),indent=2)+'\n');return
 a.output.mkdir(parents=True,exist_ok=True);original=Original(a.original);rows=cases();expected=b'';sizes=[]
 for row in rows:r=original.execute(row);expected+=r;sizes.append(len(r))
 inputs=a.output/'fixtures.bin';outputs=a.output/'native.bin';exe=a.output/'player_saved_level_states_v1_host.exe'
 inputs.write_bytes(word(len(rows))+b''.join(struct.pack('<10I',*[v&0xffffffff for v in fields],len(data))+data for fields,data in rows))
 sources=['player_saved_level_states_v1.cpp'];command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror',str(MODULE/'tests/player_saved_level_states_v1_host.cpp')]
 command += [str(a.library)] if a.library else [str(MODULE/x) for x in [*sources,'player_save_level_states_v1.cpp']]
 subprocess.run(command+['-o',str(exe)],check=True);env=os.environ.copy();dlls=[]
 if a.library:dlls=sorted(a.library.parent.parent.rglob('*.dll'));env['PATH']=os.pathsep.join([*(str(x.parent) for x in dlls),env['PATH']])
 run=subprocess.run([str(exe),str(inputs),str(outputs)],env=env,check=True,capture_output=True,text=True);native=outputs.read_bytes()
 if native!=expected:
  cursor=0
  for i,n in enumerate(sizes):
   if native[cursor:cursor+n]!=expected[cursor:cursor+n]:raise AssertionError({'case':i,'input':rows[i],'native':list(struct.unpack('<'+'I'*(n//4),native[cursor:cursor+n])),'original':list(struct.unpack('<'+'I'*(n//4),expected[cursor:cursor+n]))})
   cursor+=n
  raise AssertionError((len(native),len(expected)))
 report=dict(validation='PASS',cases=len(rows),original_words={hex(k):len(v) for k,v in original.words.items()},native_policy_checks=int(run.stdout.strip()),compared_projection_bytes=len(native),implementation='selected library' if a.library else 'direct translation unit',native_wired=False,scope=original.pins['scope'],source_sha256={x:hashlib.sha256((MODULE/x).read_bytes()).hexdigest() for x in [*sources,'player_saved_level_states_v1.hpp','tests/player_saved_level_states_v1_host.cpp','tests/run_player_saved_level_states_v1.py','reference/player-saved-level-states-v1/original-functions.json']})
 if a.library:
  build=next(parent for parent in a.library.parents if (parent/'compile_commands.json').is_file());commands=json.loads((build/'compile_commands.json').read_text());paths={(MODULE/x).resolve() for x in sources};report['selected_commands']=[c for c in commands if Path(c['file']).resolve() in paths];assert len(report['selected_commands'])==len(sources);report['binary_sha256']={x.name:hashlib.sha256(x.read_bytes()).hexdigest() for x in [exe,*dlls]}
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_sha256','selected_commands','binary_sha256']}))
if __name__=='__main__':main()
