"""Original uncached class application to the genuine resolved sheet.

Original property/class arithmetic and buff traversal run unmocked. Regen's
discarded debug-switch load/string/query/destructor block is skipped; its
result is unused before PROPS_Add. No regeneration decision is substituted.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]/'../game-data'
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
   old.invoke(0x3e2e20,[owner,os[3],id,0]);assert new.invoke('dh2_character_skill_class_v3',[nrows,len(table),id,view])==0;compare(('uncached',repeat,id));uncached+=1
  print(f'uncached class fixtures {repeat+1}/8',flush=True)
 report={'validation':'PASS','cases':uncached,'owner_words_compared':uncached*896,'original_sha256':hashlib.sha256(a.engine.read_bytes()).hexdigest(),'arm64_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'scope':'Original full recursive uncached ApplyClass3e2e20 target=resolved sheet, genuine RecalcProperty/ordered buffs/base/saved/gear arithmetic; all cache classes and random raw owner sheets. No Character construction or gameplay claim.'}
 a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
