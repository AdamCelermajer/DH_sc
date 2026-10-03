"""Original portal smoothing/path lifecycle versus compiled ARM64 C++.

Original lineIntersection, SmoothPath, CalcWaypointVec, IsPastWaypoint,
GetPathLengthSQ, DropPath and MovePath execute. Graph/list ownership and
allocators use bounded fixtures. Immutable graph coordinates and supplied
PFObject fields are caller facts; this does not verify actor initialization,
FindPath composition, positional validation or the character movement loop.
"""
import argparse,hashlib,json,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from navigation_search_differential import OriginalSearch,SearchCpu,words,word
from navigation_world_differential import linked_state
from navigation_differential import Cpu,ROOT,equal
from combat_result_differential import bits

COPIED=0xffffffff
FUNCTIONS=(0,0x52d538,0x52899c,0x528868,0x52aae4,0x524264,0x52d838)
NATIVE=(None,'dh2_nav_smooth_path','dh2_nav_calc_waypoint','dh2_nav_past_waypoint','dh2_nav_drop_path','dh2_nav_path_length','dh2_nav_move_path')

class PathCpu(SearchCpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='__memmove_chk':
   dst,src,count,capacity=[self.reg(i) for i in range(4)];assert count<=capacity and count<=0x2000000
   if count:uc.mem_write(dst,bytes(uc.mem_read(src,count)))
   self.put(0,dst);self.import_calls['__memmove_chk']=self.import_calls.get('__memmove_chk',0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)

class OriginalPath(OriginalSearch):
 def __init__(self,engine,manifest):
  super().__init__(engine,manifest);self.obj=self.c.data+0x8000;self.world=self.c.data+0x9000;self.result=self.c.data+0xa000;self.active=False;self.destroyed=0
  self.c.pointer(self.evt+32,0x51c114);self.c.pointer(self.evt+36,0x51b844);self.c.pointer(self.evt+40,0x51b85c)
 def hook(self,uc,address,size,unused):
  if getattr(self,'active',False):
   if address==0x310570:self.ret(self.allocate(self.c.reg(0)));return
   if address==0x524348:self.destroyed+=1
  super().hook(uc,address,size,unused)
 def prepare(self,nodes,edges,ids,position,target,radius=0,owned=0,waypoint=(13.,-17.,19.),direct=None):
  self.active=False;super().run(nodes,edges,0,0,0,(),(),(),());c=self.c
  for ptr,ident in c.node_ids.items():c.uc.mem_write(ptr+8,nodes[(ident-1)*56+8:(ident-1)*56+40])
  self.active=True;self.destroyed=0;c.uc.mem_write(self.obj,bytes(176));c.uc.mem_write(self.obj+8,struct.pack('<f',radius));c.pointer(self.obj+20,2);c.uc.mem_write(self.obj+24,struct.pack('<3f',*position));c.uc.mem_write(self.obj+64,struct.pack('<3f',*target));c.uc.mem_write(self.obj+128,struct.pack('<3f',*waypoint));c.pointer(self.obj+124,owned)
  embedded=self.obj+76;self.edge_ptrs[0]=embedded;c.edge_ids[embedded]=0
  # This is the original direct-edge vtable; its actual accessors/destructor run.
  c.uc.mem_write(embedded,words(0x96c810,0,0,bits(1.),0,0)+struct.pack('<6f',*(direct or (*position,*target))))
  if owned:
   assert ids and ids[0]==COPIED;ptr=self.allocate(48);c.uc.mem_write(ptr,bytes(c.uc.mem_read(embedded,48)));self.edge_ptrs[COPIED]=ptr;c.edge_ids[ptr]=COPIED
  self.list(self.obj+56,ids)
 def snapshot(self):
  c=self.c;rows=[];header=self.obj+56;p=word(c,header)
  while p!=header:
   edge=word(c,p+8);ident=c.edge_ids.get(edge,COPIED);source=word(c,edge+4);target=word(c,edge+8)
   coords=bytes(c.uc.mem_read(source+8,12))+bytes(c.uc.mem_read(target+8,12)) if source else bytes(c.uc.mem_read(edge+24,24))
   rows.append(words(ident,c.node_ids.get(source,0),c.node_ids.get(target,0))+bytes(c.uc.mem_read(edge+12,12))+coords);p=word(c,p)
  return words(word(c,self.obj+20))+bytes(c.uc.mem_read(self.obj+8,4))+bytes(c.uc.mem_read(self.obj+100,24))+bytes(c.uc.mem_read(self.obj+24,12))+bytes(c.uc.mem_read(self.obj+64,12))+bytes(c.uc.mem_read(self.obj+128,12))+words(len(rows),word(c,self.obj+124))+b''.join(rows)
 def operate(self,op):
  c=self.c
  if op in (3,5):output=words(c.invoke(FUNCTIONS[op],[self.world,self.obj] if op==3 else [self.obj]))
  elif op==6:
   before=self.snapshot();past=c.invoke(0x528868,[self.world,self.obj]) if word(c,self.obj+56)!=self.obj+56 else 0
   active=c.invoke(FUNCTIONS[op],[self.world,self.obj,self.result]);output=words(active,past)+bytes(c.uc.mem_read(self.result,12))
  else:c.invoke(FUNCTIONS[op],[self.world,self.obj]);output=b''
  return self.snapshot(),output

class NativePath:
 def __init__(self,library,nodes,edges):
  self.c=PathCpu(library,True,{'functions':[]});c=self.c;d=c.data;self.graph=d+0x1000;self.obj=d+0x2000;self.segments=d+0x10000;self.out=d+0x3000
  c.uc.mem_write(d+0x20000,nodes);c.uc.mem_write(d+0x40000,edges);n=len(nodes)//56;e=len(edges)//20;c.uc.mem_write(self.graph,struct.pack('<4Q12I2Q',d+0x20000,d+0x40000,0,0,n,e,0,0,n,e,0,0,0,0,0,0,0,0))
 def load(self,raw):
  c=self.c;count,owned=struct.unpack_from('<2I',raw,68);c.uc.mem_write(self.obj,raw[:68]+bytes(4)+struct.pack('<Q4I',self.segments,count,1024,owned,0))
  if count:c.uc.mem_write(self.segments,raw[76:])
 def snapshot(self):
  c=self.c;count,owned=struct.unpack('<2I',c.uc.mem_read(self.obj+80,12)[0:4]+c.uc.mem_read(self.obj+88,4));return bytes(c.uc.mem_read(self.obj,68))+words(count,owned)+(bytes(c.uc.mem_read(self.segments,count*48)) if count else b'')
 def operate(self,op):
  c=self.c
  args=[self.obj,self.graph] if op==1 else [self.obj] if op in (2,4) else [self.out,self.obj,self.graph] if op==6 else [self.out,self.obj]
  c.uc.mem_write(self.out,bytes(24));status=c.invoke(NATIVE[op],args);assert status==0,('path operation rejected',op,status)
  output=bytes(c.uc.mem_read(self.out,20 if op==6 else 4)) if op in (3,5,6) else b'';return self.snapshot(),output

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--linked-reference',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--cases',type=int,default=320);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/navigation-path/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];nodes,edges,_=linked_state(a.linked_reference);old=OriginalPath(a.engine,manifest);new=NativePath(a.library,nodes,edges);rng=random.Random(20261010);lines=[];steps=[];kinds=[0]*6;operations=[0]*7;specials=[];sessions=0
 # Independent 2D intersection coverage, including epsilon/endpoint boundaries
 # and parallel output preservation. Each comparison includes exact parameters.
 endpoints=[((0,0),(10,0),(5,-2),(5,2)),((0,0),(10,0),(5,2),(5,4)),((0,0),(10,0),(20,-2),(20,2)),((0,0),(10,0),(20,2),(20,4)),((0,0),(10,0),(2,0),(8,0)),((0,0),(10,0),(2,1),(8,1)),((0,0),(0,0),(0,0),(0,0))]
 endpoints += [((0,0),(10,0),(x,-2),(x,2)) for x in (-1e-5,0,1e-5,9.99999,10,10.00001)]
 for epsilon in (0.,-0.,1e-45,1e-5,1e-4,1.00001e-4,-1e-5,-1e-4):endpoints.append(((0,0),(1,0),(0,1),(1,1+epsilon)))
 for i in range(a.cases*2):endpoints.append(tuple((rng.uniform(-10000,10000),rng.uniform(-10000,10000)) for _ in range(4)))
 for row in endpoints:
  points=struct.pack('<8f',*(x for pair in row for x in pair));initial=struct.pack('<4f',13.,-17.,19.,-23.);c=old.c;d=c.data+0x40000;c.uc.mem_write(d,points+initial);kind=c.invoke(0x312848,[d,d+8,d+16,d+24,d+32,d+40,d+44]);expected=bytes(c.uc.mem_read(d+32,16))+words(kind)
  c=new.c;d=c.data+0x50000;c.uc.mem_write(d,points+initial+words(0,0));assert c.invoke('dh2_nav_line_intersection',[d+32,d,d+8,d+16,d+24])==0;actual=bytes(c.uc.mem_read(d+32,20));assert equal(expected,actual),('intersection',row,expected.hex(),actual.hex());lines.append(points+initial+expected);kinds[kind]+=1
 def compare(op,label=None):
  before=old.snapshot();new.load(before);expected=old.operate(op);actual=new.operate(op)
  assert equal(expected[0],actual[0]) and equal(expected[1],actual[1]),(op,label,before.hex(),expected,actual)
  steps.append(words(op,len(before),len(expected[0]),len(expected[1]))+before+expected[0]+expected[1]);operations[op]+=1
  if label:specials.append({'case':label,'operation':op,'before_segments':(len(before)-76)//48,'after_segments':(len(expected[0])-76)//48,'output_words':list(struct.unpack('<'+'I'*(len(expected[1])//4),expected[1]))})
 for i in range(a.cases):
  count=rng.randrange(1,8);ids=tuple(rng.randrange(1,len(edges)//20+1) for _ in range(count));source=struct.unpack_from('<I',edges,(ids[0]-1)*20)[0];point=struct.unpack_from('<3f',nodes,(source-1)*56+8);target=tuple(rng.uniform(-4000,4000) for _ in range(3));radius=rng.choice((0.,1.,36.,100.,1000.));old.prepare(nodes,edges,ids,point,target,radius);sessions+=1
  compare(5);compare(1);compare(1);compare(2);compare(3);compare(5)
  for _ in range(count+2):
   if word(old.c,old.obj+56)!=old.obj+56:
    snapshot=old.snapshot();s=struct.unpack_from('<3f',snapshot,100);way=struct.unpack_from('<3f',snapshot,56);position=[s[k]+rng.choice((-1.,0.,1.))*way[k] for k in range(3)];old.c.uc.mem_write(old.obj+24,struct.pack('<3f',*position))
   compare(6)
  compare(4);compare(5)
  if i%80==79:print(f'Path sessions {i+1}/{a.cases}',flush=True)
 # Direct edge has no destination node. Smoothing still owns a copy; radius
 # and target must not change its coordinates. Empty Drop leaves target intact.
 old.prepare(nodes,edges,(0,1),(-1,2,3),(5,6,7),1000);sessions+=1
 for op in (1,1,2,3,5,6,4,4,5,6):compare(op,'embedded direct edge / owned lifecycle')
 old.prepare(nodes,edges,(COPIED,1,2),(-100,20,30),(40,50,60),owned=1);sessions+=1
 for op in (1,2,5,4,4,5,6):compare(op,'preexisting owned edge drop')
 old.prepare(nodes,edges,(),(1,2,3),(4,5,6));sessions+=1
 for op in (4,5,6):compare(op,'empty path target preservation')
 reference=words(0x31564e50,len(lines),len(steps),len(nodes)//56,len(edges)//20)+nodes+edges+b''.join(lines)+b''.join(steps);a.reference_output.write_bytes(reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'linked_reference_sha256':hashlib.sha256(a.linked_reference.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'line_comparisons':len(lines),'line_classifications':kinds,'path_operation_comparisons':len(steps),'operation_counts':operations,'sessions':sessions,'mismatches':0,'owned_edge_destructors_execute':True,'original_import_calls':old.c.import_calls,'native_import_calls':new.c.import_calls,'behavior_snapshots':specials,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)}
 a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='behavior_snapshots'}),flush=True)
if __name__=='__main__':main()
