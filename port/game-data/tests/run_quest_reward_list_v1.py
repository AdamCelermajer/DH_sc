"""Whole RewardList lifecycle/assignment callers versus pinned original ARM.

One native List owns its count/current array/text/PyData. Actual immutable
QuestTable definitions feed native type reads; factories/deleting bodies and
platform allocations are explicit fixtures. Original string small-allocation,
empty-assign and deallocation bodies execute. Native string layout is semantic,
and dead string bytes are excluded. Compile/Give/gameplay effects remain unbound.
"""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put,signed,ELF_SHA
PIN=MODULE/'reference/quest-reward-list-v1/original-functions.json'
Q=0x10000100;STUB=0x10002000;OLD=0x10003000;NEW=0x10004000;OTHER=0x10004500;TEXT=0x10018000;STOP=0x30000000
STARTS=[0x482b0c,0x483950,0x4828f0,0x482ba8,0x482b4c,0x482c24]
RANGES=[(a,n) for a,n in zip(STARTS,[64,180,48,124,64,124])]+[(0x31167c,108),(0x3109e0,192),(0x3139ac,52)]
FACTORIES=[0x482ef0,0x482e94,0x482e34,0x482dd8,0x482d80]
def cases():
 rows=[]
 def add(mode=0,count=3,length=-1,character=123,mutation=0,text=20):rows.append([mode,count,length,character,mutation,0,0,text])
 for mode in [0,4]:
  for count in [-1,0,3]:
   for length in [-1,0,3]:
    for text in [0,5,20,150]:add(mode,count,length,text=text)
 for count in [-2,-1,0,1,3,6,12]:
  for length in [-1,0,3]:
   for mutation in [0,1,2,3,4,8]:add(1,count,length,mutation=mutation)
 for count in [-2,-1,0,1,3,6]:
  for length in [0,6]:
   if count>length:continue
   for owner in [0,123,0xffffffff]:add(2,count,length,owner)
 for mode in [3,5]:
  for count,length in [(-1,-1),(0,-1),(0,0),(0,3),(1,3),(3,3),(3,6),(6,6)]:
   for mutation in [0,5,6,7]:
    for text in [0,5,20,150]:add(mode,count,length,mutation=mutation,text=text)
 return rows
