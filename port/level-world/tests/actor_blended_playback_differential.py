"""Actual two-slot scene/time/root/typed instructions vs native frame captures.

The native capture supplies compiled real-asset target samples and root points
as explicit synchronous slot services. Timeline/key/event production is audited
separately, not reimplemented here. Original full366cb4/366d90,65e350,normalize,
typed wrappers/setters,366888,364444 and36440c execute actual instructions.
"""
import argparse,hashlib,json,math,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from navigation_motion_differential import MotionCpu
from navigation_differential import ROOT,equal
from aggro_differential import float_bits
from combat_result_differential import floating

class CompositionCpu(MotionCpu):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='sinf':
   value=floating(self.reg(0));self.put(0,float_bits(math.sin(value) if math.isfinite(value) else math.nan))
   self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:return super().external(uc,address,size,unused)

def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
class Reader:
 def __init__(self,b):self.b=b;self.p=0
 def take(self,n):r=self.b[self.p:self.p+n];assert len(r)==n;self.p+=n;return r
 def word(self):return struct.unpack('<I',self.take(4))[0]

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--fixtures',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/animation-blend-composition/original-functions.json').read_text())
 assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 raw=a.fixtures.read_bytes();r=Reader(raw);assert r.take(4)==b'BPF1';count=r.word();targets=r.word();node_count=r.word();root=struct.unpack('<i',r.take(4))[0]
 descriptors=[struct.unpack('<3I',r.take(12)) for _ in range(targets)];sizes=[x[2]*8 for x in descriptors];pose_size=node_count*40
 c=CompositionCpu(a.engine,False,manifest);b=c.data+0x1000;bvt=b+0x200;slots=[b+0x400,b+0x600];svts=[b+0x800,b+0xa00];timelines=[b+0xc00,b+0xd00]
 sv=b+0x1000;w=b+0x1100;tv=b+0x1200;inf=b+0x1800;bv=b+0x2000
 nvt=b+0x2400;nodes=[b+0x3000+i*0x200 for i in range(node_count)]
 tracks=[b+0x10000+i*0x100 for i in range(targets)];trackvts=[b+0x14000+i*0x100 for i in range(targets)];buffers=[b+0x18000+i*0x100 for i in range(targets)]
 service=b+0x20000;calls=[];fixture={};totals={'ordered_slot_samples':0,'ordered_root_samples':0,'ordered_target_setters':0,'completion_callbacks':0,'normal_scene_records':0,'time_only_records':0};gold=[]
 def put(at,value):c.uc.mem_write(at,words(value))
 def word(at):return struct.unpack('<I',c.uc.mem_read(at,4))[0]
 def ret(value=0):c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def hook(uc,address,size,unused):
  if address in (service,service+4,service+8,service+12):
   slot=(address-service)//4%2;time_only=address>=service+8;calls.append(('time' if time_only else 'pose',slot));totals['ordered_slot_samples']+=1
   if not time_only:
    for i,(_,node,components) in enumerate(descriptors):
     if fixture['enabled'][i] and node!=0xffffffff:
      source=fixture['sampled'][i][slot*components*4:(slot+1)*components*4];uc.mem_write(buffers[i]+slot*components*4,source)
   ret()
  elif address==service+16:
   slot=slots.index(c.reg(0));calls.append(('root',slot));totals['ordered_root_samples']+=1
   uc.mem_write(c.reg(3),fixture['root_points'][slot]);ret()
  elif address==service+20:ret(fixture['enabled'][c.reg(1)])
  elif address==service+24:ret(tracks[c.reg(1)])
  elif address==service+28:ret(11)
  elif address==service+32:
   calls.append(('completion',));totals['completion_callbacks']+=1
   assert c.reg(0)==timelines[word(b+0x70)]
   assert uc.mem_read(b+0x88+0x30,1)==b'\1' # source clears AFTER callback
   assert word(b+0x84)==struct.unpack_from('<I',fixture['before_blend'],20)[0]
   ret()
  elif address in (0x59712c,0x5970c4,0x5970f4):
   calls.append(('target',nodes.index(c.reg(0)),address));totals['ordered_target_setters']+=1
 c.uc.hook_add(UC_HOOK_CODE,hook)
 c.pointer(b,bvt);c.pointer(bvt+0x50,0x65e350);c.pointer(bvt+0x80,service+20)
 c.pointer(b+0x28,sv);c.pointer(b+0x2c,sv+8);c.pointer(b+0x34,w);c.pointer(b+0x38,w+8)
 c.pointer(b+0x58,tv);c.pointer(b+0x5c,tv+4*targets);c.pointer(b+0x64,inf);c.pointer(b+0x4c,bv)
 c.pointer(b+0x88+0x3c,b);c.pointer(b+0x88+0xc,root);c.pointer(b+0x88+0x34,service+32)
 for i in range(2):
  c.pointer(sv+4*i,slots[i]);c.pointer(slots[i],svts[i]);c.pointer(slots[i]+8,timelines[i])
  for offset,address in ((0x4c,service+4*i),(0x14,service+8+4*i),(0x44,0x599870),(0x58,service+24),(0x24,service+28),(0x7c,service+16)):c.pointer(svts[i]+offset,address)
 c.pointer(nvt+0xa4,0x59712c);c.pointer(nvt+0x94,0x5970c4);c.pointer(nvt+0x9c,0x5970f4)
 for node in nodes:c.pointer(node,nvt)
 for i,(kind,node,components) in enumerate(descriptors):
  c.pointer(tracks[i],trackvts[i]);c.pointer(trackvts[i]+0x18,{1:0x62859c,5:0x6209b4,10:0x62d72c}[kind]);c.pointer(inf+4*i,0);c.pointer(bv+4*i,buffers[i]);c.pointer(tv+4*i,0 if node==0xffffffff else nodes[node])
 for record in range(count):
  now,mode=r.word(),r.word();before_blend=r.take(32);before_history=[r.take(28) for _ in range(2)];before_delta=r.take(12);before_values=[r.take(n) for n in sizes];before_pose=r.take(pose_size);enabled=[r.word() for _ in range(targets)]
  sampled=[r.take(n) for n in sizes];root_points=[r.take(12) for _ in range(2)];expected_blend=r.take(32);expected_history=[r.take(28) for _ in range(2)];expected_delta=r.take(12);expected_pose=r.take(pose_size)
  fixture={'enabled':enabled,'sampled':sampled,'root_points':root_points,'before_blend':before_blend};calls.clear()
  c.uc.mem_write(b+0x70,before_blend[:20]);c.uc.mem_write(b+0x84,before_blend[20:24]);c.uc.mem_write(w,before_blend[24:])
  c.uc.mem_write(b+0x88+0x24,before_delta);c.uc.mem_write(b+0x88+0x30,b'\1')
  for i in range(2):c.uc.mem_write(slots[i]+0x58+0x14,before_history[i])
  for i in range(targets):c.uc.mem_write(buffers[i],before_values[i])
  for i,node in enumerate(nodes):c.uc.mem_write(node+0xac,before_pose[i*40:(i+1)*40]);put(node+0x11c,0)
  c.invoke(0x366d90 if mode else 0x366cb4,[b,now] if mode else [b,nodes[0],now],budget=20000000)
  actual_blend=bytes(c.uc.mem_read(b+0x70,20))+bytes(c.uc.mem_read(b+0x84,4))+bytes(c.uc.mem_read(w,8))
  actual_history=[bytes(c.uc.mem_read(slot+0x58+0x14,28)) for slot in slots];actual_delta=bytes(c.uc.mem_read(b+0x88+0x24,12));actual_pose=b''.join(bytes(c.uc.mem_read(node+0xac,40)) for node in nodes)
  assert equal(actual_blend,expected_blend),(record,'fade',actual_blend.hex(),expected_blend.hex())
  assert all(equal(x,y) for x,y in zip(actual_history,expected_history)),(record,'root histories')
  assert equal(actual_delta,expected_delta),(record,'aggregate delta',actual_delta.hex(),expected_delta.hex())
  assert equal(actual_pose,expected_pose),(record,'typed pose')
  assert all(equal(bytes(c.uc.mem_read(buffers[i],sizes[i])),sampled[i]) for i in range(targets)),(record,'slot buffers')
  kinds=[x[0] for x in calls];assert kinds[-1]=='completion';assert c.uc.mem_read(b+0x88+0x30,1)==b'\0'
  if mode:assert 'target' not in kinds and 'root' not in kinds;totals['time_only_records']+=1
  else:
   assert [x[1] for x in calls if x[0]=='root']==[0,1]
   if 'target' in kinds:assert kinds.index('target')>max((i for i,k in enumerate(kinds) if k=='pose'),default=-1)
   assert kinds.index('root')>max((i for i,k in enumerate(kinds) if k=='target'),default=-1);totals['normal_scene_records']+=1
  gold.append(actual_blend+b''.join(actual_history)+actual_delta+actual_pose)
  if record%100==99:print(f'Original two-slot composition {record+1}/{count}',flush=True)
 assert r.p==len(raw)
 blob=b'BPG1'+words(count,node_count)+b''.join(gold);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(blob)
 report={'validation':'PASS','original_sha256':manifest['original_sha256'],'fixture_sha256':hashlib.sha256(raw).hexdigest(),'corpus_sha256':hashlib.sha256(blob).hexdigest(),'composition_records':count,'compiled_targets':targets,'prince_nodes':node_count,**totals,'mismatches':0,'elapsed_seconds':round(time.monotonic()-started,3),'scope':'Actual full original two-slot rendered/time-only coordinators, fade, typed node setters, root aggregate/history and pending-clear callback instructions compared against native real-asset frame captures. Compiled raw target/root samples and timeline/event advancement are explicit synchronous services here and have separate audits. Completion checking is deliberately pending for ordering fixtures. No original parser, blended GPU/full-application parity claim.','import_calls':c.import_calls}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('scope','import_calls')}))
if __name__=='__main__':main()
