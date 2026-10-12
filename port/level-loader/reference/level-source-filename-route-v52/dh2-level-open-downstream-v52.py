from pathlib import Path
import subprocess,json
e=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');r=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/level-source-filename-route-v52');t=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin')
for name,a,z in [('make-full-path',0x34f34c,0x34f35c),('create-handle',0x34fe50,0x34fee4),('file-handle-C1',0x34fc18,0x34fe50),('xp-file-open',0x82b214,0x82b340)]:
 (r/(name+'.asm')).write_text(subprocess.check_output([str(t/'llvm-objdump.exe'),'-d','--demangle',f'--start-address={a}',f'--stop-address={z}',str(e)],text=True))
print((r/'make-full-path.asm').read_text());print((r/'file-handle-C1.asm').read_text())
