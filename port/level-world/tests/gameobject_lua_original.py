"""Execute original Value push and actual type binding producers.

Lua stack primitives and allocation are explicit services. No whole original
VM execution is claimed. Registration and Value branching execute ARM32.
"""
import hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/game-data/tests'))
from items_differential import Original,words
REF=ROOT/'reference/gameobject-lua-representation'
class Table(dict):
 def __init__(self):super().__init__();self.meta=None
class Probe(Original):
 def __init__(self):
  super().__init__(REPO/'.local-inputs/libDungeonHunter2.so',json.loads((REF/'original-functions.json').read_text()))
  self.stack_values=[];self.registry={};self.trace=[];self.bindings=[];self.active=False
  self.names={s['st_value']:s.name for s in self.elf.get_section_by_name('.symtab').iter_symbols()} if hasattr(self,'elf') else {}
  from elftools.elf.elffile import ELFFile
  with (REPO/'.local-inputs/libDungeonHunter2.so').open('rb') as f:
   self.names={s['st_value']:s.name for s in ELFFile(f).get_section_by_name('.symtab').iter_symbols()}
  self.uc.hook_add(UC_HOOK_CODE,self.hook)
 def text(self,p):
  out=b''
  while self.uc.mem_read(p+len(out),1)!=b'\0':out+=bytes(self.uc.mem_read(p+len(out),1))
  return out.decode()
 def value(self,i):return self.stack_values[i-1 if i>0 else i]
 def trace_value(self,v):
  if isinstance(v,Table):return dict(fields={str(k):self.trace_value(x) for k,x in v.items()},meta=None if v.meta is None else 'shared')
  if isinstance(v,tuple):return list(v)
  return v
 def hook(self,uc,a,size,_):
  if not self.active:return
  name=self.names.get(a,'') or self.imports.get(a,'');result=0
  if a==0x319af4:self.bindings.append([self.text(self.reg(1)),self.reg(2)]);return
  if getattr(self,'method_function',0):
   if name=='lua_type':result=5
   elif name=='lua_getfield':self.stack_values.append(self.stack_values[0].get(self.text(self.reg(2))))
   elif name=='lua_touserdata':
    value=self.stack_values[-1];result=value[1] if isinstance(value,tuple) and value[0]=='identity' else 0
   elif name=='lua_settop':self.stack_values.pop()
   elif a==0x3196ec:
    p=self.reg(0);start=self.reg(2);vector=self.heap;self.heap+=512;values=self.heap;self.heap+=1024
    self.pointer(p+4,vector);self.pointer(vector,values);self.pointer(vector+4,values+112)
    uc.mem_write(values,bytes(112));self.pointer(values+4,2);self.pointer(values+0x6c,self.method_function if start==1 else 0)
    self.method_trace.append(['arguments',start]);result=p
   elif a==0x31b434:uc.mem_write(self.reg(0),bytes(40));result=self.reg(0)
   elif a in (0x38eb00,0x37c9f8):self.method_result=[2 if a==0x38eb00 else 7,self.reg(1)]
   elif a==0x31b308:
    self.method_trace.append(['return',*self.method_result]);result=1
   elif a in (0x31b398,0x319228):pass
   else:result=None
   if result is not None:self.returned(result);return
  if name=='lua_createtable':self.stack_values.append(Table())
  elif name=='lua_pushstring':self.stack_values.append(self.text(self.reg(1)))
  elif name=='lua_pushlightuserdata':self.stack_values.append(('identity',self.reg(1)))
  elif name=='lua_pushnil':self.stack_values.append(None)
  elif name in ('lua_rawset','lua_settable'):
   i=self.reg(1)&0xffffffff;i=i if i<0x80000000 else i-0x100000000;t=self.value(i);v=self.stack_values.pop();k=self.stack_values.pop();t[k]=v
  elif name=='luaL_newmetatable':
   key=self.text(self.reg(1));result=int(key not in self.registry)
   if result:self.registry[key]=Table()
   self.stack_values.append(self.registry[key]);self.trace.append(['metatable',key,result])
  elif name=='lua_setmetatable':
   i=self.reg(1)&0xffffffff;i=i if i<0x80000000 else i-0x100000000;t=self.value(i);t.meta=self.stack_values.pop()
  elif name=='lua_pushcclosure':
   n=self.reg(2);values=self.stack_values[-n:] if n else []
   if n:del self.stack_values[-n:]
   self.stack_values.append(('closure',self.reg(1),values))
  elif a==0x31167c:
   p=self.reg(0);n=self.reg(1);buf=self.heap;self.heap+=max(256,n+32);self.pointer(p+16,buf);self.pointer(p+20,buf);uc.mem_write(buf,bytes(max(256,n+32)))
  elif a==0x310804:
   p=self.reg(0);data=bytes(uc.mem_read(self.reg(1),self.reg(2)-self.reg(1)));buf=self.word(p+20);pos=self.word(p+16);uc.mem_write(pos,data+b'\0');self.pointer(p+16,pos+len(data));result=p
  elif a in (0x30e868,):
   uc.mem_write(self.reg(0),bytes(uc.mem_read(self.reg(1),self.reg(2))));result=self.reg(0)
  elif a==0x30de54:result=len(self.text(self.reg(0)))
  elif a==0x31bb44:pass
  else:return
  self.returned(result)
 def push(self,kind,identity=0):
  value=self.data+0x4000;obj=self.data+0x5000;vt=self.data+0x7000
  self.uc.mem_write(value,bytes(112));self.pointer(value+4,kind);self.pointer(value+0x6c,obj if identity else 0)
  self.pointer(obj,vt);self.pointer(vt+8,0x3a2f08 if identity==2 else 0x340124);self.pointer(vt+12,0x3b56bc if identity==2 else 0x38d7ec)
  self.active=True
  try:
   if kind==7:
    self.invoke(0x37c978,[value,obj if identity else 0]);assert self.word(value+4)==7 and self.word(value+0x6c)==(obj if identity else 0)
   self.invoke(0x31cac4,[value,1234],budget=1000000)
  except Exception:
   print('failure',kind,identity,hex(self.uc.reg_read(self.pc)),self.names.get(self.uc.reg_read(self.pc)),[hex(self.reg(i)) for i in range(4)],self.stack_values);raise
  self.active=False
  return self.stack_values.pop()
 def method(self,address,identity):
  table=Table();table['_this']=('identity',identity);self.stack_values=[table];self.method_trace=[];self.method_result=[];self.method_function=address;self.active=True
  try:self.invoke(0x319fd0,[1234],budget=1000000)
  except Exception:
   print('method failure',hex(address),hex(self.uc.reg_read(self.pc)),[hex(self.reg(i)) for i in range(4)],self.method_trace);raise
  finally:self.active=False;self.method_function=0
  return self.method_result,self.method_trace
