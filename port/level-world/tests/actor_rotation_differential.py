"""Complete original UpdateRotation coordinator versus native ARM64.
Actual Character rotation-rate/visual policy and Application dt getters execute.
VisualObject::SyncRotation is an observer of call order and final Euler angles;
its quaternion/scene writes are separately reconstructed visual services.
"""
import argparse,hashlib,importlib.util,json,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
spec=importlib.util.spec_from_file_location('genuine_backend_cpu',REPO/'port/physics-backend/tests/differential.py');b=importlib.util.module_from_spec(spec);spec.loader.exec_module(b);Cpu=b.Cpu
def words(*v):return struct.pack('<'+'I'*len(v),*v)
def fl(x):return b.floating(b.float_bits(x))
def main():
 p=argparse.ArgumentParser()
 for n in ('engine','library','report','reference-output'):p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();started=time.monotonic();m=json.loads((ROOT/'reference/actor-rotation/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==m['original_sha256'];old=Cpu(a.engine,False,m);new=Cpu(a.library,True,{'functions':[]});char=old.data+0x1000;vt=old.data+0x4000;app=old.data+0x5000;visual=old.data+0x6000;state=new.data+0x1000;policy=state+128;result=policy+128
 old.uc.mem_write(char,bytes(0x2400));old.pointer(char,vt);old.pointer(vt+0xac,0x3a372c);old.pointer(vt+0x70,0x3a2e68)
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 got=(0x393740+word(0x393898))&0xffffffff;old.pointer(got+word(0x39389c),app);events=[]
 def hook(uc,at,size,unused):
  if at==0x472948:
   assert old.reg(0)==visual;events.append(bytes(uc.mem_read(char+0x16c,12)));old.put(0,0);uc.reg_write(old.pc,uc.reg_read(old.lr))
 old.uc.hook_add(UC_HOOK_CODE,hook,begin=0x472948,end=0x472948);rows=[];rng=random.Random(20261024);totals={'snap_sentinel':0,'visual_sync':0,'increment_positive':0,'increment_negative':0,'snap_to_target':0,'zero_dt':0,'dt_getter_calls':0};samples=[]
 def compare(f,raw,dt,initial,heading,present,turn,label=None):
  old.uc.mem_write(char+0x520,words(f));old.uc.mem_write(char+0x560+0xb54,struct.pack('<i',raw));rate=old.invoke(0x3a372c,[char]);visual_policy=old.invoke(0x3a2e68,[char]);before=struct.pack('<4f2I',.2,-.3,initial,heading,turn,0);old.uc.mem_write(char+0x16c,before[:16]);old.uc.mem_write(char+0x17c,words(turn));old.pointer(char+0x2d8,visual if present else 0);old.uc.mem_write(app+0x8c,words(dt));events.clear();old.invoke(0x393710,[char]);expected=bytes(old.uc.mem_read(char+0x16c,16))+bytes(old.uc.mem_read(char+0x17c,4))+bytes(4);parameters=words(rate,dt,present,visual_policy)
  new.uc.mem_write(state,before);new.uc.mem_write(policy,parameters);new.uc.mem_write(result,words(0xabcdef12));assert new.invoke('dh2_actor_update_rotation',[state,policy,result])==0;actual=bytes(new.uc.mem_read(state,24));assert actual==expected,(len(rows),label,'state',before.hex(),parameters.hex(),expected.hex(),actual.hex());sync=words(len(events));assert bytes(new.uc.mem_read(result,4))==sync and len(events)==int(present and visual_policy),(len(rows),'sync');assert not events or events[0]==expected[:12],('sync order',len(rows));rows.append(before+parameters+expected+sync)
  output_angle,output_turn=struct.unpack_from('<f',expected,8)[0],struct.unpack_from('<I',expected,16)[0];totals['snap_sentinel']+=bool(f&0x20);totals['visual_sync']+=len(events);totals['zero_dt']+=dt==0;totals['dt_getter_calls']+=not bool(f&0x20);totals['snap_to_target']+=output_angle==fl(heading);totals['increment_positive']+=output_angle!=fl(heading) and output_turn==1;totals['increment_negative']+=output_angle!=fl(heading) and output_turn==0
  if label:samples.append({'case':label,'rate_word':hex(rate),'state_words':list(struct.unpack('<6I',expected)),'visual_sync':len(events)})
 for f in (0,0x10,0x20,0x30,0x23c1):
  for raw in (-0x80000000,-25601,-25600,-25599,0,25600,0x7fffffff):
   for dt in (0,1,16,33,1000,0x7fffffff,0xffffffff):
    for heading in (-4.71238899230957,-3.1415927410125732,-0.,0.,3.1415927410125732,4.71238899230957):compare(f,raw,dt,0.,heading,1,1)
 for dt in (0,1,16,33,1000):
  step=fl(fl(1.*b.floating(0x41490fdb))*fl(fl(float(dt))*b.floating(0x3a83126f)))
  for target in (step,-step,fl(step-.0000001),fl(-step+.0000001)):
   compare(0,0,dt,0.,target,1,0,f'exact step boundary dt{dt} target{target}')
 for n in range(4096):compare(rng.getrandbits(32),rng.randrange(-0x80000000,0x80000000),rng.choice((0,1,16,33,rng.getrandbits(32))),rng.uniform(-20,20),rng.uniform(-20,20),rng.randrange(2),rng.randrange(2))
 for initial,target in ((-0.,0.),(0.,-0.),(1e30,-1e30),(-1e30,1e30)):
  compare(0,0,16,initial,target,1,0,f'angle boundary {initial},{target}')
 rejects=0
 def reject(s,p,argv=None):
  nonlocal rejects
  new.uc.mem_write(state,s);new.uc.mem_write(policy,p);new.uc.mem_write(result,words(0xabcdef12));before=[bytes(new.uc.mem_read(at,n)) for at,n in ((state,24),(policy,16),(result,4))];assert new.invoke('dh2_actor_update_rotation',argv or [state,policy,result])==1;assert before==[bytes(new.uc.mem_read(at,n)) for at,n in ((state,24),(policy,16),(result,4))];rejects+=1
 good=struct.pack('<4f2I',0.,0.,0.,1.,0,0);params=struct.pack('<f3I',1.,16,1,1)
 for offset,value in ((20,1),(16,2),(8,0x7f800000),(12,0x7fc01234)):
  s=bytearray(good);struct.pack_into('<I',s,offset,value);reject(bytes(s),params)
 for offset,value in ((0,0x7f800000),(8,2),(12,2)):
  p=bytearray(params);struct.pack_into('<I',p,offset,value);reject(good,bytes(p))
 reject(good,struct.pack('<f3I',3e38,0xffffffff,1,1));reject(struct.pack('<4f2I',0.,0.,3e38,-3e38,0,0),params)
 for argv in ([0,policy,result],[state,0,result],[state,policy,0],[state,policy,state],[state,policy,policy],[state,state,result]):reject(good,params,argv)
 ref=words(0x31524f41,len(rows))+b''.join(rows);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(ref);report={'original_sha256':m['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(ref).hexdigest(),'comparisons':len(rows),'atomic_rejection_checks':rejects,'mismatches':0,'totals':totals,'boundary_snapshots':samples,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='boundary_snapshots'}))
if __name__=='__main__':main()
