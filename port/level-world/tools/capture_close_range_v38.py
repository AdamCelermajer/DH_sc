"""Execute original whole3d63d8 with explicit handle/range/debug endpoints."""
from pathlib import Path
import sys,struct,random,json,hashlib
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(root/'port/game-data/tests'));sys.path.insert(0,str(root/'port/level-world/tests'))
from items_differential import Original,words
from unicorn import UC_HOOK_CODE
engine=root/'.local-inputs/libDungeonHunter2.so';manifest=json.loads((root/'port/level-world/reference/character-target-update/helpers/original-functions.json').read_text())
old=Original(engine,manifest);owner=old.data+0x10000;ai=owner+0x3c8;target=old.data+0x14000;vt=old.data+0x8000;cb=[old.data+0x9000+j*16 for j in range(2)]
old.uc.mem_write(owner,bytes(0x1800));old.uc.mem_write(target,bytes(0x1800));old.pointer(owner,vt);old.pointer(target,vt);old.pointer(vt+0x90,cb[0]);old.pointer(vt+0x128,cb[1]);old.pointer(ai+4,owner)
for address in cb:old.uc.mem_write(address,bytes.fromhex('1eff2fe1'))
minimum=0
def hook(uc,address,size,unused):
 if address==0x33ff8c:old.returned(target)
 elif address==cb[0]:old.returned(8)
 elif address==cb[1]:
  uc.mem_write(old.reg(1),words(minimum));uc.mem_write(old.reg(2),words(1000));uc.mem_write(old.reg(3),words(4));old.returned(1)
 elif address in (0x337888,0x3140ec,0x318254):old.returned()
 elif address==0x337a88:old.returned(0)
old.uc.hook_add(UC_HOOK_CODE,hook)
rng=random.Random(38);cases=[]
for m in (0,1,10,-10,32768,65536,65537,-2147483648):
 for p in ((0,0,0),(m,0,0),(0,0,m),(3,4,0),(float('nan'),0,0)):cases.append(((0,0,0),p,m))
for _ in range(120):cases.append((tuple(rng.uniform(-1000,1000) for _ in range(3)),tuple(rng.uniform(-1000,1000) for _ in range(3)),rng.choice((1,10,100,1000,32768,65537,-77))))
records=[]
for a,b,minimum in cases:
 payload=struct.pack('<6fi',*a,*b,minimum)
 old.uc.mem_write(owner+0x160,payload[:12]);old.uc.mem_write(target+0x160,payload[12:24]);result=old.invoke(0x3d63d8,[ai,target]);assert result in (0,1)
 records.append(payload+words(result))
dest=root/'port/level-world/reference/target-facing-v38';dest.mkdir(parents=True,exist_ok=True);corpus=words(len(records))+b''.join(records);(dest/'close-range-original.bin').write_bytes(corpus)
report={'cases':len(records),'original_sha256':hashlib.sha256(engine.read_bytes()).hexdigest(),'corpus_sha256':hashlib.sha256(corpus).hexdigest(),'scope':__doc__,'endpoint_fixtures':['Handle.AsCharacter','interaction8','Character range parameters','absent debug file'],'original_GetTargetPosition_and_distance_execute':True}
(dest/'original-capture.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
