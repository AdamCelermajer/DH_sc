"""Activity/state leaves against pinned original ARM and the selected library."""
from __future__ import annotations
import argparse,hashlib,itertools,json,os,random,struct,subprocess
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_SP,UC_ARM_REG_LR,UC_ARM_REG_PC
from run_player_info_record_v1 import Original as RecordOriginal,OFFSETS,STRINGS,BYTES
from player_locality_v1_original import image,get,put
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/player-info-level'
PIN=MODULE/'reference/player-info-activity-v1/original-functions.json'
BASE=0x10001000;ONLINE=0x10004000;STOP=0x30000000;STACK=0x20008000
FUNCTIONS=[0x80f124,0x36d48c,0x36f40c,0x36f40c]
class Original:
 def __init__(self,path):
  data,names,_=image(path);self.pins=json.loads(PIN.read_text());self.ranges=[]
  for pin in self.pins['functions']:
   a=int(pin['elf_address'],0);s=names[pin['original_symbol']]
   assert (s['st_value'],s['st_size'])==(a,pin['size'])
   assert hashlib.sha256(data[a:a+pin['size']]).hexdigest()==pin['sha256']
   self.ranges.append((a,pin['size']))
  assert names[self.pins['global_serial_symbol']]['st_value']==int(self.pins['global_serial_address'],0)
  self.serial=int(self.pins['global_serial_address'],0)
  table=names['_ZTV13NetStructUIntILj8EE']['st_value']+8
  assert struct.unpack_from('<I',data,table+0x1c)[0]==0x36da08
  self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);self.u.mem_map(0,len(data));self.u.mem_write(0,data)
  for a,n in [(0x10000000,0x10000),(0x20000000,0x10000),(STOP,0x1000)]:self.u.mem_map(a,n)
  self.words=set();self.calls=0;self.online=0;self.mutation=0
  self.u.hook_add(UC_HOOK_CODE,self.hook)
  # Original full C1 supplies genuine declared member order and vtable dispatch.
  # Activity fixtures don't reconstruct a second property or packet store.
  owner=RecordOriginal(path);rng=random.Random(0x36d48c)
  owner.prepare(bytes(rng.randrange(256) for _ in range(0x688)),0)
  self.baseline=bytes(owner.u.mem_read(BASE,0x688))
 def hook(self,u,address,size,context):
  if address==STOP:u.emu_stop();return
  if address==0x7fd794:
   self.calls+=1;u.mem_write(ONLINE+5,bytes([self.online]))
   if self.mutation==1:put(u,BASE+0x178,-1)
   if self.mutation==2:
    for offset in [0x178,0x1a0,0x1c8]:put(u,BASE+offset,0)
    put(u,BASE+0x1f0,3)
   u.reg_write(UC_ARM_REG_R0,ONLINE);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR));return
  assert any(a<=address<a+n for a,n in self.ranges),f'unscoped original {address:#x}'
  self.words.add(address)
 def project(self):
  u=self.u
  fields=[get(u,BASE),get(u,BASE+0x104),u.mem_read(BASE+0x108,1)[0],u.mem_read(BASE+0x124,1)[0],u.mem_read(BASE+0x125,1)[0],get(u,BASE+0x128),get(u,BASE+0x280)]
  out=struct.pack('<7I',*fields)+bytes(u.mem_read(BASE+4,256))
  for offset in OFFSETS:out+=bytes(u.mem_read(BASE+offset,32 if offset in STRINGS or offset in BYTES or 0x4c8<=offset<=0x528 else 36))
  fields=[get(u,BASE+0x660),get(u,BASE+0x664),get(u,BASE+0x668),u.mem_read(BASE+0x66c,1)[0],get(u,BASE+0x670),get(u,BASE+0x674),get(u,BASE+0x678),get(u,BASE+0x67c),get(u,BASE+0x680),get(u,BASE+0x684)]
  return out+struct.pack('<10I',*fields)
 def fixture(self,op,identities,activity,old_state,value,residue,serial,online,mutation=0):
  self.calls=0;self.online=online;self.mutation=mutation;u=self.u
  u.mem_write(BASE,self.baseline);u.mem_write(self.serial,struct.pack('<Q',serial));u.mem_write(STACK-0x100,bytes(0x100))
  for offset,v in zip([0x178,0x1a0,0x1c8],identities):put(u,BASE+offset,v)
  put(u,BASE+0x1f0,activity);put(u,BASE+0x240,old_state)
  # Nontrivial preexisting target header proves same-value preservation and
  # changed stores; padding and every other owned header are also projected.
  u.mem_write(BASE+0x228,struct.pack('<Q',0x1020304050607080));put(u,BASE+0x238,0x18171615)
  u.mem_write(BASE+0x23c,b'\7');put(u,STACK-32,residue)
  raw=bytes(u.mem_read(BASE,0x688));payload=struct.pack('<5IQ',op,value&0xffffffff,residue&0xffffffff,online,mutation,serial)+raw
  u.reg_write(UC_ARM_REG_R0,BASE);u.reg_write(UC_ARM_REG_R1,value&0xffffffff)
  u.reg_write(UC_ARM_REG_SP,STACK);u.reg_write(UC_ARM_REG_LR,STOP)
  u.emu_start(FUNCTIONS[op],STOP+4,count=3000);assert u.reg_read(UC_ARM_REG_PC)==STOP
  result=u.reg_read(UC_ARM_REG_R0) if op<2 else 0
  expected=bytes(u.mem_read(self.serial,8))+struct.pack('<II',result,self.calls)+self.project()
  return payload,expected
