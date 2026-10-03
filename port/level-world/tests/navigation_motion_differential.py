"""Original floor height/normal and position/direction validation versus ARM64.

Original floor/room/world height traversal, ValidatePosition, ValidateDirection,
finite-segment intersection, normalization and angle instructions execute.
Original selector/octree/collision run in independent floor-query oracles.
IEEE/libm imports are modeled; obstacle-parent service is observed, not supplied
as an implemented backend. Scene ownership, object fields and policy are caller
fixtures. Character controllers, speed/root motion and dynamic obstacles are
outside this audit.
"""
import argparse,hashlib,json,math,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_S0
from navigation_world_differential import OriginalWorld,NativeWorld,linked_state
from navigation_search_differential import SearchCpu,word,words
from navigation_differential import ROOT,equal
from aggro_differential import float_bits
from combat_result_differential import floating

def acos(value):return math.acos(value) if -1<=value<=1 else math.nan
class MotionCpu(SearchCpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='acosf':
   raw=uc.reg_read(UC_ARM64_REG_S0) if self.arm64 else self.reg(0);result=float_bits(acos(floating(raw)))
   if self.arm64:uc.reg_write(UC_ARM64_REG_S0,result)
   else:self.put(0,result)
   self.import_calls['acosf']=self.import_calls.get('acosf',0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)

class OriginalMotion(OriginalWorld):
 def hook(self,uc,address,size,unused):
  if getattr(self,'active',False):
   if address==0x30e3dc:self.c.put(0,float_bits(acos(floating(self.c.reg(0)))));self.c.import_calls['acosf']=self.c.import_calls.get('acosf',0)+1;uc.reg_write(self.c.pc,uc.reg_read(self.c.lr));return
   if address==0x528484:self.parent.append((self.fp.index(word(self.c,self.obj+16)) if word(self.c,self.obj+16) else 0xffffffff,self.fp.index(self.c.reg(2))));self.ret();return
   if address==0x525dac and self.c.reg(0):self.kind=1
   elif address==0x525e88:self.kind=2
   elif address==0x525eec:self.kind=3
   elif address==0x525f5c and self.c.reg(0):self.kind=2
   elif address==0x525b64:self.slides+=1
   elif address==0x525d18:self.successful_slides+=1
  super().hook(uc,address,size,unused)
 def object_snapshot(self):
  c=self.c;rp=word(c,self.obj+12);fp=word(c,self.obj+16);return words(word(c,self.obj+20),word(c,self.obj+4),self.room.index(rp) if rp else 0xffffffff,self.fp.index(fp) if fp else 0xffffffff)+bytes(c.uc.mem_read(self.obj+24,24))
 def position(self,point,object_raw,policy,actor):
  c=self.c;d=c.data+0x31000;c.uc.mem_write(d,point);c.uc.mem_write(self.world+144,policy);flags,objflags,room,floor=struct.unpack_from('<4I',object_raw);c.pointer(self.obj+20,flags);c.pointer(self.obj+4,objflags);c.pointer(self.obj+12,self.room[room] if room!=0xffffffff else 0);c.pointer(self.obj+16,self.fp[floor] if floor!=0xffffffff else 0);c.uc.mem_write(self.obj+24,object_raw[16:]);self.kind=0;self.parent=[];self.queries=[]
  valid=c.invoke(0x525d84,[self.world,d,self.obj if actor else 0],budget=20000000);state=self.object_snapshot();new_floor=struct.unpack_from('<I',state,12)[0];old_floor=floor if actor else 0xffffffff
  result=words(valid,self.kind,len(self.parent),int(bool(self.parent and objflags&4 and old_floor!=new_floor)),old_floor,new_floor if actor else 0xffffffff)+bytes(c.uc.mem_read(d,12))+(state if actor else object_raw)
  return result,tuple(self.queries)
 def direction(self,direction,position,flags,radius):
  c=self.c;d=c.data+0x31000;c.uc.mem_write(d,direction+position);self.queries=[];self.slides=self.successful_slides=0
  valid=c.invoke(0x525944,[self.world,d,d+12,float_bits(radius),flags],budget=20000000);return words(valid)+bytes(c.uc.mem_read(d,12)),tuple(self.queries)
 def height(self,kind,index,point,special):
  c=self.c;d=c.data+0x31000;c.uc.mem_write(d,point+struct.pack('<4f',13.,-17.,19.,-23.)+words(0,0));self.queries=[]
  if kind==0:valid=c.invoke(0x51badc,[self.fp[index],d,d+12,d+16]);room=floor=0xffffffff
  elif kind==1:
   valid=c.invoke(0x520f98,[self.room[index],d,d+12,d+16,d+32,special]);room=index if valid else 0xffffffff;fp=word(c,d+32);floor=self.fp.index(fp) if valid else 0xffffffff
  else:
   valid=c.invoke(0x525508,[self.world,d,d+12,d+16,d+28,d+32,special]);rp=word(c,d+28);fp=word(c,d+32);room=self.room.index(rp) if valid else 0xffffffff;floor=self.fp.index(fp) if valid else 0xffffffff
  return bytes(c.uc.mem_read(d+12,16))+words(room,floor,valid,0),tuple(self.queries)

class NativeMotion(NativeWorld):
 def __init__(self,library,geometry):
  self.c=MotionCpu(library,True,{'functions':[]});self.geometry=geometry;self.queries=[];self.floor_ptrs={};c=self.c;c.uc.hook_add(UC_HOOK_CODE,self.hook,begin=c.symbols['dh2_selector_floor'],end=c.symbols['dh2_selector_floor']);self.make_geometry();self.input=c.data+0x707000;self.object=c.data+0x707100;self.policy=c.data+0x707200;self.out=c.data+0x707300;self.dr=c.data+0x707400
 def position(self,point,object_raw,policy,actor):
  c=self.c;c.uc.mem_write(self.input,point);c.uc.mem_write(self.object,object_raw);c.uc.mem_write(self.policy,policy);self.queries=[]
  assert c.invoke('dh2_nav_validate_position',[self.out,self.cw,self.object if actor else 0,self.input,self.policy],budget=20000000)==0
  return bytes(c.uc.mem_read(self.out,24))+bytes(c.uc.mem_read(self.input,12))+bytes(c.uc.mem_read(self.object,40)),tuple(self.queries)
 def direction(self,direction,position,flags,radius):
  c=self.c;c.uc.mem_write(self.input,direction+position);c.uc.mem_write(self.dr,struct.pack('<2Qf3I',self.cw,self.input+12,radius,flags,0,0));self.queries=[]
  assert c.invoke('dh2_nav_validate_direction',[self.out,self.input,self.dr],budget=20000000)==0
  return bytes(c.uc.mem_read(self.out,4))+bytes(c.uc.mem_read(self.input,12)),tuple(self.queries)
 def height(self,kind,index,point,special):
  c=self.c;c.uc.mem_write(self.input,point);c.uc.mem_write(self.out,struct.pack('<4f4I',13.,-17.,19.,-23.,0,0,0,0));self.queries=[]
  if kind==0:args=[self.out,self.selectors+index*56,self.input]
  elif kind==1:args=[self.out,self.cw,index,self.input,special]
  else:args=[self.out,self.cw,self.input,special]
  assert c.invoke(('dh2_nav_floor_height','dh2_nav_room_height','dh2_nav_world_height')[kind],args)>=0
  return bytes(c.uc.mem_read(self.out,32)),tuple(self.queries)

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--floor',type=Path,required=True);p.add_argument('--linked-reference',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--cases',type=int,default=480);a=p.parse_args();started=time.monotonic();manifest=json.loads((ROOT/'reference/navigation-motion/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];data=json.loads(a.floor.read_text(encoding='utf-8-sig'));triangles=[[] for _ in range(8)]
 for row in data['floor']:
  v=struct.unpack('<9f',struct.pack('<9f',*(x for p in row['corners'] for x in p)));triangles[row['room']].append([list(v[i:i+3]) for i in range(0,9,3)])
 bounds=[data['floor_bounds'][str(i)] for i in range(8)];geometry={'triangles':triangles,'bounds':bounds,'world_bounds':[min(b[k] for b in bounds) for k in range(3)]+[max(b[k+3] for b in bounds) for k in range(3)]};nodes,edges,floors=linked_state(a.linked_reference);old=OriginalMotion(a.engine,manifest,geometry);new=NativeMotion(a.library,geometry);traits=[(0,1)]*8;rng=random.Random(20261012);records=[];totals={'height':0,'position':0,'direction':0,'segment':0,'floor_queries':0,'accepted':0,'clamped':0,'equivalent':0,'position_miss':0,'direction_valid':0,'direction_rejected':0,'slide_attempts':0,'successful_slides':0,'parent_requests':0,'parent_changes':0};reset_pending=True
 def reset(values=traits):
  nonlocal reset_pending
  old.prepare(nodes,edges,floors,values);new.prepare(nodes,edges,floors,values);reset_pending=True
 def compare(op,inputs,args,label=None):
  nonlocal reset_pending
  expected=getattr(old,('height','position','direction')[op])(*args);actual=getattr(new,('height','position','direction')[op])(*args)
  assert equal(expected[0],actual[0]) and expected[1]==actual[1],(len(records),op,label,expected,actual)
  totals[('height','position','direction')[op]]+=1;totals['floor_queries']+=len(expected[1])
  if op==1:
   valid,kind,parent,change,_,_=struct.unpack_from('<6I',expected[0]);totals[('position_miss','equivalent','accepted','clamped')[kind]]+=1;totals['parent_requests']+=parent;totals['parent_changes']+=change
  elif op==2:valid=struct.unpack_from('<I',expected[0])[0];totals['direction_valid' if valid else 'direction_rejected']+=1;totals['slide_attempts']+=old.slides;totals['successful_slides']+=old.successful_slides
  records.append(words(op,len(inputs),len(expected[0]),len(expected[1]))+inputs+expected[0]+b''.join(words(i)+point for i,point in expected[1]));reset_pending=False;return expected[0]
 def traits_raw():return bytes(new.c.uc.mem_read(new.traits,64))
 def center(tri):return [sum(x)/3 for x in zip(*tri)]
 reset();points=[[center(t) for t in rows] for rows in triangles];probes=[];probe_counts={'floor_pairs':0,'position_valid':0,'accepted':0,'direction_valid':0}
 for source_floor in range(8):
  for target_floor in range(8):
   source_raw=struct.pack('<3f',*points[source_floor][0]);source=struct.unpack('<3f',source_raw);target_raw=struct.pack('<3f',*points[target_floor][-1]);target=struct.unpack('<3f',target_raw);point=struct.pack('<3f',target[0],target[1],target[2]+36.);obj=words(0,8,source_floor,source_floor)+source_raw+bytes(12);policy=struct.pack('<fI',1000.,0)
   position_result=compare(1,traits_raw()+point+obj+policy+words(1),(point,obj,policy,1));direction=struct.pack('<3f',*(target[k]-source[k] for k in range(3)));direction_result=compare(2,traits_raw()+direction+source_raw+words(0)+struct.pack('<f',36.),(direction,source_raw,0,36.));probes.append(words(source_floor,target_floor)+position_result+direction_result);probe_counts['floor_pairs']+=1;probe_counts['position_valid']+=struct.unpack_from('<I',position_result)[0];probe_counts['accepted']+=struct.unpack_from('<I',position_result,4)[0]==2;probe_counts['direction_valid']+=struct.unpack_from('<I',direction_result)[0]
 for i in range(a.cases):
  floor=rng.randrange(8);target_floor=rng.randrange(8);source=points[floor][rng.randrange(len(points[floor]))];point=list(points[target_floor][rng.randrange(len(points[target_floor]))]);point[2]+=rng.choice((0,0,1,36,72,100,1000,-1000))
  if i%17==0:point=[1e9]*3
  elif i%11==0:point=list(source);point[0]+=rng.choice((0,1e-5,1e-4))
  raw=struct.pack('<3f',*point);actor=i%7!=0;obj=words(rng.choice((0,2,0xffffffff)),rng.choice((0,4,8,12)),rng.choice((floor,target_floor,0xffffffff)),rng.choice((floor,target_floor,0xffffffff)))+struct.pack('<6f',*source,13.,-17.,19.);policy=struct.pack('<fI',rng.choice((0.,1.,36.,72.,100.,1000.,1e10)),i%13==0)
  compare(1,traits_raw()+raw+obj+policy+words(actor),(raw,obj,policy,actor))
  kind=i%3;special=i%2;compare(0,traits_raw()+words(kind,target_floor,special)+raw,(kind,target_floor,raw,special))
  t=rng.choice(triangles[floor]);position=center(t) if i%3==0 else [t[0][k]*.499+t[1][k]*.499+t[2][k]*.002 for k in range(3)];direction=(0.,0.,0.) if i%41==0 else tuple(rng.uniform(-1000,1000) for _ in range(3)) if i%5==0 else (rng.uniform(-1000,1000),rng.uniform(-1000,1000),0.)
  dr=struct.pack('<3f',*direction);pos=struct.pack('<3f',*position);flags=rng.choice((0,2,0xffffffff));radius=rng.choice((0.,36.,1000.));compare(2,traits_raw()+dr+pos+words(flags)+struct.pack('<f',radius),(dr,pos,flags,radius))
  if i%80==79:print(f'Motion requests {i+1}/{a.cases}',flush=True)
 # Special-inclusive queries and actor cached-floor bypass, plus strict
 # capability/height/equality boundaries supplied independently of geometry.
 for floor in range(8):
  varied=list(traits);varied[floor]=(0x03000000,1);reset(varied);raw=struct.pack('<3f',*points[floor][0])
  for kind in (0,1,2):
   for special in (0,1):compare(0,traits_raw()+words(kind,floor,special)+raw,(kind,floor,raw,special))
  obj=words(0xffffffff,4,floor,floor)+struct.pack('<6f',*(points[floor][-1]),13.,-17.,19.);policy=struct.pack('<fI',1e10,1);compare(1,traits_raw()+raw+obj+policy+words(1),(raw,obj,policy,1))
 reset()
 # Explicit source misses, capability rejection and zero-direction math.
 for floor in range(8):
  pos=struct.pack('<3f',*points[floor][0]);raw=struct.pack('<3f',1.,0.,0.)
  for source in (struct.pack('<3f',1e9,1e9,1e9),struct.pack('<3f',points[floor][0][0],points[floor][0][1],1e9),pos):
   compare(2,traits_raw()+raw+source+words(0)+struct.pack('<f',0.),(raw,source,0,0.))
  varied=list(traits);varied[floor]=(2,1);reset(varied);compare(2,traits_raw()+raw+pos+words(0)+struct.pack('<f',36.),(raw,pos,0,36.));reset()
 for floor in range(8):
  t=triangles[floor][0]
  for corner in range(3):
   pos=struct.pack('<3f',*(t[corner][k]*.998+t[(corner+1)%3][k]*.001+t[(corner+2)%3][k]*.001 for k in range(3)))
   for direction in ((1000,0,0),(-1000,0,0),(0,1000,0),(0,-1000,0),(1000,1000,0),(1000,-1000,0),(-1000,1000,0),(-1000,-1000,0)):
    raw=struct.pack('<3f',*direction);compare(2,traits_raw()+raw+pos+words(0)+struct.pack('<f',36.),(raw,pos,0,36.))
 # Strict height/equality comparisons near boundaries, keeping room/floor
 # caches valid and the source distinct from the requested point.
 for delta in (-72.,-36.,-1.,0.,1.,36.,72.):
  source=list(points[0][0]);source[0]+=1.;obj=words(0,8,0,0)+struct.pack('<6f',*source,13.,-17.,19.);point=list(points[0][0]);point[2]+=delta;raw=struct.pack('<3f',*point)
  for threshold in (max(0.,abs(delta)-1e-4),abs(delta),abs(delta)+1e-4):
   policy=struct.pack('<fI',threshold,0);compare(1,traits_raw()+raw+obj+policy+words(1),(raw,obj,policy,1))
 # The separate original line2d routine must preserve output on misses and
 # retain inclusive endpoints. It is not Point2D's portal classification.
 pairs=[(0.,0.,10.,0.,5.,-2.,5.,2.),(0.,0.,10.,0.,20.,-2.,20.,2.),(0.,0.,10.,0.,2.,0.,8.,0.),(0.,0.,0.,0.,0.,0.,0.,0.)]
 pairs += [tuple(rng.uniform(-10000,10000) for _ in range(8)) for i in range(a.cases)]
 for row in pairs:
  raw=struct.pack('<8f',*row);c=old.c;d=c.data+0x40000;c.uc.mem_write(d,raw+struct.pack('<2f',13.,-17.));valid=c.invoke(0x525310,[d,d+16,d+32]);expected=words(valid)+bytes(c.uc.mem_read(d+32,8));c=new.c;d=c.data+0x50000;c.uc.mem_write(d,raw+struct.pack('<2f',13.,-17.));valid=c.invoke('dh2_nav_segment_intersect',[d+32,d,d+16]);actual=words(valid)+bytes(c.uc.mem_read(d+32,8));assert equal(expected,actual),('segment',row,expected.hex(),actual.hex());records.append(words(3,len(raw),len(expected),0)+raw+expected);totals['segment']+=1
 geometry_bytes=words(len(triangles))+b''.join(words(len(rows))+b''.join(struct.pack('<9f',*(x for p in row for x in p)) for row in rows)+struct.pack('<6f',*bounds[i]) for i,rows in enumerate(triangles));reference=words(0x31544f4d,len(records),len(nodes)//56,len(edges)//20,len(floors)//48)+nodes+edges+floors+geometry_bytes+b''.join(records);a.reference_output.write_bytes(reference)
 probe_digest=0xcbf29ce484222325
 for byte in b''.join(probes):probe_digest=((probe_digest^byte)*0x100000001b3)&0xffffffffffffffff
 probe_counts['state_fnv1a64']=f'{probe_digest:016x}'
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'linked_reference_sha256':hashlib.sha256(a.linked_reference.read_bytes()).hexdigest(),'floor_source_sha256':hashlib.sha256(a.floor.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':len(records),'totals':totals,'crypt_motion_probe':probe_counts,'mismatches':0,'original_import_calls':old.c.import_calls,'native_import_calls':new.c.import_calls,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)};a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
