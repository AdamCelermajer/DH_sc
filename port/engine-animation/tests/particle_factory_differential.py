"""Original context/generation constructors, registry and emitter prefix vs O2 native."""
import argparse,hashlib,json,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from compiled_transforms_differential import Cpu,word,relocate,database
def words(v):return struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v])
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class FactoryCpu(Cpu):
 def external(self,uc,address,size,user):
  name=self.imports.get(address)
  if name in ('pthread_mutex_lock','pthread_mutex_unlock'):
   self.import_calls[name]=self.import_calls.get(name,0)+1;self.put(0,0);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if name in ('__aeabi_uidiv','__aeabi_uidivmod'):
   left,right=self.reg(0)&0xffffffff,self.reg(1)&0xffffffff;assert right;self.put(0,left//right)
   if name.endswith('mod'):self.put(1,left%right)
   self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,user)
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,default=REPO/'.local-inputs/fx-particle-factory-discovery/particle_factory64.so');p.add_argument('--report',type=Path,default=ROOT/'reports/particle-factory-arm64-differential.json');a=p.parse_args();started=time.monotonic();mp=ROOT/'reference/fx-particle-factory/original-functions.json';manifest=json.loads(mp.read_text());engine=REPO/'.local-inputs/libDungeonHunter2.so';assert sha(engine)==manifest['original_sha256'];old=Cpu(engine,False,manifest);new=Cpu(a.library,True,{'functions':[]})
 old=FactoryCpu(engine,False,manifest);new=FactoryCpu(a.library,True,{'functions':[]});old.pointer(0x99f698,1);gen=old.data+0x1000;context=gen+0x100;vtt=context+0x200;gvt=vtt+0x100;svt=gvt+0x100;particle=svt+0x100;scratch=particle+0x200;name=scratch+0x100;foreign=name+0x400;old.pointer(vtt,gvt);old.pointer(vtt+4,svt);old.pointer(gvt-12,context-gen);old.pointer(particle+0x178,gen)
 ni=new.data+0x1000;no=new.data+0x10000;records=[];operations={};hashes=0;calls={};source_trace=[];observe=False;prefix=False
 def ret(v=0):old.put(0,v);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def hook(uc,at,size,user):
  if at in (0x64d1d0,0x6545d0,0x64d0bc,0x63a8ec,0x651924,0x650400,0x6501d4):calls[hex(at)]=calls.get(hex(at),0)+1
  if observe and at==0x64d0bc:source_trace.append(old.string(old.reg(1)).decode('latin1'))
  if prefix and at==0x63b3d0:ret(scratch)
  elif prefix and at==0x655314:assert old.reg(1)==1;ret(gen)
  elif prefix and at==0x656500:old.uc.reg_write(old.pc,0x656c88) # Bounded prefix -> actual epilogue.
 old.uc.hook_add(UC_HOOK_CODE,hook)
 def setup(seed):
  nonlocal observe,source_trace
  source_trace=[];observe=True;old.uc.mem_write(context,seed);old.invoke(0x64d1d0,[context]);old.invoke(0x6545d0,[gen,vtt]);observe=False;assert source_trace==['AnimationDatabase','BirthRate','MaxParticles'];old.uc.mem_write(foreign,words([0x13500000+i for i in range(8)]))
 def cname(s):old.uc.mem_write(name,s+b'\0');return name
 def hash(s):return old.invoke(0x64d0bc,[context,cname(s)])
 def insert(s,ptr):old.uc.mem_write(scratch,words([hash(s),ptr]));old.invoke(0x63a8ec,[scratch+0x20,context+0x30,scratch]);return old.uc.mem_read(scratch+0x24,1)[0]
 def lookup(s):return old.invoke(0x651924,[particle,cname(s)])
 def identity(ptr):
  if not ptr:return 0
  return {gen+4:1,gen+8:2,context+0x58:3,**{foreign+i*4:4+i for i in range(8)}}[ptr]
 def finish():
  image=bytearray(old.uc.mem_read(context,92))
  for offset in (0,0x34,0x38,0x3c):struct.pack_into('<I',image,offset,0)
  return bytes(image)+bytes(old.uc.mem_read(gen+4,16))+bytes(old.uc.mem_read(foreign,32))
 def compare(op,payload,expected):
  new.uc.mem_write(ni,payload);new.uc.mem_write(no,bytes(max(4,len(expected))));length=new.invoke('dh2_particle_factory_test',[op,ni,len(payload),no],budget=30000000);actual=bytes(new.uc.mem_read(no,len(expected)));assert length==len(expected)and actual==expected,(op,length,len(expected),expected.hex(),actual.hex());records.append(words([op,len(payload)])+payload+words([len(expected)])+expected);operations[str(op)]=operations.get(str(op),0)+1
 rng=random.Random(0x28c0de)
 for _ in range(160):
  seed=bytes(rng.randrange(256)for _ in range(92));setup(seed);compare(0,seed,finish())
 names=[b'',b'AnimationDatabase',b'BirthRate',b'MaxParticles',b'EmitterType',b'RadiusLength',b'Width',b'Height',bytes.fromhex('2553482e34475a5f2e'),bytes.fromhex('6e79284d705a7e2c4a')]
 assert hash(names[-2])==hash(names[-1])==0xc66e9677
 for s in names+[bytes(rng.randrange(1,256)for _ in range(rng.randrange(1,40)))for _ in range(1400)]:compare(1,s+b'\0',words([hash(s)]));hashes+=1
 names+= [b'missing_'+str(i).encode()for i in range(9)]
 for _ in range(240):
  seed=bytes(rng.randrange(256)for _ in range(92));setup(seed);n=30;payload=seed+words([n]);expected=bytearray()
  # Force null first-insert then register collision attempts in every transcript.
  commands=[(1,names[-2],0),(0,names[-2],0),(0,names[8],2),(0,names[9],3),(1,names[9],0)]
  commands +=[(rng.randrange(3),rng.choice(names),rng.choice([0xffffffff,*range(8)]) if rng.randrange(2) else rng.getrandbits(32))for x in range(n-5)]
  for action,s,v in commands:
   if action==0:v=0xffffffff if v>7 else v
   payload+=words([action,len(s)+1,v])+s+b'\0'
   if action==0:result=insert(s,0 if v==0xffffffff else foreign+v*4)
   elif action==1:result=identity(lookup(s))
   else:old.invoke(0x650400,[context,cname(s),v]);result=0
   expected+=words([result,word(bytes(old.uc.mem_read(context+0x40,4)),0)])
  compare(2,payload,bytes(expected)+finish())
 asset=REPO/'.local-inputs/character-skeleton-fx-assets/zombie_spawn_fx.bdae';raw=asset.read_bytes();base=old.data+0x40000;root=relocate(old,raw,base);db=database(old,root,old.data+0x20000);resource_rows=[];source_prefixes=[]
 for i in range(2):
  row=old.invoke(0x60e468,[db,i]);offset=row-base;assert offset==word(raw,word(raw,32)+0x7c)+i*0x90
  uri=word(raw,offset);s=raw[uri:raw.index(0,uri)];assert old.invoke(0x61a7f4,[db,base+uri])==row
  desc=word(raw,offset+0x54);typ=word(raw,offset+8);shapecount=[3,1,2][typ];shape=word(raw,offset+12);resource_rows.append({'index':i,'name':s.decode(),'record_offset':offset,'record_sha256':hashlib.sha256(raw[offset:offset+0x90]).hexdigest(),'descriptor_offset':desc,'descriptor_sha256':hashlib.sha256(raw[desc:desc+36]).hexdigest(),'shape_offset':shape,'shape_sha256':hashlib.sha256(raw[shape:shape+shapecount*4]).hexdigest(),'factory_mode':word(raw,offset+0x50),'descriptor_type':word(raw,desc),'real_cloud_color_baker':True})
  for repeat in range(20):
   seed=bytes(rng.randrange(256)for _ in range(92));setup(seed)
   keys=[b'EmitterType',b'RadiusLength',b'Width',b'Height']
   for j,s in enumerate(keys):insert(s,foreign+j*4)
   old.pointer(particle+0x18c,row);source_trace=[];observe=True;prefix=True;old.invoke(0x656450,[particle,scratch,1]);prefix=False;observe=False
   expected_names=['EmitterType']+(['RadiusLength','Width','Height']if typ==0 else ['RadiusLength']if typ==1 else ['RadiusLength','Height'])+['MaxParticles','BirthRate'];assert source_trace==expected_names,source_trace
   if repeat==0:source_prefixes.append({'name':resource_rows[-1]['name'],'ordered_setters':source_trace})
   trace=[]
   for key in expected_names[:-2]:trace.extend([hash(key.encode()),word(bytes(old.uc.mem_read(foreign+keys.index(key.encode())*4,4)),0)])
   expected=words([len(trace)]+trace+[offset])+raw[offset:offset+0x90]+raw[desc:desc+36]+words([shapecount])+raw[shape:shape+shapecount*4]+finish();compare(3,seed+words([i,len(raw)])+raw,expected)
 gold=words([0x31434650,len(records)])+b''.join(records);gp=ROOT/'reference/fx-particle-factory/particle-factory-fixtures.bin';gp.write_bytes(gold)
 sources=[ROOT/'particle_factory.hpp',ROOT/'particle_factory.cpp',ROOT/'tests/particle_factory.cpp',Path(__file__),REPO/'port/engine-resources/resources.hpp',REPO/'port/engine-resources/resources.cpp'];report={'validation':'PASS','original_sha256':sha(engine),'original_manifest_sha256':sha(mp),'arm64_library_sha256':sha(a.library),'reference_sha256':sha(gp),'source_sha256':{str(s.relative_to(REPO)).replace('\\','/'):sha(s)for s in sources},'comparisons':len(records),'operation_counts':operations,'original_source_calls':calls,'hash_comparisons':hashes,'registry_transcript_commands':240*30,'hash_collision':'2553482e34475a5f2e /6e79284d705a7e2c4a =>c66e9677','actual_fx77_emitter_inputs':resource_rows,'original_ordered_prefixes':source_prefixes,'original_instructions_executed':True,'compiled_arm64_instructions_executed':True,'constructor_registration_order':['AnimationDatabase','BirthRate','MaxParticles'],'oracle_services':['initialized hashsingletonguard','single-thread pthread allocator locks return0','ABI unsigned division','required preconstructed cloud return','borrowed other-model parameter storage'],'prefix_boundary':'656500 redirects to real epilogue656c88 after BirthRate, before Life','mismatches':0,'resource_sha256':sha(asset),'scope':'Real context/generation fields, hash-keyed unique registry/null lookup/word setters and original initParticleSystem through BirthRate prefix. Original cloud construction return is a required preconstructed context/generation service fixture; other model storage providers are explicit borrowed cells. Full cloud model/simulation/baker/material/rendering, native AnimationDatabase pointer mapping and attachment receivers remain required. Native shared leases separately audited.','elapsed_seconds':round(time.monotonic()-started,2)};a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items()if k not in ('source_sha256','scope','actual_fx77_emitter_inputs','original_ordered_prefixes')}))
if __name__=='__main__':main()
