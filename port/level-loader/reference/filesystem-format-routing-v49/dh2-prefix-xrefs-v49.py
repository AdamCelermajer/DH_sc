from pathlib import Path
import subprocess,struct,json,bisect,zipfile
elf=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');d=elf.read_bytes()
out=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/filesystem-format-routing-v49')
exe=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-nm.exe')
nm=subprocess.check_output([str(exe),'--demangle','--defined-only',str(elf)],text=True)
symbols=[]
for line in nm.splitlines():
 a,k,name=line.split(' ',2)
 if k.lower() in ['t','w']:symbols.append((int(a,16),name))
symbols.sort();addrs=[a for a,n in symbols]
ph=struct.unpack_from('<I',d,28)[0];stride,count=struct.unpack_from('<HH',d,42);segs=[struct.unpack_from('<8I',d,ph+i*stride) for i in range(count)]
def off(va):
 for typ,p,a,_,n,_,_,_ in segs:
  if typ==1 and a<=va<a+n:return p+va-a
 return None
def word(va):
 p=off(va)
 return struct.unpack_from('<I',d,p)[0] if p is not None and p+4<=len(d) else None
targets={}
for needle in [b'BTEXpvr',b'.pvr',b'pvrtc',b'pvr2',b'PVR2',b'atc',b'ATC']:
 p=0
 while True:
  p=d.find(needle,p)
  if p<0:break
  for typ,q,a,_,n,_,_,_ in segs:
   if typ==1 and q<=p<q+n:
    va=a+p-q
    if va>0x800000:
     targets[va]=needle.decode()
     if needle==b'BTEXpvr':targets[va+4]='pvr suffix of BTEXpvr'
    break
  p+=1
xrefs=[]
for typ,p,a,_,n,_,flags,_ in segs:
 if typ!=1 or not flags&1:continue
 for q in range(p,p+n-44,4):
  w=struct.unpack_from('<I',d,q)[0]
  if w&0x0f7f0000!=0x051f0000:continue
  va=a+q-p;reg=(w>>12)&15;loc=va+8+(1 if w&(1<<23) else -1)*(w&4095);v=word(loc)
  if v is None:continue
  for j in range(1,10):
   ins=struct.unpack_from('<I',d,q+4*j)[0]
   if ins&0x0fff0fff!=0x008f0000|reg:continue
   resolved=(va+4*j+8+v)&0xffffffff
   if resolved in targets:
    idx=bisect.bisect_right(addrs,va)-1;xrefs.append({'instruction':hex(va),'add_instruction':hex(va+4*j),'target':hex(resolved),'text':targets[resolved],'function':symbols[idx][1]})
report={'targets':{hex(k):v for k,v in targets.items()},'arm_pc_relative_literal_xrefs':xrefs}
with zipfile.ZipFile('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2.apk') as z:report['apk_assets']=[x for x in z.namelist() if x.startswith('assets/')]
with zipfile.ZipFile('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip') as z:
 report['cache_policy_names']=[x for x in z.namelist() if any(k in x.lower() for k in ['obfus','filemap','hash','mapping','texmap','texture.config'])]
 report['five_alpha_headers']={name:z.read(name)[:100].hex() for name in z.namelist() if any(name.lower().endswith('/pvr2_'+s+'.tga') for s in ['env_crypt_alpha','env_darktemple_alpha','env_earth_temple_alpha','env_castle_alpha','env_windtemple_alpha'])}
(out/'prefix-xrefs-policy-assets.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'xrefs':xrefs,'apk_assets':report['apk_assets'],'cache_policy_names':report['cache_policy_names'],'alpha_headers':report['five_alpha_headers']},indent=2)[:14000])
