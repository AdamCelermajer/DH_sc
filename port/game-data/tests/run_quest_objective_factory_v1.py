"""Whole13 Objective factories, baseC1/C2 and base/derivedD1/D0 versus ARM.

All constructor/destructor instructions execute, including the original empty
baseD1/D2 bodies. Only allocation/free and fresh PyDataConstants.getConstant
are declared observing providers. Every owned source field and untouched
padding is compared, including failures/exceptions after provider prefixes.
Native actual64-row composition also calls the selected ObjectiveList with
the retained QuestTable View and typed shared constants view, creating194
actual source-selected objectives/actions and closing list/object lifetimes.
This does not execute Compile/events/success predicates or live gameplay.
"""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put,signed,ELF_SHA
PIN=MODULE/'reference/quest-objective-factory-v1/original-functions.json'
Q=0x10001000;STOP=0x30000000
FACTORIES=[0x47d02c,0x47cfd0,0x47cf78,0x47cf20,0x47bf20,0x47cec4,0x47bf70,0x47d194,0x47d13c,0x47d0e4,0x47d088,0x47bec4,0x47be74]
C=[0x47bd8c,0x47be00]
D1=[0x47d2e4,0x47d5b4,0x47d564,0x47d514,0x47a2e0,0x47d474,0x47a864,0x47d424,0x47d3d4,0x47d384,0x47d1f4,0x47d294,0x47a1f0,0x47a378]
D0=[0x47e83c,0x47d840,0x47dcb4,0x47d898,0x47cdc4,0x47cd14,0x47c6b4,0x47dc5c,0x47d790,0x47d738,0x47e894,0x47d7e8,0x47ce1c,0x47a8b0]
VT=[0,0x969948,0x9692f8,0x969860,0x969808,0x9697b0,0x969758,0x969700,0x969650,0x969610,0x9695b8,0x969560,0x969508,0x969458,0x969350,0x9692a0]
SVT=[0,0,0x95c330,*[v+0x44 for v in VT[3:]]]
def evidence(data,names):
 by={v['st_value']:(k,v['st_size']) for k,v in names.items()};pins=[]
 for a in dict.fromkeys([*FACTORIES,*C,*D1,*D0,0x47a1e4]):
  name,size=by[a];literal_words=5 if a in C else 2 if a in FACTORIES or a in [D1[6],D0[6]] else 3 if a in D1[:-1]+D0[:-1] else 0
  pins.append(dict(original_symbol=name,elf_address=hex(a),size=size,ordinary_words=size//4-literal_words,sha256=hashlib.sha256(data[a:a+size]).hexdigest()))
 vtables=[]
 for v in [*VT[1:],0x95c330]:
  name,size=next((name,s['st_size']) for name,s in names.items() if name.startswith('_ZTV') and s['st_value']==v-8)
  vtables.append(dict(original_symbol=name,elf_address=hex(v-8),size=size,sha256=hashlib.sha256(data[v-8:v-8+size]).hexdigest()))
 a=0x4c4bdc;name,size=by[a]
 return dict(original_sha256=ELF_SHA,functions=pins,constant_dependency=dict(original_symbol=name,elf_address=hex(a),size=size,sha256=hashlib.sha256(data[a:a+size]).hexdigest()),vtables=vtables,lookup_names=[dict(elf_address=hex(a),text=text,sha256=hashlib.sha256(data[a:a+len(text)+1]).hexdigest()) for a,text in [(0x8c2970,'v2QuestObjectiveType'),(0x8da6a8,'Invalid')]],factory_dispatch=dict(elf_address='0x969908',size=56,entries=list(struct.unpack_from('<14I',data,0x969908)),sha256=hashlib.sha256(data[0x969908:0x969940]).hexdigest()),remaining_external=dict(allocate='0x310570',free_heap='0x310440',get_constant='0x4c4bdc fresh GameScript+2c owner query'),scope=__doc__)
def cases():
 rows=[]
 for mode in range(7):
  for kind in range(13 if mode in [0,5,6] else 14 if mode in [3,4] else 1):
   for value in [-2147483648,-1,0,13,2147483647]:
    for mutation in [0,1]:rows.append([mode,kind,value,mutation,0,0])
 return rows
class Original:
 def __init__(self,data,pins):
  self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);self.u.mem_map(0,len(data));self.u.mem_write(0,data)
  for base,size in [(0x10000000,0x10000),(0x20000000,0x10000),(STOP,0x1000)]:self.u.mem_map(base,size)
  self.ranges=[(int(x['elf_address'],0),x['size']) for x in pins['functions']];self.words=set();self.reached={a:set() for a,n in self.ranges};self.u.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,value=0):self.u.reg_write(UC_ARM_REG_R0,value&0xffffffff);self.u.reg_write(UC_ARM_REG_PC,self.u.reg_read(UC_ARM_REG_LR))
 def event(self,op,a=0,b=0,c=0):
  self.trace.append([op,a,b,c])
  if self.row[4]>0 and len(self.trace)==self.row[4]:self.status=3;self.u.emu_stop();return False
  return True
 def hook(self,u,address,size,context):
  if address==STOP:u.emu_stop();return
  a,b,c=[u.reg_read(reg) for reg in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2]]
  if address==0x310570:
   if not self.event(1,a,b,Q):return
   self.allocated=True;self.ret(Q);return
  if address==0x4c4bdc:
   def string(p):return bytes(u.mem_read(p,64)).split(b'\0',1)[0].decode()
   assert (string(b),string(c))==('v2QuestObjectiveType','Invalid')
   if self.row[3]==1:put(u,Q+4,-7);u.mem_write(Q+8,b'\x07');u.mem_write(Q+0x14,b'\x09');put(u,Q+0x10,77);put(u,Q+0xc,55)
   if not self.event(2):return
   self.ret(self.row[2]);return
  if address==0x310440:
   assert a==Q
   if self.row[3]==1:put(u,Q+0x10,77)
   if not self.event(3,a):return
   self.retired=True;self.ret();return
  matches=[a for a,n in self.ranges if a<=address<a+n];assert matches,f'unscoped Objective constructor/destructor {address:#x}'
  self.words.add(address)
  for a in matches:self.reached[a].add(address)
 def call(self,start):
  u=self.u;u.reg_write(UC_ARM_REG_R0,Q);u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP);u.emu_start(start,STOP+4,count=10000)
  assert self.status or u.reg_read(UC_ARM_REG_PC)==STOP
 def execute(self,row):
  self.row=row;self.trace=[];self.status=0;self.allocated=self.retired=False;u=self.u;u.mem_write(Q,bytes([0xcc])*48)
  if row[0] in [3,4]:put(u,Q,VT[1 if row[1]==13 else row[1]+3])
  if row[0] in [0,5,6]:calls=[FACTORIES[row[1]]]
  elif row[0]<3:calls=[C[row[0]-1]]
  else:calls=[(D1 if row[0]==3 else D0)[row[1]]]
  if row[0] in [5,6]:calls.append((D1 if row[0]==5 else D0)[row[1]])
  for start in calls:
   self.call(start)
   if self.status:break
  core=get(u,Q);secondary=get(u,Q+0x18)
  fields=[VT.index(core) if core in VT else 0xffffffff,signed(get(u,Q+4)),u.mem_read(Q+8,1)[0],get(u,Q+0xc),get(u,Q+0x10),u.mem_read(Q+0x14,1)[0],SVT.index(secondary) if secondary in SVT else 0xffffffff,get(u,Q+0x1c),get(u,Q+0x20),get(u,Q+0x24),get(u,Q+0x28),get(u,Q+0x2c),*u.mem_read(Q+9,3),*u.mem_read(Q+0x15,3)]
  return dict(status=self.status,fields=fields,allocated=int(self.allocated),retired=int(self.retired),trace=self.trace.copy())
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--library',type=Path);p.add_argument('--world-library',type=Path);p.add_argument('--write-pins',action='store_true');a=p.parse_args()
 data,names,_=image(a.original);pins=evidence(data,names)
 if a.write_pins:PIN.parent.mkdir(parents=True,exist_ok=True);PIN.write_text(json.dumps(pins,indent=2)+'\n');return
 assert pins==json.loads(PIN.read_text());assert pins['factory_dispatch']['entries']==FACTORIES+[0]
 oracle=Original(data,pins);rows=cases();expected=[oracle.execute(row) for row in rows];base_count=len(rows)
 for row,result in zip(rows.copy(),expected.copy()):
  for ordinal in range(1,len(result['trace'])+1):
   for throwing in [0,1]:failure=row.copy();failure[4:]=[ordinal,throwing];rows.append(failure);expected.append(oracle.execute(failure))
 assert oracle.reached and all(len(oracle.reached[int(f['elf_address'],0)])==f['ordinary_words'] for f in pins['functions']),'Unreached source constructor/destructor words'
 a.output.mkdir(parents=True,exist_ok=True);inputs=a.output/'inputs.txt';inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n')
 source=MODULE/'quest_objective_factory_v1.cpp';test=MODULE/'tests/quest_objective_factory_v1_host.cpp';exe=a.output/'host.exe'
 def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
 evidence_paths={source,source.with_suffix('.hpp'),MODULE/'quest_objective_list_v1.cpp',MODULE/'quest_objective_list_v1.hpp',MODULE/'quest_table_bindings_v1.cpp',MODULE/'quest_table_bindings_v1.hpp',MODULE/'quest_runtime_fields_v1.hpp',test,Path(__file__).resolve(),PIN,ROOT/'port/quest-data/quests.c',ROOT/'port/quest-data/quests.h',ROOT/'port/pydata-constants/constants.c',ROOT/'port/pydata-constants/constants.h'}
 def snapshot(paths):return {path.relative_to(ROOT).as_posix():sha(path) for path in sorted(paths)}
 selected=[];selected_commands='';dsos=[];binary_before={};paths=evidence_paths;env=os.environ.copy();env['PATH']=str(Path(a.compiler).parent)+os.pathsep+env.get('PATH','')
 if a.library:
  assert a.world_library,'The actual constants decoder is selected by dh2_level_world'
  sys.path.insert(0,str(ROOT/'port/level-world/tests'));from run_player_skill_cleanup_session_v1_host import selected_entries,actual_dependencies
  library=a.library.resolve();command_file=next(path for path in library.parents if (path/'compile_commands.json').is_file())/'compile_commands.json';selected_build=command_file.parent;ninja=shutil.which('ninja');assert ninja
  result=subprocess.run([ninja,'-C',str(selected_build),'-t','commands','dh2_game_data','dh2_level_world'],capture_output=True,text=True,env=env);assert result.returncode==0,result.stderr;selected_commands=result.stdout;selected=selected_entries(selected_build,selected_commands)
  for name in [source.name,'quest_objective_list_v1.cpp','quest_table_bindings_v1.cpp','quests.c','constants.c']:assert sum(Path(entry['file']).name==name for entry in selected)==1,name
  paths=actual_dependencies(selected_build,ninja,selected)|evidence_paths;dsos=sorted(library.parent.parent.rglob('*.dll'));assert sum(path.name=='libdh2_game_data.dll' for path in dsos)==1
  binary_before={str(path):sha(path) for path in [library,a.world_library.resolve(),*dsos]};env['PATH']=os.pathsep.join([*(str(path.parent) for path in dsos),env['PATH']])
 before=snapshot(paths);command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2',str(test)]
 if a.library:command += [str(a.library.resolve()),str(a.world_library.resolve())]
 else:
  for name in ['port/quest-data/quests.c','port/pydata-constants/constants.c']:
   obj=a.output/(Path(name).stem+'.obj');built=subprocess.run([str(Path(a.compiler).with_name('gcc.exe')),'-std=c11','-Wall','-Wextra','-Werror','-O1','-c',str(ROOT/name),'-o',str(obj)],capture_output=True,text=True);assert built.returncode==0,built.stderr;command.append(str(obj))
  command += [str(source),str(MODULE/'quest_objective_list_v1.cpp'),str(MODULE/'quest_table_bindings_v1.cpp')]
 command+=['-o',str(exe)];built=subprocess.run(command,capture_output=True,text=True);assert built.returncode==0,built.stderr
 run=subprocess.run([str(exe),str(inputs),str(a.cache.resolve())],capture_output=True,text=True,env=env);assert run.returncode==0,run.stderr;native=json.loads(run.stdout);assert len(native['results'])==len(rows)
 mismatches=[dict(index=i,input=row,expected=e,actual=n) for i,(row,e,n) in enumerate(zip(rows,expected,native['results'])) if e!=n];(a.output/'mismatches.json').write_text(json.dumps(mismatches,indent=2)+'\n');assert not mismatches,f'{len(mismatches)} mismatches at {a.output}/mismatches.json'
 after=snapshot(paths);assert before==after,'Source changed during comparison'
 report=dict(validation='PASS',cases=len(rows),valid_whole_callers=base_count,error_exception_prefixes=len(rows)-base_count,original_words=len(oracle.words),function_reached_words={hex(k):len(v) for k,v in oracle.reached.items()},native_guard_checks=native['native_guard_checks'],native_checks=native['native_checks'],actual_objective_types=native['actual_objectives'],mismatches=0,implementation='selected library' if a.library else 'direct translation units',scope=__doc__,native_wired=False,android_compilation=False,live_gameplay=False,source_sha256=before,source_before_after_equal=True,cache_sha256={name:sha(a.cache/name) for name in ['v2quests_pyarray.bin','v2quests_pyarraynames.bin','v2quests_pycst.bin']})
 if a.library:
  result=subprocess.run([ninja,'-C',str(selected_build),'-t','commands','dh2_game_data','dh2_level_world'],capture_output=True,text=True,env=env);assert result.returncode==0 and result.stdout==selected_commands;assert actual_dependencies(selected_build,ninja,selected)|evidence_paths==paths;assert {str(path):sha(path) for path in [library,a.world_library.resolve(),*dsos]}==binary_before,'Production DLL changed during comparison'
  imports={}
  for path in [exe,*dsos]:
   result=subprocess.run([str(Path(a.compiler).with_name('objdump.exe')),'-p',str(path)],capture_output=True,text=True,env=env);assert result.returncode==0,result.stderr;imports[str(path)]=re.findall(r'DLL Name: (\S+)',result.stdout)
  assert 'libdh2_game_data.dll' in imports[str(exe)]
  assert 'libdh2_level_world.dll' in imports[str(exe)]
  report.update(selected_commands=selected_commands,selected_command_entry=next(entry for entry in selected if Path(entry['file']).resolve()==source.resolve()),source_tu_count=1,commands_before_after_equal=True,binary_before_after_equal=True,binary_sha256={**binary_before,str(exe):sha(exe)},binary_imports=imports)
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({key:report[key] for key in ['validation','cases','valid_whole_callers','error_exception_prefixes','original_words','native_guard_checks','actual_objective_types','mismatches','implementation','source_before_after_equal']}))
if __name__=='__main__':main()
