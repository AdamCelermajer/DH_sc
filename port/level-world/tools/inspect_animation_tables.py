"""Inspect all bounded length-prefixed animation name/schema sections."""
import argparse,json,struct
from pathlib import Path
def sections(raw):
 offset=0;result=[]
 while offset<len(raw):
  assert len(raw)-offset>=4
  start=offset;count=struct.unpack_from('<I',raw,offset)[0];offset+=4;assert count<=10000;values=[]
  for _ in range(count):
   assert len(raw)-offset>=4
   n=struct.unpack_from('<I',raw,offset)[0];offset+=4;assert 0<n<=4096 and offset+n<=len(raw);values.append(raw[offset:offset+n].decode('ascii'));offset+=n
  result.append({'offset':start,'end':offset,'count':count,'names':values})
 return result
def main():
 p=argparse.ArgumentParser();p.add_argument('input',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 report={name:sections((a.input/name).read_bytes()) for name in ('animations_pyarraynames.bin','animations_pystructnames.bin')}
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({name:[{'offset':r['offset'],'count':r['count'],'first':r['names'][:6]} for r in rows] for name,rows in report.items()},indent=2))
if __name__=='__main__':main()
