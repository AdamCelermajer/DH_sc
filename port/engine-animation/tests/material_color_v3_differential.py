from pathlib import Path
import sys,struct,random,json,hashlib
r=Path(__file__).resolve().parents[3];sys.path.insert(0,str(r/'port/engine-animation/tests'))
from compiled_transforms_differential import Cpu
old=Cpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});new=Cpu(r/'.local-inputs/material_color_v3_arm64.so',True,{'functions':[]})
out=old.data+0x1000;accessor=out+0x100;record=accessor+0x100;sampler=record+0x100;dataset=sampler+0x100;keys=dataset+0x100
words=lambda x:struct.pack('<'+'I'*len(x),*x)
old.uc.mem_write(accessor,words([record,dataset]));old.pointer(record+8,sampler);old.pointer(dataset+8,keys);old.pointer(sampler+24,0)
na=new.data+0x1000;nk=na+0x100;no=nk+0x100;rng=random.Random(0xc0103);cases=[]
actual=bytes.fromhex('000000ff513000ffa25f00ffa25f00ff764400ff1f0d00ff000000ff000000ff')
for i in range(600):
 data=actual if i<200 else bytes(rng.randrange(256)for _ in range(32));k=rng.randrange(7);n=k+1;fraction=rng.choice([0.,.125,.5,.999,1.,-2.,2.,256.]);bits=struct.unpack('<I',struct.pack('<f',fraction))[0]
 old.uc.mem_write(keys,data);old.pointer(dataset+4,8);old.uc.mem_write(out,bytes(4));old.invoke(0x6268b0,[accessor,k,n,bits,out]);expected=bytes(old.uc.mem_read(out,4))
 new.uc.mem_write(nk,data);new.uc.mem_write(na,struct.pack('<QI4x',nk,8));assert new.invoke('dh2_material_color_v3_oracle',[no,na,k,n,bits])==0
 observed=bytes(new.uc.mem_read(no,4));assert observed==expected,(i,k,n,fraction,expected.hex(),observed.hex(),old.import_calls);cases.append(words([k,n,bits])+data+expected)
(r/'port/engine-animation/reference/particle-cloud-models-v1/material-color-v3-gold.bin').write_bytes(words([0x33434d46,len(cases)])+b''.join(cases))
report=dict(validation='PASS',comparisons=len(cases),actual_fire_key_cases=200,mismatches=0,scope='Full uchar4 source6268b0 including actual accessor and f2uiz versus compiledARM64. FIRE actual key bytes and random keys/extrapolation. Material setter reused independently original-proven module; no GPU claim.',sources={str(p.relative_to(r)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [r/'port/engine-animation/material_color_v3.cpp',r/'.local-inputs/libDungeonHunter2.so',r/'.local-inputs/material_color_v3_arm64.so']})
(r/'port/engine-animation/reports/material-color-v3-original.json').write_text(json.dumps(report,indent=2)+'\n');print(report)
