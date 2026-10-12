from pathlib import Path
import subprocess,struct,json,hashlib
e=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');d=e.read_bytes();out=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/filesystem-format-routing-v49')
t=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin')
for name,a,b in [('lowercase',0x34e090,0x34e0fc),('factory-source',0x6594a0,0x659768),('file-base-path',0x34e5f4,0x34e688)]:
 (out/(name+'.asm')).write_text(subprocess.check_output([str(t/'llvm-objdump.exe'),'-d','--demangle',f'--start-address={a}',f'--stop-address={b}',str(e)],text=True))
nm=subprocess.check_output([str(t/'llvm-nm.exe'),'--demangle','--defined-only',str(e)],text=True);syms={int(x.split(' ',2)[0],16):x.split(' ',2)[2] for x in nm.splitlines()};ph=struct.unpack_from('<I',d,28)[0];step,c=struct.unpack_from('<HH',d,42);segs=[struct.unpack_from('<8I',d,ph+i*step) for i in range(c)]
def data(a,n):
 for typ,p,v,_,z,_,_,_ in segs:
  if typ==1 and v<=a<a+n<=v+z:return d[p+a-v:p+a-v+n]
 raise ValueError(hex(a))
vts={}
for va in [0x95ca80,0x95cb88]:
 words=struct.unpack('<50I',data(va,200));vts[hex(va)]={hex(i*4):{'va':hex(x),'symbol':syms.get(x)} for i,x in enumerate(words)}
(out/'vtable-root-and-identity.json').write_text(json.dumps({'local_original_sha256':hashlib.sha256(d).hexdigest(),'supplied_original_sha256':hashlib.sha256((out/'supplied-original-libDungeonHunter2.so').read_bytes()).hexdigest(),'same_supplied_original':d==(out/'supplied-original-libDungeonHunter2.so').read_bytes(),'vtables':vts,'literal_bytes_at_8c073c':data(0x8c073c,32).hex()},indent=2)+'\n')
print((out/'lowercase.asm').read_text());print((out/'vtable-root-and-identity.json').read_text()[:4500])
