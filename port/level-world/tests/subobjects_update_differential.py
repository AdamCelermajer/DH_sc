"""Original GameObject::UpdateSubObjects versus compiled native ARM64 source.

Complete original coordinator, Character flag getters, IsAtDestination, AABB,
normalization and physical getter/setter instructions execute. Visual/root,
PFWorld validation, camera and auxiliary callbacks are explicit service fixtures.
SetXForm is a synchronous XY/angle fixture, with its exact arguments compared;
there is no Box2D frame integration. Finite bits/order are exact; NaNs use class.
"""
import argparse,hashlib,json,math,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from navigation_motion_differential import MotionCpu
from navigation_differential import ROOT,equal
from navigation_search_differential import word,words
from aggro_differential import float_bits

class UpdateCpu(MotionCpu):
 def external(self,uc,address,size,unused):
  if self.callback+32<=address<=self.callback+240:
   self.handler(address);uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)

class Original:
 def __init__(self,engine,manifest):
  self.c=UpdateCpu(engine,False,manifest);c=self.c;d=c.data
  self.game=d+0x1000;self.visual=d+0x3000;self.physics=d+0x4000;self.body=d+0x5000;self.aux=d+0x6000;self.camera=d+0x7000;self.manager=d+0x8000;self.vtable=d+0x9000;self.vv=d+0xa000;self.pv=d+0xb000;self.av=d+0xc000;self.listnode=d+0xd000
  c.handler=self.callback;c.uc.hook_add(UC_HOOK_CODE,self.hook)
  for off,addr in ((0x60,0x3a2e38),(0x64,0x3a2e44),(0x68,0x3a2e50),(0x6c,0x3a2e5c),(0x70,0x3a2e68),(0x74,0x3a2e78),(0x78,0x3a2e88),(0x28,c.callback+32),(0xa8,c.callback+48)):c.pointer(self.vtable+off,addr)
  c.pointer(self.vv+8,c.callback+64);c.pointer(self.pv+0x20,c.callback+80);c.pointer(self.av+12,c.callback+96)
  # GOT singleton pointers are load arguments to intercepted services.
  self.active=False
 def ret(self,value=0):c=self.c;c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def fdata(self,address,count=3):return bytes(self.c.uc.mem_read(address,count*4))
 def event(self,e,payload=b''):self.events.append((e,payload))
 def callback(self,address):
  c=self.c;kind=address-c.callback
  if kind==32:c.put(0,self.policy[6])
  elif kind==48:self.event(17,struct.pack('<f',self.policy[12]));c.put(0,float_bits(self.policy[12]))
  elif kind==64:self.event(1);c.put(0,0)
  elif kind==80:self.event(2);c.put(0,0)
  elif kind==96:self.event(14);c.put(0,0)
  elif kind==112:c.put(0,self.policy[5])
  else:raise AssertionError(kind)
 def hook(self,uc,address,size,unused):
  if not self.active:return
  c=self.c;g=self.game;f=self.fixture
  if address==0x394448:self.arrived=c.reg(0)
  elif address==0x39450c:self.accepted=c.reg(10)
  elif address==0x39468c and self.wake_pending:self.event(8);self.wake_pending=False
  elif address==0x47124c:
   self.event(3);self.wake_pending=bool(self.has_body);uc.mem_write(g+0x160,struct.pack('<3f',*f['root']));self.ret()
  elif address==0x470cb8:self.event(4,self.fdata(g+0x160));self.ret()
  elif address==0x470ccc:self.event(9) # actual empty ApplyRotation executes
  elif address==0x472948:self.event(10,self.fdata(g+0x174,1));self.ret()
  elif address==0x472860:self.event(11);self.ret()
  elif address==0x46e918:self.event(7,words(c.reg(1),c.reg(2))) # setter executes
  elif address==0x7e164c:
   payload=self.fdata(c.reg(1),2)+words(c.reg(2));self.event(6,payload)
   if f['transform_apply']:uc.mem_write(self.body+4,payload[:8]);uc.mem_write(self.body+0x38,payload[8:])
   self.ret(1)
  elif address==0x525d84:
   point=c.reg(1);self.event(5,self.fdata(point))
   if f.get('_original_provider'):
    valid,updated,cached=f['_original_provider'](self.fdata(point));uc.mem_write(point,updated);uc.mem_write(g+0x1e0,cached);self.ret(valid)
   else:uc.mem_write(point,struct.pack('<3f',*f['validated']));self.ret(f['valid'])
  elif address==0x31f594:self.event(12);self.ret(self.manager if f['camera_present'] else 0)
  elif address==0x3f94f4:self.event(13,self.fdata(c.reg(1))+self.fdata(c.reg(2)));self.ret(f['camera_valid'])
  elif address==0x597180:self.event(15);uc.mem_write(c.reg(0),struct.pack('<3f',*f['camera_position']));self.ret()
  elif address==0x41161c:self.event(16,struct.pack('<f',float(c.reg(1))));self.ret()
 def configure(self,state,body,policy,fixture):
  c=self.c;g=self.game;self.policy=struct.unpack('<12If',policy);self.fixture=fixture;self.has_body=fixture['body'];self.events=[];self.arrived=self.accepted=0;self.wake_pending=False
  c.uc.mem_write(g,bytes(0x1800));c.pointer(g,self.vtable)
  flags=sum((self.policy[i]!=0)<<i for i in range(4))|((not self.policy[4])<<4)|((not self.policy[5])<<6);c.pointer(g+0x520,flags)
  c.pointer(self.vtable+0x74,0x3a2e78 if self.policy[5] in (0,1) else c.callback+112)
  for off,raw in ((0x160,state[:12]),(0x1a8,state[12:24]),(0x1b8,state[24:36]),(0x1e0,state[36:48]),(0x174,state[48:52]),(0x144,state[52:76]),(0x12c,state[76:100]),(0x208,state[100:112])):c.uc.mem_write(g+off,raw)
  c.pointer(self.listnode,g+0x200);c.pointer(g+0x200,self.listnode if struct.unpack_from('<I',state,124)[0] else g+0x200)
  c.pointer(g+0x2d8,self.visual if self.policy[7] else 0);c.pointer(self.visual,self.vv);c.pointer(self.visual+4,g)
  c.pointer(g+0x2dc,self.physics if self.has_body else 0);c.pointer(self.physics,self.pv);c.pointer(self.physics+0x14,self.body)
  self.load_body(body)
  c.pointer(g+0x2e0,self.aux if self.policy[8] else 0);c.pointer(self.aux,self.av);c.pointer(self.aux+4,self.policy[9]);c.pointer(self.aux+0x3c,self.policy[10]);c.uc.mem_write(self.aux+12,state[112:124]);c.pointer(self.manager+0x128,self.camera);c.uc.mem_write(self.camera+0x25,bytes((fixture['camera_enabled'],)));c.pointer(self.camera+4,0)
 def load_body(self,raw):
  c=self.c;c.uc.mem_write(self.body,bytes(0x100));values=struct.unpack('<I11f',raw);c.uc.mem_write(self.body,struct.pack('<H',values[0]));
  for off,index,count in ((4,1,2),(0x38,3,1),(0x40,4,2),(0x48,6,1),(0x6c,7,2),(0x74,9,1),(0x8c,10,1)):c.uc.mem_write(self.body+off,raw[index*4:(index+count)*4])
  self.radius=raw[44:48]
 def snapshot(self):
  c=self.c;g=self.game;s=b''.join(self.fdata(g+off,count) for off,count in ((0x160,3),(0x1a8,3),(0x1b8,3),(0x1e0,3),(0x174,1),(0x144,6),(0x12c,6),(0x208,3)))+self.fdata(self.aux+12)+words(int(word(c,g+0x200)!=g+0x200))
  b=words(struct.unpack('<H',c.uc.mem_read(self.body,2))[0])+b''.join(self.fdata(self.body+off,count) for off,count in ((4,2),(0x38,1),(0x40,2),(0x48,1),(0x6c,2),(0x74,1),(0x8c,1)))+self.radius
  return s,b,words(self.arrived,bool(self.accepted)),tuple(self.events)
 def execute(self):self.active=True;self.c.invoke(0x3943cc,[self.game]);self.active=False;return self.snapshot()

