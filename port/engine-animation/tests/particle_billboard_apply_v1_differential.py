from pathlib import Path
import sys,struct,random,json,hashlib
sys.path.insert(0,str(Path(__file__).parent))
exec(Path(__file__).with_name('particle_cloud_models_v1_differential.py').read_text().split('rng=random.Random')[0])
new=ModelCpu(REPO/'.local-inputs/particle_billboard_v1_arm64.so',True,{'functions':[]})
rng=random.Random(0xA1FA2026);records=[]
for trial in range(500):
 n=rng.randrange(31);local=trial%2;camera=struct.pack('<3f',*([rng.uniform(-10,10) for _ in range(3)]));p=bytearray(n*100)
 for i in range(n):
  struct.pack_into('<25f',p,i*100,*[rng.uniform(-10,10) for _ in range(25)])
  if trial%5==0:struct.pack_into('<3f',p,i*100,1,2,3) # equal-distance identities
 old.uc.mem_write(model+0x60,camera);old.uc.mem_write(particle,bytes(p));old.uc.mem_write(matrix,identity+bytes([0]));old.uc.mem_write(context+0x54,bytes([local]))
 old.invoke(0x651eec,[model,particle,particle+n*100],budget=5000000)
 expected=bytes(old.uc.mem_read(model+0x74,24))+bytes(old.uc.mem_read(particle,n*100))
 payload=words([n])+camera+identity+words([local])+bytes(p);new.uc.mem_write(ni,payload);length=new.invoke('dh2_particle_billboard_apply_test_v1',[ni,no],budget=1000000);actual=bytes(new.uc.mem_read(no,length))
 assert actual==expected,(trial,n,next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),None))
 records.append(payload+expected)
out=ROOT/'reference/particle-cloud-models-v1';gold=out/'billboard-apply-fixtures.bin';gold.write_bytes(b''.join(records));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();report=dict(validation='PASS',comparisons=len(records),original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),arm64_sha256=sha(REPO/'.local-inputs/particle_billboard_v1_arm64.so'),reference_sha256=sha(gold),scope='Whole applyPRenderData n0..30 including same-vector source AlphaSort equal-distance identities, source camera distance and BBox, actual local-space branch.')
(ROOT/'reports/particle-billboard-apply-v1-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
