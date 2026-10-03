"""Original two-slot Blend/fade prefixes/normalize versus native ARM64.

Fade stops exactly before the first virtual sampling service. This proves the
weight producer, not timelines, blended poses, clone ownership or GPU output.
"""
import argparse,ctypes,hashlib,json,math,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from decor_scene_differential import Cpu,Dependencies
ROOT=Path(__file__).resolve().parents[1]
REPO=ROOT.parents[1]
class Arithmetic(Dependencies):
 def call(self,cpu,name):
  if name=='__aeabi_i2f':
   value=cpu.reg(0)&0xffffffff;value=value if value<0x80000000 else value-0x100000000;cpu.put_float(ctypes.c_float(value).value,'f')
  elif name=='__aeabi_uidivmod':
   x,y=[cpu.reg(i)&0xffffffff for i in range(2)];assert y!=0;cpu.write_reg(0,x//y);cpu.write_reg(1,x%y)
  elif name=='__aeabi_idivmod':
   x,y=[struct.unpack('<i',struct.pack('<I',cpu.reg(i)&0xffffffff))[0] for i in range(2)];assert y!=0;q=abs(x)//abs(y);q=-q if (x<0)!=(y<0) else q;cpu.write_reg(0,q&0xffffffff);cpu.write_reg(1,(x-q*y)&0xffffffff)
  else:return super().call(cpu,name)
  cpu.uc.reg_write(cpu.pc_reg,cpu.uc.reg_read(cpu.lr_reg))
def same(a,b):
 return a[:16]==b[:16] and a[20:24]==b[20:24] and all(a[i:i+4]==b[i:i+4] or (math.isnan(struct.unpack_from('<f',a,i)[0]) and math.isnan(struct.unpack_from('<f',b,i)[0])) for i in (16,24,28))
def main():
 p=argparse.ArgumentParser()
 for n in ('engine','library','report','reference-output'):p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();started=time.monotonic();manifest=json.loads((REPO/'.local-inputs/animation-blender-discovery/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];deps=Arithmetic();old=Cpu(a.engine,False,deps,manifest);new=Cpu(a.library,True,deps,{'functions':[]});s=old.data+0x1000;slots=s+0x400;weights=s+0x500;n=new.data+0x1000;records=[];counts=[0]*4;boundaries=[]
 for address in (0x36679c,0x366594):old.symbols[str(address)]=address
 def stop(uc,at,size,unused):uc.emu_stop()
 for at in (0x366df4,0x366d18):old.uc.hook_add(UC_HOOK_CODE,stop,begin=at,end=at)
 def compare(op,before,arg=0,label=None):
  old.uc.mem_write(s,bytes(0xd0));old.uc.mem_write(s+0x70,before[:24]);old.uc.mem_write(weights,before[24:]);old.uc.mem_write(s+0x28,struct.pack('<2I',slots,slots+8));old.uc.mem_write(s+0x34,struct.pack('<2I',weights,weights+8));old.write_reg(0,s);old.write_reg(1,arg&0xffffffff)
  if op in (1,3):
   old.write_reg(2,arg&0xffffffff);old.uc.reg_write(old.sp_reg,old.stack+0xe000);old.uc.reg_write(old.lr_reg,old.stop);old.uc.emu_start(0x366d90 if op==1 else 0x366cb4,old.stop,count=10000);assert old.uc.reg_read(old.pc_reg)==(0x366df4 if op==1 else 0x366d18)
  else:old.invoke(''+str(0x36679c if op==0 else 0x366594),[s,arg&0xffffffff])
  expected=bytes(old.uc.mem_read(s+0x70,24))+bytes(old.uc.mem_read(weights,8));new.uc.mem_write(n,before);assert new.invoke(('dh2_blender_begin','dh2_blender_update_weights','dh2_blender_normalize','dh2_blender_update_weights')[op],[n,arg&0xffffffff])==0;actual=bytes(new.uc.mem_read(n,32));assert same(expected,actual),(op,label,before.hex(),arg,expected.hex(),actual.hex());records.append(struct.pack('<2I',op,arg&0xffffffff)+before+expected);counts[op]+=1
  if label:boundaries.append({'label':label,'operation':op,'argument':arg,'before':list(struct.unpack('<8I',before)),'after':list(struct.unpack('<8I',expected))})
  return expected
 def state(current=0,previous=1,duration=100,remaining=100,reciprocal=.01,last=0,weights=(1.,0.)):
  return struct.pack('<2I2ifI2f',current,previous,duration,remaining,reciprocal,last,*weights)
 for duration in (-2147483648,-1,0,1,16,100,16777217,2147483647):
  for requested in (-2147483648,-1,0,1,100,2147483647):
   for current in (0,1):compare(0,state(current=current,duration=duration,reciprocal=-0.),requested,f'previous duration{duration} requested{requested}')
 for remaining in (-2147483648,-1,0,1,100,2147483647):
  for last in (0,100,0x7fffffff,0xffffffff):
   for now in (0,1,100,101,0x7fffffff,0x80000000,0xffffffff):
    for op in (1,3):compare(op,state(remaining=remaining,last=last),now,f'remaining{remaining} last{last} now{now}')
 for weights_pair in ((0.,0.),(-0.,-0.),(1.,-1.),(-1.,1.),(1.,0.),(-2.,-3.),(3e38,3e38),(1e-40,-1e-40),(1e-40,1e-40)):
  compare(2,state(weights=weights_pair),label=f'normalize{weights_pair}')
 rng=random.Random(20261003)
 for i in range(2048):
  raw=state(rng.randrange(2),rng.randrange(2),rng.randrange(-2147483648,2147483648),rng.randrange(-2147483648,2147483648),rng.choice((0.,-0.,.01,1.,-1.)),rng.getrandbits(32),(rng.uniform(-3,3),rng.uniform(-3,3)))
  op=i%4;compare(op,raw,rng.getrandbits(32))
 # Stateful transition proves outgoing duration belongs to the prior selection;
 # caller commits last_time at the documented end of each completed animator.
 raw=state(duration=0,remaining=0,last=100)
 for duration,now in ((100,100),(0,110),(200,150),(0,170),(0,400)):
  raw=compare(0,raw,duration,'stateful begin');raw=compare(1,raw,now,'stateful fade');raw=compare(2,raw,label='stateful normalize');raw=raw[:20]+struct.pack('<I',now)+raw[24:]
 rejects=0;good=state()
 for name in ('dh2_blender_begin','dh2_blender_update_weights','dh2_blender_normalize'):
  for offset,value in ((0,2),(4,2),(16,0x7fc01234),(24,0x7f800000),(28,0xff800000)):
   raw=bytearray(good);struct.pack_into('<I',raw,offset,value);new.uc.mem_write(n,bytes(raw));assert new.invoke(name,[n,16])==1;assert bytes(new.uc.mem_read(n,32))==bytes(raw);rejects+=1
  assert new.invoke(name,[0,16])==1;rejects+=1
 blob=struct.pack('<2I',0x31414c42,len(records))+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(blob);report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(blob).hexdigest(),'comparisons':len(records),'operation_counts':dict(zip(('begin','update_time_fade','normalize','animate_node_fade'),counts)),'atomic_rejection_checks':rejects,'mismatches':0,'scope':__doc__,'boundary_snapshots':boundaries,'elapsed_seconds':round(time.monotonic()-started,3)};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='boundary_snapshots'}))
if __name__=='__main__':main()
