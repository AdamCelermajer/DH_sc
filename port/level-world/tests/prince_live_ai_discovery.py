"""Bounded original instruction probes; no native comparison or full AI runtime claim."""
import hashlib,json,struct,sys,itertools
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-resources/tests'))
from cpu import Cpu
REF=ROOT/'port/level-world/reference/prince-live-ai'
manifest=json.loads((REF/'original-functions.json').read_text())
cpu=Cpu(ROOT/'.local-inputs/libDungeonHunter2.so',False,manifest)
ai=cpu.data+0x1000;owner=cpu.data+0x2000;ctrl=cpu.data+0x4000;storage=cpu.data+0x5000
records=[];timer=[]
can_attack=False;predicate_facts=(0,0,0,0,0);predicate_trace=[]
virtual=cpu.data+0x6000;vt=cpu.data+0x7000
cpu.uc.mem_write(virtual,bytes.fromhex('1eff2fe1'));cpu.pointer(vt+0x124,virtual)
def word(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
def returned(value=0):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
def hook(uc,address,size,unused):
 if can_attack and address in (0x3d574c,0x3ffd38,0x3d6188,virtual,0x3d6604):
  i=(0x3d574c,0x3ffd38,0x3d6188,virtual,0x3d6604).index(address)
  assert cpu.reg(0)==(owner+0x37c if i==1 else owner if i==3 else ai)
  if i in (0,2,4):assert cpu.reg(1)==ctrl
  predicate_trace.append(i);returned(predicate_facts[i])
 elif address in (0x3cee60,0x3ced00):
  vector=cpu.reg(5);cpu.pointer(vector+0x10,storage);cpu.pointer(vector+0x18,storage+128)
 elif address==0x3dbe24:
  assert cpu.reg(0)==owner+0x3b4 and cpu.reg(2)==0 and cpu.reg(3)==0x32
  assert word(cpu.uc.reg_read(cpu.sp))==0
  timer.append(cpu.reg(1));returned()
cpu.uc.hook_add(UC_HOOK_CODE,hook)
patterns=(0,1,2,7,8,19,31,63,85,127,128,170,199,200,254,255)
for pattern in patterns:
 for entry in (0x3ced50,0x3cebf0):
  cpu.uc.mem_write(ai,bytes((pattern,))*0x100);before=bytes(cpu.uc.mem_read(ai,0x100))
  assert cpu.invoke(entry,[ai])==ai
  after=bytes(cpu.uc.mem_read(ai,0x100))
  assert after[0x74:0x7b]==before[0x74:0x7b] and after[4:8]==before[4:8]
  assert after[0x4a:0x4c]==b'\1\1' and after[0xd0:0xd2]==b'\0\0'
  assert word(ai+0x40)==word(ai+0x44)==0 and word(storage)==ai
  assert after[0x18]==0 and word(ai+0xcc)==0xffffffff
  records.append(dict(kind='complete_charai_constructor',entry=hex(entry),prefill=pattern,attack_bytes=after[0x74:0x7b].hex(),owner_preserved=True,seeking=1,sticky=1,skill=[0,0],global_append=True))
 cpu.uc.mem_write(ctrl,bytes((pattern,))*32);assert cpu.invoke(0x408930,[ctrl,owner])==ctrl
 assert bytes(cpu.uc.mem_read(ctrl+8,3))==b'\0\0\0' and word(ctrl+4)==owner
 records.append(dict(kind='complete_hud_controller_constructor',prefill=pattern,locked=0,forced=0))
 cpu.uc.mem_write(ai,bytes((pattern,))*0x100);before=bytes(cpu.uc.mem_read(ai,0x100));cpu.invoke(0x3cb7c0,[ai,owner]);after=bytes(cpu.uc.mem_read(ai,0x100))
 assert after[:4]+after[8:]==before[:4]+before[8:] and word(ai+4)==owner
 records.append(dict(kind='set_character_preserves_session',prefill=pattern))
 for target in (0,owner,ctrl):
  cpu.uc.mem_write(ai,bytes((pattern,))*0x100);before=bytes(cpu.uc.mem_read(ai,0x100));cpu.invoke(0x3d6890,[ai,target,1]);after=bytes(cpu.uc.mem_read(ai,0x100))
  expected=bytearray(before);struct.pack_into('<I',expected,0x3c,target);struct.pack_into('<I',expected,0x40,target)
  assert after==expected
  records.append(dict(kind='forced_set_target',prefill=pattern,target=target,seeking_and_sticky_retained=True))
 cpu.uc.mem_write(ai,bytes((pattern,))*0x100);cpu.pointer(ai+4,owner);before=bytes(cpu.uc.mem_read(ai,0x100));duration=pattern*0x01010101
 timer.clear();cpu.invoke(0x3d53ec,[ai,duration]);after=bytes(cpu.uc.mem_read(ai,0x100));expected=bytearray(before);expected[0x4a]=0
 assert after==expected and timer==[duration]
 records.append(dict(kind='pause_seeking',duration=duration,seeking=0,sticky_retained=True,timer_event=0x32))
for entry in (0x3dbef8,0x3dbeec):
 for arg in (0,1,0xffffffff):
  cpu.uc.mem_write(ai,b'\xa5'*0x100);before=bytes(cpu.uc.mem_read(ai,0x100));assert cpu.invoke(entry,[ai,arg])==ai;assert bytes(cpu.uc.mem_read(ai,0x100))==before
  records.append(dict(kind='selected_ais_empty',entry=hex(entry),arg=arg))
can_attack=True
for explicit,predicate_facts in itertools.product((0,1),itertools.product((0,1),repeat=5)):
 cpu.uc.mem_write(ai,bytes(0x100));cpu.pointer(ai+4,owner);cpu.pointer(ai+0x40,ctrl);cpu.pointer(owner,vt);predicate_trace.clear()
 result=cpu.invoke(0x3d67f4,[ai,ctrl if explicit else 0])
 enemy,weapon,melee,ranged,range_ok=predicate_facts
 expected=int(bool(enemy and ((weapon and melee) or (ranged and range_ok))))
 trace=[0]
 if enemy:
  trace+=[1]
  if weapon:trace+=[2]
  if not(weapon and melee):
   trace+=[3]
   if ranged:trace+=[4]
 assert result==expected and predicate_trace==trace
 records.append(dict(kind='can_attack_original_flow',explicit_target=explicit,facts=predicate_facts,result=result,ordered_service_indices=trace))
cpu.pointer(ai+0x40,0);predicate_trace.clear();assert cpu.invoke(0x3d67f4,[ai,0])==0 and predicate_trace==[]
records.append(dict(kind='can_attack_null_target',result=0,ordered_service_indices=[]))
can_attack=False
# Execute the actual two-instruction Character constructor field producer.
# Other constructor backends are not replaced or represented by this slice.
for prior in patterns:
 cpu.uc.mem_write(owner+0x14a8,bytes((prior,)));cpu.put(4,owner);cpu.put(6,0xffffffff)
 cpu.uc.emu_start(0x3aa404,0x3aa40c,count=2);assert bytes(cpu.uc.mem_read(owner+0x14a8,1))==b'\xff'
 records.append(dict(kind='character_constructor_object_interest_slice',prefill=prior,signed_result=-1))
report=dict(validation='PASS',original_sha256=manifest['original_sha256'],probe_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),manifest_sha256=hashlib.sha256((REF/'original-functions.json').read_bytes()).hexdigest(),original_cases=len(records),native_comparisons=0,mismatches=0,scope=__doc__,services={'constructor_global_registry_storage':'caller-allocated vector capacity; original append executes','pause_seeking_timer':'timer scheduling observer only'},records=records)
(REF/'state-probes.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='records'}))

