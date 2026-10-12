"""Original cached scalar accessor + actual vector emission versus isolated O2."""
import argparse,hashlib,json,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from particle_factory_differential import FactoryCpu,words
from compiled_transforms_differential import word,relocate,database
from animation_blend_differential import bits
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,default=REPO/'.local-inputs/fx-particle-emission-discovery/particle_emission64.so');p.add_argument('--report',type=Path,default=ROOT/'reports/particle-emission-arm64-differential.json');a=p.parse_args();start=time.monotonic();mp=ROOT/'reference/fx-particle-emission/original-functions.json';manifest=json.loads(mp.read_text());original=REPO/'.local-inputs/libDungeonHunter2.so';old=FactoryCpu(original,False,manifest);new=FactoryCpu(a.library,True,{'functions':[]});old.pointer(0x99f698,1)
 gen=old.data+0x1000;ctx=gen+0x100;vt=ctx+0x100;vector=vt+0x1000;old.pointer(gen,vt);old.pointer(vt-12,ctx-gen);ni=new.data+0x1000;no=new.data+0x20000;records=[];counts={};capture=None;captured=None;stack_seed=old.stack+0xe000-136+4;calls={}
 def hook(uc,at,size,user):
  nonlocal captured
  if at in (0x65317c,0x65311c,0x64fc50,0x66a1a8,0x6e2c40,0x6e3374,0x6e32cc):calls[hex(at)]=calls.get(hex(at),0)+1
  if at==0x65327c:captured=bytes(old.uc.mem_read(old.reg(2),100))
 old.uc.hook_add(UC_HOOK_CODE,hook)
 def reset(count=0):
  old.uc.mem_write(ctx+0x24,words([vector,vector+100*count,vector+100*max(count,1)]));old.uc.mem_write(vector,bytes([0xad])*(100*max(count,1)));old.uc.mem_write(gen+4,words([bits(1),1,0,0]))
 def invoke(rate,maximum,now,last,seed):
  nonlocal captured
  old.pointer(gen+4,rate);old.pointer(gen+8,maximum);old.pointer(ctx+0x48,now);old.pointer(ctx+0x4c,last);old.uc.mem_write(stack_seed,seed);captured=None
  count=(word(bytes(old.uc.mem_read(ctx+0x28,4)),0)-word(bytes(old.uc.mem_read(ctx+0x24,4)),0))//100;first=old.invoke(0x65317c,[gen],budget=3000000);begin,end=struct.unpack('<2I',bytes(old.uc.mem_read(ctx+0x24,8)));assert first>=begin and (first-begin)%100==0;return words([(first-begin)//100,(end-begin)//100]),bytes(old.uc.mem_read(begin,end-begin)),captured
 def compare(op,payload,expected):
  new.uc.mem_write(ni,payload);new.uc.mem_write(no,bytes(len(expected)));length=new.invoke('dh2_particle_emission_test',[op,ni,len(payload),no],budget=30000000);actual=bytes(new.uc.mem_read(no,len(expected)));assert length==len(expected)and actual==expected,(op,length,len(expected),next((i for i,(x,y)in enumerate(zip(actual,expected))if x!=y),None),actual[:48].hex(),expected[:48].hex());records.append(words([op,len(payload)])+payload+words([len(expected)])+expected);counts[str(op)]=counts.get(str(op),0)+1
 rng=random.Random(0x28e177)
 for i in range(500):
  count=rng.randrange(40);maximum=rng.randrange(40);rate=bits(rng.choice([0.25,1,2,4,10,32]));carry=bits(rng.uniform(-0.5,0.99));now=bits(rng.uniform(10,20));last=bits(rng.uniform(0,1));seed=bytes(rng.randrange(256)for _ in range(100));reset(count);old.pointer(gen+0x10,carry);state=words([rate,maximum,0x12345678,carry,now,last,count,0]);r,data,template=invoke(rate,maximum,now,last,seed);assert template is not None
  fields=bytes(old.uc.mem_read(gen+4,16));expected=fields+words([now,last,word(r,4),0])+r+template;compare(0,state+seed,expected)
 for _ in range(120):
  reset();old.invoke(0x64fc50,[gen]);seed=bytes(rng.randrange(256)for _ in range(100));n=12;payload=words([n])+seed;expected=bytearray()
  for j in range(n):
   rate=bits(rng.choice([-8,-1,0,0.5,1,8,32]));maximum=rng.randrange(1,32);now=bits(rng.choice([-0.25,0,0.125,0.5,1,2]));last=bits(rng.choice([0,0.125,1]));payload+=words([rate,maximum,now,last]);r,data,_=invoke(rate,maximum,now,last,seed);expected+=bytes(old.uc.mem_read(gen+4,16))+r+words([word(r,4)])+data
  compare(2,payload,bytes(expected))
 asset=REPO/'.local-inputs/character-skeleton-fx-assets/zombie_spawn_fx.bdae';raw=asset.read_bytes();base=old.data+0x40000;root=relocate(old,raw,base);db=database(old,root,old.data+0x30000);data=word(raw,word(raw,word(raw,32)+48)+4)+20;dataset=word(raw,data)
 for i in range(word(raw,dataset)):
  slot=dataset+8+i*8;offset=struct.unpack_from('<i',raw,slot)[0];old.pointer(base+slot,base+slot+offset)
 accessor=old.data+0x6000;out=accessor+0x100;cursor=out+0x100;old.pointer(0x9f8350,0x98b2d8);rec=word(raw,word(raw,32)+40);sampler_cases=0
 for ti,ai in enumerate((1,2)):
  record=rec+32*ai;old.pointer(base+record+20,0x9f8350);old.uc.mem_write(accessor,words([base+record,base+dataset]));sampler=word(raw,record+8);count=word(raw,dataset+4+8*word(raw,sampler+24));queries=list(range(-10,2201,7))+[0,33,66,100,133,200,333,500,666,1000,1333,2000]
  for ms in queries:
   hint=rng.randrange(count);old.pointer(cursor,hint);old.pointer(out,0x12345678);old.invoke(0x66a1a8,[accessor,ms,out,cursor,1]);expected=bytes(old.uc.mem_read(cursor,4))+bytes(old.uc.mem_read(out,4));compare(1,words([ti,ms,hint,len(raw)])+raw,expected);sampler_cases+=1
  synthetic=bytearray(raw);struct.pack_into('<I',synthetic,sampler,1);old.pointer(base+sampler,1)
  for ms in queries[::3]:
   hint=rng.randrange(count);old.pointer(cursor,hint);old.pointer(out,0x12345678);old.invoke(0x66a1a8,[accessor,ms,out,cursor,1]);expected=bytes(old.uc.mem_read(cursor,4))+bytes(old.uc.mem_read(out,4));compare(1,words([ti,ms,hint,len(synthetic)])+synthetic,expected)
  old.pointer(base+sampler,word(raw,sampler))
 gold=ROOT/'reference/fx-particle-emission/particle-emission-fixtures.bin';gold.write_bytes(words([0x314d4550,len(records)])+b''.join(records));sources=[ROOT/'particle_emission.hpp',ROOT/'particle_emission.cpp',ROOT/'tests/particle_emission.cpp',Path(__file__),REPO/'port/asset-payloads/payloads.cpp',REPO/'port/engine-resources/resources.cpp',ROOT/'particle_factory.cpp',ROOT/'particle_parameter.cpp'];report={'validation':'PASS','original_sha256':sha(original),'original_manifest_sha256':sha(mp),'arm64_library_sha256':sha(a.library),'reference_sha256':sha(gold),'resource_sha256':sha(asset),'source_sha256':{str(p.relative_to(REPO)).replace('\\','/'):sha(p)for p in sources},'comparisons':len(records),'operations':counts,'actual_fx_cached_samples':sampler_cases,'source_vector_sequence_steps':120*12,'original_source_calls':calls,'original_instructions_executed':True,'compiled_arm64_instructions_executed':True,'mismatches':0,'scope':'Original SParticle100 generation vector resize, delta/remainder/cap arithmetic and explicit untouched-template seed; actual FX77 type28 cached scalar accessor sampling. Finite bounded conversion domain. No simulation/model initialization/material/render or raw AnimationDatabase ownership claim. Allocator/libc/imported IEEE services are explicit.','elapsed_seconds':round(time.monotonic()-start,2)};a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items()if k not in ('source_sha256','scope')}))
if __name__=='__main__':main()
