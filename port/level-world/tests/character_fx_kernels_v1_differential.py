"""Actual AnimFX data and texture interpreter instructions versus optimized ARM64.

Imported IEEE softfloat/libm and material-set service are explicit contracts.
Real shipping swoosh BRES accessors execute the full original key interpreter.
"""
import argparse,ctypes,hashlib,json,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_S0
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/engine-animation/tests'))
from compiled_transforms_differential import Cpu as Base,relocate,word,words,i32,bits
from animation_blend_differential import same_word
class Cpu(Base):
 def __init__(self,*a):
  super().__init__(*a);f=self.crt.cosf;f.argtypes=[ctypes.c_float];f.restype=ctypes.c_float;self.libm['cosf']=f
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='sincosf':
   v=uc.reg_read(UC_ARM64_REG_S0);number=struct.unpack('<f',words([v]))[0]
   sine=bits(self.libm['sinf'](number));cosine=bits(self.libm['cosf'](number));self.trig.extend(((3,v,cosine),(0,v,sine)));uc.mem_write(self.reg(0),words([sine]));uc.mem_write(self.reg(1),words([cosine]));self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif name=='cosf':
   v=uc.reg_read(UC_ARM64_REG_S0) if self.arm64 else self.reg(0);out=bits(self.libm[name](struct.unpack('<f',words([v]))[0]));self.trig.append((3,v,out))
   if self.arm64:uc.reg_write(UC_ARM64_REG_S0,out)
   else:self.put(0,out)
   self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:return super().external(uc,address,size,unused)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--gold',type=Path,required=True);a=p.parse_args();start=time.monotonic();scratch=R/'.local-inputs/character-fx-owner-v1'
 manifests=[json.loads((scratch/k/'original-functions.json').read_text())for k in ('lifecycle','texture','texture-matrix')];manifest={'original_sha256':manifests[0]['original_sha256'],'functions':sum([x['functions']for x in manifests],[])}
 old=Cpu(R/'.local-inputs/libDungeonHunter2.so',False,manifest);new=Cpu(a.library,True,{'functions':[]});assert sha(R/'.local-inputs/libDungeonHunter2.so')==manifest['original_sha256']
 rng=random.Random(20261118);records=[];counts={'data':0,'scalar':0,'matrix':0,'shipping_sampler':0};mats=[]
 def equal(x,y,copy=False):return len(x)==len(y)and all(same_word(a,b,copy)for a,b in zip(struct.unpack('<'+'I'*(len(x)//4),x),struct.unpack('<'+'I'*(len(y)//4),y)))
 for j in range(1024):
  flags=[rng.randrange(256)for _ in range(3)];speed=bits(rng.uniform(-10,10));loop=rng.choice((-1,0,1,2,0x7fffffff,-2147483648));setloop=rng.choice((-1,0,1,2,0x7fffffff,-2147483648));type=rng.randrange(4);duration=i32(rng.getrandbits(32))
  s,info,data,out=[old.data+x for x in (0x1000,0x2000,0x3000,0x4000)];step=s+0x100;old.uc.mem_write(s,bytes(512));old.pointer(s+16,step);old.uc.mem_write(s+20,words([type]));old.uc.mem_write(step+12,words([loop&0xffffffff]));old.uc.mem_write(step+16,bytes(flags[:2]));old.uc.mem_write(step+20,words([duration&0xffffffff]));old.uc.mem_write(step+32,bytes([flags[2]]));old.uc.mem_write(step+36,words([speed]));old.pointer(info,s);old.uc.mem_write(data,bytes(48));old.uc.mem_write(data+12,words([setloop&0xffffffff]));old.uc.mem_write(out,bytes(20));old.invoke(0x4933e4,[out,0,info,data]);b=bytes(old.uc.mem_read(out,20));expected=struct.pack('<3I',*b[:3])+b[4:16]+struct.pack('<Q',0xabcdef1234567890)
  no=new.data+0x4000;ns=new.data+0x1000;new.uc.mem_write(ns,struct.pack('<3IIii',*flags,speed,loop,duration));assert new.invoke('dh2_fx_data_v1',[no,ns,type,setloop&0xffffffff,0xabcdef1234567890])==0;actual=bytes(new.uc.mem_read(no,32));assert actual==expected,(j,actual.hex(),expected.hex());records.append(words([0,type,setloop&0xffffffff])+bytes(new.uc.mem_read(ns,24))+expected);counts['data']+=1
 # Scalar source methods use original accessor getOutputVector, no algorithm hook.
 for j in range(1024):
  vals=[bits(rng.uniform(-1e6,1e6))for _ in range(5)];t=bits(rng.uniform(-2,2));ref,key,nxt=[rng.randrange(5)for _ in range(3)]
  acc,animation,blob,values,out=[old.data+x for x in (0x1000,0x2000,0x3000,0x4000,0x5000)];old.uc.mem_write(animation,bytes(512));old.pointer(animation+8,animation+0x100);old.pointer(acc,animation);old.pointer(acc+4,blob);old.uc.mem_write(blob,words([1,5,values]));old.uc.mem_write(values,words(vals));old.invoke(0x6e3d30,[acc,0,key,nxt,t,out]);expected=bytes(old.uc.mem_read(out,4));nv=new.data+0x4000;no=new.data+0x5000;new.uc.mem_write(nv,words(vals));new.uc.reg_write(UC_ARM64_REG_S0,t);assert new.invoke('dh2_fx_texture_between_v1',[no,nv,5,key,nxt])==0;assert equal(expected,bytes(new.uc.mem_read(no,4))),(j,expected.hex(),bytes(new.uc.mem_read(no,4)).hex());records.append(words([1,5,key,nxt,t])+words(vals)+expected);counts['scalar']+=1
  old.invoke(0x6e3ac4,[acc,0,ref,key,nxt,t,out]);expected=bytes(old.uc.mem_read(out,4));new.uc.reg_write(UC_ARM64_REG_S0,t);assert new.invoke('dh2_fx_texture_delta_v1',[no,nv,5,ref,key,nxt])==0;assert equal(expected,bytes(new.uc.mem_read(no,4)));records.append(words([2,5,ref,key,nxt,t])+words(vals)+expected);counts['scalar']+=1
 # Actual applyValue builds the matrix; only its genuine material setter is a fixture.
 def hook(uc,address,size,user):
  if address==0x5cb4dc:mats.append(bytes(uc.mem_read(old.reg(3),68)));old.put(0,1);uc.reg_write(old.pc,uc.reg_read(old.lr))
 old.uc.hook_add(UC_HOOK_CODE,hook)
 for j in range(512):
  value=words([bits(rng.uniform(-4,4)),bits(rng.uniform(-4,4)),bits(rng.uniform(-720,720)),bits(rng.uniform(-4,4)),bits(rng.uniform(-4,4))]);od=old.data+0x5000;oi=od+0x100;old.uc.mem_write(od,value);old.uc.mem_write(oi,bytes(16));old.trig=[];old.invoke(0x6e38c4,[0,od,oi]);expected=mats[-1];trace=old.trig[:];nd=new.data+0x5000;no=nd+0x100;new.uc.mem_write(nd,value);new.trig=[];assert new.invoke('dh2_fx_texture_matrix_v1',[no,nd])==0;actual=bytes(new.uc.mem_read(no,68));assert equal(expected[:64],actual[:64])and expected[64]==actual[64],('matrix',j,expected.hex(),actual.hex());assert trace==new.trig;records.append(words([3])+value+expected+words([len(trace)])+b''.join(words(x)for x in trace));counts['matrix']+=1
 for asset in (1,2,3):
  raw=(scratch/'cache'/f'swoosh_prince_1hand_combo_0{asset}.bdae').read_bytes();base=old.data+0x10000;relocate(old,raw,base);root=word(raw,32);lib=word(raw,root+48);seg=word(raw,lib+4);data=word(raw,seg+12 if word(raw,seg+8)==0 else seg+20)
  for v in range(word(raw,data)):
   slot=data+8+8*v;old.pointer(base+slot,base+slot+i32(word(raw,slot)))
  for ai in range(word(raw,root+36)):
   rec=word(raw,root+40)+32*ai;ch=word(raw,rec+16)
   if word(raw,ch+8)!=87:continue
   accessor=old.data+0x1000;old.uc.mem_write(accessor,words([base+rec,base+data,0,0]));out=old.data+0x5000;cursor=out+0x100;newbase=new.data+0x10000;new.uc.mem_write(newbase,raw);no=new.data+0x5000
   end=i32(word(raw,seg+4));queries=sorted(set([-1,0,1,end-1,end,end+1]+list(range(0,end+1,7))))
   for ms in queries:
    for interpolate in (0,1):
     old.uc.mem_write(out,bytes(20));old.uc.mem_write(cursor,words([0x12345678]));old.invoke(0x6e3d78,[accessor,ms&0xffffffff,out,cursor,interpolate]);expected=bytes(old.uc.mem_read(out,20));assert old.uc.mem_read(cursor,4)==words([0x12345678]);assert new.invoke('dh2_fx_texture_sample_test_v1',[no,newbase,len(raw),ai,0,ms&0xffffffff,interpolate])==0;actual=bytes(new.uc.mem_read(no,20));assert expected==actual,('sample',asset,ai,ms,interpolate,expected.hex(),actual.hex());records.append(words([4,asset,ai,ms&0xffffffff,interpolate])+expected);counts['shipping_sampler']+=1
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(words([0x31475846,len(records)])+b''.join(records));sources=[R/'port/level-world'/s for s in ('character_fx_kernels_v1.hpp','character_fx_kernels_v1.cpp','fx_texture_animation_v1.hpp','fx_texture_animation_v1.cpp')]+[Path(__file__)];report={'validation':'PASS','original_sha256':manifest['original_sha256'],'arm64_library_sha256':sha(a.library),'reference_sha256':sha(a.gold),'comparisons':len(records),'counts':counts,'mismatches':0,'source_sha256':{x.relative_to(R).as_posix():sha(x)for x in sources},'original_instruction_execution':True,'optimized_arm64_instruction_execution':True,'services':'Identical UCRT cosf/sinf and imported IEEE helpers; material setter only captures original matrix. STL/allocator/native guards are port contracts.','scope':__doc__,'elapsed_seconds':round(time.monotonic()-start,3)};a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
