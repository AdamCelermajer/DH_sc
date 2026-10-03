"""Execute original event routines and compiled ARM64 code with caller fixtures.

Only libc, imported soft-float, and the caller's observation callback are
modeled. Engine search, loop handling, deduplication and dispatch execute.
"""
import argparse,hashlib,json,random,struct,sys,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../engine-resources/tests'))
from cpu import Cpu as BaseCpu,i32,u32

def f32(x):return struct.unpack('<f',struct.pack('<f',x))[0]
def float_bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def from_bits(x):return struct.unpack('<f',struct.pack('<I',u32(x)))[0]
def trunc(x):return max(-2147483648,min(2147483647,int(x)))
class Cpu(BaseCpu):
 def __init__(self,*args):super().__init__(*args);self.events=[]
 def string(self,p):
  value=bytearray()
  while len(value)<4097:
   c=bytes(self.uc.mem_read(p+len(value),1))[0]
   if not c:return value.decode('ascii')
   value.append(c)
  raise AssertionError('Unterminated caller string')
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='caller_completion':
   event,user=self.reg(0),self.reg(1);assert user==self.data+0x7000
   self.events.append((i32(struct.unpack('<I',self.uc.mem_read(event,4))[0]),self.string(struct.unpack(self.pointer_format,self.uc.mem_read(event+(8 if self.arm64 else 4),self.word_size))[0])))
  elif name=='strcmp':
   a,b=self.string(self.reg(0)),self.string(self.reg(1));self.put(0,(a>b)-(a<b))
  elif name=='__aeabi_i2f':self.put(0,float_bits(i32(self.reg(0))))
  elif name=='__aeabi_f2iz':self.put(0,u32(trunc(from_bits(self.reg(0)))))
  elif name in ('__aeabi_fadd','__aeabi_fsub','__aeabi_fmul','__aeabi_fdiv','__aeabi_fcmplt'):
   a,b=from_bits(self.reg(0)),from_bits(self.reg(1))
   if name=='__aeabi_fcmplt':self.put(0,int(a<b))
   else:self.put(0,float_bits(a+b if name.endswith('fadd') else a-b if name.endswith('fsub') else a*b if name.endswith('fmul') else a/b))
  else:return super().external(uc,address,size,unused)
  self.import_calls[name]=self.import_calls.get(name,0)+1;self.uc.reg_write(self.pc,self.uc.reg_read(self.lr))

def install(cpu,kind,keys,groups):
 manager,track,times,records=[cpu.data+x for x in (0x1000,0x2000,0x3000,0x4000)]
 cpu.uc.mem_write(times,struct.pack('<'+{1:'B',3:'H',4:'i'}[kind]*len(keys),*keys))
 if cpu.arm64:
  cpu.uc.mem_write(track,struct.pack('<IIQQ',kind,len(keys),times,records))
  cpu.uc.mem_write(manager,struct.pack('<i',-1))
 else:
  cpu.uc.mem_write(track,struct.pack('<6I',kind,1,len(keys),times,len(groups),records))
  cpu.uc.mem_write(manager,struct.pack('<6I',0,1,cpu.callback,cpu.data+0x7000,u32(-1),track))
 strings=cpu.data+0x10000;lists=cpu.data+0x5000
 for i,names in enumerate(groups):
  if cpu.arm64:cpu.uc.mem_write(records+i*16,struct.pack('<IIQ',len(names),0,lists))
  else:cpu.uc.mem_write(records+i*8,struct.pack('<II',len(names),lists))
  for name in names:
   cpu.pointer(lists,strings);lists+=cpu.word_size
   raw=name.encode('ascii')+b'\0';cpu.uc.mem_write(strings,raw);strings+=len(raw)
 return manager,track

