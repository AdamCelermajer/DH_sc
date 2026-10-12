from pathlib import Path
import struct,json,hashlib,subprocess
e=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');d=e.read_bytes();r=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/level-source-filename-route-v52');r.mkdir(exist_ok=True)
ph=struct.unpack_from('<I',d,28)[0];step,c=struct.unpack_from('<HH',d,42);segs=[struct.unpack_from('<8I',d,ph+i*step) for i in range(c)]
def read(va,n):
 for typ,p,a,_,z,_,_,_ in segs:
  if typ==1 and a<=va and va+n<=a+z:return d[p+va-a:p+va-a+n]
 raise ValueError(hex(va))
def w(va):return struct.unpack('<I',read(va,4))[0]
def txt(va):return read(va,512).split(b'\0')[0].decode(errors='replace')
vals={
 'initial_tag':0x3f3b70+8+w(0x3f410c),
 'local_prefix_constructor':0x3f3cac+8+w(0x3f4110),
 'array_outer':0x3f3d2c+8+w(0x3f4124)+0x90,
 'array_remove':0x3f3cec+8+w(0x3f411c)+0xa0,
 'compiled_final_prefix':0x3f3cd4+8+w(0x3f4120),
 'uncompiled_final_prefix':0x3f3cf0+8+w(0x3f4128),
 'empty_reset':0x3f3d1c+8+w(0x3f4118),
 'replace_input':0x3f3dc4+8+w(0x3f412c),
}
report={'original_sha256':hashlib.sha256(d).hexdigest(),'addresses':{k:hex(v) for k,v in vals.items()},'literals':{k:txt(v) for k,v in vals.items() if not k.startswith('array')},'arrays':{k:[{'pointer':hex(w(v+i*4)),'string':txt(w(v+i*4))} for i in range(4)] for k,v in vals.items() if k.startswith('array')}}
(r/'original-level-loadfile-route-literals.json').write_text(json.dumps(report,indent=2)+'\n')
exe=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-objdump.exe')
for name,a,z in [('level-loadfile',0x3f3b40,0x3f4154),('application-uncompiled',0x320678,0x3206d0)]:
 (r/(name+'.asm')).write_text(subprocess.check_output([str(exe),'-d','--demangle',f'--start-address={a}',f'--stop-address={z}',str(e)],text=True))
print(json.dumps(report,indent=2))
