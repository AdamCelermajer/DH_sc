"""Original full Kill Player branch, Ctrl_Kill, RaiseEvent and AI event2 callers.

Original property Set/AddInt/Add/GetInt/RecalcProperty instructions execute. Genuine
Application singleton reads execute; manager/locality/trophy/online/OnDied/FSM
callees are declared fixtures. Host event2 composition is a separate real-owner
proof. The general nonplayer/forced Kill branch is not claimed by this replay.
"""
from __future__ import annotations
import hashlib,json,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[2]
ORIGINAL='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
NAMES=['_ZN9Character4KillEP10GameObjectb','_ZN9Character9Ctrl_KillEP10GameObjectb',
 '_ZN9Character10RaiseEventEiPv','_ZN6CharAI12RaiseAIEventEiPv',
 '_ZN14CharProperties9PROPS_SetEii','_ZN14CharProperties12PROPS_AddIntEii',
 '_ZN14CharProperties9PROPS_AddEii','_ZNK14CharProperties12PROPS_GetIntEib',
 '_ZNK14CharProperties8_GetTypeEi','_ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi',
 '_ZN14CharProperties12_SetPropertyERN7Structs19CharacterPropertiesEii','_ZN14CharProperties14RecalcPropertyEi',
 '_ZN13PlayerManager13IsLocalPlayerEPK9Character','_ZN13PlayerManager20GetPlayerByCharacterEPK9Characterb',
 '_ZN13PlayerManager14GetLocalPlayerEib','_Z9GetOnlinev',
 '_ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc','_ZN13TrophyManager12UnlockTrophyEi']
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def evidence(path):
 assert sha(path)==ORIGINAL;blob=path.read_bytes()
 with path.open('rb') as f:
  e=ELFFile(f);sy={s.name:s for s in e.get_section_by_name('.symtab').iter_symbols()}
  # Exact symbol spelling is discovered only for the separately pinned closure.
  names=NAMES[:]
  for prefix in ['_ZN14CharProperties13PROPS_Resolve','_ZNK14CNetPlayerInfo7IsLocal','_ZN14CNetPlayerInfo7IsLocal',
                 '_ZN9CMatching3Get','_ZN9CMatching8IsServer','_ZNK14CMatchingLocal11GetMemberId','_ZNK14CMatchingLocal17GetServerMemberId']:
   names.extend(n for n in sy if n.startswith(prefix))
  rows=[]
  for name in dict.fromkeys(names):
   s=sy[name];a,n=int(s['st_value']),int(s['st_size']);g=next(g for g in e.iter_segments() if g['p_type']=='PT_LOAD' and g['p_vaddr']<=a and a+n<=g['p_vaddr']+g['p_filesz']);o=int(g['p_offset'])+a-int(g['p_vaddr'])
   rows.append({'original_symbol':name,'elf_address':hex(a),'size':n,'sha256':hashlib.sha256(blob[o:o+n]).hexdigest()})
 return rows
