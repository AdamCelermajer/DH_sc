"""Complete original _F_CalculateResult versus compiled ARM64 native source.

Original getters, inventory, hit/status/damage helpers and RNG execute. Caller
fixtures own sheets/equipment. Imported IEEE float and division operations are
modeled. Only the discarded result debug-switch query is skipped.
"""
import argparse,hashlib,json,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from combat_differential import Cpu as CombatCpu,i32,u32
ROOT=Path(__file__).resolve().parents[1]
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def floating(x):return struct.unpack('<f',struct.pack('<I',u32(x)))[0]
class Cpu(CombatCpu):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='__aeabi_i2f':self.put(0,bits(i32(self.reg(0))))
  elif name=='__aeabi_f2iz':self.put(0,u32(max(-2147483648,min(2147483647,int(floating(self.reg(0)))))))
  elif name in ('__aeabi_fadd','__aeabi_fsub','__aeabi_fmul'):
   a,b=floating(self.reg(0)),floating(self.reg(1));self.put(0,bits(a+b if name.endswith('fadd') else a-b if name.endswith('fsub') else a*b))
  else:return super().external(uc,address,size,unused)
  self.import_calls[name]=self.import_calls.get(name,0)+1;self.uc.reg_write(self.pc,self.uc.reg_read(self.lr))

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--characters',type=Path,required=True);p.add_argument('--spawn-reference',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/combat-result/original-functions.json').read_text(encoding='utf-8-sig'));assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});rng=random.Random(20261003)
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 rgot=u32(0x3af6ec+word(0x3af760));seed=word(rgot+word(0x3af764));calls=word(rgot+word(0x3af768))
 igot=u32(0x3f9e1c+word(0x3f9e2c));old.pointer(word(igot+word(0x3f9e30)),old.data+0x20000)
 actors=[old.data+x for x in (0x1000,0x6000)];views=[new.data+x for x in (0x1000,0x2000)];sheets=[new.data+x for x in (0x4000,0x5000)];out_old=old.data+0x30000;out_new=new.data+0x30000;request=new.data+0x31000;nrng=new.data+0x32000;fixtures=[None,None];references=[]
 def fixture(index,properties,main,off,two,dual,shield,state,combo):
  character=actors[index];old.uc.mem_write(character,bytes(0x1800));old.pointer(character+0x564,character);old.uc.mem_write(character+0xff4,bytes(4)+struct.pack('<224i',*properties));state_info=old.data+0x10000+index*0x100;old.pointer(character+0x51c,state_info);old.uc.mem_write(state_info,struct.pack('<i',state));old.uc.mem_write(character+0x14d0,struct.pack('<H',combo))
  records=old.data+0x12000+index*0x1000;slots=records+0x100;old.pointer(character+0x390,records);old.uc.mem_write(records,struct.pack('<3I',slots,slots+12,slots+12));old.uc.mem_write(slots,bytes(12))
  table=old.data+0x20000;old.uc.mem_write(table+index*328,bytes(328))
  for j,category in enumerate((main,off)):
   item=records+0x200+j*0x100;holder=records+0x500+j*4;item_id=index*2+j;old.uc.mem_write(item,struct.pack('<II',0,item_id));old.pointer(holder,item)
   if j==0 or dual or shield:old.pointer(slots+4*(j+1),holder)
   base=table+item_id*164;old.uc.mem_write(base+0x94,struct.pack('<i',category));old.uc.mem_write(base+0x58,struct.pack('<i',4 if j==0 and two else 6 if j==1 and shield else 0));old.uc.mem_write(base+0x68,struct.pack('<i',-4 if j==0 and two else -1))
  new.uc.mem_write(sheets[index],struct.pack('<224i',*properties));new.uc.mem_write(views[index],struct.pack('<QiiIIIiII',sheets[index],main,off if dual or shield else -1,two,dual,shield,state,combo,0))
  fixtures[index]=(list(properties),(main,off if dual or shield else -1,two,dual,shield,state,combo))
 skipped=0;melee_debug_skipped=0;melee_cases=0
 def hook(uc,address,size,unused):
  nonlocal skipped,melee_debug_skipped
  if address==0x3b276c:skipped+=1;uc.reg_write(old.pc,0x3b27a8)
  elif address==0x3b33b0:melee_debug_skipped+=1;uc.reg_write(old.pc,0x3b33ec)
 old.uc.hook_add(UC_HOOK_CODE,hook,begin=0x3b276c,end=0x3b33b0)
 def compare(mask,category,element,direct,initial,count,label):
  old.uc.mem_write(seed,struct.pack('<I',initial));old.uc.mem_write(calls,struct.pack('<I',count));new.uc.mem_write(nrng,struct.pack('<II',initial,count));old.uc.mem_write(out_old,b'\xcc'*40);new.uc.mem_write(out_new,b'\xcc'*40)
  old.invoke(0x3b2638,[out_old,actors[0],actors[1],mask,category,element,direct]);new.uc.mem_write(request,struct.pack('<QQQIiii',*views,nrng,mask,category,element,direct));assert new.invoke('dh2_combat_result',[out_new,request])==0
  expected=bytes(old.uc.mem_read(out_old,40));actual=bytes(new.uc.mem_read(out_new,40));assert actual==expected,(label,hex(mask),struct.unpack('<10i',expected),struct.unpack('<10i',actual))
  original_random=bytes(old.uc.mem_read(seed,4))+bytes(old.uc.mem_read(calls,4));assert original_random==bytes(new.uc.mem_read(nrng,8)),('rng',label,original_random.hex(),bytes(new.uc.mem_read(nrng,8)).hex())
  references.append(b''.join(struct.pack('<224i',*f[0]) for f in fixtures)+b''.join(struct.pack('<iiIIIiI',*f[1]) for f in fixtures)+struct.pack('<IiiiII',mask,category,element,direct,initial,count)+expected+original_random)
  return expected,original_random
 masks=(0,1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,0x22aab5,0x5554a,0xffffffff,0x0825554a,0x0622aab5)
 synthetic=0;outcomes=[0]*9;roll_counts={}
 for i in range(8000):
  props=[]
  for j in range(2):
   row=[rng.randrange(-2147483648,2147483648) if i>=4000 else rng.randrange(-256,25601) for _ in range(224)]
   for k in (97,100,178):row[k]=rng.choice((-1,0,1,2,3,4,5))*256
   for k in (79,80,81,82,95,96,98,99,174,175,179,180,123,124):
    if i<4000:row[k]=rng.randrange(-1,100)*256
   dual=rng.randrange(2);shield=int(not dual and rng.randrange(2));fixture(j,row,rng.choice((-1,0,1,5,10)),rng.choice((-1,0,1,5,10)),rng.randrange(2),dual,shield,rng.choice((-1,0,5,9,10)),rng.randrange(65536));props.append(row)
  mask=masks[i] if i<len(masks) else rng.choice(masks) if i%2 else rng.getrandbits(32);initial=rng.getrandbits(32);count=rng.getrandbits(32)
  expected,state=compare(mask,rng.choice((-1,0,1,10)),rng.choice((-1,0,1,2,3,4,5)),rng.randrange(-2147483648,2147483648),initial,count,('synthetic',i))
  flags=struct.unpack_from('<I',expected,24)[0]
  for bit in range(9):outcomes[bit]+=bool(flags&(1<<bit))
  steps=(struct.unpack_from('<I',state,4)[0]-count)&0xffffffff;roll_counts[steps]=roll_counts.get(steps,0)+1;synthetic+=1
  if i<2000:
   offhand=i%2;alternate=(i//2)%2;old.uc.mem_write(seed,struct.pack('<I',initial));old.uc.mem_write(calls,struct.pack('<I',count));new.uc.mem_write(nrng,struct.pack('<II',initial,count))
   old.invoke(0x3b3368,[out_old,*actors,offhand,alternate]);assert new.invoke('dh2_combat_melee',[out_new,*views,nrng,offhand,alternate])==0
   assert bytes(old.uc.mem_read(out_old,40))==bytes(new.uc.mem_read(out_new,40)),('melee',i,struct.unpack('<10i',old.uc.mem_read(out_old,40)),struct.unpack('<10i',new.uc.mem_read(out_new,40)))
   assert bytes(old.uc.mem_read(seed,4))+bytes(old.uc.mem_read(calls,4))==bytes(new.uc.mem_read(nrng,8)),('melee random',i);melee_cases+=1
 sys.path.insert(0,str(ROOT/'../level-world/tools'));from prepare_actors import strings
 names,_=strings((a.characters/'character_properties_pyarraynames.bin').read_bytes());raw=a.spawn_reference.read_bytes();vitals=json.loads((ROOT/'reports/vitals-arm64-differential.json').read_text());assert hashlib.sha256(raw).hexdigest()==vitals['references']['spawn']['sha256'];assert len(names)==448
 source_sheets=[struct.unpack_from('<224i',raw,i*3584+2688) for i in range(448)];target=names.index('Crypt_Skeleton');records=[];crypt=[]
 for index,row in enumerate(source_sheets):
  fixture(0,row,-1,-1,0,0,0,5,0);fixture(1,source_sheets[target],-1,-1,0,0,0,5,0)
  for mode,mask in enumerate((0x22aab5,0x0622aab5,0x0825554a,0x80000)):
   initial=(0xD22026+index*4+mode)&0xffffffff;category=-1;element=-1;direct=1024;expected,state=compare(mask,category,element,direct,initial,0,('character',index,mode));records.append(struct.pack('<IIiiiII',index,mask,category,element,direct,initial,0)+expected+state)
   if names[index] in ('Crypt_Skeleton','CryptSlime','CryptSlime_RE','Crypt_Ghost') and mode==0:crypt.append({'character':names[index],'result':list(struct.unpack('<10i',expected)),'random_after':list(struct.unpack('<II',state))})
 assert len(references)==synthetic+len(records) and all(len(r)==1920 for r in references)
 a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(struct.pack('<I',len(references))+b''.join(references))
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'synthetic_result_cases':synthetic,'source_character_result_cases':len(records),'comparisons':synthetic+len(records),'mismatches':0,'all_ten_result_words_and_rng_state_compared':True,'arm64_pointers_above_4gib':True,'outcomes_observed':outcomes,'random_draw_counts_observed':roll_counts,'original_import_calls':old.import_calls,'discarded_debug_query_blocks_skipped':skipped,'spawn_reference_sha256':hashlib.sha256(raw).hexdigest(),'native_host_reference_sha256':hashlib.sha256(a.reference_output.read_bytes()).hexdigest(),'crypt_unarmed_melee_snapshots':crypt,'scope':'Complete _F_CalculateResult and real miss/dodge/block/critical/status/damage/getter/inventory/RNG instructions; imported IEEE soft-float/division modeled; discarded debug query skipped; supplied combatant sheets/equipment; application/targeting/full AI and lifecycle remain pending','elapsed_seconds':round(time.monotonic()-started,2)}
 report.update({'melee_request_cases':melee_cases,'melee_discarded_debug_query_blocks_skipped':melee_debug_skipped,'total_comparisons':synthetic+len(records)+melee_cases,'all_outcomes_observed':[sum(bool(struct.unpack_from('<I',r,1896)[0]&(1<<bit)) for r in references) for bit in range(9)],'melee_scope':'Original F_MeleeAttack request/category/mask and full result calculations; caller buff dictionary is empty, so alternate-mode buff removal is not exercised'})
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
