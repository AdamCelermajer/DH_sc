"""Original offline DropItem callback ordering; player/world/native inventory
dependencies are explicit boundaries, never claimed as native spawn proof."""
import sys,json
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from character_menu_native_v1_original import Original,ROOT
class Drop(Original):
 def hook(self,uc,a,size,u):
  if self.active:
   if a==0x36e478:self.effects.append(['player',self.c.reg(1),self.c.reg(2)]);self.ret(self.record);return
   if a==0x7fd794:self.effects.append(['online']);self.ret(self.session);return
   if a==0x3ff200:self.effects.append(['construct']);self.temp=self.c.reg(0);self.ret();return
   if a==0x3ff858:
    sp=uc.reg_read(self.c.sp);self.effects.append(['transfer',self.c.reg(1),int(self.c.reg(2)==self.temp),self.c.reg(3),self.word(sp),self.word(sp+4)]);self.ret(0);return
   if a==0x3ec974:self.effects.append(['world',int(self.c.reg(0)==self.temp),int(self.c.reg(1)==self.character),int(self.c.reg(2)==self.character),self.c.reg(3)]);self.ret();return
   if a==0x3ff460:self.effects.append(['destroy']);self.ret();return
  super().hook(uc,a,size,u)
m=Drop();m.record=m.alloc(0x700);m.session=m.alloc(16);cases=[]
for present in (0,1):
 for index in (0,1,7,99):
  m.c.pointer(m.record+0x660,m.character if present else 0);m.c.uc.mem_write(m.session+5,b'\0');m.c.pointer(m.fn+16,1);m.number(m.args+9*12,index);m.effects=[];m.active=True
  try:m.c.invoke(m.entries['NativeInvDropItem'],[m.fn],budget=100000)
  finally:m.active=False
  cases.append(dict(present=present,index=index,effects=m.effects))
  assert m.effects==([['player',0,0],['online'],['construct'],['transfer',index,1,1,0,0],['world',1,1,1,0],['destroy']] if present else [['player',0,0]])
(ROOT/'port/engine-ui/reference/character-menu-native-v1/drop-offline-coordinator-gold-v1.json').write_text(json.dumps(dict(boundary_fixtures=['player lookup','online session','temporary constructor','TransferItem','WorldDropInventory','temporary destructor'],cases=cases),indent=2)+'\n')
print(json.dumps(dict(validation='PASS',original_offline_drop_cases=len(cases),native_world_spawn_unproved=True)))
