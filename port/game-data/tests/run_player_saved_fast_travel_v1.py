"""Whole FTVL452B versus original ARM, borrowing six canonical Save words.
Original reverse-bit parsing, low/high publication, invalid-bit throw prefixes
and whole-reader early exit for oversized strings execute. The string primitive
and temporary storage lifetime are explicit observing providers. Bad stream
lengths/terminators, truncation, span limits, allocations and alias guards are
separate native policies. C1 six-word zero-store spans are pinned separately;
the complete constructor, native SG_Load4 and live gameplay are not claimed.
"""
from __future__ import annotations
import argparse,hashlib,itertools,json,os,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_SP,UC_ARM_REG_LR,UC_ARM_REG_PC
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put
PIN=MODULE/'reference/player-saved-fast-travel-v1/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
ENTRY=0x469b18;SIZE=452;SAVE=0x10001000;STREAM=0x10003000;TEXT=0x10008000;STOP=0x30000000
def word(value):return struct.pack('<I',value&0xffffffff)
def payload(strings):return b''.join(word(len(value)+1)+value+b'\0' for value in strings)
def evidence(data,names):
 symbol='_ZN14PlayerSavegame20__LoadFastTravelListEP11IStreamBasePv';assert(names[symbol]['st_value'],names[symbol]['st_size'])==(ENTRY,SIZE)
 spans=[dict(elf_address=hex(a),size=n,sha256=hashlib.sha256(data[a:a+n]).hexdigest()) for a,n in [(0x465b78,32),(0x465640,32)]]
 return dict(original_sha256=SHA,functions=[dict(original_symbol=symbol,elf_address=hex(ENTRY),size=SIZE,sha256=hashlib.sha256(data[ENTRY:ENTRY+SIZE]).hexdigest())],constructor_zero_spans=spans,scope=__doc__)
def cases():
 rows=[];values=[b'',b'0',b'1',b'01',b'10',b'1'*31,b'1'*32,b'1'*33,b'1'*63,b'1'*64,b'1'+b'0'*63,b'0'*63+b'1',b'10'*32,b'1'*65,b'X',b'101X01',b'\0',b'11\x001',b'1'*63+b'X',b'X'+b'1'*63]
 for d,value in itertools.product(range(3),values):
  group=[b'1',b'10',b'11'];group[d]=value;rows.append(payload(group))
 for group in itertools.product([b'',b'0',b'1'*64,b'1'*65,b'X'],repeat=3):rows.append(payload(group))
 for bit in range(64):rows.append(payload([b'0'*(63-bit)+b'1'+b'0'*bit,b'',b'']))
 return rows
