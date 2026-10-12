from pathlib import Path
import subprocess,struct,json,re
elf=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');d=elf.read_bytes()
out=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/filesystem-format-routing-v49')
tool=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin')
nm=subprocess.check_output([str(tool/'llvm-nm.exe'),'--demangle','--defined-only',str(elf)],text=True)
names=[x for x in nm.splitlines() if any(k.lower() in x.lower() for k in ['textureprefix','textureformat','textureextension','compressedtexture','gpu','texturetype','texturepath'])]
rels=subprocess.check_output([str(tool/'llvm-readobj.exe'),'--relocations',str(elf)],text=True)
rr=[x for x in rels.splitlines() if '995098' in x.upper()]
ph=struct.unpack_from('<I',d,28)[0];stride,count=struct.unpack_from('<HH',d,42);segs=[struct.unpack_from('<8I',d,ph+i*stride) for i in range(count)]
def off(va):
 for typ,p,a,_,n,_,_,_ in segs:
  if typ==1 and a<=va<a+n:return p+va-a
 raise ValueError(hex(va))
def text(va):
 p=off(va);return d[p:d.index(b'\0',p)].decode(errors='replace')
strings={hex(va):text(va) for va in [0x34e728+8+0x571fb0,0x34e8a8+8+0x571e38,0x34e8c0+8+0x571e28]}
got=struct.unpack_from('<I',d,off(0x995098))[0]
print(json.dumps({'got_value':hex(got),'relocations':rr,'literals':strings,'symbols':names},indent=2))
(out/'routing-globals.json').write_text(json.dumps({'got_value':hex(got),'relocations':rr,'literals':strings,'symbols':names},indent=2)+'\n')
