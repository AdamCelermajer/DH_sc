"""Full PlayerInfo lifecycle against pinned original ARM, selected native code."""
from __future__ import annotations
import argparse,hashlib,json,os,random,struct,subprocess
from pathlib import Path
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_SP,UC_ARM_REG_LR,UC_ARM_REG_PC
from run_cnet_player_info_v1 import Original as BaseOriginal,BASES,STOP
from player_locality_v1_original import image,get,put
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/player-info-level'
PIN=MODULE/'reference/player-info-record-v1/original-functions.json';SIZE=0x688
BASE_OFFSETS=[0x130,0x158,0x180,0x1a8,0x1d0,0x1f8,0x220,0x248]
DERIVED=[0x288,0x2b0,0x2e8,0x310,0x338,0x360,0x388,0x3b0,0x3d8,0x400,0x428,0x450,0x478,0x4a0,0x4c8,0x4e8,0x508,0x528,0x548,0x570,0x598,0x5c0,0x5e8,0x610,0x638]
OFFSETS=BASE_OFFSETS+DERIVED;BYTES=[0x3b0,0x3d8,0x400];STRINGS=[0x248,0x2b0]
FUNCTIONS=[0x37418c,0x374cc0,0x373bdc,0x3777d0,0x378808,0x378808,0x371294,0x37168c,0x371ccc,0x370e48,0x36fcb0,0x370ef0,0x370bf4,0x36fd58,0x36fe04]
SCALAR=[0x310,0x338,0x360,0x388,0x428,0x478]
GUARD=0x9a2390;CREATE=0x99e238;DELETE=0x99e23c;LOADING=0x10004000;LOADING_DELETE=STOP+0x100
READS={0x370e5c:0,0x36fcc4:1,0x370f04:2,0x370c08:3,0x36fd6c:4,0x36fe18:5,
       0x373d54:6,0x373de4:7,0x373e6c:8,0x373f18:9,0x373fac:10,0x374034:11,0x3740c0:12}
