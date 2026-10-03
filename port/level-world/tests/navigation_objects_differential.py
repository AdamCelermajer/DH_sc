"""Original PFObject motion/obstacle initialization and parent lists vs ARM64.

Actual InitObject, InitObstacle, parent relocation, map/deque lookup, insertion,
erase and allocator instructions execute. Allocator storage and scene ownership
are fixtures. Original floor queries execute independent selector oracles.
The PFObject constructor executes, but only the recovered motion/obstacle view
is compared; path/debug containers are outside that view. Vec3f initialization
executes before it. Full force/avoidance, actor producers and controller pending.
"""
import argparse,hashlib,json,random,struct,time
from pathlib import Path
from navigation_world_differential import OriginalWorld,linked_state
from navigation_motion_differential import NativeMotion
from navigation_search_differential import word,words
from navigation_differential import ROOT,equal
from aggro_differential import float_bits

COUNT=8
class OriginalObjects(OriginalWorld):
 def __init__(self,engine,manifest,geometry):
  super().__init__(engine,manifest,geometry);self.objects=[self.c.data+0x40000+i*0x200 for i in range(COUNT)];self.keys=[0x100000001+i for i in range(COUNT)];self.c.invoke(0x312e00,[])
 def hook(self,uc,address,size,unused):
  if getattr(self,'active',False):
   if address==0x525dac and self.c.reg(0):self.kind=1
   elif address==0x525e88:self.kind=2
   elif address==0x525eec:self.kind=3
  super().hook(uc,address,size,unused)
 def reset(self,nodes,edges,floors,traits):
  self.prepare(nodes,edges,floors,traits);head=self.world+44;self.c.uc.mem_write(head,words(0,0,head,head,0));self.default_policy=None
  for p in self.objects:self.c.uc.mem_write(p,b'\xcc'*176);self.c.invoke(0x524644,[p])
 def snapshot_object(self,p):
  c=self.c;rp=word(c,p+12);fp=word(c,p+16)
  return words(word(c,p+20),word(c,p+4),self.room.index(rp) if rp else 0xffffffff,self.fp.index(fp) if fp else 0xffffffff)+bytes(c.uc.mem_read(p+24,24))+struct.pack('<Q',word(c,p))+bytes(c.uc.mem_read(p+8,4))+bytes(c.uc.mem_read(p+48,8))+words(0)
 def set_object(self,i,raw):
  c=self.c;p=self.objects[i];flags,of,room,floor=struct.unpack_from('<4I',raw);c.pointer(p+20,flags);c.pointer(p+4,of);c.pointer(p+12,self.room[room] if room!=0xffffffff else 0);c.pointer(p+16,self.fp[floor] if floor!=0xffffffff else 0);c.uc.mem_write(p+24,raw[16:40]);c.pointer(p,struct.unpack_from('<Q',raw,40)[0]);c.uc.mem_write(p+8,raw[48:52]);c.uc.mem_write(p+48,raw[52:60])
 def snapshot_registry(self):
  c=self.c;rows=[]
  def visit(p):
   if not p:return
   visit(word(c,p+8));fp=word(c,p+16);floor=self.fp.index(fp) if fp else 0xffffffff;v=p+20;cur=word(c,v);last=word(c,v+8);node=word(c,v+12);end=word(c,v+16);members=[]
   while cur!=end:
    members.append(self.keys[self.objects.index(word(c,cur))]);cur+=4
    if cur==last:node+=4;cur=word(c,node);last=cur+128
    assert len(members)<512
   rows.append((floor,members));visit(word(c,p+12))
  visit(word(c,self.world+48));return canonical_registry(rows)
 def execute(self,op,i,parameter):
  c=self.c;p=self.objects[i];d=c.data+0x31000;c.uc.mem_write(d,parameter or bytes(4));self.queries=[];extra=b''
  if op==0:c.invoke(0x524644,[p])
  elif op==1:
   flying,user,x,y,z,radius=struct.unpack('<IQ4f',parameter);c.uc.mem_write(d,struct.pack('<3f',x,y,z));c.invoke(0x526b18,[self.world,p,flying,d,float_bits(radius),user])
  elif op==2:
   enabled,weight,extent=struct.unpack('<I2f',parameter);c.invoke(0x528234,[self.world,p,enabled,float_bits(weight),float_bits(extent)])
  elif op==3:
   floor=struct.unpack('<I',parameter)[0];c.invoke(0x528484,[self.world,p,self.fp[floor] if floor!=0xffffffff else 0])
  elif op in (4,5):
   value=struct.unpack('<I',parameter)[0];c.invoke(0x5241f4 if op==4 else 0x524218,[p,value]);extra=words(c.invoke(0x5241e8,[p]),c.invoke(0x52420c,[p]))
  elif op==6:
   self.kind=0;oldfloor=word(c,p+16);oldflags=word(c,p+4);c.uc.mem_write(self.world+144,parameter[12:]);valid=c.invoke(0x525d84,[self.world,d,p],budget=20000000);newfloor=word(c,p+16)
   old=self.fp.index(oldfloor) if oldfloor else 0xffffffff;new=self.fp.index(newfloor) if newfloor else 0xffffffff
   extra=words(valid,self.kind,int(self.kind==2),int(self.kind==2 and oldflags&4 and old!=new),old,new)+bytes(c.uc.mem_read(d,12))
  elif op==7:
   # Constructor-owned deque/map storage executes; compare policy only.
   w=c.data+0x60000;c.uc.mem_write(w,b'\xcc'*160);c.invoke(0x522e2c,[w]);extra=bytes(c.uc.mem_read(w+144,4))+words(c.uc.mem_read(w+148,1)[0])
  else:raise AssertionError(op)
  return extra,tuple(self.queries)

