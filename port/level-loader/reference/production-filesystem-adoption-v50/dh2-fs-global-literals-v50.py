from pathlib import Path
import subprocess,struct,json
e=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');d=e.read_bytes();r=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/production-filesystem-adoption-v50')
t=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin')
for name,a,b in [('fs-base-global-init',0x34e56c,0x34e5f4),('fs-get-working',0x56c214,0x56c230),('fs-base-c1',0x34e504,0x34e56c)]:
 (r/(name+'.asm')).write_text(subprocess.check_output([str(t/'llvm-objdump.exe'),'-d','--demangle',f'--start-address={a}',f'--stop-address={b}',str(e)],text=True))
ph=struct.unpack_from('<I',d,28)[0];step,c=struct.unpack_from('<HH',d,42);segs=[struct.unpack_from('<8I',d,ph+i*step) for i in range(c)]
def data(va,n):
 for typ,p,a,_,z,_,_,_ in segs:
  if typ==1 and a<=va and va+n<=a+z:return d[p+va-a:p+va-a+n]
 return bytes(n)
def text(va):return data(va,300).split(b'\0')[0].decode(errors='replace')
nm=subprocess.check_output([str(t/'llvm-nm.exe'),'--demangle','--defined-only',str(e)],text=True)
names={int(x.split(' ',2)[0],16):x.split(' ',2)[2] for x in nm.splitlines()}
got=0x34f074+8+0x645a1c;path_pointer=struct.unpack('<I',data(got+0x600,4))[0];path_value=struct.unpack('<I',data(path_pointer,4))[0]
report={'working_directory_symbol_data':data(0x99b138,32).hex(),'path_global_pointer':hex(path_pointer),'path_global_symbol':names.get(path_pointer),'path_global_value':hex(path_value),'path_global_initial_text':text(path_value),'trace_fmt_text':text(0x34f0b8+8+0x5716a0)}
(r/'fs-global-initial-data.json').write_text(json.dumps(report,indent=2)+'\n')
print((r/'fs-base-global-init.asm').read_text());print((r/'fs-get-working.asm').read_text());print(json.dumps(report,indent=2))
