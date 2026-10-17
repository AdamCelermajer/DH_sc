"""Friendly ordinals/counts: reuse source manager fixture and actual nested lookup."""
from __future__ import annotations
import hashlib,itertools,json
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE,UC_HOOK_MEM_READ
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_PC,UC_ARM_REG_LR,UC_ARM_REG_SP
from player_local_selection_v1_original import cases as local_cases,MANAGER,FALLBACK,PLAYERS,MATCH,NET,VECTOR,VECTOR2,STOP
from player_locality_v1_original import image,put,get,signed,ELF_SHA
MODULE=Path(__file__).resolve().parents[1]
MANIFEST=MODULE/'reference/player-manager-friendly-v1/original-functions.json'
ID=0x36e5b4;PLAYER=0x36e744;COUNT=0x36d7a8;INTERNAL=0x36dfb0
def cases():
 rows=[]
 for old in local_cases():
  if old[0] in [0,1]:
   row=old.copy();row[20]=3;rows.append(row)
 base=[0,0,0,0,1,1,1,1,0,1,0,22,44,2,7,19,5,0,3,0,3]
 for mode,online,ordinal,flag in itertools.product([0,1],[0,1],[-2147483648,-2,-1,0,1,2,3,2147483647],[0,1,7]):
  row=base.copy();row[0]=mode;row[1]=ordinal;row[2]=flag;row[3]=online;rows.append(row)
 for mode,network_count,tree_count in itertools.product([0,1,2],range(4),range(4)):
  row=base.copy();row[0]=mode;row[3]=1;row[18]=tree_count;row[20]=network_count;rows.append(row)
 for route in [(0,1,1,1),(1,0,1,1),(1,1,0,1),(1,1,1,0),(1,1,1,1),(255,7,7,7)]:
  for count in range(4):
   row=base.copy();row[0]=2;row[3:7]=route;row[18]=count;rows.append(row)
 for mode,mutation,ordinal,flag in itertools.product([0,1],[1,2],[0,1],[0,1]):
  row=base.copy();row[0]=mode;row[1]=ordinal;row[2]=flag;row[3]=1;row[17]=mutation;row[10]=22;rows.append(row)
 for mode,initial,fresh in itertools.product([0,1],[0,1],[1,2]):
  row=base.copy();row[0]=mode;row[3]=initial;row[19]=fresh;rows.append(row)
 return [list(r) for r in dict.fromkeys(tuple(r) for r in rows)]
