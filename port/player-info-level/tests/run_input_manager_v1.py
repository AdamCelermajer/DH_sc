"""Compare the selected input library with pinned ARM, including failure prefixes.

The original ELF stays private. External soft-float/libm/clock are explicit
fixtures; all input constructors, getters, updates and Point3D helpers execute.
"""
from __future__ import annotations
import argparse,ctypes,hashlib,json,math,os,random,struct,subprocess
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_SP,UC_ARM_REG_LR,UC_ARM_REG_PC
from player_locality_v1_original import image,signed,put,get
ROOT=Path(__file__).resolve().parents[3]
MODULE=ROOT/'port/player-info-level'
PIN=MODULE/'reference/input-manager-v1/original-functions.json'
SIZE=0x2bc0;BASE=0x10001000;STOP=0x30000000;ORIGIN=0x99f854
FUNCTIONS=[0x34dd18,0x34cc94,0x34cb70,0x34ce88,0x34dc90,0x34c91c,0x34c98c,0x34d124,0x34d744,0x34d74c,0x34d754,0x34da9c,0x34daa8,0x34dbb4,0x34d75c,0x34d7b4,0x34d6fc]
OFFSETS=[0,0x18,0xc44,0xe50,0,0x18,0xc44,0xe50]+[0]*9
VTABLES=['_ZTV11InputDevice','_ZTV8Keyboard','_ZTV5Mouse','_ZTV7Gamepad','_ZTV12InputManager','_ZTV17InputManagerWin32']
def word(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def number(x):return struct.unpack('<f',struct.pack('<I',x))[0]
def f32(x):
 try:return number(word(x))
 except OverflowError:return math.copysign(math.inf,x)
def write(b,offset,value,fmt='I'):struct.pack_into('<'+fmt,b,offset,value)
def normalize(b,symbols):
 b=bytearray(b);tags={symbols[n]['st_value']+8:i for i,n in enumerate(VTABLES)}
 for offset in [0,0x18,0xc44,*[0xe50+0x75c*i for i in range(4)]]:
  value=struct.unpack_from('<I',b,offset)[0]
  if value in tags:write(b,offset,tags[value])
 return bytes(b)
class Original:
 def __init__(self,path):
  data,self.symbols,_=image(path);self.pins=json.loads(PIN.read_text(encoding='utf-8'))
  for p in self.pins['functions']:
   a=int(p['elf_address'],0);s=self.symbols[p['original_symbol']]
   assert (s['st_value'],s['st_size'])==(a,p['size'])
   assert hashlib.sha256(data[a:a+p['size']]).hexdigest()==p['sha256']
  self.ranges=[(int(p['elf_address'],0),p['size']) for p in self.pins['functions']]
  self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);self.u.mem_map(0,len(data));self.u.mem_write(0,data)
  for base,size in [(0x10000000,0x10000),(0x20000000,0x10000),(STOP,0x1000)]:self.u.mem_map(base,size)
  lib=ctypes.CDLL('ucrtbase' if os.name=='nt' else 'libm.so.6')
  self.libm={}
  for name in ['sqrtf','acosf']:
   f=getattr(lib,name);f.argtypes=[ctypes.c_float];f.restype=ctypes.c_float;self.libm[name]=f
  self.words=set();self.u.hook_add(UC_HOOK_CODE,self.hook)
 def hook(self,u,address,size,context):
  if address==STOP:u.emu_stop();return
  a=number(u.reg_read(UC_ARM_REG_R0));b=number(u.reg_read(UC_ARM_REG_R1));value=None
  if address==0x30e730:
   self.calls+=1
   if self.calls==self.fail or self.fail==0xffffffff:
    self.failed=True;u.emu_stop();return
   assert self.calls<=len(self.ticks),'clock fixture exhausted'
   value=self.ticks[self.calls-1]&0xffffffff
  elif address==0x30e124:value=word(self.libm['sqrtf'](a))
  elif address==0x30e3dc:value=word(self.libm['acosf'](a))
  elif address==0x30eba4:value=word(f32(a+b))
  elif address==0x30e3ac:value=word(f32(a-b))
  elif address==0x30ed6c:value=word(f32(a*b))
  elif address==0x30ec94:
   if b==0:value=word(math.nan if a==0 else math.copysign(math.inf,a*math.copysign(1,b)))
   else:value=word(f32(a/b))
  elif address==0x30e2f8:value=int(a>b)
  elif address==0x30e4b4:value=int(a>=b)
  elif address==0x30e70c:value=int(a<b)
  if value is not None:
   u.reg_write(UC_ARM_REG_R0,value);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR));return
  assert any(a<=address<a+n for a,n in self.ranges),f'unscoped original execution {address:#x}'
  self.words.add(address)
 def run(self,op,index,state,ticks,fail=0,origin=(0.,0.,0.)):
  u=self.u;u.mem_write(BASE,bytes(state));u.mem_write(ORIGIN,struct.pack('<3f',*origin))
  self.ticks=ticks;self.fail=fail;self.calls=0;self.failed=False
  for r in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]:u.reg_write(r,0)
  u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP)
  offset=OFFSETS[op]+(index*0x75c if op in [3,7] else 0)
  u.reg_write(UC_ARM_REG_R0,BASE+offset);u.reg_write(UC_ARM_REG_R1,index&0xffffffff)
  if op==16:
   for reg,off in [(UC_ARM_REG_R1,4),(UC_ARM_REG_R2,8),(UC_ARM_REG_R3,12)]:u.reg_write(reg,get(u,BASE+off))
  u.emu_start(FUNCTIONS[op],STOP+4,count=200000)
  assert self.failed or u.reg_read(UC_ARM_REG_PC)==STOP,hex(u.reg_read(UC_ARM_REG_PC))
  result=u.reg_read(UC_ARM_REG_R0)
  if op<8 or op==16:result=0
  elif op in [11,12,13,15]:result=result-BASE if result else 0xffffffff
  return normalize(u.mem_read(BASE,SIZE),self.symbols),result,0 if fail==0xffffffff else self.calls,3 if self.failed and fail!=0xffffffff else 2 if self.failed else 0
