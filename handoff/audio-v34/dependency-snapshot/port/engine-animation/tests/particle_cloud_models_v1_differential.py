"""Whole reached SParticle model methods vs compiled ARM64, actual PSRandom."""
from pathlib import Path
import sys,struct,random,json,hashlib
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];sys.path.insert(0,str(ROOT/'tests'))
from particle_factory_differential import FactoryCpu,words
from unicorn import UC_HOOK_CODE
import math
from unicorn.arm64_const import UC_ARM64_REG_D0,UC_ARM64_REG_S0
class ModelCpu(FactoryCpu):
 def external(self,uc,a,size,user):
  name=self.imports.get(a)
  if name=='sincosf' and self.arm64:
   value=struct.unpack('<f',words([uc.reg_read(UC_ARM64_REG_S0)]))[0];uc.mem_write(self.reg(0),struct.pack('<f',math.sin(value)));uc.mem_write(self.reg(1),struct.pack('<f',math.cos(value)));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if name in ('cosf','sinf'):
   bits=uc.reg_read(UC_ARM64_REG_S0) if self.arm64 else self.reg(0);value=struct.unpack('<f',words([bits]))[0];result=struct.unpack('<I',struct.pack('<f',getattr(math,name[:-1])(value)))[0]
   if self.arm64:uc.reg_write(UC_ARM64_REG_S0,result)
   else:self.put(0,result)
   uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if name=='sincos' and self.arm64:
   value=struct.unpack('<d',struct.pack('<Q',uc.reg_read(UC_ARM64_REG_D0)))[0];uc.mem_write(self.reg(0),struct.pack('<d',math.sin(value)));uc.mem_write(self.reg(1),struct.pack('<d',math.cos(value)));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if name=='__aeabi_d2f':self.put(0,struct.unpack('<I',struct.pack('<f',struct.unpack('<d',words([self.reg(0),self.reg(1)]))[0]))[0])
  elif name=='__aeabi_f2d':
   lo,hi=struct.unpack('<2I',struct.pack('<d',struct.unpack('<f',words([self.reg(0)]))[0]));self.put(0,lo);self.put(1,hi)
  elif name in ('sqrt','cos','sin'):
   value=struct.unpack('<d',struct.pack('<Q',uc.reg_read(UC_ARM64_REG_D0)))[0] if self.arm64 else struct.unpack('<d',words([self.reg(0),self.reg(1)]))[0];result=struct.pack('<d',getattr(math,name)(value));lo,hi=struct.unpack('<2I',result)
   if self.arm64:uc.reg_write(UC_ARM64_REG_D0,struct.unpack('<Q',result)[0])
   else:self.put(0,lo);self.put(1,hi)
  else:return super().external(uc,a,size,user)
  uc.reg_write(self.pc,uc.reg_read(self.lr))
old=ModelCpu(REPO/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});new=ModelCpu(REPO/'.local-inputs/particle_cloud_models_v1_arm64.so',True,{'functions':[]})
def u32(a):return struct.unpack('<I',old.uc.mem_read(a,4))[0]
def f32(v):return struct.pack('<f',v)
# Real translation-unit startup producers initialize the shared half vectors.
# __cxa_atexit only registers unrelated static destruction and has no vector writes.
def startup_service(uc,a,size,user):
 if a==0x30e304:old.put(0,0);uc.reg_write(old.pc,uc.reg_read(old.lr))
old.uc.hook_add(UC_HOOK_CODE,startup_service)
old.invoke(0x64eeb4,[],budget=100000)
old.invoke(0x69bf84,[],budget=100000)
assert bytes(old.uc.mem_read(0x9f6fd4+0x1c,12))==struct.pack('<3f',0.5,0.5,0.5)
assert bytes(old.uc.mem_read(0x9f75e8,12))==struct.pack('<3f',0.5,0.5,0.5)
model=old.data+0x1000;context=model+0x100;vt=context+0x100;seed=vt+0x100;particle=seed+0x100;matrix=particle+8000;stub=matrix+0x100;params=stub+0x100;ni=new.data+0x1000;no=ni+10000
old.pointer(model,vt);old.pointer(vt-12,context-model);old.pointer(context,vt);old.pointer(vt+0x18,stub);old.pointer(vt+0x20,stub+4)
def callback(uc,a,size,user):
 if a in (stub,stub+4):old.put(0,seed if a==stub else matrix);uc.reg_write(old.pc,uc.reg_read(old.lr))
