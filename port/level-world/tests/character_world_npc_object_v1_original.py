"""Whole original Object lifecycle/PF continuations with declared virtual,
visual, filter, Stop and PFWorld dependency services; no full-init claim."""
from pathlib import Path
import sys,struct,json,hashlib
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
root=Path(__file__).resolve().parents[3];sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu as BaseCpu
from unicorn import UC_HOOK_CODE
class Cpu(BaseCpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address) in ('__aeabi_fcmplt','__aeabi_fcmple'):
   decode=lambda v:struct.unpack('<f',struct.pack('<I',v))[0]
   a,b=decode(self.reg(0)),decode(self.reg(1));self.put(0,int(a<b if self.imports[address]=='__aeabi_fcmplt' else a<=b))
   uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,unused)
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
owner=c.data+0x1000;vt=c.data+0x4000;body=c.data+0x5000;visual=c.data+0x6000
put=lambda a,v:c.uc.mem_write(a,struct.pack('<I',v&0xffffffff))
byte=lambda a,v:c.uc.mem_write(a,bytes([v]))
get=lambda a:struct.unpack('<I',c.uc.mem_read(a,4))[0]
b=lambda a:bytes(c.uc.mem_read(a,1))[0]
fb=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
ff=lambda x:struct.unpack('<f',struct.pack('<I',x))[0]
events=[];obstacle=False;remove_body=False
queries={c.data+0x8000:'is_obstacle',c.data+0x8010:'weight',c.data+0x8020:'extent'}
for address in queries:put(address,0xe12fff1e)
def hook(uc,pc,size,user):
 if pc in queries:
  name=queries[pc];events.append(name)
  c.put(0,int(obstacle) if name=='is_obstacle' else fb(1 if name=='weight' else 0))
  if name=='extent' and remove_body:put(owner+0x2dc,0)
 elif pc==0x4713d0:events.append('visual')
 elif pc==0x46ebe4:events.append('filter_enable')
 elif pc==0x46eb70:events.append('filter_disable')
 elif pc==0x3a40b0:events.append('stop')
 elif pc==0x46e750:events.append('radius');c.put(0,fb(36))
 elif pc==0x528234:
  events.append(['init_obstacle',c.reg(2),ff(c.reg(3)),ff(get(uc.reg_read(c.sp)))])
 else:return
 uc.reg_write(c.pc,uc.reg_read(c.lr))
c.uc.hook_add(UC_HOOK_CODE,hook)
def setup(mask):
 c.uc.mem_write(owner,b'\0'*0x400);put(owner,vt)
 for offset,fn in [(0x3c,0x33dcf0),(0x40,0x38b0f0),(0x44,0x3a598c),(0x48,0x3a5974),(0xc4,0x3883b8),
                   (0xb4,c.data+0x8000),(0xb8,c.data+0x8010),(0xbc,c.data+0x8020)]:put(vt+offset,fn)
 byte(owner+0x2ed,bool(mask&1));byte(owner+0x2ee,bool(mask&2));byte(owner+0x8a,bool(mask&16))
 put(owner+0x2d8,visual if mask&4 else 0);put(owner+0x2dc,body if mask&8 else 0);put(owner+0x1cc,8)
 events.clear()
cases=0
for mask in range(32):
 setup(mask);c.invoke(0x3a598c,[owner]);assert b(owner+0x80)==bool(mask&16) and b(owner+0x85)==1 and get(owner+0x1cc)&8
 expected=(['visual'] if mask&4 else [])+(['filter_enable'] if mask&8 else []);assert events==expected
 events.clear();c.invoke(0x38c69c,[owner]);assert b(owner+0x2f0)==0 and b(owner+0x85)==bool((mask&1) or not(mask&2))
 assert events==(['visual'] if mask&2 and mask&4 else [])
 events.clear();c.invoke(0x38c710,[owner]);assert b(owner+0x2f0)==1 and b(owner+0x85)==1
 assert events==(['visual'] if mask&2 and mask&4 and mask&16 else [])
 events.clear();c.invoke(0x3a5974,[owner]);assert b(owner+0x80)==0 and b(owner+0x85)==0 and not(get(owner+0x1cc)&8) and b(owner+0x373)==1
 assert events==(['visual'] if mask&4 else [])+(['filter_disable'] if mask&8 else [])+['stop'];cases+=4
