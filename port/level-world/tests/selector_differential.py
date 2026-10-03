"""Original transformed octree selector and coupled floor collision vs ARM64.

Original getTriangles, Setup matrix/bounds, multiplication, inversion,
transformBox, octree construction/selection, AddResult, collision manager and
floor/triangle instructions execute. Allocation, supplied geometry, node-world
matrix getter and scene-service pointers are caller services. Mesh decoding,
baked-geometry production and real floor identities/flags remain separate.
"""
import argparse,hashlib,json,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from collision_differential import Cpu as CollisionCpu,OriginalFloorOracle,packed_triangle,floor_bounds
from navigation_differential import equal
ROOT=Path(__file__).resolve().parents[1]
class Cpu(CollisionCpu):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name in ('_ZdlPv','_ZdaPv','free'):
   self.frees+=1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif not self.arm64 and address==self.callback+160:
   self.matrix_getters+=1;self.put(0,self.node_matrix);uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)

class OriginalSelectorOracle(OriginalFloorOracle):
 def __init__(self,engine,manifest,cpu_class=Cpu):
  super().__init__(engine,manifest,cpu_class);c=self.cpu;c.frees=0;c.matrix_getters=0;c.node_matrix=c.data+0x1b1000
  c.pointer(c.data+0x5114,0x587694);c.pointer(c.data+0x510c,0x590f20);c.pointer(c.data+0x5038,c.callback+160)
  self.root=c.data+0x100000;self.geometry=c.data+0x120000;self.heap=c.data+0x200000;self.allocations=0
  def hook(uc,address,size,unused):
   if address in (0x5341ac,0x310568):
    n=c.reg(0);assert 0<n<=0x100000;ptr=self.heap;self.heap+=(n+15)&~15;assert self.heap<c.data+0x1f00000;uc.mem_write(ptr,bytes(n));self.allocations+=1;c.put(0,ptr);uc.reg_write(c.pc,uc.reg_read(c.lr))
   elif address==0x310450:c.frees+=1;c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
  c.uc.hook_add(UC_HOOK_CODE,hook)
 def build(self,raw,leaf):
  c=self.cpu;self.heap=c.data+0x200000;super().prepare(raw);c.uc.mem_write(self.root,bytes(68));c.uc.mem_write(self.geometry,raw);vector=struct.pack('<3I',self.geometry,self.geometry+len(raw),self.geometry+len(raw));c.uc.mem_write(self.root,vector);c.uc.mem_write(c.selector+0xc,vector);c.uc.mem_write(c.selector+0xb0,struct.pack('<2I',0,leaf));c.pointer(c.selector+0xac,self.root);c.invoke(0x587e60,[c.selector,self.root],budget=20000000)
 def configure(self,node,baked):
  c=self.cpu;c.pointer(c.selector+8,self.node if node else 0);c.uc.mem_write(c.selector+0x18,bytes((baked,)))
  if node:c.uc.mem_write(c.node_matrix,node[:65])
 def triangles_for_box(self,box,extra):
  c=self.cpu;c.uc.mem_write(self.point,box);extra_ptr=0
  if extra:extra_ptr=c.data+0x1b2000;c.uc.mem_write(extra_ptr,extra[:65])
  c.uc.mem_write(self.out,b'\xcc'*4);c.invoke(0x587694,[c.selector,c.data+0x1c0000,len(c.triangles)//36,self.out,self.point,extra_ptr],budget=10000000)
  count=struct.unpack('<I',c.uc.mem_read(self.out,4))[0];assert count<=len(c.triangles)//36;return bytes(c.uc.mem_read(c.data+0x1c0000,count*36))

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--floor',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);a=p.parse_args();start=time.monotonic();manifest=json.loads((ROOT/'reference/selector/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 oracle=OriginalSelectorOracle(a.engine,manifest);old=oracle.cpu;new=Cpu(a.library,True,{'functions':[]});rng=random.Random(20261010);data=json.loads(a.floor.read_text(encoding='utf-8-sig'))
 nt=new.data+0x1000;geometry=new.data+0x20000;nodes=new.data+0x40000;ids=new.data+0x100000;scratch=new.data+0x140000;selector=new.data+0x150000;workspace=new.data+0x150100;selected_ids=new.data+0x160000;selected_tris=new.data+0x180000;box_new=new.data+0x1b0000;node_new=new.data+0x1b1000;extra_new=new.data+0x1b2000;floor_new=new.data+0x1b3000;result_new=new.data+0x1b4000;inverse_new=new.data+0x1b5000;input_new=new.data+0x1b6000
 def matrix(values,identity=0):return struct.pack('<16fI',*values,identity)
 identity=matrix([float(i%5==0) for i in range(16)],1)
 def transform_matrix(i):
  values=[0.]*16;values[15]=1
  if i%7==0:return identity
  for col in range(3):
   for row in range(3):values[4*col+row]=rng.uniform(-.4,.4)+(rng.choice((-1.,1.))*rng.uniform(.4,2.) if col==row else 0.)
  values[12:15]=[rng.uniform(-300,300) for _ in range(3)]
  if i%11==0:values[0:12]=[0.]*12 # singular makeInverse retains its input
  return matrix(values)
 inverse_records=[];singular=0
 matrices=[identity,matrix([float(i%5==0) for i in range(16)])]
 for i in range(1000):
  values=[rng.uniform(-10,10) for _ in range(16)]
  if i%3==0:values[3]=values[7]=values[11]=0.;values[15]=1.
  if i%13==0:values[0:4]=values[4:8]
  matrices.append(matrix(values))
 for epsilon in (0.,.0000009,.000001,.0000011,-.0000011):
  values=[float(i%5==0) for i in range(16)];values[0]=epsilon;matrices.append(matrix(values))
 for raw in matrices:
  old.uc.mem_write(oracle.point,raw[:65]);expected_status=old.invoke(0x5822f8,[oracle.point]);expected=bytes(old.uc.mem_read(oracle.point,65))+b'\0'*3
  new.uc.mem_write(inverse_new,raw);status=new.invoke('dh2_selector_inverse',[inverse_new]);actual=bytes(new.uc.mem_read(inverse_new,68));assert status==expected_status and equal(expected[:64],actual[:64]) and expected[64:]==actual[64:],('inverse',raw.hex(),expected.hex(),actual.hex());singular+=not status;inverse_records.append(raw+struct.pack('<I',status)+expected)
 rooms={}
 for row in data['floor']:rooms.setdefault(row['room'],[]).append(row['corners'])
 batches=[(ts,15) for ts in rooms.values()]
 for b in range(24):
  ts=[]
  for i in range(rng.randrange(16,65)):
   x,y,z=[rng.uniform(-1000,1000) for _ in range(3)];size=rng.uniform(5,300);ts.append([[x-size,y-size,z],[x+size,y-size,z],[x,y+size,z]])
  batches.append((ts,rng.choice((0,1,15,30))))
 batch_records=[];box_comparisons=0;ray_comparisons=0;floor_comparisons=0;selected=0;hits={'ray':0,'floor':0};node_modes={'absent':0,'dynamic':0,'baked':0};extra_count=0
 for bi,(ts,leaf) in enumerate(batches):
  raw=b''.join(packed_triangle(t) for t in ts);n=len(ts);oracle.build(raw,leaf);new.uc.mem_write(geometry,raw);new.uc.mem_write(nt,struct.pack('<4Q9I',nodes,ids,scratch,geometry,n,0,0,4096,32768,4096,leaf,0,0));assert new.invoke('dh2_octree_build',[nt,geometry,n,leaf],budget=20000000)==0
  new.uc.mem_write(workspace,struct.pack('<QQII',selected_ids,selected_tris,n,0));rows=[]
  points=[p for t in ts for p in t];low=[min(p[i] for p in points) for i in range(3)];high=[max(p[i] for p in points) for i in range(3)];bounds=floor_bounds(ts)
  for qi in range(96):
   mode=qi%3;baked=mode==2;node=None if mode==0 else transform_matrix(qi);extra=transform_matrix(qi+1) if qi%4==0 else None
   oracle.configure(node,baked);node_modes[('absent','dynamic','baked')[mode]]+=1;extra_count+=bool(extra)
   if node:new.uc.mem_write(node_new,node)
   if extra:new.uc.mem_write(extra_new,extra)
   selector_bytes=struct.pack('<QQII',nt,node_new if node else 0,int(baked),0);new.uc.mem_write(selector,selector_bytes)
   point=[rng.uniform(lo-50,hi+50) for lo,hi in zip(low,high)]
   if qi%4==0:point=[sum(v)/3 for v in zip(*ts[(qi//4)%n])]
   size=rng.choice((0.,1.,100.,10000.));box=struct.pack('<6f',*(x-size for x in point),*(x+size for x in point));expected=oracle.triangles_for_box(box,extra);new.uc.mem_write(box_new,box);count=new.invoke('dh2_selector_triangles',[workspace,selector,box_new,extra_new if extra else 0],budget=10000000);actual=bytes(new.uc.mem_read(selected_tris,count*36));assert equal(expected,actual),(bi,qi,'selector',len(expected)//36,count)
   rows.append(struct.pack('<I',0)+struct.pack('<II',int(node is not None),int(baked))+(node or identity)+struct.pack('<I',int(extra is not None))+(extra or identity)+box+struct.pack('<I',count)+expected);box_comparisons+=1;selected+=count
   # Full original collision manager dispatches its selector vtable directly to
   # getTriangles, not a fixture list. Matrix/inverse/tree/result code executes.
   ray=struct.pack('<6f',*point[:2],point[2]+1000,*point[:2],point[2]-1000)
   if qi%5==0:ray=struct.pack('<6f',*(x+rng.uniform(-1000,1000) for x in point),*(x+rng.uniform(-1000,1000) for x in point))
   new.uc.mem_write(input_new,ray);new.uc.mem_write(result_new,b'\xcc'*56);expected_result=oracle.execute('ray',ray);hit=new.invoke('dh2_selector_raycast',[result_new,selector,workspace,input_new],budget=10000000);actual_result=bytes(new.uc.mem_read(result_new,56));assert hit==struct.unpack_from('<I',expected_result)[0] and equal(expected_result,actual_result),(bi,qi,'coupled ray',expected_result.hex(),actual_result.hex())
   rows.append(struct.pack('<I',1)+struct.pack('<II',int(node is not None),int(baked))+(node or identity)+ray+expected_result);ray_comparisons+=1;hits['ray']+=hit
   # PFFloor checks supplied world bounds, then follows the same complete chain.
   oracle.prepare(raw,bounds);new.uc.mem_write(floor_new,selector_bytes+bounds+struct.pack('<Q',workspace));packed_point=struct.pack('<3f',*point);new.uc.mem_write(input_new,packed_point);new.uc.mem_write(result_new,b'\xcc'*56);expected_result=oracle.execute('floor',packed_point);hit=new.invoke('dh2_selector_floor',[result_new,floor_new,input_new],budget=10000000);actual_result=bytes(new.uc.mem_read(result_new,56));assert hit==struct.unpack_from('<I',expected_result)[0] and equal(expected_result,actual_result),(bi,qi,'coupled floor',expected_result.hex(),actual_result.hex())
   rows.append(struct.pack('<I',2)+struct.pack('<II',int(node is not None),int(baked))+(node or identity)+bounds+packed_point+expected_result);floor_comparisons+=1;hits['floor']+=hit
  batch_records.append(struct.pack('<II',n,leaf)+raw+struct.pack('<I',len(rows))+b''.join(rows))
 before=bytes(new.uc.mem_read(result_new,56));new.uc.mem_write(workspace+16,struct.pack('<I',n-1));assert new.invoke('dh2_selector_raycast',[result_new,selector,workspace,input_new])&0xffffffff==0xffffffff and bytes(new.uc.mem_read(result_new,56))==before
 reference=struct.pack('<III',0x314c4553,len(inverse_records),len(batch_records))+b''.join(inverse_records)+b''.join(batch_records);a.reference_output.write_bytes(reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'floor_source_sha256':hashlib.sha256(a.floor.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'matrix_inverse_comparisons':len(inverse_records),'singular_inverses':singular,'octree_builds':len(batches),'selector_box_comparisons':box_comparisons,'selected_triangle_comparisons':selected,'coupled_ray_comparisons':ray_comparisons,'coupled_floor_comparisons':floor_comparisons,'hits':hits,'node_modes':node_modes,'extra_matrix_cases':extra_count,'original_matrix_getters':old.matrix_getters,'original_allocations':oracle.allocations,'original_deallocations':old.frees,'mismatches':0,'atomic_rejection_checks':1,'original_import_calls':old.import_calls,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-start,2)};a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
