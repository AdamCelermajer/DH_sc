"""Actual Faery/FaeryList ARM32 readers against optimized native ARM64 decoders.
Stream and allocator are explicit services. Payload bytes and all scalar words
are exact; original vtable/script pointers normalize to native owned backing.
"""
import argparse,hashlib,json,struct,sys,random
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];REF=ROOT/'port/level-world/reference/character-skills';SCRATCH=ROOT/'.local-inputs/character-skills'
sys.path.insert(0,str(ROOT/'port/game-data/tests'));from items_differential import Original,words,strings
sys.path.insert(0,str(ROOT/'port/level-world/tests'));from navigation_differential import Cpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def span(b):return words(len(b))+b
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();manifest=json.loads((REF/'faery-readers/original-functions.json').read_text());old=Original(ROOT/'.local-inputs/libDungeonHunter2.so',manifest);new=Cpu(a.library,True,{'functions':[]});raw=(SCRATCH/'faeries_pyarray.bin').read_bytes();rows=[];lists=[];tables=[];inp=new.data+0x1000;out=new.data+0x10000;view=out+0x100;used=out+0x200
 def original_row(b):
  old.blob=b;old.cursor=0;p=old.data+0x3000;old.uc.mem_write(p,bytes(36));old.invoke(0x50637c,[p,old.stream]);w=list(struct.unpack('<9I',old.uc.mem_read(p,36)));script=bytes(old.uc.mem_read(w[6],w[5]));w[0]=w[6]=0;return old.cursor,words(*w),script
 def row(b):
  n,w,s=original_row(b);new.uc.mem_write(inp,b);assert new.invoke('dh2_faery_decode_record',[out,view,used,inp,len(b)])==0;v,nb,res=struct.unpack('<QII',new.uc.mem_read(view,16));assert not res and bytes(new.uc.mem_read(out,36))==w and struct.unpack('<I',new.uc.mem_read(used,4))[0]==n and bytes(new.uc.mem_read(v,nb))==s;rows.append(span(b[:n])+w+span(s));return n
 def list_(b):
  old.blob=b;old.cursor=0;p=old.data+0x3000;old.uc.mem_write(p,bytes(12));old.invoke(0x4eab9c,[p,old.stream]);want=bytes(old.uc.mem_read(old.word(p+8),old.word(p+4)*4));n=old.cursor;new.uc.mem_write(inp,b);assert not new.invoke('dh2_faery_decode_list',[view,used,inp,len(b)]);v,nb,res=struct.unpack('<QII',new.uc.mem_read(view,16));assert not res and bytes(new.uc.mem_read(v,nb))==want and struct.unpack('<I',new.uc.mem_read(used,4))[0]==n;lists.append(span(b[:n])+span(want));return n
 at=4
 for _ in range(struct.unpack_from('<I',raw)[0]):at+=list_(raw[at:])
 assert at==100;at+=4
 for _ in range(struct.unpack_from('<I',raw,100)[0]):at+=row(raw[at:])
 assert at==699
 rng=random.Random(0x50637c)
 for i in range(192):
  s=rng.choice([b'',b'skill',b'embedded\0suffix',bytes(range(256)),b'x'*1024]);b=words(*[rng.getrandbits(32) for _ in range(4)])+span(s)+words(*[rng.getrandbits(32) for _ in range(2)]);row(b)
 for i in range(96):b=words(i%11)+words(*[rng.getrandbits(32) for _ in range(i%11)]);list_(b)
 # Table dimension/boundary corpus derived from actual reader outputs above.
 for i in range(32):
  nl=i%5;nr=i%8;b=words(nl)+b''.join(words(j%3)+words(*range(j%3)) for j in range(nl));listend=len(b);b+=words(nr)
  for j in range(nr):s=bytes([j])*j;b+=words(j,j+1,j+2,j+3)+span(s)+words(j+4,j+5)
  want=words(nl,nr,listend,len(b));new.uc.mem_write(inp,b);assert not new.invoke('dh2_faery_tables_measure',[out,inp,len(b)]) and bytes(new.uc.mem_read(out,16))==want;tables.append(span(b)+want)
 new.uc.mem_write(inp,raw);assert not new.invoke('dh2_faery_tables_measure',[out,inp,len(raw)]) and bytes(new.uc.mem_read(out,16))==words(4,16,100,699);tables.insert(0,span(raw)+words(4,16,100,699))
 gold=REF/'faery-fixtures.bin';gold.write_bytes(words(0x31594146,len(rows),len(lists),len(tables))+b''.join(rows+lists+tables));sources=['port/game-data/faery_tables.hpp','port/game-data/faery_tables.cpp','port/game-data/tests/faery_tables_differential.py']
 report={'validation':'PASS','original_sha256':manifest['original_sha256'],'library_sha256':sha(a.library),'original_reader_record_comparisons':len(rows),'original_reader_list_comparisons':len(lists),'native_table_boundary_checks':len(tables),'actual_faeries':16,'actual_lists':4,'mismatches':0,'gold_sha256':sha(gold),'source_sha256':{s:sha(ROOT/s) for s in sources},'capture_sha256':sha(REF/'faery-readers/original-functions.json'),'actual_original_table_report_sha256':sha(REF/'faery-original-probe.json'),'input_sha256':{p.name:sha(p) for p in SCRATCH.glob('faeries_*.bin')},'scope':__doc__+' Table boundary cases use individually proved serialized record/list widths; full actual source tables executed in separately bound faery-original-probe.json.'}
 (ROOT/'port/level-world/reports/faery-tables-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
