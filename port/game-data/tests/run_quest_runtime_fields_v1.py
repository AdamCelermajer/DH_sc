"""Complete Quest scalar callers versus original ARM, with explicit child providers.

The three actual embedded-list constructors execute too. Whole Quest D1/D2
destruction order, action clears and failure prefixes also execute. List assignment,
owner propagation, objective creation, list destruction and virtual cleanup are declared fixture
boundaries; no quest VM/property/child implementation or live gameplay is inferred.
"""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put,signed,ELF_SHA
PIN=MODULE/'reference/quest-runtime-fields-v1/original-functions.json'
Q=0x10000100;ROWS=[0x10003000,0x10004000];ACTIONS=[0x10006000,0x10006100];STOP=0x30000000
STARTS=[0x4808e4,0x4807d8,0x48081c,0x480060,0x47f78c,0x47f70c,0x4809a4,0x480a14]
RANGES=[(a,n) for a,n in zip(STARTS,[96,68,200,252,112,40,112,112])]+[(0x4786f0,20),(0x47a3d4,20),(0x482b0c,64),(0x31167c,108)]
EXTERNAL={0x4786f0:1,0x47a3d4:2,0x482b0c:3,0x47a414:4,0x4828f0:5,0x478914:6,0x47aa48:7,0x483950:8,0x47a3e8:9,0x47a5cc:10,0x47a5f0:11,STOP+0x100:12,STOP+0x104:12,STOP+0x108:12,STOP+0x10c:12,0x459090:15,0x47a798:16,0x482ba8:17,0x47a8cc:18,0x478eac:19}
READS={0x480828:0x14,0x480830:0x18,0x480840:0x1c,0x480844:0x20,0x480870:0x24,0x480874:0x28,0x4808bc:0x34,0x4808c0:0x38,0x4808d4:0x2c,0x4808d8:0x30,0x4808a8:0x118,**{a:0x9c for a in [0x4800b4,0x4800d8,0x4800fc,0x480114,0x48012c,0x480150]}}
def cases():
 rows=[]
 def add(mode=0,state=-1,difficulty=1,mask=3,owner=123,mutation=0):rows.append([mode,state,difficulty,mask,owner,mutation,0,0])
 for difficulty in [-2147483648,-2,-1,0,1,2,3,2147483647]:
  for owner in [0,123,0xffffffff]:add(difficulty=difficulty,owner=owner);add(difficulty=difficulty,owner=owner,mutation=6)
 for mask in range(4):
  for owner in [0,123,0xffffffff]:add(mode=1,mask=mask,owner=owner);add(mode=1,mask=mask,owner=owner,mutation=1)
  for mode in [6,7]:
   for mutation in [0,11,12]:add(mode=mode,mask=mask,mutation=mutation)
 for difficulty in [-2,-1,0,1,2,3]:
  for owner in [0,123,0xffffffff]:
   for mutation in range(5):add(mode=2,difficulty=difficulty,owner=owner,mutation=mutation)
 for state in [-2147483648,-1,*range(12),2147483647]:
  for mutation in [0,5]:add(mode=3,state=state,mutation=mutation)
  add(mode=5,state=state)
  for flag in [0,1,2,255,0xffffffff]:
   for mutation in [0,9,10]:add(mode=4,state=state,difficulty=flag,mutation=mutation)
 return rows
