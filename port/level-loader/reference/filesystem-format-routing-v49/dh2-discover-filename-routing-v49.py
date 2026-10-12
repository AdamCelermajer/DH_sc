from pathlib import Path
import subprocess,json,struct,hashlib
elf=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so')
out=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/filesystem-format-routing-v49');out.mkdir(exist_ok=True)
tool=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin')
nm=subprocess.check_output([str(tool/'llvm-nm.exe'),'--demangle','--defined-only',str(elf)],text=True)
names=[x for x in nm.splitlines() if any(k in x for k in ['FileSystem','getFileBasename','TextureFormat','getTextureFile','getDriverType','getGPU','GPUType','getSupported','TextureManager::getHashName'])]
(out/'symbols.txt').write_text('\n'.join(names)+'\n')
b=elf.read_bytes();ph=struct.unpack_from('<I',b,28)[0];stride,count=struct.unpack_from('<HH',b,42);segs=[struct.unpack_from('<8I',b,ph+i*stride) for i in range(count)]
strings=[]
for needle in [b'pvr2_',b'pvr_',b'etc_',b'atc_',b'dxt_',b'.tga']:
 start=0
 while True:
  p=b.find(needle,start)
  if p<0:break
  start=p+1
  for typ,off,va,_,fs,_,_,_ in segs:
   if typ==1 and off<=p<off+fs:
    strings.append({'needle':needle.decode(),'va':hex(va+p-off),'offset':p,'text':b[p:b.index(b'\0',p)].decode(errors='replace')});break
(out/'literal-candidates.json').write_text(json.dumps({'elf_sha256':hashlib.sha256(b).hexdigest(),'strings':strings},indent=2)+'\n')
print('\n'.join(names));print(json.dumps(strings,indent=2))
