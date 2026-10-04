"""Execute original owned item/vector/equipment bodies with actual cached metadata.
Name/Stats/Req/AddPower, Debug and Character effects are explicit fixture services.
No external equipment/split/merge/delete/fullness result is substituted.
"""
import hashlib,json,struct,sys,random
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from fresh_inventory_v2_original import Fresh,W
from unicorn import UC_HOOK_CODE
class OriginalOwned(Fresh):
 def external(self,uc,a,z,u):
  if self.imports.get(a)=="malloc":self.returned(self.allocate(self.reg(0)))
  elif self.imports.get(a)=="free":self.returned()
  elif a==self.callback+64:self.returned(0)
  else:super().external(uc,a,z,u)
 def more(self,uc,a,z,u):
  if self.capture and a==0x337a88 and uc.reg_read(self.lr)-4==0x3fe3a8:
   self.emit(a,0x3fe3a8,0,self.infinite);self.returned(self.infinite)
  else:super().more(uc,a,z,u)
 def inventory_service(self,uc,a,z,u):
  if not self.loading:return
  if a in (0x3fb754,0x3fb290,0x3facdc):
   if self.capture:self.emit(a,0,self.reg(0))
   if a==0x3fb290 and self.toggle_stats and struct.unpack('<h',uc.mem_read(self.reg(0)+0x50,2))[0]>1:
    uc.mem_write(self.inv+0x2e,bytes([not uc.mem_read(self.inv+0x2e,1)[0]]));self.pointer(self.character+0x1320,self.word(self.character+0x1320)^1)
   self.returned()
  elif a==self.callback+64:self.returned(0)
  elif a in (0x3e08a8,0x3a999c,0x3bd140):
   if self.capture:self.emit(a,uc.reg_read(self.lr)-4)
   self.returned()
 def snapshot(self):return self.snapshot_inventory()
 def slot(self,index):return self.word(self.word(self.inv+8)+4*index)
 def perform(self,op,a,b,c):
  if op==0:
   self.uc.mem_write(self.uc.reg_read(self.sp),W(-1,0));self.invoke(0x40407c,[self.inv,a,0,0],budget=30000000);return 0
  if op==1:
   p=self.allocate(0x6c);self.invoke(0x3fc26c,[p,a,b]);return self.invoke(0x3ff5d4,[self.inv,p,c&1,(c>>1)&1])
  if op==2:return self.invoke(0x3a9fa8,[self.character,a])
  if op==3:self.uc.mem_write(self.uc.reg_read(self.sp),W(c));self.invoke(0x400634,[self.inv,a,b,c]);return 0
  if op==4:self.invoke(0x4003a4,[self.inv,a,b]);return 0
  if op==5:self.uc.mem_write(self.inv+0x2e,bytes([not self.uc.mem_read(self.inv+0x2e,1)[0]]));return 0
  if op==6:self.invoke(0x3fe164,[self.inv,a]);return 0
  if op==7:
   p=self.word(self.slot(a));q=self.invoke(0x3fc3e0,[p,b]);return self.invoke(0x3ff5d4,[self.inv,q,c&1,(c>>1)&1]) if q else 0xffffffff
  if op==8:self.uc.mem_write(self.inv+0x2c,bytes([a&255]));return 0
  if op==9:self.uc.mem_write(self.character+0x1320,W(a,b));return 0
  if op==10:self.pointer(self.inv+0x28,a);return 0
  if op==11:
   p=self.word(self.slot(a));return self.invoke(0x3fdaf0,[self.inv,a])|(self.invoke(0x4001a0,[self.inv,b])<<1)
  if op==12:return self.invoke(0x3fe330,[self.inv])
  if op==13:self.uc.mem_write(self.inv+0x2f,bytes([a&255]));return 0
  if op==14:self.infinite=a;return 0
  raise ValueError(op)

