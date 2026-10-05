"""Execute complete original SG_Load/_Load/_LoadVolatileQuestsLog.

File/section/init/online/quest operations and global inputs are explicit fixtures.
Original instructions implement all mask/slot/profile/online branches and order.
"""
from pathlib import Path
import sys,json,struct,hashlib,random
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from character_menu_native_v1_original import Original,words
SECTIONS=['PNAM','PLVL','PCLS','PDFL','LNAM','LEPT','LUSP','LVLS','SKIL','FAES','CFEE','QEST','PROP','GEAR','FTVL']
class LoadOriginal(Original):
 def __init__(self):
  super().__init__();self.load_active=False;self.events=[];self.profile_callbacks={}
  self.save=self.alloc(0x200);self.profile=self.alloc(0x100);self.created=self.alloc(0x100)
  self.online=self.alloc(0x100);self.manager=self.alloc(0x1000);self.game=self.alloc(0x100)
  self.stream=self.manager+0x6e0;self.streamvt=self.alloc(0x100)
  self.size_cb=self.c.callback+192;self.seek_cb=self.c.callback+208
  self.c.imports[self.size_cb]='menu_member';self.c.imports[self.seek_cb]='menu_member'
  self.c.pointer(self.stream,self.streamvt);self.c.pointer(self.streamvt+8,self.size_cb);self.c.pointer(self.streamvt+0x20,self.seek_cb)
  self.c.pointer(self.game+0x40,self.manager);self.filename=self.alloc(32);self.c.uc.mem_write(self.filename,b'source_profile\0')
 def event(self,op,argument=0,profile=0,section=0,reader=0,value=0):self.events.append((op,argument,profile,section,reader,value))
 def token(self,p):return 0 if not p else 1 if p==self.profile else 2 if p==self.created else 3 if p==self.stream else (_ for _ in ()).throw(AssertionError(('unknown stream',hex(p))))
 def hook(self,uc,address,size,user):
  if not self.load_active:return super().hook(uc,address,size,user)
  if address in (self.size_cb,self.seek_cb) and uc.reg_read(self.c.pc)!=address:return
  x,y,z,w=(self.c.reg(i) for i in range(4))
  if address==0x31167c:
   # Explicit original STL string allocation ABI fixture, not source filename.
   self.c.pointer(x+16,self.filename);self.c.pointer(x+20,self.filename);self.ret();return
  if address==0x3139ac:self.ret();return
  if address==0x463c84:
   assert x==self.slot&0xffffffff and z==0 and w==0
   self.event(0,x);self.c.uc.mem_write(self.filename,b'source_profile\0');self.c.pointer(y+16,self.filename);self.c.pointer(y+20,self.filename);self.ret();return
  if address==0x310570:self.ret(self.created);return
  if address==0x315ed8:
   assert x==self.created and self.cstr(y)=='source_profile' and z==0
   self.event(1);self.ret(self.created);return
  if address==0x315848:
   name=self.cstr(y);assert name in SECTIONS and self.word(uc.reg_read(self.c.sp))==self.save
   self.profile_callbacks.setdefault(name,[])
   callbacks={'first':self.named.get(z,hex(z)),'second':self.named.get(w,hex(w))}
   if callbacks not in self.profile_callbacks[name]:self.profile_callbacks[name].append(callbacks)
   assert w!=0 # Original writer callback remains present, including CFEE.
   self.event(2,profile=self.token(x),section=SECTIONS.index(name),reader=int(z!=0));self.ret();return
  if address in (0x46954c,0x469764,0x4694c8):
   assert x==self.save;self.event({0x46954c:3,0x469764:4,0x4694c8:5}[address]);self.ret();return
  if address==0x46c1a8:
   assert x in (self.save+0xb8,self.save+0x118);self.event(6,int(x==self.save+0x118));self.ret();return
  if address==0x7fd794:self.event(7);self.ret(self.online);return
  if address==0x46512c:
   # Source global Game/PlayerManager projection is a declared fixture.
   self.c.put(3,self.game);return
  if address==0x465130:self.event(8);return
  if address==0x4685a8:self.c.put(6,self.game);return
  if address==0x36f074:
   assert x==self.manager;self.event(9);self.ret(bool(self.flags&4));return
  if address==0x468614:self.event(10);return
  if address==0x4685bc:self.event(11);return
  if address==self.size_cb:
   assert x==self.stream;self.event(12,profile=3);self.c.put(0,self.length&0xffffffff);self.c.put(1,self.length>>32);uc.reg_write(self.c.pc,uc.reg_read(self.c.lr));return
  if address==self.seek_cb:
   # r1 is alignment padding for the original 64-bit seek argument; it still
   # contains the loaded vtable. The actual offset is r2:r3, both zero.
   assert x==self.stream and z==0 and w==0;self.event(13,profile=3);self.ret();return
  if address==0x468604:
   # The global quest-count pointer is a declared source input. Seed its
   # actual reached target, rather than guessing a similarly named array.
   self.c.pointer(y,37);return
  if address==0x468608:self.event(14);return
  if address==0x46c48c:
   assert x==self.save+0x118 and y==37 and z==self.stream and w==0
   self.event(15,profile=3,value=y);self.ret();return
 def run_case(self,mask,present,slot,flags,length):
  self.mask=mask;self.slot=slot;self.flags=flags;self.length=length;self.events=[]
  self.c.pointer(self.save+4,slot&0xffffffff);self.c.pointer(self.save+8,self.profile if present else 0)
  self.c.uc.mem_write(self.online+5,bytes([bool(flags&1)]))
  self.c.uc.mem_write(self.manager+0x71b,bytes([bool(flags&2)]));self.c.uc.mem_write(self.manager+0x719,bytes([bool(flags&8)]))
  self.load_active=True
  try:self.c.invoke(0x465430,[self.save,mask],budget=2000000)
  except Exception:
   print('source load failure',mask,present,slot,flags,length,'pc',hex(self.c.uc.reg_read(self.c.pc)),'events',self.events,file=sys.stderr);raise
  finally:self.load_active=False
  return self.events,self.token(self.word(self.save+8))
