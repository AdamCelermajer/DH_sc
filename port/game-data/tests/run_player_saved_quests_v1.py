"""Whole QEST, LoadQuests, UnpackQuests and UnpackQuest versus original ARM.

The composed gate also executes the complete Quest::_loadQuestData112B and
IsVolatileState40B on the same factory Record and stream. Signed/unsigned stream
primitives, action virtual+28, ObjectiveList payload and assertion logging are
explicit observing providers. They do not validate the still missing native
child/VM implementation. Direct writes, read-failure prefixes, actual streamed
indices, live vector lookup, count capture, canonical acts and tell64/seek64
truncation are compared. Invalid vector reads and hard null assertion stores
stop at their original fault boundaries; native bounds/reentry/alias policies
are separate. No Android compilation, native wiring or live gameplay is inferred.
"""
from __future__ import annotations
import argparse,hashlib,itertools,json,os,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put,signed,ELF_SHA
PIN=MODULE/'reference/player-saved-quests-v1/original-functions.json'
SAVE=0x10001000;LOGS=[SAVE+0xb8,SAVE+0x118];STREAM=0x10005000;VECTORS=0x10007000;QUESTS=0x10008000;ACTIONS=0x1000b000;STOP=0x30000000
FUNCTIONS=[('_ZN14PlayerSavegame12__LoadQuestsEP11IStreamBasePv',0x469478,80),('_ZN13QuestSavegame10LoadQuestsEP11IStreamBase',0x46c550,52),('_ZN13QuestSavegame12UnpackQuestsEiP11IStreamBaseb',0x46c48c,196),('_ZN13QuestSavegame11UnpackQuestEiiP11IStreamBaseb',0x46c344,328),('_ZN5Quest14_loadQuestDataEP11IStreamBaseb',0x47f78c,112),('_ZNK5Quest15IsVolatileStateEi',0x47f70c,40)]
def word(v):return struct.pack('<I',v&0xffffffff)
def pack(values):return b''.join(word(x) for x in values)
def evidence(data,names):
 pins=[]
 for symbol,a,n in FUNCTIONS:
  assert (names[symbol]['st_value'],names[symbol]['st_size'])==(a,n)
  pins.append(dict(original_symbol=symbol,elf_address=hex(a),size=n,sha256=hashlib.sha256(data[a:a+n]).hexdigest()))
 assert names['gAssertLevel']['st_value']==0x99f914
 spans=[dict(elf_address=hex(a),size=n,sha256=hashlib.sha256(data[a:a+n]).hexdigest()) for a,n in [(0x46be68,4),(0x8cdf24,10)]]
 assert data[0x46be68:0x46be6c]==bytes.fromhex('1eff2fe1') # actual no-op clone: bx lr
 return dict(original_sha256=ELF_SHA,functions=pins,assert_global='0x99f914',source_spans=spans,scope=__doc__,remaining_external_bodies={'unsigned_reader':'0x313b48','signed_reader':'0x38b758','quest_signed_reader':'0x459090','ObjectiveList_Load':'0x47a798','logger':'0x30e004','action_virtual_slot':'0x28','stream_tell_slot':'0x24','stream_absolute_seek64_slot':'0x20'},unreached_source_words={'unused_inline_string_heap_cleanup':['0x46c3d8','0x46c3dc','0x46c3e0','0x46c3e4','0x46c3e8','0x46c3ec','0x46c3f0','0x46c410','0x46c414'],'canary_failure':['0x46c46c']})
