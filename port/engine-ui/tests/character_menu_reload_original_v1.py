"""Execute complete original ReloadSkills; component callbacks are fixtures.

This proves caller order, fresh Save getter branches and original AS arguments,
not native component implementations or a live menu.
"""
from pathlib import Path
import sys,json,struct,random,hashlib
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from character_menu_native_v1_original import Original,words

class ReloadOriginal(Original):
 def __init__(self):
  super().__init__();self.reload_active=False;self.records=[];self.class_index=0
  self.manager=self.alloc(0x200);self.multi=self.alloc(0x200);self.render=self.alloc(0x100)
  self.c.pointer(self.manager+0xf4,self.multi);self.c.pointer(self.multi+0x138,self.render)
 def hook(self,uc,address,size,user):
  if not self.reload_active:return super().hook(uc,address,size,user)
  services={0x3e0af8:0,0x3bc4d0:1,0x3d8cfc:2,0x3d8894:3,
   0x3e0810:4,0x3a9d10:5,0x3bb828:6,0x3bb7fc:7,0x42ca8c:8,0x7ad7e8:9}
  if address==0x797124:self.ret();return
  if address not in services:return
  op=services[address];x,y,z,w=(self.c.reg(i) for i in range(4));argument=0
  if op in (1,4):argument=y
  if op in (0,4):assert x==self.character+0x560
  elif op in (2,3):assert x==self.character+0x3c8
  elif op in (1,5,6,7):assert x==self.character
  if op==9:
   assert x==self.render and self.cstr(y)=='_root.menu_CharacterMenu' and self.cstr(z)=='IsSpecTime'
   assert self.word(self.c.uc.reg_read(self.c.sp))==1
   assert self.read(w,2)==b'\0\1'
   argument=self.read(w+4,1)[0]
  self.records.append((op,argument))
  if op==6:self.ret(self.level)
  elif op==7:
   self.ret(self.classes[self.class_index]);self.class_index+=1
  elif op==8:self.ret(self.manager)
  else:self.ret()
 def run_case(self,level,classes):
  self.level=level;self.classes=classes;self.class_index=0;self.records=[];self.reload_active=True
  try:self.c.invoke(0x3a9db4,[self.character])
  finally:self.reload_active=False
  return self.records

machine=ReloadOriginal();rng=random.Random(0xD2C0);cases=[]
for level in (-2147483648,-1,0,11,12,2147483647):
 for values in ((263,0,0),(0,325,0),(0,0,290),(290,325,263),(0,0,0),(263,325,290),(325,290,263)):
  cases.append((level,values))
for _ in range(512):cases.append((rng.choice([0,11,12,25,2147483647]),tuple(rng.choice([0,263,325,290,-1,264,291,326]) for _ in range(3))))
blob=words(0x314d5243,len(cases));calls=0
for level,classes in cases:
 records=machine.run_case(level,classes);calls+=len(records)
 blob+=struct.pack('<4iI',level,*classes,len(records))+b''.join(words(*row) for row in records)
path=ROOT/'port/engine-ui/reference/character-menu-flow-v1/reload-gold-v1.bin';path.write_bytes(blob)
report={'validation':'PASS','original_complete_cases':len(cases),'ordered_component_boundaries':calls,'gold_sha256':hashlib.sha256(blob).hexdigest(),'fresh_class_getters_including_synchronous_change':True,'component_services_are_fixtures':True,'whole_menu_live':False}
(path.parent/'reload-original-v1.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report))
