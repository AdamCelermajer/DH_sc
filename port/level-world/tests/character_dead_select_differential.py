"""Actual SM_SetDeadState/getter/revive-gate instructions vs optimized ARM64.
Constants/stance and downstream registered-state dispatch are explicit services.
"""
import argparse,hashlib,json,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu,string
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(*xs):return struct.pack('<'+'I'*len(xs),*(x&0xffffffff for x in xs))
def w(c,p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def signed(v):return v if v<0x80000000 else v-0x100000000
class Audit:
 def __init__(self,path,native,manifest):
  self.c=c=Cpu(path,native,manifest);self.native=native;d=c.data;self.sm=d+0x1000;self.state=d+0x2000;self.fsm=d+0x3000;self.view=d+0x4000;self.rows=[d+0x5000,d+0x7000];self.actors=[d+0x10000,d+0x14000];self.app=d+0x18000;self.infos=d+0x19000;self.constants=[d+0x1a000,d+0x1b000];self.services=d+0x1c000;self.owner_services=d+0x1d000;self.callback=d+0x1e000;self.payload=d+0x1f000;self.trace=[];self.x={};self.nested=[];self.reentered=False
  if native:c.uc.mem_write(self.services,struct.pack('<QQ',0,self.callback));c.uc.mem_write(self.owner_services,struct.pack('<QQ',0,self.callback+16));c.uc.mem_map(c.stack-0x10000,0x10000)
  else:self.app=0x99f72c
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,value=0):c=self.c;c.put(0,value&0xffffffff);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def snapshot(self):
  c=self.c
  if self.native:
   f=struct.unpack('<QQII',c.uc.mem_read(self.fsm,24));character=f[1];present=f[2];state=signed(w(c,self.state)) if present else -1
   return [self.actors.index(character),state,present,w(c,self.state+48),w(c,self.view+24),w(c,self.state+40),w(c,self.view+20),w(c,self.state+16)]
  character=w(c,self.sm+4);ptr=w(c,self.sm+0x20);state=signed(w(c,ptr)) if ptr else -1
  return [self.actors.index(character),state,int(bool(ptr)),w(c,self.sm+0x28),w(c,self.sm+0x38),c.uc.mem_read(self.sm+0x3e,1)[0],c.uc.mem_read(self.sm+0x3f,1)[0],w(c,self.sm+0x60)]
 def mutate(self,number):
  c=self.c;x=self.x
  if number!=x['mutate']:return
  if self.native:
   c.pointer(self.fsm+8,self.actors[x['next_actor']]);c.uc.mem_write(self.view+20,words(x['next_alt']));c.uc.mem_write(self.state,words(x['next_current']));c.uc.mem_write(self.fsm+16,words(x['next_present']));c.uc.mem_write(self.sm+20,words(x['next_current'] if x['next_present'] else -1));c.pointer(self.view+8,self.rows[1]);c.uc.mem_write(self.view+16,words(x['next_count']))
  else:
   c.pointer(self.sm+4,self.actors[x['next_actor']]);c.uc.mem_write(self.sm+0x3f,bytes([x['next_alt']]));c.pointer(self.sm+0x20,self.infos+16*x['next_current'] if x['next_present'] else 0);c.pointer(0x9a6444,self.rows[1]);c.pointer(0x9a6440,x['next_count']);c.pointer(self.app+0x2c,self.constants[1])
  # Retained source row's later base remains live, not a pre-copied whole row.
  for row in self.rows:
   for i in range(20):c.uc.mem_write(row+i*(16 if self.native else 160)+(4 if self.native else 0x14),words(x['replacement'],x['replacement']+1))
 def event(self,op,mask=0,payload=0):
  number=len(self.trace);self.trace.append([op,mask,payload,*self.snapshot()]);self.mutate(number)
  if number==self.x.get('reentry',-1) and not self.reentered:
   c=self.c;self.reentered=True;context=c.uc.context_save();stack=c.stack;trace=self.trace;self.trace=[];c.stack-=0x10000
   try:
    if self.native:status=signed(c.invoke('dh2_character_dead_select',[self.view,0,0,1,self.services,self.owner_services]))
    else:c.invoke(0x3c58c8,[self.sm,0,0,1]);status=1
    self.nested.append(dict(trace=self.trace.copy(),state=self.snapshot(),status=status))
   finally:c.stack=stack;c.uc.context_restore(context);self.trace=trace
 def hook(self,uc,address,size,_):
  c=self.c;x=self.x
  if self.native:
   if address==self.callback:
    op,mask,character,a,b=struct.unpack('<IIQII',uc.mem_read(c.reg(2),24));assert not a and not b and character==self.actors[self.snapshot()[0]]
    self.event(op,mask);response=x['index'] if op==0 else x['constant'] if op==1 else x['stance'];uc.mem_write(c.reg(3),words(response));self.ret();return
   if address in [c.symbols['dh2_character_state_owner_transition'],c.symbols['dh2_character_state_owner_event']]:
    force=address==c.symbols['dh2_character_state_owner_transition'];assert c.reg(0)==self.sm
    assert (c.reg(1),c.reg(2))==(12,0xc358) if force else c.reg(1)==0xc358
    payload=c.reg(3 if force else 2);assert payload in [0,self.payload];self.event(3 if force else 4,0xc358,int(bool(payload)));self.ret(0);return
   return
  if address==0x3a3228:self.event(0);return # actual getter executes
  if address==0x4c4bdc:
   assert string(c,c.reg(1))==b'AnimStancedAnim' and string(c,c.reg(2))==b'SL__LIST_IPHONE';assert c.reg(0) in self.constants
   lr=uc.reg_read(c.lr);mask={0x3c5950:0x20000,0x3c598c:0x40000,0x3c5a00:0x10000,0x3c5a34:0x8000}[lr];self.event(1,mask);self.ret(x['constant']);return
  if address==0x3a53e0:self.event(2);self.ret(x['stance']);return
  if address in [0x3c1938,0x3c5684]:
   force=address==0x3c1938;assert (c.reg(1),c.reg(2))==(12,0xc358) if force else c.reg(1)==0xc358
   payload=c.reg(3 if force else 2);assert payload in [0,self.payload];self.event(3 if force else 4,0xc358,int(bool(payload)));self.ret()
 def fixture(self,x):
  self.x=x;self.trace=[];self.nested=[];self.reentered=False;c=self.c
  for row_index,row in enumerate(self.rows):
   for i in range(20):c.uc.mem_write(row+i*(16 if self.native else 160)+(0 if self.native else 0x10),words(*(v+i*9+row_index*77 for v in x['bases'])))
  for i,actor in enumerate(self.actors):c.uc.mem_write(actor,bytes(0x1200));c.uc.mem_write(actor+0x1000,words(x['index']))
  if self.native:
   c.uc.mem_write(self.state,bytes(56));c.uc.mem_write(self.state,words(x['current']));c.uc.mem_write(self.state+16,words(x['elapsed']));c.uc.mem_write(self.state+40,words(x['mode_before']));c.uc.mem_write(self.state+48,words(x['override']));c.uc.mem_write(self.fsm,struct.pack('<QQII',self.state,self.actors[0],x['present'],0));c.uc.mem_write(self.sm,struct.pack('<QQIiQQ',self.fsm,self.infos,20,x['current'] if x['present'] else -1,0,0));c.uc.mem_write(self.view,struct.pack('<QQIIiI',self.sm,self.rows[0],x['count'],x['alternate'],signed(x['secondary']),0))
   for i in range(20):c.uc.mem_write(self.infos+40*i,struct.pack('<i5IQII',i,0,0,0,0,0,0,0,0))
  else:
   c.uc.mem_write(self.sm,bytes(0x100));c.pointer(self.sm+4,self.actors[0]);c.pointer(self.sm+0x20,self.infos+16*x['current'] if x['present'] else 0);c.pointer(self.sm+0x28,x['override']);c.pointer(self.sm+0x38,x['secondary']);c.uc.mem_write(self.sm+0x3e,bytes([x['mode_before'],x['alternate']]));c.pointer(self.sm+0x60,x['elapsed']);c.pointer(0x9a6440,x['count']);c.pointer(0x9a6444,self.rows[0]);c.pointer(self.app+0x2c,self.constants[0])
   for i in range(20):c.uc.mem_write(self.infos+16*i,words(i))
 def run(self):
  x=self.x;c=self.c
  if self.native:return signed(c.invoke('dh2_character_dead_select',[self.view,x['mode'],self.payload if x['payload'] else 0,x['force'],self.services,self.owner_services]))
  c.invoke(0x3c58c8,[self.sm,x['mode'],self.payload if x['payload'] else 0,x['force']]);return int(any(t[0] in [3,4] for t in self.trace))
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,default=REPO/'.local-inputs/character-dead-select-discovery/oracle.so');p.add_argument('--cases',type=int,default=2400);a=p.parse_args();start=time.monotonic();scratch=REPO/'.local-inputs/character-dead-select-discovery';manifest=json.loads((scratch/'original-functions.json').read_text());old=Audit(REPO/'.local-inputs/libDungeonHunter2.so',False,manifest);new=Audit(a.library,True,{'functions':[]});rng=random.Random(20261014);records=[]
 for i in range(a.cases):
  x=dict(count=rng.choice([0,1,17,18,20]),index=rng.choice([-1,0,1,16,17,18,19,20,0x7fffffff]),current=rng.randrange(20),present=rng.randrange(2),alternate=rng.choice([0,1,255]),mode=rng.choice([0,1,255,256,0xffffffff]),force=rng.choice([0,1,256]),payload=rng.randrange(2),constant=rng.choice([0,0xffffffff,0x8000,0x10000,0x20000,0x40000,0x78000,0x12345]),stance=rng.choice([-1,0,3,0x7fffffff,-0x80000000]),bases=[rng.getrandbits(32) for _ in range(4)],override=rng.getrandbits(32),secondary=rng.getrandbits(32),mode_before=rng.randrange(256),elapsed=rng.getrandbits(32),mutate=rng.choice([-1,0,1,2,3]),next_actor=rng.randrange(2),next_current=rng.randrange(20),next_present=rng.randrange(2),next_alt=rng.choice([0,1,255]),next_count=rng.choice([0,17,18,20]),replacement=rng.getrandbits(32))
  if i%60==0:x.update(count=20,index=0,constant=0x78000,mutate=-1,reentry=1)
  old.fixture(x);new.fixture(x);r=old.run();s=new.run();assert r==s and old.trace==new.trace and old.snapshot()==new.snapshot() and old.nested==new.nested,(i,x,r,s,old.trace,new.trace,old.snapshot(),new.snapshot(),old.nested,new.nested)
  records.append(dict(config=x,status=r,trace=old.trace.copy(),state=old.snapshot(),nested=old.nested.copy()))
 guards=0
 for n in range(8):
  new.fixture(dict(x,count=20));args=[new.view,0,new.payload,1,new.services,new.owner_services]
  if n==0:args[0]=0
  elif n==1:args[-1]=0
  elif n==2:args[-2]=0
  elif n==3:new.c.uc.mem_write(new.view+28,words(1))
  elif n==4:new.c.uc.mem_write(new.view+20,words(256))
  elif n==5:new.c.pointer(new.view+8,0)
  elif n==6:new.c.uc.mem_write(new.fsm+20,words(1))
  elif n==7:new.c.uc.mem_write(new.sm+20,words(99))
  regions=[(new.view,32),(new.sm,40),(new.fsm,24),(new.state,56)];before=[bytes(new.c.uc.mem_read(p,s)) for p,s in regions];assert signed(new.c.invoke('dh2_character_dead_select',args))==-1 and not new.trace and before==[bytes(new.c.uc.mem_read(p,s)) for p,s in regions];guards+=1
 ref=ROOT/'reference/character-dead-select';ref.mkdir(parents=True,exist_ok=True)
 for name,source in [('original-functions.json',scratch/'original-functions.json'),('original-functions.asm',scratch/'reference/original-functions.asm')]: (ref/name).write_bytes(source.read_bytes())
 gold=ref/'dead-select-fixtures.json';gold.write_text(json.dumps(dict(records=records),separators=(',',':'))+'\n')
 # Fixed-width host replay; no fabricated native outcome model.
 binary=bytearray(b'DDS1'+words(len(records)))
 keys=['count','index','current','present','alternate','mode','force','payload','constant','stance','override','secondary','mode_before','elapsed','mutate','next_actor','next_current','next_present','next_alt','next_count','replacement','reentry']
 for r in records:
  x=r['config'];binary+=words(*(x.get(k,-1) for k in keys),*x['bases'],r['status'],len(r['trace']),*r['state'],len(r['nested']))
  for t in r['trace']:binary+=words(*t)
  for n in r['nested']:
   binary+=words(n['status'],len(n['trace']),*n['state'])
   for t in n['trace']:binary+=words(*t)
 (ref/'dead-select-fixtures.bin').write_bytes(binary)
 build=json.loads(Path(str(a.library)+'.build.json').read_text(encoding='utf-8-sig'));sources=build['source_sha256'];sources[str(Path(__file__).relative_to(REPO))]=sha(Path(__file__))
 report=dict(validation='PASS',scope=__doc__,original_sha256=manifest['original_sha256'],original_manifest_sha256=sha(ref/'original-functions.json'),optimized_arm64_library_sha256=sha(a.library),source_sha256=sources,comparisons=len(records),ordered_requests=sum(len(r['trace'])+sum(len(n['trace']) for n in r['nested']) for r in records),synchronous_reentry_cases=sum(bool(r['nested']) for r in records),native_atomic_guards=guards,mismatches=0,gold_sha256=sha(gold),binary_gold_sha256=sha(ref/'dead-select-fixtures.bin'),actual_getter_and_revive_gate_executed=True,registered_dispatch_boundary_explicit=True,packaged_APK=False,elapsed_seconds=round(time.monotonic()-start,2));(ROOT/'reports/character-dead-select-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