def execute(data,row,words):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
 for base,size in [(0x10000000,0x10000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
 u.mem_write(Q,bytes([0xcc])*0x6c)
 put(u,Q,0xcccccccc if row[0]==4 else row[1]);put(u,Q+0x10,row[2]);put(u,Q+0x60,row[4]);put(u,Q+0x68,ROWS[0])
 for i,action in enumerate(ACTIONS):
  table=0x10007000+i*0x100;put(u,action,table);put(u,action+0x10,0xcccccccc)
  put(u,table+0x14,STOP+0x100);put(u,table+0x1c,STOP+0x104)
  put(u,table+0x28,STOP+0x108)
  put(u,table+4,STOP+0x10c)
  put(u,Q+0x18+i*4,action if row[3]&(1<<i) else 0)
 for n,base in enumerate(ROWS):
  for i in range(71):put(u,base+i*4,0x70000000+n*0x1000+i)
  for offset in [0x14,0x1c,0x24,0x2c,0x34]:put(u,base+offset,-2 if n else 3)
  put(u,base+0x9c,-1 if n else 6);put(u,base+0x118,-1 if n else 41)
 trace=[];status=0;created=0;cursor=0
 def ret(value=0):u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def event(op,target,value=0,count=0,py=0,offset=0,stream=0):
  nonlocal status,cursor
  trace.append([op,target,value,count,py,offset,stream])
  if row[5]==9 and op==15:put(u,Q,(get(u,Q)&0xffff0000)|(row[1]&0xffff));cursor+=2
  if row[6]>0 and len(trace)==row[6]:status=3;u.emu_stop();return False
  return True
 def mutate(op,offset=0):
  if row[5]==1 and op==4:put(u,Q+0x60,get(u,Q+0x60)+7)
  if row[5]==2 and op==6:put(u,Q+0x68,ROWS[1])
  if row[5]==3 and op==7:put(u,Q+0x10,2)
  if row[5]==4 and op==9 and offset==0x3c:put(u,Q+0x68,ROWS[1])
  if row[5]==5 and op in [10,11,12]:put(u,Q+0x68,ROWS[1])
  if row[5]==6 and op==2:put(u,Q+0x60,get(u,Q+0x60)+9)
  if row[5]==10 and op==16:put(u,Q,3)
  if row[5]==11 and op==12 and offset==4:put(u,Q+0x1c,0)
  if row[5]==12 and op==12 and offset==4:put(u,Q+0x1c,ACTIONS[0])
 def hook(machine,address,size,context):
  nonlocal created,cursor
  if address==STOP:machine.emu_stop();return
  a,b,c=[machine.reg_read(reg) for reg in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2]]
  if address in EXTERNAL:
   op=EXTERNAL[address]
   if op in [1,2,3]:
    if not event(op,a):return
    mutate(op);words.add(address);return # execute all real ctor instructions
   if op in [4,5]:
    if not event(op,a,b):return
    mutate(op);ret();return
   if op in [6,7,8]:
    if not event(op,a,b,signed(c)):return
    mutate(op);ret();return
   if op==9:
    base=get(u,Q+0x68);offset=a-base
    if not event(op,a,0,0,base,offset):return
    mutate(op,offset);value=ACTIONS[0] if created==0 else ACTIONS[1];created+=1;ret(value);return
   if op in [10,11]:
    if not event(op,a):return
    mutate(op);ret();return
   if op==12:
    offset={STOP+0x100:0x14,STOP+0x104:0x1c,STOP+0x108:0x28,STOP+0x10c:4}[address]
    if not event(op,a,0,0,0,offset,b if offset==0x28 else 0):return
    if offset==0x28:cursor+=3
    mutate(op,offset);ret();return
   if op==15:
    assert a==0x10008000 and b==Q
    if not event(op,a,b,stream=a):return
    put(u,b,row[1]);cursor+=4;ret();return
   if op==16:
    assert a==Q+0x2c and b==0x10008000
    if not event(op,a,stream=b):return
    cursor+=2;mutate(op);ret();return
   if op in [17,18,19]:
    if not event(op,a):return
    ret();return
  assert any(start<=address<start+length for start,length in RANGES),f'unscoped instruction {address:#x}'
  words.add(address)
  if address in READS:
   offset=READS[address];base=get(u,Q+0x68)
   if not event(13,base+offset,0,0,base,offset):return
 u.hook_add(UC_HOOK_CODE,hook)
 u.reg_write(UC_ARM_REG_R0,Q);u.reg_write(UC_ARM_REG_R1,(ROWS[0] if row[0]==2 else 0x10008000 if row[0]==4 else row[1] if row[0]==5 else row[2])&0xffffffff)
 u.reg_write(UC_ARM_REG_R2,row[2]&0xffffffff)
 u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP)
 u.emu_start(STARTS[row[0]],STOP+4,count=100000)
 projection=[signed(get(u,Q+i*4)) for i in range(5)]+[get(u,Q+i*4) for i in range(5,23)]
 projection += [u.mem_read(Q+0x5c,1)[0],u.mem_read(Q+0x5d,1)[0],get(u,Q+0x60),u.mem_read(Q+0x64,1)[0],get(u,Q+0x68),get(u,ACTIONS[0]+0x10),get(u,ACTIONS[1]+0x10)]
 projection += [cursor,u.reg_read(UC_ARM_REG_R0) if row[0]==5 else 0]
 return dict(status=status,projection=projection,trace=trace)
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',required=True,type=Path);p.add_argument('--output',required=True,type=Path);p.add_argument('--compiler',required=True);p.add_argument('--library',type=Path);args=p.parse_args()
 data,names,_=image(args.original);pins=json.loads(PIN.read_text());assert pins['original_sha256']==ELF_SHA
 for pin in pins['functions']+pins['declared_callee_dependencies']:
  address=int(pin['elf_address'],0);symbol=names[pin['original_symbol']]
  assert(symbol['st_value'],symbol['st_size'])==(address,pin['size'])
  assert hashlib.sha256(data[address:address+pin['size']]).hexdigest()==pin['sha256']
 table=pins['volatile_state_table'];address=int(table['elf_address'],0)
 assert list(data[address:address+table['size']])==table['values']
 assert hashlib.sha256(data[address:address+table['size']]).hexdigest()==table['sha256']
 rows=cases();words=set();base=[execute(data,row,words) for row in rows]
 # Every reached service boundary is tested as an error and an exception; both
 # projections preserve all source stores delivered before that boundary.
 for row,result in zip(rows.copy(),base):
  for ordinal in range(1,len(result['trace'])+1):
   for throwing in [0,1]:failure=row.copy();failure[6:]=[ordinal,throwing];rows.append(failure)
 args.output.mkdir(parents=True,exist_ok=True);inputs=args.output/'inputs.txt';inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n')
 test=MODULE/'tests/quest_runtime_fields_v1_host.cpp';source=MODULE/'quest_runtime_fields_v1.cpp';exe=args.output/'host.exe'
 def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
 evidence={source,source.with_suffix('.hpp'),test,Path(__file__).resolve(),PIN}
 def snapshot(paths):return {path.relative_to(ROOT).as_posix():sha(path) for path in sorted(paths)}
 selected=[];selected_commands='';dsos=[];binary_before={};paths=evidence
 env=os.environ.copy();env['PATH']=str(Path(args.compiler).parent)+os.pathsep+env.get('PATH','')
 if args.library:
  sys.path.insert(0,str(ROOT/'port/level-world/tests'))
  from run_player_skill_cleanup_session_v1_host import selected_entries,actual_dependencies
  library=args.library.resolve();command_file=next(path for path in library.parents if(path/'compile_commands.json').is_file())/'compile_commands.json';selected_build=command_file.parent
  ninja=shutil.which('ninja');assert ninja
  result=subprocess.run([ninja,'-C',str(selected_build),'-t','commands','dh2_game_data'],capture_output=True,text=True,env=env);assert result.returncode==0,result.stderr
  selected_commands=result.stdout;selected=selected_entries(selected_build,selected_commands)
  assert sum(Path(entry['file']).resolve()==source.resolve() for entry in selected)==1
  paths=actual_dependencies(selected_build,ninja,selected)|evidence
  dsos=sorted(library.parent.parent.rglob('*.dll'));assert sum(path.name=='libdh2_game_data.dll' for path in dsos)==1
  binary_before={str(path):sha(path) for path in [library,*dsos]}
  env['PATH']=os.pathsep.join([*(str(path.parent) for path in dsos),env['PATH']])
 before=snapshot(paths)
 command=[args.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2',str(test),str(args.library.resolve()) if args.library else str(source),'-o',str(exe)]
 build=subprocess.run(command,capture_output=True,text=True);assert build.returncode==0,build.stderr
 native=subprocess.run([str(exe),str(inputs)],capture_output=True,text=True,env=env);assert native.returncode==0,native.stderr
 actual=json.loads(native.stdout);expected=[execute(data,row,words) for row in rows];assert len(expected)==len(actual['results'])
 mismatches=[dict(index=i,input=row,expected=e,actual=n) for i,(row,e,n) in enumerate(zip(rows,expected,actual['results'])) if e!=n]
 (args.output/'comparison.json').write_text(json.dumps(dict(expected=expected,actual=actual),indent=2)+'\n')
 (args.output/'mismatches.json').write_text(json.dumps(mismatches,indent=2)+'\n');assert not mismatches,f'{len(mismatches)} mismatches; see {args.output}/mismatches.json'
 after=snapshot(paths);assert before==after,'Selected source changed during comparison'
 report=dict(validation='PASS',cases=len(rows),valid_whole_callers=len(base),error_exception_prefixes=len(rows)-len(base),original_words=len(words),native_guard_checks=actual['native_guard_checks'],mismatches=0,implementation='selected library' if args.library else 'direct translation unit',scope=__doc__,source_sha256=before,source_before_after_equal=True)
 if args.library:
  result=subprocess.run([ninja,'-C',str(selected_build),'-t','commands','dh2_game_data'],capture_output=True,text=True,env=env);assert result.returncode==0 and result.stdout==selected_commands
  assert actual_dependencies(selected_build,ninja,selected)|evidence==paths
  assert {str(path):sha(path) for path in [library,*dsos]}==binary_before,'Frozen production DLL changed during comparison'
  imports={}
  for path in [exe,*dsos]:
   result=subprocess.run([str(Path(args.compiler).with_name('objdump.exe')),'-p',str(path)],capture_output=True,text=True,env=env);assert result.returncode==0,result.stderr
   imports[str(path)]=re.findall(r'DLL Name: (\S+)',result.stdout)
  assert 'libdh2_game_data.dll' in imports[str(exe)]
  report.update(selected_commands=selected_commands,selected_command_entry=next(entry for entry in selected if Path(entry['file']).resolve()==source.resolve()),source_tu_count=1,commands_before_after_equal=True,binary_before_after_equal=True,binary_sha256={**binary_before,str(exe):sha(exe)},binary_imports=imports)
 (args.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({key:report[key] for key in ['validation','cases','valid_whole_callers','error_exception_prefixes','original_words','native_guard_checks','mismatches','implementation','source_before_after_equal']}))
if __name__=='__main__':main()
