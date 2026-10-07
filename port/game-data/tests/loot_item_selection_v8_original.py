"""Whole source _AddLootItems on entries selected from original actual-cache tables.
Explicit Debug fixture; item creation, death and world pickup are outside scope.
"""
import sys,struct,json,hashlib
from pathlib import Path
R=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(R/'port/game-data/tests'))
from loot_entry_selection_v8_original import EntryOriginal,W
def main():
 old=EntryOriginal();blob=(R/'port/game-data/reference/loot-table-selection-v8/fixtures.bin').read_bytes();at=8;cases=[]
 def u():
  nonlocal at
  v=struct.unpack_from('<I',blob,at)[0];at+=4;return v
 for i in range(struct.unpack_from('<I',blob,4)[0]):
  header=[u()for _ in range(8)];nbytes=header[-1];expected=blob[at:at+nbytes];at+=nbytes
  seed,calls,n=struct.unpack_from('<III',expected);entries=expected[12:12+n*32]
  for all_items in (0,1):
   p=old.data+0x30000;ptrs=old.data+0x40000;vector=old.data+0x50000;out=old.data+0x60000;storage=old.data+0x70000
   for j in range(n):old.uc.mem_write(p+j*36,W(0)+entries[j*32:(j+1)*32])
   old.uc.mem_write(ptrs,W(*(p+j*36 for j in range(n))));old.uc.mem_write(vector,W(ptrs,ptrs+n*4,ptrs+n*4))
   old.uc.mem_write(out,W(storage,storage,storage+0x20000));old.random(seed,calls);old.infinite=0;old.counts=[0,0,0];old.transcript=[];old.selecting=True
   try:old.invoke(0x402dfc,[vector,out,all_items],budget=10000000)
   finally:old.selecting=False
   count=(old.word(out+4)-storage)//16;assert count<8192
   infos=[]
   for j in range(count):
    b=bytes(old.uc.mem_read(storage+j*16,16));id=struct.unpack_from('<h',b)[0];entry=(struct.unpack_from('<I',b,4)[0]-p)//36
    infos.append(W(id,entry,b[8]))
   result=old.rng()+W(count)+b''.join(infos)+W(len(old.transcript),*old.transcript)
   cases.append(W(seed,calls,all_items,n)+entries+W(len(result))+result)
 ref=R/'port/game-data/reference/loot-item-selection-v8';ref.mkdir(parents=True,exist_ok=True)
 output=b'LIS8'+W(len(cases))+b''.join(cases);(ref/'fixtures.bin').write_bytes(output)
 report=dict(validation='PASS',original_cases=len(cases),fixture_sha256=hashlib.sha256(output).hexdigest(),original_sha256=hashlib.sha256((R/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),scope=__doc__)
 (ref/'original-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
