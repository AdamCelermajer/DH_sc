"""Original uncached class reads, regeneration and fresh spawn HP/MP.

Original property/class arithmetic and buff traversal run unmocked. Regen's
discarded debug-switch load/string/query/destructor block is skipped; its
result is unused before PROPS_Add. No regeneration decision is substituted.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../engine-resources/tests'));sys.path.insert(0,str(ROOT/'../level-world/tools'));sys.path.insert(0,str(ROOT/'tools'))
from cpu import Cpu,i32
from prepare_actors import strings
from inspect_class_tables import parse
def pack(v):return struct.pack('<224i',*v)
def checksum(raw):
 h=14695981039346656037
 for x in raw:h=((h^x)*1099511628211)&0xffffffffffffffff
 return f'{h:016x}'

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--characters',type=Path,required=True);p.add_argument('--classes',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-prefix',type=Path,required=True);a=p.parse_args()
 manifest=json.loads((ROOT/'reference/vitals/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});character=old.data+0x1000;owner=character+0x560;view=new.data+0x1000;result=new.data+0x1800
 default_old,default_new=old.data+0x4000,new.data+0x2000;types_old,types_new=default_old+900,new.data+0x2400;old.pointer(0x9a645c,default_old)
 ns=[new.data+x for x in (0x4000,0x4800,0x5000,0x5800)];os=[owner+x for x in (8,0x38c,0x710,0xa94)]
 raw=(a.characters/'character_properties_pyarray.bin').read_bytes();defaults=list(struct.unpack_from('<224i',raw,4));types=list(struct.unpack_from('<224i',raw,900));names,_=strings((a.characters/'character_properties_pyarraynames.bin').read_bytes());fields,_=strings((a.characters/'character_properties_pystructnames.bin').read_bytes());table=parse(a.classes)['rows'];rng=random.Random(20261002)
 def word(x):return struct.unpack('<I',old.uc.mem_read(x,4))[0]
 got=(0x3e2e34+word(0x3e3008))&0xffffffff;old.uc.mem_write(word(got+word(0x3e300c)),struct.pack('<I',len(table)));orows,nrows=old.data+0x100000,new.data+0x100000;old.pointer(word(got+word(0x3e3010)),orows);op,np=old.data+0x140000,new.data+0x140000
 for i,row in enumerate(table):
  entries=row['entries'];old.uc.mem_write(orows+i*12,struct.pack('<III',0,len(entries),op));new.uc.mem_write(nrows+i*16,struct.pack('<QII',np,len(entries),0))
  for f in entries:old.uc.mem_write(op,struct.pack('<i5i',0,*f));new.uc.mem_write(np,struct.pack('<5i',*f));op+=24;np+=20
 lgot=(0x3e2d88+word(0x3e2e18))&0xffffffff;old.pointer(lgot+word(0x3e2e1c),old.data+0x6000)
 def fixture(d,t,sheets,groups=()):
  old.uc.mem_write(default_old,bytes(4)+pack(d));old.uc.mem_write(types_old,bytes(4)+pack(t));new.uc.mem_write(default_new,pack(d));new.uc.mem_write(types_new,pack(t))
  for o,n,s in zip(os,ns,sheets):old.uc.mem_write(o,bytes(4)+pack(s));new.uc.mem_write(n,pack(s))
  sentinel=owner+0xe18;nodes=[old.data+0x8000+i*0x100 for i in range(len(groups))];old.uc.mem_write(sentinel,struct.pack('<4I',0,nodes[0] if nodes else 0,nodes[0] if nodes else sentinel,nodes[-1] if nodes else sentinel));old.pointer(owner+0xe28,len(nodes));ob,nb=old.data+0x10000,new.data+0x10000;ngroups=new.data+0x8000;npointers=new.data+0x9000
  for i,group in enumerate(groups):
   node=nodes[i];old.uc.mem_write(node,bytes(0x100));old.uc.mem_write(node,struct.pack('<4I',1,nodes[i-1] if i else sentinel,0,nodes[i+1] if i+1<len(nodes) else 0));omap=old.data+0xa000+i*0x100;blocks=[old.data+0xc000+i*0x400+j*0x80 for j in range(len(group)//32+1)]
   for j,block in enumerate(blocks):old.pointer(omap+j*4,block)
   old.uc.mem_write(node+0x34,struct.pack('<4I',blocks[0],blocks[0],blocks[0]+128,omap));endblock=len(group)//32;old.uc.mem_write(node+0x44,struct.pack('<4I',blocks[endblock]+len(group)%32*4,blocks[endblock],blocks[endblock]+128,omap+endblock*4));new.uc.mem_write(ngroups+i*16,struct.pack('<QII',npointers,len(group),0))
   for j,sheet in enumerate(group):old.uc.mem_write(ob,bytes(4)+pack(sheet));new.uc.mem_write(nb,pack(sheet));old.pointer(blocks[j//32]+j%32*4,ob);new.pointer(npointers+j*8,nb);ob+=900;nb+=896
   npointers+=len(group)*8
  new.uc.mem_write(view,struct.pack('<7QII',default_new,types_new,*ns,ngroups if groups else 0,len(groups),0))
 def state():return b''.join(bytes(old.uc.mem_read(o+4,896)) for o in os)
 def compare(label):
  expected=state();actual=b''.join(bytes(new.uc.mem_read(n,896)) for n in ns);assert actual==expected,(label,[(i//224,i%224,x,y) for i,(x,y) in enumerate(zip(struct.unpack('<896i',expected),struct.unpack('<896i',actual))) if x!=y][:8]);return expected
 def random_sheet(d):return [x if rng.randrange(4)==0 else rng.randrange(-0x80000000,0x80000000) for x in d]
 uncached=0
 for repeat in range(8):
  for id in range(len(table)):
   sheets=[random_sheet(defaults) for _ in range(4)];sheets[0][19]=(256,512,2560,12800,-1,0x7fffffff,-0x80000000,0)[repeat]
   groups=[[random_sheet(defaults) for _ in range(n)] for n in ((2,0,40) if repeat%2 else ())];fixture(defaults,types,sheets,groups)
   old.invoke(0x3e2e20,[owner,os[0],id,0]);assert new.invoke('dh2_class_apply_to_base',[nrows,len(table),id,ns[0],view])==0;compare(('uncached',repeat,id));uncached+=1
  print(f'uncached class fixtures {repeat+1}/8',flush=True)
 # Skip only the discarded debug-switch calls. The old instructions still
 # calculate and cap delta, invoke PROPS_Add, and resolve changed properties.
 skipped=0;add_calls=[]
 def hook(uc,address,size,unused):
  nonlocal skipped
  if address in (0x3bdc28,0x3bdd14):skipped+=1;uc.reg_write(old.pc,0x3bdc64 if address==0x3bdc28 else 0x3bdd50)
  elif address==0x3e0708:add_calls.append((old.reg(1),i32(old.reg(2))))
 old.uc.hook_add(UC_HOOK_CODE,hook,begin=0x3bdc28,end=0x3bdd14);old.uc.hook_add(UC_HOOK_CODE,hook,begin=0x3e0708,end=0x3e0708)
 regen_cases=0;positive=0
 for i in range(10000):
  sheets=[random_sheet(defaults) for _ in range(4)];mana=i%2;current_id,max_id=(41,43) if mana else (36,38)
  if i<5000:
   sheets[0][current_id]=-1;sheets[1][current_id]=sheets[3][current_id]=rng.randrange(-256,128001);sheets[3][max_id]=rng.randrange(0,128001);amount=rng.choice((-1,0,1,256,12800,128000))
  else:amount=rng.randrange(-0x80000000,0x80000000)
  fixture(defaults,types,sheets);before=sheets[3][current_id];add_calls.clear();old.invoke(0x3bdbb8 if mana else 0x3bdca4,[character,amount]);assert new.invoke('dh2_vitals_regen',[view,mana,amount,result])==0;compare(('regen',i,mana,amount))
  change=struct.unpack('<3i',new.uc.mem_read(result,12));expected_delta=add_calls[0][1] if add_calls else 0;assert len(add_calls)<=1 and (not add_calls or add_calls[0][0]==current_id);after=struct.unpack('<i',old.uc.mem_read(os[3]+4+current_id*4,4))[0];assert change==(expected_delta,before,after),(i,change,expected_delta,before,after);positive+=expected_delta>0;regen_cases+=1
  if i%2000==1999:print(f'regen cases {i+1}/10000',flush=True)
 before_reference=bytearray();after_reference=bytearray();snapshots=[]
 for i,name in enumerate(names):
  source=list(struct.unpack_from('<224i',raw,4+i*896));fixture(defaults,types,[source,defaults,defaults,defaults]);old.invoke(0x3e0810,[owner,1]);assert new.invoke('dh2_class_recalc_base',[nrows,len(table),ns[0],view])==0;before_reference+=compare(('recalc original character',name))
  add_calls.clear();old.invoke(0x3b3a70,[character]);old.invoke(0x3b3a70,[character]);assert new.invoke('dh2_vitals_spawn_init',[view,result])==0;reference=compare(('spawn vitals',name));after_reference+=reference
  if name in ('Crypt_Skeleton','CryptSlime','CryptSlime_RE','Crypt_Ghost'):
   resolved=reference[3*896:4*896];values=struct.unpack('<224i',resolved);changes=list(struct.unpack('<12i',new.uc.mem_read(result,48)));snapshots.append({'character':name,'hp_raw':values[36],'max_hp_raw':values[38],'mp_raw':values[41],'max_mp_raw':values[43],'resolved_sheet_fnv1a64':checksum(resolved),'resolved_sheet_sha256':hashlib.sha256(resolved).hexdigest(),'init_passes':2,'changes':[changes[j:j+3] for j in range(0,12,3)]})
  if i%64==63:print(f'original recalc/spawn sheets {i+1}/{len(names)}',flush=True)
 references={}
 for suffix,data in (('uncached',before_reference),('spawn',after_reference)):
  path=Path(str(a.reference_prefix)+'-'+suffix+'.bin');path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(data);references[suffix]={'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest()}
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'uncached_class_entire_owner_state_cases':uncached,'regeneration_entire_owner_state_cases':regen_cases,'positive_regeneration_cases':positive,'full_original_character_class_recalc_cases':len(names),'original_character_two_pass_spawn_vitals_cases':len(names),'crypt_spawn_snapshots':snapshots,'original_stat_functions_unmocked':True,'original_regen_debug_switch_query_blocks_skipped':skipped,'discarded_query_blocks':['0x3bdc28..0x3bdc60','0x3bdd14..0x3bdd4c'],'fixture_scope':'Initialized original class/table globals, raw sheets and valid ordered buff containers. Original uncached source resolution and regen cap/add decisions execute. Debug-switch load/query/string lifecycle omitted; query result is discarded. Fresh InitPost HP/MP call sequence inferred from its Revive call and explicit _InitHpMp call; full InitPost/Revive and actor lifecycle not emulated.','references':references,'input_sha256':{f.name:hashlib.sha256(f.read_bytes()).hexdigest() for folder,files in ((a.characters,('character_properties_pyarray.bin','character_properties_pyarraynames.bin','character_properties_pystructnames.bin')),(a.classes,('character_classes_pyarray.bin','character_classes_pyarraynames.bin','character_classes_pystructnames.bin'))) for f in (folder/name for name in files)},'combat_or_regen_scheduling_implemented':False}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
