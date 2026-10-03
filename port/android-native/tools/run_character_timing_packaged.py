"""Execute original-derived timing gold against optimized or actual APK ARM64.

Expiry/debug/clock/equipment/constants/AI services are explicit fixtures. The
timer, stance and state-prefix kernels execute actual ARM64 instructions. This
does not validate the complete player AIS, callback allocating during expiry,
or live frame/touch/gameplay behavior. No build/install/device operation.
"""
import argparse,hashlib,json,struct,sys,time,zipfile
from pathlib import Path
from unicorn import UC_HOOK_CODE

REPO=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(Path(__file__).resolve().parent))
from run_character_state_packaged import native_cpu,replay_state,sha,require,write_json,relative,ORIGINAL_SHA,WORLD_MEMBER
sys.path.insert(0,str(REPO/'port/level-world/tests'))
from character_timers_differential import native_timer
ROOT=REPO/'port/level-world'
GOLD=ROOT/'reference/character-timer-discovery/probe-gold.json'
GOLD_SHA='fdc2415e2b41acac78a14ea4fd33118dbe78f46765f1b45d7ec3e8a0e0bc3f5f'
EXPORTS=('dh2_character_timer_start','dh2_character_timers_update','dh2_character_timer_pause',
 'dh2_character_timer_stop','dh2_character_timers_stop_all','dh2_character_timer_time_left',
 'dh2_character_anim_stance','dh2_character_state_event')

def timer(ident=0,repeat=0,duration=10,elapsed=0,active=1,paused=0,event=0x2a,ref=0x12345678):
 return struct.pack('<IiIIBBHiQ',ident,repeat,duration,elapsed,active,paused,0,event,ref)

