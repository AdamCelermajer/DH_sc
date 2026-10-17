"""Original campaign indexing, complete SG_Load ordering and metadata readers.

ARM32 bodies and original STL indexing/string algorithms execute. Stream I/O,
filename, profile construction, initializer/global/quest operations are explicit
services. Metadata gold is synthetic; personal campaign bytes stay private.
"""
from __future__ import annotations
import argparse, hashlib, json, random, struct, sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM
from unicorn import UC_HOOK_CODE
ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(Path(__file__).parent))
from player_savegame_v1_original import Save, W
REF = ROOT / 'port/game-data/reference/player-profile-index-v1'
SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
TAGS = ['PNAM','PLVL','PCLS','PDFL','LNAM','LEPT','LUSP','LVLS','SKIL','FAES','CFEE','QEST','PROP','GEAR','FTVL']
SYMBOLS = ['_ZN8Savegame10_cacheFileEP12StreamBuffer',
 '_ZN8Savegame4loadEPKcPFvP11IStreamBasePvES6_S4_',
 '_ZN14PlayerSavegame7SG_LoadEi', '_ZN14PlayerSavegame5_LoadEi',
 '_ZN14PlayerSavegame22_LoadVolatileQuestsLogEi',
 '_ZN14PlayerSavegame16__LoadPlayerNameEP11IStreamBasePv',
 '_ZN14PlayerSavegame17__LoadPlayerLevelEP11IStreamBasePv',
 '_ZN14PlayerSavegame17__LoadPlayerClassEP11IStreamBasePv',
 '_ZN14PlayerSavegame21__LoadDifficultyLevelEP11IStreamBasePv',
 '_ZN14PlayerSavegame15__LoadLevelNameEP11IStreamBasePv',
 '_ZN14PlayerSavegame21__LoadLevelEntryPointEP11IStreamBasePv',
 '_ZN14PlayerSavegame19__LoadUseSpawnPointEP11IStreamBasePv',
 '_ZN11IStreamBase6readAsERSs','_ZN14PlayerSavegameC1Ev']

def sha(path): return hashlib.sha256(path.read_bytes()).hexdigest()
def text(cpu,p):
 out=bytearray()
 while cpu.uc.mem_read(p+len(out),1)!=b'\0': out.extend(cpu.uc.mem_read(p+len(out),1))
 return bytes(out)

class Index(Save):
 def __init__(self):
  super().__init__(); self.proxy=0; self.file=self.data+0x10000
  for off,cb in ((4,80),(8,96),(0x20,112),(0x24,128),(0x2c,144)):
   self.pointer(self.vt+off,self.callback+cb)
  self.uc.hook_add(UC_HOOK_CODE,self.index_service)
 def index_service(self,uc,a,z,u):
  if a==0x3172d8:
   self.proxy=self.reg(0);assert self.reg(1)==self.stream
   self.pointer(self.proxy,self.vt);self.returned(self.proxy)
 def external(self,uc,a,z,u):
  if a==self.callback+16:
   n=self.reg(2); assert self.reg(0) in (self.stream,self.proxy) and self.cursor+n<=len(self.blob)
   if n: uc.mem_write(self.reg(1),self.blob[self.cursor:self.cursor+n])
   self.cursor+=n;self.put(1,0);self.returned(n)
  elif a in (self.callback+80,self.callback+144): self.returned()
  elif a==self.callback+96: self.put(1,0);self.returned(len(self.blob))
  elif a==self.callback+112:
   assert self.reg(3)==0 and self.reg(2)<=len(self.blob)
   self.cursor=self.reg(2);self.put(1,0);self.returned()
  elif a==self.callback+128: self.put(1,0);self.returned(self.cursor)
  else: super().external(uc,a,z,u)
 def run(self,blob):
  self.blob=blob;self.cursor=0;self.uc.mem_write(self.file,bytes(0x40))
  head=self.file+0x20;self.pointer(head+8,head);self.pointer(head+12,head)
  self.invoke(0x315ad0,[self.file,self.stream],budget=20000000)
  def walk(node):
   if not node:return []
   offset=self.word(node+0x28);assert self.word(node+0x2c)==0
   return walk(self.word(node+8))+[(text(self,self.word(node+0x24)),offset,self.word(node+0x30))]+walk(self.word(node+12))
  return walk(self.word(head+4))

