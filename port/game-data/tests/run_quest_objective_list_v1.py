"""Whole ObjectiveList callers using actual retained QuestTable definitions.

C1/C2, AssignPyData, CreateObjectiveWithPyData, SetOwner, _loadData, the full
member-pointer stream LoopOnAll, and D1/D2 execute versus pinned original ARM.
The actual immutable QuestTable adapter and unchanged decoder feed native calls;
source fixtures use the matching prior original decoder projections, pinned to
those exact private cache hashes. Type substitutions are explicit synthetic
test generations; original factory dispatch table is pinned. Array alloc/free,
type factories, deleting virtual4 and stream member bodies are observing fixture
providers. They do not validate missing Objective actions/Compile or gameplay.
Assignment/destructor callback prefixes and source captured/live backing order
are compared; invalid native backing stops at original fault boundaries. The
single List owns its pointer publication, and factory records own sole scalar
fields/ActionRef. Lost arrays stay in the provider arena as in the source.
No Android compilation, native hookup or live gameplay is claimed.
"""
from __future__ import annotations
import argparse,hashlib,itertools,json,os,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put,signed,ELF_SHA
PIN=MODULE/'reference/quest-objective-list-v1/original-functions.json'
LIST=0x10001000;ARRAYS=0x10003000;DEFS=0x10004000;STREAM=0x10009000;OBJECTS=0x10010000;TABLES=0x10018000;STOP=0x30000000
FUNCTIONS=[('_ZN13ObjectiveListC1Ev',0x47a3d4,20),('_ZN13ObjectiveListC2Ev',0x47a3c0,20),('_ZN13ObjectiveList12AssignPyDataEPN7Structs20v2QuestObjectiveStubEi',0x47aa48,108),('_ZN13ObjectiveList25CreateObjectiveWithPyDataEPN7Structs20v2QuestObjectiveStubE',0x47a3e8,44),('_ZN13ObjectiveList8SetOwnerEP9Character',0x47a414,48),('_ZN13ObjectiveList9_loadDataEP11IStreamBase',0x47a798,44),('_ZN13ObjectiveListD1Ev',0x47a8cc,116),('_ZN13ObjectiveListD2Ev',0x47a940,116),('_ZN13ObjectiveList9LoopOnAllEM9ObjectiveFvP11IStreamBaseES2_',0x47a6f8,116)]
def word(v):return struct.pack('<I',v&0xffffffff)
def pack(v):return b''.join(word(x) for x in v)
def evidence(data,names):
 pins=[]
 for name,a,n in FUNCTIONS:
  assert (names[name]['st_value'],names[name]['st_size'])==(a,n)
  pins.append(dict(original_symbol=name,elf_address=hex(a),size=n,sha256=hashlib.sha256(data[a:a+n]).hexdigest()))
 prior=json.loads((MODULE/'reference/quest-table-bindings-v1/original-functions.json').read_text())
 return dict(original_sha256=ELF_SHA,functions=pins,dispatch_table=dict(elf_address='0x969908',size=56,entries=list(struct.unpack('<14I',data[0x969908:0x969940])),sha256=hashlib.sha256(data[0x969908:0x969940]).hexdigest()),cache=prior['cache'],reused_decoder=prior['reused_decoder'],prior_original_reference=prior['prior_original_reference'],remaining_external=dict(allocation='0x31056c',deallocation='0x310440',delete_virtual_slot='0x4',objective_stream_member='explicit direct/virtual ARM member pointer'),scope=__doc__)
def cases():
 rows=[]
 def add(**changes):
  row=[9,47,3,0,0,99,1,0x28,-1,0,0,0,3,3,1,1,0,-0x80000000,0,0]
  for key,value in changes.items():row[int(key[1:])]=value
  rows.append(row)
 for mode in [0,1,4,5,6,7]:
  for count,backing,nulls in itertools.product([-2,0,1,3],range(2),[0,1,2,7]):add(v0=mode,v13=count,v14=backing,v11=nulls)
 for count,backing,mutation in itertools.product([-2,0,1,3],range(2),[0,1,2,3,4,9,10]):add(v0=2,v2=count,v14=backing,v10=mutation)
 for row in range(64):
  for stub in [0,1,2]:add(v0=3,v1=row,v4=stub)
 for kind,stub in itertools.product([-2,-1,*range(14),2147483647],[0,1,2]):add(v0=3,v17=kind,v4=stub)
 for mode,mutation in [(5,5),(5,6),(6,7),(7,7),(6,8),(7,8),(5,11),(6,12)]:add(v0=mode,v10=mutation)
 for encoded,function in itertools.product([0,1,64,65,-64,-63],[0x28,0x30,STOP+0x108]):
  if not(encoded&1) and function!=STOP+0x108:continue
  if encoded&1 and function==STOP+0x108:continue
  add(v0=8,v6=encoded,v7=function)
 for mode in [2,3,5,6,7,9]:
  for fail,partial,throw in itertools.product(range(1,12),range(2),range(2)):add(v0=mode,v8=fail,v16=partial,v9=throw)
 for n in [0,1,2,3]:add(v0=9,v2=n,v13=n)
 return rows
