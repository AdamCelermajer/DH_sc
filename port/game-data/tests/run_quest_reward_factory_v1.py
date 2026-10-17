"""Whole Reward factories/base constructors/deleting leaves versus original ARM.

Source five factory callers and all constructor/destructor instructions execute;
allocation/free and the fresh shared constant lookup are declared providers.
Native actual64-row composition reuses sole QuestTable definitions and constants
decoder, creates222 source-selected Gold/XP objects, but does not execute Give.
"""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put,signed,ELF_SHA
PIN=MODULE/'reference/quest-reward-factory-v1/original-functions.json'
Q=0x10001000;STOP=0x30000000
FACTORIES=[0x482ef0,0x482e94,0x482e34,0x482dd8,0x482d80]
C=[0x482ca0,0x482d10]
D1=[0x482ad8,0x482aa4,0x482a70,0x482a3c,0x482a08,0x4828ec]
D0=[0x4837d0,0x4838c0,0x483884,0x483848,0x48380c,0x482b8c]
VT=[0,0x969ae8,0x969ab0,0x969a90,0x969a70,0x969a50,0x969a30]
def cases():
 rows=[]
 for mode in range(5):
  for kind in range(5 if mode==0 else 6):
   for value in [-2147483648,-1,0,5,2147483647]:
    for mutation in [0,1]:rows.append([mode,kind,value,mutation,0,0])
 return rows
