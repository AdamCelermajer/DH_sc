"""Original line/selector-ray/floor collision instructions versus ARM64.

The triangle selector supplies an ordered, already-transformed triangle list;
its BVH selection/transform and scene-service pointers are caller fixtures.
Original normalization, same-side tests, plane intersection, all collision
manager candidate/nearest-hit branches and PFFloor bounds/vertical ray execute.
"""
import argparse,hashlib,json,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from navigation_differential import Cpu as FloatCpu,equal
ROOT=Path(__file__).resolve().parents[1]

class Cpu(FloatCpu):
 def external(self,uc,address,size,unused):
  if not self.arm64 and address in (self.callback+112,self.callback+128,self.callback+144):
   if address==self.callback+112:self.put(0,self.selector)
   elif address==self.callback+128:self.put(0,len(self.triangles)//36)
   else:
    assert self.reg(2)>=len(self.triangles)//36
    uc.mem_write(self.reg(1),self.triangles);uc.mem_write(self.reg(3),struct.pack('<I',len(self.triangles)//36));self.put(0,0)
   self.import_calls['selector fixture']=self.import_calls.get('selector fixture',0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)

class OriginalFloorOracle:
 def __init__(self,engine,manifest,cpu_class=Cpu):
  self.cpu=c=cpu_class(engine,False,manifest);self.floor=c.data+0x1000;self.manager=c.data+0x2000;self.node=c.data+0x3000;c.selector=c.data+0x4000;vt=c.data+0x5000;self.point=c.data+0x6000;self.out=c.data+0x7000;self.out_triangle=c.data+0x8000;self.ray=c.data+0x9000;storage=c.data+0x10000
  c.pointer(self.floor+0x40,self.node);c.pointer(self.node,vt);c.pointer(vt+0xb0,c.callback+112)
  c.pointer(c.selector,vt+0x100);c.pointer(vt+0x10c,c.callback+128);c.pointer(vt+0x114,c.callback+144)
  c.pointer(self.manager,vt+0x200);c.pointer(vt+0x20c,0x6c62e8);c.uc.mem_write(self.manager+0x10,struct.pack('<3I',storage,storage+4096*36,storage+4096*36))
  def word(at):return struct.unpack('<I',c.uc.mem_read(at,4))[0]
  got=(0x51b9a4+word(0x51bad4))&0xffffffff;services=c.data+0x90000;scene=services+0x100;c.pointer(word(got+word(0x51bad8))+0x10,services);c.pointer(services+0x1c,scene);c.pointer(scene+0x2c,self.manager)
  self.selected=-1;self.calls=0
  def observe(uc,address,size,unused):
   if address==0x6c69a0:self.selected=(c.reg(6)-storage)//36
  c.uc.hook_add(UC_HOOK_CODE,observe,begin=0x6c69a0,end=0x6c69a0)
 def prepare(self,triangles,bounds=None):
  self.cpu.triangles=triangles
  if bounds:self.cpu.uc.mem_write(self.floor+0x44,bounds)
 def execute(self,mode,input_bytes):
  c=self.cpu;self.selected=-1;c.uc.mem_write(self.out,b'\xcc'*12);c.uc.mem_write(self.out_triangle,b'\xcc'*36)
  if mode=='ray':c.uc.mem_write(self.ray,input_bytes);hit=c.invoke(0x6c62e8,[self.manager,self.ray,c.selector,self.out,self.out_triangle],budget=5000000)
  else:c.uc.mem_write(self.point,input_bytes);hit=c.invoke(0x51b96c,[self.floor,self.point,self.out,self.out_triangle],budget=5000000)
  self.calls+=1;return struct.pack('<II',hit,self.selected if hit else 0xffffffff)+bytes(c.uc.mem_read(self.out,12))+bytes(c.uc.mem_read(self.out_triangle,36))

def packed_triangle(points):return struct.pack('<9f',*(x for p in points for x in p))
def floor_bounds(triangles):
 points=[p for t in triangles for p in t];low=[min(p[i] for p in points) for i in range(3)];high=[max(p[i] for p in points) for i in range(3)];low[2]-=1000;high[2]+=1000;return struct.pack('<6f',*low,*high)

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--floor',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);args=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/collision/original-functions.json').read_text());assert hashlib.sha256(args.engine.read_bytes()).hexdigest()==manifest['original_sha256'];oracle=OriginalFloorOracle(args.engine,manifest);old=oracle.cpu;new=Cpu(args.library,True,{'functions':[]});rng=random.Random(20261008);floor_data=json.loads(args.floor.read_text(encoding='utf-8-sig'))
 nt=new.data+0x1000;ni=new.data+0x50000;no=new.data+0x60000;nf=new.data+0x70000;records=[];counts={'line':0,'ray':0,'floor':0};hits={key:0 for key in counts};outside_plane=0
 def line_case(t,origin,direction,label):
  nonlocal outside_plane
  packed=packed_triangle(t)+struct.pack('<6f',*origin,*direction);old.uc.mem_write(oracle.point,packed);old.uc.mem_write(oracle.out,b'\xcc'*12);expected_hit=old.invoke(0x58615c,[oracle.point,oracle.point+36,oracle.point+48,oracle.out]);expected=bytes(old.uc.mem_read(oracle.out,12))
  new.uc.mem_write(nt,packed);new.uc.mem_write(no,b'\xcc'*12);actual_hit=new.invoke('dh2_collision_line',[no,nt,nt+36,nt+48]);actual=bytes(new.uc.mem_read(no,12));assert expected_hit==actual_hit and equal(expected,actual),(label,expected_hit,actual_hit,expected.hex(),actual.hex())
  outside_plane+=not expected_hit and expected!=b'\xcc'*12;counts['line']+=1;hits['line']+=bool(actual_hit);records.append(struct.pack('<II',0,len(packed))+packed+struct.pack('<I',expected_hit)+expected)
 def query_case(mode,triangles,input_data,bounds=b'',label=None):
  raw=b''.join(packed_triangle(t) for t in triangles);oracle.prepare(raw,bounds);expected=oracle.execute(mode,input_data)
  new.uc.mem_write(nt,raw);new.uc.mem_write(ni,input_data);new.uc.mem_write(no,b'\xcc'*56)
  if mode=='ray':actual_hit=new.invoke('dh2_collision_raycast',[no,nt,len(triangles),ni])
  else:new.uc.mem_write(nf,struct.pack('<QII',nt,len(triangles),0)+bounds);actual_hit=new.invoke('dh2_collision_floor',[no,nf,ni])
  actual=bytes(new.uc.mem_read(no,56));assert actual_hit==struct.unpack_from('<I',expected)[0] and equal(expected,actual),(mode,label,expected.hex(),actual.hex())
  counts[mode]+=1;hits[mode]+=bool(actual_hit);packed=struct.pack('<I',len(triangles))+raw+input_data+bounds;records.append(struct.pack('<II',1 if mode=='ray' else 2,len(packed))+packed+expected)
 # Plane/edge/vertex thresholds, signed zero and degenerate triangle coverage.
 for i in range(2400):
  a=[rng.uniform(-3000,3000) for _ in range(3)];b=[x+rng.uniform(-800,800) for x in a];c=[x+rng.uniform(-800,800) for x in a];t=[a,b,c];origin=[sum(x)/3 for x in zip(*t)];direction=[rng.uniform(-1,1) for _ in range(3)]
  if i%6==0:origin=list(a)
  if i%6==1:origin=[(x+y)*.5 for x,y in zip(a,b)]
  if i%6==2:origin=[x+1500 for x in origin]
  if i%6==3:direction=[0.,0.,rng.choice((0.,1.,-1.,.000001,.0000009))]
  if i>=2200:t=[[0.,0.,0.],[1.,0.,0.],[0.,1.,0.]];origin=[rng.choice((0.,.5,1.,-0.,1.0000001)),rng.choice((0.,.5,1.,-0.)),rng.choice((0.,1.,-1.))];direction=[0.,0.,rng.choice((0.,1.,.000001,.0000009,.0000011))]
  if i%47==0:t=[a,a,a]
  line_case(t,origin,direction,('line',i))
 # Random ordered candidates, stacked geometry, endpoint exclusion and large
 # triangles where the original nearest-vertex shortcut affects the result.
 for i in range(1400):
  triangles=[]
  for j in range(rng.randrange(1,9)):
   z=rng.uniform(-1000,1000);x,y=rng.uniform(-500,500),rng.uniform(-500,500);size=rng.uniform(50,2500);triangles.append([[x-size,y-size,z],[x+size,y-size,z+float(j%3)],[x,y+size,z]])
  if i%5==0:triangles.append(triangles[0])
  start=[rng.uniform(-600,600),rng.uniform(-600,600),rng.uniform(-2000,2000)];end=[*start[:2],rng.uniform(-2000,2000)]
  if i%4==0:end=[rng.uniform(-600,600) for _ in range(3)]
  if i%13==0:start=list(triangles[0][0])
  if i%17==0:end=list(start)
  query_case('ray',triangles,struct.pack('<6f',*start,*end),label=i)
 # Authored floor triangle order and room boundaries. Selector input snapshots
 # are intentionally explicit; this is not yet the original BVH producer.
 rooms={}
 for row in floor_data['floor']:rooms.setdefault(row['room'],[]).append(row['corners'])
 for room,triangles in rooms.items():
  bounds=floor_bounds(triangles)
  for i,t in enumerate(triangles):
   for position in (t[0],[(a+b)*.5 for a,b in zip(t[0],t[1])],[sum(v)/3 for v in zip(*t)]):
    query_case('floor',triangles,struct.pack('<3f',*position),bounds,(room,i))
  low=struct.unpack_from('<3f',bounds);high=struct.unpack_from('<3f',bounds,12)
  for i in range(40):
   point=[rng.uniform(a-100,b+100) for a,b in zip(low,high)];query_case('floor',triangles,struct.pack('<3f',*point),bounds,(room,'bounds',i))
 # IEEE nonfinite inputs are raw geometry values, not parser acceptance policy.
 for value in (float('nan'),float('inf'),-float('inf')):
  for axis in range(3):
   t=[[0.,0.,0.],[1.,0.,0.],[0.,1.,0.]];o=[.25,.25,1.];o[axis]=value;line_case(t,o,[0.,0.,-1.],('nonfinite',value,axis))
 before=bytes(new.uc.mem_read(no,56));assert new.invoke('dh2_collision_raycast',[no,0,1,ni])&0xffffffff==0xffffffff;assert bytes(new.uc.mem_read(no,56))==before
 reference=struct.pack('<II',0x314c4f43,len(records))+b''.join(records);args.reference_output.write_bytes(reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(args.library.read_bytes()).hexdigest(),'floor_source_sha256':hashlib.sha256(args.floor.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':sum(counts.values()),'cases':counts,'hits':hits,'plane_hit_outside_triangle_cases':outside_plane,'mismatches':0,'atomic_rejection_checks':1,'original_import_calls':old.import_calls,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)};args.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
