"""Original NativeSaveGame post-save coordinator; SG_Save and native saved
level/property/world/achievement services are explicit fixture boundaries."""
import sys,json
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from character_menu_native_v1_original import Original,ROOT
class Save(Original):
 def hook(self,uc,a,size,u):
  if self.active:
   if a==0x3bc4a8:self.effects.append(['save']);self.ret();return
   if a==0x3bbed0:self.effects.append(['skill',self.c.reg(1)]);self.ret(self.levels[self.c.reg(1)]);return
   if a==0x4c4bdc:
    key=self.cstr(self.c.reg(2));self.effects.append(['constant',self.cstr(self.c.reg(1)),key]);self.ret(self.maximum if 'Skill' in key else 50);return
   if a==0x3f9d08:self.effects.append(['is_player']);self.ret(self.player);return
   if a==0x3df6e0:prop=self.c.reg(1);self.effects.append(['property',prop]);self.ret(self.points if prop==157 else self.level);return
   if a==0x3a3f70:self.effects.append(['achievement_key',self.cstr(self.c.reg(0))]);self.ret(17);return
   if a==0x3813b8:self.effects.append(['achievement']);self.ret();return
  super().hook(uc,a,size,u)
m=Save();saved=m.alloc(512);m.c.pointer(m.character+0x14e8,saved);vt=m.alloc(64);m.c.pointer(m.character,vt);m.c.pointer(vt+40,0x3f9d08);m.c.pointer(m.fn+16,0);cases=[]
for levels in ([],[0],[1],[10],[10,0],[10,10]):
 for maximum in (5,10):
  for points in (0,1):
   for level in (49,50):
    for player in (0,1):
     m.levels=levels;m.maximum=maximum;m.points=points;m.level=level;m.player=player;m.effects=[];m.c.pointer(saved+0x84,len(levels));m.active=True
     try:m.c.invoke(m.entries['NativeSaveGame'],[m.fn],budget=100000)
     finally:m.active=False
     keys=[e[1] for e in m.effects if e[0]=='achievement_key'];expected=['epic_maxskill']*sum(v==maximum and v!=0 for v in levels)
     if player and all(levels):expected+=['epic_all_skills_unlocked']
     if player and points==0 and level==50:expected+=['epic_spent_all_skill_points']
     assert keys==expected,(levels,maximum,points,level,player,keys,expected)
     assert m.effects[0]==['save']
     cases.append(dict(levels=levels,maximum=maximum,points=points,level=level,player=player,effects=m.effects))
(ROOT/'port/engine-ui/reference/character-menu-native-v1/save-coordinator-gold-v1.json').write_text(json.dumps(dict(boundary_fixtures=['SG_Save','saved-level getters','design constants','property getters','IsPlayer','achievement'],cases=cases),indent=2)+'\n')
print(json.dumps(dict(validation='PASS',original_save_post_coordinator_cases=len(cases),native_SG_Save_unproved_here=True)))
