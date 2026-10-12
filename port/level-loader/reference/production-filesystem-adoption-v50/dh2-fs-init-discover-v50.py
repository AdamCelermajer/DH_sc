from pathlib import Path
import subprocess,struct,json,bisect
e=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');d=e.read_bytes();r=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/production-filesystem-adoption-v50');r.mkdir(exist_ok=True)
t=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin')
nm=subprocess.check_output([str(t/'llvm-nm.exe'),'--demangle','--defined-only',str(e)],text=True);syms=[]
for line in nm.splitlines():
 a,k,n=line.split(' ',2)
 if k.lower() in ['t','w']:syms.append((int(a,16),n))
syms.sort();aa=[a for a,n in syms]
chosen=[(a,n) for a,n in syms if any(n.startswith(k) for k in ['glitch::io::CFileSystem::CFileSystem()','glitch::io::CFileSystem::changeWorkingDirectoryTo','glitch::io::CFileSystem::getWorkingDirectory','FileSystemWin32::FileSystemWin32()','FileSystemBase::changeWorkingDirectoryTo','IrrFactory::createFileSystem()','_GLOBAL__I_.._source_glitch_io_CFileSystem.cpp'])]
for a,n in chosen:
 nxt=aa[bisect.bisect_right(aa,a)]
 (r/f'fs-{a:x}.asm').write_text(subprocess.check_output([str(t/'llvm-objdump.exe'),'-d','--demangle',f'--start-address={a}',f'--stop-address={nxt}',str(e)],text=True))
ph=struct.unpack_from('<I',d,28)[0];step,c=struct.unpack_from('<HH',d,42);segs=[struct.unpack_from('<8I',d,ph+i*step) for i in range(c)]
targets={0x56c058,0x56c0a8,0x56c528,0x34e1d4,0x350aec,0x34f6f4,0x34f788};hits=[]
for typ,p,a,_,n,_,flags,_ in segs:
 if typ!=1 or not flags&1:continue
 for q in range(p,p+n-4,4):
  w=struct.unpack_from('<I',d,q)[0]
  if w&0x0e000000!=0x0a000000:continue
  delta=w&0xffffff
  if delta&0x800000:delta-=0x1000000
  va=a+q-p;dest=(va+8+delta*4)&0xffffffff
  if dest in targets:
   idx=bisect.bisect_right(aa,va)-1;hits.append({'va':hex(va),'target':hex(dest),'caller_va':hex(syms[idx][0]),'caller':syms[idx][1]})
(r/'original-fs-init-callers.json').write_text(json.dumps({'functions':{hex(a):n for a,n in chosen},'calls':hits},indent=2)+'\n')
print((r/'original-fs-init-callers.json').read_text())
for name in ['fs-56c058.asm','fs-56c5cc.asm','fs-56c528.asm','fs-34e1d4.asm','fs-350aec.asm','fs-34f6f4.asm']:
 print((r/name).read_text())
