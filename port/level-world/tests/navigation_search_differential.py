"""Original graph-node search versus compiled native ARM64 instructions.

The complete findNode/markNode, GraphSparse lookup, original map/list/vector
instructions, RB balancing and heap operations execute. Predicates and graph
container ownership are caller fixtures; the original allocator is supplied
bounded storage. World endpoint selection and movement are outside this audit.
"""
import argparse,hashlib,json,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from navigation_differential import Cpu,ROOT,equal

def words(*values):return struct.pack('<'+'I'*len(values),*values)
def word(cpu,address):return struct.unpack('<I',cpu.uc.mem_read(address,4))[0]

class SearchCpu(Cpu):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if not self.arm64 and name in ('malloc','free'):
   self.import_calls[name]=self.import_calls.get(name,0)+1;self.put(0,self.allocate(self.reg(0)) if name=='malloc' else 0);uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif address in (self.callback+128,self.callback+144,self.callback+160):
   kind=(address-self.callback-128)//16;ident=self.reg(1)
   if not self.arm64:ident=(self.edge_ids if kind==1 else self.node_ids)[ident]
   value=int(ident in self.goals) if kind==0 else int(ident not in (self.denied_edges if kind==1 else self.denied_nodes))
   self.events.append((kind,ident,value));self.put(0,value);uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:return super().external(uc,address,size,unused)

