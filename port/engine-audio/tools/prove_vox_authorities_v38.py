"""Bounded original Vox spatial authority call capture, no output/device."""
from pathlib import Path
import sys,struct,json,hashlib,random
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages');sys.path.insert(0,str(root/'port/engine-animation/tests'))
from animation_blend_differential import Cpu as FloatCpu
class Cpu(FloatCpu):
 def external(self,uc,a,n,p):
  if self.imports.get(a)=='__aeabi_i2f':
   value=self.reg(0);value=value if value<0x80000000 else value-0x100000000;self.put(0,struct.unpack('<I',struct.pack('<f',value))[0]);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,a,n,p)
from unicorn import UC_HOOK_CODE
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
out=root/'port/engine-audio/reference/authorities-v38';out.mkdir(parents=True,exist_ok=True)
owner=c.data+0x1000;channels=owner+0x400;position=channels+0x400;file=position+0x400;disabled=file+0x400;pointer=disabled+0x400
u=lambda a:struct.unpack('<I',c.uc.mem_read(a,4))[0]
f=lambda a:struct.unpack('<f',c.uc.mem_read(a,4))[0]
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
vector=lambda a:tuple(struct.unpack('<3f',c.uc.mem_read(a,12)))
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
# Original GOT same-platform gate.
got=0x36a7d0+8+u(0x36aeb0);c.pointer(got+u(0x36aeb4),disabled)
captures=[];kind=0;current=(0.,0.,0.);active=True;listener_position=[10.,20.,30.];listener_front=[0.,0.,-1.];listener_up=[0.,1.,0.];listener_stream=None;listener_at=0
def hook(uc,a,n,p):
 global current,listener_at
 if listener_stream is not None and a in [0x313a90,0x459090,0x3df1a0,0x4db94c]:
  value=listener_stream[listener_at:listener_at+4];assert len(value)==4;listener_at+=4
  if a==0x313a90:ret(struct.unpack('<I',value)[0])
  else:uc.mem_write(c.reg(1),value);ret()
 elif a in [0x89347c,0x893478]:ret()
 elif a==0x8624f8:ret(1)
 elif a==0x862618:captures.append(['priority',c.reg(2)]);ret()
 elif a==0x889894:
  c.pointer(c.reg(2),9);c.pointer(c.reg(3),0x20);c.uc.mem_write(u(c.uc.reg_read(c.sp)),b'\0');c.pointer(u(c.uc.reg_read(c.sp)+4),kind);c.pointer(u(c.uc.reg_read(c.sp)+8),0);ret()
 elif a==0x862438:ret()
 elif a==0x861e48:
  current=tuple(struct.unpack('<3f',struct.pack('<3I',c.reg(2),c.reg(3),u(c.uc.reg_read(c.sp)))));captures.append(['position',*current]);ret()
 elif a==0x861d88:captures.append(['float',c.reg(2),struct.unpack('<f',struct.pack('<I',c.reg(3)))[0]]);ret()
 elif a==0x861d60:captures.append(['integer',c.reg(2),c.reg(3)]);ret()
 elif a in [0x861950,0x862058,0x8621c0,0x8683ac]:ret()
 elif a==0x30e76c:ret(0) # explicit already-resolved lazy group guard; masks !=0x20
 elif a==0x861aa8:
  addresses=[c.reg(1),c.reg(2),c.reg(3),u(c.uc.reg_read(c.sp)),u(c.uc.reg_read(c.sp)+4),u(c.uc.reg_read(c.sp)+8)]
  for ptr,value in zip(addresses,listener_front+listener_up):c.pointer(ptr,bits(value))
  ret()
 elif a==0x861d28:
  for ptr,value in zip([c.reg(2),c.reg(3),u(c.uc.reg_read(c.sp))],current):c.pointer(ptr,bits(value))
  ret()
 elif a==0x861b10:
  for ptr,value in zip([c.reg(1),c.reg(2),c.reg(3)],listener_position):c.pointer(ptr,bits(value))
  ret()
