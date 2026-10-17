"""Original CNetPlayerInfo lifecycle versus the selected native library."""
from __future__ import annotations
import argparse,hashlib,json,os,random,struct,subprocess
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_SP,UC_ARM_REG_LR,UC_ARM_REG_PC
from player_locality_v1_original import image,get,put
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/player-info-level'
PIN=MODULE/'reference/cnet-player-info-v1/original-functions.json'
BASES=[0x10001000,0x10002000];SIZE=0x284;STOP=0x30000000
OFFSETS=[0x130,0x158,0x180,0x1a8,0x1d0,0x1f8,0x220,0x248]
FUNCTIONS=[0x80f7b4,0x80fc28,0x80f27c,0x4417d8,0x377448,0x378708,0x378708,0x80f618,0x80f6f4]
class Original:
 def __init__(self,path):
  data,self.symbols,_=image(path);self.pins=json.loads(PIN.read_text());self.ranges=[]
  for p in self.pins['functions']:
   a=int(p['elf_address'],0);s=self.symbols[p['original_symbol']]
   assert (s['st_value'],s['st_size'])==(a,p['size'])
   assert hashlib.sha256(data[a:a+p['size']]).hexdigest()==p['sha256']
   self.ranges.append((a,p['size']))
  self.serial=int(self.pins['global_serial_address'],0)
  assert self.symbols[self.pins['global_serial_symbol']]['st_value']==self.serial
  self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);self.u.mem_map(0,len(data));self.u.mem_write(0,data)
  for a,n in [(0x10000000,0x10000),(0x20000000,0x10000),(STOP,0x1000)]:self.u.mem_map(a,n)
  self.words=set();self.providers={};self.heap=0x10005000
  self.u.hook_add(UC_HOOK_CODE,self.hook)
 def text(self,address):
  start=get(self.u,address+0x14);end=get(self.u,address+0x10)
  assert 0<=end-start<=4096,(hex(address),hex(start),hex(end))
  return bytes(self.u.mem_read(start,end-start)) if end>start else b''
 def string(self,address,value):
  if len(value)<16:target=address;self.u.mem_write(address,bytes(16))
  else:target=self.heap;self.heap+=(len(value)+16+15)&~15
  self.u.mem_write(target,value+b'\0');put(self.u,address+0x10,target+len(value));put(self.u,address+0x14,target)
 def hook(self,u,address,size,context):
  if address==STOP:u.emu_stop();return
  regs=[u.reg_read(r) for r in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2]];a,b,c=regs;value=None
  if address==0x30e460:u.mem_write(a,bytes([b&255])*c);value=a
  elif address==0x30e868:u.mem_write(a,bytes(u.mem_read(b,c)));value=a
  elif address==0x30e5e0:
   x=bytes(u.mem_read(a,c)) if c else b'';y=bytes(u.mem_read(b,c)) if c else b'';value=(x>y)-(x<y)
  elif address==0x3116e8:assert b==c;self.string(a,b'');value=a
  elif address==0x32b918:self.string(a,self.text(b));value=a
  elif address==0x31167c:assert b==16;u.mem_write(a,bytes(b));value=a
  elif address==0x3109e0:self.string(a,bytes(u.mem_read(b,c-b)));value=a
  elif address==0x318254:value=a # Declared string retirement; no prefix stores.
  if value is not None:
   self.providers[hex(address)]=self.providers.get(hex(address),0)+1
   u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR));return
  assert any(a<=address<a+n for a,n in self.ranges),f'unscoped original {address:#x}'
  self.words.add(address)
 def call(self,function,receiver,arg=0):
  u=self.u;u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP)
  u.reg_write(UC_ARM_REG_R0,receiver);u.reg_write(UC_ARM_REG_R1,arg)
  u.emu_start(function,STOP+4,count=20000);assert u.reg_read(UC_ARM_REG_PC)==STOP
 def project(self,address,active=True):
  u=self.u;fields=[get(u,address),get(u,address+0x104),u.mem_read(address+0x108,1)[0],u.mem_read(address+0x124,1)[0],u.mem_read(address+0x125,1)[0],get(u,address+0x128),get(u,address+0x280)]
  result=struct.pack('<7I',*fields)+bytes(u.mem_read(address+4,256))
  for i,off in enumerate(OFFSETS):result+=bytes(u.mem_read(address+off,32 if i==7 else 36))
  value=self.text(address+0x268) if active else b''
  return result+struct.pack('<I',len(value))+value
 def prepare(self,residue,serial):
  self.heap=0x10005000;self.u.mem_write(self.serial,struct.pack('<Q',serial))
  for base in BASES:self.u.mem_write(base,residue);self.call(FUNCTIONS[0],base)
 def capture(self,op):
  u=self.u;a,b=BASES;source=bytes(u.mem_read(a,SIZE));dest=bytes(u.mem_read(b,SIZE));sa=self.text(a+0x268);sb=self.text(b+0x268)
  before=bytes(u.mem_read(self.serial,8));self.call(FUNCTIONS[op],b,b if op==6 else a)
  expected=bytes(u.mem_read(self.serial,8))+self.project(a)+self.project(b,op not in [7,8])
  return struct.pack('<I',op)+before+source+dest+struct.pack('<I',len(sa))+sa+struct.pack('<I',len(sb))+sb,expected
