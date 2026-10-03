"""Inspect original serialization recovered from Structs::*::read, not C++ layout."""
import argparse,json,struct
from pathlib import Path
class Reader:
 def __init__(self,b):self.b=b;self.p=0
 def read(self,fmt):
  n=struct.calcsize('<'+fmt)
  if self.p+n>len(self.b):raise ValueError('truncated input')
  v=struct.unpack_from('<'+fmt,self.b,self.p)[0];self.p+=n;return v
 def count(self):
  n=self.read('I')
  if n>10000:raise ValueError('count limit')
  return n
 def ints(self):return [self.read('i') for _ in range(self.count())]
def parse(raw,sections):
 names=[s['names'] for s in sections['animations_pyarraynames.bin']]
 fields=[s['names'] for s in sections['animations_pystructnames.bin']]
 r=Reader(raw);out={};anims=[]
 for i in range(r.count()):
  loop=r.read('i');steps=[]
  for _ in range(r.count()):
   formats=['B','i','i','i','B','i','B',None,'i','i','f','B']
   steps.append({k:(r.ints() if f is None else r.read(f)) for k,f in zip(fields[0],formats)})
  anims.append({'name':names[0][i],'Loop':loop,'Steps':steps,'Type':r.read('i')})
 out['animations']=anims;out['animation_end']=r.p;cams=[]
 for i in range(r.count()):cams.append({'name':names[1][i],'CamAnims':r.ints(),**{k:r.read('i') for k in fields[2][1:]}})
 out['cameras']=cams;out['camera_end']=r.p;chars=[]
 for i in range(r.count()):chars.append({'name':names[2][i],**{k:(r.ints() if k in ('Interact','Spells') else r.read('i')) for k in fields[3]}})
 out['characters']=chars;out['consumed']=r.p;out['size']=len(r.b)
 if r.p!=len(r.b):raise ValueError(f'unparsed suffix {len(r.b)-r.p}')
 return out
def main():
 p=argparse.ArgumentParser();p.add_argument('inputs',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 sections=json.loads((a.inputs.parent/'animation-table-sections.json').read_text());out=parse((a.inputs/'animations_pyarray.bin').read_bytes(),sections)
 a.output.write_text(json.dumps(out,indent=2)+'\n');print(json.dumps({k:v for k,v in out.items() if k.endswith('end') or k in ('consumed','size')}));print(json.dumps([c for c in out['characters'] if c['name'] in ('Skeleton','Skeleton2','Slime','Ghost')],indent=2))
if __name__=='__main__':main()
