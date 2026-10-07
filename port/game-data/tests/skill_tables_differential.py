"""Full source SkillList/Skill readers, tables/names and getter instructions vs O2.

Native bounded decoders preserve every raw scalar, bool byte, vector element and
string byte. Ownership is audited separately using these same production readers.
Original allocation/stream/string compare imports are explicit services.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/skill-tables';SCRATCH=REPO/'.local-inputs/skill-tables'
sys.path.insert(0,str(ROOT/'tests'));from items_differential import Original,strings,words
from navigation_differential import Cpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def span(b):return words(len(b))+b
def names(block):return words(len(block))+b''.join(span(b) for b in block)
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,default=SCRATCH/'libskill_tables.so');a=ap.parse_args();manifest=json.loads((REF/'original-functions.json').read_text());old=Original(REPO/'.local-inputs/libDungeonHunter2.so',manifest);new=Cpu(a.library,True,{'functions':[]})
 data=(SCRATCH/'skills_pyarray.bin').read_bytes();identifiers=(SCRATCH/'skills_pyarraynames.bin').read_bytes();schema=(SCRATCH/'skills_pystructnames.bin').read_bytes();name_blocks=strings(identifiers);fields=strings(schema);assert list(map(len,name_blocks))==[36,127] and list(map(len,fields))==[1,15]
 got=0x4b9990+old.word(0x4b9aa0);lc=old.word(got+old.word(0x4b9aa4));lp=old.word(got+old.word(0x4b9aac));got=0x4b983c+old.word(0x4b9954);sc=old.word(got+old.word(0x4b9958));sp=old.word(got+old.word(0x4b9960))
 def read_tables(blob):
  old.blob=blob;old.cursor=0;old.invoke(0x4b9964,[old.stream]);end=old.cursor;old.invoke(0x4b9810,[old.stream]);return [old.word(lc),old.word(sc),end,old.cursor]
 def projection(p):
  w=list(struct.unpack('<19I',old.uc.mem_read(p,76)));payload=[bytes(old.uc.mem_read(w[4],w[3]*4)) if w[3] else b'',bytes(old.uc.mem_read(w[10],w[9])) if w[9] else b'',bytes(old.uc.mem_read(w[15],w[14])) if w[14] else b''];w[0]=w[4]=w[10]=w[15]=0
  for i in [2,6,11]:w[i]&=255
  return words(*w),payload
 summary=read_tables(data);assert summary==[36,127,1000,11862];cache_rows=[projection(old.word(sp)+76*i) for i in range(127)];cache_lists=[]
 for i in range(36):p=old.word(lp)+12*i;cache_lists.append(bytes(old.uc.mem_read(old.word(p+8),old.word(p+4)*4)) if old.word(p+4) else b'')
 inp=new.data+0x1000;out=new.data+0x50000;views=out+0x1000;used=out+0x2000;measured=out+0x3000;row_records=[];list_records=[];table_records=[];payload_bytes=0
 def compare_row(blob,want):
  nonlocal payload_bytes
  new.uc.mem_write(inp,blob);assert new.invoke('dh2_skill_decode_record',[out,views,used,inp,len(blob)])==0;actual=bytes(new.uc.mem_read(out,76));v=struct.unpack('<QIIQIIQII',new.uc.mem_read(views,48));payload=[]
  for i in [0,3,6]:p,n,res=v[i:i+3];assert not res and inp<=p<=inp+len(blob) and p+n<=inp+len(blob);payload.append(bytes(new.uc.mem_read(p,n)) if n else b'')
  assert (actual,payload)==want;consumed=struct.unpack('<I',new.uc.mem_read(used,4))[0];payload_bytes+=sum(map(len,payload));row_records.append(span(blob[:consumed])+want[0]+b''.join(span(b) for b in want[1]));return consumed
 def compare_list(blob,want):
  new.uc.mem_write(inp,blob);assert not new.invoke('dh2_skill_decode_list',[views,used,inp,len(blob)]);p,n,res=struct.unpack('<QII',new.uc.mem_read(views,16));assert not res and bytes(new.uc.mem_read(p,n))==want;consumed=struct.unpack('<I',new.uc.mem_read(used,4))[0];list_records.append(span(blob[:consumed])+span(want));return consumed
 at=4
 for want in cache_lists:at+=compare_list(data[at:1000],want)
 assert at==1000;at+=4
 for want in cache_rows:at+=compare_row(data[at:],want)
 assert at==len(data)
 rng=random.Random(0x4ebeb0);edge=[0,1,0xffffffff,0x80000000,0x7fffffff,0x7f801234]
 def make_row(i):
  ints=[rng.choice(edge) if i<128 else rng.getrandbits(32) for _ in range(9)];v=words(*[rng.getrandbits(32) for _ in range(i%7)]);texts=[rng.choice([b'',b'embedded\0tail',bytes(range(256)),b'skill.lua',b'x'*1024]) for _ in range(2)];bo=[rng.choice([0,1,2,127,255]) for _ in range(3)];return words(ints[0])+bytes([bo[0]])+span(v)[0:4]+v+words(ints[1])+bytes([bo[1]])+words(*ints[2:4])+span(texts[0])+bytes([bo[2]])+words(*ints[4:6])+span(texts[1])+words(*ints[6:])
 # Vector serialization count is element count, not byte count.
 def row(i):
  b=make_row(i);return b[:5]+words(i%7)+b[9:]
 for i in range(256):
  b=row(i);old.blob=b;old.cursor=0;p=old.data+0x60000;old.uc.mem_write(p,bytes(76));old.invoke(0x4ebeb0,[p,old.stream]);assert old.cursor==len(b);assert compare_row(b,projection(p))==len(b)
 for i in range(128):
  b=words(i%13)+words(*[rng.getrandbits(32) for _ in range(i%13)]);old.blob=b;old.cursor=0;p=old.data+0x61000;old.uc.mem_write(p,bytes(12));old.invoke(0x4ea830,[p,old.stream]);want=bytes(old.uc.mem_read(old.word(p+8),old.word(p+4)*4)) if old.word(p+4) else b'';assert compare_list(b,want)==len(b)
 for i in range(64):
  nl=i%7;ns=i%9;b=words(nl)+b''.join(words(j%5)+words(*[rng.getrandbits(32) for _ in range(j%5)]) for j in range(nl))+words(ns)+b''.join(row(256+i*9+j) for j in range(ns));want=read_tables(b);new.uc.mem_write(inp,b);assert not new.invoke('dh2_skill_tables_measure',[measured,inp,len(b)]) and bytes(new.uc.mem_read(measured,16))==words(*want);table_records.append(span(b)+words(*want))
 want=read_tables(data);new.uc.mem_write(inp,data);assert not new.invoke('dh2_skill_tables_measure',[measured,inp,len(data)]) and bytes(new.uc.mem_read(measured,16))==words(*want);table_records.insert(0,span(data)+words(*want))
 old.blob=identifiers;old.cursor=0;old.invoke(0x4b0464,[old.stream]);ne=old.cursor;old.invoke(0x4b0938,[old.stream]);assert ne==559 and old.cursor==2757
 for base,block in zip([0x9f3ce4,0x9f3cfc],fields):
  for i,b in enumerate(block):p=old.data+0x70000+(base==0x9f3cfc)*0x10000+i*128;old.uc.mem_write(p,b+b'\0');old.pointer(base+i*24+20,p)
 query=old.data+0x90000;queries=[]
 for kind,(block,address) in enumerate(zip(name_blocks+fields,[0x4ad1a8,0x4ad0dc,0x4ad21c,0x4ad150])):
  for key in block+[b'',b'unknown',block[0].swapcase(),block[0]+b'\0tail']:
   old.uc.mem_write(query,key+b'\0');queries.append(words(kind)+span(key)+words(old.invoke(address,[query])))
 # Actual source duplicate/firstNUL row-name loading and first-match lookup.
 duplicate=list(name_blocks[1]);duplicate[:2]=[b'dup\0one',b'dup\0two'];old.blob=names(duplicate);old.cursor=0;old.invoke(0x4b0938,[old.stream]);old.uc.mem_write(query,b'dup\0');assert old.invoke(0x4ad0dc,[query])==0
 gold=REF/'skill-tables-fixtures.bin';gold.write_bytes(words(0x31544b53,len(row_records),len(list_records),len(table_records),len(queries))+b''.join(row_records+list_records+table_records+queries));source=[ROOT/'skill_tables.cpp',ROOT/'skill_tables.hpp',Path(__file__)]
 report=dict(validation='PASS',record_comparisons=len(row_records),list_comparisons=len(list_records),table_comparisons=len(table_records),original_name_queries=len(queries),actual_skills=127,actual_lists=36,actual_source_boundaries=summary,actual_names_boundaries=[559,2757],actual_schema_boundaries=[12,233],dynamic_payload_bytes=payload_bytes,duplicate_firstNUL_original=True,mismatches=0,stream_read_calls=old.reads,allocations=old.allocations,original_sha256=manifest['original_sha256'],manifest_sha256=sha(REF/'original-functions.json'),arm64_sha256=sha(a.library),corpus_sha256=sha(gold),source_sha256={p.relative_to(REPO).as_posix():sha(p) for p in source},input_sha256={p.name:sha(p) for p in [SCRATCH/'skills_pyarray.bin',SCRATCH/'skills_pyarraynames.bin',SCRATCH/'skills_pystructnames.bin']},scope=__doc__,whole_Application_registration=False,owned_Cpp_snapshot_ARM64=False)
 (ROOT/'reports/skill-tables-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