def cases(original):
 rng=random.Random(0x34d124);rows=[]
 def add(op,index,state,ticks=None,fail=0,origin=(0.,0.,0.)):
  ticks=ticks or [1000+i*17 for i in range(32)]
  expected=original.run(op,index,state,ticks,fail,origin)
  rows.append((op,index,normalize(state,original.symbols),ticks,fail,origin,expected))
 residue=bytes(rng.randrange(256) for _ in range(SIZE))
 for origin in [(0.,0.,0.),(.25,-.5,1.)]:
  for op in [0,1,2,3,16]:add(op,2,residue,origin=origin)
 # Obtain source-created storage, retaining padding and using ARM dispatch.
 original.run(0,0,residue,[1000]*32);initial=bytes(original.u.mem_read(BASE,SIZE))
 for op in range(8,17):
  for index in ([0,1,2,3] if op==13 else [-99,0,7]):add(op,index,initial)
 for mask in range(16):
  s=bytearray(initial)
  for i in range(4):s[0xe50+i*0x75c+0x758]=7 if mask>>i&1 else 0
  add(14,0,s);add(15,0,s)
 channels=[0x18+12+i*32 for i in range(97)]+[0xc44+12+i*32 for i in range(16)]+[0xe50+j*0x75c+12+i*32 for j in range(4) for i in range(54)]
 for iteration in range(44):
  s=bytearray(initial)
  for off in channels:
   vals=[rng.choice([-1.,-.2,0.,.4,.65,.95,1.,2.]),-1.,1.,rng.choice([-.5,0.,1.,2.]),.123,-1.,1.]
   for i,v in enumerate(vals):write(s,off+4*i,v,'f')
   s[off+28]=rng.randrange(256)
  for op in [5,6,7,4]:add(op,iteration%4,s)
 # Cover source stick deadzone, timestamps, repeat angle and signed wrapping.
 for x,y in [(0.,0.),(.324999,0.),(.325,0.),(.475,0.),(.5,.5),(-.5,0.),(.9,-.1)]:
  for elapsed in [199,200,201,399,400,401,-1]:
   s=bytearray(initial);p=0xe50
   for i in range(4):
    write(s,p+0x5ac+64*i,x,'f');write(s,p+0x5ac+64*i+32,y,'f')
    write(s,p+0x6fc+4*i,100,'i');write(s,p+0x73c+4*i,100,'i')
    struct.pack_into('<3f',s,p+0x6cc+12*i,1.,0.,0.)
    struct.pack_into('<3f',s,p+0x70c+12*i,*[(1.,0.,0.),(0.,1.,0.),(-1.,0.,0.),(1.1,0.,0.)][i])
   add(7,0,s,ticks=[100+elapsed]*32)
 for op in [7,4]:
  for fail in [1,2,3,4,8,16,0xffffffff]:add(op,0,initial,fail=fail)
 for count in [-2,0,1,2,4]:
  s=bytearray(initial);write(s,12,count,'i');add(14,0,s);add(15,0,s);add(4,0,s)
 s=bytearray(initial)
 for i in range(4):write(s,0xe50+0x6fc+i*4,0x7ffffff0,'i')
 add(7,0,s,ticks=[-0x7fffff00]*32)
 return rows
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--library',type=Path);p.add_argument('--compiler',default='g++');a=p.parse_args()
 a.output.mkdir(parents=True,exist_ok=True);original=Original(a.original);rows=cases(original)
 source=MODULE/'tests/input_manager_v1_host.cpp';exe=a.output/'input_manager_v1_host.exe'
 command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-fno-fast-math','-ffp-contract=off',str(source)]
 command += [str(a.library)] if a.library else [str(MODULE/'input_manager_v1.cpp')]
 command += ['-o',str(exe)];subprocess.run(command,check=True)
 fixture=a.output/'fixtures.bin';result=a.output/'native.bin'
 with fixture.open('wb') as f:
  f.write(struct.pack('<I',len(rows)))
  for op,index,state,ticks,fail,origin,_ in rows:
   f.write(struct.pack('<IiII3f',op,index,len(ticks),fail,*origin));f.write(state);f.write(struct.pack('<'+'i'*len(ticks),*ticks))
 env=os.environ.copy()
 if a.library:
  dlls=sorted(a.library.parent.parent.rglob('*.dll'))
  env['PATH']=os.pathsep.join([*(str(p.parent) for p in dlls),env['PATH']])
 subprocess.run([str(exe),str(fixture),str(result)],check=True,env=env)
 raw=result.read_bytes();assert len(raw)==len(rows)*(SIZE+20)
 clock_reads=0;prefixes=0
 for i,row in enumerate(rows):
  meta=struct.unpack_from('<5I',raw,i*(SIZE+20));state=raw[i*(SIZE+20)+20:(i+1)*(SIZE+20)]
  expected,value,calls,status=row[-1]
  if state!=expected:
   offsets=[j for j,(x,y) in enumerate(zip(state,expected)) if x!=y]
   raise AssertionError(f'case {i} op {row[0]} state mismatch {[(hex(j),state[j],expected[j]) for j in offsets[:20]]}')
  assert (meta[0],meta[1],meta[4])==(status,calls,value),(i,row[0],meta,status,calls,value)
  clock_reads+=calls;prefixes+=status!=0
 report=dict(validation='PASS',cases=len(rows),compared_bytes=len(rows)*SIZE,distinct_original_words=len(original.words),clock_reads=clock_reads,failure_prefixes=prefixes,original_sha256=original.pins['original_sha256'],implementation='selected library' if a.library else 'direct translation unit',scope=original.pins['scope'],source_sha256=hashlib.sha256((MODULE/'input_manager_v1.cpp').read_bytes()).hexdigest())
 if a.library:
  commands=json.loads((a.library.parent.parent/'compile_commands.json').read_text(encoding='utf-8'))
  selected=[c for c in commands if Path(c['file']).resolve()==(MODULE/'input_manager_v1.cpp').resolve()]
  assert len(selected)==1,'Input source must be selected exactly once'
  report['selected_command']=selected[0]
  report['binary_sha256']={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in [exe,*dlls]}
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps(report))
if __name__=='__main__':main()
