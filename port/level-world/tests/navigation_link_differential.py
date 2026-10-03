"""Full original PFFloor::_Link against native ARM64 floor sewing.

Original midpoint lookup/insertion/RB balancing, forced node construction,
boundary selection, arithmetic/thresholds, directed edge writes, validation
references and neighbour-floor sets execute. Shared graph allocation/edge
deduplication and temporary pair-vector growth are caller storage services.
Initial Crypt geometry/bounds/default flags are separately asset-verified.
This test does not execute route search or the complete floor loader.
"""
import argparse,hashlib,json,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_S0
from navigation_differential import Cpu,equal,ROOT
from selector_differential import OriginalSelectorOracle

CAPACITY=32768
def words(*x):return struct.pack('<'+'I'*len(x),*x)
def triangle(corners):return struct.pack('<9f',*(x for p in corners for x in p))
def fnv(raw):
 result=0xcbf29ce484222325
 for x in struct.unpack('<'+'I'*(len(raw)//4),raw):
  if x&0x7f800000==0x7f800000 and x&0x7fffff:x=0x7fc00000
  for byte in struct.pack('<I',x):result=((result^byte)*0x100000001b3)&0xffffffffffffffff
 return f'{result:016x}'

class OriginalGraph:
 def __init__(self,engine,manifest):
  self.c=Cpu(engine,False,manifest);c=self.c;self.og=c.data+0x1000;self.vt=c.data+0x2000;self.temp=c.data+0x3000
  self.floors=[];self.nodes={};self.edges={};self.members=set();self.pairs=0;self.allocations=0;self.support=[];self.current=0;self.oracle=None;self.plan=None;self.distance_pass=0;self.vertical_pass=0
  c.pointer(self.vt,c.callback+96);c.pointer(self.vt+0x14,0x51bc60)
  c.uc.hook_add(UC_HOOK_CODE,self.hook);self.reset()
 def word(self,p):return struct.unpack('<I',self.c.uc.mem_read(p,4))[0]
 def put(self,p,value):self.c.uc.mem_write(p,words(value))
 def ret(self,value=0):self.c.put(0,value);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def allocate(self,n):
  p=self.heap;self.heap+=(n+15)&~15;assert self.heap<self.c.data+0x1800000
  self.c.uc.mem_write(p,bytes(n));return p
 def reset(self):
  self.heap=self.c.data+0x200000;self.floors.clear();self.nodes.clear();self.edges.clear();self.members.clear();self.support.clear();self.c.uc.mem_write(self.og,bytes(64))
 def hook(self,uc,address,size,unused):
  c=self.c
  if self.plan is not None and address==0x51fd78:self.plan.append((self.floors.index(c.reg(0)),self.floors.index(c.reg(1))));self.ret()
  elif self.plan is not None and address==0x51bee8:self.plan.append((self.floors.index(c.reg(0)),0xffffffff));self.ret()
  elif self.plan is not None and address in (0x337888,0x337a88):self.ret()
  elif self.plan is not None and address==0x3140ec:uc.mem_write(c.reg(0),bytes(24));self.ret(c.reg(0))
  elif address==0x51fa64:self.current=self.floors.index(c.reg(0))
  elif address==0x5201ec:self.distance_pass+=1
  elif address==0x520204:self.vertical_pass+=1
  elif address==0x51badc:
   assert c.reg(0)==self.floors[self.current] and c.reg(2)==c.reg(3)==0
   point=bytes(uc.mem_read(c.reg(1),12));status=struct.unpack_from('<I',self.oracle.execute('floor',point))[0] if self.oracle else self.answer
   self.support.append((point,status));self.ret(status)
  elif address==0x51dca4:
   assert c.reg(0)==self.og;ident=c.reg(1);assert ident==len(self.nodes)+1
   p=self.allocate(48);c.pointer(p,self.vt);self.put(p+4,ident);self.nodes[ident]=(p,self.current);self.allocations+=1;self.ret(p)
  elif address==0x51e758:
   assert c.reg(0)==self.og;key=(c.reg(1),c.reg(2));assert all(n in self.nodes for n in key)
   if key not in self.edges:
    p=self.allocate(24);c.pointer(p,self.vt);c.pointer(p+4,self.nodes[key[0]][0]);c.pointer(p+8,self.nodes[key[1]][0]);self.edges[key]=(p,len(self.edges)+1);self.allocations+=1
   self.ret(self.edges[key][0])
  elif address==0x708ec0:self.ret(self.allocate(self.word(c.reg(0))))
  elif address==0x708f00 or address==0x310440:self.ret()
  elif address==0x51c8fc:
   # Storage fixture for the Link-local vector<pair<InvalidNode*,Node*>>.
   v=c.reg(0);value=bytes(uc.mem_read(c.reg(2),8));begin=self.word(v);end=self.word(v+4)
   if not begin:begin=end=self.allocate(65536);uc.mem_write(v,words(begin,end,begin+65536))
   assert end+8<=self.word(v+8);uc.mem_write(end,value);self.put(v+4,end+8);self.pairs+=1;self.ret()
  elif address==0x51df98:
   # Observe the real set insertion rather than replacing it.
   owner=next(i for i,p in enumerate(self.floors) if c.reg(1)==p+0x78);other=self.floors.index(self.word(c.reg(2)));self.members.add((owner,other))
 def begin(self,bounds,flags):
  index=len(self.floors);p=self.c.data+0x10000+index*0x30000;self.floors.append(p);self.c.uc.mem_write(p,bytes(0x100));self.put(p+0x20,flags);self.c.pointer(p+0x74,self.og)
  self.c.uc.mem_write(p+0x44,struct.pack('<6f',*bounds))
  for offset in (0x78,0x90):self.c.uc.mem_write(p+offset,words(0,0,p+offset,p+offset,0))
  self.c.uc.mem_write(p+0xa8,words(p+0x1000,p+0x1000,p+0x10000))
  self.c.uc.mem_write(p+0xc0,words(p+0x11000,p+0x11000,p+0x20000));return index
 def post_load_plan(self,scene,groups=None):
  c=self.c;world=c.data+0x1900000;table=world+0x1000;groups=groups or [[i] for i in range(len(scene))]
  c.uc.mem_write(world,words(0,1,table,table+len(groups)*4,table+len(groups)*4));room_bounds=[]
  for room,indices in enumerate(groups):
   p=world+0x2000+room*0x100;raw=bytes(0x100);c.uc.mem_write(p,raw);c.pointer(table+room*4,p)
   bounds=[min(scene[i][0][k] for i in indices) for k in range(3)]+[max(scene[i][0][k+3] for i in indices) for k in range(3)]
   room_bounds.append(bounds);c.uc.mem_write(p+0x3c,struct.pack('<6f',*bounds));begin=p+0x60;c.uc.mem_write(begin,words(*(self.floors[i] for i in indices)));c.uc.mem_write(p+0x30,words(begin,begin+len(indices)*4,begin+len(indices)*4))
  self.plan=[];c.invoke(0x5226cc,[world],budget=50000000);plan=self.plan;self.plan=None;return plan,groups,room_bounds
 def build(self,index,corners,answer=0):
  self.current=index;self.answer=answer;self.support.clear();self.c.uc.mem_write(self.temp,triangle(corners));self.c.invoke(0x520588,[self.floors[index],self.temp,1]);return list(self.support)
 def snapshot(self):
  c=self.c;ptr_ids={p:i for i,(p,f) in self.nodes.items()};edge_ids={p:i for p,i in self.edges.values()};trees={};roots=[]
  for f,p in enumerate(self.floors):
   header=p+0x90;root=self.word(header+4);mapping={}
   def collect(at):
    if not at:return
    mapping[at]=ptr_ids[self.word(at+28)];collect(self.word(at+8));collect(self.word(at+12))
   collect(root);roots.append(mapping.get(root,0))
   for at,ident in mapping.items():trees[ident]=words(mapping.get(self.word(at+4),0),mapping.get(self.word(at+8),0),mapping.get(self.word(at+12),0),int(not c.uc.mem_read(at,1)[0]))
  nodes=b''.join(words(i,f)+bytes(c.uc.mem_read(p+8,32))+trees[i] for i,(p,f) in self.nodes.items())
  edges=b''.join(words(*key)+bytes(c.uc.mem_read(p+0x10,8))+bytes(c.uc.mem_read(p+0xc,4)) for key,(p,i) in self.edges.items())
  validations=[];invalid=[]
  for p in self.floors:
   validations.append([edge_ids.get(self.word(at),0) for at in range(self.word(p+0xc0),self.word(p+0xc4),4)])
   raw=bytearray(c.uc.mem_read(self.word(p+0xa8),self.word(p+0xac)-self.word(p+0xa8)))
   for offset in range(0,len(raw),56):
    for k in (36,40):struct.pack_into('<I',raw,offset+k,ptr_ids.get(struct.unpack_from('<I',raw,offset+k)[0],0))
   invalid.append(bytes(raw))
  return nodes,edges,invalid,validations,roots

class NativeGraph:
 def __init__(self,library):
  self.c=Cpu(library,True,{'functions':[]});c=self.c;self.g=c.data+0x1000;self.t=c.data+0x2000;self.f=c.data+0x3000;self.w=c.data+0x4000
  self.nodes=c.data+0x10000;self.edges=c.data+0x100000;self.invalid=c.data+0x200000;self.validation=c.data+0x300000
  self.owners=c.data+0x340000;self.links=c.data+0x380000;self.first=c.data+0x400000;self.second=c.data+0x440000;self.floor_count=0
 def reset(self):
  self.floor_count=0;self.c.uc.mem_write(self.g,struct.pack('<4Q12I2Q',self.nodes,self.edges,self.invalid,self.validation,0,0,0,0,4096,CAPACITY,4096,CAPACITY,0,0,0,0,self.c.callback+96,0))
  self.c.uc.mem_write(self.w,struct.pack('<QQIIQQII',self.first,self.second,4096,0,self.owners,self.links,0,4096))
 def counts(self):return struct.unpack('<4I',self.c.uc.mem_read(self.g+32,16))
 def begin(self,bounds,flags):
  i=self.floor_count;self.floor_count+=1;n,e,invalid,v=self.counts();assert self.c.invoke('dh2_nav_begin_floor',[self.g,i])==0
  self.c.uc.mem_write(self.f+i*48,words(i,flags,0,n,invalid,0)+struct.pack('<6f',*bounds))
 def build(self,index,corners,queries):
  c=self.c;c.queries=queries;c.query_index=0;c.uc.mem_write(self.t,triangle(corners));before=self.counts()[3]
  assert c.invoke('dh2_nav_triangle',[self.g,self.t,struct.unpack('<I',c.uc.mem_read(self.f+index*48+4,4))[0]])==0
  assert c.query_index==len(queries)
  for i in range(before,self.counts()[3]):c.uc.mem_write(self.owners+4*i,words(index))
  first=struct.unpack('<I',c.uc.mem_read(self.f+index*48+16,4))[0];c.uc.mem_write(self.f+index*48+20,words(self.counts()[2]-first));c.uc.mem_write(self.f+index*48+8,bytes(c.uc.mem_read(self.g+64,4)))
 def snapshot(self):
  c=self.c;n,e,invalid,v=self.counts();validations=[[] for _ in range(self.floor_count)]
  for i in range(v):validations[struct.unpack('<I',c.uc.mem_read(self.owners+4*i,4))[0]].append(struct.unpack('<I',c.uc.mem_read(self.validation+4*i,4))[0])
  rows=[];roots=[]
  for i in range(self.floor_count):
   root,first,count=struct.unpack('<I4xII',c.uc.mem_read(self.f+i*48+8,16));roots.append(root);rows.append(bytes(c.uc.mem_read(self.invalid+56*first,56*count)))
  return bytes(c.uc.mem_read(self.nodes,n*56)),bytes(c.uc.mem_read(self.edges,e*20)),rows,validations,roots
 def serialized(self):
  c=self.c;n,e,invalid,v=self.counts();link_count=struct.unpack('<I',c.uc.mem_read(self.w+40,4))[0]
  return words(n,e,invalid,v,self.floor_count,link_count)+bytes(c.uc.mem_read(self.g+64,16))+bytes(c.uc.mem_read(self.nodes,n*56))+bytes(c.uc.mem_read(self.edges,e*20))+bytes(c.uc.mem_read(self.invalid,invalid*56))+bytes(c.uc.mem_read(self.validation,v*4))+bytes(c.uc.mem_read(self.owners,v*4))+bytes(c.uc.mem_read(self.f,self.floor_count*48))+bytes(c.uc.mem_read(self.links,link_count*8))

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--floor',type=Path,required=True)
 p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--cases',type=int,default=240);a=p.parse_args();start=time.monotonic()
 manifest=json.loads((ROOT/'reference/navigation-link/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=OriginalGraph(a.engine,manifest);new=NativeGraph(a.library);rng=random.Random(20261014);cases=[];records=[];comparisons=0;link_calls=0;forced=0;initial_triangles=0
 source=json.loads(a.floor.read_text());by_floor={}
 for row in source['floor']:by_floor.setdefault(row['room'],[]).append(row['corners'])
 cases.append([(source['floor_bounds'][str(i)],source['floor_object_flags'][str(i)],by_floor[i],None) for i in by_floor])
 # Exact strict distance/vertical cutoffs and inclusive +/-1 bounds. Wide
 # supplied bounds isolate thresholds from the room overlap prefilter.
 wide=[-1000.,-1000.,-1000.,1000.,1000.,1000.]
 for dx,dz,width in [(x,0.,10.) for x in (19.999998,20.,20.000002)]+[(0.,z,100.) for z in (99.99999,100.,100.00001)]:
  for flags in (1,0x02000001):
   cases.append([(wide,1,[[[0.,0.,0.],[width,0.,0.],[0.,width,0.]]],0),
    (wide,flags,[[[dx,0.,dz],[dx+width,0.,dz],[dx,width,dz]]],0)])
 for limit in (5.9999995,6.,6.0000005,-1.0000001,-1.,-.99999994):
  bounds=list(wide)
  if limit>0:bounds[0]=limit
  else:bounds[3]=limit
  cases.append([(wide,1,[[[0.,0.,0.],[10.,0.,0.],[0.,10.,0.]]],0),
   (bounds,1,[[[0.,0.,0.],[10.,0.,0.],[0.,10.,0.]]],0)])
 for index in range(a.cases):
  scene=[]
  for f in range(2+rng.randrange(3)):
   x,y,z=(rng.uniform(-50,50) for k in range(3));triangles=[]
   for k in range(1+rng.randrange(4)):
    width=rng.choice((0.,1.,10.,50.,100.));triangles.append([[x,y,z],[x+width,y,z],[x,y+width,z+rng.choice((0.,1.,99.999,100.,100.001))]])
   flags=rng.choice((1,1,1,0x01000000,0x02000001,0x04000001));scene.append(([x-5,y-5,z-1000,x+110,y+110,z+1000],flags,triangles,rng.choice((0,1))))
  cases.append(scene)
 collision_manifest=json.loads((ROOT/'reference/collision/original-functions.json').read_text());oracle=OriginalSelectorOracle(a.engine,collision_manifest)
 def compare(label):
  nonlocal comparisons
  expected=old.snapshot();actual=new.snapshot();assert equal(expected[0],actual[0]),('nodes',label);assert equal(expected[1],actual[1]),('edges',label)
  assert len(expected[2])==len(actual[2]) and all(equal(x,y) for x,y in zip(expected[2],actual[2])),('invalid',label)
  assert expected[3:]==actual[3:],('validation/tree roots',label,expected[3:],actual[3:])
  count=struct.unpack('<I',new.c.uc.mem_read(new.w+40,4))[0];members={struct.unpack('<2I',new.c.uc.mem_read(new.links+i*8,8)) for i in range(count)};assert members==old.members,('floor sets',label,members,old.members);comparisons+=1
 for ci,scene in enumerate(cases):
  old.reset();new.reset();old.oracle=oracle if ci==0 else None
  for i,(bounds,flags,ts,answer) in enumerate(scene):
   old.begin(bounds,flags);new.begin(bounds,flags)
   if old.oracle:
    raw=b''.join(triangle(t) for t in ts);oracle.build(raw,15);oracle.configure(None,True);oracle.prepare(raw,struct.pack('<6f',*bounds))
   for t in ts:new.build(i,t,old.build(i,t,answer));initial_triangles+=1
   compare((ci,'build',i))
  initial=new.serialized();steps=[]
  plan,groups,room_bounds=old.post_load_plan(scene,[[i] for i in range(len(scene))] if not ci or ci%2 else [list(range(len(scene)))])
  def overlap(a,b,margin):
   at=new.t;new.c.uc.mem_write(at,struct.pack('<12f',*a,*b));new.c.uc.reg_write(UC_ARM64_REG_S0,struct.unpack('<I',struct.pack('<f',margin))[0]);return new.c.invoke('dh2_nav_bounds_overlap',[at,at+12,at+24,at+36])
  native_plan=[]
  for room,indices in enumerate(groups):
   for other in range(room+1,len(groups)):
    if overlap(room_bounds[room],room_bounds[other],50):
     for i in indices:
      for j in groups[other]:
       if not (scene[i][1]|scene[j][1])&0x04000000 and overlap(scene[i][0],scene[j][0],50):native_plan.append((i,j))
   for at,i in enumerate(indices):
    if scene[i][1]&0x04000000:continue
    for j in indices[at+1:]:
     if not scene[j][1]&0x04000000 and overlap(scene[i][0],scene[j][0],0):native_plan.append((i,j))
    native_plan.append((i,0xffffffff))
  assert plan==native_plan,('post load call order',ci,plan,native_plan)
  # Synthetic direct pairs additionally exercise the floor-level disabled gate.
  if ci:
   plan=[(i,j) for i in range(len(scene)) for j in range(i+1,len(scene))]
   if ci<=18:plan+=list(plan)+[(j,i) for i,j in plan]
  for i,j in plan:
   if j!=0xffffffff:
    before=len(old.nodes);old.c.invoke(0x51fd78,[old.floors[i],old.floors[j]],budget=50000000)
    assert new.c.invoke('dh2_nav_link',[new.g,new.f+i*48,new.f+j*48,new.w],budget=50000000)==0,(ci,i,j,new.counts())
    forced+=len(old.nodes)-before;link_calls+=1;compare((ci,'link',i,j));expected=new.serialized();steps.append(words(i,j,len(expected))+expected)
   else:
    old.c.invoke(0x51bee8,[old.floors[i]]);new.c.uc.mem_write(new.f+i*48+20,words(0));compare((ci,'post',i));expected=new.serialized();steps.append(words(i,0xffffffff,len(expected))+expected)
  records.append(words(len(initial))+initial+words(len(steps))+b''.join(steps))
  if ci==0:crypt={'nodes':new.counts()[0],'edges':new.counts()[1],'validation_references':new.counts()[3],'state_fnv1a64':fnv(new.serialized()),'neighbour_floor_relations':len(old.members)}
 reference=words(0x314b4e4c,len(records))+b''.join(records);a.reference_output.write_bytes(reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'floor_source_sha256':hashlib.sha256(a.floor.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'cases':len(cases),'initial_triangle_comparisons':initial_triangles,'logical_snapshot_comparisons':comparisons,'link_calls':link_calls,'forced_nodes_created':forced,'crypt':crypt,'mismatches':0,'temporary_pair_vector_services':old.pairs,'graph_storage_allocations':old.allocations,'distance_threshold_passes':old.distance_pass,'vertical_threshold_passes':old.vertical_pass,'postload_caller_order_comparisons':len(cases),'scope':__doc__,'elapsed_seconds':round(time.monotonic()-start,2)}
 a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
