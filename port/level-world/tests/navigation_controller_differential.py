"""Original UpdatePath/IsAtDestination versus compiled native coordination.

MovePath, IsAtDestination, Stop, heading, force/avoidance, boundary steering and
original container instructions execute. Actual Character path/physics virtual
getters execute; avoidance policy, debug preference and physical setter services
are caller fixtures. Physical Stop emits a service request in the native view;
its backend, UpdateSubObjects, actor initialization/root motion are not verified.
Original floor queries execute independent original selector/collision oracles.
Imported IEEE/libm are modeled; graph/body ownership uses bounded fixtures.
"""
import argparse,hashlib,json,random,struct,time
from pathlib import Path
from unittest.mock import patch
from navigation_avoidance_differential import OriginalAvoidance,NativeAvoidance,actor
from navigation_path_differential import OriginalPath,NativePath,PathCpu
from navigation_heading_differential import HeadingCpu
from navigation_motion_differential import MotionCpu
import navigation_search_differential as search
import navigation_motion_differential as motion
from navigation_world_differential import linked_state
from navigation_search_differential import words,word
from navigation_objects_differential import canonical_registry
from navigation_differential import ROOT,equal
from aggro_differential import float_bits

class ControllerCpu(HeadingCpu,PathCpu,MotionCpu):
 def external(self,uc,address,size,unused):
  if address==self.callback+192:
   self.put(0,self.controller_avoid);uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)

