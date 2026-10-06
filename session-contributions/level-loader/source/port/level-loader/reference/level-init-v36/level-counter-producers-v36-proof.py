"""Actual original Level counter stores and full QuickSave gating.
Counter slices stop before unrelated effects. C1 zero register is an explicit
fixture justified by earlier source MOV r6,#0; full C1 remains separately owned.
QuickSave executes the complete original routine with explicit actual-service
fixtures for player lookup, IsDead, online/hosting and Save observation. No
persistent file is written, and LevelSavegame::Save itself is not reconstructed.
"""
from pathlib import Path
import sys,struct,json,hashlib,argparse,itertools
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
shared=Path(r'C:/Users/adamc/Desktop/workspace/DH_sc');sys.path.insert(0,str(shared/'port/game-data/tests'))
from items_differential import Original
from unicorn import UC_HOOK_CODE
p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,default=shared/'.local-inputs/libDungeonHunter2.so');p.add_argument('--output',type=Path,default=Path(__file__).parent);a=p.parse_args()
c=Original(a.engine,{'functions':[]});obj=c.data+0x4000;manager=c.data+0x6000;app=c.data+0x8000;player_manager=app+0x2000;player=player_manager+0x2000;character=player+0x1000;save=character+0x2000;online=save+0x1000;vt=online+0x1000
W=lambda *v:struct.pack('<'+'I'*len(v),*(n&0xffffffff for n in v))
mode='slice';stop=0;save_calls=[];events=[];facts={}
def ret(n=0):c.put(0,n);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,address,size,unused):
 if mode=='slice' and address==stop:uc.reg_write(c.pc,c.stop);uc.emu_stop()
 elif mode=='save':
  if address==0x36e478:assert c.reg(0)==player_manager and c.reg(1)==0 and c.reg(2)==1;events.append('local_player');ret(player)
  elif address==0x3a2ed4:assert c.reg(0)==character;events.append('dead');ret(facts['dead'])
  elif address==0x7fd794:events.append('online');ret(online)
  elif address==0x36f074:assert c.reg(0)==player_manager;events.append('hosting');ret(facts['hosting'])
  elif address==0x4615ec:
   assert c.reg(0)==save;events.append('save');save_calls.append({'flag39':uc.mem_read(save+0x39,1)[0],'stored_position':list(struct.unpack('<3I',uc.mem_read(character+0x1468,12)))});ret()
 elif mode=='player_save':
  if address==0x3a49f0:assert c.reg(0)==character;events.append(('is_player',facts['is_player']));ret(facts['is_player'])
  elif address==0x3bb784:assert c.reg(0)==character;events.append(('is_blocked',facts['blocked']));ret(facts['blocked'])
  elif address==0x3bd120:assert c.reg(0)==character;events.append(('get_level',4));ret(4)
  elif address in (0x3bb840,0x3bbc58,0x3bb89c,0x3bc4a8,0x3bb770):
   assert c.reg(0)==character
   name={0x3bb840:'set_player_level',0x3bbc58:'set_save_date',0x3bb89c:'set_entry_point',0x3bc4a8:'player_save',0x3bb770:'block'}[address]
   events.append((name,c.reg(1),c.reg(2)));ret()
c.uc.hook_add(UC_HOOK_CODE,hook)
stores=[]
def slice(entry,end,registers):
 global stop
 stop=end
 for r,value in registers.items():c.put(r,value)
 c.invoke(entry,[],budget=1000)
for poison in (0,0xa5a5a5a5,0xffffffff):
 for entry,end,zero in ((0x3f321c,0x3f3230,6),(0x3f35b4,0x3f35c8,6),(0x3f751c,0x3f7524,7)):
  c.uc.mem_write(obj,bytes([poison&255])*0x200);slice(entry,end,{4:obj,zero:0,1:0});got=[c.word(obj+off) for off in (0x134,0x138)];assert got==[0,0];stores.append({'entry':hex(entry),'poison':poison,'field134':got[0],'field138':got[1]})
 c.uc.mem_write(obj,bytes([poison&255])*0x200);slice(0x3f726c,0x3f7274,{4:obj});assert c.word(obj+0x138)==500;stores.append({'entry':'0x3f726c','poison':poison,'field138':500})
got_load=0x3f69a8+c.word(0x3f7920);application_load=c.word(got_load+c.word(0x3f7a18));c.pointer(application_load+0x38,manager)
for count in (0,1,195,500,0x7fffffff,0x80000000,0xffffffff):
 c.pointer(manager+0x1c,count);c.pointer(obj+0x138,0xa5a5a5a5);slice(0x3f7104,0x3f7114,{4:obj,5:got_load,7:c.word(0x3f7a18)});assert c.word(obj+0x138)==count;stores.append({'entry':'0x3f7104','manager1c':count,'field138':count})
