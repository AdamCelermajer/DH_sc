"""Original Include/AddFile/loadFile choreography; VM/STL/cache operations explicit services."""
import hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
from elftools.elf.elffile import ELFFile
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from character_script_selection_differential import Cpu as Base,string
class Cpu(Base):
 def external(self,uc,a,size,unused):
  if self.imports.get(a)=='strlen':
   self.put(0,len(string(self,self.reg(0))));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,a,size,unused)
ENGINE=ROOT/'.local-inputs/libDungeonHunter2.so'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
manifest=json.loads((HERE/'original-functions.json').read_text());c=Cpu(ENGINE,False,manifest)
d=c.data;owner=d+0x1000;args=d+0x2000;vec=d+0x3000;values=d+0x4000;text=d+0x8000
manager=d+0x10000;node=d+0x11000;stream=d+0x12000;vt=d+0x13000;err=d+0x14000
instance=d+0x15000;state=d+0x16000;errtext=d+0x17000;strings=d+0x30000
mode='';trace=[];rows=[];loaded=False;load_status=0;call_status=0;stack=[];nexttext=strings
def w(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
def ret(x=0):c.put(0,x);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def putstring(obj,data):
 global nexttext
 p=nexttext;nexttext+=len(data)+16;c.uc.mem_write(p,data+b'\0');c.uc.mem_write(obj,bytes(24))
 c.pointer(obj+0x10,p+len(data));c.pointer(obj+0x14,p);return p
def hook(uc,a,size,unused):
 global loaded
 if mode=='include' and a==0x37b574:
  trace.append(dict(service='LuaScript.Load',script=c.reg(0),name=string(c,c.reg(1)).hex()));ret(load_status)
 elif mode=='manager':
  if a==0x3338cc:
   putstring(c.reg(0),string(c,w(c.reg(1)+0x14))+string(c,c.reg(2)));ret(c.reg(0))
  elif a==0x30ebd4:
   first=string(c,c.reg(0));needle=string(c,c.reg(1));i=first.find(needle);ret(0 if i<0 else c.reg(0)+i)
  elif a==0x379ef8:
   obj=c.reg(0);putstring(obj,string(c,w(obj+0x14))+string(c,c.reg(1)));ret(obj)
  elif a==0x37a190:
   trace.append(dict(service='loaded_lookup',name=string(c,w(c.reg(1))).decode(),hit=loaded));ret(node if loaded else c.reg(0))
  elif a==0x37a300:
   trace.append(dict(service='cache_lookup',name=string(c,w(c.reg(1))).decode()));ret(node)
  elif a==d+0x19000:
   assert (c.reg(0),c.reg(2),c.reg(3))==(stream,0,0)
   trace.append(dict(service='stream_reset',offset=0));ret()
  elif a==0x31acf4:
   assert (c.reg(1),c.reg(2))==(owner+4,stream)
   trace.append(dict(service='Instance.loadFile',status=load_status,loaded_before=loaded))
   c.pointer(c.reg(0)+4,load_status);ret(c.reg(0))
  elif a==0x31a68c:trace.append(dict(service='Error.destroy'));ret()
  elif a==0x3140ec:putstring(c.reg(0),string(c,c.reg(1)));ret(c.reg(0))
  elif a==0x37a9e8:
   assert c.reg(1)==owner+0x80
   trace.append(dict(service='loaded_insert',name=string(c,w(c.reg(2)+0x14)).decode()));loaded=True;ret(c.reg(0))
  elif a==0x3139ac:ret(c.reg(0))
 elif mode=='instance':
  if a==0x31a804:uc.mem_write(c.reg(0),bytes(32));ret(c.reg(0))
  elif a==0x84bbbc:
   reader,ctx=c.reg(1),c.reg(2)
   assert (c.reg(0),w(ctx),w(ctx+8),w(ctx+16))==(state,instance,stream,1024)
   trace.append(dict(service='lua_load',reader=hex(reader),chunk_name=string(c,c.reg(3)).decode(),status=load_status,top_before=len(stack)))
   stack.append('error' if load_status else 'closure');ret(load_status)
  elif a==0x84bc50:
   assert (c.reg(0),c.reg(1),c.reg(2),c.reg(3))==(state,0,0,0)
   assert stack.pop()=='closure';trace.append(dict(service='lua_pcall',args=0,results=0,error_handler=0,status=call_status))
   if call_status:stack.append('error')
   ret(call_status)
  elif a==0x84c384:
   assert (c.reg(0),c.reg(1),c.reg(2))==(state,0xffffffff,0) and stack[-1]=='error'
   trace.append(dict(service='lua_tolstring',index=-1));ret(errtext)
  elif a==0x84b140:
   assert c.reg(0)==state and c.reg(1)==0xfffffffe and stack.pop()=='error'
   trace.append(dict(service='lua_settop',index=-2));ret()
  elif a==0x3109e0:
   data=bytes(uc.mem_read(c.reg(1),c.reg(2)-c.reg(1)))
   trace.append(dict(service='error_string_copy',bytes=data.hex()));putstring(c.reg(0),data);ret(c.reg(0))
c.uc.hook_add(UC_HOOK_CODE,hook)
c.pointer(args+4,vec);c.pointer(node+0x28,stream);c.pointer(stream,vt);c.pointer(vt+0x20,d+0x19000)
c.pointer(instance+4,state);c.uc.mem_write(errtext,b'nested\0discarded\0')
mode='include'
for count in (0,1,2,16,33):
 for kind in range(8):
  c.uc.mem_write(values,bytes(112*max(count,1)));c.pointer(values+4,kind);c.pointer(values+0x20,text)
  c.uc.mem_write(text,b'child\0ignored');c.uc.mem_write(vec,struct.pack('<III',values,values+112*count,values+112*count))
  for load_status in (0,1):
   trace.clear();c.invoke(0x37efe4,[args,0,owner]);assert bool(trace)==bool(count and kind==4)
   rows.append(dict(group='Include',count=count,type=kind,load_boolean=load_status,trace=trace.copy()))
mode='manager'
for name in (b'',b'child',b'child.lua',b'child.luac',b'child.lua.extra',b'child.luac.extra',b'folder.lua/child',b'child.LUA'):
 for loaded in (False,True):
  for load_status in (0,2,4):
   before=loaded;putstring(owner+0x68,b'data/scripts/ai/');c.uc.mem_write(text,name+b'\0')
   trace.clear();result=c.invoke(0x37b23c,[manager,owner,text]);expected=bool(name) and (before or load_status==0)
   assert result==int(expected)
   if name:
    assert trace[0]['service']=='loaded_lookup'
    assert ([r['service'] for r in trace]==['loaded_lookup']) if before else True
    if not before:
     assert [r['service'] for r in trace][:4]==['loaded_lookup','cache_lookup','stream_reset','Instance.loadFile']
     assert any(r['service']=='loaded_insert' for r in trace)==(load_status==0)
   else:assert not trace
   rows.append(dict(group='AddFile.cached',name=name.decode(),loaded_before=before,status=load_status,result=result,trace=trace.copy()))
mode='instance'
for load_status in (0,2,3,4,5):
 for call_status in (0,2,4,5):
  for top in (0,1,5):
   stack=list(range(top));trace.clear();c.invoke(0x31acf4,[err,instance,stream])
   assert stack==list(range(top)) and w(err+4)==(load_status or call_status)
   assert sum(r['service']=='lua_pcall' for r in trace)==int(load_status==0)
   assert sum(r['service']=='lua_settop' for r in trace)==int(bool(load_status or call_status))
   rows.append(dict(group='Instance.loadFile',load_status=load_status,call_status=call_status,initial_stack=top,final_stack=len(stack),status=w(err+4),trace=trace.copy()))
symbol_rows=[]
with ENGINE.open('rb') as f:
 elf=ELFFile(f)
 for s in elf.get_section_by_name('.symtab').iter_symbols():
  if s['st_size'] and any(k in s.name for k in ('LuaScript','LuaManager')) and any(k in s.name for k in ('GetBool','SetBool','GetString','SetString','GetInt','SetInt','Include')):
   symbol_rows.append(dict(symbol=s.name,address=hex(s['st_value']),size=s['st_size']))
report=dict(validation='PASS',original_sha256=sha(ENGINE),probe_sha256=sha(Path(__file__)),manifest_sha256=sha(HERE/'original-functions.json'),original_instructions_executed=True,cases=len(rows),rows=rows,mismatches=0,symbol_inventory=symbol_rows,boundaries=['std::string allocation/copy/destruction','loaded std::set/cache std::map lookup/insertion','cached StreamBuffer reset','Instance.loadFile service in AddFile cases','lua_load/lua_pcall/error-stack services in Instance cases'],scope=__doc__)
(HERE/'include-original-probe.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(dict(validation='PASS',cases=len(rows),groups={k:sum(r['group']==k for r in rows) for k in sorted({r['group'] for r in rows})})))
