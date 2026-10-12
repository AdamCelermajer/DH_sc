"""Original-only eligible Idle prerequisites, not complete Character frame parity.

Outer constructors run with named allocation/base/container/property providers.
CanUpdate runs its actual eligible visual-node branch with online/Player lookup
fixtures. AI Update runs the actual zoned skip, FSM runs actual Idle OnUpdate,
and state FX runs its actual unchanged/non-slow return. No native counterpart,
scene/Step/navigation/timer-expiry/FX factory or whole application is claimed.
"""
import hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[4]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from navigation_motion_differential import MotionCpu
REF=Path(__file__).resolve().parent
manifest=json.loads((REF/'original-functions.json').read_text())
c=MotionCpu(ROOT/'.local-inputs/libDungeonHunter2.so',False,manifest)
owner=c.data+0x1000;controller=c.data+0x5000;storage=c.data+0x6000
airow=c.data+0x7000;visual=c.data+0x8000;node=c.data+0x9000
online=c.data+0xa000;player=c.data+0xb000;info=c.data+0xc000
behavior=c.data+0xd000;vt=c.data+0xe000
app=struct.unpack('<I',c.uc.mem_read(0x994a98+0x37f4,4))[0]
mode='';calls=[]
services={0x33f310:'ObjectBase constructor backend',0x524644:'PFObject storage',
 0x31167c:'FixedString storage',0x4a2730:'NetworkObject constructor',
 0x38aac8:'PF radius update',0x4a191c:'NetworkObject bind',
 0x3ff330:'Inventory constructor',0x3dbb0c:'Timers constructor',
 0x3df084:'Properties constructor',0x3a6a24:'network snapshot',
 0x3db480:'Timers SetCharacter',0x3cb7c0:'AI SetCharacter',
 0x3c9890:'Animator SetCharacter',0x3c1600:'FSM SetCharacter',
 0x3dec0c:'Properties SetCharacter',0x3c7318:'FSM StateInfo registration',
 0x310570:'allocation'}
def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def byte(p):return c.uc.mem_read(p,1)[0]
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,address,size,_):
 if mode=='constructor':
  if address in services:
   calls.append({'key':hex(address),'fixture':services[address]})
   if address==0x310570:assert c.reg(0)==16;ret(controller)
   elif address in (0x3db480,0x3cb7c0,0x3c9890,0x3c1600,0x3dec0c):c.pointer(c.reg(0)+4,c.reg(1));ret()
   else:ret(c.reg(0))
  elif address in (0x3cee60,0x3ced00):
   p=c.reg(5);c.pointer(p+0x10,storage);c.pointer(p+0x18,storage+128)
 else:
  if address==0x7fd794:calls.append({'fixture':'current online receiver','byte5':0});ret(online)
  elif address==0x36e478:
   assert c.reg(1)==0 and c.reg(2)==1
   calls.append({'fixture':'GetPlayer(0,true) +660 character projection'});ret(player)
  elif address==0x3a3024:
   calls.append({'fixture':'resolved current AI row','type':4});ret(airow)
  elif address in (0x3cb908,0x3cc5a4,0x3cf3f0,0x3d1050):
   raise AssertionError('Zoned AI skip unexpectedly entered '+hex(address))
  elif address in (0x33de90,0x494978,0x495430):
   raise AssertionError('Eligible unchanged branch unexpectedly entered backend '+hex(address))
  elif address==0x31f66c:calls.append({'actual':'Application.GetDt','word':word(app+0x8c)})
c.uc.hook_add(UC_HOOK_CODE,hook)
records=[]
for pattern in (0,1,85,170,255):
 mode='constructor';calls=[];c.uc.mem_write(owner,bytes([pattern])*0x2100);c.uc.mem_write(controller,bytes([pattern])*32)
 assert c.invoke(0x3aa1b4,[owner,0])==owner
 raw={hex(off):word(owner+off) for off in (0x14e0,0x14fc,0x1484,0x1488,0x148c,0x1490,0x1494,0x1498,0x149c,0x14a0,0x1500,0x1504)}
 assert raw=={hex(off):0xbf800000 if off==0x14fc else 0xffffffff if off in (0x1498,0x1500,0x1504) else 0 for off in (0x14e0,0x14fc,0x1484,0x1488,0x148c,0x1490,0x1494,0x1498,0x149c,0x14a0,0x1500,0x1504)}
 assert byte(owner+0x2fc)==0 and byte(owner+0x2ee)==1 and byte(owner+0x2f0)==0
 assert word(word(controller)+8)==0x3a2f44
 records.append({'kind':'actual_outer_constructor','prefill':pattern,'raw_words':raw,'forced_update_2fc':0,'zoned':1,'in_zone':0,'controller_update_key':'0x3a2f44','providers':calls})
