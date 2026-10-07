"""Actual Character::Skin and owned starter equipment vs O2 native caller.
Visual category/module/Skin mutation and Debug are explicit controlled services,
not full VisualObject mesh cloning/GPU or original factory parity.
"""
import sys,struct,json,hashlib,argparse
from pathlib import Path
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/game-data/tests'))
from fresh_inventory_owned_v4_original import OriginalOwned,W
from item_presentation_v5_differential import NativePresentation
def block(b):return W(len(b))+b
def hashname(s):
 h=0
 for c in s:h=(h*31+c)&0xffffffff
 return h&0x7fff
class Source(OriginalOwned):
 def text(self,p):return self.cstring(p)if p else b''
 def external(self,uc,a,z,u):
  if self.imports.get(a)=='strstr':
   p=self.reg(0);at=self.text(p).find(self.text(self.reg(1)));self.returned(p+at if at>=0 else 0)
  else:super().external(uc,a,z,u)
 def inventory_service(self,uc,a,z,u):
  if getattr(self,'skin_active',False):
   if a==0x3a999c:return
   if a in(0x470e5c,0x474568,0x470e18,0x473cd8,0x337888,0x337a88):
    visual=0 if a in(0x337888,0x337a88)else 0x100000001+self.word(self.character+0x2d8)-self.visual_base
    name=self.text(self.reg(1))if a in(0x470e5c,0x473cd8)else self.text(self.reg(2))if a==0x474568 else self.text(self.word(self.reg(1)+20))if a==0x337a88 else b''
    trace=W(a,visual,visual>>32)+block(name);result=0
    if a==0x470e5c:result=hashname(name)
    if a==0x474568:trace+=W(self.reg(1));result=0xffffffff if self.fallback and b'__naked'not in name and b'__placeholder'not in name else hashname(name)
    if a in(0x470e18,0x473cd8):trace+=W(self.reg(1),self.reg(2))if a==0x470e18 else W(self.reg(2),self.reg(3))
    self.skin_trace+=trace
    if visual and self.change_visual:self.pointer(self.character+0x2d8,self.word(self.character+0x2d8)+1)
    self.returned(result);return
  super().inventory_service(uc,a,z,u)
class Native(NativePresentation):
 def external(self,uc,a,z,u):
  if self.imports.get(a)=='strstr':
   p=self.reg(0);at=self.text(p).find(self.text(self.reg(1)));self.put(0,p+at if at>=0 else 0);uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,a,z,u)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,required=True);a=ap.parse_args();old=Source();new=Native(a.library);cases=[];calls=0
 cache=b''.join(block((R/'.local-inputs/items-discovery'/n).read_bytes())for n in('loot_table_pyarray.bin','loot_table_pyarraynames.bin','loot_table_pystructnames.bin'))
 for loot,n in((165,5),(174,5),(213,6)):
  for selected in(0,1):
   for fallback in(0,1):
    for mutation in(0,1):
     old.skin_active=False;old.fresh_inventory(12);old.loading=True;old.capture=True;old.requests=[];old.mutation=0;old.toggle_stats=False;old.minimal=False;old.infinite=0;old.uc.mem_write(old.inv+0x2e,bytes([selected]));old.uc.mem_write(old.seed,W(1));old.uc.mem_write(old.rngcalls,W(0));old.perform(0,loot,0,0);old.visual_base=old.allocate(4);old.pointer(old.character+0x2d8,old.visual_base);old.fallback=fallback;old.change_visual=mutation
     commands=[(15,0)]+[(2,i)for i in range(n)]+[(5,0),(15,0)];expected=b''
     for op,index in commands:
      old.skin_active=False;old.capture=True
      if op!=15:old.perform(op,index,0,0)
      old.capture=False;old.skin_active=True;old.skin_trace=b'';old.invoke(0x3a999c,[old.character]);expected+=block(old.skin_trace);calls+=1
     inp=W(loot,selected,fallback,mutation,len(commands))+b''.join(W(*x)for x in commands);native=cache+inp;new.heap=new.data+0x200000;new.uc.mem_write(new.data+0x1000,native);size=new.invoke('dh2_player_skin_fixture_v5',[new.data+0x1000,new.data+0x100000],budget=30000000);actual=bytes(new.uc.mem_read(new.data+0x100000,size))if size!=0xffffffff else None;assert actual==expected,(len(cases),actual,expected);cases.append(block(inp)+block(expected))
 ref=R/'port/game-data/reference/player-item-effects-v5';gold=ref/'skin-fixtures.bin';gold.write_bytes(b'SKV5'+W(len(cases))+b''.join(cases));sources=['port/game-data/player_gear_effects_v5.hpp','port/game-data/player_gear_effects_v5.cpp','port/game-data/tests/player_skin_v5_fixture.cpp',Path(__file__).relative_to(R).as_posix()];sources += ['port/game-data/'+n+ext for n in('fresh_inventory_owned_v4','loot_tables_v2','item_inventory_v1','player_savegame_v1','skill_tables','items','properties','class_tables','data','item_gear_properties_v5','item_power_tables_v5')for ext in('.hpp','.cpp')];report=dict(validation='PASS',comparisons=len(cases),actual_starter_classes=3,original_skin_invocations=calls,visual_identity_64bit=True,synchronous_visual_replacement=True,mismatches=0,source_sha256={p:sha(R/p)for p in sources},original_sha256=sha(R/'.local-inputs/libDungeonHunter2.so'),library_sha256=sha(a.library),gold_sha256=sha(gold),scope=__doc__);(R/'port/game-data/reports/player-skin-v5-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
