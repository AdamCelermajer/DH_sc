from pathlib import Path
import subprocess,struct,json,bisect
e=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');d=e.read_bytes();r=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/production-filesystem-adoption-v50')
t=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin');nm=subprocess.check_output([str(t/'llvm-nm.exe'),'--demangle','--defined-only',str(e)],text=True);syms=[]
for line in nm.splitlines():
 a,k,n=line.split(' ',2)
 if k.lower() in ['t','w']:syms.append((int(a,16),n))
syms.sort();aa=[a for a,n in syms];ph=struct.unpack_from('<I',d,28)[0];step,c=struct.unpack_from('<HH',d,42);segs=[struct.unpack_from('<8I',d,ph+i*step) for i in range(c)]
def word(va):
 for typ,p,a,_,n,_,_,_ in segs:
  if typ==1 and a<=va<va+4<=a+n:return struct.unpack_from('<I',d,p+va-a)[0]
 return None
hits=[]
for i,(a,name) in enumerate(syms[:-1]):
 end=syms[i+1][0]
 if end-a>100_000:continue
 regs={}
 for va in range(a,end,4):
  w=word(va)
  if w is None:continue
  rd=(w>>12)&15;rn=(w>>16)&15;rm=w&15
  if w&0x0f7f0000==0x051f0000:
   value=word(va+8+(1 if w&(1<<23) else -1)*(w&4095))
   regs[rd]=value
  elif w&0x0fe00ff0==0x00800000 and rn==15:
   if regs.get(rm) is not None:regs[rd]=(va+8+regs[rm])&0xffffffff
  elif w&0x0e500ff0==0x06100000 and regs.get(rn) is not None and regs.get(rm) is not None:
   addr=(regs[rn]+regs[rm])&0xffffffff;value=word(addr);regs[rd]=value
   if value in [0x99b108,0x99b138]:hits.append({'instruction':hex(va),'global':hex(value),'function':name,'function_va':hex(a)})
  elif w&0x0e000000==0x0a000000:regs={}
for name,a,z in [('android-native-init',0x5325c4,0x532a8c),('android-res-loader-init',0x5324c0,0x5325c4)]:
 (r/(name+'.asm')).write_text(subprocess.check_output([str(t/'llvm-objdump.exe'),'-d','--demangle',f'--start-address={a}',f'--stop-address={z}',str(e)],text=True))
(r/'original-path-global-xrefs.json').write_text(json.dumps({'bounded_ARM_PC_relative_GOT_scan':hits},indent=2)+'\n')
print(json.dumps(hits,indent=2));print((r/'android-native-init.asm').read_text()[:18000])
