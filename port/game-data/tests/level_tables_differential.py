"""Replay original Level/FastTravel record gold against actual O2 ARM64 code."""
import argparse,hashlib,json,struct,sys
from pathlib import Path
DATA=Path(__file__).resolve().parents[1];ROOT=DATA.parents[1]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from navigation_differential import Cpu

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--library',type=Path,required=True);parser.add_argument('--build',type=Path,required=True);parser.add_argument('--report',type=Path,required=True);args=parser.parse_args()
 if args.report.exists():raise RuntimeError('Preserve previous level-table proof')
 build=json.loads(args.build.read_text(encoding='utf-8-sig'));assert sha(args.library)==build['library_sha256']
 assert all(sha(ROOT/name)==digest for name,digest in build['source_sha256'].items())
 reference=DATA/'reference/level-tables';captured=json.loads((reference/'original-loader-capture.json').read_text());gold=reference/'level-table-fixtures.bin';assert captured['validation']=='SOURCE_CAPTURED' and sha(gold)==captured['gold_sha256']
 raw=gold.read_bytes();cursor=0
 def word():
  nonlocal cursor
  value=struct.unpack_from('<I',raw,cursor)[0];cursor+=4;return value
 def span():
  nonlocal cursor
  size=word();value=raw[cursor:cursor+size];cursor+=size;assert len(value)==size;return value
 assert word()==0x3144544c;count=word();cpu=Cpu(args.library,True,{'functions':[]});out=cpu.data+0x1000;text=cpu.data+0x2000;used=cpu.data+0x2100;input_at=cpu.data+0x4000;words_count=text_count=0
 for i in range(count):
  kind=word();blob=span();n=18 if kind else 7;expected=raw[cursor:cursor+n*4];cursor+=n*4;strings=[span() for _ in range(2 if kind else 1)]
  cpu.uc.mem_write(input_at,blob);cpu.uc.mem_write(out,bytes([0xa5])*72);cpu.uc.mem_write(text,bytes([0xb6])*32);cpu.uc.mem_write(used,struct.pack('<I',0xcccccccc))
  result=cpu.invoke('dh2_level_decode_record' if kind else 'dh2_fast_travel_decode_record',[out,text,used,input_at,len(blob)])
  assert result==0 and bytes(cpu.uc.mem_read(out,n*4))==expected,(i,kind,result)
  assert struct.unpack('<I',cpu.uc.mem_read(used,4))[0]==len(blob)
  for j,expected_string in enumerate(strings):
   pointer,size,reserved=struct.unpack('<QII',cpu.uc.mem_read(text+j*16,16));assert reserved==0 and input_at<=pointer<=pointer+size<=input_at+len(blob)
   assert bytes(cpu.uc.mem_read(pointer,size))==expected_string if size else expected_string==b''
   text_count+=1
  words_count+=n
 assert cursor==len(raw) and count==212
 report=dict(validation='PASS',scope=__doc__,record_cases=count,compared_words=words_count,compared_strings=text_count,gold_sha256=sha(gold),original_capture_sha256=sha(reference/'original-loader-capture.json'),original_sha256=captured['original_sha256'],build_sha256=sha(args.build),library_sha256=sha(args.library),source_sha256=build['source_sha256'],source_Application_level_selection_verified=False)
 args.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))

if __name__=='__main__':main()
