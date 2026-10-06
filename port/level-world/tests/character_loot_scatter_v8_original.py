from pathlib import Path
import sys,struct,random,json,hashlib
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(root/'port/game-data/tests'))
from loot_power_creation_v7_differential import OriginalLoot,W
from unicorn import UC_HOOK_CODE
class Scatter(OriginalLoot):
 def __init__(self):
  self.scattering=False;super().__init__();self.uc.hook_add(UC_HOOK_CODE,self.scatter_hook)
 def scatter_hook(self,uc,a,z,u):
  # Test domain deliberately supplies unit delta: fixture normalization is
  # identity. This does not prove Normalize or global static initialization.
  if self.scattering and a==0x34d0b0:self.returned()
old=Scatter();rng=random.Random(0x53434138);rows=[]
for i in range(256):
 source=[float(rng.randrange(-500,501))for _ in range(3)]
 direction=[0.,0.,0.];direction[(i//2)%3]=1. if i%4 else -1.
 killer=[source[n]+direction[n]for n in range(3)]
 basis=[0.,0.,1.];seed=rng.getrandbits(32);calls=rng.getrandbits(32);has=i%2
 p=old.data+0x70000;q=p+0x200;out=q+0x200
 old.uc.mem_write(p+0x160,struct.pack('<3f',*source));old.uc.mem_write(q+0x160,struct.pack('<3f',*killer));old.uc.mem_write(0x99f878,struct.pack('<3f',*basis));old.random(seed,calls)
 old.scattering=True
 try:old.invoke(0x3ec668,[out,p,q if has else 0])
 finally:old.scattering=False
 rows.append(W(seed,calls,has)+struct.pack('<9f',*source,*killer,*basis)+bytes(old.uc.mem_read(out,12))+old.rng())
ref=root/'port/level-world/reference/character-loot-world-v8';blob=b'LSC8'+W(len(rows))+b''.join(rows)
(ref/'scatter-fixtures.bin').write_bytes(blob)
report=dict(validation='PASS',cases=len(rows),original_sha256=hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(blob).hexdigest(),normalization_and_basis_fixtures=True,production_world=False)
(ref/'scatter-original-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