def execute(data,row,words,ranges):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
 for base,size in [(0x10000000,0x10000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
 u.mem_write(Q,bytes([0xcc])*28)
 if row[0]>=3:put(u,Q,VT[1 if row[1]==5 else row[1]+2])
 trace=[];status=0;allocated=False;retired=False
 def ret(value=0):u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def event(op,a=0,b=0,c=0):
  nonlocal status
  trace.append([op,a,b,c])
  if row[4]>0 and len(trace)==row[4]:status=3;u.emu_stop();return False
  return True
 def hook(machine,address,size,context):
  nonlocal allocated,retired
  if address==STOP:machine.emu_stop();return
  a,b,c=[machine.reg_read(reg) for reg in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2]]
  if address==0x310570:
   if not event(1,a,b,Q):return
   allocated=True;ret(Q);return
  if address==0x4c4bdc:
   def string(p):return bytes(machine.mem_read(p,64)).split(b'\0',1)[0].decode()
   assert (string(b),string(c))==('v2QuestRewardType','Invalid')
   if row[3]==1:
    put(u,Q+4,-7);machine.mem_write(Q+8,b'\x07');put(u,Q+0x10,77);put(u,Q+0xc,55);put(u,Q+0x14,44)
   if not event(2):return
   ret(row[2]);return
  if address==0x310440:
   if row[3]==1:put(u,Q+0x10,77)
   if not event(3,a):return
   retired=True;ret();return
  assert any(start<=address<start+length for start,length in ranges),f'unscoped reward factory instruction {address:#x}'
  words.add(address)
 u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_ARM_REG_R0,Q);u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP)
 start=FACTORIES[row[1]] if row[0]==0 else C[row[0]-1] if row[0]<3 else D1[row[1]] if row[0]==3 else D0[row[1]]
 u.emu_start(start,STOP+4,count=100000)
 vt=get(u,Q);dispatch=VT.index(vt) if vt in VT else 0xffffffff
 fields=[dispatch,signed(get(u,Q+4)),u.mem_read(Q+8,1)[0],get(u,Q+0xc),get(u,Q+0x10),get(u,Q+0x14),get(u,Q+0x18),*u.mem_read(Q+9,3)]
 return dict(status=status,fields=fields,allocated=int(allocated),retired=int(retired),trace=trace)
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--library',type=Path);p.add_argument('--world-library',type=Path);a=p.parse_args()
 data,names,_=image(a.original);pins=json.loads(PIN.read_text());assert pins['original_sha256']==ELF_SHA;ranges=[]
 for pin in pins['functions']+[pins['constant_dependency']]:
  address=int(pin['elf_address'],0);assert(names[pin['original_symbol']]['st_value'],names[pin['original_symbol']]['st_size'])==(address,pin['size']);assert hashlib.sha256(data[address:address+pin['size']]).hexdigest()==pin['sha256']
  if pin!=pins['constant_dependency']:ranges.append((address,pin['size']))
 words=set();rows=cases();base=[execute(data,row,words,ranges) for row in rows]
 for row,result in zip(rows.copy(),base):
  for ordinal in range(1,len(result['trace'])+1):
   for throwing in [0,1]:failure=row.copy();failure[4:]=[ordinal,throwing];rows.append(failure)
 a.output.mkdir(parents=True,exist_ok=True);inputs=a.output/'inputs.txt';inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n')
 source=MODULE/'quest_reward_factory_v1.cpp';test=MODULE/'tests/quest_reward_factory_v1_host.cpp';exe=a.output/'host.exe'
 def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
 evidence={source,source.with_suffix('.hpp'),MODULE/'quest_reward_list_v1.hpp',test,Path(__file__).resolve(),PIN}
 def snapshot(paths):return {path.relative_to(ROOT).as_posix():sha(path) for path in sorted(paths)}
 selected=[];selected_commands='';dsos=[];binary_before={};paths=evidence
 env=os.environ.copy();env['PATH']=str(Path(a.compiler).parent)+os.pathsep+env.get('PATH','')
 if a.library:
  assert a.world_library
  sys.path.insert(0,str(ROOT/'port/level-world/tests'))
  from run_player_skill_cleanup_session_v1_host import selected_entries,actual_dependencies
  library=a.library.resolve();command_file=next(path for path in library.parents if(path/'compile_commands.json').is_file())/'compile_commands.json';selected_build=command_file.parent
  ninja=shutil.which('ninja');assert ninja
  result=subprocess.run([ninja,'-C',str(selected_build),'-t','commands','dh2_game_data','dh2_level_world'],capture_output=True,text=True,env=env);assert result.returncode==0,result.stderr
  selected_commands=result.stdout;selected=selected_entries(selected_build,selected_commands)
  assert sum(Path(entry['file']).resolve()==source.resolve() for entry in selected)==1
  assert sum(Path(entry['file']).name=='constants.c' for entry in selected)==1
  paths=actual_dependencies(selected_build,ninja,selected)|evidence
  dsos=sorted(library.parent.parent.rglob('*.dll'));assert sum(path.name=='libdh2_game_data.dll' for path in dsos)==1
  binary_before={str(path):sha(path) for path in [library,a.world_library.resolve(),*dsos]}
  env['PATH']=os.pathsep.join([*(str(path.parent) for path in dsos),env['PATH']])
 before=snapshot(paths);command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2',str(test)]
 if a.library:command+=[str(a.library.resolve()),str(a.world_library.resolve())]
 else:
  for name in ['port/quest-data/quests.c','port/pydata-constants/constants.c']:
   obj=a.output/(Path(name).stem+'.obj');built=subprocess.run([str(Path(a.compiler).with_name('gcc.exe')),'-std=c11','-Wall','-Wextra','-Werror','-O1','-c',str(ROOT/name),'-o',str(obj)],capture_output=True,text=True);assert built.returncode==0,built.stderr;command.append(str(obj))
  command += [str(source),str(MODULE/'quest_table_bindings_v1.cpp')]
 command+=['-o',str(exe)];built=subprocess.run(command,capture_output=True,text=True);assert built.returncode==0,built.stderr
 run=subprocess.run([str(exe),str(inputs),str(a.cache.resolve())],capture_output=True,text=True,env=env);assert run.returncode==0,run.stderr
 native=json.loads(run.stdout);expected=[execute(data,row,words,ranges) for row in rows];assert len(native['results'])==len(rows)
 mismatches=[dict(index=i,input=row,expected=e,actual=n) for i,(row,e,n) in enumerate(zip(rows,expected,native['results'])) if e!=n];(a.output/'mismatches.json').write_text(json.dumps(mismatches,indent=2)+'\n');assert not mismatches,f'{len(mismatches)} mismatches at {a.output}/mismatches.json'
 after=snapshot(paths);assert before==after,'Selected source changed during comparison'
 report=dict(validation='PASS',cases=len(rows),valid_whole_callers=len(base),error_exception_prefixes=len(rows)-len(base),original_words=len(words),native_guard_checks=native['native_guard_checks'],native_checks=native['native_checks'],actual_reward_types=native['actual_rewards'],mismatches=0,implementation='selected libraries' if a.library else 'direct translation units',scope=__doc__,source_sha256=before,source_before_after_equal=True,cache_sha256={name:sha(a.cache/name) for name in ['v2quests_pyarray.bin','v2quests_pyarraynames.bin','v2quests_pycst.bin']})
 if a.library:
  result=subprocess.run([ninja,'-C',str(selected_build),'-t','commands','dh2_game_data','dh2_level_world'],capture_output=True,text=True,env=env);assert result.returncode==0 and result.stdout==selected_commands;assert actual_dependencies(selected_build,ninja,selected)|evidence==paths;assert {str(path):sha(path) for path in [library,a.world_library.resolve(),*dsos]}==binary_before,'Frozen production DLL changed during comparison'
  imports={}
  for path in [exe,*dsos]:
   result=subprocess.run([str(Path(a.compiler).with_name('objdump.exe')),'-p',str(path)],capture_output=True,text=True,env=env);assert result.returncode==0,result.stderr;imports[str(path)]=re.findall(r'DLL Name: (\S+)',result.stdout)
  assert 'libdh2_game_data.dll' in imports[str(exe)] and 'libdh2_level_world.dll' in imports[str(exe)]
  report.update(selected_commands=selected_commands,selected_command_entry=next(entry for entry in selected if Path(entry['file']).resolve()==source.resolve()),source_tu_count=1,commands_before_after_equal=True,binary_before_after_equal=True,binary_sha256={**binary_before,str(exe):sha(exe)},binary_imports=imports)
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({key:report[key] for key in ['validation','cases','valid_whole_callers','error_exception_prefixes','original_words','native_guard_checks','actual_reward_types','mismatches','implementation','source_before_after_equal']}))
if __name__=='__main__':main()