class OriginalController(OriginalAvoidance):
 def __init__(self,engine,manifest,geometry):
  with patch.object(search,'SearchCpu',ControllerCpu):super().__init__(engine,manifest,geometry)
  c=self.c;self.games=[c.data+0x70000+i*0x800 for i in range(8)];self.objects=[p+0x1c8 for p in self.games];self.cvtable=c.data+0x1d01000
  c.pointer(self.cvtable+0x5c,0x3a2e2c);c.pointer(self.cvtable+0x64,0x3a2e44);c.pointer(self.cvtable+0x7c,c.callback+192)
  got=0x3940d8+word(c,0x394324)
  # GOT world offset is read from UpdatePath's literal at 0x39432c.
  c.pointer(got+word(c,0x39432c),self.world)
  c.pointer(self.evt+32,0x51c114);c.pointer(self.evt+36,0x51b844);c.pointer(self.evt+40,0x51b85c)
  self.running=False
 def set_actor(self,i,raw):
  super().set_actor(i,raw);c=self.c;p=self.objects[i];canonical=bytearray(raw[:64]);struct.pack_into('<Q',canonical,40,self.games[i] if struct.unpack_from('<Q',raw,40)[0] else 0);self.set_object(i,bytes(canonical))
  c.uc.mem_write(p+64,raw[64:76]);c.pointer(p+124,0);c.uc.mem_write(p+128,bytes(12));self.list(p+56,[])
  c.pointer(self.physics[i]+0x14,c.data+0x1e00000+i*0x100);c.uc.mem_write(c.data+0x1e00000+i*0x100,bytes(0x100))
 def hook(self,uc,address,size,unused):
  if getattr(self,'running',False):
   c=self.c
   if address==c.callback+192:self.ret(self.policy_values[1]);return
   if address==0x337a88:self.ret(self.policy_values[2]);return
   if address in (0x46e918,0x46e978,0x46ea80):self.physical_calls.append(address);self.ret();return
   if address==0x310570:self.ret(self.allocate(c.reg(0)));return
   if address==0x52d838:self.move_before=struct.unpack_from('<I',self.path_snapshot(),68)[0]
   elif address==0x393600 and self.move_before is not None:
    count=struct.unpack_from('<I',self.path_snapshot(),68)[0];self.move=words(bool(count),count<self.move_before)+bytes(uc.mem_read(c.reg(1),12))+words(0)
   elif address==0x394158:self.at_destination=c.reg(0)
   elif address==0x3938f8:self.stopped=1
   elif address==0x525d60:self.boundary_checked=1
   elif address==0x394300:self.direction_valid=c.reg(0)
  super().hook(uc,address,size,unused)
 def path_snapshot(self):return OriginalPath.snapshot(self)
 def object_snapshot(self,i):
  raw=bytearray(self.snapshot_object(self.objects[i]));struct.pack_into('<Q',raw,40,(0x100000001+i) if struct.unpack_from('<Q',raw,40)[0] else 0);return bytes(raw)
 def configure(self,i,state,policy,path_raw):
  c=self.c;game=self.games[i];self.obj=self.objects[i];self.policy_values=struct.unpack('<4I',policy);up,avoid,debug,physics=self.policy_values;c.controller_avoid=avoid
  c.pointer(game,self.cvtable);c.pointer(game+0x520,(up<<7)|(physics<<1));c.uc.mem_write(game+0x160,state[:12]);c.uc.mem_write(game+0x1a8,state[12:24]);c.uc.mem_write(game+0x1b8,state[24:36]);c.uc.mem_write(game+0x178,state[36:40]);c.uc.mem_write(game+0x1b5,bytes((struct.unpack_from('<I',state,40)[0],)));c.uc.mem_write(game+0x1b4,bytes((struct.unpack_from('<I',state,48)[0],)));c.uc.mem_write(game+0x1c4,bytes((struct.unpack_from('<I',state,52)[0],)))
  p=self.obj;c.uc.mem_write(p+8,path_raw[4:8]);c.pointer(p+20,struct.unpack_from('<I',path_raw)[0]);c.uc.mem_write(p+24,path_raw[32:44]);c.uc.mem_write(p+64,path_raw[44:56]);c.uc.mem_write(p+128,path_raw[56:68]);c.pointer(p+124,struct.unpack_from('<I',path_raw,72)[0]);embedded=p+76
  c.uc.mem_write(embedded,words(0x96c810,0,0,float_bits(1.),0,0)+path_raw[8:32]);self.edge_ptrs[0]=embedded;c.edge_ids[embedded]=0
  ids=[]
  for at in range(76,len(path_raw),48):
   ident=struct.unpack_from('<I',path_raw,at)[0];ids.append(ident)
   if ident==0xffffffff:
    ptr=self.allocate(48);c.uc.mem_write(ptr,words(0x96c810,0,0)+path_raw[at+12:at+24]+path_raw[at+24:at+48]);self.edge_ptrs[ident]=ptr;c.edge_ids[ptr]=ident
  self.list(p+56,ids);assert equal(self.path_snapshot(),path_raw)
 def state_snapshot(self,i):
  c=self.c;game=self.games[i];return bytes(c.uc.mem_read(game+0x160,12))+bytes(c.uc.mem_read(game+0x1a8,12))+bytes(c.uc.mem_read(game+0x1b8,12))+bytes(c.uc.mem_read(game+0x178,4))+words(c.uc.mem_read(game+0x1b5,1)[0],0,c.uc.mem_read(game+0x1b4,1)[0],c.uc.mem_read(game+0x1c4,1)[0])
 def execute(self,op,i):
  self.queries=[];self.physical_calls=[];self.move_before=None;self.move=bytes(24);self.force=bytes(16);self.evaluated=self.adjusted=self.turn_limited=0;self.at_destination=self.stopped=self.boundary_checked=self.direction_valid=0;self.records=b'';self.running=True
  if op==0:self.at_destination=self.c.invoke(0x39361c,[self.games[i]])
  else:self.c.invoke(0x3940c0,[self.games[i]],budget=20000000)
  self.running=False;result=self.move+self.force+words(self.evaluated,self.adjusted,self.turn_limited,0,self.at_destination,self.stopped,self.boundary_checked,self.direction_valid,bool(self.physical_calls),0)
  assert len(self.physical_calls) in (0,3)
  return self.state_snapshot(i),self.object_snapshot(i),self.path_snapshot(),self.snapshot_registry(),result,tuple(self.queries)

