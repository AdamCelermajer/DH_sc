"""HitFor health decisions/writes versus compiled ARM64 native source.

Original property mutation/resolution and health branches execute. Caller
fixtures supply actor/session/debug facts; kill/audio/tutorial/achievement
services are observed or omitted, not emulated as a complete actor lifecycle.
"""
import argparse,hashlib,json,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from combat_result_differential import Cpu as FloatCpu,i32,u32,bits,floating
ROOT=Path(__file__).resolve().parents[1]
class Cpu(FloatCpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='__aeabi_fcmpge':
   self.put(0,int(floating(self.reg(0))>=floating(self.reg(1))));self.import_calls['__aeabi_fcmpge']=self.import_calls.get('__aeabi_fcmpge',0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--characters',type=Path,required=True);p.add_argument('--spawn-reference',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--cases',type=int,default=10000);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/health/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});rng=random.Random(20261003)
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 character=old.data+0x1000;owner=character+0x560;main_player=old.data+0x4000;child=old.data+0x6000;game=old.data+0x8000;services=old.data+0xa000;online=old.data+0xc000;vtable=old.data+0xe000
 default_old=old.data+0x10000;type_old=default_old+900;old.pointer(0x9a645c,default_old)
 view=new.data+0x1000;request=new.data+0x1100;result=new.data+0x1200;default_new=new.data+0x2000;type_new=default_new+896;ns=[new.data+x for x in (0x4000,0x4800,0x5000,0x5800)];os=[owner+x for x in (8,0x38c,0x710,0xa94)]
 raw=(a.characters/'character_properties_pyarray.bin').read_bytes();defaults=struct.unpack_from('<224i',raw,4);types=struct.unpack_from('<224i',raw,900)
 # Original HitFor GOT service pointers and pre-existing online singleton.
 got=u32(0x3a8bdc+word(0x3a921c));global_services=word(got+word(0x3a9224))
 online_got=u32(0x7fd758+word(0x7fd78c));old.pointer(word(online_got+word(0x7fd790)),online)
 for obj in (character,main_player,child):old.pointer(obj,vtable)
 old.pointer(vtable+0x28,0x3a49f0);old.pointer(vtable+0x34,0x3a2ed4);old.pointer(vtable+0x54,0x33dd10)
 facts=0;queries=0;adds=[];sets=[];kills=[];service_counts={};omitted=0
 def returned(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def hook(uc,address,size,unused):
  nonlocal queries,omitted
  if address==0x3e0708:adds.append((old.reg(1),i32(old.reg(2))))
  elif address==0x3e07a0:sets.append((old.reg(1),i32(old.reg(2))))
  elif address in (0x36e478,0x337888,0x3140ec,0x318254,0x337a88,0x320e14,0x3a49f0,0x3a3064,0x33dd10,0x40570c,0x3bb8e4):
   key=hex(address);service_counts[key]=service_counts.get(key,0)+1
   if address==0x36e478:returned(game)
   elif address==0x337a88:queries+=1;returned(int(bool(facts&(128 if queries==1 else 256))))
   elif address==0x3a49f0:returned(int(bool(facts&(1024 if old.reg(0)==child else 2))))
   elif address==0x3a3064:returned(int(bool(facts&4)))
   elif address==0x33dd10:returned(int(bool(facts&2048)))
   elif address==0x40570c:kills.append((old.reg(1),old.reg(2)));returned()
   elif address==0x3bb8e4:returned(1) # Existing potion; tutorial service omitted.
   else:returned()
  elif address in (0x3a8dd0,0x3a8e54):
   omitted+=1;uc.reg_write(old.pc,0x3a8e54 if address==0x3a8dd0 else 0x3a8c04)
 old.uc.hook_add(UC_HOOK_CODE,hook)
 def pack(sheet):return struct.pack('<224i',*sheet)
 def fixture(sheets,f,state,armed):
  nonlocal facts,queries
  facts=f;queries=0;adds.clear();sets.clear();kills.clear()
  old.uc.mem_write(default_old,bytes(4)+pack(defaults));old.uc.mem_write(type_old,bytes(4)+pack(types));new.uc.mem_write(default_new,pack(defaults));new.uc.mem_write(type_new,pack(types))
  for o,n,s in zip(os,ns,sheets):old.uc.mem_write(o,bytes(4)+pack(s));new.uc.mem_write(n,pack(s))
  sentinel=owner+0xe18;old.uc.mem_write(sentinel,struct.pack('<4I',0,0,sentinel,sentinel));old.pointer(owner+0xe28,0)
  new.uc.mem_write(view,struct.pack('<7QII',default_new,type_new,*ns,0,0,0))
  old.uc.mem_write(character+0x1448,bytes((armed,int(bool(f&1)))));old.uc.mem_write(main_player+0x1449,bytes((int(bool(f&64)),)));old.pointer(character+0x418,child if f&512 else 0);old.pointer(character+0x11c,0x12345678)
  old.pointer(game+0x660,main_player if f&32 else 0);old.pointer(game+0x714,u32(state));old.pointer(global_services+0x40,game if f&16 else 0);old.uc.mem_write(online+5,bytes((int(bool(f&8)),)))
 references=[];counts={'dead_skips':0,'kill_requests':0,'low_health_cues':0,'negative_adds':0,'blocked_zero_adds':0};snapshots=[]
 def compare(sheets,damage,f,state,armed,label):
  fixture(sheets,f,state,armed);before=sheets[3][36]
  old.invoke(0x3a8bc4,[character,damage,0]);new.uc.mem_write(request,struct.pack('<QIIiI',view,damage,f,state,armed));new.uc.mem_write(result,b'\xcc'*32);assert new.invoke('dh2_health_hit',[result,request])==0
  expected_sheets=b''.join(bytes(old.uc.mem_read(o+4,896)) for o in os);actual=b''.join(bytes(new.uc.mem_read(n,896)) for n in ns);assert actual==expected_sheets,(label,'sheets',f,damage,state,adds,sets,struct.unpack('<8i',new.uc.mem_read(result,32)),[(i//224,i%224,x,y) for i,(x,y) in enumerate(zip(struct.unpack('<896i',expected_sheets),struct.unpack('<896i',actual))) if x!=y][:8])
  after=struct.unpack_from('<i',expected_sheets,2688+144)[0];final_armed=old.uc.mem_read(character+0x1448,1)[0];life=3 if word(character+0x11c)==3 else -1;cue=int(bool(armed and not final_armed));delta=adds[0][1] if adds else 0
  expected=struct.pack('<iiiIIIiI',delta,before,after,len(kills),final_armed,cue,life,int(bool(f&1)));actual=bytes(new.uc.mem_read(result,32));assert actual==expected,(label,struct.unpack('<8i',expected),struct.unpack('<8i',actual),f,damage,state,armed)
  assert len(adds)==(0 if f&1 else 1) and (not adds or adds[0][0]==36);assert all(p==36 and x==0 for p,x in sets) and len(kills)<=1 and all(k==(0,0) for k in kills)
  counts['dead_skips']+=bool(f&1);counts['kill_requests']+=len(kills);counts['low_health_cues']+=cue;counts['negative_adds']+=delta<0;counts['blocked_zero_adds']+=not(f&1) and delta==0
  references.append(b''.join(pack(s) for s in sheets)+struct.pack('<IIiI',damage,f,state,armed)+expected_sheets+expected)
  return struct.unpack('<iiiIIIiI',expected)
 boundary_cases=0
 for maximum in (25600,2147483647,-2147483648):
  half=int(maximum/2);threshold=int(floating(bits(float(maximum)*.75)))
  for hp in (half,half+1,threshold-1,threshold,threshold+1):
   for armed in (0,1):
    if not -2147483648<=hp<=2147483647:continue
    sheets=[list(defaults) for _ in range(4)];sheets[1][36]=sheets[3][36]=hp;sheets[3][38]=maximum
    compare(sheets,0,2|16|32,0,armed,('threshold',maximum,hp,armed));boundary_cases+=1
 for i in range(a.cases):
  sheets=[list(defaults) for _ in range(4)]
  if i>=a.cases//2:sheets=[[x if rng.randrange(4)==0 else rng.randrange(-2147483648,2147483648) for x in defaults] for _ in range(4)]
  else:sheets[1][36]=sheets[3][36]=rng.choice((0,1,256,12800,25600,rng.randrange(-256,128001)));sheets[3][38]=rng.choice((-1,0,1,25600,128000))
  damage=rng.choice((0,1,256,12800,0x7fffffff,0x80000000,0xffffffff,rng.getrandbits(32)));f=i if i<4096 else rng.randrange(4096);state=rng.choice((-1,0,1,4,5,6,2147483647,-2147483648));armed=i%2;compare(sheets,damage,f,state,armed,('synthetic',i))
  if i%2000==1999:print(f'HitFor comparisons {i+1}/{a.cases}',flush=True)
 sys.path.insert(0,str(ROOT/'../level-world/tools'));from prepare_actors import strings
 names,_=strings((a.characters/'character_properties_pyarraynames.bin').read_bytes());spawn=a.spawn_reference.read_bytes();vitals=json.loads((ROOT/'reports/vitals-arm64-differential.json').read_text());assert hashlib.sha256(spawn).hexdigest()==vitals['references']['spawn']['sha256']
 for index,name in enumerate(names):
  sheets=[list(struct.unpack_from('<224i',spawn,index*3584+part*896)) for part in range(4)]
  for damage in (256,16384):
   change=compare(sheets,damage,32|16,0,1,('source',index,damage))
   if name in ('Crypt_Skeleton','CryptSlime','CryptSlime_RE','Crypt_Ghost'):snapshots.append({'character':name,'damage_raw':damage,'change':list(change)})
 a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(struct.pack('<I',len(references))+b''.join(references))
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'synthetic_cases':a.cases,'threshold_boundary_cases':boundary_cases,'source_character_cases':len(names)*2,'comparisons':len(references),'mismatches':0,'all_four_property_sheets_and_health_requests_compared':True,'arm64_pointers_above_4gib':True,'counts':counts,'source_snapshots':snapshots,'original_import_calls':old.import_calls,'service_fact_observer_calls':service_counts,'audio_party_achievement_blocks_omitted':omitted,'reference_sha256':hashlib.sha256(a.reference_output.read_bytes()).hexdigest(),'spawn_reference_sha256':hashlib.sha256(spawn).hexdigest(),'scope':'Original HitFor health branches and original PROPS_Add/Set/resolution run. Actor classification/remoteness, debug query, session/main-player facts are caller supplied; controller kill is observed without executing its backend; tutorial/audio/party/achievement services omitted. Separate IsDead lifecycle transition and full F_ApplyResult remain pending. Buff dictionary is empty.','elapsed_seconds':round(time.monotonic()-started,2)}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