def compare(elf,exe,cache,temp,env,run):
 sys.path.insert(0,str(ROOT/'engine-resources/tests'));from cpu import Cpu
 rows=evidence(elf);cpu=Cpu(elf,False,{'functions':rows});instructions=set();records=[]
 char=cpu.data+0x1000;manager=cpu.data+0x8000;fresh_manager=manager+0x100;online=manager+0x200
 trophy=manager+0x400;fresh_trophy=trophy+0x100;vt=manager+0x800;cb=manager+0x1000;ai=char+0x3c8;avt=manager+0x2000
 defaults=cpu.data+0x10000
 def w(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
 def signed(x):return x if x<0x80000000 else x-0x100000000
 def ret(value=0):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
 def text(at):return bytes(cpu.uc.mem_read(at,64)).split(b'\0')[0].decode()
 raw=(cache/'data/pydata/character_properties_pyarray.bin').read_bytes();defs=list(struct.unpack_from('<224i',raw,4));types=list(struct.unpack_from('<224i',raw,900))
 got=(0x3a5b48+w(0x3a61a4))&0xffffffff;app=w(got+w(0x3a61a8));trophy_global=w(got+w(0x3a61b0))
 cpu.pointer(0x9a645c,defaults);cpu.uc.mem_write(defaults,bytes(4)+struct.pack('<224i',*defs)+bytes(4)+struct.pack('<224i',*types))
 scenarios=[(mode,count,local,on,7,None,0,0,0,0) for mode in (0,1) for count in (-2,8,9,10,49,50,99,100,101) for local,on in ((0,0),(1,0),(1,1))]
 scenarios += [(mode,7,1,1,-1,[49,50,100],0,0,1,1) for mode in (0,1)]
 scenarios += [(1,9,1,0,7,None,1,0,0,0),(1,9,1,1,7,None,0,1,0,0)]
 for mode,count,local,on,tid,fresh,outer_dead,inner_dead,mutate_manager,mutate_trophy in scenarios:
  trace=[];events=[];calls={'dead':0,'get':0};started=bool(mode)
  cpu.uc.mem_write(char,bytes(0x6000));cpu.pointer(char,vt);cpu.pointer(vt+0x34,cb);cpu.pointer(vt+0x28,cb+4)
  cpu.uc.mem_write(cb,struct.pack('<3I',*[0xe12fff1e]*3));cpu.pointer(ai,avt);cpu.pointer(ai+4,char);cpu.pointer(avt+0x24,cb+8)
  cpu.pointer(app+0x40,manager);cpu.pointer(trophy_global,trophy)
  sheets=[char+0x560+x+4 for x in (8,0x38c,0x710,0xa94)]
  for sheet in sheets:cpu.uc.mem_write(sheet,struct.pack('<224i',*defs))
  # Actual runtime-only HP and source saved count initially agree with host.
  cpu.pointer(sheets[1]+25*4,(count*256)&0xffffffff);cpu.pointer(sheets[3]+25*4,(count*256)&0xffffffff);cpu.pointer(sheets[3]+36*4,123*256)
  sentinel=char+0x560+0xe18;cpu.uc.mem_write(sentinel,struct.pack('<4I',0,0,sentinel,sentinel));cpu.pointer(char+0x560+0xe28,0)
  def ident(p):return 1 if p==char else 2 if p==manager else 3 if p==fresh_manager else 4 if p==trophy else 5 if p==fresh_trophy else 0
  def add(op,subject=0,arg=0,index=0,name=0):trace.append([op,subject,arg,index,name])
  def hook(_,at,__,___):
   nonlocal started
   if any(int(r['elf_address'],16)<=at<int(r['elf_address'],16)+r['size'] for r in rows):instructions.add(at)
   if at==0x3a5b6c:started=True
   if at==0x3a5b18 and mode:add(12,1)
   if at==cb:
    if started:add(11,1)
    v=(outer_dead if calls['dead']==0 else inner_dead) if mode else 0;calls['dead']+=1;ret(v)
   elif at==cb+4:add(0,1);ret(1)
   elif at==0x3e0798:assert cpu.reg(0)==char+0x560 and cpu.reg(1)==25 and cpu.reg(2)==1;add(1,1,1,25)
   elif at in (0x3a5ba0,0x3a5bc8):add(2)
   elif at==0x36effc:assert cpu.reg(0)==manager and cpu.reg(1)==char;add(3,2);ret(local)
   elif at==0x3a602c:add(4)
   elif at==0x3df6e0:
    assert cpu.reg(0)==char+0x560 and cpu.reg(1)==25 and not cpu.reg(2);add(5,1,0,25)
    if fresh:cpu.pointer(sheets[3]+25*4,(fresh[calls['get']]*256)&0xffffffff)
    calls['get']+=1
    if mutate_trophy:cpu.pointer(trophy_global,fresh_trophy)
   elif at==0x3a3f70:
    name=text(cpu.reg(0));n={'quest_died_10_times':10,'quest_died_50_times':50,'quest_died_100_times':100}[name];add(6,0,0,0,n);ret(tid&0xffffffff)
   elif at==0x3813b8:assert cpu.reg(0)==trophy;add(7,4,signed(cpu.reg(1)));ret()
   elif at==0x7fd794:
    add(8);cpu.uc.mem_write(online+5,bytes([on]));
    if mutate_manager:cpu.pointer(app+0x40,fresh_manager)
    ret(online)
   elif at==0x36e478:assert cpu.reg(0)==(fresh_manager if mutate_manager else manager) and cpu.reg(1)==0 and cpu.reg(2)==1;add(9,ident(cpu.reg(0)),1);ret()
   elif at==0x3a4d5c:assert cpu.reg(0)==char and cpu.reg(1)==2 and cpu.reg(2)==manager+0x3000;add(13,1,2)
   elif at==cb+8:assert cpu.reg(0)==ai and cpu.reg(1)==manager+0x3000;events.append([1,0x24,2,1]);ret()
   elif at==0x3c5684:assert cpu.reg(0)==char+0x4fc and cpu.reg(1)==2 and cpu.reg(2)==manager+0x3000;events.append([0,0,2,1]);ret()
  h=cpu.uc.hook_add(UC_HOOK_CODE,hook)
  try:cpu.invoke(0x3ad528 if mode else 0x3a5b18,[char,manager+0x3000,0])
  finally:cpu.uc.hook_del(h)
  expected={'dead':cpu.uc.mem_read(char+0x1449,1)[0],'saved':signed(w(sheets[1]+25*4)),'cached':signed(w(sheets[3]+25*4)),'hp':signed(w(sheets[3]+36*4)),'trace':trace,'events':events}
  values=fresh or [-2147483648]*3
  actual=json.loads(run([exe,'--oracle',cache,temp,mode,count,local,on,tid,*values,outer_dead,inner_dead,mutate_manager,mutate_trophy],env))
  assert actual==expected,(mode,count,local,on,tid,fresh,outer_dead,inner_dead,expected,actual)
  records.append({'input':[mode,count,local,on,tid,values,outer_dead,inner_dead,mutate_manager,mutate_trophy],'original':expected,'compiled':actual})
 return {'validation':'PASS','comparisons':len(records),'mismatches':0,'functions':rows,'instructions_executed':[hex(x) for x in sorted(instructions)],'executed_words':len(instructions),'imports_executed':cpu.import_calls,'results':records,'scope':__doc__}
if __name__=='__main__':
 import argparse
 p=argparse.ArgumentParser();p.add_argument('--original-elf',required=True,type=Path);p.add_argument('--output',required=True,type=Path);a=p.parse_args()
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps({'original_sha256':ORIGINAL,'functions':evidence(a.original_elf),'scope':__doc__},indent=2)+'\n')
