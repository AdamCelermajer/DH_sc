"""Whole Character::InitFinal caller; reached game services remain providers.
Real original branch/field/virtual instructions execute, including the1395
latch, fresh visual publication and194-byte truncation. GameObject InitFinal,
classification, local record, position/light/CharAI/property/Save bodies are
declared observing providers, not new reconstructed dependency bodies.
"""
from __future__ import annotations
import argparse,hashlib,json,os,random,struct,subprocess,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_PC,UC_ARM_REG_LR,UC_ARM_REG_SP
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/level-world'
PIN=MODULE/'reference/character-init-final-v1/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
ENTRY=0x3b4978;SIZE=588;STOP=0x30000000;C=0x10001000;V1=0x10004000;V2=0x10004100;P=0x10005000;L=0x10006000;TARGET=0x10008000
PLAYER=0x10009000;AI_FINAL=PLAYER+0x10
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def evidence(path):
 assert sha(path)==SHA
 with path.open('rb') as f:
  elf=ELFFile(f);s=next(s for s in elf.get_section_by_name('.symtab').iter_symbols() if s.name=='_ZN9Character9InitFinalEv');assert(s['st_value'],s['st_size'])==(ENTRY,SIZE)
  loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD'];segment=next(s for s in loads if s['p_vaddr']<=ENTRY<ENTRY+SIZE<=s['p_vaddr']+s['p_filesz']);f.seek(segment['p_offset']+ENTRY-segment['p_vaddr']);raw=f.read(SIZE)
  def read(a,n):
   segment=next(s for s in loads if s['p_vaddr']<=a<a+n<=s['p_vaddr']+s['p_filesz']);f.seek(segment['p_offset']+a-segment['p_vaddr']);return f.read(n)
  literals=[]
  for a,text in [(0x8c3e30,'MonsterLight'),(0x8c3e40,'SceneLight')]:
   b=read(a,len(text)+1);assert b==text.encode()+b'\0';literals.append(dict(elf_address=hex(a),text=text,sha256=hashlib.sha256(b).hexdigest()))
 return dict(original_sha256=SHA,functions=[dict(original_symbol=s.name,elf_address=hex(ENTRY),size=SIZE,sha256=hashlib.sha256(raw).hexdigest())],literals=literals,scope=__doc__)
def image(path):
 raw=path.read_bytes();assert hashlib.sha256(raw).hexdigest()==SHA
 with path.open('rb') as f:
  e=ELFFile(f);segments=[s for s in e.iter_segments() if s['p_type']=='PT_LOAD'];symbols=list(e.get_section_by_name('.dynsym').iter_symbols());names={s.name:s for s in symbols}
  length=(max(s['p_vaddr']+s['p_memsz'] for s in segments)+4095)&~4095;data=bytearray(length)
  for s in segments:data[s['p_vaddr']:s['p_vaddr']+s['p_filesz']]=raw[s['p_offset']:s['p_offset']+s['p_filesz']]
  for section in e.iter_sections():
   if section['sh_type'] not in ['SHT_REL','SHT_RELA']:continue
   for rel in section.iter_relocations():
    target=rel['r_offset'];kind=rel['r_info_type'];symbol=symbols[rel['r_info_sym']]
    if kind in [21,22]:struct.pack_into('<I',data,target,symbol['st_value'])
    elif kind==2:struct.pack_into('<I',data,target,(struct.unpack_from('<I',data,target)[0]+symbol['st_value'])&0xffffffff)
 return bytes(data),names
