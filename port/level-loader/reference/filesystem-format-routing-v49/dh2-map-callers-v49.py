from pathlib import Path
import struct,subprocess,bisect,json,zipfile
e=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');d=e.read_bytes();out=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/filesystem-format-routing-v49')
exe=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin')
nm=subprocess.check_output([str(exe/'llvm-nm.exe'),'--demangle','--defined-only',str(e)],text=True);syms=[]
for x in nm.splitlines():
 a,k,n=x.split(' ',2)
 if k.lower() in ['t','w']:syms.append((int(a,16),n))
syms.sort();aa=[a for a,n in syms];ph=struct.unpack_from('<I',d,28)[0];step,c=struct.unpack_from('<HH',d,42);segs=[struct.unpack_from('<8I',d,ph+i*step) for i in range(c)];hits=[]
for typ,p,a,_,n,_,flags,_ in segs:
 if typ!=1 or not flags&1:continue
 for q in range(p,p+n-4,4):
  w=struct.unpack_from('<I',d,q)[0]
  if w&0x0e000000!=0x0a000000:continue
  delta=w&0xffffff
  if delta&0x800000:delta-=0x1000000
  va=a+q-p;target=(va+8+delta*4)&0xffffffff
  if target in [0x56e6dc,0x34e96c,0x56e064]:
   idx=bisect.bisect_right(aa,va)-1;hits.append({'va':hex(va),'target':hex(target),'function_va':hex(syms[idx][0]),'function':syms[idx][1]})
data_save={}
with zipfile.ZipFile('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2.apk') as z:
 raw=z.read('assets/data.save');data_save={'length':len(raw),'prefix':raw[:100].hex(),'contains_pvr2':b'pvr2' in raw.lower()}
(out/'map-callers.json').write_text(json.dumps({'calls':hits,'apk_data_save':data_save},indent=2)+'\n')
print(json.dumps({'calls':hits,'apk_data_save':data_save},indent=2))
for h in hits:
 if h['target']=='0x56e6dc':
  a=int(h['function_va'],16);p=bisect.bisect_right(aa,a);b=aa[p] if p<len(aa) else a+4096
  (out/f'obfuscation-caller-{a:x}.asm').write_text(subprocess.check_output([str(exe/'llvm-objdump.exe'),'-d','--demangle',f'--start-address={a}',f'--stop-address={b}',str(e)],text=True))
