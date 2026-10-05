"""Original ChangeFaery + real saved current/count field access.
Fresh difficulty, UpdateAllSkills and reached world effects are boundaries.
"""
import sys,json
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from character_menu_native_v1_original import Original,ROOT
class Faery(Original):
 def hook(self,uc,a,size,u):
  if self.active:
   if a in (0x3bb9d8,0x3bba20):return
   if a==0x3bb8e4:self.effects.append(['difficulty']);self.ret(self.difficulty);return
   if a==0x3d8894:self.effects.append(['update_all']);self.ret();return
   if a==0x3a54d4:self.effects.append(['position',int(self.c.reg(0)==self.faery)]);self.ret(self.position);return
   if a==0x394d34:self.effects.append(['move_target',int(self.c.reg(0)==self.faery),int(self.c.reg(1)==self.position),self.c.reg(2),self.c.reg(3)]);self.ret();return
   if a==0x3c99a0:self.effects.append(['effect',int(self.c.reg(0)==self.faery+0x49c)]);self.ret();return
  super().hook(uc,a,size,u)
m=Faery();saved=m.alloc(512);m.faery=m.alloc(0x600);m.position=m.alloc(16);m.c.pointer(m.character+0x14e8,saved);m.stat_entry=0x3ae99c;cases=[]
for tier in range(3):
 for index in range(5):
  for present in (0,1):
   for j in range(3):m.c.pointer(saved+0xa0+j*4,5);m.c.pointer(saved+0xac+j*4,0)
   m.difficulty=tier;m.c.pointer(m.character+0x420,m.faery if present else 0);m.effects=[];m.active=True
   try:m.c.invoke(m.stat_entry,[m.character,index],budget=100000)
   finally:m.active=False
   current=[m.word(saved+0xac+j*4) for j in range(3)];expected=[0]*3;expected[tier]=index;assert current==expected
   assert m.effects==[['difficulty'],['update_all']]+([['position',1],['move_target',1,1,0,1],['effect',1]] if present else [])
   cases.append(dict(tier=tier,index=index,present=present,current=current,effects=m.effects))
(ROOT/'port/engine-ui/reference/character-menu-native-v1/faery-coordinator-gold-v1.json').write_text(json.dumps(dict(boundary_fixtures=['difficulty','UpdateAllSkills','world position/target/effect'],cases=cases),indent=2)+'\n')
print(json.dumps(dict(validation='PASS',original_change_faery_cases=len(cases),original_saved_current_fields=True,native_world_effect_unproved=True)))
