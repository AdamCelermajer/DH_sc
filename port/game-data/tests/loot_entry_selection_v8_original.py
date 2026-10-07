"""Whole original entry helpers, with explicit Debug/PlayerManager fixtures.
No full AddLoot, death, visual pickup, or application RNG seed producer claim.
"""
import sys,struct,random,json,hashlib
from pathlib import Path
R=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(R/'port/game-data/tests'))
from loot_power_creation_v7_differential import OriginalLoot,W
from unicorn import UC_HOOK_CODE
class EntryOriginal(OriginalLoot):
 def __init__(self):
  self.selecting=False
  super().__init__();self.uc.hook_add(UC_HOOK_CODE,self.entry_hook)
 def entry_hook(self,uc,a,z,u):
  if not self.selecting:return
  if a==0x337888:self.transcript.append(0);self.returned()
  elif a==0x337a88:
   key=self.stringvalue(self.reg(1));assert key in (b'InfiniteLootDrops',b'isTracingItemPctRoll',b'isTracingItemInventory_Loot')
   self.transcript.append(1 if key==b'InfiniteLootDrops' else 2 if key==b'isTracingItemPctRoll' else 6);self.returned(self.infinite if key==b'InfiniteLootDrops' else 0)
  elif a in (0x36eac8,0x36eac0,0x36eab8):
   slot=(0x36eac8,0x36eac0,0x36eab8).index(a);self.transcript.append(3+slot);self.returned(self.counts[slot])
 def run(self,op,entries,seed,calls,infinite,counts):
  p=self.data+0x30000;ptrs=self.data+0x40000;vector=self.data+0x50000
  for i,entry in enumerate(entries):self.uc.mem_write(p+i*36,W(0,*entry))
  self.uc.mem_write(ptrs,W(*(p+i*36 for i in range(len(entries)))))
  self.uc.mem_write(vector,W(ptrs,ptrs+4*len(entries),ptrs+4*len(entries)))
  loot=self.data+0x60000;self.uc.mem_write(loot,W(0,0,0,len(entries),p,0,0,0,0))
  self.infinite=infinite;self.counts=counts;self.transcript=[];self.random(seed,calls);self.selecting=True
  try:result=self.invoke((0x4027a4,0x402854,0x402bdc,0x4028d0,0x402a60)[op],[vector if op==3 else loot if op==4 else p],budget=2000000)
  finally:self.selecting=False
  return W(result)+self.rng()+W(len(self.transcript),*self.transcript)
def main():
 old=EntryOriginal();rng=random.Random(0x4c455638);cases=[]
 for i in range(1000):
  op=i%5;n=rng.randrange(1,8) if op>=3 else 1
  entries=[]
  for j in range(n):
   probability=rng.choice([0,1,50,99,100,101,256,-1]);entries.append([0,0,rng.randrange(1,90),0,probability,rng.randrange(1,90),rng.randrange(1,90),rng.randrange(1,90)])
  seed=rng.getrandbits(32);calls=rng.getrandbits(32);infinite=int(i%19==0);counts=[rng.randrange(0,4) for _ in range(3)]
  expected=old.run(op,entries,seed,calls,infinite,counts)
  cases.append(W(op,n,seed,calls,infinite,*counts)+b''.join(W(*v)for v in entries)+W(len(expected))+expected)
 ref=R/'port/game-data/reference/loot-entry-selection-v8';ref.mkdir(parents=True,exist_ok=True)
 blob=b'LES8'+W(len(cases))+b''.join(cases);(ref/'fixtures.bin').write_bytes(blob)
 report={'validation':'PASS','original_cases':len(cases),'original_sha256':hashlib.sha256((R/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'fixture_sha256':hashlib.sha256(blob).hexdigest(),'scope':__doc__}
 (ref/'original-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
