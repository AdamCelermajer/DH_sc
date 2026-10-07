"""Original recursive preload/ordered unique instructions vs optimized ARM64.
Debug and string storage are required synchronous fixtures, not accepted FX
factory/rendering services. Real source GetModule default insertion is separately
executed by reference/character-skeleton-fx/probe.py.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/engine-animation/tests'))
from compiled_transforms_differential import Cpu,word
def words(values):return struct.pack('<'+'I'*len(values),*(v&0xffffffff for v in values))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Audit:
 def __init__(self,path,native,manifest):
  self.c=c=Cpu(path,native,manifest);self.native=native;d=c.data;self.manager=d+0x1000;self.table=d+0x2000;self.sets=d+0x3000;self.steps=d+0x4000;self.queue=d+0x5000;self.services=d+0x6000;self.callback=d+0x7000;self.trace=[]
  if native:c.uc.mem_write(self.services,struct.pack('<QQ',0,self.callback));c.uc.mem_map(c.stack-0x10000,0x10000)
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def string(self,p):return self.c.string(p).decode()
 def event(self,op,name):
  self.trace.append([op,name]);i=len(self.trace)-1;c=self.c
  if i==self.x['mutate']:
   if self.native:c.uc.mem_write(self.sets+8,words([1,0]));c.uc.mem_write(self.steps,struct.pack('<ii',11,0))
   else:c.uc.mem_write(self.sets+12,words([1]));c.uc.mem_write(self.steps+4,words([11]))
  if i==self.x['reentry'] and not self.reentered:
   self.reentered=True;context=c.uc.context_save();stack=c.stack;c.stack-=0x10000
   try:self.run(1,False)
   finally:c.stack=stack;c.uc.context_restore(context)
 def hook(self,uc,address,size,_):
  c=self.c
  if self.native:
   if address!=self.callback:return
   op=c.reg(1);name=self.string(c.reg(2))if c.reg(2)else '';out=c.reg(3);self.event(op,name);uc.mem_write(out,words([self.x['module']if op==2 else 0]));self.ret();return
  if address==0x3140ec:
   dst,src=c.reg(0),c.reg(1);uc.mem_write(dst,words([src,src+len(c.string(src)),0,0,0,dst]));self.ret(dst)
  elif address in (0x318254,0x3139ac):self.ret()
  elif address==0x337888:self.event(1,'');self.ret()
  elif address in (0x337ec8,0x337a88):
   op=2 if address==0x337ec8 else 3;name=self.string(word(bytes(uc.mem_read(c.reg(1),4)),0));self.event(op,name);self.ret(self.x['module']if op==2 else 0)
 def fixture(self,x):
  self.x=x;self.trace=[];self.reentered=False;c=self.c;c.uc.mem_write(self.queue,words(x['initial'])+bytes(256-len(x['initial'])*4))
  for i,steps in enumerate(x['sets']):
   ptr=self.steps+i*256
   if self.native:
    c.uc.mem_write(self.sets+i*16,struct.pack('<QiI',ptr,len(steps),0))
    for j,(effect,kind)in enumerate(steps):c.uc.mem_write(ptr+j*8,struct.pack('<ii',effect,kind))
   else:
    c.uc.mem_write(self.sets+i*24,words([0,0,0,len(steps),ptr,0]))
    for j,(effect,kind)in enumerate(steps):c.uc.mem_write(ptr+j*48,words([0,effect,0,0,0,0,0,kind,0,0,0,0]))
  if self.native:
   c.uc.mem_write(self.table,struct.pack('<QII',self.sets,3,16));c.uc.mem_write(self.manager,struct.pack('<QII',self.queue,len(x['initial']),64))
  else:
   c.uc.mem_write(self.manager,bytes(56));c.uc.mem_write(self.manager+16,words([self.queue,self.queue+len(x['initial'])*4,self.queue+256]));c.uc.mem_write(c.symbols['_ZN6Arrays19AnimatedEffectTable4sizeE'],words([3]));c.uc.mem_write(c.symbols['_ZN6Arrays19AnimatedEffectTable7membersE'],words([self.sets]));c.uc.mem_write(c.symbols['_ZN6Arrays10EffectDict4sizeE'],words([16]))
 def run(self,id=None,effect=None):
  c=self.c;id=self.x['id']if id is None else id;effect=self.x['effect']if effect is None else effect
  if self.native:return c.invoke('dh2_fx_register_effect'if effect else'dh2_fx_register_set',[self.table,self.manager,id&0xffffffff,self.services])
  c.invoke(0x496434 if effect else 0x4967e8,[self.manager,id&0xffffffff]);return 1
 def output(self):
  c=self.c;n=word(bytes(c.uc.mem_read(self.manager+8,4)),0)if self.native else(word(bytes(c.uc.mem_read(self.manager+20,4)),0)-self.queue)//4
  return list(struct.unpack('<'+'i'*n,c.uc.mem_read(self.queue,n*4)))
def main():
 p=argparse.ArgumentParser();p.add_argument('--cases',type=int,default=1000);p.add_argument('--library',type=Path,default=REPO/'.local-inputs/character-skeleton-fx-discovery/oracle.so');a=p.parse_args();ref=ROOT/'reference/character-skeleton-fx';manifest=json.loads((ref/'original-functions.json').read_text());old=Audit(REPO/'.local-inputs/libDungeonHunter2.so',False,manifest);new=Audit(a.library,True,{'functions':[]});rng=random.Random(77);records=[]
 for i in range(a.cases):
  x={'sets':[[[rng.choice([-1,0,3,7,15,16]),rng.choice([0,2,-1])]for _ in range(rng.randrange(5))],[[rng.choice([-1,0,3,7,15,16]),0]for _ in range(rng.randrange(5))],[]],'id':rng.choice([-1,0,0,1,2,3,15,16]),'effect':int(i%3==0),'module':int(i%7!=0),'initial':[3,9]if i%2 else[],'mutate':2 if i%17==0 else -1,'reentry':2 if i%31==0 else -1}
  if i%4==0:x['sets'][0].insert(0,[1,1])
  old.fixture(x);new.fixture(x);assert old.run()==new.run()==1;assert old.trace==new.trace and old.output()==new.output(),(i,x,old.trace,new.trace,old.output(),new.output());records.append(dict(config=x,trace=old.trace,output=old.output()))
 gold=ref/'preload-fixtures.json';gold.write_text(json.dumps(dict(records=records),separators=(',',':'))+'\n');binary=bytearray(b'FXP1'+words([len(records)]))
 for r in records:
  x=r['config'];binary+=words([x['id'],x['effect'],x['module'],x['mutate'],x['reentry'],len(x['initial']),len(r['trace']),len(r['output'])]);binary+=words(x['initial'])
  for steps in x['sets']:binary+=words([len(steps)]);binary+=b''.join(struct.pack('<ii',*s)for s in steps)
  for op,name in r['trace']:
   b=name.encode();binary+=words([op,len(b)])+b
  binary+=words(r['output'])
 (ref/'preload-fixtures.bin').write_bytes(binary);guards=0
 for i in range(4):
  new.fixture(x);args=[new.table,new.manager,x['id']&0xffffffff,new.services]
  if i==0:args[0]=0
  elif i==1:args[1]=0
  elif i==2:args[3]=0
  else:new.c.uc.mem_write(new.manager+8,words([65]))
  before=bytes(new.c.uc.mem_read(new.queue,256));assert new.c.invoke('dh2_fx_register_set',args)&0xffffffff==0xffffffff and not new.trace and before==bytes(new.c.uc.mem_read(new.queue,256));guards+=1
 build=json.loads(Path(str(a.library)+'.build.json').read_text(encoding='utf-8-sig'));sources=build['source_sha256'];sources[Path(__file__).relative_to(REPO).as_posix()]=sha(Path(__file__))
 report=dict(validation='PASS',original_sha256=manifest['original_sha256'],manifest_sha256=sha(ref/'original-functions.json'),arm64_library_sha256=sha(a.library),source_sha256=sources,comparisons=len(records),ordered_services=sum(len(r['trace'])for r in records),reentry_cases=sum(bool(r['config']['reentry']>=0 and len(r['trace'])>r['config']['reentry'])for r in records),atomic_guards=guards,mismatches=0,gold_sha256=sha(gold),binary_gold_sha256=sha(ref/'preload-fixtures.bin'),scope=__doc__);(ROOT/'reports/visual-fx-preload-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
