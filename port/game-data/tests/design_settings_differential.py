"""Complete original DesignSettings row/table/name readers vs O2 native decoding.

Stream/allocation/string imports are explicit byte/storage services. All43 raw
words execute actual float/integer reader instructions. Source table names are
loaded by actual readNames; field lookup uses borrowed genuine schema CString
metadata, not an inferred whole Application static initialization.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/design-settings';SCRATCH=REPO/'.local-inputs/design-settings'
sys.path.insert(0,str(ROOT/'tests'))
from items_differential import Original,strings,words
from navigation_differential import Cpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def span(raw):return words(len(raw))+raw
def names(block):return words(len(block))+b''.join(span(x) for x in block)
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,default=SCRATCH/'libdesign_settings.so');a=ap.parse_args();manifest=json.loads((REF/'original-functions.json').read_text());old=Original(REPO/'.local-inputs/libDungeonHunter2.so',manifest);new=Cpu(a.library,True,{'functions':[]})
 data=(SCRATCH/'design_pyarray.bin').read_bytes();name_data=(SCRATCH/'design_pyarraynames.bin').read_bytes();schema_data=(SCRATCH/'design_pystructnames.bin').read_bytes();schema=strings(schema_data)[0];assert len(schema)==43 and schema[11]==b'EnemySpottedAggro'
 old.blob=data;old.cursor=0;old.invoke(0x4b3cd0,[old.stream]);used=old.cursor;count=old.word(0x9a6494);table=old.word(0x9a6498);assert used==176 and count==1;rawrow=bytes(old.uc.mem_read(table,176));actual=words(0)+rawrow[4:];assert struct.unpack_from('<I',actual,48)[0]==0x41200000
 old.blob=name_data;old.cursor=0;old.invoke(0x4b7638,[old.stream]);name_used=old.cursor;assert name_used==15 and old.word(0x9a649c)
 # Explicit borrowed CString metadata for source fixed43-field strcmp loop.
 for i,name in enumerate(schema):p=old.data+0x50000+i*128;old.uc.mem_write(p,name+b'\0');old.pointer(0x9eae14+i*24+20,p)
 # Derive offsets from the executed row reader; m_dataOffsets requires a
 # separate static initializer and is not claimed populated by this loader.
 import re
 asm=(REF/'reference/original-functions.asm').read_text();section=asm.split('# _ZN7Structs14DesignSettings4readEP11IStreamBase')[1].split('\n# ')[0];lines=section.splitlines();offsets=[];integer=[]
 for i,line in enumerate(lines):
  if 'bl       #0x459090' in line or 'bl       #0x4db94c' in line:
   offset=int(re.search(r'#(0x[0-9a-f]+|[0-9]+)',lines[i-1]).group(1),0);offsets.append(offset)
   if '0x459090' in line:integer.append(offset//4-1)
 assert offsets==list(range(4,176,4)) and integer==[4,8,9,*range(24,34)]
 row_queries=[];field_queries=[];key=old.data+0x60000
 for target,queries,addr in [([b'Default',b'default',b'',b'unknown',b'Default\0tail'],row_queries,0x4aed88),(schema+[b'enemySpottedAggro',b'',b'unknown',b'EnemySpottedAggro\0tail'],field_queries,0x4aedfc)]:
  for value in target:old.uc.mem_write(key,value+b'\0');queries.append((value,old.invoke(addr,[key])))
 output=new.data+0x1000;consumed=new.data+0x2000;nc=new.data+0x2100;inputp=new.data+0x3000;records=[];tables=[];rng=random.Random(20261004)
 def record(raw,expected):
  new.uc.mem_write(inputp,raw);new.uc.mem_write(output,bytes([0xa5])*176);new.uc.mem_write(consumed,words(0xa5a5a5a5));assert new.invoke('dh2_design_settings_decode_record',[output,consumed,inputp,len(raw)])==0;assert bytes(new.uc.mem_read(output,176))==expected and struct.unpack('<I',new.uc.mem_read(consumed,4))[0]==172;records.append(span(raw)+expected)
 record(data[4:176],actual)
 boundary=[0,0x80000000,1,0x7fffffff,0xffffffff,0x7f800000,0xff800000,0x7fc01234,0x7f801234,0x007fffff,0x00800000]
 for n in range(384):
  raw=words(*[rng.choice(boundary) if n<192 else rng.getrandbits(32) for _ in range(43)]);old.blob=raw;old.cursor=0;p=old.data+0x70000;old.uc.mem_write(p,bytes(176));old.invoke(0x4ee0d0,[p,old.stream]);assert old.cursor==172;expected=words(0)+bytes(old.uc.mem_read(p+4,172));record(raw,expected)
 for n in range(96):
  rows=n%9;payload=words(rows)+b''.join(words(*[rng.choice(boundary) if n<48 else rng.getrandbits(32) for _ in range(43)]) for _ in range(rows));old.blob=payload;old.cursor=0;old.invoke(0x4b3cd0,[old.stream]);count=old.word(0x9a6494);p=old.word(0x9a6498);assert count==rows and old.cursor==len(payload);expected=b''.join(words(0)+bytes(old.uc.mem_read(p+i*176+4,172)) for i in range(rows));new.uc.mem_write(inputp,payload);assert new.invoke('dh2_design_settings_decode_table',[output,8,nc,consumed,inputp,len(payload)])==0;assert struct.unpack('<I',new.uc.mem_read(nc,4))[0]==rows and struct.unpack('<I',new.uc.mem_read(consumed,4))[0]==len(payload) and bytes(new.uc.mem_read(output,rows*176))==expected;tables.append(span(payload)+words(rows)+expected)
 # Preserve actual full cache input/table comparison, including suffix consumed.
 old.blob=data;old.cursor=0;old.invoke(0x4b3cd0,[old.stream]);new.uc.mem_write(inputp,data);assert new.invoke('dh2_design_settings_decode_table',[output,8,nc,consumed,inputp,len(data)])==0;assert struct.unpack('<I',new.uc.mem_read(consumed,4))[0]==176 and bytes(new.uc.mem_read(output,176))==actual;tables.insert(0,span(data)+words(1)+actual)
 # Actual ordered duplicate-name lookup, not invented map insertion.
 old.blob=words(3)+data[4:176]*3;old.cursor=0;old.invoke(0x4b3cd0,[old.stream]);duplicate_names=[b'Default',b'Default',b'Other'];old.blob=names(duplicate_names);old.cursor=0;old.invoke(0x4b7638,[old.stream]);old.uc.mem_write(key,b'Default\0');assert old.invoke(0x4aed88,[key])==0
 # Raw source instruction targets determine field kind; no name-based inference.
 corpus=words(0x31535344,len(records),len(tables),len(row_queries),len(field_queries))+b''.join(records)+b''.join(tables)+b''.join(span(k)+words(v) for k,v in row_queries+field_queries)+words(*[int(i in integer) for i in range(43)]);gold=REF/'design-settings-fixtures.bin';gold.write_bytes(corpus)
 report=dict(validation='PASS',record_comparisons=len(records),table_comparisons=len(tables),word_comparisons=44*(len(records)+sum(n%9 for n in range(96))+1),original_name_queries=len(row_queries)+len(field_queries),original_duplicate_firstmatch=True,integer_fields=integer,actual_cache_rows=1,actual_records_consumed=used,actual_names_consumed=name_used,authored_threat_bits=0x41200000,mismatches=0,original_sha256=manifest['original_sha256'],manifest_sha256=sha(REF/'original-functions.json'),arm64_sha256=sha(a.library),corpus_sha256=sha(gold),source_sha256={p.relative_to(REPO).as_posix():sha(p) for p in [ROOT/'design_settings.cpp',ROOT/'design_settings.hpp',Path(__file__)]},input_sha256={p.name:sha(p) for p in [SCRATCH/'design_pyarray.bin',SCRATCH/'design_pyarraynames.bin',SCRATCH/'design_pystructnames.bin']},source_offsets=list(offsets),stream_read_calls=old.reads,allocation_calls=old.allocations,scope=__doc__)
 (ROOT/'reports/design-settings-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