def main():
 p=Probe();rows=[]
 for kind,identity in [(7,0),(7,1),(7,1),(7,2),(7,2),(2,1),(0,0)]:
  before=len(p.trace);bindings=len(p.bindings);v=p.push(kind,identity);rows.append(dict(kind=kind,identity=identity,value=p.trace_value(v),events=p.trace[before:],bindings=p.bindings[bindings:]))
 assert rows[0]['value'] is None
 assert [r['events'][0][2] for r in rows[1:5]]==[1,0,1,0]
 receiver=p.data+0x5000;p.pointer(receiver+0x408,receiver+0x1000)
 method_cases=[]
 for address in (0x38ebe4,0x3b6c7c):
  result,trace=p.method(address,receiver);method_cases.append(dict(address=address,receiver=receiver,result=result,trace=trace))
 out=dict(validation='PASS',original_sha256=hashlib.sha256((REPO/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),cases=rows,method_cases=method_cases,registry={k:p.trace_value(v) for k,v in p.registry.items()},explicit_services=__doc__+' Method Arguments STL/vector projection and ReturnValues append/return are additionally explicit services; actual Binder method gate and GetID/GetTarget wrapper instructions execute.')
 (REF/'object-producer-probe.json').write_text(json.dumps(out,indent=2)+'\n')
 print(json.dumps({'validation':'PASS','cases':len(rows),'method_counts':{k:len(v['fields']['__index']['fields']) for k,v in out['registry'].items()}}))
if __name__=='__main__':main()
