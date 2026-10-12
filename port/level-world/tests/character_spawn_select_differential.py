"""Actual SM_SetSpawnState and Random clone versus optimized native source; transition/timer services explicit."""
import argparse,hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
R=Path(__file__).resolve().parents[1];REPO=R.parents[1];sys.path.insert(0,str(R/'tests'))
from character_script_selection_differential import Cpu
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser()
 for n in ('engine','library','output'):p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();assert sha(a.engine)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
 manifest=R/'reference/character-spawn-state/original-functions.json';old=Cpu(a.engine,False,json.loads(manifest.read_text()));new=Cpu(a.library,True,{'functions':[]})
 char=old.data+0x1000;sm=char+0x4fc;view=new.data+0x1000;fsm=new.data+0x2000;state=new.data+0x3000;rng=new.data+0x4000;svc=new.data+0x5000
 owner=0xabcdef0123456789;context=0xcdef0123456789ab;P=None;traces=[[],[]];records=[];random_cases=0;zero_timers=0
 def w(c,a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
 def put(c,a,v):c.uc.mem_write(a,struct.pack('<I',v&0xffffffff))
 def signed(x):return x if x<0x80000000 else x-0x100000000
 base=(0x3c26b4+w(old,0x3c2728))&0xffffffff;seed=w(old,base+w(old,0x3c272c));calls=w(old,base+w(old,0x3c2730))
 def ret(c,v):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def original(uc,address,size,unused):
  if address==0x30eb2c:
   n,d=signed(old.reg(0)),signed(old.reg(1));assert n>=0 and d>0
   old.put(0,n//d);old.put(1,n%d);uc.reg_write(old.pc,uc.reg_read(old.lr))
  elif address==0x3c1938:
   assert old.reg(0)==sm and old.reg(1)==1 and old.reg(2)==0xffffffff and old.reg(3)==0
   traces[0].append(struct.pack('<4IQQ',0,1,0xffffffff,0,owner,0));ret(old,P[6])
  elif address==0x3dbe24:
   assert old.reg(0)==char+0x3b4 and old.reg(2)==0 and old.reg(3)==0x2d and w(old,uc.reg_read(old.sp))==0
   traces[0].append(struct.pack('<4IQQ',1,old.reg(1),0,0x2d,owner,0));ret(old,P[6])
 old.uc.hook_add(UC_HOOK_CODE,original)
 def native(uc,address,size,unused):
  assert new.reg(0)==context and new.reg(1)==view
  r=bytes(uc.mem_read(new.reg(2),32));assert struct.unpack('<4IQQ',r)[4:]==(owner,0)
  traces[1].append(r);put(new,new.reg(3),P[6]);ret(new,0)
 new.imports[new.callback+32]='body_callback';new.body_callback=native;new.uc.mem_write(svc,struct.pack('<QQ',context,new.callback+32))
 intervals=[(-2147483648,-1),(-1,-1),(-1,100),(0,0),(0,1),(2,1),(100,100),(1,1000),(2147483646,2147483647),(0,2147483647),(2147483647,-2147483648),(0,-2147483648)]
 for delayed,ignored,interval,initial_seed,initial_calls in itertools.product((0,1,255,0xffffffff),(0,1,0xffffffff),intervals,(0,1,2,123,14348906,14348907,2147483647,0xffffffff),(0,17,0xffffffff)):
  P=[delayed,ignored,*interval,initial_seed,initial_calls,0xffffffff if len(records)%2 else 7,0];traces[0].clear();traces[1].clear()
  put(old,sm+4,char);put(old,char+0x1434,P[2]);put(old,char+0x1438,P[3]);put(old,seed,P[4]);put(old,calls,P[5])
  new.uc.mem_write(state,struct.pack('<i13I',17,*([0]*13)));new.uc.mem_write(fsm,struct.pack('<QQII',state,owner,1,0));new.uc.mem_write(view,struct.pack('<QQii',fsm,rng,P[2],P[3]));new.uc.mem_write(rng,struct.pack('<II',P[4],P[5]))
  old.invoke(0x3c2734,(sm,P[0],P[1]));assert new.invoke('dh2_character_spawn_select',(view,P[0],P[1],svc))==1
  expected=(w(old,char+0x1434),w(old,char+0x1438),w(old,seed),w(old,calls));observed=(w(new,view+16),w(new,view+20),w(new,rng),w(new,rng+4))
  assert expected==observed and traces[0]==traces[1] and len(traces[0])==1,(P,expected,observed,traces)
  random_cases+=expected[3]!=P[5];words=struct.unpack('<4IQQ',traces[0][0]);zero_timers+=words[:2]==(1,0)
  records.append(struct.pack('<8I',*(x&0xffffffff for x in P))+struct.pack('<4I',*expected)+traces[0][0])
 gold=R/'reference/character-spawn-state/spawn-select-fixtures.bin';gold.write_bytes(b'SPS1'+struct.pack('<I',len(records))+b''.join(records))
 paths=(R/'character_spawn_select.hpp',R/'character_spawn_select.cpp',R/'character_native_fsm.hpp',R/'character_state.hpp',REPO/'port/game-data/combat.hpp',REPO/'port/game-data/combat.cpp',Path(__file__))
 report=dict(validation='PASS',scope=__doc__,original_sha256=sha(a.engine),library_sha256=sha(a.library),source_sha256={x.relative_to(REPO).as_posix():sha(x) for x in paths},original_capture_sha256=sha(manifest),reference_sha256=sha(gold),comparisons=len(records),ordered_requests=len(records),actual_original_random_executions=random_cases,zero_duration_timer_cases=zero_timers,mismatches=0,compiler_division_helper='Exact signed quotient/remainder service only; original RNG multiply/unsigned reduction/state/counter instructions execute.',registered_state_and_timer_backends=False,APK=False)
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
