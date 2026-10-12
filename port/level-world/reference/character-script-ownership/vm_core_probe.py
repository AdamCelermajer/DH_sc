"""Actual original Lua constructor and library core; heap/libc/setjmp services explicit."""
import hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[2];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu as Base,string
HERE=Path(__file__).resolve().parent;ENGINE=REPO/'.local-inputs/libDungeonHunter2.so'
class Cpu(Base):
 def external(self,uc,a,size,unused):
  name=self.imports.get(a)
  if name in ('strlen','memcmp','strchr','_setjmp','setjmp'):
   self.import_calls[name]=self.import_calls.get(name,0)+1
   if name=='strlen':value=len(string(self,self.reg(0)))
   elif name=='memcmp':
    x=bytes(uc.mem_read(self.reg(0),self.reg(2)));y=bytes(uc.mem_read(self.reg(1),self.reg(2)));value=int(x>y)-int(x<y)
   elif name=='strchr':
    x=string(self,self.reg(0))+b'\0';index=x.find(bytes((self.reg(1)&255,)));value=0 if index<0 else self.reg(0)+index
   else:value=0
   self.put(0,value&0xffffffff);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,a,size,unused)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
manifest={'functions':[]}
for p in HERE.rglob('original-functions.json'):manifest['functions']+=json.loads(p.read_text())['functions']
c=Cpu(ENGINE,False,manifest);heap=c.data+0x100000;allocations=0;frees=0;bindings=[]
def w(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
def ret(x=0):c.put(0,x);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,a,size,unused):
 global heap,allocations,frees
 if a==0x310500:
  opaque,old,oldsz,newsz=[c.reg(i) for i in range(4)]
  assert opaque==0 and newsz<0x100000
  if not newsz:frees+=int(bool(old));ret(0)
  elif old and newsz<=oldsz:ret(old)
  else:
   ptr=heap;heap+=(newsz+15)&~15;assert heap<c.data+0x1f00000
   uc.mem_write(ptr,bytes(newsz));allocations+=1
   if old:uc.mem_write(ptr,bytes(uc.mem_read(old,min(oldsz,newsz))));frees+=1
   ret(ptr)
 elif a==0x31167c:ret(c.reg(0))
 elif a==0x31a4d4:bindings.append(string(c,c.reg(1)).decode());ret()
c.uc.hook_add(UC_HOOK_CODE,hook)
rows=[]
for ctor in (0x31b268,0x31b2a8):
 obj=c.data+0x1000;c.uc.mem_write(obj,bytes(64));c.invoke(ctor,[obj]);state=w(obj+4)
 assert state and c.uc.mem_read(obj+8,1)==b'\x01'
 assert c.invoke('lua_gettop',[state])==0
 row={'constructor':hex(ctor),'state':hex(state),'library_steps':[]}
 row['absent_before_open']=[]
 for name in ('_G','math','table','string'):
  address=c.data+0x1a000;c.uc.mem_write(address,name.encode()+b'\0')
  c.invoke('lua_getfield',[state,-10002,address]);assert c.invoke('lua_type',[state,-1])==0
  c.invoke('lua_settop',[state,0]);row['absent_before_open'].append(name)
 for fn,expected in ((0x31b010,2),(0x31b000,3),(0x31aff8,4),(0x31b008,5)):
  returned=c.invoke(fn,[obj]);top=c.invoke('lua_gettop',[state]);assert top==expected
  row['library_steps'].append(dict(function=hex(fn),returned=returned,top=top))
 if ctor==0x31b268:
  row['repeat_library_steps']=[]
  for fn,expected in ((0x31b010,7),(0x31b000,8),(0x31aff8,9),(0x31b008,10)):
   returned=c.invoke(fn,[obj]);top=c.invoke('lua_gettop',[state]);assert top==expected
   row['repeat_library_steps'].append(dict(function=hex(fn),returned=returned,top=top))
 c.invoke(0x31b180,[obj]);rows.append(row)
obj=c.data+0x4000;c.uc.mem_write(obj,bytes(0x1000));c.invoke(0x3d8fb0,[obj,1]);state=w(obj+8)
assert c.invoke('lua_gettop',[state])==0 and not bindings
c.invoke(0x3d8f2c,[obj]);assert c.invoke('lua_gettop',[state])==5 and len(bindings)==35
rows.append(dict(constructor='CharAIScript(true)',state=hex(state),pre_bind_stack=0,post_bind_stack=5,registered_names=bindings.copy(),registration_bodies_executed=False))
c.invoke(0x3d926c,[obj])
report=dict(validation='PASS',original_sha256=sha(ENGINE),probe_sha256=sha(Path(__file__)),manifest_sha256={str(p.relative_to(HERE)):sha(p) for p in HERE.rglob('original-functions.json')},original_instructions_executed=True,lua_newstate_core_executed=True,lua_library_cores_executed=True,lua_close_core_executed=True,rows=rows,heap_allocations=allocations,heap_frees=frees,imports=c.import_calls,executed_captured_functions=sorted({c.address_owner[a] for a in c.seen}),scope=__doc__,boundaries=['LuaAllocator heap services (growing allocation/copy/shrink/free)','libc memcpy/memset/strlen/memcmp/strchr','setjmp success continuation only','string reserve','Binder.bindFunction registration service'])
(HERE/'vm-core-probe.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(dict(validation='PASS',rows=len(rows),library_stack=[x['top'] for x in rows[0]['library_steps']],allocations=allocations,frees=frees)))
