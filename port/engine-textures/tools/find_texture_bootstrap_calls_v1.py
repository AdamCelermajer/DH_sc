from pathlib import Path
import sys,struct,json,bisect
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
root=Path(__file__).resolve().parents[3];rows=[]
with (root/'.local-inputs/libDungeonHunter2.so').open('rb') as f:
 e=ELFFile(f);symbols=sorted((s['st_value'],s['st_size'],s.name) for s in e.get_section_by_name('.symtab').iter_symbols() if s['st_size'] and s['st_info']['type']=='STT_FUNC');starts=[x[0] for x in symbols]
 for p in e.iter_segments():
  if p['p_type']!='PT_LOAD' or not p['p_flags']&1:continue
  data=p.data();base=p['p_vaddr']
  for off in range(0,len(data)-3,4):
   w=struct.unpack_from('<I',data,off)[0]
   if w&0x0f000000!=0x0b000000:continue
   disp=(w&0xffffff);disp=disp-0x1000000 if disp&0x800000 else disp;a=base+off;target=(a+8+disp*4)&0xffffffff
   if target not in (0x5a90b0,0x6e0a3c):continue
   i=bisect.bisect_right(starts,a)-1;s=symbols[i];rows.append(dict(call=hex(a),target=hex(target),caller=hex(s[0]),size=s[1],symbol=s[2]))
(root/'port/engine-textures/reference/texture-owner-v1/bootstrap-callers.json').write_text(json.dumps(rows,indent=2)+'\n');print(json.dumps(rows,indent=2))
