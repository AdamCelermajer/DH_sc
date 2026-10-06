"""Source IMA native block instructions and source event histories; no device use."""
from pathlib import Path
import json,sys,struct,hashlib,random,ctypes
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path(__file__).resolve().parents[3];original=root/'port/engine-animation/tests/particle_cloud_models_v1_differential.py'
env={'__file__':str(original),'__name__':'audio_v34'};exec(compile(original.read_text().split('rng=random.Random')[0],str(original),'exec'),env)
ModelCpu=env['ModelCpu'];words=env['words'];u32=env['u32']
from unicorn.arm64_const import UC_ARM64_REG_S0,UC_ARM64_REG_S1
class AudioCpu(ModelCpu):
 def external(self,uc,a,size,user):
  if self.imports.get(a) in ('__aeabi_idiv','__aeabi_idivmod'):
   x,y=ctypes.c_int32(self.reg(0)).value,ctypes.c_int32(self.reg(1)).value;assert y;q=int(x/y);self.put(0,q&0xffffffff);self.put(1,(x-q*y)&0xffffffff);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if self.imports.get(a)=='powf':
   function=ctypes.CDLL('ucrtbase').powf;function.argtypes=[ctypes.c_float,ctypes.c_float];function.restype=ctypes.c_float
   x=uc.reg_read(UC_ARM64_REG_S0)if self.arm64 else self.reg(0);y=uc.reg_read(UC_ARM64_REG_S1)if self.arm64 else self.reg(1);value=function(struct.unpack('<f',words([x]))[0],struct.unpack('<f',words([y]))[0]);bits=struct.unpack('<I',struct.pack('<f',value))[0]
   if self.arm64:uc.reg_write(UC_ARM64_REG_S0,bits)
   else:self.put(0,bits)
   uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,a,size,user)
old=AudioCpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});env['old']=old
new=AudioCpu(root/'.local-inputs/audio-v34/ima-oracle-arm64.so',True,{'functions':[]})
from unicorn import UC_HOOK_CODE
base=old.data+0x40000;receiver=base;stream=base+0x400;vt=stream+0x100;dispatch=vt+0x100;segments=dispatch+0x100;segrow=segments+0x100;state=segrow+0x100;scratch=state+0x100;output=scratch+0x2000
old.pointer(stream,vt);old.pointer(vt+0x18,dispatch);old.pointer(vt+0x10,dispatch+4);old.pointer(vt+0x1c,dispatch+8)
current=b'';position=0;heap=old.data+0x100000;active=False;draws=[]
def ret(v=0):old.put(0,v);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
def hook(uc,a,size,user):
 global position,heap
 if a==dispatch:ret(position)
 elif a==0x30e2a4:
  x,y=ctypes.c_int32(old.reg(0)).value,ctypes.c_int32(old.reg(1)).value;assert y;ret(int(x/y)&0xffffffff)
 elif a==dispatch+4:position=old.reg(1);ret(0)
 elif a==dispatch+8:
  n=min(old.reg(2),len(current)-position);assert n>=0;uc.mem_write(old.reg(1),current[position:position+n]);position+=n;ret(n)
 elif active and a==0x30eda8:ret(draws.pop(0))
 elif active and a==0x30e904:
  x,y=old.reg(0),old.reg(1);assert y;old.put(0,x//y);old.put(1,x%y);uc.reg_write(old.pc,uc.reg_read(old.lr))
 elif active and a==0x310648:
  n=old.reg(0);pointer=(heap+15)&~15;heap=pointer+max(n,16);uc.mem_write(pointer,bytes(n));ret(pointer)
 elif active and a==0x310444:ret(0)
old.uc.hook_add(UC_HOOK_CODE,hook)
cache=root/'.local-inputs/audio-v34/cache';records=[];corpus=[]
for file in sorted(cache.glob('*.vxn')):
 raw=file.read_bytes();chunks={};p=0
 while p+8<=len(raw):
  tag=raw[p:p+4].decode();size=struct.unpack_from('<I',raw,p+4)[0];chunks[tag]=(p+8,size);p+=8+size
 fmt=chunks['Afmt'][0];_,channels,rate,align,bits=struct.unpack_from('<HHIHH',raw,fmt);assert channels==2 and align==1024 and bits==4
 data=chunks['Data'][0];seg=chunks['Segm'][0];rows=struct.unpack_from('<I',raw,seg)[0];comparisons=0
 for index in range(rows):
  row=raw[seg+4+index*24:seg+4+(index+1)*24];offset,length,frames=struct.unpack_from('<III',row);frames_per_block=(align-4*channels)*2//channels+1
  for block in [0,1,length//align//2,length//align-1]:
   current=raw;position=0;old.uc.mem_write(receiver,bytes(512));old.pointer(receiver+4,stream);old.uc.mem_write(receiver+10,struct.pack('<H',channels));old.uc.mem_write(receiver+16,struct.pack('<h',align));old.pointer(receiver+20,data);old.pointer(receiver+24,segments);old.pointer(segments+4,segrow);old.uc.mem_write(segrow,row);old.pointer(receiver+0x19c,scratch);old.uc.mem_write(state,words([0,0,block*align,block*frames_per_block]));old.uc.mem_write(output,bytes(8192))
   count=old.invoke(0x887068,[receiver,output,state],budget=1000000);assert count==min(frames_per_block,frames-block*frames_per_block)
   expected=bytes(old.uc.mem_read(output,count*channels*2));payload=words([channels,align])+raw[data+offset+block*align:data+offset+(block+1)*align];ni=new.data+0x10000;no=ni+0x10000;new.uc.mem_write(ni,payload);actual_frames=new.invoke('dh2_audio_ima_v34',[ni,no],budget=1000000);assert actual_frames==frames_per_block
   actual=bytes(new.uc.mem_read(no,count*channels*2));assert actual==expected,(file.name,index,block,next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),None));records.append(words([len(payload),count])+payload+words([len(expected)])+expected);comparisons+=1
 corpus.append(dict(file=file.name,segments=rows,comparisons=comparisons))
