"""Original AnimatedFX bodies and full PlayAnimFX caller versus O2 ARM64.
Scene sync, animator/visibility, debug and end-sampling are explicit services.
"""
import argparse,json,random,struct,sys,hashlib,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/engine-animation/tests'))
from compiled_transforms_differential import Cpu,words,word,bits,i32
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--gold',type=Path,required=True);a=p.parse_args();start=time.monotonic();m=json.loads((R/'.local-inputs/character-fx-owner-v1/lifecycle/original-functions.json').read_text());old=Cpu(R/'.local-inputs/libDungeonHunter2.so',False,m);new=Cpu(a.library,True,{'functions':[]});assert sha(R/'.local-inputs/libDungeonHunter2.so')==m['original_sha256']
 os=old.data+0x1000;ns=new.data+0x1000;od=old.data+0x2000;nd=new.data+0x2000;ov=old.data+0x3000;nv=new.data+0x3000;setptr=old.data+0x5000;anchor=old.data+0x6000;cb=old.data+0x7000;service=new.data+0x7000;sv=new.data+0x7100;visual=old.data+0x8000;vroot=old.data+0x9000;manager=old.data+0xa000;entries=old.data+0xb000;reference=old.data+0xd000
 old.uc.mem_write(cb,words([0xe12fff1e]));new.uc.mem_write(service,words([0xd65f03c0]));new.uc.mem_write(sv,struct.pack('<QQ',0xabcdef1122334455,service))
 ptrids={0:0,setptr:0x123456789abcdef0,anchor:0x100000001,cb:0x2222222233333333,visual:0x4444444455555555}
 def norm(p):return ptrids.get(p,p)
 def original_state():
  b=bytes(old.uc.mem_read(os,84));return struct.pack('<4Q',*[norm(word(b,k))for k in (4,80,40,44)])+b[8:12]+b[20:24]+b[28:36]+struct.pack('<6I',*[b[k]for k in (36,24,48,49,50,76)])+b[52:76]
 def new_state():return bytes(new.uc.mem_read(ns,96))
 logs=[[],[]];facts={};mutate=None;mutation_hits=[0,0]
 def snapshot(which,op,arg=0,id=0,f=0,payload=0):
  logs[which].append((op,arg,id,f,payload,original_state()if which==0 else new_state()))
  if mutate and op==mutate['op'] and len(logs[which])==mutate['at']:
   mutation_hits[which]+=1
   # Genuine synchronous callback changes source state before caller resumes.
   if which==0:old.uc.mem_write(os+20,words([9]));old.pointer(os+4,cb);old.uc.mem_write(os+48,b'\x07')
   else:new.uc.mem_write(ns+36,words([9]));new.uc.mem_write(ns,struct.pack('<Q',ptrids[cb]));new.uc.mem_write(ns+56,words([7]))
 def ret(c,value=0):c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def oldhook(uc,address,size,user):
  if address==0x492aa0:snapshot(0,1,old.reg(1));ret(old)
  elif address==0x4924b0:
   f=old.reg(1);uc.mem_write(os+32,words([f]));snapshot(0,2,0,0,f);ret(old)
  elif address==0x492694:
   arg=old.reg(1);snapshot(0,3,arg);uc.mem_write(os+24,bytes([arg]));ret(old)
  elif address==0x492744:snapshot(0,4);ret(old)
  elif address==0x471368:snapshot(0,6,old.reg(1));ret(old)
  elif address==cb:snapshot(0,7,0,norm(cb),0,norm(old.reg(1)));ret(old)
  elif address==0x492818:snapshot(0,8);uc.reg_write(old.pc,0x49288c)
  elif address==0x31f66c:snapshot(0,11);ret(old,facts['dt'])
  elif address==0x337888:
   if kind!=3:snapshot(0,13)
   ret(old)
  elif address==0x337a88:
   op=14 if norm(word(uc.mem_read(os+80,4),0)) and not any(x[0]==14 for x in logs[0])else 15;snapshot(0,op);ret(old)
  elif address==0x4924e0:snapshot(0,16);ret(old,facts['complete'])
  elif address==0x3140ec:uc.mem_write(old.reg(0),bytes(24));ret(old)
  elif address==0x3139ac:ret(old)
  elif address==0x494ad4:ret(old,os)
  elif address==0x494550:ret(old) # free-vector insertion allocation boundary
  elif address==0x337ec8:ret(old,1)
  elif address==anchor+0x200:snapshot(0,9,0,norm(anchor));ret(old,facts['dead'])
  # Direct source flag reads are projected as provider calls natively.
  elif address==0x4930d8:snapshot(0,10,0,norm(old.reg(3)))
  elif address==0x492ff0:snapshot(0,12,0,norm(old.reg(3)))
  elif address==0x4927f4:
   # Source writes set.finished directly; projected caller identity service.
   if word(uc.mem_read(os+80,4),0):snapshot(0,7,1,norm(word(uc.mem_read(os+80,4),0)))
 old.uc.hook_add(UC_HOOK_CODE,oldhook)
 def newhook(uc,address,size,user):
  if address!=service:return
  assert new.reg(0)==0xabcdef1122334455 and new.reg(1)==ns
  q=new.reg(2);b=bytes(uc.mem_read(q,32));op,arg,id,f,res,payload=struct.unpack('<IIQIIQ',b);assert res==0;snapshot(1,op,arg,id,f,payload)
  if op in (9,10,11,12,16):uc.mem_write(q+4,words([facts[{9:'dead',10:'disabled',11:'dt',12:'stationary',16:'complete'}[op]]]))
  ret(new)
 new.uc.hook_add(UC_HOOK_CODE,newhook)
 rng=random.Random(1234931);records=[];counts={k:0 for k in ('set','loop_end','update','play','drop')};mutations=0;configured=0;requests=0
 for kind in range(5):
  for j in range(512):
   hascb=rng.randrange(2);hasset=rng.randrange(2);hasanchor=rng.randrange(2);flags=[rng.randrange(2)for _ in range(6)];loop=rng.choice((-2,-1,0,1,2,2147483647));timer=rng.choice((-1,0,1,20,2147483647));speed=rng.choice((0,0x80000000,bits(1),bits(-2),0x7fc12345));position=[bits(rng.uniform(-100,100))for _ in range(6)]
   initial=struct.pack('<4QiiiI6I6I',ptrids[cb]if hascb else 0,ptrids[setptr]if hasset else 0,ptrids[anchor]if hasanchor else 0,ptrids[visual],272,loop,timer,speed,*flags,*position)
   new.uc.mem_write(ns,initial);old.uc.mem_write(os,bytes(84));old.pointer(os+4,cb if hascb else 0);old.pointer(os+80,setptr if hasset else 0);old.pointer(os+40,anchor if hasanchor else 0);old.pointer(os+44,visual);old.uc.mem_write(os+8,words([272]));old.uc.mem_write(os+20,words([loop&0xffffffff]));old.uc.mem_write(os+28,words([timer&0xffffffff]));old.uc.mem_write(os+32,words([speed]));
   for k,x in zip((36,24,48,49,50,76),flags):old.uc.mem_write(os+k,bytes([x]))
   old.uc.mem_write(os+52,words(position));old.uc.mem_write(setptr,bytes(128));old.uc.mem_write(visual,bytes(128));old.pointer(visual+8,vroot);old.uc.mem_write(vroot,bytes(0x300));old.uc.mem_write(anchor,bytes(0x300));old.pointer(anchor,anchor+0x100);old.pointer(anchor+0x134,anchor+0x200);old.uc.mem_write(anchor+0x200,words([0xe12fff1e]))
   facts={k:rng.randrange(2)for k in ('dead','disabled','stationary','complete')};facts['dt']=rng.choice((0,1,16,0x7fffffff,0xffffffff));old.uc.mem_write(anchor+0x81,bytes([facts['disabled']]));old.uc.mem_write(anchor+0x84,bytes([facts['stationary']]));logs=[[],[]];mutation_hits=[0,0];mutate={'op':1 if kind in (0,3)else 7,'at':1 if kind in (0,3)else 2}if j%16==0 else None
   data=struct.pack('<3IIiiQ',*[rng.randrange(2)for _ in range(3)],bits(rng.uniform(-5,5)),rng.choice((-1,0,2)),rng.choice((-1,100)),ptrids[setptr]);new.uc.mem_write(nd,data);df=struct.unpack('<3IIiiQ',data);old.uc.mem_write(od,bytes(df[:3])+b'\0'+struct.pack('<IiiI',df[3],df[4],df[5],setptr));
   if kind==0:old.invoke(0x492e8c,[os,*df[:3],df[3],df[4]&0xffffffff,setptr,cb]);rc=new.invoke('dh2_fx_set_anim_v1',[ns,nd,ptrids[cb],sv]);name='set'
   elif kind==1:old.invoke(0x4927a4,[os]);rc=new.invoke('dh2_fx_handle_loop_end_v1',[ns,sv]);name='loop_end'
   elif kind==2:old.invoke(0x492f68,[os]);rc=new.invoke('dh2_fx_update_v1',[ns,sv]);name='update'
   elif kind==3:
    pv=words(position);old.uc.mem_write(ov,pv);new.uc.mem_write(nv,pv);use_data=j%2;rotate=j%4==0
    if rotate:old.invoke(0x4956b8,[0,272,ov,ov+12,anchor if hasanchor else 0,od if use_data else 0])
    else:old.invoke(0x495b54,[0,272,ov,anchor if hasanchor else 0,od if use_data else 0])
    logs[0]=[x for x in logs[0]if x[0]!=13];expectedcb=norm(word(old.uc.mem_read(os+4,4),0));new.uc.mem_write(ns,initial);rc=new.invoke('dh2_fx_play_v1',[ns,nv,nv+12 if rotate else 0,ptrids[anchor]if hasanchor else 0,nd if use_data else 0,expectedcb,sv]);name='play'
   else:
    old.uc.mem_write(manager,bytes(64));old.uc.mem_write(manager+4,b'\x01');old.pointer(manager+40,entries);old.pointer(manager+44,entries+273*24);header=entries+272*24+16;old.pointer(header,header);old.pointer(header+4,header);old.pointer(reference,os);old.invoke(0x494978,[manager,reference]);assert word(old.uc.mem_read(reference,4),0)==0;rc=new.invoke('dh2_fx_drop_reset_v1',[ns,sv]);name='drop'
   assert rc==0
   assert original_state()==new_state(),('state',name,j,initial.hex(),original_state().hex(),new_state().hex(),logs)
   assert logs[0]==logs[1],('calls',name,j,logs[0],logs[1])
   assert mutation_hits[0]==mutation_hits[1];counts[name]+=1;configured+=bool(mutate);mutations+=bool(mutation_hits[0]);requests+=len(logs[0]);records.append(words([kind])+initial+data+words([facts[k]for k in ('dead','disabled','stationary','complete','dt')])+words([int(bool(mutate))])+original_state()+words([len(logs[0])])+b''.join(struct.pack('<IIQIQ',*x[:5])+x[5]for x in logs[0]))
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(words([0x31535846,len(records)])+b''.join(records));sources=[R/'port/level-world'/x for x in ('character_fx_state_v1.hpp','character_fx_state_v1.cpp')]+[Path(__file__)];report={'validation':'PASS','comparisons':len(records),'counts':counts,'synchronous_mutation_cases':mutations,'configured_mutation_cases':configured,'ordered_service_comparisons':requests,'original_sha256':m['original_sha256'],'arm64_library_sha256':sha(a.library),'reference_sha256':sha(a.gold),'source_sha256':{x.relative_to(R).as_posix():sha(x)for x in sources},'scope':__doc__,'mismatches':0,'elapsed_seconds':round(time.monotonic()-start,3)};a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
