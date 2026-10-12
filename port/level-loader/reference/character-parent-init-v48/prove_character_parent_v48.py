from pathlib import Path
import sys,struct,json,hashlib
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE,UC_HOOK_MEM_WRITE
base=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');ref=base/'reference/character-parent-init-v48';elf=root/'.local-inputs/libDungeonHunter2.so'
assert hashlib.sha256(elf.read_bytes()).hexdigest()=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
class Oracle(Cpu):
 def text(self,p):
  out=bytearray()
  while self.uc.mem_read(p,1)!=b'\0':out+=self.uc.mem_read(p,1);p+=1
  return bytes(out)
 def external(self,uc,a,size,u):
  name=self.imports.get(a)
  if name in ['__aeabi_fcmplt','__aeabi_fmul']:
   left=struct.unpack('<f',struct.pack('<I',self.reg(0)))[0];right=struct.unpack('<f',struct.pack('<I',self.reg(1)))[0]
   value=int(left<right) if name=='__aeabi_fcmplt' else struct.unpack('<I',struct.pack('<f',left*right))[0]
   self.put(0,value);uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif name in ['strcmp','strstr']:
   x,y=self.reg(0),self.reg(1);left,right=self.text(x),self.text(y)
   if name=='strcmp':value=0 if left==right else -1 if left<right else 1
   else:
    at=left.find(right);value=x+at if at>=0 else 0
   self.put(0,value);uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,a,size,u)
c=Oracle(elf,False,{'functions':[]});actor=c.data+0x1000;heap=c.data+0x50000;allocs=[];writes=[];type_value=0;isplayer_reads=[]
def word(a,v):c.uc.mem_write(a,struct.pack('<I',v&0xffffffff))
def read(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def allocate(n):
 global heap
 p=heap;heap+=(n+15)&~15;c.uc.mem_write(p,bytes(n));return p
def hook(uc,a,size,u):
 if a==0x31167c:
  p=c.reg(0);n=c.reg(1);buf=allocate(n);word(p+0x10,buf);word(p+0x14,buf);allocs.append({'kind':'CString allocation leaf','receiver_offset':hex(p-actor),'bytes':n});ret()
 elif a==0x310570:
  n=c.reg(0);p=allocate(n);allocs.append({'kind':'operator new leaf','bytes':n});ret(p)
 elif a==0x3a3054:ret(type_value) # explicit type-domain provider
 elif a==0x3a4a14:isplayer_reads.append({'pc':hex(a),'source_offset':'0x44','name_begin':hex(read(actor+0x44)),'archetype_begin':hex(read(actor+0x5c))})
def memwrite(uc,access,a,size,v,u):
 if a-actor in [0x28,0x10c,0xec,0xa8,0xac,0xcc,0xd0,0x370,0x2d8]:writes.append({'pc':hex(c.uc.reg_read(c.pc)),'offset':hex(a-actor),'bytes':size,'value':hex(v)})
c.uc.hook_add(UC_HOOK_CODE,hook);c.uc.hook_add(UC_HOOK_MEM_WRITE,memwrite)
ctors=[]
for ctor in [0x33f15c,0x33f310]:
 for poison in [0,0xa5,0x7f]:
  c.uc.mem_write(actor,bytes([poison])*0x1f90);allocs.clear();writes.clear();c.invoke(ctor,[actor,0])
  fields={'checkpoint28':c.uc.mem_read(actor+0x28,1)[0],'optimization10c':c.uc.mem_read(actor+0x10c,1)[0],'difficulty_ec':struct.unpack('<i',c.uc.mem_read(actor+0xec,4))[0],'condition8c_compiled':read(actor+0xa8),'condition8c_tested':c.uc.mem_read(actor+0xac,1)[0],'conditionb0_compiled':read(actor+0xcc),'conditionb0_tested':c.uc.mem_read(actor+0xd0,1)[0]}
  assert fields=={'checkpoint28':0,'optimization10c':0,'difficulty_ec':-1,'condition8c_compiled':0,'condition8c_tested':0,'conditionb0_compiled':0,'conditionb0_tested':0},fields
  assert read(actor+0x44)==read(actor+0x40) and read(actor+0x5c)==read(actor+0x58) and read(actor+0x44)!=read(actor+0x5c)
  ctors.append({'constructor':hex(ctor),'source_GO_id':0,'poison':poison,'fields':fields,'writes':list(writes),'allocation_leaves':list(allocs),'name_CString30_begin44':read(actor+0x44),'archetype_CString48_begin5c':read(actor+0x5c)})
prefix=[]
for poison in [0,0xa5,0x7f]:
 c.uc.mem_write(actor+0x370,bytes([poison])*4);c.put(4,actor);c.put(7,0xffffffff);writes.clear();c.uc.emu_start(0x38c5b0,0x38c5b8,count=3);assert c.uc.reg_read(c.pc)==0x38c5b8
 assert bytes(c.uc.mem_read(actor+0x370,4))==b'\xff\xff'+bytes([poison])*2
 prefix.append({'poison':poison,'idle_sound370':-1,'upper_bytes_preserved':True,'writes':list(writes),'source_register_authority':'r4=this38c3a4; r7=-1 established38c48c; original emits source halfword only'})
def string(p,text):
 b=text.encode()+b'\0';buf=allocate(len(b));c.uc.mem_write(buf,b);word(p+0x10,buf+len(b)-1);word(p+0x14,buf)
player_cases=[]
for type_value in [0,1,2,3]:
 for name,archetype in [('PlayerCharacterKnight','NPC'),('NPC','PlayerCharacterKnight'),('xPlayerCharacter','PlayerCharacter'),('PlayerCharacter',''),('','PlayerCharacter')]:
  string(actor+0x30,name);string(actor+0x48,archetype);isplayer_reads.clear();c.invoke(0x3a49f0,[actor]);expected=int(type_value==1 if type_value else name.startswith('PlayerCharacter'));assert c.reg(0)==expected
  player_cases.append({'type':type_value,'name30':name,'archetype48':archetype,'result':bool(expected),'read_field44':list(isplayer_reads)})
report={'schema':'dh2-character-parent-init-original-v48','validation':'PASS','original_elf_sha256':hashlib.sha256(elf.read_bytes()).hexdigest(),'whole_ObjectBase_C1_C2_cases':ctors,'original_GameObject_C2_idle_halfword_prefix_cases':prefix,'whole_IsPlayer_name_vs_archetype_cases':player_cases,'correction':'Original IsPlayer type0 uses name CString30 buffer44, not archetype CString48 buffer5c. V41 labels and V42 actual field borrow were wrong; nonzero-type branch unaffected.','scope':'Whole original ObjectBase C1/C2 and ConditionData C1 execute; only CString allocation and operator-new allocation leaves supplied. Whole original IsPlayer body executes with explicit type-domain provider and exact strstr leaf semantics. GameObject idle370 proof is exact unconditional prefix with documented original register authority, not whole GameObject/Character constructor.'}
(ref/'character-parent-init-original-v48.json').write_text(json.dumps(report,indent=2)+'\n');(ref/'prove_character_parent_v48.py').write_bytes(Path(__file__).read_bytes());print('PASS whole ObjectBase6cases; missing constructor fields7; exact idle halfword3poisons; IsPlayer NAME20cases')
