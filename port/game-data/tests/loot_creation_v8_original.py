"""Whole original AddLoot40407c on every actual loot-table row.
Real cache/RNG/power/value bodies; explicit text, player-count, difficulty and
storage fixtures. World item spawning and pickup are outside this proof.
"""
import sys,struct,json,hashlib
from pathlib import Path
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/game-data/tests'))
from loot_entry_selection_v8_original import EntryOriginal,W
from unicorn import UC_HOOK_CODE
class CreationOriginal(EntryOriginal):
 def __init__(self):
  self.creating=False
  super().__init__();self.uc.hook_add(UC_HOOK_CODE,self.creation_hook)
  got=0x40408c+8+self.word(0x404628)
  self.application=self.word(got+self.word(0x404644))
  self.manager=self.data+0x180000;self.level=self.data+0x181000
  self.pointer(self.application+0x40,self.manager);self.pointer(self.manager+0x6c4,1);self.pointer(self.level+0x118,0)
 def entry_hook(self,uc,a,z,u):
  if self.creating and a==0x337a88:
   key=self.stringvalue(self.reg(1));assert key in (b'InfiniteLootDrops',b'isTracingItemPctRoll',b'isTracingItemInventory_Loot',b'MP_MinimalRandoms',b'InfiniteInventory'),key
   self.returned();return
  super().entry_hook(uc,a,z,u)
 def creation_hook(self,uc,a,z,u):
  if not self.creating:return
  if a in (0x3fb754,0x3fb290,0x3facdc):self.returned()
  elif a==0x31f594:self.returned(self.level)
  elif a==0x3ff5d4:
   p=self.reg(1);begin,end=self.word(p+0x5c),self.word(p+0x60)
   ids=[self.word(q)for q in range(begin,end,32)]
   quantity=struct.unpack('<h',uc.mem_read(p+0x50,2))[0]
   self.items.append(W(self.word(p+4),quantity,self.word(p+0x54),len(ids),*ids));self.returned()
 def run_table(self,id):
  inventory=self.data+0x182000;self.uc.mem_write(inventory,bytes(0x40));self.uc.mem_write(inventory+0x2c,bytes([255,0,0,0]));self.pointer(inventory+0x28,0x7fffffff)
  self.items=[];self.creating=self.selecting=self.loadingsource=True
  try:self.invoke(0x40407c,[inventory,id,0,0,0xffffffff,0],budget=100000000)
  finally:self.creating=self.selecting=self.loadingsource=False
  return W(len(self.items))+b''.join(self.items)
def main():
 old=CreationOriginal();base=0x4039b8+old.word(0x404040);count=old.word(old.word(base+old.word(0x404048)))
 old.infinite=0;old.counts=[1,1,1];old.transcript=[];old.random(0x13579bdf,0);cases=[]
 for id in range(count):cases.append(W(id)+old.run_table(id)+old.rng())
 output=b'LCV8'+W(count)+b''.join(cases);ref=R/'port/game-data/reference/loot-creation-v8';ref.mkdir(parents=True,exist_ok=True);(ref/'fixtures.bin').write_bytes(output)
 report=dict(validation='PASS',original_cases=count,fixture_sha256=hashlib.sha256(output).hexdigest(),original_sha256=hashlib.sha256((R/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),scope=__doc__)
 (ref/'original-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