gold=root/'port/engine-audio/reports/audio-v34-ima-gold.bin';gold.parent.mkdir(exist_ok=True);gold.write_bytes(words([0x34334149,len(records)])+b''.join(records))
# GetEventSoundUid88bb6c, source rand modulo and same-list FIFO/remaining state.
owner=base+0x20000;row=owner+0x100;values=row+0x100;selected=values+0x100;event_records=[]
for kind in [0,1]:
 for limit in [0,1,2,3]:
  source=[39,40,41];old.uc.mem_write(row,bytes(44));old.pointer(owner+0x24,row);old.pointer(owner+0x28,row+44);old.pointer(row+8,row+8);old.pointer(row+12,row+8);old.pointer(row+16,values);old.pointer(row+20,values+12);old.pointer(row+24,values+64);old.uc.mem_write(values,words(source));old.uc.mem_write(row+28,struct.pack('<hhhh',kind,limit,100,3));remaining=source[:];history=[];cursor=0;trace=[]
  for i in range(24):
   rand0=13+i*7;rand1=27+i*11;draws=[rand0]+([rand1]if kind==0 else []);active=True;old.invoke(0x88bb6c,[owner,0,selected],budget=1000000);active=False;assert not draws
   if kind==1:
    if cursor>=len(remaining):cursor=0
    expected=remaining[cursor];cursor+=1
   else:
    choice=rand1%len(remaining);expected=remaining[choice];history.append(expected);remaining[choice]=remaining[-1];remaining.pop()
    if len(history)>limit or not remaining:remaining.append(history.pop(0))
   actual=u32(selected);assert actual==expected,(kind,limit,i,actual,expected);trace.append(actual)
  event_records.append(dict(kind=kind,history_limit=limit,source=source,trace=trace))
