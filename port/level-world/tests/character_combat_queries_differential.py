"""Actual range/inventory/table lookup instructions vs optimized native ARM64.

Character cached property words and immutable ItemTable/CharAnimTable/AnimTable
rows are concrete fixture storage. Equipment ownership is borrowed read-only.
No query capability bool or GetItem/GetCharAnimTableId return is mocked.
"""
import argparse,hashlib,itertools,json,struct,sys,time
from pathlib import Path
from navigation_differential import Cpu,ROOT
sys.path.insert(0,str(ROOT/'tools'))
from prepare_player_combat import animation_tables
from prepare_actors import strings
def put(cpu,address,value):cpu.uc.mem_write(address,words(value))
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--manifest',type=Path,required=True);p.add_argument('--assets',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);a=p.parse_args();m=json.loads(a.manifest.read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==m['original_sha256'];old=Cpu(a.engine,False,m);new=Cpu(a.library,True,{'functions':[]})
 raw=(a.assets/'data/animations_pyarray.bin').read_bytes();sequences,characters=animation_tables(raw);banks=[c[0][0] for c in characters];types=[s['type'] for s in sequences];itemtypes=[-3,0,1,2,3,4,5,6,7,0x7fffffff]
 op=old.data+0x10000;oi=op+0x37c;sets=old.data+0x14000;nodes=old.data+0x15000;refs=old.data+0x16000;inst=old.data+0x17000;items=old.data+0x18000;bankptr=old.data+0x20000;bankdata=old.data+0x21000;countptr=old.data+0x30000;seqcountptr=countptr+16;seqptr=countptr+32;seqdata=old.data+0x31000;itemptr=old.data+0x40000
 np=new.data+0x10000;ni=new.data+0x11000;ns=ni+0x100;nr=ni+0x200;nn=ni+0x300;nt=new.data+0x18000;nb=new.data+0x21000;nseq=new.data+0x31000
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 got=0x3a3480+word(0x3a34e0);put(old,got+word(0x3a34e4),bankptr);put(old,bankptr,bankdata);put(old,got+word(0x3a34e8),seqcountptr);put(old,seqcountptr,len(types));put(old,got+word(0x3a34ec),seqptr);put(old,seqptr,seqdata)
 got=0x3a3240+word(0x3a325c);put(old,got+word(0x3a3260),countptr);put(old,countptr,len(banks))
 got=0x3f9e1c+word(0x3f9e2c);put(old,got+word(0x3f9e30),itemptr);put(old,itemptr,items)
 for i,t in enumerate(itemtypes):put(old,items+i*164+0x58,t);put(new,nt+i*164+88,t)
 for i,b in enumerate(banks):put(old,bankdata+i*160+4,b);new.uc.mem_write(nb+i*8,words(b,0))
 for i,t in enumerate(types):put(old,seqdata+i*20+16,t);put(new,nseq+i*4,t)
 put(old,oi+0x14,sets)
 for i in range(2):put(old,sets+12*i,nodes+16*i);put(old,refs+4*i,inst+16*i)
 new.uc.mem_write(np,bytes(896));new.uc.mem_write(ni,struct.pack('<QIi',ns,2,0))
 cases=[];sentinel=-2147483648
 for kind,property32,current,h0,h1 in itertools.product(range(2),(-2147483648,-2,-1,0,1,4,0x7fffffff),range(2),range(-1,10),range(-1,10)):cases.append((kind,48,property32,current,h0,h1,sentinel,sentinel))
 for bank in range(-3,len(banks)+4):cases.append((2,bank,-1,0,-1,-1,sentinel,sentinel))
 for attack,t in itertools.product((-2,-1,0,len(types)-1,len(types),len(types)+1),(-3,0,1,2,7)):cases.append((2,48,-1,0,-1,-1,attack,t))
 props=(a.assets/'data/character_properties_pyarray.bin').read_bytes();fields,_=strings((a.assets/'data/character_properties_pystructnames.bin').read_bytes());assert len(fields)==224
 for i in range(struct.unpack_from('<I',props)[0]):
  row=struct.unpack_from('<224i',props,4+896*i);cases.append((0,row[2],row[32],0,-1,-1,sentinel,sentinel));cases.append((2,row[2],row[32],0,-1,-1,sentinel,sentinel))
 records=[];totals=[0,0,0];started=time.monotonic()
 for kind,bank,prop,current,h0,h1,attack,typ in cases:
  put(old,op+0x1000,bank);put(old,op+0x1078,prop);put(new,np+8,bank);put(new,np+128,prop);old.uc.mem_write(oi+0x2e,bytes([current]));new.uc.mem_write(ni+12,words(current))
  for i,id in enumerate((h0,h1)):
   put(old,nodes+16*i+4,0 if id<0 else refs+4*i);put(old,inst+16*i+4,id)
   new.uc.mem_write(ns+8*i,struct.pack('<Q',0 if id<0 else nr+8*i));new.uc.mem_write(nr+8*i,struct.pack('<Q',nn+8*i));new.uc.mem_write(nn+8*i,words(id))
  selected=bank if 0<=bank<len(banks) else 17
  if attack!=sentinel:put(old,bankdata+selected*160+4,attack);new.uc.mem_write(nb+selected*8,words(attack))
  target=attack if attack!=sentinel else banks[selected]
  if typ!=sentinel and 0<=target<len(types):put(old,seqdata+target*20+16,typ);put(new,nseq+target*4,typ)
  before=bytes(new.uc.mem_read(np,896))
  result=old.invoke((0x3a4d3c,0x3fffa4,0x3a346c)[kind],[op if kind!=1 else oi])
  native=new.invoke(('dh2_character_can_range_attack','dh2_inventory_has_ranged_weapon','dh2_character_has_combo_attack')[kind],([np,ni,nt,len(itemtypes)],[ni,nt,len(itemtypes)],[np,nb,len(banks),nseq,len(types)])[kind])
  assert native==result,(kind,bank,prop,current,h0,h1,attack,typ,result,native);assert before==bytes(new.uc.mem_read(np,896));records.append(words(kind,bank,prop,current,h0,h1,attack,typ,result));totals[kind]+=1
  if attack!=sentinel:put(old,bankdata+selected*160+4,banks[selected]);new.uc.mem_write(nb+selected*8,words(banks[selected]))
  if typ!=sentinel and 0<=target<len(types):put(old,seqdata+target*20+16,types[target]);put(new,nseq+target*4,types[target])
 blob=b'CQC1'+words(len(records),len(itemtypes),len(banks),len(types))+words(*itemtypes,*banks,*types)+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(blob)
 source=['port/level-world/character_combat_queries.hpp','port/level-world/character_combat_queries.cpp'];repo=ROOT.parents[1]
 report={'validation':'PASS','original_sha256':m['original_sha256'],'arm64_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'manifest_sha256':hashlib.sha256(a.manifest.read_bytes()).hexdigest(),'corpus_sha256':hashlib.sha256(blob).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'source_sha256':{n:hashlib.sha256((repo/n).read_bytes()).hexdigest() for n in source},'comparisons':len(records),'range_queries':totals[0],'inventory_queries':totals[1],'combo_queries':totals[2],'property32':fields[32],'property2':fields[2],'real_property_rows':struct.unpack_from('<I',props)[0],'real_animation_banks':len(banks),'real_sequences':len(types),'table_inputs_sha256':{str(p.relative_to(a.assets)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (a.assets/'data/animations_pyarray.bin',a.assets/'data/character_properties_pyarray.bin',a.assets/'data/character_properties_pystructnames.bin')},'mismatches':0,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,3)};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