class Load(Save):
 def __init__(self):
  super().__init__();self.active=False;self.events=[];self.callbacks={}
  self.profile=self.allocate(0x100);self.created=self.allocate(0x100)
  self.online=self.allocate(0x100);self.manager=self.allocate(0x1000);self.game=self.allocate(0x100)
  self.quest_stream=self.manager+0x6e0;svt=self.allocate(0x100)
  self.pointer(self.quest_stream,svt);self.pointer(svt+8,self.callback+192);self.pointer(svt+0x20,self.callback+208)
  self.pointer(self.game+0x40,self.manager);self.filename=self.allocate(32);self.uc.mem_write(self.filename,b'source_profile\0')
  self.inverse={v:k for k,v in self.symbols.items()};self.uc.hook_add(UC_HOOK_CODE,self.load_service)
 def token(self,p):
  return {0:0,self.profile:1,self.created:2,self.quest_stream:3}[p]
 def event(self,op,argument=0,profile=0,section=0,reader=0,value=0):
  self.events.append((op,argument,profile,section,reader,value))
 def external(self,uc,a,z,u):
  if self.active and a in (self.callback+192,self.callback+208):
   assert self.reg(0)==self.quest_stream
   if a==self.callback+192:
    self.event(12,profile=3);self.put(0,self.length&0xffffffff);self.put(1,self.length>>32)
    uc.reg_write(self.pc,uc.reg_read(self.lr))
   else:
    assert self.reg(2)==0 and self.reg(3)==0
    self.event(13,profile=3);self.returned()
  else:super().external(uc,a,z,u)
 def load_service(self,uc,a,z,u):
  if not self.active:return
  x,y,z,w=(self.reg(i) for i in range(4))
  if a==0x31167c:
   self.pointer(x+16,self.filename);self.pointer(x+20,self.filename);self.returned()
  elif a==0x3139ac:self.returned()
  elif a==0x463c84:
   assert x==self.slot&0xffffffff and z==0 and w==0
   self.event(0,x);self.uc.mem_write(self.filename,b'source_profile\0')
   self.pointer(y+16,self.filename);self.pointer(y+20,self.filename);self.returned()
  elif a==0x315ed8:
   assert text(self,y)==b'source_profile' and z==0
   self.created=x;self.event(1);self.returned(x)
  elif a==0x315848:
   name=text(self,y).decode();assert name in TAGS and self.word(uc.reg_read(self.sp))==self.obj and w
   self.callbacks.setdefault(name,[])
   row={'first':self.inverse.get(z,hex(z)),'second':self.inverse.get(w,hex(w))}
   if row not in self.callbacks[name]:self.callbacks[name].append(row)
   self.event(2,profile=self.token(x),section=TAGS.index(name),reader=int(z!=0));self.returned()
  elif a in (0x46954c,0x469764,0x4694c8):
   assert x==self.obj;self.event({0x46954c:3,0x469764:4,0x4694c8:5}[a]);self.returned()
  elif a==0x46c1a8:
   assert x in (self.obj+0xb8,self.obj+0x118);self.event(6,int(x==self.obj+0x118));self.returned()
  elif a==0x7fd794:self.event(7);self.returned(self.online)
  elif a==0x46512c:self.put(3,self.game)
  elif a==0x465130:self.event(8)
  elif a==0x4685a8:self.put(6,self.game)
  elif a==0x36f074:assert x==self.manager;self.event(9);self.returned(bool(self.flags&4))
  elif a==0x468614:self.event(10)
  elif a==0x4685bc:self.event(11)
  elif a==0x468604:self.pointer(y,37)
  elif a==0x468608:self.event(14)
  elif a==0x46c48c:
   assert x==self.obj+0x118 and y==37 and z==self.quest_stream and w==0
   self.event(15,profile=3,value=y);self.returned()
 def run(self,mask,present,slot,flags,length):
  self.slot=slot;self.flags=flags;self.length=length;self.events=[]
  self.uc.mem_write(self.obj,bytes(0x200));self.pointer(self.obj+4,slot&0xffffffff)
  self.pointer(self.obj+8,self.profile if present else 0)
  self.uc.mem_write(self.online+5,bytes([bool(flags&1)]))
  self.uc.mem_write(self.manager+0x71b,bytes([bool(flags&2)]));self.uc.mem_write(self.manager+0x719,bytes([bool(flags&8)]))
  self.active=True
  try:self.invoke(0x465430,[self.obj,mask],budget=2000000)
  finally:self.active=False
  return self.events,self.token(self.word(self.obj+8))