def execute(data,row,words):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
 for base,size in [(0x10000000,0x30000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
 u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP)
 keys=[2,7,19];trace=[];online_calls=0;internal_calls=0;selected=0;value=-1
 count=row[18];nodes=[p-0x18 for p in PLAYERS[1:]];header=MANAGER+0x690
 for index,p in enumerate(PLAYERS):
  put(u,p+0x660,0 if index==0 else row[9+index]);put(u,p+0x670,-1 if index==0 else row[12+index])
  u.mem_write(p+0x66c,bytes([0 if index==0 else row[6+index]&255]))
 for node in nodes:
  for off in [4,8,12]:put(u,node+off,0)
 if count==3:
  root=nodes[1];put(u,nodes[0]+4,root);put(u,nodes[2]+4,root);put(u,root+8,nodes[0]);put(u,root+12,nodes[2])
 elif count==2:
  root=nodes[0];put(u,root+12,nodes[1]);put(u,nodes[1]+4,root)
 elif count==1:root=nodes[0]
 else:root=0
 if root:put(u,root+4,header)
 put(u,header+4,root);put(u,header+8,nodes[0] if count else header);put(u,header+12,nodes[count-1] if count else header)
 put(u,MANAGER+0x6a0,count)
 for i,node in enumerate(nodes):put(u,node+16,keys[i])
 put(u,MANAGER+0x6a8,VECTOR);put(u,MANAGER+0x6ac,VECTOR+row[20]*4)
 # A deliberately different local vector catches wrong +6b4 selection.
 put(u,MANAGER+0x6b4,VECTOR2);put(u,MANAGER+0x6b8,VECTOR2+4)
 for i,key in enumerate(keys):put(u,VECTOR+i*4,key)
 for i,key in enumerate([7,19,2]):put(u,VECTOR2+i*4,key)
 table=0x10009000;put(u,MATCH,table);put(u,table+0x64,0x805cc8)
 def ret(v):u.reg_write(UC_ARM_REG_R0,v&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(machine,address,size,context):
  nonlocal online_calls,internal_calls,value
  if address==STOP:machine.emu_stop();return
  providers={0x7fd794,0x320e98,0x800f8c,0x805cc8,0x8100dc,0x8100e0,0x36dec4}
  if address not in providers:
   assert any(a<=address<a+n for a,n in [(ID,400),(PLAYER,32),(COUNT,108),(INTERNAL,236)]),hex(address)
   words.add(address)
  if address==0x36e750:value=signed(machine.reg_read(UC_ARM_REG_R0))
  if address==INTERNAL:
   ident=signed(machine.reg_read(UC_ARM_REG_R1));flag=machine.reg_read(UC_ARM_REG_R2);trace.append([8,ident,flag]);internal_calls+=1
   if row[17] and internal_calls==1:
    if row[17]==1:put(machine,VECTOR,19)
    elif row[17]==2:put(machine,MANAGER+0x6a8,VECTOR2);put(machine,MANAGER+0x6ac,VECTOR2+row[20]*4)
    else:put(machine,MANAGER+0x6ac,VECTOR)
  if address==0x7fd794:
   online=row[3]
   if online_calls and row[19]:online=0 if row[19]==1 else 1
   online_calls+=1;trace.append([1,0,online]);machine.mem_write(0x1001c005,bytes([online&255]));ret(0x1001c000)
  elif address==0x320e98:trace.append([2,0,row[4]]);machine.mem_write(0x1001c124,bytes([row[4]&255]));ret(0x1001c100)
  elif address==0x800f8c:trace.append([3,0,MATCH]);ret(MATCH)
  elif address==0x805cc8:trace.append([4,MATCH,row[5]]);ret(row[5])
  elif address==0x8100dc:trace.append([5,0,NET]);ret(NET)
  elif address==0x8100e0:trace.append([6,NET,row[6]]);ret(row[6])
  elif address==0x36dec4:
   ident=signed(machine.reg_read(UC_ARM_REG_R1));flag=machine.reg_read(UC_ARM_REG_R2);trace.append([16,ident,flag])
   ret(PLAYERS[keys.index(ident)+1] if ident in keys else FALLBACK)
 def reads(machine,access,address,size,v,context):
  if address==MANAGER+0x6a8:trace.append([7,0,(get(machine,MANAGER+0x6ac)-get(machine,address))//4])
  for p in PLAYERS:
   if address==p+0x660:trace.append([10,p,get(machine,address)])
   if address==p+0x670:trace.append([11,p,signed(get(machine,address))])
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_READ,reads)
 u.reg_write(UC_ARM_REG_R0,MANAGER);u.reg_write(UC_ARM_REG_R1,row[1]&0xffffffff);u.reg_write(UC_ARM_REG_R2,row[2]&0xffffffff)
 u.emu_start([ID,PLAYER,COUNT][row[0]],STOP+4,count=20000);assert u.reg_read(UC_ARM_REG_PC)==STOP
 if row[0] in [0,2]:value=signed(u.reg_read(UC_ARM_REG_R0))
 if row[0]==1:selected=u.reg_read(UC_ARM_REG_R0)
 return dict(value=value,player=selected,trace=trace)
def capture(path):
 data,symbols,_=image(path);pins=json.loads(MANIFEST.read_text())
 for pin in pins['functions']:
  symbol=symbols[pin['original_symbol']];address=int(pin['elf_address'],0);size=pin['size']
  assert (symbol['st_value'],symbol['st_size'])==(address,size)
  assert hashlib.sha256(data[address:address+size]).hexdigest()==pin['sha256']
 rows=cases();words=set();results=[execute(data,row,words) for row in rows]
 return rows,results,dict(validation='PASS',cases=len(rows),distinct_executed_words=len(words),original_sha256=ELF_SHA,scope=pins['scope'])
