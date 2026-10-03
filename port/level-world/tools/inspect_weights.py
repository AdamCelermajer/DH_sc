"""Inspect original influence totals without modifying/normalizing weights."""
import json,struct,sys
from pathlib import Path
raw=Path(sys.argv[1]).read_bytes()
def u(p):return struct.unpack_from('<I',raw,p)[0]
def text(p):return raw[p:raw.index(b'\0',p)].decode()
root=u(32);geometries={text(u(u(root+0x6c)+i*16)):u(u(root+0x6c)+i*16+12) for i in range(u(root+0x68))}
out=[]
for i in range(u(root+0x70)):
 c=u(root+0x74)+i*12;s=u(c+8);joints=u(s+116);count=raw[s+152];n=u(geometries[text(u(s+112))[1:]]+4);data=u(s+128)
 if struct.unpack_from('<i',raw,root+100)[0]>0:data=u(data+4)
 stride=4*(count+1);totals=[];bad=[]
 for v in range(n):
  p=data+v*stride;ids=list(raw[p:p+4]);weights=struct.unpack_from('<'+'f'*count,raw,p+4);total=sum(weights);totals.append(total)
  if abs(total-1)>.002:bad.append({'vertex':v,'joints':ids,'weights':weights,'total':total})
 out.append({'id':text(u(c+4)),'vertices':n,'joints':joints,'influence_count':count,'min_sum':min(totals),'max_sum':max(totals),'nonunit':bad[:20],'nonunit_count':len(bad)})
print(json.dumps(out,indent=2))
