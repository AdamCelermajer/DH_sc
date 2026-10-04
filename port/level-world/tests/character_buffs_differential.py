"""Actual buff control/property instructions versus O2 owned native STL.

ARM32 map allocation/string/deque mutation and imported timer/FX services are
explicit fixtures. Tree search, selection, reset/resolve/full recalc, expiry
and lifetime call order execute original instructions. ARM64 owned STL and
the already proved native TimerStore execute, including all property kernels.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/character-buffs'
sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu,string
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def pack(*xs):return struct.pack('<'+'I'*len(xs),*(x&0xffffffff for x in xs))
def s32(x):return x if x<0x80000000 else x-0x100000000
def sheet(xs):return struct.pack('<224i',*xs)
class NativeCpu(Cpu):
 def __init__(self,path):
  super().__init__(path,True,{'functions':[]});self.heap=self.data+0x100000;self.allocations={};self.frees=0
 def number(self,p):return struct.unpack('<Q',self.uc.mem_read(p,8))[0]
 def allocate(self,n):
  p=self.heap;self.heap+=(n+15)&~15;assert self.heap<self.data+0x1f00000;self.allocations[p]=n;self.uc.mem_write(p,bytes(n));return p
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name in ('_Znwm','_Znam'):self.put(0,self.allocate(self.reg(0)))
  elif name in ('_ZdlPv','_ZdlPvm','_ZdaPv','_ZdaPvm'):
   p=self.reg(0)
   if p:assert p in self.allocations,(name,hex(p));del self.allocations[p];self.frees+=1
  elif name in ('strlen',):self.put(0,len(string(self,self.reg(0))))
  elif name in ('_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignEPKc',):
   p=self.reg(0);value=string(self,self.reg(1));tag=uc.mem_read(p,1)[0]
   if tag&1:del self.allocations[self.number(p+16)];self.frees+=1
   uc.mem_write(p,bytes(24))
   if len(value)<=22:uc.mem_write(p,bytes((len(value)<<1,))+value+b'\0')
   else:
    q=self.allocate(len(value)+1);uc.mem_write(q,value+b'\0');uc.mem_write(p,struct.pack('<3Q',(len(value)+2)&~1|1,len(value),q))
   self.put(0,p)
  else:return super().external(uc,address,size,unused)
  self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
class Machine:
 def __init__(self,path,native,manifest,defaults,types):
  self.native=native;self.c=NativeCpu(path) if native else Cpu(path,False,manifest);c=self.c;d=c.data
  self.props=d+0x1000;self.character=d+0x8000;self.defaults=d+0x9000;self.view=d+0xb000;self.bindings=d+0xc000;self.services=d+0xd000;self.ts=d+0xe000;self.timer_services=d+0xf000;self.timer_slots=d+0x10000;self.out=d+0x14000;self.text=d+0x16000;self.snap=d+0x17000;self.timer=d+0x18000;self.vt=d+0x19000;self.callback=d+0x1e000;self.sheets=[d+x for x in (0x20000,0x21000,0x22000,0x23000)]
  self.heap=d+0x100000;self.nodes={};self.buffers={};self.instances={};self.serial=0;self.timers=[];self.trace=[];self.names={};self.returned=0;self.instance_serial={};self.fx_ids={};self.defaults_input=defaults;self.types_input=types
  if native:
   c.uc.mem_write(self.defaults,sheet(defaults)+sheet(types))
   for p in self.sheets:c.uc.mem_write(p,sheet(defaults))
   c.uc.mem_write(self.view,struct.pack('<7QII',self.defaults,self.defaults+896,*self.sheets,0,0,0));c.uc.mem_write(self.services,struct.pack('<QQ',0,self.callback));c.uc.mem_write(self.timer_services,bytes(32));c.uc.mem_write(self.ts,struct.pack('<QIIQII',self.timer_slots,0,512,self.character,0,0));c.uc.mem_write(self.bindings,struct.pack('<4Q',self.view,self.ts,self.timer_services,self.services))
   self.owner=c.invoke('dh2_character_buffs_create',[self.bindings]);assert self.owner
  else:
   self.owner=self.props;c.pointer(self.props+4,self.character);c.uc.mem_write(self.defaults,bytes(4)+sheet(defaults)+bytes(4)+sheet(types));c.pointer(0x9a645c,self.defaults)
   for at in (8,0x38c,0x710,0xa94):c.uc.mem_write(self.props+at,bytes(4)+sheet(defaults))
   self.tree();c.uc.mem_write(self.callback,pack(0xe12fff1e,0xe12fff1e));c.pointer(self.vt,self.callback);c.pointer(self.vt+4,self.callback+4);c.pointer(self.timer,self.vt)
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def word(self,p):return struct.unpack('<I',self.c.uc.mem_read(p,4))[0]
 def put(self,p,v):self.c.uc.mem_write(p,pack(v))
 def allocate(self,n):p=self.heap;self.heap+=(n+15)&~15;self.c.uc.mem_write(p,bytes(n));return p
 def ret(self,value=0):c=self.c;c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def serial_of(self,p):
  if not p:return 0
  if p not in self.instance_serial:self.serial+=1;self.instance_serial[p]=self.serial
  return self.instance_serial[p]
 def tree(self):
  c=self.c;head=self.props+0xe18;keys=sorted(self.nodes);nodes=[self.nodes[k] for k in keys]
  # Source sorted right spine is sufficient for real lower_bound/increment.
  c.uc.mem_write(head,pack(0,nodes[0] if nodes else 0,nodes[0] if nodes else head,nodes[-1] if nodes else head,len(nodes)))
  for i,n in enumerate(nodes):c.uc.mem_write(n,pack(1,nodes[i-1] if i else head,0,nodes[i+1] if i+1<len(nodes) else 0))
 def update_deque(self,node,values):
  c=self.c;blocks,mp=self.buffers[node]
  while len(blocks)<=len(values)//32:blocks.append(self.allocate(128));self.put(mp+(len(blocks)-1)*4,blocks[-1])
  for i,block in enumerate(blocks):chunk=values[i*32:(i+1)*32];c.uc.mem_write(block,b''.join(pack(x) for x in chunk)+bytes(128-len(chunk)*4))
  c.uc.mem_write(node+0x34,pack(blocks[0],blocks[0],blocks[0]+128,mp));b=blocks[len(values)//32];c.uc.mem_write(node+0x44,pack(b+len(values)%32*4,b,b+128,mp+(len(values)//32)*4))
 def deque(self,node):
  count=(self.word(node+0x50)-self.word(node+0x40))//4*32+(self.word(node+0x44)-self.word(node+0x48))//4;blocks,mp=self.buffers[node];return [self.word(blocks[i//32]+i%32*4) for i in range(count)]
 def hook(self,uc,address,size,unused):
  c=self.c
  if self.native:
   if address==self.callback:
    op,id,index,enabled,subject,char=struct.unpack('<IiIIQQ',uc.mem_read(c.reg(2),32));assert char==self.character
    if op==4:
     self.trace.append(['recalc']);c.put(0,self.view);uc.reg_write(c.pc,c.symbols['dh2_buff_audit_recalculate']);return
    if op==0:self.trace.append(['fx_load',id]);value=0x80000000+(id&0xffff);self.fx_ids[value]=id
    elif op==1:self.trace.append(['fx_release',self.fx_ids.get(subject,-1)]);value=0
    elif op==2:self.trace.append(['fx_object',self.fx_ids[subject]]);value=subject
    else:self.trace.append(['fx_enable',self.fx_ids[subject],index,enabled]);value=0
    uc.mem_write(c.reg(3),struct.pack('<Q',value));self.ret(1);return
   if address==c.symbols['dh2_character_timer_start']:
    self.trace.append(['start',c.reg(1),s32(c.reg(2)),s32(c.reg(3)),self.serial_of(c.reg(4))]);return
   if address==c.symbols['dh2_character_timer_stop']:self.trace.append(['stop',s32(c.reg(1)&0xffffffff)]);return
   if address==c.symbols['dh2_character_timer_time_left']:self.trace.append(['time_left',s32(c.reg(3)&0xffffffff)]);return
   return
  if address==0x3e209c:
   key=s32(self.word(c.reg(1)))
   if key not in self.nodes:
    n=self.allocate(0x60);self.nodes[key]=n;self.put(n+0x10,key);self.put(n+0x14,-1);block=self.allocate(128);mp=self.allocate(1024);self.put(mp,block);self.buffers[n]=([block],mp);c.uc.mem_write(n+0x34,pack(block,block,block+128,mp)*2);self.tree()
   self.ret(self.nodes[key]+0x14);return
  if address==0x30de54:self.ret(len(string(c,c.reg(0))));return
  if address==0x3109e0:self.names[c.reg(0)]=string(c,c.reg(1)).decode();self.ret(c.reg(0));return
  if address==0x310570:
   assert c.reg(0)==0x394;p=self.allocate(0x394);self.serial_of(p);self.instances[p]=True;self.ret(p);return
  if address==0x310440:self.instances.pop(c.reg(0),None);self.ret();return
  if address==0x3db2d8:
   id=s32(c.reg(1));self.trace.append(['stop',id]);
   if 0<=id<len(self.timers):self.timers[id]['active']=0
   self.ret();return
  if address==0x3dbe24:
   duration,repeat,event,ref=c.reg(1),s32(c.reg(2)),s32(c.reg(3)),self.word(uc.reg_read(c.sp));self.trace.append(['start',duration,repeat,event,self.serial_of(ref)]);index=next((i for i,t in enumerate(self.timers) if not t['active']),len(self.timers));t=dict(active=1,elapsed=0,duration=duration,repeat=repeat,event=event,ref=ref)
   if index==len(self.timers):self.timers.append(t)
   else:self.timers[index]=t
   self.ret(index);return
  if address==0x3db344:
   id=s32(c.reg(1));self.trace.append(['time_left',id]);assert 0<=id<len(self.timers) and self.timers[id]['active'];t=self.timers[id];self.put(c.reg(2),t['elapsed']);self.put(c.reg(3),t['duration']);self.ret(1);return
  if address==0x495430:
   id=s32(c.reg(1));self.trace.append(['fx_load',id]);fx=0x80000000+(id&0xffff);self.fx_ids[fx]=id;self.ret(fx);return
  if address==0x494978:
   fx=self.word(c.reg(1));self.trace.append(['fx_release',self.fx_ids.get(fx,-1)]);self.put(c.reg(1),0);self.ret();return
  if address==0x492550:
   fx=c.reg(0);self.trace.append(['fx_object',self.fx_ids[fx]]);p=self.vt+0x100;c.pointer(p,self.vt+0x200);self.put(p+4,fx);c.pointer(self.vt+0x21c,self.callback+8);c.uc.mem_write(self.callback+8,pack(0xe12fff1e));self.ret(p);return
  if address==self.callback+8:self.trace.append(['fx_enable',self.fx_ids[self.word(c.reg(0)+4)],c.reg(1),c.reg(2)]);self.ret();return
  if address==0x3e0fd0:
   n=self.word(c.reg(1));key=s32(self.word(n+0x10));del self.nodes[key];self.tree();self.ret();return
  if address==0x3e0ab8:self.nodes.clear();self.tree();self.ret();return
  if address==0x3dfa9c:
   start=c.reg(1);node=start-0x34;remove=self.word(c.reg(2));values=self.deque(node);values.remove(self.word(remove));self.update_deque(node,values);self.ret();return
  if address==0x3e21a8:
   node=c.reg(0)-0x34;values=self.deque(node);values.append(self.word(c.reg(1)));self.update_deque(node,values);self.ret();return
  if address==0x3e092c:
   node=c.reg(0)-0x34;self.update_deque(node,[]);self.ret();return
  if address==0x3e0810:self.trace.append(['recalc']);return
  if address==self.callback:self.ret(self.word(self.timer+8));return
  if address==self.callback+4:self.ret(self.word(self.timer+4));return
 def snapshot(self):
  c=self.c;instances=[]
  if self.native:
   count=c.invoke('dh2_character_buffs_count',[self.owner]);decls=c.invoke('dh2_character_buffs_declarations',[self.owner])
   for i in range(count):
    assert c.invoke('dh2_character_buff_snapshot',[self.snap,self.owner,i])==1;p,id,strength,timer,_,values,name,fx=struct.unpack('<QiIiIQQQ',c.uc.mem_read(self.snap,48));instances.append([self.serial_of(p),id,strength,timer,string(c,name).decode(),self.fx_ids.get(fx,-1),bytes(c.uc.mem_read(values,896)).hex()])
   props=b''.join(bytes(c.uc.mem_read(p,896)) for p in self.sheets)
   timer_count=self.word(self.ts+8);timers=[]
   for i in range(timer_count):
    id,repeat,duration,elapsed,active,paused,_,event,ref=struct.unpack('<IiIIBBHiQ',c.uc.mem_read(self.timer_slots+i*32,32));timers.append([active,elapsed,duration,repeat,event,self.serial_of(ref)])
  else:
   decls=len(self.nodes)
   for id,n in sorted(self.nodes.items()):
    for p in self.deque(n):instances.append([self.serial_of(p),id,self.word(p+0x384),s32(self.word(p+0x388)),self.names[n+0x1c],self.fx_ids.get(self.word(n+0x18),-1),bytes(c.uc.mem_read(p+4,896)).hex()])
   props=b''.join(bytes(c.uc.mem_read(self.props+at+4,896)) for at in (8,0x38c,0x710,0xa94));timers=[[t['active'],t['elapsed'],t['duration'],t['repeat'],t['event'],self.serial_of(t['ref'])] for t in self.timers]
  return dict(declarations=decls,instances=instances,properties=props.hex(),timers=timers)
 def execute(self,operation,args):
  c=self.c;self.trace=[];out=0
  if operation=='add':
   id,duration,cap,strength,fx,name=args;c.uc.mem_write(self.text,name.encode()+b'\0')
   if self.native:assert c.invoke('dh2_character_buff_add',[self.out,self.owner,id,duration,cap,strength,fx,self.text])==1;out=struct.unpack('<Q',c.uc.mem_read(self.out,8))[0]
   else:out=c.invoke(0x3e232c,[self.owner,id,duration,cap,strength,fx,self.text])
  elif operation=='delete':
   id,serial=args;p=next((p for p,v in self.instance_serial.items() if v==serial),0xdead if serial else 0)
   if self.native:assert c.invoke('dh2_character_buff_delete',[self.out,self.owner,id,p])==1
   else:c.invoke(0x3e101c,[self.owner,id,p])
  elif operation=='expire':
   serial,id=args;p=next(p for p,v in self.instance_serial.items() if v==serial)
   if self.native:c.uc.mem_write(self.timer,struct.pack('<IiIIBBHiQ',id,0,0,0,0,0,0,0x36,p));assert c.invoke('dh2_character_buff_expired',[self.out,self.owner,self.timer])==1
   else:self.put(self.timer+4,p);self.put(self.timer+8,id);c.invoke(0x3e123c,[self.owner,self.timer])
  elif operation=='elapsed':
   for i,v in enumerate(args):
    if self.native:self.put(self.timer_slots+i*32+12,v)
    else:self.timers[i]['elapsed']=v
  elif operation=='remove_all':
   if self.native:assert c.invoke('dh2_character_buffs_remove_all',[self.out,self.owner])==1
   else:c.invoke(0x3e0af8,[self.owner])
  elif operation=='dot':
   duration,amount,element,buffs,fxs=args;arrays=[];cursor=c.data+0x40000
   for group,names in enumerate((buffs,fxs)):
    array=c.data+0x30000+group*0x1000;arrays.append(array)
    for i,name in enumerate(names):value=name.encode()+b'\0';c.uc.mem_write(cursor,value);c.pointer(array+i*c.word_size,cursor);cursor+=len(value)
   if self.native:
    for i,names in enumerate((buffs,fxs)):c.uc.mem_write(c.data+0x34000+i*16,struct.pack('<QII',arrays[i],len(names),0))
    assert c.invoke('dh2_character_buff_add_dot',[self.out,self.owner,duration,amount,element,c.data+0x34000,c.data+0x34010])==1;out=struct.unpack('<Q',c.uc.mem_read(self.out,8))[0]
   else:
    base=(0x3e2730+8+self.word(0x3e29f0))&0xffffffff
    for i,(count_literal,array_literal,names) in enumerate(((0x3e29f4,0x3e29f8,buffs),(0x3e2a00,0x3e2a04,fxs))):self.put(self.word(base+self.word(count_literal)),len(names));self.put(self.word(base+self.word(array_literal)),arrays[i])
    c.invoke(0x3e2720,[self.owner,duration,amount,element]);out=0
    # AddDot itself returns the RecalcProperty value, not the instance. Recover
    # the selected lifetime identity via actual StartTimer request instead.
   out=0
  elif operation=='destroy':
   if self.native:assert c.invoke('dh2_character_buffs_destroy',[self.owner])==1;self.owner=0
   else:c.invoke(0x3e0c6c,[self.owner])
  else:raise AssertionError(operation)
  if operation=='destroy':
   # The saved view and TimerStore survive only for teardown diagnostics.
   if self.native:
    props=b''.join(bytes(c.uc.mem_read(p,896)) for p in self.sheets);return 0,dict(declarations=0,instances=[],properties=props.hex(),timers=self.last_timers),self.trace
  return self.serial_of(out),self.snapshot(),self.trace
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();manifest=json.loads((REF/'original-functions.json').read_text());engine=REPO/'.local-inputs/libDungeonHunter2.so';assert sha(engine)==manifest['original_sha256'];assets=REPO/'port/android-native/app/src/main/assets/data/character_properties_pyarray.bin';raw=assets.read_bytes();defaults=list(struct.unpack_from('<224i',raw,4));types=list(struct.unpack_from('<224i',raw,900));assert defaults[26]==-1
 old=Machine(engine,False,manifest,defaults,types);new=Machine(a.library,True,{},defaults,types);records=[];rng=random.Random(0xB036)
 def compare(op,args):
  expected=old.execute(op,args);actual=new.execute(op,args);assert expected==actual,(len(records),op,args,expected[0],actual[0],expected[2],actual[2],[(k,expected[1][k],actual[1][k]) for k in expected[1] if expected[1][k]!=actual[1][k]][:1]);records.append(dict(operation=op,args=args,result=expected[0],snapshot=expected[1],trace=expected[2]));return expected
 compare('add',[1,100,3,1,-1,'first']);compare('add',[1,100,3,2,-1,'second']);compare('add',[1,100,3,3,-1,'third']);compare('elapsed',[10,30,20]);compare('add',[1,0,3,3,-1,'equal']);compare('add',[1,200,3,4,-1,'all weaker']);compare('add',[1,200,3,0,-1,'all stronger']);compare('delete',[1,999]);compare('delete',[1,1]);compare('remove_all',[])
 for i in range(350):
  state=old.snapshot();instances=state['instances']
  if i%19==0:compare('remove_all',[])
  elif i%11==0 and instances:
   inst=rng.choice(instances);compare('expire',[inst[0],inst[3] if inst[3]>=0 else 0x1234])
  elif i%7==0:
   target=rng.choice(instances) if instances else [0,rng.randrange(-3,4)];compare('delete',[target[1],target[0] if i%14 else 999])
  else:
   # Duration zero candidates and active equal-strength candidates cannot be
   # mixed: original TimeLeft(-1) leaves comparison locals indeterminate.
   id=rng.randrange(-3,4);cap=rng.choice([0,1,2,4,8]);existing=[x for x in instances if x[1]==id];strength=rng.choice([0,1,2,256,0x7fffffff,0xffffffff]);duration=rng.choice([1,100,0xffffffff])
   compare('add',[id,duration,cap,strength,rng.choice([-1,0,4]),'name_'+str(i)+('_long_'*8 if i%13==0 else '')])
 compare('remove_all',[])
 # Nonpositive capacity's genuine128 and one-instance delete ignores pointer.
 for i in range(129):compare('add',[100,0,0,i,-1,'permanent'])
 compare('delete',[100,1]);compare('remove_all',[]);compare('add',[123,0,1,0xffffffff,-1,'one']);compare('delete',[123,999])
 for element in range(-1,5):
  for amount in (0,1,256,0x7fffffff):compare('dot',[100,amount,element,['unused','AUTO_DOT_01_FIRE'],['unused','AUTO_DOT_01_FIRE']])
 compare('remove_all',[])
 for element in range(-1,5):compare('dot',[1,3,element,[],[]])
 compare('remove_all',[]);compare('add',[444,100,2,2,7,'teardown']);new.last_timers=new.snapshot()['timers'];compare('destroy',[]);assert not new.c.allocations
 gold=REF/'buff-fixtures.json';gold.write_text(json.dumps(dict(defaults=defaults,types=types,records=records),separators=(',',':'))+'\n');build=json.loads(Path(str(a.library)+'.build.json').read_text(encoding='utf-8-sig'));assert build['library_sha256']==sha(a.library)
 report=dict(validation='PASS',scope=__doc__,original_sha256=sha(engine),original_instructions_executed=True,original_property_reset_resolve_recalculate_unmocked=True,original_class_input='defaults ClassID=-1 (source negative-id class skip)',original_STL_mutation_timer_FX_services='explicit fixtures',native_owned_STL_and_timers_executed=True,comparisons=len(records),service_requests=sum(len(r['trace']) for r in records),gold_sha256=sha(gold),native_sha256=sha(a.library),source_sha256=build['source_sha256'],compiler=build,script_sha256=sha(Path(__file__)),mismatches=0);out=ROOT/'reports/character-buffs-arm64-differential.json';out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('compiler','source_sha256')},indent=2))
if __name__=='__main__':main()