def replay_timing(cpu,gold):
 slots,store,services,facts,output,state,state_facts,state_services=[cpu.data+offset for offset in (0x10000,0x20000,0x20100,0x20200,0x20300,0x21000,0x22000,0x23000)]
 callback,chain_return,grow_callback,state_callback=[cpu.data+offset for offset in (0x30000,0x30020,0x30040,0x30060)]
 context=0xfedcba9876543210;owner=0xabcdef0123456789
 events=[];chain=[];action=None;mode='timer';blocked=locked=forced=0;return_address=0;grow_calls=0
 for address in (callback,chain_return,grow_callback,state_callback):cpu.uc.mem_write(address,bytes.fromhex('c0035fd6'))
 def word(address):return struct.unpack('<I',cpu.uc.mem_read(address,4))[0]
 def qword(address):return struct.unpack('<Q',cpu.uc.mem_read(address,8))[0]
 def ret(value=0):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
 def hook(uc,address,size,unused):
  nonlocal action,return_address,grow_calls
  if address==callback:
   require(cpu.reg(0)==context and cpu.reg(1)==owner,'expiry callback 64-bit context/owner')
   ptr=cpu.reg(3);ident=word(ptr);event=cpu.reg(2)&0xffffffff
   require(ptr==slots+32*ident,'borrowed native timer pointer identity')
   events.append((event,ident*32,qword(ptr+24)))
   if mode=='chain':
    chain.append(('raise',word(state+8)))
    if forced or not(locked or blocked):chain.append(('ai-expired',word(state+8)))
    chain.append(('state-event',word(state+8)))
    return_address=uc.reg_read(cpu.lr);uc.reg_write(cpu.lr,chain_return)
    for i,value in enumerate((state,state_facts,event,ptr,state_services)):cpu.put(i,value)
    uc.reg_write(cpu.pc,cpu.symbols['dh2_character_state_event']);return
   current=action;action=None
   if current=='stop':uc.mem_write(ptr+16,b'\0')
   elif current=='pause':uc.mem_write(ptr+17,b'\1')
   elif current=='restart':uc.mem_write(ptr+4,struct.pack('<iII',0,30,0));uc.mem_write(ptr+16,b'\1')
   elif current=='start-later':uc.mem_write(slots+32,timer(1,event=0x2b))
   ret()
  elif address==chain_return:
   require(cpu.reg(0)==0,'expiry state-event no-transition result')
   chain.append(('attack-on-event',word(state+8))) # Original current-OnEvent gate observation, native result projection.
   uc.reg_write(cpu.pc,return_address)
  elif address==grow_callback:
   require(cpu.reg(0)==context and cpu.reg(1)==store and word(store+24)==0,'allocator service outside Update')
   requested=cpu.reg(2);count=word(store+8);previous=qword(store);target=slots+0x1000
   uc.mem_write(target,bytes(uc.mem_read(previous,count*32))+bytes((requested-count)*32))
   uc.mem_write(store,struct.pack('<QII',target,count,requested));grow_calls+=1;ret(1)
  elif address==state_callback:raise AssertionError('unexpected state service in expiry fixture')
 for address in (callback,chain_return,grow_callback,state_callback):cpu.uc.hook_add(UC_HOOK_CODE,hook,begin=address,end=address)
 cpu.uc.mem_write(services,struct.pack('<QQQQ',context,callback,0,0))
 cpu.uc.mem_write(state_services,struct.pack('<QQ',context,state_callback))
 def reset(rows,capacity=20):
  require(len(rows)<=capacity,'timer fixture capacity')
  cpu.uc.mem_write(slots,b''.join(rows)+bytes((capacity-len(rows))*32));cpu.uc.mem_write(store,struct.pack('<QIIQII',slots,len(rows),capacity,owner,0,0));events.clear();chain.clear()
 update_cases=observations=0
 for repeat,duration,elapsed,active,paused,dt,event,expected,expected_events in gold['timers']:
  reset([timer(0,repeat,duration,elapsed,active,paused,event)])
  require(cpu.invoke('dh2_character_timers_update',[store,dt,0,services])==1,'timer Update status')
  require(bytes(cpu.uc.mem_read(slots,32))==native_timer(bytes.fromhex(expected)),f'timer state {update_cases}')
  require(events==[tuple(row) for row in expected_events],f'timer events {update_cases}')
  require(word(store+24)==0,'balanced Update depth');update_cases+=1;observations+=len(events)
 for row in gold['reentry']:
  reset([timer(0,-1),timer(1,active=0)]);action=row['case']
  require(cpu.invoke('dh2_character_timers_update',[store,35,0,services])==1,'timer reentry status')
  raw=struct.pack('<16I',*row['timer_words'])
  require(bytes(cpu.uc.mem_read(slots,64))==native_timer(raw[:32])+native_timer(raw[32:]),'timer reentry fields')
  require(events==[tuple(event) for event in row['events']],'timer reentry order')
 # 64-bit identity and controls, plus explicit allocation boundary services.
 reset([],20);require(cpu.invoke('dh2_character_timer_start',[store,10,0,0x2a,0xfedcba9876543210,services])==0,'first timer ID')
 require(qword(slots+24)==0xfedcba9876543210,'64-bit user_ref')
 cpu.uc.mem_write(slots+12,struct.pack('<I',4))
 require(cpu.invoke('dh2_character_timer_pause',[store,0,1])==1,'pause')
 require(cpu.invoke('dh2_character_timer_time_left',[output,output+4,store,0])==1 and bytes(cpu.uc.mem_read(output,8))==struct.pack('<II',4,10),'raw elapsed/duration including paused timer')
 require(cpu.invoke('dh2_character_timers_update',[store,50,1,services])==1 and word(slots+12)==4 and not events,'source script gate')
 require(cpu.invoke('dh2_character_timer_pause',[store,0,0])==1,'resume')
 require(cpu.invoke('dh2_character_timer_stop',[store,0])==1,'stop')
 require(cpu.invoke('dh2_character_timer_start',[store,20,2,-1,77,services])==0,'lowest free reuse')
 cpu.uc.mem_write(store+12,struct.pack('<I',1));before=bytes(cpu.uc.mem_read(slots,32))
 require(cpu.invoke('dh2_character_timer_start',[store,10,0,0x2a,0,services])&0xffffffff==0xfffffffe and bytes(cpu.uc.mem_read(slots,32))==before,'capacity service absent guard')
 cpu.uc.mem_write(store+24,struct.pack('<I',1))
 require(cpu.invoke('dh2_character_timer_start',[store,10,0,0x2a,0,services])&0xffffffff==0xfffffffd,'no growth during Update')
 cpu.uc.mem_write(store+24,bytes(4));cpu.uc.mem_write(services+16,struct.pack('<Q',grow_callback))
 require(cpu.invoke('dh2_character_timer_start',[store,22,0,0x2a,0,services])==1 and grow_calls==1 and word(store+8)==2,'caller growth outside Update')
 require(cpu.invoke('dh2_character_timers_stop_all',[store])==1,'stop all')
 require(cpu.invoke('dh2_character_timer_stop',[store,99])==0 and cpu.invoke('dh2_character_timer_pause',[store,99,1])==0,'out of range controls')
 cpu.uc.mem_write(services+16,bytes(8));reset([timer(active=0)])
 timer_rejects=0
 def reject(mutation=lambda:None,service_pointer=services):
  nonlocal timer_rejects
  reset([timer(active=0)]);mutation();before_store=bytes(cpu.uc.mem_read(store,32));before_timer=bytes(cpu.uc.mem_read(slots,32))
  require(cpu.invoke('dh2_character_timers_update',[store,33,0,service_pointer])&0xffffffff==0xffffffff,'malformed timer accepted')
  require(bytes(cpu.uc.mem_read(store,32))==before_store and bytes(cpu.uc.mem_read(slots,32))==before_timer and not events,'malformed timer mutation');timer_rejects+=1
 reject(lambda:cpu.uc.mem_write(store+28,struct.pack('<I',1)))
 reject(lambda:cpu.uc.mem_write(store+8,struct.pack('<I',21)))
 reject(lambda:cpu.uc.mem_write(store,bytes(8)))
 reject(service_pointer=0)
 reject(lambda:cpu.uc.mem_write(services+8,bytes(8)));cpu.uc.mem_write(services+8,struct.pack('<Q',callback))
 reject(lambda:cpu.uc.mem_write(services+24,struct.pack('<Q',1)));cpu.uc.mem_write(services+24,bytes(8))
 reject(lambda:cpu.uc.mem_write(slots+18,struct.pack('<H',1)))
 reject(lambda:cpu.uc.mem_write(slots,struct.pack('<I',7)))
 reset([timer(active=0)])
 require(cpu.invoke('dh2_character_timer_pause',[store,0,2])&0xffffffff==0xffffffff,'malformed pause');timer_rejects+=1
 require(cpu.invoke('dh2_character_timer_time_left',[0,output,store,0])&0xffffffff==0xffffffff,'null time output');timer_rejects+=1
 stance_cases=0
 for player,predicates,count,expected in gold['stances']:
  cpu.uc.mem_write(facts,struct.pack('<IiII',player|(predicates<<1),count,0,0));cpu.uc.mem_write(output,struct.pack('<i',-77))
  require(cpu.invoke('dh2_character_anim_stance',[output,facts])==1 and word(output)==expected,'source stance priority/clamp');stance_cases+=1
 stance_rejects=0
 for fp,op,data in ((0,output,bytes(16)),(facts,0,bytes(16)),(facts,output,struct.pack('<4I',64,5,0,0)),(facts,output,struct.pack('<4I',0,5,1,0)),(facts,output,struct.pack('<4I',0,5,0,1))):
  cpu.uc.mem_write(facts,data);cpu.uc.mem_write(output,struct.pack('<I',99))
  require(cpu.invoke('dh2_character_anim_stance',[op,fp])&0xffffffff==0xffffffff and word(output)==99,'malformed stance mutation');stance_rejects+=1
 gate_cases=0
 cpu.uc.mem_write(state_facts,bytes(96))
 for event,previous,expected in gold['gate_projection']:
  cpu.uc.mem_write(state,struct.pack('<i13I',5,0,previous,*([0]*11)))
  require(cpu.invoke('dh2_character_state_event',[state,state_facts,event,0,state_services])==0 and word(state+8)==expected,'source gate prefix');gate_cases+=1
 mode='chain'
 for locked,blocked,forced,ais,expected in gold['expiry_chains']:
  reset([timer()]);cpu.uc.mem_write(state,struct.pack('<i13I',5,0,0xa5,*([0]*11)))
  require(cpu.invoke('dh2_character_timers_update',[store,10,0,services])==1,'expiry chain status')
  require(chain==[tuple(row) for row in expected] and word(state+8)==0xa4,'expiry-to-prefix source ordering/projection')
  require(ais in (0,1),'null/default AIS corpus producer')
 return {'validation':'PASS','timer_update_cases':update_cases,'timer_expiry_observations':observations,
  'timer_source_reentry_cases':len(gold['reentry']),'timer_malformed_no_mutation_cases':timer_rejects,
  'stance_cases':stance_cases,'stance_malformed_no_mutation_cases':stance_rejects,'gate_projection_cases':gate_cases,
  'expiry_chain_cases':len(gold['expiry_chains']),'borrowed_context_owner_and_user_ref_above4gib':True,
  'grow_service_outside_update_only':True,'mismatches':0,
  'scope':'Actual ARM64 timer/stance/state-event prefix instructions; expiry reentry, AI routing/expired callback, allocator, clock, equipment/constants are controlled services. Source original gate before OnEvent is compared to native post-event projection; no full AIS or live frame parity.'}

