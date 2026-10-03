import struct
from pathlib import Path
b=Path('.local-inputs/world/crypt.bdae').read_bytes();w=lambda p:struct.unpack_from('<I',b,p)[0]
def text(p):
 if not p or p>=len(b):return ''
 t=b[p:p+256].split(b'\0',1)[0]
 return t.decode('ascii') if t and all(32<=c<127 for c in t) else ''
r=w(32);vs=w(r+156);node=w(vs+12);instances=w(node+68);g=w(instances+4);binding=w(g+16)
print('instance',hex(g),[hex(w(g+i*4)) for i in range(6)])
for o in range(0,60,4):print(hex(o),hex(w(binding+o)),text(w(binding+o)))
