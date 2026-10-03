"""Independently decode the evidenced ClassTable list of five-word formulas."""
import argparse,collections,json,struct
from pathlib import Path
import sys
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'level-world/tools'))
from inspect_animation_tables import sections

def parse(folder):
 names=sections((folder/'character_classes_pyarraynames.bin').read_bytes());schema=sections((folder/'character_classes_pystructnames.bin').read_bytes());raw=(folder/'character_classes_pyarray.bin').read_bytes();offset=0
 def word():
  nonlocal offset
  value=struct.unpack_from('<I',raw,offset)[0];offset+=4;return value
 count=word();assert len(names)==1 and count==len(names[0]['names'])==260;rows=[]
 for name in names[0]['names']:
  size=word();assert size<10000;entries=[]
  for _ in range(size):entries.append(list(struct.unpack_from('<5i',raw,offset)));offset+=20
  rows.append({'name':name,'entries':entries})
 assert offset==len(raw)
 return {'schema':schema,'rows':rows,'bytes':offset,'formula_counts':dict(collections.Counter(e[1] for r in rows for e in r['entries']))}

def main():
 p=argparse.ArgumentParser();p.add_argument('folder',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args();result=parse(a.folder);a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k!='rows'}));print(json.dumps([{'id':i,**r} for i,r in enumerate(result['rows']) if i in (16,17,18,19,20,21,225)],indent=2))
if __name__=='__main__':main()
