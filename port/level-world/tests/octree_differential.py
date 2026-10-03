"""Original octree construction and identity-space box selection vs ARM64.

Original bounds, getEdges, octant containment, stable partition, vector growth,
recursive tree construction, node/triangle box filters and AddResult execute.
Allocation/deallocation are caller storage services. Mesh loading, selector
setup/inverse transforms and nonidentity AddResult transforms are not covered.
"""
import argparse,hashlib,json,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from navigation_differential import Cpu as FloatCpu,equal
ROOT=Path(__file__).resolve().parents[1]
class Cpu(FloatCpu):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name in ('_ZdlPv','_ZdaPv','free'):self.frees+=1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--floor',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/octree/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});old.frees=0;rng=random.Random(20261009);data=json.loads(a.floor.read_text(encoding='utf-8-sig'))
 selector=old.data+0x1000;root=old.data+0x2000;input_old=old.data+0x10000;out_old=old.data+0x90000;heap=old.data+0x200000
 nt=new.data+0x1000;triangles_new=new.data+0x10000;nodes_new=new.data+0x40000;ids_new=new.data+0x100000;scratch_new=new.data+0x140000;box_new=new.data+0x150000;out_new=new.data+0x160000;capacity=4096
 allocations=0;node_comparisons=0;triangle_comparisons=0;box_comparisons=0;selected=0;records=[]
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 def ret(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def hook(uc,address,size,unused):
  nonlocal heap,allocations
  if address in (0x5341ac,0x310568):
   size=old.reg(0);assert 0<size<=0x100000;ptr=heap;heap+=(size+15)&~15;assert heap<old.data+0x1f00000;old.uc.mem_write(ptr,bytes(size));allocations+=1;ret(ptr)
  elif address in (0x310450,):old.frees+=1;ret()
 old.uc.hook_add(UC_HOOK_CODE,hook)
 batches=[];rooms={}
 for row in data['floor']:rooms.setdefault(row['room'],[]).append(row['corners'])
 for ts in rooms.values():
  for leaf in (0,1,15,1000):batches.append((ts,leaf))
 for batch in range(40):
  triangles=[]
  for i in range(rng.randrange(16,97)):
   center=[rng.choice((-250.,250.)) if batch%2 else rng.uniform(-1000,1000) for _ in range(3)];size=rng.choice((0.,1.,10.,1000.))
   triangles.append([[c+rng.uniform(-size,size) for c in center] for _ in range(3)])
  batches.append((triangles,rng.choice((0,1,15,30))))
 batches+=[([[[0.,0.,0.]]*3]*20,0),([[[.0000001*i,0.,0.]]*3 for i in range(20)],0)]
 for bi,(ts,leaf) in enumerate(batches):
  raw=b''.join(struct.pack('<9f',*(x for p in t for x in p)) for t in ts);n=len(ts);heap=old.data+0x200000;old.uc.mem_write(selector,bytes(0x100));old.uc.mem_write(root,bytes(68));old.uc.mem_write(input_old,raw);old.uc.mem_write(root,struct.pack('<3I',input_old,input_old+len(raw),input_old+len(raw)));old.uc.mem_write(selector+0xb4,struct.pack('<I',leaf));old.invoke(0x587e60,[selector,root],budget=20000000)
  new.uc.mem_write(triangles_new,raw);new.uc.mem_write(nt,struct.pack('<4Q9I',nodes_new,ids_new,scratch_new,triangles_new,n,0,0,capacity,capacity*8,capacity,leaf,0,0));assert new.invoke('dh2_octree_build',[nt,triangles_new,n,leaf],budget=20000000)==0
  tree_records=[]
  def compare(op,np):
   nonlocal node_comparisons,triangle_comparisons
   first,count=struct.unpack('<2I',new.uc.mem_read(nodes_new+(np-1)*64,8));obounds=bytes(old.uc.mem_read(op+44,24));nbounds=bytes(new.uc.mem_read(nodes_new+(np-1)*64+40,24));assert equal(obounds,nbounds),(bi,'bounds',obounds.hex(),nbounds.hex())
   on=(word(op+4)-word(op))//36;assert on==count,(bi,'node triangles',on,count);oraw=bytes(old.uc.mem_read(word(op),on*36));indices=struct.unpack('<'+'I'*count,new.uc.mem_read(ids_new+first*4,count*4)) if count else ();nraw=b''.join(raw[36*i:36*(i+1)] for i in indices);assert oraw==nraw,(bi,'stable partition')
   ochildren=[word(op+12+4*i) for i in range(8)];nchildren=struct.unpack('<8I',new.uc.mem_read(nodes_new+(np-1)*64+8,32));assert [bool(x) for x in ochildren]==[bool(x) for x in nchildren],(bi,'octants')
   tree_records.append(obounds+struct.pack('<II',count,sum(bool(x)<<i for i,x in enumerate(ochildren)))+oraw);node_comparisons+=1;triangle_comparisons+=count
   for oc,nc in zip(ochildren,nchildren):
    if oc:compare(oc,nc)
  compare(root,1);assert word(selector+0xb0)==struct.unpack('<I',new.uc.mem_read(nt+36,4))[0]
  all_points=[p for t in ts for p in t];low=[min(p[i] for p in all_points) for i in range(3)];high=[max(p[i] for p in all_points) for i in range(3)];queries=[]
  boxes=[[*low,*high]]
  for i in range(30):
   p=[rng.uniform(x-100,y+100) for x,y in zip(low,high)];size=rng.choice((0.,.000001,1.,50.,1000.));boxes.append([*(x-size for x in p),*(x+size for x in p)])
  for box in boxes:
   packed=struct.pack('<6f',*box);old.uc.mem_write(selector+0x44,packed);old.uc.mem_write(selector+0x9c,b'\1');old.uc.mem_write(selector+0xa0,struct.pack('<3I',out_old,n,0));old.invoke(0x5870fc,[selector,root],budget=5000000);expected_count=word(selector+0xa8);assert expected_count<=n
   expected=bytes(old.uc.mem_read(out_old,expected_count*36));new.uc.mem_write(box_new,packed);count=new.invoke('dh2_octree_box',[nt,box_new,out_new,n],budget=5000000);indices=struct.unpack('<'+'I'*count,new.uc.mem_read(out_new,count*4)) if count else ();actual=b''.join(raw[36*i:36*(i+1)] for i in indices);assert count==expected_count and actual==expected,(bi,'box query',box,count,expected_count)
   queries.append(packed+struct.pack('<I',count)+expected);box_comparisons+=1;selected+=count
  tree=b''.join(tree_records);records.append(struct.pack('<III',n,leaf,len(tree_records))+raw+struct.pack('<I',len(tree))+tree+struct.pack('<I',len(queries))+b''.join(queries))
 reference=struct.pack('<II',0x3154434f,len(records))+b''.join(records);a.reference_output.write_bytes(reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'floor_source_sha256':hashlib.sha256(a.floor.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'build_comparisons':len(batches),'node_comparisons':node_comparisons,'partitioned_triangle_comparisons':triangle_comparisons,'box_comparisons':box_comparisons,'selected_triangle_comparisons':selected,'mismatches':0,'original_allocations':allocations,'original_deallocations':old.frees,'original_import_calls':old.import_calls,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)};a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
