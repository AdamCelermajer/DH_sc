"""Original full damage/DoT instructions versus compiled ARM64 combat code.

Uses relocated caller character/property/inventory fixtures. Original property
getters, inventory queries, damage logic and RNG run; only imported unsigned
division is modeled. This is not the complete hit-roll/application lifecycle.
"""
import argparse,hashlib,json,random,struct,sys,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../engine-resources/tests'))
from cpu import Cpu as BaseCpu,i32,u32
class Cpu(BaseCpu):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name not in ('__aeabi_idivmod','__aeabi_uidivmod'):return super().external(uc,address,size,unused)
  convert=u32 if name=='__aeabi_uidivmod' else i32;a,b=convert(self.reg(0)),convert(self.reg(1));assert b
  quotient=(abs(a)//abs(b))*(-1 if (a<0)!=(b<0) else 1);remainder=a-quotient*b
  self.put(0,u32(quotient));self.put(1,u32(remainder));self.import_calls[name]=self.import_calls.get(name,0)+1;self.uc.reg_write(self.pc,self.uc.reg_read(self.lr))
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--characters',type=Path,required=True);p.add_argument('--spawn-reference',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/combat/original-functions.json').read_text(encoding='utf-8-sig'));assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});rng=random.Random(20261002)
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 rgot=u32(0x3af6ec+word(0x3af760));seed=word(rgot+word(0x3af764));calls=word(rgot+word(0x3af768));context=u32(0x3b2144+word(0x3b262c))
 igot=u32(0x3f9e1c+word(0x3f9e2c));old.pointer(word(igot+word(0x3f9e30)),old.data+0x20000)
 actors=[old.data+x for x in (0x1000,0x6000)];views=[new.data+x for x in (0x1000,0x2000)];sheets=[new.data+x for x in (0x4000,0x5000)];out_old=old.data+0x30000;out_new=new.data+0x30000;request=new.data+0x31000;nrng=new.data+0x32000
 def fixture(index,properties,main,off,two,dual,state,combo):
  character=actors[index];old.uc.mem_write(character,bytes(0x1800));old.pointer(character+0x564,character);old.uc.mem_write(character+0xff4,bytes(4)+struct.pack('<224i',*properties));state_info=old.data+0x10000+index*0x100;old.pointer(character+0x51c,state_info);old.uc.mem_write(state_info,struct.pack('<i',state));old.uc.mem_write(character+0x14d0,struct.pack('<H',combo))
  records=old.data+0x12000+index*0x1000;slots=records+0x100;old.pointer(character+0x390,records);old.uc.mem_write(records,struct.pack('<3I',slots,slots+12,slots+12));old.uc.mem_write(slots,bytes(12))
  table=old.data+0x20000;old.uc.mem_write(table+index*328,bytes(328))
  for j,category in enumerate((main,off)):
   item=records+0x200+j*0x100;holder=records+0x500+j*4;item_id=index*2+j;old.uc.mem_write(item,struct.pack('<II',0,item_id));old.pointer(holder,item);old.pointer(slots+4*(j+1),holder)
   base=table+item_id*164;old.uc.mem_write(base+0x94,struct.pack('<i',category));old.uc.mem_write(base+0x58,struct.pack('<i',4 if j==0 and two else 6 if j==1 and not dual else 0));old.uc.mem_write(base+0x68,struct.pack('<i',-4 if j==0 and two else -1))
  new.uc.mem_write(sheets[index],struct.pack('<224i',*properties));new.uc.mem_write(views[index],struct.pack('<QiiIIIiII',sheets[index],main,off,two,dual,int(not dual),state,combo,0))
 def set_rng(s,c=0):old.uc.mem_write(seed,struct.pack('<I',s));old.uc.mem_write(calls,struct.pack('<I',c));new.uc.mem_write(nrng,struct.pack('<II',s,c))
 def compare_rng(label):assert bytes(old.uc.mem_read(seed,4))+bytes(old.uc.mem_read(calls,4))==bytes(new.uc.mem_read(nrng,8)),('random',label)
 random_cases=0;damage_cases=0;dot_cases=0;bonus_cases=0
 for i in range(10000):
  s=rng.getrandbits(32);count=rng.choice((0,1,-1,100,-100,0x7fffffff,-0x80000000,rng.randrange(-0x80000000,0x80000000)));set_rng(s,rng.getrandbits(32));expected=i32(old.invoke(0x3af6d8,[count]));actual=i32(new.invoke('dh2_combat_random',[nrng,count]));assert expected==actual,(s,count,expected,actual);compare_rng(('random',i));random_cases+=1
 for i in range(3000):
  full=i>=2000
  props=[]
  for j in range(2):
   row=[rng.randrange(-0x80000000,0x80000000) if full else rng.randrange(-256,25601) for _ in range(224)]
   for k in (97,100,178):row[k]=rng.choice((-1,0,1,2,3,4,5))*256
   for k in (79,80,81,82,95,96,98,99,174,175,179,180,123,124):
    if i%13==0:row[k]=0
   main,off=rng.randrange(-1,7),rng.randrange(-1,7);two,dual=rng.randrange(2),rng.randrange(2);state=rng.choice((-1,0,5,9,13));combo=rng.choice((0,1,2,10,65535));fixture(j,row,main,off,two,dual,state,combo);props.append(row)
  # Bonus queries run the original inventory and item/property accessors.
  for offhand in (0,1):
   expected=i32(old.invoke(0x3df8ac,[actors[0]+0x560,offhand]));assert new.invoke('dh2_combat_bonus',[views[0],offhand,out_new])==0;actual=struct.unpack('<i',new.uc.mem_read(out_new,4))[0];assert expected==actual,('bonus',i,offhand,expected,actual);bonus_cases+=1
  flags=rng.randrange(16);type=rng.choice((0,1,2,3,4,-1));element=rng.randrange(-1,6);direct=rng.randrange(-0x80000000,0x80000000);set_rng(rng.getrandbits(32),rng.getrandbits(32));old.uc.mem_write(context+0x32,bytes((int(bool(flags&4)),int(bool(flags&8)))))
  old.uc.mem_write(out_old,b'\xcc'*28);new.uc.mem_write(out_new,b'\xcc'*28)
  old.invoke(0x3b1fb8,[out_old,actors[0],actors[1],direct,type,element,int(bool(flags&1)),int(bool(flags&2))])
  new.uc.mem_write(request,struct.pack('<QQQiiiI',*views,nrng,direct,type,element,flags));assert new.invoke('dh2_combat_damage',[out_new,request])==0
  expected=bytes(old.uc.mem_read(out_old,28));actual=bytes(new.uc.mem_read(out_new,28));assert expected==actual,('damage',i,type,flags,element,struct.unpack('<7i',expected),struct.unpack('<7i',actual),props);compare_rng(('damage',i));damage_cases+=1
  set_rng(rng.getrandbits(32),rng.getrandbits(32));magic=i%2;old.invoke(0x3b09c4,[out_old,actors[0],actors[1],magic]);assert new.invoke('dh2_combat_dot',[out_new,*views,nrng,magic])==0
  expected=bytes(old.uc.mem_read(out_old,12));actual=bytes(new.uc.mem_read(out_new+16,12));assert expected==actual,('dot',i,magic,struct.unpack('<3i',expected),struct.unpack('<3i',actual));compare_rng(('dot',i));dot_cases+=1
 sys.path.insert(0,str(ROOT/'../level-world/tools'));from prepare_actors import strings
 names,_=strings((a.characters/'character_properties_pyarraynames.bin').read_bytes());assert len(names)==448
 raw=a.spawn_reference.read_bytes();vitals=json.loads((ROOT/'reports/vitals-arm64-differential.json').read_text());assert hashlib.sha256(raw).hexdigest()==vitals['references']['spawn']['sha256']
 source_sheets=[struct.unpack_from('<224i',raw,i*3584+2688) for i in range(448)];target=names.index('Crypt_Skeleton');records=[];crypt=[]
 for index,row in enumerate(source_sheets):
  fixture(0,row,-1,-1,0,0,5,0);fixture(1,source_sheets[target],-1,-1,0,0,5,0)
  for mode,(type,flags) in enumerate(((0,0),(1,1),(2,2),(3,0))):
   initial=(0xD22026+index*4+mode)&0xffffffff;set_rng(initial);old.uc.mem_write(context+0x32,b'\0\0');element=0;direct=1024
   old.invoke(0x3b1fb8,[out_old,actors[0],actors[1],direct,type,element,int(bool(flags&1)),int(bool(flags&2))]);new.uc.mem_write(request,struct.pack('<QQQiiiI',*views,nrng,direct,type,element,flags));assert new.invoke('dh2_combat_damage',[out_new,request])==0
   expected=bytes(old.uc.mem_read(out_old,28));actual=bytes(new.uc.mem_read(out_new,28));assert expected==actual,('character',index,names[index],type,flags,struct.unpack('<7i',expected),struct.unpack('<7i',actual));compare_rng(('character',index,mode))
   rng_state=bytes(old.uc.mem_read(seed,4))+bytes(old.uc.mem_read(calls,4));records.append(struct.pack('<IiiiIII',index,type,element,direct,flags,initial,0)+expected+rng_state)
   if names[index] in ('Crypt_Skeleton','CryptSlime','CryptSlime_RE','Crypt_Ghost') and mode==0:crypt.append({'character':names[index],'damage':list(struct.unpack('<7i',expected)),'random_after':list(struct.unpack('<II',rng_state))})
 a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(struct.pack('<I',len(records))+b''.join(records))
 source_cases=len(records)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'random_cases':random_cases,'damage_cases':damage_cases,'dot_cases':dot_cases,'bonus_cases':bonus_cases,'source_character_damage_cases':source_cases,'comparisons':random_cases+damage_cases+dot_cases+bonus_cases+source_cases,'mismatches':0,'all_damage_words_and_rng_state_compared':True,'arm64_pointers_above_4gib':True,'original_import_calls':old.import_calls,'spawn_reference_sha256':hashlib.sha256(raw).hexdigest(),'native_host_reference_sha256':hashlib.sha256(a.reference_output.read_bytes()).hexdigest(),'crypt_unarmed_damage_snapshots':crypt,'scope':'Full CF__CalcDamage and CF_CalcDotDamage plus weapon-category bonus and random helper (unsigned modulo even for negative range); supplied resolved property/equipment snapshots; original inventory/property instructions execute; hit rolls, result application and full combat lifecycle remain pending','elapsed_seconds':round(time.monotonic()-started,2)}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
