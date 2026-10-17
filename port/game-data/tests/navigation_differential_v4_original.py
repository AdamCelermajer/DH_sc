"""Original PFFloor graph construction versus compiled ARM64 source.

CreateNodes/CreateNode/CreateEdge, CompPos lookup/insertion and RB balancing,
normalization and weight writes execute original instructions. GraphSparse node
and edge allocation are caller storage fixtures (deduplicated directed pairs).
Floor collision results are supplied snapshots, with every query coordinate and
call order compared. This does not validate floor collision or A* routing.
"""
import argparse,hashlib,json,math,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_S0
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../game-data/tests'))
from aggro_differential import Cpu as Base,float_bits
from combat_result_differential import floating

class Cpu(Base):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if address==self.callback+96:
   if self.arm64:
    point=bytes(uc.mem_read(self.reg(2),12));expected,status=self.queries[self.query_index];self.query_index+=1
    assert equal(point,expected),('floor query',self.query_index,point.hex(),expected.hex())
    self.put(0,status)
   else:self.put(0,struct.unpack('<I',uc.mem_read(self.reg(0)+4,4))[0])
  elif name in ('__aeabi_fmul','__aeabi_fdiv'):
   a,b=floating(self.reg(0)),floating(self.reg(1))
   value=a*b if name.endswith('fmul') else a/b if b else math.nan if not a else math.copysign(math.inf,a*math.copysign(1,b))
   self.put(0,float_bits(value))
  elif name=='sqrtf':
   raw=uc.reg_read(UC_ARM64_REG_S0) if self.arm64 else self.reg(0);value=floating(raw);result=float_bits(math.sqrt(value) if value>=0 else math.nan)
   if self.arm64:uc.reg_write(UC_ARM64_REG_S0,result)
   else:self.put(0,result)
  elif name in ('__aeabi_fcmplt','__aeabi_fcmple','__aeabi_fcmpeq'):
   a,b=floating(self.reg(0)),floating(self.reg(1));self.put(0,int(a<b if name.endswith('lt') else a<=b if name.endswith('le') else a==b))
  else:return super().external(uc,address,size,unused)
  self.import_calls[name or 'floor/id callback']=self.import_calls.get(name or 'floor/id callback',0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))

