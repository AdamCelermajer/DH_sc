"""Full supported offline/nonplayer HitFor instructions and live service order.

Original property kernels, Cmd_Kill virtual dispatch, GetHandle, handle map/cache
and Character cast execute. Game/debug/classification/full Kill are declared
fixture services; unsupported player/online paths compare only reached prefixes.
"""
import argparse,hashlib,json,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu,string
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def w(c,p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def signed(x):return x if x<0x80000000 else x-0x100000000
def words(*xs):return struct.pack('<'+'I'*len(xs),*(x&0xffffffff for x in xs))
def packed(xs):return struct.pack('<224i',*xs)

class Audit:
 def __init__(self,path,native,manifest,defaults,types):
  self.native=native;self.c=c=Cpu(path,native,manifest);d=c.data
  self.actor=d+0x1000;self.main=d+0x4000;self.enemy=d+0x7000;self.vt=d+0xa000;self.cb=d+0xb000;self.bind=d+0xc000;self.out=d+0xd000;self.attacker=d+0xe000;self.view=d+0xf000
  self.defaults=d+0x10000;self.types=d+0x11000;self.sheets=[d+x for x in (0x12000,0x14000,0x16000,0x18000)];self.game=d+0x20000;self.online=d+0x22000;self.manager=d+0x24000;self.global_context=d+0x26000;self.shared=d+0x28000;self.registry=d+0x29000;self.records=d+0x2a000;self.keybuf=d+0x2b000;self.controllers=[d+0x30000,d+0x31000]
  self.trace=[];self.kernels={};self.config={};self.players=0;self.prefix=False;self.kill_controller=0
  self.identities=[0,self.actor,self.main,self.enemy,*self.controllers]
  if native:
   c.uc.mem_write(self.bind,struct.pack('<QQ',0,self.cb));c.uc.mem_write(self.view,struct.pack('<7QII',self.defaults,self.types,*self.sheets,0,0,0));c.uc.mem_write(self.defaults,packed(defaults));c.uc.mem_write(self.types,packed(types))
  else:
   self.sheets=[self.actor+0x560+x+4 for x in (8,0x38c,0x710,0xa94)]
   c.pointer(0x9a645c,self.defaults);c.uc.mem_write(self.defaults,bytes(4)+packed(defaults)+bytes(4)+packed(types))
   got=(0x3a8bdc+w(c,0x3a921c))&0xffffffff;c.pointer(w(c,got+w(c,0x3a9224))+0x40,self.game)
   got=(0x7fd758+w(c,0x7fd78c))&0xffffffff;c.pointer(w(c,got+w(c,0x7fd790)),self.online)
   for addr,literal,offset in [(0x33dd40,0x33dd68,0x33dd6c),(0x33fde4,0x33fe90,0x33fe94)]:c.pointer((addr+w(c,literal)+w(c,offset))&0xffffffff,self.global_context)
   c.pointer(self.global_context+0x38,self.manager)
   for obj in (self.actor,self.main,self.enemy):c.pointer(obj,self.vt)
   for off,cb in [(0x34,0),(0x28,4),(0x54,8),(0x24,12),(0x58,16)]:c.pointer(self.vt+off,self.cb+cb)
   for p in self.controllers:c.pointer(p+4,self.actor)
   c.uc.mem_write(self.cb,words(*([0xe12fff1e]*5)))
  if native:c.uc.mem_map(c.stack-0x10000,0x10000)
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ident(self,p):return self.identities.index(p) if p in self.identities else 99
 def finish(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def boundary(self):self.prefix=True;self.c.uc.reg_write(self.c.sp,self.c.stack+0xe000);self.c.uc.reg_write(self.c.pc,self.c.stop)
 def event(self,op,subject=0,target=0,name=''):
  x=self.config;self.trace.append([op,subject,target,name]);c=self.c
  if op==x.get('mutate_op',-1) and not self.mutated:
   self.mutated=True;c.uc.mem_write(self.sheets[3]+36*4,words(x['mutate_hp']));c.uc.mem_write(self.sheets[1]+36*4,words(x['mutate_hp']));c.pointer(self.actor+16 if self.native else self.actor+0x378,self.controllers[1]);c.uc.mem_write(self.actor+24 if self.native else self.actor+0x11c,words(-19))
  if op==6 and x.get('reentry') and not self.reentered:
   self.reentered=True;saved_context=c.uc.context_save();saved_stack=c.stack;saved_trace=self.trace;saved_meta=self.meta;self.trace=[];self.meta=[0]*10;c.stack-=0x10000
   try:
    if self.native:
     status=signed(c.invoke('dh2_character_hit_for',[self.out+64,self.actor,x['reentry'],self.attacker,self.bind]));nested_output=list(struct.unpack('<IIiiiiiIIi',c.uc.mem_read(self.out+64,40)))
    else:
     c.invoke(0x3a8bc4,[self.actor,x['reentry'],self.identities[x['attacker']]]);status=-3 if self.prefix else 1;nested_output=self.meta.copy();nested_output[0:2]=[self.trace[-1][0]+1,len(self.trace)];nested_output[7]=sum(t[0]==8 for t in self.trace);nested_output[8]=int(any(t[0]==9 for t in self.trace) and not x['remote']);nested_output[9]=status
    self.reentry_runs.append(dict(trace=self.trace,output=nested_output,status=status))
   finally:c.stack=saved_stack;c.uc.context_restore(saved_context);self.trace=saved_trace;self.meta=saved_meta
  if op==0:return x['dead'] if subject==1 else x['main_dead']
  if op==1:return self.main if x['main_present'] else 0
  if op==3:return x['god'] if name=='GOD_Monster' else x['oneshot']
  if op==4:return x['monster']
  if op==5:return x['online']
  if op==6:return x['app_oneshot']
  if op==7:
   if subject==1:v=x['players'][min(self.players,len(x['players'])-1)];self.players+=1;return v
   return x['attacker_player']
  if op==9:return x['remote']
  if op==10:return x['character']
  return 0
 def hook(self,uc,address,size,unused):
  c=self.c
  if self.native:
   if address!=self.cb:return
   op,force,subject,target,name=struct.unpack('<IIQQQ',uc.mem_read(c.reg(2),32));assert force==0
   value=self.event(op,self.ident(subject),self.ident(target),string(c,name).decode() if name else '');uc.mem_write(c.reg(3),struct.pack('<Q',value));self.finish();return
  if address in (0x3e0708,0x3e07a0,0x40570c,0x33dd2c,0x33fdc0,0x33ff54):self.kernels[hex(address)]=self.kernels.get(hex(address),0)+1
  if address==0x3a8c40:self.meta[3]=signed(w(c,self.actor+0x1088))
  if address==0x3a8c4c:self.meta[4]=signed(w(c,self.actor+0x1090))
  if address==0x3a8d00:self.meta[2]=signed(c.reg(2));self.meta[6]=signed(w(c,c.uc.reg_read(c.sp)+0x10))
  if address==0x3a8d58:self.meta[5]=signed(w(c,self.actor+0x1088))
  if address==0x40570c:self.kill_controller=self.ident(c.reg(0));return
  if address==0x708ec0:
   count=w(c,c.reg(0));assert count==48; p=c.heap;c.heap+=(count+15)&~15;uc.mem_write(p,bytes(count));self.finish(p);return
  if address==0x708f00:self.finish();return
  if address==0x3140ec:c.pointer(c.reg(0),c.reg(1));self.finish(c.reg(0));return
  if address==0x318254:self.finish();return
  if address==0x7fd794:self.event(5);return
  # Stop at the reached unsupported source body, retaining its full prefix.
  if address==0x3a8cb4:self.boundary();return
  if address in (0x3a9038,0x3a8d88,0x3a8f7c,0x3a8e9c):self.boundary();return
  if address==0x3a8e60 and not self.config['attacker']:
   self.boundary();return
  op={0x36e478:1,0x337888:2,0x337a88:3,0x3a3064:4,0x320e14:6,self.cb:0,self.cb+4:7,self.cb+8:9,self.cb+12:10,self.cb+16:8}.get(address)
  if op is None:return
  subject=self.ident(c.reg(0)) if op in (0,4,7,9,10) else self.kill_controller if op==8 else 0
  target=self.ident(c.reg(1)) if op==8 else 0
  name=string(c,w(c,c.reg(1))).decode() if op==3 else string(c,c.reg(1)).decode() if op==6 else ''
  if op==1:assert c.reg(1)==0 and c.reg(2)==1
  if op==8:assert c.reg(2)==0
  value=self.event(op,subject,target,name);self.finish(self.game if op==1 else value)
 def fixture(self,sheets,x):
  c=self.c;self.config=x;self.trace=[];self.players=0;self.prefix=False;self.mutated=False;self.kill_controller=0;self.meta=[0]*10;self.reentered=False;self.reentry_runs=[]
  for p,s in zip(self.sheets,sheets):c.uc.mem_write(p,packed(s))
  c.uc.mem_write(self.out,b'\xcc'*40)
  attacker=self.identities[x['attacker']];cached=self.identities[x['cached']]
  if self.native:
   c.uc.mem_write(self.view,struct.pack('<7QII',self.defaults,self.types,*self.sheets,0,0,0))
   c.uc.mem_write(self.actor,struct.pack('<QQQiI',self.actor,self.view,self.controllers[0],123,0));c.uc.mem_write(self.attacker,struct.pack('<QQQ',attacker,self.shared if attacker else 0,self.registry if attacker else 0));c.uc.mem_write(self.shared,struct.pack('<iIQ',x['key'],x['oldframe'],cached));c.uc.mem_write(self.records,struct.pack('<iIQiIQ',-7,0,self.actor,5,0,self.enemy)+bytes(16*6));c.uc.mem_write(self.registry,struct.pack('<Q4I',self.records,2,8,x['frame'],0))
  else:
   c.pointer(self.actor+0x378,self.controllers[0]);c.pointer(self.actor+0x11c,123);c.pointer(self.game+0x660,self.main if x['main_present'] else 0);c.uc.mem_write(self.online+5,bytes((x['online'],)))
   sentinel=self.actor+0x560+0xe18;c.uc.mem_write(sentinel,words(0,0,sentinel,sentinel));c.pointer(self.actor+0x560+0xe28,0)
   c.heap=c.data+0x100000;c.uc.mem_write(self.manager,bytes(0x100));m=self.manager+12;c.uc.mem_write(m,words(0,0,m,m,0));c.pointer(self.manager+0x78,x['frame'])
   for k,obj in [(-7,self.actor),(5,self.enemy)]:c.uc.mem_write(self.keybuf,words(k));p=c.invoke(0x33fc88,[m,self.keybuf]);c.pointer(p+0x18,obj)
   for obj in (self.actor,self.enemy):c.pointer(obj+0x2c,self.shared)
   c.uc.mem_write(self.shared,words(x['key'],cached,x['oldframe']));self.trace=[]
 def state(self):
  c=self.c;shared=struct.unpack('<iIQ' if self.native else '<iII',c.uc.mem_read(self.shared,16 if self.native else 12));key,frame,cached=shared if self.native else (shared[0],shared[2],shared[1]);lifecycle=signed(w(c,self.actor+24 if self.native else self.actor+0x11c));entries=[]
  if self.native:
   count=w(c,self.registry+8)
   entries=[(k,self.ident(obj)) for k,_,obj in struct.iter_unpack('<iIQ',c.uc.mem_read(self.records,16*count))]
  else:
   def visit(p):
    if not p:return
    visit(w(c,p+8));entries.append((signed(w(c,p+16)),self.ident(w(c,p+44))));visit(w(c,p+12))
   visit(w(c,self.manager+16))
  return [lifecycle,key,frame,self.ident(cached),entries]

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,default=REPO/'.local-inputs/libDungeonHunter2.so');p.add_argument('--library',type=Path,default=REPO/'.local-inputs/character-hit-discovery/oracle.so');p.add_argument('--cases',type=int,default=2600);a=p.parse_args();start=time.monotonic();ref=ROOT/'reference/character-hit';ref.mkdir(parents=True,exist_ok=True);capture=REPO/'.local-inputs/character-hit-discovery/full';manifest=json.loads((capture/'original-functions.json').read_text());assert manifest['original_sha256']==sha(a.engine)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80';(ref/'original-functions.json').write_bytes((capture/'original-functions.json').read_bytes());(ref/'original-functions.asm').write_bytes((capture/'reference/original-functions.asm').read_bytes());raw=(REPO/'port/android-native/app/src/main/assets/data/character_properties_pyarray.bin').read_bytes();defaults=list(struct.unpack_from('<224i',raw,4));types=list(struct.unpack_from('<224i',raw,900));old=Audit(a.engine,False,manifest,defaults,types);new=Audit(a.library,True,{'functions':[]},defaults,types);rng=random.Random(20261005);records=[];requests=prefixes=kills=0
 for i in range(a.cases):
  sheets=[defaults.copy() for _ in range(4)];hp=rng.choice([-2147483648,-1,0,1,256,100000,2147483647,rng.randrange(-2147483648,2147483648)]);sheets[1][36]=sheets[3][36]=hp;sheets[3][38]=rng.randrange(-2147483648,2147483648)
  if i%3==0:sheets[0][36]=rng.randrange(-2147483648,2147483648)
  damage=rng.choice([0,1,255,256,0x7fffffff,0x80000000,0xffffffff,rng.getrandbits(32)])
  x=dict(dead=int(i%13==0),main_present=int(i%7!=0),main_dead=int(i%11==0),god=int(i%5==0),monster=int(i%17!=0),online=int(i%31==0),oneshot=int(i%19==0),app_oneshot=int(i%23==0),players=[0],attacker_player=int(i%29==0),character=int(i%9!=0),remote=int(i%2==0),attacker=rng.choice([1,1,3]),cached=rng.choice([0,1,3]),key=rng.choice([-7,0,5,17]),oldframe=rng.choice([0,0xffffffff]),frame=rng.choice([0,7,0xffffffff]))
  if i%37==0:x['players']=[1]
  elif i%41==0:x['players']=[0,1]
  if i%43==0:x['attacker']=0
  if i%4==0:x.update(mutate_op=rng.choice([1,3,6,8,9]),mutate_hp=rng.choice([-1,0,1,2147483647]))
  old.fixture(sheets,x);new.fixture(sheets,x);old.c.invoke(0x3a8bc4,[old.actor,damage,old.identities[x['attacker']]]);status=signed(new.c.invoke('dh2_character_hit_for',[new.out,new.actor,damage,new.attacker,new.bind]));assert status==(-3 if old.prefix else 1),(i,'status',status,old.prefix,old.trace,new.trace)
  assert old.trace==new.trace,(i,'trace',old.trace,new.trace,x)
  assert old.state()==new.state(),(i,'state',old.state(),new.state(),x)
  final=[bytes(old.c.uc.mem_read(p,896)).hex() for p in old.sheets];assert final==[bytes(new.c.uc.mem_read(p,896)).hex() for p in new.sheets],(i,'sheets',x)
  output=list(struct.unpack('<IIiiiiiIIi',new.c.uc.mem_read(new.out,40)));expected=old.meta;expected[0:2]=[old.trace[-1][0]+1,len(old.trace)];expected[7]=sum(t[0]==8 for t in old.trace);expected[8]=int(any(t[0]==9 for t in old.trace) and not x['remote']);expected[9]=status;assert output==expected,(i,'metadata',output,expected);requests+=len(old.trace);prefixes+=old.prefix;kills+=output[7]
  records.append(dict(sheets=sheets,damage=damage,config=x,trace=old.trace,state=old.state(),final_sheets=final,output=output,original_prefix_only=old.prefix))
 guards=0
 for kind in range(10):
  new.fixture(sheets,x);args=[new.out,new.actor,damage,new.attacker,new.bind]
  if kind==0:args[1]=0
  elif kind==1:args[0]=new.sheets[3]
  elif kind==2:args[1]+=1
  elif kind==3:args[4]=0
  elif kind==4:new.c.uc.mem_write(new.actor+28,words(1))
  elif kind==5:new.c.uc.mem_write(new.registry+12,words(1))
  elif kind==6:new.c.uc.mem_write(new.records+4,words(1))
  elif kind==7:args[3]=new.actor
  elif kind==8:new.c.uc.mem_write(new.view+40,struct.pack('<Q',new.sheets[3]+1))
  elif kind==9:new.c.uc.mem_write(new.attacker+8,struct.pack('<Q',new.registry))
  regions=[(new.out,40),(new.actor,32),(new.shared,16),(new.registry,24),(new.records,128)]+[(p,896) for p in new.sheets];before=[bytes(new.c.uc.mem_read(p,n)) for p,n in regions];assert signed(new.c.invoke('dh2_character_hit_for',args))==-1 and not new.trace and before==[bytes(new.c.uc.mem_read(p,n)) for p,n in regions],('atomic',kind);guards+=1
 reentry=[]
 for damage in [0,1,255,256,0x7fffffff,0x80000000,0xffffffff,4096]:
  for nested in [1,256]:
   sheets=[defaults.copy() for _ in range(4)];sheets[1][36]=sheets[3][36]=100000;sheets[3][38]=100000
   x=dict(dead=0,main_present=1,main_dead=0,god=0,monster=1,online=0,oneshot=0,app_oneshot=0,players=[0],attacker_player=0,character=1,remote=0,attacker=1,cached=1,key=-7,oldframe=0,frame=1,reentry=nested)
   old.fixture(sheets,x);new.fixture(sheets,x);old.c.invoke(0x3a8bc4,[old.actor,damage,old.actor]);reentry_status=signed(new.c.invoke('dh2_character_hit_for',[new.out,new.actor,damage,new.attacker,new.bind]));assert reentry_status==1,('reentry status',damage,nested,reentry_status,new.trace,new.reentry_runs)
   assert old.trace==new.trace and old.reentry_runs==new.reentry_runs and old.state()==new.state(),('reentry',damage,nested,old.reentry_runs,new.reentry_runs)
   assert all(bytes(old.c.uc.mem_read(o,896))==bytes(new.c.uc.mem_read(n,896)) for o,n in zip(old.sheets,new.sheets))
   reentry.append(dict(damage=damage,nested_damage=nested,outer_trace=old.trace,nested=old.reentry_runs,after_hp=signed(w(old.c,old.sheets[3]+144)),state=old.state()));requests+=len(old.trace)+sum(len(r['trace']) for r in old.reentry_runs)
 gold=ref/'hit-fixtures.json';gold.write_text(json.dumps(dict(defaults=defaults,types=types,records=records,reentry=reentry),separators=(',',':'))+'\n');build=json.loads(Path(str(a.library)+'.build.json').read_text(encoding='utf-8-sig'));sources=dict(build['source_sha256']);sources[str(Path(__file__).resolve().relative_to(REPO))]=sha(Path(__file__));report=dict(validation='PASS',scope=__doc__,original_sha256=sha(a.engine),arm64_library_sha256=sha(a.library),source_sha256=sources,comparisons=len(records)+len(reentry),gold_replay_cases=len(records),synchronous_reentry_cases=len(reentry),ordered_requests=requests,unsupported_original_prefixes=prefixes,controller_kill_fixture_calls=kills,atomic_guards=guards,mismatches=0,gold_sha256=sha(gold),original_kernel_entry_counts=old.kernels,original_import_calls=old.c.import_calls,native_import_calls=new.c.import_calls,full_kill_backend=False,packaged_APK=False,elapsed_seconds=round(time.monotonic()-start,2));out=ROOT/'reports/character-hit-arm64-differential.json';out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('source_sha256','original_import_calls','native_import_calls')}))
if __name__=='__main__':main()