report=dict(validation='PASS',original_instructions_executed=True,compiled_arm64_instructions_executed=True,original_sha256=hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),ima_blocks=len(records),corpus=corpus,source_event_sequences=event_records,scope='Original native IMA blocks over actual first/middle/tail segments; explicit stream read/seek callbacks. Whole source event selector with explicit same rand/allocator boundary. No Android/audible acceptance.')
event_gold=root/'port/engine-audio/reports/audio-v34-event-gold.bin'
event_gold.write_bytes(words([0x34335645,len(event_records)])+b''.join(words([row['kind'],row['history_limit'],len(row['trace'])])+b''.join(words([13+i*7,27+i*11,uid])for i,uid in enumerate(row['trace']))for row in event_records))
# Whole source listener setup and three driver spatial leaves, finite domain.
rng=random.Random(0x3D342026);spatial=[];result=output+0x10000
for trial in range(420):
 relative=trial%2;model=trial%7;position=[rng.uniform(-50,50)for _ in range(3)];listener_position=[rng.uniform(-10,10)for _ in range(3)];listener_velocity=[rng.uniform(-2,2)for _ in range(3)];velocity=[rng.uniform(-2,2)for _ in range(3)];front=[0,0,-1];up=[0,1,0];factor=[0,1,.5,2][trial%4];speed=343.3;ratio=struct.unpack('<f',struct.pack('<f',speed/factor if factor else speed))[0];reference=3.5;maximum=35.;rolloff=[0,.2,1,2][trial%4]
 listener=struct.pack('<12f',*listener_position,*listener_velocity,*front,*up);source=struct.pack('<6fII5f',*position,*velocity,relative,model,reference,maximum,rolloff,factor,ratio);payload=listener+source
 old.invoke(0x890198,list(struct.unpack('<12I',listener))+list(struct.unpack('<2I',struct.pack('<2f',factor,speed)))+[model],budget=100000)
 old.uc.mem_write(receiver,bytes(512));old.uc.mem_write(receiver+0x6c,struct.pack('<6f',*position,*velocity));old.pointer(receiver+0x90,relative);old.uc.mem_write(receiver+0x94,struct.pack('<3f',maximum,reference,rolloff))
 old.invoke(0x891b80,[receiver,result,result+4],budget=1000000);pan=bytes(old.uc.mem_read(result,8));distance=old.invoke(0x892114,[receiver],budget=1000000);doppler=old.invoke(0x891ed0,[receiver],budget=1000000);expected=pan+words([distance,doppler]);new.uc.mem_write(ni,payload);length=new.invoke('dh2_audio_spatial_v34',[ni,no],budget=1000000);actual=bytes(new.uc.mem_read(no,length))
 assert length==16 and actual==expected,('spatial',trial,relative,model,actual.hex(),expected.hex());spatial.append(payload+expected)
spatial_gold=root/'port/engine-audio/reports/audio-v34-spatial-gold.bin';spatial_gold.write_bytes(words([0x34335053,len(spatial)])+b''.join(spatial));report['source_spatial_comparisons']=len(spatial)
# Exact normal two-state native fade kernel; terminal source type3 is separate.
envelopes=[];rng=random.Random(0xFADE34);old.pointer(0xa33e10,output)
for trial in range(240):
 channels=1+trial%2;frames=rng.randrange(1,160);delay=rng.randrange(0,32);remaining=rng.randrange(1,96);step=(1073741824//remaining)*(1 if trial%3 else -1);gain=0 if step>0 else 1073741824
 samples=struct.pack('<'+'h'*(frames*channels),*[rng.randrange(-32768,32768)for _ in range(frames*channels)]);accumulator=struct.pack('<'+'i'*(frames*channels),*[rng.randrange(-65536,65536)for _ in range(frames*channels)])
 payload=words([frames,channels,delay,remaining,step&0xffffffff,gain])+samples+accumulator
 old.uc.mem_write(receiver,bytes(512));old.uc.mem_write(receiver+10,struct.pack('<h',channels));old.uc.mem_write(receiver+18,struct.pack('<h',16));old.uc.mem_write(state,bytes(128));old.uc.mem_write(state+0x28,words([delay,0,remaining,step&0xffffffff,gain]));old.uc.mem_write(scratch,samples);old.uc.mem_write(output,accumulator)
 old.invoke(0x8844e8,[receiver,scratch,len(samples),state],budget=1000000)
 expected=bytes(old.uc.mem_read(state+0x28,4))+bytes(old.uc.mem_read(state+0x30,12))+bytes(old.uc.mem_read(state+0x24,4))+bytes(old.uc.mem_read(output,len(accumulator)))
 new.uc.mem_write(ni,payload);length=new.invoke('dh2_audio_envelope_v34',[ni,no],budget=1000000);actual=bytes(new.uc.mem_read(no,length));assert actual==expected,('envelope',trial,frames,delay,remaining,actual[:20].hex(),expected[:20].hex());envelopes.append(words([len(payload)])+payload+words([len(expected)])+expected)
(root/'port/engine-audio/reports/audio-v34-envelope-gold.bin').write_bytes(words([0x34334e45,len(envelopes)])+b''.join(envelopes));report['source_native_envelope_comparisons']=len(envelopes)
(root/'port/engine-audio/reports/audio-v34-original-proof.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