def execute(data,row,words):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
 for base,size in [(0x10000000,0x30000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
 arrays={};objects={};trace=[];status=0;factories=0;text_alive=True
 def object(identity):
  if identity not in objects:
   objects[identity]=0;put(u,identity,0x1001a000);put(u,identity+4,-1);put(u,identity+0xc,STUB+50*16);put(u,identity+0x10,0xcccccccc)
  return identity
 def array(identity,length):
  arrays[identity]=length
  for i in range(length):put(u,identity+i*4,object(identity+0x4000+i*0x100))
 if row[2]>=0:array(OLD,row[2])
 array(OTHER,12)
 put(u,Q,row[1]);put(u,Q+4,OLD if row[2]>=0 else 0);put(u,Q+0x20,STUB+5*16)
 # Source embedded string has inline capacity16 or a real heap buffer.
 start=Q+8 if row[7]<16 else TEXT
 if start==TEXT:put(u,Q+8,TEXT+row[7]+1)
 u.mem_write(start,b'x'*row[7]+b'\0');put(u,Q+0x18,start+row[7]);put(u,Q+0x1c,start)
 put(u,0x1001a004,STOP+0x100)
 for i in range(64):put(u,STUB+i*16+4,i%5)
 def ret(value=0):u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def event(op,a=0,b=0,c=0):
  nonlocal status
  trace.append([op,a,b,c])
  if row[5]>0 and len(trace)==row[5]:status=3;u.emu_stop();return False
  return True
 def hook(machine,address,size,context):
  nonlocal factories,text_alive
  if address==STOP:machine.emu_stop();return
  a,b=[machine.reg_read(reg) for reg in [UC_ARM_REG_R0,UC_ARM_REG_R1]]
  if address==0x31167c:
   if not event(1,16):return
  elif address==0x3109e0:
   if not event(2):return
   if row[4]==8:put(u,Q,2)
  elif address==0x3139ac:
   if not event(12):return
   text_alive=False
  elif address==0x31056c:
   if not event(3,a,b,NEW):return
   arrays[NEW]=a//4;machine.mem_write(NEW,bytes(a))
   if row[4]==1:put(u,Q,0)
   if row[4]==2:put(u,Q,1)
   ret(NEW);return
  elif address in FACTORIES:
   kind=FACTORIES.index(address);identity=0x10010000+factories*0x100
   if not event(5,kind,identity):return
   object(identity);objects[identity]=1;factories+=1
   if row[4]==3:put(u,Q+4,OTHER)
   if row[4]==4:put(u,Q,1)
   ret(identity);return
  elif address==STOP+0x100:
   if not event(10,a):return
   objects[a]=2
   if row[4]==5:put(u,Q,1)
   if row[4]==6:put(u,Q+4,OTHER)
   ret();return
  elif address in [0x310440,0x708f00]:
   if a==TEXT:ret();return
   assert address==0x310440
   if not event(11,a):return
   if row[4]==7:put(u,Q+4,OTHER)
   ret();return
  assert any(start<=address<start+length for start,length in RANGES),f'unscoped reward instruction {address:#x}'
  words.add(address)
 u.hook_add(UC_HOOK_CODE,hook)
 u.reg_write(UC_ARM_REG_R0,Q);u.reg_write(UC_ARM_REG_R1,STUB if row[0]==1 else row[3]&0xffffffff);u.reg_write(UC_ARM_REG_R2,row[1]&0xffffffff)
 u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP);u.emu_start(STARTS[row[0]],STOP+4,count=100000)
 length=get(u,Q+0x18)-get(u,Q+0x1c) if text_alive else 0
 return dict(status=status,fields=[signed(get(u,Q)),get(u,Q+4),get(u,Q+0x20),length,1 if text_alive else 0],arrays=[[id,[get(u,id+i*4) for i in range(length)]] for id,length in sorted(arrays.items())],objects=[[id,signed(get(u,id+4)),get(u,id+0xc),get(u,id+0x10),phase] for id,phase in sorted(objects.items())],trace=trace)
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--library',type=Path);a=p.parse_args()
 data,names,_=image(a.original);pins=json.loads(PIN.read_text());assert pins['original_sha256']==ELF_SHA
 for pin in pins['functions']+pins['declared_callee_dependencies']:
  address=int(pin['elf_address'],0);assert(names[pin['original_symbol']]['st_value'],names[pin['original_symbol']]['st_size'])==(address,pin['size'])
  assert hashlib.sha256(data[address:address+pin['size']]).hexdigest()==pin['sha256']
 words=set();rows=cases();base=[execute(data,row,words) for row in rows]
 for row,result in zip(rows.copy(),base):
  for ordinal in range(1,len(result['trace'])+1):
   for throwing in [0,1]:failure=row.copy();failure[5:7]=[ordinal,throwing];rows.append(failure)
 a.output.mkdir(parents=True,exist_ok=True);inputs=a.output/'inputs.txt';inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n')
 source=MODULE/'quest_reward_list_v1.cpp';test=MODULE/'tests/quest_reward_list_v1_host.cpp';exe=a.output/'host.exe'
 def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
 evidence={source,source.with_suffix('.hpp'),test,Path(__file__).resolve(),PIN}
 def snapshot(paths):return {path.relative_to(ROOT).as_posix():sha(path) for path in sorted(paths)}
 selected=[];selected_commands='';dsos=[];binary_before={};paths=evidence
 env=os.environ.copy();env['PATH']=str(Path(a.compiler).parent)+os.pathsep+env.get('PATH','')
 if a.library:
  sys.path.insert(0,str(ROOT/'port/level-world/tests'))
  from run_player_skill_cleanup_session_v1_host import selected_entries,actual_dependencies
  library=a.library.resolve();command_file=next(path for path in library.parents if(path/'compile_commands.json').is_file())/'compile_commands.json';selected_build=command_file.parent
  ninja=shutil.which('ninja');assert ninja
  result=subprocess.run([ninja,'-C',str(selected_build),'-t','commands','dh2_game_data'],capture_output=True,text=True,env=env);assert result.returncode==0,result.stderr
  selected_commands=result.stdout;selected=selected_entries(selected_build,selected_commands)
  assert sum(Path(entry['file']).resolve()==source.resolve() for entry in selected)==1
  paths=actual_dependencies(selected_build,ninja,selected)|evidence
  dsos=sorted(library.parent.parent.rglob('*.dll'));assert sum(path.name=='libdh2_game_data.dll' for path in dsos)==1
  binary_before={str(path):sha(path) for path in [library,*dsos]}
  env['PATH']=os.pathsep.join([*(str(path.parent) for path in dsos),env['PATH']])
 before=snapshot(paths)
 command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2',str(test)]
 if a.library:command.append(str(a.library.resolve()))
 else:
  cc=str(Path(a.compiler).with_name('gcc.exe'));obj=a.output/'quests.obj';c=subprocess.run([cc,'-std=c11','-Wall','-Wextra','-Werror','-O1','-c',str(ROOT/'port/quest-data/quests.c'),'-o',str(obj)],capture_output=True,text=True);assert c.returncode==0,c.stderr
  command += [str(source),str(MODULE/'quest_table_bindings_v1.cpp'),str(obj)]
 command+=['-o',str(exe)];built=subprocess.run(command,capture_output=True,text=True);assert built.returncode==0,built.stderr
 run=subprocess.run([str(exe),str(inputs)],capture_output=True,text=True,env=env);assert run.returncode==0,run.stderr
 native=json.loads(run.stdout);expected=[execute(data,row,words) for row in rows];assert len(native['results'])==len(rows)
 mismatches=[dict(index=i,input=row,expected=e,actual=n) for i,(row,e,n) in enumerate(zip(rows,expected,native['results'])) if e!=n]
 (a.output/'mismatches.json').write_text(json.dumps(mismatches,indent=2)+'\n');assert not mismatches,f'{len(mismatches)} mismatches at {a.output}/mismatches.json'
 after=snapshot(paths);assert before==after,'Selected source changed during comparison'
 report=dict(validation='PASS',cases=len(rows),valid_whole_callers=len(base),error_exception_prefixes=len(rows)-len(base),original_words=len(words),native_guard_checks=native['native_guard_checks'],mismatches=0,implementation='selected library' if a.library else 'direct translation units',scope=__doc__,source_sha256=before,source_before_after_equal=True)
 if a.library:
  result=subprocess.run([ninja,'-C',str(selected_build),'-t','commands','dh2_game_data'],capture_output=True,text=True,env=env);assert result.returncode==0 and result.stdout==selected_commands
  assert actual_dependencies(selected_build,ninja,selected)|evidence==paths
  assert {str(path):sha(path) for path in [library,*dsos]}==binary_before,'Frozen production DLL changed during comparison'
  imports={}
  for path in [exe,*dsos]:
   result=subprocess.run([str(Path(a.compiler).with_name('objdump.exe')),'-p',str(path)],capture_output=True,text=True,env=env);assert result.returncode==0,result.stderr
   imports[str(path)]=re.findall(r'DLL Name: (\S+)',result.stdout)
  assert 'libdh2_game_data.dll' in imports[str(exe)]
  report.update(selected_commands=selected_commands,selected_command_entry=next(entry for entry in selected if Path(entry['file']).resolve()==source.resolve()),source_tu_count=1,commands_before_after_equal=True,binary_before_after_equal=True,binary_sha256={**binary_before,str(exe):sha(exe)},binary_imports=imports)
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({key:report[key] for key in ['validation','cases','valid_whole_callers','error_exception_prefixes','original_words','native_guard_checks','mismatches','implementation','source_before_after_equal']}))
if __name__=='__main__':main()