# Both target fields stay raw32-bit scalars; signed order appears only in tail.
assert c.word(0x3f6eb0)==0xe1520003 and c.word(0x3f6eb4)==0xb5943138 and c.word(0x3f6eb8)==0xa5943134
got_save=0x3f05b0+c.word(0x3f0688);application_save=c.word(got_save+c.word(0x3f068c));c.pointer(application_save+0x40,player_manager)
c.pointer(character,vt);c.pointer(vt+0x34,0x3a2ed4)
mode='save';cases=[]
for state,present,has_player,dead,network,force in itertools.product(range(40),(0,1),(0,1),(0,1),range(4),(0,1)):
 facts={'dead':dead,'hosting':int(network==2 or network==3)}
 c.pointer(obj+0x130,state);c.pointer(obj+0xec,save if present else 0);c.pointer(player+0x660,character if has_player else 0)
 c.uc.mem_write(online+5,bytes((int(network!=0),)));c.uc.mem_write(player_manager+0x719,bytes((int(network==3),)));c.uc.mem_write(save+0x39,b'\x4d')
 c.uc.mem_write(character+0x160,W(0x3f800000,0x40000000,0x40400000));c.uc.mem_write(character+0x1468,W(0xaaaaaaaa,0xbbbbbbbb,0xcccccccc));events.clear();save_calls.clear()
 c.invoke(0x3f059c,[obj,force],budget=2000)
 expected=bool(state==38 and present and has_player and not dead and network in (0,2));assert len(save_calls)==int(expected),(state,present,has_player,dead,network,force,events)
 assert c.uc.mem_read(save+0x39,1)==b'\x4d'
 if expected:assert save_calls==[{'flag39':0 if force else 77,'stored_position':[0x3f800000,0x40000000,0x40400000]}]
 else:assert bytes(c.uc.mem_read(character+0x1468,12))==W(0xaaaaaaaa,0xbbbbbbbb,0xcccccccc)
 cases.append({'state':state,'save_present':present,'player_present':has_player,'dead':dead,'network_fixture':network,'force':force,'saved':expected,'events':list(events)})
# Separate SG_SavePlayer original routine has NO loading-state guard.
mode='player_save';player_cases=[];c.pointer(vt+0x28,0x3a49f0);c.pointer(obj+0x110,9)
for state,present,is_player,force,blocked in itertools.product((0,7,36,37,38),(0,1),(0,1),(0,1),(0,1)):
 facts={'is_player':is_player,'blocked':blocked};c.pointer(obj+0x130,state);events.clear()
 c.invoke(0x3efa54,[obj,character if present else 0,force],budget=2000)
 saved=any(event[0]=='player_save' for event in events);assert saved==bool(present and is_player)
 if saved:
  names=[event[0] for event in events];assert names==['is_player','is_blocked']+(['block'] if force else[])+['get_level','set_player_level','set_save_date','set_entry_point','player_save','block']
  assert next(event[1:] for event in events if event[0]=='set_entry_point')==(9,0xffffffff)
  assert events[-1][1]==blocked
 player_cases.append({'state':state,'player_present':present,'is_player':is_player,'force':force,'blocked':blocked,'saved':saved})
report={'validation':'PASS','original_sha256':hashlib.sha256(a.engine.read_bytes()).hexdigest(),'counter_store_cases':len(stores),'counter_store_results':stores,'quicksave_cases':len(cases),'quicksave_saves':sum(case['saved'] for case in cases),'quicksave_incomplete_saves':sum(case['saved'] and case['state']!=38 for case in cases),'quicksave_scope':__doc__,'quicksave_boundary_cases':[case for case in cases if case['state'] in (36,37,38,39) and case['save_present'] and case['player_present'] and not case['dead']],'whole_load_init_verified':False,'sg_save_player_cases':len(player_cases),'sg_save_player_incomplete_save_calls':sum(case['saved'] and case['state']!=38 for case in player_cases),'sg_save_player_scope':'Whole original SG_SavePlayer(Character*,bool) control flow with explicit IsPlayer/blocked/level/save-effect observers. No actual persistent save writes.'}
a.output.mkdir(parents=True,exist_ok=True);(a.output/'level-counter-producers-v36-original.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('counter_store_results','quicksave_boundary_cases','quicksave_scope')}))