# Fresh source projection; initialized/published state inputs below are fixtures,
# not an execution of the whole XML/InitPost/LevelLoadStates pipeline.
mode='constructor';calls=[];c.uc.mem_write(owner,bytes(0x2100));c.uc.mem_write(controller,bytes(32));c.invoke(0x3aa1b4,[owner,0])
c.pointer(airow+0x38,4);c.pointer(visual+8,node);c.pointer(owner+0x2d8,visual)
c.pointer(player+0x660,c.data+0xf000);c.uc.mem_write(online+5,b'\0')
mode='eligible'
for enabled in (0,1,255):
 for stale_node_flag in (0,1,255):
  calls=[];c.uc.mem_write(owner+0x80,bytes([enabled]));c.uc.mem_write(node+0x200,bytes([stale_node_flag]));c.pointer(node+0x118,0)
  assert c.invoke(0x3a52a4,[owner])==1
  assert byte(node+0x200)==int(enabled!=0)
  records.append({'kind':'actual_CanUpdate_eligible_visual','enabled80':enabled,'initial_node200':stale_node_flag,'result':1,'final_node200':byte(node+0x200),'providers':calls})
calls=[];assert c.invoke(0x3a2f44,[controller])==controller
records.append({'kind':'actual_default_controller_update','providers':calls})
for flags in (0x2380,0x23c1):
 calls=[];c.pointer(owner+0x520,flags);c.pointer(owner+0x3e4,c.data+0x11000)
 assert byte(owner+0x3e0)==0 and byte(controller+8)==byte(controller+9)==0
 old=byte(owner+0x88);c.invoke(0x3cfbf4,[owner+0x3c8]);assert byte(owner+0x88)==old
 records.append({'kind':'actual_outer_AI_zoned_skip','flags520':flags,'updated88_retained':old,'providers':calls})
# Actual FSM virtual receiver layout: StateInfo{ID,behavior}; behavior.v14.
c.pointer(info,3);c.pointer(info+4,behavior);c.pointer(behavior,vt);c.pointer(vt+0x14,0x3c0e80);c.pointer(owner+0x51c,info)
c.pointer(owner+0x528,0);c.uc.mem_write(owner+0x1b5,b'\0')
for elapsed in (0,1,0xfffffff0,0xffffffff):
 for dt in (0,1,16,1000,0xffffffff):
  calls=[];c.pointer(owner+0x55c,elapsed);c.pointer(app+0x8c,dt);c.invoke(0x3c628c,[owner+0x4fc]);expected=(elapsed+dt)&0xffffffff
  assert word(owner+0x55c)==expected
  assert sum(v.get('actual')=='Application.GetDt' for v in calls)==1
  records.append({'kind':'actual_FSM_then_Idle','previous':elapsed,'dt':dt,'elapsed':expected,'providers':calls})
# Actual source WalkSpeed input is CharProperties+0xb50 = Character+0x10b0,
# resolved property46. Nonnegative modifiers yield >=1; this is not HP ratio.
for raw in (0,1,256,0x7fffffff):
 calls=[];c.pointer(owner+0x10b0,raw);c.pointer(owner+0x1490,0);c.pointer(owner+0x528,0);c.pointer(owner+0x520,0x2380)
 c.invoke(0x3a4470,[owner]);assert word(owner+0x1490)==0
 records.append({'kind':'actual_Idle_unchanged_stateFX','resolved_walk_modifier':raw,'final_stateFX1490':0,'providers':calls})
inputs=[Path(__file__),REF/'original-functions.json',REF/'constructors/original-functions.json',REF/'dependencies/original-functions.json',REF/'eligible-helpers/original-functions.json',REF/'fx-queries/original-functions.json']
report={'validation':'PASS','scope':__doc__,'original_sha256':manifest['original_sha256'],'original_cases':len(records),'native_comparisons':0,'full_frame':False,'source_sha256':{p.relative_to(ROOT).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs},'imports':c.import_calls,'records':records}
(REF/'original-prerequisites.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='records'}))
