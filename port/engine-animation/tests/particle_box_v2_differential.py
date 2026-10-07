from pathlib import Path
import sys,struct,random,json,hashlib
r=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(r/'port/engine-animation/tests'))
scope={};exec((r/'port/engine-animation/tests/particle_cloud_models_v1_differential.py').read_text().split('old=ModelCpu')[0].replace('ROOT=Path(__file__).resolve().parents[1]','ROOT=Path(r"'+str(r/'port/engine-animation')+'")'),scope)
Cpu=scope['ModelCpu'];old=Cpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});new=Cpu(r/'.local-inputs/particle_box_v2_arm64.so',True,{'functions':[]})
model=old.data+0x1000;matrix=model+0x100;seed=matrix+0x100;out=seed+0x100
nm=new.data+0x1000;size=nm+0x100;nmat=size+0x100;ns=nmat+0x100;no=ns+0x100
rng=random.Random(0xB022026);cases=0
for i in range(500):
 dims=[rng.choice([0.,20.,200.,101.425,rng.uniform(-100,100)]) for _ in range(3)];s=struct.pack('<3f',*dims)
 m=[rng.uniform(-2,2) for _ in range(16)];m[3]=m[7]=m[11]=0;m[15]=1;mat=struct.pack('<16f',*m);initial=rng.randrange(1,2147483647)
 old.uc.mem_write(model,bytes(92));old.invoke(0x69b354,[model,*struct.unpack('<3I',s)])
 new.uc.mem_write(size,s);assert new.invoke('dh2_particle_box_construct_v2',[nm,size])==0
 assert bytes(old.uc.mem_read(model+4,12))==bytes(new.uc.mem_read(nm+12,12))
 old.uc.mem_write(matrix,mat);old.invoke(0x69efac,[model,matrix],budget=500000)
 new.uc.mem_write(nmat,mat);assert new.invoke('dh2_particle_box_transform_v2',[nm,nmat])==0
 assert bytes(old.uc.mem_read(model+4,12))==bytes(new.uc.mem_read(nm+12,12)),('transform origin',i)
 assert bytes(old.uc.mem_read(model+0x38,36))==bytes(new.uc.mem_read(nm+24,36)),('transform edges',i)
 old.pointer(seed,initial);new.uc.mem_write(ns,struct.pack('<i',initial));old.invoke(0x69b534,[out,model,seed],budget=500000)
 assert new.invoke('dh2_particle_box_generate_v2',[no,nm,ns])==0
 assert bytes(old.uc.mem_read(out,12))==bytes(new.uc.mem_read(no,12)),('generate',i)
 assert bytes(old.uc.mem_read(seed,4))==bytes(new.uc.mem_read(ns,4));cases+=1
report=dict(validation='PASS',comparisons=cases,scope='Whole original PDBox float ctor69b354, transform69efac, generate69b534 including real PSRandom sequential draws versus compiledARM64. Finite authored/random matrices, zero/positive/negative dimensions. No particle factory/GL claim.',sources={str(p.relative_to(r)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [r/'port/engine-animation/particle_box_v2.cpp',r/'.local-inputs/libDungeonHunter2.so',r/'.local-inputs/particle_box_v2_arm64.so']})
(r/'port/engine-animation/reports/particle-box-v2-original.json').write_text(json.dumps(report,indent=2)+'\n');print(report)
