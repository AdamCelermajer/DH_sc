from pathlib import Path
import json,struct
r=Path(__file__).resolve().parents[3];s=(r/'port/level-world/tests/canonical_dummy_v14_original.py').read_text();s=s[:s.index('snapshots=[]')].replace('n<=0x374','n<=0x6f8').replace('if n==0x374:','if n==0x6f8:');exec(compile(s,'original-destructible-primitives','exec'))
rows=[]
for _ in range(3):
 p=c.invoke(0x340d5c,[]);assert p==allocations[-1] and word(p+0xf4)==1 and word(p+0x394)==2 and word(p+0x398)==0 and word(p+0x6f0)==word(p+0x6f4)==0
 assert c.uc.mem_read(p+0x28,1)==b'\1' and c.uc.mem_read(p+0x84,1)==b'\0' and c.uc.mem_read(p+0xf8,1)==b'\2'
 assert word(p+0x100)==p+0x3a0 and word(p+0x104)==p+0x548 and word(p+0x374)==0xa5a5a5a5
 rows.append({'go_id':1,'state394':2,'opener398':0,'progress6f0':0,'progress6f4':0,'data374_unproduced_poison':hex(word(p+0x374)),'network_offsets':['0x3a0','0x548']})
out=r/'port/level-world/reference/destructible-container-v16';(out/'constructor-original.json').write_text(json.dumps({'status':'PASS','cases':rows,'scope':'whole source factory/Destructible/Container/GameObject constructor graph, including both real NetStructContainer constructors; imported allocation/mutex/unsigned divide and inline CString reserve fixtures'},indent=2));print('Destructible whole original constructor PASS',len(rows))
