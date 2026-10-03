import struct
from pathlib import Path
b=Path('.local-inputs/prince_menu_idle_knight.bdae').read_bytes();w=lambda p:struct.unpack_from('<I',b,p)[0]
r=w(32);seg=w(r+48);print('segments',w(seg),hex(w(seg+4)))
for i in range(w(seg)):
 p=w(seg+4)+i*24;print(i,[hex(w(p+k*4)) for k in range(6)])