def equal(a,b):
 return len(a)==len(b) and all(x==y or math.isnan(floating(x)) and math.isnan(floating(y)) for x,y in zip(struct.unpack('<'+'I'*(len(a)//4),a),struct.unpack('<'+'I'*(len(b)//4),b)))

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--floor',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--collision',action='store_true');p.add_argument('--selector',action='store_true');args=p.parse_args();started=time.monotonic()
 if args.selector:args.collision=True
 manifest=json.loads((ROOT/'reference/navigation/original-functions.json').read_text());assert hashlib.sha256(args.engine.read_bytes()).hexdigest()==manifest['original_sha256'];old=Cpu(args.engine,False,manifest);new=Cpu(args.library,True,{'functions':[]})
 floor_data=json.loads(args.floor.read_text(encoding='utf-8-sig'));rng=random.Random(20261007);capacity=4096
 def authored_bounds(f):
  if 'floor_bounds' in floor_data:return struct.pack('<6f',*floor_data['floor_bounds'][str(f)])
  return floor_bounds(floor_views[f])
 of=old.data+0x1000;og=old.data+0x3000;vt=old.data+0x4000;ot=old.data+0x5000;ovalid=old.data+0x10000;oinvalid=old.data+0x20000
 ng=new.data+0x1000;nt=new.data+0x2000;nnodes=new.data+0x10000;nedges=new.data+0x50000;ninvalid=new.data+0xa0000;nvalid=new.data+0x100000
 collision_oracle=None;floor_views={};floor_set=new.data+0x120000;floor_array=floor_set+0x100;floor_geometry=floor_set+0x1000
 if args.collision:
  from collision_differential import OriginalFloorOracle,packed_triangle,floor_bounds
  collision_manifest=json.loads((ROOT/'reference/collision/original-functions.json').read_text());assert collision_manifest['original_sha256']==manifest['original_sha256']
  if args.selector:
   from selector_differential import OriginalSelectorOracle
   collision_oracle=OriginalSelectorOracle(args.engine,collision_manifest)
  else:collision_oracle=OriginalFloorOracle(args.engine,collision_manifest)
  for row in floor_data['floor']:floor_views.setdefault(row['room'],[]).append(row['corners'])
  assert sorted(floor_views)==list(range(len(floor_views)))
  new.uc.mem_write(floor_set,struct.pack('<QII',floor_array,len(floor_views),0))
  for f,triangles in floor_views.items():
   raw=b''.join(packed_triangle(t) for t in triangles);new.uc.mem_write(floor_geometry,raw)
   if args.selector:
    base=new.data+0x200000+f*0x50000;tree=base+0x100;workspace=base+0x180
    new.uc.mem_write(tree,struct.pack('<4Q9I',base+0x1000,base+0x21000,base+0x29000,floor_geometry,len(triangles),0,0,2048,8192,2048,15,0,0));assert new.invoke('dh2_octree_build',[tree,floor_geometry,len(triangles),15],budget=20000000)==0
    new.uc.mem_write(workspace,struct.pack('<QQII',base+0x2b000,base+0x2d000,len(triangles),0));new.uc.mem_write(floor_array+56*f,struct.pack('<QQII',tree,0,1,0)+authored_bounds(f)+struct.pack('<Q',workspace))
   else:new.uc.mem_write(floor_array+40*f,struct.pack('<QII',floor_geometry,len(triangles),0)+authored_bounds(f))
   floor_geometry+=len(raw)
  def observe_native_query(uc,address,size,unused):
   assert new.reg(0)==floor_set and new.reg(1)==floor
   point=bytes(uc.mem_read(new.reg(2),12));expected,status=new.queries[new.query_index];new.query_index+=1;assert equal(point,expected),('native collision query',point.hex(),expected.hex())
  query_symbol='dh2_selector_floor_query' if args.selector else 'dh2_collision_floor_query'
  new.uc.hook_add(UC_HOOK_CODE,observe_native_query,begin=new.symbols[query_symbol],end=new.symbols[query_symbol])
 nodes={};edges={};queries=[];heap=old.data+0x200000;floor=0;edge_index=0;probe_index=0;mask=63;service_allocs=0;map_allocs=0;records=[];comparisons=0;node_comparisons=0;edge_comparisons=0;invalid_comparisons=0;query_comparisons=0;tree_comparisons=0;validation_comparisons=0;gates={'disabled_nodes':0,'disabled_edges':0}
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 def put(at,value):old.uc.mem_write(at,struct.pack('<I',value))
 def ret(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def allocate(size):
  nonlocal heap
  ptr=heap;heap+=(size+15)&~15;old.uc.mem_write(ptr,bytes(size));return ptr
 def hook(uc,address,size,unused):
  nonlocal edge_index,probe_index,service_allocs,map_allocs
  if address==0x51fa64:edge_index+=1;probe_index=0
  elif address==0x51badc:
   assert old.reg(0)==of and old.reg(2)==old.reg(3)==0
   point=bytes(uc.mem_read(old.reg(1),12));value=struct.unpack_from('<I',collision_oracle.execute('floor',point))[0] if collision_oracle else (mask>>((edge_index-1)*2+probe_index))&1;probe_index+=1;queries.append((point,value));ret(value)
  elif address==0x51dca4:
   assert old.reg(0)==og;id=old.reg(1);assert id==len(nodes)+1;ptr=allocate(48);old.pointer(ptr,vt);put(ptr+4,id);nodes[id]=(ptr,floor);service_allocs+=1;ret(ptr)
  elif address==0x51e758:
   assert old.reg(0)==og;key=(old.reg(1),old.reg(2));assert all(x in nodes for x in key)
   if key not in edges:
    ptr=allocate(24);old.pointer(ptr,vt);old.pointer(ptr+4,nodes[key[0]][0]);old.pointer(ptr+8,nodes[key[1]][0]);edges[key]=(ptr,len(edges)+1);service_allocs+=1
   ret(edges[key][0])
  elif address==0x708ec0:assert word(old.reg(0))==32;map_allocs+=1;ret(allocate(32))
  elif address==0x708f00:raise AssertionError('Unexpected deallocation')
 old.uc.hook_add(UC_HOOK_CODE,hook)
 old.pointer(vt,old.callback+96);old.pointer(vt+0x14,0x51bc60)
 def reset():
  nonlocal heap
  heap=old.data+0x200000;nodes.clear();edges.clear();old.uc.mem_write(og,bytes(64));new.uc.mem_write(ng,struct.pack('<4Q12I2Q',nnodes,nedges,ninvalid,nvalid,0,0,0,0,*([capacity]*4),0,0,0,0,new.symbols[query_symbol] if args.collision else new.callback+96,floor_set if args.collision else 0))
 def begin(f):
  nonlocal floor
  floor=f;old.uc.mem_write(of,bytes(0x100));old.pointer(of+0x74,og);old.uc.mem_write(of+0x90,struct.pack('<5I',0,0,of+0x90,of+0x90,0));old.uc.mem_write(of+0xa8,struct.pack('<3I',oinvalid,oinvalid,oinvalid+capacity*56));old.uc.mem_write(of+0xc0,struct.pack('<3I',ovalid,ovalid,ovalid+capacity*4));assert new.invoke('dh2_nav_begin_floor',[ng,f])==0
  if collision_oracle:
   raw=b''.join(packed_triangle(t) for t in floor_views[f])
   if args.selector:collision_oracle.build(raw,15);collision_oracle.configure(None,True)
   collision_oracle.prepare(raw,authored_bounds(f))
 def snapshot():
  nonlocal node_comparisons,edge_comparisons,invalid_comparisons,tree_comparisons,validation_comparisons
  counts=struct.unpack('<4I',new.uc.mem_read(ng+32,16));assert counts==(len(nodes),len(edges),(word(of+0xac)-oinvalid)//56+current_native_invalid_start,(word(of+0xc4)-ovalid)//4+current_native_validation_start)
  # Each floor resets only its map and local original output vectors.
  original_nodes=b''.join(struct.pack('<II',id,f)+bytes(old.uc.mem_read(ptr+8,32)) for id,(ptr,f) in nodes.items())
  native_nodes=b''.join(bytes(new.uc.mem_read(nnodes+56*i,40)) for i in range(counts[0]));assert equal(original_nodes,native_nodes),('nodes',floor);node_comparisons+=counts[0]
  original_edges=b''.join(struct.pack('<II',*key)+bytes(old.uc.mem_read(ptr+0x10,8))+bytes(old.uc.mem_read(ptr+0xc,4)) for key,(ptr,id) in edges.items());native_edges=bytes(new.uc.mem_read(nedges,20*counts[1]));assert equal(original_edges,native_edges),('edges',floor);edge_comparisons+=counts[1]
  local_invalid=(word(of+0xac)-oinvalid)//56;original_invalid=bytearray(old.uc.mem_read(oinvalid,56*local_invalid))
  ptr_ids={ptr:id for id,(ptr,f) in nodes.items()}
  for i in range(local_invalid):
   for offset in (36,40):struct.pack_into('<I',original_invalid,i*56+offset,ptr_ids.get(struct.unpack_from('<I',original_invalid,i*56+offset)[0],0))
  native_invalid=bytes(new.uc.mem_read(ninvalid+56*current_native_invalid_start,56*local_invalid));assert equal(original_invalid,native_invalid),('invalid',floor);invalid_comparisons+=local_invalid
  local_validation=(word(of+0xc4)-ovalid)//4;eptr_ids={ptr:id for ptr,id in edges.values()};original_validation=b''.join(struct.pack('<I',eptr_ids.get(word(ovalid+4*i),0)) for i in range(local_validation));native_validation=bytes(new.uc.mem_read(nvalid+4*current_native_validation_start,4*local_validation));assert original_validation==native_validation;validation_comparisons+=local_validation
  tree={}
  def walk(at):
   if not at:return
   id=ptr_ids[word(at+28)];tree[id]=(word(at+4),word(at+8),word(at+12),int(not uc_byte(at)));walk(word(at+8));walk(word(at+12))
  def uc_byte(at):return old.uc.mem_read(at,1)[0]
  walk(word(of+0x94));map_ids={}
  def label(at):
   if not at:return
   map_ids[at]=ptr_ids[word(at+28)];label(word(at+8));label(word(at+12))
  label(word(of+0x94));tree_bytes=bytearray()
  for id,(parent,left,right,red) in tree.items():
   expected=(map_ids.get(parent,0),map_ids.get(left,0),map_ids.get(right,0),red);actual=struct.unpack('<4I',new.uc.mem_read(nnodes+(id-1)*56+40,16));assert actual==expected,('tree',id,expected,actual);tree_bytes.extend(struct.pack('<I4I',id,*expected));tree_comparisons+=1
  assert struct.unpack('<I',new.uc.mem_read(ng+64,4))[0]==map_ids.get(word(of+0x94),0)
  return struct.pack('<4I',*counts)+native_nodes+native_edges+native_invalid+native_validation
 batches=[]
 # Exact cache triangles, supplied all-pass, mixed and all-fail collision facts.
 for mode in (63,0,None):batches.append([(x['room'],x['corners'],0,rng.randrange(64) if mode is None else mode) for x in floor_data['floor']])
 for batch in range(64):
  rows=[]
  for i in range(12):
   a=[rng.uniform(-3000,3000) for _ in range(3)];b=[x+rng.uniform(-600,600) for x in a];c=[x+rng.uniform(-600,600) for x in a]
   if i>0 and i%3==0:a,b,c=rows[-1][1][2],rows[-1][1][1],a
   flags=rng.choice((0,0,0,0x01000000,0x02000000));rows.append((batch//8,[a,b,c],flags,rng.randrange(64)))
  batches.append(rows)
 # Duplicate triangles, zero-length edges and near-epsilon equivalence chains.
 for delta in (0.,.00009,.0001,.00011,-.00009):
  rows=[]
  for i in range(32):rows.append((0,[[i*delta,0.,0.],[1.+i*delta,0.,0.],[i*delta,1.,0.]],0,63))
  rows.extend([(0,[[0.,0.,0.]]*3,0,63),(0,[[0.,0.,0.],[1.,0.,0.],[0.,1.,0.]],0x02000000,63)])
  batches.append(rows)
 if args.collision:batches=[[(x['room'],x['corners'],floor_data.get('floor_object_flags',{}).get(str(x['room']),0),0) for x in floor_data['floor']]]
 floor_summaries=[]
 for bi,rows in enumerate(batches):
  reset();last_floor=None;inputs=[]
  for f,corners,flags,qmask in rows:
   if f!=last_floor:
    if last_floor is not None:
     snapshot();floor_summaries.append({'floor':last_floor,'nodes':len(nodes)-floor_node_start,'edges':len(edges)-floor_edge_start,'invalid':struct.unpack('<I',new.uc.mem_read(ng+40,4))[0]-current_native_invalid_start})
    begin(f);last_floor=f;current_native_invalid_start=struct.unpack('<I',new.uc.mem_read(ng+40,4))[0];current_native_validation_start=struct.unpack('<I',new.uc.mem_read(ng+44,4))[0]
    floor_node_start=len(nodes);floor_edge_start=len(edges)
   packed=struct.pack('<9f',*(x for p in corners for x in p));old.uc.mem_write(ot,packed);put(of+0x20,flags);edge_index=0;queries.clear();mask=qmask;old.invoke(0x520588,[of,ot,1]);assert edge_index in (0,3)
   new.queries=list(queries);new.query_index=0;new.uc.mem_write(nt,packed);assert new.invoke('dh2_nav_triangle',[ng,nt,flags])==0;assert new.query_index==len(queries)
   query_comparisons+=len(queries);comparisons+=1;gates['disabled_nodes']+=bool(flags&0x01000000);gates['disabled_edges']+=bool(flags&0x02000000)
   inputs.append(struct.pack('<II',f,flags)+packed+struct.pack('<I',len(queries))+b''.join(p+struct.pack('<I',s) for p,s in queries))
   # Compare after every triangle; catches reuse, mutation and tree divergence.
   counts=struct.unpack('<4I',snapshot()[:16]);state=bytes(new.uc.mem_read(ng+32,16))+bytes(new.uc.mem_read(nnodes,counts[0]*56))+bytes(new.uc.mem_read(nedges,counts[1]*20))+bytes(new.uc.mem_read(ninvalid,counts[2]*56))+bytes(new.uc.mem_read(nvalid,counts[3]*4))+bytes(new.uc.mem_read(ng+64,16))
   # Arithmetic NaN payload/sign is not portable across CPUs. Canonicalize only
   # NaN words before hashing the original-verified logical snapshot.
   state=b''.join(struct.pack('<I',0x7fc00000 if x&0x7f800000==0x7f800000 and x&0x7fffff else x) for x in struct.unpack('<'+'I'*(len(state)//4),state))
   digest=0xcbf29ce484222325
   for x in state:digest=((digest^x)*0x100000001b3)&0xffffffffffffffff
   inputs[-1]+=struct.pack('<4IQ',*counts,digest)
  records.append(struct.pack('<I',len(inputs))+b''.join(inputs))
  if args.collision:floor_summaries.append({'floor':last_floor,'nodes':len(nodes)-floor_node_start,'edges':len(edges)-floor_edge_start,'invalid':struct.unpack('<I',new.uc.mem_read(ng+40,4))[0]-current_native_invalid_start})
 # Atomic caller rejection checks are independent of the engine's allocator.
 before=bytes(new.uc.mem_read(ng,96));new.uc.mem_write(nt,struct.pack('<9I',0x7f800000,*([0]*8)));assert new.invoke('dh2_nav_triangle',[ng,nt,0])==1;assert bytes(new.uc.mem_read(ng,96))==before
 new.uc.mem_write(nt,bytes(36));new.uc.mem_write(ng+48,struct.pack('<I',struct.unpack_from('<I',before,32)[0]));capacity_before=bytes(new.uc.mem_read(ng,96));assert new.invoke('dh2_nav_triangle',[ng,nt,0])==2;assert bytes(new.uc.mem_read(ng,96))==capacity_before
 floor_prefix=b''
 if args.collision:floor_prefix=struct.pack('<I',len(floor_views))+b''.join(struct.pack('<I',len(ts))+b''.join(packed_triangle(t) for t in ts)+authored_bounds(f) for f,ts in floor_views.items())
 reference=struct.pack('<II',0x3341564e if args.selector else 0x3241564e if args.collision else 0x3141564e,len(records))+floor_prefix+b''.join(records);args.reference_output.write_bytes(reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(args.library.read_bytes()).hexdigest(),'floor_source_sha256':hashlib.sha256(args.floor.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'batches':len(batches),'last_graph_counts':list(counts),'last_graph_state_fnv1a64':f'{digest:016x}','bounds_from_authored_floor_records':'floor_bounds' in floor_data,'triangle_comparisons':comparisons,'node_record_comparisons':node_comparisons,'edge_record_comparisons':edge_comparisons,'invalid_record_comparisons':invalid_comparisons,'validation_reference_comparisons':validation_comparisons,'tree_record_comparisons':tree_comparisons,'floor_query_comparisons':query_comparisons,'mismatches':0,'atomic_rejection_checks':2,'flags':gates,'original_map_allocations':map_allocs,'graph_storage_allocations':service_allocs,'original_import_calls':old.import_calls,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)}
 if args.collision:report.update({'scope':'Original graph construction and original floor/manager/triangle collision instructions compared with native graph calling native floor queries. Selector triangle order/transforms, bounds, room-to-floor grouping and zero floor flags are caller snapshots; original BVH, floor identity/flag producers, sewing and route search remain pending.','floor_support_answers_are_caller_fixtures':False,'original_floor_collision_calls':collision_oracle.calls,'floor_summaries':floor_summaries})
 if args.selector:report.update({'scope':'Original graph plus original floor/manager/selector/matrix/octree/triangle instructions compared with the compiled native chain. Raw already-baked geometry, room-to-floor grouping, bounds, zero flags and node/service ownership are caller inputs. Original mesh extraction/baking, floor producers, sewing and route search remain pending.','original_octree_selector_executes':True,'native_octree_selector_used':True,'selector_triangle_order_is_caller_fixture':False})
 if args.selector and 'floor_object_flags' in floor_data:report.update({'scope':'Original graph/floor/manager/selector/matrix/octree/triangle instructions receive authored geometry and bounds verified separately by floor_records_differential. Object flags come from authored floor records; scene/service ownership is supplied. Full original loader/metadata lookup, cross-floor sewing and route search do not execute.','object_flags_from_authored_floor_records':True})
 args.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
