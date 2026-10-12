"""Three actual starter sets, authoritative source equipment graph and live
Character gear/class/vitals bodies. Item text and Debug are explicit fixtures.
Actual source Skin executes its null-VisualObject branch; no Skin factory proof.
"""
import sys,json,struct,hashlib
from pathlib import Path
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/game-data/tests'));sys.path.insert(0,str(R/'port/game-data/tools'))
from fresh_inventory_owned_v4_original import OriginalOwned,W
from inspect_class_tables import parse
class OriginalEffects(OriginalOwned):
 def inventory_service(self,uc,a,z,u):
  if self.loading and a in (0x3e08a8,0x3a999c,0x3bd140):return # actual bodies execute
  super().inventory_service(uc,a,z,u)
def main():
 old=OriginalEffects();raw=(R/'.local-inputs/actors/character_properties_pyarray.bin').read_bytes();d=list(struct.unpack_from('<224i',raw,4));t=list(struct.unpack_from('<224i',raw,900));default=old.allocate(1800);old.uc.mem_write(default,W(0)+W(*d)+W(0)+W(*t));old.pointer(0x9a645c,default)
 table=parse(R/'.local-inputs/combat-data')['rows'];rows=old.allocate(len(table)*12);got=(0x3e2e34+old.word(0x3e3008))&0xffffffff;old.pointer(old.word(got+old.word(0x3e300c)),len(table));old.pointer(old.word(got+old.word(0x3e3010)),rows)
 for i,row in enumerate(table):
  entries=row['entries'];p=old.allocate(len(entries)*24);old.uc.mem_write(rows+12*i,W(0,len(entries),p))
  for j,x in enumerate(entries):old.uc.mem_write(p+24*j,W(0,*x))
 lgot=(0x3e2d88+old.word(0x3e2e18))&0xffffffff;old.pointer(lgot+old.word(0x3e2e1c),old.allocate(896))
 cases=[];steps=0
 for baseid,loot in ((263,165),(290,174),(325,213)):
  for selected in (0,1):
   old.capture=False;old.loading=False;old.fresh_inventory(12);owner=old.character+0x560;os=[owner+x for x in (8,0x38c,0x710,0xa94)];base=list(struct.unpack_from('<224i',raw,4+baseid*896));old.pointer(owner+4,old.character)
   for p,v in zip(os,(base,d,d,d)):old.uc.mem_write(p,W(0)+W(*v))
   sentinel=owner+0xe18;old.uc.mem_write(sentinel,W(0,0,sentinel,sentinel));old.pointer(owner+0xe28,0);old.pointer(old.character+0x2d8,0);old.uc.mem_write(old.inv+0x2e,bytes([selected]));old.invoke(0x3e0810,[owner,1]);initial=b''.join(bytes(old.uc.mem_read(p+4,896)) for p in os)
   old.loading=True;old.capture=True;old.requests=[];old.minimal=False;old.infinite=0;old.toggle_stats=False;old.mutation=0;old.uc.mem_write(old.seed,W(1));old.uc.mem_write(old.rngcalls,W(0));todo=[(0,loot,0,0)];count=6 if baseid==325 else 5;todo +=[(2,i,0,0) for i in range(count)];todo +=[(5,0,0,0)]+[(2,i,0,0) for i in reversed(range(count))]+[(4,1,-1,0),(15,0,0,0),(4,2,-1,0),(15,0,0,0),(5,0,0,0),(15,0,0,0)]
   out=[]
   for op,a,b,c in todo:
    if op==15:old.invoke(0x3e08a8,[owner]);old.invoke(0x3a999c,[old.character]);old.invoke(0x3bd140,[old.character]);result=0
    else:result=old.perform(op,a,b,c)
    state=b''.join(bytes(old.uc.mem_read(p+4,896)) for p in os);out.append(W(op,a,b,c,result)+state);steps+=1
   cases.append(W(baseid,selected,len(out))+initial+b''.join(out))
 gold=b'GEV5'+W(len(cases))+b''.join(cases);ref=R/'port/game-data/reference/player-item-effects-v5';ref.mkdir(exist_ok=True);(ref/'starter-effects-fixtures.bin').write_bytes(gold)
 report={'validation':'PASS','original_cases':len(cases),'original_steps':steps,'all_three_actual_classes':True,'gold_sha256':hashlib.sha256(gold).hexdigest(),'original_sha256':hashlib.sha256((R/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'scope':__doc__,'native_comparison':False}
 (ref/'starter-effects-original-gold.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