setup(0);byte(owner+0x8a,1);byte(owner+0x80,0xa5);c.invoke(0x33ddc8,[owner,1]);assert b(owner+0x80)==0xa5 and not events;cases+=1
for mask in range(8):
 setup(8 if mask&1 else 0);obstacle=bool(mask&2);remove_body=bool(mask&4)
 # Constructor-null floor must call nothing, even when every virtual would be needed.
 c.invoke(0x393ea0,[owner]);assert not events;cases+=1
 put(owner+0x1c8,1)
 for off,val in [(0x144,-4),(0x148,-6),(0x150,8),(0x154,10)]:put(owner+off,fb(val))
 c.invoke(0x393ea0,[owner]);physical=bool(mask&1) and not(obstacle and remove_body)
 assert ff(get(owner+0x1d0))==(36 if physical else 8)
 expected=['is_obstacle']
 if obstacle:expected+=['weight','extent',['init_obstacle',int(bool(mask&1)),1.0,0.0]]
 if physical:expected+=['radius']
 assert events==expected,(mask,events,expected);cases+=1
filter_cpu=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});refilters=[]
def filter_hook(uc,pc,size,user):
 if pc==0x7e7afc:
  refilters.append(filter_cpu.reg(1));uc.reg_write(filter_cpu.pc,uc.reg_read(filter_cpu.lr))
filter_cpu.uc.hook_add(UC_HOOK_CODE,filter_hook)
filter_cases=0
for present in range(4):
 for initially_disabled in range(2):
  for enable in range(2):
   base=filter_cpu.data+0x1000;world=base+0x100;shapes=[base+0x200,base+0x300]
   filter_cpu.uc.mem_write(base,b'\0'*0x400);filter_cpu.pointer(base+4,world);filter_cpu.pointer(world+0x10,world+0x50)
   for i,shape in enumerate(shapes):
    filter_cpu.pointer(base+0x18+4*i,shape if present&(1<<i) else 0)
    filter_cpu.uc.mem_write(shape+0x22,struct.pack('<hhh',9,10,11))
   filter_cpu.uc.mem_write(base+0x20,struct.pack('<hhh',-11,13,17));filter_cpu.uc.mem_write(base+0x26,bytes([initially_disabled]));refilters.clear()
   filter_cpu.invoke(0x46ebe4 if enable else 0x46eb70,[base])
   reached=bool(initially_disabled)==bool(enable)
   assert refilters==[shapes[i] for i in range(2) if reached and present&(1<<i)]
   for i,shape in enumerate(shapes):
    actual=struct.unpack('<hhh',filter_cpu.uc.mem_read(shape+0x22,6))
    expected=(-11,13,17) if enable else (0,0,0)
    assert actual==(expected if reached and present&(1<<i) else (9,10,11))
   assert bytes(filter_cpu.uc.mem_read(base+0x26,1))[0]==(0 if enable else 1);filter_cases+=1
room_cpu=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});room_events=[];next_node=room_cpu.data+0x10000
def room_hook(uc,pc,size,user):
 global next_node
 if pc==0x337888:room_events.append('debug_load')
 elif pc==0x3140ec:
  name=bytes(uc.mem_read(room_cpu.reg(1),100)).split(b'\0')[0].decode();assert name=='isTracingRoomZoneInit';room_events.append('string')
 elif pc==0x337a88:room_events.append('query');room_cpu.put(0,0)
 elif pc==0x318254:room_events.append('destroy_string')
 elif pc==0x39670c:room_events.append('allocate_list_node');room_cpu.put(0,next_node);next_node+=16
 elif pc==0x708f00:room_events.append('free_list_node')
 else:return
 uc.reg_write(room_cpu.pc,uc.reg_read(room_cpu.lr))