def cases(original):
 rows=[];serials=[0,9,0xffffffff,0xffffffffffffffff]
 identities=list(itertools.product([-2147483648,-1,0,2147483647],repeat=3))
 for ident in identities:
  for activity in [0,1,2,3,4,255,-1,0x10003]:
   for op,online in [(0,0),(1,0),(1,1),(1,7),(1,255)]:
    rows.append(original.fixture(op,ident,activity,0,0,0,9,online))
 for op in [2,3]:
  for old,value,residue_mode,serial in itertools.product([0,3,255,-1,-2147483648,0x10003],[-128,-1,0,1,3,127,128,255],range(3),serials):
   residue=[value,old,0x76543210][residue_mode]
   rows.append(original.fixture(op,(0,0,0),3,old,value,residue,serial,0))
 for online,mutation in itertools.product([0,1], [1,2]):
  rows.append(original.fixture(1,(0,0,0) if mutation==1 else (-1,-1,-1),3 if mutation==1 else 0,0,0,0,9,online,mutation))
 return rows
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--library',type=Path);a=p.parse_args()
 a.output.mkdir(parents=True,exist_ok=True);original=Original(a.original);rows=cases(original)
 inputs=a.output/'fixtures.bin';outputs=a.output/'native.bin';exe=a.output/'player_info_activity_v1_host.exe'
 inputs.write_bytes(struct.pack('<I',len(rows))+b''.join(row[0] for row in rows))
 sources=['player_info_activity_v1.cpp','player_info_record_v1.cpp','cnet_player_info_v1.cpp','netstruct_members_v1.cpp','character_level_member.cpp']
 command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror',str(MODULE/'tests/player_info_activity_v1_host.cpp')]
 command += [str(a.library)] if a.library else [str(MODULE/x) for x in sources]
 subprocess.run(command+['-o',str(exe)],check=True);env=os.environ.copy();dlls=[]
 if a.library:
  dlls=sorted(a.library.parent.parent.rglob('*.dll'));env['PATH']=os.pathsep.join([*(str(x.parent) for x in dlls),env['PATH']])
 native_run=subprocess.run([str(exe),str(inputs),str(outputs)],env=env,check=True,capture_output=True,text=True)
 expected=b''.join(row[1] for row in rows);native=outputs.read_bytes()
 if native!=expected:
  mismatch=[(i,native[i] if i<len(native) else None,expected[i]) for i in range(len(expected)) if i>=len(native) or native[i]!=expected[i]]
  raise AssertionError((len(native),len(expected),mismatch[:20]))
 report=dict(validation='PASS',cases=len(rows),activity_cases=sum(struct.unpack_from('<I',r[0])[0]<2 for r in rows),state_cases=sum(struct.unpack_from('<I',r[0])[0]>=2 for r in rows),native_policy_checks=int(native_run.stdout.strip()),compared_projection_bytes=len(native),original_words=len(original.words),implementation='selected library' if a.library else 'direct translation units',scope=original.pins['scope'],shared_serial_wrap_cases=True,activity_and_set_state_fields_distinct=True,source_sha256={x:hashlib.sha256((MODULE/x).read_bytes()).hexdigest() for x in [*sources,'player_info_activity_v1.hpp','tests/player_info_activity_v1_host.cpp','tests/run_player_info_activity_v1.py']})
 if a.library:
  commands=json.loads((a.library.parent.parent/'compile_commands.json').read_text());report['selected_commands']=[c for c in commands if Path(c['file']).name in sources];assert len(report['selected_commands'])==len(sources)
  report['binary_sha256']={x.name:hashlib.sha256(x.read_bytes()).hexdigest() for x in [exe,*dlls]}
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items() if k not in ['selected_commands','binary_sha256']}))
if __name__=='__main__':main()
