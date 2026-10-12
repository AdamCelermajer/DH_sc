"""Actual Kill/Ctrl_Kill instructions, property kernels and exact service order.

Loot, aggro enumeration, classification/handle, XP, trophies, constants and
event dispatch are declared fixture services. No supported source blocks skip.
Null current-Level probes compare the reached unsafe producer prefix only.
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
  self.actors=[d+0x1000+i*0x2000 for i in range(4)];self.views=[d+0x10000+i*0x100 for i in range(4)];self.sheets=[[d+0x12000+i*0x5000+j*0x1000 for j in range(4)] for i in range(4)];self.defaults=d+0x40000;self.types=d+0x41000;self.vt=d+0x42000;self.cb=d+0x43000;self.bind=d+0x44000;self.out=d+0x45000;self.world=d+0x46000;self.levels=[d+0x48000,d+0x49000];self.manager=d+0x4a000;self.trophy=d+0x4b000;self.constants=d+0x4c000;self.identities=[0,*self.actors,*self.levels,self.manager,self.trophy,self.constants];self.trace=[];self.config={};self.level_calls=self.count_calls=0;self.dead_written=0;self.prefix=False;self.kernels={}
  if native:
   c.uc.mem_write(self.bind,struct.pack('<QQ',0,self.cb));c.uc.mem_write(self.defaults,packed(defaults));c.uc.mem_write(self.types,packed(types))
  else:
   self.sheets=[[p+0x560+x+4 for x in (8,0x38c,0x710,0xa94)] for p in self.actors];c.pointer(0x9a645c,self.defaults);c.uc.mem_write(self.defaults,bytes(4)+packed(defaults)+bytes(4)+packed(types));self.got=(0x3a5b48+w(c,0x3a61a4))&0xffffffff;self.app=w(c,self.got+w(c,0x3a61a8));self.trophy_global=w(c,self.got+w(c,0x3a61b0));c.pointer(self.app+0x40,self.manager);c.pointer(self.app+0x2c,self.constants);c.pointer(self.trophy_global,self.trophy)
   for p in self.actors:c.pointer(p,self.vt)
   for off,cb in [(0x34,0),(0x28,4),(0x24,8),(0x54,12)]:c.pointer(self.vt+off,self.cb+cb)
   c.uc.mem_write(self.cb,words(*([0xe12fff1e]*4)))
   self.quest_vtables={w(c,self.got+w(c,address))+8:i+1 for i,address in enumerate([0x3a61c8,0x3a61d4,0x3a61dc,0x3a61e4])}
  if native:c.uc.mem_map(c.stack-0x10000,0x10000)
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ident(self,p):return self.identities.index(p) if p in self.identities else 99
 def finish(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def boundary(self):self.prefix=True;self.c.uc.reg_write(self.c.sp,self.c.stack+0xe000);self.c.uc.reg_write(self.c.pc,self.c.stop)
 def event(self,op,subject=0,target=0,arg=0,name='',key='',payload=None,index=0):
  x=self.config;self.trace.append([op,subject,target,arg,name,key,payload or [],index]);c=self.c
  live_dead=c.uc.mem_read(self.actors[0]+(48 if self.native else 0x1449),1)[0]
  if op==x.get('reentry_op',-1) and not self.reentered:
   self.reentered=True;context=c.uc.context_save();stack=c.stack;trace=self.trace;written=self.dead_written;prefix=self.prefix;self.trace=[];self.dead_written=0;self.prefix=False;c.stack-=0x10000
   try:
    if self.native:
     symbol='dh2_character_ctrl_kill' if x['nested_wrapper'] else 'dh2_character_kill';status=signed(c.invoke(symbol,[self.out+64,self.actors[0],0,x['nested_force'],self.world,self.bind]));output=list(struct.unpack('<5Ii',c.uc.mem_read(self.out+64,24)))
    else:
     c.invoke(0x3ad528 if x['nested_wrapper'] else 0x3a5b18,[self.actors[0],0,x['nested_force']]);status=-3 if self.prefix else 1;output=[self.trace[-1][0]+1,len(self.trace),self.dead_written,sum(t[0]==9 for t in self.trace),sum(t[0]==11 for t in self.trace),status]
    self.reentry_runs.append(dict(trace=self.trace,output=output,status=status))
   finally:c.stack=stack;c.uc.context_restore(context);self.trace=trace;self.dead_written=written;self.prefix=prefix
  if op==x.get('mutate_op',-1) and not self.mutated:
   self.mutated=True
   if self.native:c.uc.mem_write(self.actors[0]+40,struct.pack('<ihhBB6x',x['mutate_oid'],x['mutate_property'],x['mutate_template'],c.uc.mem_read(self.actors[0]+48,1)[0],x['mutate_suppress']));c.pointer(self.actors[0]+16,self.identities[x['mutate_killer']]);c.pointer(self.world,self.trophy+16)
   else:c.uc.mem_write(self.actors[0]+0x64,words(x['mutate_oid']));c.uc.mem_write(self.actors[0]+0x13c8,struct.pack('<hh',x['mutate_property'],x['mutate_template']));c.uc.mem_write(self.actors[0]+0x14e4,bytes((x['mutate_suppress'],)));c.pointer(self.actors[0]+0x144c,self.identities[x['mutate_killer']]);c.pointer(self.trophy_global,self.trophy+16)
  if op==0:v=live_dead if x.get('live_dead') else x['dead'][min(self.dead_calls,len(x['dead'])-1)];self.dead_calls+=1;return 0,v
  if op==1:return 0,x['players'][subject-1]
  if op==2:return 0,x['local'][subject-1]
  if op==3:return 0,x['online']
  if op==5:
   value=x['levels'][min(self.level_calls,len(x['levels'])-1)];self.level_calls+=1;return self.levels[value-1] if value else 0,0
  if op==7:
   value=x['counts'][min(self.count_calls,len(x['counts'])-1)];self.count_calls+=1;return 0,value
  if op==8:return self.identities[x['entries'][index]] if index<len(x['entries']) else 0,0
  if op==10:return 0,x['trophy_id']
  if op==12:return 0,x['is_character']
  if op==13:return self.identities[x['cast']],0
  if op==15:return 0,x['remote']
  if op==16:return 0,x['constant_values'][['KillXEnemies','ClearEnemies','KillEnemyTemplate','ClearEnemyTemplate'].index(key)]
  return 0,0
 def hook(self,uc,address,size,unused):
  c=self.c
  if self.native:
   if address!=self.cb:return
   op,arg,subject,target,name,key,event,index,reserved=struct.unpack('<IiQQQQQiI',uc.mem_read(c.reg(2),56));assert reserved==0
   payload=[]
   if event:
    kind,dtype,killer,oid,network,prop,flag0,flag1,rs,level,rs2=struct.unpack('<IIQiiiBBHQQ',uc.mem_read(event,48));assert rs==rs2==0;payload=[kind,dtype,self.ident(killer),oid,network,prop,flag0,flag1,self.ident(level)]
   pointer,value=self.event(op,self.ident(subject),self.ident(target),arg,string(c,name).decode() if name else '',string(c,key).decode() if key else '',payload,index);uc.mem_write(c.reg(3),struct.pack('<QiI',pointer,value,0));self.finish();return
  if address in (0x3e0708,0x3e0798,0x3e07a0,0x3df6e0):self.kernels[hex(address)]=self.kernels.get(hex(address),0)+1
  if address==0x3a5b5c:self.dead_written=1
  if address==0x3a5bec and c.reg(0)==0:self.boundary();return
  if address==0x3a5e54 and c.reg(0)==0:self.boundary();return
  if address==0x33ff54:self.finish(self.identities[self.config['cast']]);return
  op={self.cb:0,self.cb+4:1,self.cb+8:12,self.cb+12:15,0x36effc:2,0x7fd794:3,0x36e478:4,0x31f594:5,0x3a5ae4:6,0x3d4a10:7,0x3d72d0:8,0x3a4d5c:9,0x3a3f70:10,0x3813b8:11,0x33dd2c:13,0x3bf828:14,0x4c4bdc:16,0x339090:17}.get(address)
  if op is None:return
  subject=self.ident(c.reg(0)) if op in (0,1,6,9,11,12,14,15,16,17) else self.ident(c.reg(1)) if op==2 else 1 if op in (7,8) else self.ident(c.reg(1)) if op==13 else 0
  target=self.ident(c.reg(1)) if op in (6,14) else self.ident(c.reg(2)) if op==9 else 0
  arg=signed(c.reg(1)) if op in (9,11) else signed(c.reg(2)) if op==4 else 0
  name=string(c,c.reg(0)).decode() if op==10 else string(c,c.reg(1)).decode() if op==16 else '';key=string(c,c.reg(2)).decode() if op==16 else '';index=c.reg(1) if op==8 else 0;payload=[]
  if op==17:
   p=c.reg(1);flags=uc.mem_read(p+16,2);payload=[self.quest_vtables[w(c,p)],w(c,p+4),self.ident(w(c,p+8)),signed(w(c,p+12)),signed(w(c,p+20)),signed(w(c,p+24)),flags[0],flags[1],subject]
  pointer,value=self.event(op,subject,target,arg,name,key,payload,index)
  if op==3:self.finish(self.manager);c.uc.mem_write(self.manager+5,bytes((self.config['online'],)));return
  if op==5:self.finish(pointer);return
  if op==8:c.pointer(c.reg(2),pointer);c.pointer(c.reg(3),0x7fc12345);self.finish();return
  if op==13:c.uc.mem_write(c.reg(0),words(0,0,0));self.finish();return
  self.finish(value)
 def fixture(self,sheets,x):
  c=self.c;self.config=x;self.trace=[];self.level_calls=self.count_calls=self.dead_calls=0;self.dead_written=0;self.prefix=False;self.mutated=False;self.reentered=False;self.reentry_runs=[]
  for i,actor in enumerate(self.actors):
   for p,s in zip(self.sheets[i],sheets[i]):c.uc.mem_write(p,packed(s))
   if self.native:
    c.uc.mem_write(self.views[i],struct.pack('<7QII',self.defaults,self.types,*self.sheets[i],0,0,0));c.uc.mem_write(actor,struct.pack('<QQQQQihhBB6x',actor,self.views[i],self.identities[x['killer']],self.identities[x['owners'][i]],self.identities[x['targets'][i]],signed((x['oid']+i)&0xffffffff),x['property'],x['template'],x['dead_byte'],x['suppress']))
   else:
    c.pointer(actor+0x144c,self.identities[x['killer']]);c.pointer(actor+0x14d4,self.identities[x['owners'][i]]);c.pointer(actor+0x14a4,self.identities[x['targets'][i]]);c.pointer(actor+0x64,(x['oid']+i)&0xffffffff);c.uc.mem_write(actor+0x13c8,struct.pack('<hh',x['property'],x['template']));c.uc.mem_write(actor+0x1449,bytes((x['dead_byte'],)));c.uc.mem_write(actor+0x14e4,bytes((x['suppress'],)));sentinel=actor+0x560+0xe18;c.uc.mem_write(sentinel,words(0,0,sentinel,sentinel));c.pointer(actor+0x560+0xe28,0)
  for i,level in enumerate(self.levels):
   if self.native:c.uc.mem_write(level,struct.pack('<QII',level,x['loot_gate'],0))
   else:c.pointer(level+0x150,x['loot_gate'])
  if self.native:c.uc.mem_write(self.world,struct.pack('<QQ',self.trophy,self.constants))
  else:c.pointer(self.trophy_global,self.trophy)
  c.uc.mem_write(self.out,b'\xcc'*24)
 def state(self):
  c=self.c;out=[]
  for p in self.actors:
   if self.native:
    _,_,killer,owner,target,oid,prop,template,dead,suppress=struct.unpack('<QQQQQihhBB6x',c.uc.mem_read(p,56))
   else:
    killer=w(c,p+0x144c);owner=w(c,p+0x14d4);target=w(c,p+0x14a4);oid=signed(w(c,p+0x64));prop,template=struct.unpack('<hh',c.uc.mem_read(p+0x13c8,4));dead=c.uc.mem_read(p+0x1449,1)[0];suppress=c.uc.mem_read(p+0x14e4,1)[0]
   out.append([self.ident(killer),self.ident(owner),self.ident(target),oid,prop,template,dead,suppress])
  return out
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,default=REPO/'.local-inputs/libDungeonHunter2.so');p.add_argument('--library',type=Path,default=REPO/'.local-inputs/character-kill-discovery/oracle.so');p.add_argument('--cases',type=int,default=2200);a=p.parse_args();start=time.monotonic();ref=ROOT/'reference/character-kill';ref.mkdir(parents=True,exist_ok=True);capture=REPO/'.local-inputs/character-kill-discovery';manifest=json.loads((capture/'original-functions.json').read_text());assert manifest['original_sha256']==sha(a.engine)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80';(ref/'original-functions.json').write_bytes((capture/'original-functions.json').read_bytes());(ref/'original-functions.asm').write_bytes((capture/'reference/original-functions.asm').read_bytes());raw=(REPO/'port/android-native/app/src/main/assets/data/character_properties_pyarray.bin').read_bytes();defaults=list(struct.unpack_from('<224i',raw,4));types=list(struct.unpack_from('<224i',raw,900));old=Audit(a.engine,False,manifest,defaults,types);new=Audit(a.library,True,{'functions':[]},defaults,types);rng=random.Random(20261006);records=[];requests=prefixes=0
 for i in range(a.cases):
  sheets=[[defaults.copy() for _ in range(4)] for _ in range(4)]
  for part in sheets:
   for index in (23,24,25,36):part[1][index]=part[3][index]=rng.choice([0,1,99<<8,499<<8,999<<8,1999<<8,9<<8,49<<8,99<<8,rng.getrandbits(32)])&0x7fffffff
  x=dict(dead=[int(i%17==0),int(i%23==0)],players=[int(i%7==0),1,0,rng.randrange(2)],local=[rng.randrange(2) for _ in range(4)],online=int(i%3==0),levels=[int(i%37!=0),2 if i%41 else 0],loot_gate=int(i%2==0),counts=[rng.randrange(5)],entries=[rng.randrange(5) for _ in range(4)],owners=[0,0,rng.choice([0,2]),0],targets=[1]*4,killer=rng.choice([0,1,2,3]),oid=rng.randrange(-2147483648,2147483648),property=rng.randrange(-32768,32768),template=rng.choice([-1,-32768,32767,17]),dead_byte=rng.randrange(2),suppress=int(i%11==0),is_character=rng.randrange(2),cast=rng.randrange(5),remote=int(i%13==0),trophy_id=rng.randrange(-2147483648,2147483648),constant_values=[rng.randrange(-2147483648,2147483648) for _ in range(4)])
  if i%5==0:x['counts']=[2,0]
  if i%4==0:x.update(mutate_op=rng.choice([1,2,5,6,9,12,15,16,17]),mutate_oid=rng.randrange(-2147483648,2147483648),mutate_property=rng.randrange(-32768,32768),mutate_template=rng.choice([-1,19]),mutate_suppress=rng.randrange(2),mutate_killer=rng.randrange(1,5))
  attacker=rng.randrange(5);force=rng.choice([0,0,0,1,255]);wrapper=i%2==0;old.fixture(sheets,x);new.fixture(sheets,x);old.c.invoke(0x3ad528 if wrapper else 0x3a5b18,[old.actors[0],old.identities[attacker],force]);status=signed(new.c.invoke('dh2_character_ctrl_kill' if wrapper else 'dh2_character_kill',[new.out,new.actors[0],new.identities[attacker],force,new.world,new.bind]));assert status==(-3 if old.prefix else 1),(i,'status',status,old.prefix,old.trace,new.trace,x)
  assert old.trace==new.trace,(i,'trace',old.trace,new.trace,x);assert old.state()==new.state(),(i,'state',old.state(),new.state(),x)
  final=[[bytes(old.c.uc.mem_read(p,896)).hex() for p in part] for part in old.sheets];assert final==[[bytes(new.c.uc.mem_read(p,896)).hex() for p in part] for part in new.sheets],(i,'sheets',x)
  output=list(struct.unpack('<5Ii',new.c.uc.mem_read(new.out,24)));expected=[old.trace[-1][0]+1,len(old.trace),old.dead_written,sum(t[0]==9 for t in old.trace),sum(t[0]==11 for t in old.trace),status];assert output==expected,(i,'output',output,expected)
  records.append(dict(wrapper=wrapper,sheets=sheets,attacker=attacker,force=force,config=x,trace=old.trace,state=old.state(),final_sheets=final,output=expected,original_prefix_only=old.prefix));requests+=len(old.trace);prefixes+=old.prefix
 guards=0
 for kind in range(8):
  new.fixture(sheets,x);args=[new.out,new.actors[0],new.identities[attacker],force,new.world,new.bind]
  if kind==0:args[1]=0
  elif kind==1:args[0]=new.sheets[0][3]
  elif kind==2:args[1]+=1
  elif kind==3:args[-1]=0
  elif kind==4:new.c.uc.mem_write(new.actors[0]+50,b'\1')
  elif kind==5:args[4]=new.actors[0]
  elif kind==6:new.c.uc.mem_write(new.views[0]+40,struct.pack('<Q',new.sheets[0][3]+1))
  elif kind==7:args[0]=new.world
  regions=[(new.out,24)]+[(p,56) for p in new.actors]+[(p,896) for part in new.sheets for p in part];before=[bytes(new.c.uc.mem_read(p,n)) for p,n in regions];assert signed(new.c.invoke('dh2_character_ctrl_kill',args))==-1 and not new.trace and before==[bytes(new.c.uc.mem_read(p,n)) for p,n in regions],('atomic',kind);guards+=1
 reentry=[]
 gates=[(wrapper,op,force) for wrapper in [False,True] for op in [0,1,5,17] for force in ([1,255] if op==0 else [0] if op==17 else [1])]+[(True,9,1)]
 for wrapper,op,force in gates:
  for nested_wrapper in [False,True]:
   sheets=[[defaults.copy() for _ in range(4)] for _ in range(4)]
   x=dict(dead=[0],players=[0]*4,local=[0]*4,online=0,levels=[1],loot_gate=1,counts=[0],entries=[],owners=[0]*4,targets=[0]*4,killer=0,oid=-91,property=-32768,template=17,dead_byte=0,suppress=0,is_character=1,cast=1,remote=0,trophy_id=3,constant_values=[11,12,13,14],live_dead=True,reentry_op=op,nested_wrapper=nested_wrapper,nested_force=1)
   old.fixture(sheets,x);new.fixture(sheets,x);old.c.invoke(0x3ad528 if wrapper else 0x3a5b18,[old.actors[0],0,force]);status=signed(new.c.invoke('dh2_character_ctrl_kill' if wrapper else 'dh2_character_kill',[new.out,new.actors[0],0,force,new.world,new.bind]));expected=[old.trace[-1][0]+1,len(old.trace),old.dead_written,sum(t[0]==9 for t in old.trace),sum(t[0]==11 for t in old.trace),1]
   assert status==1 and old.reentered and new.reentered and old.trace==new.trace and old.reentry_runs==new.reentry_runs and old.state()==new.state() and expected==list(struct.unpack('<5Ii',new.c.uc.mem_read(new.out,24))),('reentry',wrapper,op,force,nested_wrapper,old.trace,new.trace,old.reentry_runs,new.reentry_runs,old.state(),new.state())
   final=[[bytes(old.c.uc.mem_read(p,896)).hex() for p in part] for part in old.sheets];assert final==[[bytes(new.c.uc.mem_read(p,896)).hex() for p in part] for part in new.sheets]
   reentry.append(dict(wrapper=wrapper,sheets=sheets,attacker=0,force=force,config=x,trace=old.trace,nested=old.reentry_runs,state=old.state(),final_sheets=final,output=expected));requests+=len(old.trace)+sum(len(r['trace']) for r in old.reentry_runs)
 gold=ref/'kill-fixtures.json';gold.write_text(json.dumps(dict(defaults=defaults,types=types,records=records,reentry=reentry),separators=(',',':'))+'\n');build=json.loads(Path(str(a.library)+'.build.json').read_text(encoding='utf-8-sig'));sources=dict(build['source_sha256']);sources[str(Path(__file__).resolve().relative_to(REPO))]=sha(Path(__file__));report=dict(validation='PASS',scope=__doc__,original_sha256=sha(a.engine),arm64_library_sha256=sha(a.library),source_sha256=sources,comparisons=len(records)+len(reentry),gold_replay_cases=len(records),synchronous_reentry_cases=len(reentry),ctrl_kill_comparisons=sum(r['wrapper'] for r in records)+sum(r['wrapper'] for r in reentry),ordered_requests=requests,unsafe_original_prefixes=prefixes,atomic_guards=guards,mismatches=0,gold_sha256=sha(gold),original_kernel_entry_counts=old.kernels,original_import_calls=old.c.import_calls,native_import_calls=new.c.import_calls,full_loot_xp_quest_AI_backends=False,packaged_APK=False,elapsed_seconds=round(time.monotonic()-start,2));out=ROOT/'reports/character-kill-arm64-differential.json';out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('source_sha256','original_import_calls','native_import_calls')}))
if __name__=='__main__':main()
