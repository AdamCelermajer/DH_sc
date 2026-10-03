"""Original position-to-position PFWorld search versus native ARM64.

World/room collision traversal, midpoint-map lookup, endpoint choice, direct
branches, PFObject predicates, graph search, failed-pair cache and tail trimming
execute original instructions. Each original floor query delegates to a second
original-instruction selector/octree/collision oracle with authored floor inputs.
Native queries run the compiled chain. Scene ownership and allocator storage are
fixtures; this does not execute FindPath smoothing or MovePath.
"""
import argparse,hashlib,json,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from navigation_search_differential import OriginalSearch,NativeSearch,words,word
from navigation_differential import ROOT,equal
from selector_differential import OriginalSelectorOracle

def linked_state(path):
 raw=path.read_bytes();assert struct.unpack_from('<I',raw)[0]==0x314b4e4c;at=8;length=struct.unpack_from('<I',raw,at)[0];at+=4+length;steps=struct.unpack_from('<I',raw,at)[0];at+=4
 for _ in range(steps):
  a,b,length=struct.unpack_from('<3I',raw,at);at+=12;state=raw[at:at+length];at+=length
 n,e,inv,v,f,links=struct.unpack_from('<6I',state);nodes=state[40:40+n*56];edges=state[40+n*56:40+n*56+e*20];at=40+n*56+e*20+inv*56+v*8
 return nodes,edges,state[at:at+f*48]

