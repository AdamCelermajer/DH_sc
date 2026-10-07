"""Actual starting AddLoot force-add ownership, RNG and source Debug order.
Name/stat/requirements/fullness and online player queries are explicit services.
No initial auto-equip, full item effects or campaign loading parity is claimed.
"""
import hashlib,json,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from item_inventory_v1_original import Inventory,W
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_SP
class Fresh(Inventory):
 def __init__(self):
  self.capture=False
  super().__init__();self.blob=(ROOT/'.local-inputs/items-discovery/loot_table_pyarray.bin').read_bytes();self.cursor=229006
  for a in (0x4b9fe0,0x4b9e8c):self.invoke(a,[self.stream],budget=30000000)
  self.uc.hook_add(UC_HOOK_CODE,self.more)
  base=0x401b10+self.word(0x401b84);self.seed=self.word(base+self.word(0x401b88));self.rngcalls=self.word(base+self.word(0x401b8c))
 def emit(self,op,caller,item=0,arg=0,index=0):
  self.requests.append(W(op,caller,self.word(item+4) if item else 0,int.from_bytes(self.uc.mem_read(item+0x50,2),'little') if item else 0,self.word(item+0x54) if item else 0,arg,index))
 def inventory_service(self,uc,a,z,u):
  if self.capture and a in (0x3fb754,0x3fb290,0x3facdc):
   self.emit(a,0,self.reg(0))
   if a==0x3fb290 and self.mutation==1:self.pointer(self.reg(0)+4,925)
   if a==0x3fb754 and self.mutation==2 and self.word(self.reg(0)+0x54)!=0:self.pointer(self.reg(0)+4,925)
  elif self.capture and a==0x3fe330:
   end=self.word(self.inv+0xc);p=self.word(self.word(end-4));self.emit(a,0,p,0,(end-self.word(self.inv+8))//4-1)
  elif self.capture and a==0x3faab0:self.emit(0x3ff70c,0,self.reg(0))
  super().inventory_service(uc,a,z,u)
 def more(self,uc,a,z,u):
  if not self.capture:return
  caller=self.uc.reg_read(self.lr)-4
  if a==0x337888:self.emit(a,caller);self.returned(0)
  elif a==0x337a88:self.emit(a,caller,0,1 if caller==0x40411c and self.minimal else 0);self.returned(1 if caller==0x40411c and self.minimal else 0)
  elif a==0x31f594:self.emit(a,caller);self.returned(0)
def main():
 c=Fresh();rng=random.Random(0xF125);cases=[]
 for k in range(60):
  seed=rng.getrandbits(32);counter=rng.getrandbits(32);cap=(-1,0,5)[k%3];c.fresh_inventory(cap);c.loading=True;c.capture=True;c.requests=[];c.mutation=0 if k<36 else (1 if k<48 else 2);c.minimal=k%7==0
  c.uc.mem_write(c.seed,W(seed));c.uc.mem_write(c.rngcalls,W(counter));sp=c.uc.reg_read(UC_ARM_REG_SP);c.uc.mem_write(sp,W(-1,0))
  loot=(165,174,213)[k%3];c.invoke(0x40407c,[c.inv,loot,0,0],budget=20000000);c.capture=False
  snapshot=c.snapshot_inventory();requests=b''.join(c.requests)
  cases.append(W(seed,counter,cap,int(c.minimal),c.mutation,loot,c.word(c.seed),c.word(c.rngcalls),len(snapshot))+snapshot+W(len(c.requests))+requests)
 gold=b'FIV2'+W(len(cases))+b''.join(cases);ref=ROOT/'port/game-data/reference/player-creation-v2';ref.mkdir(parents=True,exist_ok=True);(ref/'fresh-fixtures.bin').write_bytes(gold)
 report={'validation':'PASS','original_cases':len(cases),'source_services':sum(struct.unpack_from('<I',v,36+struct.unpack_from('<I',v,32)[0])[0] for v in cases),'gold_sha256':hashlib.sha256(gold).hexdigest(),'original_sha256':hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'scope':__doc__,'native_comparison':False}
 (ref/'fresh-original-gold.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
