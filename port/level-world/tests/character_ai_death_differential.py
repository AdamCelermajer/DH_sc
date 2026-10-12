"""Actual OnDied/AI_SetDead, target-clear/sync and timer-stop instructions.

Group, selected AIS, native SM_SetDeadState, bulk aggro and skill/spell bodies
are required labelled oracle services. No fake full death/backend acceptance.
"""
import argparse,hashlib,json,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu,string
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(*xs):return struct.pack('<'+'I'*len(xs),*(x&0xffffffff for x in xs))
def word(c,p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def signed(x):return x if x<0x80000000 else x-0x100000000
class Audit:
 def __init__(self,path,native,manifest):
  self.native=native;self.c=c=Cpu(path,native,manifest);d=c.data;self.ai=d+0x1000;self.actors=[d+0x10000,d+0x14000];self.owners=[d+0x18000,d+0x19000] if native else self.actors;self.target=d+0x20000 if native else self.ai;self.target_owners=[d+0x21000,d+0x22000];self.stores=[d+0x23000,d+0x24000] if native else [p+0x3b4 for p in self.actors];self.slots=[d+0x25000,d+0x26000];self.groups=[d+0x27000,d+0x28000];self.active=[d+0x29000,d+0x2a000];self.vtables=[d+0x2b000,d+0x2c000];self.target_services=d+0x2d000;self.services=d+0x2e000;self.callback=d+0x2f000;self.target_callback=self.callback+16;self.out=d+0x30000;self.config={};self.trace=[];self.nested=[];self.identities=[0,self.ai,*self.actors,*self.groups,*self.active,*[p+0x4fc for p in self.actors],*self.stores];self.stopped=0;self.resets=0;self.reentered=False
  if native:
   c.uc.mem_write(self.services,struct.pack('<QQ',0,self.callback));c.uc.mem_write(self.target_services,struct.pack('<QQ',0,self.target_callback));c.uc.mem_map(c.stack-0x10000,0x10000)
  else:
   c.uc.mem_write(self.callback,words(0xe12fff1e))
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ident(self,p):return self.identities.index(p) if p in self.identities else 99
 def ret(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def owner_index(self):
  c=self.c;p=struct.unpack('<Q',c.uc.mem_read(self.ai+8,8))[0] if self.native else word(c,self.ai+4);return self.owners.index(p)
 def snapshot(self):
  c=self.c
  if self.native:
   _,owner,_,group,active,_,timer0,timer1,_=struct.unpack('<6Q2IQ',c.uc.mem_read(self.ai,64));_,_,candidate,target,last,alive,sight,changed,_,_=struct.unpack('<5Q4BI',c.uc.mem_read(self.target,48));ow=[word(c,p+8)&65535 for p in self.target_owners];timer=[c.uc.mem_read(p+32*i+16,1)[0] for p in self.slots for i in range(3)]
  else:
   owner=word(c,self.ai+4);group=word(c,self.ai+0x34);active=word(c,self.ai+0x1c);timer0=word(c,self.ai+0x10);timer1=word(c,self.ai+0x14);candidate=word(c,self.ai+0x3c);target=word(c,self.ai+0x40);last=word(c,self.ai+0x44);alive,sight=c.uc.mem_read(self.ai+0x48,2);changed=c.uc.mem_read(self.ai+0x4c,1)[0];ow=[word(c,p+0x14d0)&65535 for p in self.actors];timer=[c.uc.mem_read(p+32*i+20,1)[0] for p in self.slots for i in range(3)]
  return [self.owners.index(owner),self.ident(group),self.ident(active),timer0,timer1,self.ident(candidate),self.ident(target),self.ident(last),alive,sight,changed,*ow,*timer]
 def change_owner(self,index):
  c=self.c;c.pointer(self.ai+(8 if self.native else 4),self.owners[index])
  if self.native:c.pointer(self.target+8,self.target_owners[index])
 def timer_bytes(self):
  result=[]
  for p in self.slots:
   values=[]
   for i in range(3):
    raw=bytes(self.c.uc.mem_read(p+32*i,32))
    if not self.native:
     _,ident,repeat,duration,elapsed,active,paused,pad,event,ref=struct.unpack('<IIiIIBBHiI',raw);raw=struct.pack('<IiIIBBHiQ',ident,repeat,duration,elapsed,active,paused,pad,event,ref)
    values.append(raw.hex())
   result.append(values)
  return result
 def mutate(self,op):
  x=self.config;c=self.c
  if op==x['mutate_op']:
   self.change_owner(x['next_owner']);c.pointer(self.ai+(32 if self.native else 0x1c),self.active[x['next_active']-1] if x['next_active'] else 0)
   if self.native:c.pointer(self.ai+40,self.vtables[x['next_active']-1] if x['next_active'] else 0)
   c.uc.mem_write(self.ai+(48 if self.native else 0x10),words(x['next_timer0'],x['next_timer1']))
  if op==x.get('reentry_op',-1) and not self.reentered:
   self.reentered=True;context=c.uc.context_save();stack=c.stack;trace=self.trace;stopped=self.stopped;resets=self.resets;self.trace=[];self.stopped=0;self.resets=0;c.stack-=0x10000
   try:
    if self.native:status=signed(c.invoke('dh2_character_ai_on_died',[self.out+64,self.ai,self.actors[1],self.target_services,self.services]));output=list(struct.unpack('<5Ii',c.uc.mem_read(self.out+64,24)))
    else:c.invoke(0x3d1000,[self.ai,self.actors[1]]);status=1;output=[13,sum(t[0]<7 for t in self.trace),self.stopped,self.resets,1,1]
    self.nested.append(dict(trace=self.trace,output=output,status=status))
   finally:c.stack=stack;c.uc.context_restore(context);self.trace=trace;self.stopped=stopped;self.resets=resets
 def event(self,op,subject=0,owner=0,payload=0,callee=0,operation=0,argument=0,text=''):
  self.trace.append([op,subject,owner,payload,callee,operation,argument,text,*self.snapshot()]);self.mutate(op)
 def hook(self,uc,address,size,_):
  c=self.c
  if self.native:
   if address==self.callback:
    op,arg,subject,owner,payload,callee,operation,reserved=struct.unpack('<II4QII',uc.mem_read(c.reg(2),48));assert not reserved;self.event(op,self.ident(subject),self.ident(owner),self.ident(payload),callee,operation,arg);self.ret();return
   if address==self.target_callback:
    op,reserved,subject,name=struct.unpack('<IIQQ',uc.mem_read(c.reg(2),24));assert not reserved;assert op in (0,1);text=string(c,name).decode() if name else '';self.event(20+op,text=text);uc.mem_write(c.reg(3),words(self.config['debug'] if op==1 else 0));self.ret();return
   if address==c.symbols['dh2_character_timer_stop']:
    store=self.stores.index(c.reg(0));ident=c.reg(1)&0xffffffff;self.event(30,self.ident(self.stores[store]),argument=ident);self.stopped+=ident<3
   if address==c.symbols['dh2_character_ai_sync_last_target']:self.resets=1
   return
  if address==0x3d6d40:self.resets=1
  if address==0x337888:self.event(20);self.ret();return
  if address==0x3140ec:c.uc.mem_write(c.reg(0),bytes(24));c.pointer(c.reg(0)+20,c.reg(1));self.ret(c.reg(0));return
  if address in (0x318254,0x708f00,0x310440):self.ret();return
  if address==0x337a88:self.event(21,text=string(c,word(c,c.reg(1)+20)).decode());self.ret(self.config['debug']);return
  if address==0x3db2d8:
   store=self.stores.index(c.reg(0));ident=c.reg(1);self.event(30,self.ident(self.stores[store]),argument=ident);self.stopped+=ident<3;return
  op={0x3d2628:0,self.callback:1,0x3c58c8:2,0x3d5fa8:3,0x3d6abc:4,0x3d8ae0:5,0x3d8a98:6}.get(address)
  if op is None:return
  subject=self.ident(c.reg(0));owner=self.ident(c.reg(1)) if op==0 else 0;payload=self.ident(c.reg(2)) if op==0 else self.ident(c.reg(1)) if op==1 else 0;callee=0x12345678 if op==1 else address;operation=0x24 if op==1 else 0;argument=c.reg(3) if op==2 else c.reg(1) if op==4 else 0
  self.event(op,subject,owner,payload,callee,operation,argument);self.ret()
 def fixture(self,x):
  self.config=x;self.trace=[];self.nested=[];self.stopped=self.resets=0;self.reentered=False;c=self.c
  for i,p in enumerate(self.actors):
   if self.native:
    c.uc.mem_write(self.owners[i],struct.pack('<3Q',p,p+0x4fc,self.stores[i]));c.uc.mem_write(self.target_owners[i],struct.pack('<QHHI',p,x['word'][i],0,0));c.uc.mem_write(self.stores[i],struct.pack('<QIIQII',self.slots[i],3,3,p,0,0))
   else:c.uc.mem_write(p,bytes(0x1800));c.uc.mem_write(p+0x14d0,struct.pack('<H',x['word'][i]));c.pointer(self.stores[i]+8,self.slots[i]);c.pointer(self.stores[i]+12,self.slots[i]+96)
   for j in range(3):
    raw=struct.pack('<IiIIBBHiQ',j,-1,100,99,x['timers'][i*3+j],0,0,0x33,0) if self.native else struct.pack('<IIiIIBBHiI',0,j,-1,100,99,x['timers'][i*3+j],0,0,0x33,0);c.uc.mem_write(self.slots[i]+32*j,raw)
   c.pointer(self.vtables[i]+(9*8 if self.native else 0x24),0x12345678 if self.native else self.callback)
   if not self.native:c.pointer(self.active[i],self.vtables[i])
  if self.native:
   c.uc.mem_write(self.ai,struct.pack('<6Q2IQ',self.ai,self.owners[x['owner']],self.target,self.groups[0] if x['group'] else 0,self.active[x['active']-1] if x['active'] else 0,self.vtables[x['active']-1] if x['active'] else 0,x['timer0'],x['timer1'],0));c.uc.mem_write(self.target,struct.pack('<5Q4BI',self.ai,self.target_owners[x['owner']],*[self.identities[v] for v in x['targets']],*x['bytes'],0,0))
  else:
   c.uc.mem_write(self.ai,bytes(0x100));c.pointer(self.ai+4,self.actors[x['owner']]);c.pointer(self.ai+0x34,self.groups[0] if x['group'] else 0);c.pointer(self.ai+0x1c,self.active[x['active']-1] if x['active'] else 0);c.uc.mem_write(self.ai+0x10,words(x['timer0'],x['timer1']));c.uc.mem_write(self.ai+0x3c,words(*(self.identities[v] for v in x['targets'])));c.uc.mem_write(self.ai+0x48,bytes(x['bytes'][:2]));c.uc.mem_write(self.ai+0x4c,bytes(x['bytes'][2:]))
  c.uc.mem_write(self.out,b'\xcc'*24)
def main():
 p=argparse.ArgumentParser();p.add_argument('--cases',type=int,default=1600);p.add_argument('--library',type=Path,default=REPO/'.local-inputs/character-ai-death-discovery/oracle.so');a=p.parse_args();start=time.monotonic();engine=REPO/'.local-inputs/libDungeonHunter2.so';scratch=REPO/'.local-inputs/character-ai-death-discovery';manifest=json.loads((scratch/'original-functions.json').read_text());old=Audit(engine,False,manifest);new=Audit(a.library,True,{'functions':[]});rng=random.Random(20261007);records=[];requests=0
 for i in range(a.cases):
  x=dict(owner=rng.randrange(2),group=int(i%3!=0),active=rng.randrange(3),timer0=rng.choice([0,1,2,3,0xffffffff]),timer1=rng.choice([0,1,2,3,0xffffffff]),word=[rng.randrange(65536) for _ in range(2)],timers=[rng.randrange(2) for _ in range(6)],targets=[rng.randrange(2,4) for _ in range(3)],bytes=[rng.randrange(256) for _ in range(3)],debug=rng.choice([0,1,255]),mutate_op=rng.choice([-1,0,1,2,3,5]),next_owner=rng.randrange(2),next_active=rng.randrange(3),next_timer0=rng.choice([0,1,2,0xffffffff]),next_timer1=rng.choice([0,1,2,0xffffffff]))
  wrapper=i%2==0
  if i%80==0:x.update(group=1,reentry_op=0,mutate_op=-1);wrapper=True
  old.fixture(x);new.fixture(x);old.c.invoke(0x3d1000 if wrapper else 0x3d6cdc,[old.ai,old.actors[1]] if wrapper else [old.ai]);status=signed(new.c.invoke('dh2_character_ai_on_died' if wrapper else 'dh2_character_ai_set_dead',[new.out,new.ai,new.actors[1],new.target_services,new.services] if wrapper else [new.out,new.ai,new.target_services,new.services]));expected=[13,sum(t[0]<7 for t in old.trace),old.stopped,1,1,1]
  assert status==1 and old.trace==new.trace and old.snapshot()==new.snapshot() and old.timer_bytes()==new.timer_bytes() and old.nested==new.nested and list(struct.unpack('<5Ii',new.c.uc.mem_read(new.out,24)))==expected,(i,x,old.trace,new.trace,old.snapshot(),new.snapshot(),old.nested,new.nested)
  records.append(dict(wrapper=wrapper,config=x,trace=old.trace,nested=old.nested,state=old.snapshot(),timers=old.timer_bytes(),output=expected));requests+=len(old.trace)+sum(len(n['trace']) for n in old.nested)
 guards=0
 for op in range(8):
  new.fixture(x);args=[new.out,new.ai,new.actors[1],new.target_services,new.services]
  if op==0:args[1]=0
  elif op==1:args[0]=new.ai
  elif op==2:args[1]+=1
  elif op==3:args[-1]=0
  elif op==4:new.c.uc.mem_write(new.ai+56,words(1,0))
  elif op==5:args[3]=0
  elif op==6:new.c.uc.mem_write(new.target+44,words(1))
  elif op==7:args[0]=new.slots[0]
  regions=[(new.out,24),(new.ai,64),(new.target,48)]+[(p,96) for p in new.slots];before=[bytes(new.c.uc.mem_read(p,n)) for p,n in regions];assert signed(new.c.invoke('dh2_character_ai_on_died',args))==-1 and not new.trace and before==[bytes(new.c.uc.mem_read(p,n)) for p,n in regions];guards+=1
 ref=ROOT/'reference/character-ai-death';ref.mkdir(parents=True,exist_ok=True);(ref/'original-functions.json').write_bytes((scratch/'original-functions.json').read_bytes());(ref/'original-functions.asm').write_bytes((scratch/'reference/original-functions.asm').read_bytes());gold=ref/'death-fixtures.json';gold.write_text(json.dumps(dict(records=records),separators=(',',':'))+'\n');build=json.loads(Path(str(a.library)+'.build.json').read_text(encoding='utf-8-sig'));sources=dict(build['source_sha256']);sources[str(Path(__file__).relative_to(REPO))]=sha(Path(__file__));report=dict(validation='PASS',scope=__doc__,original_sha256=sha(engine),arm64_library_sha256=sha(a.library),source_sha256=sources,comparisons=len(records),synchronous_reentry_cases=sum(bool(r['nested']) for r in records),ordered_requests=requests,atomic_guards=guards,mismatches=0,gold_sha256=sha(gold),full_group_state_aggro_skill_spell_bodies=False,packaged_APK=False,elapsed_seconds=round(time.monotonic()-start,2));(ROOT/'reports/character-ai-death-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