class Original:
 def __init__(self,path):
  assert hashlib.sha256(path.read_bytes()).hexdigest()==SHA;data,names,_=image(path);self.pins=evidence(data,names);assert self.pins==json.loads(PIN.read_text());self.words=set()
  self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);self.u.mem_map(0,len(data));self.u.mem_write(0,data)
  for base,size in [(0x10000000,0x20000),(0x20000000,0x10000),(STOP,0x1000)]:self.u.mem_map(base,size)
  self.u.hook_add(UC_HOOK_CODE,self.code);self.u.hook_add(UC_HOOK_MEM_READ,self.read);self.u.hook_add(UC_HOOK_MEM_WRITE,self.write)
 def ret(self):u=self.u;u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def primitive(self,length):self.read_calls+=1;assert self.cursor+length<=len(self.payload);raw=self.payload[self.cursor:self.cursor+length];self.cursor+=length;return raw
 def code(self,u,address,size,context):
  if address==STOP:u.emu_stop();return
  r0=u.reg_read(UC_ARM_REG_R0);r1=u.reg_read(UC_ARM_REG_R1)
  if address==0x461da8:
   assert r0==STREAM;self.strings+=1;length=struct.unpack('<I',self.primitive(4))[0];raw=self.primitive(length);assert raw[-1:]==b'\0';self.length=length-1;u.mem_write(TEXT,raw);put(u,r1+0x10,TEXT+self.length);put(u,r1+0x14,TEXT);self.ret();return
  if address==0x31167c:put(u,r0+0x10,TEXT);put(u,r0+0x14,TEXT);self.ret();return
  if address==0x3139ac:self.ret();return
  if address==0x708f50:self.failed=True;u.emu_stop();return
  assert ENTRY<=address<0x469ccc,f'unscoped original {address:#x}';self.words.add(address)
  if address==0x469cbc:self.decision=2
  if address==0x469c80 and not self.decision:self.decision=1
 def read(self,u,access,address,size,value,context):
  if size==1 and TEXT<=address<TEXT+self.length:self.characters+=1
 def write(self,u,access,address,size,value,context):
  if SAVE+0x17c<=address<SAVE+0x194:self.stores+=1
  if address in [SAVE+0x180,SAVE+0x188,SAVE+0x190]:self.completed+=1
 def execute(self,payload):
  self.payload=payload;self.cursor=self.read_calls=self.strings=self.characters=self.stores=self.completed=self.length=self.decision=0;self.failed=False;u=self.u;u.mem_write(SAVE,bytes(0x198))
  for d in range(3):put(u,SAVE+0x17c+d*8,0x10203040+d);put(u,SAVE+0x180+d*8,0x50607080+d)
  u.reg_write(UC_ARM_REG_R0,STREAM);u.reg_write(UC_ARM_REG_R1,SAVE);u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP);u.emu_start(ENTRY,STOP+4,count=10000);assert self.failed or u.reg_read(UC_ARM_REG_PC)==STOP
  return struct.pack('<8I',int(self.failed),self.cursor,self.read_calls,self.strings,self.characters,self.stores,self.completed,self.decision)+bytes(u.mem_read(SAVE+0x17c,24))
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--library',type=Path);p.add_argument('--write-pins',action='store_true');a=p.parse_args()
 if a.write_pins:
  assert hashlib.sha256(a.original.read_bytes()).hexdigest()==SHA;data,names,_=image(a.original);PIN.parent.mkdir(parents=True,exist_ok=True);PIN.write_text(json.dumps(evidence(data,names),indent=2)+'\n');return
 a.output.mkdir(parents=True,exist_ok=True);original=Original(a.original);rows=cases();expected=b''.join(original.execute(row) for row in rows);inputs=a.output/'fixtures.bin';outputs=a.output/'native.bin';exe=a.output/'player_saved_fast_travel_v1_host.exe';inputs.write_bytes(word(len(rows))+b''.join(word(len(row))+row for row in rows))
 sources=['player_saved_fast_travel_v1.cpp'];command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror',str(MODULE/'tests/player_saved_fast_travel_v1_host.cpp')];command += [str(a.library)] if a.library else [str(MODULE/x) for x in [*sources,'player_save_level_states_v1.cpp']];subprocess.run(command+['-o',str(exe)],check=True);env=os.environ.copy();dlls=[]
 if a.library:dlls=sorted(a.library.parent.parent.rglob('*.dll'));env['PATH']=os.pathsep.join([*(str(x.parent) for x in dlls),env['PATH']])
 run=subprocess.run([str(exe),str(inputs),str(outputs)],env=env,check=True,capture_output=True,text=True);native=outputs.read_bytes()
 if native!=expected:
  for i,row in enumerate(rows):
   if native[i*56:(i+1)*56]!=expected[i*56:(i+1)*56]:raise AssertionError({'case':i,'input':row,'native':struct.unpack('<14I',native[i*56:(i+1)*56]),'original':struct.unpack('<14I',expected[i*56:(i+1)*56])})
  raise AssertionError((len(native),len(expected)))
 report=dict(validation='PASS',cases=len(rows),original_words=len(original.words),native_policy_checks=int(run.stdout.strip()),compared_projection_bytes=len(native),implementation='selected library' if a.library else 'direct translation unit',native_wired=False,scope=original.pins['scope'],source_sha256={x:hashlib.sha256((MODULE/x).read_bytes()).hexdigest() for x in [*sources,'player_saved_fast_travel_v1.hpp','player_savegame_v1.hpp','tests/player_saved_fast_travel_v1_host.cpp','tests/run_player_saved_fast_travel_v1.py','reference/player-saved-fast-travel-v1/original-functions.json']})
 if a.library:
  build=next(parent for parent in a.library.parents if (parent/'compile_commands.json').is_file());commands=json.loads((build/'compile_commands.json').read_text());paths={(MODULE/x).resolve() for x in sources};report['selected_commands']=[c for c in commands if Path(c['file']).resolve() in paths];assert len(report['selected_commands'])==len(sources);report['binary_sha256']={x.name:hashlib.sha256(x.read_bytes()).hexdigest() for x in [exe,*dlls]}
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_sha256','selected_commands','binary_sha256']}))
if __name__=='__main__':main()