old.uc.hook_add(UC_HOOK_CODE,callback)
methods=[0x64c2fc,0x64c3f4,0x64bd1c,0x64be08,0x64c190,0x64c25c,0x64e3c8,0x653bb8,0x69c6ec,0x633624]
identity=struct.pack('<16f',1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1)
rng=random.Random(0xC10D2026);counts={};records=[]
for op in range(10):
 for trial in range(100):
  n=1 if op==8 else rng.randrange(0,16);initial_seed=rng.randrange(1,2147483647);dt=rng.choice([0,0.01,0.25,1,2]);config=bytearray(128);particles=bytearray(n*100)
  for i in range(n):
   for k in range(25):struct.pack_into('<f',particles,i*100+4*k,rng.uniform(-10,10))
   struct.pack_into('<2f',particles,i*100+0x3c,rng.uniform(-1,3),rng.uniform(0.1,4))
  if op==0:struct.pack_into('<2f',config,0,rng.choice([0.1,0.5,2]),rng.choice([0,0.1,0.8]))
  elif op in (2,3):struct.pack_into('<4f',config,0,rng.choice([1,200,-10]),rng.choice([0,0.2,1]),rng.choice([0,0.1,1]),rng.choice([0,0.1,2]))
  elif op==6:struct.pack_into('<4fI4f',config,0,rng.choice([0,1,2]),rng.choice([0,0.15,1]),rng.choice([0,1.5]),rng.choice([0,1]),0,0,0,1,0)
  elif op==7:struct.pack_into('<6f',config,0,0,0,1,rng.choice([0,0.3,1]),360,0.15)
  elif op==8:struct.pack_into('<6fI',config,0,0,0,0,1,0,1,0)
  elif op==9:struct.pack_into('<2fI',config,0,1,0,0)
  config=bytes(config)
  old.uc.mem_write(particle,bytes(particles));old.pointer(seed,initial_seed);old.pointer(context+0x50,struct.unpack('<I',f32(dt))[0]);old.uc.mem_write(matrix,identity+bytes([0]));old.uc.mem_write(model+4,bytes(64))
  if op in (0,2,3):old.uc.mem_write(model+4,config[:8 if op==0 else 16])
  elif op==6:
   old.uc.mem_write(model+4,config[:16]);old.uc.mem_write(model+0x14,config[20:32]);old.uc.mem_write(model+0x20,config[32:36]);old.uc.mem_write(model+0x24,config[16:20])
  elif op==7:old.uc.mem_write(model+4,config[:24])
  elif op==8:
   old.uc.mem_write(model+4,config[:12]);old.uc.mem_write(model+0x10,config[12:20]);old.uc.mem_write(model+0x20,config[20:24]);old.uc.mem_write(model+0x28,config[24:25])
  elif op==9:
   old.pointer(model,params);old.pointer(params,matrix);old.uc.mem_write(params+4,config[:12])
  old.invoke(methods[op],[particle,model,seed] if op==8 else [model,particle,particle+n*100,context] if op==9 else [model,particle,particle+n*100],budget=5000000)
  expected=words([0,u32(seed)])+bytes(old.uc.mem_read(particle,n*100));payload=words([n,initial_seed])+f32(dt)+bytes(config)+identity+bytes(particles)
  new.uc.mem_write(ni,payload);length=new.invoke('dh2_particle_cloud_models_test_v1',[op,ni,len(payload),no],budget=5000000);actual=bytes(new.uc.mem_read(no,length))
  assert actual==expected,(op,trial,n,next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),None),actual[:108].hex(),expected[:108].hex())
  records.append(words([op,len(payload)])+payload+words([len(expected)])+expected);counts[str(op)]=counts.get(str(op),0)+1
out=ROOT/'reference/particle-cloud-models-v1';out.mkdir(exist_ok=True);gold=out/'original-fixtures.bin';gold.write_bytes(words([0x314c434d,len(records)])+b''.join(records));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();report=dict(validation='PASS',comparisons=len(records),operations=counts,original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),arm64_sha256=sha(REPO/'.local-inputs/particle_cloud_models_v1_arm64.so'),reference_sha256=sha(gold),original_instructions_executed=True,compiled_arm64_instructions_executed=True,scope='Whole reached Life/Size/Spin/Motion/Sphere/directionalGravity methods over explicit model parameters, actual PSRandom. Virtual seed/world matrix callbacks are explicit borrowed fixture producers. No whole cloud/render claim.')
(ROOT/'reports/particle-cloud-models-v1-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