def main():
 old=OriginalOwned();metadata=[]
 for i in range(1322):metadata.append([i,old.word(old.table+164*i+0x58),struct.unpack('<i',old.uc.mem_read(old.table+164*i+0x68,4))[0],old.uc.mem_read(old.table+164*i+0x1c,1)[0]])
 assert not [x for x in metadata if x[3] and x[2]!=-1 and x[1] not in (13,14)]
 stack=[925]
 original_stack=bytes(old.uc.mem_read(old.table+164*664+0x1c,1))
 cases=[];actions=0;services=0
 for k in range(84):
  synthetic=664 if (48<=k<72 or k>=80) else 0xffffffff
  old.uc.mem_write(old.table+164*664+0x1c,b"\1" if synthetic==664 else original_stack)
  cap=12 if k>=72 else (-1,0,12)[k%3];flags=k%4|(0x100 if k>=80 else 0);selected=(k//4)%2;old.fresh_inventory(cap);old.pointer(old.inv+4,0);old.uc.mem_write(old.character+0x1320,W(flags&1,flags>>1));old.uc.mem_write(old.inv+0x2e,bytes([selected]));old.pointer(old.inv+0x28,10000);old.loading=True;old.capture=True;old.requests=[];old.minimal=False;old.infinite=0;old.toggle_stats=bool(flags&0x100);old.mutation=0;old.uc.mem_write(old.seed,W(1));old.uc.mem_write(old.rngcalls,W(0));steps=[]
  todo=[(0,(165,174,213)[k%3],0,0)]
  # Build authentic starting equipment, then force-add a real stack for native
  # splitting and eventual unequip/merge/deletion on the SAME owned graph.
  starter_count=6 if k%3==2 else (4 if cap==0 else 5)
  # cap0 deletes potion, all remaining starters equippable. Other caps retain.
  for i in range(starter_count):todo.append((2,i,0,0))
  id=664 if synthetic==664 else 925
  if synthetic==664:todo.extend([(1,id,4,1),(3,1,starter_count,0),(4,1,-1,0),(5,0,0,0),(1,925,7,0),(8,12,0,0),(1,925,7,0),(6,10001,0,0),(6,-500,0,0),(10,25,0,0),(6,100,0,0),(6,-100,0,0)])
  # Additional real stackable duplicates test merging with force off. Quantity
  # remains small, avoiding unrecovered negative/assert continuation domains.
  if synthetic!=664:todo.extend([(8,12,0,0),(1,id,4,1),(7,starter_count,2,1),(1,id,3,0),(1,id,2,0),(6,10001,0,0),(6,-500,0,0),(10,25,0,0),(6,100,0,0),(6,-100,0,0)])
  else:todo.extend([(1,id,3,0),(1,id,2,0)])
  if k>=72:
   old.pointer(old.inv+4,old.character)
   if k>=80:todo=[(0,165,0,0),(1,664,4,1),(3,1,5,0),(11,5,0,0)]
   elif k<76:todo=[(1,925,1,1)]*102+[(12,0,0,0),(13,1,0,0),(12,0,0,0),(13,0,0,0),(14,1,0,0),(12,0,0,0),(14,0,0,0),(12,0,0,0)]
   else:todo=[(1,925,(0,32767,65535,65536)[k-76],1),(7,0,0,1),(7,0,32767,1),(12,0,0,0)]
  for step in todo:
   old.requests=[]
   try:result=old.perform(*step)
   except Exception:
    print('failure',k,step,'pc',hex(old.uc.reg_read(old.pc)));raise
   snap=old.snapshot();trace=b''.join(old.requests);steps.append(W(*step,result,len(snap))+snap+W(len(old.requests))+trace);actions+=1;services+=len(old.requests)
  old.loading=False;old.capture=False;cases.append(W(cap,flags,selected,synthetic,len(steps))+b''.join(steps))
 ref=ROOT/'port/game-data/reference/player-inventory-owned-v4';ref.mkdir(parents=True,exist_ok=True);gold=b'IOV4'+W(len(cases))+b''.join(cases);(ref/'fixtures.bin').write_bytes(gold)
 sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest();report={'validation':'PASS','cases':len(cases),'synchronous_cached_flags_and_selected_mutation_cases':4,'actual_metadata_cases':56,'synthetic_stackable_equipment_cases':28,'comparisons':actions,'source_effect_requests':services,'original_sha256':sha(ROOT/'.local-inputs/libDungeonHunter2.so'),'gold_sha256':sha(ref/'fixtures.bin'),'script_sha256':sha(__file__),'metadata_sha256':sha(ROOT/'.local-inputs/items-discovery/loot_table_pyarray.bin'),'scope':__doc__,'native_comparison':False};(ref/'original-gold.json').write_text(json.dumps(report,indent=2)+'\n');(ref/'actual-metadata.json').write_text(json.dumps(metadata)+'\n');print(json.dumps(report))
if __name__=='__main__':main()

