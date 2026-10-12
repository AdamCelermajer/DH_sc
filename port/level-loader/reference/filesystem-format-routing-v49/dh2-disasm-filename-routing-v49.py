from pathlib import Path
import subprocess,json,struct
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/filesystem-format-routing-v49')
elf=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so')
tool=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-objdump.exe')
for name,a,z in [('create-open',0x34e688,0x34e96c),('basename',0x56c740,0x56ca70),('open-resource',0x34ff28,0x34ff84),('paths',0x34f068,0x34f10c),('format-path',0x34f8a4,0x34fbbc),('cfs-open',0x56d4e0,0x56dc40)]:
 (b/(name+'.asm')).write_text(subprocess.check_output([str(tool),'-d','--demangle',f'--start-address={a}',f'--stop-address={z}',str(elf)],text=True))
print((b/'create-open.asm').read_text())