class NativeController(NativeAvoidance):
 def __init__(self,library,geometry):
  with patch.object(motion,'MotionCpu',ControllerCpu):super().__init__(library,geometry)
  c=self.c;d=c.data;self.graph=d+0x1000;self.state=d+0x730000;self.cp=d+0x730100;self.po=d+0x731000;self.segments=d+0x732000;self.cr=d+0x740000;self.cwspace=d+0x740100;self.co=d+0x740200
  c.uc.mem_write(self.cwspace,struct.pack('<Q2IQ2IQ2I',d+0x741000,1024,0,d+0x750000,8,0,d+0x751000,16,0))
 def configure(self,i,state,policy,path_raw):
  c=self.c;c.uc.mem_write(self.state,state);c.uc.mem_write(self.cp,policy);count,owned=struct.unpack_from('<2I',path_raw,68);c.uc.mem_write(self.po,path_raw[:68]+bytes(4)+struct.pack('<Q4I',self.segments,count,1024,owned,0))
  if count:c.uc.mem_write(self.segments,path_raw[76:])
  c.uc.mem_write(self.cr,struct.pack('<9Q',self.state,self.po,self.objects[i],self.cw,self.graph,self.scene,self.cp,self.cwspace,0x100000001+i));c.uc.mem_write(self.co,bytes(80))
 def execute(self,op,i):
  c=self.c;self.queries=[]
  if op==0:
   answer=c.invoke('dh2_nav_is_at_destination',[self.state,self.po]);assert answer in (0,1);c.uc.mem_write(self.co+56,words(answer))
  else:assert c.invoke('dh2_nav_update_path',[self.co,self.cr],budget=20000000)==0
  self.obj=self.po;snapshot=NativePath.snapshot(self)
  return bytes(c.uc.mem_read(self.state,56)),bytes(c.uc.mem_read(self.objects[i],64)),snapshot,self.snapshot_registry(),bytes(c.uc.mem_read(self.co,80)),tuple(self.queries)

