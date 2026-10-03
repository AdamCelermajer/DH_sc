"""Original melee attack decisions with named query/search/log/FSM service fixtures. No native comparison."""
import hashlib,itertools,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from navigation_differential import Cpu
REF=ROOT/'reference/prince-live-attack'
constants_path=REPO/'.local-inputs/live-animation-selection-discovery/constants.json'
frontal_angle=json.loads(constants_path.read_text())['CharacterDesign']['Attack_FrontalAngle'];assert frontal_angle==90
manifest=json.loads((REF/'original-functions.json').read_text());cpu=Cpu(REPO/'.local-inputs/libDungeonHunter2.so',False,manifest)
owner=cpu.data+0x1000;ai=owner+0x3c8;target=cpu.data+0x4000;candidate=cpu.data+0x6000;fallback=cpu.data+0x8000;vt=cpu.data+0xa000;state=cpu.data+0xb000;buffer=cpu.data+0xc000
virtual=[cpu.data+0xd000+i*16 for i in range(3)]
controller=cpu.data+0xe000;cvtable=cpu.data+0xf000;network=cpu.data+0x10000;packet=cpu.data+0x11000
for address in virtual:cpu.uc.mem_write(address,bytes.fromhex('1eff2fe1'))
cpu.pointer(vt+0x34,virtual[0]);cpu.pointer(vt+0x124,virtual[1]);cpu.pointer(vt+0x28,virtual[2])
trace=[];settings=[];fixture={};list_handle=0
def word(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
def byte(at):return bytes(cpu.uc.mem_read(at,1))[0]
def returned(value=0):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
def string(at):
 b=bytearray()
 while len(b)<256:
  v=byte(at+len(b))
  if not v:return b.decode('ascii')
  b.append(v)
 raise AssertionError('unterminated original setting key')
def hook(uc,address,size,unused):
 global list_handle
 if address==0x7fd794:returned(network)
 elif address==0x80b1bc:trace.append(['network_queue']);returned(network)
 elif address==0x80a244:
  trace.append(['network_packet',string(cpu.reg(0)),cpu.reg(1)]);returned(packet)
 elif address==0x80e2a4:
  trace.append(['network_send',byte(packet+0x50),struct.unpack('<H',uc.mem_read(packet+0x52,2))[0],byte(packet+0x54)]);returned()
 elif address==0x3d01ac:trace.append(['melee_entry',cpu.reg(1),cpu.reg(2)])
 elif address==0x3ad87c:trace.append(['character_attack_entry',cpu.reg(1)])
 elif address in virtual:
  kind=virtual.index(address);subject=cpu.reg(0)
  value=fixture['dead'] if kind==0 and subject==owner else fixture['target_dead'] if kind==0 else fixture['range'] if kind==1 else fixture['player']
  trace.append([('dead','range','player')[kind],subject]);returned(value)
 elif address==0x3d076c:
  trace.append(['range_redirect',cpu.reg(1),cpu.reg(2)]);returned()
 elif address in (0x337888,0x3140ec,0x318254):returned()
 elif address==0x337a88:returned(fixture['dump'])
 elif address in (0x4a2240,0x4a191c):returned()
 elif address==0x4a2730:
  list_handle=cpu.reg(0);trace.append(['list_create',cpu.reg(1),cpu.reg(2),cpu.reg(3),word(cpu.uc.reg_read(cpu.sp))])
 elif address==0x3d015c:trace.append(['list_sort_reset'])
 elif address==0x3d0020:
  trace.append(['search',cpu.reg(1),cpu.reg(2)]);h=cpu.reg(0)
  cpu.pointer(h,buffer);cpu.pointer(h+0x10,buffer+16 if fixture['found'] else buffer);cpu.pointer(buffer,candidate);returned()
 elif address==0x38fb18:
  h=cpu.reg(0);trace.append(['list_pop']);cpu.pointer(h,word(h)+16);returned()
 elif address==0x38d18c:trace.append(['list_destroy']);returned()
 elif address==0x4c4bdc:
  key=[string(cpu.reg(1)),string(cpu.reg(2))];settings.append(key);trace.append(['setting',*key]);returned(frontal_angle)
 elif address==0x3d6890:
  request=cpu.reg(1);mode=cpu.reg(2);trace.append(['set_target',request,mode,byte(ai+0x78)])
  cpu.pointer(ai+0x3c,request);cpu.pointer(ai+0x40,request);returned()
 elif address==0x3d49c4:trace.append(['sync_last_target'])
 elif address==0x3d67f4:trace.append(['can_attack',cpu.reg(1)]);returned(fixture['can_attack'])
 elif address==0x3d6188:trace.append(['in_melee',cpu.reg(1)]);returned(fixture['in_melee'])
 elif address==0x3c5684:
  trace.append(['state_event',cpu.reg(1),cpu.reg(2),byte(ai+0x78),byte(ai+0x4a)]);assert cpu.reg(0)==owner+0x4fc;returned()
cpu.uc.hook_add(UC_HOOK_CODE,hook)
keys=('dead','blocked','range','attacking','last','heading','found','can_attack','in_melee','target_dead','player','dump','mode','explicit','current','ooi')
rng=random.Random(20261004);fixtures=[dict(zip(keys,values)) for values in itertools.product((0,1),repeat=6)]
fixtures=[dict(dict.fromkeys(keys,0),**f) for f in fixtures]
fixtures += [dict(zip(keys,(rng.randrange(2) for _ in keys))) for _ in range(1024)]
records=[]
for fixture in fixtures:
 cpu.uc.mem_write(owner,bytes(0x1800));cpu.pointer(owner,vt);cpu.pointer(target,vt);cpu.pointer(candidate,vt);cpu.pointer(fallback,vt);cpu.pointer(ai+4,owner)
 cpu.pointer(owner+0x51c,state);cpu.pointer(state,5 if fixture['attacking'] else 3);cpu.pointer(owner+0x528,fixture['blocked']);cpu.uc.mem_write(owner+0x1b5,bytes((fixture['heading'],)))
 cpu.uc.mem_write(ai+0x78,b'\xa5');cpu.uc.mem_write(ai+0x79,bytes((fixture['last'],)));cpu.uc.mem_write(ai+0x7a,b'\x5a');cpu.pointer(ai+0x74,0x12345678)
 cpu.pointer(ai+0x40,target if fixture['current'] else 0);cpu.pointer(ai+0x44,0);cpu.uc.mem_write(owner+0x14a8,bytes((8 if fixture['ooi'] else 255,)));cpu.pointer(owner+0x14a4,fallback);trace.clear()
 requested=target if fixture['explicit'] else 0;cpu.invoke(0x3ad87c,[owner,requested]) if not fixture['mode'] else cpu.invoke(0x3d01ac,[ai,requested,1])
 if fixture['dead'] or fixture['blocked']:assert not any(e[0]=='list_create' for e in trace)
 elif fixture['range']:assert trace[-1]==['range_redirect',requested,fixture['mode']]
 elif fixture['attacking'] and fixture['last']:assert not any(e[0]=='list_create' for e in trace)
 if not fixture['dead'] and not fixture['blocked'] and not fixture['range'] and not(fixture['attacking'] and fixture['last']):assert byte(ai+0x78)==int(bool(fixture['attacking']))
 events=[e for e in trace if e[0]=='state_event']
 if fixture['dead'] or fixture['blocked'] or fixture['range'] or fixture['attacking'] or fixture['mode']:assert events==[]
 else:
  chosen=target if fixture['explicit'] else candidate if fixture['found'] else target if fixture['current'] else 0
  if not chosen and fixture['ooi'] and not fixture['heading']:chosen=fallback
  assert word(ai+0x40)==chosen
  assert events==([['state_event',0xc354,chosen,0,1 if chosen==fallback else 0]] if not chosen or fixture['in_melee'] else []),(fixture,events,chosen)
 assert byte(ai+0x79)==fixture['last'] and byte(ai+0x7a)==0x5a and word(ai+0x74)==0x12345678
 records.append(dict(fixture=fixture.copy(),trace=trace.copy(),target=word(ai+0x40),last_target=word(ai+0x44),continued=byte(ai+0x78),seeking=byte(ai+0x4a)))
command_records=[]
for locked,blocked,forced,online,enabled,character_present,explicit in itertools.product((0,1),repeat=7):
 fixture=dict.fromkeys(keys,0);fixture.update(found=1,in_melee=1,player=1)
 cpu.uc.mem_write(owner,bytes(0x1800));cpu.pointer(owner,vt);cpu.pointer(owner+0x374,cvtable);cpu.pointer(cvtable+0x38,0x3ad874);cpu.pointer(ai+4,owner);cpu.pointer(owner+0x51c,state);cpu.pointer(state,3)
 cpu.uc.mem_write(owner+0x108,b'\x37');cpu.uc.mem_write(target+0x108,struct.pack('<I',0x1234));cpu.pointer(ai+0x40,0);cpu.pointer(ai+0x44,0);cpu.uc.mem_write(ai+0x78,b'\0');cpu.uc.mem_write(owner+0x14a8,b'\xff')
 cpu.uc.mem_write(controller,bytes(32));cpu.pointer(controller+4,owner+0x374);cpu.pointer(controller+0xc,owner if character_present else 0);cpu.uc.mem_write(controller+8,bytes((locked,forced,enabled)));cpu.uc.mem_write(0x9a318b,bytes((blocked,)));cpu.uc.mem_write(network+5,bytes((online,)));trace.clear()
 requested=target if explicit else 0;cpu.invoke(0x405b04,[controller,requested])
 gated=not forced and (locked or blocked);entries=[e for e in trace if e[0]=='melee_entry']
 if gated:assert entries==[] and trace==[]
 else:
  spec=bool(online and enabled and character_present)
  assert entries==([['melee_entry',requested,1]] if spec else [])+[['melee_entry',requested,0]],trace
  assert any(e[0]=='character_attack_entry' for e in trace)
 command_records.append(dict(locked=locked,blocked=blocked,forced=forced,online=online,enabled=enabled,character_present=character_present,explicit=explicit,trace=trace.copy()))
report=dict(validation='PASS',original_cases=len(records)+len(command_records),melee_cases=len(records),controller_cases=len(command_records),native_comparisons=0,mismatches=0,original_sha256=manifest['original_sha256'],manifest_sha256=hashlib.sha256((REF/'original-functions.json').read_bytes()).hexdigest(),script_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),scope=__doc__,settings=sorted(set(tuple(x) for x in settings)),settings_fixture_value=frontal_angle,parsed_constants_sha256=hashlib.sha256(constants_path.read_bytes()).hexdigest(),query_backend_and_fsm_are_services=True,records=records,controller_records=command_records)
(REF/'melee-probes.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('records','controller_records')}))
