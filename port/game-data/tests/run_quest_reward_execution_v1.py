"""Complete five Reward Compile and Gold/XP Give callers versus original ARM.

Original Gold Give executes the genuine nested AddGold68B and SetGold nonnegative
storage prefix. SetGold notification suffix and Character::_GiveXP912B remain
explicit provider boundaries. Native gold uses the selected sole imported owner,
canonical property/loot tables and actual Character receipt words. No live game
or real XP delivery is inferred from these host comparisons.
"""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put,signed,ELF_SHA
PIN=MODULE/'reference/quest-reward-execution-v1/original-functions.json'
Q=0x10000100;STUB=0x10002000;CHARS=[0x10004000,0x10007000];STOP=0x30000000
COMPILE=[0x482880,0x482894,0x4828a8,0x4828bc,0x4828d0]
def cases():
 rows=[]
 for kind in range(5):
  for flag in [0,1,7,255]:
   for parameter in [-2147483648,-1,0,1,2147483647]:rows.append([0,kind,flag,5,55,parameter,0,1,0,0])
 for mode in [1,2]:
  for flag in [0,1,7,255]:
   for amount in ([-2000,-1,0,5,5000,2147483647] if mode==1 else [-2147483648,-1,0,5,8388607,2147483647]):
    for mutation in [0,1,2]:
     for grant in ([1] if mode==1 else [0,1]):rows.append([mode,mode-1,flag,amount,55,1,mutation,grant,0,0])
 return rows
