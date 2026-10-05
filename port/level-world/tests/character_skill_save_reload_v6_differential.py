"""Original SG_ReloadSkills + InitSkills instructions versus SAME native V1 Save.
Array/node allocation, selected Character list and complete Load boundary are
declared fixtures. Maps are really erased; this is not a production Load owner.
"""
from pathlib import Path
import json,sys,struct,hashlib,random
from unicorn import UC_HOOK_CODE
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[3];sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from aggro_differential import Cpu
ELF=ROOT/'.local-inputs/libDungeonHunter2.so';LIB=ROOT/'.local-inputs/libcharacter_skill_application_v6_oracle.so'
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def ret(c,v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
class Native(Cpu):
 def __init__(self,path,arm64,provenance):
  super().__init__(path,arm64,provenance)
  # Actual C++ fixture takes addresses of defined callbacks through GLOB_DAT.
  # The shared minimal loader only relocates JUMP_SLOT/RELATIVE by default.
  with path.open('rb')as stream:
   elf=ELFFile(stream)
   for section in elf.iter_sections():
    if section['sh_type']!='SHT_RELA':continue
    syms=elf.get_section(section['sh_link'])
    for reloc in section.iter_relocations():
     if reloc['r_info_type']not in(1025,257):continue
     symbol=syms.get_symbol(reloc['r_info_sym'])
     if symbol['st_shndx']!='SHN_UNDEF':self.pointer(self.base+reloc['r_offset'],self.symbols[symbol.name]+reloc['r_addend'])
 def external(self,uc,at,size,user):
  name=self.imports.get(at)
  if name in('_Znwm','_Znwj','_Znam','_Znaj'):
   n=self.reg(0);assert n<0x10000;result=self.heap;self.heap+=(max(n,1)+31)&~31;uc.mem_write(result,bytes(max(n,1)));ret(self,result)
  elif name in('_ZdlPv','_ZdlPvm','_ZdaPv','_ZdaPvm'):ret(self)
  elif name=='memmove':
   dst,src,n=[self.reg(i)for i in range(3)];uc.mem_write(dst,bytes(uc.mem_read(src,n)));ret(self,dst)
  else:super().external(uc,at,size,user)
manifest=json.loads((ROOT/'port/level-world/reference/character-skill-combat-v6/original-functions.json').read_text())
assert hashlib.sha256(ELF.read_bytes()).hexdigest()==manifest['original_sha256']
old=Cpu(ELF,False,{'functions':[]});new=Native(LIB,True,{'functions':[]});old.heap=old.data+0x100000;new.heap=new.data+0x100000
save=old.data+0x1000;character=old.data+0x2000;maps=old.data+0x3000;list_object=old.data+0x4000;ids=old.data+0x5000;rows=old.data+0x6000;node=old.data+0x7000
ni=new.data+0x1000;nj=ni+0x100;callback=ni+0x200;no=ni+0x300
current={};trace={'old':[],'new':[]};fixture=0;initializing=False
def word(c,p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def snapshot(native):
 if native:
  saved=new.uc.context_save();stack=new.stack;new.stack=new.uc.reg_read(new.sp)-0x10000
  try:new.invoke('dh2_skill_save_fixture_snapshot_v6',[fixture,no]);data=list(struct.unpack('<64i',new.uc.mem_read(no,256)))
  finally:new.stack=stack;new.uc.context_restore(saved)
  return [data[0],data[2],data[3]],data[4:4+data[1]*3]
 initialized=word(old,save+0x80)!=0;count=word(old,save+0x84);raw=[]
 if initialized:
  for i in range(count):
   p=word(old,save+0x80)+8*i;raw.extend([word(old,p),struct.unpack('<H',old.uc.mem_read(p+4,2))[0],old.uc.mem_read(p+6,1)[0]])
 return [int(initialized),word(old,maps+16),word(old,maps+40)],raw
def observe(native,op,mask=0):
 state,rows_now=snapshot(native);trace['new'if native else 'old'].append([op,mask,state,rows_now]);return 0
def hook_old(uc,at,size,user):
 if at==0x310440 or at==0x708f00:ret(old)
 elif at==0x31056c:
  n=old.reg(0);result=old.heap;old.heap+=(max(n,1)+31)&~31;uc.mem_write(result,bytes(max(n,1)));ret(old,result)
 elif at==0x3bc5fc:
  assert old.reg(0)==character;observe(False,1);ret(old,list_object)
 elif at==0x465430:
  assert old.reg(0)==save and old.reg(1)==8;observe(False,2,8);ret(old)
def hook_new(uc,at,size,user):
 if at==callback:
  assert new.reg(0)==fixture;op=new.reg(1);mask=new.reg(3);observe(True,op,mask);ret(new)
old.uc.hook_add(UC_HOOK_CODE,hook_old);new.uc.hook_add(UC_HOOK_CODE,hook_new);new.uc.mem_write(callback,bytes.fromhex('c0035fd6'))
rng=random.Random(2026100581);records=[]
for i in range(2048):
 old_count=i%13;count=(i//13)%13;old_ids=[100+j for j in range(old_count)];selected=[rng.randrange(1,10000)for _ in range(count)]
 old.heap=old.data+0x100000;new.heap=new.data+0x100000
 old.uc.mem_write(save,bytes(0x200));old.pointer(save+0x10,character);old.pointer(save+0x80,rows);old.pointer(save+0x84,old_count);old.pointer(save+0x88,maps);old.pointer(save+0x8c,maps+48);old.pointer(save+0x90,maps+48)
 for j in range(2):head=maps+j*24;old.uc.mem_write(head,words(0,0,head,head,0,0))
 if old_count:
  old.uc.mem_write(node,words(1,maps,0,0,2,0));old.pointer(maps+4,node);old.pointer(maps+8,node);old.pointer(maps+12,node);old.pointer(maps+16,1)
 for j,id in enumerate(old_ids):old.uc.mem_write(rows+j*8,struct.pack('<iHBB',id,j+7,0,0))
 old.uc.mem_write(list_object,words(0,count,ids));old.uc.mem_write(ids,words(*selected)if selected else bytes(4));new.uc.mem_write(ni,words(*old_ids)if old_ids else bytes(4));new.uc.mem_write(nj,words(*selected)if selected else bytes(4))
 fixture=new.invoke('dh2_skill_save_fixture_create_v6',[ni,old_count,nj,count]);assert fixture
 assert snapshot(True)[0]==[1,int(old_count>0),0],(i,hex(fixture),snapshot(True))
 trace={'old':[],'new':[]};old.invoke(0x467324,[save]);status=new.invoke('dh2_skill_save_fixture_reload_v6',[fixture,fixture,callback]);assert status==0,(i,status,trace,new.import_calls)
 assert trace['old']==trace['new'],(i,trace)
 assert snapshot(False)==snapshot(True),(i,snapshot(False),snapshot(True))
 records.append(dict(old_count=old_count,selected=selected,trace=trace['old'],after=snapshot(False)))
 new.invoke('dh2_skill_save_fixture_destroy_v6',[fixture])
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
gold=ROOT/'port/level-world/reference/character-skill-combat-v6/save-reload-gold-v6.json';gold.write_text(json.dumps(records,indent=2)+'\n')
report=dict(validation='PASS',cases=len(records),ordered_provider_boundaries=sum(len(x['trace'])for x in records),mismatches=0,original_sha256=sha(ELF),optimized_sha256=sha(LIB),gold_sha256=sha(gold),scope=__doc__)
(ROOT/'port/level-world/reports/character-skill-save-reload-v6-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