class Native:
 def __init__(self,library):
  self.c=UpdateCpu(library,True,{'functions':[]});c=self.c;d=c.data;self.state=d+0x1000;self.body=d+0x2000;self.policy=d+0x3000;self.services=d+0x4000;self.request=d+0x5000;self.out=d+0x6000;self.transform=d+0x7000;c.handler=self.callback
 def configure(self,state,body,policy,fixture):
  c=self.c;self.fixture=fixture;self.events=[];c.uc.mem_write(self.state,state);c.uc.mem_write(self.body,body);c.uc.mem_write(self.policy,policy);c.uc.mem_write(self.services,struct.pack('<2Q',0,c.callback+128));c.uc.mem_write(self.transform,bytes(16));c.uc.mem_write(self.request,struct.pack('<5Q',self.state,self.body if fixture['body'] else 0,self.transform if fixture['body'] else 0,self.policy,self.services));c.uc.mem_write(self.out,bytes(8))
 def callback(self,address):
  c=self.c;e=c.reg(1);ptr=c.reg(2);f=self.fixture;payload=b'';result=0
  if e in (4,5):payload=bytes(c.uc.mem_read(self.state,12))
  elif e==10:payload=bytes(c.uc.mem_read(self.state+48,4))
  elif e==13:payload=bytes(c.uc.mem_read(self.state,12))+bytes(c.uc.mem_read(self.state+24,12))
  elif e in (6,7,16,17):payload=bytes(c.uc.mem_read(ptr,{6:12,7:8,16:4,17:4}[e]))
  self.events.append((e,payload))
  if e==3:c.uc.mem_write(self.state,struct.pack('<3f',*f['root']))
  elif e==5:
   if f.get('_native_provider'):
    result,updated,cached=f['_native_provider'](bytes(c.uc.mem_read(ptr,12)));c.uc.mem_write(ptr,updated);c.uc.mem_write(self.state+36,cached)
   else:c.uc.mem_write(ptr,struct.pack('<3f',*f['validated']));result=f['valid']
  elif e==6 and f['transform_apply']:c.uc.mem_write(self.body+4,payload[:8]);c.uc.mem_write(self.body+12,payload[8:12])
  elif e==12:c.uc.mem_write(ptr,struct.pack('<f',float(f['camera_enabled'])));result=f['camera_present']
  elif e==13:result=f['camera_valid']
  elif e==15:c.uc.mem_write(ptr,struct.pack('<3f',*f['camera_position']))
  c.put(0,result)
 def execute(self):
  c=self.c;assert c.invoke('dh2_subobjects_update',[self.out,self.request])==0
  return bytes(c.uc.mem_read(self.state,128)),bytes(c.uc.mem_read(self.body,48)),bytes(c.uc.mem_read(self.out,8)),tuple(self.events)