def execute(data,row,words,ranges):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
 for base,size in [(0x10000000,0x10000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
 put(u,Q+0xc,STUB);put(u,Q+0x14,STUB+16 if row[0]==0 else STUB);put(u,Q+0x10,CHARS[0]);u.mem_write(Q+8,bytes([row[2]]))
 for i in range(2):
  put(u,STUB+i*16+4,row[1]);put(u,STUB+i*16+8,row[3+i]);put(u,STUB+i*16+12,row[5]);put(u,CHARS[i]+0x37c+0x20,1000+i*1000);put(u,CHARS[i]+0x37c+0x28,5000);put(u,CHARS[i]+0x1500,-50);put(u,CHARS[i]+0x1504,-70)
 trace=[];status=0
 def ret(value=0):u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def mutate():
  if row[6]==1:put(u,Q+0x14,STUB+16);put(u,Q+0x10,CHARS[1])
  if row[6]==2:u.mem_write(Q+8,b'\0')
 def event(op,a=0,b=0,c=0):
  nonlocal status
  trace.append([op,a,b,c]);mutate()
  if row[8]>0 and len(trace)==row[8]:status=3;u.emu_stop();return False
  return True
 def hook(machine,address,size,context):
  if address==STOP:machine.emu_stop();return
  a,b,c=[machine.reg_read(reg) for reg in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2]]
  if address==0x3fe010:
   inventory=machine.reg_read(UC_ARM_REG_R5);character=inventory-0x37c
   if not event(1,character,signed(get(u,inventory+0x20))):return
   # The required notification provider covers the unreconstructed suffix;
   # source epilogue still restores the actual nested SetGold frame.
   machine.reg_write(UC_ARM_REG_PC,0x3fe034);return
  if address==0x3bf498:
   if not event(2,a,signed(b),c):return
   ret(row[7]);return
  assert any(start<=address<start+length for start,length in ranges),f'unscoped reward execution instruction {address:#x}'
  words.add(address)
 u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_ARM_REG_R0,Q);u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP)
 start=COMPILE[row[1]] if row[0]==0 else 0x483480 if row[0]==1 else 0x483420;u.emu_start(start,STOP+4,count=100000)
 return dict(status=status,value=u.reg_read(UC_ARM_REG_R0) if row[0]!=0 and not status else 0,fields=[u.mem_read(Q+8,1)[0],get(u,Q+0x14),get(u,Q+0x10)],characters=[[signed(get(u,c+0x37c+0x20)),signed(get(u,c+0x1500)),signed(get(u,c+0x1504))] for c in CHARS],trace=trace)
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--selected',action='store_true');a=p.parse_args()
 data,names,_=image(a.original);pins=json.loads(PIN.read_text());assert pins['original_sha256']==ELF_SHA;ranges=[]
 for pin in pins['functions']:
  address=int(pin['elf_address'],0);assert(names[pin['original_symbol']]['st_value'],names[pin['original_symbol']]['st_size'])==(address,pin['size']);assert hashlib.sha256(data[address:address+pin['size']]).hexdigest()==pin['sha256'];ranges.append((address,pin['size']))
 words=set();rows=cases();base=[execute(data,row,words,ranges) for row in rows]
 for row,result in zip(rows.copy(),base):
  for ordinal in range(1,len(result['trace'])+1):
   for throwing in [0,1]:failure=row.copy();failure[8:]=[ordinal,throwing];rows.append(failure)
 a.output.mkdir(parents=True,exist_ok=True);inputs=a.output/'inputs.txt';inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n')
 source=MODULE/'quest_reward_execution_v1.cpp';test=MODULE/'tests/quest_reward_execution_v1_host.cpp';exe=a.output/'host.exe'
 def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
 evidence={source,source.with_suffix('.hpp'),MODULE/'quest_reward_factory_v1.hpp',MODULE/'quest_reward_list_v1.hpp',test,Path(__file__).resolve(),PIN}
 def snapshot(paths):return {path.relative_to(ROOT).as_posix():sha(path) for path in sorted(paths)}
 sys.path.insert(0,str(ROOT/'port/level-world/tests'))
 from run_player_skill_cleanup_session_v1_host import selected_entries,actual_dependencies
 library=a.library.resolve();command_file=next(path for path in library.parents if(path/'compile_commands.json').is_file())/'compile_commands.json';selected_build=command_file.parent;ninja=shutil.which('ninja');assert ninja
 env=os.environ.copy();env['PATH']=str(Path(a.compiler).parent)+os.pathsep+env.get('PATH','')
 result=subprocess.run([ninja,'-C',str(selected_build),'-t','commands','dh2_game_data'],capture_output=True,text=True,env=env);assert result.returncode==0,result.stderr;selected_commands=result.stdout;selected=selected_entries(selected_build,selected_commands)
 assert sum(Path(entry['file']).resolve()==source.resolve() for entry in selected)==int(a.selected)
 assert sum(Path(entry['file']).name=='fresh_inventory_owned_v4.cpp' for entry in selected)==1
 paths=actual_dependencies(selected_build,ninja,selected)|evidence;before=snapshot(paths)
 dsos=sorted(library.parent.parent.rglob('*.dll'));binary_before={str(path):sha(path) for path in [library,*dsos]};env['PATH']=os.pathsep.join([*(str(path.parent) for path in dsos),env['PATH']])
 command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2',str(test)]
 if not a.selected:command.append(str(source))
 command+=[str(library),'-o',str(exe)];built=subprocess.run(command,capture_output=True,text=True);assert built.returncode==0,built.stderr
 run=subprocess.run([str(exe),str(inputs),str(a.cache.resolve())],capture_output=True,text=True,env=env);assert run.returncode==0,run.stderr
 native=json.loads(run.stdout);expected=[execute(data,row,words,ranges) for row in rows];assert len(native['results'])==len(rows)
 mismatches=[dict(index=i,input=row,expected=e,actual=n) for i,(row,e,n) in enumerate(zip(rows,expected,native['results'])) if e!=n];(a.output/'mismatches.json').write_text(json.dumps(mismatches,indent=2)+'\n');assert not mismatches,f'{len(mismatches)} mismatches at {a.output}/mismatches.json'
 after=snapshot(paths);assert before==after,'Selected source changed during comparison'
 result=subprocess.run([ninja,'-C',str(selected_build),'-t','commands','dh2_game_data'],capture_output=True,text=True,env=env);assert result.returncode==0 and result.stdout==selected_commands;assert actual_dependencies(selected_build,ninja,selected)|evidence==paths;assert {str(path):sha(path) for path in [library,*dsos]}==binary_before,'Frozen production DLL changed during comparison'
 imports={}
 for path in [exe,*dsos]:
  result=subprocess.run([str(Path(a.compiler).with_name('objdump.exe')),'-p',str(path)],capture_output=True,text=True,env=env);assert result.returncode==0,result.stderr;imports[str(path)]=re.findall(r'DLL Name: (\S+)',result.stdout)
 assert 'libdh2_game_data.dll' in imports[str(exe)]
 report=dict(validation='PASS',cases=len(rows),valid_whole_callers=len(base),error_exception_prefixes=len(rows)-len(base),original_words=len(words),native_guard_checks=native['native_guard_checks'],native_checks=native['native_checks'],mismatches=0,implementation='selected library' if a.selected else 'direct execution TU +selected actual inventory/table library',scope=__doc__,source_sha256=before,source_before_after_equal=True,cache_sha256={name:sha(a.cache/name) for name in ['loot_table_pyarray.bin','loot_table_pyarraynames.bin','loot_table_pystructnames.bin']},selected_commands=selected_commands,commands_before_after_equal=True,binary_before_after_equal=True,binary_sha256={**binary_before,str(exe):sha(exe)},binary_imports=imports)
 if a.selected:report.update(selected_command_entry=next(entry for entry in selected if Path(entry['file']).resolve()==source.resolve()),source_tu_count=1)
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({key:report[key] for key in ['validation','cases','valid_whole_callers','error_exception_prefixes','original_words','native_guard_checks','mismatches','implementation','source_before_after_equal']}))
if __name__=='__main__':main()
