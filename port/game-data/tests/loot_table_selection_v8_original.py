"""Whole original _AddLootTable against actual cache, explicit Debug/counts.
No item creation/death/application visual or RNG constructor claim.
"""
import sys,random,json,hashlib
from pathlib import Path
R=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(R/'port/game-data/tests'))
from loot_entry_selection_v8_original import EntryOriginal,W
def main():
 old=EntryOriginal();base=0x4039b8+old.word(0x404040);count=old.word(old.word(base+old.word(0x404048)))
 rng=random.Random(0x4c545338);cases=[]
 for i in range(128):
  id=rng.randrange(count);seed=rng.getrandbits(32);calls=rng.getrandbits(32);infinite=int(i%31==0);counts=[rng.randrange(4)for _ in range(3)]
  out=old.data+0x70000;old.uc.mem_write(out,W(0,0,0));old.infinite=infinite;old.counts=counts;old.transcript=[];old.random(seed,calls);old.selecting=True
  try:old.invoke(0x4039a0,[id,out,0],budget=10000000)
  finally:old.selecting=False
  begin,end=old.word(out),old.word(out+4);n=(end-begin)//4;assert n<10000
  entries=b''.join(bytes(old.uc.mem_read(old.word(begin+4*j)+4,32))for j in range(n))
  expected=old.rng()+W(n)+entries+W(len(old.transcript),*old.transcript)
  cases.append(W(id,seed,calls,infinite,*counts,len(expected))+expected)
 ref=R/'port/game-data/reference/loot-table-selection-v8';ref.mkdir(parents=True,exist_ok=True)
 blob=b'LTS8'+W(len(cases))+b''.join(cases);(ref/'fixtures.bin').write_bytes(blob)
 report={'validation':'PASS','original_cases':len(cases),'actual_loot_tables':count,'original_sha256':hashlib.sha256((R/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'fixture_sha256':hashlib.sha256(blob).hexdigest(),'scope':__doc__}
 (ref/'original-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
