"""Original TransmuteItem arithmetic and reached action ordering.
Cached property/design, inventory effects and world/achievement are boundaries.
"""
import sys,json,struct
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from character_menu_native_v1_original import Original,ROOT
class Transmute(Original):
 def hook(self,uc,a,size,u):
  if self.active:
   if a==0x3dedb4:self.ret(self.bonus);return
   if a==0x4c4bdc:assert self.cstr(self.c.reg(2))=='TransmuteMultiplier';self.ret(self.multiplier);return
   if a==0x3fa17c:self.effects.append(['quantity',-1]);self.ret();return
   if a==0x3fc63c:self.ret(0);return
   if a==0x3fe448:self.effects.append(['remove',self.c.reg(1)]);self.ret();return
   if a==0x3fe164:self.effects.append(['gold',self.c.reg(1)]);self.ret();return
   if a==0x3e0798:self.effects.append(['property',self.c.reg(1),self.c.reg(2)]);self.ret();return
   if a==0x36effc:self.effects.append(['local']);self.ret(self.local);return
   if a==0x3df6e0:self.effects.append(['count']);self.ret(self.count);return
   if a==0x3f9d08:self.effects.append(['is_player']);self.ret(self.player);return
   if a==0x3a3f70:self.effects.append(['achievement_key',self.cstr(self.c.reg(0))]);self.ret(17);return
   if a==0x3813b8:self.effects.append(['achievement',self.c.reg(1)]);self.ret();return
  super().hook(uc,a,size,u)
m=Transmute();m.stat_entry=0x3a4a3c;vt=m.alloc(64);m.c.pointer(vt+40,0x3f9d08);m.c.pointer(m.character,vt);cases=[]
for value in (0,1,25,8388607,2147483647):
 for bonus in (-256,0,256):
  for multiplier in (1,256,65536):
   for quantity in (1,2):
    for preview in (0,1):
     for local,count,player in ((0,300,1),(1,299,1),(1,300,0),(1,300,1)):
      m.c.pointer(m.item+84,value);m.c.uc.mem_write(m.item+80,struct.pack('<h',quantity));m.bonus=bonus;m.multiplier=multiplier;m.local=local;m.count=count;m.player=player;m.effects=[];m.active=True
      try:m.c.invoke(m.stat_entry,[m.character,m.item,preview],budget=100000)
      finally:m.active=False
      cases.append(dict(value=value,bonus=bonus,multiplier=multiplier,quantity=quantity,preview=preview,local=local,count=count,player=player,result=m.c.reg(0),effects=m.effects))
out=ROOT/'port/engine-ui/reference/character-menu-native-v1/transmute-coordinator-gold-v1.json';out.write_text(json.dumps(dict(boundary_fixtures=['cached property','design constant','inventory effects','world and achievement'],cases=cases),indent=2)+'\n')
assert all(not c['effects'] for c in cases if c['preview'])
prices={(c['value'],c['bonus'],c['multiplier'],c['result']) for c in cases}
(out.parent/'transmute-value-gold-v1.bin').write_bytes(struct.pack('<I',len(prices))+b''.join(struct.pack('<iiii',*row) for row in sorted(prices)))
print(json.dumps(dict(validation='PASS',original_transmute_cases=len(cases),native_effect_dependency_bodies_unproved_here=True)))
