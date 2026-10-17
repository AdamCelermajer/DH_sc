"""Whole NativeCreate, indexed ctor, offline SG_Save, seven writers and saveAll.
AS conversion, filename/profile registration, clock/online/slot providers,
string byte storage, stream virtuals, lock/I/O/cache publication and temporary
destruction are declared leaves. No private campaign or name is used.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3];REF=ROOT/'port/game-data/reference/player-profile-create-v1'
sys.path.insert(0,str(Path(__file__).parent))
from player_profile_index_v1_original import Load,W,text,SHA
WRITERS={'PNAM':0x4688c0,'PLVL':0x468930,'PCLS':0x4698e4,'PDFL':0x4688f8,'LNAM':0x468b20,'LEPT':0x4688c8,'LUSP':0x4689a8}
ADDRESSES=[0x43f630,0x4655ac,0x465430,0x468574,0x439c34,0x467718,0x467744,0x464b2c,0x4684f0,0x315fb8,0x3fa188,0x46b0a4,0x461668,0x38b808,0x461770,0x33e138,*WRITERS.values()]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def evidence(elf):
 assert sha(elf)==SHA;raw=elf.read_bytes();rows=[];assembly=[]
 with elf.open('rb') as f:
  e=ELFFile(f);sy=list(e.get_section_by_name('.dynsym').iter_symbols())
  for a in dict.fromkeys(ADDRESSES):
   s=next(s for s in sy if s['st_value']==a and s['st_size']);n=s['st_size']
   g=next(g for g in e.iter_segments() if g['p_type']=='PT_LOAD' and g['p_vaddr']<=a<a+n<=g['p_vaddr']+g['p_filesz'])
   body=raw[g['p_offset']+a-g['p_vaddr']:g['p_offset']+a-g['p_vaddr']+n]
   rows.append({'original_symbol':s.name,'elf_address':hex(a),'size':n,'sha256':hashlib.sha256(body).hexdigest()})
   assembly.append(s.name+' '+hex(a)+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}' for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(body,a)))
 return rows,'\n\n'.join(assembly)+'\n'
class Create(Load):
 def __init__(self):
  super().__init__();self.native=False;self.writer_mode=False;self.buffers={};self.published=None;self.persisted=b'';self.output=b'';self.writes=[]
  self.args=self.allocate(128);self.as_name=self.allocate(128);self.as_class=self.allocate(128);self.conversions=0
  self.names=[('Class%03d'%i).encode() for i in range(330)]
  for i,s in [(263,b'KnightPlayerBase'),(290,b'MagePlayerBase'),(325,b'RoguePlayerBase')]:self.names[i]=s
  self.name_pointers=self.allocate(4*len(self.names))
  for i,s in enumerate(self.names):p=self.allocate(len(s)+1);self.uc.mem_write(p,s+b'\0');self.pointer(self.name_pointers+4*i,p)
  self.pointer(0x9a6458,len(self.names));self.pointer(0x9a6460,self.name_pointers);self.pointer(0x9a64a0,3)
  self.wvt=self.allocate(128)
  for off,cb in [(0x1c,128),(0x2c,144),(0x30,160)]:self.pointer(self.wvt+off,self.callback+cb)
  self.uc.hook_add(UC_HOOK_CODE,self.create_service)
 def cstring(self,s):p=self.allocate(len(s)+1);self.uc.mem_write(p,s+b'\0');return p
 def string(self,obj,s):p=self.cstring(s);self.pointer(obj+16,p+len(s));self.pointer(obj+20,p)
 def create_service(self,uc,a,z,u):
  if not self.native and not self.writer_mode:return
  x,y,z,w=(self.reg(i) for i in range(4))
  if a==0x420a84:
   self.conversions+=1;self.returned(self.as_name if self.conversions==1 else self.as_class)
  elif a==0x4660c8:self.returned(self.slot)
  elif a==0x4655ac:self.obj=x
  elif a==0x315ed8:
   self.created=x;uc.mem_write(x,bytes(0x80));head=x+0x20;self.pointer(head+8,head);self.pointer(head+12,head)
   self.string(x+4,b'synthetic-profile');self.returned(x)
  elif a==0x463c84:
   self.string(y,b'synthetic-profile');self.returned()
  elif a==0x315848:
   tag=text(self,y).decode();self.callbacks.setdefault(tag,[])
   self.callbacks[tag].append({'reader':hex(z),'writer':hex(w)});self.returned()
  elif a==0x7fd794:self.events.append((7,));self.returned(self.online)
  elif a==0x31167c:self.string(x,b'');self.returned()
  elif a==0x3116e8:self.string(x,bytes(uc.mem_read(y,w if False else z-y)));self.returned()
  elif a in (0x33076c,0x3109e0):
   self.string(x,text(self,y) if a==0x33076c else bytes(uc.mem_read(y,z-y)));self.returned(x)
  elif a==0x3139ac:self.returned()
  elif a==0x60b0cc:self.returned(self.seed)
  elif self.imports.get(a)=='time':self.returned(self.date)
  elif a==0x315fb8 and self.native:
   self.saved_fields=self.fields();self.replay_save=bytes(uc.mem_read(self.obj,0x198));self.returned()
  elif a==0x30ed30:
   bits=struct.unpack('<II',struct.pack('<d',float(x)));self.put(0,bits[0]);self.put(1,bits[1]);uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif a==0x797488:self.published=struct.unpack('<d',W(z,w))[0];self.returned()
  elif a==0x46378c:self.returned()
  elif self.writer_mode and a==0x316d3c:self.pointer(x,self.wvt);self.buffers[x]=bytearray();self.returned(x)
  elif self.writer_mode and a in (0x3136b4,0x3136b8,0x313c90):self.returned()
  elif self.writer_mode and a==0x315110:
   if self.uc.mem_read(x+0x1d,1)==b'\x01':self.persisted=bytes(self.buffers[self.word(x)])
   self.returned()
  elif self.writer_mode and a==0x315ad0:
   self.cached=bytes(self.buffers[y]);self.returned()
 def external(self,uc,a,z,u):
  if self.native and self.imports.get(a)=='time':self.returned(self.date)
  elif self.native and self.imports.get(a)=='__aeabi_ui2d':
   bits=struct.unpack('<II',struct.pack('<d',float(self.reg(0))));self.put(0,bits[0]);self.put(1,bits[1]);uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif self.writer_mode and self.imports.get(a)=='malloc':self.returned(self.allocate(self.reg(0)))
  elif self.writer_mode and self.imports.get(a)=='free':self.returned()
  elif self.writer_mode and a==self.callback+128:
   x,y,n=self.reg(0),self.reg(1),self.reg(2);b=bytes(uc.mem_read(y,n));self.writes.append(b)
   if x in self.buffers:
    buffer=self.buffers[x];pos=self.positions.get(x,0)
    if pos+n>len(buffer):buffer.extend(bytes(pos+n-len(buffer)))
    buffer[pos:pos+n]=b;self.positions[x]=pos+n
   else:self.output+=b
   self.put(1,0);self.returned(n)
  elif self.writer_mode and a==self.callback+144:self.positions[self.reg(0)]=self.reg(2);self.returned()
  elif self.writer_mode and a==self.callback+160:self.put(1,0);self.returned(self.positions.get(self.reg(0),0))
  else:super().external(uc,a,z,u)
 def fields(self):
  o=self.obj;return {'slot':self.word(o+4),'level':self.word(o+0x30),'class':self.word(o+0x34),'difficulty':self.word(0x9a6060),'unlocked':self.word(o+0x3c),'date':self.word(o+0x38),'name':text(self,self.word(o+0x2c)).decode(),
   'word50':[self.word(o+0x50+4*i) for i in range(3)],'seeds':[self.word(o+0x5c+4*i) for i in range(3)],'quests':[self.word(o+0xfc+4*i) for i in range(3)],'entry':[self.word(o+0x40+4*i) for i in range(3)],'spawn':list(self.uc.mem_read(o+0x4c,3)),'byte194':self.uc.mem_read(o+0x194,1)[0],'mode':self.word(o+0x178)}
 def run(self,name,class_name,slot,seed,date,argc=2):
  self.slot=slot;self.seed=seed;self.date=date;self.flags=0;self.callbacks={};self.events=[];self.conversions=0;self.published=None;self.saved_fields=None;self.replay_save=None
  self.uc.mem_write(self.as_name,b'\xff'+bytes(127));self.pointer(self.as_name+12,self.cstring(name))
  self.uc.mem_write(self.as_class,b'\xff'+bytes(127));self.pointer(self.as_class+12,self.cstring(class_name))
  self.uc.mem_write(self.args,bytes(128));self.pointer(self.args+16,argc);self.pointer(self.args+12,self.allocate(12));self.pointer(self.args+20,1)
  self.native=True;self.active=False
  try:self.invoke(0x43f630,[self.args],budget=2000000)
  finally:self.active=False;self.native=False
  # NativeCreate's temporary Save lives in its stack frame. Later independent
  # writer/saveAll calls need the captured live Save bytes in a stable fixture
  # address; otherwise their own stack frames overwrite the retired source
  # frame. String/profile pointers retain their same declared leaf storage.
  if self.replay_save is not None:
   self.obj=self.allocate(0x198);self.uc.mem_write(self.obj,self.replay_save)
  return {'argc':argc,'name':name.decode(),'class_name':class_name.decode(),'slot':slot,'seed_time':seed,'date_time':date,'conversions':self.conversions,'published':self.published,'fields':self.saved_fields,'registered':{k:v for k,v in sorted(self.callbacks.items())},'online_queries':sum(e[0]==7 for e in self.events)}
 def writers(self):
  self.writer_mode=True;self.active=False;self.positions={};out={}
  stream=self.allocate(8);self.pointer(stream,self.wvt)
  try:
   for tag,a in WRITERS.items():self.output=b'';self.writes=[];self.invoke(a,[stream,self.obj],budget=200000);out[tag]={'hex':self.output.hex(),'writes':[b.hex() for b in self.writes]}
   return out
  finally:self.writer_mode=False
 def save_all(self):
  profile=self.word(self.obj+8);head=profile+0x20;nodes=[]
  for tag in sorted(WRITERS):
   n=self.allocate(64);self.pointer(n+0x24,self.cstring(tag.encode()));self.pointer(n+0x38,WRITERS[tag]);self.pointer(n+0x3c,self.obj);nodes.append(n)
  for i,n in enumerate(nodes):self.pointer(n+4,head if i==0 else nodes[i-1]);self.pointer(n+12,nodes[i+1] if i+1<len(nodes) else 0)
  self.pointer(head+4,nodes[0]);self.pointer(head+8,nodes[0]);self.pointer(head+12,nodes[-1]);self.pointer(profile+0x30,len(nodes))
  self.writer_mode=True;self.positions={};self.buffers={};self.cached=b'';self.persisted=b''
  try:self.invoke(0x315fb8,[profile],budget=2000000);assert self.cached==self.persisted
  finally:self.writer_mode=False
  return self.persisted
def main():
 p=argparse.ArgumentParser();p.add_argument('--original-elf',type=Path,required=True);a=p.parse_args();rows,asm=evidence(a.original_elf.resolve());REF.mkdir(parents=True,exist_ok=True)
 (REF/'original-functions.json').write_text(json.dumps({'original_sha256':SHA,'functions':rows,'scope':__doc__},indent=2)+'\n');(REF/'original-functions.asm').write_text(asm)
 c=Create();executed=set();words={q for r in rows for q in range(int(r['elf_address'],16),int(r['elf_address'],16)+r['size'],4)}
 c.uc.hook_add(UC_HOOK_CODE,lambda uc,a,z,u:executed.add(a) if a in words else None)
 cases=[]
 for class_name in [b'KnightPlayerBase',b'MagePlayerBase',b'RoguePlayerBase',b'Unknown',b'Class264']:
  for seed,date in [(0,0),(0xffffffff,0x80000000),(0x12345678,0x87654321)]:
   case=c.run(b'ReconstructedHero',class_name,len(cases)%4,seed,date)
   if case['published'] is not None:case['writers']=c.writers();case['profile_hex']=c.save_all().hex()
   cases.append(case)
 for argc in [0,1,3]:cases.append(c.run(b'ReconstructedHero',b'KnightPlayerBase',0,0,0,argc))
 capture={'validation':'PASS','original_sha256':SHA,'cases':cases,'executed_words':len(executed),'generator_sha256':sha(Path(__file__)),'scope':__doc__,'declared_leaves':['AS as_string/value result','NextFreeSlot','Savegame ctor/filename/load registration (separate profile-index original proof)','GetRealTime/time','fresh offline Online','byte/string allocation/copy','stream virtuals','saveAll lock/read/write/cache publication','temporary Save destructor']}
 def block(b):return W(len(b))+b
 binary=b'PCT1'+W(len(c.names))+b''.join(block(n) for n in c.names)+W(len(cases))
 for case in cases:
  f=case['fields'];binary+=W(case['argc'])+block(case['name'].encode())+block(case['class_name'].encode())+W(case['slot'],case['seed_time'],case['date_time'],int(f is not None))+block(bytes.fromhex(case.get('profile_hex','')))
  if f:binary+=W(*(f[k] for k in ['slot','level','class','difficulty','unlocked','date','byte194','mode']))+W(*(f[k][i] for k in ['word50','seeds','quests','entry'] for i in range(3)))+bytes(f['spawn'])
 (REF/'fixtures.bin').write_bytes(binary);capture['fixtures_sha256']=sha(REF/'fixtures.bin')
 (REF/'original-capture.json').write_text(json.dumps(capture,indent=2)+'\n');print(json.dumps({'validation':'PASS','cases':len(cases),'executed_words':len(executed)}))
if __name__=='__main__':main()
