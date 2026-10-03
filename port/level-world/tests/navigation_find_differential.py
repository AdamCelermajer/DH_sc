"""Full original FindPath, ordinary world search and smoothing versus ARM64.

The original drop/search/smooth/waypoint sequence executes with previous paths,
failed caches and actual Crypt floor queries. Profiling/string/debug timing
services and allocator ownership are fixtures. Original floor queries delegate
to independent original selector/octree/collision execution. Full PFObject
initialization, cache invalidation and position/controller movement are pending.
"""
import argparse,hashlib,json,random,struct,time
from pathlib import Path
from navigation_world_differential import OriginalWorld,NativeWorld,linked_state
from navigation_path_differential import OriginalPath,PathCpu,words,word
from unicorn import UC_HOOK_CODE
from navigation_differential import ROOT,equal

class OriginalFind(OriginalWorld):
 def __init__(self,*args):
  super().__init__(*args);self.c.pointer(self.evt+32,0x51c114);self.c.pointer(self.evt+36,0x51b844);self.c.pointer(self.evt+40,0x51b85c)
 def hook(self,uc,address,size,unused):
  if getattr(self,'active',False):
   if address in (0x3136b4,0x3136b8):self.ret();return
   if address==0x310570:self.ret(self.allocate(self.c.reg(0)));return
   if address==0x52d538:
    self.route_path=[];p=word(self.c,self.obj+56)
    while p!=self.obj+56:self.route_path.append(self.c.edge_ids[word(self.c,p+8)]);p=word(self.c,p)
  super().hook(uc,address,size,unused)
 def prepare(self,*args):
  super().prepare(*args);c=self.c;c.pointer(self.obj+76,0x96c810);c.uc.mem_write(self.obj+80,words(0,0,0x3f800000,0,0));c.uc.mem_write(self.obj+100,bytes(24));c.uc.mem_write(self.obj+24,bytes(12));c.uc.mem_write(self.obj+64,bytes(12));c.uc.mem_write(self.obj+128,bytes(12));c.pointer(self.obj+124,0);self.list(self.obj+56,())
 def run_find(self,source,target,limit=10000,flags=0,radius=0.,enabled=1):
  c=self.c;self.enabled=enabled;self.source=self.target=self.kind=0;self.stats=(0,0,0,0,0);self.queries=[];self.algorithm=0;self.route_path=[]
  c.uc.mem_write(self.obj+24,struct.pack('<3f',*source));c.uc.mem_write(self.obj+8,struct.pack('<f',radius));c.pointer(self.obj+20,flags);to=c.data+0x31000;c.uc.mem_write(to,struct.pack('<3f',*target));found=c.invoke(0x52db48,[self.world,self.obj,to,limit],budget=50000000)
  cache=[]
  for at in range(self.cache,word(c,self.world+124),12):a,b,lim=struct.unpack('<3I',c.uc.mem_read(at,12));cache.append((c.node_ids[a],c.node_ids[b],lim))
  return words(found,self.source,self.target,self.kind if found else 0,*self.stats,len(self.route_path))+words(*self.route_path),OriginalPath.snapshot(self),words(len(cache))+b''.join(words(*r) for r in cache),tuple(self.queries)

