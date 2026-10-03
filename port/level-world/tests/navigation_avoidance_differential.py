"""Original obstacle force/avoidance and concrete physical collision filtering.

Force/avoidance, map/deque/vector instructions, canCollide and the concrete
PhysicalObject onCollisionTest virtual implementation execute. Scene membership,
GameObject/physical/shape fields and immutable first path destination are caller
fixtures. Imported IEEE/libm services are modeled. Original allocator instructions
execute with supplied heap storage. Speed/root motion and actor producers pending.
"""
import argparse,hashlib,json,math,random,struct,time
from pathlib import Path
from unicorn.arm64_const import UC_ARM64_REG_S0
from navigation_objects_differential import OriginalObjects,NativeObjects,COUNT,canonical_registry
from navigation_world_differential import linked_state
from navigation_search_differential import word,words
from navigation_motion_differential import acos
from navigation_differential import ROOT,equal
from aggro_differential import float_bits
from combat_result_differential import floating

class OriginalAvoidance(OriginalObjects):
 def __init__(self,engine,manifest,geometry):
  super().__init__(engine,manifest,geometry);self.games=[self.c.data+0x70000+i*0x400 for i in range(COUNT)];self.physics=[self.c.data+0x1a00000+i*0x100 for i in range(COUNT)];self.shapes=[self.c.data+0x1b00000+i*0x100 for i in range(COUNT)];self.owners=[self.c.data+0x1c00000+i*0x100 for i in range(COUNT)];self.phys_vtable=self.c.data+0x1d00000;self.c.pointer(self.phys_vtable+8,0x46e6bc);self.c.invoke(0x312e00,[]);self.records=b'';self.force=bytes(16)
 def hook(self,uc,address,size,unused):
  if getattr(self,'active',False):
   c=self.c
   if address==0x30e3dc:self.ret(float_bits(acos(floating(c.reg(0)))));return
   if address==0x527660:self.evaluated=1
   elif address==0x527d24:self.force=bytes(uc.mem_read(uc.reg_read(c.sp)+0x34,12))+words(c.reg(0))
   elif address==0x527dac:self.adjusted=1
   elif address==0x528028:self.turn_limited=1
   elif address==0x524c74:self.records=self.snapshot_buffer(c.reg(0))
  super().hook(uc,address,size,unused)
 def set_actor(self,i,raw):
  object_raw=bytearray(raw[:64]);user=struct.unpack_from('<Q',object_raw,40)[0];struct.pack_into('<Q',object_raw,40,self.games[i] if user else 0);self.set_object(i,bytes(object_raw));c=self.c;p=self.objects[i];c.uc.mem_write(p+64,raw[64:76]);has_path=struct.unpack_from('<I',raw,76)[0]
  edge=p+76;c.uc.mem_write(edge,words(0x96c810,0,0,float_bits(1.),0,0)+bytes(12)+raw[80:92]);self.edge_ptrs[i+1000]=edge;c.edge_ids[edge]=i+1000;self.list(p+56,[i+1000] if has_path else [])
  self.actor_raw[i]=raw
  physical=struct.unpack('<4Ih3Hh3H',raw[96:]);present,disabled,owner_present,owner_enabled=physical[:4];body=self.physics[i];game=self.games[i];c.uc.mem_write(game,bytes(0x400));c.pointer(game+0x2dc,body if present else 0);c.uc.mem_write(body,bytes(0x100));c.pointer(body,self.phys_vtable);c.pointer(body+8,self.owners[i] if owner_present else 0);c.uc.mem_write(self.owners[i]+0x80,bytes((owner_enabled,)));c.uc.mem_write(body+0x26,bytes((disabled,)))
  for j in range(2):
   group,category,mask,exists=physical[4+j*4:8+j*4];shape=self.shapes[i]+j*0x40;c.uc.mem_write(shape,bytes(0x40));c.uc.mem_write(shape+0x22,struct.pack('<HHh',category,mask,group));c.pointer(body+0x18+j*4,shape if exists else 0)
 def load_scene(self,actors,registry):
  self.actor_raw=[None]*COUNT
  for i in range(COUNT):self.set_actor(i,actors[i*128:(i+1)*128])
  head=self.world+44;self.c.uc.mem_write(head,words(0,0,head,head,0));fcount,ecount=struct.unpack_from('<2I',registry);floor_keys=struct.unpack_from('<'+'I'*fcount,registry,8) if fcount else ();at=8+fcount*4
  for floor in floor_keys:
   d=self.c.data+0x31000;self.c.pointer(d,self.fp[floor] if floor!=0xffffffff else 0);value=self.c.invoke(0x527560,[head,d])
   # Replay the real push_back slow path, including original block allocation.
   for j in range(ecount):
    f,reserved,key=struct.unpack_from('<IIQ',registry,at+j*16)
    if f!=floor:continue
    ptr=self.objects[self.keys.index(key)];cur=word(self.c,value+16);last=word(self.c,value+24)
    if cur==last-4:self.c.pointer(d,ptr);self.c.invoke(0x5280b0,[value,d])
    else:self.c.pointer(cur,ptr);self.c.pointer(value+16,cur+4)
  assert self.snapshot_registry()==registry
 def snapshot_buffer(self,p):
  c=self.c;start,end=word(c,p),word(c,p+4);records=[]
  for at in range(start,end,20):raw=bytes(c.uc.mem_read(at,20));key=self.keys[self.objects.index(struct.unpack_from('<I',raw,16)[0])];records.append(raw[:16]+struct.pack('<Q',key))
  return b''.join(records)
 def execute(self,op,i,target,direction,buffer,prefix):
  c=self.c;d=c.data+0x31000;p=self.objects[i];self.evaluated=self.adjusted=self.turn_limited=0;self.records=b'';self.force=bytes(16);c.uc.mem_write(d,direction+bytes(32));vector=d+0x40;storage=self.allocate(max(20,len(prefix)//24*20))
  original_prefix=b''.join(prefix[j:j+16]+words(self.objects[self.keys.index(struct.unpack_from('<Q',prefix,j+16)[0])]) for j in range(0,len(prefix),24))
  if original_prefix:c.uc.mem_write(storage,original_prefix)
  c.uc.mem_write(vector,words(storage,storage+len(original_prefix),storage+len(original_prefix)))
  if op==0:
   count=c.invoke(0x527660,[self.world,p,d+16,vector if buffer else 0],budget=20000000);result=bytes(c.uc.mem_read(d+16,12))+words(count);records=self.snapshot_buffer(vector) if buffer else b''
  elif op==1:
   assert not prefix;c.invoke(0x527cc4,[self.world,p,d],budget=20000000);result=self.force+words(self.evaluated,self.adjusted,self.turn_limited,0)+bytes(c.uc.mem_read(d,12));records=self.records if buffer else b''
  else:
   a_present=struct.unpack_from('<I',self.actor_raw[i],96)[0];b_present=struct.unpack_from('<I',self.actor_raw[target],96)[0];assert a_present
   result=words(c.invoke(0x46e768,[self.physics[i],self.physics[target] if b_present else 0]));records=b''
  return result,records,self.snapshot_registry()

class NativeAvoidance(NativeObjects):
 def __init__(self,library,geometry):
  super().__init__(library,geometry);self.actors=self.c.data+0x720000;self.scene=self.actors+0x1000;self.keys=self.actors+0x1100;self.force_buffer=self.actors+0x1200;self.force_records=self.actors+0x2000;self.objects=[self.actors+i*128 for i in range(COUNT)];self.c.uc.mem_write(self.keys,b''.join(struct.pack('<Q',0x100000001+i) for i in range(COUNT)))
  self.c.uc.mem_write(self.scene,struct.pack('<3Q2I',self.registry,self.actors,self.keys,COUNT,0))
 def load_scene(self,actors,registry):
  c=self.c;c.uc.mem_write(self.actors,actors);fcount,ecount=struct.unpack_from('<2I',registry);c.uc.mem_write(self.registry,struct.pack('<Q2IQ2I',self.entries,ecount,512,self.buckets,fcount,16))
  if fcount:c.uc.mem_write(self.buckets,registry[8:8+fcount*4])
  if ecount:c.uc.mem_write(self.entries,registry[8+fcount*4:])
 def execute(self,op,i,target,direction,buffer,prefix):
  c=self.c;c.uc.mem_write(self.input,direction);c.uc.mem_write(self.force_buffer,struct.pack('<Q2I',self.force_records,len(prefix)//24,512));c.uc.mem_write(self.request,struct.pack('<3Q',self.scene,0x100000001+i,self.force_buffer if buffer else 0))
  if prefix:c.uc.mem_write(self.force_records,prefix)
  if op==0:status=c.invoke('dh2_nav_obstacle_force',[self.out,self.request],budget=20000000);result=bytes(c.uc.mem_read(self.out,16))
  elif op==1:status=c.invoke('dh2_nav_avoid_obstacles',[self.out,self.input,self.request],budget=20000000);result=bytes(c.uc.mem_read(self.out,32))+bytes(c.uc.mem_read(self.input,12))
  else:status=0;result=words(c.invoke('dh2_nav_can_collide',[self.actors+i*128+96,self.actors+target*128+96]))
  assert status==0,(op,i,status);count=word(c,self.force_buffer+8);records=bytes(c.uc.mem_read(self.force_records,count*24)) if buffer and op<2 and count else b''
  return result,records,self.snapshot_registry()

def actor(position=(0.,0.,0.),target=(1000.,0.,0.),floor=0,flags=14,radius=36.,weight=1.,extent=36.,user=0,has_path=0,path_target=(1000.,0.,0.),physical=(1,0,0,1,0,1,65535,1,0,0,0,0)):
 return struct.pack('<4I6fQ3fI3fI3fI4Ih3Hh3H',2,flags,floor,floor,*position,0.,0.,1.,user,radius,weight,extent,0,*target,has_path,*path_target,0,*physical)

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--floor',type=Path,required=True);p.add_argument('--linked-reference',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--cases',type=int,default=960);a=p.parse_args();started=time.monotonic();manifest=json.loads((ROOT/'reference/navigation-avoidance/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];data=json.loads(a.floor.read_text(encoding='utf-8-sig'));triangles=[[] for _ in range(8)]
 for row in data['floor']:triangles[row['room']].append([list(struct.unpack('<3f',struct.pack('<3f',*point))) for point in row['corners']])
 bounds=[data['floor_bounds'][str(i)] for i in range(8)];geometry={'triangles':triangles,'bounds':bounds,'world_bounds':[min(b[k] for b in bounds) for k in range(3)]+[max(b[k+3] for b in bounds) for k in range(3)]};nodes,edges,floors=linked_state(a.linked_reference);old=OriginalAvoidance(a.engine,manifest,geometry);new=NativeAvoidance(a.library,geometry);old.reset(nodes,edges,floors,[(0,1)]*8);new.reset(nodes,edges,floors,[(0,1)]*8);rng=random.Random(20261014);records=[];snapshots=[];totals={n:0 for n in ('force','avoid','can_collide','force_records','contributions','adjusted','turn_limited','gated','registry_keys_compared')}
 def compare(op,actors,members=(),direction=(100.,0.,0.),i=0,target=1,buffer=1,prefix=b'',floor_keys=None,label=None):
  registry=canonical_registry([(f,[(0x100000001+j) for floor,j in members if floor==f]) for f in (floor_keys if floor_keys is not None else sorted({floor for floor,_ in members}))]);raw=b''.join(actors);assert len(raw)==COUNT*128
  old.load_scene(raw,registry);new.load_scene(raw,registry);direction_raw=struct.pack('<3f',*direction);expected=old.execute(op,i,target,direction_raw,buffer,prefix);actual=new.execute(op,i,target,direction_raw,buffer,prefix)
  for kind,(left,right) in enumerate(zip(expected,actual)):assert equal(left,right) if kind<2 else left==right,(len(records),op,label,kind,left.hex(),right.hex())
  totals[('force','avoid','can_collide')[op]]+=1;totals['force_records']+=len(expected[1])//24;totals['registry_keys_compared']+=struct.unpack_from('<I',expected[2])[0]
  if op<2:totals['contributions']+=struct.unpack_from('<I',expected[0],12)[0]
  if op==1:ev,adj,limit,_=struct.unpack_from('<4I',expected[0],16);totals['adjusted']+=adj;totals['turn_limited']+=limit;totals['gated']+=not ev
  records.append(words(op,i,target,buffer,len(prefix),len(registry),len(expected[0]),len(expected[1]),len(expected[2]))+direction_raw+raw+prefix+registry+b''.join(expected))
  if label:snapshots.append({'case':label,'operation':op,'output_words':list(struct.unpack('<'+'I'*(len(expected[0])//4),expected[0])),'records':len(expected[1])//24,'registry_keys':struct.unpack_from('<I',expected[2])[0]})
  return expected
 base=[actor(position=(0.,0.,0.))]+[actor(position=(30.+j*3.,10.+j*2.,100.*j)) for j in range(7)];members=[(0,j) for j in range(8)]
 probe_digest=0xcbf29ce484222325;probe={'floors':0,'force_contributions':0,'adjusted':0,'turn_limited':0}
 for floor in range(8):
  source=struct.unpack('<3f',struct.pack('<3f',*(sum(values)/3 for values in zip(*triangles[floor][0]))));actors=[];offsets=((0,0),(30,10),(35,-15),(120,0),(20,5),(25,10),(20,-10),(30,0));members=[]
  for i,(x,y) in enumerate(offsets):
   f=(floor+1)%8 if i==7 else floor;physical=(1,0,0,1,-3 if i in (0,4) else 0,1,65535,1,0,0,0,0)
   actors.append(actor(position=(source[0]+x,source[1]+y,source[2]),target=(source[0]+1000,source[1],source[2]),floor=f,flags=8 if i==5 else 4 if i==6 else 14,user=0x100000001+i,has_path=1,path_target=(source[0]+300,source[1],source[2]),physical=physical));members.append((f,i))
  force=compare(0,actors,members);avoid=compare(1,actors,members);probe['floors']+=1;probe['force_contributions']+=struct.unpack_from('<I',force[0],12)[0];probe['adjusted']+=struct.unpack_from('<I',avoid[0],20)[0];probe['turn_limited']+=struct.unpack_from('<I',avoid[0],24)[0]
  for byte in words(floor)+force[0]+force[1]+avoid[0]+avoid[1]+avoid[2]:probe_digest=((probe_digest^byte)*0x100000001b3)&0xffffffffffffffff
 probe['state_fnv1a64']=f'{probe_digest:016x}'
 members=[(0,j) for j in range(8)]
 for op in (0,1):compare(op,base,members,label='all neighbours and self registration')
 compare(0,base,members,buffer=0,label='force sum without diagnostic vector');compare(0,base,members,prefix=struct.pack('<4fQ',13.,-17.,19.,23.,0x100000008),label='diagnostic vector appends existing records')
 for flags,floor in ((14,0xffffffff),(15,0),(12,0),(0,0)):
  varied=list(base);varied[0]=actor(flags=flags,floor=floor);compare(1,varied,members,label=f'early flag/floor gate {flags} {floor}')
 for position,label in [((0,0,1000),'coincident XY ignores Z'),((72.99999,0,0),'inside influence boundary'),((73,0,0),'strict influence boundary'),((73.00001,0,0),'outside influence boundary')]:
  varied=list(base);varied[1]=actor(position=position);compare(0,varied,[(0,1)],label=label);compare(1,varied,[(0,1)],label=label+' avoidance')
 for has_path,path_x,target_x in ((0,0,40),(0,0,30),(0,0,29.999),(1,30,1000),(1,29.999,1000),(1,30.001,1000)):
  varied=list(base);varied[0]=actor(target=(target_x,0,0),has_path=has_path,path_target=(path_x,0,1000));varied[1]=actor(position=(30,0,1000));compare(0,varied,[(0,1)],label=f'path/desired endpoint threshold {has_path} {path_x} {target_x}')
 for direction in ((100,0,0),(-100,0,0),(0,100,0),(0,0,0),(100,0,100),(0,0,100)):
  compare(1,base,[(0,1)],direction=direction,label=f'single obstacle direction {direction}');compare(1,base,members,direction=direction,label=f'multiple obstacle direction {direction}')
 # Force output vector grows beyond its original inline allocation and source
 # deque crosses blocks. Duplicate members contribute in their exact order.
 compare(0,base,[(0,1+(n%7)) for n in range(72)],label='72 duplicate contributions across deque blocks');compare(1,base,[(0,1+(n%7)) for n in range(72)],label='72 duplicate steering contributions')
 # Physical gate priority: signed group equality precedes masks, primary shape
 # precedes secondary, and both owned objects must permit collision.
 for ga in (-32768,-3,0,2,32767):
  for gb in (-32768,-3,0,2,32767):
   for masks in ((1,1,1,1),(1,0,1,0),(1,2,2,1),(0,65535,0,65535)):
    ca,ma,cb,mb=masks;varied=list(base);varied[0]=actor(user=0x100000001,physical=(1,0,0,1,ga,ca,ma,1,2,65535,65535,1));varied[1]=actor(position=(30,0,0),user=0x100000002,physical=(1,0,0,1,gb,cb,mb,1,2,65535,65535,1));compare(2,varied,label=f'physical groups {ga} {gb} masks {masks}');compare(0,varied,[(0,1)])
 for present,disabled,owner,enabled,primary,secondary in ((0,0,0,1,1,1),(1,1,0,1,1,1),(1,0,1,0,1,1),(1,0,0,0,0,1),(1,0,0,0,0,0),(1,0,1,1,0,1)):
  varied=list(base);varied[0]=actor(user=0x100000001);varied[1]=actor(position=(30,0,0),user=0x100000002,physical=(present,disabled,owner,enabled,0,1,1,primary,0,1,1,secondary));compare(2,varied,label=f'physical body gates {present} {disabled} {owner} {enabled} {primary} {secondary}');compare(0,varied,[(0,1)])
 for dy in (-1e-3,-1e-4,-1e-5,-1e-6,0.,1e-6,1e-5,1e-4,1e-3):
  varied=list(base);varied[1]=actor(position=(30,dy,0));compare(1,varied,[(0,1)],label=f'steering sign epsilon y {dy}')
 for extent in (.0001,.01,.1,1.,10.,100.,1000.):
  varied=list(base);varied[1]=actor(position=(30,5,0),extent=extent);varied[2]=actor(position=(31,6,0),extent=extent);compare(1,varied,[(0,1),(0,2)],label=f'multiple contribution turn threshold {extent}')
 for n in range(a.cases):
  actors=[]
  for j in range(8):
   physical=(rng.randrange(2),rng.randrange(2),rng.randrange(2),rng.randrange(2),rng.choice((-3,0,2)),rng.randrange(16),rng.randrange(16),rng.randrange(2),rng.choice((-3,0,2)),rng.randrange(16),rng.randrange(16),rng.randrange(2))
   actors.append(actor(position=(rng.uniform(-100,100),rng.uniform(-100,100),rng.uniform(-1000,1000)),target=(rng.uniform(-150,150),rng.uniform(-150,150),rng.uniform(-1000,1000)),floor=rng.choice((0,1,0xffffffff)),flags=rng.randrange(16),radius=rng.choice((0.,1.,36.,100.)),weight=rng.choice((0.,1.,36.,-36.)),extent=rng.choice((-100.,-0.,0.,1.,36.,1000.)),user=(0x100000001+j) if rng.randrange(2) else 0,has_path=rng.randrange(2),path_target=(rng.uniform(-100,100),rng.uniform(-100,100),rng.uniform(-1000,1000)),physical=physical))
  op=n%3;i=rng.randrange(8);target=(i+1)%8
  if op==2:raw=bytearray(actors[i]);struct.pack_into('<I',raw,96,1);actors[i]=bytes(raw)
  members=[(rng.choice((0,1,0xffffffff)),rng.randrange(8)) for _ in range(rng.randrange(16))];direction=tuple(rng.uniform(-200,200) for _ in range(3));compare(op,actors,members,direction,i,target,0 if n%11==0 else 1)
  if n%240==239:print(f'Avoidance/filter requests {n+1}/{a.cases}',flush=True)
 # Additional positive steering/force cases separate from the many physical,
 # membership and actor-flag rejection combinations above.
 for n in range(384):
  position=tuple(rng.uniform(-2000,2000) for _ in range(3));actors=[];floor=n%8
  for j in range(8):
   offset=(0,0,0) if j==0 else (rng.uniform(-50,50),rng.uniform(-50,50),rng.uniform(-1000,1000));physical=(1,0,0,1,0,1,65535,1,0,0,0,0)
   actors.append(actor(position=tuple(position[k]+offset[k] for k in range(3)),target=(position[0]+1000,position[1]+1000,position[2]),floor=floor,radius=rng.choice((1.,36.,100.)),weight=rng.choice((-36.,0.,1.,36.)),extent=rng.choice((-1000.,.001,.1,1.,36.,1000.)),user=0x100000001+j,has_path=n%2,path_target=(position[0]+80,position[1]+80,position[2]+1000),physical=physical))
  members=[(floor,j) for j in range(8)];direction=tuple(rng.uniform(-1000,1000) for _ in range(3));compare(0,actors,members,direction=direction);compare(1,actors,members,direction=direction)
 geometry_raw=words(len(triangles))+b''.join(words(len(rows))+b''.join(struct.pack('<9f',*(x for point in row for x in point)) for row in rows)+struct.pack('<6f',*bounds[i]) for i,rows in enumerate(triangles))
 reference=words(0x31445641,len(records),COUNT,len(nodes)//56,len(edges)//20,len(floors)//48)+nodes+edges+floors+geometry_raw+b''.join(records);a.reference_output.write_bytes(reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'linked_reference_sha256':hashlib.sha256(a.linked_reference.read_bytes()).hexdigest(),'floor_source_sha256':hashlib.sha256(a.floor.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':len(records),'mismatches':0,'totals':totals,'crypt_avoidance_probe':probe,'behavior_snapshots':snapshots,'complete_ordered_forces_and_registry_compared':True,'original_physical_filter_and_concrete_virtual_execute':True,'original_import_calls':old.c.import_calls,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)}
 a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='behavior_snapshots'}),flush=True)
if __name__=='__main__':main()