def parse(file):
 raw=file.read_bytes();w=lambda p:struct.unpack_from('<I',raw,p)[0];root=w(32);record=w(root+0x2c)
 if not record:return None
 kind,components,count,times,n,groups=struct.unpack_from('<6I',raw,record);assert components==1 and count==n
 width={1:1,3:2,4:4}[kind];keys=[];names=[]
 for i in range(count):
  keys.append(struct.unpack_from('<'+{1:'B',3:'H',4:'i'}[kind],raw,times+width*i)[0]);size,p=struct.unpack_from('<II',raw,groups+8*i)
  names.append([raw[w(p+4*j):raw.index(b'\0',w(p+4*j))].decode('ascii') for j in range(size)])
 return kind,keys,names

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--actors',type=Path,required=True);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/events/original-functions.json').read_text(encoding='utf-8-sig'));assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});rng=random.Random(221026)
 fixtures=[];authored=[]
 for file in sorted(a.actors.glob('*.bdae')):
  parsed=parse(file)
  if parsed:fixtures.append(parsed);authored.append({'file':file.name,'sha256':hashlib.sha256(file.read_bytes()).hexdigest(),'type':parsed[0],'keys':parsed[1],'names':parsed[2]})
 for kind in (1,3,4):
  for j in range(35):
   count=rng.randrange(0,24);high={1:255,3:65535,4:2147483647}[kind];low=0 if kind!=4 else -2147483648
   keys=sorted(rng.randint(low,high) for _ in range(count))
   if j%3==0:keys=sorted([min(high,max(low,k)) for k in (-100,-1,0,0,1,3,8,30,100,200,1000,65535,16777216,16777217,2147483647)])
   groups=[[rng.choice(['attack_mainhand','footstep','fx','duplicate']) for _ in range(rng.randrange(4))] for _ in keys]
   fixtures.append((kind,keys,groups))
 counts={'find':0,'lookup':0,'interval':0,'loop_update':0};callback_events=0
 for index,(kind,keys,groups) in enumerate(fixtures):
  om,ot=install(old,kind,keys,groups);nm,nt=install(new,kind,keys,groups)
  assert new.invoke('dh2_events_validate',[nt])==1
  probes=[-2147483648,-16777217,-100,-1,0,1,99,100,101,266,267,999,1000,1001,16777216,16777217,2147483647]
  for key in keys:
   t=key if kind==4 else trunc(f32(f32(key)*f32(33.333332061767578125)))
   probes.extend([i32(t-1),t,i32(t+1)])
  probes+= [rng.randint(-2147483648,2147483647) for _ in range(20)]
  for ms in probes:
   expected=i32(old.invoke(0x60e02c,[om,ms]));actual=i32(new.invoke('dh2_events_find',[nt,ms]));assert actual==expected,('find',index,kind,keys,ms,expected,actual);counts['find']+=1
  for name in ('attack_mainhand','footstep','fx','duplicate','absent'):
   old.uc.mem_write(old.data+0x9000,name.encode()+b'\0');new.uc.mem_write(new.data+0x9000,name.encode()+b'\0')
   expected=i32(old.invoke(0x60feec,[om,old.data+0x9000]));actual=i32(new.invoke('dh2_events_time',[nt,new.data+0x9000]));assert expected==actual,('lookup',index,name,expected,actual);counts['lookup']+=1
  for j in range(100):
   previous=rng.choice(probes);current=rng.choice(probes);start,end=sorted([rng.choice(probes),rng.choice(probes)]);enabled=j%17!=0
   old.pointer(om+8,old.callback if enabled else 0);old.events.clear();new.events.clear()
   old.invoke(0x60ecb4,[om,previous,current]);assert new.invoke('dh2_events_update_interval',[nt,previous,current,new.callback if enabled else 0,new.data+0x7000])==1
   assert old.events==new.events,('interval',index,previous,current,old.events,new.events);counts['interval']+=1;callback_events+=len(old.events)
   old.events.clear();new.events.clear()
   if j%7==0:
    last=rng.randrange(-1,len(keys)+1);old.uc.mem_write(om+16,struct.pack('<i',last));new.uc.mem_write(nm,struct.pack('<i',last))
   old.invoke(0x60ebe0,[om,previous,current,start,end]);assert new.invoke('dh2_events_update',[nt,nm,previous,current,start,end,new.callback if enabled else 0,new.data+0x7000])==1
   expected=struct.unpack('<i',old.uc.mem_read(om+16,4))[0];actual=struct.unpack('<i',new.uc.mem_read(nm,4))[0]
   assert old.events==new.events and expected==actual,('update',index,kind,keys,previous,current,start,end,expected,actual,old.events,new.events);counts['loop_update']+=1;callback_events+=len(old.events)
  assert struct.unpack('<I',old.uc.mem_read(om+4,4))[0]==1,'Original reference-count leak'
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'comparisons':sum(counts.values()),'calls':counts,'callback_events_compared':callback_events,'authored_tracks':authored,'synthetic_tracks':105,'mismatches':0,'arm64_pointers_above_4gib':True,'original_import_calls':old.import_calls,'scope':'Immutable event-track lookup, callback order/lag, interval and single-wrap dispatch plus last-entry state; libc and imported soft-float helpers modeled; callback mutation, full timeline and combat not validated','elapsed_seconds':round(time.monotonic()-started,2)}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='authored_tracks'}))
if __name__=='__main__':main()