class Original(BaseOriginal):
 def __init__(self,path):
  super().__init__(path);data,names,_=image(path);self.pins=json.loads(PIN.read_text());self.ranges=[]
  for p in self.pins['functions']:
   a=int(p['elf_address'],0);s=names[p['original_symbol']]
   assert (s['st_value'],s['st_size'])==(a,p['size']) and hashlib.sha256(data[a:a+p['size']]).hexdigest()==p['sha256']
   self.ranges.append((a,p['size']))
  self.tracing=False;self.residues=[0]*13;self.seen=set();self.trace=[];self.ids={};self.next=1;self.byte_heap=0x10009000
  assert struct.unpack_from('<I',data,0x374b1c)[0]+0x374988==GUARD
  # Deleted payload provider's source vtable slot; no fabricated object body.
  put(self.u,LOADING,LOADING+0x100);put(self.u,LOADING+0x104,LOADING_DELETE)
 def hook(self,u,address,size,context):
  a,b,c=[u.reg_read(r) for r in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2]]
  if self.tracing and address in READS:
   index=READS[address];sp=u.reg_read(UC_ARM_REG_SP)
   if index not in self.seen:
    self.seen.add(index)
    off=0x20 if index<6 else [0x60,0x38,0x88,0x10d,0xed,0xcd,0xad][index-6]
    self.residues[index]=u.mem_read(sp+off,1)[0] if index>=9 else get(u,sp+off)
  if address==0x3140ec:
   value=bytes(u.mem_read(b,256)).split(b'\0',1)[0];self.string(a,value);result=a
  elif address==0x30e76c:
   assert a==GUARD
   result=int(not u.mem_read(a,1)[0]);self.trace.append([3,result,0,0])
  elif address==0x30ea3c:
   assert a==GUARD;u.mem_write(a,b'\1');self.trace.append([5,0,0,0]);result=0
  elif address==0x8102c0:
   assert (a,b)==(0x374ca0,0x36d258);self.trace.append([4,a,b,0]);result=None
  elif address==0x31056c:
   assert b==2;result=self.byte_heap;self.byte_heap+=0x100
   self.ids[result]=self.next;self.trace.append([1,a,b,self.next]);self.next+=1
  elif address==0x310440:
   self.trace.append([2,self.ids.pop(a),0,0]);result=0
  elif address==LOADING_DELETE:self.trace.append([6,a,0,0]);result=0
  else:result=None
  if result is not None:
   self.providers[hex(address)]=self.providers.get(hex(address),0)+1
   u.reg_write(UC_ARM_REG_R0,result&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR));return
  super().hook(u,address,size,context)
 def project(self,base,active=True):
  u=self.u
  fields=[get(u,base),get(u,base+0x104),u.mem_read(base+0x108,1)[0],u.mem_read(base+0x124,1)[0],u.mem_read(base+0x125,1)[0],get(u,base+0x128),get(u,base+0x280)]
  result=struct.pack('<7I',*fields)+bytes(u.mem_read(base+4,256))
  for off in OFFSETS:
   result+=bytes(u.mem_read(base+off,32 if off in STRINGS or off in BYTES or 0x4c8<=off<=0x528 else 36))
   if off in STRINGS:
    value=self.text(base+off+0x20) if active else b''
    result+=struct.pack('<I',len(value))+value
   elif off in BYTES:
    pointer=get(u,base+off+0x20);length=get(u,base+off+0x24)
    result+=struct.pack('<II',length,int(pointer!=0))
    if pointer and length:result+=bytes(u.mem_read(pointer,length))
  plain=[get(u,base+0x660),get(u,base+0x664),get(u,base+0x668),u.mem_read(base+0x66c,1)[0],get(u,base+0x670),get(u,base+0x674),get(u,base+0x678),get(u,base+0x67c),get(u,base+0x680),get(u,base+0x684)]
  return result+struct.pack('<10I',*plain)
 def prepare(self,residue,serial):
  self.heap=0x10005000;self.byte_heap=0x10009000;self.ids={};self.next=1;self.trace=[]
  self.u.mem_write(self.serial,struct.pack('<Q',serial));self.u.mem_write(GUARD,bytes(8))
  self.u.mem_write(0x20000000,bytes(0x10000))
  for base in BASES:self.u.mem_write(base,residue);self.call(FUNCTIONS[0],base)
 def fixture(self,op,value):
  u=self.u;a,b=BASES;guard=int(bool(u.mem_read(GUARD,1)[0]))
  before=bytes(u.mem_read(self.serial,8));payload=struct.pack('<III',op,value&0xffffffff,guard)+before
  for base in BASES:
   payload+=bytes(u.mem_read(base,SIZE))
   for off in STRINGS:
    v=self.text(base+off+0x20);payload+=struct.pack('<I',len(v))+v
   for off in BYTES:
    ptr=get(u,base+off+0x20);length=get(u,base+off+0x24);v=bytes(u.mem_read(ptr,length)) if ptr and length else b''
    payload+=struct.pack('<I',len(v))+v
  self.trace=[];self.ids={};self.next=1
  for record,base in enumerate(BASES):
   for index,off in enumerate(BYTES):
    ptr=get(u,base+off+0x20)
    if ptr:self.ids[ptr]=record*3+index+100
  self.residues=[0]*13;self.seen=set();self.tracing=True
  self.call(FUNCTIONS[op],b,b if op==5 else a if op in [3,4] else 0x10003000 if op==8 else value)
  self.tracing=False
  if op in [0,1,2]:assert len(self.seen)==13,self.seen
  if op>=9:assert len(self.seen)==1,self.seen
  expected=bytes(u.mem_read(self.serial,8))+self.project(a)+self.project(b,op not in [6,7])
  expected+=struct.pack('<III',int(bool(u.mem_read(GUARD,1)[0])),get(u,CREATE),get(u,DELETE))
  expected+=struct.pack('<I',len(self.trace))+b''.join(struct.pack('<4I',*t) for t in self.trace)
  name=self.text(0x10003000)
  return payload+struct.pack('<I',len(name))+name+struct.pack('<13I',*self.residues),expected