def evidence(elf):
 assert sha(elf)==SHA
 blob=elf.read_bytes();rows=[];assembly=[]
 with elf.open('rb') as f:
  e=ELFFile(f);sy={s.name:s for s in e.get_section_by_name('.symtab').iter_symbols()}
  names=SYMBOLS[:]
  # Discover literal template helper spellings from the pinned binary.
  names += [n for n in sy if n.startswith('_ZN11IStreamBase6readAsI') and sy[n]['st_value'] in (0x313b48,0x38b758,0x33e040)]
  for name in names:
   s=sy[name];a,n=int(s['st_value']),int(s['st_size']);g=next(g for g in e.iter_segments() if g['p_vaddr']<=a<a+n<=g['p_vaddr']+g['p_filesz'])
   off=int(g['p_offset'])+a-int(g['p_vaddr']);body=blob[off:off+n]
   rows.append({'original_symbol':name,'elf_address':hex(a),'size':n,'sha256':hashlib.sha256(body).hexdigest()})
   assembly.append(name+' '+hex(a)+' '+str(n)+'\n'+'\n'.join(f'{i.address:08x} {i.mnemonic} {i.op_str}' for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(body,a)))
 return rows,'\n\n'.join(assembly)+'\n'

def main():
 p=argparse.ArgumentParser();p.add_argument('--original-elf',type=Path,default=ROOT/'.local-inputs/libDungeonHunter2.so');p.add_argument('--cache',type=Path,default=ROOT.parent/'cache/files');a=p.parse_args()
 assert sha(a.original_elf)==sha(ROOT/'.local-inputs/libDungeonHunter2.so')==SHA
 REF.mkdir(parents=True,exist_ok=True);rows,asm=evidence(a.original_elf)
 (REF/'original-functions.json').write_text(json.dumps({'original_sha256':SHA,'functions':rows,'scope':__doc__},indent=2)+'\n')
 (REF/'original-functions.asm').write_text(asm)
 instruction_addresses={x for r in rows for x in range(int(r['elf_address'],16),int(r['elf_address'],16)+r['size'],4)};executed=set()
 def track(cpu):
  cpu.uc.hook_add(UC_HOOK_CODE,lambda uc,address,size,user:executed.add(address) if address in instruction_addresses else None)
 c=Index();track(c);rng=random.Random(20261004);cases=[]
 for k in range(96):
  entries=[(rng.choice([b'name',b'levl',b'clss',b'skil',b'faer',b'invt',b'x\0zz']),bytes(rng.randrange(256) for _ in range(rng.randrange(40)))) for _ in range(k%12)]
  blob=W(len(entries))+b''.join(W(len(payload))+tag+payload for tag,payload in entries)
  result=c.run(blob);gold=W(len(result))
  for tag,offset,size in result:gold+=W(len(tag))+tag+W(offset,size)
  cases.append(W(len(blob))+blob+W(len(gold))+gold)
 index_blob=b'PIX1'+W(len(cases))+b''.join(cases);(REF/'fixtures.bin').write_bytes(index_blob)
 load=Load();track(load);cases=[]
 inputs=[(mask,present,slot,flags,length) for mask in (0,1,2,4,8,16,32,63,-1) for present in (0,1) for slot in (-1,0) for flags in (0,1,3,5,9,13,15) for length in (0,1,0x100000000)]
 rng=random.Random(0xd25a)
 inputs += [(rng.randint(-64,127),rng.randrange(2),rng.choice([-1,0,3]),rng.randrange(16),rng.choice([0,1,23,0x100000000])) for _ in range(1024)]
 boundaries=0
 for args in inputs:
  events,profile=load.run(*args);boundaries+=len(events)
  mask,present,slot,flags,length=args
  cases.append(struct.pack('<iIiIQII',mask,present,slot,flags,length,profile,len(events))+b''.join(W(*e) for e in events))
 load_blob=b'PSL1'+W(len(cases))+b''.join(cases);(REF/'load-fixtures.bin').write_bytes(load_blob)
 (REF/'section-callbacks.json').write_text(json.dumps(load.callbacks,indent=2)+'\n')
 # Synthetic metadata reader bodies produce their actual destination words.
 c=Save();track(c);metadata=[];rng=random.Random(0x1a4d)
 from items_differential_v4_original import strings
 character_names_path=a.cache/'data/pydata/character_properties_pyarraynames.bin'
 character_names=strings(character_names_path.read_bytes())[0]
 name_table=c.allocate(len(character_names)*4)
 for i,name in enumerate(character_names):
  p=c.allocate(len(name)+1);c.uc.mem_write(p,name+b'\0');c.pointer(name_table+4*i,p)
 c.pointer(0x9a6458,len(character_names));c.pointer(0x9a6460,name_table)
 difficulty_got=(0x46897c+c.word(0x468998))&0xffffffff
 selected_difficulty=c.word(difficulty_got+c.word(0x46899c))
 for k in range(128):
  values=[rng.getrandbits(32) for _ in range(13)];flags=bytes(rng.choice((0,1,2,127,255)) for _ in range(3))
  label=('source_profile_%03u'%k).encode();class_name=character_names[(k*13)%len(character_names)] if k%7 else b'unknown_source_character'
  blobs=[W(len(label)+1)+label+b'\0',W(values[0]),W(len(class_name)+1)+class_name+b'\0',W(values[1],values[2]),W(*values[:10]),W(*values[10:]),flags];outputs=[]
  for address,blob in zip((0x4698dc,0x4689a0,0x469d88,0x468968,0x468b78,0x468938,0x468af0),blobs):
   c.fresh([]);c.blob=blob;c.cursor=0
   c.invoke(address,[c.stream,c.obj]);assert c.cursor==len(blob)
   offsets=[0x38,*range(0x50,0x5c,4),*range(0x5c,0x68,4),*range(0xfc,0x108,4),*range(0x15c,0x168,4)] if address==0x468b78 else [0x40,0x44,0x48] if address==0x468938 else [0x30] if address==0x4689a0 else [0x34] if address==0x469d88 else []
   if address==0x4698dc:output=text(c,c.word(c.obj+0x2c))
   elif address==0x468968:output=W(c.word(selected_difficulty),c.word(c.obj+0x3c))
   else:output=b''.join(W(c.word(c.obj+off)) for off in offsets) if offsets else bytes(c.uc.mem_read(c.obj+0x4c,3))
   outputs.append(W(len(blob))+blob+W(len(output))+output)
  metadata.append(b''.join(outputs))
 metadata_blob=b'PMD2'+W(len(metadata))+b''.join(metadata);(REF/'metadata-fixtures.bin').write_bytes(metadata_blob)
 helpers=[Path(__file__).resolve(),ROOT/'port/game-data/tests/player_savegame_v1_original.py',ROOT/'port/game-data/tests/items_differential_v4_original.py',ROOT/'port/game-data/tests/navigation_differential_v4_original.py',ROOT/'port/engine-resources/tests/cpu.py']
 report={'validation':'PASS','original_sha256':SHA,'index_complete_cases':96,'load_complete_cases':len(inputs),'ordered_boundaries':boundaries,'metadata_complete_cases':128,'reader_bodies':7,'fixture_sha256':{n:sha(REF/n) for n in ('fixtures.bin','load-fixtures.bin','metadata-fixtures.bin')},'generator_sha256':{p.relative_to(ROOT).as_posix():sha(p) for p in helpers},'character_names_sha256':sha(character_names_path),'executed_words':len(executed),'instructions_executed':[hex(x) for x in sorted(executed)],'scope':__doc__,'private_campaign_exported':False}
 (REF/'original-capture.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