class OriginalSearch:
 def __init__(self,engine,manifest):
  self.c=SearchCpu(engine,False,manifest);c=self.c;self.g=c.data+0x1000;self.a=c.data+0x1100;self.test=c.data+0x1200;self.out=c.data+0x1300
  self.nvt=c.data+0x2000;self.evt=c.data+0x2100;self.tvt=c.data+0x2200;self.allocations=0;self.frees=0;self.states=b''
  c.allocate=self.allocate
  c.pointer(self.nvt,c.callback+96);c.pointer(self.evt+4,0x51bc48);c.pointer(self.evt+12,0x51bc50);c.pointer(self.evt+16,0x51c104)
  for k in range(3):c.pointer(self.tvt+k*4,c.callback+128+k*16)
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def allocate(self,count):
  assert 0<count<0x100000;address=self.heap;self.heap+=(count+15)&~15;assert self.heap<self.c.data+0x1800000
  self.c.uc.mem_write(address,bytes(count));return address
 def ret(self,value=0):self.c.put(0,value);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def hook(self,uc,address,size,unused):
  c=self.c
  if address==0x708ec0:self.allocations+=1;self.ret(self.allocate(word(c,c.reg(0))))
  elif address in (0x708f00,0x310440):self.frees+=1;self.ret()
  elif address==0x52b3cc:
   base=uc.reg_read(c.sp)+0x34;states=bytearray(self.count*16)
   def walk(p):
    if not p:return
    walk(word(c,p+8));ident=word(c,p+16);edge=word(c,p+20)
    struct.pack_into('<4I',states,(ident-1)*16,c.edge_ids.get(edge,0),word(c,p+24),word(c,p+28),1);walk(word(c,p+12))
   walk(word(c,base+4));self.states=bytes(states)
 def tree(self,header,entries):
  c=self.c;allocated=[]
  def make(rows,parent):
   if not rows:return 0
   mid=len(rows)//2;p=self.allocate(24);allocated.append((rows[mid][0],p));left=make(rows[:mid],p);right=make(rows[mid+1:],p)
   c.uc.mem_write(p,words(1,parent,left,right,*rows[mid]));return p
  root=make(sorted(entries),header);ordered=sorted(allocated);c.uc.mem_write(header,words(0,root,ordered[0][1] if ordered else header,ordered[-1][1] if ordered else header,len(entries)))
 def list(self,header,ids):
  c=self.c;c.uc.mem_write(header,words(header,header))
  for ident in ids:
   p=self.allocate(12);last=word(c,header+4);c.uc.mem_write(p,words(header,last,self.edge_ptrs[ident]));c.pointer(last,p);c.pointer(header+4,p)
 def run(self,nodes,edges,start,limit,external,prefix,goals,denied_edges,denied_nodes):
  c=self.c;self.heap=c.data+0x100000;self.count=len(nodes)//56;self.states=bytes(self.count*16);c.node_ids={};c.edge_ids={};self.edge_ptrs={}
  c.events=[];c.goals=set(goals);c.denied_edges=set(denied_edges);c.denied_nodes=set(denied_nodes)
  node_ptrs={}
  for i in range(self.count):
   p=self.allocate(80);node_ptrs[i+1]=p;c.node_ids[p]=i+1;c.pointer(p,self.nvt);c.pointer(p+4,i+1)
  outgoing={i:[] for i in node_ptrs}
  for i in range(len(edges)//20):
   source,target,distance,clearance,weight=struct.unpack_from('<5I',edges,i*20);p=self.allocate(24);self.edge_ptrs[i+1]=p;c.edge_ids[p]=i+1
   c.uc.mem_write(p,words(self.evt,node_ptrs[source],node_ptrs[target],weight,distance,clearance));outgoing[source].append((target,p))
  for ident,p in node_ptrs.items():self.tree(p+0x2c,outgoing[ident])
  c.uc.mem_write(self.g,bytes(64));self.tree(self.g+4,list(node_ptrs.items()));c.uc.mem_write(self.a,bytes(48));c.pointer(self.a+4,self.g)
  self.list(self.a+8,[]);self.list(self.out,prefix);c.pointer(self.test,self.tvt)
  found=c.invoke(0x52ad4c,[self.a,start,self.test,limit,self.out if external else 0],budget=50000000)
  header=self.out if external else self.a+8;path=[];p=word(c,header)
  while p!=header:
   assert len(path)<=self.count+len(prefix);path.append(c.edge_ids[word(c,p+8)]);p=word(c,p)
  stats=(found,*struct.unpack('<4I',c.uc.mem_read(self.a+0x18,16)),len(path))
  return words(*stats),words(*path),self.states,tuple(c.events)

class NativeSearch:
 def __init__(self,library):self.c=SearchCpu(library,True,{'functions':[]})
 def run(self,nodes,edges,start,limit,external,prefix,goals,denied_edges,denied_nodes):
  c=self.c;d=c.data;n=len(nodes)//56;e=len(edges)//20
  g=d+0x1000;view=d+0x1100;test=d+0x1200;request=d+0x1300;result=d+0x1400;workspace=d+0x1500
  np=d+0x10000;ep=d+0x100000;order=d+0x200000;offsets=d+0x300000;states=d+0x400000;heap=d+0x500000;path=d+0x600000
  if nodes:c.uc.mem_write(np,nodes)
  if edges:c.uc.mem_write(ep,edges)
  ordered=[];off=[0]
  for ident in range(1,n+1):
   outgoing=[(struct.unpack_from('<I',edges,i*20+4)[0],i+1) for i in range(e) if struct.unpack_from('<I',edges,i*20)[0]==ident]
   ordered.extend(i for _,i in sorted(outgoing));off.append(len(ordered))
  if ordered:c.uc.mem_write(order,words(*ordered))
  c.uc.mem_write(offsets,words(*off));c.uc.mem_write(g,struct.pack('<4Q12I2Q',np,ep,0,0,n,e,0,0,n,e,0,0,0,0,0,0,0,0))
  c.uc.mem_write(view,struct.pack('<3Q',g,order,offsets));c.uc.mem_write(test,struct.pack('<4Q',c.callback+128,c.callback+144,c.callback+160,0))
  c.uc.mem_write(request,struct.pack('<2Q4I',view,test,start,limit,external,0));c.uc.mem_write(result,struct.pack('<6IQ2I',0,0,0,0,0,len(prefix),path,n+len(prefix),0))
  c.uc.mem_write(workspace,struct.pack('<2Q2I',states,heap,n,e+1));
  if n:c.uc.mem_write(states,bytes(n*16))
  if prefix:c.uc.mem_write(path,words(*prefix))
  c.events=[];c.goals=set(goals);c.denied_edges=set(denied_edges);c.denied_nodes=set(denied_nodes)
  assert c.invoke('dh2_nav_search',[result,request,workspace],budget=50000000)==0
  stats=bytes(c.uc.mem_read(result,24));count=struct.unpack_from('<I',stats,20)[0]
  return stats,bytes(c.uc.mem_read(path,count*4)) if count else b'',bytes(c.uc.mem_read(states,n*16)) if n else b'',tuple(c.events)

def crypt_graph(path):
 raw=path.read_bytes();magic,count=struct.unpack_from('<2I',raw);assert magic==0x314b4e4c and count;at=8
 length=struct.unpack_from('<I',raw,at)[0];at+=4+length;steps=struct.unpack_from('<I',raw,at)[0];at+=4
 for _ in range(steps):
  a,b,length=struct.unpack_from('<3I',raw,at);at+=12;state=raw[at:at+length];at+=length
 n,e=struct.unpack_from('<2I',state);return state[40:40+n*56],state[40+n*56:40+n*56+e*20]

def synthetic(n,rows):
 nodes=b''.join(words(i+1,0)+bytes(48) for i in range(n));edges=b''.join(words(a,b)+struct.pack('<3f',w,1.,w) for a,b,w in rows);return nodes,edges

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--linked-reference',type=Path,required=True)
 p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--cases',type=int,default=600);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/navigation-search/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=OriginalSearch(a.engine,manifest);new=NativeSearch(a.library);records=[];labels=[];probes=[];rng=random.Random(20261008);comparisons=events=state_count=success=failure=0
 def compare(nodes,edges,start,limit,external=0,prefix=(),goals=(),denied_edges=(),denied_nodes=(),label=None):
  nonlocal comparisons,events,state_count,success,failure
  request=(nodes,edges,start,limit,external,prefix,goals,denied_edges,denied_nodes);expected=old.run(*request);actual=new.run(*request)
  for kind,(left,right) in enumerate(zip(expected,actual)):assert (equal(left,right) if kind==2 else left==right),(comparisons,label,kind,left,right)
  n=len(nodes)//56;e=len(edges)//20;stats,path,states,calls=expected
  record=words(n,e,start,limit,external,len(prefix))+nodes+edges+words(*(int(i in goals)|(int(i in denied_nodes)<<1) for i in range(1,n+1)))+words(*(int(i in denied_edges) for i in range(1,e+1)))+words(*prefix)+stats+path+states+words(len(calls))+b''.join(words(*call) for call in calls)
  records.append(record);comparisons+=1;events+=len(calls);state_count+=n;success+=word_value(stats)!=0;failure+=word_value(stats)==0
  if label:labels.append({'case':label,'stats':list(struct.unpack('<6I',stats)),'path':list(struct.unpack('<'+'I'*(len(path)//4),path))})
  if label and label.startswith('Crypt-') and limit==10000:probes.append(words(start,goals[0])+stats+path)
 def word_value(raw):return struct.unpack_from('<I',raw)[0]
 # Explicit short searches expose goal bypass, early queue purge, budget and
 # reverse output semantics before the large authored graph fixtures.
 explicit=[(0,[]),(1,[]),(4,[(1,2,1),(1,3,1),(2,4,1),(3,4,1)]),(5,[(1,2,5),(1,3,1),(3,2,1),(2,4,1),(3,5,2),(5,4,0)]),
  (4,[(1,2,0),(2,1,0),(1,3,0),(2,3,0),(3,4,0)]),(5,[(1,5,10),(1,2,1),(2,5,1),(1,3,2),(3,4,1)])]
 for gi,(n,rows) in enumerate(explicit):
  nodes,edges=synthetic(n,rows)
  for start in (0,1,n+1):
   for limit in (0,1,2,100):
    for target in (1,n,n+1):compare(nodes,edges,start,limit,goals=(target,),label=f'explicit-{gi}-{start}-{limit}-{target}')
  if rows:
   compare(nodes,edges,1,100,1,(1,),goals=(n,),denied_edges=range(1,len(rows)+1),denied_nodes=range(1,n+1),label=f'goal-bypasses-validity-{gi}')
   compare(nodes,edges,1,1,1,(1,),goals=(),label=f'external-prefix-failure-{gi}')
   compare(nodes,edges,0,100,1,(1,),goals=(n,),label=f'external-prefix-missing-start-{gi}')
 crypt_nodes,crypt_edges=crypt_graph(a.linked_reference);n=len(crypt_nodes)//56;e=len(crypt_edges)//20;crypt_cases=0
 floor_ids={}
 for i in range(n):floor_ids.setdefault(struct.unpack_from('<I',crypt_nodes,i*56+4)[0],[]).append(i+1)
 for source_floor,source in floor_ids.items():
  for target_floor,target in floor_ids.items():
   for limit in (0,1,8,10000):
    compare(crypt_nodes,crypt_edges,source[0],limit,goals=(target[-1],),label=f'Crypt-{source_floor}-{target_floor}-budget-{limit}');crypt_cases+=1
 for i in range(a.cases):
  n=rng.randrange(1,33);pairs=[(x,y) for x in range(1,n+1) for y in range(1,n+1) if x!=y];rng.shuffle(pairs);pairs=pairs[:rng.randrange(min(len(pairs),n*5)+1)]
  rng.shuffle(pairs);nodes,edges=synthetic(n,[(x,y,rng.choice((0.,.25,1.,1.,2.,8.,1024.,1e-30,1e30))) for x,y in pairs]);start=rng.randrange(n+2);limit=rng.choice((0,1,2,8,1000))
  compare(nodes,edges,start,limit,i%2,(1,) if pairs and i%2 else (),tuple(rng.sample(range(1,n+1),rng.randrange(min(n,3)+1))),tuple(j for j in range(1,len(pairs)+1) if rng.randrange(5)==0),tuple(j for j in range(1,n+1) if rng.randrange(5)==0))
  if i%100==99:print(f'Original search synthetic {i+1}/{a.cases}',flush=True)
 reference=words(0x31435253,len(records))+b''.join(records);a.reference_output.write_bytes(reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'linked_reference_sha256':hashlib.sha256(a.linked_reference.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),
  'comparisons':comparisons,'synthetic_cases':a.cases,'authored_crypt_cases':crypt_cases,'predicate_call_comparisons':events,'search_node_record_comparisons':state_count,'successful_searches':success,'failed_searches':failure,'mismatches':0,
  'original_allocations':old.allocations,'original_deallocations':old.frees,'original_import_calls':old.c.import_calls,'native_import_calls':new.c.import_calls,'behavior_snapshots':labels,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)}
 probe_digest=0xcbf29ce484222325
 for byte in b''.join(probes):probe_digest=((probe_digest^byte)*0x100000001b3)&0xffffffffffffffff
 report['crypt_route_probe']={'floor_pairs':len(probes),'successful':sum(struct.unpack_from('<I',p,8)[0]!=0 for p in probes),'segments':sum(struct.unpack_from('<I',p,28)[0] for p in probes),'state_fnv1a64':f'{probe_digest:016x}'}
 a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='behavior_snapshots'}),flush=True)
if __name__=='__main__':main()