def cases():
 rows=[]
 def add(values,**kwargs):
  row=[0,0,0,77,0,0,1,0,0,1,0,0,0,0,0,-1,0,0,0]
  for key,value in kwargs.items():row[int(key[1:])]=value
  rows.append((row,pack(values)))
 def group(count,indices=None,base=100):
  if indices is None:indices=list(reversed(range(count)))
  return [count,*itertools.chain.from_iterable((i,base+n%12) for n,i in enumerate(indices)),base+31,base+32,base+33]
 for counts in itertools.product(range(3),repeat=3):
  values=sum((group(c,base=100+d*100) for d,c in enumerate(counts)),[])
  for mode in [0,1]:add(values,v0=mode,**{f'v{6+i}':c for i,c in enumerate(counts)},**{f'v{9+i}':c for i,c in enumerate(counts)})
 for d,which,flag,count in itertools.product(range(3),range(2),[0,1,255,0xffffffff],[0,1,2,3]):
  add(group(count,base=0x70000000+d),v0=2,v1=which,v2=d,v4=flag,**{f'v{6+which*3+d}':count})
 for d,which,index,ordinal,flag in itertools.product(range(3),range(2),[0,1,2],[-99,0,7],[0,255]):
  add([index,2+index],v0=3,v1=which,v2=d,v3=ordinal,v4=flag,**{f'v{6+which*3+d}':3})
 for mode in [0,1,2]:
  for count,declared in itertools.product(range(3),[0,1,2,0xffffffff]):
   if count==declared:continue
   add([declared,0xffffffff,0xffffffff],v0=mode,v6=count,v9=count)
 for assertion in [-2,0,1,2,3]:
  for d,which in itertools.product(range(3),range(2)):
   add([1,0,11,12,13],v0=2,v1=which,v2=d,v5=assertion,v12=1<<(which*9+d*3),**{f'v{6+which*3+d}':1})
 for tell_high in [0,1,0x7fffffff,0xffffffff]:add(group(1)+group(0)+group(0),v13=tell_high)
 for mutation in [1,2,3,4]:
  values=group(3,[0,0,0],base=2) if mutation in [1,2] else group(1,base=2)
  add(values,v0=2,v6=3 if mutation in [1,2] else 1,v14=mutation,v4=253)
 for invalid in [-1,3,0x7fffffff,0x80000000]:add([invalid,999],v0=3,v6=3)
 for state in [-0x80000000,-2,-1,*range(16),0x7fffffff]:add([0,state],v0=3,v6=1,v4=255)
 add([1,0,1,7,8,9,0,10,11,12,0,13,14,15],v0=0,v6=1,v9=2) # second log mismatch, then fresh count reads.
 # Inject failure at each reached actual provider boundary, both before writes
 # and after a two-byte signed/unsigned read; untouched current tails stay old.
 baseline=group(2,base=2)+group(1,base=7)+group(0,base=10)
 for index in range(1,63):
  for partial,throw in [(0,0),(1,0),(0,1)]:add(baseline,v6=2,v7=1,v9=2,v10=1,v15=index,v16=partial,v17=throw)
 return rows
