"""Original/optimized ARM64 object push with identical explicit Lua primitives.

Original virtual type/createBindings bodies and native source catalog execute;
Lua API storage/allocation are fixture services. Actual VM audited separately.
"""
import hashlib,json,struct,random
from pathlib import Path
from unicorn import UC_HOOK_CODE
from gameobject_lua_original import Probe,Table,REF,REPO,words
from visual_timeline_differential import TimelineCpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Native(Probe,TimelineCpu):
 def __init__(self,library):
  TimelineCpu.__init__(self,library,True,{'functions':[]})
  self.stack_values=[];self.registry={};self.trace=[];self.bindings=[];self.names={};self.heap=self.data+0x100000;self.active=False
  self.services=self.data+0x4000;self.callback_type=self.data+0x4100;self.callback_methods=self.data+0x4200;self.out=self.data+0x4300
  self.uc.mem_write(self.services,struct.pack('<4Q',0,self.callback_type,self.callback_methods,self.callback_methods+64))
  for p in (self.callback_type,self.callback_methods):self.uc.mem_write(p,bytes.fromhex('c0035fd6'))
  self.uc.hook_add(UC_HOOK_CODE,self.hook)
 def external(self,uc,address,size,_):
  if self.imports.get(address,'').startswith('lua'):return
  if self.imports.get(address)=='strlen':self.returned(len(self.text(self.reg(0))));return
  return TimelineCpu.external(self,uc,address,size,_)
 def nested(self,fn,args):
  saved=self.uc.context_save();stack=self.stack;self.stack=self.uc.reg_read(self.sp)-0x10000
  try:return self.invoke(fn,args)
  finally:self.stack=stack;self.uc.context_restore(saved)
 def trace_value(self,v):
  if isinstance(v,tuple) and v[0]=='identity' and v[1]==self.data+0x5000:return ['identity',0x2005000]
  return Probe.trace_value(self,v)
 def hook(self,uc,a,size,_):
  if not self.active:return
  if a==self.callback_type:
   output=self.reg(2);assert not self.nested('dh2_gameobject_lua_type',[output,int(self.identity==2)]);self.returned();return
  if a==self.callback_methods:
   output,count=self.reg(2),self.reg(3);assert not self.nested('dh2_gameobject_lua_methods',[output,count,int(self.identity==2)]);self.returned();return
  name=self.imports.get(a,'')
  if name=='lua_newuserdata':
   size=self.reg(1);p=self.heap;self.heap+=(size+15)&~15;uc.mem_write(p,bytes(size));self.stack_values.append(('userdata',p));self.returned(p);return
  if name=='lua_remove':
   i=self.reg(1)&0xffffffff;i=i if i<0x80000000 else i-0x100000000;del self.stack_values[i-1 if i>0 else i];self.returned();return
  if name=='lua_pushcclosure':
   assert self.reg(2)==1;value=self.stack_values.pop();assert value[0]=='userdata';address=self.word(value[1]+16);self.stack_values.append(('closure',0x319fd0,[('identity',address)]));self.bindings.append([self.pending_name,address]);self.returned();return
  if name=='lua_pushstring':self.pending_name=self.text(self.reg(1))
  return Probe.hook(self,uc,a,size,_)
 def push(self,kind,identity=0):
  self.identity=identity;self.active=True
  try:
   assert kind in (0,2,7)
   if kind==7:self.invoke('dh2_script_object_push',[1234,0,self.services,self.data+0x5000 if identity else 0],budget=1000000)
   else:
    # Only source object producer is compared here. Generic scalar primitive
    # alternatives are direct Lua calls and remain historical runtime scope.
    self.stack_values.append(None if kind==0 else ('identity',self.data+0x5000))
  finally:self.active=False
  return self.stack_values.pop()
def main():
 lib=REPO/'.local-inputs/gameobject-lua-representation/libgameobject_lua.so';build=json.loads((lib.parent/'arm64-build.json').read_text(encoding='utf-8-sig'))
 assert sha(lib)==build['library_sha256'];assert all(sha(REPO/k)==v for k,v in build['source_bindings'].items())
 old=Probe();new=Native(lib);cases=0;registrations=0
 for identity in [0,1,1,2,2]+[i%3 for i in range(128)]:
  for machine in (old,new):machine.trace=[];machine.bindings=[]
  a=old.push(7,identity);b=new.push(7,identity)
  assert old.trace_value(a)==new.trace_value(b),(cases,old.trace_value(a),new.trace_value(b))
  assert old.trace==new.trace and old.bindings==new.bindings,(cases,old.trace,new.trace,old.bindings,new.bindings)
  cases+=1;registrations+=len(old.bindings)
 assert old.registry.keys()==new.registry.keys()
 wrappers=0;rng=random.Random(20261004)
 for identity in [0,1,0x7fffffff,0x80000000,0xffffffff]+[rng.getrandbits(32) for _ in range(128)]:
  for address,fn in [(0x38ebe4,'dh2_gameobject_lua_get_id'),(0x3b6c7c,'dh2_gameobject_lua_get_target')]:
   old.method_function=address;old.method_result=[];old.active=True;old.pointer(old.data+0x5408,identity)
   old.invoke(address,[old.data+0x8100,old.data+0x8200,old.data+0x5000 if address==0x3b6c7c else identity]);old.active=False;old.method_function=0
   output=new.data+0x8100;count=new.data+0x8200;state=new.data+0x8300
   new.uc.mem_write(state,struct.pack('<5Q4BI',0,0,0,identity,0,0,0,0,0,0))
   assert not new.invoke(fn,[output,count,state if address==0x3b6c7c else identity])
   actual=[new.word(output),struct.unpack('<Q',new.uc.mem_read(output+32,8))[0]]
   assert actual==old.method_result and new.word(count)==1,(address,actual,old.method_result)
   wrappers+=1
 report=dict(validation='PASS',producer_cases=cases,source_ordered_registrations=registrations,effective_methods=sum(len(v['__index']) for v in old.registry.values()),mismatches=0,original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),compiler_inputs=build,manifest_sha256=sha(REF/'original-functions.json'),producer_fixture_sha256=sha(REF/'object-producer-probe.json'),source_sha256={str(Path(__file__).relative_to(REPO)):sha(Path(__file__)),str((Path(__file__).parent/'gameobject_lua_original.py').relative_to(REPO)):sha(Path(__file__).parent/'gameobject_lua_original.py')},scope=__doc__,whole_original_VM=False)
 report['wrapper_cases']=wrappers
 (REF.parents[1]/'reports/gameobject-lua-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('validation','producer_cases','wrapper_cases','source_ordered_registrations','effective_methods','mismatches')}))
if __name__=='__main__':main()