def main():
 p=argparse.ArgumentParser();g=p.add_mutually_exclusive_group(required=True);g.add_argument('--apk',type=Path);g.add_argument('--library',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--skip-prior-state',action='store_true');a=p.parse_args();started=time.monotonic()
 require(sha(REPO/'.local-inputs/libDungeonHunter2.so')==ORIGINAL_SHA,'original ELF identity');require(sha(GOLD)==GOLD_SHA,'timing discovery gold identity')
 output=a.output.resolve();require(not output.exists(),'output exists; use a new report path to preserve historical proof')
 artifact={}
 if a.apk:
  apk=a.apk.resolve();require(apk.is_file(),'APK missing');artifact={'apk':str(apk),'apk_sha256':sha(apk),'apk_member':WORLD_MEMBER}
  with zipfile.ZipFile(apk) as archive:payload=archive.read(WORLD_MEMBER)
  directory=REPO/'.local-inputs/character-timing-packaged'/artifact['apk_sha256'];directory.mkdir(parents=True,exist_ok=True);library=directory/'libdh2_level_world.so';library.write_bytes(payload)
 else:library=a.library.resolve();require(library.is_file(),'ARM64 library missing');artifact={'standalone_library':str(library)}
 cpu=native_cpu(library);require(all(name in cpu.symbols for name in EXPORTS),'timing export missing')
 timing=replay_timing(cpu,json.loads(GOLD.read_text()));prior=None
 if not a.skip_prior_state:prior=replay_state(cpu,ROOT/'reference/character-state/state-reference.bin')
 sources=[ROOT/name for name in ('character_timers.cpp','character_timers.hpp','character_stance.cpp','character_stance.hpp','character_state.cpp','character_state.hpp')]
 report={'validation':'PASS',**artifact,'library_sha256':sha(library),'original_sha256':ORIGINAL_SHA,
  'gold_sha256':GOLD_SHA,'prior_state_gold_sha256':sha(ROOT/'reference/character-state/state-reference.bin'),
  'source_sha256':{relative(path):sha(path) for path in sources},'script_sha256':sha(Path(__file__)),
  'reused_package_harness_sha256':sha(Path(__file__).with_name('run_character_state_packaged.py')),
  'timing_differential':timing,'prior_state_differential':prior,'native_import_calls':cpu.import_calls,
  'data_relocations':cpu.data_relocations,'elapsed_seconds':round(time.monotonic()-started,2),'scope':__doc__}
 write_json(output,report);print(json.dumps({key:value for key,value in report.items() if key not in ('source_sha256','native_import_calls','scope')},indent=2))
if __name__=='__main__':main()
