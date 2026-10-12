"""Inspect actual constructed rule virtual dispatch in the supplied ARM ELF."""
import argparse,json,pathlib,sys
ap=argparse.ArgumentParser()
for n in ('engine','dependency-root'):ap.add_argument('--'+n,type=pathlib.Path,required=True)
a=ap.parse_args()
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]/'tests'))
from procedural_blocks_original import create_original_blocks_cpu
p=create_original_blocks_cpu(a.engine,a.dependency_root)
root=p.data+0x1a00000;app=root+0x1000
p.invoke(0x48dff8,[root,app])
rows=[]
for name,ctor in [('RootRule',None),('EndPath',0x48dd18),('ForceBlock',0x48dd80),('Path',0x48ddf8)]:
    at=root if ctor is None else root+0x2000
    if ctor:p.invoke(ctor,[at,root,root])
    vt=p.word(at)
    rows.append({'type':name,'vtable':hex(vt),'entries':[hex(p.word(vt+4*i)) for i in range(8)]})
print(json.dumps({'vtables':rows},indent=2))