class NativeFind(NativeWorld):
 def __init__(self,library,geometry):
  self.c=PathCpu(library,True,{'functions':[]});self.geometry=geometry;self.queries=[];self.floor_ptrs={};c=self.c;c.uc.hook_add(UC_HOOK_CODE,self.hook,begin=c.symbols['dh2_selector_floor'],end=c.symbols['dh2_selector_floor']);self.make_geometry();d=c.data;self.po=d+0x707000;self.fr=d+0x707100;self.segments=d+0x720000
 def prepare(self,*args):
  super().prepare(*args);c=self.c;c.uc.mem_write(self.po,bytes(c.uc.mem_read(self.obj,32))+bytes(40)+struct.pack('<Q4I',self.segments,0,self.n+1,0,0));c.uc.mem_write(self.po+8,bytes(24))
 def run_find(self,source,target,limit=10000,flags=0,radius=0.,enabled=1):
  c=self.c;d=c.data;self.queries=[];c.uc.mem_write(self.po,words(flags)+struct.pack('<f',radius));c.uc.mem_write(self.po+32,struct.pack('<3f',*source));c.uc.mem_write(self.fr,struct.pack('<5Q3f3I',self.rw,self.cw,self.po,self.result,self.workspace,*target,limit,enabled,0));c.uc.mem_write(self.result,words(0,0,0,0)+struct.pack('<6IQ2I',0,0,0,0,0,0,d+0x600000,self.n+1,0))
  status=c.invoke('dh2_nav_find_path',[self.fr],budget=50000000);assert status==0,('find path rejected',status)
  result=bytes(c.uc.mem_read(self.result,40));count=struct.unpack_from('<I',result,36)[0];result+=bytes(c.uc.mem_read(d+0x600000,count*4)) if count else b''
  count=word(c,self.po+80);owned=word(c,self.po+88);snapshot=bytes(c.uc.mem_read(self.po,68))+words(count,owned)+(bytes(c.uc.mem_read(self.segments,count*48)) if count else b'');cache_count=word(c,self.rw+40)
  return result,snapshot,words(cache_count)+(bytes(c.uc.mem_read(self.failed,cache_count*12)) if cache_count else b''),tuple(self.queries)

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--floor',type=Path,required=True);p.add_argument('--linked-reference',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--cases',type=int,default=200);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/navigation-path/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];data=json.loads(a.floor.read_text(encoding='utf-8-sig'));triangles=[[] for _ in range(8)]
 for row in data['floor']:
  v=struct.unpack('<9f',struct.pack('<9f',*(x for p in row['corners'] for x in p)));triangles[row['room']].append([list(v[i:i+3]) for i in range(0,9,3)])
 bounds=[data['floor_bounds'][str(i)] for i in range(8)];geometry={'triangles':triangles,'bounds':bounds,'world_bounds':[min(b[k] for b in bounds) for k in range(3)]+[max(b[k+3] for b in bounds) for k in range(3)]};nodes,edges,floors=linked_state(a.linked_reference);old=OriginalFind(a.engine,manifest,geometry);new=NativeFind(a.library,geometry);traits=[(0,1)]*8;rng=random.Random(20261011);records=[];snapshots=[];totals={'direct':0,'graph':0,'failed':0,'floor_queries':0,'cached_failures':0,'replaced_paths':0};reset_pending=True;current_traits=traits;current_disabled=();sessions=0
 def reset(traits=traits,disabled=()):
  nonlocal reset_pending,current_traits,current_disabled,sessions
  old.prepare(nodes,edges,floors,traits,disabled);new.prepare(nodes,edges,floors,traits,disabled);reset_pending=True;current_traits=traits;current_disabled=disabled;sessions+=1
 def compare(source,target,limit=10000,flags=0,radius=0.,enabled=1,label=None):
  nonlocal reset_pending
  replaced=word(old.c,old.obj+56)!=old.obj+56;expected=old.run_find(source,target,limit,flags,radius,enabled);actual=new.run_find(source,target,limit,flags,radius,enabled)
  for i,(left,right) in enumerate(zip(expected,actual)):assert equal(left,right) if i<3 else left==right,(len(records),label,i,left,right)
  fields=struct.unpack_from('<10I',expected[0]);totals[('failed','direct','graph')[fields[3]]]+=1;totals['floor_queries']+=len(expected[3]);totals['cached_failures']+=not fields[0] and fields[1]>0 and fields[2]>0 and not old.algorithm;totals['replaced_paths']+=replaced
  inputs=words(int(reset_pending),sum(1<<i for i in current_disabled),*(x for pair in current_traits for x in pair))+struct.pack('<6f2IfI',*source,*target,limit,flags,radius,int(enabled));reset_pending=False
  records.append(inputs+words(len(expected[0]))+expected[0]+words(len(expected[1]))+expected[1]+words(len(expected[2]))+expected[2]+words(len(expected[3]))+b''.join(words(i)+point for i,point in expected[3]))
  if label:snapshots.append({'case':label,'fields':fields,'owned':struct.unpack_from('<I',expected[1],72)[0],'segments':fields[9]})
  if label and label.startswith('Crypt-'):probes.append(words(*(int(x) for x in label.split('-')[1:]))+expected[0]+expected[1])
 def center(row):return [sum(x)/3 for x in zip(*row)]
 points=[[center(row) for row in rows] for rows in triangles];probes=[];reset()
 for source_floor in range(8):
  for target_floor in range(8):compare(points[source_floor][0],points[target_floor][-1],label=f'Crypt-{source_floor}-{target_floor}')
 for i in range(a.cases):
  if i%10==0:reset()
  source=rng.choice(rng.choice(points));target=rng.choice(rng.choice(points));limit=rng.choice((0,1,8,10000));flags=rng.choice((0,1,2,0xffffffff));radius=rng.choice((0.,1.,36.,100.,500.));enabled=i%13!=0
  compare(source,target,limit,flags,radius,enabled)
  if i%3==0:compare(source,target,limit,flags,radius,enabled,label='repeated cached/replaced request')
  if i%50==49:print(f'FindPath requests {i+1}/{a.cases}',flush=True)
 for i in range(8):
  reset(disabled=(i,));compare(points[i][0],points[i][0],label=f'direct-with-missing-tree-{i}');compare(points[i][0],points[(i+1)%8][-1],label=f'missing-midpoint-tree-{i}')
  varied=list(traits);varied[i]=(0x03000000,1);reset(varied);compare(points[i][0],points[i][0],label=f'special-floor-filter-{i}')
  varied=list(traits);varied[i]=(2,1);reset(varied);compare(points[(i+1)%8][0],points[i][-1],flags=0,label=f'capability-floor-{i}')
 reset();compare(points[0][0],points[0][0],label='direct path');compare([1e9]*3,points[0][0],label='missing source replaces path');compare(points[0][0],[1e9]*3,label='missing target')
 geometry_bytes=words(len(triangles))+b''.join(words(len(rows))+b''.join(struct.pack('<9f',*(x for p in row for x in p)) for row in rows)+struct.pack('<6f',*bounds[i]) for i,rows in enumerate(triangles));reference=words(0x31564e46,len(records),len(nodes)//56,len(edges)//20,len(floors)//48)+nodes+edges+floors+geometry_bytes+b''.join(records);a.reference_output.write_bytes(reference)
 probe_digest=0xcbf29ce484222325
 for byte in b''.join(probes):probe_digest=((probe_digest^byte)*0x100000001b3)&0xffffffffffffffff
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'linked_reference_sha256':hashlib.sha256(a.linked_reference.read_bytes()).hexdigest(),'floor_source_sha256':hashlib.sha256(a.floor.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':len(records),'sessions':sessions,'totals':totals,'mismatches':0,'behavior_snapshots':snapshots,'crypt_findpath_probe':{'floor_pairs':len(probes),'successful':64,'owned':64,'segments':sum(s['segments'] for s in snapshots if s['case'].startswith('Crypt-')),'state_fnv1a64':f'{probe_digest:016x}'},'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)};a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='behavior_snapshots'}),flush=True)
if __name__=='__main__':main()
