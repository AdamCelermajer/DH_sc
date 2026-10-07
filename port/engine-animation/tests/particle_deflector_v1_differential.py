from pathlib import Path
import sys,struct,random,json,math,hashlib
REPO=Path(__file__).resolve().parents[3];BASE=REPO/'port/engine-animation'
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
sys.path.insert(0,str(BASE/'tests'))
# Reuse the existing independent Bionic/libm oracle ABI projections without
# executing the other suite's main body or changing its historical fixtures.
path=BASE/'tests/particle_cloud_models_v1_differential.py';namespace={'__file__':str(path)}
exec(path.read_text().split('old=ModelCpu')[0],namespace)
ModelCpu=namespace['ModelCpu'];words=namespace['words']
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_S0
class DeflectorCpu(ModelCpu):
 def external(self,uc,a,z,u):
  name=self.imports.get(a)
  if name in ('logf','expf'):
   raw=uc.reg_read(UC_ARM64_REG_S0)if self.arm64 else self.reg(0)
   value=struct.unpack('<f',words([raw]))[0];result=struct.unpack('<I',struct.pack('<f',getattr(math,name[:-1])(value)))[0]
   if self.arm64:uc.reg_write(UC_ARM64_REG_S0,result)
   else:self.put(0,result)
   uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,a,z,u)
old=DeflectorCpu(REPO/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
new=DeflectorCpu(REPO/'.local-inputs/particle_deflector_v1_arm64.so',True,{'functions':[]})
native_random_calls=[]
def native_trace(uc,a,z,u):
 if a==new.symbols.get('dh2_particle_random_v1'):native_random_calls.append((new.reg(0),bytes(uc.mem_read(new.reg(0),4)).hex()))
new.uc.hook_add(UC_HOOK_CODE,native_trace)
model=old.data+0x1000;params=model+0x100;current=params+0x100;context=current+0x100;vt=context+0x100;seed=vt+0x100;stub=seed+0x100;particle=stub+0x100
old.pointer(context,vt);old.pointer(vt+0x18,stub)
def callback(uc,a,z,u):
 if a==stub:old.put(0,seed);uc.reg_write(old.pc,uc.reg_read(old.lr))
old.uc.hook_add(UC_HOOK_CODE,callback)
ni=new.data+0x1000;no=ni+0x10000
rng=random.Random(0xDEF1EC70);records=[]
for trial in range(160):
 n=rng.randrange(0,8);dt=rng.choice([0.01,.1,.5,1.0]);initial_seed=rng.randrange(1,2147483647)
 p=[rng.choice([0,.27,.8]),rng.choice([0,.2]),rng.choice([0,.2,.7]),rng.choice([0,.5,1]),rng.choice([0,1,.3]),rng.choice([1,1000]),rng.choice([1,1000])]
 m=[1,0,0,0,0,1,0,0,0,0,1,0,rng.uniform(-1,1),rng.uniform(-1,1),rng.uniform(-1,1),1]
 if trial%3==1:m=[1,0,0,0,0,0,-1,0,0,1,0,0,m[12],m[13],m[14],1]
 prev=m.copy();prev[12]+=rng.uniform(-.2,.2);prev[13]+=rng.uniform(-.2,.2);prev[14]+=rng.uniform(-.2,.2)
 raw=bytearray(n*100)
 for i in range(n):
  for k in range(25):struct.pack_into('<f',raw,i*100+4*k,rng.uniform(-3,3))
  for k in range(3):struct.pack_into('<f',raw,i*100+4*k,m[12+k]+rng.uniform(-.5,.5));struct.pack_into('<f',raw,i*100+12+4*k,rng.uniform(-20,20))
 old.pointer(params,current);old.uc.mem_write(params+4,struct.pack('<7f',*p));old.uc.mem_write(current,struct.pack('<16fB',*prev,1))
 old.invoke(0x6349a0,[model,params]);old.uc.mem_write(current,struct.pack('<16fB',*m,1));old.pointer(context+0x50,struct.unpack('<I',struct.pack('<f',dt))[0]);old.pointer(seed,initial_seed);old.uc.mem_write(particle,bytes(raw))
 old.invoke(0x6338e0,[model,particle,particle+n*100,context],budget=5000000)
 expected=words([0,struct.unpack('<I',old.uc.mem_read(seed,4))[0]])+bytes(old.uc.mem_read(model+4,65))+bytes(old.uc.mem_read(current,65))+bytes(2)+bytes(old.uc.mem_read(particle,n*100))
 payload=words([n,initial_seed])+struct.pack('<f7f16f16f',dt,*p,*prev,*m)+bytes(raw)
 new.uc.mem_write(ni,payload);length=new.invoke('dh2_particle_deflector_test_v1',[ni,len(payload),no],budget=5000000);actual=bytes(new.uc.mem_read(no,length))
 if actual!=expected:
  at=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),None)
  print(json.dumps({'trial':trial,'initial_seed':initial_seed,'native_random_calls':native_random_calls,'count':n,'params':p,'dt':dt,'difference':at,'differences':[i for i,(a,b)in enumerate(zip(actual,expected))if a!=b],'input_vectors':[struct.unpack_from('<6f',raw,i*100)for i in range(n)],'expected_vectors':[struct.unpack_from('<6f',expected,140+i*100)for i in range(n)],'actual_vectors':[struct.unpack_from('<6f',actual,140+i*100)for i in range(n)]}));raise AssertionError('Deflector differential mismatch')
 records.append(words([len(payload)])+payload+words([len(expected)])+expected)
out=BASE/'reference/particle-deflector-v1';gold=out/'original-fixtures.bin';gold.write_bytes(words([0x31464544,len(records)])+b''.join(records))
report={'validation':'PASS','comparisons':len(records),'original_instructions_executed':True,'compiled_arm64_instructions_executed':True,
 'reference_sha256':hashlib.sha256(gold.read_bytes()).hexdigest(),
 'original_sha256':hashlib.sha256((REPO/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),
 'arm64_sha256':hashlib.sha256((REPO/'.local-inputs/particle_deflector_v1_arm64.so').read_bytes()).hexdigest(),
 'source_sha256':{str(p.relative_to(REPO)).replace('\\','/'):hashlib.sha256(p.read_bytes()).hexdigest()for p in (BASE/'particle_deflector_v1.hpp',BASE/'particle_deflector_v1.cpp',Path(__file__),BASE/'tests/particle_deflector_v1_bridge.cpp')},
 'scope':'whole constructor and SParticle apply including history/markers/RNG over explicit live-matrix and seed fixture borrows; modeled libm, no historical Bionic libm parity claim'}
(out/'differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