def main():
 p=argparse.ArgumentParser()
 for name in ('engine','library','floor','linked-reference','report','reference-output'):p.add_argument('--'+name,type=Path,required=True)
 p.add_argument('--cases',type=int,default=384);a=p.parse_args();started=time.monotonic();manifest=json.loads((ROOT/'reference/navigation-controller/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 data=json.loads(a.floor.read_text(encoding='utf-8-sig'));triangles=[[] for _ in range(8)]
 for row in data['floor']:triangles[row['room']].append([list(struct.unpack('<3f',struct.pack('<3f',*point))) for point in row['corners']])
 bounds=[data['floor_bounds'][str(i)] for i in range(8)];geometry={'triangles':triangles,'bounds':bounds,'world_bounds':[min(b[k] for b in bounds) for k in range(3)]+[max(b[k+3] for b in bounds) for k in range(3)]}
 nodes,edges,floors=linked_state(a.linked_reference);old=OriginalController(a.engine,manifest,geometry);new=NativeController(a.library,geometry);old.reset(nodes,edges,floors,[(0,1)]*8);new.reset(nodes,edges,floors,[(0,1)]*8)
 records=[];snapshots=[];rng=random.Random(20261019);totals={n:0 for n in ('destination_queries','updates','move_calls','at_destination','stopped','avoidance_evaluated','adjusted','turn_limited','boundary_checks','direction_valid','physical_stop_requests','floor_queries')}
 def path(position,target,ids=(),radius=36.,waypoint=(13.,-17.,0.),owned=0):
  rows=[]
  for ident in ids:
   if ident in (0,0xffffffff):row=words(ident,0,0)+struct.pack('<9f',1.,0.,0.,*position,*target)
   else:
    aa,bb,distance,clearance,weight=struct.unpack_from('<2I3f',edges,(ident-1)*20);row=words(ident,aa,bb)+struct.pack('<3f',weight,distance,clearance)+nodes[(aa-1)*56+8:(aa-1)*56+20]+nodes[(bb-1)*56+8:(bb-1)*56+20]
   rows.append(row)
  return words(2)+struct.pack('<16f',radius,*position,*target,*position,*target,*waypoint)+words(len(ids),owned)+b''.join(rows)
 def state(position,destination,requested=1,boundary=1,heading=(.2,-.3,17.),angle=.37,active=1):return struct.pack('<10f4I',*position,*destination,*heading,angle,active,0,requested,boundary)
 def compare(op,position,destination,ids=(),policy=(1,1,0,0),requested=1,boundary=1,floor=0,flags=14,waypoint=(13.,-17.,0.),owned=0,members=None,label=None,initial=None,target=None,coincident=False,path_override=None,object_override=None,registry_override=None):
  actors=[];target=target or destination
  for j in range(8):
   delta=(0,0) if j==0 or coincident else (30+j*2,10+j*3)
   actors.append(actor(position=(position[0]+delta[0],position[1]+delta[1],position[2]+j*17),target=destination,path_target=target,floor=floor,flags=flags if j==0 else 14,user=0x100000001+j,has_path=int(bool(ids)),physical=(1,0,0,1,0,1,65535,1,0,0,0,0)))
  if object_override:actors[0]=object_override+actors[0][64:]
  members=members if members is not None else [(floor,j) for j in range(8)];registry=canonical_registry([(f,[0x100000001+j for fi,j in members if fi==f]) for f in sorted({f for f,_ in members})]);raw=b''.join(actors);before=initial or state(position,destination,requested,boundary);policy_raw=words(*policy);path_raw=path_override or path(position,target,ids,waypoint=waypoint,owned=owned)
  if registry_override is not None:registry=registry_override
  old.load_scene(raw,registry);new.load_scene(raw,registry);old.configure(0,before,policy_raw,path_raw);new.configure(0,before,policy_raw,path_raw)
  expected=old.execute(op,0);actual=new.execute(op,0)
  for kind,(left,right) in enumerate(zip(expected,actual)):
   assert (left==right if kind in (3,5) else equal(left,right)),(len(records),label,kind,left.hex() if isinstance(left,bytes) else left,right.hex() if isinstance(right,bytes) else right)
  output=struct.unpack('<20I',expected[4]);totals[('destination_queries','updates')[op]]+=1;totals['move_calls']+=int(old.move_before is not None);totals['at_destination']+=output[14];totals['stopped']+=output[15];totals['avoidance_evaluated']+=output[10];totals['adjusted']+=output[11];totals['turn_limited']+=output[12];totals['boundary_checks']+=output[16];totals['direction_valid']+=output[17];totals['physical_stop_requests']+=output[18];totals['floor_queries']+=len(expected[5])
  records.append(words(op,len(path_raw),len(expected[2]),len(registry),len(expected[3]),len(expected[5]))+policy_raw+before+raw+path_raw+registry+b''.join(expected[:5])+b''.join(words(f)+point for f,point in expected[5]))
  if label:snapshots.append({'case':label,'output_words':list(output),'remaining_segments':struct.unpack_from('<I',expected[2],68)[0]})
  return expected
 for floor in range(8):
  point=tuple(sum(v)/3 for v in zip(*triangles[floor][0]));end=(point[0]+1000,point[1]+1000,point[2]);compare(1,point,end,floor=floor,label=f'Crypt floor {floor} coordinator')
 for length in (0.,79.999,80.,80.001,1000.):
  for z in (0.,10000.):
   for op in (0,1):
    for requested in (0,1):compare(op,(0.,0.,0.),(length,0.,z),requested=requested,boundary=0,policy=(1,0,0,1),label=f'destination {length} Z {z} op {op} requested {requested}')
 for ids,owned in (((),0),((0,),0),((0xffffffff,),1),((1,2,3),0),((1,),0)):
  for up in (0,1):
   for avoid in (0,1):
    for skip in (0,1):compare(1,(0.,0.,0.),(1000.,1000.,1000.),ids=ids,owned=owned,policy=(up,avoid,skip,0),label=f'path {ids} gates {up} {avoid} {skip}')
 for flags in (0,1,2,4,8,12,14,15):compare(1,(0.,0.,0.),(1000.,0.,0.),flags=flags,boundary=0,label=f'PF flag gate {flags}')
 compare(1,(0.,0.,0.),(1000.,0.,0.),boundary=0,coincident=True,label='coincident force NaNs preserve comparison branches')
 for policy in ((0,1,0,1),(1,0,0,0),(1,1,0,0),(1,1,1,1)):
  compare(1,(100.,200.,300.),(100.,200.,-10000.),policy=policy,requested=0,initial=state((100.,200.,300.),(100.,200.,-10000.),0,1,active=0),label=f'inactive arrived heading gates {policy}')
  compare(1,(100.,200.,300.),(1000.,200.,300.),policy=policy,initial=state((100.,200.,300.),(1000.,200.,300.),1,1,active=0),label=f'inactive heading starts gates {policy}')
 for floor in range(8):
  for tri in triangles[floor][:8]:
   point=tuple(tri[0])
   for dx,dy in ((1000.,0.),(-1000.,0.),(0.,1000.),(0.,-1000.)):
    compare(1,point,(point[0]+dx,point[1]+dy,point[2]),floor=floor,policy=(1,0,0,0),members=[])
 compare(1,(1e9,1e9,1e9),(1e9+1000,1e9+1000,1e9),policy=(1,0,0,0),members=[],label='boundary query miss retains heading')
 compare(1,(0.,0.,-3e38),(1000.,1000.,3e38),policy=(1,0,0,0),boundary=0,members=[],label='overflowing Z delta is ignored by original facing')
 compare(1,(0.,0.,0.),(1e30,1e30,0.),policy=(1,0,0,0),boundary=0,members=[],label='overflowing squared XY magnitude follows original arithmetic')
 # Carry the exact controller/PF/path state through successive ticks. Position
 # advancement is an explicit test input, not a reconstructed physics backend.
 for chain in range(4):
  ids=(1+chain*8,2+chain*8,3+chain*8);first_from=struct.unpack_from('<I',edges,(ids[0]-1)*20)[0];point=list(struct.unpack_from('<3f',nodes,(first_from-1)*56+8));point[0]-=100;goal=(point[0]+700,point[1]+300,point[2]);previous=None
  for tick in range(96):
   before=bytearray(previous[0]) if previous else bytearray(state(point,goal,boundary=0));struct.pack_into('<3f',before,0,*point)
   previous=compare(1,point,goal,ids=ids,policy=(1,chain%2,0,1),boundary=0,members=[],initial=bytes(before),path_override=previous[2] if previous else None,object_override=previous[1] if previous else None,registry_override=previous[3] if previous else None,label=f'carry chain {chain} tick {tick}' if tick in (0,95) else None)
   destination=struct.unpack_from('<3f',previous[0],12);dx,dy=destination[0]-point[0],destination[1]-point[1];distance=(dx*dx+dy*dy)**.5
   if distance>0:point[0]+=min(40.,distance)*dx/distance;point[1]+=min(40.,distance)*dy/distance
 for n in range(a.cases):
  floor=n%8;point=tuple(sum(v)/3 for v in zip(*rng.choice(triangles[floor])));destination=tuple(point[k]+rng.uniform(-2000,2000) for k in range(3));ids=tuple(rng.randrange(1,len(edges)//20+1) for _ in range(rng.randrange(5))) if n%3==0 else ();policy=(rng.randrange(2),rng.randrange(2),rng.randrange(2),rng.randrange(2));members=[(floor,rng.randrange(8)) for _ in range(rng.randrange(16))]
  compare(int(n%5!=0),point,destination,ids=ids,policy=policy,requested=rng.randrange(2),boundary=rng.randrange(2),floor=floor,flags=rng.choice((0,1,2,8,12,14,15)),members=members,waypoint=tuple(rng.uniform(-100,100) for _ in range(3)))
 geometry_raw=words(8)+b''.join(words(len(rows))+b''.join(struct.pack('<9f',*(x for p in row for x in p)) for row in rows)+struct.pack('<6f',*bounds[i]) for i,rows in enumerate(triangles))
 reference=words(0x31445055,len(records),len(nodes)//56,len(edges)//20,len(floors)//48)+nodes+edges+floors+geometry_raw+b''.join(records);a.reference_output.write_bytes(reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'linked_reference_sha256':hashlib.sha256(a.linked_reference.read_bytes()).hexdigest(),'floor_source_sha256':hashlib.sha256(a.floor.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':len(records),'mismatches':0,'totals':totals,'behavior_snapshots':snapshots,'original_coordinator_and_helpers_execute':True,'physical_setters_are_services':True,'physical_backend_reconstructed':False,'imported_libm_modeled':True,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)}
 a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='behavior_snapshots'}))
if __name__=='__main__':main()
