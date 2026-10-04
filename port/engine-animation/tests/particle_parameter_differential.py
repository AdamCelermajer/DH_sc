"""Original CFloatEx instructions versus optimized native particle parameter kernels."""
import argparse,hashlib,json,random,struct,sys,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from compiled_transforms_differential import Cpu,word
from animation_blend_differential import bits
def words(v):return struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v])
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def nan(v):return v&0x7f800000==0x7f800000 and v&0x7fffff!=0
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,default=REPO/'.local-inputs/fx-particle-animation-discovery/particle_parameter64.so');p.add_argument('--report',type=Path,default=ROOT/'reports/particle-parameter-arm64-differential.json');p.add_argument('--reference-output',type=Path,default=ROOT/'reference/fx-particle-animation/particle-parameter-fixtures.bin');a=p.parse_args();start=time.monotonic()
 engine=REPO/'.local-inputs/libDungeonHunter2.so';mp=ROOT/'reference/fx-particle-animation/original-functions.json';manifest=json.loads(mp.read_text());assert sha(engine)==manifest['original_sha256'];old=Cpu(engine,False,manifest);new=Cpu(a.library,True,{'functions':[]})
 out=old.data+0x1000;acc=out+0x100;record=acc+0x100;sampler=record+0x100;dataset=sampler+0x100;values=dataset+0x100;weights=values+0x1000;obj=weights+0x1000
 old.uc.mem_write(acc,words([record,dataset]));old.pointer(record+8,sampler);old.pointer(dataset+8,values);old.pointer(sampler+24,0);old.pointer(obj,0x98b2d0+8)
 ni=new.data+0x1000;no=new.data+0x10000;records=[];counts={};fx=0
 def compare(op,payload,expected,label):
  new.uc.mem_write(ni,payload);new.uc.mem_write(no,bytes(4));status=new.invoke('dh2_particle_parameter_test',[op,ni,len(payload),no]);actual=word(bytes(new.uc.mem_read(no,4)),0)
  assert status==0 and (expected==actual or (op not in (0,5) and nan(expected) and nan(actual))),(label,op,hex(expected),hex(actual),status)
  records.append(words([op,len(payload)])+payload+words([0,expected]));counts[str(op)]=counts.get(str(op),0)+1
 def sample(op,v,k,n,r,f,label):
  old.uc.mem_write(values,words(v));old.pointer(dataset+4,len(v));old.uc.mem_write(out,b'abcd')
  if op==0:old.invoke(0x6e32cc,[obj,acc,k,out])
  elif op==1:old.invoke(0x6e3374,[acc,k,n,f,out])
  elif op==2:old.invoke(0x6e3288,[acc,k,n,out])
  else:old.invoke(0x6e32f4,[acc,r,k,n,f,out])
  compare(op,words([len(v),k,n,r,f]+v),word(bytes(old.uc.mem_read(out,4)),0),label)
 raw=(REPO/'.local-inputs/character-skeleton-fx-assets/zombie_spawn_fx.bdae').read_bytes();root=word(raw,32);lib=word(raw,root+48);seg=word(raw,lib+4);data=word(raw,seg+20);rec=word(raw,root+40);tracks=[]
 fractions=[bits(x)for x in (-2.,-0.,0.,0.125,0.5,0.999,1.,2.)]+[0x7f800000,0xff800000,0x7fc12345,0x7fa00123]
 for index in (1,2):
  row=rec+index*32;sam=word(raw,row+8);vi=word(raw,sam+24);vector=data+4+vi*8;count=word(raw,vector);at=vector+4+struct.unpack_from('<i',raw,vector+4)[0];v=list(struct.unpack_from('<'+'I'*count,raw,at));ch=word(raw,row+16);nameat=word(raw,ch+4);name=raw[nameat:raw.index(0,nameat)].decode();tracks.append({'uri':name,'type':word(raw,ch+8),'key_count':count,'values_sha256':hashlib.sha256(words(v)).hexdigest()});assert tracks[-1]['type']==28
  for k in range(count):
   sample(0,v,k,k,k,0,name);fx+=1
   for n in range(count):
    for f in fractions:sample(1,v,k,n,k,f,name);fx+=1
 pool=[bits(x)for x in (0.,-0.,0.5,1.,-1.,2.,256.,1e30,1e-38)]+[0x7f800000,0xff800000,0x7fc12345,0x7fa00123,0x00000001,0x80000001]
 rng=random.Random(0x28b17)
 for _ in range(2200):
  v=[rng.choice(pool)if rng.randrange(2)else rng.getrandbits(32)for _ in range(rng.randrange(1,9))];k,n,r=[rng.randrange(len(v))for _ in range(3)];op=rng.randrange(4);sample(op,v,k,n,r,rng.choice(fractions),'IEEE source')
 for _ in range(1800):
  count=rng.choice([-3,-1,0,1,1,2,3,4,7]);n=max(0,count);v=[rng.choice(pool)for _ in range(n)];w=[rng.choice(pool)for _ in range(n)];old.uc.mem_write(values,words(v)or bytes(4));old.uc.mem_write(weights,words(w)or bytes(4));old.invoke(0x6e3124,[obj,values,weights,count,out]);expected=word(bytes(old.uc.mem_read(out,4)),0)
  old.invoke(0x6e3184,[obj,values,weights,count,out]);assert word(bytes(old.uc.mem_read(out,4)),0)==expected
  compare(4,words([count]+v+w),expected,'blend and add')
 for v in pool+[rng.getrandbits(32)for _ in range(300)]:
  old.uc.mem_write(values,words([v]));old.invoke(0x6e3258,[obj,values,out,0]);compare(5,words([v]),word(bytes(old.uc.mem_read(out,4)),0),'raw apply')
 guards=new.invoke('dh2_particle_parameter_test_guards',[]);assert guards==16;gold=words([0x31505046,len(records)])+b''.join(records);a.reference_output.write_bytes(gold)
 sources=[ROOT/'particle_parameter.hpp',ROOT/'particle_parameter.cpp',ROOT/'tests/particle_parameter.cpp',Path(__file__)];report={'validation':'PASS','original_sha256':sha(engine),'original_manifest_sha256':sha(mp),'arm64_library_sha256':sha(a.library),'reference_sha256':sha(a.reference_output),'source_sha256':{str(x.relative_to(REPO)).replace('\\','/'):sha(x)for x in sources},'comparisons':len(records),'operation_counts':counts,'actual_fx77_key_interpretations':fx,'actual_fx77_tracks':tracks,'atomic_guards':guards,'mismatches':0,'original_instructions_executed':True,'compiled_arm64_instructions_executed':True,'asset_sha256':sha(REPO/'.local-inputs/character-skeleton-fx-assets/zombie_spawn_fx.bdae'),'scope':'CFloatEx keys, interpolation, delta, weighted blend/add and raw4-byte resolved parameter application. Copy bits exact; arithmetic NaNs compare classification, all other words exact. Original accessor getters execute. Parameter map/factory/emission/attachments/rendering excluded.','elapsed_seconds':round(time.monotonic()-start,2)};a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items()if k not in ('scope','source_sha256','actual_fx77_tracks')}))
if __name__=='__main__':main()
