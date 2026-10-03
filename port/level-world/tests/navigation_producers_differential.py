"""Original GameObject UpdatePFObject and concrete producers versus ARM64.

UpdatePFObject, actual concrete vtable dispatch, physical getRadius, InitObstacle
and its map/deque instructions execute. GameObject bounds, user link, existing
body radius/ownership and scene storage are fixtures. Original independent
selector oracles execute floor queries. IEEE imports are modeled. This does not
reconstruct body constructors, capability initialization or the moving controller.
"""
import argparse,hashlib,json,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from navigation_objects_differential import OriginalObjects,NativeObjects,canonical_registry
from navigation_world_differential import linked_state
from navigation_differential import ROOT,equal
from navigation_search_differential import words,word

VTABLES=(0x964750,0x965f38,0x9658d8,0x967548,0x965700)
GETTERS=((0x340104,0x34010c,0x340114),(0x3a2ee8,0x3a2ef0,0x3a2efc),
         (0x39f3a8,0x39f3b0,0x39f3bc),(0x3ee40c,0x3ee414,0x3ee420),
         (0x39cafc,0x39cb04,0x39cb10))

class OriginalProducers(OriginalObjects):
 def __init__(self,engine,manifest,geometry):
  super().__init__(engine,manifest,geometry)
  self.games=[self.c.data+0x70000+i*0x400 for i in range(8)]
  self.objects=[p+0x1c8 for p in self.games]
  self.bodies=[self.c.data+0x1a00000+i*0x100 for i in range(8)]
  c=self.c;got=0x393ec0+word(c,0x393f88);c.pointer(got+word(c,0x393f8c),self.world)
 def hook(self,uc,address,size,unused):
  if getattr(self,'active',False) and address==0x528234:
   self.obstacle_calls.append(bytes(uc.mem_read(self.c.reg(1)+8,4)))
  super().hook(uc,address,size,unused)
 def producer_traits(self,mode):
  return words(self.c.invoke(GETTERS[mode][0],[self.games[0]]),
               self.c.invoke(GETTERS[mode][1],[self.games[0]]),
               self.c.invoke(GETTERS[mode][2],[self.games[0]]),0)
 def update(self,i,fields):
  mode,present,radius,reserved,x0,y0,x1,y1=struct.unpack('<2IfI4f',fields)
  c=self.c;game=self.games[i];c.pointer(game,VTABLES[mode]);c.pointer(game+0x2dc,self.bodies[i] if present else 0)
  c.uc.mem_write(self.bodies[i]+12,struct.pack('<f',radius))
  c.uc.mem_write(game+0x144,struct.pack('<2f',x0,y0));c.uc.mem_write(game+0x150,struct.pack('<2f',x1,y1))
  self.queries=[];self.obstacle_calls=[];c.invoke(0x393ea0,[game],budget=20000000)
  return tuple(self.queries),tuple(self.obstacle_calls)

class NativeProducers(NativeObjects):
 def __init__(self,library,geometry):
  super().__init__(library,geometry);self.fields=self.c.data+0x70e000
  self.c.uc.hook_add(UC_HOOK_CODE,self.producer_hook,
                    begin=self.c.symbols['dh2_nav_init_obstacle'],end=self.c.symbols['dh2_nav_init_obstacle'])
 def producer_hook(self,uc,address,size,unused):
  p=struct.unpack('<Q',uc.mem_read(self.c.reg(0)+16,8))[0]
  self.obstacle_calls.append(bytes(uc.mem_read(p+48,4)))
 def producer_traits(self,mode):
  assert self.c.invoke('dh2_nav_producer_traits',[self.out,mode])==0
  return bytes(self.c.uc.mem_read(self.out,16))
 def update(self,i,fields):
  c=self.c;c.uc.mem_write(self.fields,fields)
  c.uc.mem_write(self.request,struct.pack('<5Q',self.cw,self.registry,self.objects[i],0x100000001+i,self.fields))
  self.queries=[];self.obstacle_calls=[]
  assert c.invoke('dh2_nav_update_game_object',[self.request],budget=20000000)==0
  return tuple(self.queries),tuple(self.obstacle_calls)