c.uc.hook_add(UC_HOOK_CODE,hook)
c.uc.mem_write(file,b'exact.wav\0');records=[];gold=[];new=Cpu(root/'.local-inputs/audio-v38/vox-fields-arm64.so',True,{'functions':[]});rng=random.Random(0x383844);ni=new.data+0x10000;no=ni+0x10000
for trial in range(102):
  kind=trial%3;ref,maxdist=[(-1.,-1.),(3.,7.),(-1.,7.),(3.,-1.)][trial%4];source_position=[2.,3.,4.]
  if trial>=12:
   source_position=[rng.uniform(-200,200)for _ in range(3)];listener_position=[rng.uniform(-20,20)for _ in range(3)];listener_front=[rng.uniform(-3,3)for _ in range(3)];listener_up=[rng.uniform(-3,3)for _ in range(3)]
  c.uc.mem_write(owner,bytes(512));c.pointer(owner,0x8888);c.pointer(owner+8,channels);c.pointer(channels+8,0x7777);c.pointer(owner+0x54,1000);c.pointer(owner+0x58,2000);c.pointer(owner+0x5c,bits(.5));c.uc.mem_write(position,struct.pack('<3f',*source_position));captures=[];current=(0.,0.,0.)
  c.invoke(0x36a7c0,[owner,2,file,1,9,2,0,position,bits(ref),bits(maxdist)],budget=1000000)
  initial=struct.pack('<6f?3xi5f',7,8,9,0,0,0,False,4,55,66,.2,1,343.3);expected_position=[7.,8.,9.];relative=False;reference=55.;maximum=66.;rolloff=.2
  for call in captures:
   if call[0]=='position':expected_position=call[1:]
   elif call[:2]==['integer',0]:relative=bool(call[2])
   elif call[:2]==['float',1]:maximum=call[2]
   elif call[:2]==['float',2]:reference=call[2]
   elif call[:2]==['float',3]:rolloff=call[2]
  expected=struct.pack('<6f?3xi5f',*expected_position,0,0,0,relative,4,reference,maximum,rolloff,1,343.3)
  authority=struct.pack('<12fiif',*listener_position,0,0,0,*listener_front,*listener_up,1000,2000,.5);payload=authority+struct.pack('<i5f',kind,*source_position,ref,maxdist)+initial;assert len(payload)==136
  new.uc.mem_write(ni,payload);length=new.invoke('dh2_vox_source_fields_v38',[ni,no],budget=1000000);actual=bytes(new.uc.mem_read(no,length));assert actual==expected,('properties',trial,actual.hex(),expected.hex());gold.append(payload+expected)
  records.append(dict(position_type=kind,reference_override=ref,maximum_override=maxdist,calls=captures))
report=dict(validation='PASS',scope='Original PlaySoundPackSound properties and relative transform on existing loaded source; explicit driver observers and non-special mask fixture. No source UID binding or audible output claim.',original_sha256=hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),records=records)
report['compiled_arm64_comparisons']=len(gold);(out/'original-properties-gold.bin').write_bytes(struct.pack('<II',0x38334156,len(gold))+b''.join(gold));
rows=json.loads((root/'port/level-world/reference/audio-source-v34/routing-census.json').read_text())['listeners'];listener_proof=[]
for row in rows:
 listener_stream=struct.pack('<6I',*row['words']);listener_at=0;c.uc.mem_write(owner,bytes(28));c.invoke(0x4ed978,[owner,c.data+0x7000],budget=100000);actual=bytes(c.uc.mem_read(owner+4,24));assert actual==listener_stream and listener_at==24,(row['name'],actual.hex(),listener_stream.hex(),listener_at);(out/(row['name']+'.bin')).write_bytes(actual);listener_proof.append(dict(name=row['name'],source_words=row['words'],all_original_fields_match=True))
listener_stream=None;report['original_listener_rows']=listener_proof
constructor=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});ctor_owner=constructor.data+0x1000;ctor_mode='constructor';model=[]
def stop_ctor():constructor.uc.reg_write(constructor.sp,constructor.stack+0xe000);constructor.uc.reg_write(constructor.pc,constructor.stop)
def ctor_hook(uc,a,n,p):
 if ctor_mode=='constructor' and a==0x31167c:constructor.put(0,ctor_owner+0x38);uc.reg_write(constructor.pc,uc.reg_read(constructor.lr))
 elif ctor_mode=='constructor' and a==0x324114:stop_ctor()
 elif ctor_mode=='initialize' and a==0x861a08:uc.reg_write(constructor.pc,uc.reg_read(constructor.lr))
 elif ctor_mode=='initialize' and a==0x861b38:model.append([constructor.reg(1),constructor.reg(2)]);stop_ctor()
constructor.uc.hook_add(UC_HOOK_CODE,ctor_hook);constructor.uc.mem_write(ctor_owner,bytes([255])*1024);constructor.invoke(0x36c7b0,[ctor_owner],budget=100000);distance_fields=bytes(constructor.uc.mem_read(ctor_owner+0x54,12));assert distance_fields==struct.pack('<iif',1000,1000,1.);(out/'constructor-distance-fields.bin').write_bytes(distance_fields)
ctor_mode='initialize';constructor.put(5,ctor_owner);constructor.invoke(0x36c3bc,[],budget=100000);assert model==[[2,4]];report['constructor_distance_fields']=[1000,1000,1.];report['initialize_distance_model_call']=model[0];report['constructor_prefix_boundary']='324114 before soundpack/engine construction';report['initialize_block']='36c3bc..36c3d8'
(out/'original-properties.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',compiled_arm64_comparisons=len(gold),original_listener_rows=len(rows))))
