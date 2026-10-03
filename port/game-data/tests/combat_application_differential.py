"""Original offline actor result application and core kill versus ARM64.

Original health/regen/property instructions execute. Aggro/status/FSM/FX/audio
services are observers. Original Ctrl_Kill guards, Kill's dead/HP writes and
RaiseEvent(2) execute; subsequent reward/script/AI services are omitted.
"""
import argparse,hashlib,json,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from health_differential import Cpu as HealthCpu,i32,u32
from combat_result_differential import floating
ROOT=Path(__file__).resolve().parents[1]
class Cpu(HealthCpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='__aeabi_fcmpgt':
   self.put(0,int(floating(self.reg(0))>floating(self.reg(1))));uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif address==self.callback:uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--characters',type=Path,required=True);p.add_argument('--spawn-reference',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--cases',type=int,default=6000);p.add_argument('--player-attacker',action='store_true');p.add_argument('--player-defender',action='store_true');a=p.parse_args();assert not (a.player_attacker and a.player_defender);started=time.monotonic()
 manifest=json.loads((ROOT/'reference/combat-application/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});rng=random.Random(20261004)
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 chars=[old.data+x for x in (0x1000,0x4000)];owners=[c+0x560 for c in chars];os=[[owner+x for x in (8,0x38c,0x710,0xa94)] for owner in owners];ns=[[new.data+0x10000+i*0x4000+x for x in (0,0x800,0x1000,0x1800)] for i in range(2)];views=[new.data+0x1000+i*0x100 for i in range(2)];states=[new.data+0x1400+i*0x100 for i in range(2)];result_old=old.data+0x20000;result_new=new.data+0x20000;request=new.data+0x21000;output=new.data+0x22000
 defaults_old=old.data+0x8000;types_old=defaults_old+900;defaults_new=new.data+0x4000;types_new=defaults_new+896;old.pointer(0x9a645c,defaults_old);game=old.data+0x30000;online=old.data+0x32000;vtable=old.data+0x34000;ai_vtable=old.data+0x35000;controllers=[old.data+0x36000+i*0x100 for i in range(2)]
 got=u32(0x3a8bdc+word(0x3a921c));global_services=word(got+word(0x3a9224));old.pointer(global_services+0x40,game);old.pointer(game+0x660,chars[1] if a.player_defender else chars[0]);old.pointer(game+0x6c4,1);old.pointer(game+0x714,0)
 online_got=u32(0x7fd758+word(0x7fd78c));old.pointer(word(online_got+word(0x7fd790)),online);old.uc.mem_write(online,bytes(72))
 old.pointer(vtable+0x28,0x3a49f0);old.pointer(vtable+0x34,0x3a2ed4);old.pointer(vtable+0x54,0x33dd10);old.pointer(vtable+0x58,0x3ad528);old.pointer(ai_vtable+0xb4,old.callback)
 rgot=u32(0x3af6ec+word(0x3af760));seed=word(rgot+word(0x3af764));random_calls=word(rgot+word(0x3af768));igot=u32(0x3f9e1c+word(0x3f9e2c));item_table=old.data+0x140000;old.pointer(word(igot+word(0x3f9e30)),item_table)
 raw=(a.characters/'character_properties_pyarray.bin').read_bytes();defaults=struct.unpack_from('<224i',raw,4);types=struct.unpack_from('<224i',raw,900)
 calls=[];adds=[];hit=None;regen=[];threat=0;statuses={};kill_events=0;omitted=0;current_label=None;fsm_state=3;idle_queries=0;low_cues=0;idle_reactions=0
 service_addresses={0x337888,0x3140ec,0x3139ac,0x318254,0x337a88,0x320e14,0x3a49f0,0x33dd10,0x3bc6b8,0x3af77c,0x3afee0,0x36e478,0x3d7c68,0x3e2720,0x3c5b3c,0x3c5c60,0x3c5d84,0x3c5ea0,0x3c5ffc,0x3c6144,0x3e2a5c,0x3a4d5c,0x31f594,0x3bb8e4}
 def returned(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def hook(uc,address,size,unused):
  nonlocal hit,threat,kill_events,omitted,idle_queries
  if address==0x3e0708:adds.append((old.reg(0),old.reg(1),i32(old.reg(2))))
  elif address==0x3a8bc4:hit={'before':word(chars[1]+0x1088),'delta':0};calls.append('hit')
  elif address==0x3bdca4 or address==0x3bdbb8:regen.append((36 if address==0x3bdca4 else 41,word(chars[0]+(0x1088 if address==0x3bdca4 else 0x109c)),len(adds)));calls.append('hp' if address==0x3bdca4 else 'mp')
  elif address==0x3a5b6c:omitted+=1;calls.append('kill');uc.reg_write(old.pc,0x3a5b48)
  elif a.player_defender and address==0x3c0260:
   assert old.reg(1)==0;idle_queries+=1 # Run original SM_IsIdle and SM_GetState instructions.
  elif a.player_defender and address in (0x3b1758,0x3a8dd0,0x3b1bb8,0x3b1c28,0x3b1c98):
   omitted+=1;uc.reg_write(old.pc,{0x3b1758:0x3b11d0,0x3a8dd0:0x3a8e54,0x3b1bb8:0x3b18d0,0x3b1c28:0x3b1860,0x3b1c98:0x3b19a0}[address])
  elif a.player_attacker and address==0x3b13e0:
   omitted+=1;uc.reg_write(old.pc,0x3b11e8) # player achievements, after core/services
  elif address in (0x3a8e54,0x3bdc28,0x3bdd14,0x3b1638,0x3b276c,0x3b33b0):
   omitted+=1;uc.reg_write(old.pc,{0x3a8e54:0x3a8c04,0x3bdc28:0x3bdc64,0x3bdd14:0x3bdd50,0x3b1638:0x3b1680,0x3b276c:0x3b27a8,0x3b33b0:0x3b33ec}[address])
  elif address in service_addresses:
   if address==0x36e478:returned(game)
   elif address==0x3a49f0:returned(int((a.player_attacker and old.reg(0)==chars[0]) or (a.player_defender and old.reg(0)==chars[1])))
   elif address==0x3bb8e4:returned(1) # non-tutorial difficulty snapshot
   elif address==0x31f594:returned(0) # no active critical-triggered skill manager
   elif address==0x3d7c68:threat=old.reg(2);calls.append('aggro');returned()
   elif address==0x3e2720:statuses['dot']=[i32(old.reg(i)) for i in (1,2,3)];calls.append('dot');returned()
   elif address in (0x3c5b3c,0x3c5c60,0x3c5d84,0x3c5ea0,0x3c5ffc,0x3c6144,0x3e2a5c):
    name={0x3c5b3c:'dodge',0x3c5c60:'block',0x3c5d84:'hurt',0x3c5ea0:'push',0x3c5ffc:'stun',0x3c6144:'fear',0x3e2a5c:'slow'}[address];statuses[name]=[i32(old.reg(i)) for i in (1,2,3)];calls.append(name);returned()
   elif address==0x3a4d5c:assert old.reg(1)==2;kill_events+=1;calls.append('death_event');returned()
   else:returned()
  elif address==old.callback:calls.append('ai_callback');returned()
 old.uc.hook_add(UC_HOOK_CODE,hook)
 def pack(s):return struct.pack('<224i',*s)
 def actor_state(index):
  c=chars[index];return struct.pack('<4Ii',old.uc.mem_read(c+0x1449,1)[0],old.uc.mem_read(c+0x1448,1)[0],struct.unpack('<H',old.uc.mem_read(c+0x14d0,2))[0],old.uc.mem_read(c+0x53b,1)[0],i32(word(c+0x11c)))
 def fixture(sheets,actor_states):
  old.uc.mem_write(defaults_old,bytes(4)+pack(defaults));old.uc.mem_write(types_old,bytes(4)+pack(types));new.uc.mem_write(defaults_new,pack(defaults));new.uc.mem_write(types_new,pack(types))
  for i in range(2):
   c=chars[i];old.uc.mem_write(c,bytes(0x2000));old.pointer(c,vtable);old.pointer(c+0x378,controllers[i]);old.pointer(controllers[i]+4,c);old.pointer(c+0x3c8,ai_vtable)
   for o,n,s in zip(os[i],ns[i],sheets[i]):old.uc.mem_write(o,bytes(4)+pack(s));new.uc.mem_write(n,pack(s))
   sentinel=owners[i]+0xe18;old.uc.mem_write(sentinel,struct.pack('<4I',0,0,sentinel,sentinel));old.pointer(owners[i]+0xe28,0);old.pointer(c+0x110,0xffffffff)
   d,armed,combo,push,life=actor_states[i];old.uc.mem_write(c+0x1448,bytes((armed,d)));old.uc.mem_write(c+0x14d0,struct.pack('<H',combo));old.uc.mem_write(c+0x53b,bytes((push,)));old.pointer(c+0x11c,u32(life))
   new.uc.mem_write(views[i],struct.pack('<7QII',defaults_new,types_new,*ns[i],0,0,0));new.uc.mem_write(states[i],struct.pack('<4Ii',*actor_states[i]))
   old.pointer(c+0x564,c);state_info=old.data+0x110000+i*0x100;old.pointer(c+0x51c,state_info);old.pointer(state_info,u32(fsm_state if a.player_defender and i==1 else 5))
   equip=old.data+0x120000+i*0x1000;slots=equip+0x100;old.pointer(c+0x390,equip);old.uc.mem_write(equip,struct.pack('<3I',slots,slots+12,slots+12));old.uc.mem_write(slots,bytes(12));item=equip+0x200;holder=equip+0x500;old.uc.mem_write(item,struct.pack('<II',0,i));old.pointer(holder,item);old.pointer(slots+4,holder);old.uc.mem_write(item_table+i*164,bytes(164));old.pointer(item_table+i*164+0x94,0xffffffff);old.pointer(item_table+i*164+0x68,0xffffffff)
 references=[];counts={'kills':0,'hits':0,'statuses':[0]*8};snapshots=[]
 def compare(sheets,actor_states,result,label,state=3):
  nonlocal hit,threat,current_label,fsm_state,low_cues,idle_reactions
  fsm_state=state
  fixture(sheets,actor_states);calls.clear();adds.clear();regen.clear();statuses.clear();hit=None;threat=0;current_label=label;old.uc.mem_write(result_old,struct.pack('<6iIIii',*result));new.uc.mem_write(result_new,struct.pack('<6iIIii',*result))
  old.invoke(0x3b10b4,[result_old,*chars,0]);new.uc.mem_write(request,struct.pack('<5Q',result_new,*views,*states));assert new.invoke('dh2_combat_apply_monster_to_player' if a.player_defender else 'dh2_combat_apply_player_to_monster' if a.player_attacker else 'dh2_combat_apply_monster',[output,request]+([int(state in (3,13,18))] if a.player_defender else []))==0
  expected_sheets=b''.join(bytes(old.uc.mem_read(o+4,896)) for row in os for o in row);actual_sheets=b''.join(bytes(new.uc.mem_read(n,896)) for row in ns for n in row);assert actual_sheets==expected_sheets,(label,'sheets')
  expected_result=bytes(old.uc.mem_read(result_old,40));assert bytes(new.uc.mem_read(result_new,40))==expected_result,(label,'result');state_after=b''.join(actor_state(i) for i in range(2));assert b''.join(bytes(new.uc.mem_read(s,20)) for s in states)==state_after,(label,'actor state',list(struct.unpack('<10i',state_after)),[struct.unpack('<5i',new.uc.mem_read(s,20)) for s in states])
  before=sheets[1][3][36];after=i32(word(chars[1]+0x1088));dead=old.uc.mem_read(chars[1]+0x1449,1)[0];kill=int(not actor_states[1][0] and bool(dead));delta=next((value for owner,p,value in adds if owner==owners[1] and p==36),0);armed=old.uc.mem_read(chars[1]+0x1448,1)[0];cue=int(bool(actor_states[1][1] and not armed));low_cues+=cue;idle_reactions+=struct.unpack('<6iIIii',expected_result)[6]!=result[6];hc=[delta,before,after,kill,armed,cue,3 if kill else -1,int(bool(hit and actor_states[1][0]))]
  regen_values=[]
  for id,before_raw,start in regen:
   raw_add=next((v for owner,p,v in adds[start:] if owner==owners[0] and p==id),0);regen_values.extend((raw_add,i32(before_raw),i32(word(chars[0]+0xff8+id*4))))
  mask=sum(1<<i for i,name in enumerate(('dot','dodge','block','hurt','push','stun','fear','slow')) if name in statuses);dot=statuses.get('dot',(0,0,0));stun=statuses.get('stun',(0,))[0];fear=statuses.get('fear',(0,))[0];slow=statuses.get('slow',(0,))[0]
  # A zero fear duration produces no service call, and is still returned as 0.
  special=int(bool(result[7]&0x18000000)) if not dead else 0;push=((result[7]>>20)&1) if not dead else 0
  expected=struct.pack('<iiiIIIiI',*hc)+struct.pack('<6i',*regen_values)+struct.pack('<III6iII',threat,int(hit is not None),mask,*dot,stun,fear,slow,special,push);actual=bytes(new.uc.mem_read(output,100));assert actual==expected,(label,'application',struct.unpack('<25i',expected),struct.unpack('<25i',actual),calls,result)
  expected_order=(['aggro','hit']+(['kill','death_event'] if kill else []) if result[0]>0 else [])+['hp','mp']+[name for name in ('dot','dodge','block','hurt','push','stun','fear','slow') if name in statuses]+([] if result[7]&0x20000000 else ['ai_callback','ai_callback']);assert calls==expected_order,(label,'service order',calls,expected_order)
  counts['hits']+=hit is not None;counts['kills']+=kill
  for bit in range(8):counts['statuses'][bit]+=bool(mask&(1<<bit))
  record=b''.join(pack(s) for owner in sheets for s in owner)+b''.join(struct.pack('<4Ii',*s) for s in actor_states)+struct.pack('<6iIIii',*result)+expected_sheets+state_after+expected_result+expected;assert len(record)==14596
  if a.player_defender:record+=struct.pack('<I',int(state in (3,13,18)))
  references.append(record)
  return expected_result,state_after,expected
 for i in range(a.cases):
  full=i>=a.cases//2;sheets=[[[x if rng.randrange(4)==0 else rng.randrange(-2147483648,2147483648) for x in defaults] if full else list(defaults) for part in range(4)] for owner in range(2)]
  if not full:
   for owner in sheets:owner[1][36]=owner[3][36]=rng.randrange(1,20001);owner[1][41]=owner[3][41]=rng.randrange(0,20001);owner[3][38]=owner[3][43]=20000
   for p in (140,143,146,185,187,189):sheets[0][3][p]=rng.choice((-1,0,1,128,255,256,768))
   sheets[0][3][204]=rng.randrange(0,2049)
  # Player modes classify exactly one supplied actor as player; the direct
  # player owner is also the main player when testing a player defender.
  states_in=[(0,rng.randrange(2),rng.randrange(65536),rng.randrange(2),rng.randrange(-10,11)),(rng.randrange(2),rng.randrange(2),rng.randrange(65536),rng.randrange(2),rng.randrange(-10,11))]
  mask=rng.getrandbits(32)&~0x400000;result=[rng.choice((-1,0,1,256,16384,rng.randrange(1,100001))),rng.randrange(-1,6),rng.choice((-1,0,1,255,256,768)),rng.choice((-1,0,1,256)),rng.randrange(-256,2049),rng.randrange(-256,2049),rng.randrange(512),mask,-1,-1]
  compare(sheets,states_in,result,('synthetic',i),rng.choice((-1,0,3,5,13,18,19)) if a.player_defender else 3)
  if i%1000==999:print(f'Monster application cases {i+1}/{a.cases}',flush=True)
 sys.path.insert(0,str(ROOT/'../level-world/tools'));from prepare_actors import strings
 names,_=strings((a.characters/'character_properties_pyarraynames.bin').read_bytes());spawn=a.spawn_reference.read_bytes();vitals=json.loads((ROOT/'reports/vitals-arm64-differential.json').read_text());assert hashlib.sha256(spawn).hexdigest()==vitals['references']['spawn']['sha256'];target=names.index('KnightPlayerBase' if a.player_defender else 'Crypt_Skeleton');defender=[list(struct.unpack_from('<224i',spawn,target*3584+p*896)) for p in range(4)]
 for index,name in enumerate(names):
  sheets=[ [list(struct.unpack_from('<224i',spawn,index*3584+p*896)) for p in range(4)],defender];states_in=[(0,1,0,0,0),(0,1,0,0,0)]
  for amount in (256,16384):
   result=[amount,-1,0,0,0,0,0,0x22aab5,-1,-1];r,s,o=compare(sheets,states_in,result,('source',index,amount))
   if name in ('CryptSlime','CryptSlime_RE','Crypt_Ghost','Crypt_Skeleton'):snapshots.append({'attacker':name,'defender':names[target],'damage_raw':amount,'result_after':list(struct.unpack('<10i',r)),'state_after':list(struct.unpack('<10i',s)),'application_words':list(struct.unpack('<25I',o))})
 live_sequences=[]
 pairs=[(name,'KnightPlayerBase') for name in ('Crypt_Skeleton','CryptSlime','Crypt_Ghost')] if a.player_defender else [('KnightPlayerBase',name) for name in ('Crypt_Skeleton','CryptSlime','Crypt_Ghost')] if a.player_attacker else [('Crypt_Skeleton','CryptSlime'),('CryptSlime','Crypt_Ghost'),('Crypt_Ghost','Crypt_Skeleton')]
 for attacker_name,defender_name in pairs:
  sheets=[[list(struct.unpack_from('<224i',spawn,names.index(name)*3584+p*896)) for p in range(4)] for name in (attacker_name,defender_name)];actor_states=[(0,1,0,0,0),(0,1,0,0,0)];old.pointer(seed,0xD22026);old.pointer(random_calls,0);attacks=[]
  for attack in range(256 if a.player_attacker or a.player_defender else 16):
   fixture(sheets,actor_states);old.invoke(0x3b3368,[result_old,*chars,0,0]);before_result=list(struct.unpack('<6iIIii',old.uc.mem_read(result_old,40)));r,s,o=compare(sheets,actor_states,before_result,('live',attacker_name,defender_name,attack));after_states=list(struct.unpack('<10i',s));words=list(struct.unpack('<25I',o));attacks.append({'result_before':before_result,'result_after':list(struct.unpack('<6iIIii',r)),'random_after':[word(seed),word(random_calls)],'state_after':after_states,'application_words':words})
   sheets=[[list(struct.unpack('<224i',old.uc.mem_read(pointer+4,896))) for pointer in row] for row in os];actor_states=[tuple(after_states[:5]),tuple(after_states[5:])]
   if actor_states[1][0]:break
  assert actor_states[1][0],(attacker_name,'target survived 16 attacks');live_sequences.append({'attacker':attacker_name,'defender':defender_name,'initial_random':[0xD22026,0],'attacks':attacks})
 walking_live_case=None
 if a.player_defender:
  sheets=[[list(struct.unpack_from('<224i',spawn,names.index(name)*3584+p*896)) for p in range(4)] for name in ('Crypt_Skeleton','KnightPlayerBase')];actor_states=[(0,1,0,0,0),(0,1,0,0,0)];old.pointer(seed,0xD22026);old.pointer(random_calls,0);fsm_state=13;fixture(sheets,actor_states);old.invoke(0x3b3368,[result_old,*chars,0,0]);before_result=list(struct.unpack('<6iIIii',old.uc.mem_read(result_old,40)));r,s,o=compare(sheets,actor_states,before_result,('walking',13),13);walking_live_case={'attacker':'Crypt_Skeleton','defender':'KnightPlayerBase','defender_fsm_state':13,'result_before':before_result,'result_after':list(struct.unpack('<6iIIii',r)),'random_after':[word(seed),word(random_calls)],'state_after':list(struct.unpack('<10i',s)),'application_words':list(struct.unpack('<25I',o))}
 a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(struct.pack('<I',len(references))+b''.join(references));report={'player_attacker':a.player_attacker,'player_defender':a.player_defender,'player_idle_queries':idle_queries,'low_health_cues':low_cues,'result_reactions_or_death_suppressions':idle_reactions,'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'synthetic_cases':a.cases,'source_cases':len(names)*2,'comparisons':len(references),'mismatches':0,'counts':counts,'core_kill_death_events':kill_events,'omitted_service_blocks':omitted,'all_owner_sheets_result_actor_states_and_requests_compared':True,'original_service_request_order_verified':True,'source_snapshots':snapshots,'live_sequences':live_sequences,'walking_live_case':walking_live_case,'reference_sha256':hashlib.sha256(a.reference_output.read_bytes()).hexdigest(),'spawn_reference_sha256':hashlib.sha256(spawn).hexdigest(),'scope':('Player defender: original SM_IsIdle(false)/SM_GetState, idle hurt/dodge mutation, low-health hysteresis, and saved block/evade/knockdown counters execute. Offline single-player non-tutorial difficulty; player stat manager, threshold achievement dispatch, tutorial and warning audio omitted. ' if a.player_defender else '')+('Player attacker, nonplayer defender, no active critical-triggered skill; player achievement branch omitted. ' if a.player_attacker else '')+'Offline single-player F_ApplyResult, direct health owner, debug switches false, living main player, no inventory gold mask. Original damage gates, combo, threat arithmetic, HitFor, regen and property instructions run; original Ctrl_Kill/Kill dead/HP prefix and death event execute. Remaining kill rewards/scripts, aggro tree, status/buff/FSM, cancel-sneak, combat text/sound and AI callbacks are observed/omitted. Impact FX skipped. Buff dictionaries empty. Live fixtures run original F_MeleeAttack/getters/inventory/result/RNG before application with persistent owner sheets.','elapsed_seconds':round(time.monotonic()-started,2)};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('source_snapshots','live_sequences','walking_live_case')}),flush=True)
if __name__=='__main__':main()
