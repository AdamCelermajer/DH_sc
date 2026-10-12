from pathlib import Path
import struct,json
e=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');b=e.read_bytes();r=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/level-source-filename-route-v52');ph=struct.unpack_from('<I',b,28)[0];step,c=struct.unpack_from('<HH',b,42);segs=[struct.unpack_from('<8I',b,ph+i*step) for i in range(c)]
def read(a,n):
 for t,p,v,_,z,_,_,_ in segs:
  if t==1 and v<=a and a+n<=v+z:return b[p+a-v:p+a-v+n]
 raise ValueError(hex(a))
def w(a):return struct.unpack('<I',read(a,4))[0]
def txt(a):return read(a,300).split(b'\0')[0].decode(errors='replace')
marker=0x34fc80+8+w(0x34fe34);fmt=0x34fca0+8+w(0x34fe3c);modes=0x320698+8+w(0x3206cc)
report={'file_handle_marker':{'address':hex(marker),'literal':txt(marker)},'file_handle_RES_PATH_format':{'address':hex(fmt),'literal':txt(fmt)},'uncompiled_mode_tokens':[{'address':hex(w(modes+i*4)),'literal':txt(w(modes+i*4))} for i in range(4)]}
(r/'downstream-source-literals-v52.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
