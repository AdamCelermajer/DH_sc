from pathlib import Path
import sys,struct,json,hashlib
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(root/'port/game-data/tests'))
from player_savegame_v1_original import Save,W
from unicorn import UC_HOOK_CODE
class Manager(Save):
 def __init__(self):
  self.managing=False;super().__init__();self.manager=self.allocate(0x800);self.online=self.allocate(0x100);self.uc.hook_add(UC_HOOK_CODE,self.manager_hook)
 def manager_hook(self,uc,a,z,u):
  if not self.managing:return
  if a==0x7fd794:self.returned(self.online)
  elif a==0x37418c:
   p=self.reg(0);uc.mem_write(p,bytes(0x688));self.pointer(p+0x66c,1)
   for off in(0x664,0x668,0x670,0x674,0x678,0x67c):self.pointer(p+off,0xffffffff)
   self.returned(p)
  elif a==0x378808:uc.mem_write(self.reg(0),bytes(uc.mem_read(self.reg(1),0x688)));self.returned(self.reg(0))
  elif a==0x371294:self.returned()
 def external(self,uc,a,z,u):
  if self.imports.get(a)=='malloc':self.returned(self.allocate(self.reg(0)))
  elif self.imports.get(a)=='free':self.returned()
  else:super().external(uc,a,z,u)
old=Manager();old.managing=True;old.invoke(0x374b28,[old.manager]);rows=[]
inputs=[(9,3,2,1),(2,8,0,0),(7,4,1,1),(1,5,3,1)]
for id,controller,index,local in inputs:old.invoke(0x378a40,[old.manager,id,controller,index,local])
records={id:old.invoke(0x36dfb0,[old.manager,id,0])for id,*_ in inputs}
for id,*_ in inputs:
 p=records[id];character=old.allocate(0x1500)if id!=7 else 0
 old.pointer(p+0x660,character)
 if character:old.uc.mem_write(character+0x13c8,struct.pack('<h',263 if id in(1,9) else 290))
old.pointer(old.manager+0x6c4,3)
def record(p):return W(old.word(p+0x670),bool(old.word(p+0x660)),old.word(p+0x678),old.word(p+0x67c),old.word(p+0x66c)&255)
for kind,a in enumerate((0x36dfb0,0x36e744,0x36e478,0x36e2ac)):
 for index in(-1,0,1,2,3,4,7,9,99):
  for flag in(0,1):rows.append(W(kind,index,flag)+record(old.invoke(a,[old.manager,index,flag])))
for base in(263,290,325):rows.append(W(4,base,0,old.invoke(0x36ea50,[old.manager,base]),0,0,0,0))
ref=root/'port/level-world/reference/player-manager-owner-v1';blob=b'PMO1'+W(len(inputs))+b''.join(W(*v)for v in inputs)+W(len(rows))+b''.join(rows)
(ref/'query-fixtures.bin').write_bytes(blob)
report=dict(validation='PASS',comparisons=len(rows),original_sha256=hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(blob).hexdigest(),player_info_network_constructor_and_copy_fixtures=True,canonical_world=False)
(ref/'query-original-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