class Original:
 def __init__(self,path,cache):
  data,names,_=image(path);self.pins=evidence(data,names);assert self.pins==json.loads(PIN.read_text());self.words=set();self.counts={a:set() for _,a,_ in FUNCTIONS};self.factories=self.pins['dispatch_table']['entries'][:13];self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);self.u.mem_map(0,len(data));self.u.mem_write(0,data)
  prior=self.pins['prior_original_reference'];assert hashlib.sha256((ROOT/prior['path']).read_bytes()).hexdigest()==prior['sha256'];self.definitions=json.loads((ROOT/prior['path']).read_text())['rows']
  for name,pin in self.pins['cache'].items():assert hashlib.sha256((cache/name).read_bytes()).hexdigest()==pin['sha256']
  for base,size in [(0x10000000,0x30000),(0x20000000,0x10000),(STOP,0x1000)]:self.u.mem_map(base,size)
  self.u.hook_add(UC_HOOK_CODE,self.code)
 def ret(self,v=0):self.u.reg_write(UC_ARM_REG_R0,v&0xffffffff);self.u.reg_write(UC_ARM_REG_PC,self.u.reg_read(UC_ARM_REG_LR))
 def event(self,op,target=0,value=0,extra=0):self.events.append([op,target,value&0xffffffff,extra&0xffffffff,self.cursor]);self.attempts+=1;return self.row[8]>0 and self.attempts==self.row[8]
 def failed_stop(self):self.failed=True;self.u.emu_stop()
 def array_id(self,p):return (p-ARRAYS)//0x100+1 if p else 0
 def object_id(self,p):return (p-OBJECTS)//0x100+1 if p else 0
 def code(self,u,a,size,context):
  if a==STOP:u.emu_stop();return
  r0,r1,r2,r3=[u.reg_read(r) for r in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]]
  if a==0x31056c:
   self.capacity[2]=min(r0//4,64);failed=self.event(1,3,r0,r1)
   if failed:self.failed_stop();return
   if self.row[10]==1:put(u,LIST,0)
   self.ret(0 if self.row[10]==10 else ARRAYS+0x200);return
  if a in self.factories:
   i=3+self.created;self.created+=1;kind=self.factories.index(a);failed=self.event(2,i+1,kind,r0)
   if failed:self.failed_stop();return
   if not self.mutated:
    if self.row[10]==2:put(u,LIST,1)
    if self.row[10]==3:put(u,LIST+4,ARRAYS+0x100)
    if self.row[10]==4:put(u,LIST+8,DEFS+self.row[18]*0x200)
    self.mutated=True
   self.ret(0 if self.row[10]==9 else OBJECTS+i*0x100);return
  if a==STOP+0x108:
   assert r1==STREAM
   adjustment=self.adjustment;i=next(i for i in range(15) if r0==OBJECTS+i*0x100+adjustment);q=OBJECTS+i*0x100;function=0x28 if self.row[0]!=8 else self.row[7]&0xffffffff
   failed=self.event(3,i+1,function,r0)
   if failed and self.row[16]:put(u,q+0x18,(get(u,q+0x18)&0xffff0000)|((0x70000000+self.cursor//4)&0xffff));self.cursor+=2
   if failed:self.failed_stop();return
   put(u,q+0x18,0x70000000+self.cursor//4);self.cursor+=4
   if not self.mutated:
    if self.row[10]==5:put(u,LIST,0)
    if self.row[10]==6:put(u,LIST+4,ARRAYS+0x100)
    self.mutated=True
   self.ret();return
  if a==STOP+0x104:
   i=self.object_id(r0);failed=self.event(4,i,4)
   if failed and self.row[16]:put(u,r0+0x1c,1)
   if failed:self.failed_stop();return
   put(u,r0+0x1c,1)
   if not self.mutated:
    if self.row[10]==7:put(u,LIST,1);put(u,LIST+4,ARRAYS+0x100)
    if self.row[10]==8:put(u,LIST,0)
    self.mutated=True
   self.ret();return
  if a==0x310440:
   i=self.array_id(r0);failed=self.event(5,i)
   if failed:self.failed_stop();return
   self.retired[i-1]=1;self.ret();return
  assert any(start<=a<start+n for _,start,n in FUNCTIONS),f'unscoped original {a:#x}'
  self.words.add(a)
  for _,start,n in FUNCTIONS:
   if start<=a<start+n:self.counts[start].add(a)
  if a==0x47a400 and signed(u.reg_read(UC_ARM_REG_R4)) not in range(13):self.failed_stop();return
  if a==0x47a404 and not r0:self.failed_stop();return
  # Guard source raw slot read/store at the corresponding actual backing.
  if a in [0x47aa98,0x47a428,0x47a738,0x47a8f0,0x47a964]:
   if a==0x47aa98:base=u.reg_read(UC_ARM_REG_R7);i=u.reg_read(UC_ARM_REG_R6)
   elif a==0x47a428:base=r2;i=r3
   elif a==0x47a738:base=r2;i=u.reg_read(UC_ARM_REG_R6)
   else:base=u.reg_read(UC_ARM_REG_R5);i=u.reg_read(UC_ARM_REG_R4)
   idx=self.array_id(base)-1
   if not base or idx not in range(3) or i>=self.capacity[idx]:self.failed_stop();return
  if a==0x47a430 and not r2:self.failed_stop();return
  if a==0x47a744 and not r0:self.failed_stop();return
 def call(self,mode):
  u=self.u;u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_R0,LIST)
  if mode==2:u.reg_write(UC_ARM_REG_R1,self.definition);u.reg_write(UC_ARM_REG_R2,self.row[2]&0xffffffff)
  elif mode==3:u.reg_write(UC_ARM_REG_R0,self.definition)
  elif mode==4:u.reg_write(UC_ARM_REG_R1,self.row[5]&0xffffffff)
  elif mode==5:u.reg_write(UC_ARM_REG_R1,STREAM)
  elif mode==8:u.reg_write(UC_ARM_REG_R1,self.row[7]&0xffffffff);u.reg_write(UC_ARM_REG_R2,self.row[6]&0xffffffff);u.reg_write(UC_ARM_REG_R3,STREAM)
  u.emu_start(FUNCTIONS[mode][1],STOP+4,count=10000)
  assert self.failed or u.reg_read(UC_ARM_REG_PC)==STOP
  if mode==3 and not self.failed:self.returned=self.object_id(u.reg_read(UC_ARM_REG_R0))
 def execute(self,row):
  self.row=row;self.events=[];self.cursor=self.attempts=self.created=self.returned=0;self.failed=self.mutated=False;self.capacity=[row[12],row[12],0];self.retired=[0,0,0];u=self.u;u.mem_write(ARRAYS,bytes(0x300));u.mem_write(OBJECTS,bytes(0x2000));self.adjustment=(row[6]//2) if row[0]==8 else 0
  for i in range(15):
   q=OBJECTS+i*0x100;t=TABLES+i*0x100;put(u,q,t);put(u,q+4,-17);put(u,q+0xc,0);put(u,q+0x10,0x50000000+i);put(u,q+0x18,0x60000000+i);put(u,q+0x1c,0);put(u,t+4,STOP+0x104)
   if self.adjustment:put(u,q+self.adjustment,t)
   for offset in [0x28,0x30]:put(u,t+offset,STOP+0x108)
  for i in range(row[12]):put(u,ARRAYS+i*4,0 if row[11]&(1<<i) else OBJECTS+i*0x100);put(u,ARRAYS+0x100+i*4,OBJECTS+(row[12]-1-i)*0x100)
  for r in self.definitions:
   base=DEFS+r['index']*0x200
   for i,d in enumerate(r['lists'][1]):put(u,base+i*0x2c+4,d['common'][0])
   put(u,base+0x100+4,r['accept']['common'][0]);put(u,base+0x12c+4,r['end']['common'][0])
  self.definition=DEFS+row[1]*0x200+(0x100 if row[4]==1 else 0x12c if row[4]==2 else row[3]*0x2c)
  if row[17]!=-0x80000000:put(u,self.definition+4,row[17])
  put(u,LIST,row[13]);put(u,LIST+4,ARRAYS if row[14] else 0);put(u,LIST+8,DEFS+row[1]*0x200+(0x100 if row[4]==1 else 0x12c if row[4]==2 else 0) if row[15] else 0)
  for mode in ([0,2,4,5,6] if row[0]==9 else [row[0]]):
   self.call(mode)
   if self.failed:break
  out=[int(self.failed),self.cursor,get(u,LIST),self.array_id(get(u,LIST+4)),get(u,LIST+8),self.returned,self.created]
  for a in range(3):out += [self.capacity[a],self.retired[a],*[self.object_id(get(u,ARRAYS+a*0x100+i*4)) for i in range(self.capacity[a])]]
  for i in range(15):q=OBJECTS+i*0x100;out += [get(u,q+4),get(u,q+0xc),get(u,q+0x10),get(u,q+0x18),get(u,q+0x1c)]
  out += [len(self.events),*sum(self.events,[])];return word(len(out))+pack(out)
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--library',type=Path);p.add_argument('--write-pins',action='store_true');a=p.parse_args()
 if a.write_pins:data,names,_=image(a.original);PIN.parent.mkdir(parents=True,exist_ok=True);PIN.write_text(json.dumps(evidence(data,names),indent=2)+'\n');return
 a.output.mkdir(parents=True,exist_ok=True);oracle=Original(a.original,a.cache);rows=cases();expected=[oracle.execute(row) for row in rows];inp=a.output/'fixtures.bin';out=a.output/'native.bin';exe=a.output/'quest_objective_list_v1_host.exe';inp.write_bytes(word(len(rows))+b''.join(pack(row) for row in rows))
 sources=['quest_objective_list_v1.cpp','quest_table_bindings_v1.cpp'];cmd=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror',str(MODULE/'tests/quest_objective_list_v1_host.cpp')]
 if a.library:cmd.append(str(a.library))
 else:
  decoder=a.output/'quests.obj';subprocess.run([str(Path(a.compiler).with_name('gcc.exe')),'-std=c11','-Wall','-Wextra','-Werror','-c',str(ROOT/'port/quest-data/quests.c'),'-o',str(decoder)],check=True);cmd += [str(MODULE/s) for s in sources]+[str(decoder)]
 subprocess.run(cmd+['-o',str(exe)],check=True);env=os.environ.copy();dlls=[]
 if a.library:dlls=sorted(a.library.parent.parent.rglob('*.dll'));env['PATH']=os.pathsep.join([*(str(x.parent) for x in dlls),env['PATH']])
 run=subprocess.run([str(exe),str(inp),str(out),str(a.cache.resolve())],env=env,capture_output=True,text=True);assert run.returncode==0,run.stderr;native=out.read_bytes();cursor=0
 for i,want in enumerate(expected):
  n=4+struct.unpack_from('<I',native,cursor)[0]*4;actual=native[cursor:cursor+n]
  if actual!=want:raise AssertionError(dict(case=i,row=rows[i],native=list(struct.unpack('<'+'I'*(len(actual)//4),actual)),original=list(struct.unpack('<'+'I'*(len(want)//4),want))))
  cursor+=n
 assert cursor==len(native)
 assert {a:len(v) for a,v in oracle.counts.items()}=={a:(10 if a==0x47a3e8 else n//4) for _,a,n in FUNCTIONS}
 report=dict(validation='PASS',cases=len(rows),original_reached_words=len(oracle.words),function_reached_words={hex(k):len(v) for k,v in oracle.counts.items()},native_policy_checks=int(run.stdout.strip()),implementation='selected library' if a.library else 'direct translation units',native_wired=False,android_compilation=False,live_gameplay=False,scope=__doc__,source_sha256={x:hashlib.sha256((MODULE/x).read_bytes()).hexdigest() for x in [*sources,'quest_objective_list_v1.hpp','quest_table_bindings_v1.hpp','tests/quest_objective_list_v1_host.cpp','tests/run_quest_objective_list_v1.py','reference/quest-objective-list-v1/original-functions.json']},selected_library=str(a.library) if a.library else None)
 if a.library:
  build=next(parent for parent in a.library.parents if (parent/'compile_commands.json').is_file());commands=json.loads((build/'compile_commands.json').read_text());paths={(MODULE/x).resolve() for x in sources}|{(ROOT/'port/quest-data/quests.c').resolve()};report['selected_commands']=[c for c in commands if Path(c['file']).resolve() in paths];assert len(report['selected_commands'])==len(paths);report['binary_sha256']={x.name:hashlib.sha256(x.read_bytes()).hexdigest() for x in [exe,*dlls]}
 report['source_sha256'].update({x:hashlib.sha256((ROOT/x).read_bytes()).hexdigest() for x in ['port/quest-data/quests.c','port/quest-data/quests.h']})
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_sha256','selected_commands','binary_sha256']}))
if __name__=='__main__':main()
