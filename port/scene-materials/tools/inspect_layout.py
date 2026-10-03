"""Inspect serialized links without relocating or executing the engine."""
import struct
import sys
from pathlib import Path

for path in map(Path, sys.argv[1:]):
    b=path.read_bytes()
    def w(p): return struct.unpack_from('<I',b,p)[0]
    def s(p):
        o=w(p)
        return b[o:b.index(0,o)].decode() if o else None
    r=w(32)
    print(path.name)
    for i in range(w(r+0x4c)):
        p=w(r+0x50)+20*i
        print('image',i,s(p),s(p+8))
    for i in range(w(r+0x68)):
        p=w(r+0x6c)+16*i; mesh=w(p+12)
        print('mesh',i,s(p))
        for j in range(w(mesh+12)):
            q=w(mesh+16)+56*j
            print('primitive',j,'material',s(q+4),'indices',w(q+40))
    def node(p):
        print('node',s(p))
        for i in range(w(p+0x40)):
            q=w(p+0x44)+8*i
            if w(q)!=3: continue
            g=w(q+4)
            print('geometry',s(g+4),w(g+8))
            for j in range(w(g+12)):
                k=w(g+16)+60*j
                print('binding',j,s(k),s(k+4),w(k+8))
        for i in range(w(p+0x38)):node(w(p+0x3c)+80*i)
    for i in range(w(r+0x98)):
        p=w(r+0x9c)+16*i
        for j in range(w(p+8)):node(w(p+12)+80*j)
