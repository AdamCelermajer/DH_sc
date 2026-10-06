from pathlib import Path
import sys,struct,json
r=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(r/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
script=c.data+0x10000;manager=c.data+0x20000;node=c.data+0x30000;stream=c.data+0x40000;vtable=stream+0x100;seek=c.data+0x50000;heap=c.data+0x100000;names=c.data+0x60000
def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def put(p,v):c.uc.mem_write(p,struct.pack('<I',v&0xffffffff))
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def string(p):
 b=bytearray()
 while p and (v:=c.uc.mem_read(p,1)[0]):b.append(v);p+=1
 return b.decode()
def cstring(p,text):
 global heap
 data=text.encode()+b'\0';ptr=heap;heap+=((len(data)+31)//32)*32;c.uc.mem_write(ptr,data);put(p+16,ptr+len(data)-1);put(p+20,ptr)
base=(0x37b580+8+word(0x37b598))&0xffffffff;global_services=word((base+word(0x37b59c))&0xffffffff);put(global_services+0x3c,manager)
put(node+0x28,stream);put(stream,vtable);put(vtable+0x20,seek)
loaded=set();queries=[];seeks=parses=0;present=True;lua_status=0
def hook(uc,a,z,u):
 global seeks,parses
 if a==0x3338cc:cstring(c.reg(0),string(word(c.reg(1)+20))+string(c.reg(2)));ret(c.reg(0))
 elif a==0x379ef8:cstring(c.reg(0),string(word(c.reg(0)+20))+string(c.reg(1)));ret(c.reg(0))
 elif a==0x30ebd4:
  text,needle=string(c.reg(0)),string(c.reg(1));index=text.find(needle);ret(c.reg(0)+index if index>=0 else 0)
 elif a==0x30ec7c:
  left=bytes(c.uc.mem_read(c.reg(0),c.reg(2))).split(b'\0')[0];right=bytes(c.uc.mem_read(c.reg(1),c.reg(2))).split(b'\0')[0];ret(0 if left==right else 1)
 elif a==0x37a190:ret(node if string(word(c.reg(1))) in loaded else c.reg(0))
 elif a==0x37a300:queries.append(string(word(c.reg(1))));ret(node if present else c.reg(0))
 elif a==0x37b444:
  # Actual source file-open NULL delivery is an explicit filesystem fixture;
  # return through source manager epilogue without manufacturing cached bytes.
  c.put(5,0);c.uc.reg_write(c.pc,0x37b30c)
 elif a==seek:assert c.reg(0)==stream and c.reg(2)==c.reg(3)==0;seeks+=1;ret()
 elif a==0x31acf4:assert c.reg(1)==script+4 and c.reg(2)==stream;put(c.reg(0)+4,lua_status);parses+=1;ret()
 elif a==0x3140ec:cstring(c.reg(0),string(c.reg(1)));ret(c.reg(0))
 elif a==0x37a9e8:loaded.add(string(word(c.reg(2)+20)));ret()
 elif a in (0x3139ac,0x31a68c):ret()
c.uc.hook_add(UC_HOOK_CODE,hook);cstring(script+0x68,'data/scripts/')
cases=[]
for name,found,status in [('level/combat_formulas',True,0),('level/death_scripts.lua',True,0),('level/death_scripts.luac',True,0),('level/x.lua.tail',True,0),('level/x.luac.tail',True,0),('level/fail',True,2),('level/fail',True,0),('level/combat_formulas',True,0),('',True,0),('missing',False,0)]:
 c.uc.mem_write(names,name.encode()+b'\0');present=found;lua_status=status;before_q=len(queries);before_s=seeks;before_p=parses
 result=c.invoke(0x37b574,[script,names]);cases.append({'name':name,'present':found,'lua_status':status,'result':result,'requested':queries[before_q:],'seek_calls':seeks-before_s,'parse_calls':parses-before_p,'loaded':sorted(loaded)})
assert cases[0]['requested']==['data/scripts/level/combat_formulas.luac']
assert cases[1]['requested']==['data/scripts/level/death_scripts.luac'] and not cases[2]['requested']
assert cases[3]['requested']==['data/scripts/level/x.lua.tailc'] and cases[4]['requested']==['data/scripts/level/x.luac.tail']
assert not cases[5]['result'] and cases[6]['result']==1 and not cases[7]['requested'] and not cases[8]['result'] and not cases[9]['result']
p=r/'port/level-world/reference/lua-script-v13/load-wrapper-original.json';report={'status':'PASS','cases':cases,'scope':'Whole LuaScript.Load37b574 and LuaManager.AddFile37b23c extension/membership/cache-hit/seek/load-status/insert/return bodies. CString/libc primitives, loaded/cache tree lookups/insertion, source file-openNULL and Instance loader/error disposal are declared receivers; native positive VM/assets validated separately.'};p.write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
