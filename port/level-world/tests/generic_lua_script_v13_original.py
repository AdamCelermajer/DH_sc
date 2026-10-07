from pathlib import Path
import sys,struct,json,hashlib
r=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(r/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def put(p,v):c.uc.mem_write(p,struct.pack('<I',v&0xffffffff))
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def string(p):
 out=bytearray()
 while p and (x:=c.uc.mem_read(p,1)[0]):out.append(x);p+=1
 return out.decode()
heap=c.data+0x100000;states=[];bindings=[];libraries=[];closed=[]
def hook(uc,a,z,u):
 global heap
 if a==0x31b224:states.append(heap);v=heap;heap+=0x1000;ret(v) # Lua engine state allocation is declared external VM fixture.
 elif a==0x31167c:assert c.reg(1)==16;ret() # inline CString reserve fixture; original stores/pointer remain.
 elif a in (0x31b010,0x31b000,0x31aff8,0x31b008):libraries.append(hex(a));ret()
 elif a==0x31a4d4:bindings.append({'name':string(c.reg(1)),'callback':hex(c.reg(2)),'receiver':hex(c.reg(3))});ret()
 elif a==0x85797c:closed.append(c.reg(0));ret()
c.uc.hook_add(UC_HOOK_CODE,hook)
cases=[]
for deferred in [0,1,0,1]:
 p=c.data+0x10000+len(cases)*0x1000;c.uc.mem_write(p,b'\xa5'*0x94);before=len(bindings);lib_before=len(libraries)
 c.invoke(0x37c584,[p,deferred]);assert c.uc.mem_read(p+0xc,1)==b'\1' and word(p+0x14)==p+4 and word(p+0x18)==0
 for o in [0x20,0x2c,0x38,0x44,0x50,0x5c,0x84,0x90]:assert word(p+o)==0
 cases.append({'deferred':deferred,'owned_state':word(p+8),'binding_count':len(bindings)-before,'libraries':libraries[lib_before:]})
 assert len(bindings)-before==(0 if deferred else 33),(len(bindings)-before,bindings)
 # Source Instance destructor closes only owned state. Whole LuaScriptD1
 # teardown of empty source containers/inline path reaches that same close.
 c.invoke(0x37c004,[p]);assert closed[-1]==cases[-1]['owned_state']
assert len(set(states))==4
out=r/'port/level-world/reference/lua-script-v13';report={'status':'PASS','cases':cases,'bindings':bindings[:33],'private_states':len(states),'closed_states':len(closed),'source_sha256':hashlib.sha256((r/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'scope':'Whole LuaScript C1(bool0/1), embedded InstanceC1 stores and LuaScriptD1/InstanceD1 ownership/empty teardown. Actual library calls/binder deliveries/newstate/Lua close/inline reserve are explicit primitive receiver fixtures; no shared global interpreter inferred.'}
(out/'constructor-binding-original.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
