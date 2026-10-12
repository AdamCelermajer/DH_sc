"""Actual source FX77 byte-color interpreters/contribution/CMaterial writes."""
import argparse,hashlib,json,random,struct,sys,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from compiled_transforms_differential import Cpu,word
from animation_blend_differential import bits
def words(v):return struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v])
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,default=REPO/'.local-inputs/libDungeonHunter2.so');p.add_argument('--library',type=Path,default=REPO/'.local-inputs/fx-material-animation-discovery/material_color64.so');p.add_argument('--report',type=Path,default=ROOT/'reports/material-color-arm64-differential.json');p.add_argument('--reference-output',type=Path,default=ROOT/'reference/fx-material-animation/material-color-fixtures.bin');a=p.parse_args();start=time.monotonic()
 manifest=json.loads((ROOT/'reference/fx-material-animation/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256'];old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]})
 out=old.data+0x1000;accessor=out+0x100;record=accessor+0x100;sampler=record+0x100;dataset=sampler+0x100;keys=dataset+0x100;default=keys+0x100;metadata=default+0x100;parameter=metadata+0x100;header=parameter+0x100;definition=header+0x100;info=definition+0x100;values=info+0x100;weights=values+0x1000
 old.uc.mem_write(accessor,words([record,dataset]));old.pointer(record+8,sampler);old.pointer(dataset+8,keys);old.pointer(sampler+24,0);old.pointer(metadata+8,default)
 old.pointer(parameter+4,header);old.uc.mem_write(header+14,struct.pack('<H',1));old.pointer(header+32,definition);old.uc.mem_write(definition+8,words([1,0]));old.uc.mem_write(info+8,words([0]))
 native_input=new.data+0x1000;native_output=new.data+0x10000;records=[];counts={};real_fx=0
 def compare(op,payload,status,expected,label):
  new.uc.mem_write(native_input,payload);new.uc.mem_write(native_output,bytes(32));actual=new.invoke('dh2_material_color_test',[op,native_input,len(payload),native_output]);actual=(actual&0xffffffff);actual=actual if actual<0x80000000 else actual-0x100000000
  observed=bytes(new.uc.mem_read(native_output,len(expected)));assert status==actual and expected==observed,(label,status,actual,expected.hex(),observed.hex())
  records.append(words([op,len(payload)])+payload+words([status,len(expected)])+expected);counts[str(op)]=counts.get(str(op),0)+1
 def alpha(op,keydata,has,pointer,k,n,r,frac,initial=b'\x11\x22\x33\x44',dv=b'\xff\xff\xff\0',label='synthetic'):
  old.uc.mem_write(keys,keydata);old.pointer(dataset+4,len(keydata));old.pointer(record+24,metadata if has else 0);old.pointer(metadata+8,default if pointer else 0);old.uc.mem_write(default,dv);old.uc.mem_write(out,initial)
  if op==0:old.invoke(0x61a3fc,[accessor,k,out])
  elif op==1:old.invoke(0x61a484,[accessor,k,n,frac,out])
  else:old.invoke(0x619978,[accessor,r,k,n,frac,out])
  expected=bytes(old.uc.mem_read(out,4));payload=words([len(keydata),has,pointer,k,n,r,frac])+initial+dv+keydata;compare(op,payload,0,expected,label)
  if label.startswith('actual FX77'):
   state=words([8,1,0x3f800000,0x3f800000,0x3f800000,0,71,73]);old.uc.mem_write(definition+6,b'\x08');old.uc.mem_write(definition+8,words([1,0]));old.uc.mem_write(parameter+12,state[24:32]);old.uc.mem_write(parameter+32,state[8:24]);
   if op==0:old.invoke(0x622a18,[0,accessor,k,parameter,info])
   else:old.invoke(0x622a74,[accessor,k,n,frac,parameter,info])
   applied=state[:8]+bytes(old.uc.mem_read(parameter+32,16))+bytes(old.uc.mem_read(parameter+12,8));compare(6 if op==0 else 7,state+payload,1,applied,'actual source key->material application')
 raw=(REPO/'.local-inputs/character-skeleton-fx-assets/zombie_spawn_fx.bdae').read_bytes();root=word(raw,32);lib=word(raw,root+48);segment=word(raw,lib+4);data=word(raw,segment+20);vector=data+4+8;count=word(raw,vector);key_offset=vector+4+struct.unpack_from('<i',raw,vector+4)[0];fxkeys=raw[key_offset:key_offset+count];rec=word(raw,root+40);md=word(raw,rec+24);dptr=word(raw,md+8);fxdefault=raw[dptr:dptr+4];assert count==12 and fxdefault==b'\xff\xff\xff\0'
 fractions=[bits(x)for x in (-2.,-0.,0.,0.125,0.5,0.999,1.,2.)]+[0x7f800000,0xff800000,0x7fc12345,0x7fa00123]
 for k in range(count):
  alpha(0,fxkeys,1,1,k,k,k,0,dv=fxdefault,label='actual FX77 key');real_fx+=1
  for n in range(count):
   for f in fractions:alpha(1,fxkeys,1,1,k,n,k,f,dv=fxdefault,label='actual FX77 between');real_fx+=1
 # Source branches without metadata and key metadata with null value pointer.
 rng=random.Random(0x77c010)
 for _ in range(1200):
  keydata=bytes(rng.randrange(256)for x in range(3));has=rng.randrange(2);k,n,r=[rng.randrange(3)for x in range(3)];f=rng.choice(fractions);dv=bytes(rng.randrange(256)for x in range(4));initial=bytes(rng.randrange(256)for x in range(4));op=rng.randrange(3);pointer=has or rng.randrange(2)
  alpha(op,keydata,has,pointer,k,n,r,f,initial,dv)
 for k in range(3):alpha(0,b'\xff\0\x80',1,0,k,k,k,0,label='source null default key')
 def set_parameter(state,element,color):
  t,c,*tail=struct.unpack('<8I',state);old.uc.mem_write(definition+6,bytes([t]));old.uc.mem_write(definition+8,words([c,0]));old.uc.mem_write(parameter+12,words(tail[4:]));old.uc.mem_write(parameter+32,words(tail[:4]));old.uc.mem_write(values,color)
  result=old.invoke(0x5cad38,[parameter,0,element,values]);expected=words([t,c])+bytes(old.uc.mem_read(parameter+32,16))+bytes(old.uc.mem_read(parameter+12,8));return result,expected
 for _ in range(800):
  color=bytes(rng.randrange(256)for x in range(4));t=rng.choice([8,16]);c=rng.randrange(4);element=rng.randrange(4);state=words([t,c]+[rng.getrandbits(32)for x in range(4)]+[23,41]);status,expected=set_parameter(state,element,color);compare(4,state+words([element])+color,status,expected,'material setter')
 for t in (8,16):
  for color in (b'\0\0\0\0',b'\xff\xff\xff\xff',b'\x01\x02\x03\x04',b'\0\xff\0\xff'):
   state=words([t,1,0,0,0,0,11,12]);status,expected=set_parameter(state,0,color);compare(4,state+words([0])+color,status,expected,'initial material write')
   # Same value must retain both arbitrary cache stamps; unequal alpha invalidates.
   state=expected[:24]+words([11,12]);status,expected=set_parameter(state,0,color);compare(4,state+words([0])+color,status,expected,'equal material write')
 weight_pool=[bits(x)for x in (0.,-0.,0.5,1.,-1.,2.,256.,1e30)]+[0x7f800000,0xff800000,0x7fc12345,0x7fa00123]
 for _ in range(1200):
  n=rng.randrange(7);value=bytes(rng.randrange(256)for x in range(n*4));wb=words([rng.choice(weight_pool)for x in range(n)]);old.uc.mem_write(values,value or bytes(4));old.uc.mem_write(weights,wb or bytes(4));old.uc.mem_write(out,b'abcd');old.invoke(0x626124,[0,values,weights,n,out]);expected=bytes(old.uc.mem_read(out,4));compare(3,words([n])+b'abcd'+value+wb,0,expected,'uchar4 contribution')
  t=rng.choice([8,16]);state=words([t,1]+[rng.getrandbits(32)for x in range(4)]+[17,19]);_,_=set_parameter(state,0,b'\0'*4)
  # Restore input state before executing original contribution->actual setter.
  old.uc.mem_write(parameter+12,state[24:32]);old.uc.mem_write(parameter+32,state[8:24]);old.uc.mem_write(values,value or bytes(4));old.uc.mem_write(weights,wb or bytes(4));old.invoke(0x625a28,[0,values,weights,n,parameter,info]);expected=state[:8]+bytes(old.uc.mem_read(parameter+32,16))+bytes(old.uc.mem_read(parameter+12,8));compare(5,state+words([n])+value+wb,1,expected,'actual blended material application')
 guards=new.invoke('dh2_material_color_test_guards',[]);assert guards==13
 gold=words([0x31434d46,len(records)])+b''.join(records);a.reference_output.write_bytes(gold)
 source=[ROOT/'material_color.hpp',ROOT/'material_color.cpp',ROOT/'tests/material_color.cpp',Path(__file__)];report={'validation':'PASS','original_sha256':sha(a.engine),'original_manifest_sha256':sha(ROOT/'reference/fx-material-animation/original-functions.json'),'arm64_library_sha256':sha(a.library),'reference_sha256':sha(a.reference_output),'source_sha256':{str(x.relative_to(REPO)).replace('\\','/'):sha(x)for x in source},'comparisons':len(records),'operation_counts':counts,'actual_fx77_key_interpretations':real_fx,'atomic_guards':guards,'mismatches':0,'original_instructions_executed':True,'compiled_arm64_instructions_executed':True,'asset_sha256':hashlib.sha256(raw).hexdigest(),'scope':'Source component3 byte key/interpolation/wrapped delta, uchar4 contributions and resolved CMaterial parameter types8/16 including stamps. Actual original accessor getters, f2uiz, key/interpolation application and blended setter execute; native directory/name/factory, particle28, event/timeline and GPU composition excluded. IEEE arithmetic follows shared soft-float contract; final converted byte outputs are exact.','elapsed_seconds':round(time.monotonic()-start,2)};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items()if k not in ('scope','source_sha256')}))
if __name__=='__main__':main()