def cases(original):
 rng=random.Random(0x37418c);rows=[]
 for case in range(10):
  residue=bytes(rng.randrange(256) for _ in range(SIZE));serial=[0,9,0xffffffff,0xffffffffffffffff][case%4]
  for op in range(len(FUNCTIONS)):
   original.prepare(residue,serial);u=original.u;a,b=BASES
   for record,base in enumerate(BASES):
    for index,off in enumerate(OFFSETS):
     if off in STRINGS or off in BYTES:continue
     if case>0 and not 0x4c8<=off<=0x528:put(u,base+off+0x20,[0,-1,1,7,0x80000000,0xffffffff,999][(case+index+record)%7])
     if 0x4c8<=off<=0x528:u.mem_write(base+off+29,bytes([(case+record)%8]))
     put(u,base+off+0x18,0x100+index*17+record)
    original.string(base+0x268,[b'',b'Crypt',b'A'*31][(case+record)%3])
    original.string(base+0x2d0,[b'',b'Warrior',b'Rogue',b'B'*40][(case+record)%4])
    for index,off in enumerate([0x660,0x664,0x668,0x670,0x674,0x678,0x67c,0x684]):put(u,base+off,case*100+record+index)
    u.mem_write(base+0x66c,bytes([case+record]));put(u,base+0x280,case+record+44)
    if case>1:
     put(u,base+4+55*4,a+0x310);u.mem_write(base+0x108,bytes([case]));u.mem_write(base+0x124,bytes([case+2]));u.mem_write(base+0x125,bytes([case+4]));put(u,base+0x128,case+333)
    if case>3:
     for off in BYTES:
      length=get(u,base+off+0x24);ptr=get(u,base+off+0x20)
      u.mem_write(ptr,bytes(rng.randrange(256) for _ in range(length)))
   if op in [0,1] and case%2==0:u.mem_write(GUARD,bytes(8));put(u,CREATE,0);put(u,DELETE,0)
   if case%2:put(u,b+0x680,LOADING)
   if case%3==1:put(u,a+0x680,LOADING)
   # Defined stack backing with variations; observed reached residues are
   # delivered to native code, rather than assuming the original stack was zero.
   u.mem_write(0x20007000,bytes([case*23])*0x1000)
   original.string(0x10003000,[b'',b'Mage',b'Long name '+b'C'*30,
                              'מכשף'.encode('utf-8'),b'A\0B'][case%5])
   value=[0,-1,1,7,0x80000000][case%5]
   rows.append(original.fixture(op,value))
 return rows
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--library',type=Path);a=p.parse_args()
 a.output.mkdir(parents=True,exist_ok=True);original=Original(a.original);rows=cases(original)
 inputs=a.output/'fixtures.bin';outputs=a.output/'native.bin';exe=a.output/'player_info_record_v1_host.exe';inputs.write_bytes(struct.pack('<I',len(rows))+b''.join(r[0] for r in rows))
 command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror',str(MODULE/'tests/player_info_record_v1_host.cpp')]
 command += [str(a.library)] if a.library else [str(MODULE/x) for x in ['player_info_record_v1.cpp','cnet_player_info_v1.cpp','netstruct_members_v1.cpp','character_level_member.cpp']]
 subprocess.run(command+['-o',str(exe)],check=True);env=os.environ.copy();dlls=[]
 if a.library:
  dlls=sorted(a.library.parent.parent.rglob('*.dll'));env['PATH']=os.pathsep.join([*(str(p.parent) for p in dlls),env['PATH']])
 run=subprocess.run([str(exe),str(inputs),str(outputs)],check=True,env=env,capture_output=True,text=True)
 policies=int(run.stdout.strip());native=outputs.read_bytes();expected=b''.join(r[1] for r in rows)
 if native!=expected:
  differences=[(i,native[i] if i<len(native) else None,expected[i]) for i in range(len(expected)) if i>=len(native) or native[i]!=expected[i]]
  raise AssertionError((len(native),len(expected),differences[:20]))
 report=dict(validation='PASS',cases=len(rows),compared_projection_bytes=len(native),original_words=len(original.words),native_policy_checks=policies,declared_provider_calls=original.providers,implementation='selected library' if a.library else 'direct translation units',scope=original.pins['scope'],shared_serial_wrap_cases=True,borrowed_pointer_aliases_preserved=True,source_sha256={x:hashlib.sha256((MODULE/x).read_bytes()).hexdigest() for x in ['player_info_record_v1.cpp','player_info_record_v1.hpp','cnet_player_info_v1.cpp','netstruct_members_v1.cpp','character_level_member.cpp']})
 if a.library:
  commands=json.loads((a.library.parent.parent/'compile_commands.json').read_text());report['selected_commands']=[c for c in commands if Path(c['file']).name in ['player_info_record_v1.cpp','cnet_player_info_v1.cpp','netstruct_members_v1.cpp','character_level_member.cpp']];assert len(report['selected_commands'])==4
  report['binary_sha256']={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in [exe,*dlls]}
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items() if k not in ['selected_commands','binary_sha256']}))
if __name__=='__main__':main()