class Original:
 def __init__(self,path):
  data,names,_=image(path);self.pins=evidence(data,names);assert self.pins==json.loads(PIN.read_text());self.words=set();self.counts={a:set() for _,a,_ in FUNCTIONS};self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);self.u.mem_map(0,len(data));self.u.mem_write(0,data)
  for base,size in [(0x10000000,0x20000),(0x20000000,0x10000),(STOP,0x1000)]:self.u.mem_map(base,size)
  self.u.hook_add(UC_HOOK_CODE,self.code)
 def ret(self,v=0):u=self.u;u.reg_write(UC_ARM_REG_R0,v&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def event(self,op,target=0,value=0,flag=0):
  self.events.append([op,self.log,self.d,self.index,target,value&0xffffffff,self.cursor,flag&0xffffffff]);self.attempts+=1
  return self.row[15]>0 and self.attempts==self.row[15]
 def read(self,dest,failed):
  n=2 if failed and self.row[16] else 0 if failed else 4
  if self.cursor+n>len(self.payload):self.failed=True;self.u.emu_stop();return False
  if n:self.u.mem_write(dest,self.payload[self.cursor:self.cursor+n]);self.cursor+=n
  if failed:self.failed=True;self.u.emu_stop();return False
  return True
 def code(self,u,a,size,context):
  if a==STOP:u.emu_stop();return
  r0,r1,r2,r3=[u.reg_read(r) for r in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]]
  if a==STOP+0x100:
   assert r0==STREAM;failed=self.event(1);u.reg_write(UC_ARM_REG_R1,self.row[13]&0xffffffff)
   if failed:self.failed=True;u.emu_stop();return
   self.ret(self.cursor);return
  if a==STOP+0x104:
   assert r0==STREAM;failed=self.event(2,0,r2,r3)
   if failed:self.failed=True;u.emu_stop();return
   assert r3==0 and r2<=len(self.payload);self.cursor=r2;self.ret();return
  if a==0x46be68:self.words.add(a);return # execute its pinned original bx lr
  if a in [0x313b48,0x38b758]:
   assert r0==STREAM;caller=u.reg_read(UC_ARM_REG_LR)-4;role={0x46c4bc:0,0x46c3a0:1,0x46c518:2,0x46c530:3,0x46c53c:4}[caller]
   if not self.read(r1,self.event(3 if a==0x313b48 else 4,role)):return
   if role==1:self.index=get(u,r1)
   if self.row[14]==1 and not self.mutated and role==0:put(u,LOGS[self.log]+self.d*12+8,VECTORS+(self.log*3+self.d)*16+4);self.mutated=True
   if self.row[14]==3 and role==4:put(u,r1,get(u,r1)^0x55)
   self.ret();return
  if a==0x459090:
   assert r0==STREAM and QUESTS<=r1<QUESTS+19*0x100;self.current_quest=(r1-QUESTS)//0x100
   if not self.read(r1,self.event(8,self.current_quest)):return
   self.ret();return
  if a==STOP+0x108:
   assert r1==STREAM;delta=r0-ACTIONS;target=(delta//0x100)*2+(delta%0x100)//0x40
   if self.event(9,target,0x28):self.failed=True;u.emu_stop();return
   self.ret();return
  if a==0x47a798:
   i=(r0-QUESTS-0x2c)//0x100;assert r0==QUESTS+i*0x100+0x2c and r1==STREAM
   if self.event(10,i):self.failed=True;u.emu_stop();return
   if self.row[14]==2 and not self.mutated:put(u,VECTORS+(self.log*3+self.d)*16,QUESTS+18*0x100);self.mutated=True
   if self.row[14]==4:put(u,QUESTS+i*0x100,3)
   self.ret();return
  if a==0x30e004:
   assert get(u,u.reg_read(UC_ARM_REG_SP))==0xc2
   if self.event(7,0,0xc2):self.failed=True;u.emu_stop();return
   self.ret();return
  assert any(start<=a<start+n for _,start,n in FUNCTIONS),f'unscoped original {a:#x}'
  self.words.add(a)
  for _,start,n in FUNCTIONS:
   if start<=a<start+n:self.counts[start].add(a)
  if a==0x46c48c:self.log=LOGS.index(r0);self.d=r1
  if a==0x46c344:self.log=LOGS.index(r0);self.d=r2;self.index=r1
  if a==0x46c3b4:
   idx=signed(r2);length=(get(u,LOGS[self.log]+self.d*12+8)-get(u,LOGS[self.log]+self.d*12+4))//4
   if idx<0 or idx>=length:self.failed=True;u.emu_stop();return
  if a==0x47f78c:
   i=(r0-QUESTS)//0x100;assert r0==QUESTS+i*0x100
   if self.event(5,i,0,r2):self.failed=True;u.emu_stop();return
  if a==0x46c420:
   if self.event(6,0,self.row[5]):self.failed=True;u.emu_stop();return
  if a==0x46c428 and self.row[5]==2:self.failed=True;u.emu_stop();return
 def execute(self,row,payload):
  self.row=row;self.payload=payload;self.cursor=self.attempts=self.log=self.d=self.index=0;self.mutated=self.failed=False;self.events=[];u=self.u
  u.mem_write(SAVE,bytes(0x200));put(u,STREAM,STREAM+0x100);put(u,STREAM+0x100+0x24,STOP+0x100);put(u,STREAM+0x100+0x20,STOP+0x104);put(u,0x99f914,row[5])
  for l in range(2):
   for d in range(3):
    vector=VECTORS+(l*3+d)*16;count=row[6+l*3+d];put(u,LOGS[l]+d*12+4,vector);put(u,LOGS[l]+d*12+8,vector+count*4);put(u,LOGS[l]+d*12+12,vector+count*4)
    for j in range(count):i=l*9+d*3+j;put(u,vector+j*4,0 if row[12]&(1<<i) else QUESTS+i*0x100)
    for offset,base in [(0x2c,0x11000000),(0x38,0x22000000),(0x44,0x33000000),(0x50,0x44000000)]:put(u,LOGS[l]+offset+d*4,base+l*16+d)
  for i in range(19):
   q=QUESTS+i*0x100;u.mem_write(q,bytes(0x6c));put(u,q,0x60000000+i);put(u,q+0x10,(i%9)//3);u.mem_write(q+0x64,bytes([i%2]))
   for j in range(2):action=ACTIONS+i*0x100+j*0x40;put(u,q+0x18+j*4,action);put(u,action,action+0x80);put(u,action+0x80+0x28,STOP+0x108)
  mode=row[0];self.log=row[1];self.d=row[2] if mode>=2 else 0;self.index=(row[3]&0xffffffff) if mode==3 else 0
  u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP)
  if mode==0:u.reg_write(UC_ARM_REG_R0,STREAM);u.reg_write(UC_ARM_REG_R1,SAVE)
  elif mode==1:u.reg_write(UC_ARM_REG_R0,LOGS[row[1]]);u.reg_write(UC_ARM_REG_R1,STREAM)
  elif mode==2:u.reg_write(UC_ARM_REG_R0,LOGS[row[1]]);u.reg_write(UC_ARM_REG_R1,row[2]);u.reg_write(UC_ARM_REG_R2,STREAM);u.reg_write(UC_ARM_REG_R3,row[4]&0xffffffff)
  else:u.reg_write(UC_ARM_REG_R0,LOGS[row[1]]);u.reg_write(UC_ARM_REG_R1,row[3]&0xffffffff);u.reg_write(UC_ARM_REG_R2,row[2]);u.reg_write(UC_ARM_REG_R3,STREAM);put(u,0x20008000,row[4])
  u.emu_start(FUNCTIONS[mode][1],STOP+4,count=100000);assert self.failed or u.reg_read(UC_ARM_REG_PC)==STOP
  out=[int(self.failed),self.cursor]
  for l in range(2):
   for offset in [0x2c,0x38,0x44,0x50]:out += [get(u,LOGS[l]+offset+d*4) for d in range(3)]
   out += [(get(u,LOGS[l]+d*12+8)-get(u,LOGS[l]+d*12+4))//4 for d in range(3)]
  for i in range(19):out += [get(u,QUESTS+i*0x100),u.mem_read(QUESTS+i*0x100+0x64,1)[0]]
  out += [len(self.events)];out += sum(self.events,[]);return word(len(out))+pack(out)
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--library',type=Path);p.add_argument('--write-pins',action='store_true');a=p.parse_args()
 if a.write_pins:
  data,names,_=image(a.original);PIN.parent.mkdir(parents=True,exist_ok=True);PIN.write_text(json.dumps(evidence(data,names),indent=2)+'\n');return
 a.output.mkdir(parents=True,exist_ok=True);original=Original(a.original);rows=cases();expected=[original.execute(*row) for row in rows];inputs=a.output/'fixtures.bin';outputs=a.output/'native.bin';exe=a.output/'player_saved_quests_v1_host.exe';inputs.write_bytes(word(len(rows))+b''.join(pack(row)+word(len(payload))+payload for row,payload in rows))
 sources=['player_saved_quests_v1.cpp','quest_runtime_fields_v1.cpp'];command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror',str(MODULE/'tests/player_saved_quests_v1_host.cpp')];command += [str(a.library)] if a.library else [str(MODULE/x) for x in [*sources,'player_save_level_states_v1.cpp']];subprocess.run(command+['-o',str(exe)],check=True);env=os.environ.copy();dlls=[]
 if a.library:dlls=sorted(a.library.parent.parent.rglob('*.dll'));env['PATH']=os.pathsep.join([*(str(x.parent) for x in dlls),env['PATH']])
 run=subprocess.run([str(exe),str(inputs),str(outputs)],env=env,check=True,capture_output=True,text=True);native=outputs.read_bytes();cursor=0
 for i,want in enumerate(expected):
  n=4+struct.unpack_from('<I',native,cursor)[0]*4;actual=native[cursor:cursor+n]
  if actual!=want:raise AssertionError({'case':i,'row':rows[i][0],'payload':list(struct.unpack('<'+'I'*(len(rows[i][1])//4),rows[i][1])),'native':list(struct.unpack('<'+'I'*(len(actual)//4),actual)),'original':list(struct.unpack('<'+'I'*(len(want)//4),want))})
  cursor+=n
 assert cursor==len(native)
 report=dict(validation='PASS',cases=len(rows),original_words=len(original.words),function_executed_words={hex(k):len(v) for k,v in original.counts.items()},native_policy_checks=int(run.stdout.strip()),compared_projection_bytes=len(native),implementation='selected library' if a.library else 'direct translation units',native_wired=False,scope=__doc__,source_sha256={x:hashlib.sha256((MODULE/x).read_bytes()).hexdigest() for x in [*sources,'player_saved_quests_v1.hpp','player_savegame_v1.hpp','quest_runtime_fields_v1.hpp','tests/player_saved_quests_v1_host.cpp','tests/run_player_saved_quests_v1.py','reference/player-saved-quests-v1/original-functions.json']})
 if a.library:
  build=next(parent for parent in a.library.parents if (parent/'compile_commands.json').is_file());commands=json.loads((build/'compile_commands.json').read_text());paths={(MODULE/x).resolve() for x in sources};report['selected_commands']=[c for c in commands if Path(c['file']).resolve() in paths];assert len(report['selected_commands'])==len(sources);report['binary_sha256']={x.name:hashlib.sha256(x.read_bytes()).hexdigest() for x in [exe,*dlls]}
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_sha256','selected_commands','binary_sha256']}))
if __name__=='__main__':main()
