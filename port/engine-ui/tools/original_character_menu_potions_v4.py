from pathlib import Path
import sys,struct,json,hashlib
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
sys.path.insert(0,str(root/'port/engine-animation/tests'))
from particle_factory_differential import FactoryCpu
from unicorn import UC_HOOK_CODE
cpu=FactoryCpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
frame=cpu.data+0x1000;array=frame+0x100;values=array+0x100;receiver=values+0x100;vtable=receiver+0x100;record=vtable+0x100;character=record+0x800;potion=character+0x1800;service=potion+0x100;buffer=service+0x100
trace=[];pending='';operation='numbers';live=True;watch=False
def ret(v=0):cpu.put(0,v);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
def hook(uc,a,n,user):
 global pending
 if a==0x36e478:trace.append(['local',cpu.reg(1),cpu.reg(2)]);ret(record)
 elif a==0x43c388:trace.append(['player',cpu.reg(0),cpu.reg(1)]);ret(character if live else 0)
 elif a==0x413a7c:pending=cpu.string(cpu.reg(1)).decode();cpu.uc.mem_write(cpu.reg(0),bytes(20));ret(cpu.reg(0))
 elif a==0x797124:ret() # actual AS-value destructor provider; no result rewrite
 elif a==service:
  ptr=cpu.reg(2);kind=cpu.uc.mem_read(ptr+1,1)[0]
  value=struct.unpack('<d',cpu.uc.mem_read(ptr+4,8))[0]if operation=='numbers'else cpu.string(struct.unpack('<I',cpu.uc.mem_read(ptr+4,4))[0]).decode()
  trace.append(['member',pending,value]);assert kind==2 if operation=='numbers'else kind==4
  if watch and pending=='NumPotions':cpu.uc.mem_write(character+0x3a8,b'\xf9')
  ret(0) # Source ignores false SetMember result
 elif operation=='text' and a==0x797a54:cpu.put(0,0);cpu.put(1,0);uc.reg_write(cpu.pc,uc.reg_read(cpu.lr))
 elif operation=='text' and a==0x31167c:ret(cpu.reg(0)) # explicit CString allocator boundary
 elif operation=='text' and a==0x4c4bdc:trace.append(['constant',cpu.string(cpu.reg(1)).decode(),cpu.string(cpu.reg(2)).decode()]);ret(17)
 elif operation=='text' and a==0x508edc:trace.append(['getString',cpu.reg(1)]);ret(buffer)
 elif operation=='text' and a==0x508ef4:
  trace.append(['parse',cpu.string(cpu.reg(2)).decode(),cpu.reg(3)]);cpu.pointer(cpu.reg(1)+20,buffer+100);ret(1)
 elif operation=='text' and a==0x797350:cpu.uc.mem_write(cpu.reg(0),bytes([0,4,0,0])+struct.pack('<I',cpu.reg(1))+bytes(4));ret()
 elif operation=='text' and a==0x3139ac:ret() # actual CString destructor boundary
cpu.uc.hook_add(UC_HOOK_CODE,hook)
cpu.uc.mem_write(frame,bytes(32));cpu.pointer(frame+12,array);cpu.pointer(array,values);cpu.pointer(frame+16,1);cpu.pointer(frame+20,0);cpu.uc.mem_write(values,bytes([0,5,0,0])+struct.pack('<I',receiver)+bytes(4));cpu.pointer(receiver,vtable);cpu.pointer(vtable+0x1c,service);cpu.pointer(character+0x37c+0x24,potion)
rows=[]
for count in(-32768,-1,0,5,32767):
 for cap in(-128,-1,0,5,127):
  trace.clear();cpu.pointer(record+0x660,character);cpu.uc.mem_write(potion+0x50,struct.pack('<h',count));cpu.uc.mem_write(character+0x3a8,struct.pack('<b',cap));cpu.invoke(0x44996c,[frame]);assert trace==[['local',0,1],['member','NumPotions',float(count)],['member','MaxNumPotions',float(cap)]]
  rows.append({'operation':'whole_NativeGetNumPotions','count':count,'capacity':cap,'trace':trace.copy()})
trace.clear();watch=True;cpu.invoke(0x44996c,[frame]);assert trace[-1]==['member','MaxNumPotions',-7.];rows.append({'operation':'capacity_reentry','trace':trace.copy()});watch=False
trace.clear();cpu.pointer(record+0x660,0);cpu.invoke(0x44996c,[frame]);assert trace==[['local',0,1]];rows.append({'operation':'numbers_null_character','trace':trace.copy()})
operation='text';cpu.pointer(frame+16,2);cpu.pointer(frame+20,1);cpu.uc.mem_write(values,bytes([0,2,0,0])+bytes(8));cpu.uc.mem_write(values+12,bytes([0,5,0,0])+struct.pack('<I',receiver)+bytes(4));cpu.uc.mem_write(buffer,b'actual declared ^d format\0');cpu.uc.mem_write(buffer+100,b'actual declared formatted result\0')
trace.clear();cpu.uc.mem_write(potion+0x50,struct.pack('<h',5));cpu.invoke(0x446978,[frame]);assert trace==[['player',0,0],['constant','StrID','GAMEPLAYMENUS_POTIONS'],['getString',17],['parse','actual declared ^d format',5],['member','StrNumPotions','actual declared formatted result']];rows.append({'operation':'whole_NativeGetStringNumPotions','trace':trace.copy()})
trace.clear();live=False;cpu.invoke(0x446978,[frame]);assert trace==[['player',0,0]];rows.append({'operation':'text_null_character','trace':trace.copy()})
out={'validation':'PASS','cases':len(rows),'rows':rows,'original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'scope':'whole original potion wrapper ARM bodies, real GetNumPotions3fc690 signed-short getter and capacity signed-byte reads; explicit actual-AS/CString/player/constant/getString/parse provider fixtures. Not whole localization cache or live menu proof.'}
p=root/'port/engine-ui/reference/character-menu-application-v4/potions-original-proof.json';p.write_text(json.dumps(out,indent=2));print(json.dumps({'validation':out['validation'],'cases':len(rows),'last_rows':rows[-4:]}))
