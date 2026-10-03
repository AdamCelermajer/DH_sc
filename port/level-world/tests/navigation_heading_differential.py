"""Original LookTowards/SetHeadingDirection versus native ARM64.

All original heading arithmetic and quadrant/normalization branches execute.
Imported IEEE and atan/sqrt services are modeled consistently on both CPUs;
device libm accuracy is a separate check. Heading state and input are fixtures.
This does not implement UpdatePath, velocity/root motion or the physics step.
"""
import argparse,hashlib,json,math,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_S0
from navigation_differential import ROOT,Cpu
from navigation_search_differential import words
from aggro_differential import float_bits
from combat_result_differential import floating

class HeadingCpu(Cpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='atanf':
   raw=uc.reg_read(UC_ARM64_REG_S0) if self.arm64 else self.reg(0)
   result=float_bits(math.atan(floating(raw)))
   if self.arm64:uc.reg_write(UC_ARM64_REG_S0,result)
   else:self.put(0,result)
   self.import_calls['atanf']=self.import_calls.get('atanf',0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)

def main():
 p=argparse.ArgumentParser()
 for name in ('engine','library','report','reference-output'):p.add_argument('--'+name,type=Path,required=True)
 p.add_argument('--cases',type=int,default=4096);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/navigation-heading/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=HeadingCpu(a.engine,False,manifest);new=HeadingCpu(a.library,True,{'functions':[]});game=old.data+0x1000;input_old=old.data+0x2000;state=new.data+0x1000;input_new=new.data+0x2000
 old.uc.mem_write(game,bytes(0x400));called=0
 def observe(uc,address,size,unused):
  nonlocal called
  if address==0x393b1c:called+=1
 old.uc.hook_add(UC_HOOK_CODE,observe,begin=0x393b1c,end=0x393b1c)
 records=[];totals={'look':0,'set_heading':0,'self_alias':0,'look_calls':0,'active':0,'normalized':0};snapshots=[]
 def compare(op,initial,direction,rotate=1,alias=0,label=None):
  nonlocal called
  old.uc.mem_write(game+0x1b8,initial[:12]);old.uc.mem_write(game+0x178,initial[12:16]);old.uc.mem_write(game+0x1b5,bytes((struct.unpack_from('<I',initial,16)[0],)));old.uc.mem_write(input_old,direction)
  new.uc.mem_write(state,initial);new.uc.mem_write(input_new,direction);called=0
  source_old=game+0x1b8 if alias else input_old;source_new=state if alias else input_new
  old.invoke(0x393b1c if op==0 else 0x393be8,[game,source_old] if op==0 else [game,source_old,rotate])
  assert new.invoke('dh2_nav_look_towards' if op==0 else 'dh2_nav_set_heading',[state+12,source_new] if op==0 else [state,source_new,rotate])==0
  expected=bytes(old.uc.mem_read(game+0x1b8,12))+bytes(old.uc.mem_read(game+0x178,4))+words(old.uc.mem_read(game+0x1b5,1)[0],0)
  actual=bytes(new.uc.mem_read(state,24));assert expected==actual,(len(records),op,label,initial.hex(),direction.hex(),expected.hex(),actual.hex())
  chosen=initial[:12] if alias else direction;x,y,_=struct.unpack('<3f',chosen)
  totals[('look','set_heading')[op]]+=1;totals['self_alias']+=alias;totals['look_calls']+=called;totals['active']+=struct.unpack_from('<I',expected,16)[0];totals['normalized']+=int(op==1 and x*x+y*y>1)
  records.append(words(op,rotate,alias,called)+initial+direction+expected)
  if label:snapshots.append({'case':label,'output_words':list(struct.unpack('<6I',expected)),'look_calls':called})
  return expected
 def initial(direction=(13.,-17.,19.),angle=.37,active=1):return struct.pack('<4f2I',*direction,angle,active,0)
 directions=[(x,y,z) for x,y in ((0.,0.),(-0.,0.),(0.,-0.),(-0.,-0.),(1.,0.),(-1.,0.),(0.,1.),(0.,-1.),(1.,1.),(-1.,1.),(-1.,-1.),(1.,-1.)) for z in (0.,100.)]
 for x in (0.,.009999,.01,.010001,-.01,.5,1.,1.0000001192092896,1000.):directions.extend(((x,0.,17.),(0.,x,-17.)))
 for vector in directions:
  for op in (0,1):
   for rotate in ((1,) if op==0 else (0,1)):
    raw=struct.pack('<3f',*vector)
    for alias in (0,1):compare(op,initial(vector if alias else (13.,-17.,19.),angle=-0. if vector[0]==0 else .37),raw,rotate,alias,f'heading {op} rotate {rotate} alias {alias} direction {vector}')
 rng=random.Random(20261018)
 for n in range(a.cases):
  op=n%2;scale=rng.choice((.001,.01,1.,10.,10000.));vector=tuple(rng.uniform(-scale,scale) for _ in range(3));alias=rng.randrange(2)
  compare(op,initial(vector if alias else (13.,-17.,19.),rng.uniform(-10,10),rng.randrange(2)),struct.pack('<3f',*vector),rng.randrange(2),alias)
 probe=0xcbf29ce484222325;probe_counts={'directions':0,'active':0}
 for n in range(128):
  vector=(math.sin(n*math.tau/128),-math.cos(n*math.tau/128),0.)
  result=compare(1,initial(active=0),struct.pack('<3f',*vector),1,0)
  probe_counts['directions']+=1;probe_counts['active']+=struct.unpack_from('<I',result,16)[0]
  for value in words(n)+result:probe=((probe^value)*0x100000001b3)&0xffffffffffffffff
 probe_counts['state_fnv1a64']=f'{probe:016x}'
 reference=words(0x31474448,len(records))+b''.join(records);a.reference_output.write_bytes(reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':len(records),'mismatches':0,'totals':totals,'heading_probe':probe_counts,'behavior_snapshots':snapshots,'original_heading_branches_execute':True,'self_alias_semantics_compared':True,'imported_libm_modeled':True,'original_import_calls':old.import_calls,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)}
 a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='behavior_snapshots'}))
if __name__=='__main__':main()