def main():
 p=argparse.ArgumentParser()
 for name in ('engine','library','floor','linked-reference','report','reference-output'):p.add_argument('--'+name,type=Path,required=True)
 p.add_argument('--cases',type=int,default=1024);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/navigation-producers/original-functions.json').read_text())
 assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 data=json.loads(a.floor.read_text(encoding='utf-8-sig'));triangles=[[] for _ in range(8)]
 for row in data['floor']:triangles[row['room']].append([list(struct.unpack('<3f',struct.pack('<3f',*point))) for point in row['corners']])
 bounds=[data['floor_bounds'][str(i)] for i in range(8)]
 geometry={'triangles':triangles,'bounds':bounds,'world_bounds':[min(b[k] for b in bounds) for k in range(3)]+[max(b[k+3] for b in bounds) for k in range(3)]}
 nodes,edges,floors=linked_state(a.linked_reference);old=OriginalProducers(a.engine,manifest,geometry);new=NativeProducers(a.library,geometry)
 traits=[(0,1)]*8;records=[];totals={'updates':0,'class_counts':[0]*5,'null_user':0,'physical_radius':0,'bounds_radius':0,'obstacle_calls':0,'floor_queries':0,'registry_entries_compared':0,'registry_keys_compared':0};snapshots=[]
 def objects(o):return b''.join(o.snapshot_object(p) for p in o.objects) if o is old else b''.join(bytes(o.c.uc.mem_read(p,64)) for p in o.objects)
 def reset():old.reset(nodes,edges,floors,traits);new.reset(nodes,edges,floors,traits);assert objects(old)==objects(new)
 def fixture(i,raw):old.set_object(i,bytes(raw));new.c.uc.mem_write(new.objects[i],bytes(raw))
 def compare(i,fields,label=None):
  before=objects(old);before_reg=old.snapshot_registry();assert before==objects(new) and before_reg==new.snapshot_registry()
  expected=old.update(i,fields);actual=new.update(i,fields)
  after=objects(old);registry=old.snapshot_registry()
  assert expected==actual,(len(records),label,'query/call order',expected,actual)
  assert after==objects(new),(len(records),label,'objects',after[i*64:(i+1)*64].hex(),objects(new)[i*64:(i+1)*64].hex())
  assert registry==new.snapshot_registry(),(len(records),label,'registry')
  mode,present=struct.unpack_from('<2I',fields);user=struct.unpack_from('<Q',before,i*64+40)[0];nk,nm=struct.unpack_from('<2I',registry)
  totals['updates']+=1;totals['class_counts'][mode]+=1;totals['null_user']+=int(not user)
  totals['physical_radius']+=int(bool(user and present));totals['bounds_radius']+=int(bool(user and not present));totals['obstacle_calls']+=len(expected[1]);totals['floor_queries']+=len(expected[0]);totals['registry_entries_compared']+=nm;totals['registry_keys_compared']+=nk
  records.append(words(i,len(before_reg),len(registry),len(expected[0]),len(expected[1]))+fields+before+before_reg+after+registry+b''.join(words(floor)+point for floor,point in expected[0])+b''.join(expected[1]))
  if label:snapshots.append({'case':label,'object_words':list(struct.unpack('<16I',after[i*64:(i+1)*64])),'obstacle_calls':len(expected[1]),'registry_keys':nk,'registry_members':nm})
  return after[i*64:(i+1)*64],registry
 def center(row):return [sum(values)/3 for values in zip(*row)]
 points=[[center(row) for row in rows] for rows in triangles]
 def fields(mode,present,radius=.36,bounds=(-36.,-24.,36.,24.)):return struct.pack('<2IfI4f',mode,present,radius,0,*bounds)
 def actor(i,floor,flags=8,user=17,point=None):
  return struct.pack('<4I6fQ3fI',2,flags,floor,floor,*(point or points[floor][0]),0.,0.,1.,user,36.,1.,0.,0)
 reset();trait_gold=b''.join(old.producer_traits(mode) for mode in range(5));assert trait_gold==b''.join(new.producer_traits(mode) for mode in range(5))
 probe=0xcbf29ce484222325;probe_counts={'floors':0,'registered':0,'physical_radius_updates':0}
 for floor in range(8):
  reset();fixture(0,actor(0,floor));state,registry=compare(0,fields(1,1,.36),f'Crypt floor {floor} Character producer')
  probe_counts['floors']+=1;probe_counts['registered']+=int(bool(struct.unpack_from('<I',state,4)[0]&4));probe_counts['physical_radius_updates']+=int(struct.unpack_from('<f',state,48)[0]==struct.unpack('<f',struct.pack('<f',.36*100))[0])
  for value in words(floor)+state+registry:probe=((probe^value)*0x100000001b3)&0xffffffffffffffff
 probe_counts['state_fnv1a64']=f'{probe:016x}'
 for mode in range(5):
  for present in (0,1):
   for user in (0,17):
    reset();fixture(0,actor(0,0,user=user));compare(0,fields(mode,present),f'class {mode} body {present} user {user}')
 # User-null preserves existing registration. Base GameObject does not call
 # InitObstacle even if fixture membership was already registered.
 for mode in range(5):
  reset();fixture(0,actor(0,0));compare(0,fields(1,1),f'initial register before class {mode}')
  compare(0,fields(mode,0,bounds=(-10.,-50.,30.,50.)),f'body removal class {mode}')
  raw=bytearray(objects(old)[:64]);struct.pack_into('<Q',raw,40,0);fixture(0,raw)
  compare(0,fields(mode,1,2.),f'null user retains state class {mode}')
 for mode in range(5):
  for radius in (-0.,0.,.001,.36,1.,17.,1000.):
   reset();fixture(0,actor(0,0));compare(0,fields(mode,1,radius),f'physical radius {radius} class {mode}')
  for box in ((0.,0.,0.,0.),(-2.,-7.,4.,7.),(-7.,-2.,7.,4.),(-2.,-2.,2.,2.),(2.,2.,2.,2.),(-0.,0.,0.,-0.),(-1e6,-1e4,1e6,1e4)):
   reset();fixture(0,actor(0,0));compare(0,fields(mode,0,bounds=box),f'bounds {box} class {mode}')
 for point in (points[3][0],[1e9]*3):
  for mode in range(1,5):
   for flag in (8,9):
    reset();raw=actor(0,0,flags=flag,point=point);raw=bytearray(raw);struct.pack_into('<2I',raw,8,0xffffffff,0xffffffff);fixture(0,raw)
    compare(0,fields(mode,1),f'uncached floor class {mode} flag {flag} miss {point==[1e9]*3}')
 # Deliberate duplicate registration crosses original deque block growth.
 reset()
 for i in range(8):fixture(i,actor(i,0,user=17+i))
 for n in range(72):
  i=n%8;raw=bytearray(objects(old)[i*64:(i+1)*64]);struct.pack_into('<I',raw,4,8);fixture(i,raw);compare(i,fields(1,1))
 for i in range(8):compare(i,fields(1,0),f'first duplicate removal {i}')
 rng=random.Random(20261017)
 for n in range(a.cases):
  if n%64==0:reset()
  i=rng.randrange(8);mode=rng.randrange(5);present=rng.randrange(2)
  if n%5==0:
   floor=rng.randrange(8);raw=bytearray(actor(i,floor,flags=rng.choice((0,1,8,9,12,13)),user=rng.choice((0,17+i))))
   if n%15==0:struct.pack_into('<2I',raw,8,0xffffffff,0xffffffff)
   fixture(i,raw)
  lowx,lowy=rng.uniform(-1000,0),rng.uniform(-1000,0)
  box=(lowx,lowy,lowx+rng.uniform(0,2000),lowy+rng.uniform(0,2000))
  compare(i,fields(mode,present,rng.choice((0.,.001,.36,1.,10.,1000.)),box))
  if n%256==255:print(f'Producer updates {n+1}/{a.cases}',flush=True)
 geometry_raw=words(8)+b''.join(words(len(rows))+b''.join(struct.pack('<9f',*(x for point in row for x in point)) for row in rows)+struct.pack('<6f',*bounds[i]) for i,rows in enumerate(triangles))
 reference=words(0x31445250,len(records),8,len(nodes)//56,len(edges)//20,len(floors)//48)+b''.join(struct.pack('<Q',key) for key in old.keys)+nodes+edges+floors+geometry_raw+trait_gold+b''.join(records)
 a.reference_output.write_bytes(reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'linked_reference_sha256':hashlib.sha256(a.linked_reference.read_bytes()).hexdigest(),'floor_source_sha256':hashlib.sha256(a.floor.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':len(records)+5,'trait_comparisons':5,'mismatches':0,'totals':totals,'crypt_producers_probe':probe_counts,'behavior_snapshots':snapshots,'full_registry_and_all_object_fields_compared':True,'original_concrete_vtable_dispatch_executes':True,'obstacle_before_radius_update_verified':True,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)}
 a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='behavior_snapshots'}),flush=True)
if __name__=='__main__':main()
