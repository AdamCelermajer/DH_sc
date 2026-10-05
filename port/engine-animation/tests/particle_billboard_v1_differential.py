from pathlib import Path
import sys,struct,random,json,hashlib
sys.path.insert(0,str(Path(__file__).parent))
# Shared original ABI services and actual translation-unit startup producers.
exec(Path(__file__).with_name('particle_cloud_models_v1_differential.py').read_text().split('rng=random.Random')[0])
new=ModelCpu(REPO/'.local-inputs/particle_billboard_v1_arm64.so',True,{'functions':[]})
base=0x64dcf0+u32(0x64e3c0);global_basis=u32(base+u32(0x64e3c4))
old.uc.mem_write(context+4,bytes(32))
rng=random.Random(0xB1112026);records=[]
for trial in range(500):
 # Real orthogonal views, supplied caller authority; include nonzero spin.
 angle=rng.uniform(-3,3);c=math.cos(angle);s=math.sin(angle)
 view=struct.pack('<16f',c,s,0,0,-s,c,0,0,0,0,1,0,1,2,3,1)
 p=bytearray(100);struct.pack_into('<2f',p,0x50,rng.choice([0,rng.uniform(-6,6)]),rng.choice([-1,0,1]))
 old.uc.mem_write(matrix,view);old.uc.mem_write(particle,bytes(p))
 old.invoke(0x64dcd8,[context,matrix],budget=1000000)
 expected=bytes(old.uc.mem_read(global_basis+0x18,24))
 old.invoke(0x6c0238,[context,particle],budget=1000000)
 expected+=bytes(old.uc.mem_read(global_basis+0x30,48))
 new.uc.mem_write(ni,view+bytes(p));length=new.invoke('dh2_particle_billboard_test_v1',[ni,no],budget=1000000);actual=bytes(new.uc.mem_read(no,length))
 assert actual==expected,(trial,next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),None),actual.hex(),expected.hex())
 records.append(view+bytes(p)+expected)
out=ROOT/'reference/particle-cloud-models-v1';gold=out/'billboard-fixtures.bin';gold.write_bytes(b''.join(records));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
report=dict(validation='PASS',comparisons=len(records),original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),arm64_sha256=sha(REPO/'.local-inputs/particle_billboard_v1_arm64.so'),reference_sha256=sha(gold),scope='Whole unlocked camera-facing per-system basis and contextbyte20=0 per-particle billboard corners including actual source quaternion spin. Explicit borrowed camera/world services; GPU streams/material/UV/indices outside comparison.')
(ROOT/'reports/particle-billboard-v1-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
