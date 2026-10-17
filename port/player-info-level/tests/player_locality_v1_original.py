"""Pinned original locality callers; declared network/factory callee fixtures."""
from __future__ import annotations
import hashlib,json,struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE,UC_HOOK_MEM_READ
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_PC,UC_ARM_REG_LR,UC_ARM_REG_SP
MODULE=Path(__file__).resolve().parents[1]
MANIFEST=MODULE/'reference/player-locality-v1/original-functions.json'
ELF_SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
LOOKUP=0x36eea8;LOCAL=0x36effc;CNET=0x80f1ec;SERVER=0x7fe4e0;GET_MATCH=0x800f8c
MEMBER=0x801b30;SERVER_MEMBER=0x801b48
MANAGER=0x10001000;FALLBACK=MANAGER+8;PLAYERS=[FALLBACK,0x10003000,0x10004000,0x10005000]
MATCH=0x10008000;MATCH2=0x10010000;NET=0x10018000;VECTOR=0x10019000;STOP=0x30000000
ABSENT=-2147483648
def signed(v):return v-0x100000000 if v&0x80000000 else v
def put(u,a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
def get(u,a):return struct.unpack('<I',u.mem_read(a,4))[0]
def image(path):
 raw=path.read_bytes();assert hashlib.sha256(raw).hexdigest()==ELF_SHA
 with path.open('rb') as f:
  e=ELFFile(f);segments=[s for s in e.iter_segments() if s['p_type']=='PT_LOAD'];symbols=list(e.get_section_by_name('.dynsym').iter_symbols());names={s.name:s for s in symbols}
  length=(max(s['p_vaddr']+s['p_memsz'] for s in segments)+4095)&~4095;data=bytearray(length)
  for s in segments:data[s['p_vaddr']:s['p_vaddr']+s['p_filesz']]=raw[s['p_offset']:s['p_offset']+s['p_filesz']]
  pins=json.loads(MANIFEST.read_text())
  for pin in pins['functions']:
   address=int(pin['elf_address'],0);size=pin['size'];s=names[pin['original_symbol']]
   assert (s['st_value'],s['st_size'])==(address,size)
   assert hashlib.sha256(data[address:address+size]).hexdigest()==pin['sha256']
  for table in pins['vtables']:
   address=int(table['elf_address'],0);size=table['size']
   assert hashlib.sha256(data[address:address+size]).hexdigest()==table['sha256']
  # Load at source virtual addresses; apply only ordinary address relocations.
  # External call providers below are explicit fixtures, never imported code.
  for section in e.iter_sections():
   if section['sh_type'] not in ['SHT_REL','SHT_RELA']:continue
   for rel in section.iter_relocations():
    target=rel['r_offset'];kind=rel['r_info_type'];symbol=symbols[rel['r_info_sym']]
    if kind in [21,22]:struct.pack_into('<I',data,target,symbol['st_value'])
    elif kind==2:struct.pack_into('<I',data,target,(struct.unpack_from('<I',data,target)[0]+symbol['st_value'])&0xffffffff)
  for table in pins['vtables']:
   symbol=names[table['original_symbol']];assert (symbol['st_value'],symbol['st_size'])==(int(table['elf_address'],0),table['size'])
   for slot,target in table['slots'].items():
    assert struct.unpack_from('<I',data,symbol['st_value']+8+int(slot,0))[0]==int(target['address'],0)
  return bytes(data),names,pins
def cases():
 base=[2,99,0,0,0,0,0,22,99,44,-1,0,-1,7,7,-1,ABSENT,ABSENT,0]
 rows=[]
 def add(**changes):
  row=base.copy()
  for key,value in changes.items():row[int(key[1:])]=value
  rows.append(row)
 for mode in [0,1]:
  for char in [0,22,99,44,77]:add(v0=mode,v1=char)
  add(v0=mode,v7=99,v8=99,v9=99);add(v0=mode,v1=0,v7=0);add(v0=mode,v18=3)
  for online,game,room,initialized in [(1,0,1,1),(1,1,0,1),(1,1,1,0),(1,1,1,1),(7,1,1,1)]:
   for char in [22,99,77]:add(v0=mode,v1=char,v2=7,v3=online,v4=game,v5=room,v6=initialized)
 add(v0=0,v1=22,v2=9,v3=1,v4=1,v5=1,v6=1,v18=1)
 add(v0=0,v1=99,v3=1,v4=1,v5=1,v6=1,v18=2)
 for active in [0,1,7]:
  for member in [-2,-1,0,7]:
   for first,second,bmember in [(-1,7,-1),(0,7,7),(0,8,0),(7,7,-2)]:add(v0=2,v10=member,v11=active,v12=first,v13=second,v15=bmember)
 for mutation in [-1,0,7]:
  add(v0=2,v10=99,v11=1,v12=0,v13=7,v16=mutation)
  add(v0=2,v10=-1,v17=mutation,v15=-1)
 for active in [0,1,7]:
  for first,second,server in [(-1,7,7),(0,7,7),(0,8,7),(7,-1,-1),(7,-1,0)]:add(v0=3,v11=active,v12=first,v13=second,v14=server)
 for provider in [-1,0,1,2,3,4,9]:
  for existing in [0,1]:add(v0=4,v1=provider,v2=existing)
 add(v0=4,v1=0,v2=0,v3=1);add(v0=4,v1=1,v2=0,v3=1)
 for mode in [5,6]:
  for active in [0,1,7,255]:add(v0=mode,v11=active)
 return rows
def execute(data,symbols,row,words):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
 for base,size in [(0x10000000,0x30000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
 u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP)
 mode=row[0];trace=[];acquired=0;members=0;selected=0;constructors=0;entry_count=0 if row[18]==3 else 3
 singleton=symbols['_ZN9CMatching10s_MatchingE']['st_value'];provider=symbols['_ZN9CMatching18s_matchingProviderE']['st_value']
 put(u,singleton,MATCH if mode==4 and row[2] else 0);put(u,provider,row[1] if mode==4 else 1)
 ids=[2,7,19];chars=[0,*row[7:10]]
 for player,char in zip(PLAYERS,chars):put(u,player+0x660,char);put(u,player+0x1a0,row[10]);put(u,player,symbols['_ZTV10PlayerInfo']['st_value']+8)
 for matching in [MATCH,MATCH2]:
  u.mem_write(matching+0xc,bytes([row[11]&255]));put(u,matching,symbols['_ZTV14CMatchingLocal']['st_value']+8);put(u,matching+0x3638,row[12]);put(u,matching+0x363c,row[14])
 # Three source RB nodes in ascending order, with the actual +18 value/+660.
 nodes=[player-0x18 for player in PLAYERS[1:]];header=MANAGER+0x690
 put(u,MANAGER+0x698,nodes[0] if entry_count else header)
 for i,node in enumerate(nodes):
  put(u,node+4,header if i==1 else nodes[1]);put(u,node+8,nodes[0] if i==1 else 0);put(u,node+12,nodes[2] if i==1 else 0);put(u,node+16,ids[i])
 put(u,header+4,nodes[1] if entry_count else 0);put(u,header+12,nodes[2] if entry_count else header)
 put(u,MANAGER+0x6a8,VECTOR);put(u,MANAGER+0x6ac,VECTOR+12)
 for i,value in enumerate(ids):put(u,VECTOR+i*4,value)
 def ret(value):u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def mutate_info(value):
  if value!=ABSENT:
   for p in PLAYERS:put(u,p+0x1a0,value)
 def hook(machine,address,size,context):
  nonlocal acquired,members,selected,constructors
  if address==STOP:machine.emu_stop();return
  if mode==5 and address==0x807d4c:machine.emu_stop();return
  if mode==6 and address==0x8076c0:machine.emu_stop();return
  fixture_addresses={0x7fd794,0x320e98,0x805cc8,0x8100dc,0x8100e0,0x36dfb0,0x36dec4,0x310570,0x800ed8,0x81e9dc,0x7ffd04}
  if mode==4:fixture_addresses.add(0x807d08)
  if address not in fixture_addresses:
   ranges=[(LOOKUP,340),(LOCAL,44),(CNET,80),(SERVER,100),(GET_MATCH,280),(MEMBER,12),(SERVER_MEMBER,12),(0x80003c,160),(0x807d08,0x44),(0x8076a4,0x1c)]
   assert any(a<=address<a+n for a,n in ranges),f'Unexpected unscoped original execution: {address:#x}'
   words.add(address)
  if address==0x36f010:
   selected=machine.reg_read(UC_ARM_REG_R0);trace.append([11,selected,0])
  if address==0x7fd794:trace.append([1,0,row[3]]);machine.mem_write(0x1001c000+5,bytes([row[3]&255]));ret(0x1001c000)
  elif address==0x320e98:trace.append([2,0,row[4]]);machine.mem_write(0x1001c100+0x24,bytes([row[4]&255]));ret(0x1001c100)
  elif address==GET_MATCH and mode!=4:
   match=MATCH if acquired==0 else MATCH2;acquired+=1;put(machine,singleton,match)
   if acquired==2:mutate_info(row[17])
   trace.append([3,0,match]) # The actual Get body still executes.
  elif address==0x805cc8:trace.append([4,machine.reg_read(UC_ARM_REG_R0),row[5]]);ret(row[5])
  elif address==0x8100dc:trace.append([5,0,NET]);ret(NET)
  elif address==0x8100e0:trace.append([6,machine.reg_read(UC_ARM_REG_R0),row[6]]);ret(row[6])
  elif address in [0x36dfb0,0x36dec4]:
   ident=signed(machine.reg_read(UC_ARM_REG_R1));flag=machine.reg_read(UC_ARM_REG_R2);trace.append([8 if address==0x36dfb0 else 9,ident,flag]);player=PLAYERS[ids.index(ident)+1] if ident in ids else FALLBACK
   if address==0x36dfb0:
    if row[18]==1:put(machine,VECTOR,19)
    if row[18]==2:put(machine,MANAGER+0x6ac,VECTOR)
   ret(player)
  elif address==MEMBER:
   matching=machine.reg_read(UC_ARM_REG_R0)
   value=row[15] if matching==MATCH2 else row[12 if members==0 else 13]
   if matching==MATCH:members+=1
   put(machine,matching+0x3638,value);trace.append([13,matching,value]) # Real 12B leaf executes.
  elif address==SERVER_MEMBER:
   matching=machine.reg_read(UC_ARM_REG_R0);put(machine,matching+0x363c,row[14]);mutate_info(row[16]);trace.append([14,matching,row[14]])
  elif address==0x310570:ret(MATCH) # Declared allocation fixture, no copied allocator.
  elif mode==4 and address in [0x807d08,0x800ed8,0x81e9dc]:
   kind=0 if address==0x807d08 else 1 if address==0x800ed8 else 2;bytes_=0xa9e8 if kind==0 else 0xabb8 if kind==1 else 0x6c08;mode_=machine.reg_read(UC_ARM_REG_R1) if kind==2 else 0
   trace.append([kind,bytes_,mode_]);constructors+=1
   if row[3]:put(machine,provider,2 if kind==0 else 3 if kind==1 else 4)
   ret(MATCH)
  elif mode==5 and address==0x80003c:
   # Actual CMatching C2 body executes, with each member-record constructor a
   # declared external callee. The local prefix stops before other owners.
   pass
  elif mode==5 and address==0x7ffd04:ret(machine.reg_read(UC_ARM_REG_R0))
 def read_hook(machine,access,address,size,value,context):
  if mode<4:
   for player in PLAYERS:
    if address==player+0x660:trace.append([10,player,get(machine,address)])
    if address==player+0x1a0:trace.append([12,player,signed(get(machine,address))])
   if address==MANAGER+0x6a8:trace.append([7,0,(get(machine,MANAGER+0x6ac)-get(machine,MANAGER+0x6a8))//4])
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_READ,read_hook)
 function=[LOOKUP,LOCAL,CNET,SERVER,GET_MATCH,0x807d08,0x8076a4][mode]
 u.reg_write(UC_ARM_REG_R0,MANAGER if mode<2 else FALLBACK if mode==2 else MATCH)
 u.reg_write(UC_ARM_REG_R1,row[1]&0xffffffff);u.reg_write(UC_ARM_REG_R2,row[2]&0xffffffff)
 if mode==6:
  u.reg_write(UC_ARM_REG_R5,MATCH);u.reg_write(UC_ARM_REG_R10,0xffffffff);u.reg_write(UC_ARM_REG_R3,0x123);put(u,MATCH+0x3638,17);put(u,MATCH+0x363c,33)
 u.emu_start(function,STOP+4,count=10000)
 assert u.reg_read(UC_ARM_REG_PC)==([0x807d4c,0x8076c0][mode-5] if mode>=5 else STOP),hex(u.reg_read(UC_ARM_REG_PC))
 if mode>=5:return {'value':0,'player':0,'route':0,'registered':False,'trace':[[u.mem_read(MATCH+0xc,1)[0],signed(get(u,MATCH+0x3638)),signed(get(u,MATCH+0x363c))]]}
 if mode==4:return {'value':signed(get(u,provider)),'player':u.reg_read(UC_ARM_REG_R0),'route':constructors,'registered':False,'trace':trace}
 player=u.reg_read(UC_ARM_REG_R0) if mode==0 else selected if mode==1 else FALLBACK if mode==2 else 0
 route=0
 if mode in [0,1] and not(mode==1 and row[1]==0):route=2 if player==FALLBACK else 3 if any(t[0]==9 for t in trace) else 1
 return {'value':0 if mode==0 else signed(u.reg_read(UC_ARM_REG_R0)),'player':player,'route':route,'registered':route==1,'trace':trace}
def capture(path):
 data,symbols,pins=image(path);rows=cases();words=set();results=[execute(data,symbols,row,words) for row in rows]
 return rows,results,{'validation':'PASS','original_sha256':ELF_SHA,'cases':len(rows),'distinct_executed_words':len(words),'scope':pins['scope'],'constructor_projection_cases':sum(r[0]>=5 for r in rows),'matching_get_cases':sum(r[0]==4 for r in rows)}