class OriginalWorld(OriginalSearch):
 def __init__(self,engine,manifest,geometry):
  super().__init__(engine,manifest);self.geometry=geometry;self.oracles=[];collision_manifest=json.loads((ROOT/'reference/selector/original-functions.json').read_text())
  for index,rows in enumerate(geometry['triangles']):
   oracle=OriginalSelectorOracle(engine,collision_manifest);raw=b''.join(struct.pack('<9f',*(x for p in row for x in p)) for row in rows);oracle.build(raw,15);oracle.configure(None,True);oracle.prepare(raw,struct.pack('<6f',*geometry['bounds'][index]));self.oracles.append(oracle)
  self.world=self.c.data+0x8000;self.obj=self.c.data+0x9000;self.fp=[self.c.data+0x10000+i*0x200 for i in range(len(self.oracles))];self.room=[self.c.data+0x20000+i*0x200 for i in range(len(self.oracles))]
  self.c.pointer(self.nvt+4,0x51c0fc);self.c.pointer(self.evt+24,0x51c10c)
  c=self.c;got=0x52b578+word(c,0x52c330);c.pointer(got+word(c,0x52c334),c.data+0x30000);c.pointer(c.data+0x30000,0xaca126)
  self.active=False;self.queries=[]
 def hook(self,uc,address,size,unused):
  c=self.c
  if not getattr(self,'active',False):return super().hook(uc,address,size,unused)
  if address in (0x337888,0x337a88):self.ret()
  elif address==0x337ec8:self.ret(self.enabled)
  elif address==0x3140ec:uc.mem_write(c.reg(0),bytes(24));self.ret(c.reg(0))
  elif address==0x51b96c:
   index=self.fp.index(c.reg(0));point=bytes(uc.mem_read(c.reg(1),12));self.queries.append((index,point));result=self.oracles[index].execute('floor',point);hit=struct.unpack_from('<I',result)[0]
   if hit:uc.mem_write(c.reg(2),result[8:20]);uc.mem_write(c.reg(3),result[20:56])
   self.ret(hit)
  elif address==0x52be4c:self.source=c.node_ids.get(c.reg(6),0);self.target=c.node_ids.get(uc.reg_read(c.reg0+10),0)
  elif address in (0x52c158,0x52c160,0x52c250):self.kind=1
  elif address==0x52ad4c:self.algorithm=c.reg(0);self.kind=2
  elif address==0x52b3cc:
   self.stats=(uc.mem_read(self.algorithm+20,1)[0],*struct.unpack('<4I',uc.mem_read(self.algorithm+24,16)));super().hook(uc,address,size,unused)
  else:return super().hook(uc,address,size,unused)
 def prepare(self,nodes,edges,floors,traits,roots_disabled=()):
  self.active=False;super().run(nodes,edges,0,0,0,(),(),(),());c=self.c;ptrs={i:p for p,i in c.node_ids.items()};maps={}
  for ident,p in ptrs.items():
   raw=nodes[(ident-1)*56:ident*56];floor=struct.unpack_from('<I',raw,4)[0];c.uc.mem_write(p+8,raw[8:40]);c.pointer(p+40,self.fp[floor]);maps[ident]=self.allocate(32)
  self.c.uc.mem_write(self.world,bytes(160));table=self.allocate(len(self.room)*4);c.uc.mem_write(table,words(*self.room));c.uc.mem_write(self.world+8,words(table,table+len(self.room)*4,table+len(self.room)*4));c.uc.mem_write(self.world+20,struct.pack('<6f',*self.geometry['world_bounds']));c.pointer(self.world+72,self.g)
  self.cache=self.allocate(65536);c.uc.mem_write(self.world+120,words(self.cache,self.cache,self.cache+65536));self.c.uc.mem_write(self.obj,b'\xcc'*160);c.pointer(self.obj+76,self.evt);c.pointer(self.obj+80,0);c.pointer(self.obj+84,0);c.edge_ids[self.obj+76]=0;self.edge_ptrs[0]=self.obj+76
  for i,p in enumerate(self.fp):
   c.uc.mem_write(p,bytes(0x100));c.uc.mem_write(p+32,words(traits[i][1],traits[i][0]));c.uc.mem_write(p+68,struct.pack('<6f',*self.geometry['bounds'][i]));root=struct.unpack_from('<I',floors,i*48+8)[0];root=0 if i in roots_disabled else root
   ids=[ident for ident in ptrs if struct.unpack_from('<I',nodes,(ident-1)*56+4)[0]==i];header=p+144;c.uc.mem_write(header,words(0,maps.get(root,0),header,header,len(ids)))
   for ident in ids:
    parent,left,right,red=struct.unpack_from('<4I',nodes,(ident-1)*56+40);c.uc.mem_write(maps[ident],words(int(not red),maps.get(parent,header),maps.get(left,0),maps.get(right,0))+nodes[(ident-1)*56+8:(ident-1)*56+20]+words(ptrs[ident]))
   rp=self.room[i];c.uc.mem_write(rp,bytes(128));t=self.allocate(4);c.pointer(t,p);c.uc.mem_write(rp+48,words(t,t+4,t+4));c.uc.mem_write(rp+60,struct.pack('<6f',*self.geometry['bounds'][i]))
  self.active=True
 def run_route(self,source,target,limit=10000,flags=0,radius=0.,actor=1,output=1,enabled=1,prefix=()):
  c=self.c;self.enabled=enabled;self.source=self.target=self.kind=0;self.stats=(0,0,0,0,0);self.queries=[];self.algorithm=0
  start=c.data+0x31000;end=start+16;c.uc.mem_write(start,struct.pack('<3f',*source));c.uc.mem_write(end,struct.pack('<3f',*target));c.uc.mem_write(self.obj+8,struct.pack('<f',radius));c.pointer(self.obj+20,flags);self.list(self.out,prefix)
  found=c.invoke(0x52b560,[self.world,self.obj if actor else 0,start,end,limit,self.out if output else 0],budget=50000000)
  path=[];p=word(c,self.out)
  while p!=self.out:assert len(path)<=self.count+len(prefix)+1;path.append(c.edge_ids[word(c,p+8)]);p=word(c,p)
  cache=[]
  for at in range(self.cache,word(c,self.world+124),12):a,b,lim=struct.unpack('<3I',c.uc.mem_read(at,12));cache.append((c.node_ids[a],c.node_ids[b],lim))
  result=words(found,self.source,self.target,self.kind if found else 0,*self.stats,len(path))+words(*path)
  return result,bytes(c.uc.mem_read(self.obj+100,24)),words(len(cache))+b''.join(words(*row) for row in cache),tuple(self.queries)