def rows():
 result=[]
 # Every ordinary branch, both fresh IsPlayer outcomes and the Faerie/Follower
 # short circuit, null/present local Character, visual and local-player tail.
 for latch in [0,1,7]:
  for faerie,follower in [(0,0),(0,1),(1,0),(1,1)]:
   for local_character in [0,1]:
    for visual in [0,1]:
     for first,second,local in [(0,0,0),(0,1,1),(1,0,1),(1,1,0),(1,1,1)]:result.append([latch,100,17,faerie,follower,local_character,visual,first,second,local,257,0])
 for threshold,roll in [(-2147483648,-2147483648),(-1,0),(0,-1),(0,0),(1,0),(100,99),(100,100),(2147483647,2147483646)]:result.append([0,threshold,roll,0,0,0,1,1,1,1,-1,0])
 for property in [-2147483648,-1,0,1,255,256,257,2147483647]:result.append([0,100,17,1,0,1,1,1,1,1,property,0])
 for mutation in [1,4,8,1|4|8]:result.append([0,100,17,1,0,1,1,1,1,1,7,mutation])
 return result
def execute(data,row,seen):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
 for base,size in [(0x10000000,0x10000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
 def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def reg(n):return u.reg_read([UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2][n])
 def ret(value=0):u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def text(a):return bytes(u.mem_read(a,64)).split(b'\0',1)[0].decode()
 latch,threshold,roll,faerie,follower,localchar,visual,first,second,local,property,mutation=row
 u.mem_write(C+0x1395,bytes([latch]));put(C+0x274,threshold);put(C+0x2d8,V1 if visual else 0);u.mem_write(C+0x3a8,b'\x63')
 put(C,C+0x2000);put(C+0x2028,PLAYER);put(C+0x3c8,C+0x2100);put(C+0x2110,AI_FINAL)
 put(V1+0x40,77);put(V2+0x40,88);put(P+0x660,L if localchar else 0)
 u.mem_write(TARGET,struct.pack('<3f',10.0,-8.5,3.125));position=[0,0,0];trace=[];players=0;strings={};guard_checks=0
 # Application is a static object stored through the original GOT relocation.
 app=word(0x994a98+word(0x3b4bb8));manager=0x10007000;put(app+0x40,manager)
 scene=0x10007300;scene2=0x10007400;put(app+0x10,scene);put(scene+0x1c,scene2)
 external={0x38bd64:0,0x38cd48:1,0x3a3094:2,0x3a307c:3,0x36e478:4,0x393ae4:5,0x3935dc:6,0x393db4:7,PLAYER:8,0x40c3cc:9,AI_FINAL:10,0x36effc:11,0x3d8894:12,0x3e0810:13,0x3df6e0:14,0x3bc4a8:15}
 def hook(machine,address,size,context):
  nonlocal players,guard_checks,position
  if address==STOP:machine.emu_stop();return
  if ENTRY<=address<ENTRY+SIZE:
   assert address<0x3b4bb0,'literal data executed';seen.add(address)
  if address in external:
   op=external[address];trace.append(op)
   if op==0:
    assert reg(0)==C
    if mutation&1:put(C+0x274,31)
    ret(roll)
   elif op==1:
    assert reg(0)==C
    if mutation&8:u.mem_write(C+0x1395,b'\0')
    ret(0x87654321)
   elif op==2:assert reg(0)==C;ret(faerie)
   elif op==3:assert reg(0)==C;ret(follower)
   elif op==4:assert (reg(0),reg(1),reg(2))==(manager,0,1);ret(P)
   elif op==5:assert reg(0)==L;u.mem_write(reg(1),struct.pack('<3f',-2.5,4.125,-0.0));ret(0x87654321)
   elif op==6:assert reg(0)==L;ret(TARGET)
   elif op==7:assert reg(0)==C and reg(2)==1;position=list(struct.unpack('<3I',u.mem_read(reg(1),12)));ret(0x87654321)
   elif op==8:assert reg(0)==C;ret(first if players==0 else second);players+=1
   elif op==9:
    assert reg(0)==scene2+0x294 and strings[reg(1)]==('SceneLight' if first else 'MonsterLight')
    if mutation&4:put(C+0x2d8,V2)
    ret(42)
   elif op==10:assert reg(0)==C+0x3c8;ret(0x87654321)
   elif op==11:assert (reg(0),reg(1))==(manager,C);ret(local)
   elif op==12:assert reg(0)==C+0x3c8;ret(0x87654321)
   elif op==13:assert (reg(0),reg(1))==(C+0x560,1);ret(0x87654321)
   elif op==14:assert(reg(0),reg(1),reg(2))==(C+0x560,0xc2,0);ret(property)
   elif op==15:assert reg(0)==C;ret(0x87654321)
  elif address==0x3140ec:strings[reg(0)]=text(reg(1));ret(reg(0))
  elif address==0x318254:del strings[reg(0)];ret(0x87654321)
  elif address==0x30eba4:
   a,b=struct.unpack('<2f',struct.pack('<2I',reg(0),reg(1)));ret(struct.unpack('<I',struct.pack('<f',a+b))[0])
  elif address==0x30e310:raise AssertionError('source stack-canary failure')
  elif not(ENTRY<=address<0x3b4bb0):raise AssertionError('unmodeled reached body '+hex(address))
 u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_R0,C);u.emu_start(ENTRY,STOP+4,count=10000);assert u.reg_read(UC_ARM_REG_PC)==STOP
 final_threshold=31 if not latch and mutation&1 else threshold
 rejected=not latch and roll>=final_threshold
 writes=0 if latch else 1
 if 9 in trace:writes+=1
 if 14 in trace:writes+=1
 return dict(status=0,decision=1 if latch else 2 if rejected else 3,latch=u.mem_read(C+0x1395,1)[0],threshold=final_threshold,visual=22 if word(C+0x2d8)==V2 else 11 if word(C+0x2d8)==V1 else 0,light=[word(V1+0x40),word(V2+0x40)],byte=u.mem_read(C+0x3a8,1)[0],position=position,calls=len(trace),stores=writes,trace=trace)
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--original-elf',required=True,type=Path);p.add_argument('--compiler',required=True);p.add_argument('--library',required=True,type=Path);p.add_argument('--output',required=True,type=Path);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
 assert evidence(a.original_elf)==json.loads(PIN.read_text());data,names=image(a.original_elf)
 commands=json.loads((a.library.parent.parent/'compile_commands.json').read_text());selected=[r for r in commands if Path(r['file']).name=='character_init_final_v1.cpp'];assert len(selected)==1
 exe=a.output/'init_final_host.exe';subprocess.run([a.compiler,'-std=c++17','-Wall','-Wextra','-Werror',str(MODULE/'tests/character_init_final_v1.cpp'),str(a.library),'-o',str(exe)],check=True)
 env=os.environ.copy();dlls=sorted(a.library.parent.parent.rglob('*.dll'));env['PATH']=os.pathsep.join([*(str(p.parent) for p in dlls),env['PATH']]);host=json.loads(subprocess.check_output([str(exe)],env=env,text=True));assert host['validation']=='PASS'
 seen=set();records=[]
 for row in rows():
  expected=execute(data,row,seen);actual=json.loads(subprocess.check_output([str(exe),*map(str,row)],env=env,text=True));assert expected==actual,(row,expected,actual);records.append(dict(row=row,matched=True))
 files=[MODULE/'character_init_final_v1.hpp',MODULE/'character_init_final_v1.cpp',MODULE/'tests/character_init_final_v1.cpp',Path(__file__).resolve(),PIN]
 report=dict(validation='PASS',host=host,comparisons=len(records),mismatches=0,executed_original_words=len(seen),instruction_addresses=[hex(x) for x in sorted(seen)],selected_commands=selected,source_sha256={p.relative_to(ROOT).as_posix():sha(p) for p in files},binary_sha256={p.name:sha(p) for p in [exe,*dlls]},scope=__doc__,native_wired=False,records=records)
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:report[k] for k in ['validation','host','comparisons','mismatches','executed_original_words','native_wired']}))
if __name__=='__main__':main()
