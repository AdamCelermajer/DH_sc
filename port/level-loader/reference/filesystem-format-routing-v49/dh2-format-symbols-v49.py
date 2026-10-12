from pathlib import Path
import subprocess,re,json
elf=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so')
out=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/filesystem-format-routing-v49')
exe=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-nm.exe')
nm=subprocess.check_output([str(exe),'--demangle','--defined-only',str(elf)],text=True)
names=[x for x in nm.splitlines() if any(k.lower() in x.lower() for k in ['getObfuscatedFile','compression','pvr','pvrtc','s3tc','textureloader','imageLoader','configuration','gettexturefilename','texture_','textureprefix'])]
strings=[{'offset':m.start(),'text':m.group().decode()} for m in re.finditer(rb'[\x20-\x7e]{4,}',elf.read_bytes()) if any(k in m.group().lower() for k in [b'pvr',b'etc_',b'atc_',b'dxt_',b'textureformat',b'textureprefix',b'texturecompression'])]
(out/'format-symbols.json').write_text(json.dumps({'names':names,'strings':strings},indent=2)+'\n')
print('\n'.join(names));print(json.dumps(strings,indent=2))
