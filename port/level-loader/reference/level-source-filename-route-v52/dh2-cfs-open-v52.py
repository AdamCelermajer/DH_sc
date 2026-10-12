from pathlib import Path
import subprocess
e=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');r=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/level-source-filename-route-v52');t=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-objdump.exe')
text=subprocess.check_output([str(t),'-d','--demangle','--start-address=0x56dc40','--stop-address=0x56df38',str(e)],text=True)
(r/'cfs-physical-open.asm').write_text(text);print(text)
