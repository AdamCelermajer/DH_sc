"""Whole original queue function with declared virtual-query and GetDt services.
This is a source branch audit, not a native differential claim.
"""
from pathlib import Path
import sys,struct,itertools,json,hashlib
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
word=lambda a:struct.unpack('<I',c.uc.mem_read(a,4))[0]
put=lambda a,v:c.uc.mem_write(a,struct.pack('<I',v&0xffffffff))
base=(0x3ce9d0+word(0x3cebdc))&0xffffffff
count=word((base+word(0x3cebe0))&0xffffffff)
queue=word((base+word(0x3cebe8))&0xffffffff)
block=word((base+word(0x3cebec))&0xffffffff)
assert word(count)==0xffffffff
buf=c.data+0x1000;node=c.data+0x2000
ais=[c.data+0x3000+i*0x100 for i in range(3)]
actors=[c.data+0x4000+i*0x2000 for i in range(3)]
controllers=[c.data+0xb000+i*0x100 for i in range(3)]
dead=remote=zoned=dt=0
attack_bits=None
attack_calls=[]
def hook(uc,pc,size,user):
 if attack_bits is not None and pc in (0x3d574c,0x3ffd38,0x3d6188,0x3d6604,0x3d6860):
  query={0x3d574c:0,0x3ffd38:1,0x3d6188:2,0x3d6860:3,0x3d6604:4}[pc]
  attack_calls.append(query);c.put(0,(attack_bits>>query)&1)
  uc.reg_write(c.pc,pc+4 if pc==0x3d6860 else uc.reg_read(c.lr))
 elif pc in (0x3ceae4,0x3ceb7c,0x3ceba4):
  c.put(0,{0x3ceae4:dead,0x3ceb7c:remote,0x3ceba4:zoned}[pc]);uc.reg_write(c.pc,pc+4)
 elif pc==0x31f66c:
  c.put(0,dt);uc.reg_write(c.pc,uc.reg_read(c.lr))
c.uc.hook_add(UC_HOOK_CODE,hook)
def reset():
 put(node,buf)
 c.uc.mem_write(queue,struct.pack('<8I',buf,buf,buf+128,node,buf+12,buf,buf+128,node))
 for ai,actor,controller in zip(ais,actors,controllers):
  c.uc.mem_write(ai,b'\0'*0x100);put(ai+4,actor)
  c.uc.mem_write(actor,b'\0'*0x1800);put(actor+0x378,controller)
  c.uc.mem_write(controller,b'\0'*16)
 for i,ai in enumerate(ais):put(buf+4*i,ai)
records=[]
for forced,blocked,locked,dead,remote,visible,zoned,flag2ee,flag2f0 in itertools.product((0,1),repeat=9):
 reset();put(count,0);c.uc.mem_write(block,bytes([blocked]))
 for actor,controller in zip(actors,controllers):
  c.uc.mem_write(controller+8,bytes([locked,forced]));c.uc.mem_write(actor+0x80,bytes([visible]));c.uc.mem_write(actor+0x2ee,bytes([flag2ee]));c.uc.mem_write(actor+0x2f0,bytes([flag2f0]))
 skip=(not forced and (blocked or locked)) or dead or (not remote and not visible) or (not remote and visible and zoned and flag2ee and not flag2f0)
 c.invoke(0x3ce9b8,[])
 front=word(word(queue));expected=ais[0] if skip else ais[1]
 assert front==expected,(forced,blocked,locked,dead,remote,visible,zoned,flag2ee,flag2f0,hex(front))
 assert word(count)==180
 records.append(int(skip))
for dt in (0,1,179,180,200,0x7fffffff):
 reset();put(count,180);c.invoke(0x3ce9b8,[]);assert word(count)==((180-dt)&0xffffffff);assert word(word(queue))==ais[0]
attack_records=[]
for attack_bits in range(32):
 reset();put(ais[0]+0x40,actors[1]);attack_calls.clear()
 answer=c.invoke(0x3d67f4,[ais[0],0])
 expected=bool((attack_bits&1) and (((attack_bits&2) and (attack_bits&4)) or ((attack_bits&8) and (attack_bits&16))))
 assert bool(answer)==expected
 expected_calls=[0]
 if attack_bits&1:
  expected_calls.append(1)
  if attack_bits&2:expected_calls.append(2)
  if not((attack_bits&2) and (attack_bits&4)):
   expected_calls.append(3)
   if attack_bits&8:expected_calls.append(4)
 assert attack_calls==expected_calls,(attack_bits,attack_calls,expected_calls)
 attack_records.append([attack_bits,answer,attack_calls.copy()])
attack_bits=None
report={'validation':'PASS','whole_original_queue_cases':len(records)+6,'whole_original_can_attack_cases':len(attack_records),'can_attack_ordered_queries_matched':True,'original_countdown_address':hex(count),'initial_countdown':-1,'virtual_services':'Queue: Dead/RemotelyUpdated/Zoned/GetDt; CanAttack: Enemy/InventoryMelee/MeleeGeometry/CanRange/RangeGeometry explicit fixture services','all_remaining_original_instructions':True,'native_differential':False,'original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest()}
(root/'port/level-world/reports/character-world-ai-queue-v1-original-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report))
