from pathlib import Path
import subprocess,struct,json
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/level-lightset-v53';r.mkdir(exist_ok=True)
e=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');tools=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin')
symbols=subprocess.check_output([str(tools/'llvm-nm.exe'),'-C','-S',str(e)],text=True)
rows=[x for x in symbols.splitlines() if 'LightSet' in x or 'LoadFromXML' in x and 'Level::' in x];(r/'symbols.txt').write_text('\n'.join(rows)+'\n');print('\n'.join(rows))
blob=e.read_bytes();ph=struct.unpack_from('<I',blob,28)[0];step,c=struct.unpack_from('<HH',blob,42);segs=[struct.unpack_from('<8I',blob,ph+i*step) for i in range(c)]
def read(a,n):
 for t,p,v,_,z,_,_,_ in segs:
  if t==1 and v<=a and a+n<=v+z:return blob[p+a-v:p+a-v+n]
 raise ValueError(hex(a))
def w(a):return struct.unpack('<I',read(a,4))[0]
def txt(a):return read(a,250).split(b'\0')[0].decode(errors='replace')
tags=[{'at':hex(pc+8+w(lit)),'text':txt(pc+8+w(lit))} for pc,lit in [(0x3f4600,0x3f48d8),(0x3f4788,0x3f48f4)]];(r/'outer-tags.json').write_text(json.dumps(tags,indent=2)+'\n');print(tags)
for row in rows:
 bits=row.split();a=int(bits[0],16);n=int(bits[1],16)
 if n and ('Load' in row or 'Init' in row or '_LoadLightSet' in row):
  out=subprocess.check_output([str(tools/'llvm-objdump.exe'),'-d','--demangle','--start-address='+hex(a),'--stop-address='+hex(a+n),str(e)],text=True);(r/('source-'+bits[0]+'.asm')).write_text(out)
