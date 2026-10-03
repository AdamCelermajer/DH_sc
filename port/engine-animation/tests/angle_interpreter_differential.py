"""Original float angle key wrappers and genuine angle-axis conversion vs ARM64.

Only sinf/cosf imported libm is modeled with the same UCRT on both CPUs.
No timeline/search/default producer or missing-default stack behavior is invented.
"""
import argparse,ctypes,json,math,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from compiled_transforms_differential import Cpu,bits,floating,sha,word,words,i32
from animation_blend_differential import same_word
from unicorn.arm64_const import UC_ARM64_REG_S0

class AngleCpu(Cpu):
 def __init__(self,*args):
  super().__init__(*args);self.cosf=self.crt.cosf;self.cosf.argtypes=[ctypes.c_float];self.cosf.restype=ctypes.c_float
 def external(self,uc,address,size,user):
  if self.imports.get(address)=='sincosf':
   assert self.arm64
   value=uc.reg_read(UC_ARM64_REG_S0);sine=bits(self.libm['sinf'](floating(value)));cosine=bits(self.cosf(floating(value)))
   self.trig.extend(((0,value,sine),(3,value,cosine)));uc.mem_write(self.reg(0),words([sine]));uc.mem_write(self.reg(1),words([cosine]));self.import_calls['sincosf']=self.import_calls.get('sincosf',0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if self.imports.get(address)!='cosf':return super().external(uc,address,size,user)
  value=uc.reg_read(UC_ARM64_REG_S0) if self.arm64 else self.reg(0);output=bits(self.cosf(floating(value)));self.trig.append((3,value,output))
  if self.arm64:uc.reg_write(UC_ARM64_REG_S0,output)
  else:self.put(0,output)
  self.import_calls['cosf']=self.import_calls.get('cosf',0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--gold',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args()
 manifest_path=ROOT/'reference/component-angle/original-functions.json';manifest=json.loads(manifest_path.read_text());assert sha(a.engine)==manifest['original_sha256']
 old=AngleCpu(a.engine,False,manifest);new=AngleCpu(a.library,True,{'functions':[]});cases=[];inputs=[]
 def fixture(axis,values,origin='synthetic'):
  angles=words(values);default=words(axis+[0x7fc0abcd]);count=len(values)
  for key in sorted(set((0,count//2,count-1))):
   cases.append((0,key,key,0,angles,default,origin))
   for fraction in (0,0.25,0.5,1,-0.25,1.25):cases.append((1,key,min(key+1,count-1),bits(fraction),angles,default,origin))
 # Exact authored scalar-angle tracks. These have present four-float variants.
 assets=REPO/'port/android-native/app/src/main/assets';bank=json.loads((assets/'data/prince-animation-bank.json').read_text());resources={r['clip_id']:r for r in bank['resources']};authored_tracks=0
 for cid in (1077,1078,1079):
  entry=resources[cid];path=assets/entry['asset'];raw=path.read_bytes();assert sha(path)==entry['sha256'];inputs.append({'asset':entry['asset'],'sha256':sha(path)})
  root=word(raw,32);library=word(raw,root+48);segment=word(raw,library+4);data=word(raw,segment+(12 if word(raw,segment+8)==0 else 20))
  for index in range(word(raw,root+36)):
   record=word(raw,root+40)+index*32;channel=word(raw,record+16)
   if word(raw,channel+8)!=9:continue
   sampler=word(raw,record+8);assert word(raw,sampler+16)==6 and word(raw,sampler+20)==1 and not word(raw,record+28)
   descriptor=word(raw,record+24);assert descriptor and word(raw,descriptor+4)==4;default=word(raw,descriptor+8);axis=list(struct.unpack_from('<3I',raw,default))
   vector=data+4+word(raw,sampler+24)*8;count=word(raw,vector);slot=vector+4;values=slot+i32(word(raw,slot));assert count>0
   fixture(axis,list(struct.unpack_from('<'+'I'*count,raw,values)),'authored');authored_tracks+=1
 assert authored_tracks==7
 for axis in ([0,0,0],[0,0,0x3f800000],[0x3f800000,0,0],[0x80000000,0x3f800000,0x80000000],[bits(2),bits(-3),bits(.5)],[0x7fc01234,0x3f800000,0x7f800000]):
  for values in ([0,0x80000000,bits(math.pi)],[bits(-1),bits(1),bits(3)],[0x7f7fffff,0xff7fffff,0x7fc05678],[0x7f800000,0xff800000,0]):fixture(list(axis),list(values))
 rng=random.Random(0x61f94c)
 for _ in range(40):fixture([bits(rng.uniform(-3,3)) for _ in range(3)],[bits(rng.uniform(-30,30)) for _ in range(4)])
 records=[];nan_differences=0;calls=0
 for operation,key,next_,fraction,angles,default,origin in cases:
  expected=None;traces=[]
  for cpu in (old,new):
   base=cpu.data+0x10000;accessor=base;values=base+0x100;defaults=base+0x200;output=base+0x300;cpu.uc.mem_write(values,angles);cpu.uc.mem_write(defaults,default);cpu.uc.mem_write(output,words([0x12345678]*4));cpu.trig=[]
   if cpu.arm64:
    cpu.uc.mem_write(accessor,struct.pack('<QQII',values,defaults,len(angles)//4,0));cpu.uc.reg_write(UC_ARM64_REG_S0,fraction)
    result=cpu.invoke('dh2_animation_angle_between' if operation else 'dh2_animation_angle_key',[output,accessor,key]+([next_] if operation else []));assert result==0
   else:
    animation=base+0x400;sampler=base+0x500;data=base+0x600;descriptor=base+0x700
    cpu.uc.mem_write(animation,bytes(32));cpu.uc.mem_write(sampler,bytes(28));cpu.pointer(animation+8,sampler);cpu.pointer(animation+24,descriptor);cpu.pointer(descriptor+8,defaults);cpu.pointer(accessor,animation);cpu.pointer(accessor+4,data);cpu.uc.mem_write(data,words([1,len(angles)//4,values]));
    cpu.invoke(0x61f94c if operation else 0x61f594,[accessor,key,next_,fraction,output] if operation else [accessor,key,output])
   actual=bytes(cpu.uc.mem_read(output,16));traces.append(cpu.trig[:])
   if expected is None:expected=actual
   else:
    for left,right in zip(struct.unpack('<4I',expected),struct.unpack('<4I',actual)):assert same_word(left,right),(len(records),origin,hex(left),hex(right));nan_differences+=left!=right
  assert len(traces[0])==len(traces[1])==2
  for left,right in zip(*traces):assert left[0]==right[0] and all(same_word(x,y) for x,y in zip(left[1:],right[1:]))
  calls+=2;records.append(words([operation,key,next_,fraction,len(angles)//4])+angles+default+expected+words([2])+b''.join(words(t) for t in traces[0]))
 # Native validation guards: no reads through rejected/overlapping inputs.
 base=new.data+0x10000;values=base+0x100;defaults=base+0x200;out=base+0x300;new.uc.mem_write(values,words([0,1]));new.uc.mem_write(defaults,words([0]*4));good=struct.pack('<QQII',values,defaults,2,0);rejects=0
 def reject(raw,outptr=out,aptr=base,key=0,next_=1,status=-1):
  nonlocal rejects
  new.uc.mem_write(base,raw);new.uc.mem_write(out,words([0x12345678]*4));before=bytes(new.uc.mem_read(base,0x400));new.trig=[];new.uc.reg_write(UC_ARM64_REG_S0,bits(.5));r=new.invoke('dh2_animation_angle_between',[outptr,aptr,key,next_]);assert r&0xffffffff==status&0xffffffff and before==bytes(new.uc.mem_read(base,0x400)) and not new.trig;rejects+=1
 reject(good,outptr=0);reject(good,aptr=0);reject(good,aptr=base+1);reject(good,outptr=out+1);reject(good,outptr=base);reject(good,outptr=values);reject(good,outptr=defaults);reject(good,key=2);reject(good,next_=2)
 for field,value in ((0,0),(0,values+1),(1,defaults+1),(2,0),(3,1)):
  row=[values,defaults,2,0];row[field]=value;reject(struct.pack('<QQII',*row))
 reject(struct.pack('<QQII',values,0,2,0),status=-2)
 blob=b'ANG1'+words([len(records)])+b''.join(records);a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(blob)
 sources=[ROOT/'angle_interpreter.hpp',ROOT/'angle_interpreter.cpp',Path(__file__),ROOT/'tests/angle_interpreter.cpp',REPO/'port/engine-math/math.cpp',REPO/'port/engine-math/math.hpp']
 report={'validation':'PASS','original_sha256':sha(a.engine),'arm64_library_sha256':sha(a.library),'corpus_sha256':sha(a.gold),'comparisons':len(records),'authored_tracks':authored_tracks,'authored_comparisons':sum(x[-1]=='authored' for x in cases),'ordered_libm_calls':calls,'atomic_rejection_checks':rejects,'arithmetic_nan_payload_differences':nan_differences,'mismatches':0,'source_sha256':{str(p.relative_to(REPO)).replace('\\','/'):sha(p) for p in sources},'manifest_sha256':sha(manifest_path),'inputs':inputs,'libm_contract':'Actual source conversion/math execute; imported sinf/cosf modeled same UCRT on both CPUs. Compiler sincosf has identical two-function import model, no source math replacement. Finite/signedzero exact; arithmetic NaNs classification. No historical Bionic-libm parity claim.','scope':'Float angle single-key and two-key interpolation wrappers with present raw default variants. Source angle-axis conversion reused. Missing default rejected: source scratch angle undefined. No key search, compressed offsets, delta quaternion wrappers, full factory/resource lifecycle or APK claim.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
