"""Inspect serialized skin fields without executing original engine code."""
import argparse,struct,json
from pathlib import Path
def inspect(path):
 b=path.read_bytes();w=lambda p:struct.unpack_from('<I',b,p)[0]
 text=lambda p:b[p:b.index(0,p)].decode('utf-8') if p and p<len(b) else None
 root=w(32);n,base=w(root+112),w(root+116);rows=[]
 for i in range(n):
  c=base+i*12;s=w(c+8)
  if i==18:print('bind shape 18',text(w(c+4)),struct.unpack_from('<16f',b,s+16))
  if i<3:
   for k in range(w(root+104)):
    g=w(root+108)+16*k
    if '#'+text(w(g))==text(w(s+0x70)):
     m=w(g+12);print('mesh',hex(m),[w(m+j*4) for j in range(5)])
   print(text(w(c+4)),hex(s))
   for o in range(0,160,4):print(hex(o),hex(w(s+o)),text(w(s+o)) if o in (0x70,) else '')
   print('joints',[(hex(w(w(s+0x78)+j*4)),text(w(w(s+0x78)+j*4))) for j in range(w(s+0x74))])
   for o in (4,12,0x80,0x88,0x90):
    ptr=w(s+o);print('array',hex(o),hex(ptr),[hex(w(ptr+j*4)) for j in range(32)])
   d=w(s+0x80);off=w(d+4);print('weight bytes',hex(off),b[off:off+128].hex(),struct.unpack_from('<32f',b,off))
   def nodes(p):
    if i==0:print('node',text(w(p)),text(w(p+8)))
    for j in range(w(p+56)):nodes(w(p+60)+j*80)
   for j in range(w(root+152)):
    v=w(root+156)+j*16
    for k in range(w(v+8)):nodes(w(v+12)+k*80)
  rows.append({'index':i,'id':text(w(c+4)),'type':w(c),'source':text(w(s+0x70)),'joints':w(s+0x74),'buffers':w(s+0x84),'influences':b[s+0x98]})
 print(json.dumps(rows,indent=2))
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('model',type=Path);inspect(p.parse_args().model)
