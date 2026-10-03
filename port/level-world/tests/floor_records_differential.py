"""Authored Crypt floor producers versus original instructions and packaged ARM64.

CopyMeshSceneNode's choreography, original position/getter/relative-matrix,
mesh constructor, bounds and final retained-geometry Z adjustment execute.
Deep resource copying, clone object allocation/construction and name ownership
are caller services. Full floor loading/metadata parsing is not executed.
"""
import argparse,hashlib,json,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from floor_source_differential import Cpu,OriginalMeshOracle
from navigation_differential import equal
ROOT=Path(__file__).resolve().parents[1]
class Reader:
 def __init__(self,path):self.raw=path.read_bytes();self.at=0
 def read(self,n):
  assert 0<=n<=len(self.raw)-self.at;result=self.raw[self.at:self.at+n];self.at+=n;return result
 def word(self):return struct.unpack('<I',self.read(4))[0]
class OriginalRecordOracle(OriginalMeshOracle):
 def __init__(self,engine,manifest):
  super().__init__(engine,manifest);c=self.cpu;c.uc.mem_write(self.mesh+4,struct.pack('<I',100000));self.parent=c.data+0x850000;self.parent_vt=self.parent+0x200;self.name=self.parent+0x400;self.clone_calls=0;self.deep_copy_services=0;self.clone_constructor_services=0
  c.pointer(self.parent,self.parent_vt);c.pointer(self.parent+0x1c,self.name);c.pointer(self.parent_vt+0x24,0x596e2c);c.uc.mem_write(self.name,b'authored_floor\0')
  def hook(uc,address,size,unused):
   def ret(value=None):
    if value is not None:c.put(0,value)
    uc.reg_write(c.pc,uc.reg_read(c.lr))
   if self.stage=='clone_move' and address==0x520ca0 or self.stage=='raise' and address==0x520f08:uc.reg_write(c.pc,c.stop)
   elif self.stage=='clone_copy' and address==0x59b2a4:
    self.deep_copy_services+=1;c.pointer(c.reg(0),struct.unpack('<I',uc.mem_read(c.reg(1),4))[0]);ret()
   elif self.stage=='clone_copy' and address==0x585118:
    self.clone_constructor_services+=1;out=c.reg(0);pos=c.reg(3);sp=uc.reg_read(c.sp);rot,scale=struct.unpack('<2I',uc.mem_read(sp,8));uc.mem_write(out,bytes(320));c.pointer(out,c.symbols['_ZTVN6glitch5scene14CMeshSceneNodeE']+0x1c);c.pointer(out+0x130,self.mesh)
    for destination,source,n in ((out+0xac,pos,12),(out+0xb8,rot,16),(out+0xc8,scale,12)):uc.mem_write(destination,bytes(uc.mem_read(source,n)))
    uc.mem_write(out+0x11c,struct.pack('<I',14));ret(out)
   elif self.stage=='clone_copy' and address==0x598a04:c.pointer(c.reg(0)+0x1c,c.reg(1));ret()
  c.uc.hook_add(UC_HOOK_CODE,hook)
 def clone(self,position,rotation,scale):
  c=self.cpu;self.heap=c.data+0x200000;vt=c.data+0x5000;c.pointer(self.node,vt);c.pointer(self.node+0xec,self.parent);c.pointer(self.node+0x130,self.mesh)
  for slot,address in ((0x90,0x5970bc),(0x98,0x5970ec),(0xa0,0x597124),(0xa4,0x59712c),(0xf8,0x584aa4)):c.pointer(vt+slot,address)
  c.uc.mem_write(self.node+0x54,position);c.uc.mem_write(self.node+0xac,bytes(12));c.uc.mem_write(self.node+0xb8,rotation);c.uc.mem_write(self.node+0xc8,scale);c.uc.mem_write(self.node+0x11c,bytes(4));c.put(4,self.floor);c.put(5,self.node);self.stage='clone_move';c.invoke(0x520c60,[self.node]);assert bytes(c.uc.mem_read(self.node+0xac,12))==position
  self.stage='clone_copy';clone=c.invoke(0x50f89c,[self.node]);self.stage=None;self.clone_calls+=1
  # Run actual dirty relative-matrix and parentless absolute update bodies.
  c.invoke(0x597c60,[clone,0]);return bytes(c.uc.mem_read(clone+0x24,68))
 def raised(self,raw):
  c=self.cpu;at=c.data+0x810000;c.uc.mem_write(at,raw);c.pointer(self.floor+0x68,at);c.uc.mem_write(self.floor+0x6c,struct.pack('<I',len(raw)//36));c.uc.mem_write(c.stack+0xe030,struct.pack('<I',len(raw)//36));c.put(4,self.floor);c.put(5,at);self.stage='raise';c.invoke(0x520e8c,[]);self.stage=None;return bytes(c.uc.mem_read(at,len(raw)))

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--asset-library',type=Path,required=True);p.add_argument('--inputs',type=Path,required=True);p.add_argument('--floor-output',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args();start=time.monotonic();manifest=json.loads((ROOT/'reference/floor-records/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];old=OriginalRecordOracle(a.engine,manifest);new=Cpu(a.library,True,{'functions':[]});new.load_asset_library(a.asset_library);r=Reader(a.inputs);assert r.word()==0x31494c46;count=r.word();assert 0<count<=512
 matrix_new=new.data+0x200000;trs_new=new.data+0x210000;output=new.data+0x10000;descriptor=new.data+0x1000;stream=new.data+0x300000;written=new.data+0x2000;box_new=new.data+0x220000;local_new=new.data+0x221000;floor_data={'floor':[],'floor_bounds':{},'floor_object_flags':{},'source':'actual authored floor streams verified against original selector constructor and clone/bounds routines'};triangles=0;clone_comparisons=0
 def clone_case(position,rotation,scale):
  nonlocal clone_comparisons
  expected=old.clone(position,rotation,scale);new.uc.mem_write(trs_new,position+rotation+scale);assert new.invoke('dh2_floor_clone_matrix',[matrix_new,trs_new,trs_new+12,trs_new+28])==0;actual=bytes(new.uc.mem_read(matrix_new,68));assert equal(expected,actual),('clone matrix',expected.hex(),actual.hex());clone_comparisons+=1;return expected
 for i in range(count):
  room,geometry,name_len=r.word(),r.word(),r.word();name=r.read(name_len).decode();position,rotation,scale=r.read(12),r.read(16),r.read(12);local,flags=r.read(24),r.read(8);assert flags==struct.pack('<2I',0,1);parts=[]
  for j in range(r.word()):
   vertices,draw,kind,components,stride,primitive,vn,inn=struct.unpack('<8I',r.read(32));parts.append((r.read(vn),r.read(inn),vertices,draw,kind,components,stride,primitive))
  clone_stored,world_stored,bounds_stored=r.read(68),r.read(24),r.read(24);n=r.word();raw_stored,raised_stored=r.read(n*36),r.read(n*36);clone=clone_case(position,rotation,scale);assert equal(clone_stored,clone)
  raw=old.create(parts,clone,True);assert equal(raw,raw_stored) and len(raw)==n*36;at=stream;descriptors=[]
  for vertex,indices,vertices,draw,kind,components,stride,primitive in parts:
   vp=at;new.uc.mem_write(at,vertex);at+=(len(vertex)+15)&~15;ip=at if indices else 0
   if indices:new.uc.mem_write(at,indices);at+=(len(indices)+15)&~15
   descriptors.append(struct.pack('<Q4IQII',vp,kind,components,stride,vertices,ip,draw,primitive))
  new.uc.mem_write(descriptor,b''.join(descriptors));new.uc.mem_write(matrix_new,clone);assert new.invoke('dh2_floor_mesh_triangles',[output,n,written,descriptor,len(parts),matrix_new,1],budget=20000000)==0;assert struct.unpack('<I',new.uc.mem_read(written,4))[0]==n and equal(bytes(new.uc.mem_read(output,n*36)),raw)
  world,bounds=old.bounds(local,clone);assert equal(world,world_stored) and equal(bounds,bounds_stored);new.uc.mem_write(local_new,local);assert new.invoke('dh2_floor_transform_bounds',[box_new,local_new,matrix_new])==0 and equal(bytes(new.uc.mem_read(box_new,24)),world);assert new.invoke('dh2_floor_source_bounds',[box_new,box_new])==0 and equal(bytes(new.uc.mem_read(box_new,24)),bounds)
  raised=old.raised(raw);assert equal(raised,raised_stored);assert new.invoke('dh2_floor_raise_triangles',[output,output,n])==0 and equal(bytes(new.uc.mem_read(output,n*36)),raised);triangles+=n;floor_data['floor_bounds'][str(i)]=list(struct.unpack('<6f',bounds));floor_data['floor_object_flags'][str(i)]=struct.unpack('<2I',flags)[1]
  for row in struct.iter_unpack('<9f',raw):floor_data['floor'].append({'room':i,'corners':[list(row[k:k+3]) for k in (0,3,6)]})
 assert r.at==len(r.raw)
 # Nontrivial local rotation/scale exercises the clone path independently of
 # Crypt's identity BaseMeshSceneNode children.
 rng=random.Random(20261012)
 for i in range(200):
  position=struct.pack('<3f',*[rng.uniform(-10000,10000) for _ in range(3)]);rotation=struct.pack('<4f',*[rng.uniform(-1,1) for _ in range(4)]);scale=struct.pack('<3f',*[rng.uniform(-4,4) for _ in range(3)]);clone_case(position,rotation,scale)
 a.floor_output.write_text(json.dumps(floor_data,separators=(',',':'))+'\n');report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'asset_library_sha256':hashlib.sha256(a.asset_library.read_bytes()).hexdigest(),'input_sha256':hashlib.sha256(a.inputs.read_bytes()).hexdigest(),'floor_source_sha256':hashlib.sha256(a.floor_output.read_bytes()).hexdigest(),'authored_floor_records':count,'authored_mesh_constructor_comparisons':count,'authored_triangle_comparisons':triangles,'authored_bounds_comparisons':count,'authored_raised_triangle_comparisons':triangles,'clone_transform_comparisons':clone_comparisons,'original_copy_mesh_scene_node_calls':old.clone_calls,'resource_copy_services':old.deep_copy_services,'clone_constructor_services':old.clone_constructor_services,'original_import_calls':old.cpu.import_calls,'mismatches':0,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-start,2)};a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