def cases(original):
 rng=random.Random(0x80f7b4);rows=[]
 for case in range(8):
  residue=bytes(rng.randrange(256) for _ in range(SIZE))
  serial=[0,9,0xffffffff,0xffffffffffffffff][case%4]
  for op in range(9):
   original.prepare(residue,serial)
   u=original.u;a,b=BASES
   for record,base in enumerate(BASES):
    for i,off in enumerate(OFFSETS[:7]):
     if case>0:put(u,base+off+0x20,[0,-1,1,7,0x80000000,0xffffffff,999][(case+i+record)%7])
     put(u,base+off+0x18,0x100+i*17+record)
    original.string(base+0x268,[b'',b'Warrior',b'Crypt',b'A'*31][(case+record)%4])
    put(u,base+0x280,case+record+44)
    if case>1:
     put(u,base+4+55*4,a+OFFSETS[2]);u.mem_write(base+0x108,bytes([case]));u.mem_write(base+0x124,bytes([case+2]));u.mem_write(base+0x125,bytes([case+4]));put(u,base+0x128,case+333)
   rows.append(original.capture(op))
 return rows
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--library',type=Path);a=p.parse_args()
 a.output.mkdir(parents=True,exist_ok=True);original=Original(a.original);rows=cases(original)
 inputs=a.output/'fixtures.bin';outputs=a.output/'native.bin';exe=a.output/'cnet_player_info_v1_host.exe'
 inputs.write_bytes(struct.pack('<I',len(rows))+b''.join(r[0] for r in rows))
 command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror',str(MODULE/'tests/cnet_player_info_v1_host.cpp')]
 command += [str(a.library)] if a.library else [str(MODULE/x) for x in ['cnet_player_info_v1.cpp','netstruct_members_v1.cpp','character_level_member.cpp']]
 subprocess.run(command+['-o',str(exe)],check=True)
 env=os.environ.copy();dlls=[]
 if a.library:
  dlls=sorted(a.library.parent.parent.rglob('*.dll'));env['PATH']=os.pathsep.join([*(str(p.parent) for p in dlls),env['PATH']])
 subprocess.run([str(exe),str(inputs),str(outputs)],check=True,env=env)
 expected=b''.join(r[1] for r in rows);native=outputs.read_bytes()
 if native!=expected:
  differences=[(i,native[i] if i<len(native) else None,expected[i]) for i in range(len(expected)) if i>=len(native) or native[i]!=expected[i]]
  raise AssertionError((len(native),len(expected),differences[:20]))
 report=dict(validation='PASS',cases=len(rows),compared_projection_bytes=len(native),original_words=len(original.words),declared_provider_calls=original.providers,implementation='selected library' if a.library else 'direct translation units',scope=original.pins['scope'],shared_serial_wrap_cases=True,borrowed_pointer_aliases_preserved=True,source_sha256={x:hashlib.sha256((MODULE/x).read_bytes()).hexdigest() for x in ['cnet_player_info_v1.cpp','netstruct_members_v1.cpp','character_level_member.cpp']})
 if a.library:
  commands=json.loads((a.library.parent.parent/'compile_commands.json').read_text())
  report['selected_commands']=[c for c in commands if Path(c['file']).name in ['cnet_player_info_v1.cpp','netstruct_members_v1.cpp','character_level_member.cpp']]
  assert len(report['selected_commands'])==3
  report['binary_sha256']={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in [exe,*dlls]}
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items() if k not in ['selected_commands','binary_sha256']}))
if __name__=='__main__':main()