machine=LoadOriginal();cases=[]
for mask in (0,1,2,4,8,16,32,63,-1):
 for present in (0,1):
  for slot in (-1,0):
   for flags in (0,1,3,5,9,13,15):
    for size in (0,1,0x100000000):cases.append((mask,present,slot,flags,size))
rng=random.Random(0xd25a)
for _ in range(1024):cases.append((rng.randint(-64,127),rng.randrange(2),rng.choice([-1,0,3]),rng.randrange(16),rng.choice([0,1,23,0x100000000])))
blob=words(0x314c5350,len(cases));boundaries=0
for case in cases:
 events,profile=machine.run_case(*case);boundaries+=len(events)
 mask,present,slot,flags,length=case
 blob+=struct.pack('<iIiIQII',mask,present,slot,flags,length,profile,len(events))
 blob+=b''.join(words(*event) for event in events)
ref=ROOT/'port/game-data/reference/player-save-load-v1';ref.mkdir(parents=True,exist_ok=True)
(ref/'load-gold-v1.bin').write_bytes(blob)
report={'validation':'PASS','original_complete_cases':len(cases),'ordered_boundaries':boundaries,
 'gold_sha256':hashlib.sha256(blob).hexdigest(),'component_and_global_services_are_fixtures':True,
 'source_masks_and_slot_profile_guards_executed':True,'whole_Savegame_file_class_proven':False}
(ref/'load-original-v1.json').write_text(json.dumps(report,indent=2)+'\n')
(ref/'section-callbacks-v1.json').write_text(json.dumps(machine.profile_callbacks,indent=2)+'\n')
print(json.dumps(report))
