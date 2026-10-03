"""Execute original resolver, sheet accesses, RB traversal and deque helpers.

Only caller-owned property sheets, character-table globals and ordered buff
containers are initialized. No original property/traversal function is mocked.
The reconstructed API uses architecture-independent sheets and ordered views.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../engine-resources/tests'));sys.path.insert(0,str(ROOT/'../level-world/tools'));sys.path.insert(0,str(ROOT/'tools'))
from cpu import Cpu
from prepare_actors import strings
from inspect_class_tables import parse
def pack(v):return struct.pack('<224i',*v)

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--characters',type=Path,required=True);p.add_argument('--classes',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--row-reference',type=Path,required=True);a=p.parse_args()
 manifest=json.loads((ROOT/'reference/properties/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];rules=json.loads((ROOT/'reference/properties/property-rules.json').read_text())
 old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});owner=old.data+0x1000;view=new.data+0x1000;result=new.data+0x1800
 default_old,default_new=old.data+0x2000,new.data+0x2000;types_old,types_new=default_old+900,new.data+0x2400;old.pointer(int(rules['character_array_global'],16),default_old)
 ns=[new.data+x for x in (0x4000,0x4800,0x5000,0x5800)];os=[owner+x for x in (8,0x38c,0x710,0xa94)]
 raw=(a.characters/'character_properties_pyarray.bin').read_bytes();defaults=list(struct.unpack_from('<224i',raw,4));types=list(struct.unpack_from('<224i',raw,900));names,_=strings((a.characters/'character_properties_pyarraynames.bin').read_bytes());fields,_=strings((a.characters/'character_properties_pystructnames.bin').read_bytes())
 rng=random.Random(20261002);cases=0;mutations=0;groups_tested=set();buff_counts=set();effective_types=set();combinations=set()
 def fixture(d,t,sheets,groups):
  old.uc.mem_write(default_old,bytes(4)+pack(d));old.uc.mem_write(types_old,bytes(4)+pack(t));new.uc.mem_write(default_new,pack(d));new.uc.mem_write(types_new,pack(t))
  for o,n,s in zip(os,ns,sheets):old.uc.mem_write(o,bytes(4)+pack(s));new.uc.mem_write(n,pack(s))
  sentinel=owner+0xe18;nodes=[old.data+0x8000+i*0x100 for i in range(len(groups))];old.uc.mem_write(sentinel,struct.pack('<4I',0,nodes[0] if nodes else 0,nodes[0] if nodes else sentinel,nodes[-1] if nodes else sentinel));old.pointer(owner+0xe28,len(nodes))
  oldbuf,newbuf=old.data+0x10000,new.data+0x10000;ngroups=new.data+0x8000;npointers=new.data+0x9000
  for i,group in enumerate(groups):
   node=nodes[i];old.uc.mem_write(node,bytes(0x100));old.uc.mem_write(node,struct.pack('<4I',1,nodes[i-1] if i else sentinel,0,nodes[i+1] if i+1<len(nodes) else 0))
   # Original deque blocks contain 32 ARM32 pointers. Exercise cross-block
   # distance and iterator advancement with genuine original helpers.
   omap=old.data+0xa000+i*0x100;blocks=[old.data+0xc000+i*0x400+j*0x80 for j in range(len(group)//32+1)]
   for j,block in enumerate(blocks):old.pointer(omap+j*4,block)
   old.uc.mem_write(node+0x34,struct.pack('<4I',blocks[0],blocks[0],blocks[0]+128,omap));endblock=len(group)//32;endptr=blocks[endblock]+len(group)%32*4;old.uc.mem_write(node+0x44,struct.pack('<4I',endptr,blocks[endblock],blocks[endblock]+128,omap+endblock*4))
   new.uc.mem_write(ngroups+i*16,struct.pack('<QII',npointers,len(group),0))
   for j,sheet in enumerate(group):old.uc.mem_write(oldbuf,bytes(4)+pack(sheet));new.uc.mem_write(newbuf,pack(sheet));old.pointer(blocks[j//32]+(j%32)*4,oldbuf);new.pointer(npointers+j*8,newbuf);oldbuf+=900;newbuf+=896
   npointers+=len(group)*8;buff_counts.add(len(group))
  new.uc.mem_write(view,struct.pack('<7QII',default_new,types_new,*ns,ngroups if groups else 0,len(groups),0));groups_tested.add(len(groups))
 def compare(label):
  for i,(o,n) in enumerate(zip(os,ns)):
   expected=bytes(old.uc.mem_read(o+4,896));actual=bytes(new.uc.mem_read(n,896));assert actual==expected,(label,'sheet',i,[(j,x,y) for j,(x,y) in enumerate(zip(struct.unpack('<224i',expected),struct.unpack('<224i',actual))) if x!=y][:6])
 def random_sheet(d):return [x if rng.randrange(4)==0 else rng.randrange(-0x80000000,0x80000000) for x in d]
 for repeat in range(80):
  d=defaults if repeat<40 else [rng.choice((-1,0,256,-256,0x7fffffff,-0x80000000)) for _ in range(224)]
  t=types if repeat<40 else [rng.choice((-1,0,1,2,4,8,16,32,36,3,5,7,48,63,64,-2)) for _ in range(224)]
  gcount=(0,1,3,6)[repeat%4];groups=[[random_sheet(d) for _ in range((0,1,2,5,32,40)[(i+repeat)%6])] for i in range(gcount)]
  fixture(d,t,[random_sheet(d) for _ in range(4)],groups)
  for prop in range(224):
   expected=old.invoke(0x3dfe60,[owner,prop]);assert new.invoke('dh2_property_resolve',[view,prop,result])==0;actual=struct.unpack('<I',new.uc.mem_read(result,4))[0];assert actual==expected,(repeat,prop,fields[prop],hex(expected),hex(actual));effective_types.add(16 if t[prop]==-1 else t[prop]);cases+=1
  compare(('resolve',repeat));print(f'resolve fixtures {repeat+1}/80',flush=True) if repeat%10==9 else None
 operations=((0x3e07a0,'dh2_property_set'),(0x3e0708,'dh2_property_add'),(0x3e0808,'dh2_property_set_int'),(0x3e0614,'dh2_property_set_to_sheet'))
 for repeat in range(8):
  d=defaults if repeat<4 else [rng.choice((-1,0,256,-256)) for _ in range(224)];t=types if repeat<4 else [rng.choice((-1,0,1,2,4,8,16,32,36,3,5,7,48,63)) for _ in range(224)];groups=[[random_sheet(d) for _ in range(n)] for n in (2,0,40)];sheets=[random_sheet(d) for _ in range(4)]
  for prop in range(224):
   for oldfn,newfn in operations:
    fixture(d,t,sheets,groups);value=rng.randrange(-0x80000000,0x80000000);args_old=[owner,prop,value];args_new=[view,prop,value]
    if oldfn==0x3e0614:args_old.append(os[2]);args_new.append(ns[2])
    old.invoke(oldfn,args_old);assert new.invoke(newfn,args_new)==0;compare(('mutation',repeat,prop,newfn));mutations+=1
  print(f'mutation fixtures {repeat+1}/8',flush=True)
 # Bind original class arrays and cached-sheet identity exactly as the class
 # oracle does. Then compare complete resolved sheets for the four Crypt rows.
 table=parse(a.classes)['rows'];orows,nrows=old.data+0x100000,new.data+0x100000;op,np=old.data+0x140000,new.data+0x140000
 def word(x):return struct.unpack('<I',old.uc.mem_read(x,4))[0]
 got=(0x3e2e34+word(0x3e3008))&0xffffffff;old.uc.mem_write(word(got+word(0x3e300c)),struct.pack('<I',len(table)));old.pointer(word(got+word(0x3e3010)),orows)
 for i,row in enumerate(table):
  entries=row['entries'];old.uc.mem_write(orows+i*12,struct.pack('<III',0,len(entries),op));new.uc.mem_write(nrows+i*16,struct.pack('<QII',np,len(entries),0))
  for f in entries:old.uc.mem_write(op,struct.pack('<i5i',0,*f));new.uc.mem_write(np,struct.pack('<5i',*f));op+=24;np+=20
 lgot=(0x3e2d88+word(0x3e2e18))&0xffffffff;old.pointer(lgot+word(0x3e2e1c),os[0]);snapshots=[]
 for name in ('Crypt_Skeleton','CryptSlime','CryptSlime_RE','Crypt_Ghost'):
  source=list(struct.unpack_from('<224i',raw,4+names.index(name)*896));fixture(defaults,types,[source,defaults,defaults,defaults],[]);id=source[fields.index('ClassID')]
  old.invoke(0x3e2e20,[owner,os[0],id,0]);assert new.invoke('dh2_class_apply',[nrows,len(table),id,ns[0],0])==0
  for prop in range(224):old.invoke(0x3dfe60,[owner,prop]);assert new.invoke('dh2_property_resolve',[view,prop,result])==0;cases+=1
  compare(name);resolved=bytes(new.uc.mem_read(ns[3],896));values=struct.unpack('<224i',resolved);checksum=14695981039346656037
  for byte in resolved:checksum=((checksum^byte)*1099511628211)&0xffffffffffffffff
  snapshots.append({'character':name,'resolved_sheet_sha256':hashlib.sha256(resolved).hexdigest(),'resolved_sheet_fnv1a64':f'{checksum:016x}','hp_raw':values[36],'max_hp_raw':values[38],'mp_raw':values[41],'max_mp_raw':values[43]})
 row_reference=bytearray()
 for i,name in enumerate(names):
  source=list(struct.unpack_from('<224i',raw,4+i*896));fixture(defaults,types,[source,defaults,defaults,defaults],[])
  for prop in range(224):old.invoke(0x3dfe60,[owner,prop]);assert new.invoke('dh2_property_resolve',[view,prop,result])==0;cases+=1
  compare(('raw character',name));row_reference+=bytes(old.uc.mem_read(os[3]+4,896))
  if i%64==63:print(f'original character sheets {i+1}/{len(names)}',flush=True)
 a.row_reference.parent.mkdir(parents=True,exist_ok=True);a.row_reference.write_bytes(row_reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'original_resolver':'0x3dfe60','bit_exact_property_resolutions':cases,'bit_exact_mutation_entire_state_cases':mutations,'actual_type_fixtures':40,'synthetic_type_fixtures':40,'original_character_rows_resolved':len(names),'original_character_resolved_reference_sha256':hashlib.sha256(row_reference).hexdigest(),'effective_types_tested':sorted(effective_types),'buff_group_counts_tested':sorted(groups_tested),'buff_deque_sizes_tested':sorted(buff_counts),'original_property_functions_and_container_traversal_unmocked':True,'crypt_resolved_snapshots':snapshots,'input_sha256':rules['input_sha256'],'scope':'Resolved supplied base/saved/gear sheets and ordered buff groups; actual default/type rows; original set/add/set-int/set-to-sheet. All raw character rows and four cached class base snapshots. Does not reconstruct gear/buff producers, uncached class reads, spawn HP initialization, damage or saves.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
