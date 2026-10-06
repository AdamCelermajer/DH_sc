from pathlib import Path
import sys,struct,json,hashlib
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def put(p,v):c.uc.mem_write(p,struct.pack('<I',v&0xffffffff))
def returned(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
events=[]
quest_mode=0
stub=c.data+0x70000;vtable=stub+0x100;character=stub+0x200;gs=stub+0x400
put(stub,0xe12fff1e);put(vtable+0x28,stub);put(character,vtable)
def hook(uc,a,z,u):
 if a==0x3ff5d4:
  item=c.reg(1);events.append(['add',word(item+4),c.reg(2),c.reg(3)]);returned(len(events)-1)
 elif a==0x3f9e00:events.append(['source_id',word(c.reg(0)+4)]) # original ID leaf executes
 elif a==0x310440:events.append(['delete_slot',word(word(c.reg(0))+4)]);returned()
 elif a==0x3fe164:events.append(['add_gold',c.reg(0)==destination,c.reg(1)]);returned()
 elif a==stub:events.append(['is_player']);returned(quest_mode!=1)
 elif a==0x31f594:events.append(['current_gs']);returned(0 if quest_mode==3 else gs)
 elif a==0x4c4bdc:
  r1=bytes(c.uc.mem_read(c.reg(1),100)).split(b'\0')[0].decode();r2=bytes(c.uc.mem_read(c.reg(2),100)).split(b'\0')[0].decode()
  assert (r1,r2)==('v2QuestObjectiveType','GatherLoot');events.append(['constant',r1,r2]);returned(37)
 elif a==0x339090:
  p=c.reg(1);assert c.reg(0)==gs
  fields=[word(p+4),word(p+8),word(p+12),word(p+20),word(p+24),*bytes(c.uc.mem_read(p+16,2))]
  assert fields==[37,character,0xffffffff,0xffffffff,0,0,0],fields;events.append(['async_owned_fields',37,0,-1,-1,0,0]);returned()
c.uc.hook_add(UC_HOOK_CODE,hook)
rows=[]
for count in range(4):
 source=c.data+0x10000;destination=c.data+0x12000;array=c.data+0x14000
 c.uc.mem_write(source,bytes(0x100));c.uc.mem_write(destination,bytes(0x100))
 put(source+8,array);put(source+12,array+count*4)
 for i in range(count):
  slot=c.data+0x15000+i*0x100;item=slot+0x40;put(array+i*4,slot);put(slot,item);put(item+4,i)
 events=[];c.invoke(0x3ffa68,[source,destination,0,1])
 expected=[]
 for i in range(count):expected.extend([['add',i,0,1],['source_id',i],['delete_slot',i]])
 expected.extend([['add_gold',True,0],['add_gold',False,0]])
 assert events==expected,(count,events);assert word(source+12)==array
 rows.append({'count':count,'events':events,'source_vector_empty':True})
quest_rows=[]
for quest_mode in range(1,5):
 c.uc.mem_write(source,bytes(0x100));c.uc.mem_write(destination,bytes(0x100));put(source+8,array);put(source+12,array+4)
 slot=c.data+0x15000;item=slot+0x40;put(array,slot);put(slot,item);put(item+4,0);put(destination+4,character)
 sentinel=destination+0x30;node=destination+0x100
 put(sentinel,node if quest_mode>=3 else sentinel);put(node,sentinel);put(node+8,0)
 events=[];c.invoke(0x3ffa68,[source,destination,0,1]);assert word(source+12)==array
 assert ['is_player'] in events;assert (['current_gs'] in events)==(quest_mode>=3)
 assert any(x[0]=='async_owned_fields' for x in events)==(quest_mode==4)
 quest_rows.append({'mode':quest_mode,'events':events})
report={'status':'PASS','cases':rows,'quest_cases':quest_rows,'original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'scope':'Whole original TransferInventoryTo instructions and player/gathering-list/current-GS/positive AsyncCall branches; Add/allocator/gold/virtual/GS/constant/Async receivers declared observers, event fields/order checked. No production GS/quest owner claimed.'}
p=root/'port/level-world/reference/world-item-live-v5/transfer-inventory-original.json';p.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
