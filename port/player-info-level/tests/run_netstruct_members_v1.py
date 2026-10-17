"""ARM member leaves and owned byte-array lifecycle allocation traces."""
from __future__ import annotations
import argparse,hashlib,json,os,random,struct,subprocess
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_SP,UC_ARM_REG_LR,UC_ARM_REG_PC
from player_locality_v1_original import image,get,put
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/player-info-level';PIN=MODULE/'reference/netstruct-members-v1/original-functions.json'
BASE=0x10001000;ARG=0x10002000;OLD=0x10003000;VALUE=0x10004000;STOP=0x30000000;SERIAL=0xa33530
CTOR={36:0x370550,3:0x370608,30:0x3706c0};BUFFER={36:0x36ffd0,3:0x370258,30:0x3702a8};VTABLE={36:0x963820,3:0x963898,30:0x9638e0}
class Original:
 def __init__(self,path):
  data,self.symbols,_=image(path);self.pins=json.loads(PIN.read_text());self.ranges=[]
  for p in self.pins['functions']:
   a=int(p['elf_address'],0);s=self.symbols[p['original_symbol']]
   assert (s['st_value'],s['st_size'])==(a,p['size']) and hashlib.sha256(data[a:a+p['size']]).hexdigest()==p['sha256']
   self.ranges.append((a,p['size']))
  assert self.symbols['_ZN9NetStruct15s_changeCounterE']['st_value']==SERIAL
  self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);self.u.mem_map(0,len(data));self.u.mem_write(0,data)
  for a,n in [(0x10000000,0x30000),(0x20000000,0x10000),(STOP,0x1000)]:self.u.mem_map(a,n)
  self.words=set();self.u.hook_add(UC_HOOK_CODE,self.hook)
 def hook(self,u,address,size,context):
  if address==STOP:u.emu_stop();return
  a,b,c=[u.reg_read(r) for r in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2]];value=None
  if address==0x30e460:u.mem_write(a,bytes([b&255])*c);value=a
  elif address==0x30e868:u.mem_write(a,bytes(u.mem_read(b,c)));value=a
  elif address==0x30e5e0:
   x=bytes(u.mem_read(a,c)) if c else b'';y=bytes(u.mem_read(b,c)) if c else b'';value=(x>y)-(x<y)
  elif address==0x31056c:
   assert b==2;value=self.heap;self.heap+=0x100;self.ids[value]=self.next
   self.trace.append([1,a,b,self.next]);self.next+=1
  elif address==0x310440:
   self.trace.append([2,self.ids.pop(a),0,0]);value=0
  if value is not None:u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR));return
  assert any(a<=address<a+n for a,n in self.ranges),f'unscoped original {address:#x}'
  self.words.add(address)
 def run(self,op,width,value,serial,header,old,data):
  u=self.u;u.mem_write(BASE,header+bytes(4));u.mem_write(SERIAL,struct.pack('<Q',serial));self.heap=0x10008000;self.ids={OLD:0} if old else {};self.next=1;self.trace=[]
  if old:u.mem_write(OLD,old)
  if data:u.mem_write(VALUE,data)
  if op<3:put(u,BASE+0x20,OLD if old else 0);put(u,BASE+0x24,len(old));put(u,ARG,VALUE if data else 0);put(u,ARG+4,len(data))
  else:put(u,ARG,value)
  function=CTOR[width] if op==0 else 0x36f264 if op==1 else BUFFER[width] if op==2 else [0x36d9f0,0x36da08,0x36da20][op-3]
  u.reg_write(UC_ARM_REG_R0,BASE)
  u.reg_write(UC_ARM_REG_R1,(VALUE if data else 0) if op==2 else ARG);u.reg_write(UC_ARM_REG_R2,len(data))
  u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP)
  u.emu_start(function,STOP+4,count=10000);assert u.reg_read(UC_ARM_REG_PC)==STOP
  result=bytes(u.mem_read(SERIAL,8))+bytes(u.mem_read(BASE,32))
  if op<3:
   pointer=get(u,BASE+0x20);size=get(u,BASE+0x24);result+=struct.pack('<II',size,int(pointer!=0))
   if pointer and size:result+=bytes(u.mem_read(pointer,size))
  else:result+=bytes(u.mem_read(BASE+0x20,4))
  result+=struct.pack('<I',len(self.trace))+b''.join(struct.pack('<4I',*t) for t in self.trace)
  return result
def cases(original):
 rng=random.Random(0x36ddac);rows=[]
 def add(op,width,value,serial,header,old,data):
  expected=original.run(op,width,value,serial,header,old,data)
  fixture=struct.pack('<IIIQ',op,width,value,serial)+header+struct.pack('<I',len(old))+old+struct.pack('<I',len(data))+data
  rows.append((fixture,expected))
 for width in [3,30,36]:
  for op in range(3):
   for old,data in [(b'',b''),(b'',b'abc'),(b'abc',b'abc'),(b'abc',b'abd'),(b'abc',b''),(b'ab',b'abc'),(b'abc',bytes(range(width)))]:
    if op==0 and old:continue
    h=bytearray(rng.randrange(256) for _ in range(36));struct.pack_into('<II',h,0,VTABLE[width],width*8)
    add(op,width,0,0xffffffffffffffff,bytes(h),old,data)
 for op in [3,4,5]:
  for old in [0,1,7,0x7fffffff,0x80000000,0xffffffff]:
   for value in [0,1,7,0x80000000,0xffffffff]:
    h=bytearray(rng.randrange(256) for _ in range(36));struct.pack_into('<II',h,0,[0x963730,0x965858,0x9636d0][op-3],1 if op==5 else 32)
    struct.pack_into('<I',h,32,old);h[29]=old&255
    add(op,0,value,0xffffffff,bytes(h),b'',b'')
 return rows
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--library',type=Path);a=p.parse_args()
 a.output.mkdir(parents=True,exist_ok=True);original=Original(a.original);rows=cases(original)
 inputs=a.output/'fixtures.bin';outputs=a.output/'native.bin';exe=a.output/'netstruct_members_v1_host.exe';inputs.write_bytes(struct.pack('<I',len(rows))+b''.join(r[0] for r in rows))
 command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror',str(MODULE/'tests/netstruct_members_v1_host.cpp')]
 command += [str(a.library)] if a.library else [str(MODULE/x) for x in ['netstruct_members_v1.cpp','character_level_member.cpp']]
 subprocess.run(command+['-o',str(exe)],check=True);env=os.environ.copy()
 if a.library:env['PATH']=os.pathsep.join([*(str(p.parent) for p in a.library.parent.parent.rglob('*.dll')),env['PATH']])
 run=subprocess.run([str(exe),str(inputs),str(outputs)],check=True,env=env,capture_output=True,text=True)
 policies=int(run.stdout.strip());assert policies==6
 native=outputs.read_bytes();expected=b''.join(r[1] for r in rows)
 if native!=expected:
  differences=[(i,native[i] if i<len(native) else None,expected[i]) for i in range(len(expected)) if i>=len(native) or native[i]!=expected[i]]
  raise AssertionError((len(native),len(expected),differences[:20]))
 report=dict(validation='PASS',cases=len(rows),compared_bytes=len(native),original_words=len(original.words),native_policy_checks=policies,implementation='selected library' if a.library else 'direct translation units',scope=original.pins['scope'],source_sha256=hashlib.sha256((MODULE/'netstruct_members_v1.cpp').read_bytes()).hexdigest())
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps(report))
if __name__=='__main__':main()