def canonical_registry(rows):
 rows=sorted(rows,key=lambda row:0 if row[0]==0xffffffff else row[0]+1)
 return words(len(rows),sum(len(members) for _,members in rows))+words(*(floor for floor,_ in rows))+b''.join(struct.pack('<IIQ',floor,0,key) for floor,members in rows for key in members)

class NativeObjects(NativeMotion):
 def __init__(self,library,geometry):
  super().__init__(library,geometry);self.objects=[self.c.data+0x708000+i*64 for i in range(COUNT)];self.registry=self.c.data+0x709000;self.entries=self.registry+0x100;self.buckets=self.registry+0x3000;self.request=self.c.data+0x70d000
 def reset(self,nodes,edges,floors,traits):
  self.prepare(nodes,edges,floors,traits);self.c.uc.mem_write(self.registry,struct.pack('<Q2IQ2I',self.entries,0,512,self.buckets,0,16))
  for p in self.objects:assert self.c.invoke('dh2_nav_object_defaults',[p])==0
 def snapshot_registry(self):
  c=self.c;n=word(c,self.registry+8);f=word(c,self.registry+24);keys=struct.unpack('<'+'I'*f,c.uc.mem_read(self.buckets,f*4)) if f else ();rows={k:[] for k in keys}
  for index in range(n):floor,reserved,key=struct.unpack('<IIQ',c.uc.mem_read(self.entries+16*index,16));assert reserved==0;rows[floor].append(key)
  return canonical_registry(rows.items())
 def execute(self,op,i,parameter,key):
  c=self.c;p=self.objects[i];r=self.request;extra=b'';self.queries=[]
  if op==0:status=c.invoke('dh2_nav_object_defaults',[p])
  elif op==1:
   flying,user,x,y,z,radius=struct.unpack('<IQ4f',parameter);c.uc.mem_write(r,struct.pack('<3Q4f2I',self.cw,p,user,x,y,z,radius,flying,0));status=c.invoke('dh2_nav_init_object',[r])
  elif op==2:
   enabled,weight,extent=struct.unpack('<I2f',parameter);c.uc.mem_write(r,struct.pack('<4Q2f2I',self.cw,self.registry,p,key,weight,extent,enabled,0));status=c.invoke('dh2_nav_init_obstacle',[r])
  elif op==3:status=c.invoke('dh2_nav_change_obstacle_parent',[self.registry,p,key,struct.unpack('<I',parameter)[0]])
  elif op in (4,5):
   status=c.invoke('dh2_nav_object_set_flying' if op==4 else 'dh2_nav_object_set_swimming',[p,struct.unpack('<I',parameter)[0]]);extra=words(c.invoke('dh2_nav_object_is_flying',[p]),c.invoke('dh2_nav_object_is_swimming',[p]))
  elif op==6:
   c.uc.mem_write(self.input,parameter);c.uc.mem_write(r,struct.pack('<6Q',self.cw,self.registry,p,key,self.input,self.input+12));status=c.invoke('dh2_nav_validate_object_position',[self.out,r],budget=20000000);extra=bytes(c.uc.mem_read(self.out,24))+bytes(c.uc.mem_read(self.input,12))
  elif op==7:status=c.invoke('dh2_nav_motion_policy_defaults',[self.out]);extra=bytes(c.uc.mem_read(self.out,8))
  else:raise AssertionError(op)
  assert status==0,(op,i,status,parameter.hex());return extra,tuple(self.queries)

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--floor',type=Path,required=True);p.add_argument('--linked-reference',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--cases',type=int,default=640);a=p.parse_args();started=time.monotonic();manifest=json.loads((ROOT/'reference/navigation-objects/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];data=json.loads(a.floor.read_text(encoding='utf-8-sig'));triangles=[[] for _ in range(8)]
 for row in data['floor']:triangles[row['room']].append([list(struct.unpack('<3f',struct.pack('<3f',*point))) for point in row['corners']])
 bounds=[data['floor_bounds'][str(i)] for i in range(8)];geometry={'triangles':triangles,'bounds':bounds,'world_bounds':[min(b[k] for b in bounds) for k in range(3)]+[max(b[k+3] for b in bounds) for k in range(3)]};nodes,edges,floors=linked_state(a.linked_reference);old=OriginalObjects(a.engine,manifest,geometry);new=NativeObjects(a.library,geometry);rng=random.Random(20261013);records=[];totals={name:0 for name in ('defaults','init_object','init_obstacle','parent_change','set_flying','set_swimming','position','policy_defaults','floor_queries','registry_entries_compared','registry_keys_compared')};snapshots=[];traits=[(0,1)]*8
 def objects(o):return b''.join(o.snapshot_object(p) for p in o.objects) if o is old else b''.join(bytes(o.c.uc.mem_read(p,64)) for p in o.objects)
 def reset():
  old.reset(nodes,edges,floors,traits);new.reset(nodes,edges,floors,traits);assert objects(old)==objects(new),(objects(old).hex(),objects(new).hex())
 def fixture(i,raw):old.set_object(i,raw);new.c.uc.mem_write(new.objects[i],raw)
 def compare(op,i,parameter=b'',label=None):
  before=objects(old);before_registry=old.snapshot_registry();assert before==objects(new) and before_registry==new.snapshot_registry()
  expected=old.execute(op,i,parameter);actual=new.execute(op,i,parameter,old.keys[i]);after=objects(old);registry=old.snapshot_registry();native=objects(new)
  assert equal(expected[0],actual[0]) and expected[1]==actual[1],(len(records),op,label,'result/query',expected,actual)
  assert after==native,(len(records),op,label,'objects',after[i*64:(i+1)*64].hex(),native[i*64:(i+1)*64].hex())
  assert registry==new.snapshot_registry(),(len(records),op,label,'registry',registry.hex(),new.snapshot_registry().hex())
  keys,entries=struct.unpack_from('<2I',registry);totals[('defaults','init_object','init_obstacle','parent_change','set_flying','set_swimming','position','policy_defaults')[op]]+=1;totals['floor_queries']+=len(expected[1]);totals['registry_entries_compared']+=entries;totals['registry_keys_compared']+=keys
  records.append(words(op,i,len(parameter),len(expected[0]),len(before_registry),len(registry),len(expected[1]))+parameter+before+before_registry+expected[0]+after+registry+b''.join(words(floor)+point for floor,point in expected[1]))
  if label:snapshots.append({'case':label,'object_words':list(struct.unpack('<16I',after[i*64:(i+1)*64])),'registry_floor_keys':keys,'registry_members':entries,'floor_queries':len(expected[1])})
  return expected[0],after[i*64:(i+1)*64],registry
 def center(triangle):return [sum(values)/3 for values in zip(*triangle)]
 points=[[center(row) for row in rows] for rows in triangles]
 probe_digest=0xcbf29ce484222325;probe_counts={'floor_pairs':0,'accepted':0,'registered':0,'relocated':0}
 for source in range(8):
  for target in range(8):
   reset();compare(1,source,struct.pack('<IQ4f',0,17+source,*points[source][0],36.));compare(2,source,struct.pack('<I2f',1,1.,36.));point=points[target][-1]
   result,state,registry=compare(6,source,struct.pack('<4fI',point[0],point[1],point[2]+36,1000.,0));probe_counts['floor_pairs']+=1;probe_counts['accepted']+=struct.unpack_from('<I',result,4)[0]==2;probe_counts['registered']+=struct.unpack_from('<I',registry,4)[0]==1;probe_counts['relocated']+=struct.unpack_from('<I',result,12)[0]!=0
   for byte in words(source,target)+result+state+registry:probe_digest=((probe_digest^byte)*0x100000001b3)&0xffffffffffffffff
 probe_counts['state_fnv1a64']=f'{probe_digest:016x}'
 reset();compare(7,0,label='original world height policy');compare(0,0,label='default motion and obstacle fields')
 for i in range(8):
  compare(1,i,struct.pack('<IQ4f',0,17+i,*points[i][0],.5),f'Crypt grounded floor {i}');compare(2,i,struct.pack('<I2f',1,1.,36.),f'enable Crypt obstacle {i}')
 # Relocate through position validation; full original parent backend executes.
 for i in range(8):
  point=points[(i+1)%8][-1];compare(6,i,struct.pack('<4fI',point[0],point[1],point[2]+36,1000.,0),f'accepted position relocates {i}')
 # Direct relocation deliberately leaves the cached floor unchanged.
 compare(3,0,words(0xffffffff),'null target floor is an actual map key');compare(3,0,words(4),'missing old membership does not append');compare(2,0,struct.pack('<I2f',0,2.,1.),'disable missing old membership');compare(2,0,struct.pack('<I2f',1,2.,1.),'enable appends to cached floor');compare(2,0,struct.pack('<I2f',1,4.,-1.),'negative extent stays enabled');compare(2,0,struct.pack('<I2f',1,0.,1.),'zero weight accepted');compare(2,0,struct.pack('<I2f',1,4.,-0.),'signed zero extent disables')
 # Duplicates are legal caller state; remove only the first matching member.
 reset();compare(1,0,struct.pack('<IQ4f',0,17,*points[0][0],36.));compare(2,0,struct.pack('<I2f',1,1.,1.));raw=bytearray(objects(old)[:64]);struct.pack_into('<I',raw,4,8);fixture(0,bytes(raw));compare(2,0,struct.pack('<I2f',1,1.,1.),'duplicate registration');compare(3,0,words(1),'only first duplicate relocates');compare(2,0,struct.pack('<I2f',0,1.,1.),'only first remaining duplicate removed')
 # Enable with no cached floor queries geometry; misses preserve old parameters.
 for flying in (0,1):
  for point in (points[2][0],[1e9,1e9,1e9]):
   raw=struct.pack('<4I6fQ3fI',2,8|flying,0xffffffff,0xffffffff,*point,13.,-17.,19.,29,36.,7.,9.,0);fixture(1,raw);compare(2,1,struct.pack('<I2f',1,2.,36.),f'uncached enable flying {flying} hit {point!=[1e9]*3}')
 for flags in (0,1,2,3,0xffffffff):
  raw=bytearray(objects(old)[2*64:3*64]);struct.pack_into('<I',raw,0,flags);fixture(2,bytes(raw))
  for op in (4,5):
   for value in (0,1):compare(op,2,words(value),f'capability {op} preserves mask {flags:x}, value {value}')
 # Cross original deque block boundaries, then exercise both erase directions.
 reset()
 for i in range(8):compare(1,i,struct.pack('<IQ4f',0,17+i,*points[0][0],36.))
 for n in range(72):
  i=n%8;raw=bytearray(objects(old)[i*64:(i+1)*64]);struct.pack_into('<I',raw,4,8);fixture(i,bytes(raw));compare(2,i,struct.pack('<I2f',1,1.,36.),f'deque block registration {n}' if n in (30,31,32,63,64,71) else None)
 for n in range(36):compare(3,(n*5)%8,words(1),f'deque first-match relocation {n}' if n in (0,31,35) else None)
 for floor in (0,1):
  for n in range(40):
   i=(n*3)%8;raw=bytearray(objects(old)[i*64:(i+1)*64]);struct.pack_into('<I',raw,4,12);struct.pack_into('<I',raw,12,floor);fixture(i,bytes(raw));compare(2,i,struct.pack('<I2f',0,1.,36.),f'deque removal floor {floor} index {n}' if n in (0,31,39) else None)
 # Independent source object/caller fixtures exercise stale caches and all flags.
 for n in range(a.cases):
  if n%64==0:reset()
  i=rng.randrange(COUNT);op=rng.choice((1,1,2,2,2,3,4,5,6));point=list(rng.choice(points[rng.randrange(8)]));point[2]+=rng.choice((0,36,72,1000,-1000))
  if n%19==0:point=[1e9]*3
  if op==1:parameter=struct.pack('<IQ4f',rng.randrange(2),rng.randrange(65536),*point,rng.choice((-100.,-0.,0.,.5,1.,36.,500.)))
  elif op==2:parameter=struct.pack('<I2f',rng.randrange(2),rng.choice((0.,.1,1.,2.,36.,1000.)),rng.choice((-10.,-0.,0.,36.,1000.)))
  elif op==3:parameter=words(rng.choice(tuple(range(8))+(0xffffffff,)))
  elif op in (4,5):parameter=words(rng.randrange(2))
  else:parameter=struct.pack('<4fI',*point,rng.choice((0.,1.,36.,100.,1000.)),rng.randrange(2))
  compare(op,i,parameter)
  if n%160==159:print(f'Object/registry requests {n+1}/{a.cases}',flush=True)
 # InitObject preserves cached room/floor and normals on a query miss.
 reset()
 compare(1,0,struct.pack('<IQ4f',0,3,*points[0][0],36.))
 for flying in (0,1):compare(1,0,struct.pack('<IQ4f',flying,3,1e9,1e9,1e9,-1.),f'InitObject miss preserves cache flying {flying}')
 geometry_raw=words(len(triangles))+b''.join(words(len(rows))+b''.join(struct.pack('<9f',*(x for point in row for x in point)) for row in rows)+struct.pack('<6f',*bounds[i]) for i,rows in enumerate(triangles))
 reference=words(0x314a424f,len(records),COUNT,len(nodes)//56,len(edges)//20,len(floors)//48)+b''.join(struct.pack('<Q',key) for key in old.keys)+nodes+edges+floors+geometry_raw+b''.join(records);a.reference_output.write_bytes(reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'linked_reference_sha256':hashlib.sha256(a.linked_reference.read_bytes()).hexdigest(),'floor_source_sha256':hashlib.sha256(a.floor.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':len(records),'mismatches':0,'totals':totals,'behavior_snapshots':snapshots,'crypt_objects_probe':probe_counts,'full_registry_and_all_object_fields_compared':True,'original_map_and_deque_instructions_execute':True,'parent_service_attached_to_position_validation':True,'original_world_height_limit':100.,'original_import_calls':old.c.import_calls,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)}
 a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='behavior_snapshots'}),flush=True)
if __name__=='__main__':main()
