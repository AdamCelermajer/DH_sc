"""Actual Potion0 lookup/ItemInstance ctor/force-add, SetQty and remove/delete
ownership instructions. Item localization/stats/reqs/fullness/destruction are
the same explicit controlled service boundaries as inventory load proof.
"""
import json,hashlib,random,struct,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from item_inventory_v1_original import Inventory,ROOT,W
def main():
 c=Inventory();r=random.Random(20261004);cases=[];comparisons=0;calls=0
 literal=0x3ffca8+c.word(0x3ffd18);assert c.cstring(literal)==b'Potion0'
 for k in range(48):
  cap=r.choice([-1,0,5]);c.fresh_inventory(cap);ops=[]
  for j in range(16):
   kind=j%3==2;value=r.choice([0,1,2,32767,32768,65535,65536,0x7fffffff]);c.trace=[];c.loading=True
   c.invoke(0x3fe878 if kind else 0x3ffc40,[c.inv] if kind else [c.inv,value],budget=20000000);c.loading=False
   snap=c.snapshot_inventory();ops.append(W(int(kind),value,len(snap))+snap+W(len(c.trace))+b''.join(c.trace));comparisons+=1;calls+=len(c.trace)
  cases.append(W(cap,len(ops))+b''.join(ops))
 gold=b'POT1'+W(len(cases))+b''.join(cases);ref=ROOT/'port/game-data/reference/item-inventory-v1';(ref/'potion-fixtures.bin').write_bytes(gold)
 report={'validation':'PASS','comparisons':comparisons,'ordered_required_services':calls,'original_sha256':hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'gold_sha256':hashlib.sha256(gold).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'source_literal':{'address':hex(literal),'value':'Potion0'},'scope':__doc__,'native_comparison':False};(ref/'original-potion-gold.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
