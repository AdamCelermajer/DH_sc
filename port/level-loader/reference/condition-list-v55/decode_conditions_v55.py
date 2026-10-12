from pathlib import Path
import struct,json,collections,hashlib
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/condition-list-v55'
class Reader:
 def __init__(self,p):self.b=p.read_bytes();self.at=0
 def word(self):v=struct.unpack_from('<i',self.b,self.at)[0];self.at+=4;return v
 def text(self):n=self.word();v=self.b[self.at:self.at+n].decode();self.at+=n;return v
n=Reader(r/'v2conditions_pyarraynames.bin');names=[n.text() for _ in range(n.word())];assert n.at==len(n.b)
d=Reader(r/'v2conditions_pyarray.bin');count=d.word();assert count==len(names);rows=[]
for name in names:
 c=d.word();assert 0<=c<=1000;stubs=[{'op':d.word(),'argument1':d.word(),'argument2':d.word()} for _ in range(c)];op=d.word();rows.append({'name':name,'count':c,'list_op':op,'stubs':stubs})
assert d.at==len(d.b)
v=json.loads((b/'reports/chapter-coverage-v36.json').read_text());print('inventory keys',v.keys())
levels=v['levels'];swamp=next(x for x in levels if x.get('identity')=='SWAMP' or x.get('name')=='SWAMP' or x.get('design_row',{}).get('name')=='SWAMP');print('swamp keys',swamp.keys());print(json.dumps(swamp.get('objects',[])[:1],indent=2)[:2000])
(r/'decoded-conditions-v55.json').write_text(json.dumps({'rows':rows,'data_consumed':d.at,'names_consumed':n.at,'type_counts':dict(collections.Counter(s['op'] for x in rows for s in x['stubs'])),'source_sha256':{p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in r.glob('*.bin')}},indent=2)+'\n');print('PASS rows',len(rows),'stubs',sum(x['count'] for x in rows),'bytes',d.at)