room_cpu.uc.hook_add(UC_HOOK_CODE,room_hook)
def rw(a):return struct.unpack('<I',room_cpu.uc.mem_read(a,4))[0]
def rb(a):return bytes(room_cpu.uc.mem_read(a,1))[0]
def rput(a,v):room_cpu.pointer(a,v)
room_cases=0
for x in (-1,0,5,10,11):
 for y in (-1,0,5,10,11):
  obj=room_cpu.data+0x1000;table=room_cpu.data+0x2000;rooms=[room_cpu.data+0x3000,room_cpu.data+0x4000]
  room_cpu.uc.mem_write(obj,b'\0'*0x400);rput(obj,table);rput(table+0xc4,0x3883b8);rput(table+0x3c,0x33dcf0)
  rput(obj+0x160,fb(x));rput(obj+0x164,fb(y));rput(obj+0x168,fb(9999));room_cpu.uc.mem_write(obj+0x2ee,b'\1')
  for room in rooms:
   room_cpu.uc.mem_write(room,b'\0'*0x400)
   for off,value in ((0x12c,0),(0x130,0),(0x138,10),(0x13c,10)):rput(room+off,fb(value))
   rput(room+0x394,room+0x394);rput(room+0x398,room+0x394)
  room_events.clear();accepted=0<=x<=10 and 0<=y<=10
  assert room_cpu.invoke(0x396a90,[rooms[0],obj])==int(accepted);room_cases+=1
  if not accepted:assert not room_events;continue
  assert room_events==['debug_load','string','query','destroy_string','allocate_list_node']
  assert rw(obj+0x2f4)==rooms[0] and rb(obj+0x2ef)==rb(obj+0x2f0)==rb(obj+0x85)==1
  room_events.clear();assert room_cpu.invoke(0x396a90,[rooms[1],obj])==1;room_cases+=1
  assert room_events==['debug_load','string','query','destroy_string','free_list_node','allocate_list_node']
  assert rw(rooms[0]+0x394)==rooms[0]+0x394 and rw(obj+0x2f4)==rooms[1]
  old_tail=rw(rooms[1]+0x398);room_cpu.invoke(0x39672c,[rooms[1],obj]);assert rw(rooms[1]+0x398)==old_tail;room_cases+=1
  room_cpu.invoke(0x3968cc,[rooms[1],obj]);assert rw(rooms[1]+0x394)==rooms[1]+0x394 and rw(obj+0x2f4)==rooms[1] and rb(obj+0x2ef)==1;room_cases+=1
property_cpu=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
object_base=property_cpu.data+0x1000;map_root=property_cpu.data+0x2000;nodes=[map_root+0x100,map_root+0x200];props=[map_root+0x300,map_root+0x400];property_vtable=map_root+0x500
def property_hook(uc,pc,size,user):
 if pc==0x5134c0:property_cpu.put(0,map_root)
 elif pc==0x32b918:
  dst=property_cpu.reg(0);property_cpu.pointer(dst+0x10,property_cpu.reg(1)+0x10);property_cpu.pointer(dst+0x14,0)
 elif pc==0x3109e0:pass
 elif pc==0x318254:pass
 else:return
 uc.reg_write(property_cpu.pc,uc.reg_read(property_cpu.lr))
property_cpu.uc.hook_add(UC_HOOK_CODE,property_hook)
property_cases=0
for poison in (0,0x33,0xaa,0xff):
 property_cpu.uc.mem_write(object_base,bytes([poison])*0x200)
 property_cpu.uc.mem_write(map_root,b'\0'*0x600)
 property_cpu.pointer(map_root+8,nodes[0]);property_cpu.pointer(map_root+12,nodes[1])
 property_cpu.pointer(nodes[0]+4,map_root);property_cpu.pointer(nodes[0]+12,nodes[1]);property_cpu.pointer(nodes[1]+4,nodes[0])
 property_cpu.pointer(property_vtable+12,0x33de80)
 for node,prop,offset,default in zip(nodes,props,(0x80,0x7c),(0,1)):
  property_cpu.pointer(node+0x28,prop);property_cpu.pointer(prop,property_vtable);property_cpu.pointer(prop+4,offset);property_cpu.uc.mem_write(prop+0x20,bytes([default]))
 property_cpu.invoke(0x5136ec,[object_base+4])
 assert bytes(property_cpu.uc.mem_read(object_base+0x80,1))==b'\1' and bytes(property_cpu.uc.mem_read(object_base+0x84,1))==b'\0';property_cases+=1
leaf_cases=0
for address,expected in ((0x3a2ee8,1),(0x3a2ef0,fb(50)),(0x3a2efc,fb(20))):
 assert c.invoke(address,[owner])==expected;leaf_cases+=1
setup(8);put(owner+0x1c8,1)
for offset,address in ((0xb4,0x3a2ee8),(0xb8,0x3a2ef0),(0xbc,0x3a2efc)):put(vt+offset,address)
c.invoke(0x393ea0,[owner])
assert events==[['init_obstacle',1,50.0,20.0],'radius'] and ff(get(owner+0x1d0))==36
leaf_cases+=1
report=dict(validation='PASS',whole_original_Character_PF_provider_calls=leaf_cases,whole_original_LoadDefaultProperties_calls=property_cases,whole_original_method_calls=cases,whole_original_room_method_calls=room_cases,whole_original_physical_filter_calls=filter_cases,explicit_dependency_services=True,native_differential=False,
 original_sha256=hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),
 source_null_floor_UpdatePFObject=True,SetEnable_true_ctor_no_visible_write=True,room_entry_does_not_write_byte80=True)
(root/'port/level-world/reports/character-world-npc-object-v1-original-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
