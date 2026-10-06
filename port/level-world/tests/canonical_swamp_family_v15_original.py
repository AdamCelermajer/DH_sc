from pathlib import Path
import json,struct
r=Path(__file__).resolve().parents[3]
s=(r/'port/level-world/tests/canonical_dummy_v14_original.py').read_text()
s=s[:s.index('snapshots=[]')].replace('n<=0x374','n<=0x394').replace('if n==0x374:','if n in (0x374,0x378,0x394):')
exec(compile(s,'dummy-constructor-primitives','exec'))
rows=[]
for factory,kind,expected,size in [(0x3410fc,'Decor',20,0x378),(0x340e10,'SpawnPoint',13,0x394)]:
 for _ in range(3):
  p=c.invoke(factory,[]);assert p==allocations[-1] and word(p+0xf4)==expected
  v=word(p);slots={hex(o):hex(word(v+o)) for o in (0x18,0x1c,0x58)}
  if kind=='Decor':
   assert bytes(c.uc.mem_read(p+0x84,1))==b'\1' and bytes(c.uc.mem_read(p+0x375,2))==b'\1\1'
   assert slots=={'0x18':'0x3899d0','0x1c':'0x388a98','0x58':'0x38cd48'}
  else:
   assert word(p+0x374)==word(p+0x390)==0xffffffff and c.uc.mem_read(word(p+0x38c),1)==b'\0'
   assert slots=={'0x18':'0x3ea554','0x1c':'0x3ea28c','0x58':'0x38cd48'}
  rows.append({'kind':kind,'go_id':expected,'slots':slots,'allocation':size,'static84':c.uc.mem_read(p+0x84,1)[0]})
report={'status':'PASS','cases':rows,'scope':'Whole original class factory/base constructor graph; poisoned allocation. Imported allocation/free, single-thread mutex, unsigned division and inline CString reserve are explicit primitive fixtures.'}
(r/'port/level-world/reference/swamp-families-v14/family-constructors-original-v15.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