class NativeWorld(NativeSearch):
 def __init__(self,library,geometry):
  super().__init__(library);self.geometry=geometry;self.queries=[];self.floor_ptrs={};c=self.c;c.uc.hook_add(UC_HOOK_CODE,self.hook,begin=c.symbols['dh2_selector_floor'],end=c.symbols['dh2_selector_floor']);self.make_geometry()
 def hook(self,uc,address,size,unused):self.queries.append((self.floor_ptrs[self.c.reg(1)],bytes(uc.mem_read(self.c.reg(2),12))))
 def make_geometry(self):
  c=self.c;d=c.data;self.cw=d+0x700000;self.rooms=d+0x701000;self.floors=d+0x702000;self.selectors=d+0x703000;self.traits=d+0x704000;self.fgs=d+0x705000;self.rw=d+0x706000;self.request=d+0x706100;self.result=d+0x706200;self.workspace=d+0x706300;self.obj=d+0x706400;self.failed=d+0x710000
  c.uc.mem_write(self.cw,struct.pack('<2Q2I6f',self.rooms,self.floors,len(self.geometry['triangles']),len(self.geometry['triangles']),*self.geometry['world_bounds']))
  for i,rows in enumerate(self.geometry['triangles']):
   base=d+0x800000+i*0x40000;raw=b''.join(struct.pack('<9f',*(x for p in row for x in p)) for row in rows);c.uc.mem_write(base,raw);tree=base+0x8000;workspace=base+0x8100
   c.uc.mem_write(tree,struct.pack('<4Q9I',base+0x10000,base+0x20000,base+0x28000,base,len(rows),0,0,2048,8192,2048,15,0,0));assert c.invoke('dh2_octree_build',[tree,base,len(rows),15],budget=20000000)==0
   c.uc.mem_write(workspace,struct.pack('<2Q2I',base+0x2a000,base+0x30000,len(rows),0));selector=self.selectors+i*56;self.floor_ptrs[selector]=i;c.uc.mem_write(selector,struct.pack('<2Q2I6fQ',tree,0,1,0,*self.geometry['bounds'][i],workspace))
   ids=self.rooms+0x400+i*4;c.uc.mem_write(ids,words(i));c.uc.mem_write(self.rooms+i*40,struct.pack('<6fQ2I',*self.geometry['bounds'][i],ids,1,0));c.uc.mem_write(self.floors+i*16,struct.pack('<Q2I',selector,0,1))
 def prepare(self,nodes,edges,floors,traits,roots_disabled=()):
  super().run(nodes,edges,0,0,0,(),(),(),());c=self.c;d=c.data;n=len(nodes)//56;e=len(edges)//20;self.n=n
  c.uc.mem_write(self.fgs,floors)
  for i,pair in enumerate(traits):c.uc.mem_write(self.traits+i*8,words(*pair));c.uc.mem_write(self.floors+i*16+8,words(*pair))
  for i in roots_disabled:c.uc.mem_write(self.fgs+i*48+8,words(0))
  c.uc.mem_write(self.rw,struct.pack('<3Q2IQ2I',d+0x1100,self.fgs,self.traits,len(traits),0,self.failed,0,4096));c.uc.mem_write(self.workspace,struct.pack('<2Q2I',d+0x1500,d+0x620000,n,0));c.uc.mem_write(self.obj,b'\xcc'*32)
 def run_route(self,source,target,limit=10000,flags=0,radius=0.,actor=1,output=1,enabled=1,prefix=()):
  c=self.c;d=c.data;self.queries=[];c.uc.mem_write(self.obj,words(flags)+struct.pack('<f',radius));c.uc.mem_write(self.request,struct.pack('<3Q6f4I',self.rw,self.cw,self.obj if actor else 0,*source,*target,limit,enabled,output,0))
  c.uc.mem_write(self.result,words(0,0,0,0)+struct.pack('<6IQ2I',0,0,0,0,0,len(prefix),d+0x600000,self.n+len(prefix)+1,0))
  if prefix:c.uc.mem_write(d+0x600000,words(*prefix))
  status=c.invoke('dh2_nav_route',[self.result,self.request,self.workspace],budget=50000000);assert status==0,('native route rejected',status,source,target)
  result=bytes(c.uc.mem_read(self.result,40));count=struct.unpack_from('<I',result,36)[0];result+=bytes(c.uc.mem_read(d+0x600000,count*4)) if count else b''
  cache_count=word(c,self.rw+40);cache=words(cache_count)+(bytes(c.uc.mem_read(self.failed,cache_count*12)) if cache_count else b'')
  return result,bytes(c.uc.mem_read(self.obj+8,24)),cache,tuple(self.queries)

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--floor',type=Path,required=True);p.add_argument('--linked-reference',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--cases',type=int,default=320);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/navigation-world/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];data=json.loads(a.floor.read_text(encoding='utf-8-sig'));triangles=[[] for _ in range(8)]
 for row in data['floor']:
  values=struct.unpack('<9f',struct.pack('<9f',*(x for p in row['corners'] for x in p)));triangles[row['room']].append([list(values[i:i+3]) for i in range(0,9,3)])
 bounds=[data['floor_bounds'][str(i)] for i in range(8)];geometry={'triangles':triangles,'bounds':bounds,'world_bounds':[min(b[k] for b in bounds) for k in range(3)]+[max(b[k+3] for b in bounds) for k in range(3)]}
 nodes,edges,floors=linked_state(a.linked_reference);old=OriginalWorld(a.engine,manifest,geometry);new=NativeWorld(a.library,geometry);traits=[(0,1)]*8;rng=random.Random(20261009);records=[];snapshots=[];probes=[];totals={'direct':0,'graph':0,'failed':0,'floor_queries':0,'cache_rejections':0};reset_pending=True;current_traits=traits;current_disabled=()
 def reset(traits=traits,disabled=()):
  nonlocal reset_pending,current_traits,current_disabled
  old.prepare(nodes,edges,floors,traits,disabled);new.prepare(nodes,edges,floors,traits,disabled);reset_pending=True;current_traits=traits;current_disabled=disabled
 def compare(source,target,limit=10000,flags=0,radius=0.,actor=1,output=1,enabled=1,prefix=(),label=None):
  nonlocal reset_pending
  args=(source,target,limit,flags,radius,actor,output,enabled,prefix);expected=old.run_route(*args);actual=new.run_route(*args)
  for kind,(left,right) in enumerate(zip(expected,actual)):assert equal(left,right) if kind<3 else left==right,(len(records),label,kind,left,right)
  fields=struct.unpack_from('<10I',expected[0]);totals[('failed','direct','graph')[fields[3]]]+=1;totals['floor_queries']+=len(expected[3]);totals['cache_rejections']+=not fields[0] and fields[1]>0 and fields[2]>0 and not old.algorithm
  inputs=words(int(reset_pending),sum(1<<i for i in current_disabled),*(x for pair in current_traits for x in pair))+struct.pack('<6f2If4I',*source,*target,limit,flags,radius,int(actor),int(output),int(enabled),len(prefix))+words(*prefix)
  records.append(inputs+words(len(expected[0]))+expected[0]+expected[1]+words(len(expected[2]))+expected[2]+words(len(expected[3]))+b''.join(words(i)+point for i,point in expected[3]));reset_pending=False
  if label:snapshots.append({'case':label,'fields':fields,'path':list(struct.unpack_from('<'+'I'*fields[9],expected[0],40))})
  if label and label.startswith('Crypt-'):probes.append(words(*(int(x) for x in label.split('-')[1:]))+expected[0])
 def center(row):return [sum(x)/3 for x in zip(*row)]
 points=[[center(row) for row in rows] for rows in triangles]
 reset()
 for source_floor in range(8):
  for target_floor in range(8):compare(points[source_floor][0],points[target_floor][-1],label=f'Crypt-{source_floor}-{target_floor}')
 for i in range(a.cases):
  reset();source=rng.choice(rng.choice(points));target=rng.choice(rng.choice(points));prefix=(1,) if i%11==0 else ()
  compare(source,target,rng.choice((0,1,8,10000)),rng.choice((0,1,2,0xffffffff)),rng.choice((0.,36.,100.,500.,1e10)),i%7!=0,i%9!=0,i%13!=0,prefix)
  if i%3==0:compare(source,target,0,label='repeated failed-pair cache')
  if i%80==79:print(f'World routes {i+1}/{a.cases}',flush=True)
 for i in range(8):
  reset(disabled=(i,));compare(points[i][0],points[(i+1)%8][-1],label=f'missing-midpoint-tree-{i}');compare(points[i][0],points[i][0],label=f'direct-with-missing-tree-{i}')
  varied=list(traits);varied[i]=(0x03000000,1);reset(varied);compare(points[i][0],points[i][0],label=f'special-floor-collision-filter-{i}')
  varied=list(traits);varied[i]=(0,0);reset(varied);compare(points[(i+1)%8][0],points[i][-1],label=f'disabled-floor-node-validity-{i}')
 reset();compare([1e9,1e9,1e9],points[0][0],label='source outside world');compare(points[0][0],[1e9,1e9,1e9],label='target outside world')
 geometry_bytes=words(len(triangles))+b''.join(words(len(rows))+b''.join(struct.pack('<9f',*(x for p in row for x in p)) for row in rows)+struct.pack('<6f',*bounds[i]) for i,rows in enumerate(triangles))
 reference=words(0x32545257,len(records),len(nodes)//56,len(edges)//20,len(floors)//48)+nodes+edges+floors+geometry_bytes+b''.join(records);a.reference_output.write_bytes(reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'linked_reference_sha256':hashlib.sha256(a.linked_reference.read_bytes()).hexdigest(),'floor_source_sha256':hashlib.sha256(a.floor.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':len(records),'totals':totals,'mismatches':0,'behavior_snapshots':snapshots,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)}
 probe_digest=0xcbf29ce484222325
 for byte in b''.join(probes):probe_digest=((probe_digest^byte)*0x100000001b3)&0xffffffffffffffff
 report['crypt_world_route_probe']={'floor_pairs':len(probes),'successful':sum(struct.unpack_from('<I',p,8)[0]!=0 for p in probes),'direct':sum(struct.unpack_from('<I',p,20)[0]==1 for p in probes),'graph':sum(struct.unpack_from('<I',p,20)[0]==2 for p in probes),'segments':sum(struct.unpack_from('<I',p,44)[0] for p in probes),'state_fnv1a64':f'{probe_digest:016x}'}
 a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='behavior_snapshots'}),flush=True)
if __name__=='__main__':main()