def state(position=(0.,0.,0.),destination=(1000.,0.,0.),previous=(13.,-17.,19.),path=False,target=(0.,1000.,300.),aux=(0.,0.,0.)):
 return struct.pack('<31fI',*position,*destination,.2,-.3,17.,*previous,.37,-1.,-2.,-3.,4.,5.,6.,*([99.]*6),*target,*aux,int(path))
def body(position=(0.,0.),flags=0):return struct.pack('<I11f',flags,*position,.91,3.,-5.,7.,11.,-13.,17.,19.,23.)
def policy(pv=0,pp=0,rv=0,rp=0,sr=1,floor=1,camera=0,visual=0,aux=0,kind=2,mode=3,speed=137.):return struct.pack('<12If',pv,pp,rv,rp,sr,floor,camera,visual,aux,kind,mode,0,speed)
def fixture(**kw):
 d={'body':True,'root':(7.,11.,13.),'validated':(23.,29.,31.),'valid':1,'camera_present':1,'camera_enabled':1,'camera_valid':1,'camera_position':(0.,0.,0.),'transform_apply':True};d.update(kw);return d
def canonical(raw):return words(*(0x7fc00000 if v&0x7f800000==0x7f800000 and v&0x7fffff else v for v in struct.unpack('<'+'I'*(len(raw)//4),raw)))

def floor_audit(args,old,new):
 from navigation_objects_differential import OriginalObjects,NativeObjects
 from navigation_world_differential import linked_state
 data=json.loads(args.floor.read_text(encoding='utf-8-sig'));triangles=[[] for _ in range(8)]
 for row in data['floor']:triangles[row['room']].append([list(struct.unpack('<3f',struct.pack('<3f',*point))) for point in row['corners']])
 bounds=[data['floor_bounds'][str(i)] for i in range(8)];geometry={'triangles':triangles,'bounds':bounds,'world_bounds':[min(b[k] for b in bounds) for k in range(3)]+[max(b[k+3] for b in bounds) for k in range(3)]}
 nodes,edges,floors=linked_state(args.linked_reference);manifest=json.loads((ROOT/'reference/navigation-objects/original-functions.json').read_text());original=OriginalObjects(args.engine,manifest,geometry);native=NativeObjects(args.floor_library,geometry);traits=[(0,1)]*8;counts={'coordinator_cases':0,'validation_calls':0,'floor_queries':0,'registry_relocations':0};outputs=[]
 def prepare(source):
  original.reset(nodes,edges,floors,traits);native.reset(nodes,edges,floors,traits);point=tuple(sum(v)/3 for v in zip(*triangles[source][0]));parameter=struct.pack('<IQ4f',0,17,*point,36.)
  original.execute(1,0,parameter);native.execute(1,0,parameter,original.keys[0]);parameter=struct.pack('<I2f',1,1.,36.);original.execute(2,0,parameter);native.execute(2,0,parameter,original.keys[0]);assert original.snapshot_object(original.objects[0])==bytes(native.c.uc.mem_read(native.objects[0],64));assert original.snapshot_registry()==native.snapshot_registry();return point
 def provider(which,point):
  parameter=point+struct.pack('<fI',1000.,0)
  if which is original:
   result,queries=which.execute(6,0,parameter);obj=which.snapshot_object(which.objects[0]);registry=which.snapshot_registry();counts['validation_calls']+=1;counts['floor_queries']+=len(queries);counts['registry_relocations']+=struct.unpack_from('<I',result,12)[0];outputs.append((result,queries,obj,registry))
  else:
   result,queries=which.execute(6,0,parameter,original.keys[0]);obj=bytes(which.c.uc.mem_read(which.objects[0],64));registry=which.snapshot_registry();expected=outputs[-1];assert equal(result,expected[0]) and queries==expected[1] and equal(obj,expected[2]) and registry==expected[3],('floor service',result,expected)
  return struct.unpack_from('<I',result)[0],result[24:36],obj[16:28]
 # Every floor-pair with early and trailing validation, attached original and
 # native obstacle registry backends. One rejected camera branch restores PF
 # cache without invoking validation, preserving its original call ordering.
 for source in range(8):
  for target in range(8):
   for floor_mode in (0,1):
    point=prepare(source);target_point=tuple(sum(v)/3 for v in zip(*triangles[target][-1]));s=state(position=point,destination=(target_point[0]+1000,target_point[1]+1000,target_point[2]),previous=point);b=body(position=(target_point[0]*.01,target_point[1]*.01));p=policy(pp=1,rp=1,visual=1,pv=1,floor=floor_mode)
    f=fixture(_original_provider=lambda point:provider(original,point),_native_provider=lambda point:provider(native,point));old.configure(s,b,p,f);new.configure(s,b,p,f);expected=old.execute();actual=new.execute()
    assert all(equal(x,y) for x,y in zip(expected[:3],actual[:3])),('Crypt coordinator state',source,target,floor_mode,expected[:3],actual[:3]);assert len(expected[3])==len(actual[3]) and all(x[0]==y[0] and equal(x[1],y[1]) for x,y in zip(expected[3],actual[3])),('Crypt coordinator ordered calls',source,target,floor_mode);counts['coordinator_cases']+=1
 checksum=hashlib.sha256(b''.join(canonical(row[0])+canonical(row[2])+row[3]+b''.join(words(floor)+point for floor,point in row[1]) for row in outputs)).hexdigest()
 return {**counts,'mismatches':0,'validation_output_sha256':checksum,'floor_source_sha256':hashlib.sha256(args.floor.read_bytes()).hexdigest(),'floor_library_sha256':hashlib.sha256(args.floor_library.read_bytes()).hexdigest(),'linked_reference_sha256':hashlib.sha256(args.linked_reference.read_bytes()).hexdigest(),'scope':'Callback bridge executes complete original and compiled ARM64 PF position validation, selectors and obstacle parent registry in independent instruction CPUs. Crypt authored floor geometry and PFObject initialization are attached; no supplied validity answers. Visual/root, transform, camera and auxiliary backends remain fixtures.'}

def main():
 p=argparse.ArgumentParser()
 for name in ('engine','library','report','reference-output'):p.add_argument('--'+name,type=Path,required=True)
 for name in ('floor','linked-reference','floor-library'):p.add_argument('--'+name,type=Path)
 p.add_argument('--cases',type=int,default=1024);a=p.parse_args();started=time.monotonic();manifest=json.loads((ROOT/'reference/subobjects-update/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=Original(a.engine,manifest);new=Native(a.library);rng=random.Random(20261003);records=[];samples=[];counts={};carry=0
 def compare(s=state(),b=body(),p=policy(),f=None,label=None):
  f=f or fixture();old.configure(s,b,p,f);new.configure(s,b,p,f);expected=old.execute();actual=new.execute()
  for i,(x,y) in enumerate(zip(expected[:3],actual[:3])):assert equal(x,y),(len(records),label,i,x.hex(),y.hex())
  assert len(expected[3])==len(actual[3]),(len(records),label,'events',expected[3],actual[3])
  for x,y in zip(expected[3],actual[3]):assert x[0]==y[0] and equal(x[1],y[1]),(len(records),label,'event',expected[3],actual[3])
  for e,payload in expected[3]:counts[str(e)]=counts.get(str(e),0)+1
  eventraw=words(len(expected[3]))+b''.join(words(e,len(payload))+canonical(payload) for e,payload in expected[3]);config=struct.pack('<7I9f',f['body'],f['valid'],f['camera_present'],f['camera_enabled'],f['camera_valid'],f['transform_apply'],0,*f['root'],*f['validated'],*f['camera_position']);record=canonical(s)+canonical(b)+p+config+b''.join(canonical(x) for x in expected[:3])+eventraw;records.append(words(len(record))+record)
  if label:samples.append({'case':label,'events':[e for e,payload in expected[3]],'result':list(struct.unpack('<2I',expected[2]))})
  return expected
 for visual in (0,1):
  for hasbody in (False,True):
   for flags in (0,8):
    for bits in range(32):compare(p=policy(pv=bits&1,pp=(bits>>1)&1,rv=(bits>>2)&1,rp=(bits>>3)&1,sr=(bits>>4)&1,visual=visual),b=body(flags=flags),f=fixture(body=hasbody))
 for d in (0.,.999999,1.,1.000001,-1.,-1.000001):
  for axis in (0,1):
   bp=[0.,0.];bp[axis]=d/100.;compare(b=body(position=bp),p=policy(pv=1,pp=1,visual=1),label=f'physics strict epsilon {axis} {d}')
 for distance in (0.,19.999998,20.,20.000002,79.99999,80.,80.00001):
  for z in (0.,1000.):compare(s=state(destination=(distance,0.,z)),p=policy(pp=1,rp=1),label=f'distance {distance} Z {z}')
 for valid in (0,1):
  for floor in (0,1,2):
   for camera in (0,1):
    for present in (0,1):
     for canmove in (0,1):compare(b=body(position=(1.,2.)),p=policy(pv=1,pp=1,rp=1,visual=1,floor=floor,camera=camera),f=fixture(valid=valid,camera_present=present,camera_valid=canmove),label=f'validation {valid} floor {floor} camera {camera} present {present} allowed {canmove}')
 for mode in (0,1,2,3,4,5):
  for kind in (1,2):
   for enabled in (0,1):
    for delta in (0.,math.sqrt(3.),2.):compare(s=state(aux=(delta,0.,0.)),p=policy(aux=1,kind=kind,mode=mode),f=fixture(camera_enabled=enabled),label=f'aux {kind} mode {mode} enabled {enabled} distance {delta}')
 for special in (0.,-0.,math.inf,-math.inf,math.nan,1e30,-1e30):
  for axis in range(3):
   position=[0.,0.,0.];position[axis]=special;compare(s=state(position=position),p=policy(pp=1,pv=1,rp=1,visual=1),f=fixture(transform_apply=False),label=f'IEEE position {axis} {special}')
 for i in range(a.cases):
  pos=[rng.uniform(-1000,1000) for _ in range(3)];dest=[x+rng.uniform(-1000,1000) for x in pos];bp=[x*.01+rng.choice((0.,.00999,.01,.010001,10.)) for x in pos[:2]]
  compare(s=state(position=pos,destination=dest,path=bool(i%3),aux=pos),b=body(position=bp,flags=rng.choice((0,1,2,8,10,65535))),p=policy(pv=rng.randrange(2),pp=rng.randrange(2),rv=rng.randrange(2),rp=rng.randrange(2),sr=rng.randrange(2),floor=rng.randrange(2),camera=rng.randrange(2),visual=rng.randrange(2),aux=rng.randrange(2),mode=rng.randrange(6),speed=rng.choice((0.,-137.,137.,1e30,math.nan))),f=fixture(body=bool(rng.randrange(2)),valid=rng.randrange(2),camera_valid=rng.randrange(2),transform_apply=bool(rng.randrange(2))))
 # Exact output state feeds successive calls; no fabricated frame advancement.
 s=state();b=body(position=(2.,3.));p=policy(pv=1,pp=1,rp=1,visual=1,floor=0,aux=1,mode=4)
 for i in range(32):out=compare(s,b,p,fixture(valid=i%2,camera_valid=i%3!=0),label=f'carry tick {i}');s,b=out[:2];carry+=1
 # Atomic rejection leaves every caller field untouched and emits no events.
 rejection=0
 for kind in range(11):
  new.configure(state(),body(),policy(),fixture());c=new.c;before=bytes(c.uc.mem_read(new.state,128))+bytes(c.uc.mem_read(new.body,48));args=[new.out,new.request]
  if kind==0:args[0]=0
  elif kind==1:args[1]=0
  elif kind==2:c.pointer(new.request,0)
  elif kind==3:c.pointer(new.request+16,0)
  elif kind==4:c.pointer(new.request+24,0)
  elif kind==5:c.pointer(new.request+32,0)
  elif kind==6:args[0]=new.state
  elif kind==7:c.pointer(new.services+8,0)
  elif kind==8:c.uc.mem_write(new.body,words(65536))
  elif kind==9:c.uc.mem_write(new.policy+44,words(1))
  elif kind==10:c.pointer(new.request+16,new.body)
  before=bytes(c.uc.mem_read(new.state,128))+bytes(c.uc.mem_read(new.body,48))
  assert c.invoke('dh2_subobjects_update',args)==1;assert not new.events;assert before==bytes(c.uc.mem_read(new.state,128))+bytes(c.uc.mem_read(new.body,48));rejection+=1
 reference=words(0x31554253,len(records))+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(reference)
 code=set(range(0x3943cc,0x3949a4,4));observed=code&old.c.seen
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':len(records),'random_cases':a.cases,'state_carry_ticks':carry,'atomic_rejection_checks':rejection,'mismatches':0,'ordered_event_counts':counts,'behavior_snapshots':samples,'finite_bits_exact':True,'arithmetic_nan_comparison':'NaN class; payload/sign not portable','original_functions_seen':sorted({old.c.address_owner[x] for x in old.c.seen}),'coordinator_executable_instructions':len(code),'coordinator_instructions_observed':len(observed),'coordinator_unobserved_instructions':[hex(x) for x in sorted(code-observed)],'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)}
 if a.floor or a.linked_reference or a.floor_library:
  assert a.floor and a.linked_reference and a.floor_library,'floor integration needs all three arguments';report['crypt_floor_registry_integration']=floor_audit(a,old,new);a.report.with_name('subobjects-update-crypt-floor-integration.json').write_text(json.dumps(report['crypt_floor_registry_integration'],indent=2)+'\n')
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='behavior_snapshots'}))
if __name__=='__main__':main()
