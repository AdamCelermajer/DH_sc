"""Complete saved level/WorldMap initializer versus pinned original ARM."""
from __future__ import annotations
import argparse,hashlib,itertools,json,os,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R4,UC_ARM_REG_SP,UC_ARM_REG_LR,UC_ARM_REG_PC
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put
PIN=MODULE/'reference/player-save-level-states-v1/original-functions.json'
SAVE=0x10001000;STOP=0x30000000;FUNCTION=0x46954c
TABLES=[0x10021000,0x10022000];SECOND=0x10023000
COUNTS=[0x9a65e4,0x9a66f8];MEMBERS=[0x9a65e8,0x9a66fc];STRIDES=[0x48,0x14];DEFAULTS=[0x28,8]
def source_offset(slot):return (0x74 if slot%2 else 0x68)+(slot//2)*4
class Original:
 def __init__(self,path):
  data,names,_=image(path);self.pins=json.loads(PIN.read_text())
  pin=self.pins['functions'][0];symbol=names[pin['original_symbol']]
  assert (symbol['st_value'],symbol['st_size'])==(FUNCTION,pin['size'])
  assert hashlib.sha256(data[FUNCTION:FUNCTION+pin['size']]).hexdigest()==pin['sha256']
  for name,address in self.pins['table_globals'].items():assert names[name]['st_value']==int(address,0)
  got=0x469578+struct.unpack_from('<I',data,0x469674)[0]
  for offset,address in zip([0x18c0,0x874,0x2274,0x3dd0],[COUNTS[0],MEMBERS[0],COUNTS[1],MEMBERS[1]]):
   assert struct.unpack_from('<I',data,got+offset)[0]==address
  self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);self.u.mem_map(0,len(data));self.u.mem_write(0,data)
  for base,size in [(0x10000000,0x40000),(0x20000000,0x10000),(STOP,0x1000)]:self.u.mem_map(base,size)
  self.words=set();self.u.hook_add(UC_HOOK_CODE,self.code);self.u.hook_add(UC_HOOK_MEM_READ,self.read);self.u.hook_add(UC_HOOK_MEM_WRITE,self.write)
 def code(self,u,address,size,context):
  if address==STOP:u.emu_stop();return
  if address==0x31056c:
   length=u.reg_read(UC_ARM_REG_R0);tag=u.reg_read(UC_ARM_REG_R1);identity=self.next;self.next+=1
   self.events.append([2,length,tag,identity]);assert tag==0
   pointer=0 if not length and self.zero_null else self.heap
   if pointer:
    self.heap+=0x400;self.allocations[pointer]=(identity,length//4);u.mem_write(pointer,bytes([0x5a])*(length or 4))
   if not self.changed:
    if self.mutation==1 and self.table==0:put(u,COUNTS[0],1);self.changed=True
    elif self.mutation==3 and self.table==0:put(u,MEMBERS[0],SECOND);self.changed=True
    elif self.mutation==4 and self.table==1:put(u,COUNTS[1],0);self.changed=True
   u.reg_write(UC_ARM_REG_R0,pointer);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR));return
  assert FUNCTION<=address<FUNCTION+316,f'unscoped original {address:#x}'
  self.words.add(address)
 def read(self,u,access,address,size,value,context):
  if address in COUNTS:
   self.table=COUNTS.index(address);self.events.append([1,self.table,get(u,address),0])
  for table in range(2):
   start=get(u,MEMBERS[table]);relative=address-start-DEFAULTS[table]
   if relative>=0 and relative%STRIDES[table]==0 and relative//STRIDES[table]<3:
    row=relative//STRIDES[table];self.events.append([3,table,row,get(u,address)])
    if not self.changed and self.mutation==2 and table==0 and row==0:put(u,COUNTS[0],1);self.changed=True
 def write(self,u,access,address,size,value,context):
  for slot in range(6):
   if address==SAVE+source_offset(slot):
    identity,capacity=self.allocations.get(value,(0,0));self.events.append([4,slot,identity,capacity])
  for start,(identity,count) in self.allocations.items():
   if start<=address<start+count*4:self.events.append([5,identity,(address-start)//4,value&0xffffffff])
 def call(self):
  u=self.u;u.reg_write(UC_ARM_REG_R0,SAVE);u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP)
  u.emu_start(FUNCTION,STOP+4,count=10000);assert u.reg_read(UC_ARM_REG_PC)==STOP
 def execute(self,row):
  mask,levels,world,mutation,zero_null,repeat=row;u=self.u
  self.events=[];self.allocations={};self.next=16;self.heap=0x10018000;self.table=0;self.changed=False;self.mutation=mutation;self.zero_null=zero_null
  u.mem_write(SAVE,bytes(0x198))
  for table,count in enumerate([levels,world]):
   put(u,COUNTS[table],count);put(u,MEMBERS[table],TABLES[table]);u.mem_write(TABLES[table],bytes(3*STRIDES[table]))
   for index in range(3):put(u,TABLES[table]+index*STRIDES[table]+DEFAULTS[table],[1,2,3][index] if table==0 else [4,5,6][index])
  for index,word in enumerate([101,202,303]):put(u,SECOND+index*STRIDES[0]+DEFAULTS[0],word)
  for slot in range(6):
   if mask&(1<<slot):
    pointer=0x10010000+slot*0x100;put(u,SAVE+source_offset(slot),pointer);self.allocations[pointer]=(100+slot,2)
    put(u,pointer,1000+slot);put(u,pointer+4,-2000-slot)
  self.call()
  if repeat:self.call()
  result=b''
  for slot in range(6):
   pointer=get(u,SAVE+source_offset(slot));identity,count=self.allocations.get(pointer,(0,0))
   result+=struct.pack('<II',identity,count)
   if pointer:result+=bytes(u.mem_read(pointer,count*4))
  return result+struct.pack('<I',len(self.events))+b''.join(struct.pack('<4I',*event) for event in self.events)
def cases():
 rows=[]
 for mask,levels,world in itertools.product(range(64),[0,1,3],[0,2,3]):rows.append([mask,levels,world,0,0,0])
 for mask,mutation in itertools.product([0,1,2,5,42,63],range(5)):rows.append([mask,3,3,mutation,0,1])
 for mask in range(64):rows.append([mask,0,0,0,1,1])
 return rows
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--library',type=Path);a=p.parse_args()
 a.output.mkdir(parents=True,exist_ok=True);original=Original(a.original);rows=cases();expected=b''.join(original.execute(row) for row in rows)
 inputs=a.output/'fixtures.bin';outputs=a.output/'native.bin';exe=a.output/'player_save_level_states_v1_host.exe'
 inputs.write_bytes(struct.pack('<I',len(rows))+b''.join(struct.pack('<6I',*row) for row in rows))
 sources=['player_save_level_states_v1.cpp'];command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror',str(MODULE/'tests/player_save_level_states_v1_host.cpp')]
 command += [str(a.library)] if a.library else [str(MODULE/x) for x in sources]
 subprocess.run(command+['-o',str(exe)],check=True);env=os.environ.copy();dlls=[]
 if a.library:
  dlls=sorted(a.library.parent.parent.rglob('*.dll'));env['PATH']=os.pathsep.join([*(str(x.parent) for x in dlls),env['PATH']])
 run=subprocess.run([str(exe),str(inputs),str(outputs)],env=env,check=True,capture_output=True,text=True);native=outputs.read_bytes()
 if native!=expected:
  differences=[(i,native[i] if i<len(native) else None,expected[i]) for i in range(len(expected)) if i>=len(native) or native[i]!=expected[i]]
  raise AssertionError((len(native),len(expected),differences[:20]))
 report=dict(validation='PASS',cases=len(rows),original_words=len(original.words),native_policy_checks=int(run.stdout.strip()),compared_projection_bytes=len(native),implementation='selected library' if a.library else 'direct translation unit',scope=original.pins['scope'],source_sha256={x:hashlib.sha256((MODULE/x).read_bytes()).hexdigest() for x in [*sources,'player_save_level_states_v1.hpp','player_savegame_v1.hpp','tests/player_save_level_states_v1_host.cpp','tests/run_player_save_level_states_v1.py','reference/player-save-level-states-v1/original-functions.json']})
 if a.library:
  build=next(parent for parent in a.library.parents if (parent/'compile_commands.json').is_file())
  commands=json.loads((build/'compile_commands.json').read_text());paths={(MODULE/x).resolve() for x in sources}
  report['selected_commands']=[c for c in commands if Path(c['file']).resolve() in paths];assert len(report['selected_commands'])==len(sources)
  report['binary_sha256']={x.name:hashlib.sha256(x.read_bytes()).hexdigest() for x in [exe,*dlls]}
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items() if k not in ['source_sha256','selected_commands','binary_sha256']}))
if __name__=='__main__':main()
