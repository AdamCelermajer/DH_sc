"""Original ItemInventory ctor, ItemInstance ctor, named inventory reader,
actual table lookup and force-insert ownership. Name/stats/requirements/power,
fullness/equipment/notifications are explicit required controlled services.
This corpus does not claim those deeper game effects or campaign file parity.
"""
import json,random,struct,hashlib,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from player_savegame_v1_original import Save,ROOT,SCR,W,encoded
from items_differential import strings
from unicorn import UC_HOOK_CODE
class Inventory(Save):
 def __init__(self):
  super().__init__();self.trace=[];self.loading=False;self.inv=self.character+0x37c
  self.blob=(ROOT/'.local-inputs/items-discovery/loot_table_pyarray.bin').read_bytes();self.cursor=0
  for a in (0x4ba4fc,0x4ba3c8,0x4ba27c,0x4ba12c):self.invoke(a,[self.stream],budget=20000000)
  self.blob=(ROOT/'.local-inputs/items-discovery/loot_table_pyarraynames.bin').read_bytes();self.cursor=0
  for a in (0x4b53f4,0x4b5258,0x4b4a54,0x4b4724):self.invoke(a,[self.stream])
  self.itemnames=strings(self.blob)[3]
  got=0x46a3b8+self.word(0x46a774);count=self.word(self.word(got+self.word(0x46a784)));p=self.word(self.word(got+self.word(0x46a788)))
  self.powers=[self.cstring(self.word(p+4*i)) for i in range(count)]
  got=0x4ba158+self.word(0x4ba26c);self.table=self.word(self.word(got+self.word(0x4ba278)))
  self.ids=[i for i in range(len(self.itemnames)) if self.word(self.table+164*i+0x58) in (0,4,6,13,14)]
  self.uc.hook_add(UC_HOOK_CODE,self.inventory_service)
 def cstring(self,p):
  b=bytearray()
  while self.uc.mem_read(p+len(b),1)!=b'\0':b.extend(self.uc.mem_read(p+len(b),1))
  return bytes(b)
 def record(self,op,item=0,arg=0,index=0,set=0):
  self.trace.append(W(op,self.word(item+4) if item else 0,int.from_bytes(self.uc.mem_read(item+0x50,2),'little') if item else 0,self.word(item+0x54) if item else 0,arg,index,set))
 def inventory_service(self,uc,a,z,u):
  if not self.loading:return
  if a in (0x3fb754,0x3fb290,0x3facdc):self.record(a,self.reg(0));self.returned()
  elif a==0x3fbc60:
   item=self.reg(0);power=self.reg(1);self.record(a,item,power);start=self.word(item+0x5c);end=self.word(item+0x60)
   if not start:start=self.allocate(256);self.pointer(item+0x5c,start);end=start
   self.pointer(end,power);self.pointer(item+0x60,end+4);self.returned()
  elif a==0x3fdfd8:self.record(a,0,self.reg(1)) # execute scalar clamp and genuine no-player gate
  elif a==0x3faab0:self.record(0x3ff70c,self.reg(0));self.returned()
  elif a==0x3fe330:
   end=self.word(self.inv+0xc);slot=self.word(end-4);item=self.word(slot);index=(end-self.word(self.inv+8))//4-1;self.record(a,item,0,index);self.returned(0)
  elif a==0x400634:
   index=self.reg(2);set=uc.mem_read(self.character+0x3aa,1)[0];item=0
   if index!=0xffffffff:
    slot=self.word(self.word(self.inv+8)+4*index);item=self.word(slot);self.pointer(self.word(self.word(self.inv+0x14)+12*set)+4*self.reg(1),slot);uc.mem_write(slot+4,bytes([set,self.reg(1)]))
   self.record(a,item,self.reg(1),index,set);self.returned()
  elif a==self.callback+64:self.returned(0) # controlled owner IsPlayer=false
  elif a==0x3f9c44:pass
 def fresh_inventory(self,capacity):
  self.fresh([1]);self.uc.mem_write(self.character,bytes(0x1800));vt=self.data+0x9000;self.pointer(self.character,vt);self.pointer(vt+0x28,self.callback+64)
  self.invoke(0x3ff200,[self.inv]);self.uc.mem_write(self.inv+0x2c,bytes([capacity&255]));self.trace=[]
 def snapshot_inventory(self):
  out=W(self.word(self.inv+0x20),self.uc.mem_read(self.inv+0x2e,1)[0]);start=self.word(self.inv+8);end=self.word(self.inv+0xc);out+=W((end-start)//4)
  potion=self.word(self.inv+0x24);potionindex=0xffffffff
  for i in range((end-start)//4):
   slot=self.word(start+4*i);item=self.word(slot)
   if item==potion:potionindex=i
   out+=W(self.word(item+4),int.from_bytes(self.uc.mem_read(item+0x50,2),'little'),self.word(item+0x54),self.uc.mem_read(item+0x68,1)[0],self.uc.mem_read(slot+4,1)[0],self.uc.mem_read(slot+5,1)[0])
   powers=self.word(item+0x5c);count=(self.word(item+0x60)-powers)//4;out+=W(count)+bytes(self.uc.mem_read(powers,4*count)) if count else W(0)
  out+=W(potionindex)
  for s in range(2):
   p=self.word(self.word(self.inv+0x14)+12*s)
   for j in range(9):
    slot=self.word(p+4*j);index=0xffffffff
    for i in range((end-start)//4):
     if slot and self.word(start+4*i)==slot:index=i
    out+=W(index)
  return out
def main():
 c=Inventory();r=random.Random(20261004);cases=[]
 for k in range(72):
  capacity=r.choice([-1,0,5]);c.fresh_inventory(capacity);selection=r.choice([0,1,127,255]);blob=W(r.choice([0,5,9999,100000]),selection,r.randrange(1,7));n=struct.unpack_from('<I',blob,8)[0]
  for j in range(n):
   id=r.choice(c.ids);type=c.word(c.table+164*id+0x58);equip=[r.randrange(9) if type not in (13,14) and r.randrange(3)==0 else -1 for _ in range(2)]
   powers=[r.choice(c.powers+[b'unknownPower']) for _ in range(r.randrange(3))]
   blob+=encoded(c.itemnames[id])+W(*equip,r.choice([0,1,-1,32767,32768,65535,65536]),r.randrange(1000))+bytes([r.choice([0,1,255])])+W(len(powers))+b''.join(encoded(p) for p in powers)
  c.blob=blob;c.cursor=0;c.loading=True
  try:c.invoke(0x46a3a0,[c.stream,c.obj],budget=20000000)
  except Exception:print('failed case',k,'pc',hex(c.uc.reg_read(c.pc)),'registers',[hex(c.reg(x)) for x in range(4)],'trace',[x.hex() for x in c.trace]);raise
  c.loading=False;assert c.cursor==len(blob)
  snap=c.snapshot_inventory();trace=b''.join(c.trace);cases.append(W(capacity,len(blob))+blob+W(len(snap))+snap+W(len(c.trace))+trace)
 ref=ROOT/'port/game-data/reference/item-inventory-v1';ref.mkdir(parents=True,exist_ok=True);gold=b'INV1'+W(len(c.powers))+b''.join(W(len(p))+p for p in c.powers)+W(len(cases))+b''.join(cases);(ref/'fixtures.bin').write_bytes(gold)
 report={'validation':'PASS','comparisons':len(cases),'original_sha256':hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'gold_sha256':hashlib.sha256(gold).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'scope':__doc__,'native_comparison':False}
 (ref/'original-gold.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
