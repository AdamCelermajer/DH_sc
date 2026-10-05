from pathlib import Path
import sys,struct,json,hashlib
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
read=lambda a:struct.unpack('<I',c.uc.mem_read(a,4))[0]
put=lambda a,v:c.uc.mem_write(a,struct.pack('<I',v&0xffffffff))
base=(read(0x38bd54)+0x38bc50)&0xffffffff
network=read(base+read(0x38bd58));local=read(base+read(0x38bd60));counts=read(base+read(0x38bd5c))
cases=0
for mode in (0,1):
 for seed in (0,1,14348906,14348907,0xffffffff,0x80000000,123456789):
  put(local,seed);put(network,seed);put(counts,0xfffffffd);put(counts+4,17)
  expected=seed;counter=17 if mode else 0xfffffffd
  for _ in range(100):
   expected=((expected*59051+177149)&0xffffffff)%14348907;counter=(counter+1)&0xffffffff
   actual=c.invoke(0x38bc3c,[mode]);assert actual==expected%99
   assert read(network if mode else local)==expected
   assert read(counts+4*mode)==counter
   assert read(local if mode else network)==seed;cases+=1
report=dict(validation='PASS',whole_original_generate_spawn_calls=cases,native_differential=False,
 original_sha256=hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),
 seeds='separate original local/network globals',source_initial_data_zero=True)
(root/'port/level-world/reports/character-world-npc-spawn-v1-original-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
